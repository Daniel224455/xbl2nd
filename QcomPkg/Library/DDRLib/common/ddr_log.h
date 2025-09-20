#ifndef __DDR_LOG_H__
#define __DDR_LOG_H__

/**
 * @file ddr_log.h
 * @brief
 * Header file for DDR logging.
 */
 /*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2012,2014,2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
                                EDIT HISTORY

================================================================================
when       who     what, where, why
--------   ---     -------------------------------------------------------------
04/11/14   sr      supported DDR logging for SNS DDR driver .
09/12/12   sl      Use macro-style DDR logging.
07/19/12   tw      Implemented qdss swevent wrapper around DDR logging.
03/02/12   tw      Initial version.
==============================================================================*/

#include <stdio.h>
#include <stdarg.h>
#ifdef BUILD_BOOT_CHAIN
#include "ddr_profiler.h"
#endif

/*==============================================================================
                                  MACROS
==============================================================================*/
#ifndef BUILD_BOOT_CHAIN
#define PROFILER_TIMING_MSG 0
#endif
#define DDR_LOG_BUF_LEN_MAX 256

/*==============================================================================
                                  TYPES
==============================================================================*/
enum ddr_log_level
{
  DDR_STATUS, /* not provided for DDRSNS , existing for legacy targets support */
  DDR_BOOTLOG = PROFILER_TIMING_MSG,
  DDR_WARNING, /* not provided for DDRSNS , existing for legacy targets support */
  DDR_ERROR = PROFILER_TIMING_MSG, /* provided for flagging Error conditions in DDRSNS */
  DDR_NORMAL, /* provided for printing interesting/debug  information along with BEGIN/END of each DDRSNS function */
  DDR_DETAIL, /* provided for detailed logging information i.e; coarse/fine delay training & other training(CA/WRLRL/RCW) results with DDRSNS */
};

/*==============================================================================
                                  DATA
==============================================================================*/
extern enum ddr_log_level ddr_log_level;

/*==============================================================================
                                  FUNCTIONS
==============================================================================*/
#ifdef BUILD_BOOT_CHAIN
#define ddr_printf(severity, xxstring, ...)                               \
      do                                                                  \
      {                                                                   \
        boot_profiler_ddr_message((boot_profiler_log_level)severity, xxstring, ##__VA_ARGS__);     \
      } while (0)                                                  
#else
static inline void ddr_printf(uint32 severity, const char* fmt, ...)
{
    #ifdef SVE_PRINTF_ENABLED
    va_list vargs;
    va_start(vargs, fmt);
    vprintf(fmt, vargs);
    va_end(vargs);
    #endif
    
    #if defined( COMPILING_TINY_AXF ) && !defined( JTAGLESS_TINY )
    #include "vv_msg.h"
    extern char ddr_printf_buf[DDR_LOG_BUF_LEN_MAX];
    if(severity == DDR_ERROR)
    {
        va_list vargs;
        va_start(vargs, fmt);
        vsnprintf(ddr_printf_buf, DDR_LOG_BUF_LEN_MAX-1, fmt, vargs);
        va_end(vargs);
        
        vv_msg(SEV_ERROR, ST_FUNCTION, ddr_printf_buf);
        vv_msg(SEV_ERROR, ST_FUNCTION, "\n");
    }
    #endif
}
#endif // BUILD_BOOT_CHAIN

#endif /* __DDR_LOG_H__ */
