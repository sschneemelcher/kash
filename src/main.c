#include "utils.h"
#include "main.h"
#include "keys.h"
#include "parse.h"
#include "prompt.h"
#include "run.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void intHandler(int dummy) {}

int main(int argc, char **argv, char **envp) {
  signal(SIGINT, intHandler);
  shell_loop(envp);
  return 0;
}

int shell_loop(char **env) {
  char input[MAX_INPUT] = "";
  char prompt[MAX_PROMPT] = "";
  struct command cmd;

  while (1) {
    print_prompt(prompt);
    handle_keys(input);
    parse_input(input, &cmd);
    switch (cmd.builtin) {
    case NONE:
      run(cmd, env);
      break;
    case CD:
      run_cd(cmd);
      break;
    case EXIT:
      return EXIT_SUCCESS;
    case EMPTY:
      break;
    }
  }
  return EXIT_SUCCESS;
}
