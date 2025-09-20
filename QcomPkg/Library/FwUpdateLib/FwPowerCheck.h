/** @file FwPowerCheck.h
   
  Header for Battery power level checks for Firmware Update.

  Microsoft requirement is that system must have at least 50% battery life to
  as a pre-installation check to permit firmware update.

  Set ALLOW_NO_BATTERY_FIRMWARE_UPDATE to 1 to permit firmware update
  on devices that have no battery connected on Qualcomm reference hardware.

  Copyright (c) 2012, 2017, 2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 ---------      ---         --------------------------------------------------
 2017/04/18     gparam      Changed the update status from internal type to
                            standard ones defined in EDK headers
 2012/04/30     rsb         Initial version

=============================================================================*/
#ifndef __QCOM_FW_POWERCHECK_H_
#define __QCOM_FW_POWERCHECK_H_

#include "FwEsrt.h"
#include <Library/FwCommonLib.h>

/**
 * Determine if the battery power levels have sufficient power
 * to permit firmware update to proceed based on the pre-condition
 * checks required for UEFI firmware update:
 *    1) System must have at least 25% battery life
 *
 * For Qualcomm reference hardware (if ALLOW_NO_BATTERY_FIRMWARE_UPDATE is defined):
 *    2) or has a DC adaptor plugged in with no battery
 *
 * @param bPowerGood        - TRUE : Sufficient power to proceed with firmware update.
 *                            FALSE: Insufficient power to proceed with firmware update.
 * @return EFI_STATUS 
 */


EFI_STATUS
FwUpdateCheckPowerLevel(
  OUT BOOLEAN *bPowerGood
);



#endif
