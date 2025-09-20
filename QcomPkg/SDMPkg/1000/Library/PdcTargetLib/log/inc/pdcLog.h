#ifndef _PDCINT_LOG_H_
#define _PDCINT_LOG_H_
/*============================================================================
  FILE:         pdcInt_log.h

  OVERVIEW:     This file declares the interface to the drivers logging system

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

==============================================================================
============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
                           GLOBAL FUNCTION DECLARATIONS
 =============================================================================*/

/**
 * pdcLog_printf
 *
 * @brief Logs a debug message using the standard C printf function format
 *
 * @param arg_count:  Number of parameters that are being logged.
 * @param format:     The format string for printing log.
 */
void pdcLog_printf(int arg_count, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _PDCINT_LOG_H_ */

