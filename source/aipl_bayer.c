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
 * @file    aipl_bayer.c
 * @brief   Bayer pattern decoding functions
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
 *
******************************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include "aipl_bayer.h"
#include <RTE_Device.h>
#include "aipl_config.h"
#if AIPL_USE_MVE
#include <arm_mve.h>
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    AIPL_BAYER_RGB,
    AIPL_BAYER_BGR
} aipl_bayer_rgb_order_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void aipl_bayer_aipl_bayer_clear_borders(uint8_t *rgb, int sx, int sy,
                                                int w);
static aipl_error_t aipl_bayer_decoding(const void *restrict bayer,
                                        void *restrict rgb,
                                        uint32_t width, uint32_t height,
                                        aipl_color_filter_t tile,
                                        aipl_bayer_method_t method,
                                        aipl_bayer_rgb_order_t order);
static aipl_error_t aipl_bayer_nearest_neighbor(const uint8_t *restrict bayer,
                                                uint8_t *restrict rgb,
                                                int sx, int sy,
                                                int tile, int order);
static aipl_error_t aipl_bayer_simple(const uint8_t * restrict bayer,
                                      uint8_t * restrict rgb,
                                      int sx, int sy,
                                      int tile, int order);
static aipl_error_t aipl_bayer_bilinear(const uint8_t *restrict bayer,
                                        uint8_t *restrict rgb,
                                        int sx, int sy,
                                        int tile, int order);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
aipl_error_t aipl_bayer_decoding_rgb888(const void *bayer, void *output,
                                         uint32_t width, uint32_t height,
                                         aipl_color_filter_t tile,
                                         aipl_bayer_method_t method)
{
    return aipl_bayer_decoding(bayer, output, width, height, tile, method,
                               AIPL_BAYER_RGB);
}

aipl_error_t aipl_bayer_decoding_bgr888(const void *bayer, void *output,
                                        uint32_t width, uint32_t height,
                                        aipl_color_filter_t tile,
                                        aipl_bayer_method_t method)
{
    return aipl_bayer_decoding(bayer, output, width, height, tile, method,
                               AIPL_BAYER_BGR);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
aipl_error_t aipl_bayer_decoding(const void *restrict bayer,
                                  void *restrict rgb,
                                  uint32_t width, uint32_t height,
                                  aipl_color_filter_t tile,
                                  aipl_bayer_method_t method,
                                  aipl_bayer_rgb_order_t order)
{
    switch (method) {
        case AIPL_BAYER_METHOD_NEAREST:
            return aipl_bayer_nearest_neighbor((const uint8_t*)bayer,
                                                (uint8_t*)rgb,
                                                width, height,
                                                tile, order);
        case AIPL_BAYER_METHOD_SIMPLE:
            return aipl_bayer_simple((const uint8_t*)bayer,
                                     (uint8_t*)rgb,
                                     width, height,
                                     tile, order);
        case AIPL_BAYER_METHOD_BILINEAR:
            return aipl_bayer_bilinear((const uint8_t*)bayer,
                                       (uint8_t*)rgb,
                                       width, height,
                                       tile, order);
        default:
            return AIPL_ERR_BAYER_INVALID_METHOD;
    }
}

static void aipl_bayer_clear_borders(uint8_t *rgb, int sx, int sy, int w)
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

static aipl_error_t aipl_bayer_nearest_neighbor(const uint8_t *restrict bayer,
                                                uint8_t *restrict rgb,
                                                int sx, int sy,
                                                int tile, int order)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
        || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    if (order == AIPL_BAYER_BGR)
        blue = -blue;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
        || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile > AIPL_COLOR_FILTER_MAX) || (tile < AIPL_COLOR_FILTER_MIN))
      return AIPL_ERR_BAYER_INVALID_FILTER;

    /* add black border */
    imax = sx * sy * 3;
    for (i = sx * (sy - 1) * 3; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * 3;
    for (i = (sx - 1) * 3; i < imax; i += iinc) {
        rgb[i++] = 0;
        rgb[i++] = 0;
        rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

#if AIPL_USE_MVE & 1
    // Index table into 16 RGB pairs for scatter stores: { 0, 6, 12, .. }
    const uint8x16_t inc6 = vmulq_n_u8(vidupq_n_u8(0, 1), 6);
#endif

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[1];
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = bayer[bayerStep];
            bayer++;
            rgb += 3;
        }

#if AIPL_USE_MVE & 1
		// Helium lets us process 16 at a time (8 per beat on Cortex-M55)
		int pairs_to_go = (bayerEnd - bayer) / 2;
		while (pairs_to_go > 0) {
			mve_pred16_t p = vctp8q(pairs_to_go);
			uint8x16x2_t rg = vld2q(bayer);
			uint8x16x2_t gb = vld2q(bayer + bayerStep);
#ifndef __ICCARM__
			__builtin_prefetch(bayer + bayerStep + 16 * 2 * 2);
#endif
            uint8x16_t r0 = rg.val[0];
            uint8x16_t g0 = rg.val[1];
            uint8x16_t b0 = gb.val[1];
            vstrbq_scatter_offset_p(rgb - blue, inc6, r0, p);
            vstrbq_scatter_offset_p(rgb, inc6, g0, p);
            vstrbq_scatter_offset_p(rgb + blue, inc6, b0, p);

            uint8x16x2_t gr = vld2q(bayer + 1);
            uint8x16x2_t bg = vld2q(bayer + bayerStep + 1);
            uint8x16_t r1 = gr.val[1];
            uint8x16_t g1 = bg.val[1];
            uint8x16_t b1 = bg.val[0];
            vstrbq_scatter_offset_p(rgb + 3 - blue, inc6, r1, p);
            vstrbq_scatter_offset_p(rgb + 3, inc6, g1, p);
            vstrbq_scatter_offset_p(rgb + 3 + blue, inc6, b1, p);
            bayer += 16 * 2;
            rgb += 16 * 6;
            pairs_to_go -= 16;
		}

        bayer += pairs_to_go * 2;
        rgb += pairs_to_go * 6;
#else
        for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
            rgb[-blue] = bayer[0];
            rgb[0] = bayer[1];
            rgb[blue] = bayer[bayerStep + 1];

            rgb[3 - blue] = bayer[2];
            rgb[3] = bayer[bayerStep + 2];
            rgb[3 + blue] = bayer[bayerStep + 1];
        }
#endif

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[0];
            rgb[0] = bayer[1];
            rgb[blue] = bayer[bayerStep + 1];
            bayer++;
            rgb += 3;
        }

        bayer -= width;
        rgb -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }

    return AIPL_ERR_OK;
}

static aipl_error_t aipl_bayer_simple(const uint8_t * restrict bayer,
                                      uint8_t * restrict rgb,
                                      int sx, int sy,
                                      int tile, int order)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
            || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    if (order == AIPL_BAYER_BGR)
        blue = -blue;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
            || tile == AIPL_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    if ((tile > AIPL_COLOR_FILTER_MAX) || (tile < AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    /* add black border */
    imax = sx * sy * 3;
    for (i = sx * (sy - 1) * 3; i < imax; i++)
    {
        rgb[i] = 0;
    }

    iinc = (sx - 1) * 3;
    for (i = (sx - 1) * 3; i < imax; i += iinc)
    {
        rgb[i++] = 0;
        rgb[i++] = 0;
        rgb[i++] = 0;
    }


    rgb += 1;
    width -= 1;
    height -= 1;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const uint8_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[1];
            rgb[0] = (bayer[0] + bayer[bayerStep + 1] + 1) >> 1;
            rgb[blue] = bayer[bayerStep];
            bayer++;
            rgb += 3;
		}

#if AIPL_USE_MVE & 1
		// Helium lets us process 16 at a time (8 per beat on Cortex-M55)
		int pairs_to_go = (bayerEnd - bayer) / 2;
		int tmp;
		// The asm pointers will overrun - easiest to just figure out where we should
		// end up here, and forget the final pointer from the assembler
		const uint8_t *bayerAsm = bayer;
		uint8_t *rgbAsm = rgb;
		bayer += pairs_to_go * 2;
		rgb += pairs_to_go * 6;
		__asm (

CE("    VMOV.I8     Q4,#0\n\t")
"       WLSTP.8     LR, %[pairs_to_go], 2f\n"
CE("    VMOV.I8     Q5,#0\n\t")
"       MOVS        %[tmp], #0\n\t"
"       VIDUP.U8    Q6,%[tmp],#1\n\t"
"       MOVS        %[tmp], #6\n\t"
"       VMUL.U8     Q6,Q6,%[tmp]\n"
"1:\n\t"
"       VLD20.8     {Q0,Q1},[%[bayer]]\n\t"
   "    ADD         %[tmp], %[bayer], %[bayerStep]\n\t"
CE("    VMAX.U8     Q7,Q0,Q1\n\t")
"       VLD21.8     {Q0,Q1},[%[bayer]]!\n\t"
CE("    VPT.U8      HS, Q7, %[expHigh]\n\t")
CE("    VADDT.U8    Q4,Q4,%[one]\n\t")
"       VLD20.8     {Q2,Q3},[%[tmp]]\n\t"
CE("    VMIN.U8     Q7,Q0,Q1\n\t")
"       VLD21.8     {Q2,Q3},[%[tmp]]\n\t"
   "    PLD         [%[tmp], #64]\n\t"
   "    SUB         %[tmp], %[rgb], %[blue]\n\t"
CE("    VPT.U8      HS, Q7, %[expLow]\n\t")
CE("    VADDT.U8    Q5,Q5,%[one]\n\t")
   "    VSTRB.8     Q0,[%[tmp], Q6]\n\t"
   "    VRHADD.U8   Q1,Q1,Q2\n\t"
   "    VSTRB.8     Q1,[%[rgb], Q6]\n\t"
   "    ADD         %[tmp], %[rgb], %[blue]\n\t"
   "    VSTRB.8     Q3,[%[tmp], Q6]\n\t"
   "    SUB         %[tmp], %[bayer], #31\n\t"
   "    VLD20.8     {Q0,Q1}, [%[tmp]]\n\t"
   "    VLD21.8     {Q0,Q1}, [%[tmp]]\n\t"
   "    ADD         %[tmp], %[bayerStep]\n\t"
   "    VLD20.8     {Q2,Q3}, [%[tmp]]\n\t"
   "    VLD21.8     {Q2,Q3}, [%[tmp]]\n\t"
   "    ADD         %[rgb], %[rgb], #3\n\t"
   "    VRHADD.U8   Q0, Q0, Q3\n\t"
   "    VSTRB.8     Q0, [%[rgb], Q6]\n\t"
   "    SUB         %[tmp], %[rgb], %[blue]\n\t"
   "    VSTRB.8     Q1, [%[tmp], Q6]\n\t"
   "    ADD         %[tmp], %[rgb], %[blue]\n\t"
   "    VSTRB.8     Q2, [%[tmp], Q6]\n\t"
   "    ADD         %[rgb], #16*6-3\n\t"
   "    LETP        LR,1b\n"
CE("    VADDVA.U8   %[high_count],Q4\n\t") // Note we rely on image being <= 512 * 16 to avoid overflow before here
CE("    VADDVA.U8   %[not_low_count],Q5\n") // (Could use predicated VADDVAT inside the loop, but VADDT pipelines better)

"2:\n\t"
   : [bayer] "+r"(bayerAsm), [rgb] "+r"(rgbAsm),
     [tmp] "=&Te"(tmp)
CE(, [high_count] "+Te"(high_count), [not_low_count] "+Te"(not_low_count))
   : [bayerStep] "r"(bayerStep), [blue] "r"(blue), [one] "r"(1),
     [expHigh] "r"(THRESH_HIGH),
     [expLow] "r"(THRESH_LOW),
     [pairs_to_go] "r"(pairs_to_go)
   : "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "lr", "cc", "memory");
#else
        for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
            rgb[-blue] = bayer[0];
            rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
            rgb[blue] = bayer[bayerStep + 1];

            rgb[3 - blue] = bayer[2];
            rgb[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
            rgb[3 + blue] = bayer[bayerStep + 1];
        }
#endif
		if (bayer < bayerEnd) {
			rgb[-blue] = bayer[0];
			rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
			rgb[blue] = bayer[bayerStep + 1];
			bayer++;
			rgb += 3;
		}

		bayer -= width;
		rgb -= width * 3;

		blue = -blue;
		start_with_green = !start_with_green;
	}
	return AIPL_ERR_OK;
}

static aipl_error_t aipl_bayer_bilinear(const uint8_t *restrict bayer,
                                        uint8_t *restrict rgb,
                                        int sx, int sy,
                                        int tile, int order)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == AIPL_COLOR_FILTER_BGGR
        || tile == AIPL_COLOR_FILTER_GBRG ? -1 : 1;
    if (order == AIPL_BAYER_BGR)
        blue = -blue;
    int start_with_green = tile == AIPL_COLOR_FILTER_GBRG
        || tile == AIPL_COLOR_FILTER_GRBG;

    if ((tile > AIPL_COLOR_FILTER_MAX) || (tile < AIPL_COLOR_FILTER_MIN))
        return AIPL_ERR_BAYER_INVALID_FILTER;

    aipl_bayer_clear_borders(rgb, sx, sy, 1);
    rgb += rgbStep + 3 + 1;
    height -= 2;
    width -= 2;

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
            rgb += 3;
        }
        for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
            t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
                    bayer[bayerStep * 2 + 2] + 2) >> 2;
            t1 = (bayer[1] + bayer[bayerStep] +
                    bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
                    2) >> 2;
            rgb[-blue] = (uint8_t) t0;
            rgb[0] = (uint8_t) t1;
            rgb[blue] = bayer[bayerStep + 1];

            t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
            t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                    1) >> 1;
            rgb[3 - blue] = (uint8_t) t0;
            rgb[3] = bayer[bayerStep + 2];
            rgb[3 + blue] = (uint8_t) t1;
        }

        if (bayer < bayerEnd) {
            t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
                  bayer[bayerStep * 2 + 2] + 2) >> 2;
            t1 = (bayer[1] + bayer[bayerStep] +
                  bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
                  2) >> 2;
            rgb[-blue] = (uint8_t) t0;
            rgb[0] = (uint8_t) t1;
            rgb[blue] = bayer[bayerStep + 1];
            bayer++;
            rgb += 3;
        }

        bayer -= width;
        rgb -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }
    return AIPL_ERR_OK;
}
