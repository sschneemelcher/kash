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
  enum builtin_t { NONE, EXIT, CD, EMPTY } builtin;
};

char* HOME;
