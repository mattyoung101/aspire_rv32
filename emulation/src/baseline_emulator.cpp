#include "aspire/baseline_emulator.hpp"
#include "aspire/config.hpp"
#include "aspire/simstop.hpp"
#include "aspire/state.hpp"
#include "aspire/util.hpp"
#include "riscv-disas/riscv-disas.h"
#include "rv/rv.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <cpptrace/cpptrace.hpp>

// This file implements the baseline emulator for Aspire based on rv.c.
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
    aspire::emu::BaselineEmulator *emu = static_cast<aspire::emu::BaselineEmulator *>(user);

    // check if it's an MMIO request
    if (addr >= MMIO_BEGIN && addr <= MMIO_END) {
        // make sure it's not a multi-byte operation in the IO range, which we don't (yet) supported
        if (width > 1) {
            spdlog::error("Unsupported {} byte {} in MMIO range at address 0x{:X}", width,
                is_store ? "store" : "load", addr);
            return RV_BAD;
        }

        if (is_store) {
            emu->mmio.store(addr, data[0]);
            return RV_OK;
        } else {
            data[0] = emu->mmio.load(addr);
            return RV_OK;
        }
    }

    // normal memory access
    if (!is_store) {
        std::memcpy(data, emu->memory.data() + addr, width);

    } else {
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
}

void aspire::emu::BaselineEmulator::step() {
    spdlog::trace("step()");

    // Disassemble instruction at PC
    char buf[128] = { 0 };
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
        spdlog::error("CPU trap! Result: 0x{:X}, prior PC: 0x{:X}, mcause: 0x{:X}, trapcode: {} (\"{}\"), "
                      "interrupt: {}",
            result, pc, mcause, trapcode, aspire::emu::trapcodeToString(trapcode), interrupt);
        throw cpptrace::runtime_error("CPU trap");
    }

    // No traps, make sure we haven't gone out of machine mode just in case
    if (cpu.priv != RV_PMACH) {
        spdlog::error("Illegal privilege mode: {}", cpu.priv);
        throw cpptrace::runtime_error("Illegal privilege mode");
    }

    // Check sim stop
    if (simStop->simStopRequested) {
        spdlog::info("Received sim stop request in BaselineEmulator");
        exitRequested = true;
    }
}

aspire::emu::State aspire::emu::BaselineEmulator::getState() {
    aspire::emu::State state {};
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
    spdlog::info("Inject: Old value: 0x{:X}, new value: 0x{:X} (word: {}, bit: {})", oldValue, newValue,
        wordIdx, bitIdx);
}

void aspire::emu::BaselineEmulator::memdump(const std::string &path) {
    spdlog::info("Dumping contents of RAM to file {}", path);

    // copy RAM locally
    char bytes[RAM_SIZE] = { 0 };
    std::memcpy(bytes, memory.data(), RAM_SIZE);

    // write to file
    std::ofstream stream(path, std::ios::out | std::ios::binary);
    stream.write(bytes, RAM_SIZE);
    stream.close();
}
