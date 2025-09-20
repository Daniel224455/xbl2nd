/** @file  
               LogBuffer.h

GENERAL DESCRIPTION
   Declare interface for loading SSD key store.

	
  Copyright (c) 2012 Copyright Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

*/

/*===========================================================================
                             EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 11/08/12   shl      Initial version

===========================================================================*/

#ifndef __LOG_BUFFER_H__
#define __LOG_BUFFER_H__

#define QSEE_LOG_BUF_SIZE (4096)

/*
 * TZ Log ring buffer position
 */
struct tzdbg_log_pos_t
{
  UINT16 wrap;
  UINT16 offset;
};

 /*
 * TZ Log ring buffer
 */
typedef struct tzdbg_log_t
{
  struct tzdbg_log_pos_t log_pos;
  UINT8 log_buf[];
}tzdbg_log_t;


/**
*  QseeRegisterLogBuffer()
*
*  @brief 
*  Allocate the log buffer and then register it on TZ side
*  
*  @param[in]  VOID
*
*  @return
*  EFI_STATUS
*/

EFI_STATUS QseeRegisterLogBuffer(void);

/**
*  DisplayQseeLog ()
*
*  @brief 
*  Display the content from log buffer fill from TZ side
*  
*  @param[in]  startOffset
*  @param[in]  endOffset
*
*  @return
*  EFI_STATUS
*/

EFI_STATUS DisplayQseeLog(UINT32 startOffset, UINT32 endOffset);

#endif  /* __LOG_BUFFER_H__  */

