#ifndef CIPHERLIB_H
#define CIPHERLIB_H
/*
                    CipherLib.h

  Cipher header file, interface between CipherDxe and library implemented in HashLib and HashLibArmv8

  Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.

**/

/*===========================================================================

                      EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/28/18   jp      Initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include <Include/com_dtypes.h>
#include <Uefi.h>
#include <Library/BaseLib.h>

typedef enum 
{
  CIPHER_LIB_ALG_AES128            = 0x0,
  CIPHER_LIB_ALG_AES256            = 0x1
} CIPHER_LIB_Alg_Type;

typedef enum 
{
  CIPHER_LIB_MODE_ECB              = 0x0,
  CIPHER_LIB_MODE_CBC              = 0x1,
  CIPHER_LIB_MODE_CTR              = 0x2,
  CIPHER_LIB_MODE_XTS              = 0x3,
  CIPHER_LIB_MODE_CCM              = 0x4,
  CIPHER_LIB_MODE_CMAC             = 0x5,
  CIPHER_LIB_MODE_CTS              = 0x6
} CIPHER_LIB_Mode_Type;

typedef enum 
{
  CIPHER_LIB_ENCRYPT               = 0x00, 
  CIPHER_LIB_DECRYPT               = 0x01 
} CIPHER_LIB_Dir_Type;

typedef enum 
{
  CIPHER_LIB_ENGINE_ARMV8          = 0x00,
  CIPHER_LIB_ENGINE_CE_HW          = 0x01
} CIPHER_LIB_Engine_Type;

typedef enum {
  CIPHER_LIB_OEM_HW_KEY            = 0,      /** OEM HW KEY - default                                 */
  CIPHER_LIB_QC_HW_KEY,                      /** QCOM HW KEY                                          */

  CIPHER_LIB_MAX_HW_KEYS
}CIPHER_LIB_HW_Key_Type;

typedef enum {
  CIPHER_LIB_NO_PAD = 0,          /** No padding - default                                            */
  CIPHER_LIB_ZERO,                /** Padding with Zero                                               */
  CIPHER_LIB_ISO10126,            /** Padding with random number                                      */
  CIPHER_LIB_PKCS7,               /** Padding with padding length                                     */

  CIPHER_LIB_MAX_PAD_COUNT
}CIPHER_LIB_Pad_Type;

typedef enum 
{ 
  CIPHER_LIB_PARAM_DIRECTION       = 0x01,
  CIPHER_LIB_PARAM_KEY             = 0x02,
  CIPHER_LIB_PARAM_IV              = 0x03,
  CIPHER_LIB_PARAM_MODE            = 0x04,
  CIPHER_LIB_PARAM_NONCE           = 0x05,
  CIPHER_LIB_PARAM_XTS_KEY         = 0x06,
  CIPHER_LIB_PARAM_XTS_DU_SIZE     = 0x07,
  CIPHER_LIB_PARAM_CCM_PAYLOAD_LEN = 0x08,
  CIPHER_LIB_PARAM_CCM_MAC_LEN     = 0x09,
  CIPHER_LIB_PARAM_CCM_HDR_LEN     = 0x10,
  CIPHER_LIB_PARAM_HWKEY_TYPR      = 0x11,
  CIPHER_LIB_PARAM_CNTR_SIZE       = 0x12,
  CIPHER_LIB_PARAM_PAD_TYPE        = 0x13
} CIPHER_LIB_Param_Type;

typedef struct 
{
  void *                    pClientCtxt;
  CIPHER_LIB_Alg_Type       Algo;
  CIPHER_LIB_Mode_Type      Mode;
  CIPHER_LIB_Dir_Type       Dir; 
  CIPHER_LIB_Engine_Type    Engine;
} CIPHER_LIB_Ctx;

EFI_STATUS CipherInitCeHw
(
  CIPHER_LIB_Ctx *pCtx
);

EFI_STATUS CipherDeInitCeHw
(
  CIPHER_LIB_Ctx *pCtx
);

EFI_STATUS CipherSetParamCeHw
(
  CIPHER_LIB_Ctx          *pCtx,
  CIPHER_LIB_Param_Type   paramType,
  UINT8                   *pParam, 
  UINTN                   paramSize
);

EFI_STATUS CipherDataCeHw
(
  CIPHER_LIB_Ctx      *pCtx,
  CONST UINT8         *pDataIn, 
  UINTN               dataInSize,
  UINT8               *pDataOut, 
  UINTN               *Cipher_vld_osz
);

EFI_STATUS CipherInitArmv8
(
  CIPHER_LIB_Ctx *pCtx
);

EFI_STATUS CipherDeInitArmv8
(
  CIPHER_LIB_Ctx *pCtx
);

EFI_STATUS CipherSetParamArmv8
(
  CIPHER_LIB_Ctx          *pCtx,
  CIPHER_LIB_Param_Type   paramType,
  UINT8                   *pParam, 
  UINTN                   paramSize
);

EFI_STATUS CipherDataArmv8
(
  CIPHER_LIB_Ctx      *pCtx,
  CONST UINT8         *pDataIn, 
  UINTN               dataInSize,
  UINT8               *pDataOut, 
  UINTN               *Cipher_vld_osz
);

#endif // CIPHERLIB_H
