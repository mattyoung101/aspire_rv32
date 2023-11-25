# Towards Dynamically Reconfigurable Processor Microarchitectures for RISC-V

## Abstract
Modern processors are typically designed with the goals of performance, power, and area (PPA) in mind. However, a lot
of the time, the goals of maximising performance while minimising power can be conflicting. In this thesis, 
I introduce a novel 32-bit RISC-V RV32IC processor that allows its microarchitecture to be partially reconfigured at runtime.
Users can write to architectural registers to dynamically gate off sections of the processor's pipeline, allowing
them to target low power, high performance, or any combination of the two. The processor is designed in SystemVerilog
and synthesised to a Lattice ECP5 FPGA using entirely open-source tools. Both synthesis-driven and real-world power
measurements are taken against a CoreMark benchmark. Future enhancements, including designing a Linux-capable RV64GC 
processor, are discussed.
