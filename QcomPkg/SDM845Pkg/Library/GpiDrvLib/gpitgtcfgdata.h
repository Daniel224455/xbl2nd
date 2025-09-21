/**
 * @file gpitgtcfgdata.h     
 *
 * This file contains configuration data for the GPI driver for SDM845.
 *
 * Copyright (c) 2016, Qualcomm Technologies Inc. All rights reserved.
 */

/*==============================================================================
                             EDIT HISTORY

when       who     what, where, why
--------   ---     ------------------------------------------------------------
03/22/16   AH      Created
==============================================================================*/

#ifndef __GPITGTCFGDATA_H__
#define __GPITGTCFGDATA_H__

#include "gsihwio.h"

/** Configs supported for
 *  SSC
 *  GSI North
 *  GSI South
 */

#define MAX_NUM_QUP    3

/** @name QUP Instance */
typedef enum
{
   QUPv3_SOUTH = 0,
   QUPv3_NORTH = 1,
   QUPv3_SSC   = 2
}QUP_TYPE;

/** @brief GPI Configuration
    Provides default GPI configuration.
 */
typedef struct _tgt_gpi_config_type
{
  uint8      active;
  uint8      type;
  uint32     gsi_pa;
  uint32     irq;         /* Muxed IRQ */
}tgt_gpi_config_type;

/** @brief GPII Configuration
    Provides default GPII configuration.
 */
typedef struct _tgt_gpii_config_type
{
  QUP_TYPE  qup_type;     /* GSI HW instance this GPII is part of   */
  uint8     gpii_id;      /* GPI Interface ID                       */
  uint8     protocol;     /* Protocol running on this GPII          */
  uint8     options;      /* Various GPII options in bitmask format */
  uint8     ee;           /* Local execution environment index      */
  uint8     se;           /* Serial Engine index                    */
  uint32    irq;          /* IRQ for this ee                        */
}tgt_gpii_config_type;

#endif  /* __GPITGTCFGDATA_H__ */
