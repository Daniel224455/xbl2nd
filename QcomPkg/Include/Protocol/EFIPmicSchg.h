/** 
@file  EFIPmicSchg.h
@brief PMIC SCHG for UEFI.
*/
/*=============================================================================
Copyright (c) 2017 - 2020 Qualcomm Technologies, Inc.
All rights reserved.
Qualcomm Technologies Inc Confidential and Proprietary.
    
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
04/24/20   ravi    Added support for requesting maximum power
12/13/18   ivy     support pmic ship mode
08/01/18   ivy     Added API to scale ADC Jeita. Added API to set batt them pull to register
07/20/18   sb      Changed USB port type to match hardware
06/29/18   dc 	   Expand ICL Status API for Max ICL
06/20/18   cs      Added support for batt missing status
05/17/18   ivy     Added support for DC IRQ status
03/26/18   ra      Support for HW JEITA status
03/01/18   ra      Adding support for skin and die temp thresholds
02/20/18   ra      Adding new protocols
01/24/18   ra      Adding support to get ICL status
11/27/17   ra      Adding battery missing config in charger
06/13/17   ll      Enable/disable HW AFP based on debug board detection
02/15/17   va      Add re run APSD 
02/06/17   cs      add config to enable/disable INOV
01/27/17   sm      Added support for pbs triggered AFP
01/23/17   sm      CFG parameter Structure changes
12/19/16   mr      Added API to get SCHG Charger PmicIndex
10/19/16   sm      removed EnableHvdcp cfg param from EFI_PM_SCHG_CFGDATA_TYPE struct
10/17/16   sm      Added SetChargeCmdBit() API 
                   Added elements in EFI_PM_SCHG_WDOG_STS_TYPE enum
10/13/16   cs      add protocol API for setting dcin current limit.
09/13/16   sm      removed unused config parameters from EFI_PM_SCHG_CFGDATA_TYPE
09/06/16   cs      wipower changes to remove 3.5 sec, not suspend DCIn
08/16/16   sm      Added API to enable/disable HW JEITA
07/18/16   sm      Added API to configure USB ICL options
07/15/16   cs      Adding Wipower support
06/24/16   sm      Added API to return if charging is enabled or not. 
06/23/16   sm      Added EnableHvdcp in EFI_PM_SCHG_CFGDATA_TYPE struct
06/23/16   va      Adding support for Charger Fg Peripheral dumps
06/09/16   sm      Added SupportHostMode in EFI_PM_SCHG_CFGDATA_TYPE
05/20/16   va      Update for struct mis match 
05/10/16   sm      Added back SchgExit for placeholder
04/21/16   sm      Added APIs to enable/disable and pet watchdog
03/28/16   sm      Removed GetHVDCP API and Enabled GetOtgSts and UsbinValid APIs
03/28/16   va      Adding Jeita Compensation params
03/21/16   va      Adding Exit API for clean up
02/19/16   sm      Initial draft

=============================================================================*/

#ifndef __EFIPMICSCHG_H__
#define __EFIPMICSCHG_H__

/*===========================================================================

                        MACRO DECLARATIONS

===========================================================================*/
/** @addtogroup efi_pmicSchg_constants 
@{ */
/**
Protocol version.
*/

#define PMIC_SCHG_REVISION 0x0000000000010016
/** @} */ /* end_addtogroup efi_pmicSchg_constants */

/*  Protocol GUID definition */
/** @ingroup efi_pmicSchg_protocol */
#define EFI_PMIC_SCHG_PROTOCOL_GUID \
    { 0xae6ae96e, 0x483f, 0x42ae, { 0x9c, 0xc1, 0x9f, 0xac, 0x1b, 0x58, 0x47, 0x28 } } 


/** @cond */
/*===========================================================================

                        EXTERNAL VARIABLES

===========================================================================*/
/**
External reference to the PMIC SCHG Protocol GUID.
*/
extern EFI_GUID gQcomPmicSchgProtocolGuid;

/*===========================================================================

                        FUNCTION DECLARATIONS

===========================================================================*/

EFI_STATUS EFI_PmicSchgEnableHWJeita(UINT32 PmicDeviceIndex, BOOLEAN Enable);

/*===========================================================================

                        TYPE DEFINITIONS

===========================================================================*/

/**
Protocol declaration.
*/
typedef struct _EFI_QCOM_PMIC_SCHG_PROTOCOL   EFI_QCOM_PMIC_SCHG_PROTOCOL;
/** @endcond */

/** @addtogroup efi_pmicSchg_data_types 
@{ */


/**  Attached charger type. **/
typedef enum 
{
   PM_SCHG_POWER_PATH__NONE,    /**< No charger. */
   PM_SCHG_POWER_PATH__BATTERY,    /**< Battery.    */
   PM_SCHG_POWER_PATH__USBIN,    /**< USB path.   */
   PM_SCHG_POWER_PATH__DCIN,    /**< DCIN path.  */
   PM_SCHG_POWER_PATH__INVALID,    /**< DCIN path.  */
} PM_SCHG_POWER_PATH_TYPE;

/**  Charging port types. **/
typedef enum EFI_PM_SCHG_CHGR_PORT_TYPE
{
  EFI_PM_SCHG_CHG_PORT_SDP_CHARGER,         /**< Standard Downstream Port         */
  EFI_PM_SCHG_CHG_PORT_OCP_CHARGER,         /**< Other Charging Port              */
  EFI_PM_SCHG_CHG_PORT_CDP_CHARGER,         /**< Charging Downstream Port         */
  EFI_PM_SCHG_CHG_PORT_DCP_CHARGER,         /**< Dedicated Charging Port          */
  EFI_PM_SCHG_CHG_PORT_FLOAT_CHARGER,       /**< Floating Charging Port           */
  EFI_PM_SCHG_CHG_PORT_QC_2P0,              /**< Quick Charger 2.0 Charging Port  */
  EFI_PM_SCHG_CHG_PORT_QC_3P0,              /**< Quick Charger 3.0 Charging Port  */
  EFI_PM_SCHG_CHG_PORT_INVALID = 0xFF,             /**< INVALID PORT                     */
}EFI_PM_SCHG_CHGR_PORT_TYPE;

typedef enum EFI_PM_SCHG_OTG_UVLO_SENSOR_SRC_TYPE
{
 EFI_PM_SCHG_OTG_UVLO_SENSOR__ANA,     /**<Analog Comparator       */
 EFI_PM_SCHG_OTG_UVLO_SENSOR__FG,      /**<Fuel Gauge ADC          */
 EFI_PM_SCHG_OTG_UVLO_SENSOR__BOTH,    /**<Or of both Analog and FG*/
 EFI_PM_SCHG_OTG_UVLO_SENSOR__INVALID  /**<Invalid type sensor     */
}EFI_PM_SCHG_OTG_UVLO_SENSOR_SRC_TYPE;

typedef enum EFI_PM_SCHG_OTG_CTRL_TYPE
{
  EFI_PM_SCHG_OTG_CTR__CMD_REG_RID_DISABLED,  /**< Command register with RID Disabled*/  
  EFI_PM_SCHG_OTG_CTR__PIN_CTRL_RID_DISABLED, /**< Pin control with RID Disabled     */
  EFI_PM_SCHG_OTG_CTR__CMD_REG_RID_ENABLED,   /**< Command register with RID Enabled */
  EFI_PM_SCHG_OTG_CTR__AUTO_OTG_RID_ENABLED,  /**< Auto OTG (RID Enabled)            */
  EFI_PM_SCHG_OTG_CTR__INVALID
}EFI_PM_SCHG_OTG_CTRL_TYPE;

typedef enum
{
  EFI_PM_SCHG_DCDC_OTG_HICCUP_CNTR_RST_TIMER_SEL,    /*0 = 450/840/1700/3500ms (dpenpends on BOOST_LARGE_CAP_SS_TIMEOUT 1 = 40ms*/
  EFI_PM_SCHG_DCDC_OTG_CFG_OTG_EN_SRC_CFG,           /*0 = OTG Enable Source Command       1 = OTG Enable Source Type-C/RID*/
  EFI_PM_SCHG_DCDC_OTG_CFG_ENABLE_OTG_IN_DEBUG_MODE, /*0 = Do not enable OTG in debug mode 1 = Enable OTG in debug mode*/
  EFI_PM_SCHG_DCDC_FAST_ROLE_SWAP_START_OPTION = 6,  /*0 = Assuming VDDCAP, FPFET and switcher reference remains on, directly switch to boost mode
                                                       1 = Turn off buck and immediately turn on boost*/
  EFI_PM_SCHG_DCDC_OTG_CFG_INVALID,                  /*INVALID*/
}EFI_PM_SCHG_DCDC_OTG_CFG_TYPE;

/**  Supported Input priorities. **/
typedef enum 
{
   EFI_PM_SCHG_INPUT_PRIORITY_DCIN,    /**< DCIN has priority.  */
   EFI_PM_SCHG_INPUT_PRIORITY_USBIN,   /**< USB has priority.   */
   EFI_PM_SCHG_INPUT_PRIORITY_INVALID, /**< INVALID input priority  */
} EFI_PM_SCHG_INPUT_PRIORITY_TYPE;

typedef enum
{
   EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_DEB_LV_ADAPTER,     /**< USBIN Input Collapse Option for LV adapter>*/
   EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_DEB_HV_ADAPTER,     /**< USBIN Input Collapse Option for HV adapter>*/
   EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_AICL_EN,            /**< TRUE:enabled, FALSE: disabled>*/
   EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_DEB_5V_ADAPTER_SEL, /**< USBIN Input Collapse Glitch Filter for 5V Select, Unregulated or 5V - 9V Adapter >*/
   EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_DEB_9V_ADAPTER_SEL, /**< USBIN Input Collapse Glitch Filter for 9V Adapter Select. >*/
   EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_DEB_9V_ADAPTER_EN,  /**< USBIN Input Collapse Glitch Filter for 9V Adapter Enable  >*/
   EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_OV_OPTION,          /**< FALSE = OV on USBIN will block DCIN current path; TRUE = OV on USBIN will not block DCIN current path>*/
   EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_INVALID             /**< INVALID >*/
}EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_TYPE;

typedef struct {

  BOOLEAN  PrintChargerAppDbgMsgToFile;
  /**< Flag to Enable/Disable Charger debug messages to File.  */

  BOOLEAN  PrintChargerAppDbgMsg;
  /**< Flag to Enable/Disable Charger debug messages.  */

  BOOLEAN  EnableChargerFgDump;
  /**< Flag to Enable/Disable Charger FG periperal debugs*/

  /* add other debug config here*/

}EFI_PM_SCHG_DEBUG_CFGDATA_TYPE;

typedef struct
{
	
	BOOLEAN JeitaHardLimitEnable;
	/**< Enable/Disable Jeita Hard Limit*/

	BOOLEAN JeitaSoftColdFvCompEnable;
	/**< Enable/Disable Float Voltage compensation during soft cold*/

	BOOLEAN JeitaSoftHotFvCompEnable;
	/**< Enable/Disable Float Voltage compensation during soft hot*/

	BOOLEAN JeitaSoftColdCcCompEnable;
	/**< Enable/Disable Charge Current compensation during soft cold*/

	BOOLEAN JeitaSoftHotCcCompEnable;
	/**< Enable/Disable Charge Current compensation during soft cold*/

	UINT32   JeitaCcCompHotCfg;
	/**< JEITA Charge Current Compensation when in battery temperature hot soft-limit in mA */

	UINT32   JeitaCcCompColdCfg;
	/**< JEITA Charge Current Compensation when in battery temperature cold soft-limit in mA */

	UINT32   JeitaFvCompHotCfg;
	/**< JEITA Float Voltage Compensation when in battery temperature hot soft-limit in mA */

	UINT32   JeitaFvCompColdCfg;
	/**< JEITA Float Voltage Compensation when in battery temperature cold soft-limit in mA */

	INT32    JeitaCriticalTempLowLimit;
	/**< Lower limit of Batery temperature value to perform AFP*/

	INT32    JeitaHardColdLimit; 
	/**< Lower limit of Batery temperature value to stop charging*/

	INT32    JeitaSoftColdLimit;
	/**< Lower limit of Batery temperature value to lower charging parameters*/

	INT32    JeitaSoftHotLimit;
	/**< Upper limit of Batery temperature value to lower charging parameters*/

	INT32    JeitaHardHotLimit;
	/**< Upper limit of Batery temperature value to stop charging*/

	INT32    JeitaCriticalTempHighLimit;
	/**< Upper limit of Batery temperature value to perform AFP*/
}EFI_PM_HW_JEITA_DATA;

typedef struct
{
	BOOLEAN  ProgramSkinAndChargerHotThreshold;
	/**<  Enables Pmic Skin and Charger Hot Threshols to be programmed via configs instead of profiles */
	INT32   DeviceSkinHotInC;
	/**<  Device Skin hot */
	INT32   DeviceSkinTooHotInC;
	/**<  Device Skin too hot */
	INT32   ChargerHotInC;
	/**<  Charger Hot Threshold */
	INT32   ChargerTooHotInC;
	/**<  Charger too Hot Threshold */

}EFI_PM_SKIN_DIE_TEMP_THRESHOLD;

typedef struct
{
  UINT32   ChargingTermCurrent;
  /**<  Charging termination current*/

  UINT32   ChgFccMax;
  /**< Value of FCC_MAX in mA */

  UINT32  ChgFvMax;
  /**< Maximum Float Voltage */

  INT32   HwAfpHotThreshold;
  /**<Enable/Disable Charge Current compensation during soft cold*/

  INT32   HwAfpColdThreshold;
  /**<Enable/Disable Charge Current compensation during soft cold*/

  BOOLEAN EnableSkinTempINov;
  /**<Enable/Disable using skin temp for charging thermal imitiageion*/

  BOOLEAN EnableDieTempINov;
  /**<Enable/Disable using die temp for thermal mitigation*/

  BOOLEAN EnableDieTempCompINov;
  /**<Enable/Disable using die temp comparator for thermal mitigation*/

  UINT32   BattMissingCfg;
  /**<  Battery Missing Config Param*/

  BOOLEAN EnDebugAccessMode;
  /**<  Debug Accessory Config Param*/

  BOOLEAN EnFMB;
  /**<  FMB Config Param*/

  UINT32 DAMSelIcl3AorFMB;
  /**<  DAM ICL Config Param*/

  EFI_PM_SCHG_DEBUG_CFGDATA_TYPE SchgDbgCfg;
  EFI_PM_HW_JEITA_DATA SchgJeitaData;
  EFI_PM_SKIN_DIE_TEMP_THRESHOLD SchgSkinDieTemp;

}EFI_PM_SCHG_CFGDATA_TYPE;

/**< Hvdcp data type*/
typedef struct
{
  BOOLEAN bEnHvdcp;
}EFI_SCHG_HVDCP_CFGDATA_TYPE;

/**< JEITA types supported */
typedef enum
{
  EFI_PM_SCHG_JEITA_TYPE_HW,
  /**< Hardware JEITA */

  EFI_PM_SCHG_JEITA_TYPE_SW,
  /**< Software JEITA. */

  EFI_PM_SCHG_JEITA_TYPE_INVALID,
  /**< JEITA types invalid */
} EFI_PM_SCHG_JEITA_TYPE;

typedef enum {
  EFI_PM_SCHG_AICL_RERUN_TIME_3S,
  EFI_PM_SCHG_AICL_RERUN_TIME_12S,
  EFI_PM_SCHG_AICL_RERUN_TIME_45S,
  EFI_PM_SCHG_AICL_RERUN_TIME_3M,
  EFI_PM_SCHG_AICL_RERUN_TIME_CFG_INVALID, /*Invalid*/
}EFI_PM_SCHG_AICL_RERUN_TIME_CFG;

/**  OTG status type **/
typedef enum
{
    EFI_PM_SCHG_DCDC_OTG_ENABLED,
    EFI_PM_SCHG_DCDC_OTG_DISABLED,
    EFI_PM_SCHG_DCDC_OTG_ERROR,
    EFI_PM_SCHG_DCDC_OTG_EXECUTING_ENABLE_SEQ,
    EFI_PM_SCHG_DCDC_OTG_EXECUTING_DISABLE_SEQ,
    EFI_PM_SCHG_DCDC_OTG_STATUS_INVALID
} EFI_PM_SCHG_DCDC_OTG_STATUS_TYPE;

typedef enum
{
  EFI_PM_SCHG_USB_ICL_MODE_SDP_2_0,        /**< Select USB 2.0 CFG, Set USB51 Mode to 500 mA, set USBIN_MODE to USB 5/1 or USB 9/1.5 Current Level >*/
  EFI_PM_SCHG_USB_ICL_MODE_SDP_3_0,        /**< Select USB 3.0 CFG, Set USB51 Mode to 500 mA, set USBIN_MODE to USB 5/1 or USB 9/1.5 Current Level >*/
  EFI_PM_SCHG_USB_ICL_MODE_WALL_CHARGERS, /**< Select USB 2.0 CFG(Default), Set USB51 Mode to 100 mA, set USBIN_MODE to HC Mode Current Level     >*/
  EFI_PM_SCHG_USB_ICL_MODE_INVALID
}EFI_PM_SCHG_USB_ICL_MODE_TYPE;

typedef enum
{
  EFI_PM_SCHG_WDOG_STS_BARK,                          /**< Indicates BARK WDOG Timer expired   >*/
  EFI_PM_SCHG_WDOG_STS_BITE_CHARGING_DISABLED,        /**< Indicates BITE WDOG Timer expired and Charging is Disabled  >*/
  EFI_PM_SCHG_WDOG_STS_BITE_CHARGING_NOT_DISABLED,    /**< Indicates BITE WDOG Timer expired but Charging is not Disabled  >*/
  EFI_PM_SCHG_WDOG_STS_INVALID,
}EFI_PM_SCHG_WDOG_STS_TYPE;

typedef struct
{
  BOOLEAN bChargingEnabled;
}EFI_PM_SCHG_CHARGING_STATUS;

typedef struct
{
  UINT32 PmicIndex;
  UINT32 SlaveIndex;
} EFI_PM_SCHG_PMIC_INFO;

typedef struct
{
	UINT32 max_icl_status;                         /* Max ICL value */
	UINT32 aicl_icl_status;                        /* ICL value post AICL */
	UINT32 thermal_icl_status;                     /* ICL value post HW therm adjustment */
	UINT32 final_icl_status;                       /* ICL value post AICL, HW and SW therm adjustment */ 
	UINT32 aicl_done;
}EFI_PM_SCHG_ICL_STATUS;

typedef enum
{
	EFI_PM_SCHG_TYPEC_CC_OUT_RESERVED,
	EFI_PM_SCHG_TYPEC_CC_OUT_CC1,
	EFI_PM_SCHG_TYPEC_CC_OUT_CC2,
	EFI_PM_SCHG_TYPEC_CC_OUT_OPEN,
	EFI_PM_SCHG_TYPEC_CC_OUT_INVALID
} EFI_PM_SCHG_TYPEC_CC_OUT_STATUS;

/**  TYPEC DFP Current Advertisement - from UFP perspective **/
typedef enum
{
	EFI_PM_SCHG_TYPEC_DFP_CURR_ADV_RESERVED,
	EFI_PM_SCHG_TYPEC_DFP_CURR_ADV_3A,             // 3.0A @ 5V
	EFI_PM_SCHG_TYPEC_DFP_CURR_ADV_1P5A,           // 1.5A @ 5V
	EFI_PM_SCHG_TYPEC_DFP_CURR_ADV_STD,            // 900mA @ 5V - STD TypeC USB Power
	EFI_PM_SCHG_TYPEC_DFP_CURR_ADV_INVALID
} EFI_PM_SCHG_TYPEC_DFP_CURR_ADV;

/**  TYPEC UFP Connection Type (Cable Connector) - from DFP perspective  **/
typedef enum
{
	EFI_PM_SCHG_TYPEC_CABLE_CONNECTOR_RESERVED,
	EFI_PM_SCHG_TYPEC_STATE_UNATTACHED,            // DFP_OPEN_OPEN
	EFI_PM_SCHG_TYPEC_UFP_ATTACHED,                // DFP_RD_OPEN
	EFI_PM_SCHG_TYPEC_PWR_CABLE_NO_UFP_ATTCHED,    // DFP_RA_OPEN
	EFI_PM_SCHG_TYPEC_PWR_CABLE_UFP,               // DFP_RD_RA_VCONN
	EFI_PM_SCHG_TYPEC_AUDIO_ADAP_MODE,             // DFP_RA_RA
	EFI_PM_SCHG_TYPEC_DEBUG_ACCESSORY_MODE,        // DFP_RD_RD
	// DFP-to-DFP and UFP-to-UFP are undetectable states
	EFI_PM_SCHG_TYPEC_CABLE_CONNECTOR_INVALID
} EFI_PM_SCHG_TYPEC_UFP_CONNECTOR_TYPE;

/**  TYPEC Port status on attach of USB cable **/
typedef struct
{
	EFI_PM_SCHG_TYPEC_CC_OUT_STATUS         cc_out_sts;
	EFI_PM_SCHG_TYPEC_DFP_CURR_ADV          dfp_curr_adv;
	EFI_PM_SCHG_TYPEC_UFP_CONNECTOR_TYPE    ufp_conn_type;
	BOOLEAN                                vbus_sts;
	BOOLEAN                                vbus_err_sts;
	BOOLEAN                                debounce_done_sts;
	BOOLEAN                                vconn_oc_sts;
	BOOLEAN                                ccout_out_hiz_sts;
} EFI_PM_SCHG_TYPEC_PORT_STATUS;

typedef enum
{
	EFI_PM_SCHG_TYPEC_CONNECT_MODE_NONE,
	EFI_PM_SCHG_TYPEC_CONNECT_MODE_DFP,
	EFI_PM_SCHG_TYPEC_CONNECT_MODE_UFP,
	EFI_PM_SCHG_TYPEC_CONNECT_MODE_INVALID
} EFI_PM_SCHG_TYPEC_CONNECT_MODE_STATUS;

/**  TYPEC VConn Enable Control **/
typedef enum
{
	EFI_PM_SCHG_TYPEC_VCONN_HW_CTRL,
	EFI_PM_SCHG_TYPEC_VCONN_SW_CTRL_DISABLE,
	EFI_PM_SCHG_TYPEC_VCONN_SW_CTRL_ENABLE,
	EFI_PM_SCHG_TYPEC_VCONN_EN_CTRL_INVALID
} EFI_PM_SCHG_TYPEC_VCONN_EN_CTRL;

typedef struct
{
	BOOLEAN jeitaHardHot;
	BOOLEAN jeitaSoftHot;
	BOOLEAN jeitaHardCold;
	BOOLEAN jeitaSoftCold;
}EFI_PM_SCHG_BATT_TEMP_STATUS;

typedef enum {
  EFI_PM_SCHG_DC_IRQ_DCIN_0,		/*TBD*/
  EFI_PM_SCHG_DC_IRQ_DCIN_VASHDN,	/*The DCIN voltage has crossed the auto-shutdown threshold*/
  EFI_PM_SCHG_DC_IRQ_DCIN_UV,		/*The DCIN voltage has crossed the under-voltage threshold*/
  EFI_PM_SCHG_DC_IRQ_DCIN_OV,		/*The DCIN voltage has crossed the over-voltage threshold*/
  EFI_PM_SCHG_DC_IRQ_DCIN_PLUGIN,	/*DCIN plugged in, goes low on un-plug*/
  EFI_PM_SCHG_DC_IRQ_REVI,			/*The DCIN voltage is below the revi threshold*/
  EFI_PM_SCHG_DC_IRQ_PON,			/*The DCIN_PON Pin is Enabled*/
  EFI_PM_SCHG_DC_IRQ_EN,			/*The DCIN_EN Pin is Enabled*/
  EFI_PM_SCHG_DC_IRQ_INVALID,		/*Invalid*/
}EFI_PM_SCHG_DC_IRQ_TYPE; 

typedef enum
{
	ICL_3A,
	ICL_FMB
}DAM_SEL_ICL;

typedef enum {
  EFI_PM_SCHG_THERM_CMP,
  EFI_PM_SCHG_BATT_ID_BMISS,
  EFI_PM_SCHG_FAKE_BAT_DET,
}EFI_PM_SCHG_BATT_MISSING_STATUS_TYPE;

typedef enum
{
  EFI_PM_SCHG_OS_BOOTLOADER,
  EFI_PM_SCHG_OS_HLOS,
  EFI_PM_SCHG_OS_INVALID,
}EFI_PM_SCHG_OS_TYPE;
/*===========================================================================

                        EXTERNAL VARIABLES

===========================================================================*/
/**
External reference to the PMIC SCHG CFG DATA.
*/
extern EFI_PM_SCHG_CFGDATA_TYPE gSchgCfgData;

/*===========================================================================

                        EXTERNAL FUNCTION DECLARATIONS

===========================================================================*/
/* EFI_PM_SCHG_INT */ 
/** @ingroup
  @par Summary
  Initializes SCHG module

  @param[in]  PmicDeviceIndex    0:Primary, 1:Secondary

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_INIT)(
    IN UINT32 PmicDeviceIndex,
    IN EFI_PM_SCHG_CFGDATA_TYPE SchgCfgData,
    IN BOOLEAN bDbgBoard
    );

/* EFI_PM_SCHG_ENABLE_CHARGER */
/** @ingroup efi_pmSchg_enable_charger
  @par Summary
  Enables the charger module to charge the battery.

  @param[in]  PmicDeviceIndex    Primary: 0
  @param[in]  ChargerEnable      If TRUE, enable the charger; if FALSE, disable
                                 the charger.

 @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error.
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_ENABLE_CHARGER)(
  IN UINT32  PmicDeviceIndex,
  IN BOOLEAN ChargerEnable
);

/* EFI_PMIC_SCHG_USBIN_VALID */
/** @ingroup efi_pmicSchg_usbin_valid
  @par Summary
  Indicates whether the VBUS on PMIC is high or low.

  @param[in]  PmicDeviceIndex    0:Primary, 1:Secondary
  @param[out] Valid              TRUE if VBUS high else FALSE.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_USBIN_VALID)(
  IN  UINT32  PmicDeviceIndex,
  OUT BOOLEAN *Valid
);

/* EFI_PMIC_SCHG_GET_POWER_PATH */
/** @ingroup
  @par Summary
  Returns information about the charger attached to the device.

  @param[in]  PmicDeviceIndex    0:Primary, 1:Secondary
  @param[out] ChargerPath        Charger path; see #PM_SCHG_POWER_PATH_TYPE
                                 for details.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_GET_POWER_PATH)(
  IN  UINT32                   PmicDeviceIndex,
  OUT PM_SCHG_POWER_PATH_TYPE  *ChargerPath
);

/* EFI_PMIC_SCHG_IS_BATTERY_PRESENT */
/** @ingroup efi_pmicSchg_is_battery_present
  @par Summary
  Indicates whether the battery is present.

  @param[in]  PmicDeviceIndex    0:Primary, 1:Secondary
  @param[out] BatteryPresent     Whether or not the battery is present.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_IS_BATTERY_PRESENT)(
  IN  UINT32  PmicDeviceIndex,
  OUT BOOLEAN *BatteryPresent
);


/* EFI_PMIC_SCHG_GET_CHARGER_PORT_TYPE */
/** @ingroup efi_pmic_schg_get_charger_port_type
  @par Summary
  reads charger port type.

  @param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
  @param[out] PortType           Charging port type.
                                 For more info refer enum EFI_PM_SCHG_CHGR_PORT_TYPE

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_GET_CHARGER_PORT_TYPE)(
  IN  UINT32                       PmicDeviceIndex,
  OUT EFI_PM_SCHG_CHGR_PORT_TYPE *PortType
);

/**@ingroup efi_pmic_schg_config_otg
  @par Summary
  This will configure OTG

  @param[in]  PmicDeviceIndex   0:Primary, 1:Secondary
  @param[in]  OtgCfgType        OTG configuration parameters. Refer struct
                                #EFI_PM_SCHG_DCDC_OTG_CFG_TYPE for more info

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_CONFIG_OTG)(
  IN  UINT32                    PmicDeviceIndex,
  IN  EFI_PM_SCHG_DCDC_OTG_CFG_TYPE  OtgCfgType,
  IN  BOOLEAN                   SetValue
);

/**@ingroup efi_pmic_schg_config_otg
  @par Summary
  This will set the OTG current limit. Valid values are 250 to 1000mAmp

  @param[in]  PmicDeviceIndex   0:Primary, 1:Secondary
  @param[in]  ImAmp             Current limit in milliAmp.
                                Valid values are 250 to 1000mAm

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_SET_OTG_I_LIMIT)(
  IN  UINT32                      PmicDeviceIndex,
  IN  UINT32                      ImAmp
);

/**@ingroup efi_pmic_schg_set_usb_max_current
  @par Summary
  This will set the USB max current limit. Valid values are 100 to 2500mAmp

  @param[in]  PmicDeviceIndex   0:Primary, 1:Secondary
  @param[in]  MaxCurrent        Current limit in milliAmp.
                                Valid values are 250 to 2500mA

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_SET_USB_MAX_CURRENT)(
  IN UINT32          PmicDeviceIndex,
  IN UINT32          MaxCurrent
);

/**@ingroup efi_pmic_schg_en_afp_mode
  @par Summary
  This will force device to go into AFP mode causing batfet to open and shutting down the device

  @param[in]  PmicDeviceIndex   0:Primary, 1:Secondary

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_ENABLE_AFP_MODE)(
  IN  UINT32                      PmicDeviceIndex
);

/**@ingroup efi_pmic_schg_set_charger_path_priority
  @par Summary
  This will set charger path priority

  @param[in]  PmicDeviceIndex   0:Primary, 1:Secondary
  @param[in]  ChgrPath          Refer enum#EFI_PM_SCHG_CHGR_PATH_TYPE for more info

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_SET_INPUT_PRIORITY)(
  IN  UINT32                           PmicDeviceIndex,
  IN  EFI_PM_SCHG_INPUT_PRIORITY_TYPE  ChgrPath
);

/**@ingroup efi_pmic_schg_set_fcc_max_current
  @par Summary
  This will set the Battery Fast Charging max current limit. Valid values are 300 to 3000mAmp

  @param[in]  PmicDeviceIndex   0:Primary, 1:Secondary
  @param[in]  MaxCurrentInMA    Current limit in milliAmp.
                                Valid values are 300 to 3000mA

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_SET_FCC_MAX_CURRENT)(
  IN UINT32          PmicDeviceIndex,
  IN UINT32          MaxCurrentInMA
);

/**@ingroup efi_pmic_schg_set_fv_max_voltage
  @par Summary
  This will set the Battery Floating Voltage . Valid values are 100 to 2500mAmp

  @param[in]  PmicDeviceIndex   0:Primary, 1:Secondary
  @param[in]  MaxVoltageInMv   MaxVoltage limit in mV.
                                Valid values are 3600mV to 4500 mv

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_SET_FV_MAX_VOLTAGE)(
  IN UINT32          PmicDeviceIndex,
  IN UINT32          MaxVoltageInMv
);

/**@ingroup efi_pmic_schg_enable_otg
  @par Summary
  This API enables/disables OTG via command register

  @param[in]  PmicDeviceIndex   0:Primary, 1:Secondary
  @param[in]  Enable            TRUE: Enables; FALSE: Disables itv

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_ENABLE_OTG)(
  IN  UINT32   PmicDeviceIndex,
  IN  BOOLEAN  Enable
);

/**@ingroup efi_pmic_schg_get_otg_ocp_sts
  @par Summary
  Returns OTG Over-current protection (OCP) status

  @param[in]   PmicDeviceIndex   0:Primary, 1:Secondary
  @param[out]  Ok                OTG OCP is in good standing, FLASE: OTG OCP needs to be re-enabled

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_GET_OTG_STATUS)(
  IN  UINT32                       PmicDeviceIndex,
  OUT EFI_PM_SCHG_DCDC_OTG_STATUS_TYPE  *pOtgStatus
);

/**@ingroup efi_pmic_schg_config_aicl
  @par Summary
  This API can be used to configure various AICL parameters

  @param[in]   PmicDeviceIndex   0:Primary, 1:Secondary
  @param[in]   AiclCfgType       AICL cfg type. Refer #EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_TYPE for more info
  @param[in]   Set               TRUE or FALSE

  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_CONFIG_AICL)(
  IN UINT32                                  PmicDeviceIndex,
  IN EFI_PM_SCHG_USB_CHGPTH_AICL_CFG_TYPE  AiclCfgType,
  IN BOOLEAN                                 Set
);

/* EFI_PMIC_SCHG_USB_SUSPEND */
/** @ingroup efi_pmic_schg_usb_suspend
  @par Summary
  Enables/ Disables USB suspend mode

  @param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
  @param[in]  Enable             TRUE to enable and FALSE to disable USB suspend mode

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_USB_SUSPEND)(
  IN  UINT32   PmicDeviceIndex,
  IN BOOLEAN   Enable
);

/* EFI_PMIC_SCHG_ENABLE_CHG_WDOG */
/** @ingroup EFI_PMIC_SCHG_ENABLE_CHG_WDOG
  @par Summary
  Enables/ Disables charger watchdog

  @param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
  @param[in]  Enable             TRUE to enable and FALSE to disable Charger watchdog

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_ENABLE_CHG_WDOG)(
  IN UINT32  PmicDeviceIndex,
  IN BOOLEAN Enable
);

/* EFI_PMIC_SCHG_PET_CHG_WDOG */
/** @ingroup efi_pmic_schg_pet_chg_wdog
  @par Summary
  Pets charger watchdog

  @param[in]  PmicDeviceIndex    Primary: 0, Secondary:1

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_PET_CHG_WDOG)(
  IN  UINT32   PmicDeviceIndex
);

/* EFI_PMIC_SCHG_EN_WDOG_IF_CHARGING */
/** @ingroup efi_pmic_schg_en_wdog_if_charging
  @par Summary
  Enable watchdog if device already charging

  @param[in]  PmicDeviceIndex    Primary: 0, Secondary:1

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_EN_WDOG_IF_CHARGING)(
  IN  UINT32   PmicDeviceIndex
);

/* EFI_PMIC_SCHG_PET_CHG_WDOG */
/** @ingroup efi_pmic_schg_chgr_source_reinserted
  @par Summary
  Return if USB port was changed or not

  @param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
  @param[out] Reinserted    if port changed then TRUE else FALSE

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_CHGR_SOURCE_REINSERTED)(
  IN UINT32   PmicDeviceIndex,
  OUT BOOLEAN *Reinserted
);


/* EFI_PMIC_SCHG_RERUN_AICL */
/** @ingroup efi_pmic_schg_rerun_aicl
  @par Summary
  Return AICL

  @param[in]  PmicDeviceIndex    Primary: 0, Secondary:1

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_RERUN_AICL)(
  IN  UINT32   PmicDeviceIndex,
  IN  EFI_PM_SCHG_AICL_RERUN_TIME_CFG RerunTime
);

/* EFI_PMIC_SCHG_PRINT_AICL_STS */
/** @ingroup EFI_PmicSchgPrintAiclSts
@par Summary
Get AICL status

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
@param[out] isAICLComplete     FALSE: 0, TRUE:1

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_PRINT_AICL_STS)(
IN  UINT32   PmicDeviceIndex,
OUT BOOLEAN  *IsAICLComplete
);

/* EFI_PMIC_SCHG_DUMP_PERIPHERAL */
/** @ingroup efi_pmic_schg_dump_pheripheral
  @par Summary
  Dump Charger Pheripheral

  @param[in]  PmicDeviceIndex    Primary: 0, Secondary:1

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_DUMP_PERIPHERAL)(
  VOID
);

/* EFI_PMIC_SCHG_ENABLE_HW_JEITA */
/** @ingroup EFI_PmicSchgEnableHWJeita
@par Summary
Enables HW JEITA

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
@param[in]  Enable             Disable: 0, Enable:1

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_ENABLE_JEITA)(
  IN UINT32                   PmicDeviceIndex, 
  IN EFI_PM_SCHG_JEITA_TYPE JeitaType
);

/* EFI_PMIC_SCHG_GET_CHARGING_STATUS */
/** @ingroup EFI_PmicSchgGetChargingStatus
@par Summary
Returns if charging is enabled/disabled

@param[in]   PmicDeviceIndex    Primary: 0, Secondary:1
@param[out]  pChargingEnabled    TRUE: Charging Enabled
                                 FALSE: Charging Disabled

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PMIC_SCHG_GET_CHARGING_STATUS)(
  UINT32 PmicDeviceIndex,
  OUT EFI_PM_SCHG_CHARGING_STATUS *pChargingStatus
);


/* EFI_PMIC_SCHG_DCIN_VALID */
/** @ingroup efi_pmicSchg_dcin_valid
@par Summary
Indicates whether the DC IN is plugged in or not

@param[in]  PmicDeviceIndex    0:Primary, 1:Secondary
@param[out] Valid              TRUE if VBUS high else FALSE.

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_DCIN_VALID)(
  IN  UINT32  PmicDeviceIndex,
  OUT BOOLEAN *Valid
);


/* EFI_PMIC_SCHG_DCIN_SUSPEND */
/** @ingroup efi_pmicSchg_dcin_suspend
@par Summary
to suspend dcin or not

@param[in]  PmicDeviceIndex    0:Primary, 1:Secondary
@param[in]  Suspend            TRUE:to suspend, FALSE:not to suspend.

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_DCIN_SUSPEND)(
  IN  UINT32  PmicDeviceIndex,
  IN  BOOLEAN Suspend
);

/* EFI_PMIC_SCHG_SET_DCIN_POWER */
/** @ingroup EFI_PmicSchgSetDCInPower
@par Summary
set dcin power limit to certain value

@param[in]  PmicDeviceIndex    0:Primary, 1:Secondary
@param[in]  UINT32             DCIN power in uV

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_DCIN_POWER)(
  IN  UINT32  PmicDeviceIndex,
  IN  UINT32  PowerInMicroWalts
);

/* EFI_PMIC_SCHG_SET_USB_ICL_MODE */
/** @ingroup EFI_PmicSchgSetUsbIclMode
@par Summary
Sets USBIN ICL Mode

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
@param[in]  IclModeType        See EFI_PM_SCHG_USB_ICL_MODE_TYPE for details
@param[in]  SetValue           Value to be set

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PMIC_SCHG_SET_USB_ICL_MODE)(
  IN UINT32                         PmicDeviceIndex,
  IN EFI_PM_SCHG_USB_ICL_MODE_TYPE  IclModeType
);

/* EFI_PMIC_SCHG_GET_CHG_WDOG_STATUS */
/** @ingroup EFI_PmicSchgGetChgWdogStatus
@par Summary
Returns charger WDOG status

@param[in]   PmicDeviceIndex    Primary: 0, Secondary:1
@param[OUT]  pWdogSts           See EFI_PM_SCHG_WDOG_STS_TYPE for more info

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PMIC_SCHG_GET_CHG_WDOG_STATUS)(
  IN UINT32                    PmicDeviceIndex,
  OUT EFI_PM_SCHG_WDOG_STS_TYPE *pWdogSts
);

/* EFI_PMIC_SCHG_EXIT */
/** @ingroup EFI_PmicSchgExit
@par Summary
Clean up Schg charger related
@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PMIC_SCHG_EXIT)(
  UINT32 PmicDeviceIndex
);

/* EFI_PMIC_SCHG_ENABLE_HW_JEITA */
/** @ingroup EFI_PmicSchgEnableHWJeita
@par Summary
Enables HW JEITA

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
@param[in]  Enable             Disable: 0, Enable:1

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_ENABLE_HW_JEITA)(
  IN UINT32       PmicDeviceIndex,
  IN BOOLEAN      Enable
);

/* EFI_PMIC_SCHG_TOGGLE_WIPOWER_SDLATCH */
/** @ingroup EFI_PmicSchgResetWipowerRstN
@par Summary
toggles SHDN_N_CLEAR_CMD to disable getting power from dcin when chargeOK is Low

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_TOGGLE_WIPOWER_SDLATCH)(
  IN UINT32       PmicDeviceIndex
);

/* EFI_PMIC_SCHG_SET_DCIN_CURRENT */
/** @ingroup EFI_PmicSchgSetDcInCurrent
@par Summary
Set DCIn input current limit for non-wipower DCIn Charging

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
@param[in]  DcInCurrentLimit   0 to 3000 mA

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_DCIN_CURRENT)(
  IN UINT32       PmicDeviceIndex,
  IN UINT32       DcInCurrentLimit
);

/* EFI_PMIC_SCHG_SET_CHARGER_CMD_BIT */
/** @ingroup EFI_PmicSchgSetChargeCmdBit
@par Summary
Sets value for charger command bit

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
@param[in]  Enable             FALSE: 0, TRUE:1

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_CHARGER_CMD_BIT)(
  IN UINT32       PmicDeviceIndex,
  IN BOOLEAN      Enable
);


/* EFI_PMIC_SCHG_GET_PMIC_INFO */
/** @ingroup EFI_PmicSchgGetPmicInfo
  @par Summary
  Provides SCHG charger PMIC Index.

  @param[out]  SchgPmicInfo     PMIC Info, on what SCHG charger resides

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- If input param is invalid. \n
  EFI_DEVICE_ERROR      -- Error occurred during the operation.
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_SCHG_GET_PMIC_INFO)(
    OUT EFI_PM_SCHG_PMIC_INFO *SchgPmicInfo
);

/* EFI_PMIC_SCHG_RERUN_APSD */
/** @ingroup EFI_PmicSchgReRunApsd
@par Summary
Sets value for charger Re run APSD bit

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_CONFIG_APSD)
(
  IN UINT32                      PmicDeviceIndex
);

/* EFI_PMIC_SCHG_CONFIG_HVDCP */
/** @ingroup EFI_PmicSchgConfgiHvdcp
@par Summary
Sets bit value for charger to config for HVDCP

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
@param[in]  pSchgHvdcpDataType See struct def

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_CONFIG_HVDCP)
(
  IN UINT32                      PmicDeviceIndex,
  IN EFI_SCHG_HVDCP_CFGDATA_TYPE *pSchgHvdcpDataType
);

/* EFI_PMIC_SCHG_GET_ICL_STATUS */
/** @ingroup EFI_PmicGetIclStatus
@par Summary
Gets the ICL values post AICL, thermal and other HW adjustments

@param[in]  PmicDeviceIndex    Primary: 0, Secondary:1
@param[out]  pIclStatus See struct def

@return
EFI_SUCCESS           -- Function completed successfully. \n
EFI_DEVICE_ERROR      -- Physical device reported an error. \n
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_ICL_STATUS)
(
IN UINT32                      PmicDeviceIndex,
OUT EFI_PM_SCHG_ICL_STATUS *pIclStatus
);

/* EFI_PMIC_SCHG_TYPEC_SET_VCONN */
/*@par Summary
This API sets VConn configs.

@param[in]   PmicDeviceIndex   0:Primary, 1:Secondary
@param[in]   VconnEn       VConn Enable

@return8
EFI_SUCCESS            -- Function completed successfully.
EFI_DEVICE_ERROR       -- Physical device reported an error.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_TYPEC_SET_VCONN)(
IN UINT32                      PmicDeviceIndex,
IN  BOOLEAN  VconnEn
);

/* EFI_PMIC_SCHG_TYPEC_GET_PORT_STATE */
/*@par Summary
This API returns Type-C port status.

@param[in]   PmicDeviceIndex   0:Primary, 1:Secondary
@param[out]  TypeCStatus        Type-C port status

@return
EFI_SUCCESS            -- Function completed successfully.
EFI_DEVICE_ERROR       -- Physical device reported an error.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_TYPEC_GET_PORT_STATE)(
IN UINT32                      PmicDeviceIndex,
OUT  EFI_PM_SCHG_TYPEC_PORT_STATUS  *TypeCStatus
);

/* EFI_PMIC_SCHG_TYPEC_GET_CONNECT_STATE*/
/*@par Summary
This API returns Type-C port Connection status - in terms of Port Role.

@param[in]   PmicDeviceIndex   0:Primary, 1:Secondary
@param[out]  TypeCConnStatus   Type-C port connection status

@return
EFI_SUCCESS            -- Function completed successfully.
EFI_DEVICE_ERROR       -- Physical device reported an error.
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_TYPEC_GET_CONNECT_STATE)(
IN UINT32                      PmicDeviceIndex,
OUT  EFI_PM_SCHG_TYPEC_CONNECT_MODE_STATUS  *TypeCConnStatus
);

/* EFI_PMIC_SCHG_GET_HW_JEITA_STATUS */
/*@par Summary
Returns HW JEITA Status

@param[in]  PmicDeviceIndex  : Pmic Device Index (0 for primary)
@param[out] pHwJeitaStatus   : HW JEITA Status

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_HW_JEITA_STATUS)(
IN  UINT32                       PmicDeviceIndex,
OUT EFI_PM_SCHG_BATT_TEMP_STATUS  *pHwJeitaStatus
);


/* EFI_PMIC_SCHG_GET_DC_IRQ_STATUS */
/*@par Summary
This API gets DC irq status

@param[in]  PmicDeviceIndex  : Pmic Device Index (0 for primary)
@param[in]	irq		 		 : irq 
@param[out]	status			 : Not triggered 0, triggered 1

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_DC_IRQ_STATUS)
(
IN	UINT32 							PmicDeviceIndex,
IN	EFI_PM_SCHG_DC_IRQ_TYPE 		irq,
OUT BOOLEAN *						irq_status
);


/* EFI_PMIC_SCHG_EN_DEBUG_ACCESSORY_MODE */
/*@par Summary
Enables Debug accessory and FMB mode

@param[in]  PmicDeviceIndex  : Pmic Device Index (0 for primary)
@param[in] EnDbgAccessSnk    : Enable Debug Accessory Mode
@param[in] EnFmb             : Enable FMB mode

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_EN_DEBUG_ACCESSORY_MODE)(
IN  UINT32                       PmicDeviceIndex,
IN  BOOLEAN                EnDbgAccessSnk,
IN  BOOLEAN                EnFmb
);

/* EFI_PMIC_SCHG_GET_DAM_SNK_STATUS */
/*@par Summary
Returns if factory debug cable is attached or not

@param[in]  PmicDeviceIndex  : Pmic Device Index (0 for primary)
@param[out] ConnectSts    : Tells if factory debug cable is connected or not

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_DAM_SNK_STATUS)(
IN  UINT32                       PmicDeviceIndex,
OUT  BOOLEAN                *ConnectSts
);

/* EFI_PMIC_SCHG_SET_DAM_ICL */
/*@par Summary
Sets the ICL in debug accessory mode (DAM)

@param[in]  PmicDeviceIndex  : Pmic Device Index (0 for primary)
@param[out] icl_sel    : Select the ICL in DAM mode

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_DAM_ICL)(
IN  UINT32                       PmicDeviceIndex,
IN  DAM_SEL_ICL                icl_sel
);

/* EFI_PMIC_SCHG_GET_BATT_MISSING_STATUS */
/*@par Summary
This API detects if battery is missing due to ID or therm pin floating or if it is a fake battery

@param[in]  PmicDeviceIndex  : Pmic Device Index (0 for primary)
@param[in]	status_type		 		 : id missing, therm missing or fake battery
@param[out]	status_val	 		 : return status indication if it is missing or fake battery
@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_BATT_MISSING_STATUS)
(
IN	UINT32 							PmicDeviceIndex,
IN	EFI_PM_SCHG_BATT_MISSING_STATUS_TYPE 		status_type,
OUT BOOLEAN *						status_val
);

/* EFI_PMIC_SCHG_IS_PORT_ACTIVE */
/*@par Summary
This API detects if battery is missing due to ID or therm pin floating or if it is a fake battery

@param[out]	active_pmic_index	 		 : the pmic_index for the active charging port.

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_ACTIVE_PORT_INDEX)
(
OUT UINT8 *						  active_pmic_index
);

/* EFI_PMIC_SCHG_SET_TESTMODE_DISCHARGING */
/*@par Summary
This API set bit0 in sdam2 MEM_036, ADDRESS 0x64 to indicate show discharging when debug board is connected

@param[in]	testmode_discharging	 		 : boolean indicating to show discharging or not in test mode

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_TESTMODE_DISCHARGING)
(
IN BOOLEAN testmode_discharging
);
/* EFI_PMIC_SCHG_SET_SHIPMODE */
/*@par Summary
This API set bit0 in sdam2 MEM_053, ADDRESS 0x75 to enable or disable PMIC ship mode

@param[in]	enable	 		 : boolean to enable or disable ship mode

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_SHIPMODE)
(
IN BOOLEAN enable
);


typedef enum {
   EFI_PM_VALID_PON_REASON_PON_REASON1,                 /* PON_REASON1 */
   EFI_PM_VALID_PON_REASON_WARM_RESET_REASON1,          /* WARM_RESET_REASON1 */
   EFI_PM_VALID_PON_REASON_ON_REASON,                   /* ON_REASON */
   EFI_PM_VALID_PON_REASON_POFF_REASON1,                /* POFF_REASON1 */
   EFI_PM_VALID_PON_REASON_OFF_REASON,                  /* OFF_REASON*/
   EFI_PM_VALID_PON_REASON_FAULT_REASON1,              /* FAULT_REASON1*/
   EFI_PM_VALID_PON_REASON_FAULT_REASON2,              /* FAULT_REASON2 */
   EFI_PM_VALID_PON_REASON_S3_RESET_REASON,            /* S3_RESET_REASON */
   EFI_PM_VALID_PON_REASON_INVALID
}EFI_PM_VALID_PON_REASON_INDEX_TYPE;

/* EFI_PMIC_SCHG_GET_VALID_PON_REASONS */
/*@par Summary
This API checks SEC_MOR_RESET flag and based on value (0xA5) returns pon reasons from latest or 
second latest reset

@param[in]	data_ptr : pointer to hold reset reasons

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/
typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_VALID_PON_REASONS)
(
IN UINT64* data_ptr
);

/* EFI_PMIC_SCHG_SET_OFF_MODE_SRC */
/*@par Summary
This API set sdam2 MEM_056, ADDRESS 0xC0 to configure off mode sourcing

@param[in]	OffModeSrcCfg	 : to configure off mode source

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_OFF_MODE_SRC)
(
IN UINT8 OffModeSrcCfg
);


/* EFI_PMIC_SCHG_SET_OFF_MODE_SRC_VBAT_THRESH */
/*@par Summary
This API set sdam2 MEM_057 and sdam2 MEM_058 with initial vbat thresholds

@param[in]	voltThHi	 : vbat thresholds for light load
@param[in]	voltThLo	 : vbat thresholds for heavy load

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_OFF_MODE_SRC_VBAT_THRESH)
(
IN UINT32 voltThHi,
IN UINT32 voltThLo
);

/* EFI_PMIC_SCHG_GET_DVDD_RESET */
/*@par Summary
This API get sdam2 MEM_059, to get dvdd reset info and reset the SDAM

@param[out]	reset	 : to indicate dvdd reset

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_DVDD_RESET)
(
OUT BOOLEAN* reset
);
/* EFI_PMIC_SCHG_SET_OS */
/*@par Summary
This API set sdam2 MEM_037, ADDRESS 0x1 to set OS

@param[in]	OS	 : to set OS type

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
 EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_OS)
 (
 IN EFI_PM_SCHG_OS_TYPE OS
 );

/* EFI_PMIC_SCHG_SET_REQ_MAX_PWR_CFG */
/*@par Summary
This API set sdam2 MEM_64 to request maximum power

@param[in] config

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
 EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_SET_REQ_MAX_PWR_CFG)
 (
 IN UINT8 config
 );

/* EFI_PMIC_SCHG_GET_REQ_MAX_PWR_CFG */
/*@par Summary
This API get sdam2 MEM_64 to request maximum power

@param[out] config : to get the config

@return
EFI_SUCCESS           -- Function completed successfully
EFI_INVALID_PARAMETER -- Parameter is invalid
EFI_DEVICE_ERROR      -- Physical device reported an error
*/

typedef
 EFI_STATUS(EFIAPI *EFI_PMIC_SCHG_GET_REQ_MAX_PWR_CFG)
 (
 OUT UINT8 *config
 );

/*===========================================================================

                        SCHG PROTOCOL INTERFACE

===========================================================================*/
/** @ingroup efi_pmicSchg_protocol
@par Summary
Qualcomm PMIC SCHG Protocol interface.

@par Parameters
@inputprotoparams{pmic_schg_proto_params.tex}
*/

struct _EFI_QCOM_PMIC_SCHG_PROTOCOL {
  UINT64                                         Revision;
  EFI_PMIC_SCHG_INIT                              SchgInit;
  EFI_PMIC_SCHG_ENABLE_CHARGER                   EnableCharger;
  EFI_PMIC_SCHG_GET_POWER_PATH                   GetPowerPath;
  EFI_PMIC_SCHG_IS_BATTERY_PRESENT               IsBatteryPresent;
  EFI_PMIC_SCHG_GET_CHARGER_PORT_TYPE            GetChargerPortType;
  EFI_PMIC_SCHG_CONFIG_OTG                       ConfigOtg;
  EFI_PMIC_SCHG_SET_OTG_I_LIMIT                  SetOtgILimit;
  EFI_PMIC_SCHG_ENABLE_AFP_MODE                  EnableAfpMode;
  EFI_PMIC_SCHG_SET_INPUT_PRIORITY               SetInputPriority;
  EFI_PMIC_SCHG_SET_FCC_MAX_CURRENT              SetFccMaxCurrent;
  EFI_PMIC_SCHG_SET_FV_MAX_VOLTAGE               SetFvMaxVoltage;
  EFI_PMIC_SCHG_ENABLE_OTG                       EnableOtg;
  EFI_PMIC_SCHG_USB_SUSPEND                      UsbSuspend;
  EFI_PMIC_SCHG_ENABLE_JEITA                     EnableJeita;
  EFI_PMIC_SCHG_GET_OTG_STATUS                   GetOtgStatus;
  EFI_PMIC_SCHG_USBIN_VALID                      UsbinValid;
  EFI_PMIC_SCHG_SET_USB_MAX_CURRENT              SetUsbMaxCurrent;
  EFI_PMIC_SCHG_CHGR_SOURCE_REINSERTED           ChgrSourceReinserted;
  EFI_PMIC_SCHG_RERUN_AICL                       RerunAicl;
  EFI_PMIC_SCHG_DUMP_PERIPHERAL                  DumpPeripheral;  
  EFI_PMIC_SCHG_ENABLE_CHG_WDOG                  EnableChgWdog;
  EFI_PMIC_SCHG_PET_CHG_WDOG                     PetChgWdog;
  EFI_PMIC_SCHG_GET_CHARGING_STATUS              GetChargingStatus;
  EFI_PMIC_SCHG_DCIN_VALID                       DcinValid;
  EFI_PMIC_SCHG_DCIN_SUSPEND                     DcinSuspend;
  EFI_PMIC_SCHG_SET_DCIN_POWER                   SetDcinPower;
  EFI_PMIC_SCHG_EXIT                             SchgExit;
  EFI_PMIC_SCHG_SET_USB_ICL_MODE                 SetUsbIclMode;
  EFI_PMIC_SCHG_GET_CHG_WDOG_STATUS              GetChgWdogStatus;
  EFI_PMIC_SCHG_ENABLE_HW_JEITA                  EnableHwJeita;
  EFI_PMIC_SCHG_TOGGLE_WIPOWER_SDLATCH           ToggleWipowerSDLatch;
  EFI_PMIC_SCHG_SET_DCIN_CURRENT                 SetDcinCurrent;
  EFI_PMIC_SCHG_SET_CHARGER_CMD_BIT              SetChargeCmdBit;
  EFI_PMIC_SCHG_GET_PMIC_INFO                    SchgGetPmicInfo;
  EFI_PMIC_SCHG_CONFIG_APSD                      ConfigApsd;
  EFI_PMIC_SCHG_CONFIG_HVDCP                     ConfigHvDcp;
  EFI_PMIC_SCHG_GET_ICL_STATUS                   GetIclStatus;
  EFI_PMIC_SCHG_TYPEC_SET_VCONN                  SetVconn;
  EFI_PMIC_SCHG_TYPEC_GET_PORT_STATE             GetPortState;
  EFI_PMIC_SCHG_TYPEC_GET_CONNECT_STATE          GetConnectState;
  EFI_PMIC_SCHG_GET_HW_JEITA_STATUS              GetHwJeitaStatus;
  EFI_PMIC_SCHG_GET_DC_IRQ_STATUS				 GetDCIrqStatus;

  EFI_PMIC_SCHG_EN_DEBUG_ACCESSORY_MODE          EnDebugAccessoryMode;
  EFI_PMIC_SCHG_GET_DAM_SNK_STATUS               GetDAMConnectSts;
  EFI_PMIC_SCHG_SET_DAM_ICL                      SetDAMIcl;

  EFI_PMIC_SCHG_GET_BATT_MISSING_STATUS          GetBattMissingStatus;
  EFI_PMIC_SCHG_GET_ACTIVE_PORT_INDEX            GetActivePort;
  EFI_PMIC_SCHG_SET_TESTMODE_DISCHARGING         SetTestModeDischarging;
  EFI_PMIC_SCHG_SET_SHIPMODE                     SetShipMode;
  EFI_PMIC_SCHG_GET_VALID_PON_REASONS            GetValidPonReasons;
  EFI_PMIC_SCHG_SET_OFF_MODE_SRC                 SetOffModeSrc;
  EFI_PMIC_SCHG_SET_OFF_MODE_SRC_VBAT_THRESH     SetOffModeSrcVbatThresh;
  EFI_PMIC_SCHG_GET_DVDD_RESET                   GetDvddReset;
  EFI_PMIC_SCHG_SET_OS                           SetOS;
  EFI_PMIC_SCHG_SET_REQ_MAX_PWR_CFG              SetReqMaxPwrCfg;
  EFI_PMIC_SCHG_GET_REQ_MAX_PWR_CFG              GetReqMaxPwrCfg;
  //Used by WP only
  //EFI_PMIC_SCHG_PRINT_AICL_STS                 PrintAiclSts;
  //EFI_PMIC_SCHG_ENABLE_SHIP_MODE               EnableShipMode;
  //EFI_PMIC_SCHG_EN_WDOG_IF_CHARGING            EnWdogIfCharging;

};

#endif  /* __EFIPMICSCHG_H__ */
