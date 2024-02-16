#include "aspire/uart.hpp"
#include "aspire/config.hpp"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <utility>

uint8_t aspire::mmio::UART::load(uint32_t address) {
    if (address == ASPIRE_UART_READY) {
        // We always do UART in a single cycle, so always ready.
        return 0x1;
    }

    // Can't load this address
    spdlog::error("MMIO: UART: Cannot load address 0x{:X} in UART peripheral!");
    throw std::runtime_error("Cannot load UART address");
}

void aspire::mmio::UART::store(uint32_t address, uint8_t value) {
    switch (address) {
        case ASPIRE_UART_RESET:
            if (value == 0x1) {
                spdlog::debug("MMIO: UART: Reset UART");
                buffer = "";
                c = '\0';
            }
            break;

        case ASPIRE_UART_DATA:
            c = static_cast<char>(value);
            buffer += c;
            spdlog::trace("MMIO: UART: Store {}, buffer: {}", c, buffer);
            break;

        case ASPIRE_UART_VALID:
            // If the last entered char was \n, and we wrote a 0x1, print the whole buffer
            if (c == '\n' && value == 0x1) {
                // remove the '\n'
                buffer.pop_back();
                spdlog::info("UART: {}", buffer);
                // reset internal state
                buffer = "";
                c = '\0';
            }
            // Otherwise, keep waiting, don't print yet
            break;

        default:
            spdlog::error("MMIO: UART: Cannot store address 0x{:X} (value 0x{:X})", address, value);
    }
}

aspire::Range aspire::mmio::UART::getRange() {
    return std::make_pair(ASPIRE_UART_RESET, ASPIRE_UART_READY);
}
