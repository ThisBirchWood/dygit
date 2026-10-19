#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "init.h"
#include "add.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <subcommand>\n", argv[0]);
        return 1;
    }

    char *command = argv[1];

    if (strcmp(command, "init") == 0) {
        run_init("./");
    } else if (strcmp(command, "hash-object") == 0) {
        if (argc < 3) return 1;
        char *hash = malloc(41);
        if (!hash) return 1;
        hash_object(
            argv[2],
            strlen(argv[2]),
            OBJ_BLOB,
            hash
        );
        printf("%s\n", hash);
        free(hash);
    } else {
        printf("Usage: %s <subcommand>\n", argv[0]);
        return 1;
    }

    return 0;
}