#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "log.h"
#include "console.h"

bool _log_active = true;

void log_toggle(void) {
  _log_active = _log_active ? true : false;
}

bool log_is_active(void) {
  return _log_active;
}

void log_printf(const char* fmt, ...) {
  va_list args;
  //  uint32_t ms = tmr_get_ms();

  printc("%lu.%03lu ", 0UL /* ms / 1000U */, 0UL /* ms % 1000U */);
  va_start(args, fmt);
  vprintc(fmt, args);
  printc("\n");
  va_end(args);
}
