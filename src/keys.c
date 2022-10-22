#include "utils.h"

#include "keys.h"
#include <dirent.h>
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
int get_completion(char *word, char comp[MAX_CMD]) {
  comp[0] = 0;
  char comps[MAX_COMPS][MAX_CMD];
  DIR *dir;
  char cwd[MAX_CMD];
  int len = 0;
  while ((word + len) && *(word + len) != '\0' && *(word + len) != ' ') {
    len += 1;
  }
  getcwd(cwd, MAX_CMD);
  if ((dir = opendir(cwd))) {
    struct dirent *ent = readdir(dir);
    for (int i = 0; i < MAX_COMPS && ent; i++) {
      memcpy(comps[i], ent->d_name, MAX_CMD);
      ent = readdir(dir);
    }
  };
  for (int i = 0; i < MAX_COMPS; i++) {
    if (comps[i] && memcmp(comps[i], word, len) == 0) {
      strcpy(comp, (comps[i] + len));
    }
  }

  return strlen(comp);
}

void handle_keys(char *input, char history[MAX_HISTORY][MAX_INPUT],
                 int history_idx) {

  int original_history_idx = history_idx;
  int key = 0;
  int end = 0;
  int index = 0;
  input[0] = 0;
  while (end < MAX_INPUT) {
    switch (key = getch()) {
    case 27: { // Arrow Keys emit 27 and 91
      getch(); // we dont need the 91
      key = getch();
      if (key > 'B') {
        if (key == 'C' && index < end) { // Right
          index += 1;
          printf("\033[%c", key);
        } else if (key == 'D' && index > 0) { // Left
          index -= 1;
          printf("\033[%c", key);
        }
      } else if (key <= 'B') {
        strcpy(history[history_idx], input);
        if (key == 'A') { // Up
          history_idx = MOD(history_idx - 1, MAX_HISTORY);
        } else if (key == 'B') { // Down
          history_idx = MOD(history_idx + 1, MAX_HISTORY);
        }
        strcpy(input, history[history_idx]);
        if (index > 0) {
          printf("\033[%iD", index);
        }
        printf("\033[0K%s", input);
        index = strlen(input);
        end = index;
      }
    }
    break;
    case '\t': {
      if (index == end && index != 0) {
        char comp[MAX_CMD];
        char *last_word_start = (input + index);

        for (int i = index;
             i > 0 && (last_word_start - 1) && *(last_word_start - 1) != ' ';
             i--) {
          last_word_start -= 1;
        }
        int len = get_completion(last_word_start, comp);
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
      if (*input != '\0') {
        strcpy(history[original_history_idx], input);
      }
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
