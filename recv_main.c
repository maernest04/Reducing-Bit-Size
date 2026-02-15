#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "receiver.h"

// Receive a 3-bit code from the medium and handle it
int main(void) {
    uint8_t code;
    while (read(STDIN_FILENO, &code, 1) == 1)
        receiver_handle(code);
    return 0;
}
