#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

#define ENCODE_SUFFIX_MAX_CHARS 6
#define PAD_CODE 31

void encode_suffix(const char *str, uint32_t *word);
void decode_suffix(uint32_t word, char *buf, int buf_size);

#endif
