#define MAX_INPUT 2048
#define MAX_PATH 128
#define MAX_PROMPT 256
#define MAX_ARGS 16
#define MAX_CMD 32
#define MAX_HISTORY 10
#define MAX_ALIASES 32

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MOD(a, b) ((a % b + b) % b)

#define IS_WS(a)                                                               \
  (a == ' ' || a == '\t' || a == '\r' || a == '\v' || a == '\f')


struct array {
    void* values;
    int length;
};

enum session_t {
  INTERACTIVE,
  NONINTERACTIVE,
};

struct command {
  int argc;
  char argv[MAX_ARGS][MAX_CMD];
  char *arg_ptrs[MAX_CMD];
  int bg;
  int pipe;
  enum builtin_t { NONE, EXIT, CD, EMPTY, ECHO, ALIAS } builtin;
};

int hash(char* string, int length);
void graceful_exit(char** free, int return_code);
