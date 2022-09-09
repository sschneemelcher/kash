#include "shell.h"
#include <signal.h>

void intHandler(int dummy) {}

char *HOME;

int main(int argc, char **argv, char **envp) {
  HOME = getenv("HOME");
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
  }

  char *word = strtok(input, " ");

  if (word[0] == 'e' && word[1] == 'x' && word[2] == 'i' && word[3] == 't' &&
      (word[4] == '\n' || word[4] == '\0')) {
    cmd->builtin = EXIT;
    return;
  } else if (word[0] == 'c' && word[1] == 'd') {
    cmd->builtin = CD;
    if (word[2] == '\n') {
      cmd->arg_ptrs[0] = HOME;
      return;
    } else if (word[2] == '\0') {
      char *path = strtok(NULL, " ");
      int i = 0;
      while (*path != '\n' && *path != '\n' && i < MAX_CMD) {
        cmd->argv[0][i] = *path;
        path += 1;
        i += 1;
      }
      cmd->argv[0][i] = '\0';
      cmd->arg_ptrs[0] = cmd->argv[0];
      return;
    }
  }
  cmd->builtin = NONE;
  // parse_cmd(char* cmd, struct command *cmd);
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
