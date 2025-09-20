/*===========================================================================

                                                 S E C U R I T Y    S E R V I C E S

                                 S E C U R E   B O O T  A U T H E N T I C A T I O N

                                                            A P I    M O D U L E
FILE:  secboot.c

DESCRIPTION:
        This is the interface into the secure boot API. All callers call into
        this API to ensure the image they have loaded and are going to execute
        if valid (i.e no changes were made to the image after is was signed).
 
        Verification happens in the following way:
 
        Step 1: The secure boot fuses are read for the image to identify the
                root that is trusted - i.e images cert chain MUST chain
                to this root, along with other information such as the target
                serial number, oem id values to use for authenticating the image
 
        Step 2: The certificate chain is parsed in the X509 parser. The parser
                expects a chain of either 3 certificates or 2 certificates. The
                certificates must be in this order -> attest cert -> CA cert
                -> root cert (for a 3 cert chain) or attest cert -> self signed
                 root cert (for a 2 cert chain). In addition the OU fields from
                 the attestation certificate are parsed as they will be used later
                 on in the subsequent steps. The OU fields contain information like
                 the hash algorithm for the code signature (e.g SHA256), the msm
                 hw id and sw id values used during signing.
 
        Step 3: A check is done to ensure the image version is supported, to ensure
                we are not rolling back to an older buggy version of the image even
                if it was a valid signed image.
                In addition we check to ensure the image being authenticated is
                what the caller expects - i.e SBL has loaded what it thinks is
                the TZ image, and is now authenticating the image in order to
                run it. This helps ensure images are executed in the expected order
                The msm_hw_id OU field is checked with what was read from the fuses
                only if the serial number was not used for signing the image, as we
                don't enforce OEM's to populate the OU fields as serial number based
                checks would mean each attestation cert would have to be different
                for each chip, and it may not be feasible to enforce this on OEM's.
                The check is only done if OEM ID was used for signing the image.
 
        Step 4: The parsed certificates are hashed. The root certificate is hashed
                in it's entirety (with the signature), while the other certs are hashed
                without the signature. This is because the hash of the entire root
                certificate is compared with the root of trust that we read from
                ROM or fuses (in step 1) which contains the hash of the entire certificate,
                while the other certificates hash's are compared with the hash in their
                signatures.
 
        Step 5: The certificate chain is validated. The subject/issuer pairs
                are matched to ensure each cert was issued by the subsequent cert
                in the chain. The path contraints are also checked. Then the signatures
                and the root of trust is validated. i.e for a 3 certificate chain, the
                public key in the CA cert is used to decrypt the attestation
                cert signature. The hash of the attestation cert is then compared with the
                hash in the decrypted signature. The public key in the root cert is used
                to decrypt the CA cert signature. The hash of the attestation cert is
                compared with the hash in the decrypted signature. The hash of the
                root cert is compared to the root of trust read from Step 1. If everything
                matches the certificate chain is valid and the public key in the attestation
                certificate is trusted.
                Note: Certificate signatures are expected to be in PKCSv1.5 format.
 
        Step 6: The code signature is verified.  The public key in the attestation certificate
                is used to decrypt the code signature. The hash of the code signature is matched
                with the calculated hmac (see comments in the verify_image_signature function for
                the signature algorithm).
                Note: Code signatures are in a PKCSv1.5 like format (without the HashID) and
                are hashed with the sw id (sw version + sw type) and msm hw id (jtag id
                + oem id/serial number).
 
        Step 7: A check is done to see if the attestation certificate is a debug certificate
                and is valid for this chip. If so the debug enabled flag is returned
                so that the caller can act on it by enable debugging (jtag access). Typically
                debug certs are only used when OEM's want to do RMA (a phone is returned to
                them due to some issue and they want to debug it). If the secure boot and jtag
                disable fuses are blown, the only way they can debug this is if the jtag override
                fuses are written to re-enable jtag access.
                
 
DEPENDENCIES/ASSUMPTIONS
        

EXTERNALIZED FUNCTIONS

        Details of the functions are in the function comments and
        for help and overall design notes, read the secboot.h file


    Copyright (c)2004, 2006, 2008-2015 by Qualcomm, Technologies, Inc. All Rights Reserved.
===========================================================================*/

/*=========================================================================

                                                        EDIT HISTORY FOR FILE
    This section contains comments describing changes made to the module.
    Notice that changes are listed in reverse chronological order.


when       who            what, where, why
--------   ----           --------------------------------------------------- 
03/24/15   hw             Add Dynamic Binding feature support
08/05/14   hw             Add QPSI code review fix and 64bit support
05/28/14   hw             Added RoT support
08/28/13   hw             Added retail unlock feature code
03/06/13   vg             Added a funcion to take the fuse values.
06/05/12   HW             Additional software type check for Apps PBL in 
                          secboot_authenticate.
06/01/12   HW             Hexagon compiler was optimizing assignment to use 
                          standard library memcpy. Replace the structure assignment in
                          secboot_init with explicitly MEMCPY call to avoid this compiler 
                          optimization.
02/15/12   sm             Call CeMLInit and CeMLDeInit in secboot as this is 
                          the expectation of CeML API - caller has to call these
                          functions first before invoking CeMLHashInit() etc
02/10/12   sm             Reverted back to dynamic size check for context type 
01/28/12   sm             Fixed lint warnings 
07/22/11   sm             Reduce ZI/RW area, put cert hashes on the stack     
06/21/11   sm             Updated verification of certs hw_id field against fuse     
                          generated value so it checked only when oem id (i.e not
                          serial number) is used for authentication
05/10/11   sm             Removed check of certificate's hw_id field and moved     
                          it to caller of secboot as check needs to happen only
                          if auth use serial number fuse is not blown
04/12/10   sm             Added support for flexible chain size & sha256 cert
03/08/10   sm             Optimization to reduce one RSA operation for QC chain
09/10/09   sm             Changes to support regional security
08/03/08   ANB            Initial T-Bird port.
05/02/06   AMW            Initial SC2X port.
08/25/04   SJD            Initial revision.  Original SW from the security team.
6/06/04    John Alayari   1. Modified the error code for the public
                             functions to use errno_enum_type
                          2. Cleaned up functions to return at end of
                             functions.
                          3. Added comments and headings to functions
                             and corrected some indentations to comply
                             with ASW Security Services Coding Guideline.
                          4. resturctured the code and made function call
                             from repeated blocks of code.
                          5. In secboot_verify_code_signature function;
                             fixed the bug to copy the correct  SW ID
                             and HW ID bytes to the work_buff during
                             hashing before XOR operation.

===========================================================================*/

/*==========================================================================

                                                             INCLUDE FILES

===========================================================================*/
#include "SecBootI.h"
#include "SecBootx509.h"
#include "SecMath.h"
#include "boot_util.h"
#include "secboot_hw.h"
/*=========================================================================

                                        FORWARD DECLARATIONS FOR LOCAL FUNCTIONS

=========================================================================*/
#define SECBOOT_VERSION 0x00600100 //0x00RRMMmm - RR:secboot release version, MM:major revision, mm:minor revision

static secboot_error_type secboot_hash_certificates
(
  secboot_context_type*              context_ptr,
  const uint8*                       x509_cert_chain_ptr,
  const pbl_secx509_cert_list_type*  x509_cert_list_ptr
);

static secboot_error_type secboot_verify_cert_chain
(
  secboot_context_type*         context_ptr,
  pbl_secx509_cert_list_type*   x509_cert_list_ptr,
  const uint8*                  root_of_trust_hash_ptr
);

static secboot_error_type secboot_verify_image_signature
(
  secboot_context_type*            context_ptr,
  const pbl_secx509_cert_type*     attestation_cert_info_ptr,
  const secboot_image_info_type*   image_info_ptr,
  uint64                           msm_hw_id,
  uint64                           ou_sw_id,
  secx509_code_hash_algo_type      code_hash_algo,
  secboot_image_hash_info_type*    image_hash_info_ptr
);

/* Helper function to verify the certificate chain */
static secboot_error_type  secboot_verify_cert_signature
(
  secboot_context_type*              context_ptr,
  const pbl_secx509_cert_list_type*  cert_list_ptr,
  uint32                             verifier_index,
  uint32                             verifiee_index
);

static secboot_error_type secboot_read_hw_fuses
(
  secboot_context_type*         context_ptr,
  uint32                        code_segment,
  secboot_fuse_info_type*       fuse_info_ptr
);

static secboot_error_type secboot_check_enable_rot
(
  secboot_context_type*         context_ptr,
  secx509_ou_field_info_type*   ou_field_info,
  secboot_fuse_info_type*       fuse_info,
  secboot_verified_info_type*   verified_info_ptr
);

static secboot_error_type secboot_verify_exponent
(
  const uint8* buf,
  uint32 buflen
);

/* zero indicates an error */
#define SECBOOT_HASH_LEN(hashtype) \
((hashtype) == CEML_HASH_ALGO_SHA1 ? CEML_HASH_DIGEST_SIZE_SHA1 : \
((hashtype) == CEML_HASH_ALGO_SHA256 ? CEML_HASH_DIGEST_SIZE_SHA256 : \
0))

#define SECBOOT_LOG_ERR(ctxptr, data1, data2, data3) secboot_log((ctxptr), (uintptr_t)(data1), (uintptr_t)(data2), (uintptr_t)(data3), __LINE__)

static void secboot_log
(
  secboot_context_type*        context_ptr,
  uintptr_t data1,
  uintptr_t data2,
  uintptr_t data3,
  uint32 linenum
)
{
  if (NULL != context_ptr)
  {
    context_ptr->error_log.data1 = data1;
    context_ptr->error_log.data2 = data2;
    context_ptr->error_log.data3 = data3;
    context_ptr->error_log.linenum = linenum;
  }
}

/*=============================================================================

                        LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/

/*---------------------------------------------------------------------------
    QC Attestation CA fingerprint (hash).
---------------------------------------------------------------------------*/
static const uint8 qc_ca_hash[CEML_HASH_DIGEST_SIZE_SHA1] = {
  0xae, 0xbe, 0x6b, 0xf0, 0x63, 0x26, 0x1d, 0xbc, 0x7a, 0x38, 0x7d, 0x8c,
  0x3a, 0x1c, 0xe2, 0x39, 0x5e, 0xe0, 0xce, 0x1a
};

/*---------------------------------------------------------------------------
    QC Root fingerprint (hash).
---------------------------------------------------------------------------*/
static const uint8 qc_root_hash[CEML_HASH_DIGEST_SIZE_SHA256] = {
  0xcc, 0x31, 0x53, 0xa8, 0x02, 0x93, 0x93, 0x9b, 0x90, 0xd0, 0x2d, 0x3b,
  0xf8, 0xb2, 0x3e, 0x02, 0x92, 0xe4, 0x52, 0xfe, 0xf6, 0x62, 0xc7, 0x49,
  0x98, 0x42, 0x1a, 0xda, 0xd4, 0x2a, 0x38, 0x0f
};

/*=========================================================================

                                            EXTERNAL FUNCTION DEFINITIONS

=========================================================================*/
/*=========================================================================

                                            INTERNAL FUNCTION DEFINITIONS

=========================================================================*/
/**
 * @brief This function ensures the exponent is 3 or 65537
 *
 * @param[in]         buf         Buffer containing exponent value
 *
 * @param[in]         buflen      Length of the buffer
 *                                         
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies None
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
static secboot_error_type secboot_verify_exponent(const uint8* buf, uint32 buflen)
{
  uint32 index = 0;

  if ( (buflen == 0) || (buf == NULL) )
  {
    return E_SECBOOT_FAILURE;
  }

  //Skip any leading zero's
  while ((index < buflen) && (buf[index] == 0))
  {
    index++;
  }

  //Check for exponent 3
  if ( (index+1 == buflen) && (buf[index] == 3) )
  {
    return E_SECBOOT_SUCCESS;
  }

  //Check for exponent 65537 - 0x010001
  if ( (index+3 == buflen) && (buf[index] == 1) &&  (buf[index+1] == 0) && (buf[index+2] == 1) )
  {
    return E_SECBOOT_SUCCESS;
  }

  return E_SECBOOT_FAILURE;
}

/**
 * @brief This function verifies a certificate's signature using the public RSA key
 *        in the certificate that signed it
 *
 * @param[in]         context_ptr          Pointer to context handler
 *
 * @param[in]         cert_list_ptr        Pointer to the list of parsed certificates
 * 
 * @param[in]         verifier_index       Index of the certificate in the cert list
 *                                         that should be used to verify the verifee
 *                                         certificates signature
 *
 * @param[in]         verifier_index       Index of the certificate in the cert list
 *                                         whose signature needs to be verified
 *
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies None
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
static secboot_error_type secboot_verify_cert_signature
(
  secboot_context_type*              context_ptr,
  const pbl_secx509_cert_list_type*  cert_list_ptr,
  uint32                             verifier_index,
  uint32                             verifiee_index
)
{
  secboot_error_type ret_val = E_SECBOOT_FAILURE;
  uint32           j;
  uint32           pad_start_index;
  uint32           pad_end_index;
  uint32           modulus_bitlen;
  BigInt           mod;
  BigInt           pub_exp;
  BigInt           signature;
  BigInt           decrypt_signature;
  BigInt           hash;
  CeMLHashAlgoType hash_algo;
  SECMATH_ERRNO_ET secmath_result;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Sanity check the parameters */
  if ( (cert_list_ptr == NULL)
       ||
       (verifier_index >= SECBOOT_MAX_NUM_CERTS)
       ||
       (verifiee_index >= SECBOOT_MAX_NUM_CERTS) )
  {
    SECBOOT_LOG_ERR(context_ptr, cert_list_ptr, verifier_index, verifiee_index);
    return E_SECBOOT_INVALID_PARAM;
  }

  /* ======================================================================*/
  /*  For the Secure boot code, all the certificates are chained           */
  /*  with the least-trusted certificate at index 0 and the                */
  /*  last/n'th certificate is the root certificate, which                 */
  /*  is self signed (BUT its self signature is NOT verified)              */
  /* ======================================================================*/

  do
  {
    /*=====================================================================*/
    /* 1. Validating Attestation certificate using CA Certificate.In cert[]*/
    /* table the Attestation certificate is the first element followed by  */
    /* CA and Root certificates                                            */
    /*=====================================================================*/

    /* 1. Format modulus */
    modulus_bitlen = cert_list_ptr->cert[verifier_index].pkey.key.rsa.mod_len *
                     8;
    /* check for legal range */
    if (modulus_bitlen < SECBOOT_MIN_KEY_SIZE_IN_BITS ||
        modulus_bitlen > SECBOOT_MAX_KEY_SIZE_IN_BITS)
    {
      //MSG_ERROR( "secboot: Modulus size out of legal range", 0, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, modulus_bitlen, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_FAILURE;
      break;
    }

    secmath_result = secmath_BIGINT_read_unsigned_bin(&mod,
                                               cert_list_ptr->cert[verifier_index].pkey.key.rsa.mod_data,
                                               modulus_bitlen / 8);
    if (E_SECMATH_SUCCESS != secmath_result)
    {
      //MSG_ERROR( "secboot: data is out of range", 0, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, secmath_result, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }

    /* 2. Format the public exponent from the cert info                                    */

    //Check that the exponent is 3 or 65537
    ret_val = secboot_verify_exponent(cert_list_ptr->cert[verifier_index].pkey.key.rsa.exp_e_data,
                                      cert_list_ptr->cert[verifier_index].pkey.key.rsa.exp_e_len);
    if (E_SECBOOT_SUCCESS != ret_val)
    {
      //MSG_ERROR( "secboot: invalid exponent", 0, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, ret_val, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_INVALID_DATA;
      break;
    }

    secmath_result = secmath_BIGINT_read_unsigned_bin(&pub_exp,
                                               cert_list_ptr->cert[verifier_index].pkey.key.rsa.exp_e_data,
                                               cert_list_ptr->cert[verifier_index].pkey.key.rsa.exp_e_len);
    if (E_SECMATH_SUCCESS != secmath_result)
    {
      //MSG_ERROR( "secboot: data is out of range", 0, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, secmath_result, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }
    
    /* Verified Certificate
    3. Format signature */
    secmath_result = secmath_BIGINT_read_unsigned_bin(&signature,
                                               cert_list_ptr->cert[verifiee_index].sig,
                                               cert_list_ptr->cert[verifiee_index].sig_len);
    if (E_SECMATH_SUCCESS != secmath_result)
    {
      //MSG_ERROR( "secboot: data is out of range", 0, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, secmath_result, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }

    if (cert_list_ptr->cert[verifiee_index].sig_algo == SECX509_sha1WithRSAEncryption)
    {
      hash_algo = CEML_HASH_ALGO_SHA1;
    }
    else if (cert_list_ptr->cert[verifiee_index].sig_algo == SECX509_sha256WithRSAEncryption)
    {
      hash_algo = CEML_HASH_ALGO_SHA256;
    }
    else
    {
      SECBOOT_LOG_ERR(context_ptr, cert_list_ptr->cert[verifiee_index].sig_algo, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_UNSUPPORTED;
      break;
    }

    /* 4. Format the hash into a BigInt number */   
    secmath_result = secmath_BIGINT_read_unsigned_bin(&hash,
                                               cert_list_ptr->cert[verifiee_index].cert_hash,
                                               SECBOOT_HASH_LEN(hash_algo));
    if (E_SECMATH_SUCCESS != secmath_result)
    {
      //MSG_ERROR( "secboot: BigInt allocation failed", 0, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, secmath_result, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }

    MEMSET((uint8 *)decrypt_signature.a,0,sizeof(decrypt_signature.a));

    /*  5. Actual verify function */
    secmath_result = secmath_BIGINT_modexp(&decrypt_signature,
                                                   &signature, &pub_exp, &mod);
    if (E_SECMATH_SUCCESS != secmath_result)
    {
      //MSG_ERROR( "secboot: Modexp operation failed", 0, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, secmath_result, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }

    /* 7. Check if we have a match                  */
    /* 7.1 Compare decrypted signature with hash */
    if ( MEMCMP( (uint8*) hash.a,
                 (uint8*) decrypt_signature.a,
                 SECBOOT_HASH_LEN(hash_algo) ) == 0)
    {
      /* So far - so good ... */
      ret_val = E_SECBOOT_SUCCESS;

      /*=====================================================================*/
      /* In here means that hash itself matches.                             */
      /* Now see if the padding etc is as per standard.                      */

      /* | 0001FFFF....FF | 00 | 15 bytes  | 20 bytes HASH |                 */
      /*   padding         ^     Hash ID                                     */
      /*                                        Delimiter                    */
      /* Signature Structure, PKCS v1.5 for SHA256                           */
      /* | 0001FFFF....FF | 00 | 19 bytes  | 32 bytes HASH |                 */
      /*   padding         ^     Hash ID                                     */
      /*                                        Delimiter                    */
      /*=====================================================================*/

      /* 7.2. check "FF"s                     */
      /* +16 'cos we have 15 bytes of hash ID for SHA1 + 00 */
      /* +20 'cos we have 19 bytes of hash ID for SHA256 + 00 */
      j = SECBOOT_HASH_LEN(hash_algo);
      pad_start_index = ((SECBOOT_HASH_LEN(hash_algo)+
                          (hash_algo==CEML_HASH_ALGO_SHA1 ? 16 : 20))/BLONG_SIZE);
      pad_end_index = modulus_bitlen/(8*BLONG_SIZE) - 1;
      for ( j=pad_start_index; j < pad_end_index; j++ )
      {
        if ( decrypt_signature.a[j] != 0xFFFFFFFF )
        {
          //MSG_ERROR("secboot: Certificate %d has right hash,
          //           but wrong padding", verifiee_index, 0, 0);
          SECBOOT_LOG_ERR(context_ptr, decrypt_signature.a[j], verifier_index, verifiee_index);
          ret_val = E_SECBOOT_INVALID_CERT;
          break;
        }
      }
      if ( ret_val != E_SECBOOT_SUCCESS )
      {
        break;
      }

      /* 7.3. check last 32 bit quantity. */
      if ( decrypt_signature.a[pad_end_index] != 0x0001FFFF )
      {
        //MSG_ERROR( "secboot: Certificate %d has right hash, but wrong padding",
        //           verifiee_index, 0, 0 );
        SECBOOT_LOG_ERR(context_ptr, decrypt_signature.a[pad_end_index], verifier_index, verifiee_index);
        ret_val = E_SECBOOT_INVALID_CERT;
        break;
      }

      /*=====================================================================*/
      /*7.4. Verify the HashID                                               */
      /*This is as per the PKCS signing process as described in PKCS 1.5 docs*/
      /*we need to verify bytes 21 to 35 for the fixed sequence you see below*/
      /*Byte 36 is the delimited. Also is is valid ONLY for SHA1 ! <-- note !*/
      /*Also our indexes are in terms of BLONGS (32 bits) not bytes ...      */
      /*=====================================================================*/
      if (hash_algo == CEML_HASH_ALGO_SHA1)
      {
        if ( (decrypt_signature.a[5] != 0x05000414) ||
             (decrypt_signature.a[6] != 0x0e03021a) ||
             (decrypt_signature.a[7] != 0x0906052b) ||
             (decrypt_signature.a[8] != 0x00302130) )
        {
        //MSG_ERROR( "secboot: Certificate %d has right hash, but wrong padding",
        //           verifiee_index, 0, 0 );
          SECBOOT_LOG_ERR(context_ptr, decrypt_signature.a[5], verifier_index, verifiee_index);
          ret_val = E_SECBOOT_INVALID_CERT;
          break;
        }
      }
      else if (hash_algo == CEML_HASH_ALGO_SHA256)
      {
        if ( (decrypt_signature.a[8] != 0x05000420) ||
             (decrypt_signature.a[9] != 0x03040201) ||
             (decrypt_signature.a[10] != 0x86480165) ||
             (decrypt_signature.a[11] != 0x0d060960) ||
             (decrypt_signature.a[12] != 0x00303130) )
        {
        //MSG_ERROR( "secboot: Certificate %d has right hash, but wrong padding",
        //           verifiee_index, 0, 0 );
          SECBOOT_LOG_ERR(context_ptr, decrypt_signature.a[8], verifier_index, verifiee_index);
          ret_val = E_SECBOOT_INVALID_CERT;
          break;
        }
      }
      else
      {
        SECBOOT_LOG_ERR(context_ptr, hash_algo, verifier_index, verifiee_index);
        ret_val = E_SECBOOT_INVALID_CERT;
        break;
      }
    }
    else
    {
      /* Hash doesn't match */
      //MSG_ERROR( "Certificate %d cannot be verified",
      //         verifiee_index, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, ret_val, verifier_index, verifiee_index);
      ret_val = E_SECBOOT_INVALID_CERT;
      break;
    }

  }/*lint -e(717) */ while ( FALSE );

  return ret_val;

}   /* secboot_verify_cert_signature () */

/**
 * @brief This function hashes each certificate and stores the hash of each cert
 *        in the cert structure
 *
 * @param[in]         secboot_hash_handle_ptr    Pointer to the hash handle.
 * 
 * @param[in]         x509_cert_chain_ptr        Pointer to start of the certificate chain buffer
 *
 * @param[in]         x509_cert_list_ptr         Pointer to the parsed certificates
 *
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies Caller should ensure all pointers and lengths passed in are valid
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
static secboot_error_type secboot_hash_certificates
(
  secboot_context_type*              context_ptr,
  const uint8*                       x509_cert_chain_ptr,
  const pbl_secx509_cert_list_type*  x509_cert_list_ptr
)
{
  secboot_error_type     ret_val = E_SECBOOT_SUCCESS;
  uint32                 curr_cert_num = 0;
  const uint8*           data_ptr;
  uint32                 data_len;
  CeMLHashAlgoType       hash_algo;

  do
  {
    if ((NULL == context_ptr) ||
        (NULL == x509_cert_chain_ptr) ||
        (NULL == x509_cert_list_ptr))
    {
      SECBOOT_LOG_ERR(context_ptr, context_ptr, x509_cert_chain_ptr, x509_cert_list_ptr);
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }

    //find the hash algorithm to use
    if (curr_cert_num == x509_cert_list_ptr->size - 1)
    {
      //This is the root certificiate.        
      //We validate this certificate by hashing the ENTIRE cert (start to the end of the cert sig)
      //using SHA256. This will later on be compared with the hash of the trusted root certificate
      hash_algo = CEML_HASH_ALGO_SHA256;
      data_ptr = x509_cert_chain_ptr;
      data_len = x509_cert_list_ptr->cert[curr_cert_num].asn1_size_in_bytes;
    }
    else
    {
      if (x509_cert_list_ptr->cert[curr_cert_num].sig_algo == SECX509_sha1WithRSAEncryption)
      {
        hash_algo = CEML_HASH_ALGO_SHA1;
      }
      else if (x509_cert_list_ptr->cert[curr_cert_num].sig_algo
              == SECX509_sha256WithRSAEncryption)
      {
        hash_algo = CEML_HASH_ALGO_SHA256;
      }
      else
      {
        SECBOOT_LOG_ERR(context_ptr, x509_cert_list_ptr->cert[curr_cert_num].sig_algo,
                        curr_cert_num, x509_cert_list_ptr->size);
        ret_val = E_SECBOOT_UNSUPPORTED;
        break;
      }

      //For non-root certificates, we validate the certificate by hashing the certificate
      //from start of the certificate to the start of the certificate signature,
      //as the hash will be compared later on with the hash in the signature
      data_ptr = x509_cert_chain_ptr + x509_cert_list_ptr->cert[curr_cert_num].cinf_offset;
      data_len = x509_cert_list_ptr->cert[curr_cert_num].cinf_byte_len;
    }

    ret_val = secboot_hash(&(context_ptr->crypto_hash_ftbl), hash_algo, data_ptr, data_len, NULL, 0,
                            (uint8*)(x509_cert_list_ptr->cert[curr_cert_num].cert_hash),
                            sizeof(x509_cert_list_ptr->cert[curr_cert_num].cert_hash));
    if ( E_SECBOOT_SUCCESS != ret_val)
    {
      SECBOOT_LOG_ERR(context_ptr, ret_val, curr_cert_num, hash_algo);
      ret_val = E_SECBOOT_HASH_FAIL;
      break;
    }

    //move to the start of next certificate
    x509_cert_chain_ptr += x509_cert_list_ptr->cert[curr_cert_num].asn1_size_in_bytes;

    //increment cert number
    curr_cert_num++;
  }while (curr_cert_num < x509_cert_list_ptr->size);

  return ret_val;
}

/**
 * @brief This function verifies the certificate chain
 *
 * @param[in]         x509_cert_list_ptr        Pointer to list of parsed certificates, starting
 *                                              with the attestation certificate
 *
 * @param[in]         root_of_trust_ptr         Pointer to the root of trust to validate the
 *                                              certificate chain's root cert against
 *
 *
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies Caller should ensure all pointers and lengths passed in are valid
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
static secboot_error_type secboot_verify_cert_chain
(
  secboot_context_type*        context_ptr,
  pbl_secx509_cert_list_type*  x509_cert_list_ptr,
  const uint8*                 root_of_trust_ptr
)
{
  secboot_error_type         ret_val = E_SECBOOT_SUCCESS;
  pbl_secx509_cert_ctx_type  boot_x509_ctx;
  uint32                     curr_cert_num = 0;
  uint32                     num_cert_sigs_to_verify;
  secx509_errno_enum_type    x509_result;
  do
  {

    if ((NULL == context_ptr) ||
        (NULL == x509_cert_list_ptr) ||
        (NULL == root_of_trust_ptr) ||
        (x509_cert_list_ptr->size < SECBOOT_MIN_NUM_CERTS) ||
        (x509_cert_list_ptr->size > SECBOOT_MAX_NUM_CERTS))
    {
      SECBOOT_LOG_ERR(context_ptr, context_ptr, x509_cert_list_ptr, root_of_trust_ptr);
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }

    //We verify the root certificate by comparing the hash of the entire root certificate
    //to the known root of trust. The remaining certs are verified through their signatures
    //so by default number of cert signatures to verify is total number of certs - the root cert
    num_cert_sigs_to_verify = x509_cert_list_ptr->size - 1;

    // Set up the X.509 context for the cert parser
    boot_x509_ctx.ca_list = NULL;
    boot_x509_ctx.purpose = SECX509_KEY_USAGE_DIG_SIG;
    boot_x509_ctx.depth   = ATTEST_CERT_INDEX;

    // Check if certificates are valid
    x509_result = pbl_secx509_check_cert_list( x509_cert_list_ptr,
                                               &boot_x509_ctx );
    if (E_X509_SUCCESS != x509_result)
    {
      SECBOOT_LOG_ERR(context_ptr, x509_result, 0, 0);
      ret_val = E_SECBOOT_INVALID_CERT;
      break;
    }

    // Check if this is a QC cert chain (i.e QC root and QC CA that was generated using CSMS)
    if (SECBOOT_MAX_NUM_CERTS == x509_cert_list_ptr->size)
    {
      if ( (MEMCMP(x509_cert_list_ptr->cert[ROOT_CERT_INDEX].cert_hash,
                     qc_root_hash, CEML_HASH_DIGEST_SIZE_SHA256) == 0) &&         
           (x509_cert_list_ptr->cert[CA_CERT_INDEX].sig_algo
                  == SECX509_sha1WithRSAEncryption) &&
           (MEMCMP(x509_cert_list_ptr->cert[CA_CERT_INDEX].cert_hash, qc_ca_hash,
                     CEML_HASH_DIGEST_SIZE_SHA1) == 0) )
      {
        // this is QC chain, attest cert will be different as CSMS uses generates
        // different attest key for each signing, so since we know the QC root
        // and QC CA is being used, we only need to verify the attestation cert
        // signature
        num_cert_sigs_to_verify = 1;
      }
    }

    for ( curr_cert_num = 0;  curr_cert_num < num_cert_sigs_to_verify;  curr_cert_num++)
    {
      if (secboot_verify_cert_signature(context_ptr,
                                        x509_cert_list_ptr,
                                        curr_cert_num + 1,
                                        curr_cert_num) != E_SECBOOT_SUCCESS)
      {
        ret_val = E_SECBOOT_INVALID_CERT_SIG;
        break;
      }
    }
    if (E_SECBOOT_SUCCESS != ret_val)
    {
      break;
    }

    //ALWAYS ALWAYS ensure the root certificate is trusted
    ret_val = MEMCMP(x509_cert_list_ptr->cert[x509_cert_list_ptr->size - 1].cert_hash, root_of_trust_ptr, 
                     CEML_HASH_DIGEST_SIZE_SHA256) == 0 ? E_SECBOOT_SUCCESS : E_SECBOOT_UNTRUSTED_ROOT;

  }while (0);

  return ret_val;
}

/**
 * @brief This function verifies an image signature
 *
 * @param[in,out]     attestation_cert_info_ptr Pointer to the attestation certificate.
 *
 * @param[in]         secboot_hash_handle_ptr   Pointer to the hash handle
 *
 * @param[in]         image_info_ptr            Pointer to the image
 *
 * @param[in]         msm_hw_id                 Pointer to the msm hardware id as constructed
 *                                              from the fuses
 *
 * @param[in]         ou_sw_id                  Pointer to the software id in the OU field
 *                                              of the attestation certificate
 *
 * @param[in,out]     image_hash_info_ptr       Pointer to a structure to be populated.
 *                                              with the hash of the image
 *
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies Caller should ensure all pointers and lengths passed in are valid
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
static secboot_error_type secboot_verify_image_signature
(
  secboot_context_type*            context_ptr,
  const pbl_secx509_cert_type*     attestation_cert_info_ptr,
  const secboot_image_info_type*   image_info_ptr,
  uint64                           msm_hw_id,
  uint64                           ou_sw_id,
  secx509_code_hash_algo_type      code_hash_algo,
  secboot_image_hash_info_type*    image_hash_info_ptr
)
{
  unsigned char   image_hash[CEML_HASH_DIGEST_SIZE_SHA256] = {0};
  unsigned char   inner_hash[CEML_HASH_DIGEST_SIZE_SHA256] = {0};
  unsigned char   outer_hash[CEML_HASH_DIGEST_SIZE_SHA256] = {0};
  unsigned char   work_buff[8];   /* big enough for ipad and opad stuff */
  uint32   ipad      = 0x36;
  uint32   opad      = 0x5c;
  unsigned char   sw_id[8]  = {0};
  unsigned char   msm_id[8] = {0};
  uint32          i;
  uint32          j;
  uint32          pad_start_index;
  uint32          pad_end_index;
  BigInt          mod;
  BigInt          pub_exp;
  BigInt          signature;
  BigInt          decrypt_signature;
  BigInt          hash;
  boolean         wrong_padding = FALSE;
  secboot_error_type  ret_val = E_SECBOOT_FAILURE;
  SECMATH_ERRNO_ET secmath_result;
  CeMLHashAlgoType algo = CEML_HASH_ALGO_SHA1; //we default to SHA1
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  do /* To break on errors */
  {
    if ((NULL == context_ptr) ||
        (NULL == attestation_cert_info_ptr) ||
        (NULL == image_info_ptr) ||
        (NULL == image_hash_info_ptr))
    {
      SECBOOT_LOG_ERR(context_ptr,
                      attestation_cert_info_ptr,
                      image_info_ptr,
                      image_hash_info_ptr);
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }
	
	/* Check that the signature length is a multiple of a BLONG,  
     * since the signature padding checks depend on this.  Check  
     * that the signature length (in bytes) equals the length of 
     * the verifying key modulus (in bytes, with leading zero bytes removed) 
     */
    if ( (image_info_ptr->signature_len % BLONG_SIZE) || 
         (image_info_ptr->signature_len != attestation_cert_info_ptr->pkey.key.rsa.mod_len) )
    {    
      SECBOOT_LOG_ERR(context_ptr, (uint32)attestation_cert_info_ptr->pkey.key.rsa.mod_len, 
                                   (uint32)image_info_ptr->signature_len, 0);    
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }

    //In order for SHA256 to be used it needs to be in the attestation certs
    //hash algo OU field. We default to SHA1 otherwise, for legacy reasons
    if (E_X509_CODE_HASH_SHA256==code_hash_algo)
    {
      algo = CEML_HASH_ALGO_SHA256;
    }

    /* Hash the image */
    ret_val = secboot_hash(&(context_ptr->crypto_hash_ftbl), algo,
                           image_info_ptr->header_ptr_1, image_info_ptr->header_len_1,
                           image_info_ptr->code_ptr_1, image_info_ptr->code_len_1,
                           image_hash,
                           sizeof(image_hash));
    if (E_SECBOOT_SUCCESS != ret_val)
    {
      SECBOOT_LOG_ERR(context_ptr, ret_val,
                      image_info_ptr->header_ptr_1,
                      image_info_ptr->code_ptr_1);
      ret_val = E_SECBOOT_HASH_FAIL;
      break;
    }

    /* assign the code address that we hashed for UEFI use later in the */
    /* boot chain */
    image_hash_info_ptr->code_address = (uintptr_t)image_info_ptr->code_ptr_1;
    image_hash_info_ptr->code_length = image_info_ptr->code_len_1;
    image_hash_info_ptr->image_hash_length = SECBOOT_HASH_LEN(algo);
    if (sizeof(image_hash_info_ptr->image_hash) < image_hash_info_ptr->image_hash_length)
    {
      SECBOOT_LOG_ERR(context_ptr, E_SECBOOT_INVALID_PARAM,
                      sizeof(image_hash_info_ptr->image_hash),
                      image_hash_info_ptr->image_hash_length);
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;      
    }
    MEMCPY(image_hash_info_ptr->image_hash, image_hash, image_hash_info_ptr->image_hash_length);

    /* Compute inner hash.                                              */
    /* Consult design doc for details on this non-standard hash usage.  */
    /* In short, sbl_proc_hash =                                        */
    /*   H[(MSM ID ^ opad) || H[(SWID ^ ipad) || H(code image)] ],      */
    /* where H = the hash specificed by signature hash type             */
    MEMSET( work_buff, 0 , sizeof( work_buff ) );
    MEMCPY( (uint8*) msm_id, (uint8*) &msm_hw_id, sizeof( msm_hw_id ) );
    MEMCPY( (uint8*) sw_id, (uint8*) &ou_sw_id, sizeof( ou_sw_id ) );

    /* 'Si' first added in the front */
    for ( i=0; i < 8; i++)
    {
      BSTOR8( work_buff+i, ipad ^ BLOAD8(sw_id+7-i) );
    }

    /* Process (SWID ^ ipad) */ 
    /* Process H(code image) */
    ret_val = secboot_hash(&(context_ptr->crypto_hash_ftbl), algo,
                           work_buff, 8,
                           image_hash, SECBOOT_HASH_LEN(algo),
                           inner_hash, sizeof(inner_hash));
    if (E_SECBOOT_SUCCESS != ret_val)
    {
      SECBOOT_LOG_ERR(context_ptr, ret_val,
                      image_info_ptr->header_ptr_1,
                      image_info_ptr->code_ptr_1);
      ret_val = E_SECBOOT_HASH_FAIL;
      break;
    }

    /* Compute outer hash function. */
    MEMSET( work_buff, 0 , sizeof(work_buff) );

    /* 'So' pad first added in the front */
    for (i = 0; i < 8; i++)
    {
      BSTOR8( work_buff+i, opad ^ BLOAD8(msm_id+7-i) );
    }

    /* Process (MSM ID ^ opad) */
    /* process H[(SWID ^ ipad) || H(code image)] */
    ret_val = secboot_hash(&(context_ptr->crypto_hash_ftbl), algo,
                           work_buff, 8,
                           inner_hash, SECBOOT_HASH_LEN(algo),
                           outer_hash, sizeof(outer_hash));
    if (E_SECBOOT_SUCCESS != ret_val)
    {  
      SECBOOT_LOG_ERR(context_ptr, ret_val,
                      image_info_ptr->header_ptr_1,
                      image_info_ptr->code_ptr_1);

      ret_val = E_SECBOOT_HASH_FAIL;
      break;
    }

    /* Now H[(MSM ID ^ opad) || H[(SWID ^ ipad) || H(code image)] ]
         is in hash_out */

    /* using <= combines the check for integer overflow and the check of length equal to 0. */
    if ( (NULL == attestation_cert_info_ptr->pkey.key.rsa.mod_data) ||
         ((uintptr_t)attestation_cert_info_ptr->pkey.key.rsa.mod_data +
            attestation_cert_info_ptr->pkey.key.rsa.mod_len <= 
              (uintptr_t)attestation_cert_info_ptr->pkey.key.rsa.mod_data) ||
         (attestation_cert_info_ptr->pkey.key.rsa.mod_len > MAX_KEY_SIZE_IN_BITS/8) ||
         (NULL == attestation_cert_info_ptr->pkey.key.rsa.exp_e_data) ||
         ((uintptr_t)attestation_cert_info_ptr->pkey.key.rsa.exp_e_data +
            attestation_cert_info_ptr->pkey.key.rsa.exp_e_len <=
              (uintptr_t)attestation_cert_info_ptr->pkey.key.rsa.exp_e_data) ||
         (attestation_cert_info_ptr->pkey.key.rsa.exp_e_len > 4) )
    {
      SECBOOT_LOG_ERR(context_ptr, E_SECBOOT_INVALID_DATA, 0, 0);
      ret_val = E_SECBOOT_INVALID_DATA;
      break;
    }

    /*** Verify the hash with signature ***/
    secmath_result = secmath_BIGINT_read_unsigned_bin(&mod,
                                                       attestation_cert_info_ptr->pkey.key.rsa.mod_data,
                                                       attestation_cert_info_ptr->pkey.key.rsa.mod_len );
    if ( E_SECMATH_SUCCESS != secmath_result)
    {
      SECBOOT_LOG_ERR(context_ptr, secmath_result,
                      (attestation_cert_info_ptr->pkey.key.rsa.mod_data),
                      attestation_cert_info_ptr->pkey.key.rsa.mod_len);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }

    //Check that the exponent is 3 or 65537
    ret_val = secboot_verify_exponent(attestation_cert_info_ptr->pkey.key.rsa.exp_e_data, 
                                      attestation_cert_info_ptr->pkey.key.rsa.exp_e_len);
    if (E_SECBOOT_SUCCESS != ret_val)
    {
      //MSG_ERROR( "secboot: invalid exponent", 0, 0, 0 );
      SECBOOT_LOG_ERR(context_ptr, secmath_result,
                      (attestation_cert_info_ptr->pkey.key.rsa.exp_e_data),
                      attestation_cert_info_ptr->pkey.key.rsa.exp_e_len);
      ret_val = E_SECBOOT_INVALID_DATA;
      break;
    }

    secmath_result = secmath_BIGINT_read_unsigned_bin(&pub_exp,
                                                               attestation_cert_info_ptr->pkey.key.rsa.exp_e_data,
                                                               attestation_cert_info_ptr->pkey.key.rsa.exp_e_len);
    if ( E_SECMATH_SUCCESS != secmath_result)
    {
      SECBOOT_LOG_ERR(context_ptr, secmath_result,
                      (attestation_cert_info_ptr->pkey.key.rsa.exp_e_data),
                      attestation_cert_info_ptr->pkey.key.rsa.exp_e_len);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }
    secmath_result = secmath_BIGINT_read_unsigned_bin(&signature,
                                                                image_info_ptr->signature_ptr,
                                                                image_info_ptr->signature_len );
    if ( E_SECMATH_SUCCESS != secmath_result)
    {
      SECBOOT_LOG_ERR(context_ptr, secmath_result,
                      (image_info_ptr->signature_ptr),
                      image_info_ptr->signature_len);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }

    secmath_result = secmath_BIGINT_read_unsigned_bin(&hash,
                                                       outer_hash,
                                                       SECBOOT_HASH_LEN(algo));
    if ( E_SECMATH_SUCCESS != secmath_result)
    {
      SECBOOT_LOG_ERR(context_ptr, secmath_result, outer_hash, sizeof(outer_hash));
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }

    MEMSET((uint8 *)decrypt_signature.a,0,sizeof(decrypt_signature.a));

    /* 1. Decrypt the signature */
    secmath_result = secmath_BIGINT_modexp(&decrypt_signature, &signature, &pub_exp, &mod);
    if ( E_SECMATH_SUCCESS != secmath_result)
    {
      SECBOOT_LOG_ERR(context_ptr, secmath_result, 0, 0);
      ret_val = E_SECBOOT_RSA_FAIL;
      break;
    }

    /*** Check Padding                                                            ***/
    /*** Note: The code signature is not strictly PKCS#1.5 which would be         ***/
    /*** 0001FFFF…..FFFFFF00<HashAlgorithmID><Hash>                               ***/
    /*** Our code signature is in this form                                       ***/
    /*** 0001FFFF…..FFFFFF00<Hmac like Hash>                                      ***/ 
    /***                     H[(MSM ID^opad)||H[(SWID^ipad)||H(codeimage)]]       ***/

    /* First 32 bits of padding */
    if (decrypt_signature.a[SECBOOT_HASH_LEN(algo)/BLONG_SIZE] != 0xFFFFFF00 )
    {
      SECBOOT_LOG_ERR(context_ptr, decrypt_signature.a[CEML_HASH_DIGEST_SIZE_SHA256/BLONG_SIZE], 0, 0);
      ret_val = E_SECBOOT_INVALID_IMAGE_SIG;
      break;
    }

    /* Padding in the middle (+1 to skip the BLONG checked above) */
    pad_start_index = (SECBOOT_HASH_LEN(algo)/BLONG_SIZE)+1;
    pad_end_index = (((attestation_cert_info_ptr->pkey.key.rsa.mod_len)/BLONG_SIZE)-1);
    for ( j=pad_start_index; j < pad_end_index; j++ )
    {
      if ( decrypt_signature.a[j] != 0xFFFFFFFF )
      {
        wrong_padding = TRUE;
        break;
      }
    }

    if ( wrong_padding )
    {
      SECBOOT_LOG_ERR(context_ptr, decrypt_signature.a[j], j, pad_end_index);
      ret_val = E_SECBOOT_INVALID_IMAGE_SIG;
      break;
    }

    /* Last 32 bits of Padding */
    if (decrypt_signature.a[pad_end_index] != 0x0001FFFF )
    {
      SECBOOT_LOG_ERR(context_ptr, decrypt_signature.a[pad_end_index],
                      pad_end_index, pad_start_index);
      ret_val = E_SECBOOT_INVALID_IMAGE_SIG;
      break;
    }

    /* Last step - Code to byte compare decrypted signature and hash */
    if (MEMCMP( (uint8*) hash.a,
                (uint8*) decrypt_signature.a,
                SECBOOT_HASH_LEN(algo) ) == 0)
    {
      ret_val = E_SECBOOT_SUCCESS;
    }
    else
    {
      SECBOOT_LOG_ERR(context_ptr, E_SECBOOT_INVALID_IMAGE_SIG, 0, 0);
      ret_val = E_SECBOOT_INVALID_IMAGE_SIG;
    }

  }/*lint -e(717) */ while (FALSE);

  return ret_val;

}   /* secboot_verify_image_signature() */

/**
 * @brief This function calls the crypto software driver's hash API to
 *        hash the data
 *
 * @param[in]         secboot_hash_handle_ptr    Pointer to the hash handle.
 *
 * @param[in]         hash_algo         Hash algorithm to use.
 *
 * @param[in]         data1_in_ptr      Pointer to the data to hash. Cannot be NULL. 
 *                                      as we don't expect boot images/certificates
 *                                      to be loaded at address 0
 *
 * @param[in]         data1_len         Length in bytes of the data pointed to by
 *                                      data1_in_ptr. Cannot be 0 as we don't allow
 *                                      image or certificate size to be 0
 *
 * @param[in]         data2_in_ptr      Pointer to the data to hash. This is an
 *                                      optional field and can be NULL.
 *
 * @param[in]         data2_len         Length in bytes of the data pointed to by
 *                                      data2_in_ptr. 0 if data2_in_ptr is NULL.
 *                                  
 * @param[in,out]     digest_ptr        Pointer to a buffer to store the hash result.
 *
 * @param[in]         digest_len        Length in bytes of buffer pointed to by
 *                                      digest_ptr.
 *
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies Caller should ensure the pointers and lengths passed in are valid
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
 secboot_error_type secboot_hash
(
  secboot_crypto_hash_ftbl_type*    crypto_hash_ftbl,
  CeMLHashAlgoType                  hash_algo,
  const uint8*                      data1_in_ptr,
  uint32                            data1_len,
  const uint8*                      data2_in_ptr,
  uint32                            data2_len,
  uint8*                            digest_ptr,
  size_t                            digest_len
)
{
  secboot_error_type  ret_val = E_SECBOOT_FAILURE;
  CeMLCntxHandle*     ce_cntx_ptr = NULL;
  CeMLCntxHandle      ce_cntx;
  CEMLIovecListType   ioveclist_in;
  CEMLIovecListType   ioveclist_out;
  CEMLIovecType       iovec_inbuff;
  CEMLIovecType       iovec_outbuff;
  uint32 crypto_context[CEML_PBL_HASHCTX_SIZE/sizeof(uint32) + sizeof(uint32)];

   if ( (NULL == crypto_hash_ftbl) || (NULL == data1_in_ptr) || (0 == data1_len) )
   {
      return E_SECBOOT_INVALID_PARAM;
    }
    if(crypto_hash_ftbl->CeMLInit() != CEML_ERROR_SUCCESS)
    {
      return E_SECBOOT_HASH_FAIL;
    }
  do
  {
   
    if (crypto_hash_ftbl->CeMLHashInit == CeMLHashInit)
    {
      /* 1. The PBL crypto driver is different from the usual crypto driver. We removed all */
      /* ZI/RW (static/global) variables from the PBL crypto driver, so the driver no */
      /* longer has a static buffer to allocate the context from. The Crypto driver API */
      /* cannot be changed, so we allocate the crypto context and pass it into the driver */
      /* 2. For non-PBL images, they call into secboot routines (i.e TZ calling into PBL secboot), */
      /* and provide the pointers to the crypto driver linked into their image (e.g a BAM based */
      /* crypto driver etc), in which case the context will be allocated by the non-PBL crypto */
      /* driver itself. CeMLHashInit() will allocate and initialize the CE context */
      MEMSET((uint8*)(crypto_context), 0, sizeof(crypto_context));
      ce_cntx.pClientCtxt = (void*)crypto_context;
      ce_cntx_ptr = &ce_cntx;
    }

    if (crypto_hash_ftbl->CeMLHashInit(&ce_cntx_ptr, hash_algo) != CEML_ERROR_SUCCESS)
    {
      ret_val = E_SECBOOT_HASH_FAIL;
      break;
    }

    ioveclist_in.size = 1;
    ioveclist_in.iov = &iovec_inbuff;
    ioveclist_in.iov[0].pvBase = (void*) data1_in_ptr;
    ioveclist_in.iov[0].dwLen = data1_len;

    ioveclist_out.size = 1;
    ioveclist_out.iov = &iovec_outbuff;
    ioveclist_out.iov[0].pvBase = (void*) digest_ptr;
    ioveclist_out.iov[0].dwLen = digest_len;

    if (data2_in_ptr == NULL)
    {
      if (crypto_hash_ftbl->CeMLHashAtomic(ce_cntx_ptr, ioveclist_in, &ioveclist_out) != CEML_ERROR_SUCCESS)
      {
        ret_val = E_SECBOOT_HASH_FAIL;
        break;
      }      
    }
    else
    {
      if (crypto_hash_ftbl->CeMLHashUpdate(ce_cntx_ptr, ioveclist_in) != CEML_ERROR_SUCCESS)
      {
        ret_val = E_SECBOOT_HASH_FAIL;
        break;
      }
  
      ioveclist_in.size = 1;
      ioveclist_in.iov = &iovec_inbuff;
      ioveclist_in.iov[0].pvBase = (void*) data2_in_ptr;
      ioveclist_in.iov[0].dwLen = data2_len;
      if (crypto_hash_ftbl->CeMLHashUpdate(ce_cntx_ptr, ioveclist_in) != CEML_ERROR_SUCCESS)
      {
        ret_val = E_SECBOOT_HASH_FAIL;
        break;
      }
  
      if (crypto_hash_ftbl->CeMLHashFinal(ce_cntx_ptr, &ioveclist_out) != CEML_ERROR_SUCCESS)
      {
        ret_val = E_SECBOOT_HASH_FAIL;
        break;
      }
    }
    if (crypto_hash_ftbl->CeMLHashDeInit(&ce_cntx_ptr) != CEML_ERROR_SUCCESS)
    {
      ret_val = E_SECBOOT_HASH_FAIL;
      break;
    }

    ret_val = E_SECBOOT_SUCCESS;
  }while (0);

  if(crypto_hash_ftbl->CeMLDeInit() != CEML_ERROR_SUCCESS)
  {
    ret_val = E_SECBOOT_HASH_FAIL;
  }
  return ret_val;
}

/**
 * @brief This function calls into the hardware API to get the fuse related
 *        secure boot authentication information
 *
 * @param[in]         code_segment         Code segment in SECURE_BOOTn fuse register
 *                                         containing authentication settings for
 *                                         the image.
 *
 * @param[in]         fuse_info_ptr        Pointer to fuse settings
 *                                         
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies None
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
static secboot_error_type secboot_read_hw_fuses
(
  secboot_context_type*   context_ptr,
  uint32                  code_segment,
  secboot_fuse_info_type* fuse_info_ptr
)
{
  secboot_error_type ret_val = E_SECBOOT_SUCCESS;
  secboot_hw_etype   hw_result = E_SECBOOT_HW_FAILURE;
  do
  {
    if (fuse_info_ptr == NULL)
    {
      SECBOOT_LOG_ERR(context_ptr, 0, 0, 0);
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }

    MEMSET((uint8*)fuse_info_ptr, 0, sizeof(*fuse_info_ptr));

    /* Check if the root of trust is supplied by the caller through secboot_init_fuses() */
    if(!context_ptr->fuses.use_root_of_trust_only)
    {
      /* Read the root of trust from the HW based on ROT fuse values */
      hw_result = secboot_hw_get_root_of_trust(code_segment, 
                                               fuse_info_ptr->root_of_trust);
      if (E_SECBOOT_HW_SUCCESS != hw_result)
      {
        SECBOOT_LOG_ERR(context_ptr, hw_result, code_segment, 0);
        ret_val = E_SECBOOT_HW_FAIL;
        break;
      }
    }
    else
    {
      /* Copy the root of trust which is stored in context through the function call secboot_init_fuses() */
      MEMCPY(fuse_info_ptr->root_of_trust, context_ptr->fuses.root_of_trust,SHA256_HASH_LEN);
    }

    hw_result = secboot_hw_get_rot_fuse_info(code_segment,
                                             &(fuse_info_ptr->rot_fuse_info.disable_rot_transfer),
                                             &(fuse_info_ptr->rot_fuse_info.current_sw_rot),
                                             &(fuse_info_ptr->rot_fuse_info.sw_rot_use_serial_num));
    if (E_SECBOOT_HW_SUCCESS != hw_result)
    {
      SECBOOT_LOG_ERR(context_ptr, hw_result, code_segment, 0);
      ret_val = E_SECBOOT_HW_FAIL;
      break;
    }

    hw_result = secboot_hw_get_msm_hw_id(code_segment,
                                         fuse_info_ptr->root_of_trust,
                                         &(fuse_info_ptr->msm_hw_id));
    if (E_SECBOOT_HW_SUCCESS != hw_result)
    {
      SECBOOT_LOG_ERR(context_ptr, hw_result, code_segment, 0);
      ret_val = E_SECBOOT_HW_FAIL;
      break;
    }

    hw_result = secboot_hw_get_use_serial_num(code_segment, &(fuse_info_ptr->auth_use_serial_num));
    if (E_SECBOOT_HW_SUCCESS != hw_result)
    {
      SECBOOT_LOG_ERR(context_ptr, hw_result, code_segment, 0);
      ret_val = E_SECBOOT_HW_FAIL;
      break;
    }

    hw_result = secboot_hw_get_serial_num(&fuse_info_ptr->serial_num);
    if (E_SECBOOT_HW_SUCCESS != hw_result)
    {
      SECBOOT_LOG_ERR(context_ptr, hw_result, code_segment, 0);
      ret_val = E_SECBOOT_HW_FAIL;
      break;
    }

    hw_result = secboot_hw_get_soc_hw_version(&fuse_info_ptr->soc_hw_version_family_device_num);
    if (E_SECBOOT_HW_SUCCESS != hw_result)
    {
      SECBOOT_LOG_ERR(context_ptr, hw_result, code_segment, 0);
      ret_val = E_SECBOOT_HW_FAIL;
      break;
    }

  }while (0);

  return ret_val;
}

/**
* @brief See documentation in public header 
*  
* @note The crypto hash function pointers argument is to enable the reuse 
*       of the PBL secboot routines with callers crypto driver. This way
*       the entire secboot code does not need to be linked into secondary
*       images like SBL, TZ, MBA etc, but they can make use of the PBL's
*       routines. The PBL crypto driver uses the register-based hash interface
*       which is not as performnat as using BAM. In addition if the crypto
*       engine is being used by another entity (i.e HLOS uses it while TZ starts
*       authenticating an image), the PBL crypto driver would not detect it.
*       Also the clock setup is done by the caller for PBL crypto driver. Due
*       to the limitations of the PBL crypto driver, and the requirement to
*       enable reuse of the secboot functions, the ability for caller to provide
*       the crypto driver it wants the PBL secboot code to use was added via this
*       crypto hash ftbl ptr argument.
*/
secboot_error_type secboot_init
(
  const secboot_crypto_hash_ftbl_type*  crypto_hash_ftbl_ptr,
  secboot_handle_type*      secboot_handle_ptr
)
{
  secboot_context_type* context_ptr;

  if ((secboot_handle_ptr == NULL) ||
      (sizeof(secboot_handle_ptr->handle) < sizeof(secboot_context_type)))
  {
    return E_SECBOOT_FAILURE;
  }

  //Ensure the buffer is initialized
  MEMSET((uint8*)secboot_handle_ptr->handle, 0, sizeof(secboot_handle_ptr->handle));

  context_ptr = (secboot_context_type*) secboot_handle_ptr->handle;

  context_ptr->version = SECBOOT_VERSION;

  if (crypto_hash_ftbl_ptr != NULL)
  {
    //Call into the caller's crypto driver
    if ((NULL == crypto_hash_ftbl_ptr->CeMLInit) ||
        (NULL == crypto_hash_ftbl_ptr->CeMLDeInit) ||
        (NULL == crypto_hash_ftbl_ptr->CeMLHashInit) ||
        (NULL == crypto_hash_ftbl_ptr->CeMLHashUpdate) ||
        (NULL == crypto_hash_ftbl_ptr->CeMLHashFinal) ||
        (NULL == crypto_hash_ftbl_ptr->CeMLHashDeInit) ||
        (NULL == crypto_hash_ftbl_ptr->CeMLHashAtomic))
    {
      //Clean out the buffer. Can't call SECBOOT_LOG_ERR() as it stores the error in this
      //buffer which we're clearing out
      MEMSET((uint8*)secboot_handle_ptr->handle, 0, sizeof(secboot_handle_ptr->handle));
      return E_SECBOOT_INIT;
    }

    MEMCPY(&context_ptr->crypto_hash_ftbl,
           crypto_hash_ftbl_ptr, 
           sizeof(secboot_crypto_hash_ftbl_type));
  }
  else
  {
    //Call into the crypto driver routines linked into this image
    context_ptr->crypto_hash_ftbl.CeMLInit = CeMLInit;
    context_ptr->crypto_hash_ftbl.CeMLDeInit = CeMLDeInit;
    context_ptr->crypto_hash_ftbl.CeMLHashInit = CeMLHashInit;
    context_ptr->crypto_hash_ftbl.CeMLHashDeInit = CeMLHashDeInit;
    context_ptr->crypto_hash_ftbl.CeMLHashUpdate = CeMLHashUpdate;
    context_ptr->crypto_hash_ftbl.CeMLHashFinal = CeMLHashFinal;
    context_ptr->crypto_hash_ftbl.CeMLHashAtomic = CeMLHashAtomic;
  }
  

  /* Set the default value to FALSE.This variable will be set to TRUE if the function 
     secboot_init_fuses() is called to supply the fuse values from externally */
  context_ptr->use_supplied_fuses = FALSE;

  //This denotes the context has been setup
  context_ptr->magic_number = CONTEXT_MAGIC_NUMBER;

  return E_SECBOOT_SUCCESS;
}

/**
 * @brief This function checks ROT_EN OU field and fills up the enable_rot
 *        information for verified return information
 *
 * @param[in]         context_ptr          Pointer to secboot context
 * @param[in]         ou_field_info        Pointer to ou field data
 * @param[in]         fuse_info            Pointer to fuse settings
 *         
 * @param[in/out]     verified_info_ptr    Pointer to verified information
 *     
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies None
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
static secboot_error_type secboot_check_enable_rot
(
  secboot_context_type*         context_ptr,
  secx509_ou_field_info_type*   ou_field_info,
  secboot_fuse_info_type*       fuse_info,
  secboot_verified_info_type*   verified_info_ptr
)
{
  secboot_error_type ret_val = E_SECBOOT_FAILURE;
  uint64          rot_enable = 0;
  uint32          serial_num = 0;

  do
  {
    if ((ou_field_info == NULL) || (fuse_info == NULL) || (verified_info_ptr == NULL) || (context_ptr == NULL))
    {
      SECBOOT_LOG_ERR(context_ptr, ou_field_info, fuse_info, verified_info_ptr);
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }

    /* by default setting the rot_disabled to TURE and disable ROT transfer */
    verified_info_ptr->rot_disabled = SECBOOT_ROT_TRANSFER_DISABLE;

    /* Ignore the ROT_EN OU field in certificate, and clear up the enable_rot in verified_info  */
    if ((SECBOOT_HW_ROT_TRANSFER_DISABLE == fuse_info->rot_fuse_info.disable_rot_transfer) ||   // if rot transfer is disabled by fuse ,
        (1 == fuse_info->rot_fuse_info.current_sw_rot))           // if rot transfer already happens
    {
      ret_val = E_SECBOOT_SUCCESS;
      break;
    }

    rot_enable =
          ou_field_info->rot_ou_field & SECBOOT_ROT_SETTINGS_FIELD_MASK;
    if (!( (SECBOOT_ROT_OU_FIELD_DISABLE == rot_enable) ||
           (SECBOOT_ROT_OU_FIELD_ENABLE == rot_enable) ))
    {
      // Unsupported setting...no arbitrary value is allowed
      SECBOOT_LOG_ERR(context_ptr, rot_enable, ou_field_info->rot_ou_field, 0);
      ret_val = E_SECBOOT_INVALID_ROT_VALUE;
      break;
    }

    /* ROT_USE_SERIAL_NUM is optional */
    if ((SECBOOT_ROT_OU_FIELD_ENABLE == rot_enable) 
		&& (fuse_info->rot_fuse_info.sw_rot_use_serial_num))
    {          
      // Turn on ROT_USE_SERIAL_NUM, the device's serial number must matches
      // the serial number value from the upper 32 bits of this field
      serial_num = (ou_field_info->rot_ou_field >> SECBOOT_ROT_SERIALNUM_FIELD_SHFT);
      if (serial_num != fuse_info->serial_num)
      {
        // return failure, this crash dump cert cannot be used for this device
        SECBOOT_LOG_ERR(context_ptr, fuse_info->serial_num, serial_num, 0);
        ret_val = E_SECBOOT_INVALID_ROT_SERIALNUM;
        break;
      }
    }
    /* set rot_disable to FALSE and enable ROT transfer only if rot ou field is set to SECBOOT_ROT_OU_FIELD_ENABLE */
    if (SECBOOT_ROT_OU_FIELD_ENABLE == rot_enable)
    {
      verified_info_ptr->rot_disabled = SECBOOT_ROT_TRANSFER_ENABLE;
    }

    ret_val = E_SECBOOT_SUCCESS;

  }while (0);

  return ret_val;
}

/**
 * @brief This function makes the decision on what msm_hw_id should contain, based on the OU field 
 *        enablement in the signing.
 *
 * @param[in]         context_ptr          Pointer to secboot context
 * @param[in]         ou_field_info        Pointer to ou field data
 *         
 * @param[in, out]    fuse_info            Pointer to fuse settings
 *    
 * @return E_SECBOOT_SUCCESS on success. Appropriate error code on failure.
 *
 * @dependencies None
 *
 * @sideeffects  None
 *           
 * @see None
 *
 */
static secboot_error_type secboot_update_msm_hw_id
(
  secboot_context_type*         context_ptr,
  secx509_ou_field_info_type*   ou_field_info,
  secboot_fuse_info_type*       fuse_info
)
{

  if ((NULL == context_ptr) ||
      (NULL == ou_field_info) ||
      (NULL == fuse_info))
  {
    return E_SECBOOT_INVALID_PARAM;
  }

  /* IF IN_USE_SOC_HW_VERSION ou field is enabled in signing, the higher 32bit      */
  /* of msm_hw_id is replaced with soc hw version fuse value.                       */
  /* sanity check the valid option value */
  if (!( (SECBOOT_IN_USE_SOC_HW_VER_DISABLE == ou_field_info->in_use_soc_hw_version)     ||
        (SECBOOT_IN_USE_SOC_HW_VER_ENABLE == ou_field_info->in_use_soc_hw_version) ))
  {
    SECBOOT_LOG_ERR(context_ptr, ou_field_info->in_use_soc_hw_version, 0, 0);
    return E_SECBOOT_INVALID_IN_USE_SOC_HW_VER_OPTION;
  }
    
  if (SECBOOT_IN_USE_SOC_HW_VER_ENABLE == ou_field_info->in_use_soc_hw_version) 
  {
    fuse_info->msm_hw_id = ((uint64)fuse_info->soc_hw_version_family_device_num << SECBOOT_SOC_HW_VER_FIELD_SHFT) |
                           (fuse_info->msm_hw_id & SECBOOT_SOC_HW_VER_FIELD_MASK);
  }

  /* IF USE_SERIAL_NUMBER_IN_SIGNING ou field is enabled in signing, the lower 32bit */
  /* of msm_hw_id is replaced with serial number.                                    */
  /* sanity check the valid option value */
  if ( (SECBOOT_OU_USE_SERIAL_NUMBER_DISABLE != ou_field_info->ou_use_serial_num) &&
        (SECBOOT_OU_USE_SERIAL_NUMBER_ENABLE != ou_field_info->ou_use_serial_num) )
  {
    SECBOOT_LOG_ERR(context_ptr, ou_field_info->ou_use_serial_num, 0, 0);
    return E_SECBOOT_INVALID_OU_USE_SERIAL_NUMBER_OPTION;
  }
    
  if (SECBOOT_OU_USE_SERIAL_NUMBER_ENABLE == ou_field_info->ou_use_serial_num) 
  {
    /* sanity check if this is valid serial number, when serial number is required */
    if (0 == fuse_info->serial_num)
    {
      return E_SECBOOT_INVALID_SERIAL_NUM;
    }

    fuse_info->msm_hw_id = ((uint64)fuse_info->serial_num & (~SECBOOT_OU_USE_SERIAL_NUMBER_FIELD_MASK)) |
                           (fuse_info->msm_hw_id & SECBOOT_OU_USE_SERIAL_NUMBER_FIELD_MASK);
  }

  return E_SECBOOT_SUCCESS;
}

/**
 * @brief See documentation in public header
 *
 */
secboot_error_type secboot_authenticate
(
  secboot_handle_type*                secboot_handle_ptr,
  uint32                              code_segment,
  const secboot_image_info_type*      image_info_ptr,
  secboot_verified_info_type*         verified_info_ptr
)
{
  secboot_error_type           ret_val    = E_SECBOOT_FAILURE;
  secboot_context_type*        context_ptr = NULL;
  uint32                       serial_num;
  pbl_secx509_cert_list_type   pbl_secx509_cert_list;
  secx509_ou_field_info_type   ou_field_info;
  secboot_fuse_info_type       fuse_info;
  uint64                       debug_enable = 0;
  uint64                       crash_dump_enable = 0;
  secx509_errno_enum_type      x509_result = E_X509_FAILURE;

  do
  {
    if (secboot_handle_ptr == NULL)
    {
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }
    context_ptr = (secboot_context_type*) secboot_handle_ptr->handle;

    //Validate the secboot handle parameter
    if (context_ptr->magic_number != CONTEXT_MAGIC_NUMBER)
    {
      SECBOOT_LOG_ERR(context_ptr, context_ptr->magic_number, CONTEXT_MAGIC_NUMBER, 0);
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }

    //Check Pointers ,Lengths, Boundary and Integer overflows (wrap around)
    // using <= combines the check for integer overflow and the check of length equal to 0
    if ((image_info_ptr == NULL) ||
        (verified_info_ptr == NULL) ||
        (image_info_ptr->header_ptr_1 == NULL) ||
        ((uintptr_t)image_info_ptr->header_ptr_1 + image_info_ptr->header_len_1 <= (uintptr_t)image_info_ptr->header_ptr_1) ||
        (image_info_ptr->code_ptr_1 == NULL) ||
        ((uintptr_t)image_info_ptr->code_ptr_1 + image_info_ptr->code_len_1 <= (uintptr_t)image_info_ptr->code_ptr_1) ||
        (image_info_ptr->x509_chain_ptr == NULL) ||
        ((uintptr_t)image_info_ptr->x509_chain_ptr + image_info_ptr->x509_chain_len <= (uintptr_t)image_info_ptr->x509_chain_ptr) ||
        (image_info_ptr->x509_chain_len > SECBOOT_MAX_CERT_CHAIN_SIZE) ||
        (image_info_ptr->signature_ptr == NULL) ||
        ((uintptr_t)image_info_ptr->signature_ptr + image_info_ptr->signature_len <= (uintptr_t)image_info_ptr->signature_ptr) ||
        (image_info_ptr->signature_len > (SECBOOT_MAX_KEY_SIZE_IN_BITS/8))
        )
    {
      SECBOOT_LOG_ERR(context_ptr, E_SECBOOT_INVALID_PARAM, 0, 0);
      ret_val = E_SECBOOT_INVALID_PARAM;
      break;
    }

    MEMSET((uint8*)verified_info_ptr, 0, sizeof(secboot_verified_info_type));
    MEMSET((uint8*)&pbl_secx509_cert_list, 0 , sizeof(pbl_secx509_cert_list));
    MEMSET((uint8*)&ou_field_info, 0, sizeof(ou_field_info));
    MEMSET((uint8*)&fuse_info, 0, sizeof(fuse_info));

    /* for secboot boot, rot_disable is by default set to TURE */
    verified_info_ptr->rot_disabled = SECBOOT_ROT_TRANSFER_DISABLE;

    //Update the secboot library version id
    verified_info_ptr->version_id = SECBOOT_VERSION;

    if(!context_ptr->use_supplied_fuses || context_ptr->fuses.use_root_of_trust_only)
    {
      /* Either fuses are not supplied by the caller through secboot_init_fuses() so fuses needs to be read from hw or
         Fuses are supplied by the caller through secboot_init_fuses() but caller wants secboot 
         to use only root of trust from the supplied fuses.so read other fuse values from hw */
      ret_val = secboot_read_hw_fuses(context_ptr, code_segment, &fuse_info);
      BREAKIF(E_SECBOOT_SUCCESS != ret_val);
    }
    else
    {
      /* Copy the fuses that are given by the caller through secboot_init_fuses() which are stored in context */
      MEMCPY((uint8*)(&fuse_info), (uint8*)(&(context_ptr->fuses)), sizeof(fuse_info));
    }

    //parse the certificate chain 
    x509_result = pbl_secx509_parse_cert_buffer(image_info_ptr->x509_chain_ptr,
                                                 image_info_ptr->x509_chain_len,
                                                 &pbl_secx509_cert_list,
                                                 &ou_field_info);
    if (E_X509_SUCCESS != x509_result)
    {
      SECBOOT_LOG_ERR(context_ptr, x509_result, (image_info_ptr->x509_chain_ptr),
                      image_info_ptr->x509_chain_len);
      ret_val = E_SECBOOT_X509_FAIL;
      break;
    }

    /* update msm_hw_id as required by OU fields used in signing. */
    ret_val= secboot_update_msm_hw_id(context_ptr, &ou_field_info, &fuse_info);
    BREAKIF(E_SECBOOT_SUCCESS != ret_val);

    //Ensure the software is what the caller is expecting. If caller is expecting
    //to execute a TZ image, this ensures the images is actually TZ and not SBL
    //Software type is in the lower 32 bits of the certs sw_id OU field
    //Caller could provide 2 software types (in upper 16 bits and lower 16 bits) they'd
    //like us to allow. First type should be populated in the upper 16 bits and second in
    //the lower 16 bits. We check against both types.
    //
    //Note: 2 software types are only called from Apps PBL as it allows either
    //SBL1 or EHOSTDL images. Rather than do something specific to PBL image, kept
    //secboot agnostic of caller and designed this check in generic way
    if ((image_info_ptr->sw_type & 0xFFFF0000) != 0)
    {
      //caller gave us 2 software types to allow
      if (! ( (((uint32)ou_field_info.sw_id) == (image_info_ptr->sw_type >> 16)) ||
              (((uint32)ou_field_info.sw_id) == (image_info_ptr->sw_type & 0xFFFF))
            ))
      {
        SECBOOT_LOG_ERR(context_ptr, ou_field_info.sw_id,
                        (ou_field_info.sw_id >> 32), image_info_ptr->sw_type);
        ret_val = E_SECBOOT_INVALID_SW_TYPE;
        break;
      }      
    }
    else
    {
        //caller gave us 1 software types to allow
        if (((uint32)ou_field_info.sw_id) != image_info_ptr->sw_type)
        {
          SECBOOT_LOG_ERR(context_ptr, ou_field_info.sw_id,
                          (ou_field_info.sw_id >> 32), image_info_ptr->sw_type);
          ret_val = E_SECBOOT_INVALID_SW_TYPE;
          break;
        }
    }

    //Check for anti-rollback i.e image is not older than what is supported
    //Software version is in the upper 32 bits of the certs sw_id OU field
    //Image version number is coming from the cert OU field and we are checking 
    //that it is not less than the SW version defined in the fuse 
    if ((ou_field_info.sw_id >> 32) < (image_info_ptr->sw_version))
    {
      SECBOOT_LOG_ERR(context_ptr, (ou_field_info.sw_id),
                      (ou_field_info.sw_id >> 32),
                      image_info_ptr->sw_version);
      ret_val = E_SECBOOT_INVALID_SW_VERSION;
      break;
    }

    //hash the certificates
    ret_val = secboot_hash_certificates(context_ptr,      
                                        image_info_ptr->x509_chain_ptr,
                                        &pbl_secx509_cert_list);
    BREAKIF(E_SECBOOT_SUCCESS != ret_val);

    //verify the certificate chain along with the root certificate
    ret_val = secboot_verify_cert_chain(context_ptr,
                                        &pbl_secx509_cert_list,
                                        fuse_info.root_of_trust);
    BREAKIF(E_SECBOOT_SUCCESS != ret_val);

    // Ensure the cert chain is for the msm we're verifying
    // the code on. Since hw_id is jtag_id + oem_id + model_id,
    // the oem/model id gets verified as well.
    // Do this only if serial number is not being used for authentication
    // as we don't force OEM's using serial number for authentication to
    // populate the certs OEM_ID/msm_hw_id OU fields.
    if ((fuse_info.auth_use_serial_num == 0) &&
        (ou_field_info.hw_id != fuse_info.msm_hw_id))
    {
      SECBOOT_LOG_ERR(context_ptr, (uint32)ou_field_info.hw_id,
                      (fuse_info.msm_hw_id),
                      (fuse_info.msm_hw_id >> 32));
      ret_val = E_SECBOOT_INVALID_MSM_HW_ID;
      break;
    }

    //verify the image signature
    ret_val = secboot_verify_image_signature(context_ptr,
                                             pbl_secx509_cert_list.cert,
                                             image_info_ptr,
                                             fuse_info.msm_hw_id,
                                             ou_field_info.sw_id,
                                             ou_field_info.code_hash_algo,
                                             &(verified_info_ptr->image_hash_info));
    BREAKIF(E_SECBOOT_SUCCESS != ret_val);

    verified_info_ptr->sw_id = ou_field_info.sw_id;
    verified_info_ptr->msm_hw_id = fuse_info.msm_hw_id;

    // Set debug to disable, as missing debug ou is equal to debug disabled
    verified_info_ptr->enable_debug = SECBOOT_DEBUG_DISABLE;

    // Check if debug needs to be enabled. Debug re-enablement is only allowed
    // if the chip serial number matches the debug serial number value i.e cert is
    // a debug cert. The lower 32 bits of the debug OU field indicate whether debug
    // is to be enabled (1) and the upper 32 bits indicate which chip this debug cert is
    // meant for (chip's serial number matches the upper 32 bits of the debug OU field)
    // i.e if the debug OU field value is 0x1234ABCD00000001, this means enable debug
    // for chip with serial number 0x1234ABCD. If the debug cert is used on a chip with
    // a different serial number, we fail authentication.
    debug_enable =
          ou_field_info.debug_enable & SECBOOT_DEBUG_SETTINGS_FIELD_MASK;
    if (!( (SECBOOT_DEBUG_NOP == debug_enable)     ||
          (SECBOOT_DEBUG_DISABLE == debug_enable) ||
           (SECBOOT_DEBUG_ENABLE == debug_enable) ))
    {
      // Unsupported setting...no arbitrary value is allowed
      SECBOOT_LOG_ERR(context_ptr, ou_field_info.debug_enable, 0, 0);
      ret_val = E_SECBOOT_INVALID_DEBUG;
      break;
    }

    if (SECBOOT_DEBUG_ENABLE == debug_enable)
    {          
      // Debug needs to be re-enabled only if the serial number matches
      // Extract the serial number value from the upper 32 bits of this field
      serial_num = (ou_field_info.debug_enable >> SECBOOT_DEBUG_SERIALNUM_FIELD_SHFT);
      if (serial_num != fuse_info.serial_num)
      {
        // return failure, this debug cert cannot be used for this device
        SECBOOT_LOG_ERR(context_ptr, fuse_info.serial_num, serial_num, 0);
        ret_val = E_SECBOOT_INVALID_DEBUG;
        break;
      }
    }
    //So far, so good save off the debug settings for caller
    verified_info_ptr->enable_debug = debug_enable;

    // check whether crash dump is enabled for the specific phone,
    // if the chip serial number matches the crash dump OU field's serial number value.
    // The lower 32 bits of the debug OU field indicate whether the crash dump
    // is to be enabled (1) and the upper 32 bits indicate which chip this cert is meant
    // for (chip's serial number matches the upper 32 bits of the crash dump OU field)
    // i.e if the crash dump OU field value is 0x1234ABCD00000001, this means enable crash
    // dump for chip with serial number 0x1234ABCD. If the cert is used on a chip with
    // a different serial number, we fail authentication.
    crash_dump_enable =
          ou_field_info.crash_dump_enable & SECBOOT_CRASH_DUMP_SETTINGS_FIELD_MASK;
    if (!( (SECBOOT_CRASH_DUMP_DISABLE == crash_dump_enable) ||
           (SECBOOT_CRASH_DUMP_ENABLE == crash_dump_enable) ))
    {
      // Unsupported setting...no arbitrary value is allowed
      SECBOOT_LOG_ERR(context_ptr, ou_field_info.crash_dump_enable, 0, 0);
      ret_val = E_SECBOOT_INVALID_CRASH_DUMP;
      break;
    }

    if (SECBOOT_CRASH_DUMP_ENABLE == crash_dump_enable)
    {          
      // the crash dump is enabled only if the device's serial number matches
      // the serial number value from the upper 32 bits of this field
      serial_num = (ou_field_info.crash_dump_enable >> SECBOOT_CRASH_DUMP_SERIALNUM_FIELD_SHFT);
      if (serial_num != fuse_info.serial_num)
      {
        // return failure, this crash dump cert cannot be used for this device
        SECBOOT_LOG_ERR(context_ptr, fuse_info.serial_num, serial_num, 0);
        ret_val = E_SECBOOT_INVALID_CRASH_DUMP;
        break;
      }
    }
    //So far, good to save the crash dump settings for caller
    verified_info_ptr->enable_crash_dump = crash_dump_enable;

    // check whether ROT is enabled for the specific phone,
    // if the chip serial number matches the ROT OU field's serial number value.
    // The lower 32 bits of the ROT OU field indicate whether the ROT
    // is to be enabled (1) and the upper 32 bits indicate which chip this cert is meant
    // for (chip's serial number matches the upper 32 bits of the ROT OU field)
    // i.e if the ROT OU field value is 0x1234ABCD00000001, this means enable ROT
    // for chip with serial number 0x1234ABCD. If the cert is used on a chip with
    // a different serial number, we fail authentication.
	ret_val = secboot_check_enable_rot(context_ptr, &ou_field_info, &fuse_info, verified_info_ptr);
	BREAKIF(E_SECBOOT_SUCCESS != ret_val);

    // We've come this far without any errors
    ret_val = E_SECBOOT_SUCCESS;
  } while (0);

  return ret_val;
}

/**
 * @brief See documentation in public header
 *
 */
secboot_error_type secboot_deinit
(
  secboot_handle_type*      secboot_handle_ptr
)
{
  if (secboot_handle_ptr == NULL)
  {
    return E_SECBOOT_DEINIT;
  }

  //Cleanup the handle
  MEMSET((uint8*)secboot_handle_ptr->handle, 0, sizeof(secboot_handle_ptr->handle));

  return E_SECBOOT_SUCCESS;
}

/**
 * @brief See documentation in public header
 *
 */
secboot_error_type secboot_get_ftbl(secboot_ftbl_type* ftbl_ptr)
{
  if (NULL == ftbl_ptr)
    return E_SECBOOT_INVALID_PARAM;

  ftbl_ptr->secboot_init = secboot_init;
  ftbl_ptr->secboot_authenticate = secboot_authenticate;
  ftbl_ptr->secboot_deinit = secboot_deinit;
  ftbl_ptr->secboot_init_fuses = secboot_init_fuses;
  
  return E_SECBOOT_SUCCESS;
}

/**
 * @brief See documentation in public header
 *
 * NOTE:  parameter "fuses" is passed by value to keep backward compatibility
 */
secboot_error_type secboot_init_fuses
(
  secboot_handle_type* secboot_handle_ptr,
  secboot_fuse_info_type fuses  
)
{
  secboot_context_type*        context_ptr = NULL;
  
  if (secboot_handle_ptr == NULL)
  {
    return E_SECBOOT_INVALID_PARAM;    
  }

  context_ptr = (secboot_context_type*) secboot_handle_ptr->handle;

  /* Store the fuses in context */
  MEMCPY((uint8*)(&(context_ptr->fuses)), (uint8*)(&fuses), sizeof(context_ptr->fuses));

  /* Set this variable to TRUE so that fuses will not be read from hw if use_root_of_trust_only is FALSE.
     if use_root_of_trust_only is TRUE then remianing fuses will be read from hw*/
  context_ptr->use_supplied_fuses = TRUE;
  
  return E_SECBOOT_SUCCESS;
}


