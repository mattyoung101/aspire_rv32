# Aspire - A RISC-V RV32 Processor
_Aspire_ is a RISC-V CPU designed for my Bachelor of Computer Science (Honours) thesis project at
the University of Queensland. The title of the thesis is,
"A triple modular redundancy fault tolerant RISC-V processor for safety-critical applications".

Aspire is designed in SystemVerilog, and implements the RV32I_Zmmul_Zicsr instruction set
architecture. The processor uses triple-modular redundancy (TMR) to prevent single-event upsets (SEUs),
making it potentially useful in safety-critical and rad-hardened applications. Particular industry
areas where the processor may be useful include space, defence, medical and automotive.

**Disclaimer:** Although intended for safety-critical applications, Aspire is a thesis project and
is not certified or audited in any capacity. There is no warranty, and I make no guarantees of 
correctness. Please carefully consider if these factors make Aspire suitable for your use-case.

## Features
- TBA
- Implements RV32I_Zmmul_Zicsr ISA
    - Features a single cycle 32x32-bit multiplier, 64-bit result, both signed/unsigned
    - (Maybe) Supports `clz` from Zbb (no other instructions from that extension though)
- Hardware UART TX peripheral (115200 baud, 8 bit word size, 1 stop bit, no parity)
   - Based on this upstream project: https://github.com/medalotte/SystemVerilog-UART (MIT licence)
   - Minor in-house modifications to simplify it (removed RX, etc)
   - Not currently software reconfigurable, i.e., config (including baud rate!) is fixed in hardware
- Watchdog timer
    - Can be reset, enabled and disabled by software
    - 1 second timeout (not currently software configurable)
    - If the timeout expires, the watchdog peripheral will trigger a hardware system reset automatically
- (Extension) GPIO
    - Based on (TODO upstream)

## Getting started with Aspire
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

You need a C++ compiler that supports C++20. I use Clang 16. You also need CMake, Verilator, GTKWave, Icarus
Verilog, Sigrok and Pulseview.

All of these are in the regular Arch repos: `yay -S cmake clang verilator gtkwave iverilog sigrok-cli
pulseview`

**Generating papers**

Papers are written in LaTeX and built using `latexmk`. You will therefore need TeXLive and latexmk.
You can install as follows: `yay -S texlive texlive-binextra`.

The build script uses [Just](https://github.com/casey/just), a command line runner. You can acquire it with
`cargo install just` if you have Cargo, or I believe it's on the AUR as well.

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
