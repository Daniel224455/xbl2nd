#ifndef __GPITGTCFGDATA_H__
#define __GPITGTCFGDATA_H__

/**
 * @file gpitgtcfgdata.h     
 * This file contains configuration data for the GPI driver for the Napali SDM845.
 *
 */
/*
===============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------
01/24/18   ah      File modified for Hana.

===============================================================================
                   Copyright (c) 2017-2018 QUALCOMM Technologies Incorporated.
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/

#include "comdef.h"

#define NUM_GPII_PER_QUP           13

/** @brief GPI Configuration
    Provides default GPI configuration.
 */
typedef struct _tgt_gpi_config_type
{
  uint8   active;
  uint8   type;
  uint64  gsi_pa;
  uint32  irq;         /* Muxed IRQ */
  uint32  gpii_irq[NUM_GPII_PER_QUP];
}tgt_gpi_config_type;

#endif  /* __GPITGTCFGDATA_H__ */

