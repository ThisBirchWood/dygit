// hash.c
#include "object.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/evp.h>

#define HEADER_MAX 32

static const char HEX[] = "0123456789abcdef";

int create_header(
    struct object *object,
    char *out,
    size_t out_size,
    size_t *header_len
) {
    const char *prefix = NULL;

    switch (object->type) {
        case OBJ_BLOB:   prefix = "blob";   break;
        case OBJ_COMMIT: prefix = "commit"; break;
        case OBJ_TREE:   prefix = "tree";   break;
        case OBJ_TAG:    prefix = "tag";    break;
    }

    if (!prefix) return -1;

    int n = snprintf(out, out_size, "%s %zu", prefix, object->content_len);
    if (n < 0 || (size_t)n >= out_size) return -1;

    *header_len = (size_t)n + 1;
    return 0;
}

void oid_to_hex(const unsigned char *oid, char *out) {
    for (size_t i = 0; i < HASH_RAW_LEN; i++) {
        out[2 * i]     = HEX[oid[i] >> 4];
        out[2 * i + 1] = HEX[oid[i] & 0x0F];
    }
    out[2 * HASH_RAW_LEN] = '\0';
}

int get_object_hash(
    struct object *object,
    unsigned char out[HASH_RAW_LEN]
) {
    char header[HEADER_MAX];
    size_t header_len;

    int rc = create_header(object, header, sizeof header, &header_len);
    if (rc != 0) return 1; 

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) return 1;

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    if (EVP_DigestInit_ex(ctx, EVP_sha1(), NULL) != 1) goto fail;
    if (EVP_DigestUpdate(ctx, header, header_len) != 1) goto fail;
    if (EVP_DigestUpdate(ctx, object->content, object->content_len) != 1) goto fail;
    if (EVP_DigestFinal_ex(ctx, digest, &digest_len) != 1) goto fail;

    if (digest_len != HASH_RAW_LEN) goto fail;
    memcpy(out, digest, HASH_RAW_LEN);

    EVP_MD_CTX_free(ctx);
    return 0;

fail:
    EVP_MD_CTX_free(ctx);
    return 1;
}