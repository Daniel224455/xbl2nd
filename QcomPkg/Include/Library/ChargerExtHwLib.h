/**
@file  ChargerExtHwLib.h
@brief Provide External Charger Lib related definition
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
02/01/18   mr      Added Sink enable and External dep config APIs
10/26/17  pbitra   Removed Battery status and watchdog related APIs
10/16/17   pbitra  Added APIS to support charging functionality
08/10/17   mr      Initial Release
=============================================================================*/
#ifndef __CHARGEREXTHWLIB_H__
#define __CHARGEREXTHWLIB_H__

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
/* Enum for External Charger H/w Dependencies */
typedef enum CHARGER_EXT_HW_EXT_HW_DEP_HW_TYPE
{
    CHARGER_EXT_HW_EXT_HW_DEP_MODULE_EN,
    CHARGER_EXT_HW_EXT_HW_DEP_QUP_IO_EN,
    CHARGER_EXT_HW_EXT_HW_DEP_MAX
} CHARGER_EXT_HW_EXT_HW_DEP_HW_TYPE;

/* Enum for BC 1.2 Charger Port type */
typedef enum _CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE
{
    CHARGER_EXT_HW_HSUSB_CHGPORT_UNKNOWN,
    CHARGER_EXT_HW_HSUSB_CHGPORT_SDP,               /**< Standard Downstream Port */
    CHARGER_EXT_HW_HSUSB_CHGPORT_CDP,               /**< Charging Downstream Port */
    CHARGER_EXT_HW_HSUSB_CHGPORT_DCP,               /**< Dedicated Charging Port  */
    CHARGER_EXT_HW_HSUSB_CHGPORT_PULLUP,            /**< PullUp Charging Port     */
    CHARGER_EXT_HW_HSUSB_CHGPORT_OPEN,
    CHARGER_EXT_HW_HSUSB_CHGPORT_UNSTABLE,
    CHARGER_EXT_HW_HSUSB_CHGPORT_INVALID = 0xFF,
} CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE;

typedef enum
{
  CHARGER_EXT_WDOG_STS_BARK,                          /**< Indicates BARK WDOG Timer expired   >*/
  CHARGER_EXT_WDOG_STS_BITE_CHARGING_DISABLED,        /**< Indicates BITE WDOG Timer expired and Charging is Disabled  >*/
  CHARGER_EXT_WDOG_STS_BITE_CHARGING_NOT_DISABLED,    /**< Indicates BITE WDOG Timer expired but Charging is not Disabled  >*/
  CHARGER_EXT_WDOG_STS_INVALID,
}CHARGER_EXT_WDOG_STS_TYPE;

typedef enum _CHARGER_EXT_HW_EXT_HW_DEP_INIT_STATE
{
    CHARGER_EXT_HW_EXT_HW_DEP_STATE_NONE,       /* TODO: HiZ state is not hadled for now */
    CHARGER_EXT_HW_EXT_HW_DEP_ENABLE_HIGH,
    CHARGER_EXT_HW_EXT_HW_DEP_ENABLE_LOW,
    CHARGER_EXT_HW_EXT_HW_DEP_STATE_INVALID
} CHARGER_EXT_HW_EXT_HW_DEP_STATE;

typedef struct
{
  UINT32 StateOfCharge;
  INT32  ChargeCurrent;
  INT32  BatteryVoltage;
  INT32  BatteryTemperature;
}CHARGER_EXT_BATT_STATUS_INFO;

/*===========================================================================*/
/*                  FUNCTION PROTOTYPES                                      */
/*===========================================================================*/
/**
  Initialize External Charger Hw Library functions

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_Init(VOID);


/**
  Get Vbus Detect result from External Charger Module

  @param[OUT]   bVbusDetect     Return the result of Vbus Detect

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_GetVbusDetect(BOOLEAN *bVbusDetect);

/**
  Get HSUSB Charger Port Type from External Charger Module

  @param[OUT]   ChgPortType     Return Charger Port Tpye

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_GetHSUsbPortType(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType);

/**
  Set Sink Max Input Current for External Charger Module

  @param[IN]    HwInfo          Hardware Information for this request
  @param[IN]    MaxCurrentMA    Value to set Max Input Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_SetSnkMaxInputCurrent(UINT32 MaxCurrentMA);

/**
  Enable/Disable charging functionality for Rohm Charger Module

  @param[IN]    Enable    		Enable/Disable

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS
ChargerExtHwLib_EnableCharger(BOOLEAN Enable);

/**
  Get Charging status

  @param[OUT]    pChargingSts    TRUE/FALSE

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

EFI_STATUS
ChargerExtHwLib_GetChargingStatus(BOOLEAN *pChargingSts);


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
ChargerExtHwLib_SetChargingTermCurrent(UINT32 ChargingTermCurrent);

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
ChargerExtHwLib_SetFCCMax(UINT32 FCCMaxCurrent);


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
ChargerExtHwLib_SetFVMax(UINT32 FVMax);

/**
  Enable/Disable Sink Mode

  @param[IN]    bSinkEn

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS ChargerExtHwLib_SetSinkEn(BOOLEAN bSinkEn);

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
EFI_STATUS ChargerExtHwLib_ConfigExtDep(CHARGER_EXT_HW_EXT_HW_DEP_HW_TYPE ExtDep, CHARGER_EXT_HW_EXT_HW_DEP_STATE State);

/**
  Complete formalities befor exiting Lib

  @param[IN]    VOID

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLib_Exit(void);


#endif  /* __CHARGEREXTHWLIB_H__ */
