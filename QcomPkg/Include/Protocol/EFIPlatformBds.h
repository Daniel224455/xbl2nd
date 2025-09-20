/**
  @file  EFIPlatformBds.h
  @brief Platform BDS EFI protocol interface.
*/
/*=============================================================================
  Copyright (c) 2018,2019 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/


/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/28/19   pa       Added Capsule Update
 03/22/19   kpa      Added ProcessHotkeys, PostFwConfigSec
 11/16/18   kpa      Initial Draft

=============================================================================*/

#ifndef __EFIPLATFORMBDS_H__
#define __EFIPLATFORMBDS_H__

/** @cond */
typedef struct _EFI_PLATFORMBDS_PROTOCOL EFI_PLATFORMBDS_PROTOCOL;
/** @endcond */

/** @addtogroup efi_platformbds_constants
@{ */
/**
  Protocol version.
*/
#define EFI_PLATFORMBDS_PROTOCOL_REVISION 0x0000000000010003
/** @} */ /* end_addtogroup efi_platformbds_constants */

/*  Protocol GUID definition */
/** @ingroup efi_platformbds_protocol */
/*{D874D61A-4B87-7608-A00F-58ADD7052530}*/
#define EFI_PLATFORMBDS_PROTOCOL_GUID \
  { 0xD874D61A, 0x4B87, 0x7608, { 0xA0, 0x0F, 0x58, 0xAD, 0xD7, 0x05, 0x25, 0x30 } }

 

/** @cond */
/**
  External reference to the Platform BDS Protocol GUID defined
  in the .dec file.
*/
extern EFI_GUID gEfiPlatformBdsProtocolGuid;
/** @endcond */


/*==============================================================================

                             API IMPLEMENTATION

==============================================================================*/

/* ============================================================================
**  Function : EFI_PreConsoleInit
** ============================================================================
*/
/** @ingroup efi_PreConsoleInit
  @par Summary
  Platform Bds Pre Console stage initializations

  @param[in]      This               Pointer to the EFI_PLATFORMBDS_PROTOCOL instance.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_NOT_READY         -- Protocol Dependencies not satisfied. \n
*/
typedef
EFI_STATUS
(EFIAPI *EFI_PRE_CONSOLE_INIT)(
   IN EFI_PLATFORMBDS_PROTOCOL       *This
   );

   
/* ============================================================================
**  Function : EFI_PostConsoleInit
** ============================================================================
*/
/** @ingroup efi_PostConsoleInit
  @par Summary
  Platform Bds Post Console stage initializations

  @param[in]      This               Pointer to the EFI_PLATFORMBDS_PROTOCOL instance.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_NOT_READY         -- Protocol Dependencies not satisfied. \n
*/
typedef
EFI_STATUS
(EFIAPI *EFI_POST_CONSOLE_INIT)(
   IN EFI_PLATFORMBDS_PROTOCOL       *This
   );

/* ============================================================================
**  Function : EFI_SetPlatformSecurity
** ============================================================================
*/
/** @ingroup efi_SetPlatformSecurity
  @par Summary
  Platform Bds security stage initializations

  @param[in]      This               Pointer to the EFI_PLATFORMBDS_PROTOCOL instance.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_NOT_READY         -- Protocol Dependencies not satisfied. \n
*/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_PLATFORM_SECURITY)(
   IN EFI_PLATFORMBDS_PROTOCOL       *This
   );

/* ============================================================================
**  Function : EFI_FwProvision
** ============================================================================
*/
/** @ingroup efi_FwProvision
  @par Summary
  Platform Bds set Firmware Provisions

  @param[in]      This               Pointer to the EFI_PLATFORMBDS_PROTOCOL instance.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_NOT_READY         -- Protocol Dependencies not satisfied. \n
*/
typedef
EFI_STATUS
(EFIAPI *EFI_FIRMWARE_PROVISION)(
   IN EFI_PLATFORMBDS_PROTOCOL       *This
   );

/* ============================================================================
**  Function : EFI_ProcessFwCapsules
** ============================================================================
*/
/** @ingroup EFI_ProcessFwCapsules
  @par Summary
  Perform capsule update

  @param[in]      This               Pointer to the EFI_PLATFORMBDS_PROTOCOL instance.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_NOT_READY         -- Protocol Dependencies not satisfied. \n
*/
typedef
EFI_STATUS
(EFIAPI *EFI_PROCESS_FW_CAPSULES)(
   IN EFI_PLATFORMBDS_PROTOCOL       *This
   );
/* ============================================================================
**  Function : EFI_ProcessHotkeys
** ============================================================================
*/
/** @ingroup efi_ProcessHotkeys
  @par Summary
  Platform Bds api to handle hotkey detections

  @param[in]      This        Pointer to the EFI_PLATFORMBDS_PROTOCOL instance.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_NOT_READY         -- Protocol Dependencies not satisfied. \n
*/
typedef
EFI_STATUS
(EFIAPI *EFI_PROCESS_HOTKEYS)(
   IN EFI_PLATFORMBDS_PROTOCOL       *This
   );

/* ============================================================================
**  Function : EFI_PostFwConfigSec
** ============================================================================
*/
/** @ingroup efi_PostFwConfigSec
  @par Summary
  Platform Bds api for security initializations after Firmware provisions

  @param[in]      This        Pointer to the EFI_PLATFORMBDS_PROTOCOL instance.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_NOT_READY         -- Protocol Dependencies not satisfied. \n
*/
typedef
EFI_STATUS
(EFIAPI *EFI_POST_FW_CFG_SEC)(
   IN EFI_PLATFORMBDS_PROTOCOL       *This
   );
   
   
/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_platformbds_protocol
  @par Summary
  Qualcomm Environment Protocol interface.

  @par Parameters
*/
struct _EFI_PLATFORMBDS_PROTOCOL {
  UINT64                      Revision;
  EFI_PRE_CONSOLE_INIT        PreConsoleInit;   
  EFI_POST_CONSOLE_INIT       PostConsoleInit;   
  EFI_SET_PLATFORM_SECURITY   SetPlatformSecurity;   
  EFI_FIRMWARE_PROVISION      FwProvision;   
  EFI_PROCESS_HOTKEYS         ProcessHotkeys; 
  EFI_POST_FW_CFG_SEC         PostFwConfigSec;
  EFI_PROCESS_FW_CAPSULES     ProcessFwCapsules; 
};
#endif /* __EFIPLATFORMBDS_H__ */

