int shell_loop(char **env, int sess, int input_fd, char* input_str);
void intHandler(int dummy);
int execute_commands(char *input, char **env, char **aliases, int sess, int pipes[8][2]);
