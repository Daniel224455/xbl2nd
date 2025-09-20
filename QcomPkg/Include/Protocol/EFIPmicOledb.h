/** @ingroup 
@file  EFIPmicOledb.h
@brief PMIC oledb for UEFI.
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

#ifndef __EFIPMICOLEDB_H__
#define __EFIPMICOLEDB_H__

/*===========================================================================
INCLUDE FILES
===========================================================================*/


/*===========================================================================
MACRO DECLARATIONS
===========================================================================*/
/** @ingroup @addtogroup efi_pmicOledb_constants 
@{ */
/** @ingroup
Protocol version.
*/

#define PMIC_OLEDB_REVISION 0x0000000000010005
/** @ingroup @} */ /* end_addtogroup efi_pmicOledb_constants */

/*  Protocol GUID definition */
/** @ingroup @ingroup efi_pmicOledb_protocol */
#define EFI_PMIC_OLEDB_PROTOCOL_GUID \
    { 0xDAE8D7EE, 0x4C18, 0x3FC8, { 0xC2, 0xBF, 0x33, 0x87, 0x06, 0xF8, 0xF1, 0x29 } };

/** @ingroup @cond */
/*===========================================================================
EXTERNAL VARIABLES
===========================================================================*/
/** @ingroup
External reference to the PMIC OLEDB Protocol GUID.
*/
extern EFI_GUID gQcomPmicOledbProtocolGuid;

/*===========================================================================
TYPE DEFINITIONS
===========================================================================*/
/** @ingroup
Protocol declaration.
*/
typedef struct _EFI_QCOM_PMIC_OLEDB_PROTOCOL   EFI_QCOM_PMIC_OLEDB_PROTOCOL;
/** @ingroup @endcond */



/*===========================================================================
FUNCTION DEFINITIONS
===========================================================================*/

/* EFI_PM_OLEDB_MODULE_RDY */ 
/** @ingroup efi_pm_oledb_module_rdy
  @par Summary
  sets OLEDB module ready to be enabled. 

  @param[in]  PmicDeviceIndex  Primary: 0. Secondary: 1.               
  @param[in]  OledbRdy         TRUE: OLEDB module ready to be enabled. 
                               FALSE: Otherwise

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/
typedef 
EFI_STATUS (EFIAPI* EFI_PM_OLEDB_MODULE_RDY)(
   IN UINT32  PmicDeviceIndex, 
   IN BOOLEAN OledbRdy
   );
                        
  
/* EFI_PM_OLEDB_ENABLE_MODULE */ 
/** @ingroup efi_pm_oledb_enable_module
  @par Summary
  This function enables OLEDB module 

  @param[in]  PmicDeviceIndex  Primary: 0. Secondary: 1.               
  @param[in]  Enable           TRUE: Enable OLEDB module. FALSE: Otherwise

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/                        
typedef                   
EFI_STATUS (EFIAPI* EFI_PM_OLEDB_ENABLE_MODULE)(
   IN UINT32  PmicDeviceIndex, 
   IN BOOLEAN Enable
   );
                          
                          
/* EFI_PM_OLEDB_ENABLE_EXT_PIN_CTL */ 
/** @ingroup efi_pm_oledb_enable_ext_pin_ctl
  @par Summary
  This function configures external pin control to enable OLEDB module 

  @param[in]  PmicDeviceIndex  Primary: 0. Secondary: 1.               
  @param[in]  Enable           TRUE: Enable OLEDB ext pin control. FALSE: Otherwise

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/                             
typedef                   
EFI_STATUS (EFIAPI* EFI_PM_OLEDB_ENABLE_EXT_PIN_CTL)(
   IN UINT32  PmicDeviceIndex, 
   IN BOOLEAN Enable
   );
                          
                          
/* EFI_PM_OLEDB_SET_OUTPUT_VOLT */ 
/** @ingroup efi_pm_oledb_set_output_volt
  @par Summary
  This function configures external pin control to enable OLEDB module 

  @param[in]  PmicDeviceIndex  Primary: 0. Secondary: 1.               
  @param[in]  VoutMv           Voltage in millivolt to be set

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/                            
typedef                   
EFI_STATUS (EFIAPI* EFI_PM_OLEDB_SET_OUTPUT_VOLT) (
   IN UINT32 PmicDeviceIndex, 
   IN UINT32 VoutMv
   );


/*===========================================================================
PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup @ingroup efi_pmicOledb_protocol
@par Summary
Qualcomm PMIC OLEDB Protocol interface.

@par Parameters
@inputprotoparams{pmic_oledb_proto_params.tex} 
*/

struct _EFI_QCOM_PMIC_OLEDB_PROTOCOL {
  UINT64                          Revision;
  EFI_PM_OLEDB_MODULE_RDY         ModuleRdy;
  EFI_PM_OLEDB_ENABLE_MODULE      EnableModule;
  EFI_PM_OLEDB_ENABLE_EXT_PIN_CTL EnableExtPinCtl;
  EFI_PM_OLEDB_SET_OUTPUT_VOLT    SetOutputVolt;
};


#endif  /* __EFIPMICOLEDB_H__ */
