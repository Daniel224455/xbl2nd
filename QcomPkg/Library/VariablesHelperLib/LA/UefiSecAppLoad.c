/**********************************************************************
 * @file DefaultVarStore.c
 *
 * Copyright (c) 2018-2019 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 *********************************************************************/
/*=====================================================================
                        Edit History

when       who     what, where, why
--------   ----    ----------------------------------------------------
01/17/19   ai      Dynamically select varstore partition
=====================================================================*/

#include <Library/QcomLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiSecAppLoaderLib.h>

#include <Protocol/EFIQseecom.h>

#define UEFISECAPP_PARTITION_GUID \
    { 0xBE8A7E08, 0x1B7A, 0x4CAE, { 0x99, 0x3A, 0xD5, 0xB7, 0xFB, 0x55, 0xB3, 0xC2 } }

/**
 * Load UefiSecApp TA
 */
EFI_STATUS LoadUefiSecApp (VOID)
{
  EFI_STATUS              Status      = EFI_INVALID_PARAMETER;
  QCOM_QSEECOM_PROTOCOL   *Qseecom    = NULL;
  EFI_GUID                UefiSecAppPartitionGuid = UEFISECAPP_PARTITION_GUID;
  HandleInfo              HandleInfoList = {0};
  UINT32                  HandleId    = 0;
  UINT32                  Attribs     = 0;
  UINT32                  MaxHandles  = 0;

  /* Check if uefisecapp partition exists */
  Attribs = BLK_IO_SEL_MATCH_ROOT_DEVICE;
  Attribs |= BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;
  HandleFilter->PartitionType = &UefiSecAppPartitionGuid;

  Status = GetBlkIOHandles(
      Attribs, HandleFilter, &HandleInfoList, &MaxHandles);
  if (EFI_ERROR(Status) || (MaxHandles != 1))
  {
    DEBUG((EFI_D_ERROR, "UefiSecApp partition %g - #handles = %d\n", &UefiSecAppPartitionGuid, MaxHandles));
    Status = EFI_NOT_FOUND; /* Force error status if MaxHandles > 1 */
    goto Exit;
  }

  /* Get handle to TzeLoaderProtocol */
  Status = gBS->LocateProtocol(&gQcomQseecomProtocolGuid, NULL, (VOID **)&Qseecom);
  if(EFI_ERROR(Status))
  {
    DEBUG ((EFI_D_ERROR, "Unable to locate QseeCom protocol: %r\n", Status));
    goto Exit;
  }

  /* Start uefisecapp */
  Status = Qseecom->QseecomStartAppByGuid(Qseecom, &UefiSecAppPartitionGuid, &HandleId);
  if (EFI_ERROR(Status))
  {
    DEBUG ((EFI_D_ERROR, "LoadImageFromPartition(UefiSecApp) : %r\n", Status));
    goto Exit;
  }

Exit:
  return Status;
}

