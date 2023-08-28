#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "module.h"
#include "console.h"
#include "cmd.h"
#include "command_parser.h"

#define MAX_CMD_TOKENS 10

static int32_t log_level = LOG_LEVEL_DEFAULT;
static const struct cmd_client_info* client_info[CMD_MAX_CLIENTS];

//static const char* log_level_str(int32_t level);
//static int32_t log_level_int(const char* level_name);

int32_t cmd_init(void) {
  memset(client_info, 0, sizeof(client_info));
  return 0;
}

int32_t cmd_register(const struct cmd_client_info* _client_info) {
  int32_t idx;

  for (idx = 0; idx < CMD_MAX_CLIENTS; idx++) {
    if (client_info[idx] == NULL) {
      client_info[idx] = _client_info;
      return 0;
    }
  }

  return MOD_ERR_RESOURCE;
}

int32_t cmd_execute(const char* buffer) {
  char error_message[CONSOLE_PRINT_BUFFER_SIZE] = {0};
  char* tokens[MAX_CMD_TOKENS] = {0};
  size_t padding;
  int result;
  int i;

  result = tokenize(buffer, tokens, MAX_CMD_TOKENS);

  if (result < 0) {
    result = tokenize_error_message(buffer, MAX_CMD_TOKENS, result, error_message);

    if (result < 0) {
      log_error("failed to build error message.");
    }

    padding = strlen(CONSOLE_PROMPT);

    for (i = 0; i < padding; i++) printc(" ");
    printc(error_message);
    printc("\n");

    return -1;
  }

  return 0;
}
