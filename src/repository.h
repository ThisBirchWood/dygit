#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "file.h"

struct repository {
    char dygit_dir[PATH_MAX];
    char object_dir[PATH_MAX];
};

int open_repository(struct repository *repo, const char *start_path);

#endif