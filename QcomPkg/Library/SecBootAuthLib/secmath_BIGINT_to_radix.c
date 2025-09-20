/**
  @file secmath_BIGINT_to_radix.c

  @brief Write a big integer into NULL terminate string
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

SECMATH_ERRNO_ET secmath_BIGINT_to_radix(const BigInt * a, char *str, uint32 * len,
                               uint32 radix)
{
  SECMATH_ERRNO_ET ret = E_SECMATH_FAILURE;
  int i = 0;
  BigInt tmpa;

  if ((NULL == a) || (NULL == str) || (NULL == len))
  {
    return E_SECMATH_INVALID_ARG;
  }

  if ((radix != 16) && (radix != 10) && (radix != 8) && (radix != 2))
  {
    return E_SECMATH_INVALID_ARG;
  }

  /* Flip bytes to the correct order MSword on the left */
  if ((ret = secmath_BIGINT_read_unsigned_bin(&tmpa, (uint8 *) (a->a), sizeof(a->a)))
      != E_SECMATH_SUCCESS)
  {
    return ret;
  }

  // Strip off leading 0's
  for (i = 0; i < sizeof(tmpa.a); ++i)
  {
    if (((uint8 *) tmpa.a)[i] != 0)
    {
      break;
    }
  }

  if (radix == 16)
  {
    if (*len / 2 < sizeof(tmpa.a) - i)
    {
      return E_SECMATH_BUFFER_OVERFLOW;
    }

    ret = secmath_bin_to_hex_str(str, *len, (uint8 *) tmpa.a + i,
                                 sizeof(tmpa.a) - i);
    *len = 2 * (sizeof(tmpa.a) - i);
  }
  else
  {
    ret = E_SECMATH_NOT_SUPPORTED;
  }

  return ret;
}
