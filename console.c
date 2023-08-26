#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>

#include "module.h"
#include "usart.h"
#include "console.h"
#include "log.h"

#define PROMPT "> "

#define CONSOLE_PRINT_BUFFER_SIZE 240
#define CONSOLE_READ_BUFFER_SIZE   80

struct console_state {
    struct console_cfg cfg;
    char read_buffer[CONSOLE_READ_BUFFER_SIZE];
    uint16_t num_read_buffer_chars;
    bool first_run_done;
};

static struct console_state state = {
  .cfg.usart_id = USART_2,
};

int32_t console_set_default_config(struct console_cfg* cfg) {
  if (cfg == NULL)
    return MOD_ERR_ARGS;

  memset(cfg, 0, sizeof(*cfg));

  cfg->usart_id = USART_2;

  return 0;
}

int32_t console_init(struct console_cfg* cfg) {
  if (cfg == NULL)
    return MOD_ERR_ARGS;

  memset(&state, 0, sizeof(state));

  state.cfg = *cfg;

  return 0;
}

int32_t console_run(void) {
  char c;

  if (!state.first_run_done) {
    state.first_run_done = true;
    printc("%s", PROMPT);
  }

  while (usart_getc(state.cfg.usart_id, &c)) {
    if (c == '\n' || c == '\r') {
      state.read_buffer[state.num_read_buffer_chars] = '\0';
      printc("\n");
      //TODO execute command
      state.num_read_buffer_chars = 0;
      printc("%s", PROMPT);

      continue;
    }

    if (c == '\b' || c == '\x7f') {
      if (state.num_read_buffer_chars > 0) {
        printc("\b \b");
        state.num_read_buffer_chars--;
      }

      continue;
    }

    if (c == LOG_TOGGLE_CHAR) {
      log_toggle();
      printc("\n<Logging %s>\n", log_is_active() ? "on" : "off");
      continue;
    }

    if (isprint(c)) {
      if (state.num_read_buffer_chars < (CONSOLE_READ_BUFFER_SIZE-1)) {
        state.read_buffer[state.num_read_buffer_chars++] = c;
        printc("%c", c);
      } else {
        printc("\a"); // bell
      }
      continue;
    }
  }

  return 0;
}

int	printc(const char* fmt, ...) {
  va_list args;
  char buf[CONSOLE_PRINT_BUFFER_SIZE];
  int rc;
  int idx;

  va_start(args, fmt);
  rc = vsnprintf(buf, CONSOLE_PRINT_BUFFER_SIZE, fmt, args);
  va_end(args);

  for (idx = 0; idx < rc; idx++) {
    usart_putc(state.cfg.usart_id, buf[idx]);
    if (buf[idx] == '\0')
      break;

    if (buf[idx] == '\n')
      usart_putc(state.cfg.usart_id, '\r');
  }

  if (rc >= CONSOLE_PRINT_BUFFER_SIZE)
    printc("[!]\n");

  return rc;
}

int vprintc(const char* fmt, va_list args) {
  char buf[CONSOLE_PRINT_BUFFER_SIZE];
  int rc;
  int idx;

  rc = vsnprintf(buf, CONSOLE_PRINT_BUFFER_SIZE, fmt, args);

  for (idx = 0; idx < rc; idx++) {
    usart_putc(state.cfg.usart_id, buf[idx]);

    if (buf[idx] == '\0')
      break;

    if (buf[idx] == '\n')
      usart_putc(state.cfg.usart_id, '\r');
  }

  if (rc >= CONSOLE_PRINT_BUFFER_SIZE)
    printc("[!]\n");

  return rc;
}
