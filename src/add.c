// add.c
#include "add.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/evp.h>

#define HEADER_MAX 32

int create_header(
    enum object_type type,
    size_t content_len,
    char *out,
    size_t out_size,
    size_t *header_len
) {
    const char *prefix = NULL;

    switch (type) {
        case OBJ_BLOB:   prefix = "blob";   break;
        case OBJ_COMMIT: prefix = "commit"; break;
        case OBJ_TREE:   prefix = "tree";   break;
        case OBJ_TAG:    prefix = "tag";    break;
    }

    if (!prefix) return -1;

    int n = snprintf(out, out_size, "%s %zu", prefix, content_len);
    if (n < 0 || (size_t)n >= out_size) return -1;

    *header_len = (size_t)n + 1;
    return 0;
}

int hash_object(
    const char *content,
    const size_t content_len,
    enum object_type type,
    char *out
) {
    char header[HEADER_MAX];
    size_t header_len;

    int rc = create_header(type, content_len, header, sizeof header, &header_len);
    if (rc != 0) return 1; 

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) return 1;

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    if (EVP_DigestInit_ex(ctx, EVP_sha1(), NULL) != 1) goto fail;
    if (EVP_DigestUpdate(ctx, header, header_len) != 1) goto fail;
    if (EVP_DigestUpdate(ctx, content, content_len) != 1) goto fail;
    if (EVP_DigestFinal_ex(ctx, digest, &digest_len) != 1) goto fail;

    static const char tbl[] = "0123456789abcdef";
    for (size_t i = 0; i < digest_len; i++) {
        out[i * 2]     = tbl[digest[i] >> 4];     // high nibble
        out[i * 2 + 1] = tbl[digest[i] & 0x0f];   // low nibble
    }

    out[digest_len * 2] = '\0';

    EVP_MD_CTX_free(ctx);
    return 0;

fail:
    EVP_MD_CTX_free(ctx);
    return 1;
}