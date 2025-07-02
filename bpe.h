#ifndef BPE_H
#define BPE_H
#include <stdint.h>
#include <stdbool.h>
#define NOB_STRIP_PREFIX
#include "nob.h"

typedef struct{
    uint32_t l,r;
}Pair;

typedef struct {
    Pair *items;
    size_t count;
    size_t capacity;
}Pairs;

bool load_pairs(Pairs *pairs, const char *filename, String_Builder sb);


#endif // BPE_H
