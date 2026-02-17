#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "encoder.h"
#include "receiver.h"

int main(void) {
    uint32_t w0, w1;
    char buf[ENCODE_MAX_CHARS + 1];
    while (read(STDIN_FILENO, &w0, 4) == 4 && read(STDIN_FILENO, &w1, 4) == 4) {
        decode_string(w0, w1, buf, sizeof(buf));
        printf("binary translated to \"%s\"\n", buf);
        receiver_handle(buf);
    }
    return 0;
}
