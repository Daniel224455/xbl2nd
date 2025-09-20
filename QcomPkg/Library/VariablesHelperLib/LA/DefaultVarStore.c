/**********************************************************************
 * @file DefaultVarStore.c
 *
 * Copyright (c) 2019 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 *********************************************************************/
/*=====================================================================
                        Edit History

when       who     what, where, why
--------   ----    ----------------------------------------------------
01/17/19   ai      Dynamically select varstore partition
=====================================================================*/

#include <Library/VariablesHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BootConfig.h>

/**
 * Set default location of varstore dynamically for non-secure configuration
 **/
EFI_STATUS GetDefaultVarStore (
    UINT32 *VarStoreQseeStorId
    )
{
  if (VarStoreQseeStorId == NULL)
  {
    DEBUG ((EFI_D_ERROR, "%a(%d): invalid parameter!\n", __FILE__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "%a(%d): using LA varstore polices\n", __FILE__, __LINE__));

  /* LA policies */
  if (boot_from_emmc())
    *VarStoreQseeStorId = QSEE_STOR_EMMC_USER;
  else if (boot_from_ufs())
    *VarStoreQseeStorId = QSEE_STOR_EMMC_GPP1;
  else if (boot_from_spi_nor())
    *VarStoreQseeStorId = QSEE_STOR_SPINOR_ALL;
  else
  {
    DEBUG((EFI_D_ERROR, "%a(%d): invalid boot device!\n, __FILE__, __LINE__"))
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

