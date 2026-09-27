// init.c
#include "init.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define DYGIT_PATH_MAX 4096

bool directory_exists(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return false;
    }
    return S_ISDIR(st.st_mode);
}

static int join_path(char *out, size_t max_size, const char *base, const char *suffix) {
    int n = snprintf(out, max_size, "%s%s", base, suffix);
    if (n < 0 || (size_t)n >= max_size) {
        return -1;
    }
    return 0;
}

int run_init(const char *dir) {
    char path[DYGIT_PATH_MAX];
    char dygit_path[DYGIT_PATH_MAX];

    if (join_path(dygit_path, DYGIT_PATH_MAX, dir, "/.dygit") != 0) {
        perror("Dygit path too long");
        return 1;
    }

    if (directory_exists(dygit_path)) {
        perror("DyGit directory already exists!");
        return 1;
    }

    if (mkdir(dygit_path, 0755) == -1) {
        perror("DyGit directory creation failed");
        return 1;
    }

    // Create "objects" directory
    if (join_path(path, DYGIT_PATH_MAX, dygit_path, "/objects") != 0) {
        perror("Objects path too long");
        rmdir(dygit_path);
        return 1;
    }

    if (mkdir(path, 0755) == -1) {
        perror("Error creating Objects directory");
        return 1;
    }
    
    // Create index file
    // Stores staging area information
    if (join_path(path, DYGIT_PATH_MAX, dygit_path, "/index") != 0) {
        perror("Index file path too long");
        rmdir(dygit_path);
        return 1;
    }

    FILE *index = fopen(path, "w");
    if (index == NULL) {
        perror("Index file creation failed");
        rmdir(dygit_path);
        return 1;
    }
    fclose(index);
    
    // Create head file
    // Stores current branch
    if (join_path(path, DYGIT_PATH_MAX, dygit_path, "/head") != 0) {
        perror("Head file path too long");
        rmdir(dygit_path);
        return 1;
    }

    FILE *head = fopen(path, "w");
    if (head == NULL) {
        perror("Head file creation failed");
        rmdir(dygit_path);
        return 1;
    }
    fclose(head);

    return 0;
}