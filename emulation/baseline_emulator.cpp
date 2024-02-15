#include <cstdint>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include "aspire/baseline_emulator.hpp"
#include "aspire/config.hpp"
#include "aspire/state.hpp"
#include "aspire/util.hpp"
#include "riscv-disas/riscv-disas.h"
#include "rv/rv.h"

// This file implements the baseline emulator for Aspire based on libriscv.
// This is used for differential fuzzing against the Verilator emulator.

extern "C" {

static rv_res bus_cb(void *user, rv_u32 addr, rv_u8 *data, rv_u32 is_store, rv_u32 width) {
    // catch out of bounds
    if (addr + width >= RAM_SIZE || addr < 0) {
        spdlog::error("Attempted to {} {} bytes at address 0x{:X} (>= 0x{:X} RAM_MAX), out of bounds!", 
                      is_store ? "write" : "read", width, addr, RAM_SIZE);
        return RV_BAD;
    }

    // cursed, but works
    aspire::emu::BaselineEmulator *emu = static_cast<aspire::emu::BaselineEmulator*>(user);

    if (!is_store) {
        //spdlog::trace("Load {} bytes at 0x{:X}", width, addr);
        std::memcpy(data, emu->memory.data() + addr, width);
    } else {
        //spdlog::trace("Store {} bytes at 0x{:X}", width, addr);
        std::memcpy(emu->memory.data() + addr, data, width);
    }

    return RV_OK;

}
};

aspire::emu::BaselineEmulator::BaselineEmulator(std::vector<uint8_t> bytes) {
    spdlog::debug("Initialising BaselineEmulator");

    // load program into memory
    std::memcpy(memory.data() + LOAD_ADDR, bytes.data(), bytes.size());
    
    // initialise the emulator
    rv_init(&cpu, this, bus_cb);

    // reset MMIO to default values
    resetMMIO();
}

void aspire::emu::BaselineEmulator::step() {
    spdlog::trace("step()");

    // Disassemble instruction at PC
    char buf[128] = {0};
    uint32_t instr = 0;
    uint32_t pc = cpu.pc;
    // load 4 bytes from RAM at PC into our 32-bit instruction value
    std::memcpy(&instr, memory.data() + pc, 4);
    // now actually disassemble it
    disasm_inst(buf, 128, rv32, pc, instr);
    spdlog::trace("PC 0x{:X} Instr: {}", pc, buf);
    
    // Step the simulation
    auto result = rv_step(&cpu);
    
    // Handle faults if they occurred
    if (result != RV_TRAP_NONE) {
        auto mcause = cpu.csr.mcause;
        auto trapcode = mcause & 0x7FFFFFFF;
        auto interrupt = mcause & 0x80000000;
        spdlog::error("CPU trap! Result: 0x{:X}, prior PC: 0x{:X}, mcause: 0x{:X}, trapcode: {} (\"{}\"), interrupt: {}", 
                      result, pc, mcause, trapcode, aspire::emu::trapcodeToString(trapcode), interrupt);
        throw std::runtime_error("CPU trap");
    }

    // No traps, make sure we haven't gone out of machine mode just in case
    if (cpu.priv != RV_PMACH) {
        spdlog::error("Illegal privilege mode: {}", cpu.priv);
        throw std::runtime_error("Illegal privilege mode");
    }
    
    // check MMIO
    // TODO only update MMIO if the previous instruction was a load/store
    updateMMIO();
}

aspire::emu::State aspire::emu::BaselineEmulator::getState() {
    aspire::emu::State state{};
    state.pc = cpu.pc;
    for (int i = 0; i < 32; i++) {
        state.regfile[i] = cpu.r[i];
    }
    return state;
}

void aspire::emu::BaselineEmulator::injectFaultAt(uint8_t wordIdx, uint8_t bitIdx) {
    uint32_t oldValue = cpu.r[wordIdx];
    cpu.r[wordIdx] ^= (1 << bitIdx); // flip the bit
    uint32_t newValue = cpu.r[wordIdx];
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
