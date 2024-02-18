#include "aspire/mmio.hpp"
#include <cpptrace/cpptrace.hpp>
#include <cstdint>
#include <spdlog/spdlog.h>
#include <variant>

uint8_t aspire::mmio::MMIO::load(uint32_t address) {
    SPDLOG_TRACE("MMIO: Load at 0x{:X}", address);

    for (auto &device : devices) {
        auto range = std::visit([](auto &arg) { return arg->getRange(); }, device);
        SPDLOG_TRACE("MMIO: Considering MMIO device with range 0x{:X} - 0x{:X}", range.first, range.second);

        // see if this device matches our address
        if (address >= range.first && address <= range.second) {
            SPDLOG_TRACE("MMIO: Found matching MMIO device");
            return std::visit([&address](auto &arg) { return arg->load(address); }, device);
        }
    }

    SPDLOG_ERROR("No matching MMIO device for load at 0x{:X}", address);
    throw cpptrace::runtime_error("Failed to find MMIO device");
}

void aspire::mmio::MMIO::store(uint32_t address, uint8_t value) {
    SPDLOG_TRACE("MMIO: Store at 0x{:X}", address);

    for (auto &device : devices) {
        auto range = std::visit([](auto &arg) { return arg->getRange(); }, device);
        SPDLOG_TRACE("MMIO: Considering MMIO device with range 0x{:X} - 0x{:X}", range.first, range.second);

        // see if this device matches our address
        if (address >= range.first && address <= range.second) {
            SPDLOG_TRACE("MMIO: Found matching MMIO device");
            std::visit([&address, &value](auto &arg) { arg->store(address, value); }, device);
            return;
        }
    }

    SPDLOG_ERROR("MMIO: No matching MMIO device for store at 0x{:X}", address);
    throw cpptrace::runtime_error("Failed to find MMIO device");
}
