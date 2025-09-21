/*! \file
*  
*  \brief  pm_app_smartchgr.h
*  \details Implementation file for pmic sbl charging algorithm
*    
*  \n &copy; Copyright (c)2018  by QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================
                                Edit History
when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/08/18   sv      Creation
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES 
===========================================================================*/
#include "i2c_api.h"
#include "pm_err_flags.h"
#include "pm_utils.h"
#include "boothw_target.h"
#include "RohmBD99954.h"
/*===========================================================================*/
/*                        GLOBALS and TYPE DEFINITIONS                       */
/*===========================================================================*/

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/

//--- Charger related --/

#define SHIFT_FROM_MASK(x) ((x & 0x01) ? 0 : \
                            (x & 0x02) ? 1 : \
                            (x & 0x04) ? 2 : \
                            (x & 0x08) ? 3 : \
                            (x & 0x10) ? 4 : \
                            (x & 0x20) ? 5 : \
                            (x & 0x40) ? 6 : \
                            (x & 0x80) ? 7 : 0)

#define ROHM_CHARGER_I2C_SLAVE_ID       0x09    /* SlaveID for ROHM BD99954 */
#define I2C_INSTANCE_ROHM_CHARGER       I2C_INSTANCE_013

/* For MSM GPIO for External Charger Module Enable/Power-On */
#define MSM_GPIO_87                     87      /* MSM GPIO87 */
/* GPIO44 for QUP12_I2C IOs */
#define MSM_GPIO_44                     44      /* MSM GPIO44 */

/*
Enum for BC 1.2 Charger Port type
*/
typedef enum
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

/*
Data Sturcuture for HW status
*/
typedef enum _EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE
{
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_UNKNOWN,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP,               /**< Standard Downstream Port */
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_OCP,               /**< Other Charging Port      */
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_CDP,               /**< Charging Downstream Port */
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_DCP,               /**< Dedicated Charging Port  */
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_FLOAT_CHARGER,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_QC_2P0,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_QC_3P0,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_INVALID = 0xFF,
}EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE;

//EXT CHG HW information
typedef struct
{
    UINT32  Index;
    UINT32  Address;
} EXTCHG_PORTHW_HW_INFO;




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

//-- FG related --//----------------------------------------------

#define I2C_SLAVE_ID_BATTERYPACK   0x0B    /* SlaveID for bq30z55*/
#define I2C_INSTANCE_BATTERYPACK   I2C_INSTANCE_013

/**
  SBS Commands
*/
#define SBS_CMD_BatteryMode		0x03
#define SBS_CMD_StateOfCharge	0x0E
#define SBS_CMD_Current			0x0A
#define SBS_CMD_Voltage			0x09
#define SBS_CMD_ChargingCurrent	0x14
#define SBS_CMD_ChargingVoltage	0x15
#define SBS_CMD_BatteryStatus	0x16
#define SBS_CMD_OperationStatus	0x54
#define SBS_CMD_ChargingStatus	0x55


#define SBS_DATA_SIZE 2
#define SBS_CMD_SIZE 1

#define FG_READ_RETRY_COUNT 3

typedef VOID *SmartChargerFG_HandleType;

// I2C Slave Context for FG Device
typedef struct
{
  SmartChargerFG_HandleType hI2CFGHandle;           /**< I2C handle for slave addresses */
  i2c_instance              I2CFGInstance;          /**< I2C core Number */
  i2c_slave_config          I2CFGCfg;               /**< I2C configuration */
  UINT32                    TimeoutMs;              /**< Timeout in milisecond - in case required */
} SmartChargerFG_SlaveType;


typedef struct
{
  UINT32 StateOfCharge;
  INT32  ChargeCurrent;
  INT32  BatteryVoltage;
  INT32  BatteryTemperature;
}SmartChargerFG_BattStatusInfo;

//FG HW information
typedef struct _FG_PORTHW_HW_INFO
{
    UINT32  Index;
    UINT32  Address;
} FG_PORTHW_HW_INFO;


/*===========================================================================
                     FUNCTION DECLARATIONS 
===========================================================================*/

pm_err_flag_type pm_ext_chgr_weakBatt_init(void);
