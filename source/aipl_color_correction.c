/* Copyright (C) 2022-2024 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */

/******************************************************************************
 * @file    aipl_color_correction.c
 * @brief   Color correction functions implementation
 *
******************************************************************************/


/*********************
 *      INCLUDES
 *********************/
#include "aipl_color_correction.h"
#include <RTE_Device.h>
#include <stddef.h>
#include "aipl_config.h"
#ifdef AIPL_HELIUM_ACCELERATION
#include "aipl_mve_utils.h"
#endif
#include "aipl_utils.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
aipl_error_t aipl_color_correction_24bit(const void* input, void* output,
                                          uint32_t pitch,
                                          uint32_t width, uint32_t height,
                                          const float* ccm,
                                          uint8_t r_offset,
                                          uint8_t g_offset,
                                          uint8_t b_offset);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
aipl_error_t aipl_color_correction_rgb(const void* input, void* output,
                                       uint32_t pitch,
                                       uint32_t width, uint32_t height,
                                       aipl_color_format_t format,
                                       const float* ccm)
{
    switch (format)
    {
        case AIPL_COLOR_ARGB8888:
            return aipl_color_correction_argb8888(input, output, pitch,
                                                  width, height, ccm);
        case AIPL_COLOR_RGBA8888:
            return aipl_color_correction_rgba8888(input, output, pitch,
                                                  width, height, ccm);
        case AIPL_COLOR_ARGB4444:
            return aipl_color_correction_argb4444(input, output, pitch,
                                                  width, height, ccm);
        case AIPL_COLOR_ARGB1555:
            return aipl_color_correction_argb1555(input, output, pitch,
                                                  width, height, ccm);
        case AIPL_COLOR_RGBA4444:
            return aipl_color_correction_rgba4444(input, output, pitch,
                                                  width, height, ccm);
        case AIPL_COLOR_RGBA5551:
            return aipl_color_correction_rgba5551(input, output, pitch,
                                                  width, height, ccm);
        case AIPL_COLOR_BGR888:
            return aipl_color_correction_bgr888(input, output, pitch,
                                                width, height, ccm);
        case AIPL_COLOR_RGB888:
            return aipl_color_correction_rgb888(input, output, pitch,
                                                width, height, ccm);
        case AIPL_COLOR_RGB565:
            return aipl_color_correction_rgb565(input, output, pitch,
                                                width, height, ccm);

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

aipl_error_t aipl_color_correction_rgb_img(const aipl_image_t* input,
                                           aipl_image_t* output,
                                           const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (input->width != output->width || input->height != output->height)
        return AIPL_ERR_SIZE_MISMATCH;

    if (input->format != output->format)
        return AIPL_ERR_FORMAT_MISMATCH;

    return aipl_color_correction_rgb(input->data, output->data, input->pitch,
                                     input->width, input->height, input->format,
                                     ccm);
}

aipl_error_t aipl_color_correction_argb8888(const void* input, void* output,
                                            uint32_t pitch,
                                            uint32_t width, uint32_t height,
                                            const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + (i * pitch) * 4;
        uint8_t* dst = dst_ptr + (i * width) * 4;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_ldr_16px_argb8888(&pix, src, tail_p);

            aipl_mve_color_correction_argb_x16(&pix, ccm);

            aipl_mve_str_16px_argb8888(dst, pix, tail_p);

            src += 64;
            dst += 64;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + (i * pitch);
        aipl_argb8888_px_t* dst = dst_ptr + (i * width);

        for (uint32_t j = 0; j < width; ++j)
        {
            int16_t r = src->r * ccm[0] + src->g * ccm[1] + src->b * ccm[2];
            int16_t g = src->r * ccm[3] + src->g * ccm[4] + src->b * ccm[5];
            int16_t b = src->r * ccm[6] + src->g * ccm[7] + src->b * ccm[8];

            dst->a = src->a;
            dst->r = aipl_channel_cap(r);
            dst->g = aipl_channel_cap(g);
            dst->b = aipl_channel_cap(b);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_argb4444(const void* input, void* output,
                                            uint32_t pitch,
                                            uint32_t width, uint32_t height,
                                            const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + (i * pitch);
        uint16_t* dst = dst_ptr + (i * width);

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_ldr_16px_argb4444(&pix, src, tail_p);

            aipl_mve_color_correction_argb_x16(&pix, ccm);

            aipl_mve_str_16px_extended_argb4444(dst, pix, tail_p);

            src += 16;
            dst += 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + (i * pitch);
        aipl_argb4444_px_t* dst = dst_ptr + (i * width);

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_cnvt_px_argb4444_to_argb8888(&px, src);

            int16_t r = px.r * ccm[0] + px.g * ccm[1] + px.b * ccm[2];
            int16_t g = px.r * ccm[3] + px.g * ccm[4] + px.b * ccm[5];
            int16_t b = px.r * ccm[6] + px.g * ccm[7] + px.b * ccm[8];

            px.r = aipl_channel_cap(r);
            px.g = aipl_channel_cap(g);
            px.b = aipl_channel_cap(b);

            aipl_cnvt_px_argb8888_to_argb4444(dst, &px);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_argb1555(const void* input, void* output,
                                            uint32_t pitch,
                                            uint32_t width, uint32_t height,
                                            const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + (i * pitch);
        uint16_t* dst = dst_ptr + (i * width);

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_ldr_16px_argb1555(&pix, src, tail_p);

            aipl_mve_color_correction_argb_x16(&pix, ccm);

            aipl_mve_str_16px_argb1555(dst, pix, tail_p);

            src += 16;
            dst += 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + (i * pitch);
        aipl_argb1555_px_t* dst = dst_ptr + (i * width);

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_cnvt_px_argb1555_to_argb8888(&px, src);

            int16_t r = px.r * ccm[0] + px.g * ccm[1] + px.b * ccm[2];
            int16_t g = px.r * ccm[3] + px.g * ccm[4] + px.b * ccm[5];
            int16_t b = px.r * ccm[6] + px.g * ccm[7] + px.b * ccm[8];

            px.r = aipl_channel_cap(r);
            px.g = aipl_channel_cap(g);
            px.b = aipl_channel_cap(b);

            aipl_cnvt_px_argb8888_to_argb1555(dst, &px);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_rgba8888(const void* input, void* output,
                                            uint32_t pitch,
                                            uint32_t width, uint32_t height,
                                            const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + (i * pitch) * 4;
        uint8_t* dst = dst_ptr + (i * width) * 4;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_ldr_16px_rgba8888(&pix, src, tail_p);

            aipl_mve_color_correction_argb_x16(&pix, ccm);

            aipl_mve_str_16px_rgba8888(dst, pix, tail_p);

            src += 64;
            dst += 64;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + (i * pitch);
        aipl_rgba8888_px_t* dst = dst_ptr + (i * width);

        for (uint32_t j = 0; j < width; ++j)
        {
            int16_t r = src->r * ccm[0] + src->g * ccm[1] + src->b * ccm[2];
            int16_t g = src->r * ccm[3] + src->g * ccm[4] + src->b * ccm[5];
            int16_t b = src->r * ccm[6] + src->g * ccm[7] + src->b * ccm[8];

            dst->r = aipl_channel_cap(r);
            dst->g = aipl_channel_cap(g);
            dst->b = aipl_channel_cap(b);
            dst->a = src->a;

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_rgba4444(const void* input, void* output,
                                            uint32_t pitch,
                                            uint32_t width, uint32_t height,
                                            const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + (i * pitch);
        uint16_t* dst = dst_ptr + (i * width);

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_ldr_16px_rgba4444(&pix, src, tail_p);

            aipl_mve_color_correction_argb_x16(&pix, ccm);

            aipl_mve_str_16px_rgba4444(dst, pix, tail_p);

            src += 16;
            dst += 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + (i * pitch);
        aipl_rgba4444_px_t* dst = dst_ptr + (i * width);

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_cnvt_px_rgba4444_to_argb8888(&px, src);

            int16_t r = px.r * ccm[0] + px.g * ccm[1] + px.b * ccm[2];
            int16_t g = px.r * ccm[3] + px.g * ccm[4] + px.b * ccm[5];
            int16_t b = px.r * ccm[6] + px.g * ccm[7] + px.b * ccm[8];

            px.r = aipl_channel_cap(r);
            px.g = aipl_channel_cap(g);
            px.b = aipl_channel_cap(b);

            aipl_cnvt_px_argb8888_to_rgba4444(dst, &px);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_rgba5551(const void* input, void* output,
                                            uint32_t pitch,
                                            uint32_t width, uint32_t height,
                                            const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + (i * pitch);
        uint16_t* dst = dst_ptr + (i * width);

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_ldr_16px_rgba5551(&pix, src, tail_p);

            aipl_mve_color_correction_argb_x16(&pix, ccm);

            aipl_mve_str_16px_rgba5551(dst, pix, tail_p);

            src += 16;
            dst += 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + (i * pitch);
        aipl_rgba5551_px_t* dst = dst_ptr + (i * width);

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_cnvt_px_rgba5551_to_argb8888(&px, src);

            int16_t r = px.r * ccm[0] + px.g * ccm[1] + px.b * ccm[2];
            int16_t g = px.r * ccm[3] + px.g * ccm[4] + px.b * ccm[5];
            int16_t b = px.r * ccm[6] + px.g * ccm[7] + px.b * ccm[8];

            px.r = aipl_channel_cap(r);
            px.g = aipl_channel_cap(g);
            px.b = aipl_channel_cap(b);

            aipl_cnvt_px_argb8888_to_rgba5551(dst, &px);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_bgr888(const void* input, void* output,
                                          uint32_t pitch,
                                          uint32_t width, uint32_t height,
                                          const float* ccm)
{
    return aipl_color_correction_24bit(input, output, pitch,
                                       width, height, ccm,
                                       2, 1, 0);
}

aipl_error_t aipl_color_correction_rgb888(const void* input, void* output,
                                          uint32_t pitch,
                                          uint32_t width, uint32_t height,
                                          const float* ccm)
{
    return aipl_color_correction_24bit(input, output, pitch,
                                       width, height, ccm,
                                       0, 1, 2);
}

aipl_error_t aipl_color_correction_rgb565(const void* input, void* output,
                                          uint32_t pitch,
                                          uint32_t width, uint32_t height,
                                          const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + (i * pitch);
        uint16_t* dst = dst_ptr + (i * width);

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb565(&pix, src, tail_p);

            aipl_mve_color_correction_rgb_x16(&pix, ccm);

            aipl_mve_str_16px_rgb565(dst, pix, tail_p);

            src += 16;
            dst += 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + (i * pitch);
        aipl_rgb565_px_t* dst = dst_ptr + (i * width);

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_cnvt_px_rgb565_to_24bit(px, src, 2, 1, 0);

            int16_t r = px[2] * ccm[0] + px[1] * ccm[1] + px[0] * ccm[2];
            int16_t g = px[2] * ccm[3] + px[1] * ccm[4] + px[0] * ccm[5];
            int16_t b = px[2] * ccm[6] + px[1] * ccm[7] + px[0] * ccm[8];

            px[2] = aipl_channel_cap(r);
            px[1] = aipl_channel_cap(g);
            px[0] = aipl_channel_cap(b);

            aipl_cnvt_px_24bit_to_rgb565(dst, px, 2, 1, 0);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
aipl_error_t aipl_color_correction_24bit(const void* input, void* output,
                                          uint32_t pitch,
                                          uint32_t width, uint32_t height,
                                          const float* ccm,
                                          uint8_t r_offset,
                                          uint8_t g_offset,
                                          uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + (i * pitch) * 3;
        uint8_t* dst = dst_ptr + (i * width) * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_offset, g_offset, b_offset);

            aipl_mve_color_correction_rgb_x16(&pix, ccm);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            src += 48;
            dst += 48;
        }
}
#else
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            int16_t r = src[r_offset] * ccm[0] + src[g_offset] * ccm[1] + src[b_offset] * ccm[2];
            int16_t g = src[r_offset] * ccm[3] + src[g_offset] * ccm[4] + src[b_offset] * ccm[5];
            int16_t b = src[r_offset] * ccm[6] + src[g_offset] * ccm[7] + src[b_offset] * ccm[8];

            dst[r_offset] = aipl_channel_cap(r);
            dst[g_offset] = aipl_channel_cap(g);
            dst[b_offset] = aipl_channel_cap(b);

            src += 3;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
