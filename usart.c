#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "stm32f4xx_ll_usart.h"

#include "module.h"
#include "usart.h"

#define USART_1_FD 4
#define USART_2_FD 1
#define USART_6_FD 3

struct usart_state {
  struct usart_cfg cfg;
  FILE *stream;
  int fd;
  USART_TypeDef *uart_reg_base;
  uint16_t rx_buf_get_idx;
  uint16_t rx_buf_put_idx;
  char rx_buf[USART_RX_BUF_SIZE];
  uint16_t tx_buf_get_idx;
  uint16_t tx_buf_put_idx;
  char tx_buf[USART_TX_BUF_SIZE];
};

static struct usart_state usart_states[USART_NUM_INSTANCES];

static void usart_interrupt(enum usart_id id, IRQn_Type irq_type);

int32_t usart_set_default_config(struct usart_cfg* cfg) {
  if (cfg == NULL)
    return MOD_ERR_ARGS;

  memset(cfg, 0, sizeof(*cfg));

  cfg->create_stream = true;
  cfg->send_cr_after_nl = true;

  return 0;
}

int32_t usart_init(enum usart_id id, const struct usart_cfg* cfg) {
  struct usart_state *st;

  if (id >= USART_NUM_INSTANCES || cfg == NULL)
    return MOD_ERR_ARGS;

  st = &usart_states[id];

  st->rx_buf_get_idx = 0;
  st->rx_buf_put_idx = 0;
  st->tx_buf_get_idx = 0;
  st->tx_buf_put_idx = 0;

  st->cfg = *cfg;

  switch (id) {
  case USART_1:
    st->uart_reg_base = USART1;
    st->fd = USART_1_FD;
    break;
  case USART_2:
    st->uart_reg_base = USART2;
    st->fd = USART_2_FD;
    break;
  case USART_6:
    st->uart_reg_base = USART6;
    st->fd = USART_6_FD;
    break;
  default:
    return MOD_ERR_BAD_INSTANCE;
  }

  st->stream = NULL;

  if (cfg->create_stream) {
    st->stream = fdopen(st->fd, "r+");

    if (st->stream != NULL)
      setvbuf(st->stream, NULL, _IONBF, 0);
  }

  return SUCCESS;
}

int32_t usart_start(enum usart_id id) {
  struct usart_state* st;
  IRQn_Type irq_type;

  if (id > USART_NUM_INSTANCES)
    return MOD_ERR_ARGS;

  st = &usart_states[id];
  LL_USART_EnableIT_RXNE(st->uart_reg_base);
  LL_USART_EnableIT_TXE(st->uart_reg_base);

  switch (id) {
  case USART_1:
    irq_type = USART1_IRQn;
    break;
  case USART_2:
    irq_type = USART2_IRQn;
    break;
  case USART_6:
    irq_type = USART6_IRQn;
    break;
  default:
    return MOD_ERR_BAD_INSTANCE;
  }

  NVIC_SetPriority(irq_type,
                   NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,0));
  NVIC_EnableIRQ(irq_type);

  return SUCCESS;
}

int32_t usart_putc(enum usart_id id, char c) {
  struct usart_state* st;
  uint16_t next_put_idx;
  uint32_t primask;

  if (id > USART_NUM_INSTANCES)
    return MOD_ERR_BAD_INSTANCE;

  st = &usart_states[id];

  primask = disable_irq();

  next_put_idx = st->tx_buf_put_idx + 1;
  if (next_put_idx >= USART_TX_BUF_SIZE)
    next_put_idx = 0;

  if (next_put_idx == st->tx_buf_get_idx) {
    restore_irq(primask);
    return MOD_ERR_BUF_OVERRUN;
  }

  st->tx_buf[st->tx_buf_put_idx] = c;
  st->tx_buf_put_idx = next_put_idx;

  LL_USART_EnableIT_TXE(st->uart_reg_base);

  restore_irq(primask);

  return SUCCESS;
}

int32_t usart_getc(enum usart_id id, char* c) {
  struct usart_state* st;
  int32_t next_get_idx;
  uint32_t primask;

  if (id >= USART_NUM_INSTANCES)
    return MOD_ERR_BAD_INSTANCE;

  st = &usart_states[id];

  primask = disable_irq();

  if (st->rx_buf_get_idx == st->rx_buf_put_idx) {
    restore_irq(primask);
    return 0;
  }

  next_get_idx = st->rx_buf_get_idx + 1;

  if (next_get_idx >= USART_RX_BUF_SIZE)
    next_get_idx = 0;

  *c = st->rx_buf[st->rx_buf_get_idx];

  st->rx_buf_get_idx = next_get_idx;

  restore_irq(primask);

  return 1;
}

int usart_get_fd(enum usart_id id) {
  return 0;
}

FILE* usart_get_stream(enum usart_id id) {
  return 0;
}

void USART1_IRQHandler(void)
{
    usart_interrupt(USART_1, USART1_IRQn);
}

void USART2_IRQHandler(void)
{
    usart_interrupt(USART_2, USART2_IRQn);
}

void USART6_IRQHandler(void)
{
    usart_interrupt(USART_6, USART6_IRQn);
}

static void usart_interrupt(enum usart_id id, IRQn_Type irq_type) {
    struct usart_state* st;
    uint8_t sr;
    uint32_t primask;
    uint16_t next_rx_put_idx;

    if (id >= USART_NUM_INSTANCES) //TODO: count error
      return;

    st = &usart_states[id];

    if (st == NULL) //TODO: count error
      return;

    if (st->uart_reg_base == NULL) {
      //TODO: count error
      NVIC_DisableIRQ(irq_type);
      return;
    }

    sr = st->uart_reg_base->SR;

    primask = disable_irq();
    if (sr & LL_USART_SR_RXNE) {
        next_rx_put_idx = st->rx_buf_put_idx + 1;

        if (next_rx_put_idx >= USART_RX_BUF_SIZE)
            next_rx_put_idx = 0;

        if (next_rx_put_idx == st->rx_buf_get_idx) {
          //            INC_SAT_U16(cnts_u16[CNT_RX_BUF_OVERRUN]); ?
        } else {
            st->rx_buf[st->rx_buf_put_idx] = st->uart_reg_base->DR;
            st->rx_buf_put_idx = next_rx_put_idx;
        }
    }
    if (sr & LL_USART_SR_TXE) {
        // Can send a character.
        if (st->tx_buf_get_idx == st->tx_buf_put_idx) {
            // No characters to send, disable the interrrupt.
            LL_USART_DisableIT_TXE(st->uart_reg_base);
        } else {
            st->uart_reg_base->DR = st->tx_buf[st->tx_buf_get_idx];

            if (st->tx_buf_get_idx < USART_TX_BUF_SIZE-1)
                st->tx_buf_get_idx++;
            else
                st->tx_buf_get_idx = 0;
        }
    }

    if (sr & (LL_USART_SR_ORE | LL_USART_SR_NE |
              LL_USART_SR_FE  | LL_USART_SR_PE)) {

        // Error conditions. To clear the bit, we need to read the
        // data register, but we don't use it.
        (void)st->uart_reg_base->DR;

        /* if (sr & LL_USART_SR_ORE) */
        /*     INC_SAT_U16(cnts_u16[CNT_RX_UART_ORE]); */
        /* if (sr & LL_USART_SR_NE) */
        /*     INC_SAT_U16(cnts_u16[CNT_RX_UART_NE]); */
        /* if (sr & LL_USART_SR_FE) */
        /*     INC_SAT_U16(cnts_u16[CNT_RX_UART_FE]); */
        /* if (sr & LL_USART_SR_PE) */
        /*     INC_SAT_U16(cnts_u16[CNT_RX_UART_PE]); */
    }

    restore_irq(primask);
}
