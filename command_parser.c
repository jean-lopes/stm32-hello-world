#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "command_parser.h"

enum state {
  LEFT_TRIM,
  IDENTIFIER,
  SPACE,
  ASTERISK,
  VALUE
};

static const char* error_offset = NULL;

int tokenize_error_message(const char* command,
                           const int max_num_tokens,
                           int tokenize_exit_code,
                           char* message) {
  if (error_offset == NULL)
    return -1;

  size_t size = 1 + error_offset - command;
  char error_marker[size+1]; // +1 for \NUL
  memset(error_marker, ' ', sizeof(error_marker));
  error_marker[size-1] = '^';
  error_marker[size] = 0;

  switch (tokenize_exit_code) {
  case EXPECTED_IDENTIFIER:
    sprintf(message, "%s expected identifier.", error_marker);
    return 0;
  case EXPECTED_SPACE:
    sprintf(message, "%s expected whitespace.", error_marker);
    return 0;
  case EXPECTED_IDENTIFIER_VALUE_OR_ASTERISK:
    sprintf(message, "%s expected identifier, value or *.", error_marker);
    return 0;
  case EXCEEDED_TOKEN_LIMIT:
    sprintf(message, "%s token limit(%d) exceeded.", error_marker, max_num_tokens);
    return 0;
  default:
    message = "Unknow error code.";
    return -1;
  }
}

int tokenize(const char* s, char** tokens, int num_tokens) {
  int token_count = 0;
  const char* token_start;

  enum state state = LEFT_TRIM;
  unsigned char c = *s;

  error_offset = NULL;

  while (c != 0) {
    switch (state) {
    case LEFT_TRIM:
      while (isspace(c)) {s++; c=*s;}
      if (isalpha(c))
        state = IDENTIFIER;
      else {
        error_offset = s;
        return EXPECTED_IDENTIFIER;
      }
      break;

    case SPACE:
      if (!isspace(c) && c != 0) {
        error_offset = s;
        return EXPECTED_SPACE;
      }

      while (isspace(c)) {s++; c=*s;}

      if (isalpha(c))
        state = IDENTIFIER;
      else if (isdigit(c))
        state = VALUE;
      else if (c == '*')
        state = ASTERISK;
      else if (c == 0)
        return token_count;
      else {
        error_offset = s;
        return EXPECTED_IDENTIFIER_VALUE_OR_ASTERISK;
      }
      break;

    case IDENTIFIER:
      if (token_count >= num_tokens) {
        error_offset = s;
        return EXCEEDED_TOKEN_LIMIT;
      }
      token_start = s;
      while (isalnum(c) || c == '_' || c == '-') {s++; c=*s;}
      tokens[token_count] = strndup(token_start, s - token_start);
      token_count += 1;
      state = SPACE;
      break;

    case ASTERISK:
      if (token_count >= num_tokens) {
        error_offset = s;
        return EXCEEDED_TOKEN_LIMIT;
      }
      tokens[token_count] = "*";
      token_count += 1;
      s++;
      c = *s;
      state = SPACE;
      break;

    case VALUE:
      if (token_count >= num_tokens) {
        error_offset = s;
        return EXCEEDED_TOKEN_LIMIT;
      }
      token_start = s;
      while (isdigit(c)) {s++; c=*s;}
      tokens[token_count] = strndup(token_start, s - token_start);
      token_count += 1;
      state = SPACE;
      break;
    }
  }

  return token_count;
}

/* int main(void) { */
/*   char message[80]; */
/*   char error_msg[240]; */
/*   char* tokens[10]; */

/*   while (1) { */
/*     memset(message, 0, sizeof(message)); */
/*     memset(error_msg, 0, sizeof(error_msg)); */
/*     memset(tokens, 0, sizeof(tokens)); */

/*     fgets(message, sizeof(message), stdin); */

/*     *strrchr(message, '\n') = ' '; //change \n to space */

/*     int tc = tokenize(message, tokens, 10); */

/*     if (tc < 0) { */
/*       tokenize_error_message(message, 10, tc, error_msg); */
/*       printf("%s\n", error_msg); */
/*     } else { */
/*       printf("tokens: %d\n", tc); */
/*       for (int i = 0; i < 10; i++) { */
/*         if (tokens[i] != NULL) */
/*           printf("%d) %s\n", i+1, tokens[i]); */
/*       } */
/*     } */
/*     printf("\n"); */
/*   } */

/*   return 0; */
/* } */
