#include "prompt.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_prompt(char *prompt) {
  /* TODO do something more interesting with the prompt */
  getcwd(prompt, MAX_PATH);
  char pprompt[MAX_PROMPT] = {0};
  char *home = getenv("HOME");
  
  if (home) {
    int flag = 1;
    int idx = 0;
    while (*(home + idx) != 0) {
       if (*(home + idx) != *(prompt + idx)) flag = 0;
       idx += 1;
    }
    if (flag) {
        pprompt[0] = '~';
    }
    strcpy(pprompt + flag, prompt + (flag * idx));
  }
    // sprintf(prompt, "%s $ ", prompt);
    // printf("%s", prompt);
    printf("%s $ ", pprompt);
}
