#include "utils.h"
#include "main.h"
#include "keys.h"
#include "parse.h"
#include "prompt.h"
#include "run.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void intHandler(int dummy) {}

int main(int argc, char **argv, char **envp) {
  signal(SIGINT, intHandler);
  if (argc == 1) {
    shell_loop(envp, INTERACTIVE, "");
  } else {
    shell_loop(envp, NONINTERACTIVE, argv[1]);
  }
  return 0;
}

int shell_loop(char **env, enum session_t sess, char* inp) {
  struct command cmd;
  char input[MAX_INPUT] = "";
  if (sess == INTERACTIVE) {
    char prompt[MAX_PROMPT] = "";

    while (1) {
      print_prompt(prompt);
      handle_keys(input);
      parse_input(input, &cmd);
      run(cmd, env);
    }
  } else {
    parse_input(inp, &cmd);
    run(cmd, env);
  }
  return EXIT_SUCCESS;
}
