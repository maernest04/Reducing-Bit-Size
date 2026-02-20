#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "huffman.h"

#define NUM_COMMANDS 8
#define FIXED_BITS_PER_CHAR 7

static const char *commands[] = {
    "Pltog", "Plstat", "Plmode", "Pltarg",
    "Plkwlm", "Plinit", "Pltqcm", "Plclmp"
};

static void build_freq(unsigned *freq) {
    int i, j;
    memset(freq, 0, HUFFMAN_MAX_SYMBOLS * sizeof(unsigned));
    freq[HUFFMAN_END_SYMBOL] = NUM_COMMANDS;
    for (i = 0; i < NUM_COMMANDS; i++) {
        size_t len = strlen(commands[i]);
        for (j = 0; j < (int)len; j++)
            freq[(unsigned char)commands[i][j]]++;
    }
}

static int append_encoded_bits(const huffman_ctx_t *ctx, const unsigned char *str, size_t len,
                              unsigned char *stream, size_t stream_size, size_t *bit_pos) {
    size_t i;
    int b;
    for (i = 0; i <= len; i++) {
        unsigned char sym = (i < len) ? str[i] : HUFFMAN_END_SYMBOL;
        uint32_t code = ctx->codes[sym].code;
        int code_len = ctx->codes[sym].len;
        for (b = code_len - 1; b >= 0; b--) {
            size_t byte_idx = *bit_pos / 8;
            int bit_idx = 7 - (int)(*bit_pos % 8);
            if (byte_idx >= stream_size) return -1;
            if ((code >> b) & 1) stream[byte_idx] |= (1u << bit_idx);
            (*bit_pos)++;
        }
    }
    return 0;
}

int main(void) {
    unsigned freq[HUFFMAN_MAX_SYMBOLS];
    huffman_ctx_t ctx;
    size_t total_fixed = 0, total_huffman = 0;
    size_t all_bits = 0;
    unsigned char *stream = NULL;
    size_t stream_bits = 0;
    size_t bit_pos = 0;
    int i;

    build_freq(freq);
    huffman_build(freq, HUFFMAN_MAX_SYMBOLS, &ctx);
    if (!ctx.root) { fprintf(stderr, "huffman_build failed\n"); return 1; }

    for (i = 0; i < NUM_COMMANDS; i++) {
        const char *cmd = commands[i];
        size_t len = strlen(cmd);
        size_t fixed_bits = len * FIXED_BITS_PER_CHAR;
        size_t huff_bits = 0;
        size_t j;
        for (j = 0; j <= len; j++) {
            unsigned char sym = (j < len) ? (unsigned char)cmd[j] : HUFFMAN_END_SYMBOL;
            huff_bits += (size_t)ctx.codes[sym].len;
        }
        total_fixed += fixed_bits;
        total_huffman += huff_bits;
        fprintf(stderr, "Sending \"%s\"  fixed (7-bit): %zu bits  Huffman: %zu bits\n",
                cmd, fixed_bits, huff_bits);
        all_bits += huff_bits;
    }

    stream_bits = all_bits;
    {
        size_t stream_bytes = (stream_bits + 7) / 8 + 8;
        stream = calloc(1, stream_bytes);
        if (!stream) { huffman_free(&ctx); return 1; }
        bit_pos = 0;
        for (i = 0; i < NUM_COMMANDS; i++) {
            const char *cmd = commands[i];
            size_t len = strlen(cmd);
            if (append_encoded_bits(&ctx, (const unsigned char *)cmd, len, stream, stream_bytes, &bit_pos) != 0) {
                huffman_free(&ctx); free(stream); return 1;
            }
        }
    }

    fprintf(stderr, "---\nTotal: fixed (7-bit) would use %zu bits, Huffman actually used %zu bits\n",
            total_fixed, total_huffman);

    {
        uint32_t tb = (uint32_t)stream_bits;
        if (write(STDOUT_FILENO, &tb, 4) != 4) { huffman_free(&ctx); free(stream); return 1; }
        if (write(STDOUT_FILENO, stream, (stream_bits + 7) / 8) != (ssize_t)((stream_bits + 7) / 8)) {
            huffman_free(&ctx); free(stream); return 1;
        }
    }
    huffman_free(&ctx);
    free(stream);
    return 0;
}
