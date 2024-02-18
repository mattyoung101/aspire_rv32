#include "aspire/simstop.hpp"
#include "aspire/config.hpp"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <utility>
#include <cpptrace/cpptrace.hpp>

uint8_t aspire::mmio::SimStop::load(uint32_t address) {
    // Can't load anything from watchdog.
    SPDLOG_ERROR("MMIO: WDOG: Watchdog has no load addresses");
    throw cpptrace::runtime_error("Cannot load in WDOG");
}

void aspire::mmio::SimStop::store(uint32_t address, uint8_t value) {
    if (address == ASPIRE_SIM_STOP && value == 0x1) {
        SPDLOG_INFO("MMIO: SIMSTOP: Simulation stop requested!");
        simStopRequested = true;
    }
}

aspire::Range aspire::mmio::SimStop::getRange() {
    return std::make_pair(ASPIRE_SIM_STOP, ASPIRE_SIM_STOP);
}
