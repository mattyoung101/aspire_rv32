## Programs directory
This directory contains various test programs for the processor. There are a few basic test programs, as well as
CoreMark, which is the main target program of this processor. I also want SPEC CPU 2017, but it costs money :( 
(and is probably annoying to port)

## Building
### Prerequisites
You will need:
- CMake 3.20+ 
- Ninja
- Clang
- LLD

On Arch you can install these using using: `yay -S cmake ninja clang lld llvm`

### Compiling the toolchain from source
Yes, you read right. Basically, get trolled, we have to compile the GNU toolchain (_only as a prerequisite_) 
and the LLVM toolchain (_the actually compiler we'll be using_) from source. Why? It's due to ABI issues, the
upstream riscv-gnu-toolchain project only ships floating point libcs, but my processor doesn't include an FPU,
so we need the `ilp32` ABI, which means a source build. 

> Note that this project is making a conscious choice to
use the LLVM toolchain over the GNU one, hence why we are building LLVM. GNU is only currently required as a
prerequisite to build the LLVM toolchain (although I think it may be possible to avoid this, given enough more
hacking time).

1. Clone riscv-gnu-toolchain: https://github.com/riscv-collab/riscv-gnu-toolchain (I cloned this to `~/build`)
2. Install the dependencies listed in the README in that repo
3. Create an installation directory, I'm using `~/build/riscv-ilp32`. This is where the toolchain will end up.
4. Export the installation dir as `$RISCV`. In fish, I use `set -x RISCV /home/matt/build/riscv-ilp32`
5. Optionally set Clang as the compiler, `set -x CC clang; set -x CXX clang++` to compile GCC with Clang :)
6. Run `./configure --prefix=$RISCV --enable-llvm --disable-linux --with-arch=rv32i_zmmul_zicsr_zifencei --with-abi=ilp32` <sup>NOTE1</sup>
7. Run `make -j$(nproc)`.
8. Wait some time... (~15 minutes on my Ryzen 9 5950X)
9. Done! You can run `find . -exec llvm-strip --strip-unneeded {} \;` in `$RISCV/bin` to reduce the size of 
   the toolchain a lot, as the executables aren't stripped during build.

**NOTE1:** We have to build the toolchain with Zifencei, even though Aspire doesn't support it. Aspire is
currently expected to translate any Zifencei instructions to either illegal or no-op. We have to build with
Zifencei because the proxy kernel seems to require it. TL;DR Spike supports Zifencei, but the Aspire won't.

FIXME the above may not be true any more since we aren't using Spike

TODO: consider dockerizing this process

For the linker, I was initially planning to use the LLD built into the RISC-V toolchain, but due to an
[LLD bug](https://github.com/llvm/llvm-project/issues/64102) it appears it won't link modern RISC-V
binutils. For the time being, we are forced to use GNU ld.

## Building the programs
Now that we've built an LLVM RISC-V RV32IC ilp32 ABI toolchain (phew, mouthful), we can compile CoreMark.
You need to export the environment variable `RISCV_LLVM_HOME` which is the path to the build directory for the toolchain you
just built. So in my case, it's `set -x RISCV_LLVM_HOME /home/matt/build/riscv-ilp32`

Now you can just run `./build.sh` and CMake will take care of everything else.

## Programs
### CoreMark
The coremark repo is just a submodule to the upstream CoreMark. 

The CoreMark port is based on the baremetal example from the CoreMark repo. The contents of the `lib`
directory is mainly taken from riscv-tests:
https://github.com/riscv-software-src/riscv-tests/tree/master/benchmarks/common

It's used to implement the low-level initialisation routines such as the CRT, linker script and various
common routines like `memcpy` and `memset`.

We could also use Newlib's implementation of all of the above using `nano.specs` and `nosys.specs`, but these
specs files would require us to move to GCC. Plus, Newlib has a lot of overhead and if we can keep each binary
<64K, including CoreMark, we get more BRAM space to dedicate to other things on the chip.

### Hello world
This just prints some ASCII art in a loop as a base test.
