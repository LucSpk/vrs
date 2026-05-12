#ifndef TREE_TYPE
#define TREE_TYPE

#include <stddef.h>
#include "entry.h"

typedef struct {
    Entry *entries;
    size_t quantidade;
} Tree;

#endif