/*===========================================================================

                       S E C U R I T Y    S E R V I C E S

                  S E C U R E  B O O T   X.5 0 9  P A R S E R

                                 M O D U L E

FILE:  secboot_x509.c

DESCRIPTION:
    Secure Boot X.509 PARSER

EXTERNALIZED FUNCTIONS


  Copyright (c)2004, 2006, 2008-2015 by Qualcomm, Technologies, Inc. All Rights Reserved.
===========================================================================*/

/*=========================================================================

                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who                  what, where, why
--------   ----                 ------------------------------------------- 
03/24/15   hw                   Add Dynamic Binding feature support
08/05/14   hw                   Add QPSI code review fix and 64bit support
05/28/14   hw                   Added RoT support
08/28/13   hw                   Added retail unlock feature code
03/18/13   DJ,hw                Fix the boundary check for ASN.1 parser
01/22/13   vg,mm                Fixes for MEMCMP and Return error checks
01/28/12   sm                   Fixed lint warnings  
07/21/11   sm                   Reduce cert struct size - removed redundant 
                                array, and change from arrays to ptrs 
11/08/10   sm                   Added assembler support
04/12/10   sm                   Added support for flexible chain size
09/10/09   sm                   Changes to support regional security
03/03/08   ANB                  Port in some of the changes from Raptor.
05/02/06   AMW                  Initial SC2X port.
08/25/04   SJD                  Initial revision.  Original SW from the
                                security team.
06/06/04   John Alayari         1. Modified the error code for the public
                                   functions to use errno_enum_type
                                2. Cleaned up functions to return at end of
                                   functions.
                                3. Added comments and headings to functions
                                   and corrected some indentations to comply
                                   with ASW Security Services Coding
                                   Guideline.
                                4. Removed all references to SSL support.
03/01/04   Sid                  Modified the original code for secure boot

===========================================================================*/


/*==========================================================================

             Include Files for Module

==========================================================================*/
#include "SecBootx509.h"
#include "SecBootI.h"
/*==========================================================================

                             Macro Definitions

===========================================================================*/
#define convert_to_binary(ptr) \
              if ((BLOAD8(ptr)>=0x41)&&(BLOAD8(ptr)<=0x46)) \
                BSTOR8 (ptr , (BLOAD8(ptr)-55)); \
              else if ((BLOAD8(ptr)>=0x30)&&(BLOAD8(ptr)<=0x39)) \
                BSTOR8 (ptr , (BLOAD8(ptr)-48)); \
              else \
                break; \
              BSTOR8 (ptr , (BLOAD8(ptr)&0x0F)); \


/*===========================================================================
MACRO SECX509_NEXT_FIELD

DESCRIPTION
  This macro parses the next data field in the certificate.  If there is an
  error parsing the field, the ret_status will be set and the current scope
  will be broken from

DEPENDENCIES
  To call this macro a secerrno_enum_type ret_status must be defined and
  this must be called within a loop or switch statement so it can break
  from execution.

PARAMETERS
  data_ptr - pointer to the next certificate data and its length
  ret_ptr  - pointer to the inner certificate field that is to be parsed
  tag_id   - tag value to verifify the inner tag is proper

SIDE EFFECTS
  If no error occurs during processing the data_ptr->data will be advanced
  to the next field and data_ptr->len will be decremented by the amount the
  pointer was increased.  If an errror occurs in parsing then data_ptr will
  remain unchanged.
===========================================================================*/
#define SECX509_NEXT_FIELD( data_ptr, ret_ptr, tag_id ) \
  if ( pbl_secasn1_next_field( data_ptr, ret_ptr, tag_id ) \
       != E_ASN1_SUCCESS ) { \
        ret_status = E_X509_DATA_INVALID; \
        break; \
    }

/*===========================================================================
MACRO SECX509_OPEN_SEQUENCE

DESCRIPTION
  This macro parses open the next ASN.1 sequence of values, all this macro
  does is call SECX509_NEXT_FIELD with a sequence tag, but it has been
  defined so there should always be matching SECX509_OPEN_SEQUENCE and
  SECX509_CLOSE_SEQUENCE

DEPENDENCIES
  To call this macro a secerrno_enum_type ret_status must be defined and
  this must be called within a loop or switch statement so it can break
  from execution.

PARAMETERS
  outer - the outer pointer containing the data to be parsed
  inner - the inner pointer which will hold the parsed data

SIDE EFFECTS
  If no error occurs during processing the outer->data will be advanced
  to the next field and outer->len will be decremented by the amount the
  pointer was increased.  If an errror occurs in parsing then outer will
  remain unchanged.
===========================================================================*/
#define SECX509_OPEN_SEQUENCE( outer, inner ) \
    SECX509_NEXT_FIELD( outer, inner, SECASN1_SEQUENCE_TYPE )

/*===========================================================================
MACRO SECX509_CLOSE_SEQUENCE

DESCRIPTION
  This macro verifies that the end of

DEPENDENCIES
  To call this macro a secerrno_enum_type ret_status must be defined and
  this must be called within a loop or switch statement so it can break
  from execution.

PARAMETERS
  outer - the outer pointer containing the data to be parsed
  inner - the inner pointer which will hold the parsed data

SIDE EFFECTS
  If no error occurs during processing the outer->data will be advanced
  to the next field and outer->len will be decremented by the amount the
  pointer was increased.  If an errror occurs in parsing then outer will
  remain unchanged.
===========================================================================*/
#define SECX509_CLOSE_SEQUENCE( outer, inner ) \
    if ( pbl_secasn1_close_sequence( outer, inner ) != E_ASN1_SUCCESS ) { \
        ret_status = E_X509_DATA_INVALID; \
        break; \
    }

/*===========================================================================
MACRO SECX509_UPDATE_OU_FIELD_VALUE

DESCRIPTION
  This macro check the status after the pbl_secx509_parse_ou_field call: 
  if ou field is found, it updates the ou field variable in the assignment; 
  if ou field is not found, it updates the status to success, not an error;
  if any error, it breaks out the loop with the error code.

DEPENDENCIES
  this must be called within a loop or switch statement so it can break
  from execution.

PARAMETERS
  ou_field_value_to_update - the variable to update
  parsed_value - the value to be assigned to the variable

SIDE EFFECTS
  NONE
===========================================================================*/
#define SECX509_UPDATE_OU_FIELD_VALUE(status, ou_field_value_to_update, parsed_value) \
    if (E_X509_SUCCESS == status) \
    { \
      ou_field_value_to_update = parsed_value; \
    } \
    else if (E_X509_OU_FIELD_NOT_FOUND == status) \
    { \
      status = E_X509_SUCCESS; \
    } \
    else \
    { \
      break; \
    }
/*=========================================================================

                           Constant Declaration

==========================================================================*/
#define SECX509_SECONDS_PER_YEAR        31536000

/* ASN.1 Tag Ids */
#define SECX509_VERSION_TAG             (0xA0)
#define SECX509_ISSUER_ID_TAG           (0x81)
#define SECX509_SUBJECT_ID_TAG          (0x82)
#define SECX509_EXTENSION_TAG           (0xA3)

#define SECX509_AUTH_KID_TAG            (0x80)
#define SECX509_AUTH_ISS_TAG            (0xA1)
#define SECX509_AUTH_SER_TAG            (0x82)
#define SECX509_AUTH_ISS_NAME_TAG       (0xA4)
#define OU_FIELD_VALUE_STRING_LEN       (16)
#define OU_FIELD_VALUE_STRING_SPACE_LEN (OU_FIELD_VALUE_STRING_LEN + 1) //1 is for the space char. 

#define SECX509_OU_FIELD_VALUE_MAX_BYTES          (0x10) /* the max supported length for parsing OU field value string */
#define SECX509_OU_SHIFT_BIT_NUM_FOR_BYTE_IN_CERT (0x4) /* the number of bits to shift for converting char to int */
#define SECX509_OU_FIELD_INDEX_LEN                (0x2) /* the length of chars for the OU field index */
#define SECX509_OU_FIELD_SPACE_LEN                (0x1) /* the length of the space char*/

/* X.509 Certificate Objects */
#define SECX509_ASN1_OBJ_UNIT_NAME \
  {0x55, 0x04, 0x0B }

#define SECX509_ASN1_OBJ_ORG_NAME \
  {0x55, 0x04, 0x0A }

#define SECX509_ASN1_OBJ_COMMON_NAME \
  {0x55, 0x04, 0x03 }

#define SECX509_ASN1_OBJ_RSA \
  {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01 }

#define SECX509_ASN1_OBJ_SHA1WITHRSAENCRYPTION \
  {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x05 }

#define SECX509_ASN1_OBJ_SHA256WITHRSAENCRYPTION \
  {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B }

/* X.509 Extension Objects */
#define SECX509_EXT_AUTH_KEY_ID \
  {0x55, 0x1D, 0x23 }

#define SECX509_EXT_SUB_KEY_ID \
  {0x55, 0x1D, 0x0E }

#define SECX509_EXT_KEY_USAGE \
  {0x55, 0x1D, 0x0F }

#define SECX509_EXT_SUB_ALT_NAME \
  {0x55, 0x1D, 0x11 }

#define SECX509_EXT_BASIC_CONSTRAINT \
  {0x55, 0x1D, 0x13 }

#define SECX509_EXT_NAME_CONSTRAINT \
  {0x55, 0x1D, 0x1E }

#define SECX509_EXT_POLICY_CONSTRAINT \
  {0x55, 0x1D, 0x24 }

#define SECX509_EXT_EX_KEY_USAGE \
  {0x55, 0x1D, 0x25 }

#define SECX509_EXT_CERT_POLICIES \
  {0x55, 0x1D, 0x20 }

/* Extended Key Usage Objects */
#define SECX509_EX_KEY_SAUTH \
  {0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01 }

#define SECX509_EX_KEY_CAUTH \
  {0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01 }

#define SECX509_EX_KEY_CODE \
  {0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x03 }

#define SECX509_EX_KEY_EMAIL \
  {0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x04 }

#define SECX509_EX_KEY_TIME \
  {0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x08 }

#define SECX509_EX_KEY_MS_SGC \
  {0x2B, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x0A, 0x03, 0x03 }

#define SECX509_EX_KEY_NS_SGC \
  {0x60, 0x86, 0x48, 0x01, 0x86, 0xF8, 0x42, 0x04, 0x01 }

#define SECX509_ASN1_OBJ_SHA1 \
  { 0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2B, 0x0E, 0x03, 0x02, \
    0x1A, 0x05, 0x00, 0x04, 0x14 }

#define SECX509_ASN1_OBJ_SHA256 \
  { 0x30, 0x31, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, \
    0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20 }

#define SECX509_ASN1_OBJ_SHA384 \
  { 0x30, 0x41, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, \
    0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30 }

#define SECX509_ASN1_OBJ_SHA512 \
  { 0x30, 0x51, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, \
    0x65, 0x03, 0x04, 0x02, 0x03, 0x05, 0x00, 0x04, 0x40 }

/* ---------------------------------------------------------------------- */
/*                           String Declaration                           */
/* ---------------------------------------------------------------------- */
/* ASN.1 Objects for keys */
static const uint8 pbl_secx509_asn1_oid_obj_unit[] = SECX509_ASN1_OBJ_UNIT_NAME;
static const uint8 pbl_secx509_asn1_rsa[] = SECX509_ASN1_OBJ_RSA;
static const uint8 pbl_secx509_sha1WithRSAEncryption[] =
SECX509_ASN1_OBJ_SHA1WITHRSAENCRYPTION;
static const uint8 pbl_secx509_sha256WithRSAEncryption[] =
SECX509_ASN1_OBJ_SHA256WITHRSAENCRYPTION;

/* ASN.1 Objects for Extensions */
static const uint8 pbl_secx509_ext_auth_key_id[] = SECX509_EXT_AUTH_KEY_ID;
static const uint8 pbl_secx509_ext_sub_key_id[] = SECX509_EXT_SUB_KEY_ID;
static const uint8 pbl_secx509_ext_key_usage[] = SECX509_EXT_KEY_USAGE;
static const uint8 pbl_secx509_ext_sub_alt_name[] = SECX509_EXT_SUB_ALT_NAME;
static const uint8 pbl_secx509_ext_basic_constraint[] = 
  SECX509_EXT_BASIC_CONSTRAINT;
static const uint8 pbl_secx509_ext_name_constraint[] = 
  SECX509_EXT_NAME_CONSTRAINT;
static const uint8 pbl_secx509_ext_policy_constraint[] =
                                                SECX509_EXT_POLICY_CONSTRAINT;
static const uint8 pbl_secx509_ext_ex_key_usage[] = SECX509_EXT_EX_KEY_USAGE;
static const uint8 pbl_secx509_ext_cert_policies[] = SECX509_EXT_CERT_POLICIES;
/* ASN.1 Object for Extended Key Usage */
static const uint8 pbl_secx509_ex_key_sauth[] = SECX509_EX_KEY_SAUTH;
static const uint8 pbl_secx509_ex_key_cauth[] = SECX509_EX_KEY_CAUTH;
static const uint8 pbl_secx509_ex_key_code[] = SECX509_EX_KEY_CODE;
static const uint8 pbl_secx509_ex_key_email[] = SECX509_EX_KEY_EMAIL;
static const uint8 pbl_secx509_ex_key_time[] = SECX509_EX_KEY_TIME;
static const uint8 pbl_secx509_ex_key_ms_sgc[] = SECX509_EX_KEY_MS_SGC;
static const uint8 pbl_secx509_ex_key_ns_sgc[] = SECX509_EX_KEY_NS_SGC;

static const uint8 debug_string[]="DEBUG";
#define OU_DEBUG_STRING_LEN (0x5)
#define OU_DEBUG_VALUE_LEN (0x10)
static const uint8 crash_dump_string[]="CRASH_DUMP";
#define OU_CRASH_DUMP_STRING_LEN (0x0A)
#define OU_CRASH_DUMP_VALUE_LEN (0x10)
static const uint8 rot_string[]="ROT_EN";
#define OU_ROT_STRING_LEN (0x6)
#define OU_ROT_VALUE_LEN (0x10)
static const uint8 use_serial_num_string[]="USE_SERIAL_NUMBER_IN_SIGNING";
#define OU_USE_SERIAL_NUM_STRING_LEN (0x1C)
#define OU_USE_SERIAL_NUM_VALUE_LEN (0x04)
//The OEM_ID value is also in the HW_ID field
//(for those OEM's who use OEM ID for signing),
//so we don't explicity need to parse this field out
//static const uint8 oem_string[]="OEM_ID";
static const uint8 sw_id_string[]="SW_ID";
#define OU_SW_ID_STRING_LEN (0x5)
#define OU_SW_ID_VALUE_LEN (0x10)
static const uint8 hw_id_string[]="HW_ID";
#define OU_HW_ID_STRING_LEN (0x5)
#define OU_HW_ID_VALUE_LEN (0x10)
static const uint8 sha1_codehash_str[] = "SHA1";
#define OU_SHA1_STRING_LEN (0x4)
#define OU_SHA1_VALUE_LEN (0x4)
static const uint8 sha256_codehash_str[] = "SHA256";
#define OU_SHA256_STRING_LEN (0x6)
#define OU_SHA256_VALUE_LEN (0x4)
static const uint8 in_use_soc_hw_version_string[] = "IN_USE_SOC_HW_VERSION";
#define OU_IN_USE_SOC_HW_VER_STRING_LEN (0x15)
#define OU_IN_USE_SOC_HW_VER_VALUE_LEN (0x4)

/* ---------------------------------------------------------------------- */
/*                   Forward Function Declaration                         */
/* ---------------------------------------------------------------------- */
secx509_errno_enum_type pbl_secx509_parse_version
(
  secasn1_data_type*     data_ptr,
  pbl_secx509_version_type*  ver_ptr
);

secx509_errno_enum_type pbl_secx509_parse_validity
(
  secasn1_data_type*       data_ptr,
  pbl_secx509_cert_info_type*  cert
);

secx509_errno_enum_type pbl_secx509_parse_dn
(
  secasn1_data_type*           data_ptr,
  pbl_secx509_dn_type*         dn,
  secx509_ou_field_info_type*  ou_field_info_ptr
);

secx509_errno_enum_type pbl_secx509_parse_pub_key
(
  secasn1_data_type*    data_ptr,
  pbl_secx509_pubkey_type*  ptr
);

secx509_errno_enum_type pbl_secx509_parse_unique_id
(
  secasn1_data_type*        data_ptr,
  secasn1_bit_string_type*  bt,
  uint8                     op_tag_id
);

secx509_errno_enum_type pbl_secx509_parse_extensions
(
  secasn1_data_type*       data_ptr,
  pbl_secx509_cert_info_type*  cert,
  uint8                    op_tag_id
);

secx509_errno_enum_type pbl_secx509_parse_signature
(
  secasn1_data_type*  data_ptr,
  pbl_secx509_cert_type*  cert
);

secx509_errno_enum_type pbl_secx509_parse_sig_algorithm
(
  secasn1_data_type*      data_ptr,
  pbl_secx509_sig_algo_type*  algo
);

int pbl_secx509_check_issued
(
  const pbl_secx509_cert_info_type*  issuer,
  const pbl_secx509_cert_info_type*  subject
);

int pbl_secx509_check_purpose
(
  const pbl_secx509_cert_info_type*  cert,
  int                      id,
  int                      ca
);

static secx509_errno_enum_type pbl_secx509_check_oid_type
(
  secasn1_data_type           *data_ptr,
  const uint8                 *oid_type_ptr,
  uint32                      oid_type_len,
  boolean                     *found_oid_type
);

static secx509_errno_enum_type pbl_secx509_parse_ou_field
(
  secasn1_data_type           *data_ptr,
  uint64                      *ou_field_value,
  uint32                      ou_field_value_len,
  const uint8                 *ou_field_string,
  uint32                      ou_field_string_len
);

/* This function is not used because we need to guarantee 32bit accesses. */
#ifdef IMEM_BYTE_ACCESS
 <
 Simple util function to compare two buffers byte per byte ...
 should be useful later on
uint16 pbl_secutil_byte_compare
(
  const uint8*  value1_ptr,
  const uint8*  value2_ptr,
  const uint16  byte_len
)
{
  int   i;
  /*-----------------------------------------------------------------------*/

  for ( i=0; i<byte_len; i++ )
  {
    if ( *(value1_ptr + i) != *(value2_ptr + i) ) return (uint16)(i+1);
  }

  return 0;
} /* end pbl_secutil_byte_compare() */
#endif

/*===========================================================================

FUNCTION SECX509_PARSE_CERTIFICATE

DESCRIPTION
  Parses an X509 certificate from the data_ptr, if *cert == NULL then the
  certificate is allocated from the memory pool, if the certificate is
  already allocated then the function just uses the pre-allocated memory

DEPENDENCIES
  None

PARAMETERS
  data_ptr          - pointer to the raw certificate data
  data_bound        - address beyond which the certificate does not cross into
  cert              - pointer to the certificate
  ou_field_info_ptr - pointer to store OU Field values

RETURN VALUE
  E_X509_SUCCESS if the certificate is parsed properly
  E_X509_DATA_INVALID if the certificate cannot be parsed properly
  E_X509_NO_MEMORY if no more memory slots are available for the certs
  E_X509_NOT_SUPPORTED if an algorithm found in the cert is not supported
  E_X509_INVALID_ARG if a pointer argument is NULL
  E_X509_FAILURE if the *cert is pre-allocated but not *cert->cert or if the
            certificate data length does not match cert_len

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_certificate
(
  const uint8*                 data_ptr,
  const uint8*                 data_bound,
  pbl_secx509_cert_type*       main_cert_ptr,
  secx509_ou_field_info_type*  ou_field_info_ptr
)
{
  pbl_secx509_cert_info_type*  cert_ptr;      /* X509 certificate pointer   */
  secasn1_data_type        cert_data;     /* data holder for the cert data  */
  secasn1_data_type        tbs_cert_data; /* tbs cert data holder           */
  secx509_errno_enum_type       ret_status = E_X509_SUCCESS; /* return status    */
  /*-----------------------------------------------------------------------*/

  /* Sanity Check on pointer arguments */
  /* No need to check data_bound for NULL, since it is 
     never dereferenced.  */ 
  if ( (main_cert_ptr == NULL) || (data_ptr == NULL) ||
          !CHECK_DATA_BOUND(data_ptr, 0, data_bound) )
  {
    return E_X509_INVALID_ARG;
  }

  cert_ptr = &(main_cert_ptr->cert_info);

  /* Loop so we can break when an error occurs */
  do
  {
    /* This call will fail if data_ptr > data_bound */
    if ( pbl_secasn1_start( data_ptr, data_bound, &cert_data ) != 
          E_ASN1_SUCCESS 
       )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }
    /* A post condition of a successful call to pbl_secasn1_start is
       that cert_data is valid */
    /* Set where the certificate information starts */
    main_cert_ptr->cinf_offset = (cert_data.data - data_ptr);
    if (main_cert_ptr->cinf_offset > SECASN1_MAX_LEN)
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* record the size */
    main_cert_ptr->asn1_size_in_bytes = cert_data.len +
          main_cert_ptr->cinf_offset;
    /* check integer overflow and SECASN1 boundary*/
    if ( (main_cert_ptr->asn1_size_in_bytes < cert_data.len) || 
         (main_cert_ptr->asn1_size_in_bytes > SECASN1_MAX_LEN) )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    SECX509_OPEN_SEQUENCE( &cert_data, &tbs_cert_data );

    /* Parse the version number */
    if ( pbl_secx509_parse_version( &tbs_cert_data, &cert_ptr->version )
         != E_X509_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Parse the serial number */
    SECX509_NEXT_FIELD( &tbs_cert_data,
                        &cert_ptr->serial_number,
                        SECASN1_INTEGER_TYPE );

    /* Parse the signature since we check with outside field */
    if ( pbl_secx509_parse_sig_algorithm( &tbs_cert_data,
                                      &cert_ptr->algorithm.algo_id )
         != E_X509_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Parse the Issuers Distinguished Name */
    if ( pbl_secx509_parse_dn( &tbs_cert_data, &cert_ptr->issuer, NULL ) !=
         E_X509_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Parse the Validity */
    if ( pbl_secx509_parse_validity( &tbs_cert_data, cert_ptr ) !=
         E_X509_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Parse the Subject Distinguished Name */
    if ( pbl_secx509_parse_dn( &tbs_cert_data, &cert_ptr->subject, ou_field_info_ptr ) !=
         E_X509_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Parse Subject Public Key Info */
    if ( pbl_secx509_parse_pub_key( &tbs_cert_data, &main_cert_ptr->pkey )
         != E_X509_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    if ( main_cert_ptr->pkey.algo != SECX509_PUBKEY_RSA )
    {
      ret_status = E_X509_NOT_SUPPORTED;
      break;
    }

    /* Initialize extension data */
    cert_ptr->extension.set = FALSE;
    cert_ptr->extension.path_len = -1;

    /* For Version 1 (value 0), only basic fields are present
       For Version 2 (value 1), no extensions are present, but a UniqueIdentifier is present
       For Version 3 (value 2), extensions are used
    */
    if ( cert_ptr->version.ver > 0 )
    {

      /* Version 2 or above, look for Optional Information */

      /* Attempt to parse Issuer Unique Id */
      if ( pbl_secx509_parse_unique_id( &tbs_cert_data,
                                    &cert_ptr->issuer_unique_id,
                                    SECX509_ISSUER_ID_TAG )
           != E_X509_SUCCESS )
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }

      /* Attempt to parse Subject Unique Id */
      if ( pbl_secx509_parse_unique_id( &tbs_cert_data,
                                    &cert_ptr->subject_unique_id,
                                    SECX509_SUBJECT_ID_TAG )
           != E_X509_SUCCESS )
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }

      if ( cert_ptr->version.ver == 2 )
      {
        /* 2 means version 3, the version number is 0 based */
        /* Version 3, check for extensions */
        if ( pbl_secx509_parse_extensions( &tbs_cert_data,
                                       cert_ptr,
                                       SECX509_EXTENSION_TAG )
             != E_X509_SUCCESS )
        {
          ret_status = E_X509_DATA_INVALID;
          break;
        }
      }

    } /* cert_ptr->version.ver > 0 */

    SECX509_CLOSE_SEQUENCE( &cert_data, &tbs_cert_data );

    /* These two checks verify that the two subtractions below will not overflow. */
    if ((uintptr_t)cert_data.data < (uintptr_t)data_ptr ||
	((uintptr_t)cert_data.data - (uintptr_t)data_ptr) < main_cert_ptr->cinf_offset)
    {
      ret_status = E_X509_DATA_INVALID;
      break;      
    }

    /* Calculate the byte length of the actual certificate data */
    /* By the above tests, this arithmetic is safe */
    main_cert_ptr->cinf_byte_len = ((uintptr_t)cert_data.data - (uintptr_t)data_ptr) -
      main_cert_ptr->cinf_offset;
    /* Check that our size limits are not violated */
    if (main_cert_ptr->cinf_byte_len > SECASN1_MAX_LEN) {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Parse the signature */
    if ( pbl_secx509_parse_signature( &cert_data, main_cert_ptr ) !=
         E_X509_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    if ( cert_ptr->algorithm.algo_id >= SECX509_SIG_ALGO_MAX )
    {
      ret_status = E_X509_NOT_SUPPORTED;
      break;
    }

    /* Check to make sure the parsing has looked at the entire certificate */
    if ( pbl_secasn1_end( &cert_data ) != E_ASN1_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

  }/*lint -e(717) */ while (FALSE);

  return ret_status;
} /* pbl_secx509_parse_certificate */

/* ---------------------------------------------------------------------- */
/*                       Internal Functions                               */
/* ---------------------------------------------------------------------- */
/*===========================================================================

FUNCTION SECX509_PARSE_GEN_NAME_FOR_DIR

DESCRIPTION
  Parses a general name structure for a directory name.  Since the
  general name structure is a list we will use the first directory
  name we find.  If there are more then one directory names, the others
  will be ignored.  If there are no directory names present name will
  contain an empty DN with 0 attributes.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  name     - where the DN from the directory name will be stored

RETURN VALUE
  E_X509_SUCCESS - if the general name is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_gen_name_for_dir
(
  secasn1_data_type *data_ptr,
  pbl_secx509_dn_type *name
)
{
  secasn1_data_type tag;                          /* tag data holder       */
  secasn1_err_type err;                           /* ASN.1 error code      */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS;      /* Return Status    */

  /* Sanity Check on pointer arguments */
  if ( (name == NULL) || (data_ptr == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  /*-----------------------------------------------------------------------*/
  /* Initialize the name, so if we don't find a directory name */
  /* it will be set correctly */
  name->num_attrib = 0;

  while (data_ptr->len > 0)
  {
    err = pbl_secasn1_next_field( data_ptr, &tag, SECX509_AUTH_ISS_NAME_TAG );
    if ( err == E_ASN1_SUCCESS )
    {
      /* Found a directory name parse it */
      ret_status = pbl_secx509_parse_dn( &tag, name, NULL );
      break;
    }
    else if ( err >= E_ASN1_INVALID_DATA)
    {
      //We allow E_ASN1_INVALID_TAG or E_ASN1_NO_DATA as this is an optional field
      ret_status = E_X509_DATA_INVALID;
      break;
    }

  }

  /* We may not have found a directory name, but that is still OK, since */
  /* there may not be one */
  return ret_status;

} /* pbl_secx509_parse_gen_name_for_dir */

/*===========================================================================

FUNCTION SECX509_PARSE_AUTH_KEY_ID

DESCRIPTION
  Parse an Authority Key Identifier.  This extension should
  never be marked critical so the certificate will be marked as invalid
  if this extension is marked critical.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  akid     - where the returned authority key id is stored
  critical - whether the extension is marked critical

RETURN VALUE
  E_X509_SUCCESS - if the authority key id is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_auth_key_id
(
  secasn1_data_type *data_ptr,
  pbl_secx509_auth_key_id_type *akid,
  boolean critical
)
{
  secasn1_data_type seq;                          /* sequence holder       */
  secasn1_data_type data;                         /* data holder           */
  secasn1_err_type  err;                          /* ASN.1 error code      */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
  /*-----------------------------------------------------------------------*/
  //SECBOOT_PRINT( "X509: Parsing Authority Key Identifier ..." );

  do
  {
    /* Rules are taken from RFC 2459 sec 4.2.1.1 */
    if ( critical || akid == NULL )
    {
      /* Cannot be marked critical*/
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    akid->set = TRUE;

    SECX509_OPEN_SEQUENCE( data_ptr, &seq );

    /* These 3 tags are optional fields per the spec */

    /* Try to parse the key identifier */
    err = pbl_secasn1_next_field( &seq,
                              &akid->key_id,
                              SECX509_AUTH_KID_TAG );

    //We allow E_ASN1_INVALID_TAG or E_ASN1_NO_DATA as this is an optional field
    if ( err >= E_ASN1_INVALID_DATA ) 
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Try to parse the authority cert issuer */
    err = pbl_secasn1_next_field( &seq, &data, SECX509_AUTH_ISS_TAG );

    if ( err == E_ASN1_SUCCESS )
    {
      if ( pbl_secx509_parse_gen_name_for_dir( &data, &akid->name )
           != E_X509_SUCCESS )
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }
    }
    //We allow E_ASN1_INVALID_TAG or E_ASN1_NO_DATA as this is an optional field
    else if ( err >= E_ASN1_INVALID_DATA ) 
    {
      return E_X509_DATA_INVALID;
    }

    /* Try to parse the authority certificate serial number */
    err = pbl_secasn1_next_field( &seq, &akid->serial_number,
                               SECX509_AUTH_SER_TAG );
    //We allow E_ASN1_INVALID_TAG or E_ASN1_NO_DATA as this is an optional field
    if ( err >= E_ASN1_INVALID_DATA)
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    SECX509_CLOSE_SEQUENCE( data_ptr, &seq );

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;
} /* pbl_secx509_parse_auth_key_id */

/*===========================================================================

FUNCTION SECX509_PARSE_BASIC_CONSTRAINT

DESCRIPTION
  Parses a basic constraint extension, the basic contraint holds two pieces
  of key information:
  1) Whether the certificate is a CA certificate (End Entity)
  2) if it is a CA certificate then path length is the maximum number of
     CA certificates that may follow

  The default value for CA is FALSE meaning that the path length does not
  matter and it is set to -1

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  ext_ptr  - where the returned information will be held

RETURN VALUE
  E_X509_SUCCESS - if the basic constraint is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_basic_constraint
(
  secasn1_data_type *data_ptr,
  pbl_secx509_ext_type *ext_ptr
)
{
  secasn1_data_type   seq;                        /* sequence holder       */
  secasn1_data_type   data;                       /* data holder           */
  boolean             ca;                         /* whether it is a CA    */
  secasn1_err_type    err;                        /* ASN.1 error code      */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
  /*-----------------------------------------------------------------------*/

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (ext_ptr == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  do
  {
    SECX509_OPEN_SEQUENCE( data_ptr, &seq );

    /* Check that the sequence is not empty */
    if ( seq.len == 0 )
    {
      ext_ptr->ca.set = TRUE;
      ext_ptr->ca.val = FALSE;
      ext_ptr->path_len = -1;
    }
    else
    {

      err = pbl_secasn1_next_field( &seq, &data, SECASN1_BOOLEAN_TYPE );

      if ( err == E_ASN1_SUCCESS )
      {
        if ( pbl_secasn1_decode_boolean( &data, &ca ) != E_ASN1_SUCCESS )
        {
          ret_status = E_X509_DATA_INVALID;
          break;
        }
      }
      else if ( err == E_ASN1_INVALID_TAG )
      {
        ca = FALSE;
      }
      else
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }

      /* Set the CA value */
      ext_ptr->ca.set = TRUE;
      ext_ptr->ca.val = ca;

      err = pbl_secasn1_next_field( &seq, &data, SECASN1_INTEGER_TYPE );

      if ( err == E_ASN1_SUCCESS )
      {
        // BLOAD for only one byte.
        if (data.len != 1)
        {
          ret_status = E_X509_DATA_INVALID;
          break;
        }
        else
        {
          ext_ptr->path_len = BLOAD8(data.data);
        }
      }
      else if (( err == E_ASN1_INVALID_TAG) || (err == E_ASN1_NO_DATA ))
      {
        ext_ptr->path_len = -1;
      }
      else
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }

    }

    SECX509_CLOSE_SEQUENCE( data_ptr, &seq );

  }/*lint -e(717) */ while ( FALSE );

  /* return E_X509_SUCCESS; ?? must return correct return status !! - SID */
  return ret_status;

} /* pbl_secx509_parse_basic_constraint */

/*===========================================================================

FUNCTION SECX509_PARSE_KEY_USAGE

DESCRIPTION
  Parses a key usage extension.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  val      - where the key usage bits are returned

RETURN VALUE
  E_X509_SUCCESS - if the key usage is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_key_usage
(
  secasn1_data_type *data_ptr,
  uint32 *val
)
{
  secasn1_data_type dat;                          /* data holder           */
  secasn1_bit_string_type bt;                     /* bit string holder     */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
  /*-----------------------------------------------------------------------*/

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (val == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  do
  {
    SECX509_NEXT_FIELD( data_ptr, &dat, SECASN1_BIT_STRING_TYPE );

    if ( pbl_secasn1_decode_bit_string( &dat, &bt ) != E_ASN1_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Looking at the spec from rfc2459 and examples this is what */
    /* I think is occuring. If there are 2 bytes in the bit string*/
    /* then there should be 7 unused bits, but if there is only   */
    /* one byte, the unused bits can be ignored with the 0 bit    */
    /* being the first bit */

    /* can we read the first byte? */
    if (bt.len < 1) {
      ret_status = E_X509_DATA_INVALID;
      break;
    }
    *val = BLOAD8(bt.data) << 1;
    if ( bt.len == 2 )
    {
      bt.data++;
      if ( BLOAD8(bt.data) & 0x80L )
      {
        *val |= 0x01L;
      }
    }

  }/*lint -e(717) */ while ( FALSE );

  /* return E_X509_SUCCESS; must return correct status ! - SID*/
  return ret_status;

} /* pbl_secx509_parse_key_usage */

/*===========================================================================

FUNCTION SECX509_PARSE_EX_KEY_USAGE

DESCRIPTION
  Parses an extended key usage extension.  This is a sequence of OIDs that
  give specific key usages, but this function and RFC 2459 only looks for the
  following extended key usages:

    TLS Web Server authentication, TLS Web Client Authentication, Downloadable
    Code Signing, Email Protection, Time Stamping and Secured Gated Crypto.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  val      - where the returned extended key information will be held

RETURN VALUE
  E_X509_SUCCESS - if the version is extended key extension is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_ex_key_usage
(
  secasn1_data_type *data_ptr,
  uint32 *val
)
{
  secasn1_data_type seq;                          /* sequence holder       */
  secasn1_data_type oid;                          /* object Id holder      */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
  /*-----------------------------------------------------------------------*/

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (val == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  do
  {
    SECX509_OPEN_SEQUENCE( data_ptr, &seq );

    *val = 0;

    while ( seq.len > 0 )
    {
      SECX509_NEXT_FIELD( &seq, &oid, SECASN1_OID_TYPE );


      if ( (sizeof(pbl_secx509_ex_key_sauth) == oid.len) &&
            (!MEMCMP( pbl_secx509_ex_key_sauth,
                          oid.data,
                          oid.len ) ) )
      {
        /* Server Authentication */
        *val |= SECX509_EX_KEY_USAGE_SAUTH;
      }
      else if ( (sizeof(pbl_secx509_ex_key_cauth) == oid.len) &&
                (!MEMCMP( pbl_secx509_ex_key_cauth,
                               oid.data,
                               oid.len ) ) )
      {
        /* Client Authentication */
        *val |= SECX509_EX_KEY_USAGE_CAUTH;
      }
      else if ( (sizeof(pbl_secx509_ex_key_code) == oid.len) &&
                (!MEMCMP( pbl_secx509_ex_key_code,
                               oid.data,
                               oid.len ) ) )
      {
        /* Downloadable Code Signing */
        *val |= SECX509_EX_KEY_USAGE_CODE;
      }
      else if ( (sizeof(pbl_secx509_ex_key_email) == oid.len) &&
                (!MEMCMP( pbl_secx509_ex_key_email,
                               oid.data,
                               oid.len ) ) )
      {
        /* Email Proctection */
        *val |= SECX509_EX_KEY_USAGE_EMAIL;
      }
      else if ( (sizeof(pbl_secx509_ex_key_time) == oid.len) &&
                (!MEMCMP( pbl_secx509_ex_key_time,
                               oid.data,
                               oid.len ) ) )
      {
        /* Time Stamping */
        *val |= SECX509_EX_KEY_USAGE_TIME;
      }
      else if ( (sizeof(pbl_secx509_ex_key_ns_sgc) == oid.len) &&
                (!MEMCMP( pbl_secx509_ex_key_ns_sgc,
                               oid.data,
                               oid.len ) ) )
      {
        /* Secured Gated Crypto */
        *val |= SECX509_EX_KEY_USAGE_SGC;
      }
      else if ( (sizeof(pbl_secx509_ex_key_ms_sgc) == oid.len) &&
                (!MEMCMP( pbl_secx509_ex_key_ms_sgc,
                               oid.data,
                               oid.len ) ) )
      {
        /* Secured Gated Crypto */
        *val |= SECX509_EX_KEY_USAGE_SGC;
      }
    }

    if ( ret_status != E_X509_SUCCESS )
    {
      break;
    }

    SECX509_CLOSE_SEQUENCE( data_ptr, &seq );

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;

} /* pbl_secx509_parse_ex_key_usage */

/*===========================================================================

FUNCTION SECX509_PARSE_VERSION

DESCRIPTION
  Parse the version of the certificate.  Valid version values are
  0, 1, 2 mapping to version 1, 2, 3.  If the version is not specified
  the default value is version 1.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  ver_ptr  - where the returned version information will be held

RETURN VALUE
  E_X509_SUCCESS - if the version is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_version
(
  secasn1_data_type *data_ptr,
  pbl_secx509_version_type *ver_ptr
)
{
  secasn1_data_type d1;                           /* Temporary Data Holder */
  secasn1_err_type err;                           /* ASN.1 error type      */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
  /*-----------------------------------------------------------------------*/

  //SECBOOT_PRINT( "X509: Parsing Version ...");

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (ver_ptr == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  /* Enter a new scope for macro use */
  do
  {
    err = pbl_secasn1_next_field( data_ptr, &d1, SECX509_VERSION_TAG );
    if ( err == E_ASN1_SUCCESS )
    {
      /* Found Version Number */
      SECX509_NEXT_FIELD( &d1, &ver_ptr->val, SECASN1_INTEGER_TYPE );
      /* post condition of pbl_secasn1_next_field() is that the return value
	 is readable out through its length */

      /* Check to see that the version is only 1 byte long */
      if ( ver_ptr->val.len == 1 )
      {
        ver_ptr->ver = BLOAD8(ver_ptr->val.data);
      }
      else
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }
    }
    else 
    {
      /* Version not specified */
      ver_ptr->ver = 0;
      ret_status = E_X509_DATA_INVALID;
    }

    if ( ver_ptr->ver > 2 )
    {
      /* An invalid version number */
      ret_status = E_X509_DATA_INVALID;
      break;
    }

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;
} /* pbl_secx509_parse_version */

/*===========================================================================

FUNCTION SECX509_PARSE_VALIDITY

DESCRIPTION
  Parses the validity, which is made up of two fields, not_before and
  not_after time.  The time value is stored as an integer corresponding
  to the number of seconds from 01/06/1980 and the time will also be
  stored in the printable version found within the certificate.

  The validity information is stored in cert->not_before and
  cert->not_after.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  cert     - where the validity information will be stored

RETURN VALUE
  E_X509_SUCCESS - if the validity is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_validity
(
  secasn1_data_type *data_ptr,
  pbl_secx509_cert_info_type *cert
)
{
  secasn1_data_type seq;                          /* temporary seq holder  */
  secasn1_err_type err;                           /* ASN.1 error type      */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
  /*-----------------------------------------------------------------------*/
  //SECBOOT_PRINT( "X509: Parsing Validity ...");

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (cert == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  do
  {
    SECX509_OPEN_SEQUENCE( data_ptr, &seq );

    /* Parse the not before time */
    err = pbl_secasn1_next_field( &seq, &cert->not_before.data,
                                  SECASN1_UTC_TYPE );
    if ( err == E_ASN1_SUCCESS )
    {
      /* UTC Time format */
      /* pbl_secasn1_decode_time checks data_bound */
      if ( pbl_secasn1_decode_time( &cert->not_before.data,
                                &cert->not_before.time, TRUE )
           != E_ASN1_SUCCESS )
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }
    }
    else if ( err == E_ASN1_INVALID_TAG )
    {
      /* General Time format */
      SECX509_NEXT_FIELD( &seq, &cert->not_before.data,
                          SECASN1_NO_TYPE_CHECK );

      if ( pbl_secasn1_decode_time( &cert->not_before.data,
                                &cert->not_before.time, FALSE )
           != E_ASN1_SUCCESS )
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }

    }
    else
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Parse the not after time */
    err = pbl_secasn1_next_field( &seq, &cert->not_after.data,
                                  SECASN1_UTC_TYPE );
    if ( err == E_ASN1_SUCCESS )
    {
      /* UTC Time format */
      if ( pbl_secasn1_decode_time( &cert->not_after.data,
                                &cert->not_after.time, TRUE ) !=
           E_ASN1_SUCCESS )
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }
    }
    else if ( err == E_ASN1_INVALID_TAG )
    {
      /* General Time format */
      SECX509_NEXT_FIELD( &seq, &cert->not_after.data, SECASN1_NO_TYPE_CHECK );

      if ( pbl_secasn1_decode_time( &cert->not_after.data,
                                &cert->not_after.time, FALSE )
           != E_ASN1_SUCCESS )
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }
    }
    else
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    SECX509_CLOSE_SEQUENCE( data_ptr, &seq );

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;
} /* pbl_secx509_parse_validity */

/*===========================================================================

FUNCTION SECX509_PARSE_DN

DESCRIPTION
    Parse a distinguished name(DN) field of an X509 certificate.  Since a
    DN name may have many different attributes this function will verify that
    the fields are valid and it will record the number of attributes.

DEPENDENCIES
  None

PARAMETERS
  data_ptr          - current data holder position
  dn                - where the parsed dn field will be stored
  ou_field_info_ptr - pointer to store OU Field values

RETURN VALUE
  E_X509_SUCCESS - if the version is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  Since this function has been verified the data in the DN, when functions
  pbl_secx509_get_name_certificate, pbl_secx509_get_concat_name and
  pbl_secx509_get_dn_object_string are called they will never be parsing errors
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_dn
(
  secasn1_data_type           *data_ptr,
  pbl_secx509_dn_type         *dn,
  secx509_ou_field_info_type  *ou_field_info_ptr
)
{
  secasn1_data_type seq;                          /* sequence holder       */
  secasn1_data_type set;                          /* set holder            */
  secasn1_data_type att;                          /* attributes holder     */
  secasn1_data_type dat;                          /* data holder           */
  uint32 num_attrib = 0;                          /* num of dn attributes  */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
  uint32 i;
  boolean found_ou_field = FALSE;
  uint64 ou_field_value = 0;
  /*-----------------------------------------------------------------------*/
  //SECBOOT_PRINT( "X509: Parsing Distinguished Name ...");

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (dn == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  /* set SECBOOT_DEBUG_DISABLE to default. It will be updated later     */
  /* So missing DEUBG OU field means the debug feature disabled.        */
  /* (This is not a check of missing ou field)                          */
  if (ou_field_info_ptr != NULL)
  {
    ou_field_info_ptr->debug_enable = SECBOOT_DEBUG_DISABLE;
  }

  do
  {
    SECX509_OPEN_SEQUENCE( data_ptr, &seq );

    /* Set the marker to the beginning of the attributes */
    dn->data.data = seq.data;
    dn->data.len = seq.len;

    /* Parse all the attributes */
    while ( seq.len > 0 )
    {

      SECX509_NEXT_FIELD( &seq, &set, SECASN1_SET_TYPE );
      SECX509_OPEN_SEQUENCE( &set, &att );
      num_attrib++;

      /* We check to see if an object Identifier and value exists */
      /* This allows us to be sure that when items are attempted  */
      /* to be pulled from the DN (distinguished name) field, the */
      /* structure will be able to be parsed with no errors.      */
      SECX509_NEXT_FIELD( &att, &dat, SECASN1_OID_TYPE );
  
      /* We check if this OID type is an OU field */
      found_ou_field = FALSE;
      ret_status = pbl_secx509_check_oid_type(&dat, 
                                              pbl_secx509_asn1_oid_obj_unit, 
                                              sizeof(pbl_secx509_asn1_oid_obj_unit),
                                              &found_ou_field);
      if (E_X509_SUCCESS != ret_status)
              {
              break;
            }

      SECX509_NEXT_FIELD( &att, &dat, SECASN1_NO_TYPE_CHECK );

      /* Handle only OU field query */
      if (found_ou_field && (NULL != ou_field_info_ptr))
          {
            /* ========================================================== */
        /* "01 xxxxxxxxxxxxxxxx SW_ID"                                */
        /* Search the ou field string in ascii and extract the value. */
            /* ========================================================== */
        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_SW_ID_VALUE_LEN,
                                                sw_id_string,
                                                OU_SW_ID_STRING_LEN);
  
        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->sw_id, 
                                      ou_field_value);
  
              /* ========================================================== */
        /* "02 xxxxxxxxxxxxxxxx HW_ID"                                */
        /* Search the ou field string in ascii and extract the value. */
              /* ========================================================== */
        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_HW_ID_VALUE_LEN,
                                                hw_id_string,
                                                OU_HW_ID_STRING_LEN);
        
        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->hw_id, 
                                      ou_field_value);

            /* ========================================================== */
        /* "03 xxxxxxxxxxxxxxxx DEBUG"                                */
        /* Search the ou field string in ascii and extract the value. */
            /* ========================================================== */
        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_DEBUG_VALUE_LEN,
                                                debug_string,
                                                OU_DEBUG_STRING_LEN);
  
        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->debug_enable, 
                                      ou_field_value);
  
              /* ========================================================== */
        /* "07 xxxx SHA1/SHA256"                                      */
        /* Search the ou field string in ascii and extract the value. */
              /* ========================================================== */
        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_SHA1_VALUE_LEN,
                                                sha1_codehash_str,
                                                OU_SHA1_STRING_LEN);
        
        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->code_hash_algo, 
                                      E_X509_CODE_HASH_SHA1);

        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_SHA256_VALUE_LEN,
                                                sha256_codehash_str,
                                                OU_SHA256_STRING_LEN);
        
        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->code_hash_algo, 
                                      E_X509_CODE_HASH_SHA256);
  
              /* ========================================================== */
        /* "09 xxxxxxxxxxxxxxxx CRASH_DUMP"                           */
        /* Search the ou field string in ascii and extract the value. */
              /* ========================================================== */
        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_CRASH_DUMP_VALUE_LEN,
                                                crash_dump_string,
                                                OU_CRASH_DUMP_STRING_LEN);
  
        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->crash_dump_enable, 
                                      ou_field_value);
  
          /* ========================================================== */
        /* "10 xxxxxxxxxxxxxxxx ROT_EN"                               */
        /* Search the ou field string in ascii and extract the value. */
          /* ========================================================== */
        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_ROT_VALUE_LEN,
                                                rot_string,
                                                OU_ROT_STRING_LEN);

        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->rot_ou_field, 
                                      ou_field_value);

        /* ========================================================== */
        /* "13 xxxx IN_USE_SOC_HW_VERSION"                            */
        /* Search the ou field string in ascii and extract the value. */
        /* ========================================================== */
        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_IN_USE_SOC_HW_VER_VALUE_LEN,
                                                in_use_soc_hw_version_string,
                                                OU_IN_USE_SOC_HW_VER_STRING_LEN);

        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->in_use_soc_hw_version, 
                                      (uint16)ou_field_value);

        /* ========================================================== */
        /* "14 xxxx USE_SERIAL_NUMBER_IN_SIGNING"                     */
        /* Search the ou field string in ascii and extract the value. */
        /* ========================================================== */
        ou_field_value = 0;
        ret_status = pbl_secx509_parse_ou_field(&dat, 
                                                &ou_field_value,
                                                OU_USE_SERIAL_NUM_VALUE_LEN,
                                                use_serial_num_string,
                                                OU_USE_SERIAL_NUM_STRING_LEN);
        
        SECX509_UPDATE_OU_FIELD_VALUE(ret_status,
                                      ou_field_info_ptr->ou_use_serial_num, 
                                      (uint16)ou_field_value);

      }
      
      SECX509_CLOSE_SEQUENCE( &set, &att );
    }

    if ( ret_status != E_X509_SUCCESS )
    {
      break;
    }

    SECX509_CLOSE_SEQUENCE( data_ptr, &seq );

    dn->num_attrib = num_attrib;

  }/*lint -e(717) */ while ( FALSE );

  if ( ret_status != E_X509_SUCCESS )
  {
    /* Clean up the dn structure being returned */
    dn->data.data = NULL;
    dn->data.len = 0;
    dn->num_attrib = 0;
  }

  return ret_status;
} /* pbl_secx509_parse_dn */

/*===========================================================================

FUNCTION SECX509_PARSE_PUB_KEY

DESCRIPTION
  Parse the Public Key.  Currently we only support RSA Public Keys.  If the
  public key cannot be determined the returned public key type found in
  ptr->algo will be set to SECX509_PUBKEY_MAX

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  ptr  - where the returned public key information will be held

RETURN VALUE
  E_X509_SUCCESS - if the version is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_pub_key
(
  secasn1_data_type*    data_ptr,
  pbl_secx509_pubkey_type*  ptr
)
{
  secasn1_data_type       seq1;                  /* outer sequence holder  */
  secasn1_data_type       seq2;                  /* inner sequence holder  */
  secasn1_data_type       data;                  /* data holder            */
  secasn1_data_type       dmy;                   /* dummy value holder       */
  secasn1_bit_string_type bit_str_hldr;          /* bit string holder      */
  secasn1_data_type       bit_str;               /* decoded bit string     */
  secx509_errno_enum_type      ret_status = E_X509_SUCCESS;/* Return Status     */
  /*-----------------------------------------------------------------------*/
  //SECBOOT_PRINT( "X509: Parsing Public Key ...");

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (ptr == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  do
  {
    SECX509_OPEN_SEQUENCE( data_ptr, &seq1 );
    SECX509_OPEN_SEQUENCE( &seq1, &seq2 );
    SECX509_NEXT_FIELD( &seq2, &data, SECASN1_OID_TYPE );
    SECX509_NEXT_FIELD( &seq2, &dmy, SECASN1_NO_TYPE_CHECK );

    SECX509_CLOSE_SEQUENCE( &seq1, &seq2 );

    /* SET THIS TO NOT SUPPORTED PUBLIC KEY ALGORITHM in the beginning */
    /* to be safe if anything fails in this function */
    ptr->algo = SECX509_PUBKEY_MAX;

    if ( (sizeof(pbl_secx509_asn1_rsa) == data.len ) &&
	        ( !MEMCMP( pbl_secx509_asn1_rsa,
                        data.data,
                        data.len ) ) )
    {


      /* Parse the public key */
      SECX509_NEXT_FIELD( &seq1, &data, SECASN1_BIT_STRING_TYPE );

      if ( pbl_secasn1_decode_bit_string( &data, &bit_str_hldr ) !=
           E_ASN1_SUCCESS )
      {
        return E_X509_DATA_INVALID;
      }

      bit_str.data = bit_str_hldr.data;
      bit_str.len = bit_str_hldr.len;

      /* Ensure there can't be an integer overflow and that the bit string does not */
      /* overflow the containing structure */
      if (!CHECK_DATA_BOUND(bit_str_hldr.data, bit_str_hldr.len, data.data_bound)) 
      {
        return E_X509_DATA_INVALID;        
      }

      // Add is safe by the post condition of the above test
      bit_str.data_bound = bit_str_hldr.data + bit_str_hldr.len;

      SECX509_OPEN_SEQUENCE( &bit_str, &seq2 );
      SECX509_NEXT_FIELD( &seq2, &data, SECASN1_INTEGER_TYPE );

      if (data.len == 0)
      {
        return E_X509_DATA_INVALID;
      }

      /* Remove leading zeros from modulus. Per ASN.1 a leading zero byte */
      /* will be present if the actual value's first byte's MSB is 1.     */
      /* It's safe to remove all leading zero's as they are not applicable */
      while ( ( data.len > 0 ) && ( BLOAD8(data.data) == 0 ) )
      {
        data.data++;
        data.len--;
      }

      /* Remember the location of Public Key Modulus in the certificate memory */
      if ( data.len > (SECBOOT_MAX_KEY_SIZE_IN_BITS/8) )
      {
        return E_X509_DATA_INVALID;
      }

      ptr->key.rsa.mod_data =  data.data;
      ptr->key.rsa.mod_len = data.len;

      /* Parse the exponent */
      SECX509_NEXT_FIELD( &seq2, &data, SECASN1_INTEGER_TYPE );

      if (data.len == 0)
      {
        return E_X509_DATA_INVALID;
      }

      /* Remove leading zero's */
      while ( ( data.len > 0 ) && ( BLOAD8(data.data) == 0 ) )
      {
        data.data++;
        data.len--;
      }

      /* Remember the location of RSA Public Exponent in the certificate memory */
      if ( data.len > (SECBOOT_MAX_PUB_EXP_KEY_SIZE_IN_BITS/8) )
      {
        return E_X509_DATA_INVALID;
      }

      ptr->key.rsa.exp_e_data = data.data;
      ptr->key.rsa.exp_e_len = data.len;

      SECX509_CLOSE_SEQUENCE( &bit_str, &seq2 );

      ptr->algo = SECX509_PUBKEY_RSA;
    }

    SECX509_CLOSE_SEQUENCE( data_ptr, &seq1 );

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;
} /* pbl_secx509_parse_pub_key */

/*===========================================================================

FUNCTION SECX509_PARSE_UNIQUE_ID

DESCRIPTION
  Parses a unique ID.  Since both places a unique ID is found in the
  X509 structure, this function supplies an optional tag id to test, but
  if the optional tag id is not found, we will assume the unique ID is
  not present and still return a success.

DEPENDENCIES
  None

PARAMETERS
  data_ptr  - current data holder position
  bt        - bit string to store the unique Id
  op_tag_id - optional tag id that must be next in the data holder
              for the field to exists

RETURN VALUE
  E_X509_SUCCESS - if the data was parsed properly even if an ID is not found
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_unique_id
(
  secasn1_data_type *data_ptr,
  secasn1_bit_string_type *bt,
  uint8 op_tag_id
)
{
  secasn1_data_type data;                         /* data holder           */
  secasn1_err_type err;                           /* ASN.1 error code      */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
/*-------------------------------------------------------------------------*/
  //SECBOOT_PRINT( "X509: Parsing Unique Id ...");

  do
  {

    err = pbl_secasn1_next_field( data_ptr, &data, op_tag_id );
    //We allow E_ASN1_INVALID_TAG or E_ASN1_NO_DATA as this is an optional field
    if ( err >= E_ASN1_INVALID_DATA )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }
    else if ( err == E_ASN1_SUCCESS )
    {
      /* Since this is an IMPLICIT field only need to */
      /* look for the optional tag                    */
      if ( pbl_secasn1_decode_bit_string( &data, bt ) != E_ASN1_SUCCESS )
      {
        ret_status = E_X509_DATA_INVALID;
        break;
      }
    }

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;
} /* pbl_secx509_parse_unique_id */

/*===========================================================================

FUNCTION SECX509_PARSE_EXTENSIONS

DESCRIPTION
  Parses the extensions of the X509 certificate.  According to RFC 2459, it
  highly recommends that the following extensions be understood:

    key usage, certificate policies, subject alternative name, basic
    constraint, name constraints, policy constraints, extended key
    identifier, authority key identifier and subject key identifier.

  Currently all these extensions are parsed and checked for the critical flag,
  but beyond that the following extensions are just ignored:

    certificate policies, name constraints, policy constraints

  The rest of the extensions are fully parsed.  Also if an unknown extension
  is found that is marked critical, the certificate must be found invalid
  according to RFC 2459.

  Since the extensions are an optional field, this function accepts the
  optional tag id which it checks against the next tag.

DEPENDENCIES
  This function should only be called for version 3 certificates

PARAMETERS
  data_ptr  - current data holder position
  cert      - certificate to store the extensions
  op_tag_id - optional tag id that must be next in the data holder
              for the field to exists

RETURN VALUE
  E_X509_SUCCESS - if the extensions are parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_extensions
(
  secasn1_data_type *data_ptr,
  pbl_secx509_cert_info_type *cert,
  uint8 op_tag_id
)
{
  secasn1_data_type opt;                          /* optional field holder */
  secasn1_data_type seq1;                         /* outer sequence holder */
  secasn1_data_type seq2;                         /* inner sequence holder */
  secasn1_data_type oid;                          /* object id holder      */
  secasn1_data_type val;                          /* oid value holder      */
  secasn1_err_type err;                           /* ASN.1 error code      */
  secasn1_data_type dat;                          /* temp data holder      */
  boolean critical;                               /* Whether the current   */
                                                  /* extension is critical */
  pbl_secx509_cert_info_type *cert_ptr;           /* certificate pointer   */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS; /* Return Status         */
  /*-----------------------------------------------------------------------*/

  //SECBOOT_PRINT( "X509: Parsing Extensions ...");

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (cert == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  cert_ptr = cert;

  /* Mark all the extension to not be set */
  cert_ptr->extension.auth_key_id.set = FALSE;
  cert_ptr->extension.subject_key_id.set = FALSE;
  cert_ptr->extension.key_usage.set = FALSE;
  cert_ptr->extension.ex_key_usage.set = FALSE;
  cert_ptr->extension.ca.set = FALSE;

  do
  {

    err = pbl_secasn1_next_field( data_ptr, &opt, op_tag_id );
 
    //We allow E_ASN1_INVALID_TAG or E_ASN1_NO_DATA as this is an optional field
    if ( err >= E_ASN1_INVALID_DATA )
    {
      /* ERROR has occurred */
      ret_status = E_X509_DATA_INVALID;
    }
    else if ( err == E_ASN1_SUCCESS )
    {
      //SECBOOT_PRINT( "X509: Found Extensions ...");

      cert->extension.set = TRUE;

      /* Extensions are present */
      SECX509_OPEN_SEQUENCE( &opt, &seq1 );

      while ( seq1.len > 0 )
      {
        SECX509_OPEN_SEQUENCE( &seq1, &seq2 );

        /* Parse the object identifier */
        SECX509_NEXT_FIELD( &seq2, &oid, SECASN1_OID_TYPE );

        /* Parse the whether the value is critical */
        err = pbl_secasn1_next_field( &seq2, &dat, SECASN1_BOOLEAN_TYPE );
        if ( err == E_ASN1_SUCCESS )
        {
          if ( pbl_secasn1_decode_boolean( &dat, &critical ) !=
               E_ASN1_SUCCESS )
          {
            ret_status = E_X509_DATA_INVALID;
            break;
          }
        }
        else 
        {
          /* If a boolean tag is not present it is false */
          critical = FALSE;
        }

        /* Parse the value */
        SECX509_NEXT_FIELD( &seq2, &val, SECASN1_NO_TYPE_CHECK );

        /* We have the extension object identifier */
        if ( (sizeof(pbl_secx509_ext_auth_key_id) == oid.len) &&
             ( !MEMCMP( pbl_secx509_ext_auth_key_id,
                            oid.data,
                            oid.len ) ) )
        {
          /* Authority Key Identifier */
          //SECBOOT_PRINT( "X509: Authority Key Identifier Extension");

          if ( pbl_secx509_parse_auth_key_id( &val,
                                          &cert_ptr->extension.auth_key_id,
                                          critical ) != E_X509_SUCCESS )
          {
            ret_status = E_X509_DATA_INVALID;
          }
        }
        else if ( (sizeof(pbl_secx509_ext_sub_key_id) == oid.len) &&
                  (!MEMCMP( pbl_secx509_ext_sub_key_id,
                                 oid.data,
                                 oid.len ) ) )
        {
          /* Subject Key Identifier */
          //SECBOOT_PRINT("X509: Subject Key Identifier Extension");

          if ( critical )
          {
            ret_status = E_X509_DATA_INVALID;
          }

          /* Try to parse the key identifier */
          err = pbl_secasn1_next_field( &val,
                              &cert_ptr->extension.subject_key_id.key_id,
                              SECASN1_OCTET_STRING_TYPE );
          //We allow E_ASN1_INVALID_TAG or E_ASN1_NO_DATA as this is an optional field
          if ( err >= E_ASN1_INVALID_DATA )
          {
            ret_status = E_X509_DATA_INVALID;
            break;
          }

          cert_ptr->extension.subject_key_id.set = TRUE;
        }
        else if ( (sizeof(pbl_secx509_ext_key_usage) == oid.len) &&
                  (!MEMCMP( pbl_secx509_ext_key_usage,
                                 oid.data,
                                 oid.len ) ) )
        {
          /* Key Usage Extension (Does not matter if it is critical) */
          //SECBOOT_PRINT( "X509: Key Usage Extension");

          cert->extension.key_usage.set = TRUE;

          if ( pbl_secx509_parse_key_usage( &val,
                                            &cert->extension.key_usage.val )
               != E_X509_SUCCESS )
          {
            ret_status = E_X509_DATA_INVALID;
          }

        }
        else if ( (sizeof(pbl_secx509_ext_ex_key_usage) == oid.len) &&
                  (!MEMCMP( pbl_secx509_ext_ex_key_usage,
                                 oid.data,
                                 oid.len ) ) )
        {
          /* Key Usage Extension (Does not matter if it is critical) */
          //SECBOOT_PRINT( "X509: Extended Key Usage Extension");

          cert_ptr->extension.ex_key_usage.set = TRUE;

          if ( pbl_secx509_parse_ex_key_usage( &val,
                                           &cert->extension.ex_key_usage.val )
               != E_X509_SUCCESS )
          {
            ret_status = E_X509_DATA_INVALID;
          }
        }
        else if ( (sizeof(pbl_secx509_ext_sub_alt_name) == oid.len) &&
                  (!MEMCMP( pbl_secx509_ext_sub_alt_name,
                                 oid.data,
                                 oid.len ) ) )
        {
          /* Subject Alternative Name */
          //SECBOOT_PRINT( "X509: Subject Alternate Name Extension");

          /* If no subject Distinguished name had no attributes  */
          /* this field better be marked critical, this is the   */
          /* only time we will try to parse the object           */
          if ( cert->subject.num_attrib == 0 )
          {
            if ( critical )
            {
              if ( pbl_secx509_parse_gen_name_for_dir( &val,
                                                   &cert->subject )
                   != E_X509_SUCCESS )
              {
                ret_status = E_X509_DATA_INVALID;
              }
            }
            else
            {
              ret_status = E_X509_DATA_INVALID;
            }

          }
        }
        else if ( (sizeof(pbl_secx509_ext_basic_constraint) == oid.len) &&
                  (!MEMCMP( pbl_secx509_ext_basic_constraint,
                                 oid.data,
                                 oid.len ) ) )
        {
          /* Basic Constraints */
          if ( pbl_secx509_parse_basic_constraint( &val,
                                               &cert->extension )
               != E_X509_SUCCESS )
          {
            ret_status = E_X509_DATA_INVALID;
          }

        }
        else if ( (sizeof(pbl_secx509_ext_name_constraint) == oid.len) &&
                  (!MEMCMP( pbl_secx509_ext_name_constraint,
                                 oid.data,
                                 oid.len ) ) )
        {

          /* Name Constraints */
          //SECBOOT_PRINT( "X509: Name Constraint Extension");
        }
        else if ( (sizeof(pbl_secx509_ext_policy_constraint) == oid.len) &&
                  (!MEMCMP( pbl_secx509_ext_policy_constraint,
                                 oid.data,
                                 oid.len ) ) )
        {

          /* Policy Constraints */
          //SECBOOT_PRINT( "X509: Policy Constraint Extension");
        }
        else if ( (sizeof(pbl_secx509_ext_cert_policies) == oid.len) &&
                  (!MEMCMP( pbl_secx509_ext_cert_policies,
                                 oid.data,
                                 oid.len ) ) )
        {
          /* Certificate Policies */
          //SECBOOT_PRINT( "X509: Certificate Policies Extension");
        }
        else
        {
          /* Unknown Extension */
          if ( critical )
          {
            /* As stated in RFC 2459 if an extension is marked */
            /* critical and cannot be parsed then the cert     */
            /* must be rejected.                               */
            ret_status = E_X509_DATA_INVALID;
          }
          else
          {
            //SECBOOT_PRINT( "X509: Unrecoginized Non-Critical Extension", 0, 0, 0 );
          }
        }

        if (ret_status != E_X509_SUCCESS)
        {
          /* An error has occurred in the processing, leave the */
          /* loop that parses the extensions */
          break;
        }

        SECX509_CLOSE_SEQUENCE( &seq1, &seq2 );
      }

      if (ret_status != E_X509_SUCCESS)
      {
        break;
      }

      SECX509_CLOSE_SEQUENCE( &opt, &seq1 );
    }

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;

} /* pbl_secx509_parse_extensions */

/*===========================================================================

FUNCTION SECX509_PARSE_SIGNATURE

DESCRIPTION
  Parses the signature and creates pointers to the data in
  cert->cert->algorithm and does an actually copy to the locations
  cert->sig.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  cert     - certificate to store the signature

RETURN VALUE
  E_X509_SUCCESS - if the signature is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_signature
(
  secasn1_data_type*  data_ptr,
  pbl_secx509_cert_type*  cert
)
{
  secasn1_data_type        data;                   /* data holder          */
  secasn1_bit_string_type  bit_string;             /* bit string holder    */
  uint32                   sig_len;                /* signature length     */
  const uint8*             sig_ptr;                /* signature pointer    */
  secx509_errno_enum_type       ret_status = E_X509_SUCCESS; /* Return Status   */
  /*-----------------------------------------------------------------------*/
  //SECBOOT_PRINT( "X509: Parsing Signature ...", 0, 0, 0 );

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (cert == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  do
  {
    if ( pbl_secx509_parse_sig_algorithm( data_ptr, &cert->sig_algo ) !=
         E_X509_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    if ( cert->sig_algo >= SECX509_SIG_ALGO_MAX )
    {
      ret_status = E_X509_NOT_SUPPORTED;
      break;
    }

    /* Checks to make sure that the sig algorithm within the            */
    /* certificate is consistent with the algorithm identifier          */
    /* within the signed certificate, as RFC2459, 4.1.1.2  says         */
    /* This field MUST contain the same algorithm identifier as the     */
    /* signature field in the sequence tbsCertificate (see sec. 4.1.2.3)*/
    if ( cert->sig_algo != cert->cert_info.algorithm.algo_id )
    {
      //SECBOOT_PRINT( "X509: Outer sig algorithm does not match inner sig algorithm",
      //          0, 0, 0 );
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Parse the signature data */
    SECX509_NEXT_FIELD( data_ptr, &data, SECASN1_BIT_STRING_TYPE );

    if ( pbl_secasn1_decode_bit_string( &data, &bit_string ) !=
         E_ASN1_SUCCESS )
    {
      ret_status = E_X509_DATA_INVALID;
      break;
    }

    /* Remove any leading 0's on the signature */
    sig_ptr = bit_string.data;
    sig_len = bit_string.len;

    while ( ( sig_len > 0 ) && ( BLOAD8(sig_ptr) == 0 ) )
    {
      sig_len--;
      sig_ptr++;
    }

    cert->cert_info.algorithm.val.data = sig_ptr;
    cert->cert_info.algorithm.val.len = sig_len;
    /* Signature size same as key length */
    if ( sig_len > (SECBOOT_MAX_KEY_SIZE_IN_BITS/8) )
    {
      return E_X509_DATA_INVALID;
    }
    cert->sig = sig_ptr;
    cert->sig_len = sig_len;

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;

} /* pbl_secx509_parse_signature */

/*===========================================================================

FUNCTION SECX509_PARSE_SIG_ALGORITHM

DESCRIPTION
  Parses the signature algorithm.  If the signature algorithm is not
  recognized algo is set to SECX509_SIG_ALGO_MAX

DEPENDENCIES
  None

PARAMETERS
  data_ptr - current data holder position
  algo     - where the returned signature algo will be held

RETURN VALUE
  E_X509_SUCCESS - if the signature algorithm is parsed correctly
  E_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_sig_algorithm
(
  secasn1_data_type *data_ptr,
  pbl_secx509_sig_algo_type *algo
)
{
  secasn1_data_type data;                         /* data holder           */
  secasn1_data_type oid;                          /* object id holder      */
  secasn1_data_type dmy;                          /* dummy value holder    */
  secx509_errno_enum_type ret_status = E_X509_SUCCESS;      /* Return Status    */
  /*-----------------------------------------------------------------------*/
  //SECBOOT_PRINT( "X509: Parsing Signature Algorithm ...");

  /* Sanity Check on pointer arguments */
  if ( (data_ptr == NULL) || (algo == NULL) )
  {
    return E_X509_INVALID_ARG;
  }

  do
  {

    SECX509_OPEN_SEQUENCE( data_ptr, &data );
    SECX509_NEXT_FIELD( &data, &oid, SECASN1_OID_TYPE );
    SECX509_NEXT_FIELD( &data, &dmy, SECASN1_NO_TYPE_CHECK );

    /* Determine the signature algorithm used */
    if ( (sizeof(pbl_secx509_sha1WithRSAEncryption) == oid.len) &&
              (!MEMCMP( pbl_secx509_sha1WithRSAEncryption,
                             oid.data,
                             oid.len ) ) )
    {
      *algo = SECX509_sha1WithRSAEncryption;
    }
    else if ( (sizeof(pbl_secx509_sha256WithRSAEncryption) == oid.len) &&
              (!MEMCMP( pbl_secx509_sha256WithRSAEncryption,
                             oid.data,
                             oid.len ) ) )
    {
      *algo = SECX509_sha256WithRSAEncryption;
    }
    else
    {
      *algo = SECX509_SIG_ALGO_MAX;
    }

    SECX509_CLOSE_SEQUENCE( data_ptr, &data );

  }/*lint -e(717) */ while ( FALSE );

  return ret_status;
} /* pbl_secx509_parse_sig_algorithm */

/*===========================================================================

FUNCTION SECX509_NAME_CMP

DESCRIPTION
  Compares two distinguished names by doing a memory comparision

DEPENDENCIES
  None

PARAMETERS
  subject - a DN name to be compared
  issuer  - a DN name to be compared

RETURN VALUE
  0     - if DN names are identical
  non 0 - otherwise

SIDE EFFECTS
  None
===========================================================================*/
int pbl_secx509_name_cmp
(
  const pbl_secx509_dn_type *subject,
  const pbl_secx509_dn_type *issuer
)
{
  /*-----------------------------------------------------------------------*/

  /* Sanity Check on pointer arguments */
  if ( (subject == NULL) || (issuer == NULL) )
  {
    return 1;
  }

  if ( issuer->data.len != subject->data.len )
  {
    return issuer->data.len - subject->data.len;
  }

  return MEMCMP( issuer->data.data, subject->data.data, issuer->data.len );
} /* pbl_secx509_name_cmp */

/*===========================================================================

FUNCTION pbl_secx509_check_issued

DESCRIPTION
  Compares a certificate with an issuer certificate to determine if the
  issuer certificate issued the certificate

DEPENDENCIES
  None

PARAMETERS
  issuer  - certificate that will be check as the issuer
  subject - certificate that will check to see if the issuer issued it

RETURN VALUE
  0     - if the issuer issued the certificate
  non 0 - otherwise

SIDE EFFECTS
  None
===========================================================================*/
int pbl_secx509_check_issued
(
  const pbl_secx509_cert_info_type* issuer,
  const pbl_secx509_cert_info_type* subject
)
{
  int ret_val;
  /*-----------------------------------------------------------------------*/
  /* Sanity Check on pointer arguments */
  if ( (issuer == NULL) || (subject == NULL) )
  {
    return 1;
  }

  ret_val = pbl_secx509_name_cmp( &issuer->subject, &subject->issuer );

  if ( ret_val != 0 ) return ret_val;

  /* Check the authority key identifier */
  if ( subject->extension.auth_key_id.set )
  {

    /* Check if the key ids are present */
    if ( subject->extension.auth_key_id.key_id.len &&
         issuer->extension.subject_key_id.set )
    {

      /* Compare the key ids */
      if ( subject->extension.auth_key_id.key_id.len !=
           issuer->extension.subject_key_id.key_id.len )
      {

        return 1;
      }

      ret_val = MEMCMP( subject->extension.auth_key_id.key_id.data,
                        issuer->extension.subject_key_id.key_id.data,
                        issuer->extension.subject_key_id.key_id.len );

      if ( ret_val != 0 )
      {
        return ret_val;
      }
    }

    /* Check the Serial number */
    if ( subject->extension.auth_key_id.serial_number.len != 0 )
    {
      if ( subject->extension.auth_key_id.serial_number.len !=
           issuer->serial_number.len )
      {

        return 1;
      }

      ret_val = MEMCMP( subject->extension.auth_key_id.serial_number.data,
                        issuer->serial_number.data,
                        issuer->serial_number.len );

      if ( ret_val != 0 )
      {
        return ret_val;
      }
    }

    /* Compare the issuers name */
    if ( subject->extension.auth_key_id.name.num_attrib != 0 )
    {
      ret_val = pbl_secx509_name_cmp( &issuer->issuer,
                                  &subject->extension.auth_key_id.name );
       if (ret_val != 0) {
          return (ret_val);
       }
    }
  }

  /* Check key usage of the issuer key */
  if ( issuer->extension.key_usage.set )
  {
    if ( !( issuer->extension.key_usage.val & SECX509_KEY_USAGE_KEY_CRT ) )
    {
      return -1;
    }
  }

  return 0;

} /* pbl_secx509_check_issued */

/*===========================================================================

FUNCTION pbl_secx509_check_purpose

DESCRIPTION
  This function checks the purposes of the certificate.  Currently we
  only support an ssl server check.

DEPENDENCIES
  None

PARAMETERS
  cert - pointer to the certificate
  id   - what category the certificate is being checked against
  ca   - CA value

RETURN VALUE
  1 if the certificate is valid for the purpose
  0 otherwise

SIDE EFFECTS
  None
===========================================================================*/
int pbl_secx509_check_purpose
(
  const pbl_secx509_cert_info_type *cert,
  int id,
  int ca
)
{
  int ret_val=0;                                    /* return value        */
  /*-----------------------------------------------------------------------*/

  /* Sanity Check on pointer arguments */
  if ( cert == NULL )
  {
    return 0;
  }

  /* If we have an id of -1 do not check the purpose */
  if ( id == -1 || !cert->extension.set )
  {
    return 1;
  }

  if ((cert->extension.key_usage.val & id)!=0)
  {
      ret_val = 1;
  }
  else
  {
    //cert purpose does not match what is expected
    ret_val = 0;
  }

  return ret_val;

} /* pbl_secx509_check_purpose */


/*===========================================================================
FUNCTION pbl_secx509_parse_cert_buffer

DESCRIPTION
  Parses binary x509 certificates from memory into the buffer.
  parsing is done in order (attest cert first, CA cert next and
  root cert last).

DEPENDENCIES
  None

PARAMETERS
  cert_chain_ptr          - points to x509 certificate chain (in Flash)
                            CA cert and Attestation cert are here
  cert_chain_len          - upper bound on length of chain (parse fails
                            if chain appears to cross exceed this length,
                or if the space between the last cert and
                this length has any bytes that are not 0xFF).
  pbl_secx509_cert_list   - destination for all the certificates
                            after they have been parsed in order of chaining.
  ou_field_info_ptr       - pointer to store OU Field values                            

RETURN VALUE
  E_X509_SUCCESS on successful parsing of all certs
  E_X509_FAILURE on unsuccessful parsing of all certs
  E_X509_INVALID_ARG on invalid arguments

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_parse_cert_buffer
(
  const uint8*                 cert_chain_ptr,
  uint32                             cert_chain_len, /* including tail padding */
  pbl_secx509_cert_list_type*  pbl_secx509_cert_list,
  secx509_ou_field_info_type*  ou_field_info_ptr
)
{
 secx509_errno_enum_type return_value=E_X509_FAILURE;
  const uint8*       cert_chain_bound;
  uint32             cert_idx = 0;
  pbl_secx509_cert_type  *cert_ptr = 0;
  /*-----------------------------------------------------------------------*/
  /* Sanity Check on pointer arguments */
  if ( (cert_chain_ptr == NULL) || (pbl_secx509_cert_list == NULL)
        || (cert_chain_len == 0) || (ou_field_info_ptr == NULL))
  {
    return E_X509_INVALID_ARG;
  }

  /* Parse the certs one by one */
  do
  {
    /* check for buffer overflow */
    if (((uintptr_t)cert_chain_ptr + cert_chain_len) < (uintptr_t)cert_chain_ptr) 
    {
      return_value = E_X509_FAILURE;
      break;
    }

    /* Set the boundary. Parsing of all the certificates should be contained within */
    /* this boundary.  The above test ensures that this does not overflow. */
    cert_chain_bound = cert_chain_ptr + cert_chain_len;

    pbl_secx509_cert_list->size = 0;

    /* Parse the chained certificates, we can have minimum 2 certs in the                 */
    /* chain i.e attestation and root                                                     */
    /* Or the cert chain has a total of 18 certs of which a maximum of three will be used */
    /* i.e attesation, ca, root                                                           */
    for (cert_idx = 0; cert_idx < SECBOOT_MAX_NUM_CERTS; cert_idx++)
    {
      /* size is the counter, incrementing in every iteration */
      cert_ptr = &pbl_secx509_cert_list->cert[pbl_secx509_cert_list->size];

      /* Parse the certificate */
      return_value = pbl_secx509_parse_certificate( cert_chain_ptr,
                                                    cert_chain_bound,
                                                    cert_ptr,
                                                    ou_field_info_ptr);
      if (return_value!=E_X509_SUCCESS) break;
      
      /* Only the attestation cert which is the first cert in the chain */
      /* contains OU fields, so make sure we don't spend time during */
      /* parsing trying to find OU fields for the other certs */
      ou_field_info_ptr = NULL;

      /* Check if advancing the pointer will go off the end */
      if (!CHECK_DATA_BOUND(cert_chain_ptr, cert_ptr->asn1_size_in_bytes, cert_chain_bound))
      {
          return_value = E_X509_FAILURE;
          break;
      }

      /* advance pointer */
      cert_chain_ptr += cert_ptr->asn1_size_in_bytes;

      /* another certificate parsed and added ... */
      pbl_secx509_cert_list->size++;

      /* check if this is a root cert */
      if ( ( cert_ptr->cert_info.issuer.data.len ==
             cert_ptr->cert_info.subject.data.len ) && 
          ( MEMCMP( cert_ptr->cert_info.issuer.data.data,
                    cert_ptr->cert_info.subject.data.data,
                    cert_ptr->cert_info.subject.data.len ) == 0) )
      {
        // self-signed cert found (root), we're done
        // i.e if there are 2 certs in the chain instead of 3 we can stop here
          break;
      }
    }

    if (return_value != E_X509_SUCCESS) break;
    if (pbl_secx509_cert_list->size == 1)
    {
      // We don't allow just 1 cert (root cert).
      return_value = E_X509_FAILURE;
      break;
    }

    /* Check that any trailing space/end padding is all FF's */
    for (; cert_chain_ptr < cert_chain_bound; ++cert_chain_ptr) 
    {
      if (BLOAD8(cert_chain_ptr) != 0xFF) 
      {
        return_value = E_X509_FAILURE;
        /* don't bother to break out of this loop, since the normal 
         case goes clear to the end. */
      } 
    }
 
  }/*lint -e(717) */ while (FALSE);

  return return_value;
} /* pbl_secx509_parse_cert_buffer */

/*===========================================================================
FUNCTION pbl_secx509_check_cert_list

DESCRIPTION
  Simply checks if the certificate information, over the whole chain
  is valid and that the issuer and subject ID's are chained consecutively.
  Cryptographic signature verification down the chain is _NOT_ done here.

DEPENDENCIES
  None

PARAMETERS
  *cert_list_ptr  - Pointer to array of parsed certificates
  *check_ctx              - Sets of the parameters for the checking process

RETURN VALUE
  E_SUCCESS       - if no error
  E_DATA_INVALID  - Invalid Data
  E_NOT_SUPPORTED - Unknown Data

SIDE EFFECTS
  None
===========================================================================*/
secx509_errno_enum_type pbl_secx509_check_cert_list
(
  const pbl_secx509_cert_list_type *cert_list_ptr,
  const pbl_secx509_cert_ctx_type  *check_ctx
)
{
  uint32 curr_cert_num;
  secx509_errno_enum_type _errno = E_X509_SUCCESS;
  /*-----------------------------------------------------------------------*/

  /* Basic Sanity check */
  if ( (cert_list_ptr == NULL) || (check_ctx == NULL) ||
       (cert_list_ptr->size == 0) )
  {
    return E_X509_DATA_INVALID;
  }

  for ( curr_cert_num = 0; curr_cert_num < cert_list_ptr->size;
        curr_cert_num++ )
  {

    /* Check the certificate purpose */
    if (check_ctx->purpose > 0)
    {
      if (check_ctx->depth == curr_cert_num)
      {
        if ( !pbl_secx509_check_purpose( &(cert_list_ptr->
                                         cert[curr_cert_num].cert_info),
                                         check_ctx->purpose,
                                         curr_cert_num ) )
        {
          _errno = E_X509_DATA_INVALID;
          break;
        }
      }

      /* Check pathlen */
      if ( (curr_cert_num > 0) &&
           (cert_list_ptr->
            cert[curr_cert_num].cert_info.extension.path_len != -1) &&
            /* cast depends on current cert number not exceeding INT_MAX */
           (((int) curr_cert_num) > (cert_list_ptr->
            cert[curr_cert_num].cert_info.extension.path_len + 1)) )
      {
         _errno = E_X509_DATA_INVALID;
         break;
      }
    }

    /* All but last root cert ... */
    /* note, we don't process the root cert */
    /* This did read: if(curr_cert_num < cert_list_ptr->size - 1)...
       But ...->size is unsigned, and subtracting 1 could
       make a very large number.  So we rearrange the arithmetic. */
    if ( curr_cert_num + 1 < cert_list_ptr->size )
    {
      /* See if the issuer and subject ID's are correct and in order */
      if ( pbl_secx509_check_issued (&(cert_list_ptr->
                                     cert[curr_cert_num+1].cert_info),
                                     &(cert_list_ptr->
                                     cert[curr_cert_num].cert_info)) )
      {
        _errno = E_X509_DATA_INVALID;
        break;
      }
    }
  } /* end of 'for' */

  return _errno;
} /* pbl_secx509_check_cert_list */

/*===========================================================================
FUNCTION pbl_secx509_check_oid_type

DESCRIPTION
  query oid type with input arguments. It return TRUE in output argument, if
  the searched string data match the input query string.

DEPENDENCIES
  None

PARAMETERS
  *data_ptr           - Pointer to the searched data
  *oid_string         - Pointer to array of the query oid string
  oid_string_len      - Length of the query string
  *found_oid_type     - boolean to determine if the oid string is found

RETURN VALUE
  E_X509_SUCCESS      - if no error
  E_X509_INVALID_ARG  - input argument has NULL pointer

SIDE EFFECTS
  None
===========================================================================*/
static secx509_errno_enum_type pbl_secx509_check_oid_type
(
  secasn1_data_type           *data_ptr,
  const uint8                 *oid_type_ptr,
  uint32                      oid_type_len,
  boolean                     *found_oid_type
)
{
  if ((NULL == data_ptr) || (NULL == oid_type_ptr) || (NULL == found_oid_type))
  {
    return E_X509_INVALID_ARG;
  }

  *found_oid_type = FALSE;
  /* Found the queried oid type, if oid type string matches */
  if (MEMCMP(data_ptr->data, oid_type_ptr, oid_type_len) == 0)
  {
    *found_oid_type = TRUE;
  }

  return E_X509_SUCCESS;
}

/*===========================================================================
FUNCTION pbl_secx509_parse_ou_field

DESCRIPTION
  Parse OU field data and return the value as integer for the ou field.

DEPENDENCIES
  None

PARAMETERS
  *data_ptr           - Pointer to the searched data
  *ou_field_value     - Pointer to the returned ou field value
  ou_field_value_len  - Length of the ou field value (in bytes)
  *ou_field_string    - Pointer to the queried ou field string
  ou_field_string_len - Length of the queried ou field string (in bytes)

RETURN VALUE
  E_SUCCESS         - if no error
  x509 error number   - x509 error secx509_errno_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static secx509_errno_enum_type pbl_secx509_parse_ou_field
(
  secasn1_data_type           *data_ptr,
  uint64                      *ou_field_value,
  uint32                      ou_field_value_len,
  const uint8                 *ou_field_string,
  uint32                      ou_field_string_len
)
{
  uint64 ou_value = 0;
  uint8  mask_byte[SECX509_OU_FIELD_VALUE_MAX_BYTES];
  uint8  *mask_ptr = mask_byte;
  uint32 lshift = 0;
  uint32 j;
  const uint8  *ou_value_data_ptr;
  const uint8  *ou_string_data_ptr;

  if ((NULL == data_ptr) || (NULL == ou_field_value) || (NULL == ou_field_string))
  {
    return E_X509_INVALID_ARG;
  }

  if (ou_field_value_len > SECX509_OU_FIELD_VALUE_MAX_BYTES)
  {
    return E_X509_DATA_TOO_LARGE;
  }

  /* ==========================================================  */
  /* OU field format: <index> <OU FIELD VAlUE> <OU FIELD STRING> */
  /* <index> has alway 2 chars followed by space.                */
  /* <OU FIELD VAlUE> has as many as 16 chars followed by space. */
  /* <OU FIELD STRING> has char num that varies on definitions.  */
  /* ==========================================================  */

  /* the len of searched data should match the input argument. If not, the ou field */
  /* is not what is being asked for in the query.                                   */                                              
  if ((0 == ou_field_value_len) || 
      (data_ptr->len != SECX509_OU_FIELD_INDEX_LEN + SECX509_OU_FIELD_SPACE_LEN + ou_field_value_len
                         + SECX509_OU_FIELD_SPACE_LEN + ou_field_string_len))
  {
    return E_X509_OU_FIELD_NOT_FOUND;
  }

  if (CHECK_DATA_BOUND(data_ptr->data, (SECX509_OU_FIELD_INDEX_LEN + SECX509_OU_FIELD_SPACE_LEN),
                       data_ptr->data_bound) == FALSE)
  {
    return E_X509_BAD_DATA;
  }
  ou_value_data_ptr = data_ptr->data + SECX509_OU_FIELD_INDEX_LEN + SECX509_OU_FIELD_SPACE_LEN;

  if (CHECK_DATA_BOUND(ou_value_data_ptr, (ou_field_value_len + SECX509_OU_FIELD_SPACE_LEN),
                       data_ptr->data_bound) == FALSE)
  {
    return E_X509_BAD_DATA;
  }
  ou_string_data_ptr = ou_value_data_ptr + ou_field_value_len + SECX509_OU_FIELD_SPACE_LEN;

  if (CHECK_DATA_BOUND(ou_string_data_ptr, ou_field_string_len, data_ptr->data_bound) == FALSE)
  {
    return E_X509_BAD_DATA;
  }

  if (MEMCMP(ou_string_data_ptr, ou_field_string, ou_field_string_len) == 0)
  {
    /* calcaulate bit number to shift for converting char in cert to int. */
    /* (ou_field_value_len - 1) for skipping the first char byte.         */
    lshift = (ou_field_value_len - 1) * SECX509_OU_SHIFT_BIT_NUM_FOR_BYTE_IN_CERT;
    
    for (j = 0; j < ou_field_value_len; j ++)
    {
      /* COPY the value string to local variable. load/store data with byte addressing,  */ 
      /* which could be platform dependent. So keep BSTORE8/BLOAD8, which is defined by  */
      /* image as per platform required. (equal to mask_ptr[j] = ou_value_data_ptr[j])   */
      BSTOR8 (mask_ptr + j , BLOAD8(ou_value_data_ptr + j));
      /* convert the ascii of 0-9 and A-F to binary */              
      convert_to_binary(mask_ptr+j);
  
      /* This shifts each digit in. Each digit represents 4 bits */
      ou_value += ((uint64)BLOAD8(mask_ptr+j)) << lshift;
      /* lshift is the multiple of SECX509_OU_SHIFT_BIT_NUM_FOR_BYTE_IN_CERT. It will be */
      /* 0 in the last iteration, where the below minus oper is not needed to avoid      */
      /* integer underflow. */
      if (lshift != 0)
      {
        lshift -= SECX509_OU_SHIFT_BIT_NUM_FOR_BYTE_IN_CERT;
      }
    }
    
    *ou_field_value = ou_value;

    return E_X509_SUCCESS; 
  }

  return E_X509_OU_FIELD_NOT_FOUND;
}
