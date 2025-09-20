/** @file
  Source file PIL commands to authenticate FVs
   
  Copyright (c) 2017 Qualcomm Technologies, Inc. All rights reserved. 
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/02/18   kpa     Update AuthenticateElf to accept FvGuid
 09/08/17   bd      Initial revision

=============================================================================*/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/QcomLib.h>
#include <Include/scm_sip_interface.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Protocol/EFIScm.h>
#include <Library/DebugLib.h>
#include <Library/TzRuntimeLib.h>

#include "GuidedFvLibInternal.h"

extern EFI_GUID gQcomScmProtocolGuid;
extern EFI_GUID gEfiAblFvNameGuid;

STATIC UINT64 ScmParam[SCM_MAX_NUM_PARAMETERS] = {0};
STATIC UINT64 ScmResult[SCM_MAX_NUM_RESULTS] = {0};


STATIC inline EFI_STATUS SecurePILInitID
(
  QCOM_SCM_PROTOCOL *QcomScmProtocol,
  UINT64 ELFMetaBase,
  tz_pil_proc_e_type tz_pil_proc
)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;

  ScmParam[0] = tz_pil_proc;
  ScmParam[1] = ELFMetaBase;

  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  /*
   * Authenticate ELF meta data with PIL
   */
  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_PIL_INIT_ID,
                                 TZ_PIL_INIT_ID_PARAM_ID,
                                 ScmParam,
                                 ScmResult);
  return Status;
}

STATIC inline EFI_STATUS SecurePILMemID
( 
  QCOM_SCM_PROTOCOL *QcomScmProtocol,
  UINT64 imageBase, 
  UINTN imageSize,
  tz_pil_proc_e_type tz_pil_proc
)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;

  ScmParam[0] = tz_pil_proc;
  ScmParam[1] = imageBase;
  ScmParam[2] = (UINT64)imageSize;
  
  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_PIL_MEM_ID,
                                 TZ_PIL_MEM_ID_PARAM_ID,
                                 ScmParam,
                                 ScmResult);
  return Status;
}

STATIC inline EFI_STATUS SecurePILAuthResetID
( 
  QCOM_SCM_PROTOCOL *QcomScmProtocol,
  tz_pil_proc_e_type tz_pil_proc
)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;
  
  ScmParam[0] = tz_pil_proc;

  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  /*
   * Authenticate and load the image
   */
  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_PIL_AUTH_RESET_ID,
                                 TZ_PIL_AUTH_RESET_ID_PARAM_ID,
                                 ScmParam,
                                 ScmResult);

  return Status;
}

STATIC inline EFI_STATUS SecurePILUnlockXPUID
( 
  QCOM_SCM_PROTOCOL *QcomScmProtocol,
  tz_pil_proc_e_type tz_pil_proc
)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;

  ScmParam[0] = tz_pil_proc;

  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  /*
   * Unlock the XPU and free the memory
   */
  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_PIL_UNLOCK_XPU_ID,
                                 TZ_PIL_UNLOCK_XPU_ID_PARAM_ID,
                                 ScmParam,
                                 ScmResult);
  
  return Status;
}

EFI_STATUS AuthenticateElf(void* ElfBase, UINTN ImageSize, EFI_GUID *FvGuid, void **FvBase)
{
  void                    *MetaAddr = NULL;
  void                    *ProgSegBase = NULL;
  uint32                   ProgSegSize = 0;
  EFI_STATUS               Status = EFI_NOT_FOUND;
  STATIC QCOM_SCM_PROTOCOL *QcomScmProtocol = NULL;
  tz_pil_proc_e_type tz_pil_proc = TZ_PIL_AUTH_UNSUPPORTED_PROC;

  if( ElfBase == NULL || FvGuid == NULL || FvBase == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  
  if(CompareGuid(&gEfiAblFvNameGuid, FvGuid))
  {
    tz_pil_proc = TZ_PIL_AUTH_ABLFV_PROC;
  }
  else
  {
    tz_pil_proc = TZ_PIL_AUTH_UEFIFV_PROC;
  }
  
  Status = EFISplitELFFile(ElfBase, &MetaAddr, &ProgSegBase, &ProgSegSize);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed to parse ELF successfully\r\n", __func__));
    return Status;
  }
  
  *FvBase = ProgSegBase;  

  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID**)&QcomScmProtocol);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed to locate QcomScmProtocol\r\n", __func__));
    return Status;
  }


  /* TZ PIL init */
  Status = SecurePILInitID(QcomScmProtocol,(UINT64)MetaAddr, tz_pil_proc);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed TZ_PIL_INIT\r\n", __func__));
    return Status;
  }

  /* TZ PIL mem */
  Status = SecurePILMemID(QcomScmProtocol,(UINT64)ProgSegBase, ProgSegSize, tz_pil_proc);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed TZ_PIL_MEM\r\n", __func__));
   return Status;
  }

  /* TZ PIL auth reset */
  Status = SecurePILAuthResetID(QcomScmProtocol,tz_pil_proc);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed TZ_PIL_AUTH_RESET\r\n", __func__));
    return Status;
  }

  /* TZ PIL unlock xpu */
  Status = SecurePILUnlockXPUID(QcomScmProtocol, tz_pil_proc);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed TZ_PIL_UNLOCK_XPU\r\n", __func__));
    return Status;
  }  
  
  return Status;  
}

