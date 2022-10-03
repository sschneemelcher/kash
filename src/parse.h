void parse_input(char *, struct command *, char **);

// written as macro because then we can use pointers instead of copying memory
#define KEY_VALUE(input, key, value)                                           \
  ({                                                                           \
    key = input;                                                               \
    while (*input != '=' && *input != 0)                                       \
      input += 1;                                                              \
    *input++ = 0;                                                              \
    char quote = 0;                                                            \
    if (*input == '\'' || *input == '"' || *input == '`')                      \
      quote = *(input++);                                                        \
    value = (input);                                                         \
    while (*input != quote && *input != 0)                                     \
      input += 1;                                                              \
    if (*input == quote)                                                        \
      *input = 0;                                                              \
  })
