/** @ingroup 
@file  EFIPmicLcdb.h
@brief PMIC lcdb for UEFI.
*/
/*=============================================================================
Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
Qualcomm Technologies Confidential and Proprietary.
    
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     ----------------------------------------------------------- 

02/05/18   al      New file.
=============================================================================*/

#ifndef __EFIPMICLCDB_H__
#define __EFIPMICLCDB_H__

/*===========================================================================
INCLUDE FILES
===========================================================================*/


/*===========================================================================
MACRO DECLARATIONS
===========================================================================*/
/** @ingroup @addtogroup efi_pmicLcdb_constants 
@{ */
/** @ingroup
Protocol version.
*/

#define PMIC_LCDB_REVISION 0x0000000000010005
/** @ingroup @} */ /* end_addtogroup efi_pmicLcdb_constants */

/*  Protocol GUID definition */
/** @ingroup @ingroup efi_pmicLcdb_protocol */
#define EFI_PMIC_LCDB_PROTOCOL_GUID \
    { 0x2450bff5, 0x61ec, 0x49a1, { 0x8d, 0xe, 0x12, 0xf7, 0xb9, 0x6f, 0x59, 0xd } }

/** @ingroup @cond */
/*===========================================================================
EXTERNAL VARIABLES
===========================================================================*/
/** @ingroup
External reference to the PMIC LCDB Protocol GUID.
*/
extern EFI_GUID gQcomPmicLcdbProtocolGuid;

/*===========================================================================
TYPE DEFINITIONS
===========================================================================*/
/** @ingroup
Protocol declaration.
*/
typedef struct _EFI_QCOM_PMIC_LCDB_PROTOCOL   EFI_QCOM_PMIC_LCDB_PROTOCOL;
/** @ingroup @endcond */

/** @ingroup @addtogroup efi_pmicLcdb_data_types 
@{ */

typedef enum
{
   EFI_PM_LCDB_ENABLE_CTL_MODULE_EN,
   EFI_PM_LCDB_ENABLE_CTL_HWEN_RDY,
   EFI_PM_LCDB_ENABLE_CTL_INVALID_TYPE,
}EFI_PM_LCDB_ENABLE_CTL_TYPE;

typedef enum
{
   EFI_PM_LCDB_PULLDOWN_CTL_ENABLE,
   EFI_PM_LCDB_PULLDOWN_CTL_STRENGHT,
   EFI_PM_LCDB_PULLDOWN_CTL_INVALID_TYPE,
}EFI_PM_LCDB_PULLDOWN_CTL_TYPE;


/*===========================================================================
FUNCTION DEFINITIONS
===========================================================================*/

/*EFI_PM_LCDB_SET_BOOST_OUTPUT_MILLI_VOLT*/
/** @ingroup
  @par Summary
  This API configures LCDB output boot voltage. If Symmetry is enabled then NCP will symmetrical to LDO in negative direction
  
  @param[in] PmicDeviceIndex     Primary: 0 Secondary: 1
  @param[in] LdoMv               +Ve LDO volt in mv. Range from 4000 to 6000 mV 
  @param[in] NcpMv               -Ve NCP volt in mv. Range from 4000 to 6000 mV  
  @param[in] SymmetryOk          TRUE to enable, will make NCP same as LDO. FALSE will set LDO and NCP independently
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */
typedef 
EFI_STATUS (EFIAPI *EFI_PM_LCDB_SET_OUTPUT)(
                 IN UINT32  PmicDeviceIndex, 
                 IN INT32   LdoMv, 
                 IN INT32   NcpMv, 
                 IN BOOLEAN SymmetryOk
                 );


/*EFI_PM_LCDB_SET_HWEN_DEBOUNCE*/
/** @ingroup
  @par Summary
  This API configures LCDB HWEN debounce
  
  @param[in] PmicDeviceIndex     Primary: 0 Secondary: 1
  @param[in] Enable             TRUE enables and FALSE disabled control
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */
typedef 
EFI_STATUS (EFIAPI *EFI_PM_LCDB_SET_HWEN_DEBOUNCE)(
                 IN UINT32 PmicDeviceIndex, 
                 IN BOOLEAN Enable
                 );

/*EFI_PM_LCDB_CONFIG_ENABLE_CTRL*/
/** @ingroup
  @par Summary
  This API configures LCDB enable control
  
  @param[in] PmicDeviceIndex     Primary: 0 Secondary: 1
  @param[in] CtlType            LCDB enable control type. Refer enum #EFI_PM_LCDB_ENABLE_CTL_TYPE for more
  @param[in] Enable             TRUE enables and FALSE disabled control
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */
typedef 
EFI_STATUS (EFIAPI *EFI_PM_LCDB_CONFIG_ENABLE_CTRL)(
                 IN UINT32                      PmicDeviceIndex, 
                 IN EFI_PM_LCDB_ENABLE_CTL_TYPE CtlType, 
                 IN BOOLEAN                     Enable
                 );

/*EFI_PM_LCDB_CONFIG_PULLDOWN_CTRL*/
/** @ingroup
  @par Summary
  This API configures LCDB pulldown control
  
  @param[in] PmicDeviceIndex     Primary: 0 Secondary: 1
  @param[in] CtlType            LCDB pulldown control type. Refer enum #EFI_PM_LCDB_PULLDOWN_CTL_TYPE for more
  @param[in] Ctrl               TRUE enables and FALSE disabled control
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */
typedef 
EFI_STATUS (EFIAPI *EFI_PM_LCDB_CONFIG_PULLDOWN_CTRL)(
                 IN UINT32                        PmicDeviceIndex, 
                 IN EFI_PM_LCDB_PULLDOWN_CTL_TYPE CtlType, 
                 IN BOOLEAN                       Ctrl
                 );

/*EFI_PM_LCDB_VREG_OK_STATUS*/
/** @ingroup
  @par Summary
  This API reads LCDB VREG_OK 
  
  @param[in] PmicDeviceIndex  Primary: 0 Secondary: 1
  @param[out]VregOk           Pointer to store VREG_OK status
 
  @return
  EFI_SUCCESS            -- Function completed successfully. 
  EFI_DEVICE_ERROR       -- Physical device reported an error
 */
typedef 
EFI_STATUS (EFIAPI *EFI_PM_LCDB_VREG_OK)(
                 IN  UINT32   PmicDeviceIndex, 
                 OUT BOOLEAN  *VregOk
                 );

/*===========================================================================
PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup @ingroup efi_pmicLcdb_protocol
@par Summary
Qualcomm PMIC LCDB Protocol interface.

@par Parameters
@inputprotoparams{pmic_lcdb_proto_params.tex} 
*/

struct _EFI_QCOM_PMIC_LCDB_PROTOCOL {
  UINT64                                  Revision;
  EFI_PM_LCDB_SET_OUTPUT                  SetOutput;
  EFI_PM_LCDB_SET_HWEN_DEBOUNCE           SetHwenDebounce;
  EFI_PM_LCDB_CONFIG_ENABLE_CTRL          ConfigEnableCtrl;
  EFI_PM_LCDB_CONFIG_PULLDOWN_CTRL        ConfigPulldownCtrl;
  EFI_PM_LCDB_VREG_OK                     VregOk;
};


#endif  /* __EFIPMICLCDB_H__ */
