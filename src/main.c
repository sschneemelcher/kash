#include "utils.h"
#include <stdio.h>
#include "main.h"
#include "keys.h"
#include "parse.h"
#include "prompt.h"
#include "run.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <fcntl.h>

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

  if (!isatty(STDIN_FILENO)) {
    shell_loop(envp, NONINTERACTIVE, STDIN_FILENO);
  } else if (optind >= argc) {
    shell_loop(envp, INTERACTIVE, 0);
  } else {
    int i = optind;
    while (i < argc) {
      int fp = open(argv[i], O_RDONLY);
      if (fp == 0) {
        printf("-kash: unable to open file '%s': %s\n", argv[i], strerror(errno));
        return 1;
      } else {
        shell_loop(envp, NONINTERACTIVE, fp);
        close(fp);
      }
      i++;
    }
    return 0;
  }
}

int shell_loop(char **env, enum session_t sess, int inp) {
  struct command cmd;
  char input[MAX_INPUT + 1] = "";
  char prompt[MAX_PROMPT + 1] = "";

  while (1) {
    if (sess == INTERACTIVE) {
      print_prompt(prompt);
      handle_keys(input);
    } else {
      read(inp, input, MAX_INPUT);
      input[MAX_INPUT] = 0;
    }
    char *line_ret;
    for (char *line = strtok_r(input, "\n;", &line_ret); line;
         line = strtok_r(NULL, "\n;", &line_ret)) {
      parse_input(line, &cmd);
      if(run(cmd, env))
          return EXIT_SUCCESS;
    }
    if (sess == NONINTERACTIVE)
      break;
  }
  return EXIT_SUCCESS;
}
