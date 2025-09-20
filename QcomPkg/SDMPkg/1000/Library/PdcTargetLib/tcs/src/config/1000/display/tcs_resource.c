/*============================================================================
  FILE:         tcs_resource.c

  OVERVIEW:     This file implements the target specific TCS resources

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
#include "pdcTcsCfg.h"
#include "tcs_resource.h"

/*===========================================================================
 *                            TYPES AND DEFINITIONS
 *===========================================================================*/
#define TCS_RESOURCE_CX   "cx.lvl"
#define TCS_RESOURCE_MX   "mx.lvl"
#define TCS_RESOURCE_XO   "xo.lvl"
#define TCS_RESOURCE_SOC  "vrm.soc"

/* Resource levels hardcoded until such time they are available at runtime */
#define RES_CX_OFF  0
#define RES_CX_RET  1
#define RES_CX_MOL  2 //SVS3 (min_svs)

#define RES_MX_RET  1
#define RES_MX_MOL  4 //NOM

#define RES_XO_OFF  0
#define RES_XO_MOL  3 //ON

#define VRM_SOC_OFF 0
#define VRM_SOC_ON  1

/*===========================================================================
 *                            INTERNAL VARIABLES
 *===========================================================================*/
/* Resources used in this target */
pdc_tcs_resource g_pdcResourceList[TCS_TOTAL_RESOURCE_NUM] =
{
  {TCS_RESOURCE_CX},
  {TCS_RESOURCE_MX},
  {TCS_RESOURCE_XO},
  {TCS_RESOURCE_SOC, 0x4}    /* Enable vote is at offset of 4 */
};

/* TCS configuraiton */
pdc_tcs_config g_pdcTCSConfig[TCS_NUM_TOTAL][NUM_COMMANDS_PER_TCS] =
{
  /* Sleep 1 TCS (TCS 0) */
  {
    /* Res Index      Resource data     Options                     Addr offset */
    {{RES_IDX_XO},      {RES_XO_OFF,      TCS_CFG_OPT_NONE,           0}},   /* Cmd 0 */
    {{RES_IDX_CX},      {RES_CX_RET,      TCS_CFG_OPT_NONE,           0}},   /* Cmd 1 */
    {{RES_IDX_MX},      {RES_MX_RET,      TCS_CFG_OPT_NONE,           0}},   /* Cmd 2 */
    {{RES_IDX_SOC},     {VRM_SOC_OFF,     TCS_CFG_OPT_NONE,           0}},   /* Cmd 3 */
  },

  /* Sleep 2 TCS (TCS 1) */
  {
    /* Res Index      Resource data     Options                     Addr offset */
    {{RES_IDX_XO},      {RES_XO_OFF,      TCS_CFG_OPT_NONE,           0}},   /* Cmd 0 */
    {{RES_IDX_CX},      {RES_CX_OFF,      TCS_CFG_OPT_NONE,           0}},   /* Cmd 1 */
    {{RES_IDX_MX},      {RES_MX_RET,      TCS_CFG_OPT_NONE,           0}},   /* Cmd 2 */
    {{RES_IDX_SOC},     {VRM_SOC_OFF,     TCS_CFG_OPT_NONE,           0}},   /* Cmd 3 */
  },

  /* Wake TCS (TCS 2) */
  {
    /* Res Index      Resource data     Options                     Addr offset */
    {{RES_IDX_MX},      {RES_MX_MOL,      TCS_CFG_OPT_CMD_RESP_REQ,   0}},   /* Cmd 0 */
    {{RES_IDX_CX},      {RES_CX_MOL,      TCS_CFG_OPT_CMD_RESP_REQ,   0}},   /* Cmd 1 */
    {{RES_IDX_XO},      {RES_XO_MOL,      TCS_CFG_OPT_CMD_RESP_REQ,   0}},   /* Cmd 2 */
    {{RES_IDX_SOC},     {VRM_SOC_ON,      TCS_CFG_OPT_CMD_RESP_REQ,   0}},   /* Cmd 3 */
  }
};

