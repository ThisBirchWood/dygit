#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdbool.h>

#define PATH_MAX 4096

unsigned char *read_file(const char *path, size_t *out_len);
int write_file(const char *path, const char *content, size_t content_len);
bool path_exists(const char *path);
bool is_regular_file(const char *path);
bool is_directory(const char *path);
bool path_join(char *out, size_t out_size, const char *a, const char *b);

#endif