#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "encoder.h"

#define PREFIX "Pl"
#define PREFIX_LEN 2

int main(void) {
    const char *commands[] = {
        "Pltog", "Plstat", "Plmode", "Pltarg",
        "Plkwlm", "Plinit", "Pltqcm", "Plclmp"
    };
    const int n = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < n; i++) {
        const char *cmd = commands[i];
        const char *suffix = cmd;
        if (strncmp(cmd, PREFIX, PREFIX_LEN) == 0)
            suffix = cmd + PREFIX_LEN;
        uint32_t w;
        encode_suffix(suffix, &w);
        fprintf(stderr, "Sending \"%s\"\n", cmd);
        if (write(STDOUT_FILENO, &w, 4) != 4)
            return 1;
    }
    return 0;
}
