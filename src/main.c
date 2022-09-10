#include "utils.h"
#include "main.h"
#include "keys.h"
#include "parse.h"
#include "prompt.h"
#include "run.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  char input[MAX_INPUT + 1] = "";
  char prompt[MAX_PROMPT + 1] = "";

  while (1) {
    if (sess == INTERACTIVE) {
      print_prompt(prompt);
      handle_keys(input);
    } else {
      strcpy(input, inp);
      input[MAX_INPUT] = 0;
    }
    char *line_ret;
    for (char *line = strtok_r(input, "\n;", &line_ret); line;
         line = strtok_r(NULL, "\n;", &line_ret)) {
      parse_input(line, &cmd);
      run(cmd, env);
    }
    if (sess == NONINTERACTIVE)
      break;
  }
  return EXIT_SUCCESS;
}
