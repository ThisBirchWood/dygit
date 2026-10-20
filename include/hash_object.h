// add.h
#ifndef HASH_OBJECT_H
#define HASH_OBJECT_H

#include <stddef.h>
#include <stdbool.h>

#include "object.h"

typedef struct hash_object_opts {
    enum object_type type;
    bool write_db;
    bool is_stdin;
} hash_object_opts;

int hash_object(
    const char *path,
    const struct hash_object_opts *opts,
    char *hash
);

#endif