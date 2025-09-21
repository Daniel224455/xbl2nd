/**
  @file secmath_hex_str_to_bin.c

  @brief Convert an ASCII hex string to into a binary buffer
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
#include "SecMath.h"
#include "secmath_utils.h"
#include "string.h"

#define STRNLEN  secmath_no_c_strnlen

size_t secmath_no_c_strnlen(const char *, size_t);


SECMATH_ERRNO_ET secmath_hex_str_to_bin(uint8 * buf, uint32 buflen,
                                        const char *str, uint32 * total_len)
{
  uint32 i = 0;
  uint32 j = 0;
  char ch;
  int8 v;

  if ((NULL == buf) || (NULL == str) || (0 == buflen) || (NULL == total_len))
  {
    return E_SECMATH_INVALID_ARG;
  }

  memset(buf, 0, buflen);

  if (STRNLEN(str, (((MAX_KEY_SIZE_IN_BITS+7)/8)*2)+1) & 1)
    j = 1;

  while (((ch = str[i]) != '\0') && ((j/2) < buflen))
  {
    int tmp_ch_v = ch;
    if ((tmp_ch_v >= 0x30) && (tmp_ch_v <= 0x39))
    {
      v = (unsigned char) ch - '0';
    }
    else if ((tmp_ch_v >= 0x41) && (tmp_ch_v <= 0x5a))
    {
      v = (unsigned char) ch - 'A' + 10;
    }
    else if ((tmp_ch_v >= 0x61) && (tmp_ch_v <= 0x7a))
    {
      v = (unsigned char) ch - 'a' + 10;
    }
    else
    {
      return E_SECMATH_FAILURE;
    }

    ++i;
    if (v >= 0 && v <= 0xf)
    {
      if ((j & 1) == 0)
      {
        buf[j / 2] = v << 4;
      }
      else
      {
        buf[j / 2] |= v;
      }
      ++j;
    }
  }

  *total_len = (j / 2);

  return E_SECMATH_SUCCESS;
}

size_t secmath_no_c_strnlen(const char *s, size_t max)
{
  register size_t count = 0;
  const char *p = s;
  while (max-- && *p)
  {
    ++p;
    ++count;
  }

  return count;
}

