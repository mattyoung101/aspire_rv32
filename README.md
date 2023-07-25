# Jelly - RISC-V RV32 CPU
Jelly is a RISC-V CPU, designed in SystemVerilog, that implements the RV32IC instruction set architecture.
The project will (hopefully) be started in Semester 2 2024, as part of my Bachelor of Computer Science (Honours)
thesis at the University of Queensland.

Jelly is designed, verified and synthesised using entirely open-source tools.

**Author:** Matt Young (m.young2@uqconnect.edu.au)

## Features
TBA

## Getting started with Jelly
### Setting up the toolchain
**Synthesis tools**

**Verification tools**

**Generating papers**

Papers are written in LaTeX and built using `latexmk`. You will therefore need TeXLive and latexmk. On Arch,
you can install as follows: `yay -S texlive texlive-binextra`.

The build script uses [Just](https://github.com/casey/just), a command line runner. You can acquire it with
`cargo install just`.

### Verifying the processor
TBA

### Synthesising the processor
TBA

## Implementation details
TBA

## Papers
You can use the `Justfile` to generate PDFs from the TeX sources. Just run
`just papers` to generate them all, `just proposal` for the proposal, `just thesis` for the thesis. Try
`just --list` to display all targets. This uses `latexmk` internally.

Make sure you have the necessary dependencies as described in Setting up the toolchain.

## Licence
SystemVerilog code: Mozilla Public License v2.0 (TODO probably, would need to check with UQ)

Paper: TODO something (CC-BY-SA? Again check with UQ)
