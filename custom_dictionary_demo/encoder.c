#include "encoder.h"

#define ASCII_MIN 32
#define ASCII_MAX 126
#define NUM_CODES (ASCII_MAX - ASCII_MIN + 1)

static int char_to_code(char c) {
    if ((unsigned char)c >= ASCII_MIN && (unsigned char)c <= ASCII_MAX)
        return (unsigned char)c - ASCII_MIN;
    return PAD_CODE;
}

static char code_to_char(int code) {
    if (code >= 0 && code < NUM_CODES)
        return (char)(ASCII_MIN + code);
    return '\0';
}

void encode_string(const char *str, uint32_t *word0, uint32_t *word1) {
    uint32_t w0 = 0, w1 = 0;
    int i;
    for (i = 0; i < ENCODE_MAX_CHARS; i++) {
        int code = (str && str[i]) ? char_to_code(str[i]) : PAD_CODE;
        if (code == PAD_CODE) code = 127;
        if (i < 4)
            w0 |= (uint32_t)(code & 0x7F) << (i * 7);
        else
            w1 |= (uint32_t)(code & 0x7F) << ((i - 4) * 7);
    }
    *word0 = w0;
    *word1 = w1;
}

void decode_string(uint32_t word0, uint32_t word1, char *buf, int buf_size) {
    int i, j = 0;
    if (!buf || buf_size <= 0) return;
    for (i = 0; i < ENCODE_MAX_CHARS && j < buf_size - 1; i++) {
        int code = (i < 4) ? (word0 >> (i * 7)) & 0x7F : (word1 >> ((i - 4) * 7)) & 0x7F;
        if (code >= NUM_CODES) break;
        buf[j++] = code_to_char(code);
    }
    buf[j] = '\0';
}
