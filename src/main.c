#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "init.h"
#include "hash_object.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <subcommand>\n", argv[0]);
        return 1;
    }

    char *command = argv[1];

    if (strcmp(command, "init") == 0) {
        run_init("./");
    } else if (strcmp(command, "hash-object") == 0) {
        hash_object_cmd(argc - 1, argv + 1);
    } else {
        printf("Usage: %s <subcommand>\n", argv[0]);
        return 1;
    }

    return 0;
}