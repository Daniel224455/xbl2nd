#ifndef POWERSLEEPTARGETLIB_H
#define POWERSLEEPTARGETLIB_H

/******************************************************************//**
 * @file PowerSleepTargetLib.c
 *
 * @brief PowerSleepLib target specific functions
 *
 * Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 *********************************************************************/


#include <Library/rpmh_client.h>
#include "rsc_hwio.h"


/**
 * <!-- psleep_get_num_cores -->
 *
 * @brief Returns the # of cores supported for this target
 *
 * @return number of cores supported
 */
uint32 psleep_get_num_cores(void);

/**
 * <!-- psleep_cx_sd_supported -->
 *
 * @brief Returns whether or not we support CX collapse on this target
 *
 * @return TRUE if CX collapse is supported
 */
boolean psleep_cx_sd_supported(void);

/**
 * <!-- psleep_rsc_cfg_get_set_cfg -->
 *
 * @brief Gets the associated static config for the give sent, and indicates how many command sets are used
 *
 * @param set : if we should retrieve the sleep or wake set
 *
 * @param num_sets : Byref var to indicate # of command sets in the returned array
 *
 * @return static config command sets for the given set
 */
rpmh_command_set_t *psleep_rsc_cfg_get_set_cfg(rpmh_set_enum set, uint32 *num_sets);

#endif //POWERSLEEPTARGETLIB_H
