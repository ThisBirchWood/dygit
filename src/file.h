#ifndef FILE_H
#define FILE_H

#include <stdio.h>

unsigned char *read_file(const char *path, size_t *out_len);
int write_file(const char *path, const char *content, size_t content_len);

#endif