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
 aipl_error_t aipl_flip(const void* input, void* output,
                       uint32_t pitch,
                       uint32_t width, uint32_t height,
                       aipl_color_format_t format,
                       bool flip_horizontal, bool flip_vertical)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    if (aipl_dave2d_format_supported(format))
    {
        d2_u32 ret = aipl_dave2d_texturing(input, output,
                            pitch,
                            width, height,
                            aipl_dave2d_format_to_mode(format),
                            width, height,
                            0,
                            flip_horizontal, flip_vertical,
                            false);

        return aipl_dave2d_error_convert(ret);
    }
#else
    // Software mode is not implemented
    return AIPL_ERR_UNIMPLEMENTED;
#endif
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

