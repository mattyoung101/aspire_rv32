#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

namespace aspire {
/// A range, min to max inclusive
typedef std::pair<uint32_t, uint32_t> Range;

/// Converts nanosecond period to a frequency in MHz.
constexpr double nanosToMHz(size_t nanos) {
    double hz = 1.0 / (static_cast<double>(nanos) / 1e+9);
    double mhz = hz / 1'000'000.0;
    return mhz;
}

/// Converts a frequency in MHz to a period in nanoseconds.
constexpr double mHzToNanos(double mhz) {
    double seconds = 1.0 / (static_cast<double>(mhz) * 1'000'000);
    double nanos = seconds * 1e+9;
    return nanos;
}

};

namespace aspire::emu {

/// Converts an exception code from mcause into a string. (ONLY WHEN INTERRUPT=0)
/// Source: 3.1.15. Machine Cause Register (mcause) (RISC-V Privileged ISA Document)
constexpr auto trapcodeToString(uint32_t trapcode) {
    switch (trapcode) {
        case 0:
            return "Instruction address misaligned";
        case 1:
            return "Instruction access fault";
        case 2:
            return "Illegal instruction";
        case 3:
            return "Breakpoint";
        case 4:
            return "Load address misaligned";
        case 5:
            return "Load access fault";
        case 6:
            return "Store/AMO address misaligned";
        case 7:
            return "Store/AMO access fault";
        case 8:
            return "Environment call from U-mode";
        case 9:
            return "Environment call from S-mode";
        case 10:
            return "Reserved";
        case 11:
            return "Environment call from M-mode";
        case 12:
            return "Instruction page fault";
        case 13:
            return "Load page fault";
        case 14:
            return "Reserved";
        case 15:
            return "Store/AMO page fault";
        // The rest are basically reserved.
        default:
            return "Unknown";
    }
}

};
