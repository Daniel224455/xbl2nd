/** @ingroup 
@file  EFIPmicIbb.h
@brief PMIC ibb for UEFI.
*/
/*=============================================================================
Copyright (c) 2014 Qualcomm Technologies, Inc.
All rights reserved.
Qualcomm Technologies Confidential and Proprietary.
    
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
12/09/14   al      Sync with latest
06/24/14   al      Added API for configuring IBB delay 
06/09/14   al      Arch update  
05/14/14   al      New file.
=============================================================================*/

#ifndef __EFIPMICIBB_H__
#define __EFIPMICIBB_H__

/*===========================================================================
INCLUDE FILES
===========================================================================*/


/*===========================================================================
MACRO DECLARATIONS
===========================================================================*/
/** @ingroup @addtogroup efi_pmicIbb_constants 
@{ */
/** @ingroup
Protocol version.
*/

#define PMIC_IBB_REVISION 0x0000000000010003
/** @ingroup @} */ /* end_addtogroup efi_pmicIbb_constants */

/*  Protocol GUID definition */
/** @ingroup @ingroup efi_pmicIbb_protocol */
#define EFI_PMIC_IBB_PROTOCOL_GUID \
    { 0xc1bff82d, 0x6c92, 0x48fb, { 0x83, 0x55, 0xc7, 0xfb, 0x42, 0x18, 0xa4, 0xea } }

/** @ingroup @cond */
/*===========================================================================
EXTERNAL VARIABLES
===========================================================================*/
/** @ingroup
External reference to the PMIC IBB Protocol GUID.
*/
extern EFI_GUID gQcomPmicIbbProtocolGuid;

/*===========================================================================
TYPE DEFINITIONS
===========================================================================*/
/** @ingroup
Protocol declaration.
*/
typedef struct _EFI_QCOM_PMIC_IBB_PROTOCOL   EFI_QCOM_PMIC_IBB_PROTOCOL;
/** @ingroup @endcond */

/** @ingroup @addtogroup efi_pmicIbb_data_types 
@{ */


typedef enum
{
   EFI_PM_IBB_PWRUP_DLY_SRC_BIAS_ACTIVE, /**< IBB power stage delay source is after IBB's bias active                                        */
   EFI_PM_IBB_PWRUP_DLY_SRC_LAB_VREG_OK, /**< IBB power stage delay source is after VREG_OK from LAB and IBB's bias active 0x0 : BIAS_ACTIVE */
   EFI_PM_IBB_PWRUP_DLY_SRC_INVALID      /**< Invalid                                                                                        */
}EFI_PM_IBB_PWRUP_DLY_SRC_TYPE;

typedef enum
{
   EFI_PM_IBB_PWRDN_DLY_SRC_IBB_DISABLE,   /**< LAB power-down delay source is after IBB disable */
   EFI_PM_IBB_PWRDN_DLY_SRC_IBB_DISCHARGE, /**< LAB power-down delay is after IBB's discharged   */
   EFI_PM_IBB_PWRDN_DLY_SRC_INVALID        /**< Invalid                                          */
}EFI_PM_IBB_PWRDN_DLY_SRC_TYPE;


typedef struct
{
   UINT32                        PwrdnDelayMs;   /**< Delay between IBB and LAB when powering down. Valid values are 1,2,4,8 mS  */
   EFI_PM_IBB_PWRDN_DLY_SRC_TYPE PwrdnDelaySrc;  /**< refer EFI_PM_IBB_PWRDN_DLY_SRC_TYPE for more info*/
   UINT32                        PwrupDelayMs;   /**< Delay between IBB and LAB when powering up. Valid values are 1,2,4,8 mS  */
   BOOLEAN                       EnPwrupDelay;   /**< enable disable powerup delay                      */
   EFI_PM_IBB_PWRUP_DLY_SRC_TYPE PwrupDelaySrc;  /**< refer EFI_PM_IBB_PWRUP_DLY_SRC_TYPE for more info */
} EFI_PM_IBB_PWRUP_PWRDN_DELAY_TYPE; 

typedef struct
{
   BOOLEAN                           AmoledModeEn;                         /**< FALSE is LCD and TRUE is amoled          */
   BOOLEAN                           IbbModuleRdy;                         /**< ready to be enabled                      */
   BOOLEAN                           IbbModuleEn;                          /**< module enabled or disabled               */
   BOOLEAN                           SwireRdy;                             /**< swire_rdy                                */
   UINT32                            ChgrResistorKohms;                    /**< charger resistor                         */
   UINT32                            Swire_pulse_mv;                       /**< swire output voltage pulse in milli volt */
   BOOLEAN                           OverrideOutputVolt;                   /**< override output voltage                  */
   UINT32                            OutputVolt;                           /**< output voltage set                       */
   EFI_PM_IBB_PWRUP_PWRDN_DELAY_TYPE IbbDelayConfig;                         /**< IBB delay config                         */
   BOOLEAN                           IbbVregOk;                            /**< IBB VREG OK                              */ 
   BOOLEAN                           ShortCircuit;                         /**< IBB Module short circuit detect          */
   BOOLEAN                           IlimitError;                          /**< IBB current limit error                  */
}EFI_PM_IBB_STATUS_TYPE;


/*===========================================================================
FUNCTION DEFINITIONS
===========================================================================*/

/*EFI_PM_IBB_LCD_AMOLED_SEL*/
/** @ingroup
  @par Summary 
  This function configures module for AMOLED or LCD
  
  @param[in] PmicDeviceIndex.  Primary: 0 Secondary: 1
  @param[in] EnAmoledMode      TRUE enables for AMOLED and FALSE for LCD 
  
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error.n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_LCD_AMOLED_SEL)(
    IN UINT32  PmicDeviceIndex,
    IN BOOLEAN EnAmoledMode
    );


/*EFI_PM_IBB_MODULE_RDY*/
/** @ingroup
  @par Summary 
  This API prepares module to be enabled or not. When this is set 
  to low, the IBB module cannot be enabled (powered-up) through 
  S-Wire or IBB_ENABLE register 
  
  @param[in] PmicDeviceIndex.  Primary: 0 Secondary: 1
  @param[in] IbbRdy             TRUE if ready to be enabled else FALSE
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error. 
*/
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_MODULE_RDY)(
    IN UINT32  PmicDeviceIndex,
    IN BOOLEAN IbbRdy
    );


/*EFI_PM_IBB_CONFIG_IBB_CTRL*/
/** @ingroup
  @par Summary 
   This API enables/disables module and makes S-wire ready for listening
  
  @param[in] PmicDeviceIndex.  Primary: 0 Secondary: 1
  @param[in] EnModule      TRUE enables module and false disables module
  @param[in] SwireRdy      TRUE ready to liste to to S-wire and FALSE not ready to listen to S-Wire
   
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error. 
 */
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_CONFIG_IBB_CTRL)(
    IN UINT32  PmicDeviceIndex,
    IN BOOLEAN EnModule,
    IN BOOLEAN SwireRdy
    );


/*EFI_PM_IBB_SET_SOFT_STRT_CHGR_RESISTOR*/
/** @ingroup
  @par Summary 
  This API sets the charging resistance for soft start. 
  Time constant is ref_cap*charging resistor.
  
  @param[in] PmicDeviceIndex.  Primary: 0 Secondary: 1
  @param[in] KOhms     Charging resistor value. Values are 16 to 300 KOhms
                                 
 
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
 */
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_SET_SOFT_STRT_CHGR_RESISTOR)(
    IN UINT32  PmicDeviceIndex,
    IN UINT32  KOhms
    );


/*EFI_PM_IBB_SET_SWIRE_OUTPUT_PULSE*/
/** @ingroup
  @par Summary 
  This API sets startup voltage when there is only one s-wire 
  voltage programming pulse
  
  @param[in] PmicDeviceIndex.  Primary: 0 Secondary: 1
  @param[in] VoltInMilliVolt   Valid value range is 1400 to 7700 milli volt
                                 
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_SET_SWIRE_OUTPUT_PULSE)(
    IN UINT32  PmicDeviceIndex,
    IN UINT32  VoltInMilliVolt
    );


/*EFI_PM_IBB_CONFIG_OUTPUT_VOLT*/
/** @ingroup
  @par Summary 
  This API allows to select default voltage and set output 
  voltage
  
  @param[in] PmicDeviceIndex.     Primary: 0 Secondary: 1
  @param[in] OverrideOutputVolt   TRUE sets Output voltage given by SET_OUTPUT_VOLTAGE field otherwise default
  @param[in] OutputMillivolt      Output voltage. Range is 1400 to 7700 milli volt.
 
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_CONFIG_OUTPUT_VOLT)(
    IN UINT32   PmicDeviceIndex,
    IN BOOLEAN  OverrideOutputVolt,
    IN UINT32   OutputMillivolt
    );


/*EFI_PM_IBB_GET_STATUS*/
/** @ingroup
  @par Summary 
   This API reads the IBB module setting status
  
  @param[in] PmicDeviceIndex.   Primary: 0 Secondary: 1
  @param[in] IbbStatus          Refer struct #EFI_PM_IBB_STATUS_TYPE for more info
        
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
 */
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_GET_STATUS)(
    IN  UINT32                   PmicDeviceIndex,
    OUT EFI_PM_IBB_STATUS_TYPE   *IbbStatus
    );


/*EFI_PM_IBB_CONFIG_PWRUP_PWRDN_DLY*/
/** @ingroup
  @par Summary 
   This configures the delay for IBB module 
  
  @param[in] PmicDeviceIndex.     Primary: 0 Secondary: 1
  @param[in] DelayConfig          Refer struct #EFI_PM_IBB_PWRUP_PWRDN_DELAY_TYPE for more info
        
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
 */
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_CONFIG_PWRUP_PWRDN_DLY)(
    IN  UINT32                              PmicDeviceIndex,
    OUT EFI_PM_IBB_PWRUP_PWRDN_DELAY_TYPE   *DelayConfig
    );


/*===========================================================================
PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup @ingroup efi_pmicIbb_protocol
@par Summary
Qualcomm PMIC IBB Protocol interface.

@par Parameters
@inputprotoparams{pmic_ibb_proto_params.tex} 
*/

struct _EFI_QCOM_PMIC_IBB_PROTOCOL {
  UINT64                                  Revision;
  EFI_PM_IBB_LCD_AMOLED_SEL               LcdAmoledSel;
  EFI_PM_IBB_MODULE_RDY                   ModuleRdy;
  EFI_PM_IBB_CONFIG_IBB_CTRL              ConfigIbbCtrl;
  EFI_PM_IBB_SET_SOFT_STRT_CHGR_RESISTOR  ChgrResistor;
  EFI_PM_IBB_SET_SWIRE_OUTPUT_PULSE       SetSwireOutPulse;
  EFI_PM_IBB_CONFIG_OUTPUT_VOLT           ConfigOutVolt;
  EFI_PM_IBB_GET_STATUS                   GetStatus;
  EFI_PM_IBB_CONFIG_PWRUP_PWRDN_DLY       ConfigPwrupPwrdnDly;
};


#endif  /* __EFIPMICIBB_H__ */
