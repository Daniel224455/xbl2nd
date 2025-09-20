/** @file EFIPmicWled.h

  PMIC WLED Protocol definitions

  Copyright (c) 2012 - 2018, Qualcomm Technologies Inc. All 
  rights reserved. 

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 10/19/16   al      Adding APIs for configuring SWIRE default value
 03/29/16   al      Updating for newer target
 28/05/14   al      Adding APIs for configuring various AMOLED param.
 06/23/14   al      Added GetStatus 
 06/12/14   al      Updating as per new target
 04/29/14   al      Added API to enable AMOLED 
 08/28/13   llg     (Tech Pubs) Edited/added Doxygen comments and markup.
 01/14/13   al      Replacing enums with type uint32 
 12/21/12   al      Updating eumn for SWITCHING FREQUENCY  
 08/16/12   al      Changed EFI_PM_WLED_BST_MAX_DUTY_TYPE as per 8974 WLED document
 07/27/12   al      Renamed all Enums,added device index and resource index
 03/05/12   sm      Changes parameters for EFI_PM_WLED_ENABLE_SYNC
 02/08/12   sm      New file.
=============================================================================*/

#ifndef __EFIPMICWLED_H__
#define __EFIPMICWLED_H__

/** @ingroup efi_pmicWled_constants */
/** Protocol version. */
#define PMIC_WLED_REVISION 0x0000000000010009

/* Protocol GUID definition */
/** @ingroup efi_pmicWled_protocol */
#define EFI_PMIC_WLED_PROTOCOL_GUID \
{ 0x5bef7c41, 0xa560, 0x435b, { 0x83, 0x5b, 0x3c, 0x3e, 0xbf, 0x2b, 0xd1, 0x51 } }


/* PMIC WLED UEFI typedefs */
/** @cond */
typedef struct _EFI_QCOM_PMIC_WLED_PROTOCOL   EFI_QCOM_PMIC_WLED_PROTOCOL;
/** @endcond */

/* PMIC UEFI WLED Procotol GUID */
typedef enum {
  EFI_PM_WLED_NONE = 0,
  EFI_PM_WLED_LED1 = 1,
  EFI_PM_WLED_LED2 = 2,
  EFI_PM_WLED_LED3 = 4,
  EFI_PM_WLED_LED4 = 8,
  EFI_PM_WLED_ALL = EFI_PM_WLED_LED1 | EFI_PM_WLED_LED2 |EFI_PM_WLED_LED3 | EFI_PM_WLED_LED4,
  EFI_PM_WLED_INVALID
} EFI_PM_WLED_LED_TYPE;


/*WLED modulator brightness width*/
typedef enum
{
  EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_12BIT,  /*MODULATOR WIDTH 12bit*/
  EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_15BIT,  /*MODULATOR WIDTH 15bit*/
  EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_INVALID,
}EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_TYPE;


/*WLED OVP limits*/
typedef enum {
  EFI_PM_WLED_OVP_THR_38P5V = 0,   // 38.5V 
  EFI_PM_WLED_OVP_THR_37V,         // 37V   
  EFI_PM_WLED_OVP_THR_35p5V,       // 35.5V 
  EFI_PM_WLED_OVP_THR_34V,         // 34V   
  EFI_PM_WLED_OVP_THR_32p5V,       // 32.5V 
  EFI_PM_WLED_OVP_THR_31V,         // 31V   
  EFI_PM_WLED_OVP_THR_29P5V,       // 29.5V 
  EFI_PM_WLED_OVP_THR_28V,         // 28V   
  EFI_PM_WLED_OVP_THR_26P5V,       // 26.5V 
  EFI_PM_WLED_OVP_THR_25V,         // 25V   
  EFI_PM_WLED_OVP_THR_23P5V,       // 23.5V 
  EFI_PM_WLED_OVP_THR_22V,         // 22V   
  EFI_PM_WLED_OVP_THR_20P5V,       // 20.5V 
  EFI_PM_WLED_OVP_THR_19V,         // 19V   
  EFI_PM_WLED_OVP_THR_17P5V,       // 17.5V 
  EFI_PM_WLED_OVP_THR_16V,         // 16V   
  EFI_PM_WLED_OVP_THR_INVALID
} EFI_PM_WLED_OVP_THRESHOLD_TYPE;

/*WLED string current limits*/
typedef enum {
  EFI_PM_WLED_BST_ILIMIT_525mA ,   //525mA 
  EFI_PM_WLED_BST_ILIMIT_700mA ,   //700mA 
  EFI_PM_WLED_BST_ILIMIT_875mA ,   //875mA 
  EFI_PM_WLED_BST_ILIMIT_1050mA,   //1050mA
  EFI_PM_WLED_BST_ILIMIT_1225mA,   //1225mA
  EFI_PM_WLED_BST_ILIMIT_1400mA,   //1400mA
  EFI_PM_WLED_BST_ILIMIT_1575mA,   //1575mA
  EFI_PM_WLED_BST_ILIMIT_1750mA,   //1750mA
  EFI_PM_WLED_BST_ILIMIT_INVALID
} EFI_PM_WLED_BST_ILIMIT_TYPE;


/*WLED IRQ types*/
typedef enum {
  EFI_PM_WLED_IRQ_ILIM_FAULT ,
  EFI_PM_WLED_IRQ_OVP_FAULT,
  EFI_PM_WLED_IRQ_SC_FAULT,
  EFI_PM_WLED_IRQ_FLASH_SAFETY_TIMER,
  EFI_PM_WLED_IRQ_PRE_FLASH_DONE,
  EFI_PM_WLED_IRQ_FLASH_DONE,
  EFI_PM_WLED_IRQ_FLASH_EXIT,
  EFI_PM_WLED_IRQ_INVALID
} EFI_PM_WLED_IRQ_TYPE;


/*WLED modulator types*/
typedef enum
{
  EFI_PM_MODULATOR_TYPE_A,
  EFI_PM_MODULATOR_TYPE_B,
  EFI_PM_MODULATOR_TYPE_INVALID,
}EFI_PM_WLED_MODULATOR_TYPE;

/*WLED modulator source type*/
typedef enum
{
  EFI_PM_MODULATOR_SRC_DRV_HIGH,
  EFI_PM_MODULATOR_SRC_CABC1,
  EFI_PM_MODULATOR_SRC_CABC2,
  EFI_PM_MODULATOR_SRC_EXT,
  EFI_PM_MODULATOR_SRC_INVALID,
}EFI_PM_WLED_MODULATOR_SRC_TYPE; 

typedef struct
{
  BOOLEAN                                      Enabled;
  EFI_PM_WLED_MODULATOR_SRC_TYPE               Src;
  EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_TYPE  Width;
  UINT16                                       DutyCycle;
}EFI_PM_MOULATOR_STATUS_TYPE;

typedef struct
{
  BOOLEAN                    EnableCurrentSink;
  UINT32                     CurrentMilliamp;
  EFI_PM_WLED_MODULATOR_TYPE LedModulatorSrc;
}EFI_PM_LED_STATUS_TYPE;

typedef struct
{
  BOOLEAN                        ModuleEnable;
  EFI_PM_WLED_OVP_THRESHOLD_TYPE OvpThreshold;
  EFI_PM_WLED_BST_ILIMIT_TYPE    BstIlimitType;
  UINT32                         FswCtr;
  EFI_PM_MOULATOR_STATUS_TYPE    Modulator[2];
  EFI_PM_LED_STATUS_TYPE         Led[4];
}EFI_PM_WLED_STATUS_TYPE; 


/** @ingroup efi_pm_wled_enable
  @par Summary
  Enables the WLED module. 

  @param[in]  PmicDeviceIndex  Primary: 0.
  @param[in]  ResourceIndex    Resource index. 
  @param[in]  Enable           Whether or not the WLED module is enabled.
 
  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_ENABLE)(
  IN UINT32  PmicDeviceIndex, 
  IN UINT32  ResourceIndex,
  IN BOOLEAN Enable
);

/* EFI_PM_WLED_ENABLE_MODULATOR */
/** @ingroup efi_pm_wled_enable_modulator
  @par Summary
  Enables the digital modulator generator and modulator input for each current 
  sink.
  
  @param[in]  PmicDeviceIndex  Primary: 0.
  @param[in]  Modulator       WLED modulator type; see #EFI_PM_WLED_MODULATOR_TYPE for details.
  @param[in]  Enable          To enable TRUE and to disable FALS.
 
  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_ENABLE_MODULATOR)(
  IN UINT32                      PmicDeviceIndex,
  IN EFI_PM_WLED_MODULATOR_TYPE  Modulator,
  IN BOOLEAN                     Enable
);

/* EFI_PM_WLED_ENABLE_CURRENT_SINK */
/** @ingroup efi_pm_wled_enable_sink_current
  @par Summary
  Enables the control for the LED current sink outputs.
  
  @param[in]  PmicDeviceIndex  Primary: 0.
  @param[in]  LedType          LED type; see #EFI_PM_WLED_LED_TYPE for details.
 
  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_ENABLE_CURRENT_SINK)(
  IN UINT32               PmicDeviceIndex,
  IN EFI_PM_WLED_LED_TYPE LedType
);


/* EFI_PM_WLED_LED_FULL_SCALE_CURRENT */
/** @ingroup efi_pm_wled_led_full_scale_current
  @par Summary
  Sets the LED full scale current. 
  
  @param[in]  PmicDeviceIndex  Primary: 0.
  @param[in]  LedType          LED type; see #EFI_PM_WLED_LED_TYPE for 
                               details.
  @param[in]  CurrentMicroAmp  LED current 2500 to 30000 uAmp.
 
  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_LED_FULL_SCALE_CURRENT)(
  IN UINT32                  PmicDeviceIndex,
  IN EFI_PM_WLED_LED_TYPE    LedType,
  IN UINT32                  CurrentMicroAmp
);


/* EFI_PM_WLED_SET_LED_DUTY_CYCLE */
/** @ingroup efi_pm_wled_set_led_duty_cycle
  @par Summary
  Sets the 12-bit duty cycle control for the indicated LED digital modulator 
  control.
  
  @param[in]  PmicDeviceIndex  Primary: 0, Secondary: 1, etc.
  @param[in]  Modulator:       Type of modulator. See #EFI_PM_WLED_MODULATOR_TYPE for more info
  @param[in]  Data             Data is between 0x000 to 0xFFF. This 12-bit data 
                               is used to set the current value for the LED 
                               duty cycle. The final current for the duty 
                               cycle = 12-bit data * LSB value.
                               At FSC = 30mA, LSB = 1.875mA
                               AT FSC = 25mA, LSB = 1.563mA
                               AT FSC = 15mA, LSB = 0.938mA
 
  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_SET_LED_DUTY_CYCLE)(
  IN UINT32                     PmicDeviceIndex,
  IN EFI_PM_WLED_MODULATOR_TYPE Modulator,
  IN UINT16                     Data
);



/* EFI_PM_WLED_ENABLE_SYNC */
/** @ingroup efi_pm_wled_enable_sync
  @par Summary
  Enables the LED sync. This updates the 12-bit register values to 
  the modulator and the full scale current setting register.
  
  @param[in]  PmicDeviceIndex  Primary: 0.
  @param[in]  LedType          LED type; see #EFI_PM_WLED_LED_TYPE for details. 
  @param[in]  Enable           Whether or not the LED sync is enabled.
 
  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_ENABLE_SYNC)(
  IN UINT32                  PmicDeviceIndex,
  IN EFI_PM_WLED_LED_TYPE    LedType,
  IN BOOLEAN                 Enable
);


/* EFI_PM_WLED_SET_FSW_CTRL */
/** @ingroup efi_pm_wled_set_fsw_ctrl
  @par Summary
  Selects the boost switching frequency control.
 
  @param[in]  PmicDeviceIndex  Primary: 0.
  @param[in]  FswKhz           Boost switching frequency control in kilohertz.

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_SET_FSW_CTRL)(
  IN UINT32 PmicDeviceIndex, 
  IN UINT32 ResourceIndex,
  IN UINT32 FswKhz
);


/* EFI_PM_WLED_SET_OVP_THRESHOLD */
/** @ingroup efi_pm_wled_set_ovp_threshold
  @par Summary
  Selects the overvoltage protection threshold.
 
  @param[in]  PmicDeviceIndex  Primary: 0.
  @param[in]  ResourceIndex    Resource index. 
  @param[in]  OvpThrType       Overvoltage protection threshold; see 
                               #EFI_PM_WLED_OVP_THRESHOLD_TYPE

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_SET_OVP_THRESHOLD)(
  IN UINT32                            PmicDeviceIndex, 
  IN UINT32                            ResourceIndex,
  IN EFI_PM_WLED_OVP_THRESHOLD_TYPE    OvpThrType
);


/* EFI_PM_WLED_SET_BOOST_ILIMIT */
/** @ingroup efi_pm_wled_set_boost_ilimit
  @par Summary
  Selects the boost current limit.
 
  @param[in]  PmicDeviceIndex  Primary: 0.
  @param[in]  ResourceIndex    Resource index. 
  @param[in]  BstIlimitType    Boost current limit; see 
                               #EFI_PM_WLED_BST_ILIMIT_TYPE for details.

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_SET_BOOST_ILIMIT)(
  IN UINT32                       PmicDeviceIndex, 
  IN UINT32                       ResourceIndex,
  IN EFI_PM_WLED_BST_ILIMIT_TYPE  BstIlimitType
);


/**
 * @brief This function reads the WLED status
 * 
 * @param[in]  PmicDeviceIndex : PMIC device index. 0 for primary and 1 for secondary
 * @param[out] WledStatus      : Wled status.  Refer enum #EFI_PM_WLED_STATUS_TYPE for more info
 * 
 * @return
 *       EFI_SUCCESS        -- Function completed successfully. 
 *       EFI_PROTOCOL_ERROR -- Error occurred during the operation.
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_GET_STATUS)(
  IN  UINT32                  PmicDeviceIndex,
  OUT EFI_PM_WLED_STATUS_TYPE *WledStatus
);



/**
 * @brief This function selects the modulator input source
 * 
 * @param[in] PmicDeviceIndex : PMIC index
 * 
 * @param[in] Modulator: Type of modulator. See #EFI_PM_WLED_MODULATOR_TYPE for more info
 *
 * @param[in] Src : Input source for modulator. See #EFI_PM_WLED_MODULATOR_SRC_TYPE for more info
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_MODULATOR_SRC_SEL)(
    IN UINT32                         PmicDeviceIndex, 
    IN EFI_PM_WLED_MODULATOR_TYPE     Modulator, 
    IN EFI_PM_WLED_MODULATOR_SRC_TYPE Src
    );


/**
 * @brief This function configures modulator brightness width
 * 
 * @param[in] PmicDeviceIndex : PMIC index
 * 
 * @param[in] Modulator: Type of modulator. See #EFI_PM_WLED_MODULATOR_TYPE for more info
 *
 * @param[in] Width : Width for modulator brightness. See #EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_TYPE for more info
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH)(
    IN UINT32                                       PmicDeviceIndex,  
    IN EFI_PM_WLED_MODULATOR_TYPE                   Modulator, 
    IN EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_TYPE  Width
    );


/**
 * @brief This function selects LED modulator source
 * 
 * @param[in] PmicDeviceIndex : PMIC index*
 * @param[in]  LedType          LED type; see #EFI_PM_WLED_LED_TYPE for details. 
 * @param[in] Modulator: Type of modulator. See #EFI_PM_WLED_MODULATOR_TYPE for more info*
 *
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
typedef
EFI_STATUS (EFIAPI *EFI_PM_WLED_LED_MODULATOR_SRC_SEL)(
    IN UINT32                     PmicDeviceIndex, 
    IN EFI_PM_WLED_LED_TYPE       LedType, 
    IN EFI_PM_WLED_MODULATOR_TYPE Modulator
    );


/**
  @brief
  PMIC VREG UEFI Protocol definition
 */
struct _EFI_QCOM_PMIC_WLED_PROTOCOL {
  UINT64                                    Revision;
  EFI_PM_WLED_ENABLE                        EnableWled;
  EFI_PM_WLED_ENABLE_MODULATOR              EnableModulator;
  EFI_PM_WLED_ENABLE_CURRENT_SINK           EnableCurrentSink;
  EFI_PM_WLED_LED_FULL_SCALE_CURRENT        LedFullScaleCurrent;
  EFI_PM_WLED_SET_LED_DUTY_CYCLE            SetLedDutyCycle;
  EFI_PM_WLED_ENABLE_SYNC                   EnableSync;
  EFI_PM_WLED_SET_FSW_CTRL                  SetFswCtrl;
  EFI_PM_WLED_SET_OVP_THRESHOLD             SetOvpThreshold;
  EFI_PM_WLED_SET_BOOST_ILIMIT              SetBoostIlimit;
  EFI_PM_WLED_GET_STATUS                    GetStatus;
  EFI_PM_WLED_MODULATOR_SRC_SEL             ModulatorSrcSel;           
  EFI_PM_WLED_MODULATOR_BRIGHTNESS_WIDTH    ModulatorBrightnessWidth;
  EFI_PM_WLED_LED_MODULATOR_SRC_SEL         LedModulatorSrcSel;   
};

#endif  /* __EFIPMICWLED_H__ */
