#include "huffman.h"
#include <stdlib.h>
#include <string.h>

static void build_codes(const huffman_node_t *n, uint32_t code, int len, huffman_code_t *codes) {
    if (!n) return;
    if (n->is_leaf) {
        codes[n->symbol].code = code;
        codes[n->symbol].len = len;
        return;
    }
    build_codes(n->left, code << 1, len + 1, codes);
    build_codes(n->right, (code << 1) | 1, len + 1, codes);
}

void huffman_build(const unsigned *freq, int num_symbols, huffman_ctx_t *ctx) {
    huffman_node_t *nodes[2 * HUFFMAN_MAX_SYMBOLS];
    int n = 0;
    int i;

    memset(ctx->codes, 0, sizeof(ctx->codes));
    for (i = 0; i < num_symbols; i++) {
        if (freq[i] == 0) continue;
        huffman_node_t *leaf = calloc(1, sizeof(huffman_node_t));
        if (!leaf) { ctx->root = NULL; return; }
        leaf->is_leaf = 1;
        leaf->symbol = (unsigned char)i;
        leaf->min_sym = (unsigned char)i;
        leaf->freq = freq[i];
        nodes[n++] = leaf;
    }

    while (n > 1) {
        int a = 0, b = 1;
        unsigned fa = nodes[0]->freq, fb = nodes[1]->freq;
        unsigned ma = nodes[0]->min_sym, mb = nodes[1]->min_sym;
        if (fb < fa || (fb == fa && mb < ma)) { a = 1; b = 0; fa = nodes[1]->freq; fb = nodes[0]->freq; ma = nodes[1]->min_sym; mb = nodes[0]->min_sym; }
        for (i = 2; i < n; i++) {
            unsigned fi = nodes[i]->freq, mi = nodes[i]->min_sym;
            if (fi < fa || (fi == fa && mi < ma)) { b = a; a = i; fb = fa; fa = fi; mb = ma; ma = mi; }
            else if (fi < fb || (fi == fb && mi < mb)) { b = i; fb = fi; mb = mi; }
        }
        huffman_node_t *parent = calloc(1, sizeof(huffman_node_t));
        if (!parent) { ctx->root = NULL; return; }
        parent->is_leaf = 0;
        parent->min_sym = (nodes[a]->min_sym < nodes[b]->min_sym) ? nodes[a]->min_sym : nodes[b]->min_sym;
        parent->left = nodes[a];
        parent->right = nodes[b];
        parent->freq = nodes[a]->freq + nodes[b]->freq;
        nodes[a] = parent;
        nodes[b] = nodes[n - 1];
        n--;
    }
    ctx->root = n == 1 ? nodes[0] : NULL;
    if (ctx->root) build_codes(ctx->root, 0, 0, ctx->codes);
}

static void free_tree(huffman_node_t *n) {
    if (!n) return;
    if (!n->is_leaf) {
        free_tree(n->left);
        free_tree(n->right);
    }
    free(n);
}

void huffman_free(huffman_ctx_t *ctx) {
    if (ctx && ctx->root) free_tree(ctx->root);
    ctx->root = NULL;
}

int huffman_encode(const huffman_ctx_t *ctx, const unsigned char *str, size_t len,
                   unsigned char *out, size_t out_size, size_t *num_bits) {
    size_t bit_pos = 0;
    size_t byte_pos = 0;
    int b;
    if (!ctx->root || !out || !num_bits) return -1;
    *num_bits = 0;
    for (size_t i = 0; i <= len; i++) {
        unsigned char sym = (i < len) ? str[i] : HUFFMAN_END_SYMBOL;
        uint32_t code = ctx->codes[sym].code;
        int code_len = ctx->codes[sym].len;
        if (code_len == 0 && sym != HUFFMAN_END_SYMBOL) return -1;
        for (b = code_len - 1; b >= 0; b--) {
            int bit = (code >> b) & 1;
            if (bit) out[byte_pos] |= (1u << (7 - bit_pos));
            bit_pos++;
            (*num_bits)++;
            if (bit_pos == 8) { bit_pos = 0; byte_pos++; if (byte_pos >= out_size) return -1; }
        }
    }
    if (bit_pos != 0) byte_pos++;
    return (int)byte_pos;
}

int huffman_decode_one(const huffman_ctx_t *ctx, const unsigned char *in, size_t in_bytes,
                       size_t *bit_pos, size_t total_bits,
                       unsigned char *out, size_t out_size, size_t *out_len) {
    const huffman_node_t *cur = ctx->root;
    *out_len = 0;
    if (!ctx->root || !in || !out || !bit_pos) return -1;
    while (*bit_pos < total_bits && *out_len < out_size) {
        if (!cur->is_leaf) {
            size_t byte_idx = *bit_pos / 8;
            int bit_idx = 7 - (*bit_pos % 8);
            if (byte_idx >= in_bytes) return -1;
            int bit = (in[byte_idx] >> bit_idx) & 1;
            (*bit_pos)++;
            cur = bit ? cur->right : cur->left;
            if (!cur) return -1;
            continue;
        }
        if (cur->symbol == HUFFMAN_END_SYMBOL) return 0;
        out[(*out_len)++] = cur->symbol;
        cur = ctx->root;
    }
    return -1;
}
