#pragma once
#include <unordered_map>
#include <vector>
#include <variant>
#include "aspire/mmio_device.hpp"
#include "aspire/wdog.hpp"
#include "aspire/util.hpp"
#include "aspire/uart.hpp"
#include "aspire/simstop.hpp"

namespace aspire::mmio {

using AnyMMIO = std::variant<UART, Watchdog, SimStop>;

/// MMIO manager
class MMIO {
public:
    explicit MMIO(std::vector<AnyMMIO> &devices) : devices(devices) {}
    ~MMIO() = default;

    /// Called during memory access if the memory address is in the MMIO range and is a load operation.
    /// Returns the value that should be loaded to the CPU.
    uint8_t load(uint32_t address);

    /// Called during memory access if the memory address is in the MMIO range and is a store operation.
    void store(uint32_t address, uint8_t value);

private:
    std::vector<AnyMMIO> &devices;
};
};
