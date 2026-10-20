#ifndef OBJECT_H
#define OBJECT_H

#define HASH_RAW_LEN 20

#include <stddef.h>

enum object_type { OBJ_BLOB, OBJ_TREE, OBJ_COMMIT, OBJ_TAG };

struct object{
    enum object_type type;
    unsigned char *content;
    size_t content_len;

    char hash[HASH_RAW_LEN];
};

#endif