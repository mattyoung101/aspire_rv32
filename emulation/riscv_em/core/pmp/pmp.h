#ifndef RISCV_PMP_H
#define RISCV_PMP_H

#include <stdint.h>
#include <riscv_types.h>

#ifdef RV64
    #define PMP_NR_CFG_REGS 2
#else
    #define PMP_NR_CFG_REGS 4
#endif
#define PMP_NR_CFG_REGS_WARL_MAX 16

#define PMP_NR_ADDR_REGS 16
#define PMP_NR_ADDR_REGS_WARL_MAX 64

/* Lock bit */
#define PMP_CFG_L_BIT 7
/* Adress matching bit offset */
#define PMP_CFG_A_BIT_OFFS 3
/* Executable bit */
#define PMP_CFG_X_BIT 2
/* Writable bit */
#define PMP_CFG_W_BIT 1
/* Readable bit */
#define PMP_CFG_R_BIT 0

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    pmp_a_off = 0,
    pmp_a_tor,
    pmp_a_na4,
    pmp_a_napot

} pmp_addr_matching;

typedef struct pmp_struct
{
    union {
        struct {
            rv_uint_xlen cfg[PMP_NR_CFG_REGS];
            rv_uint_xlen addr[PMP_NR_ADDR_REGS];
        };
        rv_uint_xlen regs[PMP_NR_CFG_REGS+PMP_NR_ADDR_REGS];
    };

} pmp_td;

rv_ret pmp_write_csr_cfg(void *priv, privilege_level curr_priv, uint16_t reg_index, rv_uint_xlen csr_val);
rv_ret pmp_read_csr_cfg(void *priv, privilege_level curr_priv, uint16_t reg_index, rv_uint_xlen *out_val);
rv_ret pmp_write_csr_addr(void *priv, privilege_level curr_priv, uint16_t reg_index, rv_uint_xlen csr_val);
rv_ret pmp_read_csr_addr(void *priv, privilege_level curr_priv, uint16_t reg_index, rv_uint_xlen *out_val);
rv_ret pmp_mem_check(pmp_td *pmp, privilege_level curr_priv, rv_uint_xlen addr, uint8_t len, bus_access_type access_type);
void pmp_dump_cfg_regs(pmp_td *pmp);

#ifdef __cplusplus
}
#endif

#endif /* RISCV_PMP_H */
