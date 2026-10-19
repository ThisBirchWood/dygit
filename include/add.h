// add.h
#ifndef ADD_H
#define ADD_H
#define HASH_RAW_LEN 20

#include <stddef.h>
#include <stdbool.h>

enum object_type { OBJ_BLOB, OBJ_TREE, OBJ_COMMIT, OBJ_TAG };

int hash_object(
    const char *content,
    const size_t content_len,
    enum object_type type,
    char *out
);

#endif