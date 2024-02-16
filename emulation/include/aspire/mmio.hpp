#pragma once
#include "aspire/simstop.hpp"
#include "aspire/uart.hpp"
#include "aspire/wdog.hpp"
#include <memory>
#include <variant>
#include <vector>

namespace aspire::mmio {

/// A shared ptr to any of the MMIO subclasses
using AnyMMIOPtr = std::variant<std::shared_ptr<UART>, std::shared_ptr<Watchdog>, std::shared_ptr<SimStop>>;

/// MMIO manager
class MMIO {
public:
    explicit MMIO(std::vector<AnyMMIOPtr> devices)
        : devices(devices) {
    }
    ~MMIO() = default;

    /// Called during memory access if the memory address is in the MMIO range and is a load operation.
    /// Returns the value that should be loaded to the CPU.
    uint8_t load(uint32_t address);

    /// Called during memory access if the memory address is in the MMIO range and is a store operation.
    void store(uint32_t address, uint8_t value);

private:
    std::vector<AnyMMIOPtr> devices;
};
};
