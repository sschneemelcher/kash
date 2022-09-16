#include "utils.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *SPACE_SEP = "\n \t\r\v\f";

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
  char *word = strtok_r(input, SPACE_SEP, &word_ret);

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
  for (char *word = strtok_r(NULL, SPACE_SEP, &word_ret); word;
       word = strtok_r(NULL, SPACE_SEP, &word_ret)) {
    if (!strcmp(word, "&") && !strtok_r(NULL, " ", &word_ret)) {
      cmd->bg = 1;
      break;
    } else if (word[0] == '\"' || word[0] == '\'') {
      char sep[2] = {word[0], 0};
      char *quote_start = word + 1;
      *(word_ret - 1) = ' ';
      if (strtok_r(NULL, sep, &word_ret)) {
        memcpy(cmd->argv[i], quote_start, MAX_CMD);
        cmd->argv[i][MAX_CMD - 1] = 0;
      }
    } else {
      strcpy(cmd->argv[i], word);
    }
    cmd->arg_ptrs[i] = cmd->argv[i];
    i += 1;
  }
  cmd->arg_ptrs[i] = NULL;
}

void parse_quote(char *word, char word_ret[MAX_INPUT], char buf[MAX_CMD],
                 char mark) {}
