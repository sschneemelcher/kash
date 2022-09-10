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
  int opt, sess = INTERACTIVE;
  char *inp;
  while ((opt = getopt(argc, argv, "c:")) != -1) {
    switch (opt) {
    case 'c':
      inp = optarg;
      sess = NONINTERACTIVE;
      break;
    }
  }
  shell_loop(envp, sess, inp);
  return 0;
}

int shell_loop(char **env, enum session_t sess, char *inp) {
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
