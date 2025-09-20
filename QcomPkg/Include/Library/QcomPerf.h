/** @file QcomPerf.h

  Contains generic Qualcomm library functions.

  Copyright (c) 2019 Qualcomm Technologies, Inc. All rights reserved.


**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/01/19   kpa     Initial Revision.

=============================================================================*/
#ifndef __QCOMPERF_H__
#define __QCOMPERF_H__

/*===========================================================================
  INCLUDE FILES
===========================================================================*/
#include <Library/QcomBaseLib.h>


/*===========================================================================
  MACRO DECLARATIONS
===========================================================================*/
typedef struct _QCOM_PERF_DATA
{
  UINT32 CountExecStart;  /**< Valid Indexes for Client to Query */
}QCOM_PERF_DATA;

/*===========================================================================
  EXTERNAL VARIABLES
===========================================================================*/

/**
  Macro to save the timestamp for execution delta
  
  @return EFI_SUCCESS         On successfully retreiving file-size

**/
static inline EFI_STATUS
QPERF_START (QCOM_PERF_DATA* Handle)
{
  if(Handle == NULL)
    return EFI_INVALID_PARAMETER;

  Handle->CountExecStart = GetTimerCountus();  
  
  return EFI_SUCCESS;  
}

/**
  Macro to save the timestamp for execution delta
  
  @param  Msg                Character string to be printed along 
                             with execution delta
**/
static inline EFI_STATUS
QPERF_END (QCOM_PERF_DATA *Handle, CHAR8* Msg)
{
  UINT32 CountExecEnd;
  if(Handle == NULL || Msg == NULL)
    return EFI_INVALID_PARAMETER;

  CountExecEnd = GetTimerCountus();
  if (CountExecEnd < Handle->CountExecStart)
  {
    DEBUG ((EFI_D_INFO, "%a : micro sec Overflow \n", Msg ));
  }
  else
  {
    DEBUG ((EFI_D_INFO, "%a : %d micro sec\n", Msg, (CountExecEnd - Handle->CountExecStart)));
  }
  return EFI_SUCCESS;
}

#endif /* __QCOMPERF_H__ */
