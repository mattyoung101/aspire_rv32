#include "aspire/wdog.hpp"
#include "aspire/config.hpp"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <utility>
#include <cpptrace/cpptrace.hpp>

uint8_t aspire::mmio::Watchdog::load(uint32_t address) {
    // Can't load anything from watchdog.
    SPDLOG_ERROR("MMIO: WDOG: Watchdog has no load addresses");
    throw cpptrace::runtime_error("Cannot load in WDOG");
}

void aspire::mmio::Watchdog::store(uint32_t address, uint8_t value) {
    // FIXME
    SPDLOG_TRACE("MMIO: WDOG: Note, WDOG is not yet implemented");
}

aspire::Range aspire::mmio::Watchdog::getRange() {
    return std::make_pair(ASPIRE_WDOG_ENABLE, ASPIRE_WDOG_RESET);
}
