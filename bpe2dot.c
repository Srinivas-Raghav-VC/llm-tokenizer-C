#include <stdio.h>
#define NOB_IMPLEMENTATION
#include "bpe.h"

void render_dot(Pairs pairs, String_Builder *sb){
    sb_append_cstr(sb, "digraph {\n");
    for (uint32_t token = 0 ; token < pairs.count ; ++token){
        if (token != pairs.items[token].l){
            sb_append_cstr(sb, temp_sprintf(" %u -> %u;\n", token, pairs.items[token].l));
            sb_append_cstr(sb, temp_sprintf(" %u -> %u;\n", token, pairs.items[token].r));
        }
    }
    sb_append_cstr(sb, "}\n");
}

void usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s <input.bpe> <output.dot>\n", program_name);
}

int main(int argc , char **argv){
    const char *program_name = shift(argv,argc);

    if (argc <= 0){
        fprintf(stderr, "Usage: %s <input.bin> <output.dot>\n", program_name);
        fprintf(stderr, "ERROR: No input file specified.\n");
        return 1;
    }

    const char *input_filename = shift(argv,argc);

    if (argc <= 0){
        fprintf(stderr, "Usage: %s <input.bin> <output.dot>\n", program_name);
        fprintf(stderr, "ERROR: No output file specified.\n");
        return 1;
    }

    const char *output_filename = shift(argv,argc);

    Pairs pairs = {0};
    String_Builder sb = {0};

    if(!load_pairs(&pairs, input_filename, sb)) return 1;
    sb.count = 0;
    render_dot(pairs, &sb);
    if(!write_entire_file(output_filename, sb.items, sb.count)) return 1;
    printf("Generated DOT file: %s\n", output_filename);
    return 0;
}
