#include "hash_object.h"
#include "hash.h"
#include "object.h"
#include "file.h"

#include <stdlib.h>

int hash_object(
    const char *path,
    const struct hash_object_opts *opts,
    char *hash
) {
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

    free(file_contents);
    return 0;
}