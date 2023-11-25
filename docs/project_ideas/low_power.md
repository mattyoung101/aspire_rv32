# Low Power RISC-V Processor Microarchitecture Design

## Abstract
With worldwide sales of Internet of Things (IoT) devices increasing, reducing the power consumption of embedded
processors has become extremely important. The power usage of an existing processor can be reduced by using
lower power silicon fabrication techniques. However, significantly more power reduction can be achieved by
redesigning the processor's microarchitecture with low power goals in mind. To demonstrate how low
power microarchitectures can be designed, in this thesis I will design a simple 32-bit RISC-V RV32IC processor,
and demonstrate how various microarchitectural changes to this processor decrease its power usage significantly.
I demonstrate this both in simulation, and in the real-world by synthesising the processor for a Lattice ECP5 FPGA.



The most obvious power reduction technique in modern VLSI design is the use of complementary metal oxide semiconductor
(CMOS) transistors, which negate the static power consumption of both classical NMOS and PMOS transistors [16].
However, most modern foundries will offer additional fabrication nodes that reduce power consumption further still.
One example is TSMC, who offers nodes such as the 16NMLP [17] node that specifically targets low power design.
