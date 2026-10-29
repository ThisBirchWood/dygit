#ifndef DB_H
#define DB_H

#include "object.h"
#include "repository.h"

int persist_object_db(
    struct object *obj,
    struct repository *repo
);

#endif