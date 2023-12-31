#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "usart.h"

#define CONSOLE_PROMPT            "> "
#define CONSOLE_PRINT_BUFFER_SIZE 240
#define CONSOLE_READ_BUFFER_SIZE   80

struct console_cfg {
  enum usart_id usart_id;
};

int32_t console_set_default_config(struct console_cfg* cfg);
int32_t console_init(struct console_cfg* cfg);
int32_t console_run(void);

int	printc(const char* fmt, ...)
  __attribute__((__format__ (__printf__, 1, 2)));

int	vprintc(const char* fmt, va_list args);

#endif
