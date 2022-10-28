#include "utils.h"

#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void display_suggestions(char list[MAX_COMPS][MAX_CMD], int no_elems,  int highlight_idx) {
  printf("\n\033[s\r");
  int r = 0;
  while (no_elems >= 0) {
    if (MOD(no_elems, 20) == 0) {
      printf("\033[u\033[%iA", r);
      r += 1;
      for (int i = 0; i < r; i++) {
        printf("\n");
      }
      printf("\033[s\r");
    }
    printf("%s ", *(list + no_elems));
    no_elems -= 1;
  }
  printf("\033[u\033[%iA", r);
}

void print_prompt(char *prompt) {
  /* TODO do something more interesting with the prompt */
  getcwd(prompt, MAX_PATH);
  char pprompt[MAX_PROMPT] = {0};
  char *home = getenv("HOME");

  if (home) {
    int flag = 1;
    int idx = 0;
    while (*(home + idx) != 0) {
      if (*(home + idx) != *(prompt + idx))
        flag = 0;
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
