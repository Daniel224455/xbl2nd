#ifndef __GPIODEVCFG_H__
#define __GPIODEVCFG_H__
/*
============================================================================

                 G P I O   D E V I C E   C O N F I G
            F O R   B O O T   L O A D E R   S E T T I N G S

FILE:         GPIODevCfg.h

DESCRIPTION:  
              This file contains definitions used by the boot loader
              TLMM device config settings.

============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ---------------------------------------------------------
03/14/18   dcf     Created the initial revision.
 
============================================================================
             Copyright (c) 2018 QUALCOMM Technologies, Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
============================================================================
*/

/*==========================================================================

                     INCLUDE FILES FOR THIS MODULE

==========================================================================*/


/*==========================================================================

                     DEFINITIONS FOR THIS MODULE

==========================================================================*/


#define PIN_INPUT               0x0001
#define PIN_OUTPUT              0x0002
#define PIN_PULL_DOWN           0x0004
#define PIN_PULL_UP             0x0008
#define PIN_NO_PULL             0x0010
#define PIN_KEEPER              0x0020
#define PIN_OUT_LOW             0x0040
#define PIN_OUT_HIGH            0x0080
#define PIN_PRG_YES             0x0100
#define PIN_PRG_NO              0x0000
               
#endif /* __GPIODEVCFG_H__ */

