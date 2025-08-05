/* Copyright (C) 2025 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */

/******************************************************************************
 * @file    aipl_color_formats.h
 * @brief   Color formats type and utils definitions
 *
******************************************************************************/

#ifndef AIPL_COLOR_FORMATS_H
#define AIPL_COLOR_FORMATS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/
#define AIPL_YUV_Y_8BIT_RED_MULTIPLIER      (66)
#define AIPL_YUV_Y_8BIT_GREEN_MULTIPLIER    (129)
#define AIPL_YUV_Y_8BIT_BLUE_MULTIPLIER     (25)

#define AIPL_YUV_U_8BIT_RED_MULTIPLIER      (-38)
#define AIPL_YUV_U_8BIT_GREEN_MULTIPLIER    (-74)
#define AIPL_YUV_U_8BIT_BLUE_MULTIPLIER     (112)

#define AIPL_YUV_V_8BIT_RED_MULTIPLIER      (112)
#define AIPL_YUV_V_8BIT_GREEN_MULTIPLIER    (-94)
#define AIPL_YUV_V_8BIT_BLUE_MULTIPLIER     (-18)

#define AIPL_YUV_Y_4BIT_RED_MULTIPLIER      (66 * 0x11)
#define AIPL_YUV_Y_4BIT_GREEN_MULTIPLIER    (129 * 0x11)
#define AIPL_YUV_Y_4BIT_BLUE_MULTIPLIER     (25 * 0x11)

#define AIPL_YUV_U_4BIT_RED_MULTIPLIER      (-38 * 0x11)
#define AIPL_YUV_U_4BIT_GREEN_MULTIPLIER    (-74 * 0x11)
#define AIPL_YUV_U_4BIT_BLUE_MULTIPLIER     (112 * 0x11)

#define AIPL_YUV_V_4BIT_RED_MULTIPLIER      (112 * 0x11)
#define AIPL_YUV_V_4BIT_GREEN_MULTIPLIER    (-94 * 0x11)
#define AIPL_YUV_V_4BIT_BLUE_MULTIPLIER     (-18 * 0x11)

#define AIPL_YUV_Y_5BIT_RED_MULTIPLIER      (543)
#define AIPL_YUV_Y_5BIT_GREEN_MULTIPLIER    (1061)
#define AIPL_YUV_Y_5BIT_BLUE_MULTIPLIER     (205)

#define AIPL_YUV_U_5BIT_RED_MULTIPLIER      (-312)
#define AIPL_YUV_U_5BIT_GREEN_MULTIPLIER    (-608)
#define AIPL_YUV_U_5BIT_BLUE_MULTIPLIER     (920)

#define AIPL_YUV_V_5BIT_RED_MULTIPLIER      (920)
#define AIPL_YUV_V_5BIT_GREEN_MULTIPLIER    (-773)
#define AIPL_YUV_V_5BIT_BLUE_MULTIPLIER     (-147)

#define AIPL_YUV_Y_6BIT_GREEN_MULTIPLIER    (522)

#define AIPL_YUV_U_6BIT_GREEN_MULTIPLIER    (-299)

#define AIPL_YUV_V_6BIT_GREEN_MULTIPLIER    (-380)

#define AIPL_YUV_C_MULTIPLIER       (298)
#define AIPL_YUV_RED_E_MULTIPLIER   (409)
#define AIPL_YUV_GREEN_D_MULTIPLIER (-100)
#define AIPL_YUV_GREEN_E_MULTIPLIER (-208)
#define AIPL_YUV_BLUE_D_MULTIPLIER  (516)

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    AIPL_SPACE_RGB,
    AIPL_SPACE_YUV
} aipl_color_space_t;

typedef enum {
    /* Alpha color formats */
    AIPL_COLOR_ALPHA8,

    /* RGB color formats */
    AIPL_COLOR_ARGB8888,
    AIPL_COLOR_ARGB4444,
    AIPL_COLOR_ARGB1555,
    AIPL_COLOR_RGBA8888,
    AIPL_COLOR_RGBA4444,
    AIPL_COLOR_RGBA5551,
    AIPL_COLOR_BGR888,
    AIPL_COLOR_RGB888,
    AIPL_COLOR_RGB565,

    /* YUV color formats */
    AIPL_COLOR_YV12,
    AIPL_COLOR_I420,
    AIPL_COLOR_I422,
    AIPL_COLOR_I444,
    AIPL_COLOR_I400,
    AIPL_COLOR_NV21,
    AIPL_COLOR_NV12,
    AIPL_COLOR_YUY2,
    AIPL_COLOR_UYVY,

    /* Other format flags */
    AIPL_COLOR_RLE = 0x80000000,

    /* Unknown color value */
    AIPL_COLOR_UNKNOWN = 0xffffffff
} aipl_color_format_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
/**
 * Get the color format space
 *
 * @param format color format
 * @return color format space
 */
aipl_color_space_t aipl_color_format_space(aipl_color_format_t format);

/**
 * Get the color format depth in bits
 *
 * @param format color format
 * @return color format depth
 */
uint8_t aipl_color_format_depth(aipl_color_format_t format);

/**
 * Get color format string representation
 *
 * @param format color format
 * @return static color format string
 */
const char* aipl_color_format_str(aipl_color_format_t format);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  /* AIPL_COLOR_FORMATS_H */
