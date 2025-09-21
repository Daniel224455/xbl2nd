/**
@file  ChargerExtHwLibConfig.c
@brief Provide Access to External Charger APIs
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
02/01/18   mr      Added low level GPIO dep config API
08/10/17   mr      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
/**
  Internal Dependencies
*/
#include "ChargerExtHwLibConfig.h"

/**
  Library Dependencies
*/
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>

/**
  Protocol Dependencies
*/
#include <Protocol/EFITlmm.h>


/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
STATIC EFI_TLMM_PROTOCOL *TLMMProtocol = NULL;


/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS ChargerExtHwLibConfigInit_cls(CHARGER_EXT_HW_INFO *pHwInfo);


/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
CHARGER_EXT_HW_EXT_HW_DEP sChargerExtHw_ChgDep[CHARGER_EXT_HW_EXT_HW_DEP_MAX] =
{
    [CHARGER_EXT_HW_EXT_HW_DEP_MODULE_EN] = {CHARGER_EXT_HW_EXT_HW_DEP_MSM_RESOURCE, MSM_GPIO_87, CHARGER_EXT_HW_EXT_HW_DEP_ENABLE_LOW},
    [CHARGER_EXT_HW_EXT_HW_DEP_QUP_IO_EN] = {CHARGER_EXT_HW_EXT_HW_DEP_MSM_RESOURCE, MSM_GPIO_44, CHARGER_EXT_HW_EXT_HW_DEP_STATE_NONE}
};

static CHARGER_EXT_HW_INFO sChargerExtHwInfo[] =
{
    {CHARGER_EXT_HW_ROHM, {I2C_INSTANCE_013, ROHM_CHARGER_I2C_SLAVE_ID}, sChargerExtHw_ChgDep, NULL}
};

static CHARGER_EXT_HW_INFO *pActiveChgHwInfo = NULL;


/*===========================================================================*/
/*                 FUNCTION DEFINITIONS                                      */
/*===========================================================================*/

/**
  Initialize External Charger Hardware based input Platform Type

  @param[In] PlatformType       Platfrom Information to initialize the library.

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLibConfig_Init(EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType)
{
    EFI_STATUS Status = EFI_SUCCESS;

    switch (PlatformType)
    {
      case EFI_PLATFORMINFO_TYPE_CLS:
        CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a Initializing CLS configuration \r\n",  __FUNCTION__));
        Status = ChargerExtHwLibConfigInit_cls(sChargerExtHwInfo);
        break;

      default:
        CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a Other than CLS Platform, Not Supported. \r\n",  __FUNCTION__));
        Status = EFI_UNSUPPORTED;
        break;
    }

    return Status;
}

/**
  Initialize External Charger Hw Library functions for CLS Platform

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLibConfigInit_cls(CHARGER_EXT_HW_INFO *pHwInfo)
{
    EFI_STATUS Status = EFI_SUCCESS;
    static BOOLEAN isHwInfoInitDone = FALSE;
    UINT8 HwInfoNum = 0, HwInfoSize = 0;

    if (TRUE == isHwInfoInitDone)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a Charger HW already initialized \r\n", __FUNCTION__));
        return Status;
    }

    if (pHwInfo == NULL)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        return EFI_INVALID_PARAMETER;
    }

    HwInfoSize = sizeof(sChargerExtHwInfo)/sizeof(sChargerExtHwInfo[0]);

    for(HwInfoNum = 0; HwInfoNum < HwInfoSize; HwInfoNum++)
    {
        switch(pHwInfo[HwInfoNum].ChgrExtHwType)
        {
          case CHARGER_EXT_HW_ROHM:
            Status = ChargerExtHwLibRohm_Init(&pHwInfo[HwInfoNum]);
            if(EFI_SUCCESS == Status)
            {
                pActiveChgHwInfo = &pHwInfo[HwInfoNum];
                isHwInfoInitDone = TRUE;
            }
            break;

          default:
            CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a Charger HW not present \r\n",  __FUNCTION__));
            Status = EFI_UNSUPPORTED;
            break;
        }
    }

    Status = (TRUE == isHwInfoInitDone) ? EFI_SUCCESS : Status;

    return Status;
}

/**
  Return External Charger Hw Active Hardware Information

  @param[In] PlatformType       Hardware Information to initialize the library.
  @param[Out] pHwInfo           Pointer where HwInfo is saved

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_NOT_READY:                The physical device is not ready for this request
*/

EFI_STATUS
ChargerExtHwLibConfig_GetHWInfo(CHARGER_EXT_HW_INFO **pHwInfo)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (pActiveChgHwInfo == NULL)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a Hardware Info is not available \r\n", __FUNCTION__));
        Status = EFI_NOT_READY;
    }
    else
    {
        (*pHwInfo) = pActiveChgHwInfo;
    }

    return Status;
}


/*===========================================================================*/
/*                  INTERNAL FUNCTIONS DEFINITIONS                           */
/*===========================================================================*/
/**
 * GPIO Configuration Interface
 */
EFI_STATUS ChargerExtHw_Gpio_Config(CHARGER_EXT_HW_EXT_HW_DEP_TYPE ResourceType, UINT32 ResourceID, CHARGER_EXT_HW_EXT_HW_DEP_STATE GpioState)
{
    EFI_STATUS Status = EFI_SUCCESS;

    //CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a ResourceType: %d, ResourceID: %d, GpioState: %d \r\n", __FUNCTION__, ResourceType, ResourceID, GpioState));

    if( (ResourceType >= CHARGER_EXT_HW_EXT_HW_DEP_INVALID) ||
        (GpioState == CHARGER_EXT_HW_EXT_HW_DEP_STATE_NONE ||
         GpioState >= CHARGER_EXT_HW_EXT_HW_DEP_STATE_INVALID) )
        { return EFI_INVALID_PARAMETER; }

    if(NULL == TLMMProtocol)
    {
        Status = gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void**)&TLMMProtocol);
        if(EFI_ERROR(Status) || NULL == TLMMProtocol)
        {
            CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a LocateProtocol Failed, Status [%r] \r\n", __FUNCTION__, Status));
            return Status;
        }
    }


    switch(ResourceType)
    {
      case CHARGER_EXT_HW_EXT_HW_DEP_MSM_RESOURCE:
        {
            Status  = TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), TLMM_GPIO_ENABLE);
            Status |= TLMMProtocol->GpioOut   ((UINT32)EFI_GPIO_CFG(ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),
                          ((CHARGER_EXT_HW_EXT_HW_DEP_ENABLE_HIGH == GpioState) ? GPIO_HIGH_VALUE : GPIO_LOW_VALUE));
            if(EFI_ERROR(Status))
            {
                CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a TLMMProtocol APIs Failed, ResourceID: %d, Status [%r] \r\n", __FUNCTION__, ResourceID, Status));
            }
        }
        break;

      case CHARGER_EXT_HW_EXT_HW_DEP_PMIC_RESOURCE:
      default:
        Status = EFI_UNSUPPORTED;
    }

    return Status;
}
