# Aspire - A RISC-V RV32 Processor
_Aspire_ is a RISC-V CPU, designed in SystemVerilog, that implements the RV32IC_Zmmul_Zicsr instruction set
architecture. The project will (hopefully) be started in Semester 2 2024, as part of my Bachelor of Computer
Science (Honours) thesis at the University of Queensland.

Aspire is designed, verified and synthesised entirely by me, from scratch, using only open-source tools.

**Author:** Matt Young (m.young2@uqconnect.edu.au)

## Features
- TBA
- Implements RV32IC_Zmmul_Zicsr ISA
    - Features a single cycle 32x32-bit multiplier, 64-bit result, both signed/unsigned
    - (Maybe) Supports `clz` from Zbb (no other instructions from that extension though)
- Hardware UART TX peripheral (115200 baud, 8 bit word size, 1 stop bit, no parity)
   - Based on this upstream project: https://github.com/medalotte/SystemVerilog-UART (MIT licence)
   - Minor in-house modifications to simplify

## Getting started with Aspire
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

You need a C++ compiler that supports C++20. I use Clang 16. You also need CMake, Verilator, GTKWave, Icarus
Verilog, Sigrok and Pulseview.

All of these are in the regular Arch repos: `yay -S cmake clang verilator gtkwave iverilog sigrok-cli
pulseview`

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
SystemVerilog code: (TODO check with UQ, one of MPL 2.0 or Apache)

Paper: TODO check with UQ, hopefully CC-BY or something?

The RTL will most likely initially be released under the Mozilla Public License v2.0, a nice hybrid
permissive-copyleft licence. However, there are already a gazillion RISC-V implementations out there in the
wild, so it may be better to keep the licence simple and permissive, like the Apache 2.0. I'm still
considering. Might also consider even more lax licences like the 0BSD.

As a UQ student, I may also be subject to the university's whims regarding licencing and IP. I will need to
check that I am legally the sole owner of this project's IP and can open source it before doing so. To the
best of my knowledge, as an honours student I am able to keep my IP, but this needs thorough checking.

I'm planning to release the paper LaTeX sources under CC-BY, but I need to check with the uni's academic
integrity policy to make sure this is allowed. I'm also hoping to convert this thesis into a paper and publish
in an open access journal like IEEE Access, or at least arXiv.

---

```
        ##                                                      
     /####                              #                       
    /  ###                             ###                      
       /##                              #                       
      /  ##                                                     
      /  ##          /###      /###   ###   ###  /###     /##   
     /    ##        / #### /  / ###  / ###   ###/ #### / / ###  
     /    ##       ##  ###/  /   ###/   ##    ##   ###/ /   ### 
    /      ##     ####      ##    ##    ##    ##       ##    ###
    /########       ###     ##    ##    ##    ##       ######## 
   /        ##        ###   ##    ##    ##    ##       #######  
   #        ##          ### ##    ##    ##    ##       ##       
  /####      ##    /###  ## ##    ##    ##    ##       ####    /
 /   ####    ## / / #### /  #######     ### / ###       ######/ 
/     ##      #/     ###/   ######       ##/   ###       #####  
#                           ##                                  
 ##                         ##                                  
                            ##                                  
                             ##                                 
                                                                
                     ~~~~~~~~~~~~~~~~~~~~~~~~~~~                                            
                     ~~ THE DESIRE TO SUCCEED ~~       
                     ~~~~~~~~~~~~~~~~~~~~~~~~~~~
```
