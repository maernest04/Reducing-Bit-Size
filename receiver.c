#include "receiver.h"
#include <stdio.h>

static void test_pltog(void)  { printf("[RECV] Running test: Pltog\n"); }
static void test_plstat(void) { printf("[RECV] Running test: Plstat\n"); }
static void test_plmode(void) { printf("[RECV] Running test: Plmode\n"); }
static void test_pltarg(void) { printf("[RECV] Running test: Pltarg\n"); }
static void test_plkwlm(void) { printf("[RECV] Running test: Plkwlm\n"); }
static void test_plinit(void) { printf("[RECV] Running test: Plinit\n"); }
static void test_pltqcm(void) { printf("[RECV] Running test: Pltqcm\n"); }
static void test_plclmp(void) { printf("[RECV] Running test: Plclmp\n"); }

// Handle a 3-bit code by checking the code and calling the correct test function
void receiver_handle(uint8_t code) {
    switch (code & 0x07) {
        case 0: test_pltog();  break;
        case 1: test_plstat(); break;
        case 2: test_plmode(); break;
        case 3: test_pltarg(); break;
        case 4: test_plkwlm(); break;
        case 5: test_plinit(); break;
        case 6: test_pltqcm(); break;
        case 7: test_plclmp(); break;
        default:
            printf("[RECV] Unknown code: %u\n", code);
            break;
    }
}
