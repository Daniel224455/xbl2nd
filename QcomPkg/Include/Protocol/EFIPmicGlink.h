/**
@file  EFIPmicGlink.h
@brief Platform Usb Power Control Protocol for UEFI.
*/
/*=============================================================================
Copyright (c) 2017 - 2018 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
10/21/18   ivy     Updated pmicglink to polling based and support fetching usb info from charger FW
03/29/18   cs      added UCSI interface support
11/06/17   CS      Initial Release
=============================================================================*/

#ifndef __EFIPMICGLINK_H__
#define __EFIPMICGLINK_H__


/**
  Protocol version.
*/
#define PMIC_GLINK_REVISION 0x0000000000010002
/** @} */ /* end_addtogroup efi_pmicfg_constants */

/*  Protocol GUID definition */
/** @ingroup efi_pmic_glink_protocol */
#define EFI_PMIC_GLINK_PROTOCOL_GUID \
  { 0x7429eb07, 0xf2e3, 0x4a6b, { 0xaa, 0x96, 0xc9, 0x9, 0xcd, 0x95, 0x6e, 0xf7 } }



/*===========================================================================
EXTERNAL VARIABLES
===========================================================================*/
/**
External reference to the pmic Glink Protocol GUID.
*/

extern EFI_GUID gpmicGlinkProtocolGuid;

//define power state values
//bit 0 = 1 power online, bit 1 = 1 discharging, bit 2 = 1 charging, bit 3 = 1 battery critical
#define PMIC_GLINK_POWER_ON_LINE 0x00000001
#define PMIC_GLINK_DISCHARGING 0x00000002
#define PMIC_GLINK_CHARGING 0x00000004
#define PMIC_GLINK_BATTERY_CRITICAL1 0x00000008

#define PMIC_GLINK_UCSI_DATA_SIZE  48
/*===========================================================================
TYPE DEFINITIONS
===========================================================================*/
/**
Protocol declaration.
*/
typedef struct _EFI_PMIC_GLINK_PROTOCOL  EFI_PMIC_GLINK_PROTOCOL;


typedef enum _EFI_PMIC_GLINK_CHARGING_SOURCE
{
  PMIC_GLINK_CHARGING_SOURCE_TYPE_AC = 1, /**<  AC charger */
  PMIC_GLINK_CHARGING_SOURCE_TYPE_USB = 2, /**<  usb charger */
  PMIC_GLINK_CHARGING_SOURCE_TYPE_WIRELESS = 3, /**<  wireless charger  */
  PMIC_GLINK_CHARGING_SOURCE_TYPE_MAX = 4,
}EFI_PMIC_GLINK_CHARGING_SOURCE;

typedef struct _EFI_PM_GLINK_BATT_STATUS_INFO
{
  UINT32 StateOfCharge;
  INT32  ChargeCurrent;
  INT32  BatteryVoltage;
  INT32  BatteryTemperature;
  UINT32 PowerState;
  EFI_PMIC_GLINK_CHARGING_SOURCE ChargingSource;
}EFI_PM_GLINK_BATT_STATUS_INFO;


typedef struct _EFI_PM_GLINK_LINK_STATUS
{
  BOOLEAN IsPMICChannelOpen;
  /* Add more when needed */
}EFI_PM_GLINK_LINK_STATUS;


/* EFI_PMIC_GLINK_UCSI_READ_BUFFER */
/** @ingroup efi_pmicGlinkReadBuffer
@par Summary
read the 48 byte data array
@param[in] size
@param[out] dataArray           :  UINT8**

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_UCSI_READ_BUFFER)(
OUT UINT8** ppReadBuffer,
IN UINT8 size
);


/* EFI_PMIC_GLINK_UCSI_WRITE_BUFFER */
/** @ingroup efi_pmicGlinkReadBuffer
@par Summary
read the 48 byte data array
@param[in] size
@param[IN] dataArray           :  UINT8*

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_UCSI_WRITE_BUFFER)(
IN UINT8* pWriteBuffer,
IN UINT8 size
);

/* EFI_PMIC_GLINK_USBC_READ_BUFFER */
/** @ingroup efi_pmicGlinkReadBuffer
@par Summary
read the 16 byte data array
@param[in] size
@param[IN] dataArray           :  UINT8*

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_USBC_READ_BUFFER)(
IN UINT8** ppReadBuffer,
IN UINT8 size
);

/* EFI_PMIC_GLINK_USBC_WRITE_BUFFER */
/** @ingroup efi_pmicGlinkWriteBuffer
@par Summary
read the 8 byte data array
@param[in] size
@param[IN] dataArray           :  UINT8*

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_USBC_WRITE_BUFFER)(
IN  UINT8* pWriteBuffer,
IN  UINT8 size
);

/* EFI_PMIC_GLINK_GET_BATTERY_COUNT */
/** @ingroup efi_pmicGlinkGetBatteryCount
@par Summary
Get battery count

@param[out] BattCount           :  Battery count

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_GET_BATTERY_COUNT)(
OUT UINT32 *BatteryCount
);

/* EFI_PMIC_GLINK_GET_BATTERY_STATUS */
/** @ingroup efi_pmicGlinkGetBatteryStatus
@par Summary
Get Battery Status Information

@param[in]  battery_index     : 0 for first battery, 1 for second battery
@param[out] pBattStsInfo      :  Battery status info

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_GET_BATTERY_STATUS)(
IN  UINT32          battery_index,
OUT EFI_PM_GLINK_BATT_STATUS_INFO *pBattStsInfo
);


/* EFI_PMIC_GLINK_IS_BATTERY_PRESENT */
/** @ingroup efi_pmicGlinkIsBattery_Present
@par Summary
get if battery is present or not

@param[in]  battery_index        : 0 for first battery, 1 for second battery
@param[out] pBatteryPresent      : is battery present or not

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_IS_BATTERY_PRESENT)(
IN  UINT32          battery_index,
OUT BOOLEAN         *pBatteryPresent
);

/* EFI_PMIC_GLINK_CONNECT */
/** @ingroup efi_pmicGlinkConnect
@par Summary
open glink channel 

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_CONNECT)(
);



/* EFI_PMIC_GLINK_LINK_STATUS */
/** @ingroup efi_pmicGlinkConnect
@par Summary
open glink channel 

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_INVALID_PARAMETER -- Parameter is invalid. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
EFI_NOT_READY         -- Physical device is busy or not ready to
process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_GLINK_LINK_STATUS)(
  OUT EFI_PM_GLINK_LINK_STATUS *PmicGLinkStatus
);

/*===========================================================================
PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_PlatformPmicGlink_protocol
@par Summary
USB Power control Protocol interface.

@par Parameters
@inputprotoparams{PmicGlink_proto_params.tex}
*/
struct _EFI_PMIC_GLINK_PROTOCOL
{
    UINT64                                    Revision;
    EFI_PMIC_GLINK_UCSI_READ_BUFFER           UCSIReadBuffer;         /*Read ucsi data buffer from adsp*/
    EFI_PMIC_GLINK_UCSI_WRITE_BUFFER          UCSIWriteBuffer;        /*write ucsi data buffer to adsp*/
    EFI_PMIC_GLINK_USBC_READ_BUFFER           USBCReadBuffer;         /*Read usbc data buffer from adsp*/
    EFI_PMIC_GLINK_USBC_WRITE_BUFFER          USBCWriteBuffer;        /*Write usbc data buffer to adsp*/			
    EFI_PMIC_GLINK_GET_BATTERY_COUNT          GetBatteryCount;        /* Get number of batteries*/
    EFI_PMIC_GLINK_GET_BATTERY_STATUS         GetBatteryStatus;       /*Get Battery voltage, current, percentage, temperature, etc.*/
    EFI_PMIC_GLINK_IS_BATTERY_PRESENT         IsBatteryPresent;       /*indicate if battery is present or not*/
    EFI_PMIC_GLINK_CONNECT                    Connect;                /* GLink Open */
    EFI_PMIC_GLINK_LINK_STATUS                LinkStatus;             /*Link Status */
};

#endif /* __EFIPMICGLINK_H__ */
