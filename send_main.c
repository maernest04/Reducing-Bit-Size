#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "sender.h"

int main(void) {
    //8 possible commands
    const char *commands[] = {
        "Pltog", "Plstat", "Plmode", "Pltarg",
        "Plkwlm", "Plinit", "Pltqcm", "Plclmp"
    };
    const int n = sizeof(commands) / sizeof(commands[0]);

    // Send each command to the medium
    for (int i = 0; i < n; i++) {
        uint8_t code = sender_send(commands[i]);
        if (write(STDOUT_FILENO, &code, 1) != 1)
            return 1;
    }
    return 0;
}
