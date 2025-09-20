/**
@file  ChargerExtHwLib.c
@brief Provide Access to External Charger Hw API
*/
/*=============================================================================
Copyright (c) 2017 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
08/10/17   mr      Initial Release
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

/**
  Protocol Dependencies
*/

/**
  Internal Dependencies
*/
#include "ChargerExtHwLibConfig.h"


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
static CHARGER_EXT_HW_INFO *pChargerExtHwInfo = NULL;


/*===========================================================================*/
/*                  FUNCTION DEFINITIONS                                      */
/*===========================================================================*/

/**
  Initialize External Charger Hw Library functions

  @param  none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_Init(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType;

    // Get Hardware Info based on PlatformType
    Status = GetPlatformType(&PlatformType);
    if(EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Get platform type Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    Status = ChargerExtHwLibConfig_Init(PlatformType);
    Status |= ChargerExtHwLibConfig_GetHWInfo(&pChargerExtHwInfo);
    if(EFI_ERROR(Status) || pChargerExtHwInfo == NULL)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Initialize Hardware Configuration Error[%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a SUCCESS \r\n",  __FUNCTION__));
    return Status;
}


/**
  Get Vbus Detect result from Rohm Charger Module

  @param[OUT]   bVbusDetect     Return the result of Vbus Detect

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_GetVbusDetect(BOOLEAN *bVbusDetect)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (bVbusDetect == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }
    if (!pChargerExtHwInfo || !pChargerExtHwInfo->Func ||
        !pChargerExtHwInfo->Func->GetVbusDetect)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_NOT_READY;
    }

    Status = pChargerExtHwInfo->Func->GetVbusDetect(bVbusDetect);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }

    return Status;
}

/**
  Get HSUSB Charger Port Type from Rohm Charger Module

  @param[OUT]   ChgPortType     Return Charger Port Tpye

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_GetHSUsbPortType(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (ChgPortType == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }
    if (!pChargerExtHwInfo || !pChargerExtHwInfo->Func ||
        !pChargerExtHwInfo->Func->GetHsUsbPortType)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_NOT_READY;
    }

    Status = pChargerExtHwInfo->Func->GetHsUsbPortType(ChgPortType);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }

    return Status;
}

/**
  Set Sink Max Input Current for Rohm Charger Module

  @param[IN]    MaxCurrentMA    Value to set Max Input Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_SetSnkMaxInputCurrent(UINT32 MaxCurrentMA)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (!pChargerExtHwInfo || !pChargerExtHwInfo->Func ||
        !pChargerExtHwInfo->Func->SetSnkMaxCurrent)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_NOT_READY;
    }

    Status = pChargerExtHwInfo->Func->SetSnkMaxCurrent(MaxCurrentMA);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }

    return Status;
}
