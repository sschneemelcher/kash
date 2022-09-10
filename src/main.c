#include "main.h"
#include "utils.h"
#include "keys.h"
#include "prompt.h"
#include "parse.h"
#include "run.h"
#include <stdlib.h>
#include <unistd.h>

void intHandler(int dummy) {}

int main(int argc, char **argv, char **envp) {
  signal(SIGINT, intHandler);
  shell_loop(envp);
  return 0;
}

int shell_loop(char **env) {
  char prompt[MAX_PROMPT] = "$ ";
  char input[MAX_INPUT] = "";
  char cwd[MAX_PATH] = "";
  while (1) {
    getcwd(cwd, MAX_PATH);
    print_prompt(cwd, prompt);
    handle_keys(input);
    struct command cmd;
    parse_input(input, &cmd);
    if (cmd.builtin == NONE) {
      run(cmd, env);
    } else {
      run_builtin(cmd, env);
    }
  }
  return 0;
}
