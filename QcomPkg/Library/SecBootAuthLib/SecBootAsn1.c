/*==========================================================================

                A S W   S E C U R I T Y   S E R V I C E S

         S S L   A S N . 1   I N T E R F A C E   M O D U L E

GENERAL DESCRIPTION
  This software module contains the source code for the asn.1 DER parsing
  functions

  Here a format of a simple ASN.1 document

  SEQUENCE {
    name          OCTECT STRING
    version       [0] INTEGER DEFAULT (0)
    valid         BOOLEAN
    storage       Storage
  }

  Storeage ::== SEQUENCE {
    time          UTC TIME
    data          BIT STRING
  }

EXTERNALIZED FUNCTIONS

  pbl_secasn1_start()                Starts an ASN.1 document for parsing

  pbl_secasn1_end()                  Closes an ASN.1 document

  pbl_secasn1_close_sequence()       Closes a sequence in an ASN.1 document

  pbl_secasn1_next_field()           Parses the next field in an ASN.1 document

  pbl_secasn1_decode_bit_string()    Decodes a bit string from a generic
                                     ASN.1 field

  pbl_secasn1_decode_boolean()       Decodes a boolean value from a generic
                                     ASN.1 field

  pbl_secasn1_decode_time()          Decode a time value from a generic
                                     ASN.1 field
 
SECURITY CONSIDERATIONS 
 
These functions carefully check for numeric overflow and buffer overflow. 
The variable data_bound is one past the last valid location in the 
associated context.  No store should done at or past data_bound.  The 
data pointer may reach data_bound, but should not pass it. 
 
The macro CHECK_DATA_BOUND provides a helpful and consistent check.
 
Some functions ( pbl_secasn1_decode_bit_string, 
pbl_secasn1_decode_boolean, pbl_secasn1_decode_time ) are checking 
within a buffer that has a known position and length (and the length 
has been validated as fitting withing the enclosing structure).  In 
this case, we know the pointers are valid, and no extra bounds 
checking is needed. 
 
INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c)2008-2014 Qualcomm Technologies Inc, All Rights Reserved.
==========================================================================*/

/*==========================================================================
          EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/25/13   mm      Remove UNDEFINED Length support
03/18/13   DJ,hw   Fix the boundary check for ASN.1 parser
01/28/12   SM      Fixed lint warnings 
10/28/09   SM      Adding bounds check after variable has been set
06/01/11   vg      Added data read boundary checks before reading data.
06/25/10   SM      Use macro so we can easily switch btw using
                   boot_util.h functions or regular functions
06/24/10   SM      Reverted previous change - check bound before var is set
10/28/09   SM      Adding bounds check after variable has been set
03/03/08   ANB     Added const qualifiers.
05/02/06   AMW     Initial SC2X port.
08/25/04   SJD     Initial revision.  Original SW from the security team.
06/06/04   John Alayari
                   1. Cleaned up functions to return at end of functions
                      with proper error code.
                   2. Added comments and headings to functions and corrected
                      some indentations to comply with ASW Security Services
                      Coding Guideline.
01/24/03   Jay     Initial version

==========================================================================*/

/*=========================================================================

                               INCLUDE FILES

==========================================================================*/
#include "SecBootAsn1.h"
#include "SecBootI.h"

#define BREAKIF_SETERR(cond, err) \
        if (cond)                    \
        {                         \
           err = E_ASN1_INVALID_DATA; \
           break;                          \
        }

/*=========================================================================

                            CONSTANT DECLARATIONS

==========================================================================*/
#define SECASN1_TAG_CLAS_MASK   (0xC0L)
#define SECASN1_TAG_UNIV_VAL    (0x00L)
#define SECASN1_TAG_CTXT_VAL    (0x80L)
#define SECASN1_TAG_EXT_MASK    (0x80L)
#define SECASN1_TAG_MULB_MASK   (0x1FL)
#define SECASN1_TAG_SUBEXT_MASK (0x7FL)

#define SECASN1_LENGTH_MASK     (0x80L)
#define SECASN1_LENGTH_EXT_MASK (0x7FL)
#define SECASN1_LENGTH_FACTOR   256L

/*==============================================================
  FUNCTION PBL_CLK_JULIAN_TO_SECS
  
  ARGS
    A clk_julian_type structure.

  RETURNS
    In integer, the time in secs since Jan 6 1980, the CDMA epoc
    or 0, to indicate in invalid value.  (This means that the very
    first second in the CDMA epoc is invalid.)

  DEPENDENCIES
    None.  A completely self-contained function that depends
    only on its argument.

===============================================================*/
uint32 pbl_clk_julian_to_secs(const clk_julian_type *jts) 
{
   uint32       result;
   uint32       isLeapYear = 0;
   int          yearsGoneBy, daysGoneBy;
    
   /*   Four gigaseconds --> 126.75 years; so it will cover the 120 years */
   /*   that we are attempting to validate here */

   /*  Basic Sanity checks - more specific ones to follow */
   /*   We only allow years in the range 1980.. 2099 */
   /*   Note: All fields are unsigned */
   if (   (jts == NULL) || (jts->year   < 1980) || (jts->year   > 2099)
          || (jts->month  <    1) || (jts->month  >   12) 
          || (jts->day    <    1) || (jts->day    >   31) 
          || (jts->hour   >   23) || (jts->minute >   59)
          || (jts->second >   59))
      return 0;

   /*   if "valid" but before the start of the CDMA epoch, return 0 */
   if ((jts->year == 1980) && (jts->month==1) && (jts->day<6))
    return 0;

   /*   Thirty days hath September, April, June and November */
   if ((jts->day > 30) &&
       ((jts->month == 4) || (jts->month == 6) || 
        (jts->month == 9) || (jts->month == 11 )))
    return 0;

   /*   Check for too many days in February - */
   /*   Note that we handle only 1980 -- 2099, so we can use a simple check. */
   isLeapYear = jts->year % 4 == 0 ? 1 : 0;
   if ((jts->month == 2) && (jts->day > ( 28 + isLeapYear )))
      return 0;
                
   /*   Ok, now we know that we have a valid date */
   /*   So we start by calculating the number of days */
   /*   since "the dawn of time" to the start of this year */
   /*   Add in "leap days", one for each four years + 1 for 1980 */
   yearsGoneBy = jts->year - 1980;
   daysGoneBy  = yearsGoneBy * 365 + (( yearsGoneBy + 3 ) / 4 );

   /*   Add in the days to the start of the correct month */    
   switch (jts->month) {
    case  1: /* do nothing */                 break;
    case  2: daysGoneBy +=  31;               break;
    case  3: daysGoneBy +=  59;               break;
    case  4: daysGoneBy +=  90;               break;
    case  5: daysGoneBy += 120;               break;
    case  6: daysGoneBy += 151;               break;
    case  7: daysGoneBy += 181;               break;
    case  8: daysGoneBy += 212;               break;
    case  9: daysGoneBy += 243;               break;
    case 10: daysGoneBy += 273;               break;
    case 11: daysGoneBy += 304;               break;
    case 12: daysGoneBy += 334;               break;
    default:                                  return 0; /* can't happen */
  }

   /*   If we're a leap year, we have an extra day in February. */
   /*   Add it in - but only if the month comes after February. */
   if ( isLeapYear && jts->month > 2 )
      daysGoneBy += 1;

   /*   Add in the days of the month; remember that days start with one */
   daysGoneBy += jts->day - 1;
                
   /*   However, the epoch did not start on Jan 1, 1980, but on Jan 6th */
   daysGoneBy -= 5;

   /*   Convert days --> hours */
   result = daysGoneBy * 24 + jts->hour;
        
   /*   Convert hours --> minutes */
   result = result * 60 + jts->minute;
        
   /*   Convert minutes --> seconds */
   return result * 60 + jts->second;
}



/*===========================================================================

FUNCTION SECASN1_DECODE_TAG

DESCRIPTION
  This function decodes the tag of the current ASN.1 DER encoded field.

  Assumes that all primitive tags have a tag_id of less then 31, from
  Documentation I have read, this is true.  Assumes that we will never
  encounter a tag identifier greater then 2^14 - 1, If we encounter a
  tag with a greater identifier, the value E_ASN1_INVALID_DATE is returned,
  this prevents the searching through all of memory.  Assumes that for
  a SEQUENCE there is at most 32 optional fields.

DEPENDENCIES
  None

PARAMETERS
  data       - pointer to pointer to the tag to be decoded (*data is updated by the parsing)
  data_bound - pointer to after the data (for catching bogus length)
  tag_id     - where the tag value is returned

RETURN VALUE
  If successful, E_ASN1_SUCCESS is returned with the tag_id pointer set with
  the tag_id of the universal tag or set with the entire value if the
  tag is a context-tag for optional fields.

SIDE EFFECTS
  None
===========================================================================*/
secasn1_err_type pbl_secasn1_decode_tag
(
  const uint8 **data,
  const uint8  *data_bound,
  uint32       *tag_id
)
{
  uint32 tag_class;                     /* first 2 bits of first tag byte */
  const uint8 *data_ptr;                /* pointer to the data            */
  uint32 tag;          
  secasn1_err_type _errno = E_ASN1_SUCCESS;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  do
  {
    // Check for null pointers and zero tags.  Check that we can read one
    // byte at *data.  No need to check data_bound, as it is never dereferenced.
    BREAKIF_SETERR( ( (data == NULL) || (*data == NULL) || (tag_id == NULL) ||
		   (!CHECK_DATA_BOUND(*data, 1, data_bound)) ), _errno)
    tag = BLOAD8 (*data);
    data_ptr = *data + 1; //advance    
    
   *tag_id = 0;

    /* We don't support multi-byte tag's */
    if ( ( tag & SECASN1_TAG_MULB_MASK ) == SECASN1_TAG_MULB_MASK )
    {
          _errno = E_ASN1_INVALID_DATA;
          break;
    }


    tag_class = tag & SECASN1_TAG_CLAS_MASK;

    if ( tag_class == SECASN1_TAG_UNIV_VAL )
    {
      /* Tag is a universal tag */
      *tag_id = (tag & SECASN1_TAG_MULB_MASK);
    }
    else if ( tag_class == SECASN1_TAG_CTXT_VAL )
    {
      /* Tag is a context-specific (optional) tag */
      *tag_id = tag;
    }
	else
    {
      _errno = E_ASN1_INVALID_DATA;
      break;
    }

    *data = data_ptr;
  }/*lint -e(717) */ while (FALSE);
  return _errno;
} /* pbl_secasn1_decode_tag */

/*===========================================================================

FUNCTION SECASN1_DECODE_LENGTH

DESCRIPTION
  This function decodes the length of the current ASN.1 DER encoded field.
  When this function is called, data_ptr must be pointing at the first
  byte in the length field.

DEPENDENCIES
  Assumes that no field being parsed is has a value of size more than 64K

PARAMETERS
  data   - pointer to the length data
  data_bound - pointer to after the data (for catching bogus length)
  length - where the length value is returned

RETURN VALUE
  If successful, E_ASN1_SUCCESS is returned with the length pointer set with
  the number of bytes that are contained in the value.  If the length field
  has a length longer then 64K E_ASN1_INVALID_DATA is returned

SIDE EFFECTS
  None
===========================================================================*/
secasn1_err_type pbl_secasn1_decode_length
(
  const uint8 **data,
  const uint8  *data_bound,
  uint32       *length
)
{
  uint32 num_bytes = 0;                     /* number of bytes in the tag */
  const uint8 *data_ptr;                    /* pointer to the data        */
  uint32 loc_length = 0;
  secasn1_err_type _errno = E_ASN1_SUCCESS;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Sanity Check on pointer arguments */
  if ( (data == NULL) || (*data == NULL) || (length == NULL) )
  {
    return E_ASN1_INVALID_ARG;
  }

  *length = 0;
  data_ptr = *data;

  do
  {
    // about to access 1 byte (first byte of length, so check
    BREAKIF_SETERR(!CHECK_DATA_BOUND(data_ptr, 1, data_bound), _errno)
    if ( BLOAD8(data_ptr) & SECASN1_LENGTH_MASK )
    {
      /* Length bytes are in the long form */
      // Just checked above, and data_ptr has not advanced.  
      // No need to check again.
      num_bytes = BLOAD8(data_ptr) & SECASN1_LENGTH_EXT_MASK;
      /* Per spec the value '11111111'b shall not be used. */
      /* Just need to check for for 0x7F as the 8th bit we */
      /* know is already 1 */
      BREAKIF_SETERR(num_bytes == SECASN1_LENGTH_EXT_MASK, _errno);
      // advance data_ptr over byte just read.  Already checked.
      data_ptr++;
      if ( num_bytes == 0 )
      {
        /* The ASN.1 indefinite length is not supported. */
        _errno = E_ASN1_INVALID_DATA;
        break;
      }

      /* Calculate the size of the value.  The check is inside the loop 
         in case the size is really large. */
      while ( num_bytes != 0 )
      {
        loc_length *= SECASN1_LENGTH_FACTOR;
	    // Check before reading 1 byte.
        BREAKIF_SETERR(!CHECK_DATA_BOUND(data_ptr, 1, data_bound), _errno)
        loc_length += BLOAD8(data_ptr);
        if (loc_length > SECASN1_MAX_LEN)
        {
          _errno = E_ASN1_INVALID_DATA;
          break;
        }
        data_ptr++;  // advance over byte just read
        --num_bytes;
      }
      if(_errno != E_ASN1_SUCCESS)
      {
        break;
      }
    }
    else
    {
      /* Length bytes are in the short form; can't overflow  */
      // Check that we can read one byte.
      BREAKIF_SETERR(!CHECK_DATA_BOUND(data_ptr, 1, data_bound), _errno)
      loc_length = BLOAD8(data_ptr);
      data_ptr++;
    }

    /* Final check for consistency */
    /* Make sure we could actually read loc_length bytes */
    BREAKIF_SETERR(!CHECK_DATA_BOUND(data_ptr, loc_length, data_bound), _errno)

    *data = data_ptr;
    *length = loc_length; 

  }while(FALSE);

  return _errno;
} /* pbl_secasn1_decode_length */

/*===========================================================================

FUNCTION SECASN1_START

DESCRIPTION
  Starts an ASN.1 DER encoding by creating an initial container for the
  next_field() function

DEPENDENCIES
  None

PARAMETERS
  data    - pointer to the data
  data_bound - pointer to after the data (for catching bogus length)
  ret_ptr - pointer to the returned data to start the ASN.1 block

RETURN VALUE
  E_ASN1_SUCCESS - if the sequence is properly started
  E_ASN1_INVALID_ARG - if the pointer arguments have a NULL value
  E_ASN1_INVALID_TAG - if it is not a sequence tag at the beginning
  E_ASN1_INVALID_DATA - if the top-level object would extend beyond
                        data bound

SIDE EFFECTS
  None
===========================================================================*/
secasn1_err_type pbl_secasn1_start
(
  const uint8 *data,
  const uint8 *data_bound,
  secasn1_data_type *ret_ptr
)
{
  uint32 tag_id;                                     /* current tag id     */
  secasn1_err_type _errno = E_ASN1_INVALID_ARG;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    /* Make sure data and ret_ptr are non-null */
    if ( (data == NULL) || (ret_ptr == NULL) || 
            !CHECK_DATA_BOUND(data, 0, data_bound) ) break;

    /* Decode the tag and advance the data_ptr */
    /* pbl_scasn1_decode tag does its own checking */
    _errno = pbl_secasn1_decode_tag( &data, data_bound, &tag_id );
    if (_errno != E_ASN1_SUCCESS ) break;

    /* Verify the tag is for a sequence object */
    if ( tag_id != SECASN1_SEQUENCE_TYPE )
    {
      _errno =  E_ASN1_INVALID_TAG;
      break;
    }

    /* Decode the length of the field and advance the data_ptr */
    /* pbl_secasn1_decode_length() does its own checking */
    _errno = pbl_secasn1_decode_length( &data, data_bound, &ret_ptr->len );
    if (_errno != E_ASN1_SUCCESS ) break;

    /* check that length is within our limits */
    if (ret_ptr->len > SECASN1_MAX_LEN) {
      _errno = E_ASN1_INVALID_DATA;
      break;
    }

    /* Set the data and data_bound fields in the output structure.  */
    ret_ptr->data = data;
    ret_ptr->data_bound = data_bound;

    /* Final check for consistency */
    if (!CHECK_ASN1_INVARIANTS(*ret_ptr)) {
      _errno = E_ASN1_INVALID_DATA;
      break;
    }

  }/*lint -e(717) */ while (FALSE);
  return _errno;
} /* pbl_secasn1_start */

/*===========================================================================

FUNCTION SECASN1_END

DESCRIPTION
  closes and verifies an ASN.1 DER encoding, by checking that no data
  has been left unprocessed at the end of the stream

DEPENDENCIES
  None

PARAMETERS
  data_ptr - pointer to the end of the data holder

RETURN VALUE
  E_ASN1_SUCCESS - if the document has been closed successfully
  E_ASN1_INVALID_ARG - if the pointer argument has a NULL value
  E_ASN1_INVALID_DATA - all the data was not processed

SIDE EFFECTS
  None
===========================================================================*/
secasn1_err_type pbl_secasn1_end
(
  const secasn1_data_type *data_ptr
)
{
  secasn1_err_type _errno = E_ASN1_SUCCESS;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  do
  {
    if ( data_ptr == NULL )
    {
      _errno = E_ASN1_INVALID_ARG;
      break;
    }
    /* Check that we have consumed all the data. */
       if ( data_ptr->len != 0 )
       {
         _errno = E_ASN1_INVALID_DATA;
         break;
       }

    /* check that data has not passed data_bound */
    BREAKIF_SETERR(!CHECK_DATA_BOUND(data_ptr->data, 0, data_ptr->data_bound),
		     _errno)

  }/*lint -e(717) */ while (FALSE);

  return _errno;
} /* pbl_secasn1_end */

/*===========================================================================

FUNCTION SECASN1_CLOSE_SEQUENCE

DESCRIPTION
  This function verifies that all the data in the inner sequence has
  been processed.  The outer sequence is included in the case that
  a sequence has an undefined length. The ASN.1 indefinite length is 
  not supported.

DEPENDENCIES
  pbl_secasn1_next_field() to start a sequence value has been called

PARAMETERS
  outer - pointer to the outer sequence
  inner - pointer to the inner sequence

RETURN VALUE
  E_ASN1_SUCCESS - if the sequence was successful closed
  E_ASN1_INVALID_ARG - if a pointer argument has a NULL value
  E_ASN1_INVALID_DATA - otherwise

SIDE EFFECTS
  None
===========================================================================*/
secasn1_err_type pbl_secasn1_close_sequence
(
  secasn1_data_type *outer,
  secasn1_data_type *inner
)
{
   secasn1_err_type _errno = E_ASN1_SUCCESS;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    if ( inner == NULL ||
         outer == NULL )
    {
      _errno = E_ASN1_INVALID_ARG;
      break;
    }

    // Normal case:  All the inner data should have been parsed, hence 
    // inner->len should be zero, and inner->data and 
    // outer _data should be at the same address.
    if ( inner->len != 0 || inner->data != outer->data) {
        _errno = E_ASN1_INVALID_DATA;
        break;
      }
  }/*lint -e(717) */ while (FALSE);
  return _errno;

} /* pbl_secasn1_close_sequence */


/*===========================================================================

FUNCTION SECASN1_NEXT_FIELD

DESCRIPTION
  This function parses the next field of data in the ASN.1 DER encoding.
  The function points the secasn1_data_type at the data contained within
  the field.  ret_ptr must be non-null or the error E_ASN1_INVALID_ARG 
  will be returned.  If verify_tag_id != 0, then the function verifies the 
  tag_id against the current fields tags.  If the tag_ids do not match an 
  error code of E_ASN1_INVALID_TAG is returned.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - pointer to the data to be parsed
  ret_ptr  - where the inner field data is returned
  verify_tag_id - tag value to check the tag against

RETURN VALUE
  If successful, E_ASN1_SUCCESS will be returned and ret_ptr will be updated
  to contain the data in the field.  If an error occurs during the parsing
  the appropriate error will be returned.  On an error the data_ptr is
  returned to the initial position

SIDE EFFECTS
  None
===========================================================================*/
secasn1_err_type pbl_secasn1_next_field
(
  secasn1_data_type *data_ptr,
  secasn1_data_type *ret_ptr,
  uint32             verify_tag_id
)
{
  uint32 tag_id;                                 /* Tag Id of current tag  */
  secasn1_data_type init_data;                   /* original data */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  secasn1_err_type _errno = E_ASN1_SUCCESS;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    /* Make sure ret_ptr and data is okay*/
    if ( ret_ptr == NULL ||
         data_ptr == NULL  || 
         !CHECK_ASN1_INVARIANTS(*data_ptr))
    {
      _errno = E_ASN1_INVALID_ARG;
      break;
    }

    /* save orginal data */
    init_data = *data_ptr;

    /* Check to see if there is no data left */
    if ( data_ptr->len == 0 )
    {
      _errno = E_ASN1_NO_DATA;
      break;
    }

    /* Decode the tag and advance the data_ptr, checking that the data
     pointer is not advancing out of the outer field. (Checking is
     done in pbl_secasn1_decode_tag.) */
    _errno = pbl_secasn1_decode_tag( &data_ptr->data, data_ptr->data_bound, 
                                     &tag_id );
    if ( _errno != E_ASN1_SUCCESS )
    {
      // restore original content
      *data_ptr = init_data;
      break;
    }
    /* Check if the tag_id needs to be verified */
    if ( verify_tag_id != SECASN1_NO_TYPE_CHECK &&
         verify_tag_id != tag_id )
    {
      *data_ptr = init_data;
      _errno = E_ASN1_INVALID_TAG;
      break;
    }

    /* Decode the length of the field and advance the data_ptr.  Fails
     if length > SECASN1_MAX_LEN */
    _errno = pbl_secasn1_decode_length( &data_ptr->data, data_ptr->data_bound,
                                        &ret_ptr->len );
    if ( _errno != E_ASN1_SUCCESS )
    {
      *data_ptr = init_data;
      break;
    }


    /* Copy data field from data_ptr struct */
    ret_ptr->data = data_ptr->data;

    /* check that we can advance data_ptr->data by ret_ptr->len */
    if (!CHECK_DATA_BOUND(data_ptr->data, ret_ptr->len, 
	    data_ptr->data_bound))
    {
      _errno = E_ASN1_INVALID_DATA;
      break;        
    }
    data_ptr->data += ret_ptr->len;

    /* Decrease the outer length by the amount that the data field has 
       advanced. */
    data_ptr->len -= (data_ptr->data - init_data.data);
    /* Check for underflow in CHECK_ASN_INVARANTS below */
    /* Set up data_bound for ret_ptr */
    /* This pulls data_bound for the inner object to just off the end
       of the inner object.  The add cannot overflow because the
       check a few lines up ensures that the value of data_ptr->data at
       that point could have ret_ptr->len added to it safely.  And
       ret_ptr->data is the same as data_ptr->data was at the time.
    */
    ret_ptr->data_bound = MIN(init_data.data_bound, 
                               ret_ptr->data + ret_ptr->len);

    /* Final checks. */
    if (!CHECK_ASN1_INVARIANTS(*data_ptr) || 
        !CHECK_ASN1_INVARIANTS(*ret_ptr)) {
       _errno = E_ASN1_INVALID_DATA;
       break;
    }
  }/*lint -e(717) */ while (FALSE);
  return _errno;
} /* pbl_secasn1_next_field */

/*===========================================================================

FUNCTION SECASN1_DECODE_BIT_STRING

DESCRIPTION
  This function takes a data pointer and decodes it to a bit_string pointer
  In the ASN.1 DER encoding, the first byte of a bit string indicates, how
  many unused bits are at the end of the string.  Assumes that the bit_ptr
  has been pre-allocated.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - pointer to the bit string field
  bit_ptr  - where the decoded bit string will be returned


RETURN VALUE
  If successful, E_ASN1_SUCCESS will be returned and bit_ptr will be updated
  to contain the bit string.  If an error occurs during the conversion
  the appropriate error will be returned.

SIDE EFFECTS
  None.  The data_ptr->data and data_ptr->len values are not updated.
===========================================================================*/
secasn1_err_type pbl_secasn1_decode_bit_string
(
  const secasn1_data_type *data_ptr,
  secasn1_bit_string_type *bit_ptr
)
{
  uint8 unused_bits;         /* Number of unused bits   */
  secasn1_err_type _errno = E_ASN1_SUCCESS;
  /*- - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    /* Check that each of the parameters have been pre-allocated */
    if ( data_ptr == NULL ||
         bit_ptr == NULL)
    {
      _errno = E_ASN1_INVALID_ARG;
      break;
    }

    /* Check that the data field contains at least 1 byte (for the unused) */
    if ( data_ptr->len == 0 || data_ptr->data == NULL)
    {
      _errno = E_ASN1_INVALID_DATA;
      break;
    }

    // check all the memory space of data_ptr->len is accessable in this function
    BREAKIF_SETERR((!CHECK_DATA_BOUND(data_ptr->data, data_ptr->len, data_ptr->data_bound)), 
		           _errno);
    unused_bits = BLOAD8(data_ptr->data);
    if ( ( unused_bits > 7 ) || ( ( data_ptr->len == 1 ) && unused_bits > 0) )
    {
      /* At most there can only be 7 unused bits in a byte and no unused bits in a 0 len string */
      _errno = E_ASN1_INVALID_DATA;
      break;
    }

    /* Valid Bit String */
    /* Already checked that we can add data_ptr->len to data_ptr->data, and 
       data->pointer->len >= 1, so the add is is safe and the subtract is 
       safe. */
    bit_ptr->unused = unused_bits;
    bit_ptr->data = data_ptr->data + 1;
    bit_ptr->len = data_ptr->len - 1;

  }/*lint -e(717) */ while (FALSE);
  return (_errno);
} /* pbl_secasn1_decode_bit_string */

/*===========================================================================

FUNCTION SECASN1_DECODE_BOOLEAN

DESCRIPTION
  This function takes a data pointer and decodes it to a boolean pointer
  In the ASN.1 DER encoding, the boolean data should only be a single byte,
  with 0x00 meaning FALSE and anything else being true

DEPENDENCIES
  None

PARAMETERS
  data_ptr - pointer to the boolean field
  b_ptr - pointer where the boolean value will be returned

RETURN VALUE
  If successful, E_ASN1_SUCCESS will be returned and the boolean pointer will
  will be set.  If the data_ptr contains more than a single byte
  E_ASN1_INVALID_DATA will be returned

SIDE EFFECTS
  None
===========================================================================*/
secasn1_err_type pbl_secasn1_decode_boolean
(
  const secasn1_data_type *data_ptr,
  boolean *b_val
)
{
  secasn1_err_type _errno = E_ASN1_SUCCESS;
  /*- - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    /* Check that each of the parameters have been pre-allocated */
    if ( data_ptr == NULL ||
         b_val == NULL )
    {
      _errno = E_ASN1_INVALID_ARG;
      break;
    }

    /* Check that the data field contains only one byte and is non-null */
    if ( data_ptr->len != 1 || data_ptr->data == NULL)
    {
      _errno = E_ASN1_INVALID_DATA;
      break;
    }

    /* Check that it is safe to read one byte */
    BREAKIF_SETERR(!CHECK_DATA_BOUND(data_ptr->data, 1, data_ptr->data_bound),
       _errno);
    
    /* Valid boolean encoding */
    if ( BLOAD8(data_ptr->data) == 0 )
    {
      *b_val = FALSE;
    }
    else
    {
      *b_val = TRUE;
    }
  }/*lint -e(717) */ while (FALSE);
  return _errno;
} /* pbl_secasn1_decode_boolean */

/*===========================================================================

FUNCTION SECASN1_DECODE_TIME

DESCRIPTION
  This function takes a data pointer representing time and decodes it to
  the number of seconds since CDMA epoch time of Jan 6th, 1980.  If the time
  occurs before this date, the time is adjusted to 0.

DEPENDENCIES
  None

PARAMETERS
  data_ptr - pointer to the time data field
  time     - pointer where the time in seconds is returned
  utc_time - whether the time is in UTC time format

RETURN VALUE
  If successful, E_ASN1_SUCCESS will be returned and the time pointer will
  will be set.  E_ASN1_INVALID_DATA is returned if an invalid format is
  encountered.

SIDE EFFECTS
  None
===========================================================================*/
secasn1_err_type pbl_secasn1_decode_time
(
  const secasn1_data_type *data_ptr,
  uint32  *time,
  boolean utc_time
)
{
  const uint8 *str;                         /* pointer to the time string */
  int i;                                    /* counter variable */
  clk_julian_type jts;                      /* julian clock variable */
  secasn1_err_type _errno = E_ASN1_SUCCESS;
  /*- - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    /* Check that each of the parameters have been pre-allocated */
    if ( data_ptr == NULL ||
         time == NULL )
    {
      _errno = E_ASN1_INVALID_ARG;
      break;
    }

    // Check that we can read the entire structure
    BREAKIF_SETERR((!CHECK_DATA_BOUND(data_ptr->data, data_ptr->len, data_ptr->data_bound)),
                   _errno);

    i = data_ptr->len;
    str = data_ptr->data;

    /* Verify the length of the data and calculate the 4-digit year */
    if ( utc_time )
    {
      //YYMMDDHHMMSSZ
      if ( (i != 13) ) return E_ASN1_INVALID_DATA;

      jts.year = (BLOAD8(str)-0x30L)*10;
      str++;

      jts.year +=  (BLOAD8(str)-0x30L);
      str++;

      /* Adjust the year as specified in RFC 2459 */
      /* if year >= 50 ==> 19YY else ==> 20YY     */
      if ( jts.year >= 50 )
      {
        jts.year += 1900;
      }
      else
      {
        jts.year += 2000;
      }
    }
    else
    {
      //YYYYMMDDHHMMSSZ
      if ( i != 15 )
      {
        _errno = E_ASN1_INVALID_DATA;
        break;
      }
      //jts.year = (BLOAD8(str++)-0x30L) * 1000 + (BLOAD8(str++)-0x30L) *
      //         100 + (BLOAD8(str++)-0x30L) * 10 + (BLOAD8(str++)-0x30L);


      jts.year = (BLOAD8(str)-0x30L) * 1000;
      str++;

      jts.year = jts.year + (BLOAD8(str)-0x30L) * 100;
      str++;

      jts.year = jts.year + (BLOAD8(str)-0x30L) * 10;
      str++;

      jts.year = jts.year + (BLOAD8(str)-0x30L);
      str++;

    }

    /* Calculate the month */
    // jts.month = (BLOAD8(str++)-0x30L) * 10 + (BLOAD8(str++)-0x30L);
    jts.month = (BLOAD8(str)-0x30L) * 10;
    str++;

    jts.month = jts.month + (BLOAD8(str)-0x30L);
    str++;

    /* Calculate the day */
    //jts.day = (BLOAD8(str++)-0x30L) * 10 + (BLOAD8(str++)-0x30L);
    jts.day = (BLOAD8(str)-0x30L) * 10;
    str++;

    jts.day = jts.day + (BLOAD8(str)-0x30L);
    str++;

    /* Calculate the hour */
    //jts.hour = (BLOAD8(str++)-0x30L) * 10 + (BLOAD8(str++)-0x30L);

    jts.hour = (BLOAD8(str)-0x30L) * 10;
    str++;

    jts.hour = jts.hour + (BLOAD8(str)-0x30L);
    str++;

    /* Calculate the minute */
    //jts.minute = (BLOAD8(str++)-0x30L) * 10 + (BLOAD8(str++)-0x30L);

    jts.minute = (BLOAD8(str)-0x30L) * 10;
    str++;
    jts.minute = jts.minute + (BLOAD8(str)-0x30L);
    str++;

    /* Calculate the second */
    jts.second = (BLOAD8(str)-0x30L) * 10;
    str++;

    jts.second +=  (BLOAD8(str)-0x30L);
    str++;

    /* Calculate the time offset */
    if ( BLOAD8(str) != 'Z' )
    {
      _errno = E_ASN1_INVALID_DATA;
      break;
    }

    *time = pbl_clk_julian_to_secs(&jts);

  }/*lint -e(717) */ while (FALSE);

  return _errno;
} /* pbl_secasn1_decode_time */

/*end*/

