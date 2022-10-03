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

void intHandler(int dummy) {}

int main(int argc, char **argv, char **envp) {
  signal(SIGINT, intHandler);
  int opt;
  char inp[MAX_INPUT] = {0};
  enum session_t tsession = INTERACTIVE;
  while ((opt = getopt(argc, argv, "c:")) != -1) {
    switch (opt) {
    case 'c':
      memcpy(inp, optarg, MAX_INPUT);
      inp[MAX_INPUT - 1] = 0;
      tsession = NONINTERACTIVE;
      break;
    }
  }

  if (!isatty(STDIN_FILENO)) {
    shell_loop(envp, NONINTERACTIVE, STDIN_FILENO, NULL);
  } else if (optind >= argc) {
    shell_loop(envp, tsession, 0, inp);
  } else {
    int i = optind;
    while (i < argc) {
      int fp = open(argv[i], O_RDONLY);
      if (fp == 0) {
        printf("-kash: unable to open file '%s': %s\n", argv[i],
               strerror(errno));
        return 1;
      } else {
        shell_loop(envp, NONINTERACTIVE, fp, NULL);
        close(fp);
      }
      i++;
    }
    return 0;
  }
}

int shell_loop(char **env, int sess, int input_fd, char *input_str) {
  char *aliases[MAX_ALIASES] = {};
  char history[MAX_HISTORY][MAX_INPUT];
  for (int i = 0; i <= MAX_HISTORY; i++) {
    history[i][0] = '\0';
  }
  char input[MAX_INPUT] = "";
  char prompt[MAX_PROMPT] = "";

  int history_idx = 0;

  if (sess == INTERACTIVE) {
    char kashrc_path[MAX_PATH];
    strcpy(kashrc_path, getenv("HOME"));
    strcpy(kashrc_path + strlen(kashrc_path), "/.kashrc");
    int fp = open(kashrc_path, O_RDONLY);
    read(fp, input, MAX_INPUT);
    execute_commands(input, env, aliases, sess);
  }

  do {
    if (sess == INTERACTIVE) {
      print_prompt(prompt);
      handle_keys(input, history, history_idx);
    } else {
      if (input_str != 0) {
        memcpy(input, input_str, MAX_INPUT);
      } else {
        read(input_fd, input, MAX_INPUT);
      }
      input[MAX_INPUT - 1] = 0;
    }
    execute_commands(input, env, aliases, sess);
    strcpy(history[history_idx], input);
    history_idx = (history_idx + 1) % MAX_HISTORY;

  } while (sess == INTERACTIVE);

  for (int i = 0; i < MAX_ALIASES; i++) {
    if (aliases[i] != 0)
      free(aliases[i]);
  }
  return EXIT_SUCCESS;
}

int execute_commands(char *input, char **env, char **aliases, int sess) {
  char *line_ret;
  struct command cmd;
  for (char *line = strtok_r(input, "\n;", &line_ret); line;
       line = strtok_r(NULL, "\n;", &line_ret)) {
    parse_input(line, &cmd, aliases);
    if (run(cmd, env, aliases, sess)) {
      for (int i = 0; i < MAX_ALIASES; i++) {
        if (aliases[i])
          free(aliases[i]);
      }
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}