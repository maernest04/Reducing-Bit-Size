#ifndef SENDER_H
#define SENDER_H

#include <stdint.h>

int sender_encode(const char *cmd);
uint8_t sender_send(const char *cmd);

#endif
