/*
 *  $Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/Tools/storage/fh_loader/fh_loader/stringl/stringl.h#1 $
 *  $DateTime: 2021/04/29 02:01:56 $
 */

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)string.h	5.10 (Berkeley) 3/9/91
 */

#ifndef _STRINGL_H_
#define	_STRINGL_H_

#include <stdio.h>
#include <string.h>
#include <wchar.h>

/** @addtogroup dxy_stringl
  @{ */

//
// AEEstd.h header error codes
//
#ifndef STD_NODIGITS
    #define STD_NODIGITS   1    /**< See std_scanul(). */
#endif

#ifndef STD_NEGATIVE
    #define STD_NEGATIVE   2    /**< See std_scanul(). */
#endif

#ifndef STD_OVERFLOW
    #define STD_OVERFLOW   3    /**< See std_scanul(). */
#endif

#ifndef STD_BADPARAM
    #define STD_BADPARAM   4    /**< See std_scanul(). */
#endif

/**< UTF-16 2-byte wide char type */
typedef unsigned short wchar;

#ifdef __cplusplus
namespace std
{
    extern "C"
    {
#endif //__cplusplus

/**
Added these macros for supporting compilation on Win based
software dev environments like VC, .Net etc.
*/
#ifdef _WIN32
   #define snprintf     _snprintf
   #define vsnprintf    _vsnprintf
#endif

/**
  strlcat - Size bounded string concatenation.

  Concatenates the source string to destination string.

  This function ensures that the destination string will
  not be improperly terminated and that there will be
  no concatenation beyond the size of the destination buffer.

  @param[in,out]  dst   Destination buffer.
  @param[in]      src   Source string.
  @param[in]      siz   Size of the destination buffer in bytes.

  @return
  The length of the string that was attempted to be created,
  i.e. the sum of the source and destination strings.

  @dependencies
  None.
*/
size_t strlcat(char *dst, const char *src, size_t siz);

/**
  wcslcat - Size bounded wide string concatenation using
  C standard wide character data type wchar_t.

  Concatenates the source string to destination string.

  This function ensures that the destination string will
  not be improperly terminated and that there will be
  no concatenation beyond the size of the destination buffer.

  @param[in,out]  dst   Destination buffer.
  @param[in]      src   Source string.
  @param[in]      siz   Size of the destination buffer in units of wchar_t.

  @return
  The length of the string that was attempted to be created,
  i.e. the sum of the source and destination strings.

  @note It has been observed that wchar_t on some platforms is
  2 bytes wide (UTF-16) and on others is 4 bytes wide (UTF-32).
  So carefully consider this when using the data type wchar_t
  and this API in your application.

  @dependencies
  None.
*/

size_t wcslcat(wchar_t *dst, const wchar_t *src, size_t siz);

/**
  wstrlcat - Size bounded wide string concatenation using 2 byte
  wide (UTF-16) character data type wchar.

  Concatenates the source string to destination string.

  This function ensures that the destination string will
  not be improperly terminated and that there will be
  no concatenation beyond the size of the destination buffer.

  @param[in,out]  dst   Destination buffer.
  @param[in]      src   Source string.
  @param[in]      siz   Size of the destination buffer in units of wchar.

  @return
  The length of the string that was attempted to be created,
  i.e. the sum of the source and destination strings.

  @dependencies
  None.
*/
size_t wstrlcat(wchar* dst, const wchar* src, size_t siz);

/**
  strlcpy - Size bounded string copy.

  Copies the source string to the destination buffer.

  This function ensures that the destination buffer will always
  be NULL terminated and that there will not be a copy beyond
  the size of the destination buffer.

  @param[out] dst   Destination buffer.
  @param[in]  src   Source String.
  @param[in]  siz   Size of the destination buffer in bytes.

  @return
  The length of the source string.

  @dependencies
  None.
*/
size_t strlcpy(char *dst, const char *src, size_t siz);

/**
  wcslcpy - Size bounded wide string copy using
  C standard wide character data type wchar_t.

  Copies the source string to the destination buffer.

  This function ensures that the destination buffer will always
  be NULL terminated and that there will not be a copy beyond
  the size of the destination buffer.

  @param[out] dst   Destination buffer.
  @param[in]  src   Source String.
  @param[in]  siz   Size of the destination buffer in units of wchar_t.

  @return
  The length of the source string.

  @note It has been observed that wchar_t on some platforms is
  2 bytes wide (UTF-16) and on others is 4 bytes wide (UTF-32).
  So carefully consider this when using the data type wchar_t
  and this API in your application.

  @dependencies
  None.
*/

size_t wcslcpy(wchar_t *dst, const wchar_t *src, size_t siz);

/**
  wstrlcpy - Size bounded wide string copy using 2 byte
  wide (UTF-16) character data type wchar.

  Copies the source string to the destination buffer.

  This function ensures that the destination buffer will always
  be NULL terminated and that there will not be a copy beyond
  the size of the destination buffer.

  @param[out] dst   Destination buffer.
  @param[in]  src   Source String.
  @param[in]  siz   Size of the destination buffer in units of wchar.

  @return
  The length of the source string.

  @dependencies
  None.
*/
size_t wstrlcpy(wchar* dst, const wchar* src, size_t siz);

/**
  wstrlen - Returns the number of characters in the source string.
  Used for strings based on wchar data type i.e. 2 byte wide (UTF-16)
  characters.

  @param[in]  src   Source String.

  @return
  The number of characters in the source string.

  @dependencies
  None.
*/
size_t wstrlen(const wchar *src);

/**
  wstrcmp - Compares wchar (UTF-16) string s1 to the wchar string s2.

  This function starts comparing the first character of each string.
  If they are equal to each other, it continues with the following
  pairs until the characters differ or until a terminating
  null-character is reached.

  @param[in]  s1   String to be compared.
  @param[in]  s2   String to be compared against.

  @return
  0  - Indicates that the strings are equal.
  >0 - Indicates that the strings are not equal and a character in s1 is
       greater than the corresponding character in s2.
  <0 - Indicates that the strings are not equal and a character in s1 is
       lesser than the corresponding character in s2.

  @dependencies
  None.
*/
int wstrcmp(const wchar *s1, const wchar *s2);

/**
  wstrncmp - Compares upto n wchar (UTF-16) characters in string s1
  to the wchar string s2.

  This function starts comparing the first character of each string.
  If they are equal to each other, it continues with the following
  pairs until the characters differ or until a terminating
  null-character is reached or n comparisons have been performed.

  @param[in]  s1   String to be compared.
  @param[in]  s2   String to be compared against.
  @param[in]  n    Nmber of character to be compared.

  @return
  0  - Indicates that the strings are equal.
  >0 - Indicates that the strings are not equal and a character in s1 is
       greater than the corresponding character in s2.
  <0 - Indicates that the strings are not equal and a character in s1 is
       lesser than the corresponding character in s2.

  @dependencies
  None.
*/
int wstrncmp(const wchar *s1, const wchar *s2, size_t n);

/**
  strcasecmp - compare two strings ignoring case.

  @param[in] s1 First string.
  @param[in] s2 Second string.

  @return
  The strcasecmp() and strncasecmp() functions return an integer
  less than, equal to, or greater than zero if s1 (or the first
  n bytes  thereof)  is found, respectively, to be less than, to
  match, or be greater than s2.

  @dependencies
  None.
*/
int strcasecmp(const char * s1, const char * s2);

/**
  strncasecmp - compare two strings ignoring case (sized).

  @param[in] s1 First string.
  @param[in] s2 Second string.
  @param[in] n  The number of characters to compare (from the
                beginning).

  @return
  The strcasecmp() and strncasecmp() functions return an integer
  less than, equal to, or greater than zero if s1 (or the first
  n bytes  thereof)  is found, respectively, to be less than, to
  match, or be greater than s2.

  @dependencies
  None.
*/
int strncasecmp(const char * s1, const char * s2, size_t n);

/**
std_scanul()

Description:

  The std_scanul() converts an ASCII representation of a number
  to an unsigned long.  It expects strings that match the
  following pattern:

       spaces [+|-] digits


  'Spaces' is zero or more ASCII space or tab characters.

  'Digits' is any number of digits valid in the radix.  Letters
  'A' through 'Z' are treated as digits with values 10 through
  35. 'Digits' may begin with "0x" when a radix of 0 or 16 is
  specified.

  Upper and lower case letters can be used interchangeably.

  @param[in]  pchBuf    The start of the string to scan.

  @param[in]  nRadix    The numeric radix (or base) of the
                        number. Valid values are 2 through 36 or zero,
                        which implies auto-detection. Auto-detection
                        examines the digits field.  If it begins with
                        "0x", radix 16 is selected.  Otherwise, if it
                        begins with "0" radix 8 is selected.
                        Otherwise, radix 10 is selected.

  @param[out] ppchEnd   If ppchEnd is not NULL, *ppchEnd
                        points to the first character that did not
                        match the expected pattern shown above,
                        except on STD_BADPARAM and STD_OVERFLOW when
                        it is set to the start of the string.

  @param[out] pnError   If pnError is not NULL, *pnError
                        holds the error code, which is one of the
                        following:

        0            : Numeric value is from 0 to
                       MAX_UINT32.

        STD_NEGATIVE : The scanned value was negative and its absolute value was
                       from 1 to MAX_UINT32.  The result is the negated value
                       (cast to a uint32).

        STD_NODIGITS : No digits were found.  The result is zero.

        STD_OVERFLOW : The absolute value exceeded MAX_UINT32.  The result
                       is set to MAX_UINT32 and *ppchEnd is set to pchBuf.

        STD_BADPARAM : An improper value for nRadix was received.  The result
                       is set to zero, and *ppchEnd is set to pchBuf.

  @return
  The converted numeric result.

  @dependencies
  None.

*/
unsigned int std_scanul(const char * pchBuf, int nRadix, const char ** ppchEnd, int *pnError);

/**
  memscpy - Size bounded memory copy.

  Copies bytes from the source buffer to the destination buffer.

  This function ensures that there will not be a copy beyond
  the size of the destination buffer.

  The result of calling this on overlapping source and destination
  buffers is undefined.

  @param[out] dst       Destination buffer.
  @param[in]  dst_size  Size of the destination buffer in bytes.
  @param[in]  src       Source buffer.
  @param[in]  src_size  Number of bytes to copy from source buffer.

  @return
  The number of bytes copied to the destination buffer.  It is the
  caller's responsibility to check for trunction if it cares about it -
  truncation has occurred if the return value is less than src_size.

  @dependencies
  None.
*/

size_t memscpy(void *dst, size_t dst_size, const void *src, size_t src_size);

/**
  memsmove - Size bounded memory move.

  Moves bytes from the source buffer to the destination buffer.

  This function ensures that there will not be a copy beyond
  the size of the destination buffer.

  This function should be used in preference to memscpy() if there
  is the possiblity of source and destination buffers overlapping.
  The result of the operation is defined to be as if the copy were from
  the source to a temporary buffer that overlaps neither source nor
  destination, followed by a copy from that temporary buffer to the
  destination.

  @param[out] dst       Destination buffer.
  @param[in]  dst_size  Size of the destination buffer in bytes.
  @param[in]  src       Source buffer.
  @param[in]  src_size  Number of bytes to copy from source buffer.

  @return
  The number of bytes copied to the destination buffer.  It is the
  caller's responsibility to check for trunction if it cares about it -
  truncation has occurred if the return value is less than src_size.

  @dependencies
  None.
*/

size_t memsmove(void *dst, size_t dst_size, const void *src, size_t src_size);

/**
  secure_memset - Fill memory with a constant byte

  This function fills the first \c len bytes of the memory area pointed to by
  \c ptr with the constant byte \c value.

  @param[in]  ptr    Points to the memory area to be set.
  @param[in]  value  The value to be set.
  @param[in]  len    The number of bytes to be set.

  @return
  This function returns the pointer to the memory area ptr.

  @dependencies
  None.
*/

void* secure_memset(void* ptr, int value, size_t len);

/**
  timesafe_memcmp - Compare bytes in memory in constant time

  This function compares bytes in memory \c ptr1 against bytes in memory
  \c ptr2 in constant time which proportional to \c len.

  @param[in]  ptr1   Points to the first memory bytes to be compared.
  @param[in]  ptr2   Points to the second memory bytes to be compared.
  @param[in]  len    The number of bytes to be compared.

  @return
  This function returns 1 if the strings are different and
  0 if the strings are the same.

  @dependencies
  None.
*/

int timesafe_memcmp(const void* ptr1, const void* ptr2, size_t len);

/**
  timesafe_strncmp - Compare strings in constant time

  This function compares strings \c ptr1 and \c ptr2 in constant time
  which proportional to \c len.

  @param[in]  ptr1   Points to the first string to be compared.
  @param[in]  ptr2   Points to the second string to be compared.
  @param[in]  len    The number of bytes to be compared.

  @return
  This function returns 1 if the strings are different and
  0 if the strings are the same.

  @dependencies
  None.
*/

int timesafe_strncmp(const char* ptr1, const char* ptr2, size_t len);

/** @}  */ /* end_addtogroup dxy_stringl */

#ifdef __cplusplus
    } //extern "C"
} //namespace std
#endif //__cplusplus

//Explicit export of the libstd implemented functions
#ifdef __cplusplus
    using std::strlcat;
    using std::wcslcat;
    using std::wstrlcat;
    using std::strlcpy;
    using std::wcslcpy;
    using std::wstrlcpy;
    using std::strcasecmp;
    using std::strncasecmp;
    using std::wstrcmp;
    using std::wstrncmp;
    using std::wstrlen;
    using std::memscpy;
    using std::memsmove;
    using std::secure_memset;
    using std::timesafe_memcmp;
    using std::timesafe_strncmp;
#endif //__cplusplus

#endif /* _STRINGL_H_ */
