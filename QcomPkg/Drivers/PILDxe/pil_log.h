/*===============================================================================
  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         pil_log.h
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  06/14/17   yw         initial version

================================================================================*/

#ifndef __PIL_LOG_H__
#define __PIL_LOG_H__

#include <Library/DebugLib.h>

typedef enum {
    PIL_SUCCESS               = 0,
    PIL_INVALID_PARAM         = 1,
    PIL_INVALID_CFG           = 2,
    PIL_LOAD_ERR              = 3,
    PIL_ELF_ERR               = 4,
    PIL_SUBSYS_NOT_SUPPORTED  = 5,
    PIL_SUBSYS_NOT_FOUND      = 6,
    PIL_SMC_ERR               = 7,
    PIL_OUT_OF_MEMORY         = 8,
    PIL_SUBSYS_RUNNING        = 9,
    PIL_SUBSYS_EXIST          = 10,
    PIL_GENERIC_ERR           = 11,
    PIL_STATUS_MAX            = 0x7FFFFFFF,
} PIL_STATUS;


#define PIL_INFO(fmt, ...)    \
    DEBUG((EFI_D_INFO, fmt, ##__VA_ARGS__))
    
#define PIL_WARN(fmt, ...)    \
    DEBUG((EFI_D_WARN, fmt, ##__VA_ARGS__))
    
#define PIL_ERROR(fmt, ...)    \
    DEBUG((EFI_D_ERROR, fmt, ##__VA_ARGS__))

#define PIL_INFO_CFG(cfg, fmt, ...)  \
        DEBUG((EFI_D_INFO, "pil-%s "fmt, cfg->FwName, ##__VA_ARGS__))
    
#define PIL_WARN_CFG(cfg, fmt, ...)  \
        DEBUG((EFI_D_WARN, "pil-%s "fmt, cfg->FwName, ##__VA_ARGS__))
    
#define PIL_ERROR_CFG(cfg, fmt, ...)  \
        DEBUG((EFI_D_ERROR, "pil-%s "fmt, cfg->FwName, ##__VA_ARGS__))

extern PIL_STATUS gLastPilError;
extern EFI_STATUS gLastEfiError;
        
#define ASSERT_FAIL_EFI_RETURN(c,r,e) \
        if (!(c)) { \
           gLastEfiError = e; \
           return (r); \
        }

#define ASSERT_FAIL_PIL_RETURN(c,r,e) \
        if (!(c)) { \
           gLastPilError = e; \
           return (r); \
        }
#endif