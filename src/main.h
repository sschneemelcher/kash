int shell_loop(char **env, int sess, int input_fd, char* input_str);
int execute_commands(char *input, char **env, char *aliases[MAX_ALIASES][2], int sess, int pipes[8][2]);
