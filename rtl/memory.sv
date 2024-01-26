`default_nettype none
`include "util.sv"

// This module implements Aspire's main memory for an FPGA.
// This is ideally implemented entirely as FPGA BRAM -> this inference will be done by the synthesis tool.
// Since Aspire is von Neumann, this module encapsulates both IMEM and DMEM.
