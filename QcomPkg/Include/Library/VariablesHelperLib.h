/******************************************************************//**
 * @file VariablesHelperLib.h
 *
 * Copyright (c) 2018-2019 by Qualcomm Technologies, Inc. 
 * All Rights Reserved.
 *
 *********************************************************************/
/*=======================================================================
                        Edit History

when       who     what, where, why
--------   ----    --------------------------------------------------- 
03/08/18   ai      Initial Null checkin
01/17/19   ai      Minor Refactoring
========================================================================*/
#ifndef __UEFIVARIABLES_HELPER_LIB_H__
#define __UEFIVARIABLES_HELPER_LIB_H__

#include <Uefi.h>

/* Shared structure from qsee_stor.h in TZ */
typedef enum
{
  QSEE_STOR_EMMC_USER  = 0,           /* User Partition in eMMC,  LUN0 in UFS */
  QSEE_STOR_EMMC_BOOT0 = 1,           /* Boot0 Partition in eMMC, LUN1 in UFS */
  QSEE_STOR_EMMC_BOOT1 = 2,           /* Boot1 Partition in eMMC, LUN2 in UFS */
  QSEE_STOR_EMMC_RPMB  = 3,           /* RPMB Partition in eMMC, UFS */
  QSEE_STOR_EMMC_GPP1  = 4,           /* GPP1 Partition in eMMC,  LUN4 in UFS */
  QSEE_STOR_EMMC_GPP2  = 5,           /* GPP2 Partition in eMMC,  LUN5 in UFS */
  QSEE_STOR_EMMC_GPP3  = 6,           /* GPP3 Partition in eMMC,  LUN6 in UFS */
  QSEE_STOR_EMMC_GPP4  = 7,           /* GPP4 Partition in eMMC,  LUN7 in UFS */
  QSEE_STOR_EMMC_ALL   = 8,           /* Entire eMMC device */
  QSEE_STOR_SPINOR_ALL = 0xB,         /* SPINOR device */
  QSEE_STOR_ID_RESERVED = 0x7FFFFFFF  /* Reserved: Device ID Max */
} qsee_stor_device_id_type;

/**
  Set default location of varstore dynamically for non-secure configuration

  @return EFI_SUCCESS     Sets OUT param to varstore QSEE_STOR id
                          based on internal policy
 **/
EFI_STATUS GetDefaultVarStore (OUT UINT32*);

/**
 * Load UefiSecApp TA
 */
EFI_STATUS LoadUefiSecApp (VOID);

#endif // __UEFIVARIABLES_HELPER_LIB_H__

