#include "sender.h"
#include <string.h>
#include <ctype.h>

static int str_eq(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

// Convert a command to a 3-bit code
int sender_encode(const char *cmd) {
    if (!cmd) return -1;
    if (str_eq(cmd, "Pltog"))  return 0;
    if (str_eq(cmd, "Plstat")) return 1;
    if (str_eq(cmd, "Plmode")) return 2;
    if (str_eq(cmd, "Pltarg")) return 3;
    if (str_eq(cmd, "Plkwlm")) return 4;
    if (str_eq(cmd, "Plinit")) return 5;
    if (str_eq(cmd, "Pltqcm")) return 6;
    if (str_eq(cmd, "Plclmp")) return 7;
    return -1;
}

// Return the 3-bit code for a command
uint8_t sender_send(const char *cmd) {
    int code = sender_encode(cmd);
    return (code >= 0) ? (uint8_t)(code & 0x07) : 0;
}
