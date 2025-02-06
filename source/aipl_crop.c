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
 * @file    aipl_crop.c
 * @brief   Crop functions implementation
 *
******************************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <memory.h>

#include "RTE_Components.h"
#include CMSIS_device_header
#include <RTE_Device.h>
#include <stddef.h>
#include "aipl_config.h"
#ifdef AIPL_DAVE2D_ACCELERATION
#include "aipl_dave2d.h"
#endif
#ifdef AIPL_HELIUM_ACCELERATION
#include <arm_mve.h>
#endif

#include "aipl_crop.h"
#include "aipl_config.h"
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
static aipl_error_t aipl_crop_sw(const void* input, void* output,
                        uint32_t pitch,
                        uint32_t width, uint32_t height,
                        aipl_color_format_t format,
                        uint32_t left, uint32_t top,
                        uint32_t right, uint32_t bottom);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
aipl_error_t aipl_crop(const void* input, void* output,
                       uint32_t pitch,
                       uint32_t width, uint32_t height,
                       aipl_color_format_t format,
                       uint32_t left, uint32_t top,
                       uint32_t right, uint32_t bottom)
{
    // Check pointers
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    if (aipl_dave2d_format_supported(format)
        && format != AIPL_COLOR_ARGB1555
        && format != AIPL_COLOR_RGBA5551
        && format != AIPL_COLOR_ALPHA8)
    {
        uint32_t x = left;
        uint32_t y = top;
        uint32_t output_width = right - left;
        uint32_t output_height = bottom - top;

        d2_u32 ret = aipl_dave2d_texturing(input, output,
                                           pitch,
                                           width, height,
                                           aipl_dave2d_format_to_mode(format),
                                           output_width, output_height,
                                           -x, -y,
                                           0,
                                           false, false,
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
            return aipl_crop_sw(input, output,
                                pitch,
                                width, height,
                                format,
                                left, top,
                                right, bottom);
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

aipl_error_t aipl_crop_img(const aipl_image_t* input,
                       aipl_image_t* output,
                       uint32_t left, uint32_t top,
                       uint32_t right, uint32_t bottom)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t new_width = right - left;
    uint32_t new_hight = bottom - top;
    if (new_width != output->width || new_hight != output->height)
    {
        return AIPL_ERR_SIZE_MISMATCH;
    }

    if (output->format != input->format)
    {
        return AIPL_ERR_FORMAT_MISMATCH;
    }

    return aipl_crop(input->data, output->data,
                     input->pitch,
                     input->width, input->height,
                     input->format,
                     left, top,
                     right, bottom);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
aipl_error_t aipl_crop_sw(const void* input, void* output,
                          uint32_t pitch,
                          uint32_t width, uint32_t height,
                          aipl_color_format_t format,
                          uint32_t left, uint32_t top,
                          uint32_t right, uint32_t bottom)
{
    // Check pointers
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    // Checking the boundary
    if( (left > right) || (right > width) || (top > bottom) || (bottom > height) )
        return AIPL_ERR_FRAME_OUT_OF_RANGE;

    uint32_t bpp = aipl_color_format_depth (format);

    // Check for no cropping
    if (left == 0 && top == 0 && right == width && bottom == height) {
        // No-op if cropping and in-place
        size_t size = width * height * (bpp / 8);
        if (input != output) {
            memcpy(output, input, size);
        }
        aipl_cpu_cache_clean(output, size);
        return AIPL_ERR_OK;
    }

    // Updating the input frame column start
    uint8_t *ip_fb = (uint8_t *)input + (top * width * (bpp / 8));
    uint8_t *op_fb = (uint8_t *)output;

    uint32_t new_width = right - left;
    uint32_t new_hight = bottom - top;
    for(uint32_t i = 0; i < new_hight; ++i)
    {
        // Update row address
        const uint8_t *ip_fb_row = ip_fb + left * (bpp / 8);
        memmove(op_fb, ip_fb_row, new_width * (bpp / 8));

        // Update fb
        ip_fb += (width * (bpp / 8));
        op_fb += (new_width * (bpp / 8));
    }

    size_t size = new_width * new_hight * (bpp / 8);
    aipl_cpu_cache_clean(output, size);

    return AIPL_ERR_OK;
}

