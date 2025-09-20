/*
===========================================================================

FILE:         gpi_cfg.c

DESCRIPTION:  This file implements the GPI Config Data.

===========================================================================

                             Edit History


when       who     what, where, why
--------   ---     -------------------------------------------------------- 
02/22/17   ah      Modified GSI base definitions.
10/13/16   ah      Initial revision for Napali SSC.

===========================================================================
             Copyright (c) 2018 QUALCOMM Technologies, Incorporated.
                    All Rights Reserved
                   QUALCOMM Proprietary
===========================================================================
*/

#include "gpitgtcfgdata.h"
#include "msmhwiobase.h"

#define GSI_0_BASE       (QUPV3_WEST_QUPV3_ID_1_BASE + 0x00004000)
#define GSI_1_BASE       (QUPV3_EAST1_QUPV3_ID_0_BASE + 0x00004000)
#define GSI_2_BASE       (QUPV3_EAST_QUPV3_ID_0_BASE + 0x00004000)
#define GSI_SSC_BASE     (SSC_BASE + 0x00604000)

const tgt_gpi_config_type  tgt_gpi_config[] =
{
   { TRUE, 0, GSI_0_BASE,   0, { 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288 } },  /* QUP_0 */
   { TRUE, 1, GSI_1_BASE,   0, { 311, 312, 313, 314, 315, 316, 325, 326, 327, 328, 329, 330, 331 } },  /* QUP_1 */
   { TRUE, 2, GSI_2_BASE,   0, { 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632 } },  /* QUP_1 */
   { TRUE, 3, GSI_SSC_BASE, 0, { 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 459, 460, 461 } }   /* QUP_SSC */
};

