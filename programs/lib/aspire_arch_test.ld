/* Based on: https://github.com/YosysHQ/picorv32/blob/master/picosoc/sections.lds */

/* Version of the Aspire linker script for the RISC-V arch tests (different entry point) */

OUTPUT_ARCH(riscv)
ENTRY(rvtest_entry_point)

MEMORY
{
    IMEM (rx)        : ORIGIN = 0x000080, LENGTH = 0x10000 /* IMEM (program), 64 KiB */
    DMEM (xrw)       : ORIGIN = 0x010081, LENGTH = 0x0F800 /* DMEM (RAM), 62 KiB */
}

SECTIONS {
    /* The program code and other data goes into FLASH */
    .text :
    {
        . = ALIGN(4);
        *(.text.init)      /* Force init to go first in the executable */
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        *(.srodata)        /* .rodata sections (constants, strings, etc.) */
        *(.srodata*)       /* .rodata* sections (constants, strings, etc.) */
        . = ALIGN(4);
    } >IMEM

    /* This is the .data section which is different to picorv32 */
    .data : 
    {
        . = ALIGN(4);
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        
        /* global pointer */
        . = ALIGN(8);
        PROVIDE( __global_pointer$ = . + 0x800 );

        *(.sdata)           /* .sdata sections */
        *(.sdata*)          /* .sdata* sections */
        . = ALIGN(4);
    } >DMEM

    /* Uninitialized data section */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* define a global symbol at bss start; used by startup code */
        *(.bss)
        *(.bss*)
        *(.sbss)
        *(.sbss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;         /* define a global symbol at bss end; used by startup code */
    } >DMEM
    
    /* No heap on Aspire (currently) */
}
