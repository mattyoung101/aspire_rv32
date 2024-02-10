#pragma once
#include <stdint.h>
#include <stdbool.h>

// This file contains routines for interacting with the Aspire core, and other RISC-V RV32 utilities.
// It is effectively the very basic HAL for Aspire.

// CPU freq in Hz (estimated 20 MHz, TODO real frequency)
#define F_CPU (20000000)

/// Aspire UART peripheral
/// WRITE Active reset -> when set to '1', UART peripheral is reset
#define ASPIRE_UART_RESETN ((*(volatile uint8_t *) 0x000001))
/// WRITE UART data field -> set to data byte when ASPIRE_UART_READY = '1'
#define ASPIRE_UART_DATA ((*(volatile uint8_t *) 0x000002))
/// WRITE UART valid flag -> set this to '1' when a UART byte is ready to be transmitted
#define ASPIRE_UART_VALID ((*(volatile uint8_t *) 0x000003))
/// READ UART ready flag -> when set to '1', UART is ready for next data
#define ASPIRE_UART_READY ((*(volatile uint8_t *) 0x000004))

/// Aspire watchdog timer peripheral
/// WRITE Enable/disable watchdog -> when set to '1', WDOG is enabled, '0', watchdog is disabled
#define ASPIRE_WDOG_ENABLE ((*(volatile uint8_t *) 0x000005))
/// WRITE Reset the watchdog timer -> when set to '1', watchdog is reset
#define ASPIRE_WDOG_RESET ((*(volatile uint8_t *) 0x000006))

/// Aspire sim stop peripheral
/// WRITE Requests the simulation to exit when in simulation when '1' is written
#define ASPIRE_SIM_STOP ((*(volatile uint8_t *) 0x000007))

/// Reads the cycle counter for RV32 using the rdcycle and rdcycleh pseudoinstructions.
uint64_t aspire_read_cycles(void);

/// Reset UART peripheral
void aspire_uart_reset(void);

/// Transmits one character over UART. Waits for it to be transmitted.
void aspire_uart_put(char c);

/// Calls aspire_put_char for each char in string.
void aspire_uart_puts(char *string);

/// Enable/disable Aspire watchdog
void aspire_watchdog_set(bool enabled);

/// Resets the watchdog timer
void aspire_watchdog_reset(void);

/// Requests the simulation to exit when in simulation/emulation. Does nothing on FPGA.
void aspire_sim_stop(void);
