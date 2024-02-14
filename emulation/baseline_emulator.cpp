#include <cstdint>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include "aspire/baseline_emulator.hpp"
#include "aspire/config.hpp"
#include "aspire/state.hpp"
#include "core.h"
#include "riscv_types.h"
#include "riscv-disas.h"

// This file implements the baseline emulator for Aspire based on libriscv.
// This is used for differential fuzzing against the Verilator emulator.

extern "C" {
static rv_ret rv_bus_access(void *priv, privilege_level priv_level, bus_access_type access_type, 
                                rv_uint_xlen address, void *value, uint8_t len) {
    if (priv_level != machine_mode) {
        spdlog::error("Illegal privilege mode: {}", static_cast<int>(priv_level));
        throw std::runtime_error("Illegal privilege mode");
    }

    // catch out of bounds
    if (address + len >= RAM_SIZE || address < 0) {
        spdlog::error("Attempted to {} {} bytes at address 0x{:X} (>= 0x{:X} RAM_MAX), out of bounds!", 
                      access_type == bus_write_access ? "write" : "read", len, address, RAM_SIZE);
        spdlog::trace("Access type: {}", static_cast<int>(access_type));
        throw std::runtime_error("Illegal memory access");
    }

    // cursed, but works
    aspire::emu::BaselineEmulator *emu = static_cast<aspire::emu::BaselineEmulator*>(priv);

    if (access_type == bus_read_access || access_type == bus_instr_access) {
        spdlog::trace("Load {} bytes at 0x{:X}", len, address);
        std::memcpy(value, emu->memory.data() + address, len);
    } else if (access_type == bus_write_access) {
        spdlog::trace("Store {} bytes at 0x{:X}", len, address);
        std::memcpy(emu->memory.data() + address, value, len);
    } else {
        spdlog::error("Unhandled access type: {}", static_cast<int>(access_type));
        throw std::runtime_error("Unhandled access type");
    }

    return rv_ok;
}
};

aspire::emu::BaselineEmulator::BaselineEmulator(std::vector<uint8_t> bytes) {
    spdlog::debug("Initialising BaselineEmulator");

    // load program into memory
    std::memcpy(memory.data() + LOAD_ADDR, bytes.data(), bytes.size());
    
    // initialise the emulator
    rv_core_init(&core, this, rv_bus_access);

    // reset MMIO to default values
    resetMMIO();
}

void aspire::emu::BaselineEmulator::step() {
    // Disassemble instruction at PC
    char buf[128] = {0};
    uint32_t instr = 0;
    // load 4 bytes from RAM at PC into our 32-bit instruction value
    std::memcpy(&instr, memory.data() + core.pc, 4);
    // now actually disassemble it
    disasm_inst(buf, 128, rv32, core.pc, instr);
    spdlog::trace("Instr: {}", buf);
    
    // Step the simulation
    rv_core_run(&core);
    
    // Check to make sure it didn't enter machine mode
    if (core.curr_priv_mode != machine_mode) {
        spdlog::error("Illegal privilege mode: {}", static_cast<int>(core.curr_priv_mode));
        throw std::runtime_error("Illegal privilege mode");
    }
    
    // check MMIO
    // TODO only update MMIO if the previous instruction was a load/store
    updateMMIO();
}

aspire::emu::State aspire::emu::BaselineEmulator::getState() {
    aspire::emu::State state{};
    state.pc = core.pc;
    for (int i = 0; i < 32; i++) {
        state.regfile[i] = core.x[i];
    }
    return state;
}

void aspire::emu::BaselineEmulator::injectFaultAt(uint8_t wordIdx, uint8_t bitIdx) {
    uint32_t oldValue = core.x[wordIdx];
    core.x[wordIdx] ^= (1 << bitIdx); // flip the bit
    uint32_t newValue = core.x[wordIdx];
    spdlog::info("Inject: Old value: 0x{:X}, new value: 0x{:X} (word: {}, bit: {})", oldValue, newValue, wordIdx, bitIdx);
}

void aspire::emu::BaselineEmulator::memdump(const std::string &path) {
    spdlog::info("Dumping contents of RAM to file {}", path);

    // copy RAM locally
    char bytes[RAM_SIZE] = {0};
    std::memcpy(bytes, memory.data(), RAM_SIZE);
    
    // write to file
    std::ofstream stream(path, std::ios::out | std::ios::binary);
    stream.write(bytes, RAM_SIZE);
    stream.close();
}

void aspire::emu::BaselineEmulator::resetMMIO() {
    spdlog::debug("Reset MMIO");
    
    // reset state in RAM
    memory[ASPIRE_UART_READY] = 1;
    memory[ASPIRE_WDOG_ENABLE] = 0;
    
    // reset internal state
    wdogRemaining = F_CPU;
    uartBuffer = "";
}

void aspire::emu::BaselineEmulator::updateMMIO() {
    // Not really MMIO, but check if someone dereferenced a null pointer
    if (memory[0] != 0) {
        spdlog::error("Address 0x0 has been written to! Null pointer dereference!");
        exit(1);
    }

    // Check UART
    if (memory[ASPIRE_UART_VALID] == 1) {
        // UART is valid! Store the character.
        spdlog::trace("UART valid");

        char c = static_cast<char>(memory[ASPIRE_UART_DATA]);
        if (c == '\n') {
            // on newlines, print the buffer and reset
            spdlog::info("UART: {}", uartBuffer);
            uartBuffer = "";
        } else {
            // otherwise, append to buffer
            uartBuffer += c;
        }

        // signal the processor we have printed
        memory[ASPIRE_UART_VALID] = 1;
    }

    // Check watchdog
    if (memory[ASPIRE_WDOG_ENABLE] == 1) {
        // check if watchdog was reset
        if (memory[ASPIRE_WDOG_RESET] == 1) {
            spdlog::info("Watchdog: Reset");
            wdogRemaining = F_CPU;
        }

        // check if watchdog expired
        if (wdogRemaining-- <= 0) {
            spdlog::error("Watchdog: Timeout!");
            exit(1);
        }
    }

    // Check sim exit
    if (memory[ASPIRE_SIM_STOP] == 1) {
        spdlog::info("Simulation exit requested by running program");
        exitRequested = true;
    }
}
