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

  char last_char = ' ';
  char quote = ' ';
  int cmd_idx = 0;
  int len = 0;
  while (*input != 0 && len < MAX_CMD) {
    if (*input == ' ' && last_char != ' ' && quote == ' ') {
      last_char = ' ';
      cmd->argv[cmd_idx][len] = 0;
      cmd->arg_ptrs[cmd_idx] = cmd->argv[cmd_idx];
      input += 1;
      cmd_idx += 1;
      len = 0;
    } else if (*input == ' ' && last_char == ' ' && quote == ' ') {
      input += 1;
      last_char = *input;
    } else if ((*input == '\'' || *input == '"') && quote == ' ') {
        quote = *input;
        input += 1;
        last_char = quote;
    } else if (*input == quote) {
        *input = ' ';
        quote = ' ';
    } else {
      last_char = *input;
      cmd->argv[cmd_idx][len] = last_char;
      len += 1;
      input += 1;
    }
  }

  if (len > 0) {
    if (len == 1 && cmd->argv[cmd_idx][0] == '&') {
        cmd->bg = 1;
        cmd->arg_ptrs[cmd_idx] = 0;
      } else {
        cmd->argv[cmd_idx][len] = 0;
        cmd->arg_ptrs[cmd_idx] = cmd->argv[cmd_idx];
        cmd->arg_ptrs[cmd_idx + 1] = 0;
      }
  } else {
    cmd->arg_ptrs[cmd_idx] = 0;
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
