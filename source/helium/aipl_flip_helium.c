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
 * @file    aipl_flip_helium.c
 * @brief   Helium accelerated flip function implementations
 *
******************************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include <string.h>
#include "aipl_flip_helium.h"
#include <RTE_Device.h>
#include <stddef.h>
#include "aipl_cache.h"
#include <arm_mve.h>

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

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
aipl_error_t aipl_flip_helium(const void* input, void* output,
                              uint32_t pitch,
                              uint32_t width, uint32_t height,
                              aipl_color_format_t format,
                              bool flip_horizontal, bool flip_vertical)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (format >= AIPL_COLOR_YV12)
        return AIPL_ERR_UNSUPPORTED_FORMAT;

    const uint8_t* src = input;
    uint8_t* dst = output;

    const int rgbBytes = aipl_color_format_depth(format)/8;
    int x, y, j;

    if (flip_horizontal && flip_vertical)
    {

        for (y = 0; y < height / 2; ++y)
        {
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
                    uint8x16_t i_top_left = vldrbq_gather_offset_z(&src[top_left] + j, frw_off, tail_p);
                    uint8x16_t i_top_right = vldrbq_gather_offset_z(&src[top_right] + j, rvr_off, tail_p);
                    uint8x16_t i_bottom_left = vldrbq_gather_offset_z(&src[bottom_left] + j, frw_off, tail_p);
                    uint8x16_t i_bottom_right = vldrbq_gather_offset_z(&src[bottom_right] + j, rvr_off, tail_p);

                    vstrbq_scatter_offset_p(&dst[top_left] + j, frw_off, i_bottom_right, tail_p);
                    vstrbq_scatter_offset_p(&dst[top_right] + j, rvr_off, i_bottom_left, tail_p);
                    vstrbq_scatter_offset_p(&dst[bottom_left] + j, frw_off, i_top_right, tail_p);
                    vstrbq_scatter_offset_p(&dst[bottom_right] + j, rvr_off, i_top_left, tail_p);
                }

                top_left += 16 * rgbBytes;
                bottom_left += 16 * rgbBytes;
                top_right -= 16 * rgbBytes;
                bottom_right -= 16 * rgbBytes;
            }
        }
    }
    else if (flip_horizontal)
    {
        for (y = 0; y < height; ++y)
        {
            for (x = 0; x < width / 2; ++x)
            {
                int left = (y * width + x) * rgbBytes;
                int right = (y * width + (width - 1 - x)) * rgbBytes;

                for (j = 0; j < rgbBytes; j++)
                {
                    dst[left] = src[right];
                    dst[right++] = src[left++];
                }
            }
        }
    }
    else if (flip_vertical)
    {
        size_t row_size = width * rgbBytes;
        for (y = 0; y < height / 2; ++y)
        {
            size_t top_row = y * row_size;
            size_t bottom_row = (height - 1 - y) * row_size;
            memmove(dst + bottom_row, src + top_row, row_size);
            memmove(dst + top_row, src + bottom_row, row_size);
        }
    }
    else
        return AIPL_ERR_NOT_SUPPORTED;

    size_t size = width * height * rgbBytes;
    aipl_cpu_cache_clean(dst, size);

    return AIPL_ERR_OK;
}

aipl_error_t aipl_flip_img_helium(const aipl_image_t* input,
                                  aipl_image_t* output,
                                  bool flip_horizontal, bool flip_vertical)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (output->format != input->format)
        return AIPL_ERR_FORMAT_MISMATCH;

    return aipl_flip_helium(input->data, output->data,
                            input->pitch,
                            input->width, input->height,
                            input->format,
                            flip_horizontal, flip_vertical);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif
