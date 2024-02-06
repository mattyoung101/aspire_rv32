// This file implements the differential fuzzing simulation.
#include <iostream>
#include <fstream>
#include <libriscv/machine.hpp>
#include <libriscv/debug.hpp>
#include <spdlog/spdlog.h>
#include "aspire/baseline_emulator.hpp"
#include "aspire/state.hpp"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Aspire Differential Fuzzer - (c) 2024 Matt Young.");

    if (argc < 2) {
        spdlog::error("Usage: {} <BIN_FILE>", argv[0]);
        return 1;
    }

    const std::string program = argv[1];
    spdlog::info("Loading program: {}", program);

    // based on https://github.com/fwsGonzo/libriscv/blob/master/examples/embed/example.cpp
    // Read the RISC-V program into a std::vector
	std::ifstream stream(argv[1], std::ios::in | std::ios::binary);
	if (!stream) {
        spdlog::error("Unable to load program");
		return 1;
	}
	const std::vector<uint8_t> binary((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>()
	);

    // initialise baseline emulator
    aspire::emu::BaselineEmulator baseline(binary);

    // initialise the Verilator emulator

    // run the simulation

    return 0;
}
