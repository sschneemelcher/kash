#include "utils.h"

#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void display_suggestions(char list[MAX_COMPS][MAX_CMD], int no_elems,
                         int highlight_idx) {
  printf("\n\r");
  for (int i = 0; i < no_elems; i++) {
    printf("%s    ", list[i]);
  }
  char prompt[MAX_PROMPT];
  printf("\n\r");
  print_prompt(prompt);
}

void print_prompt(char *prompt) {
  /* TODO do something more interesting with the prompt */

  char *ps1 = getenv("PS1");
  char pprompt[MAX_PROMPT] = {0};
  int idx = 0;
  while (ps1 && *(ps1) && *(ps1 + 1)) {
    if (*(ps1) == '\\' && *(ps1 + 1) == 'W') {
      ps1 += 1;
      char cwd[MAX_PATH] = {0};
      getcwd(cwd, MAX_PATH);
      char *home = getenv("HOME");
      if (home) {
        int flag = 1;
        int home_idx = 0;
        while (*(home + home_idx) != 0) {
          if (*(home + home_idx) != *(cwd + home_idx))
            flag = 0;
          home_idx += 1;
        }
        if (flag) {
          pprompt[idx] = '~';
        }
        strcpy(pprompt + idx + flag, cwd + (flag * home_idx));
        idx = strlen(pprompt) - 1;
      }
    } else {
      pprompt[idx] = *(ps1);
    }
    ps1 += 1;
    idx += 1;
  }
  pprompt[idx] = *ps1;
  pprompt[idx + 1] = 0;

  printf("%s", pprompt);
}
