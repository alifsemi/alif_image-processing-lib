/**
 * @file aipl_cache.h
 *
 */

#ifndef AIPL_CACHE_H
#define AIPL_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
/**
 * Clean CPU cache
 *
 * The function implementation should be provided by the user
 *
 * @param ptr   pointer of the buffer to clean
 * @param size  buffer size
 */
void aipl_cpu_cache_clean(const void* ptr, uint32_t size);

/**
 * Invalidate CPU cache
 *
 * The function implementation should be provided by the user
 *
 * @param ptr   pointer of the buffer to invalidate
 * @param size  buffer size
 */
void aipl_cpu_cache_invalidate(const void* ptr, uint32_t size);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  /* AIPL_CACHE_H */
