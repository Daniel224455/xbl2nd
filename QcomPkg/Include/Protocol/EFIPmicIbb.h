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
   EFI_PM_IBB_DELAY_AMNT_1MS,
   EFI_PM_IBB_DELAY_AMNT_2MS,
   EFI_PM_IBB_DELAY_AMNT_4MS,
   EFI_PM_IBB_DELAY_AMNT_8MS,
   EFI_PM_IBB_DELAY_AMNT_12MS,
   EFI_PM_IBB_DELAY_AMNT_16MS,
   EFI_PM_IBB_DELAY_AMNT_20MS,
   EFI_PM_IBB_DELAY_AMNT_24MS,
   EFI_PM_IBB_DELAY_AMNT_28MS,
   EFI_PM_IBB_DELAY_AMNT_32MS,
   EFI_PM_IBB_DELAY_AMNT_36MS,
   EFI_PM_IBB_DELAY_AMNT_40MS,
   EFI_PM_IBB_DELAY_AMNT_INVALID,
}EFI_PM_IBB_DLY_AMNT_TYPE;



typedef struct
{
  BOOLEAN                   IbbModuleRdy;  /**< ready to be enabled                      */
  BOOLEAN                   IbbModuleEn;   /**< module enabled or disabled               */
  BOOLEAN                   SwireRdy;      /**< swire_rdy                                */
  EFI_PM_IBB_DLY_AMNT_TYPE  PwrupDly;   /**< delay between IBB and AB while powering up */
  EFI_PM_IBB_DLY_AMNT_TYPE  PwrdnDly;   /**< delay between IBB and AB while powering down */
  UINT32                    OutputVolt;   /**< output voltage set                        */
  UINT32                    DefaultVolt;  /**< default voltage set                        */
  BOOLEAN                   IbbVregOk;    /**< IBB VREG OK                               */ 
  BOOLEAN                   Overload;     /**< IBB Module short circuit detect           */
  BOOLEAN                   IlimitError;  /**< current limit error                       */
  BOOLEAN                   AmoledModeEn;  /**< FALSE is LCD and TRUE is amoled          */
}EFI_PM_IBB_STATUS_TYPE;



/*===========================================================================
FUNCTION DEFINITIONS
===========================================================================*/


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


/*EFI_PM_IBB_CONFIG_OUTPUT_VOLT*/
/** @ingroup
  @par Summary 
  This API allows to select default voltage and set output 
  voltage
  
  @param[in] PmicDeviceIndex.     Primary: 0 Secondary: 1
  @param[in] Millivolt      Output voltage. Range is 800 to 5400 millivolt..
 
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_CONFIG_OUTPUT_VOLT)(
    IN UINT32   PmicDeviceIndex,
    IN UINT32   Millivolt
    );
	
	
/*EFI_PM_IBB_CONFIG_DEFAULT_VOLT*/
/** @ingroup
  @par Summary 
  This API allows to select default voltage and set output 
  voltage
  
  @param[in] PmicDeviceIndex.     Primary: 0 Secondary: 1
  @param[in] Millivolt      Output voltage. Range is 800 to 5400 millivolt.
 
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_CONFIG_DEFAULT_VOLT)(
    IN UINT32   PmicDeviceIndex,
    IN UINT32   Millivolt
    );


/*EFI_PM_IBB_CONFIG_PWRUP_PWRDN_DLY*/
/** @ingroup
  @par Summary 
   This configures the delay for IBB module 
  
  @param[in] PmicDeviceIndex.    Primary: 0 Secondary: 1
  @param[in] PwrupDly            Delay Time while powering up. Refer struct #EFI_PM_IBB_DLY_AMNT_TYPE for more info
  @param[in] PwrdnDly            Delay Time while powering down. Refer struct #EFI_PM_IBB_DLY_AMNT_TYPE for more info
        
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
 */
typedef
EFI_STATUS (EFIAPI * EFI_PM_IBB_CONFIG_PWRUP_PWRDN_DLY)(
    IN  UINT32                    PmicDeviceIndex,
	IN  EFI_PM_IBB_DLY_AMNT_TYPE  PwrupDly,
	IN  EFI_PM_IBB_DLY_AMNT_TYPE  PwrdnDly
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
  EFI_PM_IBB_MODULE_RDY                   ModuleRdy;
  EFI_PM_IBB_CONFIG_IBB_CTRL              ConfigIbbCtrl;
  EFI_PM_IBB_CONFIG_OUTPUT_VOLT           ConfigOutVolt;
  EFI_PM_IBB_CONFIG_DEFAULT_VOLT          ConfigDefaultVolt;
  EFI_PM_IBB_CONFIG_PWRUP_PWRDN_DLY       ConfigPwrupPwrdnDly;
  EFI_PM_IBB_GET_STATUS                   GetStatus;  
};


#endif  /* __EFIPMICIBB_H__ */
