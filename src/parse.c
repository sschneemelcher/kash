#include "utils.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_input(char *input, struct command *cmd) {

  /* we need to parse the line we got
   * execvp needs a char* with the command,
   * and a char** for the arguments (including)
   * the command itself
   */

  char *word = input;
  int word_start = 0;
  int cmd_idx = 0;
  int input_idx = 0;
  int len = 0;
  while (*input != 0 && len < MAX_CMD) {
    if (*input == ' ') {
      cmd->argv[cmd_idx][len] = 0;
      cmd->arg_ptrs[cmd_idx] = cmd->argv[cmd_idx];
      input += 1;
      word = input;
      input_idx += 1;
      word_start = input_idx;
      cmd_idx += 1;
      len = 0;
    } else {
      cmd->argv[cmd_idx][len] = *input;
      len += 1;
      input_idx += 1;
      input += 1;
    }
  }
  cmd->argv[cmd_idx][len] = 0;
  if (strcmp(cmd->argv[cmd_idx], "&") == 0) {
    cmd->arg_ptrs[cmd_idx] = 0;
    cmd->bg = 1;
  } else {
    cmd->arg_ptrs[cmd_idx] = cmd->argv[cmd_idx];
    cmd->arg_ptrs[cmd_idx + 1] = 0;
  }

  if (cmd->argv[0][0] == 'c' && cmd->argv[0][1] == 'd' &&
      cmd->argv[0][2] == 0) {
    cmd->builtin = CD;
  } else if (cmd->argv[0][0] == 'e') {
    if (cmd->argv[0][1] == 'x') {
      if (strcmp(&(cmd->argv[0][2]), "it") == 0)
        cmd->builtin = EXIT;
    } else if (cmd->argv[0][1] == 'c') {
      if (strcmp(&(cmd->argv[0][2]), "ho") == 0)
        cmd->builtin = ECHO;
    }
  } else {
    cmd->builtin = NONE;
  }
}
