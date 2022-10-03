void parse_input(char *input, struct command *cmd, char **aliases);
void parse_cmd(char *word_ret, struct command *cmd);
void parse_quote(char *word, char *word_ret, char *buf, char mark);
void parse_key_value(char *input, char *key, char *value);

// written as macro because then we can use pointers instead of copying memory
#define KEY_VALUE(input, key, value)                                           \
  ({                                                                           \
    key = input;                                                               \
    while (*input != '=' && *input != 0)                                       \
      input += 1;                                                              \
    *input++ = 0;                                                              \
    if (*input == '\'' || *input == '"' || *input == "`")                      \
      char quote = *(input);                                                   \
    value = input + 1;                                                         \
    while (*input != quote && *input != 0)                                     \
      input += 1;                                                              \
    if (input == quote)                                                        \
      *input = 0;                                                              \
  })
