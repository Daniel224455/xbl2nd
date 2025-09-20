/**@file DbiDumpLib.c

  Copyright (c) 2016, Qualcomm Technologies Inc. All rights reserved.<BR>

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 
 --------   ---     -----------------------------------------------------------
 06/13/16   vk      Initial version
=============================================================================*/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ArmLib.h>
#include <Protocol/DebugSupport.h>

#include <Library/QcomBaseLib.h>


UefiInfoBlkType* GetInfoBlock (VOID);
EFI_STATUS DoWatchDogReset ( VOID );

EFI_STATUS
SetupDBIDumpTable (UINT64 MemoryBase)
{
  return EFI_SUCCESS;
}

EFI_STATUS
InitDbiDump(VOID)
{
   return EFI_SUCCESS;
}

EFI_STATUS
SaveUefiContext (EFI_SYSTEM_CONTEXT SystemContext)
{
  return EFI_SUCCESS;
}


EFI_STATUS
HandleDbiDump (EFI_SYSTEM_CONTEXT SystemContext)
{
  return EFI_SUCCESS;
}


