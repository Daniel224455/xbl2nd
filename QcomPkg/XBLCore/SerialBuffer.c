/** @file SerialBuffer.c
  
  Copyright (c) 2020 Qualcomm Technologies, Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY

 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/20/20   ps      Log buffer operation moved here from sec.c

*/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SerialPortLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Include/UefiInfoBlk.h>
#include "SerialPortShLibInstall.h"
#include "UefiPlatCfg.h"
#define SERIAL_BUFFER_SIZE        0x008000
#define MAX_SERIAL_BUFFER_SIZE    0x100000

UefiInfoBlkType* GetInfoBlock (VOID);


STATIC VOID
StaticAllocLogBuffer(UINTN* UARTLogBufferPtr, UINTN UARTLogBufferSize)
{

  UefiInfoBlkType* UefiInfoBlkPtr;
  /* Get info block */
  UefiInfoBlkPtr = GetInfoBlock();

  /* Change UartLogBuffer Ptr and size in UEFIInfoBlk structure */
  UefiInfoBlkPtr->UartLogBufferPtr = UARTLogBufferPtr;
  UefiInfoBlkPtr->UartLogBufferLen = UARTLogBufferSize;   
  
  DEBUG((EFI_D_INFO, "UART Buffer size set to 0x%x \n", UARTLogBufferSize)); 
  
  ZeroMem((VOID *) UARTLogBufferPtr, UARTLogBufferSize);
  /* Copy the exisitng Serial log buffer contents in new one and update the Serial Buffer pointer. */
  SerialBufferReInit((UINT8*) UARTLogBufferPtr, (UINT32) UARTLogBufferSize);
    
}

STATIC VOID
DynamicAllocLogBuffer(UINTN UARTLogBufferSize)
{
  UINTN            *UARTLogBufferPtr = NULL;
  UefiInfoBlkType* UefiInfoBlkPtr;
  /* Get info block */
  UefiInfoBlkPtr = GetInfoBlock();

  UARTLogBufferPtr = AllocatePagesRuntimeServiceData(EFI_SIZE_TO_PAGES(UARTLogBufferSize));
  if ((UARTLogBufferPtr == NULL))
  {
    DEBUG((EFI_D_ERROR, "Error Allocating buffer for UART log \n"));  //continue using old buffer of size 32K 
  }
  else
  {
    /* Change UartLogBuffer Ptr and size in UEFIInfoBlk structure */
    UefiInfoBlkPtr->UartLogBufferPtr = (UINTN*) UARTLogBufferPtr;
    UefiInfoBlkPtr->UartLogBufferLen = UARTLogBufferSize;
    ZeroMem((VOID *) UARTLogBufferPtr, UARTLogBufferSize);
    /* Copy the exisitng Serial log buffer contents in new one and update the Serial Buffer pointer. */
    SerialBufferReInit((UINT8*) UARTLogBufferPtr, (UINT32) UARTLogBufferSize);
  }
}
/**
  Dynamically or statically allocate UEFI log buffer and calls for addition of it in minidump
  based on uefiplat settings
  @param  none

  @retval none
**/

VOID
UEFISerialOperations (VOID)
{
  EFI_STATUS       Status = EFI_SUCCESS;
  MemRegionInfo    LogBufferMemInfo;
  UINTN            UARTLogBufferSize = 0; 

  /* Update UART log buffer size if specififed in uefiplat else allocate 32KB of serial buffer in DXE Heap. */
  Status = GetConfigValue ("UARTLogBufferSize", (UINT32*) &UARTLogBufferSize);
  if (Status == EFI_SUCCESS)
  {
    Status = GetMemRegionInfoByName("XBL Log Buffer", &LogBufferMemInfo);
    /* when Requested buffer size is within dedicated region in ipcat */
    if (Status == EFI_SUCCESS && UARTLogBufferSize <= LogBufferMemInfo.MemSize)
    {
      StaticAllocLogBuffer((UINTN*)LogBufferMemInfo.MemBase, (UINTN)LogBufferMemInfo.MemSize);
    }
    /* When Requested buffer size is greater than size of dedicated region in ipcat */
    else if ((Status == EFI_SUCCESS && UARTLogBufferSize > LogBufferMemInfo.MemSize) ||
             (Status != EFI_SUCCESS && UARTLogBufferSize > SERIAL_BUFFER_SIZE))
    {
      if(UARTLogBufferSize > MAX_SERIAL_BUFFER_SIZE)
      {
        UARTLogBufferSize = MAX_SERIAL_BUFFER_SIZE; /* 1MB serial buffer size */
      }   
      DEBUG((EFI_D_INFO, "UART Buffer size set to 0x%x \n", UARTLogBufferSize)); 

      DynamicAllocLogBuffer(UARTLogBufferSize);
    }
  }
  /* When UARTLogBufferSize is not specified */
  else
  {
    Status = GetMemRegionInfoByName("XBL Log Buffer", &LogBufferMemInfo);
    
    if (Status == EFI_SUCCESS)
    {
      /* Change UartLogBuffer Ptr and size in UEFIInfoBlk structure */
      StaticAllocLogBuffer((UINTN*)LogBufferMemInfo.MemBase, (UINTN)LogBufferMemInfo.MemSize);
    }
    /*when both UARTLogBufferSize & 'XBL Log Buffer' are not present in uefiplat.cfg*/
    else
    {
      /* keep using exiting Log buffer */
    }
  }
  /* Prevent Reinitializing of Serial Buffer */
  SerialBufferReInitLock();
}
