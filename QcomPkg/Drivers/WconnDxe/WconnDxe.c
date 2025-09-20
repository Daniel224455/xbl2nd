/** @file

  Copyright (c) 2019, Qualcomm Technologies, Inc. All rights reserved.

  Wireless Connectivity power Driver 

**/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/12/19   owen    initial edit
==============================================================================*/

/*==============================================================================

                       INCLUDE FILES FOR THIS MODULE

==============================================================================*/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <WconnBsp.h>
#include <Protocol/EFIPlatformInfo.h>


/*==============================================================================

                         DEFINES FOR THIS MODULE

==============================================================================*/

#define WCONN_MTP_HST_AND_5G            1
#define WCONN_MTP_HST_AND_4GORNONE    4

/*==============================================================================

                     EXTERNAL DEFINES FOR THIS MODULE

==============================================================================*/


/*==============================================================================

                      PROTOTYPES USED IN THIS MODULE

==============================================================================*/


/*==============================================================================

                     GLOBAL VARIABLES FOR THIS MODULE

==============================================================================*/


/*==============================================================================

                             API IMPLEMENTATION

==============================================================================*/


EFI_STATUS
EFIAPI
WconnEntryPoint (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_PLATFORMINFO_PROTOCOL             *PlatInfoProtocol = NULL;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE   PlatInfo;
  EFI_STATUS                            Status;

  Status = gBS->LocateProtocol(&gEfiPlatformInfoProtocolGuid, NULL,
                               (VOID **)&PlatInfoProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "%a: locate PlatformInfo protocol failed\r\n", __func__));
    return Status;
  }

  Status = PlatInfoProtocol->GetPlatformInfo(PlatInfoProtocol, &PlatInfo);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "%a: Getting PlatformInfo failed\r\n", __func__));
    return Status;
  }

  if (PlatInfo.platform == EFI_PLATFORMINFO_TYPE_MTP) {
    switch (PlatInfo.subtype) {
    case WCONN_MTP_HST_AND_5G:
    case WCONN_MTP_HST_AND_4GORNONE:
    {
      Status = WconnWlanPowerOn();
      if (EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR, "%a: Error powering up WLAN\r\n", __func__));
        return Status;
      }
      break;
    }
    default:
      break;
    }
  }

  return Status;
}

