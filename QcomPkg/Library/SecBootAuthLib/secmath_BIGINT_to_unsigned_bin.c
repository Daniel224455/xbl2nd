/**
  @file secmath_BIGINT_to_unsigned_bin.c

  @brief  Write a big integer into a buffer
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

SECMATH_ERRNO_ET secmath_BIGINT_to_unsigned_bin(const BigInt * a, uint8 * buf,
                                                uint32 size)
{
  BigInt tmpa;
  SECMATH_ERRNO_ET ret;

  if ((NULL == a) || (NULL == buf))
  {
    return E_SECMATH_INVALID_ARG;
  }

  if (a->n < 0 || sizeof(a->a) / sizeof(a->a[0]) < a->n)
  {
    return E_SECMATH_INVALID_ARG;
  }

  if (size < (a->n * sizeof(a->a[0])) / sizeof(*buf))
  {
    return E_SECMATH_BUFFER_OVERFLOW;
  }

  /* Flip bytes to the correct order MSword on the left */
  if ((ret = secmath_BIGINT_read_unsigned_bin(&tmpa, (uint8 *) (a->a), a->n * BLONG_SIZE))
      != E_SECMATH_SUCCESS)
  {
    return ret;
  }

  memcpy(buf, (uint8 *) (tmpa.a), a->n * BLONG_SIZE);
  return E_SECMATH_SUCCESS;
}
