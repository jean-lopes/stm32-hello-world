#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdbool.h>

#define LOG_TOGGLE_CHAR '\x0c' // CTRL+L

enum log_level {
    LOG_LEVEL_OFF = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,

    LOG_LEVEL_DEFAULT = LOG_LEVEL_INFO
};

extern bool _log_active;

void log_toggle(void);
bool log_is_active(void);
void log_printf(const char* fmt, ...);

#define log_error(fmt, ...) do { if (_log_active && log_level >= LOG_LEVEL_ERROR) \
      log_printf("[%s][%s:%s:%d]: " fmt, "ERR", __FILE__, __FUNCTION__,  __LINE__, ##__VA_ARGS__); } while(0)

#define log_warning(fmt, ...) do { if (_log_active && log_level >= LOG_LEVEL_WARNING) \
      log_printf("[%s][%s:%s:%d]: " fmt, "WRN", __FILE__, __FUNCTION__,  __LINE__, ##__VA_ARGS__); } while(0)

#define log_info(fmt, ...) do { if (_log_active && log_level >= LOG_LEVEL_INFO) \
      log_printf("[%s][%s:%s:%d]: " fmt, "INF", __FILE__, __FUNCTION__,  __LINE__, ##__VA_ARGS__); } while(0)

#define log_debug(fmt, ...) do { if (_log_active && log_level >= LOG_LEVEL_DEBUG) \
      log_printf("[%s][%s:%s:%d]: " fmt, "DBG", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); } while(0)

#define log_trace(fmt, ...) do { if (_log_active && log_level >= LOG_LEVEL_TRACE) \
      log_printf("[%s][%s:%s:%d]: " fmt, "TRC", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); } while(0)

#endif
