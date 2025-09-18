/* Copyright (C) 2025 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */

 /******************************************************************************
 * @file    aipl_arm_mve.h
 * @brief   AIPL wrapper over arm_mve.h with redefinition of some Helium intrinsics
 *          to work around a GCC 12.2.0 compiler bug
 *
******************************************************************************/

#ifndef AIPL_ARM_MVE_H
#define AIPL_ARM_MVE_H

#if defined(__ARM_FEATURE_MVE) && __ARM_FEATURE_MVE
#include <arm_mve.h>

#define GCC_VERSION (__GNUC__ * 10000\
                     + __GNUC_MINOR__ * 100\
                     + __GNUC_PATCHLEVEL__)

#if GCC_VERSION == 120200

#undef vsriq_n_s8
#undef vsriq_n_s16
#undef vsriq_n_s32
#undef vsriq_n_u8
#undef vsriq_n_u16
#undef vsriq_n_u32

#define vsriq_n_s8  aipl_vsriq_n_s8
#define vsriq_n_s16 aipl_vsriq_n_s16
#define vsriq_n_s32 aipl_vsriq_n_s32
#define vsriq_n_u8  aipl_vsriq_n_u8
#define vsriq_n_u16 aipl_vsriq_n_u16
#define vsriq_n_u32 aipl_vsriq_n_u32

__extension__
extern inline int8x16_t
    __attribute__((__always_inline__, __gnu_inline__, __artificial__))
aipl_vsriq_n_s8(int8x16_t __a, int8x16_t __b, const int __imm)
{
    int8x16_t __res = __a;

    __asm("vsri.8 %q[res], %q[b], %[imm]" : [res] "+w"(__res) : [b] "w"(__b), [imm] "n"(__imm));
    return __res;
}

__extension__
extern inline int16x8_t
    __attribute__((__always_inline__, __gnu_inline__, __artificial__))
aipl_vsriq_n_s16(int16x8_t __a, int16x8_t __b, const int __imm)
{
    int16x8_t __res = __a;

    __asm("vsri.16 %q[res], %q[b], %[imm]"
          : [res] "+w"(__res)
          : [b] "w"(__b), [imm] "n"(__imm));
    return __res;
}

__extension__
extern inline int32x4_t
    __attribute__((__always_inline__, __gnu_inline__, __artificial__))
aipl_vsriq_n_s32(int32x4_t __a, int32x4_t __b, const int __imm)
{
    int32x4_t __res = __a;

    __asm("vsri.32 %q[res], %q[b], %[imm]"
          : [res] "+w"(__res)
          : [b] "w"(__b), [imm] "n"(__imm));
    return __res;
}

__extension__
extern inline uint8x16_t
    __attribute__((__always_inline__, __gnu_inline__, __artificial__))
aipl_vsriq_n_u8(uint8x16_t __a, uint8x16_t __b, const int __imm)
{
    uint8x16_t __res = __a;

    __asm("vsri.8 %q[res], %q[b], %[imm]" : [res] "+w"(__res) : [b] "w"(__b), [imm] "n"(__imm));
    return __res;
}

__extension__
extern inline uint16x8_t
    __attribute__((__always_inline__, __gnu_inline__, __artificial__))
aipl_vsriq_n_u16(uint16x8_t __a, uint16x8_t __b, const int __imm)
{
    uint16x8_t __res = __a;

    __asm("vsri.16 %q[res], %q[b], %[imm]"
          : [res] "+w"(__res)
          : [b] "w"(__b), [imm] "n"(__imm));
    return __res;
}

__extension__
extern inline uint32x4_t
    __attribute__((__always_inline__, __gnu_inline__, __artificial__))
aipl_vsriq_n_u32(uint32x4_t __a, uint32x4_t __b, const int __imm)
{
    uint32x4_t __res = __a;

    __asm("vsri.32 %q[res], %q[b], %[imm]"
          : [res] "+w"(__res)
          : [b] "w"(__b), [imm] "n"(__imm));
    return __res;
}

#endif /* GCC_VERSION == 120200 */

#endif /* defined(__ARM_FEATURE_MVE) && __ARM_FEATURE_MVE */

#endif /* AIPL_ARM_MVE_H */
