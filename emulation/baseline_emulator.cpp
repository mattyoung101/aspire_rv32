// This file implements the baseline emulator for Aspire based on libriscv.
// This is used for differential fuzzing against the Verilator emulator.
#include <cstdint>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include "aspire/baseline_emulator.hpp"
#include "aspire/state.hpp"

/// Address where programs are executed at.
static constexpr uint32_t LOAD_ADDR = 0x80;

/// Aspire has 128 KiB of RAM
static constexpr uint32_t RAM_SIZE = 128 * 1024;

static constexpr uint32_t ASPIRE_UART_RESET = 0x000001; // SW Write
static constexpr uint32_t ASPIRE_UART_DATA = 0x000002; // SW Write
static constexpr uint32_t ASPIRE_UART_VALID = 0x000003; // SW Write
static constexpr uint32_t ASPIRE_UART_READY = 0x000004; // SW Read
static constexpr uint32_t ASPIRE_WDOG_ENABLE = 0x000005; // SW Write
static constexpr uint32_t ASPIRE_WDOG_RESET = 0x000006; // SW Write

aspire::emu::BaselineEmulator::BaselineEmulator(std::vector<uint8_t> bytes) {
    spdlog::debug("Initialising BaselineEmulator");
    

    // reset UART to default values
    resetMMIO();
}

void aspire::emu::BaselineEmulator::step() {
    
    // check MMIO
    updateMMIO();
}

aspire::emu::State aspire::emu::BaselineEmulator::getState() {
    aspire::emu::State state{};


    return state;
}

void aspire::emu::BaselineEmulator::memdump(const std::string &path) {
    spdlog::info("Dumping contents of RAM to file {}", path);

    // copy RAM locally
    char bytes[RAM_SIZE] = {0};
    // TODO

    // write to file
    std::ofstream stream(path, std::ios::out | std::ios::binary);
    stream.write(bytes, RAM_SIZE);
    stream.close();
}

void aspire::emu::BaselineEmulator::resetMMIO() {
    spdlog::debug("Reset MMIO");
    // TODO
    wdogRemaining = F_CPU;
    uartBuffer = "";
}

void aspire::emu::BaselineEmulator::updateMMIO() {
    // uint8_t buf[7] = {0};
    // machine->memory.memcpy_out(buf, ASPIRE_UART_RESET, 7);
    //spdlog::info("MMIO: {} {} {} {} {} {} {}", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);
    //spdlog::info("UART ready: {}", machine->memory.read<uint8_t>(ASPIRE_UART_READY));

    // UART valid flag? (SW requested print)
    // TODO

    // Watchdog enabled?
    // TODO
}
