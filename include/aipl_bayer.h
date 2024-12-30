/**
 * @file aipl_bayer.h
 *
 * Bayer pattern decoding functions
 *
 * Based on libdc1394 implementation:
 * https://sourceforge.net/projects/libdc1394/
 *
 */

#ifndef AIPL_BAYER_H
#define AIPL_BAYER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "aipl_color_formats.h"
#include "aipl_error.h"
#include "aipl_image.h"

/**********************
 *      TYPEDEFS
 **********************/

/**
 * A list of supported de-mosaicing techniques for Bayer-patterns.
 */
typedef enum {
    AIPL_BAYER_METHOD_NEAREST = 0,
    AIPL_BAYER_METHOD_SIMPLE,
    AIPL_BAYER_METHOD_BILINEAR
} aipl_bayer_method_t;

/**
 * Color filter tiles
 */
typedef enum {
    AIPL_COLOR_FILTER_RGGB = 512,
    AIPL_COLOR_FILTER_GBRG,
    AIPL_COLOR_FILTER_GRBG,
    AIPL_COLOR_FILTER_BGGR
} aipl_color_filter_t;

/*********************
 *      DEFINES
 *********************/
#define AIPL_COLOR_FILTER_MIN        AIPL_COLOR_FILTER_RGGB
#define AIPL_COLOR_FILTER_MAX        AIPL_COLOR_FILTER_BGGR
#define AIPL_COLOR_FILTER_NUM       (AIPL_COLOR_FILTER_MAX - AIPL_COLOR_FILTER_MIN + 1)

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Perform de-mosaicing on an 8-bit image buffer
 * into RGB888 image
 *
 * @param bayer             input raw image pointer
 * @param output            output RGB image pointer
 * @param width             image width
 * @param height            image height
 * @param tile              color filter tiles
 * @param method            de-mosaicing techniques
 * @return error code
 */
aipl_error_t aipl_bayer_decoding_rgb888(const void *bayer, void *output,
                                         uint32_t width, uint32_t height,
                                         aipl_color_filter_t tile,
                                         aipl_bayer_method_t method);

/**
 * Perform de-mosaicing on an 8-bit image buffer
 * into BGR888 image
 *
 * @param bayer             input raw image pointer
 * @param output            output RGB image pointer
 * @param width             image width
 * @param height            image height
 * @param tile              color filter tiles
 * @param method            de-mosaicing techniques
 * @return error code
 */
aipl_error_t aipl_bayer_decoding_bgr888(const void *bayer, void *output,
                                         uint32_t width, uint32_t height,
                                         aipl_color_filter_t tile,
                                         aipl_bayer_method_t method);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  /* AIPL_RESIZE_H */
