#pragma once
#include <cstdint>

namespace aspire::emu {
    /// Defines the processor's state on a given cycle.
    struct State {
        std::uint32_t regfile[32];
        std::uint32_t pc;
    };
};
