/** @file CeLibNull.c
  
  Stub functions for CeLib

  Copyright (c) 2014, Qualcomm Technologies, Inc. All rights reserved.
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 12/01/14   ck      Added CeMLHashAtomic
 09/24/14   ck      Initial revision

=============================================================================*/


/*==========================================================================

                               INCLUDE FILES

===========================================================================*/
#include "com_dtypes.h"
#include "CeML.h"
#include "crypto/secrsa.h"
#include "crypto/secmath.h"

/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/ 

/**
 * @brief Initializes the memory for malloc
 *
 *
 * @return None
 *
 * @see
 *
 */
int crypto_env_init(void* ptr, uint32 Len)
{
  return 0;	
}


/**
 * @brief This function enable the crypto bam in loader image.
 *
 */

void crypto_bam_enable(void)
{
	
}

/**
   (PKCS #1 v1.5) PKCS1 pad then encrypt
  @param[in]  key          The RSA key to encrypt to
  @param[in]  padding_type Type of padding (CE_RSA_PAD_PKCS1_V1_5_ENC or CE_RSA_PAD_PKCS1_OAEP)
  @param[in]  padding_info The parameters for OAEP padding
  @param[in]  msg          The plaintext
  @param[in]  msglen       The length of the plaintext (octets)
  @param[out] cipher       The ciphertext
  @param[in,out] cipherlen The max size and resulting size of the ciphertext

  @return
   CE_SUCCESS     - Function executes successfully.
   CE_ERROR_NOT_SUPPORTED - the feature is not supported.
   CE_ERROR_INVALID_PACKET - invalid packet.
   CE_ERROR_BUFFER_OVERFLOW - not enough space for output.

  @dependencies
  None.
*/
int ce_rsa_encrypt
(
   CE_RSA_KEY              *key,
   CE_RSA_PADDING_TYPE     padding_type,
   void                    *padding_info,
   const unsigned char     *msg,
   int                     msglen,
   unsigned char           *cipher,
   int                     *cipherlen
)
{
	return E_SECMATH_NOT_SUPPORTED;
}


/**
 * @brief
 *  Read a zero terminated string into a big integer
 */
SECMATH_ERRNO_ET secmath_BIGINT_read_radix(BigInt *a, CONST CHAR8 *str,
                                           UINT32 radix)
{
	return E_SECMATH_NOT_SUPPORTED;
}



/**
 * @brief Intialize a cipher context
 *
 * @param _h       [in] Pointer to a pointer to the cipher
 *                 context structure
 * @param pAlgo    [in] Cipher algorithm type
 *
 * @return CeMLErrorType
 *
 * @see
 *
 */

CeMLErrorType
CeMLCipherInit      (CeMLCntxHandle       ** _h,
                     CeMLCipherAlgType    pAlgo)
{
  return CEML_ERROR_SUCCESS;
}

/**
 * @brief This functions sets the Cipher paramaters used by
 *        CeMLCipherData
 *
 * @param _h        [in] Pointer to cipher context handle
 * @param nParamID  [in] Cipher parameter id to set
 * @param pParam    [in] Pointer to parameter data
 * @param cParam    [in] Size of parameter data in bytes
 *
 * @return CeMLErrorType
 *
 * @see CeMLCipherData
 *
 */

CeMLErrorType
CeMLCipherSetParam    (CeMLCntxHandle       * _h,
                       CeMLCipherParamType  nParamID,
                       const void           *pParam,
                       UINT64               cParam )
{
  return CEML_ERROR_SUCCESS;
}

/**
 * @brief Deintialize a cipher context
 *
 * @param _h       [in] Pointer to a pointer to the cipher
 *                 context structure
 * @return CeMLErrorType
 *
 * @see
 *
 */

CeMLErrorType
CeMLCipherDeInit    (CeMLCntxHandle       ** _h)					 
{
  return CEML_ERROR_SUCCESS;
}

/**
 * @brief This function encrypts/decrypts the passed message
 *        using the specified algorithm.
 *
 * @param _h        [in] Pointer to cipher context handle
 * @param ioVecIn   [in] Pointer to input data. Input data
 *                  length must be a multiple of 16 bytes
 * @param ioVecOut  [in] Pointer to output data
 *
 * @return CeMLErrorType
 *
 * @see
 *
 */

CeMLErrorType
CeMLCipherData      (CeMLCntxHandle       * _h,
                     CEMLIovecListType    ioVecIn,
                     CEMLIovecListType    * ioVecOut)
{
  return CEML_ERROR_SUCCESS;
}

/**
 * @brief This function will create a Hmac message digest using
 *        the algorithm specified.
 *
 * @param key_ptr       [in]  Pointer to key
 * @param keylen        [in]  Length of input key in bytes
 * @param ioVecIn       [in]  Pointer to input data to hash
 * @param ioVecOut      [out] Pointer to output data
 * @param palgo         [in]  Algorithm type
 *
 * @return CeMLErrorType
 *
 * @see
 *
 */

CeMLErrorType
CeMLHmac            (UINT8                * key_ptr,
                     UINT64               keylen,
                     CEMLIovecListType    ioVecIn,
                     CEMLIovecListType    * ioVecOut,
                     CeMLHashAlgoType     pAlgo)		
{
  return CEML_ERROR_SUCCESS;
}					 