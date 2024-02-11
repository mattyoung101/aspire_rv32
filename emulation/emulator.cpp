#include <cerrno>
#include <iostream>
#include <fstream>
#include <chrono>
#include <spdlog/spdlog.h>
#include "aspire/baseline_emulator.hpp"
#include "aspire/state.hpp"
#include "CLI11.hpp"

// This file implements the main entry point of the emulator.

int main(int argc, char *argv[]) {
    // setup CLI
    CLI::App app{"Aspire Emulator"};
    argv = app.ensure_utf8(argv);

    bool trace = false;
    app.add_option("-t,--trace", trace, "Enable trace mode (very verbose)");

    std::string program = "";
    app.add_option("-p,--program", program, "Path to .bin file to load")->required()->check(CLI::ExistingFile);

    // parse CLI
    CLI11_PARSE(app, argc, argv);
    
    if (trace) {
        spdlog::set_level(spdlog::level::trace);
    } else {
        spdlog::set_level(spdlog::level::debug);
    }
    spdlog::info("Aspire Emulator - (c) 2024 Matt Young.");
    spdlog::info("Loading program: {}", program);

    // based on https://github.com/fwsGonzo/libriscv/blob/master/examples/embed/example.cpp
    // Read the RISC-V program into a std::vector
	std::ifstream stream(program, std::ios::in | std::ios::binary);
	if (!stream) {
        spdlog::error("Unable to load program: {}", strerror(errno));
		return 1;
	}
	const std::vector<uint8_t> binary((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>()
	);

    // initialise baseline emulator
    aspire::emu::BaselineEmulator baseline(binary);

    // initialise the Verilator emulator

    // run the simulation
    size_t steps = 0;
    double sumNs = 0.0;

    while (true) {
        auto begin = std::chrono::steady_clock::now();
        baseline.step();
        auto end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - begin).count();
        
        // accumulate average TODO use windowed average
        sumNs += end;
        steps++;

        if (steps % 1'000'000 == 0) {
            double average = sumNs / static_cast<double>(steps);
            double hz = 1.0 / (average / 1e+9);
            double mhz = hz / 1'000'000.0;
            spdlog::info("Performance: {:.2f} ns ({:.2f} MHz)", average, mhz);
        }

        if (baseline.exitRequested) {
            spdlog::info("Exiting simulation as requested");
            break;
        }
    }

    baseline.memdump("/tmp/aspire_dump.bin");

    return 0;
}
