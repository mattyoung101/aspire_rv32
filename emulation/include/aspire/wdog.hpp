#pragma once
#include "aspire/mmio_device.hpp"

namespace aspire::mmio {
/// MMIO watchdog device
class Watchdog : public MMIODevice<Watchdog> {
public:
    uint8_t load(uint32_t address);
    void store(uint32_t address, uint8_t value);
    Range getRange();
};
};
