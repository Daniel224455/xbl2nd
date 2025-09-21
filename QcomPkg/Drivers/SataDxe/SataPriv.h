/** @file SataPriv.h
   
  XBL SATA driver header file
  
  Copyright (c) 2014,2018 Qualcomm Technologies, Inc. All Rights Reserved. 
**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   -----   ----------------------------------------------------------- 
2014/11/17   rm      Initial Version

=============================================================================*/

#ifndef _SATA_PRIV_H_
#define _SATA_PRIV_H_

#include "HALsata.h"
#include <Library/TimerLib.h>

#define udelay(us) MicroSecondDelay(us)
#define mdelay(ms) MicroSecondDelay(1000*ms)

/* Sata PHY c_ready status poll time(ms) */
#define SATA_PHY_POLL_MAX   5000

#endif /* _SATA_PRIV_H_ */