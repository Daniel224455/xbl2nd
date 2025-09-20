/** 
  @file  EFICryptoSelect.h
  @brief Crypto Select EFI Protocol for HashDxe use.
*/
/*=============================================================================
  Copyright (c) 2017 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/25/17   shl     initial version (03/28/18 jp ported from boot.xf.1.4)

=============================================================================*/

#ifndef __EFICRYPTOSELECT_H__
#define __EFICRYPTOSELECT_H__


/*===========================================================================
  INCLUDE FILES
===========================================================================*/


/*===========================================================================
  MACRO DECLARATIONS
===========================================================================*/
/** @addtogroup efi_crypto_select_constants 
@{ */
/**
  Protocol version.
*/
#define CRYPTO_SELECT_REVISION   0x0000000000010000
/** @} */ /* end_addtogroup efi_crypto_select_constants */

/*  Protocol GUID definition */
/** @ingroup efi_crypto_select_protocol */
#define EFI_QCOM_CRYPTO_SELECT_PROTOCOL_GUID \
    { 0x43ec305e, 0xe4cc, 0x4429, { 0x9b, 0x18, 0x8d, 0xab, 0x88, 0x34, 0x35, 0xba } }

/** @cond */
/*===========================================================================
  EXTERNAL VARIABLES
===========================================================================*/
/**
  External reference to the Crypto Select Protocol GUID.
*/
extern EFI_GUID gQcomCryptoSelectProtocolGuid;


/*===========================================================================
  TYPE DEFINITIONS
===========================================================================*/
/**
  Protocol declaration.
*/
typedef struct _EFI_QCOM_CRYPTO_SELECT_PROTOCOL   EFI_QCOM_CRYPTO_SELECT_PROTOCOL;

/** @endcond */

/** @addtogroup efi_crypto_select_data_types 
@{ */

/** 
  crypto source.
*/
typedef enum
{
  EFI_ARMV8_CRYPTO = 0, 
  EFI_HW_CRYPTO,
  EFI_CRYPTO_INVALID /**< Invalid source. */
}EFI_CRYPTO_TYPE;


/** @} */ /* end_addtogroup efi_crypto_select_data_types */


/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/

/* EFI_CRYPTO_TYPE_SET */ 
/** @ingroup efi_crypto_type_set
  @par Summary
  Sets the crypto type.

  @param[in]  CryptoType 

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the set.
*/
typedef
EFI_STATUS (EFIAPI *EFI_CRYPTO_TYPE_SET)(
  IN  EFI_QCOM_CRYPTO_SELECT_PROTOCOL      *This,
  IN  EFI_CRYPTO_TYPE                       CryptoType
);


/* EFI_CRYPTO_TYPE_GET */ 
/** @ingroup efi_crypto_type_get
  @par Summary
  gets the crypto type.

  @param[in]  *CryptotType 

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the get.
*/
typedef
EFI_STATUS (EFIAPI *EFI_CRYPTO_TYPE_GET)(
  IN  EFI_QCOM_CRYPTO_SELECT_PROTOCOL    *This,
  IN  EFI_CRYPTO_TYPE                    *CryptoType
);

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_crypto_select_protocol
  @par Summary
  Qualcomm Crypto Select Protocol interface.

*/
struct _EFI_QCOM_CRYPTO_SELECT_PROTOCOL {
  UINT64                   Revision;
  EFI_CRYPTO_TYPE_SET      CryptoTypeSet;
  EFI_CRYPTO_TYPE_GET      CryptoTypeGet;
};

#endif	/* __EFICRYPTOSELECT_H__ */
