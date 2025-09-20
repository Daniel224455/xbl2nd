/** 
  @file  EFINOCBWConfig.h
  @brief NOCBW EFI protocol interface.
*/
/*=============================================================================
  Copyright (c) 2020 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who       what, where, why
 --------   ---       -----------------------------------------------------------

=============================================================================*/

#ifndef __EFINOCBW_H__
#define __EFINOCBW_H__
 
/** @cond */
typedef struct _EFI_NOCBW_PROTOCOL EFI_NOCBW_PROTOCOL;
/** @endcond */

/** @addtogroup efi_nocbw_constants 
@{ */
/**
  Protocol version. 
*/
#define EFI_NOCBW_PROTOCOL_REVISION 0x0000000000030000

/** @} */ /* end_addtogroup efi_nocbw_constants */

/*  Protocol GUID definition */
/** @ingroup efi_nocbw_protocol */

#define EFI_NOCBW_PROTOCOL_GUID \
   { 0x88527AF0, 0x4594, 0x404D, { 0x95, 0x38, 0xD4, 0x48, 0xC0, 0xE3, 0x30, 0x33 } } 

  
/** @cond */
/**
  External reference to the NOCBW Protocol GUID defined 
  in the .dec file. 
*/
extern EFI_GUID gEfiNOCBWProtocolGuid;
/** @endcond */


/** @} */ /* end_addtogroup efi_nocbw_data_types */

/*==============================================================================

                             API IMPLEMENTATION

==============================================================================*/ 


/* ============================================================================
**  Function : EFI_RemoveNOCBW
** ============================================================================
*/
/** @ingroup EFI_RemoveNOCBW
  @par Summary
  Removes NOC BW vote done by this DXE
    
  @param[in]   This            Pointer to the EFI_NOCBW_PROTOCOL instance.
                        
  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_NOT_READY      -- No bus bandwidth vote placed earlier, Handle uninitialized.
*/
typedef EFI_STATUS (EFIAPI *EFI_Remove_NOCBW)( IN EFI_NOCBW_PROTOCOL *This );

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_nocbw_protocol 
  @par Summary
  NOC BW Info Protocol interface.

  @par Parameters
*/
struct _EFI_NOCBW_PROTOCOL {
   UINT64                                Revision;
   EFI_Remove_NOCBW                      RemoveNOCBW;
}; 

#endif /* __EFINOCBW_H__ */

