#include "aspire/wdog.hpp"
#include "aspire/config.hpp"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <utility>

uint8_t aspire::mmio::Watchdog::load(uint32_t address) {
    // Can't load anything from watchdog.
    spdlog::error("MMIO: WDOG: Watchdog has no load addresses");
    throw std::runtime_error("Cannot load in WDOG");
}

void aspire::mmio::Watchdog::store(uint32_t address, uint8_t value) {
    // FIXME
}

aspire::Range aspire::mmio::Watchdog::getRange() {
    return std::make_pair(ASPIRE_WDOG_ENABLE, ASPIRE_WDOG_RESET);
}
