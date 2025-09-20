/**
  @file secmath_bin_to_hex_str.c

  @brief Convert a binary buffer to a hex string
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

SECMATH_ERRNO_ET secmath_bin_to_hex_str(char *str, uint32 len, const uint8 * buf,
                              uint32 buflen)
{
  uint32 i = 0;
  uint32 j = 0;
  int8 v;

  if ((NULL == str) || (NULL == buf) || (0 == len) || (0 == buflen))
  {
    return E_SECMATH_INVALID_ARG;
  }

  if (len < ((buflen * 2) + 1))
    return E_SECMATH_BUFFER_OVERFLOW;

  for (i = 0; i < buflen; ++i)
  {
    for (j = 0; j < 2; ++j)
    {
      v = (j & 1) ? (buf[i] & 0x0f) : (buf[i] >> 4) & 0x0f;
      if (v >= 0 && v <= 9)
        str[(i * 2) + j] = '0' + v;
      else
        str[(i * 2) + j] = 'A' + (v - 10);
    }
  }

  str[2*i] = '\0';
  return E_SECMATH_SUCCESS;
}
