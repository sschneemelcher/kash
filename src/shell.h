#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

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

int shell_loop(char **env);
void print_prompt(char *cwd, char *prompt);
void parse_input(char *input, struct command *cmd);
void parse_from_index(char *input, int index, struct command *cmd);
void run(struct command cmd, char **env);
void run_builtin(struct command cmd, char **env);
void handle_keys(char *input);
char *get_completion(char *line, char *comp);

int getch() {
  struct termios raw_mode, buffered_mode;
  int ch;
  tcgetattr(STDIN_FILENO, &buffered_mode);
  memcpy(&raw_mode, &buffered_mode, sizeof(struct termios));
  raw_mode.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &buffered_mode);
  return ch;
}

void print_prompt(char *cwd, char *prompt) {
  /* TODO do something more interesting with the prompt */
  // sprintf(prompt, "%s $ ", cwd);
  // printf("%s", prompt);
  printf("%s $ ", cwd);
}

/* TODO should return a completion based on input */
char* get_completion(char *line, char *comp) {
    comp[0] = '\0';
    return comp;
}

void run(struct command cmd, char **env) {
  pid_t pid;
  int result =
      posix_spawnp(&pid, cmd.arg_ptrs[0], NULL, NULL, cmd.arg_ptrs, env);
  if (result == 0) {
    waitpid(pid, &result, 0);
  }
}
