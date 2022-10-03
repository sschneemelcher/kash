#include "utils.h"

#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_input(char *input, struct command *cmd, char **aliases) {

  if (*input == 0)
    return;
  char last_char = ' ';
  char quote = ' ';
  int cmd_idx = 0;
  int len = 0;
  while (*input != 0 && len < MAX_CMD) {
    if (IS_WS(*input) && !IS_WS(last_char) && quote == ' ') {
      /* split args on whitespace */
      last_char = ' ';
      cmd->argv[cmd_idx][len] = 0;
      cmd->arg_ptrs[cmd_idx] = cmd->argv[cmd_idx];
      input += 1;
      cmd_idx += 1;
      len = 0;
    } else if (IS_WS(*input) && IS_WS(last_char) && quote == ' ') {
      /* skip additional whitespaces */
      input += 1;
      last_char = *input;
    } else if ((*input == '\'' || *input == '"') && quote == ' ') {
      /* beginning of a quote with either ' or " */
      quote = *input;
      input += 1;
      last_char = quote;
    } else if (*input == quote) {
      /* end of quote */
      *input = ' ';
      quote = ' ';
    } else {
      /* everything else - write args into cmd struct */
      last_char = *input;
      cmd->argv[cmd_idx][len] = last_char;
      len += 1;
      input += 1;
    }
  }

  cmd->bg = 0;
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

  int hash_value =
      MOD(hash(cmd->arg_ptrs[0], strlen(cmd->arg_ptrs[0])), MAX_ALIASES);

  if (aliases[hash_value] != 0) {
    cmd->arg_ptrs[0] = aliases[hash_value];
  }

  cmd->builtin = NONE;
  if (strcmp(cmd->arg_ptrs[0], "cd") == 0) {
    cmd->builtin = CD;
  } else if (cmd->arg_ptrs[0][0] == 'e') {
    if (cmd->arg_ptrs[0][1] == 'x') {
      if (strcmp(&(cmd->arg_ptrs[0][2]), "it") == 0) {
        cmd->builtin = EXIT;
      }
    } else if (cmd->arg_ptrs[0][1] == 'c') {
      if (strcmp(&(cmd->arg_ptrs[0][2]), "ho") == 0) {
        cmd->builtin = ECHO;
      }
    }
  } else if (strcmp(cmd->arg_ptrs[0], "alias") == 0) {
    cmd->builtin = ALIAS;
  }
}


void parse_key_value(char *input, char *key, char *value) {
  char *start = input;
  while (*input != 0 && *input != '=') {
    input += 1;
  }
  if (*input == '=' && (input + 1) != 0) {
    *input = 0;
    strcpy(key, start);
    strcpy(value, input+1);
  }
}