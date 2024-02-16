#pragma once
#include "aspire/mmio_device.hpp"

namespace aspire::mmio {
/// Virtual MMIO UART device
class UART : public MMIODevice<UART> {
public:
    uint8_t load(uint32_t address);
    void store(uint32_t address, uint8_t value);
    Range getRange();

private:
    char c = '\0';
    std::string buffer = "";
};
};
