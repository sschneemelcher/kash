#include "shell.h"
#include <signal.h>

void intHandler(int dummy) {
}

int main(int argc, char **argv, char **envp) {
  signal(SIGINT, intHandler);
  shell_loop(envp);
  return 0;
}

int shell_loop(char **env) {
  char prompt[MAX_PROMPT] = "$ ";
  char input[MAX_INPUT] = "";
  char cwd[MAX_PATH] = "";
  while (1) {
    getcwd(cwd, MAX_PATH);
    print_prompt(cwd, prompt);
    handle_keys(input);
    struct command cmd;
    parse_input(input, &cmd);
    if (cmd.builtin == NONE) {
      run(cmd, env);
    } else {
      run_builtin(cmd, env);
    }
  }
  return 0;
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
      index -= 1;
      while (end > index) {
          input[end] = '\0';
          end -= 1;
      }
      input[index] = '\0';
      printf("\033[D\033[J");
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

void run_builtin(struct command cmd, char **env) {
  switch (cmd.builtin) {
  case NONE:
  case EMPTY:
    break;
  case CD: {
    if (chdir(cmd.arg_ptrs[0])) { // this is true if cd returns an error
      printf("-kash: cd: %s: No such file or directory\n", cmd.arg_ptrs[0]);
    }
    break;
  };
  case EXIT:
    exit(0);
  }
}

void parse_input(char *input, struct command *cmd) {

  /* we need to parse the line we got
   * execvp needs a char* with the command,
   * and a char** for the arguments (including)
   * the command itself
   */
  if (input[0] == '\n') {
    cmd->builtin = EMPTY;
  } else if (input[0] == 'e' && input[1] == 'x' && input[2] == 'i' &&
             input[3] == 't' && input[4] == '\n') {
    cmd->builtin = EXIT;
  } else if (input[0] == 'c' && input[1] == 'd') {
    cmd->builtin = CD;
    if (input[2] == '\n' || (input[2] == ' ' && input[3] == '\n')) {
      memcpy(cmd->argv[0], getenv("HOME"), MAX_CMD);
      cmd->arg_ptrs[0] = cmd->argv[0];
    } else if (input[2] == ' ') {
      if (input[3] == '\n') {
        memcpy(cmd->argv[0], getenv("HOME"), MAX_CMD);
        cmd->arg_ptrs[0] = cmd->argv[0];
      } else {
        parse_from_index(input, 3, cmd);
      }
    }
  } else {
    cmd->builtin = NONE;
    parse_from_index(input, 0, cmd);
  }
}

void parse_from_index(char *input, int index, struct command *cmd) {
  int word = 0;
  int word_idx = 0;
  int max_args = MAX_ARGS - 1;
  if (cmd->builtin == CD) {
    max_args = 1;
  }
  while (index < MAX_INPUT - 1 && word_idx < MAX_CMD - 1 && word < max_args &&
         input[index] != 0) {
    cmd->argv[word][word_idx] = input[index];
    index += 1;
    word_idx += 1;
    if (input[index] == ' ') {
      cmd->argv[word][word_idx] = '\0';
      cmd->arg_ptrs[word] = cmd->argv[word];
      index += 1;
      word += 1;
      word_idx = 0;
    } else if (input[index] == '\n') {
      cmd->argv[word][word_idx] = '\0';
      cmd->arg_ptrs[word] = cmd->argv[word];
      if (index != 0)
        word += 1;
      break;
    }
  }
  cmd->arg_ptrs[word] = NULL;
  cmd->argc = word;
  cmd->bg = 0;
}
