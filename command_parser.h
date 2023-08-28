#ifndef _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_

enum tokenize_error {
  EXPECTED_IDENTIFIER                   = -1,
  EXPECTED_SPACE                        = -2,
  EXPECTED_IDENTIFIER_VALUE_OR_ASTERISK = -3,
  EXCEEDED_TOKEN_LIMIT                  = -4,
};

int tokenize_error_message(const char* command,
                           const int max_num_tokens,
                           int tokenize_exit_code,
                           char* message);

int tokenize(const char* s, char** tokens, int num_tokens);

#endif
