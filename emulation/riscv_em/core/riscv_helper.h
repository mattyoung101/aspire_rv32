#ifndef RISCV_HELPER_H
#define RISCV_HELPER_H

#include <stdlib.h>
#include <stdarg.h>

#include <riscv_types.h>
#include <riscv_xlen_specifics.h>

#ifdef RISCV_EM_DEBUG
#define DEBUG_PRINT(...) do{ printf( __VA_ARGS__ ); } while( 0 )
#else
#define DEBUG_PRINT(...) do{ } while ( 0 )
#endif

#define die_msg(...) { printf(__VA_ARGS__); exit(-1); }

#define ADDR_WITHIN(_addr, _start, _size) ( (_addr >= _start) && (_addr < (_start + _size)) )
#define ADDR_WITHIN_LEN(_addr, _len, _start, _size) ( (_addr >= _start) && ((_addr + _len) <= (_start + _size)) )

#define ADDR_ALIGN_DOWN(n, m) ((n) / (m) * (m))

#define ASSIGN_MIN(a,b) (((a)<(b))?(a):(b))
#define ASSIGN_MAX(a,b) (((a)>(b))?(a):(b))

static inline void umul64wide (uint64_t a, uint64_t b, uint64_t *hi, uint64_t *lo)
{
    uint64_t a_lo = (uint32_t)a;
    uint64_t a_hi = a >> 32;
    uint64_t b_lo = (uint32_t)b;
    uint64_t b_hi = b >> 32;

    uint64_t p0 = a_lo * b_lo;
    uint64_t p1 = a_lo * b_hi;
    uint64_t p2 = a_hi * b_lo;
    uint64_t p3 = a_hi * b_hi;

    uint32_t cy = (uint32_t)(((p0 >> 32) + (uint32_t)p1 + (uint32_t)p2) >> 32);

    *lo = p0 + (p1 << 32) + (p2 << 32);
    *hi = p3 + (p1 >> 32) + (p2 >> 32) + cy;
}

static inline void mul64wide (int64_t a, int64_t b, int64_t *hi, int64_t *lo)
{
    umul64wide ((uint64_t)a, (uint64_t)b, (uint64_t *)hi, (uint64_t *)lo);
    if (a < 0LL) *hi -= b;
    if (b < 0LL) *hi -= a;
}

static inline void mulhsu64wide (int64_t a, uint64_t b, int64_t *hi, int64_t *lo)
{
    umul64wide ((uint64_t)a, (uint64_t)b, (uint64_t *)hi, (uint64_t *)lo);
    if (a < 0LL) *hi -= b;
}

static inline void umul32wide (uint32_t a, uint32_t b, uint32_t *hi, uint32_t *lo)
{
    uint32_t a_lo = (uint16_t)a;
    uint32_t a_hi = a >> 16;
    uint32_t b_lo = (uint16_t)b;
    uint32_t b_hi = b >> 16;

    uint32_t p0 = a_lo * b_lo;
    uint32_t p1 = a_lo * b_hi;
    uint32_t p2 = a_hi * b_lo;
    uint32_t p3 = a_hi * b_hi;

    uint32_t cy = (uint16_t)(((p0 >> 16) + (uint16_t)p1 + (uint16_t)p2) >> 16);

    *lo = p0 + (p1 << 16) + (p2 << 16);
    *hi = p3 + (p1 >> 16) + (p2 >> 16) + cy;
}

static inline void mul32wide (int32_t a, int32_t b, int32_t *hi, int32_t *lo)
{
    umul32wide ((uint32_t)a, (uint32_t)b, (uint32_t *)hi, (uint32_t *)lo);
    if (a < 0LL) *hi -= b;
    if (b < 0LL) *hi -= a;
}

static inline void mulhsu32wide (int32_t a, uint32_t b, int32_t *hi, int32_t *lo)
{
    umul32wide ((uint32_t)a, (uint32_t)b, (uint32_t *)hi, (uint32_t *)lo);
    if (a < 0LL) *hi -= b;
}

/*
 * Bit Operators 
 */
#define SET_BIT(_out_var,_nbit)   ((_out_var) |=  (1<<(_nbit)))
#define CLEAR_BIT(_out_var,_nbit) ((_out_var) &= ~(1<<(_nbit)))
#define FLIP_BIT(_out_var,_nbit)  ((_out_var) ^=  (1<<(_nbit)))
#define CHECK_BIT(_out_var,_nbit) ((_out_var) &   (1<<(_nbit)))
#define FIND_FIRST_BIT_SET(_var) (__builtin_ffsl(_var))

static inline void assign_u8_bit(uint8_t *out_var, uint8_t nbit, uint8_t bit_value)
{
    *out_var = (*out_var & ~(1UL << nbit)) | ((bit_value & 1) << nbit);
}

static inline void assign_u32_bit(uint32_t *out_var, uint32_t nbit, uint32_t bit_value)
{
    *out_var = (*out_var & ~(1UL << nbit)) | ((bit_value & 1) << nbit);
}

static inline void assign_xlen_bit(rv_uint_xlen *out_var, rv_uint_xlen nbit, rv_uint_xlen bit_value)
{
    *out_var = (*out_var & ~(1UL << nbit)) | ((bit_value & 1) << nbit);
}

static inline void assign_xlen_value_within_reg(rv_uint_xlen *out_var, rv_uint_xlen nbit, rv_uint_xlen value, rv_uint_xlen mask)
{
    *out_var = (*out_var & ~(mask << (nbit))) | ((value & mask) << (nbit));
}

static inline uint8_t extract8(uint8_t value, int start, int length)
{
    return (value >> start) & (0xFF >> (8 - length));
}

static inline uint32_t extract32(uint32_t value, int start, int length)
{
    return (value >> start) & (0xFFFFFFFF >> (32 - length));
}

static inline rv_uint_xlen extractxlen(rv_uint_xlen value, int start, int length)
{
    return (value >> start) & (((rv_uint_xlen)-1) >> ((sizeof(rv_uint_xlen)*8) - length));
}

#endif /* RISCV_HELPER_H */
