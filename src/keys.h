//int getch();
int get_completion(char *line, char *comp);
void handle_keys(char *input, char history[MAX_HISTORY][MAX_INPUT],
                 int history_idx);
int arrow(int index, int end);
