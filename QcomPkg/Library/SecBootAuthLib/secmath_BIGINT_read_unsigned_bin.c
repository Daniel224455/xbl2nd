/**
  @file secmath_BIGINT_read_unsigned_bin.c

  @brief Read an unsigned buffer of bytes into a big integer
*/

/*===========================================================================
   Copyright (c) 2011-2013 by Qualcomm Technologies Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE


when       who      what, where, why
--------   ---      ------------------------------------
05/11/11   vg             Initial Revision

===========================================================================*/

#include <stddef.h>
#include "secmath_utils.h"
#include "string.h"

SECMATH_ERRNO_ET secmath_BIGINT_read_unsigned_bin(BigInt * a, const uint8 * buf,
                                                  uint32 len)
{
  int32 i = 0;
  uint32 j = 0;

  if ( (NULL == a) || (NULL == buf) || (0 == len))
  {
    return E_SECMATH_INVALID_ARG;
  }
  
  if(len > sizeof(a->a))
  {
    return E_SECMATH_BUFFER_OVERFLOW;
  }
  
  memset(a->a, 0, sizeof(a->a));

  /* process bytes from least significant to most significant */
  for (i = len - 1; i >= 0; --i)
  {
    // Endianess is preserved - LSBlong to MSBlong
    (*a).a[j / 4] |= buf[i] << ((j % 4) * 8);
    ++j;
  }
  
  /* Set a->n to minimum number of words */
  a->n = (len + BLONG_SIZE - 1) / BLONG_SIZE;

  /* Strip off leading zero's. If all the words have value zero */
  /* this allows 1 zero word, because bignum math assumes */
  /* buffers are minimum 1 word long, and for certain algorithms */
  /* (future) a zero value is valid */
  while( (a->n > 1) && (a->a[a->n - 1] == 0))
  {
    --a->n;
  }
  
  return E_SECMATH_SUCCESS;
}
