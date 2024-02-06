#pragma once
#include <cstdint>
#include <vector>
#include "aspire/state.hpp"

namespace aspire::emu {
    /// Defines a particular emulator, whether that be Verilator or libriscv.
    struct Emulator {
        Emulator() = default;
        ~Emulator() = default;

        /// Steps the emulator by one cycle.
        virtual void step() = 0;
        
        // Gets the emulator state.
        virtual State get_state() = 0;
    };
};
