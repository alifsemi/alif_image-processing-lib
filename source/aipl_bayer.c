/**
 * @file aipl_bayer.c
 *
 * Bayer pattern decoding functions
 *
 * The original Bayer decoding functions are copied from libdc1394:
 * https://sourceforge.net/p/libdc1394/code/ci/master/tree/dc1394/bayer.c
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*********************
 *      INCLUDES
 *********************/
#include <stddef.h>
#include "aipl_bayer.h"
#include <RTE_Device.h>
#include "aipl_config.h"
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
static void aipl_bayer_aipl_bayer_clear_borders(uint8_t *rgb,
                                                int sx, int sy,
                                                int w);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

aipl_error_t aipl_bayer_decoding_img(const void *bayer,
                                    aipl_image_t* output,
                                    aipl_color_filter_t tile,
                                    aipl_bayer_method_t method)
{
    if (bayer == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (output->data == NULL)
        return AIPL_ERR_NULL_POINTER;

    return aipl_bayer_decoding(bayer, output->data,
                       output->width, output->height,
                       tile, method, output->format);
}

aipl_error_t aipl_bayer_decoding(const void *bayer,
                                 void *rgb,
                                 uint32_t width, uint32_t height,
                                 aipl_color_filter_t tile,
                                 aipl_bayer_method_t method,
                                 aipl_color_format_t format)
{
    switch (format) {
    case AIPL_COLOR_RGB565:
        if (method == AIPL_BAYER_METHOD_NEAREST) {
            return aipl_bayer_decoding_nearest_rgb565(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else if (method == AIPL_BAYER_METHOD_SIMPLE) {
            return aipl_bayer_decoding_simple_rgb565(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else if (method == AIPL_BAYER_METHOD_BILINEAR) {
            return aipl_bayer_decoding_linear_rgb565(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else {
            return AIPL_ERR_BAYER_INVALID_METHOD;
        }
    case AIPL_COLOR_RGB888:
        if (method == AIPL_BAYER_METHOD_NEAREST) {
            return aipl_bayer_decoding_nearest_rgb888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else if (method == AIPL_BAYER_METHOD_SIMPLE) {
            return aipl_bayer_decoding_simple_rgb888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else if (method == AIPL_BAYER_METHOD_BILINEAR) {
            return aipl_bayer_decoding_linear_rgb888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else {
            return AIPL_ERR_BAYER_INVALID_METHOD;
        }
    case AIPL_COLOR_BGR888:
        if (method == AIPL_BAYER_METHOD_NEAREST) {
            return aipl_bayer_decoding_nearest_bgr888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else if (method == AIPL_BAYER_METHOD_SIMPLE) {
            return aipl_bayer_decoding_simple_bgr888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else if (method == AIPL_BAYER_METHOD_BILINEAR) {
            return aipl_bayer_decoding_linear_bgr888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else {
            return AIPL_ERR_BAYER_INVALID_METHOD;
        }
    case AIPL_COLOR_ARGB8888:
        if (method == AIPL_BAYER_METHOD_NEAREST) {
            return aipl_bayer_decoding_nearest_argb8888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else if (method == AIPL_BAYER_METHOD_SIMPLE) {
            return aipl_bayer_decoding_simple_argb8888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else if (method == AIPL_BAYER_METHOD_BILINEAR) {
            return aipl_bayer_decoding_linear_argb8888(
                (const uint8_t*)bayer, (uint8_t*)rgb,
                width, height, tile);
        } else {
            return AIPL_ERR_BAYER_INVALID_METHOD;
        }
    default:
        return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

aipl_error_t aipl_bayer_decoding_nearest_rgb565(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 2;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    // Add black border
    imax = sx * sy * rgbBytes;
    for (i = sx * (sy - 1) * rgbBytes; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * rgbBytes;
    for (i = (sx - 1) * rgbBytes; i < imax; i += iinc) {
        int j;
        for (j = 0; j < rgbBytes; j++)
            rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            if (blue > 0) {
                rgb[-1] = ((bayer[bayerStep + 1] & 0xfc) << 3) | (bayer[bayerStep] >> 3);
                rgb[0] = (bayer[1] & 0xf8) | (bayer[bayerStep + 1] >> 5);
            } else {
                rgb[-1] = ((bayer[bayerStep + 1] & 0xfc) << 3) | (bayer[1] >> 3);
                rgb[0] = (bayer[bayerStep] & 0xf8) | (bayer[bayerStep + 1] >> 5);
            }

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer) / 2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg = vld2q(bayer);
            uint8x16x2_t gb = vld2q(bayer + bayerStep);
#ifndef __ICCARM__
            __builtin_prefetch(bayer + bayerStep + 16 * 2 * 2);
#endif
            uint8x16_t upper;
            uint8x16_t lower;

            if (blue > 0) {
                 upper = vsriq(rg.val[0], rg.val[1], 5);
                 lower = vshlq_n(rg.val[1], 3);
                 lower = vsriq(lower, gb.val[1], 3);
            } else {
                 upper = vsriq(gb.val[1], rg.val[1], 5);
                 lower = vshlq_n(rg.val[1], 3);
                 lower = vsriq(lower, rg.val[0], 3);
            }
            vstrbq_scatter_offset_p(rgb - 1, inc, lower, p);
            vstrbq_scatter_offset_p(rgb, inc, upper, p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);
            if (blue > 0) {
                upper = vsriq(gr.val[1], bg.val[1], 5);
                lower = vshlq_n(bg.val[1], 3);
                lower = vsriq(lower, bg.val[0], 3);
            } else {
                upper = vsriq(bg.val[0], bg.val[1], 5);
                lower = vshlq_n(bg.val[1], 3);
                lower = vsriq(lower, gr.val[1], 3);
            }
            vstrbq_scatter_offset_p(rgb + rgbBytes - 1, inc, lower, p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, upper, p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[-1] = ((bayer[1] & 0xfc) << 3) | (bayer[bayerStep + 1] >> 3);
                rgb[0] = (bayer[0] & 0xf8) | (bayer[1] >> 5);

                rgb[1] = ((bayer[bayerStep + 2] & 0xfc) << 3) | (bayer[bayerStep + 1] >> 3);
                rgb[2] = (bayer[2] & 0xf8) | (bayer[bayerStep + 2] >> 5);
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[-1] = ((bayer[1] & 0xfc) << 3) | (bayer[0] >> 3);
                rgb[0] = (bayer[bayerStep + 1] & 0xf8) | (bayer[1] >> 5);

                rgb[1] = ((bayer[bayerStep + 2] & 0xfc) << 3) | (bayer[2] >> 3);
                rgb[2] = (bayer[bayerStep + 1] & 0xf8) | (bayer[bayerStep + 2] >> 5);
            }
        }
#endif

        if (bayer < bayerEnd) {
            if (blue > 0) {
                rgb[-1] = ((bayer[1] & 0xfc) << 3) | (bayer[bayerStep + 1] >> 3);
                rgb[0] = (bayer[0] & 0xf8) | (bayer[1] >> 5);
            } else {
                rgb[-1] = ((bayer[1] & 0xfc) << 3) | (bayer[0] >> 3);
                rgb[0] = (bayer[bayerStep + 1] & 0xf8) | (bayer[1] >> 5);
            }

            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_nearest_bgr888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 3;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    // Add black border
    imax = sx * sy * rgbBytes;
    for (i = sx * (sy - 1) * rgbBytes; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * rgbBytes;
    for (i = (sx - 1) * rgbBytes; i < imax; i += iinc) {
        int j;
        for (j = 0; j < rgbBytes; j++)
            rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[bayerStep];
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = bayer[1];

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg = vld2q(bayer);
            uint8x16x2_t gb = vld2q(bayer + bayerStep);
#ifndef __ICCARM__
            __builtin_prefetch(bayer + bayerStep + 16*2*2);
#endif
            vstrbq_scatter_offset_p(rgb - blue, inc, gb.val[1], p);
            vstrbq_scatter_offset_p(rgb, inc, rg.val[1], p);
            vstrbq_scatter_offset_p(rgb + blue, inc, rg.val[0], p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, bg.val[0], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, bg.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, gr.val[1], p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[-1] = bayer[bayerStep + 1];
                rgb[0] = bayer[1];
                rgb[1] = bayer[0];

                rgb[2] = bayer[bayerStep + 1];
                rgb[3] = bayer[bayerStep + 2];
                rgb[4] = bayer[2];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[1] = bayer[bayerStep + 1];
                rgb[0] = bayer[1];
                rgb[-1] = bayer[0];

                rgb[4] = bayer[bayerStep + 1];
                rgb[3] = bayer[bayerStep + 2];
                rgb[2] = bayer[2];
            }
        }
#endif

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[bayerStep + 1];
            rgb[0] = bayer[1];
            rgb[blue] = bayer[0];
            
            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_nearest_rgb888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 3;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    // Add black border
    imax = sx * sy * rgbBytes;
    for (i = sx * (sy - 1) * rgbBytes; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * rgbBytes;
    for (i = (sx - 1) * rgbBytes; i < imax; i += iinc) {
        int j;
        for (j = 0; j < rgbBytes; j++)
            rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[1];
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = bayer[bayerStep];

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg = vld2q(bayer);
            uint8x16x2_t gb = vld2q(bayer + bayerStep);
#ifndef __ICCARM__
            __builtin_prefetch(bayer + bayerStep + 16*2*2);
#endif
            vstrbq_scatter_offset_p(rgb - blue, inc, rg.val[0], p);
            vstrbq_scatter_offset_p(rgb, inc, rg.val[1], p);
            vstrbq_scatter_offset_p(rgb + blue, inc, gb.val[1], p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, gr.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, bg.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, bg.val[0], p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[-1] = bayer[0];
                rgb[0] = bayer[1];
                rgb[1] = bayer[bayerStep + 1];

                rgb[2] = bayer[2];
                rgb[3] = bayer[bayerStep + 2];
                rgb[4] = bayer[bayerStep + 1];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[1] = bayer[0];
                rgb[0] = bayer[1];
                rgb[-1] = bayer[bayerStep + 1];

                rgb[4] = bayer[2];
                rgb[3] = bayer[bayerStep + 2];
                rgb[2] = bayer[bayerStep + 1];
            }
        }
#endif

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[0];
            rgb[0] = bayer[1];
            rgb[blue] = bayer[bayerStep + 1];
            
            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_nearest_argb8888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 4;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    // Add black border
    imax = sx * sy * rgbBytes;
    for (i = sx * (sy - 1) * rgbBytes; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * rgbBytes;
    for (i = (sx - 1) * rgbBytes; i < imax; i += iinc) {
        int j;
        for (j = 0; j < rgbBytes; j++)
            rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[bayerStep];
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = bayer[1];
            rgb[2] = 0xff;

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg = vld2q(bayer);
            uint8x16x2_t gb = vld2q(bayer + bayerStep);
            uint8x16_t a = vcreateq_u8(0xffffffffffffffff, 0xffffffffffffffff);
#ifndef __ICCARM__
            __builtin_prefetch(bayer + bayerStep + 16*2*2);
#endif
            vstrbq_scatter_offset_p(rgb - blue, inc, gb.val[1], p);
            vstrbq_scatter_offset_p(rgb, inc, rg.val[1], p);
            vstrbq_scatter_offset_p(rgb + blue, inc, rg.val[0], p);
            vstrbq_scatter_offset_p(rgb + 2, inc, a, p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, bg.val[0], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, bg.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, gr.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + 2, inc, a, p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[-1] = bayer[bayerStep + 1];
                rgb[0] = bayer[1];
                rgb[1] = bayer[0];
                rgb[2] = 0xff;

                rgb[3] = bayer[bayerStep + 1];
                rgb[4] = bayer[bayerStep + 2];
                rgb[5] = bayer[2];
                rgb[6] = 0xff;
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[2] = 0xff;
                rgb[1] = bayer[bayerStep + 1];
                rgb[0] = bayer[1];
                rgb[-1] = bayer[0];

                rgb[6] = 0xff;
                rgb[5] = bayer[bayerStep + 1];
                rgb[4] = bayer[bayerStep + 2];
                rgb[3] = bayer[2];
            }
        }
#endif

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[bayerStep + 1];
            rgb[0] = bayer[1];
            rgb[blue] = bayer[0];
            rgb[2] = 0xff;

            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_simple_rgb565(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 2;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    // Add black border
    imax = sx * sy * rgbBytes;
    for (i = sx * (sy - 1) * rgbBytes; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * rgbBytes;
    for (i = (sx - 1) * rgbBytes; i < imax; i += iinc) {
        int j;
        for (j = 0; j < rgbBytes; j++)
            rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            uint8_t g = (bayer[0] + bayer[bayerStep + 1] + 1) >> 1;
            if (blue > 0) {
                rgb[-1] = ((g & 0xfc) << 3) | (bayer[bayerStep] >> 3);
                rgb[0] = (bayer[1] & 0xf8) | (g >> 5);
            } else {
                rgb[-1] = ((g & 0xfc) << 3) | (bayer[1] >> 3);
                rgb[0] = (bayer[bayerStep] & 0xf8) | (g >> 5);
            }

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
		// Helium lets us process 16 at a time (8 per beat on Cortex-M55)
		int pairs_to_go = (bayerEnd - bayer) / 2;
		while (pairs_to_go > 0) {
			mve_pred16_t p = vctp8q(pairs_to_go);
			uint8x16x2_t rg = vld2q(bayer);
			uint8x16x2_t gb = vld2q(bayer + bayerStep);
#ifndef __ICCARM__
			__builtin_prefetch(bayer + bayerStep + 16 * 2 * 2);
#endif
            uint8x16_t upper;
            uint8x16_t lower;

            uint8x16_t g0 = vrhaddq_x(rg.val[1], gb.val[0], p);
            lower = vshlq_n(g0, 3);
            if (blue > 0) {
                 lower = vsriq(lower, gb.val[1], 3);
                 upper = vsriq(rg.val[0], g0, 5);
            } else {
                 lower = vsriq(lower, rg.val[0], 3);
                 upper = vsriq(gb.val[1], g0, 5);
            }
            vstrbq_scatter_offset_p(rgb - 1, inc, lower, p);
            vstrbq_scatter_offset_p(rgb, inc, upper, p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);

            uint8x16_t g1 = vrhaddq_x(gr.val[0], bg.val[1], p);
            lower = vshlq_n(g1, 3);
            if (blue > 0) {
                upper = vsriq(gr.val[1], g1, 5);
                lower = vsriq(lower, bg.val[0], 3);
            } else {
                upper = vsriq(bg.val[0], g1, 5);
                lower = vsriq(lower, gr.val[1], 3);
            }
            vstrbq_scatter_offset_p(rgb + rgbBytes - 1, inc, lower, p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, upper, p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
		}

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                uint8_t g0 = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[-1] = ((g0 & 0xfc) << 3) | (bayer[bayerStep + 1] >> 3);
                rgb[0] = (bayer[0] & 0xf8) | (g0 >> 5);

                uint8_t g1 = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[1] = ((g1 & 0xfc) << 3) | (bayer[bayerStep + 1] >> 3);
                rgb[2] = (bayer[2] & 0xf8) | (g1 >> 5);
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                uint8_t g0 = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[-1] = ((g0 & 0xfc) << 3) | (bayer[0] >> 3);
                rgb[0] = (bayer[bayerStep + 1] & 0xf8) | (g0 >> 5);

                uint8_t g1 = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[1] = ((g1 & 0xfc) << 3) | (bayer[2] >> 3);
                rgb[2] = (bayer[bayerStep + 1] & 0xf8) | (g1 >> 5);
            }
        }
#endif

        if (bayer < bayerEnd) {
            uint8_t g = (bayer[1] + bayer[bayerStep] + 1) >> 1;
            if (blue > 0) {
                rgb[-1] = ((g & 0xfc) << 3) | (bayer[bayerStep + 1] >> 3);
                rgb[0] = (bayer[0] & 0xf8) | (g >> 5);
            } else {
                rgb[-1] = ((g & 0xfc) << 3) | (bayer[0] >> 3);
                rgb[0] = (bayer[bayerStep + 1] & 0xf8) | (g >> 5);
            }

            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_simple_bgr888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 3;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    // Add black border
    imax = sx * sy * rgbBytes;
    for (i = sx * (sy - 1) * rgbBytes; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * rgbBytes;
    for (i = (sx - 1) * rgbBytes; i < imax; i += iinc) {
        int j;
        for (j = 0; j < rgbBytes; j++)
            rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[bayerStep];
            rgb[0] = (bayer[0] + bayer[bayerStep + 1] + 1) >> 1;
            rgb[blue] = bayer[1];

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg = vld2q(bayer);
            uint8x16x2_t gb = vld2q(bayer + bayerStep);
#ifndef __ICCARM__
            __builtin_prefetch(bayer + bayerStep + 16*2*2);
#endif
            vstrbq_scatter_offset_p(rgb - blue, inc, gb.val[1], p);
            uint8x16_t g0 = vrhaddq_x(rg.val[1], gb.val[0], p);
            vstrbq_scatter_offset_p(rgb, inc, g0, p);
            vstrbq_scatter_offset_p(rgb + blue, inc, rg.val[0], p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, bg.val[0], p);
            uint8x16_t g1 = vrhaddq_x(gr.val[0], bg.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, g1, p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, gr.val[1], p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[-1] = bayer[bayerStep + 1];
                rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[1] = bayer[0];

                rgb[2] = bayer[bayerStep + 1];
                rgb[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[4] = bayer[2];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[1] = bayer[bayerStep + 1];
                rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[-1] = bayer[0];

                rgb[4] = bayer[bayerStep + 1];
                rgb[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[2] = bayer[2];
            }
        }
#endif

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[bayerStep + 1];
            rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
            rgb[blue] = bayer[0];
            
            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_simple_rgb888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 3;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    // Add black border
    imax = sx * sy * rgbBytes;
    for (i = sx * (sy - 1) * rgbBytes; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * rgbBytes;
    for (i = (sx - 1) * rgbBytes; i < imax; i += iinc) {
        int j;
        for (j = 0; j < rgbBytes; j++)
            rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[1];
            rgb[0] = (bayer[0] + bayer[bayerStep + 1] + 1) >> 1;
            rgb[blue] = bayer[bayerStep];

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg = vld2q(bayer);
            uint8x16x2_t gb = vld2q(bayer + bayerStep);
#ifndef __ICCARM__
            __builtin_prefetch(bayer + bayerStep + 16*2*2);
#endif
            vstrbq_scatter_offset_p(rgb - blue, inc, rg.val[0], p);
            uint8x16_t g0 = vrhaddq_x(rg.val[1], gb.val[0], p);
            vstrbq_scatter_offset_p(rgb, inc, g0, p);
            vstrbq_scatter_offset_p(rgb + blue, inc, gb.val[1], p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, gr.val[1], p);
            uint8x16_t g1 = vrhaddq_x(gr.val[0], bg.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, g1, p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, bg.val[0], p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[-1] = bayer[0];
                rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[1] = bayer[bayerStep + 1];

                rgb[2] = bayer[2];
                rgb[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[4] = bayer[bayerStep + 1];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[1] = bayer[0];
                rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[-1] = bayer[bayerStep + 1];

                rgb[4] = bayer[2];
                rgb[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[2] = bayer[bayerStep + 1];
            }
        }
#endif

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[0];
            rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
            rgb[blue] = bayer[bayerStep + 1];
            
            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_simple_argb8888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 4;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    // Add black border
    imax = sx * sy * rgbBytes;
    for (i = sx * (sy - 1) * rgbBytes; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * rgbBytes;
    for (i = (sx - 1) * rgbBytes; i < imax; i += iinc) {
        int j;
        for (j = 0; j < rgbBytes; j++)
            rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[bayerStep];
            rgb[0] = (bayer[0] + bayer[bayerStep + 1] + 1) >> 1;
            rgb[blue] = bayer[1];
            rgb[2] = 0xff;

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg = vld2q(bayer);
            uint8x16x2_t gb = vld2q(bayer + bayerStep);
            uint8x16_t a = vcreateq_u8(0xffffffffffffffff, 0xffffffffffffffff);
#ifndef __ICCARM__
            __builtin_prefetch(bayer + bayerStep + 16*2*2);
#endif
            vstrbq_scatter_offset_p(rgb - blue, inc, gb.val[1], p);
            uint8x16_t g0 = vrhaddq_x(rg.val[1], gb.val[0], p);
            vstrbq_scatter_offset_p(rgb, inc, g0, p);
            vstrbq_scatter_offset_p(rgb + blue, inc, rg.val[0], p);
            vstrbq_scatter_offset_p(rgb + 2, inc, a, p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, bg.val[0], p);
            uint8x16_t g1 = vrhaddq_x(gr.val[0], bg.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, g1, p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, gr.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + 2, inc, a, p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[-1] = bayer[bayerStep + 1];
                rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[1] = bayer[0];
                rgb[2] = 0xff;

                rgb[3] = bayer[bayerStep + 1];
                rgb[4] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[5] = bayer[2];
                rgb[6] = 0xff;
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                rgb[2] = 0xff;
                rgb[1] = bayer[bayerStep + 1];
                rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[-1] = bayer[0];

                rgb[6] = 0xff;
                rgb[5] = bayer[bayerStep + 1];
                rgb[4] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[3] = bayer[2];
            }
        }
#endif

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[bayerStep + 1];
            rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
            rgb[blue] = bayer[0];
            rgb[2] = 0xff;

            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_linear_rgb565(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 2;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    aipl_bayer_aipl_bayer_clear_borders(rgb, sx, sy, 1);
    rgb += rgbStep + rgbBytes + 1;
    height -= 2;
    width -= 2;


#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        int t0, t1;
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            t0 = (bayer[1] + bayer[bayerStep * 2 + 1] + 1) >> 1;
            t1 = (bayer[bayerStep] + bayer[bayerStep + 2] + 1) >> 1;
            if (blue > 0) {
                rgb[-1] = ((bayer[bayerStep + 1] & 0xfc) << 3) |  ((uint8_t)t1 >> 3);
                rgb[0] = ((uint8_t)t0 & 0xf8) | (bayer[bayerStep + 1] >> 5);
            } else {
                rgb[-1] = ((bayer[bayerStep + 1] & 0xfc) << 3) |  ((uint8_t)t0 >> 3);
                rgb[0] = ((uint8_t)t1 & 0xf8) | (bayer[bayerStep + 1] >> 5);
            }

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg0 = vld2q(bayer);
            uint8x16x2_t gr0 = vld2q(bayer + 1);
            uint8x16x2_t gb0 = vld2q(bayer + bayerStep);
            uint8x16x2_t gb1 = vld2q(bayer + bayerStep + 2);
            uint8x16x2_t rg1 = vld2q(bayer + bayerStep*2);
            uint8x16x2_t gr1 = vld2q(bayer + bayerStep*2 + 1);

            uint8x16_t tmp0 = vrhaddq_x(rg0.val[0], gr0.val[1], p);
            uint8x16_t tmp1 = vrhaddq_x(rg1.val[0], gr1.val[1], p);
            uint8x16_t r0 = vrhaddq_x(tmp0, tmp1, p);

            uint8x16_t tmp2 = vrhaddq_x(rg0.val[1], gb0.val[0], p);
            uint8x16_t tmp3 = vrhaddq_x(gb1.val[0], rg1.val[1], p);
            uint8x16_t g0 = vrhaddq_x(tmp2, tmp3, p);

            uint8x16_t b0 = gb0.val[1];            
            
            uint8x16_t upper;
            uint8x16_t lower;
                        
            lower = vshlq_n(g0, 3);
            if (blue > 0) {
                 lower = vsriq(lower, b0, 3);
                 upper = vsriq(r0, g0, 5);
            } else {
                 lower = vsriq(lower, r0, 3);
                 upper = vsriq(b0, g0, 5);
            }
            vstrbq_scatter_offset_p(rgb - 1, inc, lower, p);
            vstrbq_scatter_offset_p(rgb, inc, upper, p);
                        
            uint8x16_t r1 = vrhaddq_x(gr0.val[1], gr1.val[1], p);
            uint8x16_t g1 = gb1.val[0];
            uint8x16_t b1 = vrhaddq_x(gb0.val[1], gb1.val[1], p);            

            lower = vshlq_n(g1, 3);
            if (blue > 0) {
                 lower = vsriq(lower, b1, 3);
                 upper = vsriq(r1, g1, 5);
            } else {
                 lower = vsriq(lower, r1, 3);
                 upper = vsriq(b1, g1, 5);
            }
            vstrbq_scatter_offset_p(rgb + rgbBytes - 1, inc, lower, p);
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, upper, p);

            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                      bayer[bayerStep*2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                      2) >> 2;
                rgb[-1] = (((uint8_t)t1 & 0xfc) << 3) | (bayer[bayerStep + 1] >> 3);
                rgb[0] = ((uint8_t)t0 & 0xf8) | ((uint8_t)t1 >> 5);

                t0 = (bayer[2] + bayer[bayerStep*2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[1] = ((bayer[bayerStep + 2] & 0xfc) << 3) | ((uint8_t)t1 >> 3);
                rgb[2] = ((uint8_t)t0 & 0xf8) | (bayer[bayerStep + 2] >> 5);
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                      bayer[bayerStep*2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                      2) >> 2;
                rgb[-1] = (((uint8_t)t1 & 0xfc) << 3) | ((uint8_t)t0 >> 3);
                rgb[0] = (bayer[bayerStep + 1] & 0xf8) | ((uint8_t)t1 >> 5);

                t0 = (bayer[2] + bayer[bayerStep*2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[1] = ((bayer[bayerStep + 2] & 0xfc) << 3) | ((uint8_t)t0 >> 3);
                rgb[2] = ((uint8_t)t1 & 0xf8) | (bayer[bayerStep + 2] >> 5);
            }
        }
#endif

        if (bayer < bayerEnd) {
            t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                  bayer[bayerStep*2 + 2] + 2) >> 2;
            t1 = (bayer[1] + bayer[bayerStep] +
                  bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                  2) >> 2;
            if (blue > 0) {
                rgb[-1] = (((uint8_t)t1 & 0xfc) << 3) | (bayer[bayerStep + 1] >> 3);
                rgb[0] = ((uint8_t)t0 & 0xf8) | ((uint8_t)t1 >> 5);
            } else {
                rgb[-1] = (((uint8_t)t1 & 0xfc) << 3) | ((uint8_t)t0 >> 3);
                rgb[0] = (bayer[bayerStep + 1] & 0xf8) | ((uint8_t)t1 >> 5);
            }

            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_linear_bgr888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 3;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    aipl_bayer_aipl_bayer_clear_borders(rgb, sx, sy, 1);
    rgb += rgbStep + rgbBytes + 1;
    height -= 2;
    width -= 2;


#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        int t0, t1;
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            t0 = (bayer[1] + bayer[bayerStep * 2 + 1] + 1) >> 1;
            t1 = (bayer[bayerStep] + bayer[bayerStep + 2] + 1) >> 1;
            rgb[-blue] = (uint8_t) t1;
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = (uint8_t) t0;

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg0 = vld2q(bayer);
            uint8x16x2_t gr0 = vld2q(bayer + 1);
            uint8x16x2_t gb0 = vld2q(bayer + bayerStep);
            uint8x16x2_t gb1 = vld2q(bayer + bayerStep + 2);
            uint8x16x2_t rg1 = vld2q(bayer + bayerStep*2);
            uint8x16x2_t gr1 = vld2q(bayer + bayerStep*2 + 1);

            uint8x16_t b0 = gb0.val[1];            
            vstrbq_scatter_offset_p(rgb - blue, inc, b0, p);

            uint8x16_t tmp2 = vrhaddq_x(rg0.val[1], gb0.val[0], p);
            uint8x16_t tmp3 = vrhaddq_x(gb1.val[0], rg1.val[1], p);
            uint8x16_t g0 = vrhaddq_x(tmp2, tmp3, p);
            vstrbq_scatter_offset_p(rgb, inc, g0, p);

            uint8x16_t tmp0 = vrhaddq_x(rg0.val[0], gr0.val[1], p);
            uint8x16_t tmp1 = vrhaddq_x(rg1.val[0], gr1.val[1], p);
            uint8x16_t r0 = vrhaddq_x(tmp0, tmp1, p);
            vstrbq_scatter_offset_p(rgb + blue, inc, r0, p);
            
            uint8x16_t b1 = vrhaddq_x(gb0.val[1], gb1.val[1], p);            
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, b1, p);

            uint8x16_t g1 = gb1.val[0];
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, g1, p);

            uint8x16_t r1 = vrhaddq_x(gr0.val[1], gr1.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, r1, p);
            
            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                      bayer[bayerStep*2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                      2) >> 2;
                rgb[-1] = (uint8_t) bayer[bayerStep + 1];
                rgb[0] = (uint8_t) t1;
                rgb[1] = t0;

                t0 = (bayer[2] + bayer[bayerStep*2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[2] = (uint8_t) t1;
                rgb[3] = bayer[bayerStep + 2];
                rgb[4] = (uint8_t) t0;
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                      bayer[bayerStep*2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                      2) >> 2;
                rgb[1] = bayer[bayerStep + 1];
                rgb[0] = (uint8_t) t1;
                rgb[-1] = (uint8_t) t0;

                t0 = (bayer[2] + bayer[bayerStep*2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[4] = (uint8_t) t1;
                rgb[3] = bayer[bayerStep + 2];
                rgb[2] = (uint8_t) t0;
            }
        }
#endif

        if (bayer < bayerEnd) {
            t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                  bayer[bayerStep*2 + 2] + 2) >> 2;
            t1 = (bayer[1] + bayer[bayerStep] +
                  bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                  2) >> 2;
            rgb[-blue] = bayer[bayerStep + 1];
            rgb[0] = (uint8_t) t1;
            rgb[blue] = (uint8_t) t0;
            
            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_linear_rgb888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 3;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    aipl_bayer_aipl_bayer_clear_borders(rgb, sx, sy, 1);
    rgb += rgbStep + rgbBytes + 1;
    height -= 2;
    width -= 2;


#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        int t0, t1;
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            t0 = (bayer[1] + bayer[bayerStep * 2 + 1] + 1) >> 1;
            t1 = (bayer[bayerStep] + bayer[bayerStep + 2] + 1) >> 1;
            rgb[-blue] = (uint8_t) t0;
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = (uint8_t) t1;

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg0 = vld2q(bayer);
            uint8x16x2_t gr0 = vld2q(bayer + 1);
            uint8x16x2_t gb0 = vld2q(bayer + bayerStep);
            uint8x16x2_t gb1 = vld2q(bayer + bayerStep + 2);
            uint8x16x2_t rg1 = vld2q(bayer + bayerStep*2);
            uint8x16x2_t gr1 = vld2q(bayer + bayerStep*2 + 1);

            uint8x16_t tmp0 = vrhaddq_x(rg0.val[0], gr0.val[1], p);
            uint8x16_t tmp1 = vrhaddq_x(rg1.val[0], gr1.val[1], p);
            uint8x16_t r0 = vrhaddq_x(tmp0, tmp1, p);
            vstrbq_scatter_offset_p(rgb - blue, inc, r0, p);

            uint8x16_t tmp2 = vrhaddq_x(rg0.val[1], gb0.val[0], p);
            uint8x16_t tmp3 = vrhaddq_x(gb1.val[0], rg1.val[1], p);
            uint8x16_t g0 = vrhaddq_x(tmp2, tmp3, p);
            vstrbq_scatter_offset_p(rgb, inc, g0, p);

            uint8x16_t b0 = gb0.val[1];            
            vstrbq_scatter_offset_p(rgb + blue, inc, b0, p);
            
            uint8x16_t r1 = vrhaddq_x(gr0.val[1], gr1.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, r1, p);

            uint8x16_t g1 = gb1.val[0];
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, g1, p);

            uint8x16_t b1 = vrhaddq_x(gb0.val[1], gb1.val[1], p);            
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, b1, p);
            
            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                      bayer[bayerStep*2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                      2) >> 2;
                rgb[-1] = (uint8_t) t0;
                rgb[0] = (uint8_t) t1;
                rgb[1] = bayer[bayerStep + 1];

                t0 = (bayer[2] + bayer[bayerStep*2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[2] = (uint8_t) t1;
                rgb[3] = bayer[bayerStep + 2];
                rgb[4] = (uint8_t) t0;
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                      bayer[bayerStep*2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                      2) >> 2;
                rgb[1] = bayer[bayerStep + 1];
                rgb[0] = (uint8_t) t1;
                rgb[-1] = (uint8_t) t0;

                t0 = (bayer[2] + bayer[bayerStep*2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[4] = (uint8_t) t0;
                rgb[3] = bayer[bayerStep + 2];
                rgb[2] = (uint8_t) t1;
            }
        }
#endif

        if (bayer < bayerEnd) {
            t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                  bayer[bayerStep*2 + 2] + 2) >> 2;
            t1 = (bayer[1] + bayer[bayerStep] +
                  bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                  2) >> 2;
            rgb[-blue] = (uint8_t) t0;
            rgb[0] = (uint8_t) t1;
            rgb[blue] = bayer[bayerStep + 1];
            
            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

aipl_error_t aipl_bayer_decoding_linear_argb8888(
    const uint8_t *restrict bayer,
    uint8_t *restrict rgb,
    int sx, int sy, int tile)
{
    const int rgbBytes = 4;
    const int bayerStep = sx;
    const int rgbStep = rgbBytes * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;

    if ((tile>AIPL_COLOR_FILTER_MAX)||(tile<AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    aipl_bayer_aipl_bayer_clear_borders(rgb, sx, sy, 1);
    rgb += rgbStep + rgbBytes + 1;
    height -= 2;
    width -= 2;


#ifdef AIPL_HELIUM_ACCELERATION
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc = vmulq_n_u8(vidupq_n_u8(0, 1), 2*rgbBytes);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        int t0, t1;
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            t0 = (bayer[1] + bayer[bayerStep * 2 + 1] + 1) >> 1;
            t1 = (bayer[bayerStep] + bayer[bayerStep + 2] + 1) >> 1;
            rgb[-blue] = (uint8_t) t1;
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = (uint8_t) t0;
            rgb[2] = 0xff;

            bayer++;
            rgb += rgbBytes;
        }

#ifdef AIPL_HELIUM_ACCELERATION
        // Helium lets us process 16 at a time (8 per beat on Cortex-M55)
        int pairs_to_go = (bayerEnd - bayer)/2;
        while (pairs_to_go > 0) {
            mve_pred16_t p = vctp8q(pairs_to_go);
            uint8x16x2_t rg0 = vld2q(bayer);
            uint8x16x2_t gr0 = vld2q(bayer + 1);
            uint8x16x2_t gb0 = vld2q(bayer + bayerStep);
            uint8x16x2_t gb1 = vld2q(bayer + bayerStep + 2);
            uint8x16x2_t rg1 = vld2q(bayer + bayerStep*2);
            uint8x16x2_t gr1 = vld2q(bayer + bayerStep*2 + 1);
            uint8x16_t a = vcreateq_u8(0xffffffffffffffff, 0xffffffffffffffff);

            uint8x16_t b0 = gb0.val[1];            
            vstrbq_scatter_offset_p(rgb - blue, inc, b0, p);

            uint8x16_t tmp2 = vrhaddq_x(rg0.val[1], gb0.val[0], p);
            uint8x16_t tmp3 = vrhaddq_x(gb1.val[0], rg1.val[1], p);
            uint8x16_t g0 = vrhaddq_x(tmp2, tmp3, p);
            vstrbq_scatter_offset_p(rgb, inc, g0, p);
            
            uint8x16_t tmp0 = vrhaddq_x(rg0.val[0], gr0.val[1], p);
            uint8x16_t tmp1 = vrhaddq_x(rg1.val[0], gr1.val[1], p);
            uint8x16_t r0 = vrhaddq_x(tmp0, tmp1, p);
            vstrbq_scatter_offset_p(rgb + blue, inc, r0, p);

            vstrbq_scatter_offset_p(rgb + 2, inc, a, p);

            uint8x16_t b1 = vrhaddq_x(gb0.val[1], gb1.val[1], p);            
            vstrbq_scatter_offset_p(rgb + rgbBytes - blue, inc, b1, p);

            uint8x16_t g1 = gb1.val[0];
            vstrbq_scatter_offset_p(rgb + rgbBytes, inc, g1, p);

            uint8x16_t r1 = vrhaddq_x(gr0.val[1], gr1.val[1], p);
            vstrbq_scatter_offset_p(rgb + rgbBytes + blue, inc, r1, p);

            vstrbq_scatter_offset_p(rgb + rgbBytes + 2, inc, a, p);
            
            bayer += 16 * 2;
            rgb += 16 * 2*rgbBytes;
            pairs_to_go -= 16;
        }

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 2*rgbBytes;
#else
        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                      bayer[bayerStep*2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                      2) >> 2;
                rgb[-1] = bayer[bayerStep + 1];
                rgb[0] = (uint8_t) t1;
                rgb[1] = (uint8_t) t0;
                rgb[2] = 0xff;

                t0 = (bayer[2] + bayer[bayerStep*2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[3] = (uint8_t) t1;
                rgb[4] = bayer[bayerStep + 2];
                rgb[5] = (uint8_t) t0;
                rgb[6] = 0xff;
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 2*rgbBytes) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                      bayer[bayerStep*2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                      2) >> 2;
                rgb[2] = 0xff;
                rgb[1] = bayer[bayerStep + 1];
                rgb[0] = (uint8_t) t1;
                rgb[-1] = (uint8_t) t0;

                t0 = (bayer[2] + bayer[bayerStep*2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[6] = 0xff;
                rgb[5] = (uint8_t) t1;
                rgb[4] = bayer[bayerStep + 2];
                rgb[3] = (uint8_t) t0;
            }
        }
#endif

        if (bayer < bayerEnd) {
            t0 = (bayer[0] + bayer[2] + bayer[bayerStep*2] +
                  bayer[bayerStep*2 + 2] + 2) >> 2;
            t1 = (bayer[1] + bayer[bayerStep] +
                  bayer[bayerStep + 2] + bayer[bayerStep*2 + 1] +
                  2) >> 2;
            rgb[-blue] = bayer[bayerStep + 1];
            rgb[0] = (uint8_t) t1;
            rgb[blue] = (uint8_t) t0;
            rgb[2] = 0xff;
            
            bayer++;
            rgb += rgbBytes;
        }

        bayer -= width;
        rgb -= width * rgbBytes;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void aipl_bayer_aipl_bayer_clear_borders(uint8_t *rgb,
                                                int sx, int sy,
                                                int w)
{
    int i, j;
    // black edges are added with a width w:
    i = 3 * sx * w - 1;
    j = 3 * sx * sy - 1;
    while (i >= 0) {
        rgb[i--] = 0;
        rgb[j--] = 0;
    }

    int low = sx * (w - 1) * 3 - 1 + w * 3;
    i = low + sx * (sy - w * 2 + 1) * 3;
    while (i > low) {
        j = 6 * w;
        while (j > 0) {
            rgb[i--] = 0;
            j--;
        }
        i -= (sx - 2 * w) * 3;
    }
}
