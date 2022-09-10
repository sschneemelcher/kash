#include "utils.h"
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void parse_input(char *input, struct command *cmd) {

  /* we need to parse the line we got
   * execvp needs a char* with the command,
   * and a char** for the arguments (including)
   * the command itself
   */

  if (input[0] == '\0') {
    cmd->builtin = EMPTY;
    return;
  }
  char *word_ret;
  char *word = strtok_r(input, "\n \t\r\v\f", &word_ret);

  if ((strcmp(word, "exit") == 0)) {
    cmd->builtin = EXIT;
  } else if (word[0] == 'c' && word[1] == 'd') {
    cmd->builtin = CD;
    char *path = strtok_r(NULL, " ", &word_ret);
    if (path == NULL) {
      strcpy(cmd->argv[0], getenv("HOME"));
    } else {
      strcpy(cmd->argv[0], path);
    }
    cmd->arg_ptrs[0] = cmd->argv[0];
  } else {
    strcpy(cmd->argv[0], word);
    cmd->arg_ptrs[0] = cmd->argv[0];
    cmd->builtin = NONE;
    parse_cmd(word_ret, cmd);
  }
}

void parse_cmd(char *word_ret, struct command *cmd) {
  int i = 1;
  cmd->bg = 0;
  for (char *word = strtok_r(NULL, " ", &word_ret); word;
       word = strtok_r(NULL, " ", &word_ret)) {
    if (!strcmp(word, "&") && !strtok_r(NULL, " ", &word_ret)) {
      cmd->bg = 1;
      break;
    } else {
      strcpy(cmd->argv[i], word);
      cmd->arg_ptrs[i] = cmd->argv[i];
    }
    i += 1;
  }
  cmd->arg_ptrs[i] = NULL;
}
