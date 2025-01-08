/**
 * @file aipl_rotate.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "aipl_rotate.h"
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
static aipl_error_t aipl_rotate_sw(const uint8_t* restrict input,
                                   uint8_t* restrict output,
                                   uint32_t width, uint32_t height,
                                   aipl_color_format_t format,
                                   aipl_rotation_t rotation);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
 aipl_error_t aipl_rotate(const void* input, void* output,
                          uint32_t pitch,
                          uint32_t width, uint32_t height,
                          aipl_color_format_t format,
                          aipl_rotation_t rotation)
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
                            rotation,
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
            return aipl_rotate_sw(input, output,
                                width, height,
                                format, rotation);
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

aipl_error_t aipl_rotate_img(const aipl_image_t* input,
                             aipl_image_t* output,
                             aipl_rotation_t rotation)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (output->format != input->format)
        return AIPL_ERR_FORMAT_MISMATCH;

    if ((rotation == AIPL_ROTATE_0 || rotation == AIPL_ROTATE_180) &&
        (input->width != output->width || input->height != output->height))
        return AIPL_ERR_SIZE_MISMATCH;

    if ((rotation == AIPL_ROTATE_90 || rotation == AIPL_ROTATE_270) &&
        (input->width != output->height || input->height != output->width))
        return AIPL_ERR_SIZE_MISMATCH;

    return aipl_rotate(input->data, output->data,
                     input->pitch,
                     input->width, input->height,
                     input->format,
                     rotation);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static aipl_error_t aipl_rotate_sw(const uint8_t* restrict input,
                                   uint8_t* restrict output,
                                   uint32_t width, uint32_t height,
                                   aipl_color_format_t format,
                                   aipl_rotation_t rotation)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const int rgbBytes = aipl_color_format_depth(format)/8;
    int x, y, j;

    if (rotation == AIPL_ROTATE_90)
    {
        for (y = 0; y < height; ++y)
        {
            for (x = 0; x < width; ++x)
            {
                int input_offset = (y * width + x) * rgbBytes;
                int output_offset = (x * height + height - 1 - y) * rgbBytes;
                for (j = 0; j < rgbBytes; j++)
                    output[output_offset++] = input[input_offset++];
            }
        }
    }
    else if (rotation == AIPL_ROTATE_180)
    {
        for (y = 0; y < height; ++y)
        {
            for (x = 0; x < width; ++x)
            {
                int input_offset = (y * width + x) * rgbBytes;
                int output_offset = ((height - 1 - y) * width + width - 1 - x) * rgbBytes;
                for (j = 0; j < rgbBytes; j++)
                    output[output_offset++] = input[input_offset++];
            }
        }
    }
    else if (rotation == AIPL_ROTATE_270)
    {
        for (y = 0; y < height; ++y)
        {
            for (x = 0; x < width; ++x)
            {
                int input_offset = (y * width + x) * rgbBytes;
                int output_offset = ((width - 1 - x) * height + y) * rgbBytes;
                for (j = 0; j < rgbBytes; j++)
                    output[output_offset++] = input[input_offset++];
            }
        }
    }
    else
        return AIPL_ERR_NOT_SUPPORTED;

    size_t size = width * height * rgbBytes;
    aipl_cpu_cache_clean(output, size);

    return AIPL_ERR_OK;

}
