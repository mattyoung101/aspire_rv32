// Copyright (c) 2024 Matt Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once
#include <cstdint>

// This file contains constants used in the Aspire emulator.

/// Simulated CPU frequency in Hz
static constexpr uint32_t F_CPU = 3'000'000;

/// Address where programs are executed at (start of IMEM).
static constexpr uint32_t LOAD_ADDR = 0x80;

/// Aspire has 128 KiB of RAM
static constexpr uint32_t RAM_SIZE = 128 * 1024;

// MMIO region
static constexpr uint32_t MMIO_BEGIN = 0x01;
static constexpr uint32_t MMIO_END = 0x7F;

// MMIO registers
static constexpr uint32_t ASPIRE_UART_RESET = 0x000001; // SW Write
static constexpr uint32_t ASPIRE_UART_DATA = 0x000002; // SW Write
static constexpr uint32_t ASPIRE_UART_VALID = 0x000003; // SW Write
static constexpr uint32_t ASPIRE_UART_READY = 0x000004; // SW Read
static constexpr uint32_t ASPIRE_WDOG_ENABLE = 0x000005; // SW Write
static constexpr uint32_t ASPIRE_WDOG_RESET = 0x000006; // SW Write
static constexpr uint32_t ASPIRE_SIM_STOP = 0x000007; // SW Write
