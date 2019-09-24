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
    for (size_t i = 0; i < _num_byte; i++) {
        a[i] = PQCLEAN_NAMESPACE_gf256_mul(a[i], b);
    }
}

void PQCLEAN_NAMESPACE_gf256v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf256_b, unsigned int _num_byte) {
    for (size_t i = 0; i < _num_byte; i++) {
        accu_c[i] ^= PQCLEAN_NAMESPACE_gf256_mul(a[i], gf256_b);
    }
}

#endif
