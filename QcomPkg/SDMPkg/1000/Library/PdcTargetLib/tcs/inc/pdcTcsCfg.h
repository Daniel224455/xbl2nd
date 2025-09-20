#ifndef _PDC_TCS_CFG_H_
#define _PDC_TCS_CFG_H_
/*============================================================================
  FILE:         pdcTcsCfg.h

  OVERVIEW:     This file provides the generaly types for the PDC TCS configuration

  DEPENDENCIES: None

  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.
=============================================================================
============================================================================*/
#include "comdef.h"

/*===========================================================================
 *                            TYPES AND DEFINITIONS
 *===========================================================================*/
/* Bit wise options for commands */
#define TCS_CFG_OPT_NONE          0x0000
#define TCS_CFG_OPT_CMD_WAIT      0x0001  /* Wait this and all previous command to finish */
#define TCS_CFG_OPT_CMD_RESP_REQ  0x0002  /* Command response is required */
#define TCS_CFG_OPT_READ          0x0004  /* Default is write if not specified */
#define TCS_CFG_OPT_NO_ENABLE     0x0008  /* Default is command is enabled */
#define TCS_CFG_OPT_NOT_USED      0x0010  /* RESOURCE IS NOT USED IN THIS TCS */

/* Resource data */
typedef struct pdc_tcs_resource_s
{
  char    *name; /* Resource string name */
  uint32  base_addr;
}pdc_tcs_resource;

/* Command data */
typedef struct pdc_tcs_resource_data_s
{
  uint32    res_val;      /* Resource data for each TCS */
  uint32    options;      /* Control flags (see above) */
  uint32    addr_offset;  /* Aditional required offset from base address */
}pdc_tcs_resource_data;

/* Type for the TCS command/data pair */
typedef struct pdc_tcs_config_s
{
  union
  {
    uint8             index;        /* Index reference for resource list (used for default init) */
    pdc_tcs_resource  *resourceP;   /* Resource config pointer
                                       Populated as part of PDC init and should used for any client query */
  }cmd;
  pdc_tcs_resource_data   data;     /* Data for the resource command */
}pdc_tcs_config;

#endif /* _PDC_TCS_CFG_H_ */

