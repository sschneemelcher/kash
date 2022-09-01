#include <stdio.h>
#include "msh.h"

const int MAX_INPUT = 1024;

int main(int argc, char** argv) {
    shell_loop();
    return 0;
}

int shell_loop() {
    char* prompt = "> ";
    char input[MAX_INPUT] = { 0 };

    do {
        printf("%s", prompt);
        fgets(input, MAX_INPUT, stdin);
        // just echo the input back for now
        printf("%s", input);

    } while (!(input[0] == 'e' && input[1] == 'x' && input[2] == 'i' && input[3] == 't'));

    return 0;
}
