int getch();
int get_completion(char *line, char *comp);
void handle_keys(char *input, char history[MAX_HISTORY + 1][MAX_INPUT + 1],
                 int history_idx);
int arrow(int index, int end);
