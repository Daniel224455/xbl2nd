
/*===========================================================================

                         S E C U R I T Y    S E R V I C E S

                S E C M A T H   R S A   I M P L E M E N T A T I O N

                              A P I    M O D U L E
FILE:  secmath_mod_exp.c

DESCRIPTION:

DEPENDENCIES/ASSUMPTIONS

EXTERNALIZED FUNCTIONS
             secmath_BIGINT_modexp()

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

#include <string.h>
#include "SecMath.h"
#include "secmath_montmul.h"
#include "secmath_montmul_utils.h"
#include "secmath_mod_exp.h"

static int need_base_to_3(const uint32 *exponent, uint32 expN)
{
   uint32 i;
   
   for (i = 0; i < expN; ++i) 
   {
      if (exponent[i] & (exponent[i] << 1) & 0xaaaaaaaa) 
      {
         return (1);
      }
   }
   return (0);
}


/* mod_exp does modular exponentiation.  base, exponent, modulus, and
 * result are all big nums with the bytes in native order within words
 * and the words in little-endian order.  N is the number of 32-bit
 * words in base, modulus and result. expN is the number of 32-bit
 * words in the exponent.  work is array with pointers to 4 N-word
 * bignums used for temporary storage of values.  The result and the
 * values in work must not alias any other arguments.
 *
 * base^0 is defined to be 1.
 * 
 * Return value: Returns 0 on success.  Returns -1 on any detected
 * error.  
 */

int mod_exp(uint32 *__restrict result, const uint32 *base, 
            const uint32 *exponent, const uint32 *modulus,
            const size_t N, const size_t expN, uint32 *__restrict work[4])

{
   uint32 *basepwrs[4];
   int bit_pair_idx;
   int bit_pair;
   uint32 *current_val;
   int squares_needed;
   uint32 m0_inv;
   int rv = 0;

   DEBUG_PRINT("\n*** Running in debug mode");
   /* result will temporarily hold montgomery-encoded base
      work 0 : mpy data 0
      work 1 : mpy data 1
      work 2 : montgomery encoded base^2 
      work 3 : montgomery encoded base^3 (lazy)
   */
   
   /*
   Checks to performs before doing the modular exponentiation
   1. Modulus not even
   2. N must be larger than certain value
   3. maximum one Leading bits of modulus is zero
   4. mult_R_mod_m() wont work if base >= modulus 
   */

   if(
       (N< MINIMUM_N) || 
       (compare(base, modulus, N) >= 0) ||
       ((modulus[N-1]&0xC0000000)==0) /* from {00, 01, 10, 11}, 00 is not acceptable */
     ) 
      return(-1);

   bit_pair_idx = find_bn_MSB1(exponent, expN);
   if (bit_pair_idx == -2) return (-1);  /* expN crazy big */
   if (bit_pair_idx == -1) {             /* exponent is zero */
      if (N == 0) return (-1);  /* can't set result to 1 */
      memset(result, 0, sizeof(result[0])*N);
      result[0] = 1;  /* define x^0 to be 1 for all x */
      return (0);
   }
   // at this point bit_pair_idx is ready position of MSB
   if (bit_pair_idx == 0) {  // exponent is 1
      memcpy(result, base, sizeof(result[0])*N);
      return (0);
   }
   /* adjust bit_pair_idx to point to LSB of bit pair */
   bit_pair_idx &= ~1;  /* now it is even */
   
   basepwrs[0] = 0;
   basepwrs[1] = result;
   basepwrs[2] = work[2];
   basepwrs[3] = 0;  // will be changed later if needed.
   
   // The casts are here to quiet warnings on some compilers
   m0_inv = (uint32)(-(int32)inverse32(modulus[0]));
   // Verify that we really computed negative inverse.  (Will fail if modulus is even.)
   if (m0_inv * modulus[0] != (uint32)(-1)) return (-1);

   /* fill in the needed values in basepwrs */
   /* compute base, Montgomery encoded */
   rv = mult_R_mod_m(basepwrs[1], base, modulus, N);

   /* base^2 */
   rv |= montmul_ref1(basepwrs[2], basepwrs[1], basepwrs[1], modulus, N, m0_inv);

   /* base^3 if needed */
   if (need_base_to_3(exponent, expN)) {
      basepwrs[3] = work[3];
      rv |= montmul_ref1(basepwrs[3], basepwrs[2], basepwrs[1], 
         modulus, N, m0_inv);
   }      
   
   /* compute the initial bit pair */
   bit_pair = (exponent[bit_pair_idx/32] >> (bit_pair_idx & 31)) & 0x3;

   /* set up for main loop */
   if (bit_pair != 1) {
      // obvious case 
      current_val = basepwrs[bit_pair];
      squares_needed = 2;
   } else {
      // optimization:  current_val is the base.  Normally we would 
      // square twice, but we already have base^2, so use it and save 
      // a square operation.
      current_val = basepwrs[2];
      squares_needed = 1;
   }
   bit_pair_idx -= 2;

   while (bit_pair_idx >= 0) {
      bit_pair = (exponent[bit_pair_idx/32] >> (bit_pair_idx & 31)) & 0x3;

      // Squares
      while (squares_needed > 0) {
    uint32 *dest = current_val == work[0] ? work[1] : work[0];
    rv |= montmul_ref1(dest, current_val, current_val, 
           modulus, N, m0_inv);
    current_val = dest;
    --squares_needed;
      }

      // Now the multiply
      if (bit_pair != 0) {
    uint32 *dest = current_val == work[0] ? work[1] : work[0];
    rv |= montmul_ref1(dest, basepwrs[bit_pair], current_val, 
           modulus, N, m0_inv);
    current_val = dest;
      }
      bit_pair_idx -= 2;
      squares_needed = 2;
   }

   // get back out of Montgomery 
   rv |= montmul_one_ref(result, current_val, modulus, N, m0_inv);

   /* Zero result on error as weak protection for callers who do not
      check the return code. (There is no security guarantee on when
      happens in error case.) */
   if (rv) {
      memset(result, 0, 4 * N);
   }

// Need for FIPS 140 compliance
#ifdef   ZEROIZE_SENSITIVE_DATA
   {
      int i;
      
      // zeroize work buffers
      for (i = 0; i < 4; ++i) {
        if (work[i])
       memset(work[i], 0, 4 * N);
    }     
   }

#endif

   return(rv);
}


/* computes r = b**e mod m.  
   Preconditions: b < m; The most significant word of m must be at least 0x40000000;
   m must be odd; N >= 2.
*/
SECMATH_ERRNO_ET secmath_BIGINT_modexp(BigInt * r, BigInt * b, BigInt * e, BigInt * m)
{
  uint32 temp[4][BLONGS_PER_KEY];
  uint32 *work[4];
  int i;  

  work[0]= temp[0];
  work[1]= temp[1];
  work[2]= temp[2];
  work[3]= temp[3];

  /* Make sure that the unused high words of b are all zero.  This is necessary 
     because mod_exp does not get the length of b; it just assumes it is the
     same as the length of m. */
  for (i = b->n; i < m->n; ++i) {
     b->a[i] = 0;
  }
  r->n = m->n;  // make r have the same length as the modulus.

  // All the preconditions are checked in mod_exp, and it will fail if any
  // are violated.

  if(mod_exp(r->a, b->a, e->a, m->a , m->n,e->n, work) != 0)
  {
    return E_SECMATH_FAILURE;
  }

  return E_SECMATH_SUCCESS;
}

