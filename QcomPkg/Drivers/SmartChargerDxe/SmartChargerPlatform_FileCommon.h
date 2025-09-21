/** 
  @file  SmartChargerPlatform_FileCommon.h
  @brief SmartCharger platform File definitions.
*/
/*=============================================================================
  Copyright (c) 2017 QUALCOMM Technologies Incorporated.
  All rights reserved.
  Qualcomm Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 08/02/17   pbitra  Initial revision
=============================================================================*/

#ifndef __SMARTCHARGERPLATFORM_FILE_COMMON_H__
#define __SMARTCHARGERPLATFORM_FILE_COMMON_H__

#include "SmartChargerPlatform.h"

/*===========================================================================
                     MACRO DEFINATIONS
===========================================================================*/

/*===========================================================================
                     TYPE DECLARATIONS
===========================================================================*/

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
EFI_STATUS SmartChargerPlatformFile_DebugReadFile(CHAR16 *pFilePath, UINT8 **pFileBuffer, UINTN *pDataSize);

#endif  /* __CHARGERPLATFORM_FILE_COMMON_H__ */
