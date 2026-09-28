// init.c
#include "init.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define DYGIT_PATH_MAX 4096

static int create_empty_file(const char *path) {
    FILE *f = fopen(path, "wx");
    if (f == NULL) {
        return -1;
    }
    return fclose(f);
}

static void partial_cleanup(
    const char *dygit_path, 
    const char *objects_path, 
    const char *index_path, 
    const char *head_file
) {
    unlink(index_path);
    unlink(head_file);
    rmdir(objects_path);
    rmdir(dygit_path);
}

static int join_path(char *out, size_t max_size, const char *base, const char *suffix) {
    int n = snprintf(out, max_size, "%s%s", base, suffix);
    if (n < 0 || (size_t)n >= max_size) {
        return -1;
    }
    return 0;
}

int run_init(const char *dir) {
    char dygit_path[DYGIT_PATH_MAX];
    char objects_path[DYGIT_PATH_MAX];
    char index_path[DYGIT_PATH_MAX];
    char head_path[DYGIT_PATH_MAX];

    if (join_path(dygit_path, DYGIT_PATH_MAX, dir, "/.dygit") != 0 ||
        join_path(objects_path, DYGIT_PATH_MAX, dygit_path, "/objects") != 0 ||
        join_path(index_path, DYGIT_PATH_MAX, dygit_path, "/index") != 0 ||
        join_path(head_path, DYGIT_PATH_MAX, dygit_path, "/head") != 0
    ) {
        fprintf(stderr, "dygit: path too long\n");
        return 1;
    }


    if (mkdir(dygit_path, 0755) == -1) {
        if (errno == EEXIST) {
            fprintf(stderr, "dygit: repository already exists\n");
        } else {
            perror("Error creating Objects directory");
        }

        return 1;
    }

    if (mkdir(objects_path, 0755) == -1) {
        perror("dygit: creating objects directory failed");
        goto fail;
    }
    
    if (create_empty_file(index_path) != 0) {
        perror("dygit: creating index failed");
        goto fail;
    }

    if (create_empty_file(head_path) != 0) {
        perror("dygit: creating head failed");
        goto fail;
    }

    return 0;

fail:
    partial_cleanup(dygit_path, objects_path, index_path, head_path);
    return 1;
}