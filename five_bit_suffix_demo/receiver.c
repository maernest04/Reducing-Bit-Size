#include "receiver.h"
#include <stdio.h>
#include <string.h>

static void test_pltog(void)  { printf("[RECV] Running test: Pltog\n"); }
static void test_plstat(void) { printf("[RECV] Running test: Plstat\n"); }
static void test_plmode(void) { printf("[RECV] Running test: Plmode\n"); }
static void test_pltarg(void) { printf("[RECV] Running test: Pltarg\n"); }
static void test_plkwlm(void) { printf("[RECV] Running test: Plkwlm\n"); }
static void test_plinit(void) { printf("[RECV] Running test: Plinit\n"); }
static void test_pltqcm(void) { printf("[RECV] Running test: Pltqcm\n"); }
static void test_plclmp(void) { printf("[RECV] Running test: Plclmp\n"); }

void receiver_handle(const char *str) {
    if (!str) return;
    if (strcmp(str, "Pltog") == 0)  { test_pltog();  return; }
    if (strcmp(str, "Plstat") == 0) { test_plstat(); return; }
    if (strcmp(str, "Plmode") == 0) { test_plmode(); return; }
    if (strcmp(str, "Pltarg") == 0) { test_pltarg(); return; }
    if (strcmp(str, "Plkwlm") == 0) { test_plkwlm(); return; }
    if (strcmp(str, "Plinit") == 0) { test_plinit(); return; }
    if (strcmp(str, "Pltqcm") == 0) { test_pltqcm(); return; }
    if (strcmp(str, "Plclmp") == 0) { test_plclmp(); return; }
    printf("[RECV] Unknown command: \"%s\"\n", str);
}
