#ifndef RISCV_CONFIG_H
#define RISCV_CONFIG_H

#define CSR_SUPPORT /* This is currently mandatory, because of the qemu reset-vector */
//#define ATOMIC_SUPPORT
#define MULTIPLY_SUPPORT
//#define PMP_SUPPORT

#define MROM_BASE_ADDR 0x80  // same as LOAD_ADDR in config.hpp

#define RV_EXTENSION_TO_MISA(extension) (1 << (extension - 'A'))
#define RV_SUPPORTED_EXTENSIONS ( RV_EXTENSION_TO_MISA('I') | \
                                  RV_EXTENSION_TO_MISA('M') | \
                                  RV_EXTENSION_TO_MISA('A') | \
                                  RV_EXTENSION_TO_MISA('S') | \
                                  RV_EXTENSION_TO_MISA('U') )

#endif /* RISCV_CONFIG_H */
