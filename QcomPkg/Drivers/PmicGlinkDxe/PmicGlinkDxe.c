/**
@file  PmicGlinkDxe.c
@brief USB Power Control Module related Declaration
*/
/*=============================================================================
Copyright (c) 2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
10/31/18   cs      change all success case log to INFO level
10/30/18   ivy     support battery missing error handling, either shutdown or booting to HLOS
10/21/18   ivy     Updated pmicglink to polling based and support fetching usb info from charger FW
08/24/18   cs      added getting full capacity from external fg.
03/29/18   cs      added ucsi support.
11/06/17   cs      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>

/**
  Library Dependencies
*/
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

/**
  Protocol Dependencies
*/
#include <Protocol/EFIPmicGlink.h>
#include "pmic_glink.h"


/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
EFI_PMIC_GLINK_PROTOCOL PmicGlinkProtocolImplementation;


/*===========================================================================*/
/*                  FUNCTION DEFINITIONS                                     */
/*===========================================================================*/
/**
  Initialize PmicGlink Module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS PmicGlinkInit(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status = EFI_SUCCESS;

//  Status = pmic_glink_Init();

  if(EFI_SUCCESS == Status)
  {
    Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle,
                                                    &gpmicGlinkProtocolGuid,
                                                    &PmicGlinkProtocolImplementation,
                                                    NULL);
  }
  return Status;
}



/**
EFI_PmicGlink_UCSIReadBuffer()

@brief
read ucsi data buffer from ADSP
*/
EFI_STATUS
EFIAPI
EFI_PmicGlink_UCSIReadBuffer
(
   UINT8** ppReadBuffer,
   UINT8 size
)
{
  if (ppReadBuffer != NULL && *ppReadBuffer != NULL)
  {
    return pmic_glink_ucsi_read_buffer(ppReadBuffer, size);
}
  return EFI_INVALID_PARAMETER;
}


/**
EFI_PmicGlink_UCSIWriteBuffer()

@brief
write data buffer to ADSP
*/
EFI_STATUS
EFIAPI
EFI_PmicGlink_UCSIWriteBuffer
(
  UINT8* pWriteBuffer,
  UINT8 size
)
{

  if (pWriteBuffer != NULL)
{
    return pmic_glink_ucsi_write_buffer(pWriteBuffer, size);
}

  return EFI_INVALID_PARAMETER;
}

/**
EFI_PmicGlink_USBCReadBuffer()

@brief
read usbc data buffer from ADSP
*/
EFI_STATUS
EFIAPI
EFI_PmicGlink_USBCReadBuffer
(
   UINT8** ppReadBuffer,
   UINT8 size
)
{
  if (ppReadBuffer != NULL && *ppReadBuffer != NULL)
  {
    return pmic_glink_usbc_read_buffer(ppReadBuffer, size);
	}
  return EFI_INVALID_PARAMETER;
}


/**
EFI_PmicGlink_USBCWriteBuffer()

@brief
write usbcdata buffer to ADSP
*/
EFI_STATUS
EFIAPI
EFI_PmicGlink_USBCWriteBuffer
(
  UINT8* pWriteBuffer,
  UINT8 size
)
{

  if (pWriteBuffer != NULL)
{
    return pmic_glink_usbc_write_buffer(pWriteBuffer, size);
}

  return EFI_INVALID_PARAMETER;
}

EFI_STATUS
EFIAPI
EFI_PmicGlink_GetBatteryCount
(
  OUT UINT32* batteryCount
)
{
  if (batteryCount == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  return pmic_glink_get_battery_count(batteryCount);
}

EFI_STATUS
EFIAPI
EFI_PmicGlink_GetBatteryStatus
(
  IN  UINT32          battery_index,
  OUT EFI_PM_GLINK_BATT_STATUS_INFO *pBattStsInfo
)
{
  static BOOLEAN firstTime = TRUE;
  if (pBattStsInfo == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  if (firstTime)
  {
    pmic_glink_get_battery_info(battery_index);
    firstTime = FALSE;
  }
  return pmic_glink_get_battery_status(battery_index, pBattStsInfo);
}

EFI_STATUS
EFIAPI
EFI_PmicGlink_IsBatteryPresent
(
  IN  UINT32          battery_index,
  OUT BOOLEAN         *pBatteryPresent
)
{
  EFI_STATUS status = EFI_SUCCESS;
  UINT32 battery_count = 0;

  if (pBatteryPresent == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  *pBatteryPresent = FALSE;

  status = EFI_PmicGlink_GetBatteryCount(&battery_count);
  if (!EFI_ERROR(status))
  {
    if (battery_count > battery_index)
    {
      *pBatteryPresent = TRUE;
      DEBUG((EFI_D_INFO, "EFI_PmicGlink_IsBatteryPresent:: battery index: %d is present. \r\n", battery_index));
    }
    else
    {
      DEBUG((EFI_D_WARN, "EFI_PmicGlink_IsBatteryPresent:: battery index: %d is missing becasue battery_count = %d \r\n", battery_index, battery_count));
    }
  }

  return status;
}

EFI_STATUS
EFIAPI
EFI_PmicGlink_LinkStatus
(
  OUT EFI_PM_GLINK_LINK_STATUS *PmicGLinkStatus
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(NULL == PmicGLinkStatus)
    return EFI_INVALID_PARAMETER;

  Status = pmic_glink_get_link_status(PmicGLinkStatus);

  return Status;
}


EFI_STATUS
EFIAPI
EFI_PmicGlink_Connect
(
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = pmic_glink_Init();

  return Status;
}


/**
  USB Power Control Protocol implementation
 */
EFI_PMIC_GLINK_PROTOCOL PmicGlinkProtocolImplementation =
{
  PMIC_GLINK_REVISION,
  EFI_PmicGlink_UCSIReadBuffer,
  EFI_PmicGlink_UCSIWriteBuffer,
  EFI_PmicGlink_USBCReadBuffer,
  EFI_PmicGlink_USBCWriteBuffer,
  EFI_PmicGlink_GetBatteryCount,
  EFI_PmicGlink_GetBatteryStatus,
  EFI_PmicGlink_IsBatteryPresent,
  EFI_PmicGlink_Connect,
  EFI_PmicGlink_LinkStatus,
  
};


