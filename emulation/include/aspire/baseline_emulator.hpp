#pragma once
#include <string>
#include <vector>
#include <array>
#include "aspire/config.hpp"
#include "aspire/state.hpp"
#include "core.h"

namespace aspire::emu {
    class BaselineEmulator {
public:
        /// Creates the baseline emulator and loads a program into it.
        BaselineEmulator(std::vector<uint8_t> bytes);

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
        std::array<uint8_t, RAM_SIZE> memory{};
        
        /// If set to true, the running program requested a sim exit using MMIO
        bool exitRequested = false;

private:
        // The core itself
        rv_core_td core;

        // MMIO peripheral status
        uint32_t wdogRemaining = F_CPU;
        std::string uartBuffer{};
        
        void resetMMIO();
        void updateMMIO();
    };
};
