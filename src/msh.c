#include "msh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  shell_loop();
  return 0;
}

int shell_loop() {
  char prompt[MAX_PROMPT] = "> ";
  char input[MAX_INPUT] = {0};
  char cwd[MAX_PATH] = {0};
  do {
    getcwd(cwd, MAX_PATH);
    print_prompt(cwd, prompt);
    fgets(input, MAX_INPUT, stdin);
    struct command cmd;
    if (parse_input(input, &cmd) > 0) {
      // fork and exec
      const int pid = fork();
      if (pid != 0) {
        int result = 0;
        waitpid(pid, &result, 0);
      } else {
        execvp(cmd.argv[0], cmd.arg_ptrs);
        return 0;
      }
    }
  } while (!(input[0] == 'e' && input[1] == 'x' && input[2] == 'i' &&
             input[3] == 't'));

  return 0;
}

void print_prompt(char *cwd, char *prompt) {
  /* TODO do something more interesting with the prompt */
  // sprintf(prompt, "%s $ ", cwd);
  // printf("%s", prompt);
  printf("%s $ ", cwd);
}

int parse_input(char *input, struct command *cmd) {

  /* we need to parse the line we got
   * execvp needs a char* with the command,
   * and a char** for the arguments (including)
   * the command itself
   */

  int i = 0;
  int word = 0;
  int word_idx = 0;
  while (i < MAX_INPUT - 1 && word_idx < MAX_CMD - 1 && word < MAX_ARGS - 1 &&
         input[i] != 0) {
    cmd->argv[word][word_idx] = input[i];
    i += 1;
    word_idx += 1;
    if (input[i] == ' ') {
      cmd->argv[word][word_idx] = '\0';
      cmd->arg_ptrs[word] = cmd->argv[word];
      i += 1;
      word += 1;
      word_idx = 0;
    } else if (input[i] == '\n') {
      cmd->argv[word][word_idx] = '\0';
      cmd->arg_ptrs[word] = cmd->argv[word];
      if (i != 0)
        word += 1;
      break;
    }
  }
  cmd->arg_ptrs[word] = NULL;
  cmd->argc = word;
  cmd->bg = 0;
  cmd->builtin = 0;
  return word;
}
