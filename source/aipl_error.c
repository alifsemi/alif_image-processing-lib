/**
 * @file aipl_error.c
 *
 */

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
    case AIPL_ERR_FORMAT_MISMATCH:
        return "AIPL_ERR_FORMAT_MISMATCH";
    case AIPL_ERR_SIZE_MISMATCH:
        return "AIPL_ERR_SIZE_MISMATCH";
    case AIPL_ERR_FRAME_OUT_OF_RANGE:
        return "AIPL_ERR_FRAME_OUT_OF_RANGE";
    case AIPL_ERR_D2:
        return "AIPL_ERR_D2";

    default:
        return "AIPL_ERR_UNKNOWN";
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
