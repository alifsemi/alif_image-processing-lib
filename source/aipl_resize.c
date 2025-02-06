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
 * @file    aipl_resize.c
 * @brief   Resize functions implementation
 *
******************************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include "aipl_resize.h"
#include <RTE_Device.h>
#include <stddef.h>
#include "aipl_config.h"
#ifdef AIPL_DAVE2D_ACCELERATION
#include "aipl_dave2d.h"
#endif
#ifdef AIPL_HELIUM_ACCELERATION
#include <arm_mve.h>
#endif

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
static aipl_error_t aipl_resize_sw(const void* input, void* output,
                                   int input_width, int input_height,
                                   int output_width, int output_height,
                                   aipl_color_format_t format);

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
aipl_error_t aipl_resize(const void* input, void* output,
                         uint32_t pitch,
                         uint32_t width, uint32_t height,
                         aipl_color_format_t format,
                         uint32_t output_width, uint32_t output_height,
                         bool interpolate)
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
                                           output_width, output_height,
                                           0, 0,
                                           0,
                                           false, false,
                                           true, interpolate);

        return aipl_dave2d_error_convert(ret);
    }
#endif

    switch (format)
    {
        /* Alpha color formats */
        case AIPL_COLOR_ALPHA8:
        /* RGB color formats */
        case AIPL_COLOR_ARGB8888:
        case AIPL_COLOR_RGBA8888:
        case AIPL_COLOR_RGB888:
        case AIPL_COLOR_BGR888:
            return aipl_resize_sw(input, output,
                                  width, height,
                                  output_width, output_height,
                                  format);
        case AIPL_COLOR_ARGB4444:
        case AIPL_COLOR_ARGB1555:
        case AIPL_COLOR_RGBA4444:
        case AIPL_COLOR_RGBA5551:
        case AIPL_COLOR_RGB565:
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

aipl_error_t aipl_resize_img(const aipl_image_t* input,
                             aipl_image_t* output,
                             bool interpolate)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (output->format != input->format)
        return AIPL_ERR_FORMAT_MISMATCH;

    return aipl_resize(input->data, output->data,
                       input->pitch,
                       input->width, input->height,
                       input->format,
                       output->width, output->height,
                       interpolate);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static aipl_error_t aipl_resize_sw(const void* input, void* output,
                                   int srcWidth, int srcHeight,
                                   int dstWidth, int dstHeight,
                                   aipl_color_format_t format)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint32_t pixel_size_B = aipl_color_format_depth(format)/8;

    const uint8_t* srcImage = (const uint8_t*)input;
    uint8_t* dstImage = (uint8_t*)output;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint32x4_t bgr_offset = {0,1,2,3};
#endif

// Copied from ei_camera.cpp in firmware-eta-compute
// Modified for BGR888
// This needs to be < 16 or it won't fit. Cortex-M4 only has SIMD for signed multiplies
#define FRAC_BITS 14
    const int FRAC_VAL = (1 << FRAC_BITS);
    const int FRAC_MASK = (FRAC_VAL - 1);

    uint32_t src_x_accum, src_y_accum; // accumulators and fractions for scaling the image
    uint32_t x_frac, nx_frac, y_frac, ny_frac;
    int x, y, ty;

    if (srcHeight < 2) {
        return AIPL_ERR_FRAME_OUT_OF_RANGE;
    }

    // start at 1/2 pixel in to account for integer downsampling which might miss pixels
    src_y_accum = FRAC_VAL / 2;
    const uint32_t src_x_frac = (srcWidth * FRAC_VAL) / dstWidth;
    const uint32_t src_y_frac = (srcHeight * FRAC_VAL) / dstHeight;

    //from here out, *3 b/c RGB
    srcWidth *= pixel_size_B;
    //srcHeight not used for indexing
    //dstWidth still needed as is
    //dstHeight shouldn't be scaled

    const uint8_t *s;
    uint8_t *d;

    for (y = 0; y < dstHeight; y++) {
        // do indexing computations
        ty = src_y_accum >> FRAC_BITS; // src y
        y_frac = src_y_accum & FRAC_MASK;
        src_y_accum += src_y_frac;
        ny_frac = FRAC_VAL - y_frac; // y fraction and 1.0 - y fraction

        s = &srcImage[ty * srcWidth];
        d = &dstImage[y * dstWidth * pixel_size_B]; //not scaled above
        // start at 1/2 pixel in to account for integer downsampling which might miss pixels
        src_x_accum = FRAC_VAL / 2;
        for (x = 0; x < dstWidth; x++) {
            uint32_t tx;
            // do indexing computations
            tx = (src_x_accum >> FRAC_BITS) * pixel_size_B;
            x_frac = src_x_accum & FRAC_MASK;
            nx_frac = FRAC_VAL - x_frac; // x fraction and 1.0 - x fraction
            src_x_accum += src_x_frac;
            __builtin_prefetch(&s[tx + 64]);
            __builtin_prefetch(&s[tx + srcWidth + 64]);

#ifdef AIPL_HELIUM_ACCELERATION
            uint32x4_t p00 = vldrbq_u32(&s[tx]);
            uint32x4_t p10 = vldrbq_u32(&s[tx + pixel_size_B]);
            uint32x4_t p01 = vldrbq_u32(&s[tx + srcWidth]);
            uint32x4_t p11 = vldrbq_u32(&s[tx + srcWidth + pixel_size_B]);
            p00 = vmulq(p00, nx_frac);
            p00 = vmlaq(p00, p10, x_frac);
            p00 = vrshrq(p00, FRAC_BITS);
            p01 = vmulq(p01, nx_frac);
            p01 = vmlaq(p01, p11, x_frac);
            p01 = vrshrq(p01, FRAC_BITS);
            p00 = vmulq(p00, ny_frac);
            p00 = vmlaq(p00, p01, y_frac);
            p00 = vrshrq(p00, FRAC_BITS);
            vstrbq_scatter_offset_p_u32(d, bgr_offset, p00, vctp32q(pixel_size_B));

            d += pixel_size_B;
#else
            //interpolate and write out
            for (int color = 0; color < pixel_size_B;
                color++) // do pixel_size_B times for pixel_size_B colors
            {
                uint32_t p00, p01, p10, p11;
                p00 = s[tx];
                p10 = s[tx + pixel_size_B];
                p01 = s[tx + srcWidth];
                p11 = s[tx + srcWidth + pixel_size_B];
                p00 = ((p00 * nx_frac) + (p10 * x_frac) + FRAC_VAL / 2) >> FRAC_BITS; // top line
                p01 = ((p01 * nx_frac) + (p11 * x_frac) + FRAC_VAL / 2) >> FRAC_BITS; // bottom line
                p00 = ((p00 * ny_frac) + (p01 * y_frac) + FRAC_VAL / 2) >> FRAC_BITS; //top + bottom
                *d++ = (uint8_t)p00; // store new pixel
                //ready next loop
                tx++;
            }
#endif
        } // for x
    } // for y
    return AIPL_ERR_OK;
} // resizeImage()

