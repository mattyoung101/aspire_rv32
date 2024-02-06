#pragma once
#include <libriscv/machine.hpp>
#include "aspire/emulator.hpp"

namespace aspire::emu {
    using namespace riscv;
    
    class BaselineEmulator : Emulator {
public:
        /// Creates the baseline emulator and loads a program into it.
        BaselineEmulator(std::vector<uint8_t> bytes);

        ~BaselineEmulator() = default;

        /// Steps the emulator by one cycle.
        void step();
        
        // Gets the emulator state.
        State get_state();

private:
        std::unique_ptr<Machine<RISCV32>> machine;
    };
};
