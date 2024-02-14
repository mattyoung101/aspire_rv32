// Based on : https://github.com/riscv-software-src/riscv-isa-sim/blob/master/arch_test_target/spike/model_test.h
// This file is for use by riscv-arch-test.
#pragma once

// This is for the benefit of clangd, who is too stupid apparently to figure out we did -DXLEN=32.
#ifndef XLEN
#define XLEN 32
#endif

#if XLEN == 64
  #define ALIGNMENT 3
#elif XLEN == 32
  #define ALIGNMENT 2
#else
  #error "Fix your XLEN!"
#endif

#define RVMODEL_DATA_SECTION \
        .align 8; .global begin_regstate; begin_regstate:               \
        .word 128;                                                      \
        .align 8; .global end_regstate; end_regstate:                   \
        .word 4;
        
        // We don't do UART this way.
        // .pushsection .tohost,"aw",@progbits;                            \
        // .align 8; .global tohost; tohost: .dword 0;                     \
        // .align 8; .global fromhost; fromhost: .dword 0;                 \
        // .popsection;                                                    \

// In Aspire, this writes to ASPIRE_SIM_STOP and then infinitely loops.
#define RVMODEL_HALT                                              \
    li t0, 1; /* t0 := 1 */ \
    sb t0, 0x07(zero); /* 0x07 (ASPIRE_SIM_STOP) := t0 */ \
    self_loop:  j self_loop;

// TODO: print "RV" to UART?
#define RVMODEL_BOOT

//RV_COMPLIANCE_DATA_BEGIN
#define RVMODEL_DATA_BEGIN                                              \
  .align 4; .global begin_signature; begin_signature:

//RV_COMPLIANCE_DATA_END
#define RVMODEL_DATA_END                                                      \
  .align 4; .global end_signature; end_signature:  \
  RVMODEL_DATA_SECTION                                                        \

//RVTEST_IO_INIT
#define RVMODEL_IO_INIT
//RVTEST_IO_WRITE_STR
#define RVMODEL_IO_WRITE_STR(_R, _STR)
//RVTEST_IO_CHECK
#define RVMODEL_IO_CHECK()
//RVTEST_IO_ASSERT_GPR_EQ
#define RVMODEL_IO_ASSERT_GPR_EQ(_S, _R, _I)
//RVTEST_IO_ASSERT_SFPR_EQ
#define RVMODEL_IO_ASSERT_SFPR_EQ(_F, _R, _I)
//RVTEST_IO_ASSERT_DFPR_EQ
#define RVMODEL_IO_ASSERT_DFPR_EQ(_D, _R, _I)

#ifndef RVMODEL_MCLINTBASE    
        #define RVMODEL_MCLINTBASE 0x02000000
#endif

#ifndef RVMODEL_MSIP_OFFSET    
        #define RVMODEL_MSIP_OFFSET 0x0
#endif

#ifndef RVMODEL_MTIMECMP_OFFSET    
        #define RVMODEL_MTIMECMP_OFFSET 0x4000
#endif

#ifndef RVMODEL_MTIMECMPH_OFFSET    
        #define RVMODEL_MTIMECMPH_OFFSET 0x4004
#endif

#define RVMODEL_SET_MSW_INT                                                           \
    lui t0,      ((RVMODEL_MCLINTBASE + RVMODEL_MSIP_OFFSET)>> 12);         \
    addi t0, t0, ((RVMODEL_MCLINTBASE + RVMODEL_MSIP_OFFSET) & 0xFFF);      \
    li t1, 1;                                                                         \
    sw t1, (t0);                                                                      \

#define RVMODEL_CLEAR_MSW_INT                                                         \
    lui t0,      ((RVMODEL_MCLINTBASE + RVMODEL_MSIP_OFFSET)>> 12);         \
    addi t0, t0, ((RVMODEL_MCLINTBASE + RVMODEL_MSIP_OFFSET) & 0xFFF);      \
    sw x0, (t0);                                                                      \

#define RVMODEL_SET_MTIMER_INT                                                        \
    lui t0,      ((RVMODEL_MCLINTBASE + RVMODEL_MTIMECMP_OFFSET)>> 12);     \
    addi t0, t0, ((RVMODEL_MCLINTBASE + RVMODEL_MTIMECMP_OFFSET) & 0xFFF);  \
    sw x0, (t0);                                                                      \
    lui t0,      ((RVMODEL_MCLINTBASE + RVMODEL_MTIMECMPH_OFFSET)>> 12);    \
    addi t0, t0, ((RVMODEL_MCLINTBASE + RVMODEL_MTIMECMPH_OFFSET) & 0xFFF); \
    sw x0, (t0);                                                                      \

#define RVMODEL_CLEAR_MTIMER_INT                                                      \
    addi t1,x0,1;                                                          \
    neg t1,t1;                                                                         \
    lui t0,      ((RVMODEL_MCLINTBASE + RVMODEL_MTIMECMPH_OFFSET)>> 12);    \
    addi t0, t0, ((RVMODEL_MCLINTBASE + RVMODEL_MTIMECMPH_OFFSET) & 0xFFF); \
    sw t1, (t0);                                                                      \
    lui t0,      ((RVMODEL_MCLINTBASE + RVMODEL_MTIMECMP_OFFSET)>> 12);     \
    addi t0, t0, ((RVMODEL_MCLINTBASE + RVMODEL_MTIMECMP_OFFSET) & 0xFFF);  \
    sw t1, (t0);                                                                      \

#define RVMODEL_CLEAR_MEXT_INT

