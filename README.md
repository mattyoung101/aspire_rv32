# "TBA" - RISC-V RV32 CPU
"TBA" is a RISC-V CPU, designed in SystemVerilog, that implements the RV32IC instruction set architecture.
The project will (hopefully) be started in Semester 2 2024, as part of my Bachelor of Computer Science (Honours)
thesis at the University of Queensland.

Jelly is designed, verified and synthesised entirely by me, from scratch, using only open-source tools.

**Author:** Matt Young (m.young2@uqconnect.edu.au)

## Features
- Implements RISC-V RV32IC ISA (base + "C" Compressed Instructions extension)
- Written in SystemVerilog
- Synthesisable for Lattice ECP5 FPGA and theoretically VLSI

## Getting started with "TBA"
### Setting up the toolchain
I'm going to assume you use Arch, because that's what I use (sorry). If you use another distro like Ubuntu, 
the same instructions still apply, but you might have to build some packages from source - Ubuntu ships 
prehistoric versions of a lot of the below packages. In the future, I'll figure out how to get these put into
a Docker to make this all less of a PITA.

**Synthesis tools**

You need Yosys and nextpnr-ecp5. Get the nightlies from the AUR: `yay -S yay -S yosys-nightly nextpnr-ecp5-nightly`

The nightly PKGBUILDS are maintained by a Yosys developer, so they might work better than just the -git ones.
In any case, they're both equivalent, as long as you're using the bleeding edge versions of both.

**Verification tools**

You need a C++ compiler that supports C++20. I use Clang 16. You also need CMake, Verilator, GTKWave and Icarus
Verilog.

All of these are in the regular Arch repos: `yay -S clang verilator gtkwave iverilog`

**Generating papers**

Papers are written in LaTeX and built using `latexmk`. You will therefore need TeXLive and latexmk.
You can install as follows: `yay -S texlive texlive-binextra`.

The build script uses [Just](https://github.com/casey/just), a command line runner. You can acquire it with
`cargo install just`.

**Developer tools**

If you want to modify the processor, you can use Neovim and my [Slingshot](https://github.com/mattyoung101/slingshot)
SystemVerilog LSP.

### Verifying the processor
TBA

In the `programs` directory, there are some test programs that you can compile with Clang for the processor.
There is a README in that directory explaining the process.

### Synthesising the processor
TBA

## Papers
You can use the `Justfile` to generate PDFs from the TeX sources. Just run
`just papers` to generate them all, `just proposal` for the proposal, `just thesis` for the thesis. Try
`just --list` to display all targets. This uses `latexmk` internally.

Make sure you have the necessary dependencies as described in Setting up the toolchain.

## Licence
SystemVerilog code: Mozilla Public License v2.0 (TODO probably, would need to check with UQ)

Paper: TODO something (CC-BY-SA? Again check with UQ)
