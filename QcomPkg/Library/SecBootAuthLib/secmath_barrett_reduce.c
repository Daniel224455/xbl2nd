/*===========================================================================

                         S E C U R I T Y    S E R V I C E S

                S E C M A T H   R S A   I M P L E M E N T A T I O N
                              
FILE:  secmath_barrett_reduce.c

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

#include <string.h> //for memcpy()
#include "secmath_montmul.h"
#include "secmath_montmul_utils.h"

//- The sole purpose of functions in this file is to get into the Montgomery domain
//- and the only exported function is mult_R_mod_m(), which multiplies the input to
//- to R=r^N modulo m, where r=2^32.

//-division functions ------------------------------
//this function is NOT general. It works for special cases.
//divide 3 words by 2 words. Remainder is stored in a.
static int div32(uint32 *quotient, uint32 *a, const uint32 *m)
{
#define a0   a[0]
#define a1   a[1]
#define a2   a[2]
#define m0   m[0]
#define m1   m[1]
#define r0 a0
#define r1 a1
#define r2 a2
    //uint32 r2, r1, r0;
   uint32 q;
   int64  cc;
   uint64 p;
     
   if(   !( a2<m1 || (a2==m1&&a1<m0))   ) //(a2, a1) <(m1, m0), otherwise q wont fit in one word
   {
      //set_error(E_DIVIDEND_TOO_LARGE);
      return -1;
   }

   if(1+m1==0) //m1==0xFFFFFFFF 
      q = a2; //compute q quickly
   else if(m1>0)
   {
      if(a2==m1)
         q = ~0;
      else
         q = to64(a2,a1)/(m1);  //get the approximate quotient. NOTE: it doesnt work very well, when a2==m1, since q becomes too small
   }
   else //m1==0, considering (a2, a1) <(m1, m0), this will be a two digit by one digit division
   {
      if(m0==0) //check divide by zero here chk. 
         return -1; //if this happens, it means m0=m1=0. This will not happen in RSA. But I check to remove any chance of division by zero exception
      q = to64(a1,a0)/m0;  
   }
   
   //perform A-qM, remainder is in (r2, r1, r0)
   p = (uint64)q * m0;
   cc = a0;
   cc -= LO(p);  //make sure subtractions are done in 64 bit integer not 32 bits, otherwise there will be overflows 
   r0 = LO(cc);
   cc >>= 32;
    cc -= p>>32;

   p = (uint64)q * m1;
   cc += a1;
   cc -= LO(p);
   r1 = LO(cc);
   cc >>= 32;
    cc -= p>>32;

   cc += a2;
    r2 = LO(cc);

   //check the remainder and correct the quotient
   if((int32)r2>=0)
   //subtract more m's if necessary
     while(r2!=0 || r1>m1 || (r1==m1&&r0>m0) )
     {
      cc = (uint64)r0 - m0; //do the 64 bit addition trick, instead of adding 32 bits and handeling carry. 
      r0 = LO(cc);
      cc >>=32;

      cc += (uint64)r1 - m1;
      r1 = LO(cc);
      cc >>=32;

      cc += r2;
      r2 = LO(cc);
      q++;
     }
   else //remainder is negative, r2<0, add m
     while(r2!=0) //when r2==0, the remainder is not negative anymore and is smaller than m
     {
      cc = (uint64)r0 + m0; 
      r0 = LO(cc);
      cc >>=32;

      cc += (uint64)r1 + m1;
      r1 = LO(cc);
      cc >>=32;

      cc += r2;
      r2 = LO(cc);
      q--;
     }
     *quotient = q;
     return 0;

#undef a0
#undef a1
#undef a2
#undef m0  
#undef m1
#undef r0
#undef r1
#undef r2
}
  
//divide 4 words by two words, return two words
static int div42(uint32 *q, uint32 *a, const uint32 *m)
{
   int rc;

   rc  = div32(q  , a+1, m);  
   rc |= div32(q+1, a  , m);  

   return rc;
}
//--------------------------------------------------
//NOTE; a MUST be smaller than m. The msb of m or the bit before that MUST be one.
//otherwise, the quotient will overflow
//in most cases, q does not need correction even for SHIFT=7, works fine for SHIFT=0
//SHIFT deermines the minimum m supported. Decreasing shift, decreases the precision of the computed quotient q
//SHIFT 30 means one leadng zero bit is allowd for the modulus m.
#define SHIFT (30)
#define MINIMUM_M (((uint32)1) <<SHIFT) /* MIN_M = 1/2 r */
static int reciprocal_2(uint32 l_m[2], const uint32 *m)
{
   uint32 R[4];
   int rc;

   if(m[1] < MINIMUM_M) //if(m==0) return -1; be careful about division by 0. It is covered by m<MINIMUM_M
   {
      DEBUG_PRINT("\nFATAL: modulus too small, the most significamt bit of m must be one m=%08x", m[1]);
      rc= -1;
   }else if(m[1] == MINIMUM_M && m[0] ==0){
      l_m[0] = l_m[1] = ~0;
       rc = 0;
   }else{
       R[3] = MINIMUM_M; R[2]=R[1]=R[0]=0;
       rc=div42(l_m, R, m);
   }
   return rc;
}

#if 0
// old function. use reciprocal_2() instead. reciprocal_1() is used for test purpose.
// compute approximate reciprocal  r^2/2M[N-1] = 2^63/m[N-1]
static int reciprocal_1(uint64 *l_m, uint32 m)
{
    uint64 R=(((uint64)1) <<63);  // r = 2^32, R = r^2/2
   uint32 q0, q1, r0;

   // m must be larger than or equal to 2^31, or q0 will overflow. 
   // That is, The msb of m must be set, which is the case in RSA
   // If m=2^31, then q = 2^63 and the result will fit in 64 bits of output. 
   if(m < MINIMUM_M) //if(m==0) return -1; be careful about division by 0. It is covered by m<MIN_M
   {
      DEBUG_PRINT("\nFATAL: modulus too small, the most significamt bit of m is not set m=%08x", m);
      return -1;
   }
   
   
       //divide and conqure division
   q0 = (uint32)(R/m);
   r0 = (uint32)(R-(uint64)q0*m);
   q1 = ((uint64)r0<<32)/m;
   
   *l_m =  ((uint64)q0<<32)+q1;

/* one extra bit for l_m. in this case,the ream l_m has one hidden extra bit set to one as MSB and l_m is 65 bits.
    R = -1;
   q0 = (uint32)(R/m);
   r0 = (uint32)(R-(uint64)q0*m - ((uint64)m<<32)); //r0 must be a 32 bit number
   q1 = ((uint64)r0<<32)/m;
   
   t =  ((uint64)q0<<32)+q1;
    *l_m = t; */  //real 1/m = 1 || l_m , one concatinated with l_m
   return 0;
}
#endif //reciprocal_1()

// qi = floor[a/m]
// qi = HiWord(a * l_m) 
// compute approximate q
// critical: a MUST be smaller than m, otherwise the quotient q will be larger than one word
static uint32 get_qi(const uint32 *a, const uint32 *m, size_t N, uint64 l_m)
{
   uint64 acc, a0b1, a1b0, a1b1;
    uint32 q;

#define HIa    (a[N-1])
#define LOa    (a[N-2])
//#define LOa   0   
   
   //acc = (uint64)LO(l_m) * LOa; //this has no effect on the output
   //acc >>= 32;
    acc = 0;
   a0b1 = (uint64)HIa*LO(l_m);
   a1b0 = (uint64)LOa*HI(l_m);
   acc +=LO(a0b1);
   acc +=LO(a1b0);
   acc>>=32;

   a1b1=(uint64)HI(l_m) * HIa;
   acc +=HI(a0b1);
   acc +=HI(a1b0);
   acc +=LO(a1b1);
   acc >>=SHIFT;

    acc += (HI(a1b1)<<(32-SHIFT));

   if(HI(acc)>0) //make sure it doesn't overflow.  
      q = ~0;
   else
      q = LO(acc); 
   
   /* check if this is any faster chk bjn
   acc = 0;
    a0b1 = (uint64)HIa*LO(l_m);
   a1b0 = (uint64)LOa*HI(l_m);
    acc  = a0b1 + LO(a1b0); //cannot overflow 
   acc  >>= 32;

   a1b1=(uint64)HI(l_m) * HIa;
   acc += a1b1 + HI(a1b0) ; //cannot overflow
    q = acc>>SHIFT; */
     
   return q; 
}

#ifdef DEBUG_MONTMUL
int q_add_cntr=0;
int mult_base_mod_m_cntr=0;
#endif
//computes a*r mod m, where r=2^32 and a is a big integer of length N
//a and z can overlap. that is, mult and reduction can be done inplace
int mult_base_mod_m(uint32 *z, const uint32 *m, const uint32 *a, uint32 qi, size_t N)
{
   uint32 k;
   int64  cc; //carry is defined as signed integer to allow signed extension
   uint64 p;
   uint32 z0;
   int32  carry;

#ifdef DEBUG_MONTMUL 
    mult_base_mod_m_cntr++;


   //----------
   //ONLY FOR TEST PURPOSE: check if the function works fine if qi is not computed precisely.
   //using reciprocal_2(), qi is computed relatively precise. Adding 15 or subtracting 15 
   //is compensted using extra 16 calls to add() sub().
   //at some point we are going to prove that add(), sub() calls are not needed, but for the time
   //being we keep them here to be safe
    //qi+=15;
   //qi-=15;
   //---------
#endif

   //perform a*base - qi *m       
   //first iteration
      p = (uint64)qi*m[0];    

      cc = 0;
      cc -=  (uint32)p;
      z0 = (uint32) cc;
      
      cc >>= 32;
        cc -= p>>32;

   for (k=1; k<N; k++)
   {
       p = (uint64)qi*m[k]; //p is the product of two unsigned, therefore -p doesnot fit in 64 bit, thats why this loop is arranged this way
      
      cc += a[k-1];
        z[k-1] = z0;
      cc -= (uint32)p;   // subtract lsB of qi*m[k]
      z0 = (uint32) cc;  // save lsB of cc, which is the result
      
      cc >>= 32;           // find the carry
        cc -= p>>32;         // now subtract the msB of qi*m[k], for the next cycle
   }

   cc +=a[k-1];
    z[k-1] = z0;
    carry = (int32)cc;
   
   if(carry)
   {
        if(carry>0)
           for (k=0; k<16 && carry!=0 ; k++)// the result is positive and qi has been too small. 
                //carry -= subtract(z, z, m, N);    
              carry += sub(z, m, N);    
      else if(carry<0)
            for (k=0; k<16 && carry!=0 ; k++)
                carry += add(z, m, N); 
     
        #ifdef DEBUG_MONTMUL
        q_add_cntr+=k;
      //DEBUG_PRINT("\n qi is too %s %08x, adding m %i", (add_sub==add)?"big":"small",qi, k);
        #endif

       if(k>=16)
      {
         DEBUG_PRINT("\nFATAL: qi is too small or too large %08x", qi);
         return -1;
      }
   }   
    //there is no carry at this point, but still z can be larger than m.
   //we should make z<m to make it ready for the next iteration
   while( compare(z, m, N)>=0 )
   {
       //subtract(z, z, m, N); 
      sub(z, m, N);
        #ifdef DEBUG_MONTMUL
        q_add_cntr+=1;
        #endif
   }   

   return 0;
}

//R = base ^N and base=2^32  
//WARNING: i- a must be smaller than m, otherwise this functions returns wrong result
//         ii- msb of m must be one, if it is zero then the next bit must be one, or the function returns wrong result
// caller must make sure a<m, or this function wont work
int mult_R_mod_m(uint32 *z, const uint32 *a, const uint32 *m, const size_t N)
{
   size_t u;
   int rc;
   uint32 qi;
    uint64 l_m;
   uint32 l_m2[2];
   
#ifdef DEBUG_MONTMUL
   ASSERT(compare(a, m, N)<0 ); // make sure a is a smaller than m
   ASSERT(m[N-1] >= MINIMUM_M);
#endif

   memcpy(z, a, N*sizeof(z[0]));  //this memcpy may be avoided chk bjn

   //reciprocal() returns error if m<MINIMUM_M. We let the computation go on and return error at the end.
   //rc=reciprocal_1(&l_m, m[N-1]); //return error if m is too small. msb of m[N-1] must be 1
   rc=reciprocal_2(l_m2, m+N-1-1);
   l_m = to64(l_m2[0], l_m2[1]); //reciprocal_2 IS ENDIAN DEPENDENT chk bjn

   for(u=0; u<N; u++)
   {
      qi = get_qi(z, m, N, l_m); 
      rc |= mult_base_mod_m(z, m, z, qi, N); //if there is an error report it at the end.
   }
   return rc;
}

