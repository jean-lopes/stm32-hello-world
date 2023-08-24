#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

enum usart_id {
    USART_1,
    USART_2, // File discriptor 1 (stdin/stdout).
    USART_6,

    USART_NUM_INSTANCES
};

#define USART_RX_BUF_SIZE 80
#define USART_TX_BUF_SIZE 1024

struct usart_cfg {
    bool create_stream;
    bool send_cr_after_nl;
};

int32_t usart_set_default_config(struct usart_cfg* cfg);
int32_t usart_init(enum usart_id id, const struct usart_cfg* cfg);
int32_t usart_start(enum usart_id id);

int32_t usart_putc(enum usart_id id, char c);
int32_t usart_getc(enum usart_id id, char* c);
int usart_get_fd(enum usart_id id);
FILE* usart_get_stream(enum usart_id id);

#endif
