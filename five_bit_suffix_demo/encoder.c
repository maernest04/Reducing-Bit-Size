#include "encoder.h"
#include <string.h>

static int char_to_code(char c) {
    if (c == ' ') return 0;
    if (c >= 'a' && c <= 'z') return 1 + (c - 'a');
    if (c >= '0' && c <= '4') return 27 + (c - '0');
    return PAD_CODE;
}

static char code_to_char(int code) {
    if (code == 0) return ' ';
    if (code >= 1 && code <= 26) return 'a' + (code - 1);
    if (code >= 27 && code <= 31) return '0' + (code - 27);
    return '\0';
}

void encode_suffix(const char *str, uint32_t *word) {
    uint32_t w = 0;
    size_t len = str ? strlen(str) : 0;
    int i;
    for (i = 0; i < ENCODE_SUFFIX_MAX_CHARS; i++) {
        int code = (i < (int)len && str[i]) ? char_to_code(str[i]) : 0;
        w |= (uint32_t)(code & 0x1F) << (i * 5);
    }
    *word = w;
}

void decode_suffix(uint32_t word, char *buf, int buf_size) {
    int i, j = 0;
    if (!buf || buf_size <= 0) return;
    for (i = 0; i < ENCODE_SUFFIX_MAX_CHARS && j < buf_size - 1; i++) {
        int code = (word >> (i * 5)) & 0x1F;
        buf[j++] = code_to_char(code);
    }
    buf[j] = '\0';
}
