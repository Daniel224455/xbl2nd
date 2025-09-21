/*********************************************************************
 * @file CookieLib.c
 *
 * @brief Cookie support
 *
 * Copyright (c) 2016 by Qualcomm Technologies, Inc. All Rights Reserved.
 *
 *********************************************************************/
/*=======================================================================
                        Edit History

 when       who     what, where, why
 --------   ----    ---------------------------------------------------
 11/08/17   ps      uefi magic cookie APIs
 09/26/16   vk      Cleanup to make it baselib
 08/11/16   bh      Initial revision.

========================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/SerialPortShLib.h>
#include <Library/TzRuntimeLib.h>
#include <Library/CookieLib.h>

#include <boot_shared_imem_cookie.h>
/*Define a global pointer which points to the boot shared imem cookie structure */
struct boot_shared_imem_cookie_type *BootSharedImemCookiePointer = NULL;  
EFI_STATUS
EFIAPI
SetDLOADCookie(VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 mDloadCookieAddr = 0;
  UINT32 mDloadCookieValue = 0;
  UINT64 DloadCookieInfo[2];

  Status = GetConfigValue ("DloadCookieAddr", &mDloadCookieAddr);
  if ((Status != EFI_SUCCESS) || (mDloadCookieAddr == 0))
  {
    DEBUG ((EFI_D_ERROR, "DloadCookieAddr not found in uefiplat.cfg\r\n"));
    return Status;
  }

  Status = GetConfigValue ("DloadCookieValue", &mDloadCookieValue);
  if ((Status != EFI_SUCCESS) || (mDloadCookieValue == 0))
  {
    DEBUG ((EFI_D_ERROR, "DloadCookieValue not found in uefiplat.cfg\r\n"));
    return Status;
  }

  /* For SW that supports new SCM call, we have the cookie size set to 0 */
  DloadCookieInfo[0] = mDloadCookieAddr;
  DloadCookieInfo[1] = mDloadCookieValue;
  Status = TzFastcall (TZ_IO_ACCESS_WRITE_ID, TZ_IO_ACCESS_WRITE_ID_PARAM_ID, DloadCookieInfo, 2);
  if (Status != EFI_SUCCESS)
  {
    DEBUG(( EFI_D_ERROR, "TzFastcall() failed for TZ_IO_ACCESS_WRITE_ID, Status = (0x%x)\r\n", Status));
  }
  
  return Status;
}

VOID
EFIAPI
SetUefiCrashCookie(VOID)
{
  BootSharedImemCookiePointer = (struct boot_shared_imem_cookie_type *)((UINT64)PcdGet32(PcdIMemCookiesBase));
  if(BootSharedImemCookiePointer == NULL)
  {
    DEBUG((EFI_D_INFO, "uefi_image_magic set: failed\n"));
  }
  else
  {
    BootSharedImemCookiePointer->uefi_image_magic = 0x75656669 ;
  }
}


VOID
EFIAPI
ClearUefiCrashCookie(VOID)
{
  BootSharedImemCookiePointer = (struct boot_shared_imem_cookie_type *)((UINT64)PcdGet32(PcdIMemCookiesBase));
  if(BootSharedImemCookiePointer == NULL)
  {
    DEBUG((EFI_D_INFO, "uefi_image_magic clear: failed\n")); 
  }
  else
  {
    BootSharedImemCookiePointer->uefi_image_magic = 0x0;
  }
}


EFI_STATUS
EFIAPI
ClearDLOADCookie(VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 mDloadCookieAddr = 0;
  UINT64 DloadCookieInfo[2];
    
  Status = GetConfigValue ("DloadCookieAddr", &mDloadCookieAddr);
  if ((Status != EFI_SUCCESS) || (mDloadCookieAddr == 0))
  {
    DEBUG ((EFI_D_ERROR, "DloadCookieAddr not found in uefiplat.cfg\r\n"));
    return Status;
  }
   
 /* For SW that supports new SCM call, we have the cookie size set to 0 */
  DloadCookieInfo[0] = mDloadCookieAddr;
  DloadCookieInfo[1] = 0x0;
  Status = TzFastcall (TZ_IO_ACCESS_WRITE_ID, TZ_IO_ACCESS_WRITE_ID_PARAM_ID, DloadCookieInfo, 2);
  if (Status != EFI_SUCCESS)
  {
    DEBUG(( EFI_D_ERROR, "TzFastcall() failed for TZ_IO_ACCESS_WRITE_ID, Status = (0x%x)\r\n", Status));
  }

  return Status;
}

EFI_STATUS
EFIAPI
ClearDLOADCookieRuntime(
  UINTN mDloadCookieAddr
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64 DloadCookieInfo[2];  

  if (mDloadCookieAddr == 0)
  {
    return EFI_INVALID_PARAMETER;
  }
  
  /* For SW that supports new SCM call, we have the cookie size set to 0 */
  DloadCookieInfo[0] = mDloadCookieAddr;
  DloadCookieInfo[1] = 0x0;
  Status = TzFastcall (TZ_IO_ACCESS_WRITE_ID, TZ_IO_ACCESS_WRITE_ID_PARAM_ID, DloadCookieInfo, 2);
  if (Status != EFI_SUCCESS)
  {
    DEBUG(( EFI_D_ERROR, "TzFastcall() failed for TZ_IO_ACCESS_WRITE_ID, Status = (0x%x)\r\n", Status));
  }

  return Status;
}

BOOLEAN
EFIAPI
IsDLOADCookieSet ( VOID )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 mDloadCookieAddr = 0;
  UINT64 DloadCookieInfo[2] = {0};
  
  Status = GetConfigValue ("DloadCookieAddr", &mDloadCookieAddr);
  if ((Status != EFI_SUCCESS) || (mDloadCookieAddr == 0))
  {
    DEBUG ((EFI_D_ERROR, "DloadCookieAddr not found in uefiplat.cfg\r\n"));
    return FALSE;
  }
  
  /* Only check the bits indicated by the cookie */
  /*Bits 20 to 23 are reserved for XBL dload cookies */
  DloadCookieInfo[0] = mDloadCookieAddr;
  Status = TzFastcall (TZ_IO_ACCESS_READ_ID, TZ_IO_ACCESS_READ_ID_PARAM_ID, DloadCookieInfo, 1);
  if (Status != EFI_SUCCESS)
  {
    DEBUG(( EFI_D_ERROR, "TzFastcall() failed for TZ_IO_ACCESS_READ_ID, Status = (0x%x)\r\n", Status));
  }

  if ( DloadCookieInfo[0] & DLOAD_MODE_BIT_MASK)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
   
}


EFI_STATUS
EFIAPI
SetEDLCookie(VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 mEDLCookieAddr = 0;
  UINT64 EDLCookieValue[2];

  Status = GetConfigValue ("DloadCookieAddr", &mEDLCookieAddr);
  if ((Status != EFI_SUCCESS) || (mEDLCookieAddr == 0))
  {
    DEBUG ((EFI_D_WARN, "EDLCookieAddr not found in uefiplat.cfg. Using PcdEDLCookieAddr.\r\n"));
    mEDLCookieAddr = PcdGet32(PcdEDLCookieAddr);
  }
  
  EDLCookieValue[0] = mEDLCookieAddr;
  EDLCookieValue[1] = 0x1;
  Status = TzFastcall (TZ_IO_ACCESS_WRITE_ID, TZ_IO_ACCESS_WRITE_ID_PARAM_ID, EDLCookieValue, 2);
  if (Status != EFI_SUCCESS)
  {
    DEBUG(( EFI_D_ERROR, "TzFastcall() failed for TZ_IO_ACCESS_WRITE_ID, Status = (0x%x)\r\n", Status));
  }

  return Status;
}

EFI_STATUS
EFIAPI
ClearEDLCookie(VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 mEDLCookieAddr = 0;
  UINT64 PcdEDLCookie[2];
  
  Status = GetConfigValue ("EDLCookieAddr", &mEDLCookieAddr);
  if ((Status != EFI_SUCCESS) || (mEDLCookieAddr == 0))
  {
    DEBUG ((EFI_D_WARN, "EDLCookieAddr not found in uefiplat.cfg. Using PcdEDLCookieAddr.\r\n"));
    mEDLCookieAddr = PcdGet32(PcdEDLCookieAddr);
  }

  if(PcdGet32(PcdEDLCookieSize) == 4)
  {
    /* Only clear the bit indicated by the cookie */
    PcdEDLCookie[0] = MmioRead32(mEDLCookieAddr) & ~PcdGet32(PcdEDLCookie0);
    PcdEDLCookie[1] = 0;
    Status = TzFastcall (TZ_FORCE_DLOAD_ID, TZ_FORCE_DLOAD_ID_PARAM_ID, PcdEDLCookie, 2);
    if (Status != EFI_SUCCESS)
    {
      DEBUG(( EFI_D_ERROR, "TzFastcall() failed for TZ_FORCE_DLOAD_ID, Status = (0x%x)\r\n", Status));
    }
  }
  else 
  {
    /* Clear the cookies */
    MmioWrite32(mEDLCookieAddr, 0);
    MmioWrite32(mEDLCookieAddr + 4, 0);
    MmioWrite32(mEDLCookieAddr + 8, 0);
  }

  return Status;
}

