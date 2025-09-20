#ifndef _TCS_RESOURCE_H_
#define _TCS_RESOURCE_H_
/*============================================================================
  FILE:         tcs_resource.h

  OVERVIEW:     This file provides the types for the PDC TCS driver

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

/*===========================================================================
 *                            TYPES AND DEFINITIONS
 *===========================================================================*/
/* Number of commands per TCS; assumed to be the same for all */
#define NUM_COMMANDS_PER_TCS  4

/* Enum for the intended usage of each TCS */
typedef enum
{
  TCS_NUM_SLEEP0        = 0,            /* TCS 0 - Sleep: CX retention */
  TCS_NUM_SLEEP1,                       /* TCS 1 - Sleep: CX off */
  TCS_NUM_TOTAL_SLEEPS,                 /* Number of sleep TCS's */

  TCS_NUM_WAKE0 = TCS_NUM_TOTAL_SLEEPS, /* TCS 2 - Wake:  resources to MOL */

  TCS_NUM_TOTAL                         /* Total TCS's: */
}tcs_usage;

/* General purpose index values which match the order in g_pdcResourceList */
typedef enum
{
  RES_IDX_CX = 0,
  RES_IDX_MX,
  RES_IDX_XO,
  RES_IDX_SOC,                          /* vrm.soc */

  TCS_TOTAL_RESOURCE_NUM
}resource_index;

#endif /* _TCS_RESOURCE_H_ */

