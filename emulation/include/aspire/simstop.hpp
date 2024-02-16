#pragma once
#include "aspire/mmio_device.hpp"

namespace aspire::mmio {
/// MMIO sim stop device
class SimStop : public MMIODevice<SimStop> {
public:
    uint8_t load(uint32_t address);
    void store(uint32_t address, uint8_t value);
    Range getRange();

    bool simStopRequested;
};
};
