/**
@file  UsbPwrCtrlLibPmSchg.h
@brief Provide UsbPwrCtrlLibPmSchg Related definition
*/
/*=============================================================================
Copyright (c) 2017-2019 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/25/19   dch     Support PD Status API
10/21/18   ivy     Initial Release
=============================================================================*/
#ifndef __USBPWRCTRLLIBPMUCSI_UCSI_H__
#define __USBPWRCTRLLIBPMUCSI_UCSI_H__

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
/**
  Library Dependencies
*/
#include <Library/UsbPwrCtrlLib.h>
#include "../../SDMPkg/1000/Library/UsbPwrCtrlLib/UsbPwrCtrlLibConfig.h"

/**
  Protocol Dependencies
*/
#include <Protocol/EFIPmicUsb.h>
#include <Protocol/EFIPmicSchg.h>

/* ==================================== */
/*   USBC packet type def and struct    */
/* ==================================== */

typedef enum _USBPD_UCSI_COMMAND_CODE_TYPE
{
    USBPD_UCSI_COMMAND_CODE_RESERVED,
    USBPD_UCSI_COMMAND_CODE_PPM_RESET,
    USBPD_UCSI_COMMAND_CODE_CANCEL,
    USBPD_UCSI_COMMAND_CODE_CONNECTOR_RESET,
    USBPD_UCSI_COMMAND_CODE_ACK_CC_CI,
    USBPD_UCSI_COMMAND_CODE_SET_NOTIFICATION_ENABLE,
    USBPD_UCSI_COMMAND_CODE_GET_CAPABILITY,
    USBPD_UCSI_COMMAND_CODE_GET_CONNECTOR_CAPABILITY,
    USBPD_UCSI_COMMAND_CODE_SET_UOM,
    USBPD_UCSI_COMMAND_CODE_SET_UOR,
    USBPD_UCSI_COMMAND_CODE_SET_PDM,
    USBPD_UCSI_COMMAND_CODE_SET_PDR,
    USBPD_UCSI_COMMAND_CODE_GET_ALTERNATE_MODE,
    USBPD_UCSI_COMMAND_CODE_CAM_SUPPORTED,
    USBPD_UCSI_COMMAND_CODE_GET_CURRENT_CAM,
    USBPD_UCSI_COMMAND_CODE_SET_NEW_CAM,
    USBPD_UCSI_COMMAND_CODE_GET_PDOS,
    USBPD_UCSI_COMMAND_CODE_GET_CABLE_PROPERTY,
    USBPD_UCSI_COMMAND_CODE_GET_CONNECTOR_STATUS,
    USBPD_UCSI_COMMAND_CODE_GET_ERROR_STATUS,
}
USBPD_UCSI_COMMAND_CODE_TYPE,
*PUSBPD_UCSI_COMMAND_CODE_TYPE;

typedef union _USBPD_UCSI_VERSION_DATA
{
    uint16 AsUINT16;
    struct
    {
        uint8 MajorVersion                  : 8;    /*bit [ 7: 0]*/

        uint8 MinorVersion                  : 4;    /*bit [11: 8]*/
        uint8 SubMinorVersion               : 4;    /*bit [15:12]*/
    };
}
USBPD_UCSI_VERSION_DATA,
*PUSBPD_UCSI_VERSION_DATA;

typedef union _USBPD_UCSI_CCI_DATA
{
    uint32 AsUINT32;
    struct
    {
        uint16                              : 1;    /*bit [ 0: 0]*/
        uint16 ConnectorChangeIndicator     : 7;    /*bit [ 7: 1]*/
        uint16 DataLength                   : 8;    /*bit [15: 8]*/

        uint16                              : 9;    /*bit [24:16]*/
        uint16 NotSupportedIndicator        : 1;    /*bit [25:25]*/
        uint16 CancelCompletedIndicator     : 1;    /*bit [26:26]*/
        uint16 ResetCompletedIndicator      : 1;    /*bit [27:27]*/
        uint16 BusyIndicator                : 1;    /*bit [28:28]*/
        uint16 AckCommandIndicator          : 1;    /*bit [29:29]*/
        uint16 ErrorIndicator               : 1;    /*bit [30:30]*/
        uint16 CommandCompletedIndicator    : 1;    /*bit [31:31]*/        
    };
}
USBPD_UCSI_CCI_DATA, *PUSBPD_UCSI_CCI_DATA;

typedef struct _USBPD_UCSI_GET_CONNECTOR_STATUS_CONTROL_DATA
{
    uint64 Command                      : 8;    /*bit [ 7: 0]*/
    uint64 DataLength                   : 8;    /*bit [15: 8]*/
    uint64 ConnectorNumber              : 7;    /*bit [22:16]*/
    uint64 Reserved                    	: 41;   /*bit [63:23]*/
}
USBPD_UCSI_GET_CONNECTOR_STATUS_CONTROL_DATA, *PUSBPD_UCSI_GET_CONNECTOR_STATUS_CONTROL_DATA;


typedef struct _USBPD_UCSI_GET_CONNECTOR_STATUS_MESSAGE_DATA
{
    uint32 ConnectorStatusChange             : 16;   /*bit [15: 0]*/
    uint32 PowerOpMode                       : 3;    /*bit [18:16]*/
    uint32 ConnectStatus                     : 1;    /*bit [19:19]*/
    uint32 PowerDirection                    : 1;    /*bit [20:20]*/
    uint32 ConnectPartnerFlagUSB             : 1;    /*bit [21:21]*/
    uint32 ConnectPartnerFlagAlternateMode   : 1;    /*bit [22:22]*/
    uint32                                   : 6;    /*bit [28:23]*/
    uint32 ConnectPartnerType                : 3;    /*bit [31:29]*/

    uint32 RequestDataObject                 : 32;   /*bit [63:32]*/

    uint32 BatteryChargingStatus             : 2;    /*bit [65:64]*/
    uint32 ProviderCapLimitedReason          : 4;    /*bit [69:66]*/
    uint32                                   : 26;   /*bit [95:70]*/
    uint32                                   : 32;	/*bit [127:96]*/
}
USBPD_UCSI_GET_CONNECTOR_STATUS_MESSAGE_DATA, *PUSBPD_UCSI_GET_CONNECTOR_STATUS_MESSAGE_DATA;


typedef struct _USBPD_UCSI_GET_CONNECTOR_STATUS_DATA
{
	USBPD_UCSI_VERSION_DATA 		version;			//VERSION(16 bits)
	uint16							reserve;			//RESERVE(16 bits)	  
	USBPD_UCSI_CCI_DATA 			cci;				//CCI(32 bits)	  
	USBPD_UCSI_GET_CONNECTOR_STATUS_CONTROL_DATA control;			//CONTROL(64 bits)
	USBPD_UCSI_GET_CONNECTOR_STATUS_MESSAGE_DATA message_in; 		//MESSAGE_IN(128 bits) In to OPM/OS    
	USBPD_UCSI_GET_CONNECTOR_STATUS_MESSAGE_DATA message_out;		//MESSAGE_OUT(128 bits) Out to OPM/OS
}USBPD_UCSI_GET_CONNECTOR_STATUS_DATA, *PUSBPD_UCSI_GET_CONNECTOR_STATUS_DATA;

typedef struct _USBPD_UCSI_CONNECTOR_STATUS_INFO
{
  BOOLEAN                               InfoAvailable;
  USBPD_UCSI_GET_CONNECTOR_STATUS_DATA  UcsiConSts;
}USBPD_UCSI_CONNECTOR_STATUS_INFO, *PUSBPD_UCSI_CONNECTOR_STATUS_INFO;

/* ======== Type definition for UCSI Message-In ========= */

typedef enum _USBPD_UCSI_CSC_POWER_OPMODE_TYPE
{
    USBPD_UCSI_CSC_POWER_OPMODE_NOCONSUMER,
    USBPD_UCSI_CSC_POWER_OPMODE_TYPEC_DFT,
    USBPD_UCSI_CSC_POWER_OPMODE_BC,
    USBPD_UCSI_CSC_POWER_OPMODE_PD,
    USBPD_UCSI_CSC_POWER_OPMODE_TYPEC_1P5A,
    USBPD_UCSI_CSC_POWER_OPMODE_TYPEC_3A,
}USBPD_UCSI_CSC_POWER_OPMODE_TYPE;
typedef enum _USBPD_UCSI_CSC_POWER_DIRECTION_TYPE
{
    USBPD_UCSI_CSC_POWER_DIRECTION_AS_CONSUMER,
    USBPD_UCSI_CSC_POWER_DIRECTION_AS_PROVIDER,
}USBPD_UCSI_CSC_POWER_DIRECTION_TYPE;
typedef enum _USBPD_UCSI_CSC_CONNECTOR_PARTNER_TYPE
{
    USBPD_UCSI_CSC_CONNECTOR_PARTNER_DFP = 1,
    USBPD_UCSI_CSC_CONNECTOR_PARTNER_UFP,
    USBPD_UCSI_CSC_CONNECTOR_PARTNER_POWERCABLE_NOUFP,
    USBPD_UCSI_CSC_CONNECTOR_PARTNER_POWERCABLE_UFP,
    USBPD_UCSI_CSC_CONNECTOR_PARTNER_DEBUG_ACCESSORY,
    USBPD_UCSI_CSC_CONNECTOR_PARTNER_AUDIO_ACCESSORY,
}USBPD_UCSI_CSC_CONNECTOR_PARTNER_TYPE;
typedef enum _USBPD_UCSI_CSC_BATTERY_CHARGING_STATUS_TYPE
{
    USBPD_UCSI_CSC_BATTERY_CHARGING_STATUS_NOT_CHARGING,
    USBPD_UCSI_CSC_BATTERY_CHARGING_STATUS_NORMINAL_CHARGING,
    USBPD_UCSI_CSC_BATTERY_CHARGING_STATUS_SLOW_CHARGING,
    USBPD_UCSI_CSC_BATTERY_CHARGING_STATUS_TRICKLE_CHARGING,
}USBPD_UCSI_CSC_BATTERY_CHARGING_STATUS_TYPE;


/* ==================================== */
/*   USBC packet type def and struct    */
/* ==================================== */

typedef enum _USBPD_DPM_USBC_READ_SEL_TYPE
{
    USBPD_DPM_USBC_READ_SEL_PENDING_PAN,
    //USBPD_DPM_USBC_READ_SEL_HSUSB_PORT_DETECTION    
    USBPD_DPM_USBC_READ_SEL_PIN_ASSIGNMENT_PORT0 = 0x80,
    USBPD_DPM_USBC_READ_SEL_PIN_ASSIGNMENT_PORT1 = 0x81,
    USBPD_DPM_USBC_READ_SEL_PIN_ASSIGNMENT_PORT2 = 0x82,
    USBPD_DPM_USBC_READ_SEL_PIN_ASSIGNMENT_PORT3 = 0x83,
}USBPD_DPM_USBC_READ_SEL_TYPE, *PUSBPD_DPM_USBC_READ_SEL_TYPE;

typedef struct _USBPD_DPM_USBC_PAN_ACK_DATA
{
    unsigned char port_index;    
}USBPD_DPM_USBC_PAN_ACK_DATA, *PUSBPD_DPM_USBC_PAN_ACK_DATA;

typedef enum _USBPD_DPM_USBC_WRITE_CMD_TYPE
{
    /* Reserved 0x0 to 0xF  */
    USBPD_DPM_USBC_WRITE_CMD_PAN_EN = 0x10,
    USBPD_DPM_USBC_WRITE_CMD_PAN_ACK,
    USBPD_DPM_USBC_WRITE_CMD_READ_SEL,
}USBPD_DPM_USBC_WRITE_CMD_TYPE, *PUSBPD_DPM_USBC_WRITE_CMD_TYPE;


typedef struct _USBPD_DPM_USBC_PORT_PIN_ASSIGNMENT_DATA
{
    unsigned char port_index;
    unsigned char orientation;
    unsigned char mux_ctrl;
    unsigned char dpam_hpd;
}USBPD_DPM_USBC_PORT_PIN_ASSIGNMENT_DATA, *PUSBPD_DPM_USBC_PORT_PIN_ASSIGNMENT_DATA;

typedef enum _USBPD_PIN_ASSIGNMENT_ORIENTATION_TYPE
{
    USBPD_PIN_ASSIGNMENT_ORIENTATION_NORMAL_CC1,
    USBPD_PIN_ASSIGNMENT_ORIENTATION_FLIP_CC2,
    USBPD_PIN_ASSIGNMENT_ORIENTATION_INVALID_OPEN,
}USBPD_PIN_ASSIGNMENT_ORIENTATION_TYPE, *PUSBPD_PIN_ASSIGNMENT_ORIENTATION_TYPE;

typedef struct _USBPD_DPM_USBC_PIN_ASSIGNMENT_DATA
{
    USBPD_DPM_USBC_PORT_PIN_ASSIGNMENT_DATA port[EFI_USB_PWR_CTRL_PORTHW_MAX_PORT_NO];
}USBPD_DPM_USBC_PIN_ASSIGNMENT_DATA, *PUSBPD_DPM_USBC_PIN_ASSIGNMENT_DATA;

typedef enum _USBPD_DPM_USBC_READ_DATA_TYPE
{
    USBPD_DPM_USBC_READ_DATA_PENDING_PAN,
    USBPD_DPM_USBC_READ_DATA_PIN_ASSIGNMENT,
    //USBPD_DPM_USBC_READ_DATA_HSUSB_PORT_DETECTION
}USBPD_DPM_USBC_READ_DATA_TYPE, *PUSBPD_DPM_USBC_READ_DATA_TYPE;

typedef union _USBPD_DPM_USBC_WRITE_BUFFER_CMD_PAYLOAD
{
    USBPD_DPM_USBC_READ_SEL_TYPE    read_sel;
    USBPD_DPM_USBC_PAN_ACK_DATA     pan_ack;
}USBPD_DPM_USBC_WRITE_BUFFER_CMD_PAYLOAD, *PUSBPD_DPM_USBC_WRITE_BUFFER_CMD_PAYLOAD;

typedef union _USBPD_DPM_USBC_READ_BUFFER_DATA_PAYLOAD
{
    USBPD_DPM_USBC_PORT_PIN_ASSIGNMENT_DATA     pending_pan;
    USBPD_DPM_USBC_PIN_ASSIGNMENT_DATA          pin_assignment;
}USBPD_DPM_USBC_READ_BUFFER_DATA_PAYLOAD, *PUSBPD_DPM_USBC_READ_BUFFER_DATA_PAYLOAD;


typedef struct _USBPD_DPM_USBC_WRITE_BUFFER
{
    UINT32                           cmd_type;
    USBPD_DPM_USBC_WRITE_BUFFER_CMD_PAYLOAD cmd_payload;
}USBPD_DPM_USBC_WRITE_BUFFER, *PUSBPD_DPM_USBC_WRITE_BUFFER;


typedef struct _USBPD_DPM_USBC_READ_BUFFER
{
    UINT32                           data_type;
    USBPD_DPM_USBC_READ_BUFFER_DATA_PAYLOAD data_payload;
}USBPD_DPM_USBC_READ_BUFFER, *PUSBPD_DPM_USBC_READ_BUFFER;    



/**
  Initialize the interface to PMIC UCSI Module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmUcsi_Init();
/**
  Callback function to be executed periodically to get UCSI Conector Status 

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/

VOID
EFIAPI 
UsbPwrCtrlLibPmUcsi_StatusChgCb
(
	IN EFI_EVENT Event,
	IN VOID		*Context
);


/**
  Get Vbus Detect result from PMIC SCHG Module

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   bVbusDetect     Return the result of Vbus Detect

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmUcsi_GetVbusDetect
(
  UINT8 PortIndex,
  BOOLEAN *bVbusDetect);

/**
  Get Vbus Source OK result from Library

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   bVbusSourceOK   Return the result of Vbus SourceOK

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmUcsi_GetVbusSourceOK(
  UINT8 PortIndex,
  BOOLEAN *bVbusSourceOK);

/**
  Get HSUSB Charger Port Type

  @param[IN]    HwInfo          Hardware Information for this request
  @param[OUT]   ChgPortType     Return Charger Port Tpye

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmUcsi_GetHSUsbPortType(
  UINT8 PortIndex,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE *ChgPortType);

/**
  Get TypeC Port Status

  @param[IN]    HwInfo          Hardware Information for Sink
  @param[OUT]   TypeCPortStatus Return TypeC port Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmUcsi_GetTypeCPortStatus(
  UINT8 PortIndex,
  EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS *TypeCPortStatus);

/**
  Get Port PD Status

  @param[IN]    PortIndex       Specify which port the request is for
  @param[OUT]   PDStatus        Return PD Status;

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
UsbPwrCtrlLibPmUcsi_GetPDStatus(
    UINT8 PortIndex,
    EFI_USB_PWR_CTRL_PD_STATUS  *PDStatus);

#endif
