#include <stdbool.h>
#include "bpe.h"

bool load_pairs(Pairs *pairs, const char *filename, String_Builder sb){
    if (!read_entire_file(filename, &sb)) return false;
    if (sb.count % sizeof(*pairs -> items) != 0) {
        fprintf(stderr, "ERROR : File size should be divisible by %zu\n", sizeof(Pair));
        return false;
    }

    Pair *items = (void*)sb.items;
    size_t items_count = sb.count / sizeof(*pairs->items);
    for (size_t i = 0 ; i < items_count ; i++){
        da_append(pairs, items[i]);
    }
    return true;
}


