/**
@file  ChargerExtHwLibConfig.h
@brief Provide External Charger H/w Related definition
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
#ifndef __CHARGEREXTHWLIB_CONFIG_H__
#define __CHARGEREXTHWLIB_CONFIG_H__

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>

/**
  Library Dependencies
*/
#include "i2c_api.h"
#include <Library/ChargerExtHwLib.h>
/**
  Protocol Dependencies
*/
#include <Protocol/EFIPlatformInfo.h>


/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/
#define SHIFT_FROM_MASK(x) ((x & 0x01) ? 0 : \
                            (x & 0x02) ? 1 : \
                            (x & 0x04) ? 2 : \
                            (x & 0x08) ? 3 : \
                            (x & 0x10) ? 4 : \
                            (x & 0x20) ? 5 : \
                            (x & 0x40) ? 6 : \
                            (x & 0x80) ? 7 : 0)

#ifndef CHARGEREXTHW_LIB_DEBUG
    #define CHARGEREXTHW_LIB_DEBUG(Expression) \
    do{ \
    DEBUG ((EFI_D_WARN, "ChargerExtHw:")); \
    _DEBUG (Expression); \
    } while(FALSE)
#endif

#define ROHM_CHARGER_I2C_SLAVE_ID       0x09    /* SlaveID for ROHM BD99954 */

/* For MSM GPIO for External Charger Module Enable/Power-On */
#define MSM_GPIO_87                     87      /* MSM GPIO87 */
/* GPIO44 for QUP12_I2C IOs */
#define MSM_GPIO_44                     44      /* MSM GPIO44 */

/* External Charger Hw Type */
typedef enum _CHARGER_EXT_HW_TYPE
{
    CHARGER_EXT_HW_NONE,
    CHARGER_EXT_HW_SMB,
    CHARGER_EXT_HW_ROHM,
    CHARGER_EXT_HW_INVALID
} CHARGER_EXT_HW_TYPE;

typedef enum _USB_PWR_CTRL_ANX74XX_EXT_DEP_HW_TYPE
{
    CHARGER_EXT_HW_EXT_HW_DEP_MODULE_EN,
    CHARGER_EXT_HW_EXT_HW_DEP_QUP_IO_EN,
    CHARGER_EXT_HW_EXT_HW_DEP_MAX
} CHARGER_EXT_HW_EXT_HW_DEP_HW_TYPE;

typedef enum _CHARGER_EXT_HW_EXT_HW_DEP_TYPE
{
    CHARGER_EXT_HW_EXT_HW_DEP_NONE,
    CHARGER_EXT_HW_EXT_HW_DEP_MSM_RESOURCE,
    CHARGER_EXT_HW_EXT_HW_DEP_PMIC_RESOURCE,
    CHARGER_EXT_HW_EXT_HW_DEP_INVALID
} CHARGER_EXT_HW_EXT_HW_DEP_TYPE;

typedef enum _CHARGER_EXT_HW_EXT_HW_DEP_INIT_STATE
{
    CHARGER_EXT_HW_EXT_HW_DEP_STATE_NONE,       /* TODO: HiZ state is not hadled for now */
    CHARGER_EXT_HW_EXT_HW_DEP_ENABLE_HIGH,
    CHARGER_EXT_HW_EXT_HW_DEP_ENABLE_LOW,
    CHARGER_EXT_HW_EXT_HW_DEP_STATE_INVALID
} CHARGER_EXT_HW_EXT_HW_DEP_STATE;

/* External Charger Interface Info */
typedef struct _CHARGER_EXT_HW_INTERFACE_INFO
{
    UINT32  Index;
    UINT32  Address;
} CHARGER_EXT_HW_INTERFACE_INFO;

/* External HW Dependency */
typedef struct _CHARGER_EXT_HW_EXT_HW_DEP
{
    CHARGER_EXT_HW_EXT_HW_DEP_TYPE ExtDepResource;
    UINT32  ResourceID;
    CHARGER_EXT_HW_EXT_HW_DEP_STATE InitState;
} CHARGER_EXT_HW_EXT_HW_DEP;

/* Charger Functionality Interface */
typedef EFI_STATUS (*CHARGER_EXT_GET_VBUS_DETECT)(BOOLEAN *bVbusDetect);
typedef EFI_STATUS (*CHARGER_EXT_GET_HS_USBPORT_TYPE)(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType);
typedef EFI_STATUS (*CHARGER_EXT_SET_SNK_MAX_INPUT_CURRENT)(UINT32 MaxCurrentMA);

typedef struct _CHARGER_EXT_HW_FUNC
{
    CHARGER_EXT_GET_VBUS_DETECT GetVbusDetect;
    CHARGER_EXT_GET_HS_USBPORT_TYPE GetHsUsbPortType;
    CHARGER_EXT_SET_SNK_MAX_INPUT_CURRENT SetSnkMaxCurrent;
} CHARGER_EXT_HW_FUNC;

typedef struct _CHARGER_EXT_HW_INFO
{
    CHARGER_EXT_HW_TYPE ChgrExtHwType;
    CHARGER_EXT_HW_INTERFACE_INFO ChgrExtHwIntf;
    CHARGER_EXT_HW_EXT_HW_DEP *ChgHwExtDep;
    CHARGER_EXT_HW_FUNC *Func;
} CHARGER_EXT_HW_INFO;


// I2C Slave Context for Charger Device
typedef VOID *ChargerExtHw_HandleType;

typedef struct
{
    ChargerExtHw_HandleType   hI2CHandle;       /**< I2C handle for slave addresses */
    i2c_instance              I2CInstance;      /**< I2C Core Number */
    i2c_slave_config          I2CSlaveCfg;      /**< I2C Slave Device configuration */
    UINT32                    I2CSlaveRegMap;   /**< I2C Slave Device Register Map size - 1Byte/2Byte */
    UINT32                    TimeoutMs;        /**< Timeout in milisecond - in case required */
} ChargerExtHw_Device;


/*===========================================================================*/
/*                  FUNCTION PROTOTYPES                                      */
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
ChargerExtHwLibConfig_Init(EFI_PLATFORMINFO_PLATFORM_TYPE PlatformType);

/**
  Return Active Charger Hardware Information

  @param[In] PlatformType       Hardware Information to initialize the library.
  @param[Out] pHwInfo           Pointer where HwInfo is saved

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
  EFI_NOT_READY:                The physical device is not ready for this request
*/
EFI_STATUS
ChargerExtHwLibConfig_GetHWInfo(CHARGER_EXT_HW_INFO **pHwInfo);


/**
  Initialize end External Charger Hardware and it's functionality

  @param[In] pHwInfo            Charger Hw Info

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
/* Rohm Charger Init Function */
EFI_STATUS ChargerExtHwLibRohm_Init(CHARGER_EXT_HW_INFO *pHwInfo);


/**
  APIs to read/write data on Rohm Charger Device

  @param I2CDev         - I2C Slave Device
  @param offset         - Register Offset Address
  @param data_in/out    - Data to be read/written
  @param size_in/out    - Size of data to be read/written

  @return Returns 'Count' of successful read/written byte(s) or 0
 */
i2c_status ChargerExtHw_i2c_read (ChargerExtHw_Device *I2CDev, UINT8 offset, UINT8 *data_out, UINT16 size_out);
i2c_status ChargerExtHw_i2c_write(ChargerExtHw_Device *I2CDev, UINT8 offset, UINT8 *data_in,  UINT16 size_in);

#endif  /* __CHARGEREXTHWLIB_CONFIG_H__ */
