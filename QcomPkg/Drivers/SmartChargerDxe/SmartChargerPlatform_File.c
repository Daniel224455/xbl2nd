/** @file SmartChargerPlatform_File.c

  This file will set up file logging, load and parse the cfg file and the configuration file from efiesp.

  Copyright (c) 2017,  Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY

 when         who     what, where, why
 --------     ---     -----------------------------------------------------------
 08/02/17     pbitra  Initial revision

=============================================================================*/
#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/QcomLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ParserLib.h>
#include <Library/QcomTargetLib.h>
#include <Library/FuseControlLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/FileSystemInfo.h>
#include <Guid/FileInfo.h>

#include <api/pmic/pm/pm_uefi.h>

/**
Protocol Dependencies
*/
#include <Protocol/EFIPlatformInfo.h>

#include "DDIPlatformInfo.h"
#include <Protocol/SimpleFileSystem.h>

/**
Charger headers
*/

#include "SmartChargerPlatform_File.h"
#include "SmartChargerPlatform_FileCommon.h"

#include "SmartChargerPlatform.h"
#include <api/pmic/charger/ChargerLibCommon.h>

#define SMARTCHARGER_CFG_FILE_IN_FV              L"SmartChargerCfg.cfg"
#define SMARTCHARGER_OVERWRITE_CFG_FILE_PATH     L"\\SmartChargerCfg.cfg"

extern VOID SmartChargerPlatform_ReadCfgParams( UINT8* Section, UINT8* Key, UINT8* Value);
/*===========================================================================
  FUNCTION DECLARATION
===========================================================================*/

#define NULL_CHAR              '\0'

/* LogFS Partition GUID for Config Parameters */
extern EFI_GUID gEfiLogFSPartitionGuid;


/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
EFI_STATUS SmartChargerPlatformFile_FileLogInit( SMART_CHARGER_PLATFORM_CFGDATA_TYPE ChargerPlatformCfgData )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (TRUE == ChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg_to_file)
  {
    if (!RETAIL) 
    {
      /* Mounting Log fs */
      Status = MountFatPartition(L"logfs");
      SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a LogFS mount Status = %r \r\n", __FUNCTION__, Status));
    }
    Status = SmartChargerLib_InitFileLog(ChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg_to_file);
  }
  else
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a File Log Disabled  = %d \r\n", __FUNCTION__, ChargerPlatformCfgData.ChargerLibCfgData.dbg_cfg_Data.print_charger_app_dbg_msg_to_file));
  }

  return Status;
}

/**
  SmartChargerPlatformFile_AsciiToInt

  @brief
  Converts ASCII string to integer
**/
INT32 SmartChargerPlatformFile_AsciiToInt
(
  CHAR8 *Str
)
{
    INT32 Value = 0;
    int   Sign  = 1;

    if(Str == NULL)
        return 0;

    if(*Str == '-')
    {
        Sign = -1;
        Str++;
    }

    while(*Str != NULL_CHAR)
    {
        if(*Str >= '0' && *Str <= '9')
        {
            Value = ((Value * 10) + (*Str - '0'));
        }
        Str++;
    }

    return (Value * Sign);
}

UINT32 SmartChargerPlatformFile_AsciiStrToHex (CHAR8* Str, UINT32 StrSize)
{
  UINT32 result = 0;
  if(Str == NULL)
    return 0;

  while( (*Str!=0) && StrSize )
  {
    // Break as soon as non-hex char encountered.
    if (!((*Str >= '0' && *Str <= '9') ||
          (*Str >= 'a' && *Str <= 'f') ||
          (*Str >= 'A' && *Str <= 'F')))
      {
        result = 0;
        break;
      }
    result = result << 4;
    if(*Str >= '0' && *Str <= '9')
      result = result + (*Str - '0');
    if(*Str >= 'a' && *Str <= 'f')
      result = result + (*Str - 'a') + 10;
    if(*Str >= 'A' && *Str <= 'F')
      result = result + (*Str - 'A') + 10;

    StrSize--;
    Str++;
  }

  return result;
}

EFI_STATUS SmartChargerPlatformFile_OverwriteDefaultCfg( void )
{
  EFI_STATUS  Status         = EFI_SUCCESS;
  UINT8      *pFileBuffer    = NULL;
  UINTN       DataSize       = 0;
  INTN        Pd             = 0;

  /* Attempt to load Charger Config file from LogFS parition */

  /* Read from LogFs or EFIESP */

  Status = SmartChargerPlatformFile_DebugReadFile(SMARTCHARGER_OVERWRITE_CFG_FILE_PATH, &pFileBuffer, &DataSize);
  if ((Status != EFI_SUCCESS ) || (NULL == pFileBuffer))
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a Could not open ChargerApp Config File = %r \r\n",__FUNCTION__, Status));
    return EFI_LOAD_ERROR;
  }

  /* This debug will print only when config file is found */
  SMARTCHARGER_FILE_UART_DEBUG((EFI_D_WARN, "SmartChargerDxe:: %a OverWrite ChargerApp Config parameters = %r, DataSize = %d \r\n",__FUNCTION__, Status, DataSize));

  Pd = OpenParser((UINT8 *)pFileBuffer, (UINT32)DataSize, NULL);
  if (Pd < 0)
  {
    Status = EFI_LOAD_ERROR;
  }
  else
  {
    /* Kick off Pmic CFG file interpreter */
    EnumKeyValues(Pd, NULL, SmartChargerPlatform_ReadCfgParams);
  }

  /* Clean up resources */
  CloseParser(Pd);

  if(pFileBuffer != NULL)
  {
    Status = gBS->FreePool(pFileBuffer);
  }

  return Status;

}

/**
SmartChargerPlatformFile_ReadDefaultCfgData()

@brief
Read Battery Parameter Default Configurations from configuration file
*/
EFI_STATUS
SmartChargerPlatformFile_ReadDefaultCfgData
(
  VOID
)
{
  EFI_STATUS  Status     = EFI_SUCCESS;
  UINT8*      FileBuffer = NULL;
  UINTN       FileSize   = 0;
  INTN        Pd;

  Status = ReadFromFV(SMARTCHARGER_CFG_FILE_IN_FV, (void **) &FileBuffer, &FileSize);
  if (Status == EFI_SUCCESS)
  {
    Pd = OpenParser (FileBuffer, (UINT32)FileSize, NULL);

    if (Pd < 0)
    {
      SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a  Charger Config- Parser open failed\n", __FUNCTION__));
      Status = EFI_LOAD_ERROR;
    }
    else
    {
      /* Kick off Pmic CFG file interpreter */
      EnumKeyValues (Pd, NULL, SmartChargerPlatform_ReadCfgParams);
    }

    /* Clean up resources */
    CloseParser(Pd);
    FreePool(FileBuffer);
  }

  /*Read from Debug (LogFS or EFIESP) partition to over write value - supported for debug mode*/
  if (!RETAIL) 
  {
    SmartChargerPlatformFile_OverwriteDefaultCfg();
  }

  return Status;
}

/**
SmartChargerPlatformFile_InitFileLog(BOOLEAN )

@brief  Gets File Log Info
Returns Status
*/
EFI_STATUS SmartChargerPlatformFile_GetLogInfo(SMART_CHARGER_PLATFORM_LOG_INFO *pFileLogInfo)
{
  EFI_STATUS Status  =  EFI_SUCCESS;
  SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a \r\n", __FUNCTION__));

  if(!pFileLogInfo)
    return EFI_INVALID_PARAMETER;

  if (!RETAIL)
  {
    /* Enable default file logging if production mode is disabled */
    Status = SmartChargerLib_GetLogInfo((chargerlib_loginfo * )pFileLogInfo);
    SMARTCHARGER_DEBUG(( EFI_D_WARN, "SmartChargerDxe:: %a SmartChargerLib_GetFileLogHandle Status = %r \r\n", __FUNCTION__, Status));
  }

  return Status;
}


EFI_STATUS SmartChargerPlatformFile_GetFileBuffer(CHAR16 *pFilePath, UINT8 **pFileBuffer, UINTN *pDataSize)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_GUID   *RootDeviceType = NULL;
  EFI_GUID   *PartitionType  = NULL;
  UINT32      BlkIOAttrib    = 0;

  /* Load Battery Profile file from Plat Provision Dir */
  /* Set attributes for fetching Block IO handle */
  BlkIOAttrib = BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  BlkIOAttrib |= BLK_IO_SEL_SELECT_MOUNTED_FILESYSTEM;
  BlkIOAttrib |= BLK_IO_SEL_PARTITIONED_GPT;

  BlkIOAttrib |= BLK_IO_SEL_MATCH_PARTITION_LABEL;

  BlkIOAttrib |= BLK_IO_SEL_STRING_CASE_INSENSITIVE;

  /* Load File from Plat\Provision Paritition */
  Status = LoadFileFromPartition(BlkIOAttrib,
                                 pFilePath,
                                 RootDeviceType,
                                 PartitionType,
                                 L"PLAT",
                                 pFileBuffer, pDataSize);

  if ((Status != EFI_SUCCESS ) || (NULL == pFileBuffer))
  {
    SMARTCHARGER_DEBUG((EFI_D_WARN, "SMARTChargerDxe:: %a Could not open File = %r Path %a \r\n",__FUNCTION__, Status, pFilePath));
    return EFI_LOAD_ERROR;
  }

  return Status;

}

EFI_STATUS SmartChargerPlatformFile_DebugReadFile(CHAR16 *pFilePath, UINT8 **pFileBuffer, UINTN *pDataSize)
{
  EFI_STATUS DebugStatus = EFI_SUCCESS;
  EFI_GUID   *RootDeviceType = NULL;
  EFI_GUID   *PartitionType  = NULL;
  UINT32      BlkIOAttrib    = 0;

  if(!pFilePath || !pFileBuffer)
    return EFI_INVALID_PARAMETER;

  /* Attempt to Read battery profile from LogFs partition to over write profile - supported for debug mode */

  BlkIOAttrib = BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  BlkIOAttrib |= BLK_IO_SEL_SELECT_MOUNTED_FILESYSTEM;
  BlkIOAttrib |= BLK_IO_SEL_PARTITIONED_GPT;

  BlkIOAttrib |= BLK_IO_SEL_MATCH_PARTITION_LABEL;
  BlkIOAttrib |= BLK_IO_SEL_STRING_CASE_INSENSITIVE;

  /* Load File from LogFs Paritition */
  DebugStatus = LoadFileFromPartition(BlkIOAttrib,
                                 pFilePath,
                                 RootDeviceType,
                                 PartitionType,
                                 L"logfs",
                                 pFileBuffer, pDataSize);
  if(DebugStatus == EFI_NO_MEDIA)
  {
     DebugStatus = MountFatPartition(L"logfs");
     SMARTCHARGER_DEBUG((EFI_D_WARN, "SMARTChargerDxe:: %a LogFs mount Status = %r \r\n", __FUNCTION__, DebugStatus));
       /* Load File from LogFs Paritition */
     DebugStatus = LoadFileFromPartition(BlkIOAttrib,
                                 pFilePath,
                                 RootDeviceType,
                                 PartitionType,
                                    L"logfs",
                                 pFileBuffer, pDataSize);

   }
  if((EFI_SUCCESS == DebugStatus ) && (NULL != pDataSize))
  {
     SMARTCHARGER_FILE_UART_DEBUG(( EFI_D_WARN, "SMARTChargerDxe:: %a read debug file success, size = %d \r\n", __FUNCTION__, *pDataSize));
  }
  else
  {
     SMARTCHARGER_DEBUG(( EFI_D_WARN, "SMARTChargerDxe:: %a read debug file failed %r \r\n", __FUNCTION__, DebugStatus));
  }

  return DebugStatus;
}

