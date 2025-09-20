/** @ingroup 
@file  EFIPmicAb.h
@brief PMIC ab for UEFI.
*/
/*=============================================================================
Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Confidential and Proprietary.
    
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
02/05/18   al      New file.
=============================================================================*/

#ifndef __EFIPMICAB_H__
#define __EFIPMICAB_H__

/*===========================================================================
INCLUDE FILES
===========================================================================*/


/*===========================================================================
MACRO DECLARATIONS
===========================================================================*/
/** @ingroup @addtogroup efi_pmicAb_constants 
@{ */
/** @ingroup
Protocol version.
*/

#define PMIC_AB_REVISION 0x0000000000010005
/** @ingroup @} */ /* end_addtogroup efi_pmicAb_constants */

/*  Protocol GUID definition */
/** @ingroup @ingroup efi_pmicAb_protocol */
#define EFI_PMIC_AB_PROTOCOL_GUID \
    { 0xE06FA63A, 0x48AB, 0x5682, { 0x03, 0x3B, 0x17, 0x8D, 0x35, 0x05, 0xAA, 0x6D } }

/** @ingroup @cond */
/*===========================================================================
EXTERNAL VARIABLES
===========================================================================*/
/** @ingroup
External reference to the PMIC AB Protocol GUID.
*/
extern EFI_GUID gQcomPmicAbProtocolGuid;

/*===========================================================================
TYPE DEFINITIONS
===========================================================================*/
/** @ingroup
Protocol declaration.
*/
typedef struct _EFI_QCOM_PMIC_AB_PROTOCOL   EFI_QCOM_PMIC_AB_PROTOCOL;
/** @ingroup @endcond */

/** @ingroup @addtogroup efi_pmicAb_data_types 
@{ */

typedef enum
{
  EFI_PM_AB_ISENSE_0P5,   /*0.5 ibb current sense */
  EFI_PM_AB_ISENSE_0P8,   /*0.8 ibb current sense*/
  EFI_PM_AB_ISENSE_0P9,   /*0.9 ibb current sense*/
  EFI_PM_AB_ISENSE_1P0,   /*1.0 ibb current sense*/
  EFI_PM_AB_ISENSE_1P1,   /*1.1 ibb current sense*/
  EFI_PM_AB_ISENSE_1P5,   /*1.5 ibb current sense*/
  EFI_PM_AB_ISENSE_2P0,   /*2.0 ibb current sense*/
  EFI_PM_AB_ISENSE_INVALID,
}EFI_PM_AB_ISENSE_TYPE; 


typedef struct
{
   BOOLEAN                AbModuleRdy;        /**< ready to be enabled              */
   BOOLEAN                AbModuleEn;         /**< module enabled or disabled       */
   BOOLEAN                AbIbbEnRdy;         /**< allow IBB module to enable ab   */
   BOOLEAN                EnFastPrecharge;     /**< allow fast precharge             */
   UINT32                 MaxPrechgTimeUsec;   /**< maximum pre_charge time          */
   UINT32                 OutputVolt;          /**< output voltage set               */
   BOOLEAN                AbVregOk;           /**< AB VREG OK                      */ 
   BOOLEAN                ShortCircuit;        /**< TRUE: short circuit fault. FALSE: no short circuit                  */ 
   BOOLEAN                AbStatusEn;         /**< TRUE: Current AB status enabled; FALSE: Current AB status disable  */  
   EFI_PM_AB_ISENSE_TYPE IsenseTap;           /**< Current sense tapping                         */
   EFI_PM_AB_ISENSE_TYPE IsenseGain;          /**< Current sense gain                            */
   UINT32                 PsThreshold;         /**< Threshold for when pulse-skip mode is entered */
   BOOLEAN                SelectPsTable;       /**< Select PS table                               */
   BOOLEAN                SelectDigitalPs;     /**< Select digital PS                             */
   BOOLEAN                EnablePs;            /**< Enable pulse                                  */
}EFI_PM_AB_STATUS_TYPE;



/*===========================================================================
FUNCTION DEFINITIONS
===========================================================================*/


/*EFI_PM_AB_MODULE_RDY*/     
 /** @ingroup
  @par Summary 
  This API configures module to be enabled or not enabled
  
  @param[in] PmicDeviceIndex     Primary: 0 Secondary: 1
  @param[in] AbRdy              TRUE if ready to be enabled else FALSE
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */      
typedef 
EFI_STATUS (EFIAPI *EFI_PM_AB_MODULE_RDY) (
    IN UINT32  PmicDeviceIndex, 
    IN BOOLEAN AbRdy
    );


/*EFI_PM_AB_MODULE_EN*/
 /** @ingroup
  @par Summary 
  This API enables or disabled AB module 
  
  @param[in] PmicDeviceIndex   Primary: 0 Secondary: 1
  @param[in] ModuleEn           TRUE allows IBB module to enable AB and FALSE won't
  
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */
typedef 
EFI_STATUS (EFIAPI *EFI_PM_AB_MODULE_EN)(
    IN UINT32  PmicDeviceIndex, 
    IN BOOLEAN ModuleEn
    );


/*EFI_PM_AB_IBB_RDY_EN*/ 
/** @ingroup
  @par Summary 
  This API allows AB to be enabled by IBB or not
  
  @param[in] PmicDeviceIndex     Primary: 0 Secondary: 1
  @param[in] IbbRdyEn           TRUE allows IBB module to enable AB and FALSE doesn't 
                                 
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error 
 */          
typedef 
EFI_STATUS (EFIAPI *EFI_PM_AB_IBB_RDY_EN) (
   IN UINT32  PmicDeviceIndex, 
   IN BOOLEAN IbbRdyEn
   );


/*EFI_PM_AB_CONFIG_PRECHARGE_CTRL*/
 /** @ingroup
  @par Summary 
  This API configures precharge mode and sets max precharge time
  
  @param[in] PmicDeviceIndex    Primary: 0 Secondary: 1
  @param[in] FastPrecharge     TRUE configures the fast_precharge and FLASE slow_precharge 
  @param[in] MaxTimeUsec      Max pre-charge time. Valid range is 200 to 500usec
                                 
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */
typedef 
EFI_STATUS (EFIAPI *EFI_PM_AB_CONFIG_PRECHARGE_CTRL)(
   IN UINT32 PmicDeviceIndex, 
   IN BOOLEAN FastPrecharge, 
   IN UINT32 MaxTimeUsec
   );


/*EFI_PM_AB_GET_STATUS*/ 
 /** @ingroup
  @par Summary 
  This API reads the AB module setting status 
  
  @param[in] PmicDeviceIndex    Primary: 0 Secondary: 1
  @param[in] AbStatus          Refer struct #EFI_PM_AB_STATUS_TYPE for more info
                                 
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */          
typedef 
EFI_STATUS (EFIAPI *EFI_PM_AB_GET_STATUS) (
 IN UINT32 PmicDeviceIndex, 
 OUT EFI_PM_AB_STATUS_TYPE* AbStatus
 );



/*EFI_PM_AB_CONFIG_OUTPUT_VOLT*/
/** @ingroup
  @par Summary 
  This API allows to select default voltage and set output for Vdisp 
  voltage
  
  @param[in] PmicDeviceIndex.     Primary: 0 Secondary: 1
  @param[in] OverrideOutputVolt   TRUE sets Output voltage given by SET_OUTPUT_VOLTAGE field otherwise default
  @param[in] OutputMillivolt      Output voltage. Range is 4600 to 6100 milli volt.
 
  @return
  EFI_SUCCESS            -- Function completed successfully. \n
  EFI_DEVICE_ERROR       -- Physical device reported an error. \n
*/
typedef
EFI_STATUS (EFIAPI * EFI_PM_AB_CONFIG_OUTPUT_VOLT)(
    IN UINT32   PmicDeviceIndex,
    IN BOOLEAN  OverrideOutputVolt,
    IN UINT32   OutputMillivolt
    );


/**
 * @brief This configures AB current sense 
 * 
 * @details
 *  This API configures current sense tapping point and current
 *  sense gain
 * 
 * @param[in] PmicDeviceIndex   . : Primary: 0 Secondary: 1
 * @param[in] IsenseTap           : Current sense tapping point.values 0.8; 0.9; 1.0; 1.1; Refer #EFI_PM_AB_ISENSE_TYPE for more info.
 *                                  Unsupported values will set it to default.
 * @param[in] IsenseGain          : Current sense gain. values 0.5; 1.0; 1.5; 2.0; Refer #EFI_PM_AB_ISENSE_TYPE for more info.
 *                                  Unsupported values will set it to default.
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_AB_CONFIG_CURRENT_SENSE)(
    IN UINT32                 PmicDeviceIndex,
    IN EFI_PM_AB_ISENSE_TYPE  IsenseTap,
    IN EFI_PM_AB_ISENSE_TYPE  IsenseGain 
    );


 /**
 * @brief This configures AB pulse skipping parameters  
 * 
 * @details
 *  This API configures pulse skipping parameters
 * 
 * @param[in] PmicDeviceIndex.  : Primary: 0 Secondary: 1
 * @param[in] PsThreshold       : Threshold when pulse skipping mode is entered. Range is 50 to 80 mAmps
 * @param[in] EnablePs          : TRUE to enable PS, otherwise FALSE to disable it
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not avaiable on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_AB_CONFIG_PULSE_SKIP_CTRL)(
    IN UINT32   PmicDeviceIndex,
    IN UINT32   PsThreshold,  
    IN BOOLEAN  EnablePs 
    );

/*===========================================================================
PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup @ingroup efi_pmicAb_protocol
@par Summary
Qualcomm PMIC AB Protocol interface.

@par Parameters
@inputprotoparams{pmic_ab_proto_params.tex} 
*/

struct _EFI_QCOM_PMIC_AB_PROTOCOL {
  UINT64                                 Revision;
  EFI_PM_AB_MODULE_RDY                   ModuleRdy;
  EFI_PM_AB_MODULE_EN                    ModuleEn;
  EFI_PM_AB_IBB_RDY_EN                   IbbRdyEn;
  EFI_PM_AB_CONFIG_PRECHARGE_CTRL        PrechargeCtrl;
  EFI_PM_AB_GET_STATUS                   GetStatus;
  EFI_PM_AB_CONFIG_OUTPUT_VOLT           ConfigOutVolt;
  EFI_PMIC_AB_CONFIG_CURRENT_SENSE       ConfigCurrentSense;
  EFI_PMIC_AB_CONFIG_PULSE_SKIP_CTRL     ConfigPulseSkipCtrl;
};


#endif  /* __EFIPMICAB_H__ */
