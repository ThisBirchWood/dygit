// add.h
#ifndef ADD_H
#define ADD_H

#define HASH_RAW_LEN 20

enum object_type { OBJ_BLOB, OBJ_TREE, OBJ_COMMIT, OBJ_TAG };

int obj_hash(
    const char *content, 
    const size_t content_len, 
    enum object_type type, 
    unsigned char hash[HASH_RAW_LEN]
);

int obj_write(
    const char *objects_dir,
    const char *content, 
    const size_t content_len, 
    enum object_type type, 
    unsigned char hash[HASH_RAW_LEN]
);

#endif