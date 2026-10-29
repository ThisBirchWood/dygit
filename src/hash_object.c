#include "hash_object.h"
#include "hash.h"
#include "object.h"
#include "file.h"
#include "db.h"
#include "repository.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int hash_object(
    const char *path,
    const struct hash_object_opts *opts,
    char *hash
) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof cwd) == NULL) {
        perror("getcwd");
        return 1;
    }

    size_t file_size = 0;
    unsigned char *file_contents = read_file(path, &file_size);
    if (file_contents == NULL) return 1;

    struct object obj = {
        .type = opts->type,
        .content = file_contents,
        .content_len = file_size,
    };

    unsigned char oid[HASH_RAW_LEN];

    if (get_object_hash(&obj, oid) != 0) {
        fprintf(stderr, "failed to hash object\n");
        free(file_contents);
        return 1;
    }

    oid_to_hex(oid, hash);
    snprintf(obj.hash, sizeof obj.hash, "%s", hash);

    if (opts->write_db) {
        struct repository repo = {0};
        open_repository(
            &repo,
            cwd
        );

        persist_object_db(
            &obj,
            &repo
        );
    }

    free(file_contents);
    return 0;
}