#ifndef __GPTLISTENER_H__
#define __GPTLISTENER_H__

/** @file GPTListener.h

 * This file provides header file for eMMC GPT partition Listener interface
 * on the non-secure side
 *
 * Copyright (c) 2012-2013, 2015, 2018 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Technologies Proprietary and Confidential.
 *
 **/

/*=============================================================================
                              EDIT HISTORY


  when         who     what, where, why
  ----------   ---     ---------------------------------------------------------
  2018-04-24   jt      Change argument for GPTListenerInit 
  2015-10-19   jt      Pass MediaId as argument for GPTListenerInit
  2015-01-28   rm      Add GPTListenerAllocMem  
  2013-04-05   jt      Adjust listener size 
  2012-11-02   bb      Initial Version

 =============================================================================*/

#include <Protocol/BlockIo.h>

#ifndef GPT_LISTENER_BUFFER_MAX
  #define GPT_LISTENER_BUFFER_MAX  (260*1024) // 260KB
#endif

/* This function allocates memory for the GPT Listener */
EFI_STATUS EFIAPI GPTListenerAllocMem(void);

/* This initializes the GPT Listener */
EFI_STATUS EFIAPI GPTListenerInit(EFI_BLOCK_IO_PROTOCOL *BlkIo);

#endif /* __GPTLISTENER_H_ */
