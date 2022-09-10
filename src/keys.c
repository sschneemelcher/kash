#include "keys.h"
#include "utils.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
char *get_completion(char *line, char *comp) {
  comp[0] = '\0';
  return comp;
}


void handle_keys(char *input) {
  int key = 0;
  int end = 0;
  int index = 0;
  while (end < MAX_INPUT - 1) {
    key = getch();
    // printf(" %d ", key);
    switch (key) {
    case 27:   // Arrow Keys emit 27 and
      getch(); // 91 but we dont need the 91
      switch (getch()) {
      case 'A': // Up
        break;
      case 'B': // Down
        break;
      case 'C': // Right
        if (index < end) {
          index += 1;
          printf("\033[C");
        }
        break;
      case 'D': // Left
        if (index > 0) {
          index -= 1;
          printf("\033[D");
        }
        break;
      }
      break;
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
    case '\t': {
      char comp[MAX_CMD];
      get_completion(input, comp);
      int i = 0;
      while (comp[i] != 0) { // append completion and print it
        input[index] = comp[i];
        index += 1;
        i += 1;
      }
      printf("%s", comp);
    } break;
    case '\n':
      input[end] = '\n';
      input[end + 1] = '\0';
      printf("\n");
      return;
    default:
      if (index < end) {
        int i = end;
        printf("\033[%uC", end - index); // got to end of input
        end += 1;
        while (i > index) {
          input[i] = input[i - 1];
          printf("%c\033[2D", input[i]);
          i -= 1;
        }
      }
      input[index] = key;
      index += 1;
      if (index > end) {
        end += 1;
      }
      printf("%c", key);
      break;
    }
  }
}
