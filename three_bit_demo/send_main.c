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

    for (int i = 0; i < n; i++) {
        const char *cmd = commands[i];
        uint8_t code = sender_send(cmd);
        fprintf(stderr, "Sending \"%s\"\n", cmd);
        if (write(STDOUT_FILENO, &code, 1) != 1)
            return 1;
    }
    return 0;
}
