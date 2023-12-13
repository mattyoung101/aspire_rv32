## Programs directory
This directory contains various test programs for the processor. There are a few basic test programs, as well as
serious benchmarks like Dhrystone, CoreMark. I also want SPEC CPU 2017, but it costs money :(

TODO ask UQ for a SPEC CPU 2017 Academic licence ($50 USD)

## Building
### Prerequisites
You will need:
- CMake 3.20+ 
- Ninja
- mold (linker)
- and probably some RISC-V simulator, todo.

### Compiling the toolchain from source
Yes, you read right. Basically, get trolled, we have to compile the GNU toolchain (_only as a prerequisite_) 
and the LLVM toolchain (the actually compiler we'll be using) from source. Why? It's due to ABI issues, the
upstream riscv-gnu-toolchain project only ships floating point libcs, but my processor doesn't include an FPU,
so we need the `ilp32` ABI, which means a source build.

1. Clone riscv-gnu-toolchain: https://github.com/riscv-collab/riscv-gnu-toolchain (I cloned this to ~/build)
2. Install the dependencies based on your system listed in the README in that repo
3. Create an installation directory, I'm using ~/build/riscv-ilp32
4. Now, export this path, in fish I use `set -x RISCV /home/matt/build/riscv-ilp32`
5. You can export Clang as the compiler, `set -x CC clang; set -x CXX clang++`, I forgot to do this
   unfortunately so my toolchain is built with GNU.
6. Run `./configure --prefix=$RISCV --enable-llvm --disable-linux --with-arch=rv32ic_zmmul --with-abi=ilp32`

For the linker, I was initially planning to use the LLD built into the RISC-V toolchain, but due to a
[bug](https://www.mail-archive.com/llvm-bugs@lists.llvm.org/msg67576.html) it appears it won't link RISC-V
binutils. However, [mold](https://github.com/rui314/mold), which as of writing is now MIT licenced, is a good
substitute and very fast.

## Building CoreMark
Now that we've built an LLVM RISC-V RV32IC ilp32 ABI toolchain (phew, mouthful), we can compile CoreMark.
You need to export a variable `RISCV_LLVM_HOME` which is the path to the build directory for the toolchain you
just built. So in my case, it's `set -x RISCV_LLVM_HOME /home/matt/build/riscv-ilp32`

Now you can just run `./build.sh` and CMake will take care of everything else.

## CoreMark
The coremark repo is just a submodule to the upstream CoreMark. The riscv-coremark is from Berkeley. My
original goal for this whole programs directory was to get it running under CMake and Clang, rather than 
Make and GCC, so here will be some build script modifications to make it work. However, it seems that there is
an unavoidable GCC dependency in the form of requiring libc and libm, 

The CoreMark port is courtesy of riscv-ovpsim: https://github.com/riscv-ovpsim/imperas-riscv-tests/tree/v20231026/riscv-ovpsim/examples/CoreMark
