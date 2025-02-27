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
 * @file    aipl_flip.c
 * @brief   Flip functions implementation
 *
******************************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include <memory.h>
#include "aipl_flip.h"
#include <RTE_Device.h>
#include <stddef.h>
#include "aipl_config.h"
#ifdef AIPL_DAVE2D_ACCELERATION
#include "aipl_dave2d.h"
#endif
#ifdef AIPL_HELIUM_ACCELERATION
#include <arm_mve.h>
#endif
#include "aipl_cache.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static aipl_error_t aipl_flip_sw(const uint8_t* restrict input,
                                 uint8_t* restrict output,
                                 uint32_t pitch,
                                 uint32_t width, uint32_t height,
                                 aipl_color_format_t format,
                                 bool flip_horizontal, bool flip_vertical);

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

 aipl_error_t aipl_flip(const void* input, void* output,
                       uint32_t pitch,
                       uint32_t width, uint32_t height,
                       aipl_color_format_t format,
                       bool flip_horizontal, bool flip_vertical)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    if (aipl_dave2d_format_supported(format)
        && format != AIPL_COLOR_ARGB1555
        && format != AIPL_COLOR_RGBA5551
        && format != AIPL_COLOR_ALPHA8)
    {
        d2_u32 ret = aipl_dave2d_texturing(input, output,
                            pitch,
                            width, height,
                            aipl_dave2d_format_to_mode(format),
                            width, height,
                            0, 0,
                            0,
                            flip_horizontal, flip_vertical,
                            false, false);

        return aipl_dave2d_error_convert(ret);
    }
#endif

    switch (format)
    {
        /* Alpha color formats */
        case AIPL_COLOR_ALPHA8:
        /* RGB color formats */
        case AIPL_COLOR_RGB888:
        case AIPL_COLOR_BGR888:
        case AIPL_COLOR_ARGB8888:
        case AIPL_COLOR_RGBA8888:
        case AIPL_COLOR_ARGB4444:
        case AIPL_COLOR_RGBA4444:
        case AIPL_COLOR_RGB565:
        case AIPL_COLOR_ARGB1555:
        case AIPL_COLOR_RGBA5551:
            return aipl_flip_sw(input, output,
                                pitch, width, height,
                                format,
                                flip_horizontal, flip_vertical);
        /* YUV color formats */
        case AIPL_COLOR_YV12:
        case AIPL_COLOR_I420:
        case AIPL_COLOR_NV12:
        case AIPL_COLOR_NV21:
        case AIPL_COLOR_I422:
        case AIPL_COLOR_YUY2:
        case AIPL_COLOR_UYVY:
        case AIPL_COLOR_I444:
        case AIPL_COLOR_I400:
        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

aipl_error_t aipl_flip_img(const aipl_image_t* input,
                             aipl_image_t* output,
                             bool flip_horizontal, bool flip_vertical)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (output->format != input->format)
        return AIPL_ERR_FORMAT_MISMATCH;

    return aipl_flip(input->data, output->data,
                     input->pitch,
                     input->width, input->height,
                     input->format,
                     flip_horizontal, flip_vertical);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static aipl_error_t aipl_flip_sw(const uint8_t* restrict input,
                                 uint8_t* restrict output,
                                 uint32_t pitch,
                                 uint32_t width, uint32_t height,
                                 aipl_color_format_t format,
                                 bool flip_horizontal, bool flip_vertical)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const int rgbBytes = aipl_color_format_depth(format)/8;
    int x, y, j;

    if (flip_horizontal && flip_vertical)
    {

        for (y = 0; y < height / 2; ++y)
        {
#ifdef AIPL_HELIUM_ACCELERATION
            int top_left = y * width * rgbBytes;
            int top_right = (y * width + (width - 16)) * rgbBytes;
            int bottom_left = ((height - 1 - y) * width) * rgbBytes;
            int bottom_right = ((height - 1 - y) * width + (width - 16)) * rgbBytes;

            for (x = 0; x < width / 2; x += 16)
            {
                mve_pred16_t tail_p = vctp16q(width / 2 - x);

                uint8x16_t frw_off = vidupq_n_u8(0, 1);
                frw_off = vmulq(frw_off, rgbBytes);

                uint8x16_t rvr_off = vcreateq_u8(0x08090a0b0c0d0e0f, 0x0001020304050607);
                rvr_off = vmulq(rvr_off, rgbBytes);

                for (j = 0; j < rgbBytes; j++)
                {
                    uint8x16_t i_top_left = vldrbq_gather_offset_z(&input[top_left] + j, frw_off, tail_p);
                    uint8x16_t i_top_right = vldrbq_gather_offset_z(&input[top_right] + j, rvr_off, tail_p);
                    uint8x16_t i_bottom_left = vldrbq_gather_offset_z(&input[bottom_left] + j, frw_off, tail_p);
                    uint8x16_t i_bottom_right = vldrbq_gather_offset_z(&input[bottom_right] + j, rvr_off, tail_p);

                    vstrbq_scatter_offset_p(&output[top_left] + j, frw_off, i_bottom_right, tail_p);
                    vstrbq_scatter_offset_p(&output[top_right] + j, rvr_off, i_bottom_left, tail_p);
                    vstrbq_scatter_offset_p(&output[bottom_left] + j, frw_off, i_top_right, tail_p);
                    vstrbq_scatter_offset_p(&output[bottom_right] + j, rvr_off, i_top_left, tail_p);
                }

                top_left += 16 * rgbBytes;
                bottom_left += 16 * rgbBytes;
                top_right -= 16 * rgbBytes;
                bottom_right -= 16 * rgbBytes;
            }
#else
            for (x = 0; x < width / 2; ++x)
            {
                int top_left = (y * width + x) * rgbBytes;
                int top_right = (y * width + (width - 1 - x)) * rgbBytes;
                int bottom_left = ((height - 1 - y) * width + x) * rgbBytes;
                int bottom_right = ((height - 1 - y) * width + (width - 1 - x)) * rgbBytes;

                for (j = 0; j < rgbBytes; j++)
                {
                    output[top_left] = input[bottom_right];
                    output[bottom_right++] = input[top_left++];
                    output[top_right] = input[bottom_left];
                    output[bottom_left++] = input[top_right++];
                }
            }
#endif
        }
    }
    else if (flip_horizontal)
    {
        for (y = 0; y < height; ++y)
        {
#ifdef AIPL_HELIUM_ACCELERATION
            int left = y * width * rgbBytes;
            int right = (y * width + (width - 16)) * rgbBytes;

            for (x = 0; x < width / 2; x += 16)
            {
                mve_pred16_t tail_p = vctp16q(width / 2 - x);

                uint8x16_t frw_off = vidupq_n_u8(0, 1);
                frw_off = vmulq(frw_off, rgbBytes);

                uint8x16_t rvr_off = vcreateq_u8(0x08090a0b0c0d0e0f, 0x0001020304050607);
                rvr_off = vmulq(rvr_off, rgbBytes);

                for (j = 0; j < rgbBytes; j++)
                {
                    uint8x16_t i_left = vldrbq_gather_offset_z(&input[left] + j, frw_off, tail_p);
                    uint8x16_t i_right = vldrbq_gather_offset_z(&input[right] + j, rvr_off, tail_p);

                    vstrbq_scatter_offset_p(&output[left] + j, frw_off, i_right, tail_p);
                    vstrbq_scatter_offset_p(&output[right] + j, rvr_off, i_left, tail_p);
                }

                left += 16 * rgbBytes;
                right -= 16 * rgbBytes;
            }
#else
            for (x = 0; x < width / 2; ++x)
            {
                int left = (y * width + x) * rgbBytes;
                int right = (y * width + (width - 1 - x)) * rgbBytes;

                for (j = 0; j < rgbBytes; j++)
                {
                    output[left] = input[right];
                    output[right++] = input[left++];
                }
            }
#endif
        }
    }
    else if (flip_vertical)
    {
        size_t row_size = width * rgbBytes;
        for (y = 0; y < height / 2; ++y)
        {
            size_t top_row = y * row_size;
            size_t bottom_row = (height - 1 - y) * row_size;
            memmove(output + bottom_row, input + top_row, row_size);
            memmove(output + top_row, input + bottom_row, row_size);
        }
    }
    else
        return AIPL_ERR_NOT_SUPPORTED;

    size_t size = width * height * rgbBytes;
    aipl_cpu_cache_clean(output, size);

    return AIPL_ERR_OK;
}
