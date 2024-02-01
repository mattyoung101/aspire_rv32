// This file implements the baseline emulator for Aspire based on libriscv.
// This is used for differential fuzzing against the Verilator emulator.
#include <iostream>
#include <fstream>
#include <libriscv/machine.hpp>
#include <libriscv/debug.hpp>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    spdlog::info("Aspire Baseline Emulator using libriscv");

    if (argc < 2) {
        spdlog::error("Usage: {} <PATH_TO_BINARY>", argv[0]);
        return 1;
    }

    const std::string program = argv[1];
    spdlog::info("Using program: {}", program);

    // based on https://github.com/fwsGonzo/libriscv/blob/master/examples/embed/example.cpp
    // Read the RISC-V program into a std::vector:
	std::ifstream stream(argv[1], std::ios::in | std::ios::binary);
	if (!stream) {
        spdlog::error("Unable to load program");
		return 1;
	}
	const std::vector<uint8_t> binary(
		(std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>()
	);

    using namespace riscv;
    // Aspire has 32 KiB of RAM
    // FIXME consider loading manually to replicate real chip behaviour
    Machine<RISCV32> machine{binary, {.memory_max = 32768, .allow_write_exec_segment = true, .verbose_loader = true}};
    // Setup minimal syscalls, although we will not be using any.
    machine.setup_minimal_syscalls();

    DebugMachine debug { machine };
	// Print all instructions one by one
	debug.verbose_instructions = true;
	// Break immediately
	debug.print_and_pause();

    return 0;
}
