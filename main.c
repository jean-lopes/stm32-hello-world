#include <stdbool.h>

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_bus.h"

#include "module.h"
#include "gpio.h"
#include "usart.h"
#include "console.h"

int main(void) {
  setvbuf(stdout, NULL, _IONBF, 0);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA | LL_AHB1_GRP1_PERIPH_GPIOC);

  const struct gpio_in button = {
    .name = "button",
    .port = GPIO_PORT_A,
    .pin = GPIO_PIN_0,
    .pull = GPIO_PULL_UP,
    .reversed = true,
  };

  const struct gpio_out led = {
    .name = "led",
    .port = GPIO_PORT_C,
    .pin = GPIO_PIN_13,
    .pull = GPIO_PULL_NO,
    .output_type = GPIO_OUTPUT_PUSHPULL,
    .init_value = false,
    .speed = GPIO_SPEED_FREQ_LOW,
    .reversed = true,
  };

  const struct usart_cfg usart2_cfg = {
    .create_stream = true,
    .send_cr_after_nl = true,
  };

  const struct gpio_in inputs[] = {button};
  const uint8_t num_inputs = ARRAY_SIZE(inputs);

  const struct gpio_out outputs[] = {led};
  const uint8_t num_outputs = ARRAY_SIZE(outputs);

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7);

  /* LL_USART_InitTypeDef USART_InitStruct; */
  /* USART_InitStruct.BaudRate = 115200; */
  /* USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B; */
  /* USART_InitStruct.StopBits = LL_USART_STOPBITS_1; */
  /* USART_InitStruct.Parity = LL_USART_PARITY_NONE; */
  /* USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX; */
  /* USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE; */
  /* USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16; */
  /* LL_USART_Init(USART2, &USART_InitStruct); */
  /* LL_USART_ConfigAsyncMode(USART2); */
  /* LL_USART_Enable(USART2); */

  uint32_t periphclk;
  LL_RCC_ClocksTypeDef rcc_clocks;
  LL_RCC_GetSystemClocksFreq(&rcc_clocks);
  periphclk = rcc_clocks.PCLK1_Frequency; //for usart2 only
  LL_USART_SetBaudRate(USART2, periphclk, LL_USART_OVERSAMPLING_16, 115200);
  LL_USART_SetDataWidth(USART2, LL_USART_DATAWIDTH_8B);
  LL_USART_SetStopBitsLength(USART2, LL_USART_STOPBITS_1);
  LL_USART_SetParity(USART2, LL_USART_PARITY_NONE);
  LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);
  LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);
  LL_USART_SetOverSampling(USART2, LL_USART_OVERSAMPLING_16);
  LL_USART_Enable(USART2);

  gpio_init(inputs, num_inputs, outputs, num_outputs);

  //  gpio_start(inputs, num_inputs, outputs, num_outputs);

  struct console_cfg console_cfg;
  console_set_default_config(&console_cfg);

  usart_init(USART_2, &usart2_cfg);
  console_init(&console_cfg);

  usart_start(USART_2);

  while (1) {
    console_run();

    int32_t value = gpio_get(&button);

    if (value == 1) {
      usart_putc(USART_2, 'o');
      gpio_set(&led, true);
    } else {
      gpio_set(&led, false);
    }
  }

  return 0;
}
