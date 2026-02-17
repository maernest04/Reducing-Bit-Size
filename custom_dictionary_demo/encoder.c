#include "encoder.h"

static int char_to_code(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= 'A' && c <= 'Z') return 26 + (c - 'A');
    return PAD_CODE;
}

static char code_to_char(int code) {
    if (code >= 0 && code < 26) return 'a' + code;
    if (code >= 26 && code < 52) return 'A' + (code - 26);
    return '\0';
}

void encode_string(const char *str, uint32_t *word0, uint32_t *word1) {
    uint32_t w0 = 0, w1 = 0;
    int i;
    for (i = 0; i < ENCODE_MAX_CHARS; i++) {
        int code = (str && str[i]) ? char_to_code(str[i]) : PAD_CODE;
        if (code == PAD_CODE) code = 63;
        if (i < 5)
            w0 |= (uint32_t)(code & 0x3F) << (i * 6);
        else
            w1 = (uint32_t)(code & 0x3F);
    }
    *word0 = w0;
    *word1 = w1;
}

void decode_string(uint32_t word0, uint32_t word1, char *buf, int buf_size) {
    int i, j = 0;
    if (!buf || buf_size <= 0) return;
    for (i = 0; i < ENCODE_MAX_CHARS && j < buf_size - 1; i++) {
        int code = (i < 5) ? (word0 >> (i * 6)) & 0x3F : word1 & 0x3F;
        if (code >= 63) break;
        buf[j++] = code_to_char(code);
    }
    buf[j] = '\0';
}
