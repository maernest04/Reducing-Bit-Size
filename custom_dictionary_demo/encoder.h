#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

#define ENCODE_MAX_CHARS 6
#define PAD_CODE 63

void encode_string(const char *str, uint32_t *word0, uint32_t *word1);
void decode_string(uint32_t word0, uint32_t word1, char *buf, int buf_size);

#endif
