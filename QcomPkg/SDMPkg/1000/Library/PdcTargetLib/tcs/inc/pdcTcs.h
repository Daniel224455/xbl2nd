#ifndef _PDCTCS_H_
#define _PDCTCS_H_
/*============================================================================
  FILE:         pdcTcs.h

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
#include "comdef.h"
#include "pdcTcsCfg.h"
#include "tcs_resource.h"

/*===========================================================================
 *                         GLOBAL FUNCTION DEFINITIONS
 *===========================================================================*/
/**
 * pdcTcs_getResources
 *
 * @brief Retreives the command/data pairs for each PDC TCS
 *
 * @return Array pointer of NUM_COMMANDS_PER_TCS size which contains the TCS commands for
 *         the given TCS number
 *
 * @param tcsNum: TCS number to get
 */
const pdc_tcs_config* pdcTcs_getResources(tcs_usage tcsNum);

/**
 * pdcTcs_getAllResourceList
 *
 * @brief Retreives a list of all the resources that are avaiable for control in this TCS target
 *        driver
 *
 * @return Array pointer of TCS_TOTAL_RESOURCE_NUM size which contain the resources controlled
 *         for this target
 */
const pdc_tcs_resource* pdcTcs_getAllResourceList(void);

#endif /* _PDCTCS_H_ */

