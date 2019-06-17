#ifndef _GF16_H_
#define _GF16_H_

#include "rainbow_config.h"
#include <stdint.h>

/// @file gf16.h
/// @brief Library for arithmetics in GF(16) and GF(256)
///


#ifdef _USE_GF16

uint8_t PQCLEAN_NAMESPACE_gf16_is_nonzero(uint8_t a);
uint8_t PQCLEAN_NAMESPACE_gf16_inv(uint8_t a);
uint32_t PQCLEAN_NAMESPACE_gf16v_mul_u32(uint32_t a, uint8_t b);
uint32_t PQCLEAN_NAMESPACE_gf16v_mul_u32_u32(uint32_t a, uint32_t b);
uint8_t PQCLEAN_NAMESPACE_gf16v_reduce_u32(uint32_t a);

#else

uint8_t PQCLEAN_NAMESPACE_gf256_is_nonzero(uint8_t a);
uint8_t PQCLEAN_NAMESPACE_gf256_inv(uint8_t a);
uint32_t PQCLEAN_NAMESPACE_gf256v_mul_u32(uint32_t a, uint8_t b);

#endif

#endif // _GF16_H_
