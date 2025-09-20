/**
  @file  EFIQcomEnv.h
  @brief Qualcomm Environment EFI protocol interface.
*/
/*=============================================================================
  Copyright (c) 2018 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/


/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 10/03/18   kpa      Initial Draft

=============================================================================*/

#ifndef __EFIQCOMENV_H__
#define __EFIQCOMENV_H__

/** @cond */
typedef struct _EFI_QCOMENV_PROTOCOL EFI_QCOMENV_PROTOCOL;
/** @endcond */

/** @addtogroup efi_qcomenv_constants
@{ */
/**
  Protocol version.
*/
#define EFI_QCOMENV_PROTOCOL_REVISION 0x0000000000010001
/** @} */ /* end_addtogroup efi_qcomenv_constants */

/*  Protocol GUID definition */
/** @ingroup efi_qcomenv_protocol */
#define EFI_QCOMENV_PROTOCOL_GUID \
  { 0xDC241E53, 0x498B, 0x3B59, { 0xAD, 0x17, 0xC8, 0xB2, 0x9E, 0x6B, 0x36, 0xA8 } }  

 
/** @cond */
/**
  External reference to the Qualcomm Environment Protocol GUID defined
  in the .dec file.
*/
extern EFI_GUID gEfiQcomEnvProtocolGuid;
/** @endcond */


/*==============================================================================

                             API IMPLEMENTATION

==============================================================================*/
/* ============================================================================
**  Function : EFI_QcomEnv_SetDdrThroughputLow
** ============================================================================
*/
/** @ingroup efi_QcomEnv_SetDdrThroughputLow
  @par Summary
  Sets DDR CPU busbandwith npa vote for low freq

  @param[in]      This               Pointer to the EFI_QCOMENV_PROTOCOL instance.


  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_NOT_READY         -- Protocol Dependencies not satisfied. \n
  EFI_BUFFER_TOO_SMALL  -- Input ResetReasonString buffer is not long enough,
                           Length will be returned in ResetReasonStrLen
*/
typedef
EFI_STATUS
(EFIAPI *EFI_QCOMENV_SETDDRTHROUGHPUTLOW)(
   IN EFI_QCOMENV_PROTOCOL       *This
   );

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_qcomenv_protocol
  @par Summary
  Qualcomm Environment Protocol interface.

  @par Parameters
*/
struct _EFI_QCOMENV_PROTOCOL {
   UINT64                                  Revision;
   EFI_QCOMENV_SETDDRTHROUGHPUTLOW         SetDdrThroughputLow;
};

#endif /* __EFIQCOMENV_H__ */

