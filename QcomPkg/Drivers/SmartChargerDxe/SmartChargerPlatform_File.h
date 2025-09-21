/** 
  @file  SmartChargerPlatform_File.h
  @brief SmartCharger platform file definitions.
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

#ifndef __SMARTCHARGERPLATFORM_FILE_H__
#define __SMARTCHARGERPLATFORM_FILE_H__

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
INT32 SmartChargerPlatformFile_AsciiToInt( CHAR8 *Str );

UINT32 SmartChargerPlatformFile_AsciiStrToHex (CHAR8* Str, UINT32 StrSize);

EFI_STATUS SmartChargerPlatformFile_ReadDefaultCfgData( VOID );

EFI_STATUS SmartChargerPlatformFile_FileLogInit( SMART_CHARGER_PLATFORM_CFGDATA_TYPE ChargerPlatformCfgData );

EFI_STATUS SmartChargerPlatformFile_GetLogInfo(SMART_CHARGER_PLATFORM_LOG_INFO *pFileLogInfo);

EFI_STATUS SmartChargerPlatformFile_DebugReadFile(CHAR16 *pFilePath, UINT8 **pFileBuffer, UINTN *pDataSize);

#endif  /* __SMARTCHARGERPLATFORM_FILE_H__ */
