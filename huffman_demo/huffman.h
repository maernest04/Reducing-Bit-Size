#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>
#include <stddef.h>

#define HUFFMAN_END_SYMBOL 255
#define HUFFMAN_MAX_SYMBOLS 256
#define HUFFMAN_MAX_CODE_LEN 32

typedef struct huffman_node huffman_node_t;
struct huffman_node {
    int is_leaf;
    unsigned char symbol;
    unsigned min_sym;
    huffman_node_t *left, *right;
    unsigned freq;
};

typedef struct {
    uint32_t code;
    int len;
} huffman_code_t;

typedef struct {
    huffman_node_t *root;
    huffman_code_t codes[HUFFMAN_MAX_SYMBOLS];
} huffman_ctx_t;

void huffman_build(const unsigned *freq, int num_symbols, huffman_ctx_t *ctx);
void huffman_free(huffman_ctx_t *ctx);
int huffman_encode(const huffman_ctx_t *ctx, const unsigned char *str, size_t len,
                  unsigned char *out, size_t out_size, size_t *num_bits);
int huffman_decode_one(const huffman_ctx_t *ctx, const unsigned char *in, size_t in_bytes,
                      size_t *bit_pos, size_t total_bits,
                      unsigned char *out, size_t out_size, size_t *out_len);

#endif
