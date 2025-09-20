/** @file FuseControlLib.h
   
  Contains target specific library functions related to fuses.

  Copyright (c) 2012 - 2015,2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/22/19   dp      Added IsImageCertDebugDisabled
 09/20/18   kpa     Added IsSecFuseLibNull
 02/13/15   sm      Removed Init call
 01/08/15   sk      Added more APIs for HSTI support
 10/09/14   sk      Added support for constructor and exitboot services callback
 04/09/13   shl     Added production device check
 08/14/12   niting  Add JTAG debug disable check
 05/31/12   niting  Created
=============================================================================*/
#ifndef __FUSECONTROLLIB_H__
#define __FUSECONTROLLIB_H__

#include <Uefi.h>

/**
  Check if the necessary conditions are satisfied to auto provision the RPMB

  @retval  TRUE    Auto Provisioning enabled
  @retval  FALSE   Auto Provisioning disabled

**/
BOOLEAN 
EFIAPI
AutoProvisionRPMBEnabled(void);

/**
 Check if the debug disable fuses are blown

  @retval TRUE Any of the related DEBUG_DISABLE fuses are blown
  @retval FALSE None of the related DEBUG_DISABLE fuses are blown

**/
BOOLEAN 
EFIAPI
JTAGDebugDisableFusesBlown(void);

/**
 Check if this is a production device

  @retval TRUE if it is
  @retval FALSE if it is not

**/
BOOLEAN 
EFIAPI
IsProductionDevice(void);

/**
  Check if RPMB is provisioned

  @retval TRUE if it is
  @retval FALSE if it is not

**/
BOOLEAN 
EFIAPI
IsRpmbProvisioned(void);

/**
  Check if SHK is provisioned

  @retval TRUE if it is
  @retval FALSE if it is not

**/
BOOLEAN 
EFIAPI
IsShkProvisioned(void);

/**
  Check if rollback is enabled

  @retval TRUE if it is
  @retval FALSE if it is not

**/
BOOLEAN 
EFIAPI
IsRollbackEnabled(void);

/**
  Check if debug check in Image Certificate passed

  @retval TRUE if it is
  @retval FALSE if it is not

**/
BOOLEAN 
EFIAPI
IsImageCertDebugDisabled(void);


/*===========================================================================
**  Function :  IsSecFuseLibNull
** ==========================================================================
*/
/**
 Check if SecFuseLib is a stub/ Null Lib

  @retval TRUE Null Lib implementation of SecFuseLib always returns TRUE else FALSE

**/
BOOLEAN 
EFIAPI
IsSecFuseLibNull(void);

#endif /* __FUSECONTROLLIB_H__ */
