#ifndef _MD_OEM_PUBLIC_KEY_H
#define _MD_OEM_PUBLIC_KEY_H

/*===========================================================================

                                OEM public key for minidump encryption
                                

GENERAL DESCRIPTION
  This header file contains OEM public key for minidump encryption

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None
  
Copyright 2017 by Qualcomm Technologies, Inc.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/boot.xf/2.2/QcomPkg/SDM845Pkg/Include/xbl_minidump_oem_public_key.h#1 $
  $DateTime: 2018/02/07 02:47:14 $ 
  $Author: pwbldsvc $

when       who          what, where, why
--------   --------     ----------------------------------------------------------
26/04/17   bd           Initial version
============================================================================*/

/*===========================================================================*/


typedef struct
{
  uint32      bitLength;   
  char  *n;          
  const char  *p;
  const char  *q;
  char  *e;
  const char  *d;
}raw_key_t;


raw_key_t xbl_minidump_oem_public_key = {
    2048,
    /* n */
    "",        

    /* p */
    "",

    /* q */
    "",

    /* e */
    "",
    
    /* d */
    "",
};

#endif  /* _MD_OEM_PUBLIC_KEY_H */