#include "blas_u32.h"
#include "gf.h"

#include <stddef.h>

void PQCLEAN_NAMESPACE_gf256v_predicated_add_u32(uint8_t *accu_b, uint8_t predicate, const uint8_t *a, size_t _num_byte) {
    uint8_t pr_u8 = (uint8_t) ((uint8_t) 0 - predicate);
    for (size_t i = 0; i < _num_byte; i++) {
        accu_b[i] ^= (a[i] & pr_u8);
    }
}

void PQCLEAN_NAMESPACE_gf256v_add_u32(uint8_t *accu_b, const uint8_t *a, size_t _num_byte) {
    for (size_t i = 0; i < _num_byte; i++) {
        accu_b[i] ^= a[i];
    }
}

#ifdef _USE_GF16

void PQCLEAN_NAMESPACE_gf16v_mul_scalar_u32(uint8_t *a, uint8_t gf16_b, size_t _num_byte) {
    uint8_t tmp;
    for (size_t i = 0; i < _num_byte; i++) {
        tmp   = PQCLEAN_NAMESPACE_gf16_mul(a[i] & 0xF, gf16_b);
        tmp  |= (uint8_t) (PQCLEAN_NAMESPACE_gf16_mul(a[i] >> 4,  gf16_b) << 4);
        a[i] = tmp;
    }
}

void PQCLEAN_NAMESPACE_gf16v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf16_b, size_t _num_byte) {
    for (size_t i = 0; i < _num_byte; i++) {
        accu_c[i] ^= PQCLEAN_NAMESPACE_gf16_mul(a[i] & 0xF, gf16_b);
        accu_c[i] ^= (uint8_t) (PQCLEAN_NAMESPACE_gf16_mul(a[i] >> 4, gf16_b) << 4);
    }
}

uint8_t PQCLEAN_NAMESPACE_gf16v_dot_u32(const uint8_t *a, const uint8_t *b, size_t _num_byte) {
    uint8_t r = 0;
    for (size_t i = 0; i < _num_byte; i++) {
        r ^= PQCLEAN_NAMESPACE_gf16_mul(a[i], b[i]);
    }
    return r;
}

#else

void PQCLEAN_NAMESPACE_gf256v_mul_scalar_u32(uint8_t *a, uint8_t b, unsigned int _num_byte) {
    unsigned int n_u32 = _num_byte >> 2;
    uint32_t *a_u32 = (uint32_t *)a;
    for (unsigned int i = 0; i < n_u32; i++) {
        a_u32[i] = PQCLEAN_NAMESPACE_gf256v_mul_u32(a_u32[i], b);
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    t.u32 = 0;
    a += (n_u32 << 2);
    unsigned int rem = _num_byte & 3;
    for (unsigned int i = 0; i < rem; i++) {
        t.u8[i] = a[i];
    }
    t.u32 = PQCLEAN_NAMESPACE_gf256v_mul_u32(t.u32, b);
    for (unsigned int i = 0; i < rem; i++) {
        a[i] = t.u8[i];
    }
}

void PQCLEAN_NAMESPACE_gf256v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf256_b, unsigned int _num_byte) {
    unsigned int n_u32 = _num_byte >> 2;
    uint32_t *c_u32 = (uint32_t *)accu_c;
    const uint32_t *a_u32 = (const uint32_t *)a;
    for (unsigned int i = 0; i < n_u32; i++) {
        c_u32[i] ^= PQCLEAN_NAMESPACE_gf256v_mul_u32(a_u32[i], gf256_b);
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    t.u32 = 0;
    accu_c += (n_u32 << 2);
    a += (n_u32 << 2);
    unsigned int rem = _num_byte & 3;
    for (unsigned int i = 0; i < rem; i++) {
        t.u8[i] = a[i];
    }
    t.u32 = PQCLEAN_NAMESPACE_gf256v_mul_u32(t.u32, gf256_b);
    for (unsigned int i = 0; i < rem; i++) {
        accu_c[i] ^= t.u8[i];
    }
}

#endif
