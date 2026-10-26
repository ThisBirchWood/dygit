#include "repository.h"
#include "file.h"

int open_repository(struct repository *repo, const char *start_path) {
    if (!path_join(repo->dygit_dir, sizeof repo->dygit_dir, start_path, "dygit") || !is_directory(repo->dygit_dir)) 
        return 1;

    if (!path_join(repo->object_dir, sizeof repo->object_dir, repo->dygit_dir, "objects") || !is_directory(repo->object_dir))
        return 1;

    return 0;
}