#ifndef HASH_INTERNAL_H
#define HASH_INTERNAL_H
#include "object.h"

int create_header(
    struct object *object,
    char *out,
    size_t out_size,
    size_t *header_len
);

void oid_to_hex(
    const unsigned char *oid, 
    char *out
);

int get_object_hash(
    struct object *object,
    unsigned char out[HASH_RAW_LEN]
);


#endif