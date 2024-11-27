# Aspire - A RISC-V RV32 Processor
_Aspire_ was a RISC-V CPU that was going to be designed for my Bachelor of Computer Science (Honours) thesis
project at the University of Queensland. The title of the thesis was going to be "A triple modular redundancy
fault tolerant RISC-V processor for safety-critical applications".

Aspire is designed in SystemVerilog, and was going to implement the RV32I_Zmmul_Zicsr instruction set
architecture. The processor was going to use triple-modular redundancy (TMR) to prevent single-event upsets
(SEUs), making it potentially useful in safety-critical and rad-hardened applications. Particular industry
areas where the processor may be useful include space, defence, medical and automotive.

Aspire has instead been replaced by my new thesis project, _TaMaRa_, which is an automated triple modular
redundancy EDA flow for Yosys. This is basically taking the same principles behind Aspire, but making it
applicable to _all_ circuits rather than just this particular CPU.

Despite not being started really, there's probably some use to be gained from the emulator in the `emulation`
directory.

## Getting started with Aspire
### Cloning
This repo uses git submodules, so clone with `git clone --recurse-submodules -j8 git@github.com:mattyoung101/aspire_rv32.git`

### Setting up the toolchain
I'm going to assume you use Arch, because that's what I use (sorry). If you use another distro like Ubuntu, 
the same instructions still apply, but you might have to build some packages from source - Ubuntu ships 
prehistoric versions of a lot of the below packages. In the future, I'll figure out how to get these put into
a Docker to make this all less of a PITA.

**Synthesis tools**

You need Yosys and nextpnr-ecp5. Get the nightlies from the AUR: `yay -S yosys-nightly nextpnr-ecp5-nightly`

The nightly PKGBUILDS are maintained by a Yosys developer, so they might work better than just the -git ones.
In any case, they're both equivalent, as long as you're using the bleeding edge versions of both.

**Verification tools**

You need the following long list of tools:

- CMake 3.20+
- Clang 16+ (or another C++20 compiler, but I only use Clang)
- LLD
- LLVM
- Ninja
- Verilator
- GTKWave
- Icarus Verilog
- Sigrok
- Pulseview
- Flashrom
- spdlog
- Just

All of these are in the regular Arch repos: `yay -S cmake clang lld llvm ninja verilator gtkwave iverilog
sigrok-cli pulseview flashrom spdlog just`

**Generating papers**

Papers are written in LaTeX and built using `latexmk`. You will therefore need TeXLive and latexmk.
You can install as follows: `yay -S texlive texlive-binextra`.

If you install `entr` using `yay -S entr`, you can use `just watch` to automatically recompile the papers
whenever they are modified.

**Developer tools**

If you want to modify the processor, you can use Neovim and my [Slingshot](https://github.com/mattyoung101/slingshot)
SystemVerilog LSP.

### Verifying the processor
TODO

In the `programs` directory, there are some test programs that you can compile with Clang for the processor.
There is a README in that directory explaining the process including setting up the toolchain.

In the `emulation` directory, there is an emulator for the processor. This is used for differential fuzzing 
against a Verilator model to verify the processor and find implementation bugs. There is a README in that 
directory explaining the process.

### Synthesising and deploying the processor
TODO

## Papers
You can use the `Justfile` to generate PDFs from the TeX sources. Just run
`just papers` to generate them all, `just proposal` for the proposal, `just thesis` for the thesis. Try
`just --list` to display all targets. This uses `latexmk` internally.

If you install inotify-tools using `yay -S inotifytools`, the `watchpapers.sh` script will automatically
recompile papers using `inotifywait` when they are modified.

Make sure you have the necessary dependencies as described in Setting up the toolchain.

TODO automatic recompile when changed

## Licence
Copyright (c) 2024 Matt Young.

Code and tests are available under the **Mozilla Public License v2.0**, see the file LICENSE.code.

Papers and slides (in the `papers` directory) are available under **CC Attribution 4.0 International**,
see the file LICENSE.papers.
