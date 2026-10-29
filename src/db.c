#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

#include "file.h"
#include "object.h"
#include "repository.h"

int persist_object_db(
    struct object *obj,
    struct repository *repo
) {
    char dir_path[PATH_MAX];
    char file_path[PATH_MAX];
    
    int n = snprintf(dir_path, sizeof dir_path, "%s/%.2s", repo->object_dir, obj->hash);
    if (n < 0 || (size_t)n >= sizeof dir_path) return 1;

    n = snprintf(file_path, sizeof file_path, "%s/%.2s/%s", repo->object_dir, obj->hash, obj->hash + 2);
    if (n < 0 || (size_t)n >= sizeof file_path) return 1;

    if (mkdir(dir_path, 0755) == -1 && errno != EEXIST) {
        perror("mkdir");
        return 1;
    };

    if (write_file(
        file_path,
        obj->content,
        obj->content_len
    ) != 0) {
        return 1;
    };

    return 0;
}