#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "init.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <subcommand>\n", argv[0]);
        return 1;
    }

    char *command = argv[1];

    if (strcmp(command, "init") == 0) {
        run_init("./");
    } else {
        printf("Usage: %s <subcommand>\n", argv[0]);
        return 1;
    }

    return 0;
}