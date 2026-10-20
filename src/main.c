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
        if (argc < 3) return 1;

        struct hash_object_opts opts = {
            .type = OBJ_BLOB,
            .write_db = false,
            .is_stdin = false,
        };

        char hash[41];

        hash_object(
            argv[2],
            &opts,
            hash
        );

        printf("%s\n", hash);
    } else {
        printf("Usage: %s <subcommand>\n", argv[0]);
        return 1;
    }

    return 0;
}