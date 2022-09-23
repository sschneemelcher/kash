#include "utils.h"
#include <stdio.h>
#include "main.h"
#include "parse.h"
#include "prompt.h"
#include "run.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>

void intHandler(int dummy) {}

int main(int argc, char **argv, char **envp) {
  if (!isatty(STDIN_FILENO)) {
    shell_loop(envp, NONINTERACTIVE, STDIN_FILENO);
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
  read(inp, input, MAX_INPUT);
  input[MAX_INPUT] = 0;
  parse_input(input, &cmd);
  if(run(cmd, env))
      return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
