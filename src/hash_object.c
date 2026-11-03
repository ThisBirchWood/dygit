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
#include <getopt.h>
#include <string.h>

static int parse_type(const char *s, enum object_type *out) {
    if (strcmp(s, "blob") == 0)        *out = OBJ_BLOB;
    else if (strcmp(s, "tree") == 0)   *out = OBJ_TREE;
    else if (strcmp(s, "commit") == 0) *out = OBJ_COMMIT;
    else return -1;
    return 0;
}

int hash_object_cmd(int argc, char *argv[]) {
    static const struct option long_opts[] = {
        { "type",  required_argument, NULL, 't' },
        { "write", no_argument,       NULL, 'w' },
        { NULL,    0,                 NULL, 0   }
    };

    // set defaults
    struct hash_object_opts opts = {
        //.is_stdin = false,
        .write_db = false,
        .type = OBJ_BLOB,
    };

    optind = 1;
    int c;
    while ((c = getopt_long(argc, argv, "t:w", long_opts, NULL)) != -1) {
        switch (c) {
            case 'w': 
                opts.write_db = true;
                break;
            case 't':
                if (parse_type(optarg, &opts.type) != 0) {
                    fprintf(stderr, "invalid object type: %s\n", optarg);
                    return 1;
                }
                break;
            default:
                return 1;  
        }
    }

    if (argc - optind != 1) {
        fprintf(stderr, "usage: dygit hash-object [-t <type>] [-w] <file>\n");
        return 1;
    }
    const char *path = argv[optind];

    char hash[41];
    if (hash_object(path, &opts, hash) != 0) {
        fprintf(stderr, "hash-object: failed to hash '%s'\n", path);
        return 1;
    }

    puts(hash);
    return 0;
}

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

        write_db(
            &obj,
            &repo
        );
    }

    free(file_contents);
    return 0;
}