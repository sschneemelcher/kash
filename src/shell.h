enum {
  MAX_INPUT = 1024,
  MAX_PATH = 128,
  MAX_PROMPT = 256,
  MAX_ARGS = 16,
  MAX_CMD = 32
};

struct command {
  int argc;
  char argv[MAX_ARGS][MAX_CMD];
  char **arg_ptrs;
  int bg;
  int builtin;
};

int shell_loop(char **env);
void print_prompt(char *cwd, char *prompt);
int parse_input(char *input, struct command *cmd);
void run(struct command cmd, char **env);
