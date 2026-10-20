#include <stdio.h>
#include <stdlib.h>

// Caller must free
unsigned char *read_file(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    if (fseek(f, 0, SEEK_END) != 0) goto fail;
    long size = ftell(f);
    if (size < 0) goto fail;
    rewind(f);

    unsigned char *buf = malloc(size ? size : 1);
    if (!buf) goto fail;

    size_t n = fread(buf, 1, size, f);
    if (n != (size_t)size) { free(buf); goto fail; }

    fclose(f);
    *out_len = n;
    return buf;

fail:
    fclose(f);
    return NULL;
}