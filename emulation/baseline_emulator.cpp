// This file implements the baseline emulator for Aspire based on libriscv.
// This is used for differential fuzzing against the Verilator emulator.
#include <cstdint>
#include <iostream>
#include <fstream>
#include <libriscv/machine.hpp>
#include <libriscv/debug.hpp>
#include <spdlog/spdlog.h>
#include <string.h>
#include "aspire/baseline_emulator.hpp"
#include "aspire/state.hpp"
#include "libriscv/rv32i_instr.hpp"

using namespace riscv;

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
    
    MachineOptions<RISCV32> opts = {
        .memory_max = RAM_SIZE,
        .allow_write_exec_segment = true
    };

    machine = std::make_unique<Machine<RISCV32>>(opts);
    machine->setup_minimal_syscalls();

    // load program at boot address
    machine->memory.set_page_attr(0, RAM_SIZE, {.read=true, .write=true, .exec=true});
    machine->cpu.init_execute_area(bytes.data(), LOAD_ADDR, bytes.size());
    machine->cpu.jump(LOAD_ADDR);

    // reset UART to default values
    resetMMIO();
}

void aspire::emu::BaselineEmulator::step() {
	auto& cpu = machine->cpu;
	// Read next instruction
	const auto instruction = cpu.read_next_instruction();
	// Print the instruction to terminal
    spdlog::trace("{}", cpu.to_string(instruction));
	// Execute instruction directly
	cpu.execute(instruction);
	// Increment PC to next instruction, and increment instruction counter
	cpu.increment_pc(instruction.length());
	machine->increment_counter(1);

    // spdlog::info("SP: 0x{:X}", cpu.reg(2));
    
    // check MMIO
    updateMMIO();
}

aspire::emu::State aspire::emu::BaselineEmulator::getState() {
    aspire::emu::State state{};

    state.pc = machine->cpu.pc();
    for (uint32_t i = 0; i < 31; i++) {
        state.regfile[i] = machine->cpu.reg(i);
    }

    return state;
}

void aspire::emu::BaselineEmulator::memdump(const std::string &path) {
    spdlog::info("Dumping contents of RAM to file {}", path);

    // copy RAM locally
    char bytes[RAM_SIZE] = {0};
    memset(bytes, 1, RAM_SIZE);
    machine->memory.memcpy_out(bytes, 0, RAM_SIZE);
    
    // write to file
    std::ofstream stream(path, std::ios::out | std::ios::binary);
    stream.write(bytes, RAM_SIZE);
    stream.close();
}

void aspire::emu::BaselineEmulator::resetMMIO() {
    spdlog::debug("Reset MMIO");
    machine->memory.write(ASPIRE_UART_READY, static_cast<uint8_t>(1));
    machine->memory.write(ASPIRE_WDOG_ENABLE, static_cast<uint8_t>(0));
    wdogRemaining = F_CPU;
    uartBuffer = "";
    //spdlog::info("UART ready: {}", machine->memory.read<uint8_t>(ASPIRE_UART_READY));
}

void aspire::emu::BaselineEmulator::updateMMIO() {
    // uint8_t buf[7] = {0};
    // machine->memory.memcpy_out(buf, ASPIRE_UART_RESET, 7);
    //spdlog::info("MMIO: {} {} {} {} {} {} {}", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);
    //spdlog::info("UART ready: {}", machine->memory.read<uint8_t>(ASPIRE_UART_READY));

    // UART valid flag? (SW requested print)
    const auto uartValid = machine->memory.read<uint8_t>(ASPIRE_UART_VALID);
    if (uartValid) {
        // display the character
        spdlog::trace("UART put requested! Current buffer: {}", uartBuffer);
        const auto character = machine->memory.read<char>(ASPIRE_UART_DATA);

        // if we got a newline, print the UART buffer and reset
        if (character == '\n') {
            spdlog::info("UART: {}", uartBuffer);
            uartBuffer = "";
        } else {
            // otherwise, just append to UART buffer
            uartBuffer += static_cast<char>(character);
        }
        
        // ready to transmit again 
        machine->memory.write(ASPIRE_UART_READY, static_cast<uint8_t>(1));
    }

    // Watchdog enabled?
    const auto wdogEnabled = machine->memory.read<uint8_t>(ASPIRE_WDOG_ENABLE);
    if (wdogEnabled) {
        const auto wdogReset = machine->memory.read<uint8_t>(ASPIRE_WDOG_RESET);
        
        if (wdogReset) {
            spdlog::debug("Watchdog reset!");
            wdogRemaining = F_CPU;
        }
        
        if (wdogRemaining-- <= 0) {
            spdlog::error("Watchdog timeout!");
            throw std::runtime_error("Watchdog timeout!"); // either this or reset CPU
        }
    }
}
