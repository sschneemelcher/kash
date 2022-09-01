#include "msh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum {
  MAX_INPUT = 1024,
  MAX_PATH = 128,
  MAX_PROMPT = 256,
  MAX_ARGS = 16,
  MAX_CMD = 32
};

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
    parse_input(input);
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

int parse_input(char *input) {

  /* we need to parse the line we got
   * execvp needs a char* with the command,
   * and a char** for the arguments (including)
   * the command itself
   */

  char argument_list[MAX_ARGS][MAX_CMD] = {0};
  char *argument_ptrs[MAX_ARGS] = {0};
  int i = 0;
  int word = 0;
  int word_idx = 0;
  while (i < MAX_INPUT && word_idx < MAX_CMD && word < MAX_ARGS && input[i] != 0) {
    argument_list[word][word_idx] = input[i];
    i += 1;
    word_idx += 1;
    if (input[i] == ' ') {
      argument_ptrs[word] = argument_list[word];
      i += 1;
      word += 1;
      word_idx = 0;
    } else if (input[i] == '\n') {
      argument_ptrs[word] = argument_list[word];
      break;
    }
  }

  if (argument_list[0][0] != 0) {
    // fork and exec
    const int pid = fork();
    if (pid != 0) {
      int result = 0;
      waitpid(pid, &result, 0);
    } else {
      execvp(argument_list[0], argument_ptrs);
      return 0;
    }
  }
  return 0;
}
