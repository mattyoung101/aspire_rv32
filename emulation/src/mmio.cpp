#include "aspire/mmio.hpp"
#include <cstdint>
#include <variant>
#include <spdlog/spdlog.h>

uint8_t aspire::mmio::MMIO::load(uint32_t address) {
    spdlog::trace("MMIO: Load at 0x{:X}", address);
    
    for (auto &device : devices) {
        auto range = std::visit([](auto &arg) { 
            return arg->getRange();
        }, device);
        spdlog::trace("MMIO: Considering MMIO device with range 0x{:X} - 0x{:X}", range.first, range.second);

        // see if this device matches our address
        if (address >= range.first && address <= range.second) {
            spdlog::trace("MMIO: Found matching MMIO device");

            return std::visit([&address](auto &arg) {
                return arg->load(address);
            }, device);
        }
    }

    spdlog::error("No matching MMIO device for load at 0x{:X}", address);
    throw std::runtime_error("Failed to find MMIO device");
}

void aspire::mmio::MMIO::store(uint32_t address, uint8_t value) {
    spdlog::trace("MMIO: Store at 0x{:X}", address);
    
    for (auto &device : devices) {
        auto range = std::visit([](auto &arg) { 
            return arg->getRange();
        }, device);
        spdlog::trace("MMIO: Considering MMIO device with range 0x{:X} - 0x{:X}", range.first, range.second);

        // see if this device matches our address
        if (address >= range.first && address <= range.second) {
            spdlog::trace("MMIO: Found matching MMIO device");

            std::visit([&address, &value](auto &arg) {
                arg->store(address, value);
            }, device);

            return;
        }
    }

    spdlog::error("MMIO: No matching MMIO device for store at 0x{:X}", address);
    throw std::runtime_error("Failed to find MMIO device");
}
