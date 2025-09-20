/*============================================================================
  FILE:         PCieBsp.c

  OVERVIEW:     BSP for Pcie.

  DEPENDENCIES: None

Copyright (c) 2017-2019 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  06/25/19    ts   Included support for CLS subtype 1
  10/08/18    ts   Created seperate file for each settings and Bi-furcation
                   support as well as V2 settings
  08/24/18    ts   Updated PHY init sequence for V1
  05/16/18    ts   Change PERST to function 0
  05/16/18    ts   Enable all pcie cores
  03/20/18    ts   HW Errata TCSR_PCIEPHY_LINK_CONFIG for V1 & clock Enable
  09/11/2017  ts   Initial checkin for SDM1000

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "PcieBsp.h"
#include "PcieHwioRegs.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ARRAY_LENGTH(a)                     (sizeof(a) / sizeof((a)[0]))
#define PCIE_CLASS_CODE_BRIDGE_DEVICE       (0x06000000)
#define PCIE_SUBCLASS_CODE_PCI_2_PCI_BRIDGE (0x00040000)

#define PCIE_RP(x)                          (1UL << (x))

/***************************************************************************//**
 * @def RPMASK
 *
 * RPMASK will hold the information how many root ports to be enabled
 * and which are the root ports will be enabled
 *
 * using this def, total number of port info created inside rootPort_t
 * will be restricted
 *
 * @note
 * If Root port 0 and 1 need to be enabled, then RPMASK should be
 * #define RPMASK  (PCIE_RP(0) | PCIE_RP(1))
 ******************************************************************************/
#define RPMASK (PCIE_RP(3) | PCIE_RP(2) | PCIE_RP(1) | PCIE_RP(0) )

/*----------------------------------------------------------------------------
 * Available setttings file
 * -------------------------------------------------------------------------*/
#include "PcieBsp2x2.c"
#include "PcieBsp1x4.c"
#include "PcieBspCls.c"
#include "PcieBspCls1.c"
#include "PcieBspCls2.c"
