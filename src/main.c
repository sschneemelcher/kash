#include "utils.h"

#include "keys.h"
#include "main.h"
#include "parse.h"
#include "prompt.h"
#include "run.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char **argv, char **envp) {
  signal(SIGINT, intHandler);

  int opt;
  char inp[MAX_INPUT] = {0};
  while ((opt = getopt(argc, argv, "c:")) != -1) {
    if (opt == 'c') {
      memcpy(inp, optarg, MAX_INPUT);
      inp[MAX_INPUT - 1] = 0;
      execute_commands(inp, envp, NULL);
      return EXIT_SUCCESS;
    }
  }

  if (!isatty(STDIN_FILENO)) {
    // input is piped into kash
    read(STDIN_FILENO, inp, MAX_INPUT);
    execute_commands(inp, envp, NULL);
  } else if (optind >= argc) {
    // interactive session
    shell_loop(envp, inp);
  } else {
    // start kash with files (e.g. `kash script.sh`)
    int i = optind;
    while (i < argc) {
      int fp = open(argv[i], O_RDONLY);
      if (fp == 0) {
        printf("-kash: unable to open file '%s': %s\n", argv[i],
               strerror(errno));
        return 1;
      } else {
        read(fp, inp, MAX_INPUT);
        close(fp);
        execute_commands(inp, envp, NULL);
      }
      i += 1;
    }
    return EXIT_SUCCESS;
  }
}

int shell_loop(char **env, char input[MAX_INPUT]) {

  char prompt[MAX_PROMPT] = "";
  char *aliases[MAX_ALIASES][2] = {};
  char history[MAX_HISTORY][MAX_INPUT];

  for (int i = 0; i < MAX_HISTORY; i++) {
    history[i][0] = 0;
  }
  int history_idx = 1;
  char kashrc_path[MAX_PATH];
  sprintf(kashrc_path, "%s%s", getenv("HOME"), "/.kashrc");
  int fp = open(kashrc_path, O_RDONLY);
  read(fp, input, MAX_INPUT);
  execute_commands(input, env, aliases);

  do {
    print_prompt(prompt);
    handle_keys(input, history, history_idx);
    input[MAX_INPUT - 1] = 0;

    if (!execute_commands(input, env, aliases)) {
      history_idx = MOD((history_idx + 1), MAX_HISTORY);
    }

    memset(input, 0, MAX_INPUT);
  } while (1);

  graceful_exit(aliases, EXIT_SUCCESS);
  return EXIT_FAILURE;
}

int execute_commands(char input[MAX_INPUT], char **env,
                     char *aliases[MAX_ALIASES][2]) {

  int pipes[8][2];

  if (*input == 0)
    return 1;
  char *current;
  int end_flag = 0;
  int pipe_flag = 0;
  while (*input != 0) {
    current = input;
    struct command cmd;
    while (*input != 0 && *input != '\n' && *input != ';' && *input != '|') {
      input += 1;
    }

    if (pipe_flag) {
      cmd.pipe = 2;
      pipe_flag = 0;
    }

    if (*input == 0) {
      end_flag = 1;
    } else if (*input == '|') {
      cmd.pipe = 1;
      cmd.bg = 1;
      pipe_flag = 1;
    }
    *input = 0;
    parse_input(current, &cmd, aliases);
    if (run(cmd, env, aliases, pipes)) {
      graceful_exit(aliases, EXIT_SUCCESS);
    }

    if (end_flag == 0) {
      input += 1;
    }
  }
  return 0;
}
