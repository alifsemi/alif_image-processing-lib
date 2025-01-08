/**
 * @file aipl_flip.c
 *
 */

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

    if (flip_horizontal || flip_vertical)
    {
        for (y = 0; y < height / 2; ++y)
        {
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
                    output[left] = input[right];
                    output[right++] = input[left++];
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
