#include "utils.h"

#include "keys.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

struct termios enter_raw_mode() {
  struct termios raw_mode, buffered_mode;
  tcgetattr(STDIN_FILENO, &buffered_mode);
  memcpy(&raw_mode, &buffered_mode, sizeof(struct termios));
  raw_mode.c_oflag &= ~(OPOST);
  raw_mode.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode);
  return buffered_mode;
}

void exit_raw_mode(struct termios buffered_mode) {
  tcsetattr(STDIN_FILENO, TCSANOW, &buffered_mode);
}

/* TODO should return a completion based on input */
int get_completion(char *word, char comp[MAX_CMD]) {
  if (*word == ' ' || *word == 0)
    return 0;

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
    int i = 0;
    while (i < MAX_COMPS && ent) {
      if (memcmp(ent->d_name, word, len) == 0) {
        if (ent->d_namlen > MAX_CMD - 1)
          continue;
        memcpy(comps[i], ent->d_name, ent->d_namlen);
        struct stat st;
        fstatat(dirfd(dir), ent->d_name, &st, 0);
        if (S_ISDIR(st.st_mode)) {
          comps[i][ent->d_namlen] = '/';
          comps[i][ent->d_namlen + 1] = 0;
        }

        i += 1;
      }
      ent = readdir(dir);
    }

    if (i > 1) {
      printf("\n\033[s\r");
      while (i > 0) {
        printf("%s ", comps[i - 1]);
        i -= 1;
      }
      printf("\033[u\033[A");
    } else if (i == 1) {
      strcpy(comp, (comps[0] + len));
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
  struct termios buffered_mode = enter_raw_mode();
  while (end < MAX_INPUT) {
    switch (key = getchar()) {
    case 27: {   // Arrow Keys emit 27 and 91
      getchar(); // we dont need the 91
      key = getchar();
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
    } break;
    case '\t': {
      if (index == end && index != 0 && *(input + index - 1) != 0 &&
          *(input + index - 1) != ' ') {
        char comp[MAX_CMD];
        char *last_word_start = (input + index);

        for (int i = index;
             i > 0 && (last_word_start - 1) && *(last_word_start - 1) != ' ';
             i--) {
          last_word_start -= 1;
        }
        int len = 0;
        if ((len = get_completion(last_word_start, comp))) {
          if ((len + end) < MAX_INPUT) {
            index = end + len;
          } else {
            index = MAX_INPUT - end;
          }
          memcpy((input + end), comp, index);
          end = index;
          printf("%.*s", index, comp);
        }
      }
    } break;
    case '\n':
    case 4: // eot
      input[end] = '\0';
      if (*input != '\0') {
        strcpy(history[original_history_idx], input);
      }
      printf("\r\n");
      exit_raw_mode(buffered_mode);
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
  exit_raw_mode(buffered_mode);
}
