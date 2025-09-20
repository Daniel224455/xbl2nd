/*===========================================================================

                         S E C U R I T Y    S E R V I C E S

                S E C M A T H   R S A   I M P L E M E N T A T I O N
                              
FILE:  secmath_monmul_utils.c

DESCRIPTION:

DEPENDENCIES/ASSUMPTIONS

EXTERNALIZED FUNCTIONS             

    Copyright © 2011-2013 Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*=========================================================================

                            EDIT HISTORY FOR FILE
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who            what, where, why
--------   ----           ---------------------------------------------------
05/11/11   vg             Initial Revision

===========================================================================*/
#include "SecMath.h"
#include "secmath_montmul_utils.h"
#include <limits.h>

int compare(const uint32 *a, const uint32 *b, size_t N)
{
   while (N--)
   {
           if (a[N] < b[N]) return -1;
      else if (a[N] > b[N]) return 1;
   }
   return 0;
}

//this doesnot work with negative dx. that is, it cannot decrement
int increment(uint32 *a, size_t N, uint32 dx)
{
   size_t i;
   uint32 t;

   t = a[0];
   a[0] += dx;
   if (a[0] >= t)
      return 0;
   for (i=1; i<N; i++)
      if (++a[i])
         return 0;
   return 1;
}

int decrement(uint32 *a, size_t N, uint32 dx)
{
   size_t i;
   uint32 t;

   //ASSERT(N); chk bjn
   
   t = a[0];
   a[0] -= dx;
   if (a[0] <= t)
      return 0;
   for (i=1; i<N; i++)
      if (a[i]--)
         return 0;
   return 1;
}

void negate(uint32 *pd, int n)
{
  uint32 *p = pd;
  
  while (--n >= 0)
  {
    p[n] = ~p[n];
  }
  increment(pd, n, 1);
}

int32 sub(uint32 *a, const uint32 *m, const size_t N)
{
   int64 cc;
   size_t k;

   cc = 0;
    for (k=0; k<N; k++)
    {
       cc += a[k];
      cc -= m[k]; 
       a[k] = (uint32)cc;
       cc  >>= 32;
   }
    return (int32) cc;
}

int32 add(uint32 *a, const uint32 *m, const size_t N)
{
   uint64 cc;
   size_t k;

   cc = 0;
    for (k=0; k<N; k++)
    {
       cc += a[k];
      cc += m[k]; 
       a[k] = (uint32)cc;
       cc  >>= 32;
   }
    return (int32) cc;
}

int32 addition(uint32 *z, const uint32 *a, const uint32 *b, size_t N)
{
   uint64 cc;
   size_t k;

   cc = 0;
    for (k=0; k<N; k++)
    {
       cc += a[k];
      cc += b[k]; 
       z[k] = (uint32)cc;
       cc  >>= 32;
   }
    return (int32) cc;
}

//------------------------------------------------------------------------
/* Returns the number of leading zero bits in a */
/* for ARM there is a machine instruction that does this. */
static
int find_MSB32(uint32 a)
{
   int n = 0;
#define MSB_STEP(k) if (((0 - (0x80000000U >> (k-1))) & a) == 0) {a <<= k;  n += k;}

   MSB_STEP(16);
   MSB_STEP(8);
   MSB_STEP(4);
   MSB_STEP(2);
   MSB_STEP(1);
   return (n);
}

/* returns the bit position (0 is LSB) of the MSB in a bignum, v.
 * Returns -1 if the bignum is zero, -2 on error
 */
int find_bn_MSB1(const uint32 *v, size_t N)
{
   if (N >= INT_MAX / 32) return (-2);
   while(N != 0 && v[N-1] == 0) { /* >=, but N is unsigned */
      --N;
   }
   if (N == 0) return (-1);
   return (32 * N - 1 - find_MSB32(v[N-1]));
}

//x must be odd
uint32 inverse32(uint32 x)
{
    uint32 a = 1;
    uint32 b = x;
    uint32 shval = 2;
    int k;

    x <<= 1;

    for (k = 1; k < 32; ++k) {
       if (b & shval) {
     a += shval;
     b += x; // knock out bit k in b  
       }
       x <<= 1;
       shval <<= 1;
    }
       
    return (a);
}

static void right_shift32(uint32 *a, size_t N, uint32 shift)
{
   uint32 i;
   uint32 hi_mask = 0xffffffff;

   if (shift > 32)
      return;

   hi_mask >>= 32 - (shift % 32);

   for (i = 0; i < N; ++i)
   {
      /* Right shift each BLONG by one shift */
      if (shift == 32)
        a[i] = 0;
      else
        a[i] >>= shift % 32;

      if (i < N - 1)
      {
        a[i] = a[i] | ((a[i+1] & hi_mask)
               << (32 - (shift % 32)));
      }
   }
}

void right_shift(uint32 *a, size_t N, uint32 shift)
{
   if (a == NULL)
      return;

   if (shift == 0)
      return;

   if (shift > N * BN_BITS_PER_WORD)
      shift = N * BN_BITS_PER_WORD;

   while (shift > 0)
   {
      if (shift <= 32)
      {
         right_shift32(a, N, shift);
         shift = 0;
      }
      else
      {
         right_shift32(a, N, 32);
         shift -= 32;
      }
   }
}
