#include "aspire.h"

uint64_t aspire_read_cycles(void) {
    // Based on https://github.com/ucb-bar/chipyard/issues/7#issuecomment-297475957
    // and https://stackoverflow.com/a/2769598/5007892 - but for RV32I
    uint32_t cycles_low;
    uint32_t cycles_high;
    asm volatile ("rdcycle %0" : "=r" (cycles_low)); // cycle[0:31]
    asm volatile ("rdcycleh %0" : "=r" (cycles_high)); // cycle[32:63]
    return (uint64_t) cycles_high << 32 | cycles_low;
}

void aspire_uart_put(char c) {
    // FIXME TODO
}

void aspire_uart_puts(const char *string) {
    const char *c = string;
    while (*c) {
	aspire_uart_put(*c++);
    }
}
