#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "encoder.h"

int main(void) {
    const char *commands[] = {
        "Pltog", "Plstat", "Plmode", "Pltarg",
        "Plkwlm", "Plinit", "Pltqcm", "Plclmp"
    };
    const int n = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < n; i++) {
        const char *cmd = commands[i];
        uint32_t w0, w1;
        encode_string(cmd, &w0, &w1);
        fprintf(stderr, "Sending \"%s\"\n", cmd);
        if (write(STDOUT_FILENO, &w0, 4) != 4 || write(STDOUT_FILENO, &w1, 4) != 4)
            return 1;
    }
    return 0;
}
