#include "shell.h"
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp) {
  //char *env[2] = {getenv("TERM"), NULL};
  shell_loop(envp);
  return 0;
}

int shell_loop(char** env) {
  char prompt[MAX_PROMPT] = "> ";
  char input[MAX_INPUT] = {0};
  char cwd[MAX_PATH] = {0};
  do {
    getcwd(cwd, MAX_PATH);
    print_prompt(cwd, prompt);
    fgets(input, MAX_INPUT, stdin);
    struct command cmd;
    if (parse_input(input, &cmd) > 0) {
      run(cmd, env);
    }
  } while (!(input[0] == 'e' && input[1] == 'x' && input[2] == 'i' &&
             input[3] == 't'));

  return 0;
}

void run(struct command cmd, char** env) {
  pid_t pid;
  int result =
      posix_spawnp(&pid, cmd.arg_ptrs[0], NULL, NULL, cmd.arg_ptrs, env);
  if (result == 0) {
   waitpid(pid, &result, 0);
  }
}

void print_prompt(char *cwd, char *prompt) {
  /* TODO do something more interesting with the prompt */
  // sprintf(prompt, "%s $ ", cwd);
  // printf("%s", prompt);
  printf("%s $ ", cwd);
}

int parse_input(char *input, struct command *cmd) {

  /* we need to parse the line we got
   * execvp needs a char* with the command,
   * and a char** for the arguments (including)
   * the command itself
   */

  int i = 0;
  int word = 0;
  int word_idx = 0;
  while (i < MAX_INPUT - 1 && word_idx < MAX_CMD - 1 && word < MAX_ARGS - 1 &&
         input[i] != 0) {
    cmd->argv[word][word_idx] = input[i];
    i += 1;
    word_idx += 1;
    if (input[i] == ' ') {
      cmd->argv[word][word_idx] = '\0';
      cmd->arg_ptrs[word] = cmd->argv[word];
      i += 1;
      word += 1;
      word_idx = 0;
    } else if (input[i] == '\n') {
      cmd->argv[word][word_idx] = '\0';
      cmd->arg_ptrs[word] = cmd->argv[word];
      if (i != 0)
        word += 1;
      break;
    }
  }
  cmd->arg_ptrs[word] = NULL;
  cmd->argc = word;
  cmd->bg = 0;
  cmd->builtin = 0;
  return word;
}
