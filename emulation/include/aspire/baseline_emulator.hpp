#pragma once
#include <string>
#include <vector>
#include "aspire/config.hpp"
#include "aspire/state.hpp"

namespace aspire::emu {
    class BaselineEmulator {
public:
        /// Creates the baseline emulator and loads a program into it.
        BaselineEmulator(std::vector<uint8_t> bytes);

        ~BaselineEmulator() = default;

        /// Steps the emulator by one cycle.
        void step();
        
        /// Gets the emulator state.
        State getState();
        
        /// Dumps the whole contents of RAM to the specified file path
        void memdump(const std::string &path);

private:
        uint32_t wdogRemaining = F_CPU;
        std::string uartBuffer;
        
        void resetMMIO();
        void updateMMIO();
    };
};
