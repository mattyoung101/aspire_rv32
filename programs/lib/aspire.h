#include <stdint.h>
#include <stddef.h>

// This file contains routines for interacting with the Aspire core, and other RISC-V RV32 utilities.
// It is effectively the very basic HAL for Aspire.

// CPU freq in Hz (estimated 20 MHz, TODO real frequency)
#define F_CPU (20000000)

/// Reads the cycle counter for RV32 using the rdcycle and rdcycleh pseudoinstructions.
uint64_t aspire_read_cycles(void);

/// Transmits one character over UART. Waits for it to be transmitted.
void aspire_uart_put(char c);

/// Calls aspire_put_char for each char in string.
void aspire_uart_puts(const char *string);
