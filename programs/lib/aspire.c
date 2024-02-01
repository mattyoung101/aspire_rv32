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

void aspire_uart_reset(void) {
    ASPIRE_UART_RESETN = 0; // Active LOW
    __asm__ volatile("nop;"); // ensure reset goes through (FIXME may not be necessary)
    ASPIRE_UART_RESETN = 1;
}

void aspire_uart_put(char c) {
    // wait for UART to become available
    // we do this at the start, so that if you want to put a single character to IO, you don't have to wait
    // and it can be done asynchronously in the background
    // if you want to put multiple characters, of course, it will block for all but the last
    while (ASPIRE_UART_READY == 0);

    ASPIRE_UART_VALID = 0; // tell HW: we are putting in new data
    ASPIRE_UART_DATA = c; // supply the data
    ASPIRE_UART_VALID = 1; // tell HW: you're good to go mate
}

void aspire_uart_puts(const char *string) {
    const char *c = string;
    while (*c) {
	aspire_uart_put(*c++);
    }
}

void aspire_watchdog_reset(void) {
    ASPIRE_WDOG_RESETN = 0;
    __asm__ volatile("nop"); // ensure reset goes through (FIXME may not be necessary)
    ASPIRE_WDOG_RESETN = 1;
}

void aspire_watchdog_set(bool enabled) {
    ASPIRE_WDOG_ENABLE = enabled ? 1 : 0;
    aspire_watchdog_reset();
}