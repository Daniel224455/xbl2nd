/** @file FwCrypto.c

  Cryptography functions used for firmware update

  Copyright (c) 2012,2013,2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2017/09/06     gparam      Updates to SSD key provisioning to use SCM APIs
 2017/09/06     gparam      Updates to OPM key provisioning to use SCM APIs
 2017/01/27     ai          Fix use of retail
 2013/10/03     na          Close security hole
 2013/10/02     na          Bypass encryption call based on cfg value
 2012/10/04     mic         Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/QcomTargetLib.h>
#include <Library/FuseControlLib.h>

#include <Protocol/EFIToolSupport.h>
#include <Protocol/EFIEncryption.h>
#include <Protocol/EFIScm.h>

#include <Include/scm_sip_interface.h>

#include "FwCrypto.h"

#define SSD_SIGNATURE_LEN                   8
#define WAP_TRUSTLET_NAME                   "qcom.tz.winsecapp"

#define TZEXEC_SVC_OPM_ID                   0x0020000
#define SERVICE_OPM_CREATE_CMD(x)           (TZEXEC_SVC_OPM_ID | x)

#define OPM_RSA_PRIVATE_KEY_ENCRYPT         SERVICE_OPM_CREATE_CMD(1)
#define OPM_RSA_PRIVATE_KEY_DECRYPT         SERVICE_OPM_CREATE_CMD(2)
#define OPM_MSG_DECRYPT                     SERVICE_OPM_CREATE_CMD(3)
#define OPM_CONSTRUCT_AES_KEY               SERVICE_OPM_CREATE_CMD(4)

/* Status codes */
typedef short                               INT16;
typedef INT16                               CRYPT_RESULT;

#define OPM_RETURN_CRYPTO_SUCCESS           (0)
#define OPM_RETURN_AES_ENCRYPT              (1)
#define OPM_RETURN_AES_DECRYPT              (-1)
#define OPM_RETURN_HMAC_CALC                (-2)
#define OPM_RETURN_HMAC_FAIL                (-3)
#define OPM_RETURN_ADDRESS_CHECK_FAIL       (-4)
#define OPM_RETURN_RSA_DECRYPT              (-5)
#define OPM_RETURN_OVERFLOW                 (-6)
#define OPM_RETURN_SSD_DECRYPT              (-7)
#define OPM_RETURN_RSA_PAD_UNSUPPORTED      (-8)
#define OPM_RETURN_RSA_HASH_UNSUPPORTED     (-9)
#define OPM_RETURN_MEM_ALLOC_FAILURE        (-10)
#define OPM_RETURN_UNSUPPORTED              (-11)
#define OPM_RETURN_PARSE_FAIL               (-12)

typedef struct _OPM_SVC_REQ
{
 /* Command ID */
  UINT32            commandId;
 /* Length */
  UINT32            Length;
 /* Offset of input buffer*/
  UINT32             inBufferOffset;
 /* Input Buffer Size*/
  UINT32             inBufferSize;
 /* Size for input buffer, output buffer size = input buffer size +/- hdr size*/
} __attribute__ ((packed)) OPM_SVC_REQ, *POPM_SVC_REQ;

/*------------------------------------------------------------
-------------------------------------------------------------*/

typedef struct _OPM_SVC_RSP
{
 /* status indicate ERROR */
  INT32            status;
} __attribute__ ((packed)) OPM_SVC_RSP, *POPM_SVC_RSP;

/*------------------------------------------------------------
-------------------------------------------------------------*/

const char* SsdMagicBytes = "tzssdimg";

/**
 * Function to check if buffer is SSD encrypted
 *
 * @param opmStatus       - Error returned by OPM encrypt request
 * @return : EFI_STATUS equivalent of given opmStatus
 */
EFI_STATUS
OPMErrorToEFIError(
  IN CRYPT_RESULT opmStatus
  )
{
  switch (opmStatus)
  {
    case OPM_RETURN_CRYPTO_SUCCESS:
      return EFI_SUCCESS;

    case OPM_RETURN_MEM_ALLOC_FAILURE:
      return EFI_OUT_OF_RESOURCES;

    case OPM_RETURN_UNSUPPORTED:
      return EFI_UNSUPPORTED;

    default:
      return EFI_DEVICE_ERROR;
  }
}

/**
 * Function to check if buffer is SSD encrypted
 *
 * @param buffer          - buffer
 * @param bufferSize      - buffer size
 * @param bEncrypted      - buffer is encrypted or not
 * @return : EFI_STATUS
 */
EFI_STATUS
FwCryptoSsdChecking(
  IN VOID     *buffer,
  IN UINTN    bufferSize,
  OUT BOOLEAN *bEncrypted
  )
{
  if((buffer == NULL) ||
     (bEncrypted == NULL) ||
     (bufferSize < SSD_SIGNATURE_LEN))
  {
    return EFI_INVALID_PARAMETER;
  }

  //Check the first 8-bytes for the TZ magic Number
  if( CompareMem((void*)SsdMagicBytes,buffer,SSD_SIGNATURE_LEN) == 0)
  {
    *bEncrypted = TRUE;
  }
  else
  {
     *bEncrypted = FALSE;
  }

  return EFI_SUCCESS;
}
/**
 * Function to for SSD decryption
 *
 * @param InBuf         - Input buffer
 * @param InBufSize     - Input buffer size
 * @param OutBuf        - Output buffer
 * @param OutBufSize    - Output buffer size
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwCryptoSsdDecrypt(
  IN  VOID     *InBuf,
  IN  UINTN    InBufSize,
  OUT VOID     **OutBuf,
  OUT UINTN    *OutBufSize
  )
{
  EFI_STATUS              Status          = EFI_SUCCESS;
  VOID                    *UnCachedInBuf  = NULL;
  VOID                    *UnCachedOutBuf = NULL;
  EFI_ENCRYPTION_PROTOCOL *Encryption     = NULL;
  UINT32                  BypassEncrypt    = 0;

  if((InBuf == NULL) ||
     (OutBuf == NULL) ||
     (OutBufSize == NULL)||
     (InBufSize < SSD_SIGNATURE_LEN)
     )
  {
    return EFI_INVALID_PARAMETER;
  }

  *OutBuf = NULL;
  UnCachedInBuf = UncachedAllocatePool(InBufSize);
  if(UnCachedInBuf == NULL)
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto FwCryptoSsdDecryptExit;
  }

  CopyMem(UnCachedInBuf,InBuf,InBufSize);

  if (!RETAIL)
  {
    Status = GetConfigValue ("DisableUpdateImageSecurity", &BypassEncrypt);
    if (Status != EFI_SUCCESS)
    {
      BypassEncrypt = 0;
    }
  }

  if (BypassEncrypt == 0)
  {
    Status = gBS->LocateProtocol(&gEfiEncryptionProtocolGuid,
                                 NULL,
                                 (VOID**)&Encryption);

    HANDLE_ERROR_LABEL(Status, gBS->LocateProtocol, FwCryptoSsdDecryptExit);

    /*Send buffer to Encrypt*/
    Status = Encryption->Decrypt(TZ_SSD,
                                 (UINT8*)UnCachedInBuf,
                                 InBufSize,
                                 (UINT8**)&UnCachedOutBuf,
                                 (UINT32*)OutBufSize);
    HANDLE_ERROR_LABEL(Status, Encryption->Encrypt, FwCryptoSsdDecryptExit);

    Status = gBS->AllocatePool(EfiBootServicesData, *OutBufSize ,(VOID**)OutBuf);
    HANDLE_ERROR_LABEL(Status, gBS->AllocatePool, FwCryptoSsdDecryptExit);

    CopyMem(*OutBuf,UnCachedOutBuf,*OutBufSize);
  }
  else
  {
    DEBUG ((EFI_D_ERROR, "Security Based Firmware Update Disabled!\n"));

    Status = gBS->AllocatePool(EfiBootServicesData, InBufSize ,(VOID**)OutBuf);
    HANDLE_ERROR_LABEL(Status, gBS->AllocatePool, FwCryptoSsdDecryptExit);
    *OutBufSize = InBufSize;

    CopyMem(*OutBuf,InBuf,*OutBufSize);
  }

  if(UnCachedInBuf != NULL)
  {
    UncachedFreePool(UnCachedInBuf);
    UnCachedInBuf = NULL;
  }

  return EFI_SUCCESS;

FwCryptoSsdDecryptExit:

  if(UnCachedInBuf != NULL)
  {
    UncachedFreePool(UnCachedInBuf);
    UnCachedInBuf = NULL;
  }

  if(*OutBuf != NULL)
  {
    gBS->FreePool(*OutBuf);
    *OutBuf = NULL;
  }
  return Status;
}

/**
 * Function to decrypt and encrypt OPM private key
 *
 * @param InBuf         - Input buffer
 * @param InBufSize     - Input buffer size
 * @param OutBuf        - Output buffer
 * @param OutBufSize    - Output buffer size
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FWCryptoOpmPrivKey(
  IN  VOID     *InBuf,
  IN  UINTN    InBufSize,
  OUT VOID     **OutBuf,
  OUT UINTN    *OutBufSize
  )
{
  EFI_STATUS                     Status              = EFI_SUCCESS;
  QCOM_SCM_PROTOCOL             *pQcomScmProtocol    = NULL;
  EFI_QCOM_TOOLSUPPORT_PROTOCOL *ToolSupportProtocol = NULL;
  UINT8                         *pOPMPrivKey         = NULL;
  UINT8                         *pHandShakeKey       = NULL;
  UINT8                          HandShakeKey_len8   = 0;
  UINTN                          HandShakeKey_len32  = 0;
  POPM_SVC_REQ                   pReq                = NULL;
  POPM_SVC_RSP                   pRsp                = NULL;
  VOID                          *pOPMKeyProvReq      = NULL;
  VOID                          *pHandShakeReq       = NULL;
  UINT32                         ReqLen              = 0;
  UINT32                         RspLen              = sizeof (OPM_SVC_RSP);
  UINT32                         AppId               = 0;

  if((InBuf == NULL) ||
     (OutBuf == NULL) ||
     (OutBufSize == NULL))

  {
    return EFI_INVALID_PARAMETER;
  }
  *OutBuf = NULL;

  /*locate encryption protocol*/
  Status = gBS->LocateProtocol(&gQcomScmProtocolGuid,
                               NULL,
                               (VOID**)&pQcomScmProtocol);

  HANDLE_ERROR_LABEL(Status,gBS->LocateProtocol(gQcomScmProtocolGuid),FWCryptoOpmPrivKeyExit);


  /*locate tool support protocol*/
  Status = gBS->LocateProtocol(&gQcomToolSupportProtocolGuid,
                               NULL,
                               (VOID**)&ToolSupportProtocol);
  HANDLE_ERROR_LABEL(Status,gBS->LocateProtocol(gQcomToolSupportProtocolGuid),FWCryptoOpmPrivKeyExit);

  /*Query WinsecApp ID*/
  Status = pQcomScmProtocol->ScmSendCommand(pQcomScmProtocol,
                                            APP_QUERY_APPID_CMD,
                                            &AppId,
                                            WAP_TRUSTLET_NAME,
                                            sizeof(WAP_TRUSTLET_NAME),
                                            NULL,
                                            0);
  HANDLE_ERROR_LABEL(Status,ScmSendCommand(APP_QUERY_APPID_CMD),FWCryptoOpmPrivKeyExit);

  /*allocate buffer for response*/
  Status = gBS->AllocatePool(EfiBootServicesData,RspLen,(VOID**)&pRsp);
  HANDLE_ERROR_LABEL(Status,gBS->AllocatePool(pRsp),FWCryptoOpmPrivKeyExit);

  ZeroMem(pRsp,RspLen);

  /*handshake before encryption*/
  /*get handshake key length*/
  Status = ToolSupportProtocol->GetDataLenProtocol(ToolSupportProtocol,
                                                   &HandShakeKey_len8);
  HANDLE_ERROR_LABEL(Status,ToolSupportProtocol->GetDataLenProtocol,FWCryptoOpmPrivKeyExit);

  HandShakeKey_len32 = (UINTN)HandShakeKey_len8;

  ReqLen = sizeof(OPM_SVC_REQ) + HandShakeKey_len32;

  /*allocate buffer for handshake request*/
  Status = gBS->AllocatePool(EfiBootServicesData,ReqLen,&pHandShakeReq);
  HANDLE_ERROR_LABEL(Status,gBS->AllocatePool(pHandShakeReq),FWCryptoOpmPrivKeyExit);

  ZeroMem(pHandShakeReq,ReqLen);

  pReq = (POPM_SVC_REQ)pHandShakeReq;
  pHandShakeKey = (UINT8*)(pReq + 1);

  /*get handshake key*/
  Status = ToolSupportProtocol->GetDataProtocol(ToolSupportProtocol,pHandShakeKey);
  HANDLE_ERROR_LABEL(Status,ToolSupportProtocol->GetDataProtocol,FWCryptoOpmPrivKeyExit);

  /*Setup the key provision command parameters*/
  pReq->commandId = OPM_CONSTRUCT_AES_KEY;
  pReq->Length = ReqLen;
  pReq->inBufferOffset = sizeof(OPM_SVC_REQ);
  pReq->inBufferSize = HandShakeKey_len32;

  /*perform handshake*/
  Status = pQcomScmProtocol->ScmSendCommand(pQcomScmProtocol,
                                            APP_SEND_DATA_CMD,
                                            &AppId,
                                            pHandShakeReq,
                                            ReqLen,
                                            pRsp,
                                            RspLen);
  HANDLE_ERROR_LABEL(Status,ScmSendCommand(APP_SEND_DATA_CMD, OPM_CONSTRUCT_AES_KEY),FWCryptoOpmPrivKeyExit);

  /*Validate handshake response*/
  if (pRsp->status != OPM_RETURN_CRYPTO_SUCCESS)
  {
    Status = OPMErrorToEFIError(pRsp->status);
    DEBUG((EFI_D_ERROR,"ScmSendCommand(APP_SEND_DATA_CMD, OPM_CONSTRUCT_AES_KEY) returned tzstatus 0x%x. Returning %r\n", pRsp->status, Status));
    goto FWCryptoOpmPrivKeyExit;
  }

  ReqLen = sizeof(OPM_SVC_REQ) + InBufSize;

  /*allocate buffer for encrypt request*/
  Status = gBS->AllocatePool(EfiBootServicesData,ReqLen,&pOPMKeyProvReq);
  HANDLE_ERROR_LABEL(Status,gBS->AllocatePool(pOPMKeyProvReq),FWCryptoOpmPrivKeyExit);

  ZeroMem(pOPMKeyProvReq,ReqLen);

  pReq = (POPM_SVC_REQ)pOPMKeyProvReq;

  /*Copy key to request buffer*/
  pOPMPrivKey = (UINT8*)(pReq + 1);
  CopyMem(pOPMPrivKey,InBuf,InBufSize);

  ZeroMem(pRsp,RspLen);

  /*Setup the key provision command parameters*/
  pReq->commandId = OPM_RSA_PRIVATE_KEY_ENCRYPT;
  pReq->Length = ReqLen;
  pReq->inBufferOffset = sizeof(OPM_SVC_REQ);
  pReq->inBufferSize = InBufSize;

  /*Request encryption*/
  Status = pQcomScmProtocol->ScmSendCommand(pQcomScmProtocol,
                                            APP_SEND_DATA_CMD,
                                            &AppId,
                                            pOPMKeyProvReq,
                                            ReqLen,
                                            pRsp,
                                            RspLen);
  HANDLE_ERROR_LABEL(Status,ScmSendCommand(APP_SEND_DATA_CMD, OPM_RSA_PRIVATE_KEY_ENCRYPT),FWCryptoOpmPrivKeyExit);

  /*Validate encryption response*/
  if (pRsp->status != OPM_RETURN_CRYPTO_SUCCESS)
  {
    Status = OPMErrorToEFIError(pRsp->status);
    DEBUG((EFI_D_ERROR,"ScmSendCommand(APP_SEND_DATA_CMD, OPM_RSA_PRIVATE_KEY_ENCRYPT) returned tzstatus 0x%x. Returning %r\n", pRsp->status, Status));
    goto FWCryptoOpmPrivKeyExit;
  }

  /*Copy the output of inplace encryption to output buffer*/
  Status = gBS->AllocatePool(EfiBootServicesData,InBufSize,OutBuf);
  HANDLE_ERROR_LABEL(Status,gBS->AllocatePool(OutBuf),FWCryptoOpmPrivKeyExit);

  CopyMem(*OutBuf,pOPMPrivKey,pReq->inBufferSize);
  *OutBufSize = pReq->inBufferSize;

FWCryptoOpmPrivKeyExit:

  if(pHandShakeReq != NULL)
  {
    gBS->FreePool(pHandShakeReq);
    pHandShakeReq = NULL;
  }

  if(pOPMKeyProvReq != NULL)
  {
    gBS->FreePool(pOPMKeyProvReq);
    pOPMKeyProvReq = NULL;
  }

  if(pRsp != NULL)
  {
    gBS->FreePool(pRsp);
    pRsp = NULL;
  }

  if((EFI_ERROR(Status)) &&
     (*OutBuf != NULL))
  {
    gBS->FreePool(*OutBuf);
    *OutBuf = NULL;
  }

  return Status;
}

/**
 * Function to encrypt SSD key
 *
 * @param InBuf           - Input buffer
 * @param InBufSize     - Input buffer size
 * @param OutBuf         - Output buffer
 * @param OutBufSize   - Output buffer size
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
FwCryptoMsmEncrypt(
  IN  VOID     *InBuf,
  IN  UINTN    InBufSize,
  OUT VOID     **OutBuf,
  OUT UINTN    *OutBufSize
  )
{
  EFI_STATUS               Status             = EFI_SUCCESS;
  VOID                    *pSSDKeyBuffer      = NULL;
  VOID                   **ppSSDKeyBuffer     = NULL;
  UINTN                   *pSSDKeyBufferSize  = NULL;
  QCOM_SCM_PROTOCOL       *pQcomScmProtocol   = NULL;
  UINT64                   Parameters[SCM_MAX_NUM_PARAMETERS] = {0};
  UINT64                   Results[SCM_MAX_NUM_RESULTS] = {0};
  tz_ssd_encrypt_keystore_req_t *SysCallReq = (tz_ssd_encrypt_keystore_req_t*)Parameters;

  if((InBuf == NULL) ||
     (OutBuf == NULL) ||
     (OutBufSize == NULL)||
     (InBufSize < SSD_SIGNATURE_LEN)
     )
  {
    return EFI_INVALID_PARAMETER;
  }

  *OutBuf = NULL;

  pSSDKeyBufferSize = UncachedAllocateAlignedPool(sizeof(*pSSDKeyBufferSize),EFI_PAGE_SIZE);
  if(pSSDKeyBufferSize == NULL)
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto FwCryptoMsmEncryptExit;
  }

  *pSSDKeyBufferSize = InBufSize;

  pSSDKeyBuffer = UncachedAllocateAlignedPool(*pSSDKeyBufferSize,EFI_PAGE_SIZE);
  if(pSSDKeyBuffer == NULL)
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto FwCryptoMsmEncryptExit;
  }

  ppSSDKeyBuffer = UncachedAllocateAlignedPool(sizeof(*ppSSDKeyBuffer),EFI_PAGE_SIZE);
  if(ppSSDKeyBuffer == NULL)
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto FwCryptoMsmEncryptExit;
  }

  CopyMem(pSSDKeyBuffer,InBuf,*pSSDKeyBufferSize);

  /* SCM call updates our pointer, so use a copy for passing it to
  SCM call so that the original pointer can be used later for free. */
  *ppSSDKeyBuffer = pSSDKeyBuffer;

  SysCallReq->keystore_ptr = (UINT64)ppSSDKeyBuffer;
  SysCallReq->keystore_len_ptr = (UINT64)pSSDKeyBufferSize;

  Status = gBS->LocateProtocol(&gQcomScmProtocolGuid,
                               NULL,
                               (VOID**)&pQcomScmProtocol);

  HANDLE_ERROR_LABEL(Status,gBS->LocateProtocol(gQcomScmProtocolGuid),FwCryptoMsmEncryptExit);

  /* Send buffer to Encrypt */
  /* Make a SCM Sys call */
  Status = pQcomScmProtocol->ScmSipSysCall (pQcomScmProtocol,
                                            TZ_SSD_ENCRYPT_KEY_STORE_ID,
                                            TZ_SSD_ENCRYPT_KEY_STORE_ID_PARAM_ID,
                                            Parameters,
                                            Results);
  HANDLE_ERROR_LABEL(Status,pQcomScmProtocol->ScmSipSysCall(TZ_SSD_ENCRYPT_KEY_STORE_ID),FwCryptoMsmEncryptExit);

  Status = gBS->AllocatePool(EfiBootServicesData,*pSSDKeyBufferSize,(VOID**)OutBuf);
  HANDLE_ERROR_LABEL(Status,gBS->AllocatePool,FwCryptoMsmEncryptExit);

  CopyMem(*OutBuf,*ppSSDKeyBuffer,*pSSDKeyBufferSize);
  *OutBufSize = *pSSDKeyBufferSize;

FwCryptoMsmEncryptExit:

  if(pSSDKeyBuffer != NULL)
  {
    UncachedFreeAlignedPool(pSSDKeyBuffer);
    pSSDKeyBuffer = NULL;
  }

  if(ppSSDKeyBuffer != NULL)
  {
    UncachedFreeAlignedPool(ppSSDKeyBuffer);
    ppSSDKeyBuffer = NULL;
  }

  if(pSSDKeyBufferSize != NULL)
  {
    UncachedFreeAlignedPool(pSSDKeyBufferSize);
    pSSDKeyBufferSize = NULL;
  }

  if((EFI_ERROR(Status)) &&
     (*OutBuf != NULL))
  {
    gBS->FreePool(*OutBuf);
    *OutBuf = NULL;
  }

  return Status;
}

