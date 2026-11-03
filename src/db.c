#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

#include "file.h"
#include "object.h"
#include "repository.h"
#include "hash.h"

int write_db(
    struct object *obj,
    struct repository *repo
) {
    char dir_path[PATH_MAX];
    char file_path[PATH_MAX];
    char header[HEADER_MAX];
    size_t header_len;

    int rc = create_header(obj, header, sizeof header, &header_len);
    if (rc != 0) return 1; 
    
    int n = snprintf(dir_path, sizeof dir_path, "%s/%.2s", repo->object_dir, obj->hash);
    if (n < 0 || (size_t)n >= sizeof dir_path) return 1;

    n = snprintf(file_path, sizeof file_path, "%s/%.2s/%s", repo->object_dir, obj->hash, obj->hash + 2);
    if (n < 0 || (size_t)n >= sizeof file_path) return 1;

    if (mkdir(dir_path, 0755) == -1 && errno != EEXIST) {
        perror("mkdir");
        return 1;
    }

    if (path_exists(file_path)) {
        return 1;
    }

    // Write Header, then Contents
    FILE *fp = fopen(file_path, "ab");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    if (fwrite(header, 1, header_len, fp) != header_len) {
        perror("fwrite");
        fclose(fp);
        return 1;
    }

    if (fwrite(obj->content, 1, obj->content_len, fp) != obj->content_len) {
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