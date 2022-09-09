#include "shell.h"
#include <signal.h>

void intHandler(int dummy) {}

char *HOME;

int main(int argc, char **argv, char **envp) {
  HOME = getenv("HOME");
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

void run_builtin(struct command cmd, char **env) {
  switch (cmd.builtin) {
  case NONE:
  case EMPTY:
    break;
  case CD: {
    if (chdir(cmd.arg_ptrs[0])) { // this is true if cd returns an error
      printf("-kash: cd: %s: No such file or directory\n", cmd.arg_ptrs[0]);
    }
    break;
  };
  case EXIT:
    exit(0);
  }
}

void parse_input(char *input, struct command *cmd) {

  /* we need to parse the line we got
   * execvp needs a char* with the command,
   * and a char** for the arguments (including)
   * the command itself
   */

  if (input[0] == '\n') {
    cmd->builtin = EMPTY;
  }
  char *line_ret, *word_ret;
  char *line = strtok_r(input, "\n;", &line_ret);
  char *word = strtok_r(line, "\n \t\r\v\f", &word_ret);

  if ((strcmp(word, "exit") == 0)) {
    cmd->builtin = EXIT;
  } else if (word[0] == 'c' && word[1] == 'd') {
    cmd->builtin = CD;
    char *path = strtok_r(NULL, " ", &word_ret);
    if (path == NULL) {
      strcpy(cmd->argv[0], HOME);
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
    if (*word == '&' && !strtok_r(NULL, " ", &word_ret)) {
      cmd->bg = 1;
      break;
    } else {
      strcpy(cmd->argv[i], word);
      cmd->arg_ptrs[i] = cmd->argv[i];
    }
    i+=1;
  }
}
