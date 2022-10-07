int run(struct command cmd, char **env, char* aliases[MAX_ALIASES][2], int sess, int pipes[8][2] );
void run_cmd(struct command cmd, char **env, char* aliases[MAX_ALIASES][2], int pipes[8][2]);
void run_cd(struct command cmd);
void echo(struct command cmd);
void run_alias(struct command cmd, char* aliases[MAX_ALIASES][2]);
