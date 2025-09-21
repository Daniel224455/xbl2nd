/**
@file  ChargerExtHwLib.c
@brief Provide Access to External Charger Hw API
*/
/*=============================================================================
Copyright (c) 2017-2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/01/18   mr      Added Charger Sink enabled and External dep config APIs
10/26/17   pbitra  Removed battery status related APIs
10/16/17   pbitra  Added APIS to support charging functionality
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

/**
  Enable/Disable charging functionality for Rohm Charger Module

  @param[IN]    Enable          Enable/Disable

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLib_EnableCharger(BOOLEAN Enable)
{
    EFI_STATUS Status = EFI_SUCCESS;
	BOOLEAN pChargingSts = FALSE;

    if (pChargerExtHwInfo == NULL)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_NOT_READY;
    }
	Status = pChargerExtHwInfo->Func->GetChargingStatus(&pChargingSts);
	if ((Status == EFI_SUCCESS) && (pChargingSts != Enable))
	{
    Status = pChargerExtHwInfo->Func->EnableCharger(Enable);
	}
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }
    return Status;
}

/**
  Get charging status

  @param[OUT]    pChargingSts    TRUE/FALSE

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/  
EFI_STATUS
ChargerExtHwLib_GetChargingStatus(BOOLEAN *pChargingSts)
    {

	EFI_STATUS Status = EFI_SUCCESS;

	if (pChargerExtHwInfo == NULL)
    {
		CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
		return EFI_NOT_READY;
    }

	Status = pChargerExtHwInfo->Func->GetChargingStatus(pChargingSts);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }	
	return Status;

    }

/**
  Set Charging terminating current

  @param[IN]    ChargingTermCurrent    

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS
ChargerExtHwLib_SetChargingTermCurrent(UINT32 ChargingTermCurrent)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (pChargerExtHwInfo == NULL)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_NOT_READY;
    }

    Status = pChargerExtHwInfo->Func->SetChargingTermCurrent(ChargingTermCurrent);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }
    return Status;

}

/**
  Set Max Fast Charge Current

  @param[IN]    FCCMaxCurrent    

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/


EFI_STATUS
ChargerExtHwLib_SetFCCMax(UINT32 FCCMaxCurrent)
    {
    EFI_STATUS Status = EFI_SUCCESS;

    if (pChargerExtHwInfo == NULL)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_NOT_READY;
    }

    Status = pChargerExtHwInfo->Func->SetFCCMax(FCCMaxCurrent);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }
    return Status;

}


/**
  Set Float Voltage Max

  @param[IN]    FVMax    

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLib_SetFVMax(UINT32 FVMax)
{
	EFI_STATUS Status = EFI_SUCCESS;

	if (pChargerExtHwInfo == NULL)
	{
		CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
		return EFI_NOT_READY;
	}

	Status = pChargerExtHwInfo->Func->SetFVMax(FVMax);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }
	return Status;

}

/**
  Enable/Disable VBUS Sink

  @param[IN]    bSinkEn

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLib_SetSinkEn(BOOLEAN bSinkEn)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (pChargerExtHwInfo == NULL)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_NOT_READY;
    }

    Status = pChargerExtHwInfo->Func->SetSinkEn(bSinkEn);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Failed, Error[%r] \r\n", __FUNCTION__, Status));
    }

    return Status;
}

/**
  Config External H/w Dependency Resource

  @param[IN]    ExtDepHw
  @param[IN]    State

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLib_ConfigExtDep(CHARGER_EXT_HW_EXT_HW_DEP_HW_TYPE ExtDep, CHARGER_EXT_HW_EXT_HW_DEP_STATE State)
{
    EFI_STATUS Status = EFI_SUCCESS;
    CHARGER_EXT_HW_EXT_HW_DEP *ChgHwExtDep = NULL;

    if (pChargerExtHwInfo == NULL)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_NOT_READY;
    }

    ChgHwExtDep = pChargerExtHwInfo->ChgHwExtDep;

    Status = ChargerExtHw_Gpio_Config(ChgHwExtDep[ExtDep].ExtDepResource, ChgHwExtDep[ExtDep].ResourceID, State);
    if (EFI_ERROR(Status))
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a _Gpio_Config Failed, ResourceID: %d, Error[%r] \r\n", __FUNCTION__, ChgHwExtDep[ExtDep].ResourceID, Status));
    }

    return Status;
}

/**
  Exit Charger HW Lib


  @return
  EFI_SUCCESS:                  Function returned successfully.
*/

EFI_STATUS
ChargerExtHwLib_Exit(void)
	{
		return EFI_SUCCESS;
	}



