#pragma once
#include <cstdint>
#include <string>

namespace aspire::emu {

/// Converts an exception code from mcause into a string.
/// Source: 3.1.15. Machine Cause Register (mcause) (RISC-V Privileged ISA Document)
constexpr std::string trapcodeToString(uint32_t trapcode) {
    switch (trapcode) {
        case 0: return "Instruction address misaligned";
        case 1: return "Instruction access fault";
        case 2: return "Illegal instruction";
        case 3: return "Breakpoint";
        case 4: return "Load address misaligned";
        case 5: return "Load access fault";
        case 6: return "Store/AMO address misaligned";
        case 7: return "Store/AMO access fault";
        default: return "Unknown";
    }
}

};
