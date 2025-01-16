/**
 * @file aipl_color_conversion.c
 *
 */

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
#else
#include "aipl_utils.h"
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
#if (AIPL_CONVERT_RGB888 & TO_I422 | AIPL_CONVERT_BGR888 & TO_I422)
aipl_error_t aipl_color_convert_24bit_to_i422(const void* input,
                                              void* output,
                                              uint32_t pitch,
                                              uint32_t width,
                                              uint32_t height,
                                              uint8_t r_offset,
                                              uint8_t g_offset,
                                              uint8_t b_offset);
#endif
#if (AIPL_CONVERT_RGB888 & TO_I444 | AIPL_CONVERT_BGR888 & TO_I444)
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
     | AIPL_CONVERT_RGB888 & TO_YUY2 | AIPL_CONVERT_RGB888 & TO_UYVY)
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
     | AIPL_CONVERT_NV12 & TO_ALPHA8_I400 | AIPL_CONVERT_NV21 & TO_ALPHA8_I400)
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

    /* TODO: figure out what's wrong with ARGB1555, RGBA5551 and ALPHA8 */
    if (aipl_dave2d_format_supported(input_format)
        && aipl_dave2d_format_supported(output_format)
        && output_format != AIPL_COLOR_ARGB1555
        && output_format != AIPL_COLOR_RGBA5551
        && output_format != AIPL_COLOR_ALPHA8)
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
            return aipl_color_convert_i400(input, output, pitch,
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
#if AIPL_CONVERT_I400
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

            aipl_mve_argb_x8_t pix;
            aipl_mve_loada_argb8888_8px(&pix, src, tail_p);

            uint16x8_t alpha;
            aipl_mve_calculate_y_argb_x8(&alpha, &pix);

            vstrbq_p(dst, alpha, tail_p);

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
            aipl_packa_yuv_y(dst, src);

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb8888, d2_mode_argb4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb8888_16px(&pix, src, tail_p);

            aipl_mve_storea_argb4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_packa_argb4444_px(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb8888_16px(&pix, src, tail_p);

            aipl_mve_storea_argb1555_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].t = (src[j].a & 0x80) | ((src[j].r >> 1) & 0x7c) | (src[j].g >> 6);
            dst[j].b = ((src[j].g << 2) & 0xe0) | (src[j].b >> 3);
        }
    }
#endif

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
        uint32_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t px = vld1q_z(src, tail_p);

            uint32x4_t a = vshrq(px, 24);
            px = vsliq(a, px, 8);

            vst1q_p(dst, px, tail_p);

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
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->r = src->r;
            dst->g = src->g;
            dst->b = src->b;
            dst->a = src->a;

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb8888, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb8888_16px(&pix, src, tail_p);

            aipl_mve_storea_rgba4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_packa_rgba4444_px(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb8888_16px(&pix, src, tail_p);

            aipl_mve_storea_rgba5551_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].t = (src[j].r & 0xf8) | (src[j].g >> 5);
            dst[j].b = ((src[j].g << 3) & 0xc0) | ((src[j].b >> 2) & 0x3e) | (src[j].a >> 7);
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb8888, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb8888_16px(&pix, src, tail_p);

            aipl_mve_store_rgb565_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
        }
    }
#else
    const aipl_argb8888_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].t = (src[j].r & 0xf8) | (src[j].g >> 5);
            dst[j].b = ((src[j].g << 3) & 0xe0) | (src[j].b >> 3);
        }
    }
#endif

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
            aipl_mve_load_argb8888_8px(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_calculate_y_rgb_x8(&y, &pix);

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
            aipl_mve_load_argb8888_offset_8px(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_calculate_u_rgb_x8(&u, &pix);

            uint16x8_t v;
            aipl_mve_calculate_v_rgb_x8(&v, &pix);

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
            y_dst[j] = ((66 * src[j].r + 129 * src[j].g + 25 * src[j].b + 128) >> 8) + 16;
        }

        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            uint32_t j2 = 2 * j;
            u_dst[j] = ((-38 * src[j2].r - 74 * src[j2].g + 112 * src[j2].b + 128) >> 8) + 128;
            v_dst[j] = ((112 * src[j2].r - 94 * src[j2].g - 18 * src[j2].b + 128) >> 8) + 128;
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

    const uint16x8_t input_pixel_offsets = vmulq_n_u16(vidupq_n_u16(0, 1), 4);

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 4;

        uint8_t* y_dst = y_ptr + i * width;
        uint8_t* v_dst = v_ptr + i * width;
        uint8_t* u_dst = u_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            aipl_mve_rgb_x8_t pix;
            aipl_mve_load_argb8888_8px(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_calculate_y_rgb_x8(&y, &pix);

            vstrbq_p(y_dst, y, tail_p);

            uint16x8_t u;
            aipl_mve_calculate_u_rgb_x8(&u, &pix);

            vstrbq_p(u_dst, u, tail_p);

            uint16x8_t v;
            aipl_mve_calculate_v_rgb_x8(&v, &pix);

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
            y_dst[j] = ((66 * src[j].r + 129 * src[j].g + 25 * src[j].b + 128) >> 8) + 16;
            u_dst[j] = ((-38 * src[j].r - 74 * src[j].g + 112 * src[j].b + 128) >> 8) + 128;
            v_dst[j] = ((112 * src[j].r - 94 * src[j].g - 18 * src[j].b + 128) >> 8) + 128;
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t alpha;
            aipl_mve_calculate_y_argb_x16(&alpha, &pix);

            aipl_mve_store_alpha8_16px(dst, &alpha, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_loada_argb4444_px(&px, src);
            aipl_packa_yuv_y(dst, &px);

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_loada_argb4444_px(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb1555_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = (src->ar & 0x80) | ((src->ar << 3) & 0x7c)
                     | (src->gb >> 6);
            dst->b = ((src->gb << 2) & 0xe0) | ((src->gb << 1) & 0x1f);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->r = src->ar << 4;
            dst->g = src->gb & 0xf0;
            dst->b = src->gb << 4;
            dst->a = src->ar & 0xf0;

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb4444, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->rg = (src->ar << 4) | (src->gb >> 4);
            dst->ba = (src->gb << 4) | (src->ar >> 4);

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba5551_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = (src->ar << 4) | (src->gb >> 5);
            dst->b = ((src->gb << 3) & 0x80) | ((src->gb << 2) & 0x3e)
                     | (src->ar >> 7);

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb4444, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_rgb565_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = (src->ar << 4) | (src->gb >> 5);
            dst->b = ((src->gb << 3) & 0x80) | ((src->gb << 1) & 0x1f);

            ++src;
            ++dst;
        }
    }
#endif

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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb4444_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_p(u_dst, u, tail_p);
                u_dst += 16;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_p(v_dst, v, tail_p);
                v_dst += 16;
            }
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_argb4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_argb4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_p(y_dst, y, tail_p);

            uint8x16_t u;
            aipl_mve_calculate_u_rgb_x16(&u, &pix);

            vstrbq_p(u_dst, u, tail_p);

            uint8x16_t v;
            aipl_mve_calculate_v_rgb_x16(&v, &pix);

            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            y_dst += 16;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_load_argb4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t alpha;
            aipl_mve_calculate_y_argb_x16(&alpha, &pix);

            aipl_mve_store_alpha8_16px(dst, &alpha, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_loada_argb1555_px(&px, src);
            aipl_packa_yuv_y(dst, &px);

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_loada_argb1555_px(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

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

// #ifdef AIPL_DAVE2D_ACCELERATION
//     d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
//                                                 d2_mode_argb1555, d2_mode_argb4444);

//     return aipl_dave2d_error_convert(ret);
// #else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->ar = ((src->t >> 7) * 0xf0) | ((src->t >> 3) & 0x0f);
            dst->gb = (src->t << 6) | ((src->b >> 2) & 0x30)
                      | ((src->b >> 1) & 0x0f);

            ++src;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
// #endif
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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->r = (src->t << 1) & 0xf8;
            dst->g = (src->t << 6) | ((src->b >> 2) & 0x38);
            dst->b = src->b << 3;
            dst->a = (src->t >> 7) * 0xff;

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb1555, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->rg = ((src->t << 1) & 0xf0) | ((src->t << 2) & 0x0f)
                      | (src->b >> 6);
            dst->ba = ((src->b << 3) & 0xf0) | (src->t >> 7) * 0x0f;

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba5551_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->h = (src->h << 1) | (src->h >> 15);

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_argb1555, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_rgb565_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = (src->t << 1) | (src->b >> 7);
            dst->b = ((src->b << 1) & 0xe0) | (src->b & 0x1f);

            ++src;
            ++dst;
        }
    }
#endif

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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb1555_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_p(u_dst, u, tail_p);
                u_dst += 16;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_p(v_dst, v, tail_p);
                v_dst += 16;
            }
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_argb1555_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_argb1555_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_p(y_dst, y, tail_p);

            uint8x16_t u;
            aipl_mve_calculate_u_rgb_x16(&u, &pix);

            vstrbq_p(u_dst, u, tail_p);

            uint8x16_t v;
            aipl_mve_calculate_v_rgb_x16(&v, &pix);

            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            y_dst += 16;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_load_argb1555_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba8888_16px(&pix, src, tail_p);

            uint8x16_t alpha;
            aipl_mve_calculate_y_argb_x16(&alpha, &pix);

            aipl_mve_store_alpha8_16px(dst, &alpha, tail_p);

            src += 64;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_loada_rgba8888_px(&px, src);
            aipl_packa_yuv_y(dst, &px);

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
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba8888_16px(&pix, src, tail_p);

            aipl_mve_storea_argb8888_16px(dst, &pix, tail_p);

            src += 64;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_loada_rgba8888_px(dst, src);

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba8888, d2_mode_argb4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba8888_16px(&pix, src, tail_p);

            aipl_mve_storea_argb4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->ar = (src->a & 0xf0) | (src->r >> 4);
            dst->gb = (src->g & 0xf0) | (src->b >> 4);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba8888_16px(&pix, src, tail_p);

            aipl_mve_storea_argb1555_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->h = ((uint16_t)(src->a & 0x80) << 8)
                     | ((uint16_t)(src->r & 0xf8) << 7)
                     | ((uint16_t)(src->g & 0xf8) << 2)
                     | (src->b >> 3);

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba8888, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba8888_16px(&pix, src, tail_p);

            aipl_mve_storea_rgba4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->rg = (src->r & 0xf0) | (src->g >> 4);
            dst->ba = (src->b & 0xf0) | (src->a >> 4);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba8888_16px(&pix, src, tail_p);

            aipl_mve_storea_rgba5551_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->h = ((uint16_t)(src->r & 0xf8) << 8)
                     | ((uint16_t)(src->g & 0xf8) << 3)
                     | ((src->b & 0xf8) >> 2)
                     | (src->a >> 7);

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba8888, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba8888_16px(&pix, src, tail_p);

            aipl_mve_store_rgb565_16px((uint8_t*)dst, &pix, tail_p);

            src += 64;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->h = ((uint16_t)(src->r & 0xf8) << 8)
                     | ((uint16_t)(src->g & 0xfc) << 3)
                     | (src->b >> 3);

            ++src;
            ++dst;
        }
    }
#endif

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
        int32_t cnt = width / 2;
        const uint8_t* src = src_ptr + i * pitch * 4;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba8888_offset_16px(&pix, src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 124;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                src += 4;
                ++y_dst;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_p(u_dst, u, tail_p);
                u_dst += 16;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_p(v_dst, v, tail_p);
                v_dst += 16;
            }
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba8888_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_rgba8888_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba8888_16px(&pix, src, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_p(y_dst, y, tail_p);

            uint8x16_t u;
            aipl_mve_calculate_u_rgb_x16(&u, &pix);

            vstrbq_p(u_dst, u, tail_p);

            uint8x16_t v;
            aipl_mve_calculate_v_rgb_x16(&v, &pix);

            vstrbq_p(v_dst, v, tail_p);

            src += 64;
            y_dst += 16;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_load_rgba8888_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t alpha;
            aipl_mve_calculate_y_argb_x16(&alpha, &pix);

            aipl_mve_store_alpha8_16px(dst, &alpha, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_loada_rgba4444_px(&px, src);
            aipl_packa_yuv_y(dst, &px);

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_loada_rgba4444_px(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb1555_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = ((src->ba << 4) & 0x80) | ((src->rg >> 1) & 0x78)
                     | ((src->rg >> 2) & 0x03);
            dst->b = (src->rg << 6) | (src->ba >> 3) | 0x01;

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba4444, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->ar = (src->rg >> 4) | (src->ba << 4);
            dst->gb = (src->rg << 4) | (src->ba >> 4);

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->r = src->rg & 0xf0;
            dst->g = src->rg << 4;
            dst->b = src->ba & 0xf0;
            dst->a = src->ba << 4;

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba5551_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = (src->rg & 0xf0) | ((src->rg >> 1) & 0x07);
            dst->b = (src->rg << 7) | ((src->ba >> 2) & 0x38)
                     | ((src->ba >> 3) & 0x01);

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba4444, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_rgb565_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = (src->rg & 0xf0) | ((src->rg >> 1) & 0x07);
            dst->b = (src->rg << 7) | ((src->ba >> 3) & 0x1f) | 0x01;

            ++src;
            ++dst;
        }
    }
#endif

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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba4444_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_p(u_dst, u, tail_p);
                u_dst += 16;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_p(v_dst, v, tail_p);
                v_dst += 16;
            }
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_rgba4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_p(y_dst, y, tail_p);

            uint8x16_t u;
            aipl_mve_calculate_u_rgb_x16(&u, &pix);

            vstrbq_p(u_dst, u, tail_p);

            uint8x16_t v;
            aipl_mve_calculate_v_rgb_x16(&v, &pix);

            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            y_dst += 16;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_load_rgba4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t alpha;
            aipl_mve_calculate_y_argb_x16(&alpha, &pix);

            aipl_mve_store_alpha8_16px(dst, &alpha, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_loada_rgba5551_px(&px, src);
            aipl_packa_yuv_y(dst, &px);

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_loada_rgba5551_px(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb1555_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->h = (src->h >> 1) | (src->h << 15);

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba5551, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_argb4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->ar = ((src->b & 0x01) * 0xf0) | (src->t >> 4);
            dst->gb = (src->t << 5) | ((src->b >> 3) & 0x10) | ((src->b >> 2) & 0x0f);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->r = src->t & 0xf8;
            dst->g = (src->t << 5) | ((src->b >> 3) & 0x18);
            dst->b = (src->b << 2) & 0xf8;
            dst->a = (src->b & 0x01) * 0xff;

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba5551, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_argb_x16_t pix;
            aipl_mve_loada_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_storea_rgba4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->rg = (src->t & 0xf0) | ((src->t << 1) & 0x0f) | (src->b >> 7);
            dst->ba = ((src->b << 2) & 0xf0) | ((src->b & 0x01) * 0x0f);

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgba5551, d2_mode_rgb565);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_rgb565_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = src->t;
            dst->b = (src->b & 0xc0) | ((src->b >> 1) & 0x1f);

            ++src;
            ++dst;
        }
    }
#endif

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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba5551_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_p(u_dst, u, tail_p);
                u_dst += 16;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_p(v_dst, v, tail_p);
                v_dst += 16;
            }
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba5551_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_rgba5551_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_p(y_dst, y, tail_p);

            uint8x16_t u;
            aipl_mve_calculate_u_rgb_x16(&u, &pix);

            vstrbq_p(u_dst, u, tail_p);

            uint8x16_t v;
            aipl_mve_calculate_v_rgb_x16(&v, &pix);

            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            y_dst += 16;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_load_rgba5551_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);
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
    return aipl_color_convert_24bit_to_i422(input, output, pitch,
                                            width, height,
                                            2, 1, 0);
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_I444)
aipl_error_t aipl_color_convert_bgr888_to_i444(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_24bit_to_i444(input, output, pitch,
                                            width, height,
                                            2, 1, 0);
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

    return aipl_color_convert_24bit_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  2, 1, 0);
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

    return aipl_color_convert_24bit_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  2, 1, 0);
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
    return aipl_color_convert_24bit_to_i422(input, output, pitch,
                                            width, height,
                                            0, 1, 2);
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_I444)
aipl_error_t aipl_color_convert_rgb888_to_i444(const void* input,
                                               void* output,
                                               uint32_t pitch,
                                               uint32_t width,
                                               uint32_t height)
{
    return aipl_color_convert_24bit_to_i444(input, output, pitch,
                                            width, height,
                                            0, 1, 2);
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

    return aipl_color_convert_24bit_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  0, 1, 2);
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

    return aipl_color_convert_24bit_to_yuv_packed(input, pitch,
                                                  width, height,
                                                  y_ptr, u_ptr, v_ptr,
                                                  0, 1, 2);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t alpha;
            aipl_mve_calculate_y_rgb_x16(&alpha, &pix);

            aipl_mve_store_alpha8_16px(dst, &alpha, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_argb8888_px_t px;
            aipl_loada_rgb565_px(&px, src);
            aipl_packa_yuv_y(dst, &px);

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_argb8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_loada_rgb565_px(dst, src);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_argb1555_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = 0x80 | (src->t >> 1);
            dst->b = (src->t << 7) | ((src->b >> 1) & 0x60) | (src->b & 0x1f);

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgb565, d2_mode_rgba8888);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_argb4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->ar = 0xf0 | (src->t >> 4);
            dst->gb = (src->t << 5) | ((src->b >> 3) & 0x10) | ((src->b >> 1) & 0x0f);

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_rgba8888_16px(dst, &pix, tail_p);

            src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->r = src->t & 0xf8;
            dst->g = (src->h & 0x07e0) >> 3;
            dst->b = (src->b & 0x1f) << 3;
            dst->a = 0xff;

            ++src;
            ++dst;
        }
    }
#endif

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

#ifdef AIPL_DAVE2D_ACCELERATION
    d2_s32 ret = aipl_dave2d_color_mode_convert(input, output, pitch, width, height,
                                                d2_mode_rgb565, d2_mode_rgba4444);

    return aipl_dave2d_error_convert(ret);
#else
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_rgba4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->rg = (src->t & 0xf0) | ((src->t << 1) & 0x0f) | (src->b >> 7);
            dst->ba = ((src->b << 3) & 0xf0) | 0x0f;

            ++src;
            ++dst;
        }
    }
#endif

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
#ifdef AIPL_HELIUM_ACCELERATION
    const uint16_t* src_ptr = input;
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_rgba5551_16px((uint8_t*)dst, &pix, tail_p);

            src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->t = src->t;
            dst->b = (src->b & 0xc0) | ((src->b << 1) & 0x3e) | 0x01;

            ++src;
            ++dst;
        }
    }
#endif

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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_p(u_dst, u, tail_p);
                u_dst += 16;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_p(v_dst, v, tail_p);
                v_dst += 16;
            }
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgb565_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_rgb565_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_p(y_dst, y, tail_p);

            uint8x16_t u;
            aipl_mve_calculate_u_rgb_x16(&u, &pix);

            vstrbq_p(u_dst, u, tail_p);

            uint8x16_t v;
            aipl_mve_calculate_v_rgb_x16(&v, &pix);

            vstrbq_p(v_dst, v, tail_p);

            src += 16;
            y_dst += 16;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_load_rgb565_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst + 4, &pix, 2, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            dst += 128;
            cnt -= 16;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *u_src, *v_src, 2, 1, 0);
            dst->a = 0xff;
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            ++dst;

            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            dst->a = 0xff;
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            ++dst;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_argb4444_px(dst++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_argb1555_px(dst++, px, 2, 1, 0);
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst + 4, &pix, 2, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            dst += 128;
            cnt -= 16;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *u_src, *v_src, 2, 1, 0);
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            dst->a = 0xff;
            ++dst;

            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            dst->a = 0xff;
            ++dst;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgba4444_px(dst++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgba5551_px(dst++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgb565_px(dst++, px, 2, 1, 0);
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

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_dst + 2, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            y_dst += 64;
            u_dst += 64;
            v_dst += 64;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_dst[j * 4] = y_src[j * 2];
            y_dst[j * 4 + 2] = y_src[j * 2 + 1];
            u_dst[j * 4] = u_src[j];
            v_dst[j * 4] = v_src[j];
        }
#endif
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

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_dst + 2, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            y_dst += 64;
            u_dst += 64;
            v_dst += 64;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_dst[j * 4] = y_src[j * 2];
            y_dst[j * 4 + 2] = y_src[j * 2 + 1];
            u_dst[j * 4] = u_src[j];
            v_dst[j * 4] = v_src[j];
        }
#endif
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_16px(dst, &pix, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            dst->a = 0xff;
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            ++dst;
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
        int32_t cnt = width;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_16px((uint8_t*)dst, &pix, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_argb4444_px(dst++, px, 2, 1, 0);
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
        int32_t cnt = width;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_16px((uint8_t*)dst, &pix, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_argb1555_px(dst++, px, 2, 1, 0);
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_16px(dst, &pix, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            dst->a = 0xff;
            ++dst;
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
        int32_t cnt = width;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_16px((uint8_t*)dst, &pix, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgba4444_px(dst++, px, 2, 1, 0);
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
        int32_t cnt = width;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_16px((uint8_t*)dst, &pix, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgba5551_px(dst++, px, 2, 1, 0);
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
        int32_t cnt = width;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_16px((uint8_t*)dst, &pix, tail_p);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgb565_px(dst++, px, 2, 1, 0);
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
#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width;

            uint32_t j = 0;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_src += 32;
                v_src += 32;
                u_dst += 32;
                v_dst += 32;
                cnt -= 32;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_dst[j] = u_src[j];
                v_dst[j] = v_src[j];
            }
#endif
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
#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width;

            uint32_t j = 0;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_src += 32;
                v_src += 32;
                u_dst += 32;
                v_dst += 32;
                cnt -= 32;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_dst[j] = u_src[j];
                v_dst[j] = v_src[j];
            }
#endif
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

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_dst + 2, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);

            y_src += 32;
            u_src += 32;
            v_src += 32;
            y_dst += 64;
            u_dst += 64;
            v_dst += 64;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_dst[j * 4] = y_src[j * 2];
            y_dst[j * 4 + 2] = y_src[j * 2 + 1];
            u_dst[j * 4] = u_src[j * 2];
            v_dst[j * 4] = v_src[j * 2];
        }
#endif
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

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_dst + 2, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);

            y_src += 32;
            u_src += 32;
            v_src += 32;
            y_dst += 64;
            u_dst += 64;
            v_dst += 64;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_dst[j * 4] = y_src[j * 2];
            y_dst[j * 4 + 2] = y_src[j * 2 + 1];
            u_dst[j * 4] = u_src[j * 2];
            v_dst[j * 4] = v_src[j * 2];
        }
#endif
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
#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
            vstrbq_scatter_offset_p(u_dst + 1, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
            vstrbq_scatter_offset_p(v_dst + 1, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

            u_src += 32;
            v_src += 32;
            u_dst += 32;
            v_dst += 32;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width; j += 2)
        {
            u_dst[j] = u_dst[j + 1] = u_src[j];
            v_dst[j] = v_dst[j + 1] = v_src[j];
        }
#endif
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
#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
            vstrbq_scatter_offset_p(u_dst + 1, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
            vstrbq_scatter_offset_p(v_dst + 1, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

            u_src += 32;
            v_src += 32;
            u_dst += 32;
            v_dst += 32;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width; j += 2)
        {
            u_dst[j] = u_dst[j + 1] = u_src[j];
            v_dst[j] = v_dst[j + 1] = v_src[j];
        }
#endif
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
            return AIPL_ERR_FORMAT_MISMATCH;
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

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint32_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t y = vldrbq_z_u32(y_src, tail_p);

            y = vmulq(y, vdupq_n_u32(0x00010101));
            y = vorrq(y, vdupq_n_u32(0xff000000));

            vst1q_p(dst, y, tail_p);

            y_src += 4;
            dst += 4;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].a = 0xff;
            dst[j].r = y_src[j];
            dst[j].g = y_src[j];
            dst[j].b = y_src[j];
        }
    }
#endif

    return AIPL_ERR_OK;
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

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);

            y = vshrq(y, 4);
            y = vmulq(y, vdupq_n_u16(0x0111));
            y = vorrq(y, vdupq_n_u16(0xf000));

            vstrhq_p(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].ar = 0xf0 | (y_src[j] >> 4);
            dst[j].gb = (y_src[j] & 0xf0) | (y_src[j] >> 4);
        }
    }
#endif

    return AIPL_ERR_OK;
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

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);

            y = vshrq(y, 3);
            y = vmulq(y, vdupq_n_u16(0x0421));
            y = vorrq(y, vdupq_n_u16(0x8000));

            vstrhq_p(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].t = 0x80 | ((y_src[j] & 0xf8) >> 1) | (y_src[j] >> 6);
            dst[j].b = ((y_src[j] & 0xf8) << 2) | (y_src[j] >> 3);
        }
    }
#endif

    return AIPL_ERR_OK;
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

#ifdef AIPL_HELIUM_ACCELERATION
    uint32_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint32_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 4)
        {
            mve_pred16_t tail_p = vctp32q(cnt);

            uint32x4_t y = vldrbq_z_u32(y_src, tail_p);

            y = vmulq(y, vdupq_n_u32(0x01010100));
            y = vorrq(y, vdupq_n_u32(0x000000ff));

            vst1q_p(dst, y, tail_p);

            y_src += 4;
            dst += 4;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].a = 0xff;
            dst[j].r = y_src[j];
            dst[j].g = y_src[j];
            dst[j].b = y_src[j];
        }
    }
#endif

    return AIPL_ERR_OK;
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

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);

            y = vshrq(y, 4);
            y = vmulq(y, vdupq_n_u16(0x1110));
            y = vorrq(y, vdupq_n_u16(0x000f));

            vstrhq_p(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].rg = (y_src[j] & 0xf0) | (y_src[j] >> 4);
            dst[j].ba = (y_src[j] & 0xf0) | 0x0f;
        }
    }
#endif

    return AIPL_ERR_OK;
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

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);

            y = vshrq(y, 3);
            y = vmulq(y, vdupq_n_u16(0x0842));
            y = vorrq(y, vdupq_n_u16(0x0001));

            vstrhq_p(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].t = (y_src[j] & 0xf8) | (y_src[j] >> 5);
            dst[j].b = ((y_src[j] & 0xf8) << 3) | (y_src[j] >> 2) | 0x01;
        }
    }
#endif

    return AIPL_ERR_OK;
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

#ifdef AIPL_HELIUM_ACCELERATION
    uint16_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint16_t* dst = dst_ptr + i * pitch;

        for (int32_t cnt = width; cnt > 0; cnt -= 8)
        {
            mve_pred16_t tail_p = vctp16q(cnt);

            uint16x8_t y = vldrbq_z_u16(y_src, tail_p);

            y = vshrq(y, 3);
            y = vmulq(y, vdupq_n_u16(0x0841));

            vstrhq_p(dst, y, tail_p);

            y_src += 8;
            dst += 8;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[j].t = (y_src[j] & 0xf8) | (y_src[j] >> 5);
            dst[j].b = ((y_src[j] & 0xfc) << 3) | (y_src[j] >> 3);
        }
    }
#endif

    return AIPL_ERR_OK;

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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_alpha_to_rgb_x16(&pix, &y);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_alpha_to_24bit(dst, *(y_src++));
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (int32_t cnt = width; cnt > 0; cnt -= 16)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb8888_16px(&pix, src, tail_p);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[r_offset] = src->r;
            dst[g_offset] = src->g;
            dst[b_offset] = src->b;

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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
                                      r_offset, g_offset, b_offset);

            src += 16;
            dst += 48;
            cnt -= 16;
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_load_argb4444_px(dst, src,
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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb1555_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
                                      r_offset, g_offset, b_offset);

            src += 16;
            dst += 48;
            cnt -= 16;
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_load_argb1555_px(dst, src,
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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 4;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba8888_16px(&pix, src, tail_p);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
                                      r_offset, g_offset, b_offset);

            src += 64;
            dst += 48;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[r_offset] = src->r;
            dst[g_offset] = src->g;
            dst[b_offset] = src->b;

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
        int32_t cnt = width;
        const uint16_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba4444_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
                                      r_offset, g_offset, b_offset);

            src += 16;
            dst += 48;
            cnt -= 16;
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_load_rgba4444_px(dst, src,
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba5551_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_load_rgba5551_px(dst, src,
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_16px(&pix, (uint8_t*)src, tail_p);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_load_rgb565_px(dst, src,
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_in_offset, g_in_offset, b_in_offset);

            aipl_mve_store_24bit_16px((uint8_t*)dst, &pix, tail_p,
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
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst[r_out_offset] = src[r_in_offset];
            dst[g_out_offset] = src[g_in_offset];
            dst[b_out_offset] = src[b_in_offset];

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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            uint8x16_t alpha;
            aipl_mve_calculate_y_rgb_x16(&alpha, &pix);

            aipl_mve_store_alpha8_16px(dst, &alpha, tail_p);

            src += 48;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_pack_yuv_y(dst, src,
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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            aipl_mve_store_argb8888_16px(dst, &pix, tail_p);

            src += 48;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->a = 0xff;
            dst->r = src[r_offset];
            dst->g = src[g_offset];
            dst->b = src[b_offset];

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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            aipl_mve_store_argb4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 48;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_pack_argb4444_px(dst, src,
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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            aipl_mve_store_argb1555_16px((uint8_t*)dst, &pix, tail_p);

            src += 48;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_pack_argb1555_px(dst, src,
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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            aipl_mve_store_rgba8888_16px(dst, &pix, tail_p);

            src += 48;
            dst += 64;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            dst->r = src[r_offset];
            dst->g = src[g_offset];
            dst->b = src[b_offset];
            dst->a = 0xff;

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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            aipl_mve_store_rgba4444_16px((uint8_t*)dst, &pix, tail_p);

            src += 48;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_pack_rgba4444_px(dst, src,
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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            aipl_mve_store_rgba5551_16px((uint8_t*)dst, &pix, tail_p);

            src += 48;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_pack_rgba5551_px(dst, src,
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
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            aipl_mve_store_rgb565_16px((uint8_t*)dst, &pix, tail_p);

            src += 48;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_pack_rgb565_px(dst, src,
                                r_offset, g_offset, b_offset);

            src += 3;
            ++dst;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_I422 | AIPL_CONVERT_BGR888 & TO_I422)
aipl_error_t aipl_color_convert_24bit_to_i422(const void* input,
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

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size / 2;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width / 2;
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_offset_16px(&pix, src, 2, tail_p,
                                            r_offset, g_offset, b_offset);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 93;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                src += 3;
                ++y_dst;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_p(u_dst, u, tail_p);
                u_dst += 16;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_p(v_dst, v, tail_p);
                v_dst += 16;
            }
        }
    }
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_pack_yuv_y(y_dst++, src,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_u(u_dst++, src,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_v(v_dst++, src,
                            r_offset, g_offset, b_offset);
            src += 3;

            aipl_pack_yuv_y(y_dst++, src,
                            r_offset, g_offset, b_offset);
            src += 3;
       }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_RGB888 & TO_I444 | AIPL_CONVERT_BGR888 & TO_I444)
aipl_error_t aipl_color_convert_24bit_to_i444(const void* input,
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

    uint32_t yuv_size = width * height;
    uint8_t* y_ptr = output;
    uint8_t* u_ptr = output + yuv_size;
    uint8_t* v_ptr = u_ptr + yuv_size;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width;
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_16px(&pix, src, tail_p,
                                     r_offset, g_offset, b_offset);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_p(y_dst, y, tail_p);

            uint8x16_t u;
            aipl_mve_calculate_u_rgb_x16(&u, &pix);

            vstrbq_p(u_dst, u, tail_p);

            uint8x16_t v;
            aipl_mve_calculate_v_rgb_x16(&v, &pix);

            vstrbq_p(v_dst, v, tail_p);

            src += 48;
            y_dst += 16;
            u_dst += 16;
            v_dst += 16;
            cnt -= 16;
        }
    }
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * pitch;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_pack_yuv_y(y_dst++, src,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_u(u_dst++, src,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_v(v_dst++, src,
                            r_offset, g_offset, b_offset);
            src += 3;
        }
    }
#endif

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
            aipl_mve_load_argb8888_8px(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_calculate_y_rgb_x8(&y, &pix);

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
            aipl_mve_load_argb8888_offset_8px(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_calculate_u_rgb_x8(&u, &pix);

            uint16x8_t v;
            aipl_mve_calculate_v_rgb_x8(&v, &pix);

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
            y_dst[j] = ((66 * src[j].r + 129 * src[j].g + 25 * src[j].b + 128) >> 8) + 16;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i / 2 * width / 2;
        uint8_t* v_dst = v_ptr + i / 2 * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            uint32_t j2 = 2 * j;
            u_dst[j] = ((-38 * src[j2].r - 74 * src[j2].g + 112 * src[j2].b + 128) >> 8) + 128;
            v_dst[j] = ((112 * src[j2].r - 94 * src[j2].g - 18 * src[j2].b + 128) >> 8) + 128;
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
            aipl_mve_load_argb8888_8px(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_calculate_y_rgb_x8(&y, &pix);

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
            aipl_mve_load_argb8888_offset_8px(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_calculate_u_rgb_x8(&u, &pix);

            uint16x8_t v;
            aipl_mve_calculate_v_rgb_x8(&v, &pix);

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
            y_dst[j] = ((66 * src[j].r + 129 * src[j].g + 25 * src[j].b + 128) >> 8) + 16;
        }
    }

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb8888_px_t* src = src_ptr + i * pitch;
        uint8_t* u_dst = u_ptr + i * width / 2;
        uint8_t* v_dst = v_ptr + i * width / 2;

        for (uint32_t j = 0; j < width / 2; ++j)
        {
            uint32_t j2 = 2 * j;
            u_dst[j2] = ((-38 * src[j2].r - 74 * src[j2].g + 112 * src[j2].b + 128) >> 8) + 128;
            v_dst[j2] = ((112 * src[j2].r - 94 * src[j2].g - 18 * src[j2].b + 128) >> 8) + 128;
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
            aipl_mve_load_argb8888_8px(&pix, src, tail_p);

            uint16x8_t y;
            aipl_mve_calculate_y_rgb_x8(&y, &pix);

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
            aipl_mve_load_argb8888_offset_8px(&pix, src, 2, tail_p);

            uint16x8_t u;
            aipl_mve_calculate_u_rgb_x8(&u, &pix);

            uint16x8_t v;
            aipl_mve_calculate_v_rgb_x8(&v, &pix);

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
            uint32_t j2 = 2 * j;
            y_dst[j2] = ((66 * src[j].r + 129 * src[j].g + 25 * src[j].b + 128) >> 8) + 16;
            y_dst[j2 + 2] = ((66 * src[j + 1].r + 129 * src[j + 1].g + 25 * src[j + 1].b + 128) >> 8) + 16;
            u_dst[j2] = ((-38 * src[j].r - 74 * src[j].g + 112 * src[j].b + 128) >> 8) + 128;
            v_dst[j2] = ((112 * src[j].r - 94 * src[j].g - 18 * src[j].b + 128) >> 8) + 128;
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb4444_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_p(u_dst, u, tail_p);
                    u_dst += 16;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_p(v_dst, v, tail_p);
                    v_dst += 16;
                }
            }
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb4444_px_t* src0 = src_ptr + i * pitch;
        const aipl_argb4444_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_argb4444_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_argb4444_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_argb4444_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_argb4444_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb4444_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                    u_dst += 32;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                    v_dst += 32;
                }
            }
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb4444_px_t* src0 = src_ptr + i * pitch;
        const aipl_argb4444_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_argb4444_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            u_dst += 2;
            v_dst += 2;

            aipl_load_argb4444_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_argb4444_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_argb4444_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb4444_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 62;
                cnt -= 16;
            }
            else
            {
                ++src;
                y_dst += 2;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);
                u_dst += 64;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);
                v_dst += 64;
            }
        }
    }
#else
    const aipl_argb4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_argb4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_load_argb4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb1555_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_p(u_dst, u, tail_p);
                    u_dst += 16;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_p(v_dst, v, tail_p);
                    v_dst += 16;
                }
            }
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb1555_px_t* src0 = src_ptr + i * pitch;
        const aipl_argb1555_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_argb1555_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_argb1555_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_argb1555_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_argb1555_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb1555_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                    u_dst += 32;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                    v_dst += 32;
                }
            }
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_argb1555_px_t* src0 = src_ptr + i * pitch;
        const aipl_argb1555_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_argb1555_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            u_dst += 2;
            v_dst += 2;

            aipl_load_argb1555_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_argb1555_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_argb1555_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_argb1555_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 62;
                cnt -= 16;
            }
            else
            {
                ++src;
                y_dst += 2;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);
                u_dst += 64;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);
                v_dst += 64;
            }
        }
    }
#else
    const aipl_argb1555_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_argb1555_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_argb1555_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_load_argb1555_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint8_t* src = src_ptr + i * pitch * 4;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba8888_offset_16px(&pix, src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 124;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                src += 4;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_p(u_dst, u, tail_p);
                    u_dst += 16;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_p(v_dst, v, tail_p);
                    v_dst += 16;
                }
            }
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba8888_px_t* src0 = src_ptr + i * pitch;
        const aipl_rgba8888_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba8888_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_rgba8888_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_rgba8888_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_rgba8888_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint8_t* src = src_ptr + i * pitch * 4;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba8888_offset_16px(&pix, src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 124;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                src += 4;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                    u_dst += 32;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                    v_dst += 32;
                }
            }
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba8888_px_t* src0 = src_ptr + i * pitch;
        const aipl_rgba8888_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba8888_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            u_dst += 2;
            v_dst += 2;

            aipl_load_rgba8888_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_rgba8888_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_rgba8888_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint8_t* src = src_ptr + i * pitch * 4;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba8888_offset_16px(&pix, src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 124;
                y_dst += 62;
                cnt -= 16;
            }
            else
            {
                src += 4;
                y_dst += 2;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);
                u_dst += 64;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);
                v_dst += 64;
            }
        }
    }
#else
    const aipl_rgba8888_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba8888_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba8888_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_load_rgba8888_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba4444_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_p(u_dst, u, tail_p);
                    u_dst += 16;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_p(v_dst, v, tail_p);
                    v_dst += 16;
                }
            }
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba4444_px_t* src0 = src_ptr + i * pitch;
        const aipl_rgba4444_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba4444_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_rgba4444_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_rgba4444_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_rgba4444_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba4444_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                    u_dst += 32;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                    v_dst += 32;
                }
            }
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba4444_px_t* src0 = src_ptr + i * pitch;
        const aipl_rgba4444_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba4444_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            u_dst += 2;
            v_dst += 2;

            aipl_load_rgba4444_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_rgba4444_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_rgba4444_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba4444_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 62;
                cnt -= 16;
            }
            else
            {
                ++src;
                y_dst += 2;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);
                u_dst += 64;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);
                v_dst += 64;
            }
        }
    }
#else
    const aipl_rgba4444_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba4444_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_load_rgba4444_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba5551_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_p(u_dst, u, tail_p);
                    u_dst += 16;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_p(v_dst, v, tail_p);
                    v_dst += 16;
                }
            }
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba5551_px_t* src0 = src_ptr + i * pitch;
        const aipl_rgba5551_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba5551_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_rgba5551_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_rgba5551_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_rgba5551_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba5551_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                    u_dst += 32;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                    v_dst += 32;
                }
            }
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgba5551_px_t* src0 = src_ptr + i * pitch;
        const aipl_rgba5551_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba5551_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            u_dst += 2;
            v_dst += 2;

            aipl_load_rgba5551_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_rgba5551_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_rgba5551_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgba5551_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 62;
                cnt -= 16;
            }
            else
            {
                ++src;
                y_dst += 2;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);
                u_dst += 64;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);
                v_dst += 64;
            }
        }
    }
#else
    const aipl_rgba5551_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgba5551_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgba5551_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_load_rgba5551_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_offset_16px(&pix, src, 2, tail_p,
                                            r_offset, g_offset, b_offset);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 93;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                src += 3;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_p(u_dst, u, tail_p);
                    u_dst += 16;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_p(v_dst, v, tail_p);
                    v_dst += 16;
                }
            }
        }
    }
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src0 = src_ptr + i * pitch * 3;
        const uint8_t* src1 = src0 + pitch * 3;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_pack_yuv_y(y_dst0++, src0,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_u(u_dst++, src0,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_v(v_dst++, src0,
                            r_offset, g_offset, b_offset);
            src0 += 3;

            aipl_pack_yuv_y(y_dst0++, src0,
                            r_offset, g_offset, b_offset);
            src0 += 3;

            aipl_pack_yuv_y(y_dst1++, src1,
                            r_offset, g_offset, b_offset);
            src1 += 3;

            aipl_pack_yuv_y(y_dst1++, src1,
                            r_offset, g_offset, b_offset);
            src1 += 3;
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
        int32_t cnt = width / 2;
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_offset_16px(&pix, src, 2, tail_p,
                                            r_offset, g_offset, b_offset);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 93;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                src += 3;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                    u_dst += 32;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                    v_dst += 32;
                }
            }
        }
    }
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* src0 = src_ptr + i * pitch * 3;
        const uint8_t* src1 = src0 + pitch * 3;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_pack_yuv_y(y_dst0++, src0,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_u(u_dst, src0,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_v(v_dst, src0,
                            r_offset, g_offset, b_offset);
            u_dst += 2;
            v_dst += 2;
            src0 += 3;

            aipl_pack_yuv_y(y_dst0++, src0,
                            r_offset, g_offset, b_offset);
            src0 += 3;

            aipl_pack_yuv_y(y_dst1++, src1,
                            r_offset, g_offset, b_offset);
            src1 += 3;

            aipl_pack_yuv_y(y_dst1++, src1,
                            r_offset, g_offset, b_offset);
            src1 += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_BGR888 & TO_YUY2 | AIPL_CONVERT_BGR888 & TO_UYVY\
     | AIPL_CONVERT_RGB888 & TO_YUY2 | AIPL_CONVERT_RGB888 & TO_UYVY)
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
    if (input == NULL || y_ptr == NULL || u_ptr == NULL || v_ptr == NULL)
        return AIPL_ERR_NULL_POINTER;

#ifdef AIPL_HELIUM_ACCELERATION
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width / 2;
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_24bit_offset_16px(&pix, src, 2, tail_p,
                                            r_offset, g_offset, b_offset);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 93;
                y_dst += 62;
                cnt -= 16;
            }
            else
            {
                src += 3;
                y_dst += 2;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);
                u_dst += 64;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);
                v_dst += 64;
            }
        }
    }
#else
    const uint8_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* src = src_ptr + i * pitch * 3;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_pack_yuv_y(y_dst, src,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_u(u_dst, src,
                            r_offset, g_offset, b_offset);
            aipl_pack_yuv_v(v_dst, src,
                            r_offset, g_offset, b_offset);
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;
            src += 3;

            aipl_pack_yuv_y(y_dst, src,
                            r_offset, g_offset, b_offset);
            y_dst += 2;
            src += 3;
        }
    }
#endif

    return AIPL_ERR_OK;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y = vldrbq_gather_offset_z(y_src + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst + 3, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y_src += 32;
            u_src += 16;
            v_src += 16;
            dst += 96;
            cnt -= 16;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 2;
        const uint8_t* u_src = u_ptr + i * pitch / 2;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_yuv_to_24bit(dst, *(y_src++), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst += 3;

            aipl_yuv_to_24bit(dst, *(y_src++), *(u_src++), *(v_src++),
                              r_offset, g_offset, b_offset);
            dst += 3;
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
        int32_t cnt = width;

        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_16px(dst, &pix, tail_p,
                                      r_offset, g_offset, b_offset);

            y_src += 16;
            u_src += 16;
            v_src += 16;
            dst += 48;
            cnt -= 16;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        const uint8_t* v_src = v_ptr + i * pitch;
        const uint8_t* u_src = u_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; ++j)
        {
            aipl_yuv_to_24bit(dst, *(y_src++), *(u_src++), *(v_src++),
                              r_offset, g_offset, b_offset);
            dst += 3;
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_p(u_dst, u, tail_p);
                    u_dst += 16;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_p(v_dst, v, tail_p);
                    v_dst += 16;
                }
            }
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgb565_px_t* src0 = src_ptr + i * pitch;
        const aipl_rgb565_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 4;
        uint8_t* u_dst = u_ptr + i * pitch / 4;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgb565_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst++, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst++, px, 2, 1, 0);

            aipl_load_rgb565_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_rgb565_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_rgb565_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_2_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 31;
                cnt -= 16;
            }
            else
            {
                ++src;
                ++y_dst;

                if (!(i & 1))
                {
                    uint8x16_t u;
                    aipl_mve_calculate_u_rgb_x16(&u, &pix);

                    vstrbq_scatter_offset_p(u_dst, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);
                    u_dst += 32;

                    uint8x16_t v;
                    aipl_mve_calculate_v_rgb_x16(&v, &pix);

                    vstrbq_scatter_offset_p(v_dst, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);
                    v_dst += 32;
                }
            }
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const aipl_rgb565_px_t* src0 = src_ptr + i * pitch;
        const aipl_rgb565_px_t* src1 = src0 + pitch;

        uint8_t* y_dst0 = y_ptr + i * pitch;
        uint8_t* y_dst1 = y_dst0 + pitch;
        uint8_t* v_dst = v_ptr + i * pitch / 2;
        uint8_t* u_dst = u_ptr + i * pitch / 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgb565_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            u_dst += 2;
            v_dst += 2;

            aipl_load_rgb565_px(px, src0++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst0++, px, 2, 1, 0);

            aipl_load_rgb565_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);

            aipl_load_rgb565_px(px, src1++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;
        const uint16_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_load_rgb565_offset_16px(&pix, (uint8_t*)src, 2, tail_p);

            uint8x16_t y;
            aipl_mve_calculate_y_rgb_x16(&y, &pix);

            vstrbq_scatter_offset_p(y_dst, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            if (j++ & 1)
            {
                src += 31;
                y_dst += 62;
                cnt -= 16;
            }
            else
            {
                ++src;
                y_dst += 2;

                uint8x16_t u;
                aipl_mve_calculate_u_rgb_x16(&u, &pix);

                vstrbq_scatter_offset_p(u_dst, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);
                u_dst += 64;

                uint8x16_t v;
                aipl_mve_calculate_v_rgb_x16(&v, &pix);

                vstrbq_scatter_offset_p(v_dst, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);
                v_dst += 64;
            }
        }
    }
#else
    const aipl_rgb565_px_t* src_ptr = input;

    for (uint32_t i = 0; i < height; ++i)
    {
        const aipl_rgb565_px_t* src = src_ptr + i * pitch;

        uint8_t* y_dst = y_ptr + i * pitch * 2;
        uint8_t* v_dst = v_ptr + i * pitch * 2;
        uint8_t* u_dst = u_ptr + i * pitch * 2;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_load_rgb565_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
            aipl_pack_yuv_u(u_dst, px, 2, 1, 0);
            aipl_pack_yuv_v(v_dst, px, 2, 1, 0);
            y_dst += 2;
            u_dst += 4;
            v_dst += 4;

            aipl_load_rgb565_px(px, src++, 2, 1, 0);
            aipl_pack_yuv_y(y_dst, px, 2, 1, 0);
            y_dst += 2;
        }
    }
#endif

    return AIPL_ERR_OK;
}
#endif

#if (AIPL_CONVERT_YV12 & TO_ALPHA8_I400 | AIPL_CONVERT_I420 & TO_ALPHA8_I400\
     | AIPL_CONVERT_NV12 & TO_ALPHA8_I400 | AIPL_CONVERT_NV21 & TO_ALPHA8_I400)
aipl_error_t aipl_color_convert_yuv_planar_to_alpha8(const void* input,
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
        const uint8_t* y_src = y_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * width;

        uint32_t j = 0;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_z_u8(y_src, tail_p);

            vstrbq_p_u8(dst, y, tail_p);

            y_src += 16;
            dst += 16;
            cnt -= 16;
        }
    }
#else
    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch;
        uint8_t* dst = dst_ptr + i * pitch;

        memcpy(dst, y_src, width);
    }
#endif

    return AIPL_ERR_OK;
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
        uint8_t* dst = dst_ptr + i * pitch;

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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        uint8_t* dst0 = dst_ptr + i * pitch * 4;
        uint8_t* dst1 = dst0 + pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst0 + 4, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst1 + 4, &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 16;
            v_src += 16;
            dst0 += 128;
            dst1 += 128;
            cnt -= 16;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        aipl_argb8888_px_t* dst0 = dst_ptr + i * pitch;
        aipl_argb8888_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            dst0->a = 0xff;
            dst0->r = px[2];
            dst0->g = px[1];
            dst0->b = px[0];
            ++dst0;

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            dst0->a = 0xff;
            dst0->r = px[2];
            dst0->g = px[1];
            dst0->b = px[0];
            ++dst0;

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            dst1->a = 0xff;
            dst1->r = px[2];
            dst1->g = px[1];
            dst1->b = px[0];
            ++dst1;

            aipl_yuv_to_24bit(px, *(y_src1++), *(u_src++), *(v_src++), 2, 1, 0);
            dst1->a = 0xff;
            dst1->r = px[2];
            dst1->g = px[1];
            dst1->b = px[0];
            ++dst1;
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint8_t* dst0 = dst_ptr + i * pitch * 4;
        uint8_t* dst1 = dst0 + pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst0 + 4, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst1 + 4, &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 32;
            v_src += 32;
            dst0 += 128;
            dst1 += 128;
            cnt -= 16;
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
        aipl_argb8888_px_t* dst0 = dst_ptr + i * pitch;
        aipl_argb8888_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            dst0->a = 0xff;
            dst0->r = px[2];
            dst0->g = px[1];
            dst0->b = px[0];
            ++dst0;

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            dst0->a = 0xff;
            dst0->r = px[2];
            dst0->g = px[1];
            dst0->b = px[0];
            ++dst0;

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            dst1->a = 0xff;
            dst1->r = px[2];
            dst1->g = px[1];
            dst1->b = px[0];
            ++dst1;

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            dst1->a = 0xff;
            dst1->r = px[2];
            dst1->g = px[1];
            dst1->b = px[0];
            ++dst1;

            u_src += 2;
            v_src += 2;
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb8888_offset_16px(dst + 4, &pix, 2, tail_p);

            y_src += 64;
            u_src += 64;
            v_src += 64;
            dst += 128;
            cnt -= 16;
        }
    }
#else
    aipl_argb8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_argb8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *y_src, *u_src, *v_src, 2, 1, 0);
            dst->a = 0xff;
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            ++dst;

            aipl_yuv_to_24bit(px, *(y_src + 2), *u_src, *v_src, 2, 1, 0);
            dst->a = 0xff;
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            ++dst;

            y_src += 4;
            u_src += 4;
            v_src += 4;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 16;
            v_src += 16;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        aipl_argb4444_px_t* dst0 = dst_ptr + i * pitch;
        aipl_argb4444_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_argb4444_px(dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 32;
            v_src += 32;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
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
        aipl_argb4444_px_t* dst0 = dst_ptr + i * pitch;
        aipl_argb4444_px_t* dst1 = dst0 + pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst1++, px, 2, 1, 0);

            u_src += 2;
            v_src += 2;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb4444_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 64;
            u_src += 64;
            v_src += 64;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_argb4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_argb4444_px_t* dst = dst_ptr + i * pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *y_src, *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src + 2), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb4444_px(dst++, px, 2, 1, 0);

            y_src += 4;
            u_src += 4;
            v_src += 4;
        }
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 16;
            v_src += 16;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        aipl_argb1555_px_t* dst0 = dst_ptr + i * pitch;
        aipl_argb1555_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_argb1555_px(dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 32;
            v_src += 32;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
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
        aipl_argb1555_px_t* dst0 = dst_ptr + i * pitch;
        aipl_argb1555_px_t* dst1 = dst0 + pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst1++, px, 2, 1, 0);

            u_src += 2;
            v_src += 2;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_argb1555_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 64;
            u_src += 64;
            v_src += 64;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_argb1555_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_argb1555_px_t* dst = dst_ptr + i * pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *y_src, *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src + 2), *u_src, *v_src, 2, 1, 0);
            aipl_pack_argb1555_px(dst++, px, 2, 1, 0);

            y_src += 4;
            u_src += 4;
            v_src += 4;
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        uint8_t* dst0 = dst_ptr + i * pitch * 4;
        uint8_t* dst1 = dst0 + pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst0 + 4, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst1 + 4, &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 16;
            v_src += 16;
            dst0 += 128;
            dst1 += 128;
            cnt -= 16;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        aipl_rgba8888_px_t* dst0 = dst_ptr + i * pitch;
        aipl_rgba8888_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            dst0->r = px[2];
            dst0->g = px[1];
            dst0->b = px[0];
            dst0->a = 0xff;
            ++dst0;

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            dst0->r = px[2];
            dst0->g = px[1];
            dst0->b = px[0];
            dst0->a = 0xff;
            ++dst0;

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            dst1->r = px[2];
            dst1->g = px[1];
            dst1->b = px[0];
            dst1->a = 0xff;
            ++dst1;

            aipl_yuv_to_24bit(px, *(y_src1++), *(u_src++), *(v_src++), 2, 1, 0);
            dst1->r = px[2];
            dst1->g = px[1];
            dst1->b = px[0];
            dst1->a = 0xff;
            ++dst1;
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint8_t* dst0 = dst_ptr + i * pitch * 4;
        uint8_t* dst1 = dst0 + pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst0 + 4, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst1 + 4, &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 32;
            v_src += 32;
            dst0 += 128;
            dst1 += 128;
            cnt -= 16;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        aipl_rgba8888_px_t* dst0 = dst_ptr + i * pitch;
        aipl_rgba8888_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            dst0->r = px[2];
            dst0->g = px[1];
            dst0->b = px[0];
            dst0->a = 0xff;
            ++dst0;

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            dst0->r = px[2];
            dst0->g = px[1];
            dst0->b = px[0];
            dst0->a = 0xff;
            ++dst0;

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            dst1->r = px[2];
            dst1->g = px[1];
            dst1->b = px[0];
            dst1->a = 0xff;
            ++dst1;

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            dst1->r = px[2];
            dst1->g = px[1];
            dst1->b = px[0];
            dst1->a = 0xff;
            ++dst1;

            u_src += 2;
            v_src += 2;
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
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        uint8_t* dst = dst_ptr + i * pitch * 4;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba8888_offset_16px(dst + 4, &pix, 2, tail_p);

            y_src += 64;
            u_src += 64;
            v_src += 64;
            dst += 128;
            cnt -= 16;
        }
    }
#else
    aipl_rgba8888_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_rgba8888_px_t* dst = dst_ptr + i * pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *y_src, *u_src, *v_src, 2, 1, 0);
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            dst->a = 0xff;
            ++dst;

            aipl_yuv_to_24bit(px, *(y_src + 2), *u_src, *v_src, 2, 1, 0);
            dst->r = px[2];
            dst->g = px[1];
            dst->b = px[0];
            dst->a = 0xff;
            ++dst;

            y_src += 4;
            u_src += 4;
            v_src += 4;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 16;
            v_src += 16;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        aipl_rgba4444_px_t* dst0 = dst_ptr + i * pitch;
        aipl_rgba4444_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgba4444_px(dst1++, px, 2, 1, 0);
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
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 32;
            v_src += 32;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
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
        aipl_rgba4444_px_t* dst0 = dst_ptr + i * pitch;
        aipl_rgba4444_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst1++, px, 2, 1, 0);

            u_src += 2;
            v_src += 2;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba4444_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 64;
            u_src += 64;
            v_src += 64;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgba4444_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_rgba4444_px_t* dst = dst_ptr + i * pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *y_src, *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src + 2), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba4444_px(dst++, px, 2, 1, 0);

            y_src += 4;
            u_src += 4;
            v_src += 4;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 16;
            v_src += 16;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        aipl_rgba5551_px_t* dst0 = dst_ptr + i * pitch;
        aipl_rgba5551_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgba5551_px(dst1++, px, 2, 1, 0);
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
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 32;
            v_src += 32;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
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
        aipl_rgba5551_px_t* dst0 = dst_ptr + i * pitch;
        aipl_rgba5551_px_t* dst1 = dst0 + pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst1++, px, 2, 1, 0);

            u_src += 2;
            v_src += 2;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgba5551_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 64;
            u_src += 64;
            v_src += 64;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgba5551_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_rgba5551_px_t* dst = dst_ptr + i * pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *y_src, *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src + 2), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgba5551_px(dst++, px, 2, 1, 0);

            y_src += 4;
            u_src += 4;
            v_src += 4;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        uint8_t* dst0 = dst_ptr + i * pitch * 3;
        uint8_t* dst1 = dst0 + pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst0, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst0 + 3, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst1, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst1 + 3, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 16;
            v_src += 16;
            dst0 += 96;
            dst1 += 96;
            cnt -= 16;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        uint8_t* dst0 = dst_ptr + i * pitch * 3;
        uint8_t* dst1 = dst0 + pitch * 3;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_yuv_to_24bit(dst0, *(y_src0++), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst0 += 3;

            aipl_yuv_to_24bit(dst0, *(y_src0++), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst0 += 3;

            aipl_yuv_to_24bit(dst1, *(y_src1++), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst1 += 3;

            aipl_yuv_to_24bit(dst1, *(y_src1++), *(u_src++), *(v_src++),
                              r_offset, g_offset, b_offset);
            dst1 += 3;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint8_t* dst0 = dst_ptr + i * pitch * 3;
        uint8_t* dst1 = dst0 + pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst0, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst0 + 3, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst1, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst1 + 3, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 32;
            v_src += 32;
            dst0 += 96;
            dst1 += 96;
            cnt -= 16;
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
        uint8_t* dst0 = dst_ptr + i * pitch * 3;
        uint8_t* dst1 = dst0 + pitch * 3;

        for (uint32_t j = 0; j < width; j += 2)
        {
            aipl_yuv_to_24bit(dst0, *(y_src0++), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst0 += 3;

            aipl_yuv_to_24bit(dst0, *(y_src0++), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst0 += 3;

            aipl_yuv_to_24bit(dst1, *(y_src1++), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst1 += 3;

            aipl_yuv_to_24bit(dst1, *(y_src1++), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst1 += 3;

            u_src += 2;
            v_src += 2;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y = vldrbq_gather_offset_z(y_src + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_24bit_offset_16px(dst + 3, &pix, 2, tail_p,
                                             r_offset, g_offset, b_offset);

            y_src += 64;
            u_src += 64;
            v_src += 64;
            dst += 96;
            cnt -= 16;
        }
    }
#else
    uint8_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint8_t* dst = dst_ptr + i * pitch * 3;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(dst, *y_src, *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst += 3;

            aipl_yuv_to_24bit(dst, *(y_src + 2), *u_src, *v_src,
                              r_offset, g_offset, b_offset);
            dst += 3;

            y_src += 4;
            u_src += 4;
            v_src += 4;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_z_u8(u_src, tail_p);
            uint8x16_t v = vldrbq_z_u8(v_src, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 16;
            v_src += 16;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; i += 2)
    {
        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* v_src = v_ptr + i * pitch / 4;
        const uint8_t* u_src = u_ptr + i * pitch / 4;
        aipl_rgb565_px_t* dst0 = dst_ptr + i * pitch;
        aipl_rgb565_px_t* dst1 = dst0 + pitch;

        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *(u_src++), *(v_src++), 2, 1, 0);
            aipl_pack_rgb565_px(dst1++, px, 2, 1, 0);
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
        int32_t cnt = width / 2;

        const uint8_t* y_src0 = y_ptr + i * pitch;
        const uint8_t* y_src1 = y_src0 + pitch;
        const uint8_t* u_src = u_ptr + i / 2 * pitch;
        const uint8_t* v_src = v_ptr + i / 2 * pitch;
        uint16_t* dst0 = dst_ptr + i * pitch;
        uint16_t* dst1 = dst0 + pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src0, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)dst0, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src0 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)(dst0 + 1), &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)dst1, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src1 + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)(dst1 + 1), &pix, 2, tail_p);

            y_src0 += 32;
            y_src1 += 32;
            u_src += 32;
            v_src += 32;
            dst0 += 32;
            dst1 += 32;
            cnt -= 16;
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
        aipl_rgb565_px_t* dst0 = dst_ptr + i * pitch;
        aipl_rgb565_px_t* dst1 = dst0 + pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src0++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst0++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst1++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src1++), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst1++, px, 2, 1, 0);

            u_src += 2;
            v_src += 2;
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
        int32_t cnt = width / 2;

        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        uint16_t* dst = dst_ptr + i * pitch;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t u = vldrbq_gather_offset_z(u_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            uint8x16_t v = vldrbq_gather_offset_z(v_src, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_rgb_x16_t pix;
            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)dst, &pix, 2, tail_p);

            y = vldrbq_gather_offset_z(y_src + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);

            aipl_mve_convert_yuv_to_rgb_x16(&pix, &y, &u, &v);

            aipl_mve_store_rgb565_offset_16px((uint8_t*)(dst + 1), &pix, 2, tail_p);

            y_src += 64;
            u_src += 64;
            v_src += 64;
            dst += 32;
            cnt -= 16;
        }
    }
#else
    aipl_rgb565_px_t* dst_ptr = output;

    for (uint32_t i = 0; i < height; ++i)
    {
        const uint8_t* y_src = y_ptr + i * pitch * 2;
        const uint8_t* v_src = v_ptr + i * pitch * 2;
        const uint8_t* u_src = u_ptr + i * pitch * 2;
        aipl_rgb565_px_t* dst = dst_ptr + i * pitch;


        for (uint32_t j = 0; j < width; j += 2)
        {
            uint8_t px[3];
            aipl_yuv_to_24bit(px, *y_src, *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst++, px, 2, 1, 0);

            aipl_yuv_to_24bit(px, *(y_src + 2), *u_src, *v_src, 2, 1, 0);
            aipl_pack_rgb565_px(dst++, px, 2, 1, 0);

            y_src += 4;
            u_src += 4;
            v_src += 4;
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

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_d, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y = vldrbq_gather_offset_z(y_s + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_d + 2, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            uint8x16_t u = vldrbq_z_u8(u_s, tail_p);
            vstrbq_scatter_offset_p(u_d, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_z_u8(v_s, tail_p);
            vstrbq_scatter_offset_p(v_d, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);

            y_s += 32;
            u_s += 16;
            v_s += 16;
            y_d += 64;
            u_d += 64;
            v_d += 64;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_d[j * 4] = y_s[j * 2];
            y_d[j * 4 + 2] = y_s[j * 2 + 1];
            u_d[j * 4] = u_s[j];
            v_d[j * 4] = v_s[j];
        }
#endif
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
#ifdef AIPL_HELIUM_ACCELERATION
            int32_t cnt = width / 2;

            uint32_t j = 0;
            while (cnt > 0)
            {
                mve_pred16_t tail_p = vctp8q(cnt);

                uint8x16_t u = vldrbq_gather_offset_z(u_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vstrbq_scatter_offset_p(u_d, AIPL_2_BYTE_OFFSETS_U8, u, tail_p);

                uint8x16_t v = vldrbq_gather_offset_z(v_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);

                vstrbq_scatter_offset_p(v_d, AIPL_2_BYTE_OFFSETS_U8, v, tail_p);

                u_s += 32;
                v_s += 32;
                u_d += 32;
                v_d += 32;
                cnt -= 16;
            }
#else
            for (uint32_t j = 0; j < width; j += 2)
            {
                u_d[j] = u_s[j];
                v_d[j] = v_s[j];
            }
#endif
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

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;

        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_d, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y = vldrbq_gather_offset_z(y_s + 1, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_d + 2, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            uint8x16_t u = vldrbq_gather_offset_z(u_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(u_d, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_s, AIPL_2_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(v_d, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);

            y_s += 32;
            u_s += 32;
            v_s += 32;
            y_d += 64;
            u_d += 64;
            v_d += 64;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width / 2; ++j)
        {
            y_d[j * 4] = y_s[j * 2];
            y_d[j * 4 + 2] = y_s[j * 2 + 1];
            u_d[j * 4] = u_s[j * 2];
            v_d[j * 4] = v_s[j * 2];
        }
#endif
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

#ifdef AIPL_HELIUM_ACCELERATION
        int32_t cnt = width / 2;
        while (cnt > 0)
        {
            mve_pred16_t tail_p = vctp8q(cnt);

            uint8x16_t y = vldrbq_gather_offset_z(y_s, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_d, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            y = vldrbq_gather_offset_z(y_s + 2, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(y_d + 2, AIPL_4_BYTE_OFFSETS_U8, y, tail_p);

            uint8x16_t u = vldrbq_gather_offset_z(u_s, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(u_d, AIPL_4_BYTE_OFFSETS_U8, u, tail_p);

            uint8x16_t v = vldrbq_gather_offset_z(v_s, AIPL_4_BYTE_OFFSETS_U8, tail_p);
            vstrbq_scatter_offset_p(v_d, AIPL_4_BYTE_OFFSETS_U8, v, tail_p);

            y_s += 64;
            u_s += 64;
            v_s += 64;
            y_d += 64;
            u_d += 64;
            v_d += 64;
            cnt -= 16;
        }
#else
        for (uint32_t j = 0; j < width * 2; j += 4)
        {
            y_d[j] = y_s[j];
            y_d[j + 2] = y_s[j + 2];
            u_d[j] = u_s[j];
            v_d[j] = v_s[j];
        }
#endif
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
