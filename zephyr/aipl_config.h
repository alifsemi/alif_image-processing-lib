/**
 * @file aipl_config.h
 *
 */

 #ifndef AIPL_CONFIG_H
 #define AIPL_CONFIG_H

 #ifdef __cplusplus
 extern "C" {
 #endif

 #ifndef BIT
 #define BIT(x)          (1u<<(x))
 #endif

 /**
  * Custom video alloc setting
  *
  * Options:
  *  0 - use default malloc()
  *  1 - the allocation and free functions should be
  *      provided by the user
  */
 #ifdef CONFIG_AIPL_CUSTOM_VIDEO_ALLOC
 #define AIPL_CUSTOM_VIDEO_ALLOC     1
 #else
 #define AIPL_CUSTOM_VIDEO_ALLOC     0
 #endif

 /**
 * Custom cache management functions
 *
 * Options:
 *  0 - use default functions from DFP
 *  1 - the cache invalidate and clean functions must be
 *      provided by the user
 */
 #ifdef CONFIG_AIPL_CUSTOM_CACHE
 #define AIPL_CUSTOM_CACHE           1
 #else
 #define AIPL_CUSTOM_CACHE           0
 #endif

 /**
  * Custom D/AVE2D initialization function
  *
  * Options:
  *  0 - use the default aipl_dave2d_init()
  *  1 - use user-defined initialization function;
  *      user must also provide aipl_dave2d_handle()
  */
 #ifdef CONFIG_AIPL_CUSTOM_DAVE2D_INIT
 #define AIPL_CUSTOM_DAVE2D_INIT     1
 #else
 #define AIPL_CUSTOM_DAVE2D_INIT     0
 #endif

 /**
  * Set the library to always choose D/AVE2D implementation
  * over others even if it's slower in order to reduce CPU load
  *
  * The setting only takes effect if D/AVE2D acceleration is turned on
  *
  */
 #ifdef CONFIG_AIPL_OPTIMIZE_CPU_LOAD
 #define AIPL_OPTIMIZE_CPU_LOAD
 #endif

 /**
  * Enable color format conversions
  *
  * Constants TO_<COLOR_FORMAT> can be used to
  * define conversions for each individual color format
  */
 #define TO_ALPHA8_I400  BIT(0)
 #define TO_ARGB8888     BIT(1)
 #define TO_ARGB4444     BIT(2)
 #define TO_ARGB1555     BIT(3)
 #define TO_RGBA8888     BIT(4)
 #define TO_RGBA4444     BIT(5)
 #define TO_RGBA5551     BIT(6)
 #define TO_BGR888       BIT(7)
 #define TO_RGB888       BIT(8)
 #define TO_RGB565       BIT(9)
 #define TO_YV12         BIT(10)
 #define TO_I420         BIT(11)
 #define TO_I422         BIT(12)
 #define TO_I444         BIT(13)
 #define TO_NV12         BIT(14)
 #define TO_NV21         BIT(15)
 #define TO_YUY2         BIT(16)
 #define TO_UYVY         BIT(17)
 #define TO_ALL          (TO_ALPHA8_I400 | TO_ARGB8888 | TO_ARGB4444\
                          | TO_ARGB1555 | TO_RGBA8888 | TO_RGBA4444\
                          | TO_RGBA5551 | TO_BGR888 | TO_RGB888 | TO_RGB565\
                          | TO_YV12 | TO_I420 | TO_I422 | TO_I444\
                          | TO_NV12 | TO_NV21 | TO_YUY2 | TO_UYVY)

 /**
  * Enable D/AVE2D acceleration
  */
 #ifdef CONFIG_AIPL_DAVE2D_ACCELERATION
 #define AIPL_DAVE2D_ACCELERATION
 #endif

 /**
  * Enable Helium acceleration
  */
 #ifdef CONFIG_AIPL_HELIUM_ACCELERATION
 #define AIPL_HELIUM_ACCELERATION
 #endif

 /**
  * Include every default function implementation even if it's suboptimal
  */
 #ifdef CONFIG_AIPL_INCLUDE_ALL_DEFAULT
 #define AIPL_INCLUDE_ALL_DEFAULT
 #endif

 /**
  * Include every Helium function implementation even if it's suboptimal
  */
 #ifdef CONFIG_AIPL_INCLUDE_ALL_HELIUM
 #define AIPL_INCLUDE_ALL_HELIUM
 #endif

 /**
  * Set conversion from each color format using
  * the constants above
  *
  * To completely disable color conversion the marco should
  * be defined as 0
  */
 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400
 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_ARGB8888
 #define _ALPHA8_I400_TO_ARGB8888    TO_ARGB8888
 #else
 #define _ALPHA8_I400_TO_ARGB8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_ARGB4444
 #define _ALPHA8_I400_TO_ARGB4444    TO_ARGB4444
 #else
 #define _ALPHA8_I400_TO_ARGB4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_ARGB1555
 #define _ALPHA8_I400_TO_ARGB1555    TO_ARGB1555
 #else
 #define _ALPHA8_I400_TO_ARGB1555    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_RGBA8888
 #define _ALPHA8_I400_TO_RGBA8888    TO_RGBA8888
 #else
 #define _ALPHA8_I400_TO_RGBA8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_RGBA4444
 #define _ALPHA8_I400_TO_RGBA4444    TO_RGBA4444
 #else
 #define _ALPHA8_I400_TO_RGBA4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_RGBA5551
 #define _ALPHA8_I400_TO_RGBA5551    TO_RGBA5551
 #else
 #define _ALPHA8_I400_TO_RGBA5551    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_RGB888
 #define _ALPHA8_I400_TO_RGB888      TO_RGB888
 #else
 #define _ALPHA8_I400_TO_RGB888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_BGR888
 #define _ALPHA8_I400_TO_BGR888      TO_BGR888
 #else
 #define _ALPHA8_I400_TO_BGR888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_RGB565
 #define _ALPHA8_I400_TO_RGB565      TO_RGB565
 #else
 #define _ALPHA8_I400_TO_RGB565      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_YV12
 #define _ALPHA8_I400_TO_YV12        TO_YV12
 #else
 #define _ALPHA8_I400_TO_YV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_I420
 #define _ALPHA8_I400_TO_I420        TO_I420
 #else
 #define _ALPHA8_I400_TO_I420        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_I422
 #define _ALPHA8_I400_TO_I422        TO_I422
 #else
 #define _ALPHA8_I400_TO_I422        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_I444
 #define _ALPHA8_I400_TO_I444        TO_I444
 #else
 #define _ALPHA8_I400_TO_I444        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_NV12
 #define _ALPHA8_I400_TO_NV12        TO_NV12
 #else
 #define _ALPHA8_I400_TO_NV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_NV21
 #define _ALPHA8_I400_TO_NV21        TO_NV21
 #else
 #define _ALPHA8_I400_TO_NV21        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_YUY2
 #define _ALPHA8_I400_TO_YUY2        TO_YUY2
 #else
 #define _ALPHA8_I400_TO_YUY2        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ALPHA8_I400_TO_UYVY
 #define _ALPHA8_I400_TO_UYVY        TO_UYVY
 #else
 #define _ALPHA8_I400_TO_UYVY        0
 #endif

 #define AIPL_CONVERT_ALPHA8_I400    (_ALPHA8_I400_TO_ARGB8888\
                                      | _ALPHA8_I400_TO_ARGB4444\
                                      | _ALPHA8_I400_TO_ARGB1555\
                                      | _ALPHA8_I400_TO_RGBA8888\
                                      | _ALPHA8_I400_TO_RGBA4444\
                                      | _ALPHA8_I400_TO_RGBA5551\
                                      | _ALPHA8_I400_TO_RGB888\
                                      | _ALPHA8_I400_TO_BGR888\
                                      | _ALPHA8_I400_TO_RGB565\
                                      | _ALPHA8_I400_TO_YV12\
                                      | _ALPHA8_I400_TO_I420\
                                      | _ALPHA8_I400_TO_I422\
                                      | _ALPHA8_I400_TO_I444\
                                      | _ALPHA8_I400_TO_NV12\
                                      | _ALPHA8_I400_TO_NV21\
                                      | _ALPHA8_I400_TO_YUY2\
                                      | _ALPHA8_I400_TO_UYVY)
 #else
 #define AIPL_CONVERT_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888
 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_ALPHA8_I400
 #define _ARGB8888_TO_ALPHA8_I400 TO_ALPHA8_I400
 #else
 #define _ARGB8888_TO_ALPHA8_I400 0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_ARGB4444
 #define _ARGB8888_TO_ARGB4444    TO_ARGB4444
 #else
 #define _ARGB8888_TO_ARGB4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_ARGB1555
 #define _ARGB8888_TO_ARGB1555    TO_ARGB1555
 #else
 #define _ARGB8888_TO_ARGB1555    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_RGBA8888
 #define _ARGB8888_TO_RGBA8888    TO_RGBA8888
 #else
 #define _ARGB8888_TO_RGBA8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_RGBA4444
 #define _ARGB8888_TO_RGBA4444    TO_RGBA4444
 #else
 #define _ARGB8888_TO_RGBA4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_RGBA5551
 #define _ARGB8888_TO_RGBA5551    TO_RGBA5551
 #else
 #define _ARGB8888_TO_RGBA5551    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_RGB888
 #define _ARGB8888_TO_RGB888      TO_RGB888
 #else
 #define _ARGB8888_TO_RGB888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_BGR888
 #define _ARGB8888_TO_BGR888      TO_BGR888
 #else
 #define _ARGB8888_TO_BGR888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_RGB565
 #define _ARGB8888_TO_RGB565      TO_RGB565
 #else
 #define _ARGB8888_TO_RGB565      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_YV12
 #define _ARGB8888_TO_YV12        TO_YV12
 #else
 #define _ARGB8888_TO_YV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_I420
 #define _ARGB8888_TO_I420        TO_I420
 #else
 #define _ARGB8888_TO_I420        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_I422
 #define _ARGB8888_TO_I422        TO_I422
 #else
 #define _ARGB8888_TO_I422        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_I444
 #define _ARGB8888_TO_I444        TO_I444
 #else
 #define _ARGB8888_TO_I444        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_NV12
 #define _ARGB8888_TO_NV12        TO_NV12
 #else
 #define _ARGB8888_TO_NV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_NV21
 #define _ARGB8888_TO_NV21        TO_NV21
 #else
 #define _ARGB8888_TO_NV21        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_YUY2
 #define _ARGB8888_TO_YUY2        TO_YUY2
 #else
 #define _ARGB8888_TO_YUY2        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB8888_TO_UYVY
 #define _ARGB8888_TO_UYVY        TO_UYVY
 #else
 #define _ARGB8888_TO_UYVY        0
 #endif

 #define AIPL_CONVERT_ARGB8888    (_ARGB8888_TO_ALPHA8_I400\
                                      | _ARGB8888_TO_ARGB4444\
                                      | _ARGB8888_TO_ARGB1555\
                                      | _ARGB8888_TO_RGBA8888\
                                      | _ARGB8888_TO_RGBA4444\
                                      | _ARGB8888_TO_RGBA5551\
                                      | _ARGB8888_TO_RGB888\
                                      | _ARGB8888_TO_BGR888\
                                      | _ARGB8888_TO_RGB565\
                                      | _ARGB8888_TO_YV12\
                                      | _ARGB8888_TO_I420\
                                      | _ARGB8888_TO_I422\
                                      | _ARGB8888_TO_I444\
                                      | _ARGB8888_TO_NV12\
                                      | _ARGB8888_TO_NV21\
                                      | _ARGB8888_TO_YUY2\
                                      | _ARGB8888_TO_UYVY)
 #else
 #define AIPL_CONVERT_ARGB8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444
 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_ALPHA8_I400
 #define _ARGB4444_TO_ALPHA8_I400 TO_ALPHA8_I400
 #else
 #define _ARGB4444_TO_ALPHA8_I400 0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_ARGB8888
 #define _ARGB4444_TO_ARGB8888    TO_ARGB8888
 #else
 #define _ARGB4444_TO_ARGB8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_ARGB1555
 #define _ARGB4444_TO_ARGB1555    TO_ARGB1555
 #else
 #define _ARGB4444_TO_ARGB1555    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_RGBA8888
 #define _ARGB4444_TO_RGBA8888    TO_RGBA8888
 #else
 #define _ARGB4444_TO_RGBA8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_RGBA4444
 #define _ARGB4444_TO_RGBA4444    TO_RGBA4444
 #else
 #define _ARGB4444_TO_RGBA4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_RGBA5551
 #define _ARGB4444_TO_RGBA5551    TO_RGBA5551
 #else
 #define _ARGB4444_TO_RGBA5551    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_RGB888
 #define _ARGB4444_TO_RGB888      TO_RGB888
 #else
 #define _ARGB4444_TO_RGB888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_BGR888
 #define _ARGB4444_TO_BGR888      TO_BGR888
 #else
 #define _ARGB4444_TO_BGR888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_RGB565
 #define _ARGB4444_TO_RGB565      TO_RGB565
 #else
 #define _ARGB4444_TO_RGB565      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_YV12
 #define _ARGB4444_TO_YV12        TO_YV12
 #else
 #define _ARGB4444_TO_YV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_I420
 #define _ARGB4444_TO_I420        TO_I420
 #else
 #define _ARGB4444_TO_I420        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_I422
 #define _ARGB4444_TO_I422        TO_I422
 #else
 #define _ARGB4444_TO_I422        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_I444
 #define _ARGB4444_TO_I444        TO_I444
 #else
 #define _ARGB4444_TO_I444        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_NV12
 #define _ARGB4444_TO_NV12        TO_NV12
 #else
 #define _ARGB4444_TO_NV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_NV21
 #define _ARGB4444_TO_NV21        TO_NV21
 #else
 #define _ARGB4444_TO_NV21        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_YUY2
 #define _ARGB4444_TO_YUY2        TO_YUY2
 #else
 #define _ARGB4444_TO_YUY2        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB4444_TO_UYVY
 #define _ARGB4444_TO_UYVY        TO_UYVY
 #else
 #define _ARGB4444_TO_UYVY        0
 #endif

 #define AIPL_CONVERT_ARGB4444    (_ARGB4444_TO_ALPHA8_I400\
                                      | _ARGB4444_TO_ARGB8888\
                                      | _ARGB4444_TO_ARGB1555\
                                      | _ARGB4444_TO_RGBA8888\
                                      | _ARGB4444_TO_RGBA4444\
                                      | _ARGB4444_TO_RGBA5551\
                                      | _ARGB4444_TO_RGB888\
                                      | _ARGB4444_TO_BGR888\
                                      | _ARGB4444_TO_RGB565\
                                      | _ARGB4444_TO_YV12\
                                      | _ARGB4444_TO_I420\
                                      | _ARGB4444_TO_I422\
                                      | _ARGB4444_TO_I444\
                                      | _ARGB4444_TO_NV12\
                                      | _ARGB4444_TO_NV21\
                                      | _ARGB4444_TO_YUY2\
                                      | _ARGB4444_TO_UYVY)
 #else
 #define AIPL_CONVERT_ARGB4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555
 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_ALPHA8_I400
 #define _ARGB1555_TO_ALPHA8_I400 TO_ALPHA8_I400
 #else
 #define _ARGB1555_TO_ALPHA8_I400 0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_ARGB8888
 #define _ARGB1555_TO_ARGB8888    TO_ARGB8888
 #else
 #define _ARGB1555_TO_ARGB8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_ARGB4444
 #define _ARGB1555_TO_ARGB4444    TO_ARGB4444
 #else
 #define _ARGB1555_TO_ARGB4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_RGBA8888
 #define _ARGB1555_TO_RGBA8888    TO_RGBA8888
 #else
 #define _ARGB1555_TO_RGBA8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_RGBA4444
 #define _ARGB1555_TO_RGBA4444    TO_RGBA4444
 #else
 #define _ARGB1555_TO_RGBA4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_RGBA5551
 #define _ARGB1555_TO_RGBA5551    TO_RGBA5551
 #else
 #define _ARGB1555_TO_RGBA5551    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_RGB888
 #define _ARGB1555_TO_RGB888      TO_RGB888
 #else
 #define _ARGB1555_TO_RGB888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_BGR888
 #define _ARGB1555_TO_BGR888      TO_BGR888
 #else
 #define _ARGB1555_TO_BGR888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_RGB565
 #define _ARGB1555_TO_RGB565      TO_RGB565
 #else
 #define _ARGB1555_TO_RGB565      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_YV12
 #define _ARGB1555_TO_YV12        TO_YV12
 #else
 #define _ARGB1555_TO_YV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_I420
 #define _ARGB1555_TO_I420        TO_I420
 #else
 #define _ARGB1555_TO_I420        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_I422
 #define _ARGB1555_TO_I422        TO_I422
 #else
 #define _ARGB1555_TO_I422        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_I444
 #define _ARGB1555_TO_I444        TO_I444
 #else
 #define _ARGB1555_TO_I444        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_NV12
 #define _ARGB1555_TO_NV12        TO_NV12
 #else
 #define _ARGB1555_TO_NV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_NV21
 #define _ARGB1555_TO_NV21        TO_NV21
 #else
 #define _ARGB1555_TO_NV21        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_YUY2
 #define _ARGB1555_TO_YUY2        TO_YUY2
 #else
 #define _ARGB1555_TO_YUY2        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_ARGB1555_TO_UYVY
 #define _ARGB1555_TO_UYVY        TO_UYVY
 #else
 #define _ARGB1555_TO_UYVY        0
 #endif

 #define AIPL_CONVERT_ARGB1555    (_ARGB1555_TO_ALPHA8_I400\
                                      | _ARGB1555_TO_ARGB8888\
                                      | _ARGB1555_TO_ARGB4444\
                                      | _ARGB1555_TO_RGBA8888\
                                      | _ARGB1555_TO_RGBA4444\
                                      | _ARGB1555_TO_RGBA5551\
                                      | _ARGB1555_TO_RGB888\
                                      | _ARGB1555_TO_BGR888\
                                      | _ARGB1555_TO_RGB565\
                                      | _ARGB1555_TO_YV12\
                                      | _ARGB1555_TO_I420\
                                      | _ARGB1555_TO_I422\
                                      | _ARGB1555_TO_I444\
                                      | _ARGB1555_TO_NV12\
                                      | _ARGB1555_TO_NV21\
                                      | _ARGB1555_TO_YUY2\
                                      | _ARGB1555_TO_UYVY)
 #else
 #define AIPL_CONVERT_ARGB1555    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888
 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_ALPHA8_I400
 #define _RGBA8888_TO_ALPHA8_I400 TO_ALPHA8_I400
 #else
 #define _RGBA8888_TO_ALPHA8_I400 0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_ARGB8888
 #define _RGBA8888_TO_ARGB8888    TO_ARGB8888
 #else
 #define _RGBA8888_TO_ARGB8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_ARGB4444
 #define _RGBA8888_TO_ARGB4444    TO_ARGB4444
 #else
 #define _RGBA8888_TO_ARGB4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_ARGB1555
 #define _RGBA8888_TO_ARGB1555    TO_ARGB1555
 #else
 #define _RGBA8888_TO_ARGB1555    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_RGBA4444
 #define _RGBA8888_TO_RGBA4444    TO_RGBA4444
 #else
 #define _RGBA8888_TO_RGBA4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_RGBA5551
 #define _RGBA8888_TO_RGBA5551    TO_RGBA5551
 #else
 #define _RGBA8888_TO_RGBA5551    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_RGB888
 #define _RGBA8888_TO_RGB888      TO_RGB888
 #else
 #define _RGBA8888_TO_RGB888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_BGR888
 #define _RGBA8888_TO_BGR888      TO_BGR888
 #else
 #define _RGBA8888_TO_BGR888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_RGB565
 #define _RGBA8888_TO_RGB565      TO_RGB565
 #else
 #define _RGBA8888_TO_RGB565      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_YV12
 #define _RGBA8888_TO_YV12        TO_YV12
 #else
 #define _RGBA8888_TO_YV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_I420
 #define _RGBA8888_TO_I420        TO_I420
 #else
 #define _RGBA8888_TO_I420        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_I422
 #define _RGBA8888_TO_I422        TO_I422
 #else
 #define _RGBA8888_TO_I422        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_I444
 #define _RGBA8888_TO_I444        TO_I444
 #else
 #define _RGBA8888_TO_I444        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_NV12
 #define _RGBA8888_TO_NV12        TO_NV12
 #else
 #define _RGBA8888_TO_NV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_NV21
 #define _RGBA8888_TO_NV21        TO_NV21
 #else
 #define _RGBA8888_TO_NV21        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_YUY2
 #define _RGBA8888_TO_YUY2        TO_YUY2
 #else
 #define _RGBA8888_TO_YUY2        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA8888_TO_UYVY
 #define _RGBA8888_TO_UYVY        TO_UYVY
 #else
 #define _RGBA8888_TO_UYVY        0
 #endif

 #define AIPL_CONVERT_RGBA8888    (_RGBA8888_TO_ALPHA8_I400\
                                      | _RGBA8888_TO_ARGB8888\
                                      | _RGBA8888_TO_ARGB4444\
                                      | _RGBA8888_TO_ARGB1555\
                                      | _RGBA8888_TO_RGBA4444\
                                      | _RGBA8888_TO_RGBA5551\
                                      | _RGBA8888_TO_RGB888\
                                      | _RGBA8888_TO_BGR888\
                                      | _RGBA8888_TO_RGB565\
                                      | _RGBA8888_TO_YV12\
                                      | _RGBA8888_TO_I420\
                                      | _RGBA8888_TO_I422\
                                      | _RGBA8888_TO_I444\
                                      | _RGBA8888_TO_NV12\
                                      | _RGBA8888_TO_NV21\
                                      | _RGBA8888_TO_YUY2\
                                      | _RGBA8888_TO_UYVY)
 #else
 #define AIPL_CONVERT_RGBA8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444
 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_ALPHA8_I400
 #define _RGBA4444_TO_ALPHA8_I400 TO_ALPHA8_I400
 #else
 #define _RGBA4444_TO_ALPHA8_I400 0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_ARGB8888
 #define _RGBA4444_TO_ARGB8888    TO_ARGB8888
 #else
 #define _RGBA4444_TO_ARGB8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_ARGB4444
 #define _RGBA4444_TO_ARGB4444    TO_ARGB4444
 #else
 #define _RGBA4444_TO_ARGB4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_ARGB1555
 #define _RGBA4444_TO_ARGB1555    TO_ARGB1555
 #else
 #define _RGBA4444_TO_ARGB1555    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_RGBA8888
 #define _RGBA4444_TO_RGBA8888    TO_RGBA8888
 #else
 #define _RGBA4444_TO_RGBA8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_RGBA5551
 #define _RGBA4444_TO_RGBA5551    TO_RGBA5551
 #else
 #define _RGBA4444_TO_RGBA5551    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_RGB888
 #define _RGBA4444_TO_RGB888      TO_RGB888
 #else
 #define _RGBA4444_TO_RGB888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_BGR888
 #define _RGBA4444_TO_BGR888      TO_BGR888
 #else
 #define _RGBA4444_TO_BGR888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_RGB565
 #define _RGBA4444_TO_RGB565      TO_RGB565
 #else
 #define _RGBA4444_TO_RGB565      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_YV12
 #define _RGBA4444_TO_YV12        TO_YV12
 #else
 #define _RGBA4444_TO_YV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_I420
 #define _RGBA4444_TO_I420        TO_I420
 #else
 #define _RGBA4444_TO_I420        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_I422
 #define _RGBA4444_TO_I422        TO_I422
 #else
 #define _RGBA4444_TO_I422        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_I444
 #define _RGBA4444_TO_I444        TO_I444
 #else
 #define _RGBA4444_TO_I444        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_NV12
 #define _RGBA4444_TO_NV12        TO_NV12
 #else
 #define _RGBA4444_TO_NV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_NV21
 #define _RGBA4444_TO_NV21        TO_NV21
 #else
 #define _RGBA4444_TO_NV21        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_YUY2
 #define _RGBA4444_TO_YUY2        TO_YUY2
 #else
 #define _RGBA4444_TO_YUY2        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA4444_TO_UYVY
 #define _RGBA4444_TO_UYVY        TO_UYVY
 #else
 #define _RGBA4444_TO_UYVY        0
 #endif

 #define AIPL_CONVERT_RGBA4444    (_RGBA4444_TO_ALPHA8_I400\
                                      | _RGBA4444_TO_ARGB8888\
                                      | _RGBA4444_TO_ARGB4444\
                                      | _RGBA4444_TO_ARGB1555\
                                      | _RGBA4444_TO_RGBA8888\
                                      | _RGBA4444_TO_RGBA5551\
                                      | _RGBA4444_TO_RGB888\
                                      | _RGBA4444_TO_BGR888\
                                      | _RGBA4444_TO_RGB565\
                                      | _RGBA4444_TO_YV12\
                                      | _RGBA4444_TO_I420\
                                      | _RGBA4444_TO_I422\
                                      | _RGBA4444_TO_I444\
                                      | _RGBA4444_TO_NV12\
                                      | _RGBA4444_TO_NV21\
                                      | _RGBA4444_TO_YUY2\
                                      | _RGBA4444_TO_UYVY)
 #else
 #define AIPL_CONVERT_RGBA4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551
 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_ALPHA8_I400
 #define _RGBA5551_TO_ALPHA8_I400 TO_ALPHA8_I400
 #else
 #define _RGBA5551_TO_ALPHA8_I400 0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_ARGB8888
 #define _RGBA5551_TO_ARGB8888    TO_ARGB8888
 #else
 #define _RGBA5551_TO_ARGB8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_ARGB4444
 #define _RGBA5551_TO_ARGB4444    TO_ARGB4444
 #else
 #define _RGBA5551_TO_ARGB4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_ARGB1555
 #define _RGBA5551_TO_ARGB1555    TO_ARGB1555
 #else
 #define _RGBA5551_TO_ARGB1555    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_RGBA8888
 #define _RGBA5551_TO_RGBA8888    TO_RGBA8888
 #else
 #define _RGBA5551_TO_RGBA8888    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_RGBA4444
 #define _RGBA5551_TO_RGBA4444    TO_RGBA4444
 #else
 #define _RGBA5551_TO_RGBA4444    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_RGB888
 #define _RGBA5551_TO_RGB888      TO_RGB888
 #else
 #define _RGBA5551_TO_RGB888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_BGR888
 #define _RGBA5551_TO_BGR888      TO_BGR888
 #else
 #define _RGBA5551_TO_BGR888      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_RGB565
 #define _RGBA5551_TO_RGB565      TO_RGB565
 #else
 #define _RGBA5551_TO_RGB565      0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_YV12
 #define _RGBA5551_TO_YV12        TO_YV12
 #else
 #define _RGBA5551_TO_YV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_I420
 #define _RGBA5551_TO_I420        TO_I420
 #else
 #define _RGBA5551_TO_I420        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_I422
 #define _RGBA5551_TO_I422        TO_I422
 #else
 #define _RGBA5551_TO_I422        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_I444
 #define _RGBA5551_TO_I444        TO_I444
 #else
 #define _RGBA5551_TO_I444        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_NV12
 #define _RGBA5551_TO_NV12        TO_NV12
 #else
 #define _RGBA5551_TO_NV12        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_NV21
 #define _RGBA5551_TO_NV21        TO_NV21
 #else
 #define _RGBA5551_TO_NV21        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_YUY2
 #define _RGBA5551_TO_YUY2        TO_YUY2
 #else
 #define _RGBA5551_TO_YUY2        0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGBA5551_TO_UYVY
 #define _RGBA5551_TO_UYVY        TO_UYVY
 #else
 #define _RGBA5551_TO_UYVY        0
 #endif

 #define AIPL_CONVERT_RGBA5551    (_RGBA5551_TO_ALPHA8_I400\
                                      | _RGBA5551_TO_ARGB8888\
                                      | _RGBA5551_TO_ARGB4444\
                                      | _RGBA5551_TO_ARGB1555\
                                      | _RGBA5551_TO_RGBA8888\
                                      | _RGBA5551_TO_RGBA4444\
                                      | _RGBA5551_TO_RGB888\
                                      | _RGBA5551_TO_BGR888\
                                      | _RGBA5551_TO_RGB565\
                                      | _RGBA5551_TO_YV12\
                                      | _RGBA5551_TO_I420\
                                      | _RGBA5551_TO_I422\
                                      | _RGBA5551_TO_I444\
                                      | _RGBA5551_TO_NV12\
                                      | _RGBA5551_TO_NV21\
                                      | _RGBA5551_TO_YUY2\
                                      | _RGBA5551_TO_UYVY)
 #else
 #define AIPL_CONVERT_RGBA5551    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888
 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_ALPHA8_I400
 #define _RGB888_TO_ALPHA8_I400  TO_ALPHA8_I400
 #else
 #define _RGB888_TO_ALPHA8_I400  0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_ARGB8888
 #define _RGB888_TO_ARGB8888     TO_ARGB8888
 #else
 #define _RGB888_TO_ARGB8888     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_ARGB4444
 #define _RGB888_TO_ARGB4444     TO_ARGB4444
 #else
 #define _RGB888_TO_ARGB4444     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_ARGB1555
 #define _RGB888_TO_ARGB1555     TO_ARGB1555
 #else
 #define _RGB888_TO_ARGB1555     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_RGBA8888
 #define _RGB888_TO_RGBA8888     TO_RGBA8888
 #else
 #define _RGB888_TO_RGBA8888     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_RGBA4444
 #define _RGB888_TO_RGBA4444     TO_RGBA4444
 #else
 #define _RGB888_TO_RGBA4444     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_RGBA5551
 #define _RGB888_TO_RGBA5551     TO_RGBA5551
 #else
 #define _RGB888_TO_RGBA5551     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_BGR888
 #define _RGB888_TO_BGR888       TO_BGR888
 #else
 #define _RGB888_TO_BGR888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_RGB565
 #define _RGB888_TO_RGB565       TO_RGB565
 #else
 #define _RGB888_TO_RGB565       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_YV12
 #define _RGB888_TO_YV12         TO_YV12
 #else
 #define _RGB888_TO_YV12         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_I420
 #define _RGB888_TO_I420         TO_I420
 #else
 #define _RGB888_TO_I420         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_I422
 #define _RGB888_TO_I422         TO_I422
 #else
 #define _RGB888_TO_I422         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_I444
 #define _RGB888_TO_I444         TO_I444
 #else
 #define _RGB888_TO_I444         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_NV12
 #define _RGB888_TO_NV12         TO_NV12
 #else
 #define _RGB888_TO_NV12         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_NV21
 #define _RGB888_TO_NV21         TO_NV21
 #else
 #define _RGB888_TO_NV21         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_YUY2
 #define _RGB888_TO_YUY2         TO_YUY2
 #else
 #define _RGB888_TO_YUY2         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB888_TO_UYVY
 #define _RGB888_TO_UYVY         TO_UYVY
 #else
 #define _RGB888_TO_UYVY         0
 #endif

 #define AIPL_CONVERT_RGB888     (_RGB888_TO_ALPHA8_I400\
                                  | _RGB888_TO_ARGB8888\
                                  | _RGB888_TO_ARGB4444\
                                  | _RGB888_TO_ARGB1555\
                                  | _RGB888_TO_RGBA8888\
                                  | _RGB888_TO_RGBA4444\
                                  | _RGB888_TO_RGBA5551\
                                  | _RGB888_TO_BGR888\
                                  | _RGB888_TO_RGB565\
                                  | _RGB888_TO_YV12\
                                  | _RGB888_TO_I420\
                                  | _RGB888_TO_I422\
                                  | _RGB888_TO_I444\
                                  | _RGB888_TO_NV12\
                                  | _RGB888_TO_NV21\
                                  | _RGB888_TO_YUY2\
                                  | _RGB888_TO_UYVY)
 #else
 #define AIPL_CONVERT_RGB888     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888
 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_ALPHA8_I400
 #define _BGR888_TO_ALPHA8_I400  TO_ALPHA8_I400
 #else
 #define _BGR888_TO_ALPHA8_I400  0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_ARGB8888
 #define _BGR888_TO_ARGB8888     TO_ARGB8888
 #else
 #define _BGR888_TO_ARGB8888     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_ARGB4444
 #define _BGR888_TO_ARGB4444     TO_ARGB4444
 #else
 #define _BGR888_TO_ARGB4444     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_ARGB1555
 #define _BGR888_TO_ARGB1555     TO_ARGB1555
 #else
 #define _BGR888_TO_ARGB1555     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_RGBA8888
 #define _BGR888_TO_RGBA8888     TO_RGBA8888
 #else
 #define _BGR888_TO_RGBA8888     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_RGBA4444
 #define _BGR888_TO_RGBA4444     TO_RGBA4444
 #else
 #define _BGR888_TO_RGBA4444     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_RGBA5551
 #define _BGR888_TO_RGBA5551     TO_RGBA5551
 #else
 #define _BGR888_TO_RGBA5551     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_RGB888
 #define _BGR888_TO_RGB888       TO_RGB888
 #else
 #define _BGR888_TO_RGB888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_RGB565
 #define _BGR888_TO_RGB565       TO_RGB565
 #else
 #define _BGR888_TO_RGB565       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_YV12
 #define _BGR888_TO_YV12         TO_YV12
 #else
 #define _BGR888_TO_YV12         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_I420
 #define _BGR888_TO_I420         TO_I420
 #else
 #define _BGR888_TO_I420         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_I422
 #define _BGR888_TO_I422         TO_I422
 #else
 #define _BGR888_TO_I422         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_I444
 #define _BGR888_TO_I444         TO_I444
 #else
 #define _BGR888_TO_I444         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_NV12
 #define _BGR888_TO_NV12         TO_NV12
 #else
 #define _BGR888_TO_NV12         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_NV21
 #define _BGR888_TO_NV21         TO_NV21
 #else
 #define _BGR888_TO_NV21         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_YUY2
 #define _BGR888_TO_YUY2         TO_YUY2
 #else
 #define _BGR888_TO_YUY2         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_BGR888_TO_UYVY
 #define _BGR888_TO_UYVY         TO_UYVY
 #else
 #define _BGR888_TO_UYVY         0
 #endif

 #define AIPL_CONVERT_BGR888     (_BGR888_TO_ALPHA8_I400\
                                  | _BGR888_TO_ARGB8888\
                                  | _BGR888_TO_ARGB4444\
                                  | _BGR888_TO_ARGB1555\
                                  | _BGR888_TO_RGBA8888\
                                  | _BGR888_TO_RGBA4444\
                                  | _BGR888_TO_RGBA5551\
                                  | _BGR888_TO_RGB888\
                                  | _BGR888_TO_RGB565\
                                  | _BGR888_TO_YV12\
                                  | _BGR888_TO_I420\
                                  | _BGR888_TO_I422\
                                  | _BGR888_TO_I444\
                                  | _BGR888_TO_NV12\
                                  | _BGR888_TO_NV21\
                                  | _BGR888_TO_YUY2\
                                  | _BGR888_TO_UYVY)
 #else
 #define AIPL_CONVERT_BGR888     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565
 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_ALPHA8_I400
 #define _RGB565_TO_ALPHA8_I400  TO_ALPHA8_I400
 #else
 #define _RGB565_TO_ALPHA8_I400  0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_ARGB8888
 #define _RGB565_TO_ARGB8888     TO_ARGB8888
 #else
 #define _RGB565_TO_ARGB8888     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_ARGB4444
 #define _RGB565_TO_ARGB4444     TO_ARGB4444
 #else
 #define _RGB565_TO_ARGB4444     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_ARGB1555
 #define _RGB565_TO_ARGB1555     TO_ARGB1555
 #else
 #define _RGB565_TO_ARGB1555     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_RGBA8888
 #define _RGB565_TO_RGBA8888     TO_RGBA8888
 #else
 #define _RGB565_TO_RGBA8888     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_RGBA4444
 #define _RGB565_TO_RGBA4444     TO_RGBA4444
 #else
 #define _RGB565_TO_RGBA4444     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_RGBA5551
 #define _RGB565_TO_RGBA5551     TO_RGBA5551
 #else
 #define _RGB565_TO_RGBA5551     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_RGB888
 #define _RGB565_TO_RGB888       TO_RGB888
 #else
 #define _RGB565_TO_RGB888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_BGR888
 #define _RGB565_TO_BGR888       TO_BGR888
 #else
 #define _RGB565_TO_BGR888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_YV12
 #define _RGB565_TO_YV12         TO_YV12
 #else
 #define _RGB565_TO_YV12         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_I420
 #define _RGB565_TO_I420         TO_I420
 #else
 #define _RGB565_TO_I420         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_I422
 #define _RGB565_TO_I422         TO_I422
 #else
 #define _RGB565_TO_I422         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_I444
 #define _RGB565_TO_I444         TO_I444
 #else
 #define _RGB565_TO_I444         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_NV12
 #define _RGB565_TO_NV12         TO_NV12
 #else
 #define _RGB565_TO_NV12         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_NV21
 #define _RGB565_TO_NV21         TO_NV21
 #else
 #define _RGB565_TO_NV21         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_YUY2
 #define _RGB565_TO_YUY2         TO_YUY2
 #else
 #define _RGB565_TO_YUY2         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_RGB565_TO_UYVY
 #define _RGB565_TO_UYVY         TO_UYVY
 #else
 #define _RGB565_TO_UYVY         0
 #endif

 #define AIPL_CONVERT_RGB565     (_RGB565_TO_ALPHA8_I400\
                                  | _RGB565_TO_ARGB8888\
                                  | _RGB565_TO_ARGB4444\
                                  | _RGB565_TO_ARGB1555\
                                  | _RGB565_TO_RGBA8888\
                                  | _RGB565_TO_RGBA4444\
                                  | _RGB565_TO_RGBA5551\
                                  | _RGB565_TO_RGB888\
                                  | _RGB565_TO_BGR888\
                                  | _RGB565_TO_YV12\
                                  | _RGB565_TO_I420\
                                  | _RGB565_TO_I422\
                                  | _RGB565_TO_I444\
                                  | _RGB565_TO_NV12\
                                  | _RGB565_TO_NV21\
                                  | _RGB565_TO_YUY2\
                                  | _RGB565_TO_UYVY)
 #else
 #define AIPL_CONVERT_RGB565     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12
 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_ALPHA8_I400
 #define _YV12_TO_ALPHA8_I400    TO_ALPHA8_I400
 #else
 #define _YV12_TO_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_ARGB8888
 #define _YV12_TO_ARGB8888       TO_ARGB8888
 #else
 #define _YV12_TO_ARGB8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_ARGB4444
 #define _YV12_TO_ARGB4444       TO_ARGB4444
 #else
 #define _YV12_TO_ARGB4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_ARGB1555
 #define _YV12_TO_ARGB1555       TO_ARGB1555
 #else
 #define _YV12_TO_ARGB1555       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_RGBA8888
 #define _YV12_TO_RGBA8888       TO_RGBA8888
 #else
 #define _YV12_TO_RGBA8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_RGBA4444
 #define _YV12_TO_RGBA4444       TO_RGBA4444
 #else
 #define _YV12_TO_RGBA4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_RGBA5551
 #define _YV12_TO_RGBA5551       TO_RGBA5551
 #else
 #define _YV12_TO_RGBA5551       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_RGB888
 #define _YV12_TO_RGB888         TO_RGB888
 #else
 #define _YV12_TO_RGB888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_BGR888
 #define _YV12_TO_BGR888         TO_BGR888
 #else
 #define _YV12_TO_BGR888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_RGB565
 #define _YV12_TO_RGB565         TO_RGB565
 #else
 #define _YV12_TO_RGB565         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_I420
 #define _YV12_TO_I420           TO_I420
 #else
 #define _YV12_TO_I420           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_I422
 #define _YV12_TO_I422           TO_I422
 #else
 #define _YV12_TO_I422           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_I444
 #define _YV12_TO_I444           TO_I444
 #else
 #define _YV12_TO_I444           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_NV12
 #define _YV12_TO_NV12           TO_NV12
 #else
 #define _YV12_TO_NV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_NV21
 #define _YV12_TO_NV21           TO_NV21
 #else
 #define _YV12_TO_NV21           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_YUY2
 #define _YV12_TO_YUY2           TO_YUY2
 #else
 #define _YV12_TO_YUY2           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YV12_TO_UYVY
 #define _YV12_TO_UYVY           TO_UYVY
 #else
 #define _YV12_TO_UYVY           0
 #endif

 #define AIPL_CONVERT_YV12     (_YV12_TO_ALPHA8_I400\
                                  | _YV12_TO_ARGB8888\
                                  | _YV12_TO_ARGB4444\
                                  | _YV12_TO_ARGB1555\
                                  | _YV12_TO_RGBA8888\
                                  | _YV12_TO_RGBA4444\
                                  | _YV12_TO_RGBA5551\
                                  | _YV12_TO_RGB888\
                                  | _YV12_TO_BGR888\
                                  | _YV12_TO_RGB565\
                                  | _YV12_TO_I420\
                                  | _YV12_TO_I422\
                                  | _YV12_TO_I444\
                                  | _YV12_TO_NV12\
                                  | _YV12_TO_NV21\
                                  | _YV12_TO_YUY2\
                                  | _YV12_TO_UYVY)
 #else
 #define AIPL_CONVERT_YV12     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420
 #ifdef CONFIG_AIPL_CONVERT_I420_TO_ALPHA8_I400
 #define _I420_TO_ALPHA8_I400    TO_ALPHA8_I400
 #else
 #define _I420_TO_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_ARGB8888
 #define _I420_TO_ARGB8888       TO_ARGB8888
 #else
 #define _I420_TO_ARGB8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_ARGB4444
 #define _I420_TO_ARGB4444       TO_ARGB4444
 #else
 #define _I420_TO_ARGB4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_ARGB1555
 #define _I420_TO_ARGB1555       TO_ARGB1555
 #else
 #define _I420_TO_ARGB1555       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_RGBA8888
 #define _I420_TO_RGBA8888       TO_RGBA8888
 #else
 #define _I420_TO_RGBA8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_RGBA4444
 #define _I420_TO_RGBA4444       TO_RGBA4444
 #else
 #define _I420_TO_RGBA4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_RGBA5551
 #define _I420_TO_RGBA5551       TO_RGBA5551
 #else
 #define _I420_TO_RGBA5551       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_RGB888
 #define _I420_TO_RGB888         TO_RGB888
 #else
 #define _I420_TO_RGB888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_BGR888
 #define _I420_TO_BGR888         TO_BGR888
 #else
 #define _I420_TO_BGR888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_RGB565
 #define _I420_TO_RGB565         TO_RGB565
 #else
 #define _I420_TO_RGB565         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_YV12
 #define _I420_TO_YV12           TO_YV12
 #else
 #define _I420_TO_YV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_I422
 #define _I420_TO_I422           TO_I422
 #else
 #define _I420_TO_I422           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_I444
 #define _I420_TO_I444           TO_I444
 #else
 #define _I420_TO_I444           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_NV12
 #define _I420_TO_NV12           TO_NV12
 #else
 #define _I420_TO_NV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_NV21
 #define _I420_TO_NV21           TO_NV21
 #else
 #define _I420_TO_NV21           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_YUY2
 #define _I420_TO_YUY2           TO_YUY2
 #else
 #define _I420_TO_YUY2           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I420_TO_UYVY
 #define _I420_TO_UYVY           TO_UYVY
 #else
 #define _I420_TO_UYVY           0
 #endif

 #define AIPL_CONVERT_I420     (_I420_TO_ALPHA8_I400\
                                  | _I420_TO_ARGB8888\
                                  | _I420_TO_ARGB4444\
                                  | _I420_TO_ARGB1555\
                                  | _I420_TO_RGBA8888\
                                  | _I420_TO_RGBA4444\
                                  | _I420_TO_RGBA5551\
                                  | _I420_TO_RGB888\
                                  | _I420_TO_BGR888\
                                  | _I420_TO_RGB565\
                                  | _I420_TO_YV12\
                                  | _I420_TO_I422\
                                  | _I420_TO_I444\
                                  | _I420_TO_NV12\
                                  | _I420_TO_NV21\
                                  | _I420_TO_YUY2\
                                  | _I420_TO_UYVY)
 #else
 #define AIPL_CONVERT_I420     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422
 #ifdef CONFIG_AIPL_CONVERT_I422_TO_ALPHA8_I400
 #define _I422_TO_ALPHA8_I400    TO_ALPHA8_I400
 #else
 #define _I422_TO_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_ARGB8888
 #define _I422_TO_ARGB8888       TO_ARGB8888
 #else
 #define _I422_TO_ARGB8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_ARGB4444
 #define _I422_TO_ARGB4444       TO_ARGB4444
 #else
 #define _I422_TO_ARGB4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_ARGB1555
 #define _I422_TO_ARGB1555       TO_ARGB1555
 #else
 #define _I422_TO_ARGB1555       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_RGBA8888
 #define _I422_TO_RGBA8888       TO_RGBA8888
 #else
 #define _I422_TO_RGBA8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_RGBA4444
 #define _I422_TO_RGBA4444       TO_RGBA4444
 #else
 #define _I422_TO_RGBA4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_RGBA5551
 #define _I422_TO_RGBA5551       TO_RGBA5551
 #else
 #define _I422_TO_RGBA5551       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_RGB888
 #define _I422_TO_RGB888         TO_RGB888
 #else
 #define _I422_TO_RGB888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_BGR888
 #define _I422_TO_BGR888         TO_BGR888
 #else
 #define _I422_TO_BGR888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_RGB565
 #define _I422_TO_RGB565         TO_RGB565
 #else
 #define _I422_TO_RGB565         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_YV12
 #define _I422_TO_YV12           TO_YV12
 #else
 #define _I422_TO_YV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_I420
 #define _I422_TO_I420           TO_I420
 #else
 #define _I422_TO_I420           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_I444
 #define _I422_TO_I444           TO_I444
 #else
 #define _I422_TO_I444           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_NV12
 #define _I422_TO_NV12           TO_NV12
 #else
 #define _I422_TO_NV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_NV21
 #define _I422_TO_NV21           TO_NV21
 #else
 #define _I422_TO_NV21           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_YUY2
 #define _I422_TO_YUY2           TO_YUY2
 #else
 #define _I422_TO_YUY2           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I422_TO_UYVY
 #define _I422_TO_UYVY           TO_UYVY
 #else
 #define _I422_TO_UYVY           0
 #endif

 #define AIPL_CONVERT_I422     (_I422_TO_ALPHA8_I400\
                                  | _I422_TO_ARGB8888\
                                  | _I422_TO_ARGB4444\
                                  | _I422_TO_ARGB1555\
                                  | _I422_TO_RGBA8888\
                                  | _I422_TO_RGBA4444\
                                  | _I422_TO_RGBA5551\
                                  | _I422_TO_RGB888\
                                  | _I422_TO_BGR888\
                                  | _I422_TO_RGB565\
                                  | _I422_TO_YV12\
                                  | _I422_TO_I420\
                                  | _I422_TO_I444\
                                  | _I422_TO_NV12\
                                  | _I422_TO_NV21\
                                  | _I422_TO_YUY2\
                                  | _I422_TO_UYVY)
 #else
 #define AIPL_CONVERT_I422     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444
 #ifdef CONFIG_AIPL_CONVERT_I444_TO_ALPHA8_I400
 #define _I444_TO_ALPHA8_I400    TO_ALPHA8_I400
 #else
 #define _I444_TO_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_ARGB8888
 #define _I444_TO_ARGB8888       TO_ARGB8888
 #else
 #define _I444_TO_ARGB8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_ARGB4444
 #define _I444_TO_ARGB4444       TO_ARGB4444
 #else
 #define _I444_TO_ARGB4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_ARGB1555
 #define _I444_TO_ARGB1555       TO_ARGB1555
 #else
 #define _I444_TO_ARGB1555       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_RGBA8888
 #define _I444_TO_RGBA8888       TO_RGBA8888
 #else
 #define _I444_TO_RGBA8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_RGBA4444
 #define _I444_TO_RGBA4444       TO_RGBA4444
 #else
 #define _I444_TO_RGBA4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_RGBA5551
 #define _I444_TO_RGBA5551       TO_RGBA5551
 #else
 #define _I444_TO_RGBA5551       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_RGB888
 #define _I444_TO_RGB888         TO_RGB888
 #else
 #define _I444_TO_RGB888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_BGR888
 #define _I444_TO_BGR888         TO_BGR888
 #else
 #define _I444_TO_BGR888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_RGB565
 #define _I444_TO_RGB565         TO_RGB565
 #else
 #define _I444_TO_RGB565         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_YV12
 #define _I444_TO_YV12           TO_YV12
 #else
 #define _I444_TO_YV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_I420
 #define _I444_TO_I420           TO_I420
 #else
 #define _I444_TO_I420           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_I422
 #define _I444_TO_I422           TO_I422
 #else
 #define _I444_TO_I422           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_NV12
 #define _I444_TO_NV12           TO_NV12
 #else
 #define _I444_TO_NV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_NV21
 #define _I444_TO_NV21           TO_NV21
 #else
 #define _I444_TO_NV21           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_YUY2
 #define _I444_TO_YUY2           TO_YUY2
 #else
 #define _I444_TO_YUY2           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_I444_TO_UYVY
 #define _I444_TO_UYVY           TO_UYVY
 #else
 #define _I444_TO_UYVY           0
 #endif

 #define AIPL_CONVERT_I444     (_I444_TO_ALPHA8_I400\
                                  | _I444_TO_ARGB8888\
                                  | _I444_TO_ARGB4444\
                                  | _I444_TO_ARGB1555\
                                  | _I444_TO_RGBA8888\
                                  | _I444_TO_RGBA4444\
                                  | _I444_TO_RGBA5551\
                                  | _I444_TO_RGB888\
                                  | _I444_TO_BGR888\
                                  | _I444_TO_RGB565\
                                  | _I444_TO_YV12\
                                  | _I444_TO_I420\
                                  | _I444_TO_I422\
                                  | _I444_TO_NV12\
                                  | _I444_TO_NV21\
                                  | _I444_TO_YUY2\
                                  | _I444_TO_UYVY)
 #else
 #define AIPL_CONVERT_I444     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12
 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_ALPHA8_I400
 #define _NV12_TO_ALPHA8_I400    TO_ALPHA8_I400
 #else
 #define _NV12_TO_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_ARGB8888
 #define _NV12_TO_ARGB8888       TO_ARGB8888
 #else
 #define _NV12_TO_ARGB8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_ARGB4444
 #define _NV12_TO_ARGB4444       TO_ARGB4444
 #else
 #define _NV12_TO_ARGB4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_ARGB1555
 #define _NV12_TO_ARGB1555       TO_ARGB1555
 #else
 #define _NV12_TO_ARGB1555       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_RGBA8888
 #define _NV12_TO_RGBA8888       TO_RGBA8888
 #else
 #define _NV12_TO_RGBA8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_RGBA4444
 #define _NV12_TO_RGBA4444       TO_RGBA4444
 #else
 #define _NV12_TO_RGBA4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_RGBA5551
 #define _NV12_TO_RGBA5551       TO_RGBA5551
 #else
 #define _NV12_TO_RGBA5551       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_RGB888
 #define _NV12_TO_RGB888         TO_RGB888
 #else
 #define _NV12_TO_RGB888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_BGR888
 #define _NV12_TO_BGR888         TO_BGR888
 #else
 #define _NV12_TO_BGR888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_RGB565
 #define _NV12_TO_RGB565         TO_RGB565
 #else
 #define _NV12_TO_RGB565         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_YV12
 #define _NV12_TO_YV12           TO_YV12
 #else
 #define _NV12_TO_YV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_I420
 #define _NV12_TO_I420           TO_I420
 #else
 #define _NV12_TO_I420           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_I422
 #define _NV12_TO_I422           TO_I422
 #else
 #define _NV12_TO_I422           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_I444
 #define _NV12_TO_I444           TO_I444
 #else
 #define _NV12_TO_I444           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_NV21
 #define _NV12_TO_NV21           TO_NV21
 #else
 #define _NV12_TO_NV21           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_YUY2
 #define _NV12_TO_YUY2           TO_YUY2
 #else
 #define _NV12_TO_YUY2           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV12_TO_UYVY
 #define _NV12_TO_UYVY           TO_UYVY
 #else
 #define _NV12_TO_UYVY           0
 #endif

 #define AIPL_CONVERT_NV12     (_NV12_TO_ALPHA8_I400\
                                  | _NV12_TO_ARGB8888\
                                  | _NV12_TO_ARGB4444\
                                  | _NV12_TO_ARGB1555\
                                  | _NV12_TO_RGBA8888\
                                  | _NV12_TO_RGBA4444\
                                  | _NV12_TO_RGBA5551\
                                  | _NV12_TO_RGB888\
                                  | _NV12_TO_BGR888\
                                  | _NV12_TO_RGB565\
                                  | _NV12_TO_YV12\
                                  | _NV12_TO_I420\
                                  | _NV12_TO_I422\
                                  | _NV12_TO_I444\
                                  | _NV12_TO_NV21\
                                  | _NV12_TO_YUY2\
                                  | _NV12_TO_UYVY)
 #else
 #define AIPL_CONVERT_NV12     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21
 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_ALPHA8_I400
 #define _NV21_TO_ALPHA8_I400    TO_ALPHA8_I400
 #else
 #define _NV21_TO_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_ARGB8888
 #define _NV21_TO_ARGB8888       TO_ARGB8888
 #else
 #define _NV21_TO_ARGB8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_ARGB4444
 #define _NV21_TO_ARGB4444       TO_ARGB4444
 #else
 #define _NV21_TO_ARGB4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_ARGB1555
 #define _NV21_TO_ARGB1555       TO_ARGB1555
 #else
 #define _NV21_TO_ARGB1555       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_RGBA8888
 #define _NV21_TO_RGBA8888       TO_RGBA8888
 #else
 #define _NV21_TO_RGBA8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_RGBA4444
 #define _NV21_TO_RGBA4444       TO_RGBA4444
 #else
 #define _NV21_TO_RGBA4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_RGBA5551
 #define _NV21_TO_RGBA5551       TO_RGBA5551
 #else
 #define _NV21_TO_RGBA5551       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_RGB888
 #define _NV21_TO_RGB888         TO_RGB888
 #else
 #define _NV21_TO_RGB888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_BGR888
 #define _NV21_TO_BGR888         TO_BGR888
 #else
 #define _NV21_TO_BGR888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_RGB565
 #define _NV21_TO_RGB565         TO_RGB565
 #else
 #define _NV21_TO_RGB565         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_YV12
 #define _NV21_TO_YV12           TO_YV12
 #else
 #define _NV21_TO_YV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_I420
 #define _NV21_TO_I420           TO_I420
 #else
 #define _NV21_TO_I420           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_I422
 #define _NV21_TO_I422           TO_I422
 #else
 #define _NV21_TO_I422           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_I444
 #define _NV21_TO_I444           TO_I444
 #else
 #define _NV21_TO_I444           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_NV12
 #define _NV21_TO_NV12           TO_NV12
 #else
 #define _NV21_TO_NV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_YUY2
 #define _NV21_TO_YUY2           TO_YUY2
 #else
 #define _NV21_TO_YUY2           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_NV21_TO_UYVY
 #define _NV21_TO_UYVY           TO_UYVY
 #else
 #define _NV21_TO_UYVY           0
 #endif

 #define AIPL_CONVERT_NV21     (_NV21_TO_ALPHA8_I400\
                                  | _NV21_TO_ARGB8888\
                                  | _NV21_TO_ARGB4444\
                                  | _NV21_TO_ARGB1555\
                                  | _NV21_TO_RGBA8888\
                                  | _NV21_TO_RGBA4444\
                                  | _NV21_TO_RGBA5551\
                                  | _NV21_TO_RGB888\
                                  | _NV21_TO_BGR888\
                                  | _NV21_TO_RGB565\
                                  | _NV21_TO_YV12\
                                  | _NV21_TO_I420\
                                  | _NV21_TO_I422\
                                  | _NV21_TO_I444\
                                  | _NV21_TO_NV12\
                                  | _NV21_TO_YUY2\
                                  | _NV21_TO_UYVY)
 #else
 #define AIPL_CONVERT_NV21     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2
 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_ALPHA8_I400
 #define _YUY2_TO_ALPHA8_I400    TO_ALPHA8_I400
 #else
 #define _YUY2_TO_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_ARGB8888
 #define _YUY2_TO_ARGB8888       TO_ARGB8888
 #else
 #define _YUY2_TO_ARGB8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_ARGB4444
 #define _YUY2_TO_ARGB4444       TO_ARGB4444
 #else
 #define _YUY2_TO_ARGB4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_ARGB1555
 #define _YUY2_TO_ARGB1555       TO_ARGB1555
 #else
 #define _YUY2_TO_ARGB1555       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_RGBA8888
 #define _YUY2_TO_RGBA8888       TO_RGBA8888
 #else
 #define _YUY2_TO_RGBA8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_RGBA4444
 #define _YUY2_TO_RGBA4444       TO_RGBA4444
 #else
 #define _YUY2_TO_RGBA4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_RGBA5551
 #define _YUY2_TO_RGBA5551       TO_RGBA5551
 #else
 #define _YUY2_TO_RGBA5551       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_RGB888
 #define _YUY2_TO_RGB888         TO_RGB888
 #else
 #define _YUY2_TO_RGB888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_BGR888
 #define _YUY2_TO_BGR888         TO_BGR888
 #else
 #define _YUY2_TO_BGR888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_RGB565
 #define _YUY2_TO_RGB565         TO_RGB565
 #else
 #define _YUY2_TO_RGB565         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_YV12
 #define _YUY2_TO_YV12           TO_YV12
 #else
 #define _YUY2_TO_YV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_I420
 #define _YUY2_TO_I420           TO_I420
 #else
 #define _YUY2_TO_I420           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_I422
 #define _YUY2_TO_I422           TO_I422
 #else
 #define _YUY2_TO_I422           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_I444
 #define _YUY2_TO_I444           TO_I444
 #else
 #define _YUY2_TO_I444           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_NV12
 #define _YUY2_TO_NV12           TO_NV12
 #else
 #define _YUY2_TO_NV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_NV21
 #define _YUY2_TO_NV21           TO_NV21
 #else
 #define _YUY2_TO_NV21           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_YUY2_TO_UYVY
 #define _YUY2_TO_UYVY           TO_UYVY
 #else
 #define _YUY2_TO_UYVY           0
 #endif

 #define AIPL_CONVERT_YUY2     (_YUY2_TO_ALPHA8_I400\
                                  | _YUY2_TO_ARGB8888\
                                  | _YUY2_TO_ARGB4444\
                                  | _YUY2_TO_ARGB1555\
                                  | _YUY2_TO_RGBA8888\
                                  | _YUY2_TO_RGBA4444\
                                  | _YUY2_TO_RGBA5551\
                                  | _YUY2_TO_RGB888\
                                  | _YUY2_TO_BGR888\
                                  | _YUY2_TO_RGB565\
                                  | _YUY2_TO_YV12\
                                  | _YUY2_TO_I420\
                                  | _YUY2_TO_I422\
                                  | _YUY2_TO_I444\
                                  | _YUY2_TO_NV12\
                                  | _YUY2_TO_NV21\
                                  | _YUY2_TO_UYVY)
 #else
 #define AIPL_CONVERT_YUY2     0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY
 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_ALPHA8_I400
 #define _UYVY_TO_ALPHA8_I400    TO_ALPHA8_I400
 #else
 #define _UYVY_TO_ALPHA8_I400    0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_ARGB8888
 #define _UYVY_TO_ARGB8888       TO_ARGB8888
 #else
 #define _UYVY_TO_ARGB8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_ARGB4444
 #define _UYVY_TO_ARGB4444       TO_ARGB4444
 #else
 #define _UYVY_TO_ARGB4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_ARGB1555
 #define _UYVY_TO_ARGB1555       TO_ARGB1555
 #else
 #define _UYVY_TO_ARGB1555       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_RGBA8888
 #define _UYVY_TO_RGBA8888       TO_RGBA8888
 #else
 #define _UYVY_TO_RGBA8888       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_RGBA4444
 #define _UYVY_TO_RGBA4444       TO_RGBA4444
 #else
 #define _UYVY_TO_RGBA4444       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_RGBA5551
 #define _UYVY_TO_RGBA5551       TO_RGBA5551
 #else
 #define _UYVY_TO_RGBA5551       0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_RGB888
 #define _UYVY_TO_RGB888         TO_RGB888
 #else
 #define _UYVY_TO_RGB888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_BGR888
 #define _UYVY_TO_BGR888         TO_BGR888
 #else
 #define _UYVY_TO_BGR888         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_RGB565
 #define _UYVY_TO_RGB565         TO_RGB565
 #else
 #define _UYVY_TO_RGB565         0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_YV12
 #define _UYVY_TO_YV12           TO_YV12
 #else
 #define _UYVY_TO_YV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_I420
 #define _UYVY_TO_I420           TO_I420
 #else
 #define _UYVY_TO_I420           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_I422
 #define _UYVY_TO_I422           TO_I422
 #else
 #define _UYVY_TO_I422           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_I444
 #define _UYVY_TO_I444           TO_I444
 #else
 #define _UYVY_TO_I444           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_NV12
 #define _UYVY_TO_NV12           TO_NV12
 #else
 #define _UYVY_TO_NV12           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_NV21
 #define _UYVY_TO_NV21           TO_NV21
 #else
 #define _UYVY_TO_NV21           0
 #endif

 #ifdef CONFIG_AIPL_CONVERT_UYVY_TO_YUY2
 #define _UYVY_TO_YUY2           TO_YUY2
 #else
 #define _UYVY_TO_YUY2           0
 #endif

 #define AIPL_CONVERT_UYVY     (_UYVY_TO_ALPHA8_I400\
                                  | _UYVY_TO_ARGB8888\
                                  | _UYVY_TO_ARGB4444\
                                  | _UYVY_TO_ARGB1555\
                                  | _UYVY_TO_RGBA8888\
                                  | _UYVY_TO_RGBA4444\
                                  | _UYVY_TO_RGBA5551\
                                  | _UYVY_TO_RGB888\
                                  | _UYVY_TO_BGR888\
                                  | _UYVY_TO_RGB565\
                                  | _UYVY_TO_YV12\
                                  | _UYVY_TO_I420\
                                  | _UYVY_TO_I422\
                                  | _UYVY_TO_I444\
                                  | _UYVY_TO_NV12\
                                  | _UYVY_TO_NV21\
                                  | _UYVY_TO_YUY2)
 #else
 #define AIPL_CONVERT_UYVY     0
 #endif

 #ifdef __cplusplus
 } /*extern "C"*/
 #endif

 #endif  /* AIPL_CONFIG_H */
