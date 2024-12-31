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
 * @file    aipl_error.c
 * @brief   Error utils implementation
 *
******************************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include "aipl_error.h"

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
const char* aipl_error_str(aipl_error_t error)
{
    switch (error)
    {
    case AIPL_ERR_OK:
        return "AIPL_ERR_OK";
    case AIPL_ERR_NO_MEM:
        return "AIPL_ERR_NO_MEM";
    case AIPL_ERR_NULL_POINTER:
        return "AIPL_ERR_NULL_POINTER";
    case AIPL_ERR_UNSUPPORTED_FORMAT:
        return "AIPL_ERR_UNSUPPORTED_FORMAT";
    case AIPL_ERR_FORMAT_MISMATCH:
        return "AIPL_ERR_FORMAT_MISMATCH";
    case AIPL_ERR_SIZE_MISMATCH:
        return "AIPL_ERR_SIZE_MISMATCH";
    case AIPL_ERR_FRAME_OUT_OF_RANGE:
        return "AIPL_ERR_FRAME_OUT_OF_RANGE";
    case AIPL_ERR_D2:
        return "AIPL_ERR_D2";
    case AIPL_ERR_BAYER_INVALID_METHOD:
        return "AIPL_ERR_BAYER_INVALID_METHOD";
    case AIPL_ERR_BAYER_INVALID_FILTER:
        return "AIPL_ERR_BAYER_INVALID_FILTER";

    default:
        return "AIPL_ERR_UNKNOWN";
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
