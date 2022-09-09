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
void run(struct command cmd, char **env);
void run_builtin(struct command cmd, char **env);
void handle_keys(char *input);
char *get_completion(char *line, char *comp);
void parse_cmd(char *word_ret, struct command *cmd);

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
char *get_completion(char *line, char *comp) {
  comp[0] = '\0';
  return comp;
}

void run(struct command cmd, char **env) {
  pid_t pid;
  if (!cmd.bg) {
    int result =
        posix_spawnp(&pid, cmd.arg_ptrs[0], NULL, NULL, cmd.arg_ptrs, env);
    if (result == 0) {
      waitpid(pid, &result, 0);
    }
  } else {
    posix_spawnp(&pid, cmd.arg_ptrs[0], NULL, NULL, cmd.arg_ptrs, env);
  }
}

void handle_keys(char *input) {
  int key = 0;
  int end = 0;
  int index = 0;
  while (end < MAX_INPUT - 1) {
    key = getch();
    // printf(" %d ", key);
    switch (key) {
    case 27:   // Arrow Keys emit 27 and
      getch(); // 91 but we dont need the 91
      switch (getch()) {
      case 'A': // Up
        break;
      case 'B': // Down
        break;
      case 'C': // Right
        if (index < end) {
          index += 1;
          printf("\033[C");
        }
        break;
      case 'D': // Left
        if (index > 0) {
          index -= 1;
          printf("\033[D");
        }
        break;
      }
      break;
    case 8:   // backspace
    case 127: // delete
      if (index > 0) {
        printf("\033[s"); // save current cursor position
        printf("\033[D");
        index -= 1;
        end -= 1;
        if (index < end) {
          int i = index;
          while (i < end) {
            input[i] = input[i + 1];
            printf("%c", input[i]);
            i += 1;
          }
        }
        printf(" \033[u\033[D");
      }
      break;
    case '\t': {
      char comp[MAX_CMD];
      get_completion(input, comp);
      int i = 0;
      while (comp[i] != 0) { // append completion and print it
        input[index] = comp[i];
        index += 1;
        i += 1;
      }
      printf("%s", comp);
    } break;
    case '\n':
      input[index] = '\n';
      input[index + 1] = '\0';
      printf("\n");
      return;
    default:
      if (index < end) {
        int i = end;
        printf("\033[%uC", end - index); // got to end of input
        end += 1;
        while (i > index) {
          input[i] = input[i - 1];
          printf("%c\033[2D", input[i]);
          i -= 1;
        }
      }
      input[index] = key;
      index += 1;
      if (index > end) {
        end += 1;
      }
      printf("%c", key);
      break;
    }
  }
}
