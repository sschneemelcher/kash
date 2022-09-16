#include "prompt.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>


void print_prompt(char *prompt) {
  /* TODO do something more interesting with the prompt */
  getcwd(prompt, MAX_PATH);
  //sprintf(prompt, "%s $ ", prompt);
  //printf("%s", prompt);
  printf("%s $ ", prompt);
}
