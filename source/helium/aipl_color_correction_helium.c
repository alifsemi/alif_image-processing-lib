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
 * @file    aipl_color_correction_helium.c
 * @brief   Helium accelerated color correction function implementations
 *
******************************************************************************/


/*********************
 *      INCLUDES
 *********************/
#include "aipl_color_correction_helium.h"
#include <RTE_Device.h>
#include <stddef.h>
#include "aipl_mve_utils.h"

#ifdef AIPL_HELIUM_ACCELERATION

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
aipl_error_t aipl_color_correction_24bit_helium(const void* input, void* output,
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
aipl_error_t aipl_color_correction_rgb_helium(const void* input, void* output,
                                              uint32_t pitch,
                                              uint32_t width, uint32_t height,
                                              aipl_color_format_t format,
                                              const float* ccm)
{
    switch (format)
    {
        case AIPL_COLOR_ARGB8888:
            return aipl_color_correction_argb8888_helium(input, output, pitch,
                                                         width, height, ccm);
        case AIPL_COLOR_RGBA8888:
            return aipl_color_correction_rgba8888_helium(input, output, pitch,
                                                         width, height, ccm);
        case AIPL_COLOR_ARGB4444:
            return aipl_color_correction_argb4444_helium(input, output, pitch,
                                                         width, height, ccm);
        case AIPL_COLOR_ARGB1555:
            return aipl_color_correction_argb1555_helium(input, output, pitch,
                                                         width, height, ccm);
        case AIPL_COLOR_RGBA4444:
            return aipl_color_correction_rgba4444_helium(input, output, pitch,
                                                         width, height, ccm);
        case AIPL_COLOR_RGBA5551:
            return aipl_color_correction_rgba5551_helium(input, output, pitch,
                                                         width, height, ccm);
        case AIPL_COLOR_BGR888:
            return aipl_color_correction_bgr888_helium(input, output, pitch,
                                                       width, height, ccm);
        case AIPL_COLOR_RGB888:
            return aipl_color_correction_rgb888_helium(input, output, pitch,
                                                       width, height, ccm);
        case AIPL_COLOR_RGB565:
            return aipl_color_correction_rgb565_helium(input, output, pitch,
                                                       width, height, ccm);

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

aipl_error_t aipl_color_correction_rgb_img_helium(const aipl_image_t* input,
                                                  aipl_image_t* output,
                                                  const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (input->width != output->width || input->height != output->height)
        return AIPL_ERR_SIZE_MISMATCH;

    if (input->format != output->format)
        return AIPL_ERR_FORMAT_MISMATCH;

    return aipl_color_correction_rgb_helium(input->data, output->data, input->pitch,
                                            input->width, input->height, input->format,
                                            ccm);
}

aipl_error_t aipl_color_correction_argb8888_helium(const void* input, void* output,
                                                   uint32_t pitch,
                                                   uint32_t width, uint32_t height,
                                                   const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

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

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_argb4444_helium(const void* input, void* output,
                                                   uint32_t pitch,
                                                   uint32_t width, uint32_t height,
                                                   const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

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

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_argb1555_helium(const void* input, void* output,
                                                   uint32_t pitch,
                                                   uint32_t width, uint32_t height,
                                                   const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

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

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_rgba8888_helium(const void* input, void* output,
                                                   uint32_t pitch,
                                                   uint32_t width, uint32_t height,
                                                   const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

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

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_rgba4444_helium(const void* input, void* output,
                                                   uint32_t pitch,
                                                   uint32_t width, uint32_t height,
                                                   const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

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

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_rgba5551_helium(const void* input, void* output,
                                                   uint32_t pitch,
                                                   uint32_t width, uint32_t height,
                                                   const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

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

    return AIPL_ERR_OK;
}

aipl_error_t aipl_color_correction_bgr888_helium(const void* input, void* output,
                                                 uint32_t pitch,
                                                 uint32_t width, uint32_t height,
                                                 const float* ccm)
{
    return aipl_color_correction_24bit_helium(input, output, pitch,
                                              width, height, ccm,
                                              2, 1, 0);
}

aipl_error_t aipl_color_correction_rgb888_helium(const void* input, void* output,
                                                 uint32_t pitch,
                                                 uint32_t width, uint32_t height,
                                                 const float* ccm)
{
    return aipl_color_correction_24bit_helium(input, output, pitch,
                                              width, height, ccm,
                                              0, 1, 2);
}

aipl_error_t aipl_color_correction_rgb565_helium(const void* input, void* output,
                                                 uint32_t pitch,
                                                 uint32_t width, uint32_t height,
                                                 const float* ccm)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

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

    return AIPL_ERR_OK;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
aipl_error_t aipl_color_correction_24bit_helium(const void* input, void* output,
                                                uint32_t pitch,
                                                uint32_t width, uint32_t height,
                                                const float* ccm,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

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

    return AIPL_ERR_OK;
}

#endif
