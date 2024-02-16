#pragma once
#include "aspire/config.hpp"
#include "aspire/mmio.hpp"
#include "aspire/simstop.hpp"
#include "aspire/state.hpp"
#include "aspire/uart.hpp"
#include "aspire/wdog.hpp"
#include "rv/rv.h"
#include <array>
#include <memory>
#include <string>
#include <vector>

namespace aspire::emu {
/// Baseline emulator powered by rv.c
class BaselineEmulator {
public:
    /// Creates the baseline emulator and loads a program into it.
    explicit BaselineEmulator(std::vector<uint8_t> bytes);

    ~BaselineEmulator() = default;

    /// Steps the emulator by one cycle.
    void step();

    /// Used for fault injection, flips the specified bit in the specified word in the register file.
    void injectFaultAt(uint8_t wordIdx, uint8_t bitIdx);

    /// Gets the emulator state.
    State getState();

    /// Dumps the whole contents of RAM to the specified file path
    void memdump(const std::string &path);

    /// Core memory (has to be publicly exposed for C callback function)
    std::array<uint8_t, RAM_SIZE> memory {};

    /// If set to true, the running program requested a sim exit using MMIO
    bool exitRequested = false;

    // FIXME make these private
    std::shared_ptr<mmio::UART> uart = std::make_shared<mmio::UART>();
    std::shared_ptr<mmio::Watchdog> watchdog = std::make_shared<mmio::Watchdog>();
    std::shared_ptr<mmio::SimStop> simStop = std::make_shared<mmio::SimStop>();

    /// List of MMIO devices
    std::vector<mmio::AnyMMIOPtr> mmiomap { uart, watchdog, simStop };

    /// MMIO manager
    mmio::MMIO mmio { mmiomap };

private:
    /// The CPU itself
    rv cpu;
};
};
