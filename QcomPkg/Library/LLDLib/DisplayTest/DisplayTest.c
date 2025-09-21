/** @file DisplayApp.c
   
  Test application for DisplayDxe graphics output protocol

  Copyright (c) 2010-2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  
**/


/*=========================================================================
      Include Files
==========================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/EFIDisplayPwrCtrl.h>
#include <Protocol/EFIDisplayPwr.h>
#include <Protocol/EFIClock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern EFI_STATUS DisplayBVT(UINT32 uTestNumber);

UINT32 DisplayTestMain(uint32 dwParam, char *apszParam[]){
  EFI_STATUS Status = EFI_SUCCESS;
  for (int i=0; i<dwParam; i++) {
    int val = AsciiStrDecimalToUintn(apszParam[i]);
    Status = DisplayBVT((UINT32)val);
    if (Status != EFI_SUCCESS) {
        return Status;
    }
  }
  return Status;
}
