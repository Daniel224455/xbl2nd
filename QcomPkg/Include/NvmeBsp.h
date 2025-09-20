#ifndef NVMEBSP_H
#define NVMEBSP_H

/** @file NvmeBsp.h

  Thsi file contains definitions to support NVMe on Qualcomm boards.

  Copyright (c) 2018, Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential
**/

/*=============================================================================
                              EDIT HISTORY


when         who   what, where, why
----------   ---   --------------------------------------------------------------
2018-06-15   wek   Initial version, add power on API.

=============================================================================*/

/* Turn on the power rail associated with NVMe.
 *
 * param  slot    On devices with multiple NVMe/PCIe it indicates which slot to turn on.
 *
 * return  0 on success, -1 on error.
 *
 **/
INTN nvme_core_target_power_on(UINT32 slot);

#endif /* #ifndef NVMEBSP_H */

