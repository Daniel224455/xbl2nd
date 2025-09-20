/**********************************************************************
 * @file UefiSecAppLoad.c
 *
 * Copyright (c) 2018-2019 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 *********************************************************************/
/*=====================================================================
                        Edit History

when       who     what, where, why
--------   ----    ----------------------------------------------------
03/08/18   ai      Initial Null checkin
=====================================================================*/


#include <Library/VariablesHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFITzeLoader.h>

/**
 * Load UefiSecApp TA
 */
EFI_STATUS LoadUefiSecApp (VOID)
{
  EFI_STATUS                Status      = EFI_DEVICE_ERROR;
  EFI_TZE_LOADER_PROTOCOL   *TzeLoader  = NULL;

  DEBUG ((EFI_D_INFO, "Skipping uefisecapp TA load!\n"));

  /* UefiSecApp TA is loaded by TzDxe for Win boot path.
     TzDxe must be loaded prior to  UefiSecApp TA */
  Status = gBS->LocateProtocol (&gEfiTzeLoaderProtocolGuid, NULL, (VOID**)&TzeLoader);
  return Status;
}
