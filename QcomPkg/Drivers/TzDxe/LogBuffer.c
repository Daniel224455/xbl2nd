/** @file  

               LogBuffer.c

GENERAL DESCRIPTION
  This library sets up log buffer and display the content in log buffer

	
  Copyright (c) 2018 Copyright Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

*/

/*===========================================================================
                             EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 09/11/18   dp       Allocating buffer pool from ShmBridge for log
 02/08/18   shl      Initial version

===========================================================================*/

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIScm.h>
#include <Include/scm_qsee_interface.h>
#include <Library/SerialPortLib.h>
#include "LogBuffer.h"
#include "QcomLib.h"
#include <Protocol/EFIShmBridge.h>


/*===========================================================================

                     DEFINITIONS AND CONSTANTS

===========================================================================*/

UINT64 *log_buffer = NULL;
EFI_EVENT  ExitBootServicesQseeLogEvent;

EFI_STATUS QseeDeregisterLogBuffer();
VOID EFIAPI ExitBootQseeLogHandler(
  IN EFI_EVENT  Event,
  IN VOID      *Context
);



EFI_STATUS QseeRegisterLogBuffer()
{
  EFI_STATUS   Status;
  UINT32       app_id = 0;
  UINT64      *log = NULL;

  QCOM_SCM_PROTOCOL           	*QcomScmProtocol = NULL;
  EFI_SHMBRIDGE_PROTOCOL 	  	*ShmBridgeProtocol = NULL;
  qsee_register_log_buffer_t  	*qsee_syscallp = NULL;
  UINT64 						ShmBridgeSize = QSEE_LOG_BUF_SIZE;

  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID**)&QcomScmProtocol);
  if (Status != EFI_SUCCESS)
  {
    return Status;
  }

  Status = gBS->LocateProtocol(&gEfiShmBridgeProtocolGuid, NULL, (VOID **)&ShmBridgeProtocol);
  if (Status != EFI_SUCCESS)
  {
    return Status;
  }

    // Register event for exit BS
  Status = gBS->CreateEventEx ( EVT_NOTIFY_SIGNAL,
                                TPL_CALLBACK,
                                ExitBootQseeLogHandler,
                                NULL,
                               &gEfiEventExitBootServicesGuid,
                               &ExitBootServicesQseeLogEvent
                              );
  if (Status != EFI_SUCCESS) 
  {
    DEBUG(( EFI_D_ERROR, "CreateEventEx() failed for QseeLog ---, Status = (0x%p)\r\n", Status));
    goto Exit;
  }

  qsee_syscallp= (qsee_register_log_buffer_t*)AllocatePool(sizeof(qsee_register_log_buffer_t));
  if (NULL == qsee_syscallp)
  {
     DEBUG((EFI_D_WARN, "Failed to allocate memory:(size=%d)\n", sizeof(qsee_register_log_buffer_t)));
     return EFI_OUT_OF_RESOURCES;
  }

  log = (UINT64 *)ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &ShmBridgeSize, ShmBridgeBootSvcData);
  if (NULL == log)
  {
     Status =  EFI_OUT_OF_RESOURCES;
     DEBUG ((EFI_D_ERROR, "Failed to allocate memory:(size=%d) from SHMBridge\n", QSEE_LOG_BUF_SIZE));
     goto Exit;
  }

  SetMem(log, QSEE_LOG_BUF_SIZE, 0x0);

  SetMem(qsee_syscallp, sizeof(qsee_register_log_buffer_t), 0x0);
  qsee_syscallp->log_buffer_addr = (UINT32)log;
  qsee_syscallp->log_buffer_size = (UINT32)QSEE_LOG_BUF_SIZE;

  Status = QcomScmProtocol->ScmSendCommand(QcomScmProtocol,
                                           APP_REGISTER_LOG_BUF_COMMAND,
                                           &(app_id),
                                           qsee_syscallp,
                                           sizeof(qsee_register_log_buffer_t),
                                           NULL,
                                           0);

  if (Status != EFI_SUCCESS) 
  {
      DEBUG((EFI_D_ERROR, "Failed to send register log buffer commnd: %d\n", Status));
      goto Exit;
  }

  log_buffer = log;
  Status = EFI_SUCCESS;

Exit:

  if(qsee_syscallp)
	FreePool(qsee_syscallp);
  return Status;
}

/**
  This is called when notified that boot services has exited

  @param  IN     Event          The event to be signaled.
  @param  IN OUT *Context       A pointer to event context.

  @retval VOID   

**/
VOID EFIAPI ExitBootQseeLogHandler(
  IN EFI_EVENT  Event,
  IN VOID      *Context
)
{
  EFI_STATUS   Status;

  Status = QseeDeregisterLogBuffer();

  if (Status != EFI_SUCCESS)
  {
     DEBUG ((EFI_D_WARN, "QseeDeregisterLogBuffer failed\n"));
  }

}


EFI_STATUS QseeDeregisterLogBuffer()

{
  EFI_STATUS   Status;
  EFI_SHMBRIDGE_PROTOCOL 	  	*ShmBridgeProtocol = NULL;

  Status = gBS->LocateProtocol(&gEfiShmBridgeProtocolGuid, NULL, (VOID **)&ShmBridgeProtocol);
  if (Status != EFI_SUCCESS)
  {
    return Status;
  }

  SetMem(log_buffer, QSEE_LOG_BUF_SIZE, 0x0);

  Status = ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, log_buffer);
  if (Status != EFI_SUCCESS)
  {
     DEBUG ((EFI_D_ERROR, "Failed to delete qsee log buffer from SHMBridge\n"));
     goto Exit;
  }

  log_buffer = NULL;
  
Exit:  
  return Status;
}


EFI_STATUS DisplayQseeLog(UINT32 startOffset, UINT32 endOffset)
{
  EFI_STATUS   ret = EFI_SUCCESS;
  UINT32       MaxBufSize = 0;
  UINT32       LogBufSize = 0;
  UINT32       LogBufFirstHalf = 0;
  void        *pLogBuf = NULL;
  tzdbg_log_t *log = (tzdbg_log_t *)log_buffer;

  if(log_buffer != NULL) //If the Log Buffer is still registered with Global SHMBridge, go ahead and do logging
  {
	  MaxBufSize = QSEE_LOG_BUF_SIZE - sizeof(struct tzdbg_log_pos_t);
	  if (startOffset < endOffset)
	  {
	    LogBufSize = endOffset - startOffset;
	    pLogBuf = AllocatePool(LogBufSize);
	    if (NULL == pLogBuf)
	    {
	      ret = EFI_DEVICE_ERROR;
	      DEBUG ((EFI_D_ERROR, "Failed to alloc buffer to print TZ Log:%u\n", LogBufSize));
	      goto Exit;
	    }
	    SetMem(pLogBuf, LogBufSize, 0);
	    CopyMem(pLogBuf, (void *)(log->log_buf + startOffset), LogBufSize);
	  }
	  else if ( endOffset < startOffset)
	  {
	    LogBufSize =  MaxBufSize - (startOffset - endOffset);
	    LogBufFirstHalf = MaxBufSize - startOffset;
	    pLogBuf = AllocatePool(LogBufSize);
	    if (NULL == pLogBuf)
	    {
	      ret = EFI_DEVICE_ERROR;
	      DEBUG ((EFI_D_ERROR, "Failed to alloc buffer to print TZ Log:%u\n", LogBufSize));
	      goto Exit;
	    }
	    SetMem(pLogBuf, LogBufSize, 0);
	    CopyMem(pLogBuf, (void *)(log->log_buf + startOffset), LogBufFirstHalf);
	    CopyMem((void *)(pLogBuf+ LogBufFirstHalf), log->log_buf, endOffset);
	  }
	  else //endOffset == startOffset (no log is updated)
	  {
	    ret = EFI_SUCCESS;
	    goto Exit;
	  }

	  SerialPortWrite((UINT8 *)pLogBuf, (UINTN)(LogBufSize));
	  FreePool(pLogBuf);
  }
Exit:
  return ret;
}

