#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "huffman.h"
#include "receiver.h"

#define NUM_COMMANDS 8

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

int main(void) {
    unsigned freq[HUFFMAN_MAX_SYMBOLS];
    huffman_ctx_t ctx;
    uint32_t total_bits;
    unsigned char *payload = NULL;
    size_t payload_size;
    size_t bit_pos = 0;
    unsigned char decoded[64];
    size_t dec_len;
    int i;

    if (read(STDIN_FILENO, &total_bits, 4) != 4) return 1;
    payload_size = (total_bits + 7) / 8;
    payload = malloc(payload_size);
    if (!payload) return 1;
    if (!payload) return 1;
    if (read(STDIN_FILENO, payload, payload_size) != (ssize_t)payload_size) {
        free(payload); return 1;
    }
    total_bits = payload_size * 8;

    build_freq(freq);
    huffman_build(freq, HUFFMAN_MAX_SYMBOLS, &ctx);
    if (!ctx.root) { free(payload); return 1; }

    for (i = 0; i < NUM_COMMANDS; i++) {
        if (huffman_decode_one(&ctx, payload, payload_size, &bit_pos, total_bits,
                              decoded, sizeof(decoded), &dec_len) != 0)
            break;
        decoded[dec_len] = '\0';
        printf("binary translated to \"%s\"\n", (char *)decoded);
        receiver_handle((char *)decoded);
    }
    huffman_free(&ctx);
    free(payload);
    return 0;
}
