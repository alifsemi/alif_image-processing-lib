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
 * @file    aipl_color_conversion.c
 * @brief   Color conversion functions implementation
 *
******************************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include "aipl_color_conversion.h"
#include <RTE_Device.h>
#include "aipl_config.h"
#ifdef AIPL_DAVE2D_ACCELERATION
#include "aipl_dave2d.h"
#endif
#include <string.h>
#ifdef AIPL_HELIUM_ACCELERATION
#include "aipl_mve_utils.h"
#endif
#include "aipl_utils.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
#if (AIPL_CONVERT_ALPHA8_I400 & TO_BGR888 | AIPL_CONVERT_ALPHA8_I400 & TO_RGB888)
aipl_error_t aipl_color_convert_alpha8_to_24bit(const void* input,
                                                void* output,
                                                uint32_t pitch,
                                                uint32_t width,
                                                uint32_t height,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_BGR888 | AIPL_CONVERT_ARGB8888 & TO_RGB888)
aipl_error_t aipl_color_convert_argb8888_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_BGR888 | AIPL_CONVERT_ARGB4444 & TO_RGB888)
aipl_error_t aipl_color_convert_argb4444_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_BGR888 | AIPL_CONVERT_ARGB1555 & TO_RGB888)
aipl_error_t aipl_color_convert_argb1555_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_BGR888 | AIPL_CONVERT_RGBA8888 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba8888_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_BGR888 | AIPL_CONVERT_RGBA4444 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba4444_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_BGR888 | AIPL_CONVERT_RGBA5551 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba5551_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB565 & TO_BGR888 | AIPL_CONVERT_RGB565 & TO_RGB888)
aipl_error_t aipl_color_convert_rgb565_to_24bit(const void* input,
                                                void* output,
                                                uint32_t pitch,
                                                uint32_t width,
                                                uint32_t height,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_BGR888 | AIPL_CONVERT_BGR888 & TO_RGB888)
aipl_error_t aipl_color_convert_24bit_to_24bit(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height,
                                               uint8_t r_in_offset,
                                               uint8_t g_in_offset,
                                               uint8_t b_in_offset,
                                               uint8_t r_out_offset,
                                               uint8_t g_out_offset,
                                               uint8_t b_out_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_ALPHA8_I400 | AIPL_CONVERT_BGR888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_24bit_to_alpha8(const void* input,
                                                void* output,
                                                uint32_t pitch,
                                                uint32_t width,
                                                uint32_t height,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_ARGB8888 | AIPL_CONVERT_BGR888 & TO_ARGB8888)
aipl_error_t aipl_color_convert_24bit_to_argb8888(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_ARGB4444 | AIPL_CONVERT_BGR888 & TO_ARGB4444)
aipl_error_t aipl_color_convert_24bit_to_argb4444(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_ARGB1555 | AIPL_CONVERT_BGR888 & TO_ARGB1555)
aipl_error_t aipl_color_convert_24bit_to_argb1555(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_RGBA8888 | AIPL_CONVERT_BGR888 & TO_RGBA8888)
aipl_error_t aipl_color_convert_24bit_to_rgba8888(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_RGBA4444 | AIPL_CONVERT_BGR888 & TO_RGBA4444)
aipl_error_t aipl_color_convert_24bit_to_rgba4444(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_RGBA5551 | AIPL_CONVERT_BGR888 & TO_RGBA5551)
aipl_error_t aipl_color_convert_24bit_to_rgba5551(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_RGB565 | AIPL_CONVERT_BGR888 & TO_RGB565)
aipl_error_t aipl_color_convert_24bit_to_rgb565(const void* input,
                                                void* output,
                                                uint32_t pitch,
                                                uint32_t width,
                                                uint32_t height,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_I422 | AIPL_CONVERT_BGR888 & TO_I422) \
    && defined (AIPL_HELIUM_ACCELERATION)
aipl_error_t aipl_color_convert_24bit_to_i422(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_I444 | AIPL_CONVERT_BGR888 & TO_I444) \
    && defined (AIPL_HELIUM_ACCELERATION)
aipl_error_t aipl_color_convert_24bit_to_i444(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_YV12 | AIPL_CONVERT_ARGB8888 & TO_I420)
aipl_error_t aipl_color_convert_argb8888_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_NV12 | AIPL_CONVERT_ARGB8888 & TO_NV21)
aipl_error_t aipl_color_convert_argb8888_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_YUY2 | AIPL_CONVERT_ARGB8888 & TO_UYVY)
aipl_error_t aipl_color_convert_argb8888_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_YV12 | AIPL_CONVERT_ARGB4444 & TO_I420)
aipl_error_t aipl_color_convert_argb4444_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_NV12 | AIPL_CONVERT_ARGB4444 & TO_NV21)
aipl_error_t aipl_color_convert_argb4444_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_YUY2 | AIPL_CONVERT_ARGB4444 & TO_UYVY)
aipl_error_t aipl_color_convert_argb4444_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_YV12 | AIPL_CONVERT_ARGB1555 & TO_I420)
aipl_error_t aipl_color_convert_argb1555_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_NV12 | AIPL_CONVERT_ARGB1555 & TO_NV21)
aipl_error_t aipl_color_convert_argb1555_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_YUY2 | AIPL_CONVERT_ARGB1555 & TO_UYVY)
aipl_error_t aipl_color_convert_argb1555_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_YV12 | AIPL_CONVERT_RGBA8888 & TO_I420)
aipl_error_t aipl_color_convert_rgba8888_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_NV12 | AIPL_CONVERT_RGBA8888 & TO_NV21)
aipl_error_t aipl_color_convert_rgba8888_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_YUY2 | AIPL_CONVERT_RGBA8888 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba8888_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_YV12 | AIPL_CONVERT_RGBA4444 & TO_I420)
aipl_error_t aipl_color_convert_rgba4444_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_NV12 | AIPL_CONVERT_RGBA4444 & TO_NV21)
aipl_error_t aipl_color_convert_rgba4444_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_YUY2 | AIPL_CONVERT_RGBA4444 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba4444_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_YV12 | AIPL_CONVERT_RGBA5551 & TO_I420)
aipl_error_t aipl_color_convert_rgba5551_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_NV12 | AIPL_CONVERT_RGBA5551 & TO_NV21)
aipl_error_t aipl_color_convert_rgba5551_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_YUY2 | AIPL_CONVERT_RGBA5551 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba5551_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_BGR888 & TO_YV12 | AIPL_CONVERT_BGR888 & TO_I420\
     | AIPL_CONVERT_RGB888 & TO_YV12 | AIPL_CONVERT_RGB888 & TO_I420)
aipl_error_t aipl_color_convert_24bit_to_yuv_planar(const void* input,
                                                    uint32_t pitch,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint8_t* y_ptr,
                                                    uint8_t* u_ptr,
                                                    uint8_t* v_ptr,
                                                    uint8_t r_offset,
                                                    uint8_t g_offset,
                                                    uint8_t b_offset);
#endif
#if (AIPL_CONVERT_BGR888 & TO_NV12 | AIPL_CONVERT_BGR888 & TO_NV21\
     | AIPL_CONVERT_RGB888 & TO_NV12 | AIPL_CONVERT_RGB888 & TO_NV21)
aipl_error_t aipl_color_convert_24bit_to_yuv_semi_planar(const void* input,
                                                         uint32_t pitch,
                                                         uint32_t width,
                                                         uint32_t height,
                                                         uint8_t* y_ptr,
                                                         uint8_t* u_ptr,
                                                         uint8_t* v_ptr,
                                                         uint8_t r_offset,
                                                         uint8_t g_offset,
                                                         uint8_t b_offset);
#endif
#if (AIPL_CONVERT_BGR888 & TO_YUY2 | AIPL_CONVERT_BGR888 & TO_UYVY\
     | AIPL_CONVERT_RGB888 & TO_YUY2 | AIPL_CONVERT_RGB888 & TO_UYVY)\
    && defined(AIPL_HELIUM_ACCELERATION)
aipl_error_t aipl_color_convert_24bit_to_yuv_packed(const void* input,
                                                    uint32_t pitch,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint8_t* y_ptr,
                                                    uint8_t* u_ptr,
                                                    uint8_t* v_ptr,
                                                    uint8_t r_offset,
                                                    uint8_t g_offset,
                                                    uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_YUY2 | AIPL_CONVERT_RGB888 & TO_UYVY)
aipl_error_t aipl_color_convert_rgb888_to_yuv_packed(const void* input,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height,
                                                     uint8_t* y_ptr,
                                                     uint8_t* u_ptr,
                                                     uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_BGR888 & TO_YUY2 | AIPL_CONVERT_BGR888 & TO_UYVY)
aipl_error_t aipl_color_convert_bgr888_to_yuv_packed(const void* input,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height,
                                                     uint8_t* y_ptr,
                                                     uint8_t* u_ptr,
                                                     uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_I422 & TO_BGR888 | AIPL_CONVERT_I422 & TO_RGB888)
aipl_error_t aipl_color_convert_i422_to_24bit(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset);
#endif
#if (AIPL_CONVERT_I444 & TO_BGR888 | AIPL_CONVERT_I444 & TO_RGB888)
aipl_error_t aipl_color_convert_i444_to_24bit(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB565 & TO_YV12 | AIPL_CONVERT_RGB565 & TO_I420)
aipl_error_t aipl_color_convert_rgb565_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGB565 & TO_NV12 | AIPL_CONVERT_RGB565 & TO_NV21)
aipl_error_t aipl_color_convert_rgb565_to_yuv_semi_planar(const void* input,
                                                          uint32_t pitch,
                                                          uint32_t width,
                                                          uint32_t height,
                                                          uint8_t* y_ptr,
                                                          uint8_t* u_ptr,
                                                          uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_RGB565 & TO_YUY2 | AIPL_CONVERT_RGB565 & TO_UYVY)
aipl_error_t aipl_color_convert_rgb565_to_yuv_packed(const void* input,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height,
                                                     uint8_t* y_ptr,
                                                     uint8_t* u_ptr,
                                                     uint8_t* v_ptr);
#endif
#if (AIPL_CONVERT_YV12 & TO_ALPHA8_I400 | AIPL_CONVERT_I420 & TO_ALPHA8_I400\
     | AIPL_CONVERT_NV12 & TO_ALPHA8_I400 | AIPL_CONVERT_NV21 & TO_ALPHA8_I400\
     | AIPL_CONVERT_I422 & TO_ALPHA8_I400 | AIPL_CONVERT_I444 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yuv_planar_to_alpha8(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_ALPHA8_I400 | AIPL_CONVERT_UYVY & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yuv_packed_to_alpha8(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & TO_ARGB8888 | AIPL_CONVERT_I420 & TO_ARGB8888)
aipl_error_t aipl_color_convert_yuv_planar_to_argb8888(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & TO_ARGB8888 | AIPL_CONVERT_NV21 & TO_ARGB8888)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_argb8888(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_ARGB8888 | AIPL_CONVERT_UYVY & TO_ARGB8888)
aipl_error_t aipl_color_convert_yuv_packed_to_argb8888(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & TO_ARGB4444 | AIPL_CONVERT_I420 & TO_ARGB4444)
aipl_error_t aipl_color_convert_yuv_planar_to_argb4444(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & TO_ARGB4444 | AIPL_CONVERT_NV21 & TO_ARGB4444)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_argb4444(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_ARGB4444 | AIPL_CONVERT_UYVY & TO_ARGB4444)
aipl_error_t aipl_color_convert_yuv_packed_to_argb4444(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & TO_ARGB1555 | AIPL_CONVERT_I420 & TO_ARGB1555)
aipl_error_t aipl_color_convert_yuv_planar_to_argb1555(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & TO_ARGB1555 | AIPL_CONVERT_NV21 & TO_ARGB1555)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_argb1555(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_ARGB1555 | AIPL_CONVERT_UYVY & TO_ARGB1555)
aipl_error_t aipl_color_convert_yuv_packed_to_argb1555(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGBA8888 | AIPL_CONVERT_I420 & TO_RGBA8888)
aipl_error_t aipl_color_convert_yuv_planar_to_rgba8888(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGBA8888 | AIPL_CONVERT_NV21 & TO_RGBA8888)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_rgba8888(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGBA8888 | AIPL_CONVERT_UYVY & TO_RGBA8888)
aipl_error_t aipl_color_convert_yuv_packed_to_rgba8888(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGBA4444 | AIPL_CONVERT_I420 & TO_RGBA4444)
aipl_error_t aipl_color_convert_yuv_planar_to_rgba4444(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGBA4444 | AIPL_CONVERT_NV21 & TO_RGBA4444)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_rgba4444(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGBA4444 | AIPL_CONVERT_UYVY & TO_RGBA4444)
aipl_error_t aipl_color_convert_yuv_packed_to_rgba4444(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGBA5551 | AIPL_CONVERT_I420 & TO_RGBA5551)
aipl_error_t aipl_color_convert_yuv_planar_to_rgba5551(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGBA5551 | AIPL_CONVERT_NV21 & TO_RGBA5551)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_rgba5551(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGBA5551 | AIPL_CONVERT_UYVY & TO_RGBA5551)
aipl_error_t aipl_color_convert_yuv_packed_to_rgba5551(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & TO_BGR888 | AIPL_CONVERT_I420 & TO_BGR888\
     | AIPL_CONVERT_YV12 & TO_RGB888 | AIPL_CONVERT_I420 & TO_RGB888)
aipl_error_t aipl_color_convert_yuv_planar_to_24bit(const uint8_t* y_ptr,
                                                    const uint8_t* u_ptr,
                                                    const uint8_t* v_ptr,
                                                    void* output,
                                                    uint32_t pitch,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint8_t r_offset,
                                                    uint8_t g_offset,
                                                    uint8_t b_offset);
#endif
#if (AIPL_CONVERT_NV12 & TO_BGR888 | AIPL_CONVERT_NV21 & TO_BGR888\
     | AIPL_CONVERT_NV12 & TO_RGB888 | AIPL_CONVERT_NV21 & TO_RGB888)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_24bit(const uint8_t* y_ptr,
                                                         const uint8_t* u_ptr,
                                                         const uint8_t* v_ptr,
                                                         void* output,
                                                         uint32_t pitch,
                                                         uint32_t width,
                                                         uint32_t height,
                                                         uint8_t r_offset,
                                                         uint8_t g_offset,
                                                         uint8_t b_offset);
#endif
#if (AIPL_CONVERT_YUY2 & TO_BGR888 | AIPL_CONVERT_UYVY & TO_BGR888\
     | AIPL_CONVERT_YUY2 & TO_RGB888 | AIPL_CONVERT_UYVY & TO_RGB888)
aipl_error_t aipl_color_convert_yuv_packed_to_24bit(const uint8_t* y_ptr,
                                                    const uint8_t* u_ptr,
                                                    const uint8_t* v_ptr,
                                                    void* output,
                                                    uint32_t pitch,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint8_t r_offset,
                                                    uint8_t g_offset,
                                                    uint8_t b_offset);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGB565 | AIPL_CONVERT_I420 & TO_RGB565)
aipl_error_t aipl_color_convert_yuv_planar_to_rgb565(const uint8_t* y_ptr,
                                                     const uint8_t* u_ptr,
                                                     const uint8_t* v_ptr,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGB565 | AIPL_CONVERT_NV21 & TO_RGB565)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_rgb565(const uint8_t* y_ptr,
                                                          const uint8_t* u_ptr,
                                                          const uint8_t* v_ptr,
                                                          void* output,
                                                          uint32_t pitch,
                                                          uint32_t width,
                                                          uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGB565 | AIPL_CONVERT_UYVY & TO_RGB565)
aipl_error_t aipl_color_convert_yuv_packed_to_rgb565(const uint8_t* y_ptr,
                                                     const uint8_t* u_ptr,
                                                     const uint8_t* v_ptr,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & TO_I420 | AIPL_CONVERT_I420 & TO_YV12)
aipl_error_t aipl_color_convert_yuv_planar_to_planar(const uint8_t* y_src,
                                                     const uint8_t* u_src,
                                                     const uint8_t* v_src,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & (TO_NV12 | TO_NV21) | AIPL_CONVERT_I420 & (TO_NV12 | TO_NV21))
aipl_error_t aipl_color_convert_yuv_planar_to_semi(const uint8_t* y_src,
                                                   const uint8_t* u_src,
                                                   const uint8_t* v_src,
                                                   uint8_t* y_dst,
                                                   uint8_t* u_dst,
                                                   uint8_t* v_dst,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height);
#endif
#if (AIPL_CONVERT_YV12 & (TO_YUY2 | TO_UYVY) | AIPL_CONVERT_I420 & (TO_YUY2 | TO_UYVY))
aipl_error_t aipl_color_convert_yuv_planar_to_packed(const uint8_t* y_src,
                                                     const uint8_t* u_src,
                                                     const uint8_t* v_src,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & (TO_YV12 | TO_I420) | AIPL_CONVERT_NV21 & (TO_YV12 | TO_I420))
aipl_error_t aipl_color_convert_yuv_semi_to_planar(const uint8_t* y_src,
                                                   const uint8_t* u_src,
                                                   const uint8_t* v_src,
                                                   uint8_t* y_dst,
                                                   uint8_t* u_dst,
                                                   uint8_t* v_dst,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & TO_NV21 | AIPL_CONVERT_NV21 & TO_NV12)
aipl_error_t aipl_color_convert_yuv_semi_to_semi_planar(const uint8_t* y_src,
                                                        const uint8_t* u_src,
                                                        const uint8_t* v_src,
                                                        uint8_t* y_dst,
                                                        uint8_t* u_dst,
                                                        uint8_t* v_dst,
                                                        uint32_t pitch,
                                                        uint32_t width,
                                                        uint32_t height);
#endif
#if (AIPL_CONVERT_NV12 & (TO_YUY2 | TO_UYVY) | AIPL_CONVERT_NV21 & (TO_YUY2 | TO_UYVY))
aipl_error_t aipl_color_convert_yuv_semi_to_packed(const uint8_t* y_src,
                                                   const uint8_t* u_src,
                                                   const uint8_t* v_src,
                                                   uint8_t* y_dst,
                                                   uint8_t* u_dst,
                                                   uint8_t* v_dst,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & (TO_YV12 | TO_I420) | AIPL_CONVERT_UYVY & (TO_YV12 | TO_I420))
aipl_error_t aipl_color_convert_yuv_packed_to_planar(const uint8_t* y_src,
                                                     const uint8_t* u_src,
                                                     const uint8_t* v_src,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & (TO_NV12 | TO_NV21) | AIPL_CONVERT_UYVY & (TO_NV12 | TO_NV21))
aipl_error_t aipl_color_convert_yuv_packed_to_semi(const uint8_t* y_src,
                                                   const uint8_t* u_src,
                                                   const uint8_t* v_src,
                                                   uint8_t* y_dst,
                                                   uint8_t* u_dst,
                                                   uint8_t* v_dst,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_UYVY | AIPL_CONVERT_UYVY & TO_YUY2)
aipl_error_t aipl_color_convert_yuv_packed_to_packed(const uint8_t* y_src,
                                                     const uint8_t* u_src,
                                                     const uint8_t* v_src,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & (TO_YV12 | TO_I420))
aipl_error_t aipl_color_convert_alpha8_to_yuv_planar(const uint8_t* input,
                                                     uint8_t* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & (TO_NV12 | TO_NV21))
aipl_error_t aipl_color_convert_alpha8_to_yuv_semi_planar(const uint8_t* input,
                                                          uint8_t* output,
                                                          uint32_t pitch,
                                                          uint32_t width,
                                                          uint32_t height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & (TO_YUY2 | TO_UYVY))
aipl_error_t aipl_color_convert_alpha8_to_yuv_packed(const uint8_t* input,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
aipl_error_t aipl_color_convert(const void* input, void* output,
                                uint32_t pitch,
                                uint32_t width, uint32_t height,
                                aipl_color_format_t input_format,
                                aipl_color_format_t output_format)
{
#ifdef AIPL_DAVE2D_ACCELERATION
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (aipl_dave2d_color_convert_suitable(input_format, output_format))
    {
        d2_s32 ret =
            aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                           aipl_dave2d_format_to_mode(input_format),
                                           aipl_dave2d_format_to_mode(output_format));

        return aipl_dave2d_error_convert(ret);
    }
#endif

    switch (input_format)
    {
        /* Alpha color formats */
#if AIPL_CONVERT_ALPHA8_I400
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_alpha8(input, output, pitch,
                                             width, height, output_format);
#endif

        /* RGB color formats */
#if AIPL_CONVERT_ARGB8888
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_argb8888(input, output, pitch,
                                               width, height, output_format);
#endif
#if AIPL_CONVERT_RGBA8888
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_rgba8888(input, output, pitch,
                                               width, height, output_format);
#endif
#if AIPL_CONVERT_ARGB4444
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_argb4444(input, output, pitch,
                                               width, height, output_format);
#endif
#if AIPL_CONVERT_ARGB1555
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_argb1555(input, output, pitch,
                                               width, height, output_format);
#endif
#if AIPL_CONVERT_RGBA4444
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_rgba4444(input, output, pitch,
                                               width, height, output_format);
#endif
#if AIPL_CONVERT_RGBA5551
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_rgba5551(input, output, pitch,
                                               width, height, output_format);
#endif
#if AIPL_CONVERT_RGB565
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_rgb565(input, output, pitch,
                                             width, height, output_format);
#endif
#if AIPL_CONVERT_BGR888
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_bgr888(input, output, pitch,
                                             width, height, output_format);
#endif
#if AIPL_CONVERT_RGB888
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_rgb888(input, output, pitch,
                                             width, height, output_format);
#endif

        /* YUV color formats */
#if AIPL_CONVERT_YV12
        case AIPL_COLOR_YV12:
            return aipl_color_convert_yv12(input, output, pitch,
                                           width, height, output_format);
#endif
#if AIPL_CONVERT_I420
        case AIPL_COLOR_I420:
            return aipl_color_convert_i420(input, output, pitch,
                                           width, height, output_format);
#endif
#if AIPL_CONVERT_NV12
        case AIPL_COLOR_NV12:
            return aipl_color_convert_nv12(input, output, pitch,
                                           width, height, output_format);
#endif
#if AIPL_CONVERT_NV21
        case AIPL_COLOR_NV21:
            return aipl_color_convert_nv21(input, output, pitch,
                                           width, height, output_format);
#endif
#if AIPL_CONVERT_I422
        case AIPL_COLOR_I422:
            return aipl_color_convert_i422(input, output, pitch,
                                           width, height, output_format);
#endif
#if AIPL_CONVERT_YUY2
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_yuy2(input, output, pitch,
                                           width, height, output_format);
#endif
#if AIPL_CONVERT_UYVY
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_uyvy(input, output, pitch,
                                           width, height, output_format);
#endif
#if AIPL_CONVERT_I444
        case AIPL_COLOR_I444:
            return aipl_color_convert_i444(input, output, pitch,
                                           width, height, output_format);
#endif
#if AIPL_CONVERT_ALPHA8_I400
        case AIPL_COLOR_I400:
            return aipl_color_convert_i400(input, output, pitch,
                                           width, height, output_format);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }

}

aipl_error_t aipl_color_convert_img(const aipl_image_t* input,
                                    aipl_image_t* output)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    if (input->width != output->width || input->height != output->height)
    {
        return AIPL_ERR_SIZE_MISMATCH;
    }

    return aipl_color_convert(input->data, output->data, input->pitch,
                              input->width, input->height,
                              input->format, output->format);
}

#if AIPL_CONVERT_ALPHA8_I400
aipl_error_t aipl_color_convert_alpha8(const void* input, void* output,
                                       uint32_t pitch,
                                       uint32_t width, uint32_t height,
                                       aipl_color_format_t format)
{
    switch (format)
    {
        case AIPL_COLOR_ALPHA8:
            return AIPL_ERR_FORMAT_MISMATCH;

        case AIPL_COLOR_I400:
            return aipl_color_convert_alpha8_to_i400(input, output, pitch,
                                                     width, height);

        default:
            return aipl_color_convert_i400(input, output, pitch, width, height, format);
    }
}

aipl_error_t aipl_color_convert_alpha8_to_i400(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_alpha8(input, output, pitch, width, height);
}

#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB8888)
aipl_error_t aipl_color_convert_alpha8_to_argb8888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_i400_to_argb8888(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB4444)
aipl_error_t aipl_color_convert_alpha8_to_argb4444(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_i400_to_argb4444(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB1555)
aipl_error_t aipl_color_convert_alpha8_to_argb1555(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_i400_to_argb1555(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA8888)
aipl_error_t aipl_color_convert_alpha8_to_rgba8888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_i400_to_rgba8888(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA4444)
aipl_error_t aipl_color_convert_alpha8_to_rgba4444(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_i400_to_rgba4444(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA5551)
aipl_error_t aipl_color_convert_alpha8_to_rgba5551(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_i400_to_rgba5551(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_BGR888)
aipl_error_t aipl_color_convert_alpha8_to_bgr888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_i400_to_bgr888(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGB888)
aipl_error_t aipl_color_convert_alpha8_to_rgb888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_i400_to_rgb888(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGB565)
aipl_error_t aipl_color_convert_alpha8_to_rgb565(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_i400_to_rgb565(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_YV12)
aipl_error_t aipl_color_convert_alpha8_to_yv12(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_yv12(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_I420)
aipl_error_t aipl_color_convert_alpha8_to_i420(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_i420(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_I422)
aipl_error_t aipl_color_convert_alpha8_to_i422(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_i422(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_I444)
aipl_error_t aipl_color_convert_alpha8_to_i444(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_i444(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_NV12)
aipl_error_t aipl_color_convert_alpha8_to_nv12(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_nv12(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_NV21)
aipl_error_t aipl_color_convert_alpha8_to_nv21(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_nv21(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_YUY2)
aipl_error_t aipl_color_convert_alpha8_to_yuy2(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_yuy2(input, output, pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_UYVY)
aipl_error_t aipl_color_convert_alpha8_to_uyvy(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i400_to_uyvy(input, output, pitch, width, height);
}
#endif
#endif

#if AIPL_CONVERT_ARGB8888
aipl_error_t aipl_color_convert_argb8888(const void* input, void* output,
                                         uint32_t pitch,
                                         uint32_t width, uint32_t height,
                                         aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_ARGB8888 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_argb8888_to_alpha8(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* RGB color formats */
        case AIPL_COLOR_ARGB8888:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_ARGB8888 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_argb8888_to_rgba8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_argb8888_to_argb4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_argb8888_to_argb1555(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_argb8888_to_rgba4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_argb8888_to_rgba5551(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_argb8888_to_rgb565(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_argb8888_to_bgr888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_argb8888_to_rgb888(input, output,
                                                         pitch,
                                                         width, height);
#endif        /* YUV color formats */
#if (AIPL_CONVERT_ARGB8888 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_argb8888_to_yv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_argb8888_to_i420(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_argb8888_to_nv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_argb8888_to_nv21(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_argb8888_to_i422(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_argb8888_to_yuy2(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_argb8888_to_uyvy(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_argb8888_to_i444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB8888 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_argb8888_to_i400(input, output,
                                                       pitch,
                                                       width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_ARGB8888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_argb8888_to_alpha8(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb8888, d2_mode_alpha8);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_xrgb8888(&pix, src, tail_p);

            uint16x8_t alpha;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_y(&alpha, pix);

            aipl_mve_str_8px_i400(dst, alpha, tail_p);

            src += 32;
            dst += 8;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_yuv_y(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_ARGB4444)
aipl_error_t aipl_color_convert_argb8888_to_argb4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb8888, d2_mode_argb4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_argb4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_ARGB1555)
aipl_error_t aipl_color_convert_argb8888_to_argb1555(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb8888, d2_mode_argb1555);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_argb1555(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_RGBA8888)
aipl_error_t aipl_color_convert_argb8888_to_rgba8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb8888, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint32_t* src_ptr = input;
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint32_t* src = src_ptr + i * pitch;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t px;
            aipl_mve_ldr_4px_argb8888(&px, src, tail_p);

            aipl_mve_cnvt_4px_argb8888_to_rgba8888(&px, px);

            aipl_mve_str_4px_rgba8888(dst, px, tail_p);

            src += 4;
            dst += 4;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_rgba8888(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_RGBA4444)
aipl_error_t aipl_color_convert_argb8888_to_rgba4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb8888, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_rgba4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_RGBA5551)
aipl_error_t aipl_color_convert_argb8888_to_rgba5551(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb8888, d2_mode_rgba5551);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_rgba5551(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_BGR888)
aipl_error_t aipl_color_convert_argb8888_to_bgr888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_argb8888_to_24bit(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_RGB888)
aipl_error_t aipl_color_convert_argb8888_to_rgb888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_argb8888_to_24bit(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_RGB565)
aipl_error_t aipl_color_convert_argb8888_to_rgb565(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb8888, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_rgb565(dst, src);

            ++dst;
            ++src;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_YV12)
aipl_error_t aipl_color_convert_argb8888_to_yv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_argb8888_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_I420)
aipl_error_t aipl_color_convert_argb8888_to_i420(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_argb8888_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_I422)
aipl_error_t aipl_color_convert_argb8888_to_i422(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_xrgb8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 32;
            y_dst += 8;
        }

        src = src_ptr + i * pitch * 4;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_xrgb8888(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_v(&v, pix);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 64;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width / 2;
        uint8_t* u_dst = u_ptr + i * width / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_cnvt_px_argb8888_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;

            aipl_cnvt_px_argb8888_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
       }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_I444)
aipl_error_t aipl_color_convert_argb8888_to_i444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_xrgb8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_y(&y, pix);

            uint16x8_t u;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_v(&v, pix);

            vstrbq_p(y_dst, y, tail_p);
            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 32;
            y_dst += 8;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_argb8888_to_i400(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_argb8888_to_alpha8(input, output, pitch,
                                                 width, height);
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_NV21)
aipl_error_t aipl_color_convert_argb8888_to_nv21(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_argb8888_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_NV12)
aipl_error_t aipl_color_convert_argb8888_to_nv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_argb8888_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_YUY2)
aipl_error_t aipl_color_convert_argb8888_to_yuy2(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_argb8888_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_UYVY)
aipl_error_t aipl_color_convert_argb8888_to_uyvy(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_argb8888_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_ARGB4444
aipl_error_t aipl_color_convert_argb4444(const void* input, void* output,
                                         uint32_t pitch,
                                         uint32_t width, uint32_t height,
                                         aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_ARGB4444 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_argb4444_to_alpha8(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_ARGB4444 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_argb4444_to_argb8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_argb4444_to_rgba8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
        case AIPL_COLOR_ARGB4444:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_ARGB4444 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_argb4444_to_argb1555(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_argb4444_to_rgba4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_argb4444_to_rgba5551(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_argb4444_to_rgb565(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_argb4444_to_bgr888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_argb4444_to_rgb888(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_ARGB4444 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_argb4444_to_yv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_argb4444_to_i420(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_argb4444_to_nv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_argb4444_to_nv21(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_argb4444_to_i422(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_argb4444_to_yuy2(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_argb4444_to_uyvy(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_argb4444_to_i444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_argb4444_to_i400(input, output,
                                                       pitch,
                                                       width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_ARGB4444 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_argb4444_to_alpha8(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb4444, d2_mode_alpha8);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb4444_to_yuv_y(&y, px);

            vstrbq_p(dst, y, tail_p);

            src += 8;
            dst += 8;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_yuv_y(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_ARGB8888)
aipl_error_t aipl_color_convert_argb4444_to_argb8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb4444, d2_mode_argb8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_argb8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_ARGB1555)
aipl_error_t aipl_color_convert_argb4444_to_argb1555(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb4444, d2_mode_argb1555);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_argb1555(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_RGBA8888)
aipl_error_t aipl_color_convert_argb4444_to_rgba8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb4444, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_rgba8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_RGBA4444)
aipl_error_t aipl_color_convert_argb4444_to_rgba4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb4444, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px_i;
            aipl_mve_ldr_8px_argb4444(&px_i, src, tail_p);

            uint16x8_t px_o;
            aipl_mve_cnvt_8px_argb4444_to_rgba4444(&px_o, px_i);

            aipl_mve_str_8px_rgba4444(dst, px_o, tail_p);

            src += 8;
            dst += 8;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_rgba4444(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_RGBA5551)
aipl_error_t aipl_color_convert_argb4444_to_rgba5551(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb4444, d2_mode_rgba5551);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_rgba5551(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_BGR888)
aipl_error_t aipl_color_convert_argb4444_to_bgr888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_argb4444_to_24bit(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_RGB888)
aipl_error_t aipl_color_convert_argb4444_to_rgb888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_argb4444_to_24bit(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_RGB565)
aipl_error_t aipl_color_convert_argb4444_to_rgb565(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb4444, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_rgb565(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_YV12)
aipl_error_t aipl_color_convert_argb4444_to_yv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_argb4444_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_I420)
aipl_error_t aipl_color_convert_argb4444_to_i420(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_argb4444_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_I422)
aipl_error_t aipl_color_convert_argb4444_to_i422(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);


            uint16x8_t px;
            aipl_mve_ldr_8px_argb4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb4444_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_argb4444(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb4444_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_argb4444_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_I444)
aipl_error_t aipl_color_convert_argb4444_to_i444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb4444(&px, src, tail_p);

            uint16x8_t y;
            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb4444_to_yuv(&y, &u, &v, px);

            vstrbq_p(y_dst, y, tail_p);
            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 8;
            y_dst += 8;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_argb4444_to_i400(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_argb4444_to_alpha8(input, output, pitch,
                                                 width, height);
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_NV21)
aipl_error_t aipl_color_convert_argb4444_to_nv21(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_argb4444_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_NV12)
aipl_error_t aipl_color_convert_argb4444_to_nv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_argb4444_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_YUY2)
aipl_error_t aipl_color_convert_argb4444_to_yuy2(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_argb4444_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_UYVY)
aipl_error_t aipl_color_convert_argb4444_to_uyvy(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_argb4444_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_ARGB1555
aipl_error_t aipl_color_convert_argb1555(const void* input, void* output,
                                         uint32_t pitch,
                                         uint32_t width, uint32_t height,
                                         aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_ARGB1555 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_argb1555_to_alpha8(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_ARGB1555 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_argb1555_to_argb8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_argb1555_to_rgba8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_argb1555_to_argb4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
        case AIPL_COLOR_ARGB1555:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_ARGB1555 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_argb1555_to_rgba4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_argb1555_to_rgba5551(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_argb1555_to_rgb565(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_argb1555_to_bgr888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_argb1555_to_rgb888(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_ARGB1555 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_argb1555_to_yv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_argb1555_to_i420(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_argb1555_to_nv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_argb1555_to_nv21(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_argb1555_to_i422(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_argb1555_to_yuy2(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_argb1555_to_uyvy(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_argb1555_to_i444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_argb1555_to_i400(input, output,
                                                       pitch,
                                                       width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_ARGB1555 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_argb1555_to_alpha8(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb1555, d2_mode_alpha8);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb1555(&px, src, tail_p);

            uint16x8_t r = vandq(vshrq(px, 10), vdupq_n_u16(0x001f));
            uint16x8_t g = vandq(vshrq(px, 5), vdupq_n_u16(0x001f));
            uint16x8_t b = vandq(px, vdupq_n_u16(0x001f));

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb1555_to_yuv_y(&y, px);

            vstrbq_p(dst, y, tail_p);

            src += 8;
            dst += 8;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_yuv_y(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_ARGB8888)
aipl_error_t aipl_color_convert_argb1555_to_argb8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb1555, d2_mode_argb8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_argb8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_ARGB4444)
aipl_error_t aipl_color_convert_argb1555_to_argb4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb1555, d2_mode_argb4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_argb4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_RGBA8888)
aipl_error_t aipl_color_convert_argb1555_to_rgba8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb1555, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_rgba8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_RGBA4444)
aipl_error_t aipl_color_convert_argb1555_to_rgba4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb1555, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_rgba4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_RGBA5551)
aipl_error_t aipl_color_convert_argb1555_to_rgba5551(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb1555, d2_mode_rgba5551);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_rgba5551(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_BGR888)
aipl_error_t aipl_color_convert_argb1555_to_bgr888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_argb1555_to_24bit(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_RGB888)
aipl_error_t aipl_color_convert_argb1555_to_rgb888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_argb1555_to_24bit(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_RGB565)
aipl_error_t aipl_color_convert_argb1555_to_rgb565(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb1555, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_rgb565(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_YV12)
aipl_error_t aipl_color_convert_argb1555_to_yv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_argb1555_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_I420)
aipl_error_t aipl_color_convert_argb1555_to_i420(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_argb1555_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_I422)
aipl_error_t aipl_color_convert_argb1555_to_i422(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb1555(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb1555_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);


            uint16x8_t px;
            aipl_mve_ldr_8px_offset_argb1555(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb1555_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_argb1555_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_I444)
aipl_error_t aipl_color_convert_argb1555_to_i444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px = vldrhq_z_u16(src, tail_p);

            uint16x8_t y;
            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb1555_to_yuv(&y, &u, &v, px);

            vstrbq_p(y_dst, y, tail_p);
            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 8;
            y_dst += 8;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_argb1555_to_i400(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_argb1555_to_alpha8(input, output, pitch,
                                                 width, height);
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_NV21)
aipl_error_t aipl_color_convert_argb1555_to_nv21(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_argb1555_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_NV12)
aipl_error_t aipl_color_convert_argb1555_to_nv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_argb1555_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_YUY2)
aipl_error_t aipl_color_convert_argb1555_to_yuy2(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_argb1555_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_UYVY)
aipl_error_t aipl_color_convert_argb1555_to_uyvy(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_argb1555_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_RGBA8888
aipl_error_t aipl_color_convert_rgba8888(const void* input, void* output,
                                         uint32_t pitch,
                                         uint32_t width, uint32_t height,
                                         aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_RGBA8888 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_rgba8888_to_alpha8(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_RGBA8888 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_rgba8888_to_argb8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
        case AIPL_COLOR_RGBA8888:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_RGBA8888 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_rgba8888_to_argb4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_rgba8888_to_argb1555(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_rgba8888_to_rgba4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_rgba8888_to_rgba5551(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_rgba8888_to_rgb565(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_rgba8888_to_bgr888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_rgba8888_to_rgb888(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_RGBA8888 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_rgba8888_to_yv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_rgba8888_to_i420(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_rgba8888_to_nv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_rgba8888_to_nv21(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_rgba8888_to_i422(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_rgba8888_to_yuy2(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_rgba8888_to_uyvy(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_rgba8888_to_i444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA8888 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_rgba8888_to_i400(input, output,
                                                       pitch,
                                                       width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_RGBA8888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgba8888_to_alpha8(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgba8888, d2_mode_alpha8);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgbx8888(&pix, src, tail_p);

            uint16x8_t alpha;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_y(&alpha, pix);

            vstrbq_p(dst, alpha, tail_p);

            src += 32;
            dst += 8;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_yuv_y(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_ARGB8888)
aipl_error_t aipl_color_convert_rgba8888_to_argb8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba8888, d2_mode_argb8888);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint32_t* src_ptr = input;
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint32_t* src = src_ptr + i * pitch;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t px;
            aipl_mve_ldr_4px_rgba8888(&px, src, tail_p);

            aipl_mve_cnvt_4px_rgba8888_to_argb8888(&px, px);

            aipl_mve_str_4px_rgba8888(dst, px, tail_p);

            src += 4;
            dst += 4;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_argb8888(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_ARGB4444)
aipl_error_t aipl_color_convert_rgba8888_to_argb4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba8888, d2_mode_argb4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_argb4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_ARGB1555)
aipl_error_t aipl_color_convert_rgba8888_to_argb1555(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgba8888, d2_mode_rgba8888);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_argb1555(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_RGBA4444)
aipl_error_t aipl_color_convert_rgba8888_to_rgba4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba8888, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_rgba4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_RGBA5551)
aipl_error_t aipl_color_convert_rgba8888_to_rgba5551(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgba8888, d2_mode_rgba5551);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_rgba5551(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_BGR888)
aipl_error_t aipl_color_convert_rgba8888_to_bgr888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_rgba8888_to_24bit(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba8888_to_rgb888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_rgba8888_to_24bit(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_RGB565)
aipl_error_t aipl_color_convert_rgba8888_to_rgb565(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba8888, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_rgb565(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_YV12)
aipl_error_t aipl_color_convert_rgba8888_to_yv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_rgba8888_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_I420)
aipl_error_t aipl_color_convert_rgba8888_to_i420(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_rgba8888_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_I422)
aipl_error_t aipl_color_convert_rgba8888_to_i422(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgbx8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 32;
            y_dst += 8;
        }

        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_rgbx8888(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_v(&v, pix);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 64;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width / 2;
        uint8_t* u_dst = u_ptr + i * width / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_cnvt_px_rgba8888_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;

            aipl_cnvt_px_rgba8888_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
       }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_I444)
aipl_error_t aipl_color_convert_rgba8888_to_i444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgbx8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_y(&y, pix);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_v(&v, pix);

            vstrbq_p(y_dst, y, tail_p);
            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 32;
            y_dst += 8;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgba8888_to_i400(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_rgba8888_to_alpha8(input, output, pitch,
                                                 width, height);
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_NV21)
aipl_error_t aipl_color_convert_rgba8888_to_nv21(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_rgba8888_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_NV12)
aipl_error_t aipl_color_convert_rgba8888_to_nv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_rgba8888_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_YUY2)
aipl_error_t aipl_color_convert_rgba8888_to_yuy2(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgba8888_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba8888_to_uyvy(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgba8888_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_RGBA4444
aipl_error_t aipl_color_convert_rgba4444(const void* input, void* output,
                                         uint32_t pitch,
                                         uint32_t width, uint32_t height,
                                         aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_RGBA4444 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_rgba4444_to_alpha8(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_RGBA4444 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_rgba4444_to_argb8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_rgba4444_to_rgba8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_rgba4444_to_argb4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_rgba4444_to_argb1555(input, output,
                                                           pitch,
                                                           width, height);
#endif
        case AIPL_COLOR_RGBA4444:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_RGBA4444 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_rgba4444_to_rgba5551(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_rgba4444_to_rgb565(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_rgba4444_to_bgr888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_rgba4444_to_rgb888(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_RGBA4444 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_rgba4444_to_yv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_rgba4444_to_i420(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_rgba4444_to_nv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_rgba4444_to_nv21(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_rgba4444_to_i422(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_rgba4444_to_yuy2(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_rgba4444_to_uyvy(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_rgba4444_to_i444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA4444 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_rgba4444_to_i400(input, output,
                                                       pitch,
                                                       width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_RGBA4444 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgba4444_to_alpha8(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgba4444, d2_mode_alpha8);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_y(&y, px);

            vstrbq_p(dst, y, tail_p);

            src += 8;
            dst += 8;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_yuv_y(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_ARGB8888)
aipl_error_t aipl_color_convert_rgba4444_to_argb8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba4444, d2_mode_argb8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_argb8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_ARGB1555)
aipl_error_t aipl_color_convert_rgba4444_to_argb1555(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgba4444, d2_mode_argb1555);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_argb1555(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_ARGB4444)
aipl_error_t aipl_color_convert_rgba4444_to_argb4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba4444, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px_i;
            aipl_mve_ldr_8px_rgba4444(&px_i, src, tail_p);

            uint16x8_t px_o;
            aipl_mve_cnvt_8px_rgba4444_to_argb4444(&px_o, px_i);

            aipl_mve_str_8px_rgba4444(dst, px_o, tail_p);

            src += 8;
            dst += 8;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_argb4444(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_RGBA8888)
aipl_error_t aipl_color_convert_rgba4444_to_rgba8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba4444, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_rgba8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_RGBA5551)
aipl_error_t aipl_color_convert_rgba4444_to_rgba5551(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgba4444, d2_mode_rgba5551);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_rgba5551(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_BGR888)
aipl_error_t aipl_color_convert_rgba4444_to_bgr888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_rgba4444_to_24bit(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba4444_to_rgb888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_rgba4444_to_24bit(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_RGB565)
aipl_error_t aipl_color_convert_rgba4444_to_rgb565(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba4444, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_rgb565(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_YV12)
aipl_error_t aipl_color_convert_rgba4444_to_yv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_rgba4444_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_I420)
aipl_error_t aipl_color_convert_rgba4444_to_i420(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_rgba4444_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_I422)
aipl_error_t aipl_color_convert_rgba4444_to_i422(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgba4444(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgba4444_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_I444)
aipl_error_t aipl_color_convert_rgba4444_to_i444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba4444(&px, src, tail_p);

            uint16x8_t y;
            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba4444_to_yuv(&y, &u, &v, px);

            vstrbq_p(y_dst, y, tail_p);
            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 8;
            y_dst += 8;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgba4444_to_i400(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_rgba4444_to_alpha8(input, output, pitch,
                                                 width, height);
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_NV21)
aipl_error_t aipl_color_convert_rgba4444_to_nv21(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_rgba4444_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_NV12)
aipl_error_t aipl_color_convert_rgba4444_to_nv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_rgba4444_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_YUY2)
aipl_error_t aipl_color_convert_rgba4444_to_yuy2(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgba4444_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba4444_to_uyvy(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgba4444_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_RGBA5551
aipl_error_t aipl_color_convert_rgba5551(const void* input, void* output,
                                         uint32_t pitch,
                                         uint32_t width, uint32_t height,
                                         aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_RGBA5551 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_rgba5551_to_alpha8(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_RGBA5551 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_rgba5551_to_argb8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_rgba5551_to_rgba8888(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_rgba5551_to_argb4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_rgba5551_to_argb1555(input, output,
                                                           pitch,
                                                           width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_rgba5551_to_rgba4444(input, output,
                                                           pitch,
                                                           width, height);
#endif
        case AIPL_COLOR_RGBA5551:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_RGBA5551 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_rgba5551_to_rgb565(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_rgba5551_to_bgr888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_rgba5551_to_rgb888(input, output,
                                                         pitch,
                                                         width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_RGBA5551 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_rgba5551_to_yv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_rgba5551_to_i420(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_rgba5551_to_nv12(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_rgba5551_to_nv21(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_rgba5551_to_i422(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_rgba5551_to_yuy2(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_rgba5551_to_uyvy(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_rgba5551_to_i444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGBA5551 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_rgba5551_to_i400(input, output,
                                                       pitch,
                                                       width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_RGBA5551 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgba5551_to_alpha8(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgba5551, d2_mode_alpha8);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba5551(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_y(&y, px);

            vstrbq_p(dst, y, tail_p);

            src += 8;
            dst += 8;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_yuv_y(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_ARGB8888)
aipl_error_t aipl_color_convert_rgba5551_to_argb8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba5551, d2_mode_argb8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_argb8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_ARGB1555)
aipl_error_t aipl_color_convert_rgba5551_to_argb1555(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgba5551, d2_mode_argb1555);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_argb1555(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_ARGB4444)
aipl_error_t aipl_color_convert_rgba5551_to_argb4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba5551, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_argb4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_RGBA8888)
aipl_error_t aipl_color_convert_rgba5551_to_rgba8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba5551, d2_mode_rgba5551);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_rgba8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_RGBA4444)
aipl_error_t aipl_color_convert_rgba5551_to_rgba4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba5551, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_rgba4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_BGR888)
aipl_error_t aipl_color_convert_rgba5551_to_bgr888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_rgba5551_to_24bit(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba5551_to_rgb888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_rgba5551_to_24bit(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_RGB565)
aipl_error_t aipl_color_convert_rgba5551_to_rgb565(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba5551, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_rgb565(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_YV12)
aipl_error_t aipl_color_convert_rgba5551_to_yv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_rgba5551_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_I420)
aipl_error_t aipl_color_convert_rgba5551_to_i420(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_rgba5551_to_yuv_planar(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_I422)
aipl_error_t aipl_color_convert_rgba5551_to_i422(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba5551(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgba5551(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgba5551_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_I444)
aipl_error_t aipl_color_convert_rgba5551_to_i444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba5551(&px, src, tail_p);

            uint16x8_t y;
            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba5551_to_yuv(&y, &u, &v, px);

            vstrbq_p(y_dst, y, tail_p);
            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 8;
            y_dst += 8;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgba5551_to_i400(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_rgba5551_to_alpha8(input, output, pitch,
                                                 width, height);
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_NV21)
aipl_error_t aipl_color_convert_rgba5551_to_nv21(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_rgba5551_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_NV12)
aipl_error_t aipl_color_convert_rgba5551_to_nv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_rgba5551_to_yuv_semi_planar(input, pitch,
                                                          width, height,
                                                          y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_YUY2)
aipl_error_t aipl_color_convert_rgba5551_to_yuy2(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgba5551_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba5551_to_uyvy(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgba5551_to_yuv_packed(input, pitch,
                                                     width, height,
                                                     y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_BGR888
aipl_error_t aipl_color_convert_bgr888(const void* input, void* output,
                                       uint32_t pitch,
                                       uint32_t width, uint32_t height,
                                       aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_BGR888 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_bgr888_to_alpha8(input, output,
                                                       pitch,
                                                       width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_BGR888 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_bgr888_to_argb8888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_bgr888_to_rgba8888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_bgr888_to_argb4444(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_bgr888_to_argb1555(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_bgr888_to_rgba4444(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_bgr888_to_rgba5551(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_bgr888_to_rgb565(input, output,
                                                       pitch,
                                                       width, height);
#endif
        case AIPL_COLOR_BGR888:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_BGR888 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_bgr888_to_rgb888(input, output,
                                                       pitch,
                                                       width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_BGR888 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_bgr888_to_yv12(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_bgr888_to_i420(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_bgr888_to_nv12(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_bgr888_to_nv21(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_bgr888_to_i422(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_bgr888_to_yuy2(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_bgr888_to_uyvy(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_bgr888_to_i444(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_BGR888 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_bgr888_to_i400(input, output,
                                                     pitch,
                                                     width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_BGR888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_bgr888_to_alpha8(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_24bit_to_alpha8(input, output, pitch,
                                              width, height,
                                              2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_ARGB8888)
aipl_error_t aipl_color_convert_bgr888_to_argb8888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_argb8888(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_ARGB1555)
aipl_error_t aipl_color_convert_bgr888_to_argb1555(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_argb1555(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_ARGB4444)
aipl_error_t aipl_color_convert_bgr888_to_argb4444(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_argb4444(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_RGBA8888)
aipl_error_t aipl_color_convert_bgr888_to_rgba8888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_rgba8888(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_RGBA4444)
aipl_error_t aipl_color_convert_bgr888_to_rgba4444(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_rgba4444(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_RGBA5551)
aipl_error_t aipl_color_convert_bgr888_to_rgba5551(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_rgba5551(input, output, pitch,
                                                width, height,
                                                2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_RGB565)
aipl_error_t aipl_color_convert_bgr888_to_rgb565(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_24bit_to_rgb565(input, output, pitch,
                                              width, height,
                                              2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_RGB888)
aipl_error_t aipl_color_convert_bgr888_to_rgb888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_24bit_to_24bit(input, output, pitch,
                                             width, height,
                                             2, 1, 0,
                                             0, 1, 2);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_YV12)
aipl_error_t aipl_color_convert_bgr888_to_yv12(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_24bit_to_yuv_planar(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_I420)
aipl_error_t aipl_color_convert_bgr888_to_i420(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_24bit_to_yuv_planar(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_I422)
aipl_error_t aipl_color_convert_bgr888_to_i422(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    return aipl_color_convert_24bit_to_i422(input, output, pitch,
                                            width, height,
                                            2, 1, 0);
#else
    const uint8_t* src_ptr = input;
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width / 2;
        uint8_t* u_dst = u_ptr + i * width / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_cnvt_px_24bit_to_yuv(y_dst, u_dst, v_dst, src, 2, 1, 0);

            src += 3;
            ++y_dst;
            ++u_dst;
            ++v_dst;

            aipl_cnvt_px_24bit_to_yuv_y(y_dst, src, 2, 1, 0);

            src += 3;
            ++y_dst;
       }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_I444)
aipl_error_t aipl_color_convert_bgr888_to_i444(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    return aipl_color_convert_24bit_to_i444(input, output, pitch,
                                            width, height,
                                            2, 1, 0);
#else
    const uint8_t* src_ptr = input;
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_yuv(y_dst, u_dst, v_dst, src, 2, 1, 0);

            src += 3;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_bgr888_to_i400(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_24bit_to_alpha8(input, output, pitch,
                                              width, height,
                                              2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_NV21)
aipl_error_t aipl_color_convert_bgr888_to_nv21(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_24bit_to_yuv_semi_planar(input, pitch,
                                                       width, height,
                                                       y_ptr, u_ptr, v_ptr,
                                                       2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_NV12)
aipl_error_t aipl_color_convert_bgr888_to_nv12(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_24bit_to_yuv_semi_planar(input, pitch,
                                                       width, height,
                                                       y_ptr, u_ptr, v_ptr,
                                                       2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_YUY2)
aipl_error_t aipl_color_convert_bgr888_to_yuy2(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_bgr888_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_UYVY)
aipl_error_t aipl_color_convert_bgr888_to_uyvy(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_bgr888_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_RGB888
aipl_error_t aipl_color_convert_rgb888(const void* input, void* output,
                                       uint32_t pitch,
                                       uint32_t width, uint32_t height,
                                       aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_RGB888 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_rgb888_to_alpha8(input, output,
                                                       pitch,
                                                       width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_RGB888 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_rgb888_to_argb8888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_rgb888_to_rgba8888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_rgb888_to_argb4444(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_rgb888_to_argb1555(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_rgb888_to_rgba4444(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_rgb888_to_rgba5551(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_rgb888_to_rgb565(input, output,
                                                       pitch,
                                                       width, height);
#endif
        case AIPL_COLOR_RGB888:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_RGB888 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_rgb888_to_bgr888(input, output,
                                                       pitch,
                                                       width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_RGB888 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_rgb888_to_yv12(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_rgb888_to_i420(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_rgb888_to_nv12(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_rgb888_to_nv21(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_rgb888_to_i422(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_rgb888_to_yuy2(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_rgb888_to_uyvy(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_rgb888_to_i444(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB888 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_rgb888_to_i400(input, output,
                                                     pitch,
                                                     width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_RGB888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgb888_to_alpha8(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_24bit_to_alpha8(input, output, pitch,
                                              width, height,
                                              0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_ARGB8888)
aipl_error_t aipl_color_convert_rgb888_to_argb8888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_argb8888(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_ARGB1555)
aipl_error_t aipl_color_convert_rgb888_to_argb1555(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_argb1555(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_ARGB4444)
aipl_error_t aipl_color_convert_rgb888_to_argb4444(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_argb4444(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_RGBA8888)
aipl_error_t aipl_color_convert_rgb888_to_rgba8888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_rgba8888(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_RGBA4444)
aipl_error_t aipl_color_convert_rgb888_to_rgba4444(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_rgba4444(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_RGBA5551)
aipl_error_t aipl_color_convert_rgb888_to_rgba5551(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_24bit_to_rgba5551(input, output, pitch,
                                                width, height,
                                                0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_RGB565)
aipl_error_t aipl_color_convert_rgb888_to_rgb565(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_24bit_to_rgb565(input, output, pitch,
                                              width, height,
                                              0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_BGR888)
aipl_error_t aipl_color_convert_rgb888_to_bgr888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_24bit_to_24bit(input, output, pitch,
                                             width, height,
                                             0, 1, 2,
                                             2, 1, 0);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_YV12)
aipl_error_t aipl_color_convert_rgb888_to_yv12(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_24bit_to_yuv_planar(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_I420)
aipl_error_t aipl_color_convert_rgb888_to_i420(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_24bit_to_yuv_planar(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_I422)
aipl_error_t aipl_color_convert_rgb888_to_i422(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    return aipl_color_convert_24bit_to_i422(input, output, pitch,
                                            width, height,
                                            0, 1, 2);
#else
    const uint8_t* src_ptr = input;
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width / 2;
        uint8_t* u_dst = u_ptr + i * width / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_cnvt_px_24bit_to_yuv(y_dst, u_dst, v_dst, src, 0, 1, 2);

            src += 3;
            ++y_dst;
            ++u_dst;
            ++v_dst;

            aipl_cnvt_px_24bit_to_yuv_y(y_dst, src, 0, 1, 2);

            src += 3;
            ++y_dst;
       }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_I444)
aipl_error_t aipl_color_convert_rgb888_to_i444(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    return aipl_color_convert_24bit_to_i444(input, output, pitch,
                                            width, height,
                                            0, 1, 2);
#else
    const uint8_t* src_ptr = input;
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_yuv(y_dst, u_dst, v_dst, src, 0, 1, 2);

            src += 3;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgb888_to_i400(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_24bit_to_alpha8(input, output, pitch,
                                              width, height,
                                              0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_NV21)
aipl_error_t aipl_color_convert_rgb888_to_nv21(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_24bit_to_yuv_semi_planar(input, pitch,
                                                       width, height,
                                                       y_ptr, u_ptr, v_ptr,
                                                       0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_NV12)
aipl_error_t aipl_color_convert_rgb888_to_nv12(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_24bit_to_yuv_semi_planar(input, pitch,
                                                       width, height,
                                                       y_ptr, u_ptr, v_ptr,
                                                       0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_YUY2)
aipl_error_t aipl_color_convert_rgb888_to_yuy2(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgb888_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_UYVY)
aipl_error_t aipl_color_convert_rgb888_to_uyvy(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgb888_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_RGB565
aipl_error_t aipl_color_convert_rgb565(const void* input, void* output,
                                       uint32_t pitch,
                                       uint32_t width, uint32_t height,
                                       aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_RGB565 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_rgb565_to_alpha8(input, output,
                                                       pitch,
                                                       width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_RGB565 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_rgb565_to_argb8888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_rgb565_to_rgba8888(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_rgb565_to_argb4444(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_rgb565_to_argb1555(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_rgb565_to_rgba4444(input, output,
                                                         pitch,
                                                         width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_rgb565_to_rgba5551(input, output,
                                                         pitch,
                                                         width, height);
#endif
        case AIPL_COLOR_RGB565:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_RGB565 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_rgb565_to_bgr888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_rgb565_to_rgb888(input, output,
                                                       pitch,
                                                       width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_RGB565 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_rgb565_to_yv12(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_rgb565_to_i420(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_rgb565_to_nv12(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_rgb565_to_nv21(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_rgb565_to_i422(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_rgb565_to_yuy2(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_rgb565_to_uyvy(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_rgb565_to_i444(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_RGB565 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_rgb565_to_i400(input, output,
                                                     pitch,
                                                     width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_RGB565 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgb565_to_alpha8(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgb565, d2_mode_alpha8);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgb565(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb565_to_yuv_y(&y, px);

            vstrbq_p(dst, y, tail_p);

            src += 8;
            dst += 8;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_yuv_y(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_ARGB8888)
aipl_error_t aipl_color_convert_rgb565_to_argb8888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgb565, d2_mode_argb8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_argb8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_ARGB1555)
aipl_error_t aipl_color_convert_rgb565_to_argb1555(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgb565, d2_mode_argb1555);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_argb1555(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_ARGB4444)
aipl_error_t aipl_color_convert_rgb565_to_argb4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgb565, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_argb4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_RGBA8888)
aipl_error_t aipl_color_convert_rgb565_to_rgba8888(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgb565, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_rgba8888(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_RGBA4444)
aipl_error_t aipl_color_convert_rgb565_to_rgba4444(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgb565, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_rgba4444(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_RGBA5551)
aipl_error_t aipl_color_convert_rgb565_to_rgba5551(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_rgb565, d2_mode_rgb565);

//     return aipl_dave2d_error_convert(ret);
// #else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_rgba5551(dst, src);

            ++src;
            ++dst;
        }
    }

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_BGR888)
aipl_error_t aipl_color_convert_rgb565_to_bgr888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_rgb565_to_24bit(input, output, pitch,
                                              width, height,
                                              2, 1, 0);
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_RGB888)
aipl_error_t aipl_color_convert_rgb565_to_rgb888(const void* input,
                                                   void* output,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    return aipl_color_convert_rgb565_to_24bit(input, output, pitch,
                                              width, height,
                                              0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_YV12)
aipl_error_t aipl_color_convert_rgb565_to_yv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_rgb565_to_yuv_planar(input, pitch,
                                                   width, height,
                                                   y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_I420)
aipl_error_t aipl_color_convert_rgb565_to_i420(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_rgb565_to_yuv_planar(input, pitch,
                                                   width, height,
                                                   y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_I422)
aipl_error_t aipl_color_convert_rgb565_to_i422(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgb565(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb565_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgb565(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb565_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }

        src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgb565_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_I444)
aipl_error_t aipl_color_convert_rgb565_to_i444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgb565(&px, src, tail_p);

            uint16x8_t y;
            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb565_to_yuv(&y, &u, &v, px);

            vstrbq_p(y_dst, y, tail_p);
            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 8;
            y_dst += 8;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            ++y_dst;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_rgb565_to_i400(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    return aipl_color_convert_rgb565_to_alpha8(input, output, pitch,
                                               width, height);
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_NV21)
aipl_error_t aipl_color_convert_rgb565_to_nv21(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* v_ptr = output + yuv_size;
    uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_rgb565_to_yuv_semi_planar(input, pitch,
                                                        width, height,
                                                        y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_NV12)
aipl_error_t aipl_color_convert_rgb565_to_nv12(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_rgb565_to_yuv_semi_planar(input, pitch,
                                                        width, height,
                                                        y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_YUY2)
aipl_error_t aipl_color_convert_rgb565_to_yuy2(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + 1;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgb565_to_yuv_packed(input, pitch,
                                                   width, height,
                                                   y_ptr, u_ptr, v_ptr);
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_UYVY)
aipl_error_t aipl_color_convert_rgb565_to_uyvy(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output + 1;
    uint8_t* u_ptr = output;
    uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_rgb565_to_yuv_packed(input, pitch,
                                                   width, height,
                                                   y_ptr, u_ptr, v_ptr);
}
#endif
#endif

#if AIPL_CONVERT_YV12
aipl_error_t aipl_color_convert_yv12(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_YV12 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_yv12_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_YV12 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_yv12_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_yv12_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_yv12_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_yv12_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_yv12_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_yv12_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_yv12_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_yv12_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_yv12_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
        case AIPL_COLOR_YV12:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_YV12 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_yv12_to_i420(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_yv12_to_nv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_yv12_to_nv21(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_yv12_to_i422(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_yv12_to_yuy2(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_yv12_to_uyvy(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_yv12_to_i444(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YV12 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_yv12_to_i400(input, output,
                                                   pitch,
                                                   width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_YV12 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yv12_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ARGB8888)
aipl_error_t aipl_color_convert_yv12_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_argb8888(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ARGB4444)
aipl_error_t aipl_color_convert_yv12_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_argb4444(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ARGB1555)
aipl_error_t aipl_color_convert_yv12_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_argb1555(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGBA8888)
aipl_error_t aipl_color_convert_yv12_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_rgba8888(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGBA4444)
aipl_error_t aipl_color_convert_yv12_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_rgba4444(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGBA5551)
aipl_error_t aipl_color_convert_yv12_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_rgba5551(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_BGR888)
aipl_error_t aipl_color_convert_yv12_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_24bit(y_ptr, u_ptr, v_ptr,
                                                  output, pitch,
                                                  width, height,
                                                  2, 1, 0);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGB888)
aipl_error_t aipl_color_convert_yv12_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_24bit(y_ptr, u_ptr, v_ptr,
                                                  output, pitch,
                                                  width, height,
                                                  0, 1, 2);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGB565)
aipl_error_t aipl_color_convert_yv12_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_rgb565(y_ptr, u_ptr, v_ptr,
                                                   output, pitch,
                                                   width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_I420)
aipl_error_t aipl_color_convert_yv12_to_i420(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_planar(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_I422)
aipl_error_t aipl_color_convert_yv12_to_i422(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* v_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* u_src_ptr = v_src_ptr + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;
        const uint8_t* u_src = u_src_ptr + i / 2 * pitch / 2;
        uint8_t* u_dst = u_dst_ptr + i * width / 2;
        const uint8_t* v_src = v_src_ptr + i / 2 * pitch / 2;
        uint8_t* v_dst = v_dst_ptr + i * width / 2;

        memcpy(y_dst, y_src, width);
        memcpy(u_dst, u_src, width / 2);
        memcpy(v_dst, v_src, width / 2);
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_I444)
aipl_error_t aipl_color_convert_yv12_to_i444(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* v_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* u_src_ptr = v_src_ptr + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;
        const uint8_t* u_src = u_src_ptr + i / 2 * pitch / 2;
        uint8_t* u_dst = u_dst_ptr + i * width;
        const uint8_t* v_src = v_src_ptr + i / 2 * pitch / 2;
        uint8_t* v_dst = v_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (i & 1)
        {
            memcpy(u_dst, u_dst - width, width);
            memcpy(v_dst, v_dst - width, width);
        }
        else
        {
#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width;

            uint32_t j = 0;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vld1q_z(u_src, tail_p);

                vstrbq_scatter_offset_p(u_dst + 1, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vld1q_z(v_src, tail_p);

                vstrbq_scatter_offset_p(v_dst + 1, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_src += 16;
                v_src += 16;
                u_dst += 32;
                v_dst += 32;
                cnt -= 32;
            }
#else
            for (uint32_t j = 0; j < width; ++j)
            {
                u_dst[j] = u_src[j / 2];
                v_dst[j] = v_src[j / 2];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yv12_to_i400(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_NV21)
aipl_error_t aipl_color_convert_yv12_to_nv21(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + 1;

    return aipl_color_convert_yuv_planar_to_semi(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_NV12)
aipl_error_t aipl_color_convert_yv12_to_nv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + 1;

    return aipl_color_convert_yuv_planar_to_semi(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_YUY2)
aipl_error_t aipl_color_convert_yv12_to_yuy2(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + yuv_size / 4;

    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_planar_to_packed(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YV12 & TO_UYVY)
aipl_error_t aipl_color_convert_yv12_to_uyvy(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + yuv_size / 4;

    uint8_t* u_dst = output;
    uint8_t* y_dst = u_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_planar_to_packed(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif
#endif

#if AIPL_CONVERT_I420
aipl_error_t aipl_color_convert_i420(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_I420 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_i420_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_I420 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_i420_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_i420_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_i420_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_i420_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_i420_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_i420_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_i420_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_i420_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_i420_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_I420 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_i420_to_yv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
        case AIPL_COLOR_I420:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_I420 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_i420_to_nv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_i420_to_nv21(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_i420_to_i422(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_i420_to_yuy2(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_i420_to_uyvy(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_i420_to_i444(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I420 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_i420_to_i400(input, output,
                                                   pitch,
                                                   width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_I420 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_i420_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_ARGB8888)
aipl_error_t aipl_color_convert_i420_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_argb8888(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_ARGB4444)
aipl_error_t aipl_color_convert_i420_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_argb4444(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_ARGB1555)
aipl_error_t aipl_color_convert_i420_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_argb1555(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_RGBA8888)
aipl_error_t aipl_color_convert_i420_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_rgba8888(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_RGBA4444)
aipl_error_t aipl_color_convert_i420_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_rgba4444(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_RGBA5551)
aipl_error_t aipl_color_convert_i420_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_rgba5551(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_BGR888)
aipl_error_t aipl_color_convert_i420_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_24bit(y_ptr, u_ptr, v_ptr,
                                                  output, pitch,
                                                  width, height,
                                                  2, 1, 0);
}
#endif

#if (AIPL_CONVERT_I420 & TO_RGB888)
aipl_error_t aipl_color_convert_i420_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_24bit(y_ptr, u_ptr, v_ptr,
                                                  output, pitch,
                                                  width, height,
                                                  0, 1, 2);
}
#endif

#if (AIPL_CONVERT_I420 & TO_RGB565)
aipl_error_t aipl_color_convert_i420_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_rgb565(y_ptr, u_ptr, v_ptr,
                                                   output, pitch,
                                                   width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_YV12)
aipl_error_t aipl_color_convert_i420_to_yv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + yuv_size / 4;

    return aipl_color_convert_yuv_planar_to_planar(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_I422)
aipl_error_t aipl_color_convert_i420_to_i422(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;
        const uint8_t* u_src = u_src_ptr + i / 2 * pitch / 2;
        uint8_t* u_dst = u_dst_ptr + i * width / 2;
        const uint8_t* v_src = v_src_ptr + i / 2 * pitch / 2;
        uint8_t* v_dst = v_dst_ptr + i * width / 2;

        memcpy(y_dst, y_src, width);
        memcpy(u_dst, u_src, width / 2);
        memcpy(v_dst, v_src, width / 2);
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I420 & TO_I444)
aipl_error_t aipl_color_convert_i420_to_i444(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;
        const uint8_t* u_src = u_src_ptr + i / 2 * pitch / 2;
        uint8_t* u_dst = u_dst_ptr + i * width;
        const uint8_t* v_src = v_src_ptr + i / 2 * pitch / 2;
        uint8_t* v_dst = v_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (i & 1)
        {
            memcpy(u_dst, u_dst - width, width);
            memcpy(v_dst, v_dst - width, width);
        }
        else
        {
#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width;

            uint32_t j = 0;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vld1q_z(u_src, tail_p);

                vstrbq_scatter_offset_p(u_dst + 1, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vld1q_z(v_src, tail_p);

                vstrbq_scatter_offset_p(v_dst + 1, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_src += 16;
                v_src += 16;
                u_dst += 32;
                v_dst += 32;
                cnt -= 32;
            }
#else
            for (uint32_t j = 0; j < width; ++j)
            {
                u_dst[j] = u_src[j / 2];
                v_dst[j] = v_src[j / 2];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I420 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_i420_to_i400(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_NV21)
aipl_error_t aipl_color_convert_i420_to_nv21(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + 1;

    return aipl_color_convert_yuv_planar_to_semi(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_NV12)
aipl_error_t aipl_color_convert_i420_to_nv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + yuv_size / 4;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + 1;

    return aipl_color_convert_yuv_planar_to_semi(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_YUY2)
aipl_error_t aipl_color_convert_i420_to_yuy2(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + yuv_size / 4;

    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_planar_to_packed(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I420 & TO_UYVY)
aipl_error_t aipl_color_convert_i420_to_uyvy(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + yuv_size / 4;

    uint8_t* u_dst = output;
    uint8_t* y_dst = u_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_planar_to_packed(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif
#endif

#if AIPL_CONVERT_I422
aipl_error_t aipl_color_convert_i422(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_I422 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_i422_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_I422 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_i422_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_i422_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_i422_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_i422_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_i422_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_i422_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_i422_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_i422_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_i422_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_I422 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_i422_to_yv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_i422_to_i420(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_i422_to_nv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_i422_to_nv21(input, output,
                                                   pitch,
                                                   width, height);
#endif
        case AIPL_COLOR_I422:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_I422 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_i422_to_yuy2(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_i422_to_uyvy(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_i422_to_i444(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I422 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_i422_to_i400(input, output,
                                                   pitch,
                                                   width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_I422 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_i422_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I422 & TO_ARGB8888)
aipl_error_t aipl_color_convert_i422_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px0, px1;
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px0, c0, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px1, c1, r, g, b);

            vstrwq_scatter_offset_p(dst, AIPL_8_BYTE_OFFSETS_U32, px0, tail_p);
            vstrwq_scatter_offset_p(dst + 1, AIPL_8_BYTE_OFFSETS_U32, px1, tail_p);

            y_src += 8;
            u_src += 4;
            v_src += 4;
            dst += 8;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 1]);

            aipl_cnvt_px_yuv_to_argb8888(dst + j, c0, r, g, b);
            aipl_cnvt_px_yuv_to_argb8888(dst + j + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_ARGB4444)
aipl_error_t aipl_color_convert_i422_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_argb4444(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 4;
            v_src += 4;
            dst += 8;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 1]);

            aipl_cnvt_px_yuv_to_argb4444(dst + j, c0, r, g, b);
            aipl_cnvt_px_yuv_to_argb4444(dst + j + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_ARGB1555)
aipl_error_t aipl_color_convert_i422_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_argb1555(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 4;
            v_src += 4;
            dst += 8;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 1]);

            aipl_cnvt_px_yuv_to_argb1555(dst + j, c0, r, g, b);
            aipl_cnvt_px_yuv_to_argb1555(dst + j + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_RGBA8888)
aipl_error_t aipl_color_convert_i422_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px0, px1;
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px0, c0, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px1, c1, r, g, b);

            vstrwq_scatter_offset_p(dst, AIPL_8_BYTE_OFFSETS_U32, px0, tail_p);
            vstrwq_scatter_offset_p(dst + 1, AIPL_8_BYTE_OFFSETS_U32, px1, tail_p);

            y_src += 8;
            u_src += 4;
            v_src += 4;
            dst += 8;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 1]);

            aipl_cnvt_px_yuv_to_rgba8888(dst + j, c0, r, g, b);
            aipl_cnvt_px_yuv_to_rgba8888(dst + j + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_RGBA4444)
aipl_error_t aipl_color_convert_i422_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_rgba4444(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 4;
            v_src += 4;
            dst += 8;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 1]);

            aipl_cnvt_px_yuv_to_rgba4444(dst + j, c0, r, g, b);
            aipl_cnvt_px_yuv_to_rgba4444(dst + j + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_RGBA5551)
aipl_error_t aipl_color_convert_i422_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_rgba5551(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 4;
            v_src += 4;
            dst += 8;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 1]);

            aipl_cnvt_px_yuv_to_rgba5551(dst + j, c0, r, g, b);
            aipl_cnvt_px_yuv_to_rgba5551(dst + j + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_BGR888)
aipl_error_t aipl_color_convert_i422_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i422_to_24bit(input, output, pitch,
                                            width, height,
                                            2, 1, 0);
}
#endif

#if (AIPL_CONVERT_I422 & TO_RGB888)
aipl_error_t aipl_color_convert_i422_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i422_to_24bit(input, output, pitch,
                                            width, height,
                                            0, 1, 2);
}
#endif

#if (AIPL_CONVERT_I422 & TO_RGB565)
aipl_error_t aipl_color_convert_i422_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_rgb565(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 4;
            v_src += 4;
            dst += 8;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 1]);

            aipl_cnvt_px_yuv_to_rgb565(dst + j, c0, r, g, b);
            aipl_cnvt_px_yuv_to_rgb565(dst + j + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_YV12)
aipl_error_t aipl_color_convert_i422_to_yv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 2;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* v_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* u_dst_ptr = v_dst_ptr + yuv_size / 4;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (!(i & 1))
        {
            const uint8_t* u_src = u_src_ptr + i * pitch / 2;
            uint8_t* u_dst = u_dst_ptr + i * width / 4;
            const uint8_t* v_src = v_src_ptr + i * pitch / 2;
            uint8_t* v_dst = v_dst_ptr + i * width / 4;

            memcpy(u_dst, u_src, width / 2);
            memcpy(v_dst, v_src, width / 2);
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_I420)
aipl_error_t aipl_color_convert_i422_to_i420(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 2;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 4;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (!(i & 1))
        {
            const uint8_t* u_src = u_src_ptr + i * pitch / 2;
            uint8_t* u_dst = u_dst_ptr + i * width / 4;
            const uint8_t* v_src = v_src_ptr + i * pitch / 2;
            uint8_t* v_dst = v_dst_ptr + i * width / 4;

            memcpy(u_dst, u_src, width / 2);
            memcpy(v_dst, v_src, width / 2);
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_I444)
aipl_error_t aipl_color_convert_i422_to_i444(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 2;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;
        const uint8_t* u_src = u_src_ptr + i * pitch / 2;
        uint8_t* u_dst = u_dst_ptr + i * width;
        const uint8_t* v_src = v_src_ptr + i * pitch / 2;
        uint8_t* v_dst = v_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vld1q_z(u_src, tail_p);

            vstrbq_scatter_offset_p(u_dst + 1, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vld1q_z(v_src, tail_p);

            vstrbq_scatter_offset_p(v_dst + 1, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

            u_src += 16;
            v_src += 16;
            u_dst += 32;
            v_dst += 32;
            cnt -= 32;
        }
#else
        for (uint32_t j = 0; j < width; ++j)
        {
            u_dst[j] = u_src[j / 2];
            v_dst[j] = v_src[j / 2];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_i422_to_i400(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I422 & TO_NV21)
aipl_error_t aipl_color_convert_i422_to_nv21(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 2;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* v_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* u_dst_ptr = v_dst_ptr + 1;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (!(i & 1))
        {
            const uint8_t* u_src = u_src_ptr + i * pitch / 2;
            uint8_t* u_dst = u_dst_ptr + i / 2 * width;
            const uint8_t* v_src = v_src_ptr + i * pitch / 2;
            uint8_t* v_dst = v_dst_ptr + i / 2 * width;
#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width;

            uint32_t j = 0;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vld1q_z(u_src, tail_p);

                vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vld1q_z(v_src, tail_p);

                vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_src += 16;
                v_src += 16;
                u_dst += 32;
                v_dst += 32;
                cnt -= 32;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_dst[j] = u_src[j / 2];
                v_dst[j] = v_src[j / 2];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_NV12)
aipl_error_t aipl_color_convert_i422_to_nv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 2;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + 1;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (!(i & 1))
        {
            const uint8_t* u_src = u_src_ptr + i * pitch / 2;
            uint8_t* u_dst = u_dst_ptr + i / 2 * width;
            const uint8_t* v_src = v_src_ptr + i * pitch / 2;
            uint8_t* v_dst = v_dst_ptr + i / 2 * width;
#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width;

            uint32_t j = 0;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vld1q_z(u_src, tail_p);

                vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vld1q_z(v_src, tail_p);

                vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_src += 16;
                v_src += 16;
                u_dst += 32;
                v_dst += 32;
                cnt -= 32;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_dst[j] = u_src[j / 2];
                v_dst[j] = v_src[j / 2];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_YUY2)
aipl_error_t aipl_color_convert_i422_to_yuy2(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 2;

    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + 1;
    uint8_t* v_dst_ptr = u_dst_ptr + 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width * 2;
        const uint8_t* u_src = u_src_ptr + i * pitch / 2;
        uint8_t* u_dst = u_dst_ptr + i * width * 2;
        const uint8_t* v_src = v_src_ptr + i * pitch / 2;
        uint8_t* v_dst = v_dst_ptr + i * width * 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_dst[j * 4] = y_src[j * 2];
            y_dst[j * 4 + 2] = y_src[j * 2 + 1];
            u_dst[j * 4] = u_src[j];
            v_dst[j * 4] = v_src[j];
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I422 & TO_UYVY)
aipl_error_t aipl_color_convert_i422_to_uyvy(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size / 2;

    uint8_t* u_dst_ptr = output;
    uint8_t* y_dst_ptr = u_dst_ptr + 1;
    uint8_t* v_dst_ptr = u_dst_ptr + 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width * 2;
        const uint8_t* u_src = u_src_ptr + i * pitch / 2;
        uint8_t* u_dst = u_dst_ptr + i * width * 2;
        const uint8_t* v_src = v_src_ptr + i * pitch / 2;
        uint8_t* v_dst = v_dst_ptr + i * width * 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_dst[j * 4] = y_src[j * 2];
            y_dst[j * 4 + 2] = y_src[j * 2 + 1];
            u_dst[j * 4] = u_src[j];
            v_dst[j * 4] = v_src[j];
        }
    }

    return AIPL_ERR_OK;
}
#endif
#endif

#if AIPL_CONVERT_I444
aipl_error_t aipl_color_convert_i444(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_I444 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_i444_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_I444 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_i444_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_i444_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_i444_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_i444_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_i444_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_i444_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_i444_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_i444_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_i444_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_I444 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_i444_to_yv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_i444_to_i420(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_i444_to_nv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_i444_to_nv21(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_i444_to_i422(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_i444_to_yuy2(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_I444 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_i444_to_uyvy(input, output,
                                                   pitch,
                                                   width, height);
#endif
        case AIPL_COLOR_I444:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_I444 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_i444_to_i400(input, output,
                                                   pitch,
                                                   width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_I444 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_i444_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I444 & TO_ARGB8888)
aipl_error_t aipl_color_convert_i444_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t y = vldrbq_z_u32(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c;
            aipl_mve_pre_cnvt_4px_y(&c, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px;
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px, c, r, g, b);

            vstrwq_p(dst, px, tail_p);

            y_src += 4;
            u_src += 4;
            v_src += 4;
            dst += 4;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            int32_t c;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_px_y(&c, y_src[j]);

            aipl_cnvt_px_yuv_to_argb8888(dst + j, c, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_ARGB4444)
aipl_error_t aipl_color_convert_i444_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint16x8_t u = vldrbq_u16(u_src);
            uint16x8_t v = vldrbq_u16(v_src);

            int32x4_t r0, g0, b0, r1, g1, b1;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_8px_yuv_to_rgb(&r0, &g0, &b0, &r1, &g1, &b1,
                                             u, v);

            uint16x8_t px;
            aipl_mve_cnvt_44px_yuv_to_argb4444(&px, c0, c1, r0, g0, b0,
                                               r1, g1, b1);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 8;
            v_src += 8;
            dst += 8;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            int32_t c;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_px_y(&c, y_src[j]);

            aipl_cnvt_px_yuv_to_argb4444(dst + j, c, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_ARGB1555)
aipl_error_t aipl_color_convert_i444_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint16x8_t u = vldrbq_u16(u_src);
            uint16x8_t v = vldrbq_u16(v_src);

            int32x4_t r0, g0, b0, r1, g1, b1;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_8px_yuv_to_rgb(&r0, &g0, &b0, &r1, &g1, &b1,
                                             u, v);

            uint16x8_t px;
            aipl_mve_cnvt_44px_yuv_to_argb1555(&px, c0, c1, r0, g0, b0,
                                               r1, g1, b1);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 8;
            v_src += 8;
            dst += 8;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            int32_t c;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_px_y(&c, y_src[j]);

            aipl_cnvt_px_yuv_to_argb1555(dst + j, c, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_RGBA8888)
aipl_error_t aipl_color_convert_i444_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t y = vldrbq_z_u32(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c;
            aipl_mve_pre_cnvt_4px_y(&c, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px;
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px, c, r, g, b);

            vstrwq_p(dst, px, tail_p);

            y_src += 4;
            u_src += 4;
            v_src += 4;
            dst += 4;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            int32_t c;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_px_y(&c, y_src[j]);

            aipl_cnvt_px_yuv_to_rgba8888(dst + j, c, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_RGBA4444)
aipl_error_t aipl_color_convert_i444_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint16x8_t u = vldrbq_u16(u_src);
            uint16x8_t v = vldrbq_u16(v_src);

            int32x4_t r0, g0, b0, r1, g1, b1;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_8px_yuv_to_rgb(&r0, &g0, &b0, &r1, &g1, &b1,
                                             u, v);

            uint16x8_t px;
            aipl_mve_cnvt_44px_yuv_to_rgba4444(&px, c0, c1, r0, g0, b0,
                                               r1, g1, b1);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 8;
            v_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            int32_t c;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_px_y(&c, y_src[j]);

            aipl_cnvt_px_yuv_to_rgba4444(dst + j, c, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_RGBA5551)
aipl_error_t aipl_color_convert_i444_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint16x8_t u = vldrbq_u16(u_src);
            uint16x8_t v = vldrbq_u16(v_src);

            int32x4_t r0, g0, b0, r1, g1, b1;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_8px_yuv_to_rgb(&r0, &g0, &b0, &r1, &g1, &b1,
                                             u, v);

            uint16x8_t px;
            aipl_mve_cnvt_44px_yuv_to_rgba5551(&px, c0, c1, r0, g0, b0,
                                               r1, g1, b1);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 8;
            v_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            int32_t c;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_px_y(&c, y_src[j]);

            aipl_cnvt_px_yuv_to_rgba5551(dst + j, c, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_BGR888)
aipl_error_t aipl_color_convert_i444_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i444_to_24bit(input, output, pitch,
                                            width, height,
                                            2, 1, 0);
}
#endif

#if (AIPL_CONVERT_I444 & TO_RGB888)
aipl_error_t aipl_color_convert_i444_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_i444_to_24bit(input, output, pitch,
                                            width, height,
                                            0, 1, 2);
}
#endif

#if (AIPL_CONVERT_I444 & TO_RGB565)
aipl_error_t aipl_color_convert_i444_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint16x8_t u = vldrbq_u16(u_src);
            uint16x8_t v = vldrbq_u16(v_src);

            int32x4_t r0, g0, b0, r1, g1, b1;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_8px_yuv_to_rgb(&r0, &g0, &b0, &r1, &g1, &b1,
                                             u, v);

            uint16x8_t px;
            aipl_mve_cnvt_44px_yuv_to_rgb565(&px, c0, c1, r0, g0, b0,
                                             r1, g1, b1);

            vstrhq_p(dst, px, tail_p);

            y_src += 8;
            u_src += 8;
            v_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            int32_t c;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_px_y(&c, y_src[j]);

            aipl_cnvt_px_yuv_to_rgb565(dst + j, c, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_YV12)
aipl_error_t aipl_color_convert_i444_to_yv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* v_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* u_dst_ptr = v_dst_ptr + yuv_size / 4;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (!(i & 1))
        {
            const uint8_t* u_src = u_src_ptr + i * pitch;
            uint8_t* u_dst = u_dst_ptr + i * width / 4;
            const uint8_t* v_src = v_src_ptr + i * pitch;
            uint8_t* v_dst = v_dst_ptr + i * width / 4;

#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width / 2;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vst1q_p(u_dst, u, tail_p);

                uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vst1q_p(v_dst, v, tail_p);

                u_src += 32;
                v_src += 32;
                u_dst += 16;
                v_dst += 16;
                cnt -= 16;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_dst[j / 2] = u_src[j];
                v_dst[j / 2] = v_src[j];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_I420)
aipl_error_t aipl_color_convert_i444_to_i420(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 4;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (!(i & 1))
        {
            const uint8_t* u_src = u_src_ptr + i * pitch;
            uint8_t* u_dst = u_dst_ptr + i * width / 4;
            const uint8_t* v_src = v_src_ptr + i * pitch;
            uint8_t* v_dst = v_dst_ptr + i * width / 4;

#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width / 2;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vst1q_p(u_dst, u, tail_p);

                uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vst1q_p(v_dst, v, tail_p);

                u_src += 32;
                v_src += 32;
                u_dst += 16;
                v_dst += 16;
                cnt -= 16;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_dst[j / 2] = u_src[j];
                v_dst[j / 2] = v_src[j];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_I422)
aipl_error_t aipl_color_convert_i444_to_i422(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;
        const uint8_t* u_src = u_src_ptr + i * pitch;
        uint8_t* u_dst = u_dst_ptr + i * width / 2;
        const uint8_t* v_src = v_src_ptr + i * pitch;
        uint8_t* v_dst = v_dst_ptr + i * width / 2;

        memcpy(y_dst, y_src, width);

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vst1q_p(u_dst, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vst1q_p(v_dst, v, tail_p);

            u_src += 32;
            v_src += 32;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width; j += 2)
        {
            u_dst[j / 2] = u_src[j];
            v_dst[j / 2] = v_src[j];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_i444_to_i400(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_I444 & TO_NV21)
aipl_error_t aipl_color_convert_i444_to_nv21(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* v_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* u_dst_ptr = v_dst_ptr + 1;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (!(i & 1))
        {
            const uint8_t* u_src = u_src_ptr + i * pitch;
            uint8_t* u_dst = u_dst_ptr + i / 2 * width;
            const uint8_t* v_src = v_src_ptr + i * pitch;
            uint8_t* v_dst = v_dst_ptr + i / 2 * width;

            for (uint32_t j = 0; j < width; j += 2)
            {
                u_dst[j] = u_src[j];
                v_dst[j] = v_src[j];
            }
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_NV12)
aipl_error_t aipl_color_convert_i444_to_nv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + 1;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        if (!(i & 1))
        {
            const uint8_t* u_src = u_src_ptr + i * pitch;
            uint8_t* u_dst = u_dst_ptr + i / 2 * width;
            const uint8_t* v_src = v_src_ptr + i * pitch;
            uint8_t* v_dst = v_dst_ptr + i / 2 * width;

            for (uint32_t j = 0; j < width; j += 2)
            {
                u_dst[j] = u_src[j];
                v_dst[j] = v_src[j];
            }
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_YUY2)
aipl_error_t aipl_color_convert_i444_to_yuy2(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size;

    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + 1;
    uint8_t* v_dst_ptr = u_dst_ptr + 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width * 2;
        const uint8_t* u_src = u_src_ptr + i * pitch;
        uint8_t* u_dst = u_dst_ptr + i * width * 2;
        const uint8_t* v_src = v_src_ptr + i * pitch;
        uint8_t* v_dst = v_dst_ptr + i * width * 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_dst[j * 4] = y_src[j * 2];
            y_dst[j * 4 + 2] = y_src[j * 2 + 1];
            u_dst[j * 4] = u_src[j * 2];
            v_dst[j * 4] = v_src[j * 2];
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_UYVY)
aipl_error_t aipl_color_convert_i444_to_uyvy(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + yuv_size;

    uint8_t* u_dst_ptr = output;
    uint8_t* y_dst_ptr = u_dst_ptr + 1;
    uint8_t* v_dst_ptr = u_dst_ptr + 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width * 2;
        const uint8_t* u_src = u_src_ptr + i * pitch;
        uint8_t* u_dst = u_dst_ptr + i * width * 2;
        const uint8_t* v_src = v_src_ptr + i * pitch;
        uint8_t* v_dst = v_dst_ptr + i * width * 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_dst[j * 4] = y_src[j * 2];
            y_dst[j * 4 + 2] = y_src[j * 2 + 1];
            u_dst[j * 4] = u_src[j * 2];
            v_dst[j * 4] = v_src[j * 2];
        }
    }

    return AIPL_ERR_OK;
}
#endif
#endif

#if AIPL_CONVERT_NV12
aipl_error_t aipl_color_convert_nv12(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_NV12 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_nv12_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_NV12 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_nv12_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_nv12_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_nv12_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_nv12_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_nv12_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_nv12_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_nv12_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_nv12_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_nv12_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_NV12 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_nv12_to_yv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_nv12_to_i420(input, output,
                                                   pitch,
                                                   width, height);
#endif
        case AIPL_COLOR_NV12:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_NV12 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_nv12_to_nv21(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_nv12_to_i422(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_nv12_to_yuy2(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_nv12_to_uyvy(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_nv12_to_i444(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV12 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_nv12_to_i400(input, output,
                                                   pitch,
                                                   width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_NV12 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_nv12_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_ARGB8888)
aipl_error_t aipl_color_convert_nv12_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_argb8888(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_ARGB4444)
aipl_error_t aipl_color_convert_nv12_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_argb4444(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_ARGB1555)
aipl_error_t aipl_color_convert_nv12_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_argb1555(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGBA8888)
aipl_error_t aipl_color_convert_nv12_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_rgba8888(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGBA4444)
aipl_error_t aipl_color_convert_nv12_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_rgba4444(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGBA5551)
aipl_error_t aipl_color_convert_nv12_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_rgba5551(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_BGR888)
aipl_error_t aipl_color_convert_nv12_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_24bit(y_ptr, u_ptr, v_ptr,
                                                       output, pitch,
                                                       width, height,
                                                       2, 1, 0);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGB888)
aipl_error_t aipl_color_convert_nv12_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_24bit(y_ptr, u_ptr, v_ptr,
                                                       output, pitch,
                                                       width, height,
                                                       0, 1, 2);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGB565)
aipl_error_t aipl_color_convert_nv12_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_rgb565(y_ptr, u_ptr, v_ptr,
                                                        output, pitch,
                                                        width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_YV12)
aipl_error_t aipl_color_convert_nv12_to_yv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + 1;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + yuv_size / 4;

    return aipl_color_convert_yuv_semi_to_planar(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_I420)
aipl_error_t aipl_color_convert_nv12_to_i420(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + 1;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + yuv_size / 4;

    return aipl_color_convert_yuv_semi_to_planar(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_I422)
aipl_error_t aipl_color_convert_nv12_to_i422(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + 1;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;
        const uint8_t* u_src = u_src_ptr + i / 2 * pitch;
        uint8_t* u_dst = u_dst_ptr + i * width / 2;
        const uint8_t* v_src = v_src_ptr + i / 2 * pitch;
        uint8_t* v_dst = v_dst_ptr + i * width / 2;

        memcpy(y_dst, y_src, width);

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vst1q_p(u_dst, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vst1q_p(v_dst, v, tail_p);

            u_src += 32;
            v_src += 32;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width; j += 2)
        {
            u_dst[j / 2] = u_src[j];
            v_dst[j / 2] = v_src[j];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_nv12_to_i400(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_NV21)
aipl_error_t aipl_color_convert_nv12_to_nv21(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + 1;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + 1;

    return aipl_color_convert_yuv_semi_to_semi_planar(y_src, u_src, v_src,
                                                      y_dst, u_dst, v_dst,
                                                      pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_I444)
aipl_error_t aipl_color_convert_nv12_to_i444(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* v_src_ptr = u_src_ptr + 1;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        const uint8_t* u_src = u_src_ptr + i / 2 * pitch;
        uint8_t* u_dst = u_dst_ptr + i * width;
        const uint8_t* v_src = v_src_ptr + i / 2 * pitch;
        uint8_t* v_dst = v_dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            u_dst[j] = u_dst[j + 1] = u_src[j];
            v_dst[j] = v_dst[j + 1] = v_src[j];
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_YUY2)
aipl_error_t aipl_color_convert_nv12_to_yuy2(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + 1;

    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_semi_to_packed(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV12 & TO_UYVY)
aipl_error_t aipl_color_convert_nv12_to_uyvy(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + yuv_size;
    const uint8_t* v_src = u_src + 1;

    uint8_t* u_dst = output;
    uint8_t* y_dst = u_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_semi_to_packed(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif
#endif

#if AIPL_CONVERT_NV21
aipl_error_t aipl_color_convert_nv21(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_NV21 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_nv21_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_NV21 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_nv21_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_nv21_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_nv21_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_nv21_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_nv21_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_nv21_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_nv21_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_nv21_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_nv21_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_NV21 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_nv21_to_yv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_nv21_to_i420(input, output,
                                                   pitch,
                                                   width, height);
#endif
        case AIPL_COLOR_NV21:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_NV21 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_nv21_to_nv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_nv21_to_i422(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_nv21_to_yuy2(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_nv21_to_uyvy(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_nv21_to_i444(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_NV21 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_nv21_to_i400(input, output,
                                                   pitch,
                                                   width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_NV21 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_nv21_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_ARGB8888)
aipl_error_t aipl_color_convert_nv21_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_argb8888(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_ARGB4444)
aipl_error_t aipl_color_convert_nv21_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_argb4444(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_ARGB1555)
aipl_error_t aipl_color_convert_nv21_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_argb1555(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_RGBA8888)
aipl_error_t aipl_color_convert_nv21_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_rgba8888(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_RGBA4444)
aipl_error_t aipl_color_convert_nv21_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_rgba4444(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_RGBA5551)
aipl_error_t aipl_color_convert_nv21_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_rgba5551(y_ptr, u_ptr, v_ptr,
                                                          output, pitch,
                                                          width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_BGR888)
aipl_error_t aipl_color_convert_nv21_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_24bit(y_ptr, u_ptr, v_ptr,
                                                       output, pitch,
                                                       width, height,
                                                       2, 1, 0);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_RGB888)
aipl_error_t aipl_color_convert_nv21_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_24bit(y_ptr, u_ptr, v_ptr,
                                                       output, pitch,
                                                       width, height,
                                                       0, 1, 2);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_RGB565)
aipl_error_t aipl_color_convert_nv21_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* v_ptr = y_ptr + yuv_size;
    const uint8_t* u_ptr = v_ptr + 1;

    return aipl_color_convert_yuv_semi_planar_to_rgb565(y_ptr, u_ptr, v_ptr,
                                                        output, pitch,
                                                        width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_YV12)
aipl_error_t aipl_color_convert_nv21_to_yv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + 1;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + yuv_size / 4;

    return aipl_color_convert_yuv_semi_to_planar(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_I420)
aipl_error_t aipl_color_convert_nv21_to_i420(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + 1;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + yuv_size / 4;

    return aipl_color_convert_yuv_semi_to_planar(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_I422)
aipl_error_t aipl_color_convert_nv21_to_i422(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* v_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* u_src_ptr = v_src_ptr + 1;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;
        const uint8_t* u_src = u_src_ptr + i / 2 * pitch;
        uint8_t* u_dst = u_dst_ptr + i * width / 2;
        const uint8_t* v_src = v_src_ptr + i / 2 * pitch;
        uint8_t* v_dst = v_dst_ptr + i * width / 2;

        memcpy(y_dst, y_src, width);

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vst1q_p(u_dst, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vst1q_p(v_dst, v, tail_p);

            u_src += 32;
            v_src += 32;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width; j += 2)
        {
            u_dst[j / 2] = u_src[j];
            v_dst[j / 2] = v_src[j];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV21 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_nv21_to_i400(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_yuv_planar_to_alpha8(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_NV12)
aipl_error_t aipl_color_convert_nv21_to_nv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + 1;

    yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + 1;

    return aipl_color_convert_yuv_semi_to_semi_planar(y_src, u_src, v_src,
                                                      y_dst, u_dst, v_dst,
                                                      pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_I444)
aipl_error_t aipl_color_convert_nv21_to_i444(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src_ptr = input;
    const uint8_t* v_src_ptr = y_src_ptr + yuv_size;
    const uint8_t* u_src_ptr = v_src_ptr + 1;

    yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        const uint8_t* u_src = u_src_ptr + i / 2 * pitch;
        uint8_t* u_dst = u_dst_ptr + i * width;
        const uint8_t* v_src = v_src_ptr + i / 2 * pitch;
        uint8_t* v_dst = v_dst_ptr + i * width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            u_dst[j] = u_dst[j + 1] = u_src[j];
            v_dst[j] = v_dst[j + 1] = v_src[j];
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV21 & TO_YUY2)
aipl_error_t aipl_color_convert_nv21_to_yuy2(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + 1;

    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_semi_to_packed(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_NV21 & TO_UYVY)
aipl_error_t aipl_color_convert_nv21_to_uyvy(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_src = input;
    const uint8_t* v_src = y_src + yuv_size;
    const uint8_t* u_src = v_src + 1;

    uint8_t* u_dst = output;
    uint8_t* y_dst = u_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_semi_to_packed(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif
#endif

#if AIPL_CONVERT_YUY2
aipl_error_t aipl_color_convert_yuy2(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_YUY2 & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_yuy2_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_YUY2 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_yuy2_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_yuy2_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_yuy2_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_yuy2_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_yuy2_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_yuy2_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_yuy2_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_yuy2_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_yuy2_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_YUY2 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_yuy2_to_yv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_yuy2_to_i420(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_yuy2_to_nv21(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_yuy2_to_nv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_yuy2_to_i422(input, output,
                                                   pitch,
                                                   width, height);
#endif
        case AIPL_COLOR_YUY2:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_YUY2 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_yuy2_to_uyvy(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_yuy2_to_i444(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_YUY2 & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_yuy2_to_i400(input, output,
                                                   pitch,
                                                   width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_YUY2 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yuy2_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_yuv_packed_to_alpha8(input, output, pitch,
                                                   width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_ARGB8888)
aipl_error_t aipl_color_convert_yuy2_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_argb8888(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_ARGB4444)
aipl_error_t aipl_color_convert_yuy2_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_argb4444(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_ARGB1555)
aipl_error_t aipl_color_convert_yuy2_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_argb1555(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGBA8888)
aipl_error_t aipl_color_convert_yuy2_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_rgba8888(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGBA4444)
aipl_error_t aipl_color_convert_yuy2_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_rgba4444(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGBA5551)
aipl_error_t aipl_color_convert_yuy2_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_rgba5551(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_BGR888)
aipl_error_t aipl_color_convert_yuy2_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_24bit(y_ptr, u_ptr, v_ptr,
                                                  output, pitch,
                                                  width, height,
                                                  2, 1, 0);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGB888)
aipl_error_t aipl_color_convert_yuy2_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_24bit(y_ptr, u_ptr, v_ptr,
                                                  output, pitch,
                                                  width, height,
                                                  0, 1, 2);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGB565)
aipl_error_t aipl_color_convert_yuy2_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_rgb565(y_ptr, u_ptr, v_ptr,
                                                   output, pitch,
                                                   width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_YV12)
aipl_error_t aipl_color_convert_yuy2_to_yv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + yuv_size / 4;

    return aipl_color_convert_yuv_packed_to_planar(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_I420)
aipl_error_t aipl_color_convert_yuy2_to_i420(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + yuv_size / 4;

    return aipl_color_convert_yuv_packed_to_planar(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_I422)
aipl_error_t aipl_color_convert_yuy2_to_i422(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + 1;
    const uint8_t* v_src_ptr = u_src_ptr + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch * 2;
        uint8_t* y_dst = y_dst_ptr + i * width;

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(y_dst, y, tail_p);

            y_src += 32;
            y_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 2)
        {
            y_dst[j / 2] = y_src[j];
        }
#endif

        const uint8_t* u_src = u_src_ptr + i * pitch * 2;
        uint8_t* u_dst = u_dst_ptr + i * width / 2;
        const uint8_t* v_src = v_src_ptr + i * pitch * 2;
        uint8_t* v_dst = v_dst_ptr + i * width / 2;

#ifdef AIPL_HELIUM_ACCELERATION
        cnt = width / 2;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(u_dst, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(v_dst, v, tail_p);

            u_src += 64;
            v_src += 64;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            u_dst[j / 4] = u_src[j];
            v_dst[j / 4] = v_src[j];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yuy2_to_i400(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_yuv_packed_to_alpha8(input, output, pitch,
                                                   width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_NV12)
aipl_error_t aipl_color_convert_yuy2_to_nv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + 1;

    return aipl_color_convert_yuv_packed_to_semi(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_I444)
aipl_error_t aipl_color_convert_yuy2_to_i444(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_src_ptr = input;
    const uint8_t* u_src_ptr = y_src_ptr + 1;
    const uint8_t* v_src_ptr = u_src_ptr + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch * 2;
        uint8_t* y_dst = y_dst_ptr + i * width;

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(y_dst, y, tail_p);

            y_src += 32;
            y_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 2)
        {
            y_dst[j / 2] = y_src[j];
        }
#endif

        const uint8_t* u_src = u_src_ptr + i * pitch * 2;
        uint8_t* u_dst = u_dst_ptr + i * width;
        const uint8_t* v_src = v_src_ptr + i * pitch * 2;
        uint8_t* v_dst = v_dst_ptr + i * width;

#ifdef AIPL_HELIUM_ACCELERATION
        cnt = width / 2;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
            vstrbq_scatter_offset_p(u_dst + 1, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
            vstrbq_scatter_offset_p(v_dst + 1, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

            u_src += 64;
            v_src += 64;
            u_dst += 32;
            v_dst += 32;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            u_dst[j / 2] = u_dst[j / 2 + 1] = u_src[j];
            v_dst[j / 2] = v_dst[j / 2 + 1] = v_src[j];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_NV21)
aipl_error_t aipl_color_convert_yuy2_to_nv21(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + 1;

    return aipl_color_convert_yuv_packed_to_semi(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_UYVY)
aipl_error_t aipl_color_convert_yuy2_to_uyvy(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* y_src = input;
    const uint8_t* u_src = y_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint8_t* u_dst = output;
    uint8_t* y_dst = u_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_packed_to_packed(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif
#endif

#if AIPL_CONVERT_UYVY
aipl_error_t aipl_color_convert_uyvy(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
#if (AIPL_CONVERT_UYVY & TO_ALPHA8_I400)
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_uyvy_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* RGB color formats */
#if (AIPL_CONVERT_UYVY & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_uyvy_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_uyvy_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_uyvy_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_uyvy_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_uyvy_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_uyvy_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_uyvy_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_uyvy_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_uyvy_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_UYVY & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_uyvy_to_yv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_uyvy_to_i420(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_uyvy_to_nv21(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_uyvy_to_nv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_uyvy_to_i422(input, output,
                                                   pitch,
                                                   width, height);
#endif
        case AIPL_COLOR_UYVY:
            return AIPL_ERR_FORMAT_MISMATCH;
#if (AIPL_CONVERT_UYVY & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_uyvy_to_yuy2(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_uyvy_to_i444(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_UYVY & TO_ALPHA8_I400)
        case AIPL_COLOR_I400:
            return aipl_color_convert_uyvy_to_i400(input, output,
                                                   pitch,
                                                   width, height);
#endif

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

#if (AIPL_CONVERT_UYVY & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_uyvy_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    const uint8_t* y_ptr = input + 1;

    return aipl_color_convert_yuv_packed_to_alpha8(y_ptr, output, pitch,
                                                   width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_ARGB8888)
aipl_error_t aipl_color_convert_uyvy_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_argb8888(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_ARGB4444)
aipl_error_t aipl_color_convert_uyvy_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_argb4444(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_ARGB1555)
aipl_error_t aipl_color_convert_uyvy_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_argb1555(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_RGBA8888)
aipl_error_t aipl_color_convert_uyvy_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_rgba8888(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_RGBA4444)
aipl_error_t aipl_color_convert_uyvy_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_rgba4444(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_RGBA5551)
aipl_error_t aipl_color_convert_uyvy_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_rgba5551(y_ptr, u_ptr, v_ptr,
                                                     output, pitch,
                                                     width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_BGR888)
aipl_error_t aipl_color_convert_uyvy_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_24bit(y_ptr, u_ptr, v_ptr,
                                                  output, pitch,
                                                  width, height,
                                                  2, 1, 0);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_RGB888)
aipl_error_t aipl_color_convert_uyvy_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_24bit(y_ptr, u_ptr, v_ptr,
                                                  output, pitch,
                                                  width, height,
                                                  0, 1, 2);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_RGB565)
aipl_error_t aipl_color_convert_uyvy_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    uint32_t yuv_size = pitch * height;
    const uint8_t* u_ptr = input;
    const uint8_t* y_ptr = u_ptr + 1;
    const uint8_t* v_ptr = u_ptr + 2;

    return aipl_color_convert_yuv_packed_to_rgb565(y_ptr, u_ptr, v_ptr,
                                                   output, pitch,
                                                   width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_YV12)
aipl_error_t aipl_color_convert_uyvy_to_yv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* u_src = input;
    const uint8_t* y_src = u_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + yuv_size / 4;

    return aipl_color_convert_yuv_packed_to_planar(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_I420)
aipl_error_t aipl_color_convert_uyvy_to_i420(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* u_src = input;
    const uint8_t* y_src = u_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + yuv_size / 4;

    return aipl_color_convert_yuv_packed_to_planar(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_I422)
aipl_error_t aipl_color_convert_uyvy_to_i422(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* u_src_ptr = input;
    const uint8_t* y_src_ptr = u_src_ptr + 1;
    const uint8_t* v_src_ptr = u_src_ptr + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch * 2;
        uint8_t* y_dst = y_dst_ptr + i * width;

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(y_dst, y, tail_p);

            y_src += 32;
            y_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 2)
        {
            y_dst[j / 2] = y_src[j];
        }
#endif

        const uint8_t* u_src = u_src_ptr + i * pitch * 2;
        uint8_t* u_dst = u_dst_ptr + i * width / 2;
        const uint8_t* v_src = v_src_ptr + i * pitch * 2;
        uint8_t* v_dst = v_dst_ptr + i * width / 2;

#ifdef AIPL_HELIUM_ACCELERATION
        cnt = width / 2;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(u_dst, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(v_dst, v, tail_p);

            u_src += 64;
            v_src += 64;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            u_dst[j / 4] = u_src[j];
            v_dst[j / 4] = v_src[j];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_UYVY & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_uyvy_to_i400(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* y_ptr = input + 1;

    return aipl_color_convert_yuv_packed_to_alpha8(y_ptr, output, pitch,
                                                   width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_NV12)
aipl_error_t aipl_color_convert_uyvy_to_nv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* u_src = input;
    const uint8_t* y_src = u_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + yuv_size;
    uint8_t* v_dst = u_dst + 1;

    return aipl_color_convert_yuv_packed_to_semi(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_I444)
aipl_error_t aipl_color_convert_uyvy_to_i444(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* u_src_ptr = input;
    const uint8_t* y_src_ptr = u_src_ptr + 1;
    const uint8_t* v_src_ptr = u_src_ptr + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch * 2;
        uint8_t* y_dst = y_dst_ptr + i * width;

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(y_dst, y, tail_p);

            y_src += 32;
            y_dst += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 2)
        {
            y_dst[j / 2] = y_src[j];
        }
#endif

        const uint8_t* u_src = u_src_ptr + i * pitch * 2;
        uint8_t* u_dst = u_dst_ptr + i * width;
        const uint8_t* v_src = v_src_ptr + i * pitch * 2;
        uint8_t* v_dst = v_dst_ptr + i * width;

#ifdef AIPL_HELIUM_ACCELERATION
        cnt = width / 2;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
            vstrbq_scatter_offset_p(u_dst + 1, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
            vstrbq_scatter_offset_p(v_dst + 1, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

            u_src += 64;
            v_src += 64;
            u_dst += 32;
            v_dst += 32;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            u_dst[j / 2] = u_dst[j / 2 + 1] = u_src[j];
            v_dst[j / 2] = v_dst[j / 2 + 1] = v_src[j];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_UYVY & TO_NV21)
aipl_error_t aipl_color_convert_uyvy_to_nv21(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* u_src = input;
    const uint8_t* y_src = u_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint32_t yuv_size = width * height;
    uint8_t* y_dst = output;
    uint8_t* v_dst = y_dst + yuv_size;
    uint8_t* u_dst = v_dst + 1;

    return aipl_color_convert_yuv_packed_to_semi(y_src, u_src, v_src,
                                                 y_dst, u_dst, v_dst,
                                                 pitch, width, height);
}
#endif

#if (AIPL_CONVERT_UYVY & TO_YUY2)
aipl_error_t aipl_color_convert_uyvy_to_yuy2(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* u_src = input;
    const uint8_t* y_src = u_src + 1;
    const uint8_t* v_src = u_src + 2;

    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_yuv_packed_to_packed(y_src, u_src, v_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif
#endif

#if (AIPL_CONVERT_ALPHA8_I400)
aipl_error_t aipl_color_convert_i400(const void* input, void* output,
                                     uint32_t pitch,
                                     uint32_t width, uint32_t height,
                                     aipl_color_format_t format)
{
    switch (format)
    {
        /* Alpha color formats */
        case AIPL_COLOR_ALPHA8:
            return aipl_color_convert_i400_to_alpha8(input, output,
                                                     pitch,
                                                     width, height);
        /* RGB color formats */
#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB8888)
        case AIPL_COLOR_ARGB8888:
            return aipl_color_convert_i400_to_argb8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA8888)
        case AIPL_COLOR_RGBA8888:
            return aipl_color_convert_i400_to_rgba8888(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB4444)
        case AIPL_COLOR_ARGB4444:
            return aipl_color_convert_i400_to_argb4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB1555)
        case AIPL_COLOR_ARGB1555:
            return aipl_color_convert_i400_to_argb1555(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA4444)
        case AIPL_COLOR_RGBA4444:
            return aipl_color_convert_i400_to_rgba4444(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA5551)
        case AIPL_COLOR_RGBA5551:
            return aipl_color_convert_i400_to_rgba5551(input, output,
                                                       pitch,
                                                       width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGB565)
        case AIPL_COLOR_RGB565:
            return aipl_color_convert_i400_to_rgb565(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_BGR888)
        case AIPL_COLOR_BGR888:
            return aipl_color_convert_i400_to_bgr888(input, output,
                                                     pitch,
                                                     width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGB888)
        case AIPL_COLOR_RGB888:
            return aipl_color_convert_i400_to_rgb888(input, output,
                                                     pitch,
                                                     width, height);
#endif
        /* YUV color formats */
#if (AIPL_CONVERT_ALPHA8_I400 & TO_YV12)
        case AIPL_COLOR_YV12:
            return aipl_color_convert_i400_to_yv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_I420)
        case AIPL_COLOR_I420:
            return aipl_color_convert_i400_to_i420(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_NV21)
        case AIPL_COLOR_NV21:
            return aipl_color_convert_i400_to_nv21(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_NV12)
        case AIPL_COLOR_NV12:
            return aipl_color_convert_i400_to_nv12(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_I422)
        case AIPL_COLOR_I422:
            return aipl_color_convert_i400_to_i422(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_UYVY)
        case AIPL_COLOR_UYVY:
            return aipl_color_convert_i400_to_uyvy(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_YUY2)
        case AIPL_COLOR_YUY2:
            return aipl_color_convert_i400_to_yuy2(input, output,
                                                   pitch,
                                                   width, height);
#endif
#if (AIPL_CONVERT_ALPHA8_I400 & TO_I444)
        case AIPL_COLOR_I444:
            return aipl_color_convert_i400_to_i444(input, output,
                                                   pitch,
                                                   width, height);
#endif
        case AIPL_COLOR_I400:
            return AIPL_ERR_FORMAT_MISMATCH;

        default:
            return AIPL_ERR_UNSUPPORTED_FORMAT;
    }
}

aipl_error_t aipl_color_convert_i400_to_alpha8(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        memcpy(dst, y_src, width);
    }

    return AIPL_ERR_OK;
}

#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB8888)
aipl_error_t aipl_color_convert_i400_to_argb8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_alpha8, d2_mode_argb8888);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t y;
            aipl_mve_ldr_4px_i400(&y, y_src, tail_p);

            aipl_mve_cnvt_4px_i400_to_argb8888(&y, y);

            aipl_mve_str_4px_argb8888(dst, y, tail_p);

            y_src += 4;
            dst += 4;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_i400_to_argb8888(dst, y_src);

            ++y_src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB4444)
aipl_error_t aipl_color_convert_i400_to_argb4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_alpha8, d2_mode_argb4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y;
            aipl_mve_ldr_8px_i400(&y, y_src, tail_p);

            aipl_mve_cnvt_8px_i400_to_argb4444(&y, y);

            aipl_mve_str_8px_argb4444(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_i400_to_argb4444(dst, y_src);

            ++y_src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_ARGB1555)
aipl_error_t aipl_color_convert_i400_to_argb1555(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_alpha8, d2_mode_argb1555);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y;
            aipl_mve_ldr_8px_i400(&y, y_src, tail_p);

            aipl_mve_cnvt_8px_i400_to_argb1555(&y, y);

            aipl_mve_str_8px_argb1555(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_i400_to_argb1555(dst, y_src);

            ++y_src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA8888)
aipl_error_t aipl_color_convert_i400_to_rgba8888(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_alpha8, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t y;
            aipl_mve_ldr_4px_i400(&y, y_src, tail_p);

            aipl_mve_cnvt_4px_i400_to_rgba8888(&y, y);

            aipl_mve_str_4px_rgba8888(dst, y, tail_p);

            y_src += 4;
            dst += 4;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_i400_to_rgba8888(dst, y_src);

            ++y_src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA4444)
aipl_error_t aipl_color_convert_i400_to_rgba4444(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_alpha8, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y;
            aipl_mve_ldr_8px_i400(&y, y_src, tail_p);

            aipl_mve_cnvt_8px_i400_to_rgba4444(&y, y);

            aipl_mve_str_8px_rgba4444(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_i400_to_rgba4444(dst, y_src);

            ++y_src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGBA5551)
aipl_error_t aipl_color_convert_i400_to_rgba5551(const void* input,
                                                 void* output,
                                                 uint32_t pitch,
                                                 uint32_t width,
                                                 uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_alpha8, d2_mode_rgba5551);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y;
            aipl_mve_ldr_8px_i400(&y, y_src, tail_p);

            aipl_mve_cnvt_8px_i400_to_rgba5551(&y, y);

            aipl_mve_str_8px_rgba5551(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_i400_to_rgba5551(dst, y_src);

            ++y_src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_BGR888)
aipl_error_t aipl_color_convert_i400_to_bgr888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_alpha8_to_24bit(input, output, pitch,
                                              width, height,
                                              2, 1, 0);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGB888)
aipl_error_t aipl_color_convert_i400_to_rgb888(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_alpha8_to_24bit(input, output, pitch,
                                              width, height,
                                              0, 1, 2);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_RGB565)
aipl_error_t aipl_color_convert_i400_to_rgb565(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;

#if (defined(AIPL_DAVE2D_ACCELERATION) && defined(AIPL_OPTIMIZE_CPU_LOAD))
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_alpha8, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y;
            aipl_mve_ldr_8px_i400(&y, y_src, tail_p);

            aipl_mve_cnvt_8px_i400_to_rgb565(&y, y);

            aipl_mve_str_8px_rgb565(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_i400_to_rgb565(dst, y_src);

            ++y_src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_YV12)
aipl_error_t aipl_color_convert_i400_to_yv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_alpha8_to_yuv_planar(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_I420)
aipl_error_t aipl_color_convert_i400_to_i420(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_alpha8_to_yuv_planar(input, output,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_I422)
aipl_error_t aipl_color_convert_i400_to_i422(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_src_ptr = input;

    uint32_t yuv_size = pitch * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size / 2;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        uint8_t* v_dst = v_dst_ptr + i * width / 2;
        uint8_t* u_dst = u_dst_ptr + i * width / 2;

        memset(u_dst, 0x80, width / 2);
        memset(v_dst, 0x80, width / 2);
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_NV12)
aipl_error_t aipl_color_convert_i400_to_nv12(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_alpha8_to_yuv_semi_planar(input, output,
                                                        pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_I444)
aipl_error_t aipl_color_convert_i400_to_i444(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_src_ptr = input;

    uint32_t yuv_size = pitch * height;
    uint8_t* y_dst_ptr = output;
    uint8_t* u_dst_ptr = y_dst_ptr + yuv_size;
    uint8_t* v_dst_ptr = u_dst_ptr + yuv_size;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_src_ptr + i * pitch;
        uint8_t* y_dst = y_dst_ptr + i * width;

        memcpy(y_dst, y_src, width);

        uint8_t* v_dst = v_dst_ptr + i * width;
        uint8_t* u_dst = u_dst_ptr + i * width;

        memset(u_dst, 0x80, width);
        memset(v_dst, 0x80, width);
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_NV21)
aipl_error_t aipl_color_convert_i400_to_nv21(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    return aipl_color_convert_alpha8_to_yuv_semi_planar(input, output,
                                                        pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_YUY2)
aipl_error_t aipl_color_convert_i400_to_yuy2(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* y_src = input;

    uint8_t* y_dst = output;
    uint8_t* u_dst = y_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_alpha8_to_yuv_packed(y_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & TO_UYVY)
aipl_error_t aipl_color_convert_i400_to_uyvy(const void* input,
                                             void* output,
                                             uint32_t pitch,
                                             uint32_t width,
                                             uint32_t height)
{
    const uint8_t* y_src = input;

    uint8_t* u_dst = output;
    uint8_t* y_dst = u_dst + 1;
    uint8_t* v_dst = u_dst + 2;

    return aipl_color_convert_alpha8_to_yuv_packed(y_src,
                                                   y_dst, u_dst, v_dst,
                                                   pitch, width, height);
}
#endif

#endif


/**********************
 *   STATIC FUNCTIONS
 **********************/
#if (AIPL_CONVERT_ALPHA8_I400 & TO_BGR888 | AIPL_CONVERT_ALPHA8_I400 & TO_RGB888)
aipl_error_t aipl_color_convert_alpha8_to_24bit(const void* input,
                                                void* output,
                                                uint32_t pitch,
                                                uint32_t width,
                                                uint32_t height,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;

#ifdef AIPL_HELIUM_ACCELERATION
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y;
            aipl_mve_ldr_16px_i400(&y, y_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_cnvt_16px_i400_to_rgb(&pix, y);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            y_src += 16;
            dst += 48;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_i400_to_24bit(dst, y_src);

            ++y_src;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_BGR888 | AIPL_CONVERT_ARGB8888 & TO_RGB888)
aipl_error_t aipl_color_convert_argb8888_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_xrgb8888(&pix, src, tail_p);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            src += 64;
            dst += 48;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_24bit(dst, src,
                                           r_offset, g_offset, b_offset);

            ++src;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif
#if (AIPL_CONVERT_ARGB4444 & TO_BGR888 | AIPL_CONVERT_ARGB4444 & TO_RGB888)
aipl_error_t aipl_color_convert_argb4444_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_xrgb4444(&pix, src, tail_p);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            src += 16;
            dst += 48;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_24bit(dst, src,
                                           r_offset, g_offset, b_offset);

            ++src;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif
#if (AIPL_CONVERT_ARGB1555 & TO_BGR888 | AIPL_CONVERT_ARGB1555 & TO_RGB888)
aipl_error_t aipl_color_convert_argb1555_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_xrgb1555(&pix, src, tail_p);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            src += 16;
            dst += 48;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_24bit(dst, src,
                                           r_offset, g_offset, b_offset);

            ++src;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_BGR888 | AIPL_CONVERT_RGBA8888 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba8888_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgbx8888(&pix, src, tail_p);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            src += 64;
            dst += 48;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_24bit(dst, src, r_offset, g_offset, b_offset);

            ++src;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_BGR888 | AIPL_CONVERT_RGBA4444 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba4444_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgbx4444(&pix, src, tail_p);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            src += 16;
            dst += 48;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_24bit(dst, src,
                                           r_offset, g_offset, b_offset);

            ++src;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_BGR888 | AIPL_CONVERT_RGBA5551 & TO_RGB888)
aipl_error_t aipl_color_convert_rgba5551_to_24bit(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgbx5551(&pix, src, tail_p);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            src += 16;
            dst += 48;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_24bit(dst, src,
                                           r_offset, g_offset, b_offset);

            ++src;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_BGR888 | AIPL_CONVERT_RGB565 & TO_RGB888)
aipl_error_t aipl_color_convert_rgb565_to_24bit(const void* input,
                                                void* output,
                                                uint32_t pitch,
                                                uint32_t width,
                                                uint32_t height,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb565(&pix, src, tail_p);

            aipl_mve_str_16px_rgb(dst, pix, tail_p,
                                  r_offset, g_offset, b_offset);

            src += 16;
            dst += 48;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_24bit(dst, src,
                                         r_offset, g_offset, b_offset);

            ++src;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_BGR888 | AIPL_CONVERT_BGR888 & TO_RGB888)
aipl_error_t aipl_color_convert_24bit_to_24bit(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height,
                                               uint8_t r_in_offset,
                                               uint8_t g_in_offset,
                                               uint8_t b_in_offset,
                                               uint8_t r_out_offset,
                                               uint8_t g_out_offset,
                                               uint8_t b_out_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * width * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_in_offset, g_in_offset, b_in_offset);

            aipl_mve_str_16px_rgb((uint8_t*)dst, pix, tail_p,
                                  r_out_offset, g_out_offset, b_out_offset);

            src += 48;
            dst += 48;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_24bit(dst, src,
                                        r_in_offset, g_in_offset, b_in_offset,
                                        r_out_offset, g_out_offset, b_out_offset);

            src += 3;
            dst += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_ALPHA8_I400 | AIPL_CONVERT_BGR888 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_24bit_to_alpha8(const void* input,
                                                void* output,
                                                uint32_t pitch,
                                                uint32_t width,
                                                uint32_t height,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgb(&pix, src, tail_p,
                                 r_offset, g_offset, b_offset);

            uint16x8_t alpha;
            aipl_mve_cnvt_8px_rgb_to_yuv_y(&alpha, pix);

            vstrbq_p(dst, alpha, tail_p);

            src += 24;
            dst += 8;
        }
    }
#else
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_yuv_y(dst, src,
                                        r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_ARGB8888 | AIPL_CONVERT_BGR888 & TO_ARGB8888)
aipl_error_t aipl_color_convert_24bit_to_argb8888(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * width * 4;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_offset, g_offset, b_offset);

            aipl_mve_str_16px_xrgb8888(dst, pix, tail_p);

            src += 48;
            dst += 64;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_argb8888(dst, src,
                                           r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_ARGB4444 | AIPL_CONVERT_BGR888 & TO_ARGB4444)
aipl_error_t aipl_color_convert_24bit_to_argb4444(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_offset, g_offset, b_offset);

            aipl_mve_str_16px_xrgb4444(dst, pix, tail_p);

            src += 48;
            dst += 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_argb4444(dst, src,
                                           r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_ARGB1555 | AIPL_CONVERT_BGR888 & TO_ARGB1555)
aipl_error_t aipl_color_convert_24bit_to_argb1555(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{

    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_offset, g_offset, b_offset);

            aipl_mve_str_16px_xrgb1555(dst, pix, tail_p);

            src += 48;
            dst += 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_argb1555(dst, src,
                                           r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_RGBA8888 | AIPL_CONVERT_BGR888 & TO_RGBA8888)
aipl_error_t aipl_color_convert_24bit_to_rgba8888(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * width * 4;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_offset, g_offset, b_offset);

            aipl_mve_str_16px_rgbx8888(dst, pix, tail_p);

            src += 48;
            dst += 64;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_rgba8888(dst, src,
                                           r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_RGBA4444 | AIPL_CONVERT_BGR888 & TO_RGBA4444)
aipl_error_t aipl_color_convert_24bit_to_rgba4444(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_offset, g_offset, b_offset);

            aipl_mve_str_16px_rgbx4444(dst, pix, tail_p);

            src += 48;
            dst += 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_rgba4444(dst, src,
                                           r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_RGBA5551 | AIPL_CONVERT_BGR888 & TO_RGBA5551)
aipl_error_t aipl_color_convert_24bit_to_rgba5551(const void* input,
                                                  void* output,
                                                  uint32_t pitch,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint8_t r_offset,
                                                  uint8_t g_offset,
                                                  uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_offset, g_offset, b_offset);

            aipl_mve_str_16px_rgbx5551(dst, pix, tail_p);

            src += 48;
            dst += 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_rgba5551(dst, src,
                                           r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_RGB565 | AIPL_CONVERT_BGR888 & TO_RGB565)
aipl_error_t aipl_color_convert_24bit_to_rgb565(const void* input,
                                                void* output,
                                                uint32_t pitch,
                                                uint32_t width,
                                                uint32_t height,
                                                uint8_t r_offset,
                                                uint8_t g_offset,
                                                uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_ldr_16px_rgb(&pix, src, tail_p,
                                  r_offset, g_offset, b_offset);

            aipl_mve_str_16px_rgb565(dst, pix, tail_p);

            src += 48;
            dst += 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_rgb565(dst, src,
                                         r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_I422 | AIPL_CONVERT_BGR888 & TO_I422) \
    && defined (AIPL_HELIUM_ACCELERATION)
aipl_error_t aipl_color_convert_24bit_to_i422(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgb(&pix, src, tail_p,
                                 r_offset, g_offset, b_offset);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 24;
            y_dst += 8;
        }

        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_rgb(&pix, src, 2, tail_p,
                                        r_offset, g_offset, b_offset);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgb_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb_to_yuv_v(&v, pix);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 48;
            u_dst += 8;
            v_dst += 8;
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_I444 | AIPL_CONVERT_BGR888 & TO_I444) \
    && defined(AIPL_HELIUM_ACCELERATION)
aipl_error_t aipl_color_convert_24bit_to_i444(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset)
{
    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgb(&pix, src, tail_p,
                                 r_offset, g_offset, b_offset);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgb_to_yuv_u(&u, pix);

            vstrbq_p(u_dst, u, tail_p);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb_to_yuv_v(&v, pix);

            vstrbq_p(v_dst, v, tail_p);

            src += 24;
            y_dst += 8;
            u_dst += 8;
            v_dst += 8;
        }
    }

    return AIPL_ERR_OK;
}
#endif


#if (AIPL_CONVERT_ARGB8888 & TO_YV12 | AIPL_CONVERT_ARGB8888 & TO_I420)
aipl_error_t aipl_color_convert_argb8888_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_xrgb8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 32;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_xrgb8888(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_v(&v, pix);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 64;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_argb8888_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_NV12 | AIPL_CONVERT_ARGB8888 & TO_NV21)
aipl_error_t aipl_color_convert_argb8888_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_xrgb8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 32;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_xrgb8888(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_v(&v, pix);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U16, v, tail_p);

            src += 64;
            u_dst += 16;
            v_dst += 16;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb8888_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_argb8888_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            u_dst += 2;
            v_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB8888 & TO_YUY2 | AIPL_CONVERT_ARGB8888 & TO_UYVY)
aipl_error_t aipl_color_convert_argb8888_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* y_dst = y_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_xrgb8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_y(&y, pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U16, y, tail_p);

            src += 32;
            y_dst += 16;
        }

        src = src_ptr + i * pitch * 4;
        uint8_t* u_dst = u_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_xrgb8888(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_xrgb8888_to_yuv_v(&v, pix);

            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U16, v, tail_p);

            src += 64;
            u_dst += 32;
            v_dst += 32;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_cnvt_px_argb8888_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_argb8888_to_yuv_y(y_dst, src);

            ++src;
            y_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_YV12 | AIPL_CONVERT_ARGB4444 & TO_I420)
aipl_error_t aipl_color_convert_argb4444_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb4444_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_argb4444(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb4444_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_argb4444_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_NV12 | AIPL_CONVERT_ARGB4444 & TO_NV21)
aipl_error_t aipl_color_convert_argb4444_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb4444_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_argb4444(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb4444_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 16;
            v_dst += 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb4444_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_argb4444_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            u_dst += 2;
            v_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB4444 & TO_YUY2 | AIPL_CONVERT_ARGB4444 & TO_UYVY)
aipl_error_t aipl_color_convert_argb4444_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb4444_to_yuv_y(&y, px);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U16, y, tail_p);

            src += 8;
            y_dst += 16;
        }

        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_argb4444(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb4444_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 32;
            v_dst += 32;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            aipl_cnvt_px_argb4444_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_argb4444_to_yuv_y(y_dst, src);

            ++src;
            y_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_YV12 | AIPL_CONVERT_ARGB1555 & TO_I420)
aipl_error_t aipl_color_convert_argb1555_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb1555(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb1555_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_argb1555(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb1555_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_argb1555_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_NV12 | AIPL_CONVERT_ARGB1555 & TO_NV21)
aipl_error_t aipl_color_convert_argb1555_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb1555(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb1555_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_argb1555(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb1555_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 16;
            v_dst += 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_argb1555_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_argb1555_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            u_dst += 2;
            v_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ARGB1555 & TO_YUY2 | AIPL_CONVERT_ARGB1555 & TO_UYVY)
aipl_error_t aipl_color_convert_argb1555_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_argb1555(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_argb1555_to_yuv_y(&y, px);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U16, y, tail_p);

            src += 8;
            y_dst += 16;
        }

        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_argb1555(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_argb1555_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 32;
            v_dst += 32;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            aipl_cnvt_px_argb1555_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_argb1555_to_yuv_y(y_dst, src);

            ++src;
            y_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_YV12 | AIPL_CONVERT_RGBA8888 & TO_I420)
aipl_error_t aipl_color_convert_rgba8888_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgbx8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 32;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_rgbx8888(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_v(&v, pix);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 64;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgba8888_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_NV12 | AIPL_CONVERT_RGBA8888 & TO_NV21)
aipl_error_t aipl_color_convert_rgba8888_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgbx8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 32;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_rgbx8888(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_v(&v, pix);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U16, v, tail_p);

            src += 64;
            u_dst += 16;
            v_dst += 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba8888_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgba8888_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            u_dst += 2;
            v_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA8888 & TO_YUY2 | AIPL_CONVERT_RGBA8888 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba8888_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* y_dst = y_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgbx8888(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_y(&y, pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U16, y, tail_p);

            src += 32;
            y_dst += 16;
        }

        uint8_t* u_dst = u_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_rgbx8888(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgbx8888_to_yuv_v(&v, pix);

            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U16, v, tail_p);

            src += 64;
            u_dst += 32;
            v_dst += 32;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_cnvt_px_rgba8888_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_rgba8888_to_yuv_y(y_dst, src);

            ++src;
            y_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_YV12 | AIPL_CONVERT_RGBA4444 & TO_I420)
aipl_error_t aipl_color_convert_rgba4444_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgba4444(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgba4444_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_NV12 | AIPL_CONVERT_RGBA4444 & TO_NV21)
aipl_error_t aipl_color_convert_rgba4444_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgba4444(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 16;
            v_dst += 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba4444_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgba4444_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            u_dst += 2;
            v_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA4444 & TO_YUY2 | AIPL_CONVERT_RGBA4444 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba4444_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba4444(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_y(&y, px);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U16, y, tail_p);

            src += 8;
            y_dst += 16;
        }

        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgba4444(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba4444_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 32;
            v_dst += 32;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            aipl_cnvt_px_rgba4444_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_rgba4444_to_yuv_y(y_dst, src);

            ++src;
            y_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_YV12 | AIPL_CONVERT_RGBA5551 & TO_I420)
aipl_error_t aipl_color_convert_rgba5551_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba5551(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgba5551(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgba5551_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_NV12 | AIPL_CONVERT_RGBA5551 & TO_NV21)
aipl_error_t aipl_color_convert_rgba5551_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba5551(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgba5551(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 16;
            v_dst += 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgba5551_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgba5551_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            u_dst += 2;
            v_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGBA5551 & TO_YUY2 | AIPL_CONVERT_RGBA5551 & TO_UYVY)
aipl_error_t aipl_color_convert_rgba5551_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgba5551(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_y(&y, px);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U16, y, tail_p);

            src += 8;
            y_dst += 16;
        }

        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgba5551(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgba5551_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 32;
            v_dst += 32;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            aipl_cnvt_px_rgba5551_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_rgba5551_to_yuv_y(y_dst, src);

            ++src;
            y_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_YV12 | AIPL_CONVERT_BGR888 & TO_I420\
     | AIPL_CONVERT_RGB888 & TO_YV12 | AIPL_CONVERT_RGB888 & TO_I420)
aipl_error_t aipl_color_convert_24bit_to_yuv_planar(const void* input,
                                                    uint32_t pitch,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint8_t* y_ptr,
                                                    uint8_t* u_ptr,
                                                    uint8_t* v_ptr,
                                                    uint8_t r_offset,
                                                    uint8_t g_offset,
                                                    uint8_t b_offset)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgb(&pix, src, tail_p,
                                 r_offset, g_offset, b_offset);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 24;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_rgb(&pix, src, 2, tail_p,
                                        r_offset, g_offset, b_offset);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgb_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb_to_yuv_v(&v, pix);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 48;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_yuv_y(y_dst, src, r_offset, g_offset, b_offset);

            src += 3;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_24bit_to_yuv_uv(u_dst, v_dst, src, r_offset, g_offset, b_offset);

            src += 6;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_NV12 | AIPL_CONVERT_BGR888 & TO_NV21\
     | AIPL_CONVERT_RGB888 & TO_NV12 | AIPL_CONVERT_RGB888 & TO_NV21)
aipl_error_t aipl_color_convert_24bit_to_yuv_semi_planar(const void* input,
                                                         uint32_t pitch,
                                                         uint32_t width,
                                                         uint32_t height,
                                                         uint8_t* y_ptr,
                                                         uint8_t* u_ptr,
                                                         uint8_t* v_ptr,
                                                         uint8_t r_offset,
                                                         uint8_t g_offset,
                                                         uint8_t b_offset)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgb(&pix, src, tail_p,
                                 r_offset, g_offset, b_offset);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb_to_yuv_y(&y, pix);

            vstrbq_p(y_dst, y, tail_p);

            src += 24;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_rgb(&pix, src, 2, tail_p,
                                        r_offset, g_offset, b_offset);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgb_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb_to_yuv_v(&v, pix);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U16, v, tail_p);

            src += 48;
            u_dst += 16;
            v_dst += 16;
        }
    }
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_24bit_to_yuv_y(y_dst, src, r_offset, g_offset, b_offset);

            src += 3;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_24bit_to_yuv_uv(u_dst, v_dst, src, r_offset, g_offset, b_offset);

            src += 6;
            u_dst += 2;
            v_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_YUY2 | AIPL_CONVERT_BGR888 & TO_UYVY\
     | AIPL_CONVERT_RGB888 & TO_YUY2 | AIPL_CONVERT_RGB888 & TO_UYVY)\
    && defined(AIPL_HELIUM_ACCELERATION)
aipl_error_t aipl_color_convert_24bit_to_yuv_packed(const void* input,
                                                    uint32_t pitch,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint8_t* y_ptr,
                                                    uint8_t* u_ptr,
                                                    uint8_t* v_ptr,
                                                    uint8_t r_offset,
                                                    uint8_t g_offset,
                                                    uint8_t b_offset)
{
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* y_dst = y_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_rgb(&pix, src, tail_p,
                                 r_offset, g_offset, b_offset);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb_to_yuv_y(&y, pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U16, y, tail_p);

            src += 24;
            y_dst += 16;
        }

        uint8_t* u_dst = u_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_ldr_8px_offset_rgb(&pix, src, 2, tail_p,
                                        r_offset, g_offset, b_offset);

            uint16x8_t u;
            aipl_mve_cnvt_8px_rgb_to_yuv_u(&u, pix);

            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb_to_yuv_v(&v, pix);

            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U16, v, tail_p);

            src += 48;
            u_dst += 32;
            v_dst += 32;
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_YUY2 | AIPL_CONVERT_RGB888 & TO_UYVY)
aipl_error_t aipl_color_convert_rgb888_to_yuv_packed(const void* input,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height,
                                                     uint8_t* y_ptr,
                                                     uint8_t* u_ptr,
                                                     uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    return aipl_color_convert_24bit_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  0, 1, 2);
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_cnvt_px_24bit_to_yuv(y_dst, u_dst, v_dst, src, 0, 1, 2);

            src += 3;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_24bit_to_yuv_y(y_dst, src, 0, 1, 2);

            src += 3;
            y_dst += 2;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_YUY2 | AIPL_CONVERT_BGR888 & TO_UYVY)
aipl_error_t aipl_color_convert_bgr888_to_yuv_packed(const void* input,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height,
                                                     uint8_t* y_ptr,
                                                     uint8_t* u_ptr,
                                                     uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    return aipl_color_convert_24bit_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  2, 1, 0);
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_cnvt_px_24bit_to_yuv(y_dst, u_dst, v_dst, src, 2, 1, 0);

            src += 3;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_24bit_to_yuv_y(y_dst, src, 2, 1, 0);

            src += 3;
            y_dst += 2;
        }
    }

    return AIPL_ERR_OK;
#endif
}
#endif

#if (AIPL_CONVERT_I422 & TO_BGR888 | AIPL_CONVERT_I422 & TO_RGB888)
aipl_error_t aipl_color_convert_i422_to_24bit(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t ur, ug, ub;
            aipl_mve_cnvt_8px_yuv_to_rgb(&ur, &ug, &ub,
                                         c0, r, g, b,
                                         c1, r, g, b);

            vstrbq_scatter_offset_p(dst + r_offset, AIPL_3_BYTE_OFFSETS_U16, ur, tail_p);
            vstrbq_scatter_offset_p(dst + g_offset, AIPL_3_BYTE_OFFSETS_U16, ug, tail_p);
            vstrbq_scatter_offset_p(dst + b_offset, AIPL_3_BYTE_OFFSETS_U16, ub, tail_p);

            y_src += 8;
            u_src += 4;
            v_src += 4;
            dst += 24;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 1]);

            aipl_cnvt_px_yuv_to_24bit(dst + j * 3, c0, r, g, b,
                                      r_offset, g_offset, b_offset);
            aipl_cnvt_px_yuv_to_24bit(dst + (j + 1) * 3, c1, r, g, b,
                                      r_offset, g_offset, b_offset);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_I444 & TO_BGR888 | AIPL_CONVERT_I444 & TO_RGB888)
aipl_error_t aipl_color_convert_i444_to_24bit(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = pitch * height;
    const uint8_t* y_ptr = input;
    const uint8_t* u_ptr = y_ptr + yuv_size;
    const uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t y = vldrbq_z_u32(y_src, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c;

            aipl_mve_pre_cnvt_4px_y(&c, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            int32x4_t r_o = vshrq(vaddq(c, r), 8);
            int32x4_t g_o = vshrq(vaddq(c, g), 8);
            int32x4_t b_o = vshrq(vaddq(c, b), 8);

            uint32x4_t ur = vreinterpretq_u32(vqmovunbq(vdupq_n_u16(0), r_o));
            uint32x4_t ug = vreinterpretq_u32(vqmovunbq(vdupq_n_u16(0), g_o));
            uint32x4_t ub = vreinterpretq_u32(vqmovunbq(vdupq_n_u16(0), b_o));

            vstrbq_scatter_offset_p(dst + r_offset, AIPL_3_BYTE_OFFSETS_U32, ur, tail_p);
            vstrbq_scatter_offset_p(dst + g_offset, AIPL_3_BYTE_OFFSETS_U32, ug, tail_p);
            vstrbq_scatter_offset_p(dst + b_offset, AIPL_3_BYTE_OFFSETS_U32, ub, tail_p);

            y_src += 4;
            u_src += 4;
            v_src += 4;
            dst += 12;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            int32_t c;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_px_y(&c, y_src[j]);

            aipl_cnvt_px_yuv_to_24bit(dst + j * 3, c, r, g, b,
                                      r_offset, g_offset, b_offset);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_YV12 | AIPL_CONVERT_RGB565 & TO_I420)
aipl_error_t aipl_color_convert_rgb565_to_yuv_planar(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgb565(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb565_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgb565(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb565_to_yuv_uv(&u, &v, px);

            vstrbq_p(u_dst, u, tail_p);
            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            u_dst += 8;
            v_dst += 8;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgb565_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            ++u_dst;
            ++v_dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_NV12 | AIPL_CONVERT_RGB565 & TO_NV21)
aipl_error_t aipl_color_convert_rgb565_to_yuv_semi_planar(const void* input,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint8_t* y_ptr,
                                                            uint8_t* u_ptr,
                                                            uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgb565(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb565_to_yuv_y(&y, px);

            vstrbq_p(y_dst, y, tail_p);

            src += 8;
            y_dst += 8;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgb565(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb565_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 16;
            v_dst += 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_cnvt_px_rgb565_to_yuv_y(y_dst, src);

            ++src;
            ++y_dst;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            aipl_cnvt_px_rgb565_to_yuv_uv(u_dst, v_dst, src);

            src += 2;
            u_dst += 2;
            v_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB565 & TO_YUY2 | AIPL_CONVERT_RGB565 & TO_UYVY)
aipl_error_t aipl_color_convert_rgb565_to_yuv_packed(const void* input,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t* y_ptr,
                                                       uint8_t* u_ptr,
                                                       uint8_t* v_ptr)
{
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* y_dst = y_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_rgb565(&px, src, tail_p);

            uint16x8_t y;
            aipl_mve_cnvt_8px_rgb565_to_yuv_y(&y, px);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U16, y, tail_p);

            src += 8;
            y_dst += 16;
        }

        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t px;
            aipl_mve_ldr_8px_offset_rgb565(&px, src, 2, tail_p);

            uint16x8_t u;
            uint16x8_t v;
            aipl_mve_cnvt_8px_rgb565_to_yuv_uv(&u, &v, px);

            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U16, u, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U16, v, tail_p);

            src += 16;
            u_dst += 32;
            v_dst += 32;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * width * 2;
        uint8_t* v_dst = v_ptr + i * width * 2;
        uint8_t* u_dst = u_ptr + i * width * 2;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            aipl_cnvt_px_rgb565_to_yuv(y_dst, u_dst, v_dst, src);

            ++src;
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_cnvt_px_rgb565_to_yuv_y(y_dst, src);

            ++src;
            y_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ALPHA8_I400 | AIPL_CONVERT_I420 & TO_ALPHA8_I400\
     | AIPL_CONVERT_NV12 & TO_ALPHA8_I400 | AIPL_CONVERT_NV21 & TO_ALPHA8_I400\
     | AIPL_CONVERT_I422 & TO_ALPHA8_I400 | AIPL_CONVERT_I444 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yuv_planar_to_alpha8(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    return aipl_color_convert_i400_to_alpha8(input, output, pitch,
                                             width, height);
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_ALPHA8_I400 | AIPL_CONVERT_UYVY & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yuv_packed_to_alpha8(const void* input,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    const uint8_t* y_ptr = input;
    uint8_t* dst_ptr = output;

#ifdef AIPL_HELIUM_ACCELERATION
    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        uint8_t* dst = dst_ptr + i * width;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(dst, y, tail_p);

            y_src += 32;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        uint8_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width * 2; j += 2)
        {
            dst[j / 2] = y_src[j];
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ARGB8888 | AIPL_CONVERT_I420 & TO_ARGB8888)
aipl_error_t aipl_color_convert_yuv_planar_to_argb8888(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (output == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint32_t* dst0 = dst_ptr + i * width;
        uint32_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px00, px01, px10, px11;
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px00, c00, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px01, c01, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px10, c10, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px11, c11, r, g, b);

            vstrwq_scatter_offset_p(dst0, AIPL_8_BYTE_OFFSETS_U32, px00, tail_p);
            vstrwq_scatter_offset_p(dst0 + 1, AIPL_8_BYTE_OFFSETS_U32, px01, tail_p);
            vstrwq_scatter_offset_p(dst1, AIPL_8_BYTE_OFFSETS_U32, px10, tail_p);
            vstrwq_scatter_offset_p(dst1 + 1, AIPL_8_BYTE_OFFSETS_U32, px11, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 4;
            v_src += 4;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        aipl_argb8888_px_t* dst0 = dst_ptr + i * width;
        aipl_argb8888_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_argb8888(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_argb8888(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_argb8888(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_argb8888(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_ARGB8888 | AIPL_CONVERT_NV21 & TO_ARGB8888)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_argb8888(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height)
{
    if (output == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint32_t* dst0 = dst_ptr + i * width;
        uint32_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_2_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_2_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px00, px01, px10, px11;
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px00, c00, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px01, c01, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px10, c10, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px11, c11, r, g, b);

            vstrwq_scatter_offset_p(dst0, AIPL_8_BYTE_OFFSETS_U32, px00, tail_p);
            vstrwq_scatter_offset_p(dst0 + 1, AIPL_8_BYTE_OFFSETS_U32, px01, tail_p);
            vstrwq_scatter_offset_p(dst1, AIPL_8_BYTE_OFFSETS_U32, px10, tail_p);
            vstrwq_scatter_offset_p(dst1 + 1, AIPL_8_BYTE_OFFSETS_U32, px11, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 8;
            v_src += 8;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        aipl_argb8888_px_t* dst0 = dst_ptr + i * width;
        aipl_argb8888_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_argb8888(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_argb8888(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_argb8888(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_argb8888(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_ARGB8888 | AIPL_CONVERT_UYVY & TO_ARGB8888)
aipl_error_t aipl_color_convert_yuv_packed_to_argb8888(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (output == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U16, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_4_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_4_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px0, px1;
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px0, c0, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_argb8888(&px1, c1, r, g, b);

            vstrwq_scatter_offset_p(dst, AIPL_8_BYTE_OFFSETS_U32, px0, tail_p);
            vstrwq_scatter_offset_p(dst + 1, AIPL_8_BYTE_OFFSETS_U32, px1, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 8;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_argb8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 2]);

            aipl_cnvt_px_yuv_to_argb8888(dst + j2, c0, r, g, b);
            aipl_cnvt_px_yuv_to_argb8888(dst + j2 + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ARGB4444 | AIPL_CONVERT_I420 & TO_ARGB4444)
aipl_error_t aipl_color_convert_yuv_planar_to_argb4444(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_argb4444(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_argb4444(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 4;
            v_src += 4;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        aipl_argb4444_px_t* dst0 = dst_ptr + i * width;
        aipl_argb4444_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_argb4444(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_argb4444(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_argb4444(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_argb4444(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_ARGB4444 | AIPL_CONVERT_NV21 & TO_ARGB4444)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_argb4444(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_2_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_2_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_argb4444(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_argb4444(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 8;
            v_src += 8;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        aipl_argb4444_px_t* dst0 = dst_ptr + i * width;
        aipl_argb4444_px_t* dst1 = dst0 + width;


        for (uint32_t j = 0; j < width; j += 2)
        {
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_argb4444(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_argb4444(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_argb4444(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_argb4444(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_ARGB4444 | AIPL_CONVERT_UYVY & TO_ARGB4444)
aipl_error_t aipl_color_convert_yuv_packed_to_argb4444(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U16, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_4_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_4_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_argb4444(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 8;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_argb4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 2]);

            aipl_cnvt_px_yuv_to_argb4444(dst + j2, c0, r, g, b);
            aipl_cnvt_px_yuv_to_argb4444(dst + j2 + 1, c1, r, g, b);        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ARGB1555 | AIPL_CONVERT_I420 & TO_ARGB1555)
aipl_error_t aipl_color_convert_yuv_planar_to_argb1555(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_argb1555(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_argb1555(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 4;
            v_src += 4;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        aipl_argb1555_px_t* dst0 = dst_ptr + i * width;
        aipl_argb1555_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_argb1555(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_argb1555(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_argb1555(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_argb1555(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_ARGB1555 | AIPL_CONVERT_NV21 & TO_ARGB1555)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_argb1555(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_2_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_2_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_argb1555(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_argb1555(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 8;
            v_src += 8;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        aipl_argb1555_px_t* dst0 = dst_ptr + i * width;
        aipl_argb1555_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_argb1555(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_argb1555(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_argb1555(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_argb1555(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_ARGB1555 | AIPL_CONVERT_UYVY & TO_ARGB1555)
aipl_error_t aipl_color_convert_yuv_packed_to_argb1555(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U16, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_4_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_4_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_argb1555(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 8;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_argb1555_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 2]);

            aipl_cnvt_px_yuv_to_argb1555(dst + j2, c0, r, g, b);
            aipl_cnvt_px_yuv_to_argb1555(dst + j2 + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGBA8888 | AIPL_CONVERT_I420 & TO_RGBA8888)
aipl_error_t aipl_color_convert_yuv_planar_to_rgba8888(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint32_t* dst0 = dst_ptr + i * width;
        uint32_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px00, px01, px10, px11;
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px00, c00, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px01, c01, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px10, c10, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px11, c11, r, g, b);

            vstrwq_scatter_offset_p(dst0, AIPL_8_BYTE_OFFSETS_U32, px00, tail_p);
            vstrwq_scatter_offset_p(dst0 + 1, AIPL_8_BYTE_OFFSETS_U32, px01, tail_p);
            vstrwq_scatter_offset_p(dst1, AIPL_8_BYTE_OFFSETS_U32, px10, tail_p);
            vstrwq_scatter_offset_p(dst1 + 1, AIPL_8_BYTE_OFFSETS_U32, px11, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 4;
            v_src += 4;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        aipl_rgba8888_px_t* dst0 = dst_ptr + i * width;
        aipl_rgba8888_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_rgba8888(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_rgba8888(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_rgba8888(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_rgba8888(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGBA8888 | AIPL_CONVERT_NV21 & TO_RGBA8888)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_rgba8888(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint32_t* dst0 = dst_ptr + i * width;
        uint32_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_2_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_2_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px00, px01, px10, px11;
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px00, c00, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px01, c01, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px10, c10, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px11, c11, r, g, b);

            vstrwq_scatter_offset_p(dst0, AIPL_8_BYTE_OFFSETS_U32, px00, tail_p);
            vstrwq_scatter_offset_p(dst0 + 1, AIPL_8_BYTE_OFFSETS_U32, px01, tail_p);
            vstrwq_scatter_offset_p(dst1, AIPL_8_BYTE_OFFSETS_U32, px10, tail_p);
            vstrwq_scatter_offset_p(dst1 + 1, AIPL_8_BYTE_OFFSETS_U32, px11, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 8;
            v_src += 8;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        aipl_rgba8888_px_t* dst0 = dst_ptr + i * width;
        aipl_rgba8888_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_rgba8888(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_rgba8888(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_rgba8888(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_rgba8888(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGBA8888 | AIPL_CONVERT_UYVY & TO_RGBA8888)
aipl_error_t aipl_color_convert_yuv_packed_to_rgba8888(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        uint32_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U16, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_4_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_4_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint32x4_t px0, px1;
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px0, c0, r, g, b);
            aipl_mve_cnvt_4px_yuv_to_rgba8888(&px1, c1, r, g, b);

            vstrwq_scatter_offset_p(dst, AIPL_8_BYTE_OFFSETS_U32, px0, tail_p);
            vstrwq_scatter_offset_p(dst + 1, AIPL_8_BYTE_OFFSETS_U32, px1, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 8;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_rgba8888_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 2]);

            aipl_cnvt_px_yuv_to_rgba8888(dst + j2, c0, r, g, b);
            aipl_cnvt_px_yuv_to_rgba8888(dst + j2 + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGBA4444 | AIPL_CONVERT_I420 & TO_RGBA4444)
aipl_error_t aipl_color_convert_yuv_planar_to_rgba4444(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_rgba4444(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_rgba4444(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 4;
            v_src += 4;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        aipl_rgba4444_px_t* dst0 = dst_ptr + i * width;
        aipl_rgba4444_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_rgba4444(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_rgba4444(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_rgba4444(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_rgba4444(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGBA4444 | AIPL_CONVERT_NV21 & TO_RGBA4444)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_rgba4444(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_2_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_2_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_rgba4444(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_rgba4444(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 8;
            v_src += 8;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        aipl_rgba4444_px_t* dst0 = dst_ptr + i * width;
        aipl_rgba4444_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_rgba4444(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_rgba4444(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_rgba4444(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_rgba4444(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGBA4444 | AIPL_CONVERT_UYVY & TO_RGBA4444)
aipl_error_t aipl_color_convert_yuv_packed_to_rgba4444(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U16, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_4_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_4_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_rgba4444(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 8;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_rgba4444_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 2]);

            aipl_cnvt_px_yuv_to_rgba4444(dst + j2, c0, r, g, b);
            aipl_cnvt_px_yuv_to_rgba4444(dst + j2 + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGBA5551 | AIPL_CONVERT_I420 & TO_RGBA5551)
aipl_error_t aipl_color_convert_yuv_planar_to_rgba5551(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_rgba5551(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_rgba5551(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 4;
            v_src += 4;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        aipl_rgba5551_px_t* dst0 = dst_ptr + i * width;
        aipl_rgba5551_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_rgba5551(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_rgba5551(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_rgba5551(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_rgba5551(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGBA5551 | AIPL_CONVERT_NV21 & TO_RGBA5551)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_rgba5551(const uint8_t* y_ptr,
                                                            const uint8_t* u_ptr,
                                                            const uint8_t* v_ptr,
                                                            void* output,
                                                            uint32_t pitch,
                                                            uint32_t width,
                                                            uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_2_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_2_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_rgba5551(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_rgba5551(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 8;
            v_src += 8;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        aipl_rgba5551_px_t* dst0 = dst_ptr + i * width;
        aipl_rgba5551_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_rgba5551(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_rgba5551(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_rgba5551(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_rgba5551(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGBA5551 | AIPL_CONVERT_UYVY & TO_RGBA5551)
aipl_error_t aipl_color_convert_yuv_packed_to_rgba5551(const uint8_t* y_ptr,
                                                       const uint8_t* u_ptr,
                                                       const uint8_t* v_ptr,
                                                       void* output,
                                                       uint32_t pitch,
                                                       uint32_t width,
                                                       uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U16, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_4_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_4_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_rgba5551(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 8;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_rgba5551_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 2]);

            aipl_cnvt_px_yuv_to_rgba5551(dst + j2, c0, r, g, b);
            aipl_cnvt_px_yuv_to_rgba5551(dst + j2 + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_BGR888 | AIPL_CONVERT_I420 & TO_BGR888)
aipl_error_t aipl_color_convert_yuv_planar_to_24bit(const uint8_t* y_ptr,
                                                    const uint8_t* u_ptr,
                                                    const uint8_t* v_ptr,
                                                    void* output,
                                                    uint32_t pitch,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint8_t r_offset,
                                                    uint8_t g_offset,
                                                    uint8_t b_offset)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint8_t* dst0 = dst_ptr + i * width * 3;
        uint8_t* dst1 = dst0 + width * 3;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t r0, g0, b0, r1, g1, b1;
            aipl_mve_cnvt_8px_yuv_to_rgb(&r0, &g0, &b0,
                                         c00, r, g, b,
                                         c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_rgb(&r1, &g1, &b1,
                                         c10, r, g, b,
                                         c11, r, g, b);

            vstrbq_scatter_offset_p(dst0 + r_offset, AIPL_3_BYTE_OFFSETS_U16, r0, tail_p);
            vstrbq_scatter_offset_p(dst0 + g_offset, AIPL_3_BYTE_OFFSETS_U16, g0, tail_p);
            vstrbq_scatter_offset_p(dst0 + b_offset, AIPL_3_BYTE_OFFSETS_U16, b0, tail_p);

            vstrbq_scatter_offset_p(dst1 + r_offset, AIPL_3_BYTE_OFFSETS_U16, r1, tail_p);
            vstrbq_scatter_offset_p(dst1 + g_offset, AIPL_3_BYTE_OFFSETS_U16, g1, tail_p);
            vstrbq_scatter_offset_p(dst1 + b_offset, AIPL_3_BYTE_OFFSETS_U16, b1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 4;
            v_src += 4;
            dst0 += 24;
            dst1 += 24;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint8_t* dst0 = dst_ptr + i * width * 3;
        uint8_t* dst1 = dst0 + width * 3;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_24bit(dst0 + j * 3, c00, r, g, b,
                                      r_offset, g_offset, b_offset);
            aipl_cnvt_px_yuv_to_24bit(dst0 + (j + 1) * 3, c01, r, g, b,
                                      r_offset, g_offset, b_offset);
            aipl_cnvt_px_yuv_to_24bit(dst1 + j * 3, c10, r, g, b,
                                      r_offset, g_offset, b_offset);
            aipl_cnvt_px_yuv_to_24bit(dst1 + (j + 1) * 3, c11, r, g, b,
                                      r_offset, g_offset, b_offset);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_BGR888 | AIPL_CONVERT_NV21 & TO_BGR888)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_24bit(const uint8_t* y_ptr,
                                                         const uint8_t* u_ptr,
                                                         const uint8_t* v_ptr,
                                                         void* output,
                                                         uint32_t pitch,
                                                         uint32_t width,
                                                         uint32_t height,
                                                         uint8_t r_offset,
                                                         uint8_t g_offset,
                                                         uint8_t b_offset)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint8_t* dst0 = dst_ptr + i * width * 3;
        uint8_t* dst1 = dst0 + width * 3;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_2_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_2_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t r0, g0, b0, r1, g1, b1;
            aipl_mve_cnvt_8px_yuv_to_rgb(&r0, &g0, &b0,
                                         c00, r, g, b,
                                         c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_rgb(&r1, &g1, &b1,
                                         c10, r, g, b,
                                         c11, r, g, b);

            vstrbq_scatter_offset_p(dst0 + r_offset, AIPL_3_BYTE_OFFSETS_U16, r0, tail_p);
            vstrbq_scatter_offset_p(dst0 + g_offset, AIPL_3_BYTE_OFFSETS_U16, g0, tail_p);
            vstrbq_scatter_offset_p(dst0 + b_offset, AIPL_3_BYTE_OFFSETS_U16, b0, tail_p);

            vstrbq_scatter_offset_p(dst1 + r_offset, AIPL_3_BYTE_OFFSETS_U16, r1, tail_p);
            vstrbq_scatter_offset_p(dst1 + g_offset, AIPL_3_BYTE_OFFSETS_U16, g1, tail_p);
            vstrbq_scatter_offset_p(dst1 + b_offset, AIPL_3_BYTE_OFFSETS_U16, b1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 8;
            v_src += 8;
            dst0 += 24;
            dst1 += 24;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        uint8_t* dst0 = dst_ptr + i * width * 3;
        uint8_t* dst1 = dst0 + width * 3;

        for (uint32_t j = 0; j < width; j += 2)
        {
int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_24bit(dst0 + j * 3, c00, r, g, b,
                                      r_offset, g_offset, b_offset);
            aipl_cnvt_px_yuv_to_24bit(dst0 + (j + 1) * 3, c01, r, g, b,
                                      r_offset, g_offset, b_offset);
            aipl_cnvt_px_yuv_to_24bit(dst1 + j * 3, c10, r, g, b,
                                      r_offset, g_offset, b_offset);
            aipl_cnvt_px_yuv_to_24bit(dst1 + (j + 1) * 3, c11, r, g, b,
                                      r_offset, g_offset, b_offset);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_BGR888 | AIPL_CONVERT_UYVY & TO_BGR888)
aipl_error_t aipl_color_convert_yuv_packed_to_24bit(const uint8_t* y_ptr,
                                                    const uint8_t* u_ptr,
                                                    const uint8_t* v_ptr,
                                                    void* output,
                                                    uint32_t pitch,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint8_t r_offset,
                                                    uint8_t g_offset,
                                                    uint8_t b_offset)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U16, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_4_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_4_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t ur, ug, ub;
            aipl_mve_cnvt_8px_yuv_to_rgb(&ur, &ug, &ub,
                                         c0, r, g, b,
                                         c1, r, g, b);

            vstrbq_scatter_offset_p(dst + r_offset, AIPL_3_BYTE_OFFSETS_U16, ur, tail_p);
            vstrbq_scatter_offset_p(dst + g_offset, AIPL_3_BYTE_OFFSETS_U16, ug, tail_p);
            vstrbq_scatter_offset_p(dst + b_offset, AIPL_3_BYTE_OFFSETS_U16, ub, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 24;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint8_t* dst = dst_ptr + i * width * 3;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 2]);

            aipl_cnvt_px_yuv_to_24bit(dst + j2 * 3, c0, r, g, b,
                                      r_offset, g_offset, b_offset);
            aipl_cnvt_px_yuv_to_24bit(dst + (j2 + 1) * 3, c1, r, g, b,
                                      r_offset, g_offset, b_offset);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_RGB565 | AIPL_CONVERT_I420 & TO_RGB565)
aipl_error_t aipl_color_convert_yuv_planar_to_rgb565(const uint8_t* y_ptr,
                                                     const uint8_t* u_ptr,
                                                     const uint8_t* v_ptr,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_u32(u_src);
            uint32x4_t v = vldrbq_u32(v_src);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_rgb565(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_rgb565(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 4;
            v_src += 4;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch / 2;
        const uint8_t* v_src = v_ptr + i / 2 * pitch / 2;
        aipl_rgb565_px_t* dst0 = dst_ptr + i * width;
        aipl_rgb565_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint32_t j2 = j / 2;
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j2], v_src[j2]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_rgb565(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_rgb565(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_rgb565(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_rgb565(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_RGB565 | AIPL_CONVERT_NV21 & TO_RGB565)
aipl_error_t aipl_color_convert_yuv_semi_planar_to_rgb565(const uint8_t* y_ptr,
                                                          const uint8_t* u_ptr,
                                                          const uint8_t* v_ptr,
                                                          void* output,
                                                          uint32_t pitch,
                                                          uint32_t width,
                                                          uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint16_t* dst0 = dst_ptr + i * width;
        uint16_t* dst1 = dst0 + width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y0 = vldrbq_z_u16(y_src0, tail_p);
            uint16x8_t y1 = vldrbq_z_u16(y_src1, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_2_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_2_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c00, c01, c10, c11;

            aipl_mve_pre_cnvt_8x2px_y(&c00, &c01, &c10, &c11, y0, y1);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px0, px1;
            aipl_mve_cnvt_8px_yuv_to_rgb565(&px0, c00, c01, r, g, b);
            aipl_mve_cnvt_8px_yuv_to_rgb565(&px1, c10, c11, r, g, b);

            vstrhq_p(dst0, px0, tail_p);
            vstrhq_p(dst1, px1, tail_p);

            y_src0 += 8;
            y_src1 += 8;
            u_src += 8;
            v_src += 8;
            dst0 += 8;
            dst1 += 8;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        aipl_rgb565_px_t* dst0 = dst_ptr + i * width;
        aipl_rgb565_px_t* dst1 = dst0 + width;

        for (uint32_t j = 0; j < width; j += 2)
        {
            int32_t c00, c01, c10, c11;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2x2px_y(&c00, &c01, &c10, &c11,
                                  y_src0[j], y_src0[j + 1],
                                  y_src1[j], y_src1[j + 1]);

            aipl_cnvt_px_yuv_to_rgb565(dst0 + j, c00, r, g, b);
            aipl_cnvt_px_yuv_to_rgb565(dst0 + j + 1, c01, r, g, b);
            aipl_cnvt_px_yuv_to_rgb565(dst1 + j, c10, r, g, b);
            aipl_cnvt_px_yuv_to_rgb565(dst1 + j + 1, c11, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_RGB565 | AIPL_CONVERT_UYVY & TO_RGB565)
aipl_error_t aipl_color_convert_yuv_packed_to_rgb565(const uint8_t* y_ptr,
                                                     const uint8_t* u_ptr,
                                                     const uint8_t* v_ptr,
                                                     void* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (y_ptr == NULL || u_ptr == NULL || v_ptr == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * width;

        for (int32_t cnt = width / 2; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint16x8_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U16, tail_p);
            uint32x4_t u = vldrbq_gather_offset(u_src, AIPL_4_BYTE_OFFSETS_U32);
            uint32x4_t v = vldrbq_gather_offset(v_src, AIPL_4_BYTE_OFFSETS_U32);

            int32x4_t r, g, b;
            int32x4_t c0, c1;

            aipl_mve_pre_cnvt_8px_y(&c0, &c1, y);
            aipl_mve_pre_cnvt_4px_yuv_to_rgb(&r, &g, &b, u, v);

            uint16x8_t px;
            aipl_mve_cnvt_8px_yuv_to_rgb565(&px, c0, c1, r, g, b);

            vstrhq_p(dst, px, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 8;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_rgb565_px_t* dst = dst_ptr + i * width;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            uint32_t j2 = j / 2;
            int32_t c0, c1;
            int32_t r, g, b;

            aipl_pre_cnvt_px_yuv_to_rgb(&r, &g, &b,
                                        u_src[j], v_src[j]);
            aipl_pre_cnvt_2px_y(&c0, &c1,
                                y_src[j], y_src[j + 2]);

            aipl_cnvt_px_yuv_to_rgb565(dst + j2, c0, r, g, b);
            aipl_cnvt_px_yuv_to_rgb565(dst + j2 + 1, c1, r, g, b);
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_I420 | AIPL_CONVERT_I420 & TO_YV12)
aipl_error_t aipl_color_convert_yuv_planar_to_planar(const uint8_t* y_src,
                                                     const uint8_t* u_src,
                                                     const uint8_t* v_src,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch;
        uint8_t* y_d = y_dst + i * width;

        memcpy(y_d, y_s, width);

        if (!(i & 1))
        {
            const uint8_t* u_s = u_src + i * pitch / 4;
            uint8_t* u_d = u_dst + i * width / 4;
            const uint8_t* v_s = v_src + i * pitch / 4;
            uint8_t* v_d = v_dst + i * width / 4;

            memcpy(u_d, u_s, width / 2);
            memcpy(v_d, v_s, width / 2);
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & (TO_NV12 | TO_NV21) | AIPL_CONVERT_I420 & (TO_NV12 | TO_NV21))
aipl_error_t aipl_color_convert_yuv_planar_to_semi(const uint8_t* y_src,
                                                   const uint8_t* u_src,
                                                   const uint8_t* v_src,
                                                   uint8_t* y_dst,
                                                   uint8_t* u_dst,
                                                   uint8_t* v_dst,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch;
        uint8_t* y_d = y_dst + i * width;

        memcpy(y_d, y_s, width);

        if (!(i & 1))
        {
            const uint8_t* u_s = u_src + i / 2 * pitch / 2;
            uint8_t* u_d = u_dst + i / 2 * width;
            const uint8_t* v_s = v_src + i / 2 * pitch / 2;
            uint8_t* v_d = v_dst + i / 2 * width;
#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width;

            uint32_t j = 0;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vld1q_z(u_s, tail_p);

                vstrbq_scatter_offset_p(u_d, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vld1q_z(v_s, tail_p);

                vstrbq_scatter_offset_p(v_d, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_s += 16;
                v_s += 16;
                u_d += 32;
                v_d += 32;
                cnt -= 32;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_d[j] = u_s[j / 2];
                v_d[j] = v_s[j / 2];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & (TO_YUY2 | TO_UYVY) | AIPL_CONVERT_I420 & (TO_YUY2 | TO_UYVY))
aipl_error_t aipl_color_convert_yuv_planar_to_packed(const uint8_t* y_src,
                                                     const uint8_t* u_src,
                                                     const uint8_t* v_src,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch;
        uint8_t* y_d = y_dst + i * width * 2;
        const uint8_t* u_s = u_src + i / 2 * pitch / 2;
        uint8_t* u_d = u_dst + i * width * 2;
        const uint8_t* v_s = v_src + i / 2* pitch / 2;
        uint8_t* v_d = v_dst + i * width * 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_d[j * 4] = y_s[j * 2];
            y_d[j * 4 + 2] = y_s[j * 2 + 1];
            u_d[j * 4] = u_s[j];
            v_d[j * 4] = v_s[j];
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & (TO_YV12 | TO_I420) | AIPL_CONVERT_NV21 & (TO_YV12 | TO_I420))
aipl_error_t aipl_color_convert_yuv_semi_to_planar(const uint8_t* y_src,
                                                   const uint8_t* u_src,
                                                   const uint8_t* v_src,
                                                   uint8_t* y_dst,
                                                   uint8_t* u_dst,
                                                   uint8_t* v_dst,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch;
        uint8_t* y_d = y_dst + i * width;

        memcpy(y_d, y_s, width);

        if (!(i & 1))
        {
            const uint8_t* u_s = u_src + i / 2 * pitch;
            uint8_t* u_d = u_dst + i / 2 * width / 2;
            const uint8_t* v_s = v_src + i / 2 * pitch;
            uint8_t* v_d = v_dst + i / 2 * width / 2;

#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width / 2;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vldrbq_gather_offset_z(u_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vst1q_p(u_d, u, tail_p);

                uint8x16_t v = vldrbq_gather_offset_z(v_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vst1q_p(v_d, v, tail_p);

                u_s += 32;
                v_s += 32;
                u_d += 16;
                v_d += 16;
                cnt -= 16;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_d[j / 2] = u_s[j];
                v_d[j / 2] = v_s[j];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & TO_NV21 | AIPL_CONVERT_NV21 & TO_NV12)
aipl_error_t aipl_color_convert_yuv_semi_to_semi_planar(const uint8_t* y_src,
                                                        const uint8_t* u_src,
                                                        const uint8_t* v_src,
                                                        uint8_t* y_dst,
                                                        uint8_t* u_dst,
                                                        uint8_t* v_dst,
                                                        uint32_t pitch,
                                                        uint32_t width,
                                                        uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch;
        uint8_t* y_d = y_dst + i * width;

        memcpy(y_d, y_s, width);

        if (!(i & 1))
        {
            const uint8_t* u_s = u_src + i / 2 * pitch;
            uint8_t* u_d = u_dst + i / 2 * width;
            const uint8_t* v_s = v_src + i / 2 * pitch;
            uint8_t* v_d = v_dst + i / 2 * width;

            for (uint32_t j = 0; j < width; j += 2)
            {
                u_d[j] = u_s[j];
                v_d[j] = v_s[j];
            }
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_NV12 & (TO_YUY2 | TO_UYVY) | AIPL_CONVERT_NV21 & (TO_YUY2 | TO_UYVY))
aipl_error_t aipl_color_convert_yuv_semi_to_packed(const uint8_t* y_src,
                                                   const uint8_t* u_src,
                                                   const uint8_t* v_src,
                                                   uint8_t* y_dst,
                                                   uint8_t* u_dst,
                                                   uint8_t* v_dst,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch;
        uint8_t* y_d = y_dst + i * width * 2;
        const uint8_t* u_s = u_src + i / 2 * pitch;
        uint8_t* u_d = u_dst + i * width * 2;
        const uint8_t* v_s = v_src + i / 2 * pitch;
        uint8_t* v_d = v_dst + i * width * 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_d[j * 4] = y_s[j * 2];
            y_d[j * 4 + 2] = y_s[j * 2 + 1];
            u_d[j * 4] = u_s[j * 2];
            v_d[j * 4] = v_s[j * 2];
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & (TO_YV12 | TO_I420) | AIPL_CONVERT_UYVY & (TO_YV12 | TO_I420))
aipl_error_t aipl_color_convert_yuv_packed_to_planar(const uint8_t* y_src,
                                                     const uint8_t* u_src,
                                                     const uint8_t* v_src,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch * 2;
        uint8_t* y_d = y_dst + i * width;

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(y_d, y, tail_p);

            y_s += 32;
            y_d += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 2)
        {
            y_d[j / 2] = y_s[j];
        }
#endif

        if (!(i & 1))
        {
            const uint8_t* u_s = u_src + i * pitch * 2;
            uint8_t* u_d = u_dst + i / 2 * width / 2;
            const uint8_t* v_s = v_src + i * pitch * 2;
            uint8_t* v_d = v_dst + i / 2 * width / 2;

#ifdef AIPL_HELIUM_ACCELERATION
            cnt = width / 2;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vldrbq_gather_offset_z(u_s, AIPL_4_BYTE_OFFSETS_U8, tail_p);

                vstrbq_p_u8(u_d, u, tail_p);

                uint8x16_t v = vldrbq_gather_offset_z(v_s, AIPL_4_BYTE_OFFSETS_U8, tail_p);

                vstrbq_p_u8(v_d, v, tail_p);

                u_s += 64;
                v_s += 64;
                u_d += 16;
                v_d += 16;
                cnt -= 16;
            }
#else
            for (uint32_t j = 0; j < width * 2; j += 4)
            {
                u_d[j / 4] = u_s[j];
                v_d[j / 4] = v_s[j];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & (TO_NV12 | TO_NV21) | AIPL_CONVERT_UYVY & (TO_NV12 | TO_NV21))
aipl_error_t aipl_color_convert_yuv_packed_to_semi(const uint8_t* y_src,
                                                   const uint8_t* u_src,
                                                   const uint8_t* v_src,
                                                   uint8_t* y_dst,
                                                   uint8_t* u_dst,
                                                   uint8_t* v_dst,
                                                   uint32_t pitch,
                                                   uint32_t width,
                                                   uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch * 2;
        uint8_t* y_d = y_dst + i * width;

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_p_u8(y_d, y, tail_p);

            y_s += 32;
            y_d += 16;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 2)
        {
            y_d[j / 2] = y_s[j];
        }
#endif

        if (!(i & 1))
        {
            const uint8_t* u_s = u_src + i * pitch * 2;
            uint8_t* u_d = u_dst + i / 2 * width;
            const uint8_t* v_s = v_src + i * pitch * 2;
            uint8_t* v_d = v_dst + i / 2 * width;

#ifdef AIPL_HELIUM_ACCELERATION
            cnt = width / 2;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vldrbq_gather_offset_z(u_s, AIPL_4_BYTE_OFFSETS_U8, tail_p);

                vstrbq_scatter_offset_p(u_d, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vldrbq_gather_offset_z(v_s, AIPL_4_BYTE_OFFSETS_U8, tail_p);

                vstrbq_scatter_offset_p(v_d, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_s += 64;
                v_s += 64;
                u_d += 32;
                v_d += 32;
                cnt -= 16;
            }
#else
            for (uint32_t j = 0; j < width * 2; j += 4)
            {
                u_d[j / 2] = u_s[j];
                v_d[j / 2] = v_s[j];
            }
#endif
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YUY2 & TO_UYVY | AIPL_CONVERT_UYVY & TO_YUY2)
aipl_error_t aipl_color_convert_yuv_packed_to_packed(const uint8_t* y_src,
                                                     const uint8_t* u_src,
                                                     const uint8_t* v_src,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (y_src == NULL || u_src == NULL || v_src == NULL
        || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = y_src + i * pitch * 2;
        uint8_t* y_d = y_dst + i * width * 2;
        const uint8_t* u_s = u_src + i * pitch * 2;
        uint8_t* u_d = u_dst + i * width * 2;
        const uint8_t* v_s = v_src + i * pitch * 2;
        uint8_t* v_d = v_dst + i * width * 2;

        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            y_d[j] = y_s[j];
            y_d[j + 2] = y_s[j + 2];
            u_d[j] = u_s[j];
            v_d[j] = v_s[j];
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & (TO_YV12 | TO_I420))
aipl_error_t aipl_color_convert_alpha8_to_yuv_planar(const uint8_t* input,
                                                     uint8_t* output,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint32_t yuv_size = width * height;
    uint8_t* uv_dst0 = output + yuv_size;
    uint8_t* uv_dst1 = uv_dst0 + yuv_size / 4;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = input + i * pitch;
        uint8_t* y_d = output + i * width;

        memcpy(y_d, y_s, width);

        if (!(i & 1))
        {
            uint8_t* uv_d0 = uv_dst0 + i / 2 * width / 2;
            uint8_t* uv_d1 = uv_dst1 + i / 2 * width / 2;

            memset(uv_d0, 0x80, width / 2);
            memset(uv_d1, 0x80, width / 2);
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & (TO_NV12 | TO_NV21))
aipl_error_t aipl_color_convert_alpha8_to_yuv_semi_planar(const uint8_t* input,
                                                          uint8_t* output,
                                                          uint32_t pitch,
                                                          uint32_t width,
                                                          uint32_t height)
{
    if (input == NULL || output == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint8_t* uv_dst = output + width * height;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = input + i * pitch;
        uint8_t* y_d = output + i * width;

        memcpy(y_d, y_s, width);

        if (!(i & 1))
        {
            uint8_t* uv_d = uv_dst + i / 2 * width;

            memset(uv_d, 0x80, width);
        }
    }

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_ALPHA8_I400 & (TO_YUY2 | TO_UYVY))
aipl_error_t aipl_color_convert_alpha8_to_yuv_packed(const uint8_t* input,
                                                     uint8_t* y_dst,
                                                     uint8_t* u_dst,
                                                     uint8_t* v_dst,
                                                     uint32_t pitch,
                                                     uint32_t width,
                                                     uint32_t height)
{
    if (input == NULL || y_dst == NULL || u_dst == NULL || v_dst == NULL)
        return AIPL_ERR_NULL_POINTER;

    uint8_t* start = y_dst;
    if (u_dst < start) start = u_dst;
    if (v_dst < start) start = v_dst;
    memset(start, 0x80, width * height * 2);

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_s = input + i * pitch;
        uint8_t* y_d = y_dst + i * width * 2;

#ifdef AIPL_HELIUM_ACCELERATION
        for (int32_t cnt = width / 2; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_d, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y = vldrbq_gather_offset_z(y_s + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_d + 2, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y_s += 32;
            y_d += 64;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            y_d[j] = y_s[j / 2];
            y_d[j + 2] = y_s[j / 2 + 1];
        }
#endif
    }

    return AIPL_ERR_OK;
}
#endif
