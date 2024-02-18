#include "CLI11/CLI11.hpp"
#include "aspire/baseline_emulator.hpp"
#include "pcg/pcg_random.hpp"
#include <cerrno>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <spdlog/spdlog.h>

// This file implements the main entry point of the emulator.

pcg32 rng(0x1337);
bool faultInjection = false;
uint64_t faultInjectionDelay = 1'000'000;
double faultInjectionChance = 0.25;
bool trace = false;
bool ramdump = false;
bool perf = false;

size_t steps = 0;
double sumNs = 0.0;

int main(int argc, char *argv[]) {
    // setup CLI
    CLI::App app { "Aspire Emulator" };
    argv = app.ensure_utf8(argv);

    std::string mode = "";
    app.add_option("-m,--mode", mode, "Emulation mode (one of 'baseline', 'verilator' or 'differential')")
        ->required();
    app.add_flag("--inject", faultInjection, "Enable fault injection");
    app.add_option("--inject-delay", faultInjectionDelay, "Query fault injection every this many cycles");
    app.add_option("--inject-chance", faultInjectionChance, "Fault injection chance (0.0 to 1.0)");
    app.add_flag("--trace", trace, "Enable trace mode (very verbose)");
    app.add_flag("--ramdump", ramdump, "Dump RAM at end of program");
    app.add_flag("--perf", perf, "Print emulator performance diagnostics");

    std::string program = "";
    app.add_option("-p,--program", program, "Path to .bin file to load")
        ->required()
        ->check(CLI::ExistingFile);

    // parse CLI
    CLI11_PARSE(app, argc, argv);

    if (trace) {
        spdlog::set_level(spdlog::level::trace);
    } else {
        spdlog::set_level(spdlog::level::debug);
    }
    SPDLOG_INFO("Aspire Emulator - (c) 2024 Matt Young.");

    // check mode
    if (mode != "baseline" && mode != "verilator" && mode != "differential") {
        SPDLOG_ERROR("Invalid mode: {}", mode);
        return 1;
    }

    // check fault injection
    if (faultInjectionChance <= 0.0 || faultInjectionChance > 1.0) {
        SPDLOG_ERROR(
            "Fault injection chance must be between 0.0 and 1.0, you said: {}", faultInjectionChance);
        return 1;
    }

    // dump fault injection config
    if (faultInjection) {
        SPDLOG_INFO("Fault injection: delay: {}, chance: {}", faultInjectionDelay, faultInjectionChance);
    }

    // based on https://github.com/fwsGonzo/libriscv/blob/master/examples/embed/example.cpp
    // Read the RISC-V program into a std::vector
    SPDLOG_INFO("Loading program: {}", program);
    std::ifstream stream(program, std::ios::in | std::ios::binary);
    if (!stream) {
        SPDLOG_ERROR("Unable to load program: {}", strerror(errno));
        return 1;
    }
    const std::vector<uint8_t> binary(
        (std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    // initialise baseline emulator
    aspire::emu::BaselineEmulator baseline(binary);

    // initialise the Verilator emulator
    // TODO

    // this distribution controls the chance that a fault is injected every "faultInjectionDelay" ticks
    std::uniform_real_distribution<float> faultDist(0.0, 1.0);
    // this distribution is used both to determine which register, and which bit in that register, is flipped
    std::uniform_int_distribution<uint32_t> bitDist(0, 31);

    // run the simulation
    while (true) {
        auto begin = std::chrono::steady_clock::now();

        // handle fault injection before tick
        if (faultInjection && steps % faultInjectionDelay == 0 && faultDist(rng) <= faultInjectionChance) {
            auto wordIdx = bitDist(rng);
            auto bitIdx = bitDist(rng);
            SPDLOG_INFO("Inject: Going to inject a fault this tick, reg {}, bit {}", wordIdx, bitIdx);
            baseline.injectFaultAt(wordIdx, bitIdx);
        }

        baseline.step();

        auto end
            = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - begin)
                  .count();

        // accumulate average TODO use windowed average
        sumNs += end;
        steps++;

        // print performance metrics
        if (steps % 1'000'000 == 0) {
            double average = sumNs / static_cast<double>(steps);
            double hz = 1.0 / (average / 1e+9);
            double mhz = hz / 1'000'000.0;
            if (perf)
                SPDLOG_INFO("Performance: {:.2f} ns ({:.2f} MHz)", average, mhz);
        }

        if (baseline.exitRequested) {
            SPDLOG_INFO("Exiting simulation as requested");
            break;
        }
    }

    if (ramdump) {
        baseline.memdump("/tmp/aspire_dump.bin");
    }

    SPDLOG_INFO("Executed {} cycles", steps);

    return 0;
}
