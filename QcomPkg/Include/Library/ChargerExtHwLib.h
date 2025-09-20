/**
@file  ChargerExtHwLib.h
@brief Provide External Charger Lib related definition
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
#ifndef __CHARGEREXTHWLIB_H__
#define __CHARGEREXTHWLIB_H__

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

/*
Enum for BC 1.2 Charger Port type
*/
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


#endif  /* __CHARGEREXTHWLIB_H__ */
