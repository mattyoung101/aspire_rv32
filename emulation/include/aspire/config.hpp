#pragma once
#include <cstdint>

/// Simulated CPU frequency in Hz
static constexpr uint32_t F_CPU = 20000;

/// Address where programs are executed at.
static constexpr uint32_t LOAD_ADDR = 0x80;

/// Aspire has 128 KiB of RAM
static constexpr uint32_t RAM_SIZE = 128 * 1024;

static constexpr uint32_t ASPIRE_UART_RESET = 0x000001; // SW Write
static constexpr uint32_t ASPIRE_UART_DATA = 0x000002; // SW Write
static constexpr uint32_t ASPIRE_UART_VALID = 0x000003; // SW Write
static constexpr uint32_t ASPIRE_UART_READY = 0x000004; // SW Read
static constexpr uint32_t ASPIRE_WDOG_ENABLE = 0x000005; // SW Write
static constexpr uint32_t ASPIRE_WDOG_RESET = 0x000006; // SW Write
static constexpr uint32_t ASPIRE_SIM_STOP = 0x000007; // SW Write
