/** 
  @file  EFICipher.h
  @brief Cipher EFI Protocol for CipherDxe use.
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
 03/28/18   jp     initial version

=============================================================================*/

#ifndef __EFI_CIPHER_PROTOCOL_H__
#define __EFI_CIPHER_PROTOCOL_H__

typedef struct _EFI_CIPHER_PROTOCOL EFI_CIPHER_PROTOCOL;

/**
  This function must be called to initialize a cipher calculation to be subsequently performed using the
  _EFI_CIPHER_PROTOCOL functions efiCipherCtl(), efiCipherData() and efiCipherDeInit().

  @param[in]  This                Points to this instance of EFI_CIPHER_PROTOCOL.
  @param[in]  CipherAlgorithm     Points to the EFI_GUID which identifies the cipher algorithm to use.
                                  The Guids can be used here:
                                    gEfiCipherAlgorithmAES128Guid;
                                    gEfiCipherAlgorithmAES256Guid;
  @param[in]  CipherMode          Points to the EFI_GUID which identifies the cipher mode to use.
                                  The Guids can be used here:
                                    gEfiCipherModeECBGuid;
                                    gEfiCipherModeCBCGuid;
                                    gEfiCipherModeCTRGuid;
                                    gEfiCipherModeCTSGuid;
  @param[in]  CipherEngine        Points to the EFI_GUID which identifies the cipher engine.
                                  The Guids can be used here:
                                    gEfiCipherEngineArmv8Guid;
                                    gEfiCipherEngineCeHwGuid;
  @param[in]  CipherDir           Points to the EFI_GUID which identifies the cipher direction of encrypt or decrypt.
                                  The Guids can be used here:
                                    gEfiCipherDirEncryptGuid;
                                    gEfiCipherDirDecryptGuid;

  @retval EFI_SUCCESS             Initialized successfully.
  @retval EFI_INVALID_PARAMETER   Either of these parameters is Null.
  @retval EFI_UNSUPPORTED         The specified algorithm/mode/engine is not supported by this driver.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_CIPHER_CIPHER_INIT)(
  IN CONST EFI_CIPHER_PROTOCOL      *This,
  IN CONST EFI_GUID                 *CipherAlgorithm,
  IN CONST EFI_GUID                 *CipherMode,
  IN CONST EFI_GUID                 *CipherEngine,
  IN CONST EFI_GUID                 *CipherDir
  );

/**
  This function must be called before a cipher calculation to set necessary parameters.

  @param[in]  This                Points to this instance of EFI_CIPHER_PROTOCOL.
  @param[in]  CipherSetParamType  Points to the EFI_GUID which identifies the cipher parameter type to be set.
                                  The Guids can be used here:
                                    gEfiCipherSetParamDirGuid;
                                    gEfiCipherSetParamModeGuid;
                                    gEfiCipherSetParamKeyGuid;
                                    gEfiCipherSetParamIVGuid;

  @param[in]  CipherParamSubType  Points to the EFI_GUID which indicate the parameter value for dir or mode to be set.
                                  In cas of CipherSetParamType == gEfiCipherSetParamDirGuid, the Guids can be used here:
                                    gEfiCipherDirEncryptGuid;
                                    gEfiCipherDirDecryptGuid;
                                  In cas of CipherSetParamType == gEfiCipherSetParamModeGuid, the Guids can be used here:
                                    gEfiCipherModeECBGuid;
                                    gEfiCipherModeCBCGuid;
                                    gEfiCipherModeCTRGuid;
                                    gEfiCipherModeCTSGuid;
  @param[in]  CipherParam_ibuf    Points to the start of the input buffer for user to set key or IV.
  @param[in]  Cipher_isz          The size of the input buffer, in byte.
  @param[out] Cipher_osz          Points to the returned size, in byte.
  
  @retval EFI_SUCCESS             Set Cipher Parameter successfully.
  @retval EFI_INVALID_PARAMETER   Invalid Parameters.
  @retval EFI_UNSUPPORTED         The specified algorithm/mode/engine is not supported by this driver.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_CIPHER_CIPHER_CTL)(
  IN CONST EFI_CIPHER_PROTOCOL      *This,
  IN CONST EFI_GUID                 *CipherSetParamType,
  IN CONST EFI_GUID                 *CipherParamSubType,
  IN UINT8                          *CipherParam_ibuf,
  IN UINTN                          Cipher_isz,
  OUT UINTN                         *Cipher_osz
  );

/**
  Creates a cipher text for the specified message text.

  @param[in]  This            Points to this instance of EFI_CIPHER_PROTOCOL.
  @param[in]  Cipher_ibuf     Points to the buffer of the plain message.
  @param[in]  Cipher_isz      The size of plain message, in bytes.
  @param[in]  Cipher_obuf     Points to the buffer of the output cipher message.
  @param[in]  Cipher_osz      The size of the output buffer, in bytes.
  @param[out] Cipher_vld_osz  Points to the returned size for the cipher.
  
  @retval EFI_SUCCESS           Cipher returned successfully.
  @retval EFI_INVALID_PARAMETER Cipher_ibuf or Cipher_obuf is NULL or Cipher_isz and Cipher_osz is 0.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_CIPHER_CIPHER_DATA)(
  IN CONST EFI_CIPHER_PROTOCOL    *This,
  IN CONST UINT8                  *Cipher_ibuf,
  IN UINTN                        Cipher_isz,
  IN UINT8                        *Cipher_obuf,
  IN UINTN                        Cipher_osz,
  OUT UINTN                       *Cipher_vld_osz
  );

/**
  This function is called after a cipher calculation to free the resource.

  @param[in]  This                Points to this instance of EFI_CIPHER_PROTOCOL.
  
  @retval EFI_SUCCESS             Deinit successfully.
  @retval EFI_INVALID_PARAMETER   This is NULL.
**/

typedef
EFI_STATUS
(EFIAPI *EFI_CIPHER_CIPHER_DEINIT)(
  IN CONST EFI_CIPHER_PROTOCOL      *This
  );  


///
/// This protocol allows encrypt/decrypt a message.
///
struct _EFI_CIPHER_PROTOCOL {
  EFI_CIPHER_CIPHER_INIT            efiCipherInit;
  EFI_CIPHER_CIPHER_CTL             efiCipherCtl;
  EFI_CIPHER_CIPHER_DATA            efiCipherData;
  EFI_CIPHER_CIPHER_DEINIT          efiCipherDeInit;
};

extern EFI_GUID gEfiCipherProtocolGuid;

extern EFI_GUID gEfiCipherAlgorithmAES128Guid;
extern EFI_GUID gEfiCipherAlgorithmAES256Guid;
extern EFI_GUID gEfiCipherModeECBGuid;
extern EFI_GUID gEfiCipherModeCBCGuid;
extern EFI_GUID gEfiCipherModeCTRGuid;
extern EFI_GUID gEfiCipherModeCTSGuid;
extern EFI_GUID gEfiCipherEngineArmv8Guid;
extern EFI_GUID gEfiCipherEngineCeHwGuid;
extern EFI_GUID gEfiCipherDirEncryptGuid;
extern EFI_GUID gEfiCipherDirDecryptGuid;
extern EFI_GUID gEfiCipherSetParamDirGuid;
extern EFI_GUID gEfiCipherSetParamKeyGuid;
extern EFI_GUID gEfiCipherSetParamIVGuid;
extern EFI_GUID gEfiCipherSetParamModeGuid;

#endif
