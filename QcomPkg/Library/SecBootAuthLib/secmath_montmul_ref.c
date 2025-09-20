/*===========================================================================

                         S E C U R I T Y    S E R V I C E S

                S E C M A T H   R S A   I M P L E M E N T A T I O N
                              
FILE:  secmath_monmul_ref.c

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
01/11/13   ejt            Klocwork fixes
05/11/11   vg             Initial Revision

===========================================================================*/

// $armcc -O3 -OTime montmul_ref.c
//make sure you purge unused functions, using proper compilre flag
//check the assembly output. The inner loop should be 18 instruction, having 3 loads and one store. Use $armcc -S -I -O3 -OTime montmul_ref.c


#include <string.h>
#include "secmath_montmul_utils.h"
#include "secmath_montmul.h"

#ifdef USE_MONTMUL_REF

/* Montgomery mutliplication z = a * b * R^-1 mod m, where R = r^N and r = 2^32 */
/* a and b can overlap.  z will be overwritten                                  */
/* m0_1 = - m[0]^(-1) mod r                                                     */
/* This function implements Algorithm 14.36, Handbook of Applied Cryptography   */
/* this code meant to be simple, have small data and code foot print and yet be */
/* efficient for RSA signature veriffication, in particular exp=3               */


int montmul_ref1(uint32 * __restrict z, const uint32 *a, const uint32 *b, const uint32 *m, const size_t N, uint32 m0_1)
{
  size_t  k;
  uint32 cz; /* carry bit for z */
  int rc;

#ifdef USE_ARMASM
  uint32 *zp;
  uint32 *ap, *aend;
#else
  size_t i;
#endif
  
  //if(N<MINIMUM_N) return -1; /*The minimum of N is MINIMUM_N, and MUST be checked by the caller function*/
  memset(z, 0, N * sizeof(z[0]));

  cz = 0;
  
#ifdef USE_ARMASM
  //zend = z+N;
  ap = (uint32 *)a;
  aend = ap+N;

  for ( ;ap<aend; ap++)
  {
    register uint32 ai, qi;
    uint32 aib_l, aib_h, qim_h;

    ai = *ap;
    zp = z;
    __asm
    {
       LDR      aib_l, [zp, #0]
       MOV      aib_h, 0
       UMAAL aib_l, aib_h, ai, b[0] //aib = (uint64_t)ai * b[0] + z[0];
       MUL     qi, aib_l, m0_1          //qi = LO(aib) * m0_1; // Z + ai B + qi M == 0 mod 2^32
       MOV      qim_h, 0
       UMAAL aib_l, qim_h, qi, m[0]
       //qim = (uint64_t)qi * m[0] + LO(aib); // LO(qim) is zero at this point 
    }
    //Z = Z + ai*B + qi*M
    for (zp++, k=1; k< N; zp++,k++) 
    __asm
    {
      LDR      aib_l, [zp, #0]
        UMAAL  aib_l, aib_h, ai, b[k]
        UMAAL  aib_l, qim_h, qi, m[k]
      STR      aib_l, [zp, #-4] /*aib_l is actually qim_l here*/

    } 
    
    __asm
    {
          ADDS  aib_l, aib_h, qim_h
        MOV    aib_h,#0
        ADC    aib_h, aib_h,aib_h
        ADDS   aib_l, aib_l, cz
        ADC    cz   , aib_h, 0
        STR    aib_l, [zp, #-4]
           /*
          qim = (uint64_t)HI(aib)+ HI(qim) + cz;
          z[k-1] = LO(qim);
          cz = HI(qim); */
     }
  }
#else
  for (i=0; i<N; i++)
  {
    register uint32 ai, qi;
    uint64 aib, qim; /* 64-bit accumulators */

    ai = a[i];
    aib = (uint64)ai * b[0] + z[0];
    
    qi = LO(aib) * m0_1; // Z + ai B + qi M == 0 mod 2^32
    qim = (uint64)qi * m[0] + LO(aib); // LO(qim) is zero at this point 
    //Z = Z + ai*B + qi*M
    for (k=1; k< N; k++)
    {
      aib = (uint64)ai * b[k] + z[k]    + HI(aib);
      qim = (uint64)qi * m[k] + HI(qim) + LO(aib); 
      
      z[k-1] = LO(qim); 
    }
    
    qim = (uint64)HI(aib)+ HI(qim) + cz;
    z[k-1] = LO(qim);
    cz = HI(qim); 
  }
#endif

  if(cz) //if carry subtract m  
  {  
     int32 cc; 
     cc=sub(z, m, N);
     rc = (cc+cz == 0)?0:-1;
     if(rc==-1)
     {
        DEBUG_PRINT("\nFATAL: montmul_ref1");
     }
  }else 
     rc=0;

  return rc;
}

#endif //USE_MONTMUL_REF

//---------------------------------------------------------------------------------------------
// Montgomery multiply a by one, return result in z. montmul_ref() may be used as well
// but for exp=3 signature verification, this will be much faster 
// This function is used to get out of the Montgomery domain.
int montmul_one_ref(uint32 * __restrict z, const uint32 *a, const uint32 *m, size_t N, uint32 m0_1)
{
  size_t i, k;
  int rc;

  //if(N<MINIMUM_N) return -1; /*The minimum of N is MINIMUM_N, and MUST be checked by the caller function*/ 
  memcpy(z, a, sizeof(z[0])*N);

  for (i=0; i<N; i++)
  {
    register uint32 qi;
   uint64 qim; /* 64-bit accumulator */

    qi = z[0] * m0_1; // Z + qi M == 0 mod 2^32
    qim = (uint64)qi * m[0] + z[0]; // LO(qim) is zero at this point 
    for (k=1; k< N; k++)
    {
      qim = (uint64)qi * m[k]  + z[k] + HI(qim);
      z[k-1] = LO(qim); 
    }
    z[k-1] = HI(qim);
  }
 
  rc = 0;

  //this check is not needed. 
  //This function is Montgomery multiplication by one. It performs r^N *Y = Z +QM, where r=2^32, N=number of words in a big integer, M is the modulus and Q = \sum_{i=0}^{N-1} q_i r^i  and Y is the output. The maximum value for the Q is r^N-1, because the maximum value for the q is r-1. So, Y <= (Z+(r^N-1)M)/r^N = (Z-M)/r^N +M. Since at the entry to the function Z<M, therefore the output Y<M and no subtract is needed.
  if(compare(z, m, N)>=0) //make sure the result is less than m, since this is the last stage in RSA exponentiation.
      rc = sub(z, m, N); 

  if(rc!=0)
  {
      DEBUG_PRINT("\nFATAL: montmul_one_ref");
  }
  
  return rc;
}
