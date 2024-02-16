#pragma once
#include "aspire/util.hpp"
#include <cstdint>

namespace aspire::mmio {
/// Generic interface for an MMIO device
/// This CRTP is yoinked from Ethan. Much obliged.
template <class Derived> class MMIODevice {
public:
    /// Called during memory access if the memory address is in the MMIO range and is a load operation.
    /// Returns the value that should be loaded to the CPU.
    uint8_t load(uint32_t address) {
        return self().load();
    }

    /// Called during memory access if the memory address is in the MMIO range and is a store operation.
    void store(uint32_t address, uint8_t value) {
        return self().store();
    }

    /// Returns the address range this MMIO device corresponds to.
    Range getRange() {
        return self().getRange();
    }

protected:
    inline Derived &self() {
        return static_cast<Derived &>(*this);
    }

    friend Derived;
};
};
