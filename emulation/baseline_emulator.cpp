// This file implements the baseline emulator for Aspire based on libriscv.
// This is used for differential fuzzing against the Verilator emulator.
#include <iostream>
#include <fstream>
#include <libriscv/machine.hpp>
#include <libriscv/debug.hpp>
#include <spdlog/spdlog.h>
#include "aspire/baseline_emulator.hpp"
#include "aspire/state.hpp"

using namespace riscv;

/// Address where programs are executed at.
static constexpr uint32_t LOAD_ADDR = 0x80;

aspire::emu::BaselineEmulator::BaselineEmulator(std::vector<uint8_t> bytes) {
    spdlog::debug("Initialising BaselineEmulator");
    
    MachineOptions<RISCV32> opts = {
         // Aspire has 128 KiB of RAM
        .memory_max = 128 * 1024,
        .allow_write_exec_segment = true
    };

    machine = std::make_unique<Machine<RISCV32>>(opts);

    // load program at boot address
    machine->cpu.init_execute_area(bytes.data(), LOAD_ADDR, bytes.size());
}

void aspire::emu::BaselineEmulator::step() {
    // TODO
}

aspire::emu::State aspire::emu::BaselineEmulator::get_state() {
    aspire::emu::State state{};

    state.pc = machine->cpu.pc();
    
    for (uint32_t i = 0; i < 31; i++) {
        state.regfile[i] = machine->cpu.reg(i);
    }

    return state;
}


