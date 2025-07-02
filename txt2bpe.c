#include <stdio.h>
#include <string.h>
#include <assert.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include <stdint.h>
#define NOB_IMPLEMENTATION
#include "bpe.h"

typedef struct {
    Pair key;
    size_t value;
} Freq;

typedef struct {
    uint32_t *items;
    size_t count;
    size_t capacity;
} Tokens;

typedef struct {
    Freq *items;
    size_t count;
    size_t capacity;
} Freqs;

void render_tokens(Pairs pairs, Tokens tokens) {
    printf("Tokens: ");
    for (size_t i = 0; i < tokens.count; i++) {
        uint32_t token = tokens.items[i];
        assert(token < pairs.count);
        if (pairs.items[token].l == token) {
            printf("%c", token);
        } else {
            printf("[%u]", token);
        }
    }
    printf("\n");
}


#define swap(Type, a, b) \
    do { \
        Type temp = (a); \
        (a) = (b); \
        (b) = temp; \
    } while (0)

bool dump_pairs(Pairs pairs, const char *filename) {
    return write_entire_file(filename, pairs.items, pairs.count * sizeof(Pair));
}

void usage(const char *program_name) {
    fprintf(stderr, "Usage: %s <input_text> <output_file>\n", program_name);
    fprintf(stderr, "  input_text: The text string to compress\n");
    fprintf(stderr, "  output_file: Output file for compressed pairs\n");
}

int main(int argc, char **argv) {
    printf("Starting BPE compression\n");

    const char *program_name = shift(argv, argc);

    if (argc <= 0) {
        usage(program_name);
        fprintf(stderr, "Error: No input text provided.\n");
        return 1;
    }

    const char *input_text = shift(argv, argc);

    if (argc <= 0) {
        usage(program_name);
        fprintf(stderr, "Error: No output file provided.\n");
        return 1;
    }

    const char *output_file = shift(argv, argc);

    int len = strlen(input_text);
    printf("Input text: \"%s\" (length: %d)\n", input_text, len);

    Freq *freq = NULL;
    Tokens tokens_in = {0};
    Pairs pairs = {0};
    Tokens tokens_out = {0};

    // Initialize pairs with single characters
    for (uint32_t i = 0; i < 255; i++) {
        da_append(&pairs, ((Pair){.l = i, .r = 0}));
    }

    // Initialize tokens_in with single characters
    for (int i = 0; i < len; i++) {
        da_append(&tokens_in, input_text[i]);
    }

    printf("Initialization complete. Starting compression loop...\n");

    int iteration = 0;

    // compression
    for (;;) {
        iteration++;
        printf("Iteration %d:\n", iteration);

        render_tokens(pairs, tokens_in);

        // Clear frequency map for this iteration
        hmfree(freq);
        freq = NULL;
        printf("Counting pair frequencies\n");

        for (size_t i = 0; i < tokens_in.count - 1; i++) {
            Pair pair = {
                .l = tokens_in.items[i],
                .r = tokens_in.items[i + 1]
            };
            ptrdiff_t j = hmgeti(freq, pair);
            if (j < 0) hmput(freq, pair, 1);
            else freq[j].value += 1;
        }

        // Check if we have any pairs
        if (hmlen(freq) == 0) {
            printf("No pairs found. Stopping.\n");
            break;
        }

        printf("Finding most frequent pair...\n");
        ptrdiff_t max_index = 0;
        for (size_t i = 1; i < hmlen(freq); i++) {
            if (freq[i].value > freq[max_index].value) {
                max_index = i;
            }
        }

        printf("Most frequent pair: (%u, %u) occurs %zu times\n",
               freq[max_index].key.l, freq[max_index].key.r, freq[max_index].value);

        if (freq[max_index].value <= 1) {
            printf("No pair occurs more than once. Stopping.\n");
            break;
        }

        da_append(&pairs, freq[max_index].key);
        printf("Added new token %zu for pair (%u, %u)\n",
               pairs.count - 1, freq[max_index].key.l, freq[max_index].key.r);

        printf("Replacing pairs in token sequence...\n");
        tokens_out.count = 0;
        for (size_t i = 0; i < tokens_in.count;) {
            if (i + 1 >= tokens_in.count) {
                da_append(&tokens_out, tokens_in.items[i]);
                i += 1;
            } else {
                Pair pair = {
                    .l = tokens_in.items[i],
                    .r = tokens_in.items[i + 1]
                };
                if (memcmp(&pair, &freq[max_index].key, sizeof(Pair)) == 0) {
                    da_append(&tokens_out, pairs.count - 1);
                    i += 2;
                } else {
                    da_append(&tokens_out, tokens_in.items[i]);
                    i += 1;
                }
            }
        }

        swap(Tokens, tokens_in, tokens_out);
    }

    printf("\n Compression complete after %d iterations!\n", iteration);
    printf(" Saving pairs to '%s'...\n", output_file);

    if (!dump_pairs(pairs, output_file)) {
        printf("Failed to save pairs!\n");
        return 1;
    }

    printf("Successfully saved %zu pairs to '%s'\n", pairs.count, output_file);
    printf("Final compressed tokens: ");
    render_tokens(pairs, tokens_in);

    return 0;
}
