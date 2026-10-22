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

int write_file(const char *path, const char *content, size_t content_len) {
    FILE *fp = fopen(path, "wx");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    if (fwrite(content, 1, content_len, fp) != content_len) {
        perror("fwrite");
        fclose(fp);
        return 1;
    }

    if (fclose(fp) != 0) {
        perror("fclose");
        return 1;
    }

    return 0;
}