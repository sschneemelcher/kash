#include "utils.h"

#include "keys.h"
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int getch() {
  struct termios raw_mode, buffered_mode;
  int ch;
  tcgetattr(STDIN_FILENO, &buffered_mode);
  memcpy(&raw_mode, &buffered_mode, sizeof(struct termios));
  raw_mode.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &buffered_mode);
  return ch;
}

/* TODO should return a completion based on input */
int get_completion(char *line, char *comp) {
  comp[0] = '\0';
  return strlen(comp);
}

void handle_keys(char *input, char history[MAX_HISTORY][MAX_INPUT], int history_idx) {
  int key = 0;
  int end = 0;
  int index = 0;
  input[0] = 0;
  while (end < MAX_INPUT) {
    switch (key = getch()) {
    case 27: // Arrow Keys emit 27 and 91
      switch (arrow(index, end)) {
      case 1:
        index += 1;
        break;
      case -1:
        index -= 1;
        break;
      case 2:
        history_idx = MOD(history_idx - 1, MAX_HISTORY);
        strcpy(input, history[history_idx]);
        if (index > 0)
            printf("\033[%iD",index);
        printf("\033[0K%s", input);
        index = strlen(input);
        end = index;
        break;
      case -2:
       history_idx = MOD(history_idx + 1, MAX_HISTORY) ;
        strcpy(input, history[history_idx]);
        if (index > 0)
            printf("\033[%iD",index);
        printf("\033[0K%s", input);
        index = strlen(input);
        end = index;
        break;
      }
      break;
    case '\t': {
      if (index == end) {
        char comp[MAX_CMD];
        int len = get_completion(input, comp);
        if ((len + end) < MAX_INPUT) {
          index = end + len;
        } else {
          index = MAX_INPUT - end;
        }
        memcpy((input + end), comp, index);
        end = index;
        printf("%.*s", index, comp);
      }
    } break;
    case '\n':
    case 4: // eot
      input[end] = '\0';
      printf("\n");
      return;
    case 8:   // backspace
    case 127: // delete
      if (index > 0) {
        index -= 1;
        // move content of input to the left
        memcpy((input + index), (input + index + 1), end - index);
        end -= 1;
        input[end] = 0;
        printf("\033[D\033[s\033[0J%s\033[u", &input[index]);
      }
      break;
    default:
      // like deleting in reverse
      end += 1;
      memcpy((input + index + 1), (input + index), end - index);
      input[index] = key;
      printf("\033[s\033[0J%s\033[u\033[C", &input[index]);
      index += 1;
      break;
    }
  }
}

int arrow(int index, int end) {
  getch(); // we dont need the 91
  switch (getch()) {
  case 'A': // Up
    return 2;
    break;
  case 'B': // Down
    return -2;
    break;
  case 'C': // Right
    if (index < end) {
      printf("\033[C");
      return 1;
    }
    break;
  case 'D': // Left
    if (index > 0) {
      printf("\033[D");
      return -1;
    }
    break;
  }
  return 0;
}
