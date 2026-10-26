#ifndef REPOSITORY_H
#define REPOSITORY_H

struct repository {
    char *dygit_dir; //absolute path to .dygit
    char *object_dir; //relative path to .dygit/objects
};

#endif