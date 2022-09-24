#define MAX_INPUT 256
#define MAX_PATH 128
#define MAX_PROMPT 256
#define MAX_ARGS 16
#define MAX_CMD 32
#define MAX_HISTORY 10

enum session_t {
  INTERACTIVE,
  NONINTERACTIVE,
};

struct command {
  int argc;
  char argv[MAX_ARGS][MAX_CMD];
  char *arg_ptrs[MAX_CMD];
  int bg;
  enum builtin_t { NONE, EXIT, CD, EMPTY, ECHO } builtin;
};

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MOD(a, b) ((a % b + b) % b)
