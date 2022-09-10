#include "utils.h"
#include "run.h"
#include <spawn.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void run(struct command cmd, char **env) {
  pid_t pid;
  if (!cmd.bg) {
    int result =
        posix_spawnp(&pid, cmd.arg_ptrs[0], NULL, NULL, cmd.arg_ptrs, env);
    if (result == 0) {
      waitpid(pid, &result, 0);
    }
  } else {
    posix_spawnp(&pid, cmd.arg_ptrs[0], NULL, NULL, cmd.arg_ptrs, env);
  }
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
