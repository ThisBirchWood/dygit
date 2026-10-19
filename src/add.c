// add.c
#include "add.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/evp.h>

#define PREFIX_MAX 32


int add_prefix(
    enum object_type type,
    const char *content,
    char *out,
    size_t out_size
) {
    const char *prefix = NULL;

    switch (type) {
        case OBJ_BLOB:   prefix = "blob";   break;
        case OBJ_COMMIT: prefix = "commit"; break;
        case OBJ_TREE:   prefix = "tree";   break;
        case OBJ_TAG:    prefix = "tag";    break;
    }

    if (!prefix) return -1;

    int n = snprintf(out, out_size, "%s/%s", prefix, content);
    if (n < 0 || (size_t)n >= out_size) return -1;

    return 0;
}

int hash_object(
    const char *content,
    const size_t content_len,
    enum object_type type,
    char *out
) {
    size_t p_content_len = PREFIX_MAX + content_len;
    char *p_content = malloc(p_content_len);
    if (!p_content) { return -1; }

    int rc = add_prefix(type, content, p_content, p_content_len);
    if (rc != 0) { 
        free(p_content);
        return -1; 
    }

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;
    if (EVP_Digest(p_content, p_content_len, digest, &digest_len, EVP_sha1(), NULL) != 1)
        return -1;

    static const char tbl[] = "0123456789abcdef";
    for (size_t i = 0; i < digest_len; i++) {
        out[i * 2]     = tbl[digest[i] >> 4];     // high nibble
        out[i * 2 + 1] = tbl[digest[i] & 0x0f];   // low nibble
    }
    
    out[digest_len * 2] = '\0';
    free(p_content);

}