#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "encoder.h"
#include "receiver.h"

#define PREFIX "Pl"
#define FULL_BUF_SIZE 16

int main(void) {
    uint32_t w;
    char suffix_buf[ENCODE_SUFFIX_MAX_CHARS + 1];
    char full_buf[FULL_BUF_SIZE];
    while (read(STDIN_FILENO, &w, 4) == 4) {
        decode_suffix(w, suffix_buf, sizeof(suffix_buf));
        {
            char *p = suffix_buf + strlen(suffix_buf);
            while (p > suffix_buf && p[-1] == ' ') p--;
            *p = '\0';
        }
        snprintf(full_buf, sizeof(full_buf), "%s%s", PREFIX, suffix_buf);
        printf("binary translated to \"%s\"\n", full_buf);
        receiver_handle(full_buf);
    }
    return 0;
}
