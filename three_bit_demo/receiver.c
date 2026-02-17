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

static const char *code_to_str(uint8_t code) {
    switch (code & 0x07) {
        case 0: return "Pltog";
        case 1: return "Plstat";
        case 2: return "Plmode";
        case 3: return "Pltarg";
        case 4: return "Plkwlm";
        case 5: return "Plinit";
        case 6: return "Pltqcm";
        case 7: return "Plclmp";
        default: return "?";
    }
}

void receiver_handle(uint8_t code) {
    const char *str = code_to_str(code);
    printf("binary translated to \"%s\"\n", str);
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
