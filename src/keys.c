#include "utils.h"

#include "keys.h"
#include "prompt.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

int get_completion(char *word, char comp[MAX_CMD]) {
  comp[0] = 0;
  char comps[MAX_COMPS][MAX_CMD];
  DIR *dir;
  char path[MAX_PATH + 2] = {'.', '/', 0};
  int len = 0;
  char *subdirs_end = word;
  while ((word + len) && *(word + len) != '\0' && *(word + len) != ' ') {
    len += 1;
    if (*(word + len) == '/') {
      subdirs_end = word + len + 1;
    }
  }

  if (subdirs_end != word) {
    for (int i = 0; i < MAX_PATH && i <= len && (word + i) < subdirs_end; i++) {
      path[i + 2] = word[i];
      path[i + 3] = 0;
    }
  }

  if ((dir = opendir(path))) {
    struct dirent *ent = readdir(dir);
    int i = 0;
    while (i < MAX_COMPS && ent) {
      if (memcmp(ent->d_name, subdirs_end, len - (subdirs_end - word)) == 0 ||
          *subdirs_end == 0 || *subdirs_end == ' ') {
        if (ent->d_namlen > MAX_CMD - 1)
          continue;
        strncpy(comps[i], ent->d_name, MAX_CMD);
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
      display_suggestions(comps, i, -1);
    } else if (i == 1) {
      strcpy(comp, (comps[0] + len - (subdirs_end - word)));
    }
  }

  return strlen(comp);
}

void handle_keys(char *input, char history[MAX_HISTORY][MAX_INPUT],
                 int history_idx) {

  // sets the terminal into raw mode and saves the settings of
  // the current mode (probably buffered) in buffered_mode
  struct termios raw_mode, buffered_mode;
  tcgetattr(STDIN_FILENO, &buffered_mode);
  memcpy(&raw_mode, &buffered_mode, sizeof(struct termios));
  raw_mode.c_oflag &= ~(OPOST);
  raw_mode.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode);

  int original_history_idx = history_idx;
  int key = 0;
  int end = 0;
  int index = 0;
  input[0] = 0;
  while (end < MAX_INPUT) {
    key = getchar();

    if (key == 27) { // Arrow Keys emit 27 and 91
      getchar();     // we dont need the 91
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
    } else if (key == '\t') {
      if (index == end) {
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
    } else if (key == '\n' || key == 4) { // 4 is the EOT code
      input[end] = '\0';
      if (*input != '\0') {
        strcpy(history[original_history_idx], input);
      }
      printf("\r\n\033[0J");
      tcsetattr(STDIN_FILENO, TCSANOW, &buffered_mode);
      return;
    } else if (key == 8 || key == 127) { // backspace and delete
      if (index > 0) {
        index -= 1;
        // move content of input to the left
        memcpy((input + index), (input + index + 1), end - index);
        end -= 1;
        input[end] = 0;
        printf("\033[D\033[s\033[0J%s\033[u", &input[index]);
      }
    } else {
      // like deleting in reverse
      end += 1;
      memcpy((input + index + 1), (input + index), end - index);
      input[index] = key;
      printf("\033[s\033[0J%s\033[u\033[C", &input[index]);
      index += 1;
    }
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &buffered_mode);
}
