/** @file LoadApps.c
  Implementation of modules to load hlos specific applications

  Copyright (c) 2011-2019 Qualcomm Technologies, Inc.  All
  Rights Reserved. Qualcomm Technologies Proprietary and
  Confidential.

**/

/*=============================================================================
                              EDIT HISTORY


 when      who      what, where, why
--------   ---      ----------------------------------------------------------
10/12/19   mj       Removal of Shmbridge creation in apps region
09/10/19   mj       Indentation Changes
11/23/18   pr       Changes related to assigning TZapps regions to AC_VM_HLOS_UNMAPPED
09/27/18   pj       Updates to enable loading of TZ-embedded common libs and tpm TA
09/19/18   pr       Enabled TpmDxe
09/13/18   shl      Revert to previous version
08/03/18   shl      Added TPM header file, and formating
06/14/18   pj       TPM app separation from winsecapp; removed backup app loading attempt from dedicated winsecapp partition
06/12/18   pr       Removed Dxhdcp2 app loading code as HLOS loads Dxhdcp2 app
05/03/18   pj       Ported SHMBridge changes to Poipu
07/12/16   rj       Created the file. Moved function from TzeLoader.c
10/24/16   pr       Changes to load 64 bit CommonLib binary needed for 64 bit secure apps
07/14/17   dp       MountFatPartition() call moved to LoadSecureApps() from TzeLoaderProtocolInit()
11/22/17   nk       Comment Asserts for load app for Rumi
=============================================================================*/


/*=========================================================================
      Include Files
==========================================================================*/

#include <Library/QcomLib.h>
#include <Library/UefiLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/TzRuntimeLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/EfiFileLib.h>
#include <Library/ArmLib.h>
#include <Library/PcdLib.h>
#include <Guid/FileSystemVolumeLabelInfo.h>
#include "QcomLib.h"
#include <Protocol/EFITzeLoader.h>
#include <Library/QcomBaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/EFIScm.h>
#include <Protocol/EFITpm.h>
#include <Include/scm_sip_interface.h>
#include <Include/scm_qsee_interface.h>
#include <Library/UefiCfgLib.h>
#include <Library/BootConfig.h>
#include <Library/SerialPortLib.h>
#include <Protocol/EFIShmBridge.h>
#define TZ_FVER_QSEE                         10 /**< QSEE application layer. */

#define TZBSP_computed_VERSION(major, minor, patch) \
  (((major & 0x3FF) << 22) | ((minor & 0x3FF) << 12) | (patch & 0xFFF))

#define VARIABLE_SERVICES_MBN_FILE           L"\\TZAPPS\\uefisec.mbn"
#define COMMON_LIB_MBN_FILE                  L"\\TZAPPS\\cmnlib.mbn"
#define WINSECAPP_MBN_FILE                   L"\\TZAPPS\\winsecap.mbn"
#define COMMON_LIB_64_MBN_FILE               L"\\TZAPPS\\cmnlib64.mbn"
#define TPM_MBN_FILE                         L"\\TZAPPS\\tpm.mbn"
#define MSSECAPP_MBN_FILE                    L"\\TZAPPS\\mssecapp.mbn"

STATIC EFI_GUID TzAppsPartitionType =
{ 0x14D11C40, 0x2A3D, 0x4F97, { 0x88, 0x2D, 0x10, 0x3A, 0x1E, 0xC0, 0x93, 0x33 } };


BOOLEAN TZSecureAppLoadCalled = FALSE;

extern EFI_STATUS TzGetFeatureVersion(UINT32  FeatureId, UINT32 *Version);

extern  EFI_STATUS QseeAppsRegionNotificationSyscall(
    UINT64 applications_region_addr,
    UINT64 applications_region_size
);

extern  EFI_STATUS QseeAppRegionAssignSyscall (
    UINT64 applications_region_addr,
    UINT64 applications_region_size
);

extern EFI_STATUS
LoadSecureImageFromFileSystem(
    IN  EFI_TZE_LOADER_PROTOCOL        *This,
    IN  EFI_GUID                       *pPartitionGuid,
    CHAR16                             *Path,
    IN BOOLEAN                         ServicesMbn,
    OUT UINT32                         *AppIdPtr
);

extern EFI_STATUS
LoadImageFromPartition(
    IN  EFI_TZE_LOADER_PROTOCOL        *This,
    IN  EFI_GUID                       *pPartitionGuid,
    IN  UINT16                         *pPartitionLabel,
    OUT UINT32                         *AppIdPtr
);

extern EFI_STATUS
LoadImageFromPartitionLabel(
    IN  EFI_TZE_LOADER_PROTOCOL        *This,
    IN  UINT16                         *pPartitionLabel,
    OUT UINT32                         *AppIdPtr
);

extern EFI_STATUS
LoadImageFromFile(
    IN  EFI_TZE_LOADER_PROTOCOL      *This,
    IN  CHAR8*                        FilePath,
    OUT UINT32                       *AppIdPtr
);

extern EFI_STATUS
LoadImageFromBuffer(
    IN  EFI_TZE_LOADER_PROTOCOL     *This,
    IN  UINT32                      *Buffer,
    IN  UINTN                        BufferSize,
    OUT UINT32                      *AppIdPtr
);

extern EFI_STATUS
LoadImageFromFileSystem(
    IN  EFI_TZE_LOADER_PROTOCOL        *This,
    IN  EFI_GUID                       *pPartitionGuid,
    CHAR16                             *Path,
    OUT UINT32                         *AppIdPtr

);

extern EFI_STATUS
ActivateTzEmbeddedSecureImage(
    IN  EFI_TZE_LOADER_PROTOCOL        *This,
    IN  UINT8                          *app_name,
    IN  UINT64                         app_name_length,
    IN BOOLEAN                         ServicesMbn,
    OUT UINT32                         *AppIdPtr
);


/* Prints TZ Diag Buffer Contents on to Serial Console
*/
EFI_STATUS print_tz_diag(void)
{
    EFI_STATUS              Status;
    UINT32                   SharedIMEMBaseAddress = 0;
    UINT32                  *tzDiagStartAddressRead = NULL;
    UINT8                  *tzDiagStartAddress = NULL;
    UINT32                  tzDiagSize = 0;
    UINT32                  tzDiagOffset = 0;
    UINT32                  tempSize = 0;
    UINT32                  configFlag = 0;
    //  uint8 *tzDiagStartAddress = (uint8 *)*((uint32 *)0x066BF720);
    //  uint32 diag_size = 4096;

    /* Check if qsee Diag logging is enabled (in uefiplat.cfg file) */
    Status = GetConfigValue("SecurityFlag", &configFlag);
    /*Return without printing the diag log if flag is disabled in config file*/
    if ((Status != EFI_SUCCESS) || ((configFlag & ENABLE_QSEE_DIAG_LOGS_FLAG) != ENABLE_QSEE_DIAG_LOGS_FLAG))
    {
        DEBUG((EFI_D_INFO, "Failed to get QseeDiagLogEnableFlag, Status=%r\n", Status));
        return EFI_SUCCESS;
    }
    /*Get the config values and start printing*/
    DEBUG((EFI_D_ERROR, "******START-TZDIAG**********\r\n"));

    Status = GetConfigValue("SharedIMEMBaseAddr", &SharedIMEMBaseAddress);
    if (Status != EFI_SUCCESS)
    {
        return Status;
    }
    Status = GetConfigValue("TzDiagOffset", &tzDiagOffset);
    if (Status != EFI_SUCCESS)
    {
        return Status;
    }
    if (tzDiagOffset == 0)
        return EFI_SUCCESS;
    tzDiagStartAddressRead = (UINT32 *)((UINTN)SharedIMEMBaseAddress + tzDiagOffset);
    Status = GetConfigValue("TzDiagSize", &tzDiagSize);
    if (tzDiagSize == 0)
        return EFI_SUCCESS;
    tempSize = tzDiagSize;
    tzDiagStartAddress = (UINT8 *)(UINTN) * tzDiagStartAddressRead;
    /*
      while(tempSize > 0)
      {
        DebugPrint(DEBUG_ERROR, "0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \r\n", *(tzDiagStartAddress+0),*(tzDiagStartAddress+1),*(tzDiagStartAddress+2),*(tzDiagStartAddress+3),*(tzDiagStartAddress+4),*(tzDiagStartAddress+5),*(tzDiagStartAddress+6),*(tzDiagStartAddress+7),*(tzDiagStartAddress+8),*(tzDiagStartAddress+9),*(tzDiagStartAddress+10),*(tzDiagStartAddress+11),*(tzDiagStartAddress+12),*(tzDiagStartAddress+13),*(tzDiagStartAddress+14),*(tzDiagStartAddress+15));
        tempSize -= 16;
        tzDiagStartAddress += 16;
      }
      tempSize = tzDiagSize;
      tzDiagStartAddress = (UINT8 *)*(tzDiagStartAddressRead);
      while(tempSize > 0)
      {
        DebugPrint( DEBUG_ERROR, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", *(tzDiagStartAddress+0),*(tzDiagStartAddress+1),*(tzDiagStartAddress+2),*(tzDiagStartAddress+3),*(tzDiagStartAddress+4),*(tzDiagStartAddress+5),*(tzDiagStartAddress+6),*(tzDiagStartAddress+7),*(tzDiagStartAddress+8),*(tzDiagStartAddress+9),*(tzDiagStartAddress+10),*(tzDiagStartAddress+11),*(tzDiagStartAddress+12),*(tzDiagStartAddress+13),*(tzDiagStartAddress+14),*(tzDiagStartAddress+15));
        tzDiagStartAddress += 16;
        DebugPrint( DEBUG_ERROR, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", *(tzDiagStartAddress+0),*(tzDiagStartAddress+1),*(tzDiagStartAddress+2),*(tzDiagStartAddress+3),*(tzDiagStartAddress+4),*(tzDiagStartAddress+5),*(tzDiagStartAddress+6),*(tzDiagStartAddress+7),*(tzDiagStartAddress+8),*(tzDiagStartAddress+9),*(tzDiagStartAddress+10),*(tzDiagStartAddress+11),*(tzDiagStartAddress+12),*(tzDiagStartAddress+13),*(tzDiagStartAddress+14),*(tzDiagStartAddress+15));
        tzDiagStartAddress += 16;
        DebugPrint( DEBUG_ERROR, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", *(tzDiagStartAddress+0),*(tzDiagStartAddress+1),*(tzDiagStartAddress+2),*(tzDiagStartAddress+3),*(tzDiagStartAddress+4),*(tzDiagStartAddress+5),*(tzDiagStartAddress+6),*(tzDiagStartAddress+7),*(tzDiagStartAddress+8),*(tzDiagStartAddress+9),*(tzDiagStartAddress+10),*(tzDiagStartAddress+11),*(tzDiagStartAddress+12),*(tzDiagStartAddress+13),*(tzDiagStartAddress+14),*(tzDiagStartAddress+15));
        tzDiagStartAddress += 16;
        tempSize -= 48;
        DebugPrint( DEBUG_ERROR, "\r\n");
      }
      */
    SerialPortWrite(tzDiagStartAddress, tzDiagSize);
    DEBUG((EFI_D_ERROR, "******END-TZDIAG********** \r\n"));
    return EFI_SUCCESS;
}

/**
Loads all Secure Apps at bootup

@param  Path            Directory containging ACPI table files
@param  pPartitionGuid  GUID eg:  TzAppsPartitionType
@param AppIdPtr         out  if success holds app id on return

@return EFI_SUCCESS if successful
**/

EFI_STATUS
LoadSecureApps(
    IN  EFI_TZE_LOADER_PROTOCOL        *This,
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable,
    OUT UINT32                         *AppIdPtr
)
{

    EFI_STATUS              Status = EFI_SUCCESS;
    UINT32                  AppId = 0;
    UINT32                  version = 0;
    UINT32                  computed_version = 0;
    UINT32                  CommonMbnLoadFlag = 0;
    UINT32                  VariableServicesFlag = 0;
    UINT32                  WinsecappFlag = 0;
    UINT32                  TreeTpmEnableFlag = 0;
    UINT32                  MSsecappFlag = 0;
    MemRegionInfo           TzAppsRegInfo;
    CHAR8*                  TzAppsLabel = "TzApps";
    UINT8                   EmbeddedTpmAppName[] = "tpm";

    if (TZSecureAppLoadCalled == TRUE)
    {
        Status = EFI_LOAD_ERROR;
        goto ErrorExit;
    }

    //Mounting TzApps
    Status = MountFatPartition(L"tzapps");
    if (EFI_ERROR(Status))
        DEBUG((EFI_D_WARN, "Failed to mount TZAPPS Fat partition, Status=%r\n", Status));

    //print tz diag once before start of loading secureapps
    print_tz_diag();

    Status = TzGetFeatureVersion(TZ_FVER_QSEE, &version);
    if (EFI_ERROR(Status))
    {
        goto ErrorExit;
    }

    computed_version = TZBSP_computed_VERSION(1, 2, 0);
    if (version >= computed_version)
    {
        Status = GetMemRegionInfoByName(TzAppsLabel, &TzAppsRegInfo);
        if (Status != EFI_SUCCESS)
        {
            DEBUG((EFI_D_INFO, "LoadSecureApps: Unable to find TZApps region addr in config file\n"));
            ASSERT(Status == EFI_SUCCESS);
        }

        DEBUG((EFI_D_INFO, "LoadSecureApps: Read Tzapps region with address 0x%x and size 0x%x from Memmap table\n", TzAppsRegInfo.MemBase, TzAppsRegInfo.MemSize));

        // Make an assign call to assign app region from HLOS to HLOS_UNMAPPED
        Status = QseeAppRegionAssignSyscall(TzAppsRegInfo.MemBase, TzAppsRegInfo.MemSize);
        if (Status != EFI_SUCCESS)
        {
            DEBUG ((EFI_D_ERROR, "LoadSecureApps: Assign call failed. 0x%x\n", Status));
        }
        ASSERT_EFI_ERROR(Status);

        DEBUG ((EFI_D_INFO, "LoadSecureApps: Assign call succeeded. 0x%x\n", Status));
        Status = QseeAppsRegionNotificationSyscall(TzAppsRegInfo.MemBase, TzAppsRegInfo.MemSize);
        ASSERT_EFI_ERROR(Status);
    }

    TZSecureAppLoadCalled = TRUE;

    computed_version = TZBSP_computed_VERSION(1, 1, 0);
    if (version >= computed_version)
    {
        Status = GetConfigValue("SecurityFlag", &CommonMbnLoadFlag);        // Get this flag from uefiplat.cfg. Depending on the flag, process call to this function
        if ((Status != EFI_SUCCESS) || ((CommonMbnLoadFlag & COMMON_MBN_LOAD_FLAG) != COMMON_MBN_LOAD_FLAG))
        {
            DEBUG((EFI_D_INFO, "CommonMbnLoadFlag not found in uefiplat.cfg. Defaulting to 0.\r\n"));
            // Default to 0. Does not matter what the flag is
            CommonMbnLoadFlag = 0;
            Status = EFI_SUCCESS;   // Continue for Hawker
        }
        else
        {
            Status = ActivateTzEmbeddedSecureImage(This,
                                                   NULL,
                                                   0,
                                                   TRUE,
                                                   &AppId);
            if (Status != EFI_SUCCESS)
            {
                DEBUG((EFI_D_WARN, "Unable to load embedded common libs\r\n"));
                print_tz_diag();
            }
            DEBUG((EFI_D_INFO, "LoadSecureApps: Embedded common libs load result %p\n", Status));

        }
    }

    Status = GetConfigValue("SecurityFlag", &TreeTpmEnableFlag);        // Get this flag from uefiplat.cfg. Depending on the flag, process call to this function
    if ((Status != EFI_SUCCESS) || ((TreeTpmEnableFlag & TREE_TPM_ENABLE_FLAG) != TREE_TPM_ENABLE_FLAG))
    {
        DEBUG((EFI_D_INFO, "TreeTpmEnableFlag not found in uefiplat.cfg. Defaulting to 0.\r\n"));
        // Default to 0. Does not matter what the flag is
        TreeTpmEnableFlag = 0;
        Status = EFI_SUCCESS;   // Continue for Hawker
    }
    else
    {
        Status = ActivateTzEmbeddedSecureImage(This,
            EmbeddedTpmAppName,
            sizeof(EmbeddedTpmAppName),
            FALSE,
            &AppId);
        DEBUG((EFI_D_INFO, "LoadSecureApps: Load TZ-embedded TPM app result: 0x%p\n", Status));

        if (EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR, "Embedded TPM app failed to load! Status = 0x%x\n", Status));
            print_tz_diag();
        }
    }
    
    Status = GetConfigValue("SecurityFlag", &VariableServicesFlag);        // Get this flag from uefiplat.cfg. Depending on the flag, process call to this function
    if ((Status != EFI_SUCCESS) || ((VariableServicesFlag & VARIABLE_SERVICE_FLAG) != VARIABLE_SERVICE_FLAG))
    {
        DEBUG((EFI_D_INFO, "VariableServicesFlag not found in uefiplat.cfg. Defaulting to 0.\r\n"));
        // Default to 0. Does not matter what the flag is
        VariableServicesFlag = 0;
        Status = EFI_SUCCESS;   // Continue for Hawker
    }
    else
    {
        Status = LoadSecureImageFromFileSystem(This,
            &TzAppsPartitionType,
            VARIABLE_SERVICES_MBN_FILE,
            FALSE,
            &AppId);
        DEBUG((EFI_D_INFO, "LoadSecureApps: Load from filesystem UEFISecApp: 0x%p\n", Status));
        if (EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR, "UEFISecApp failed to load! Status = 0x%x Info :", Status));
            print_tz_diag();
        }
    }

    Status = GetConfigValue("SecurityFlag", &WinsecappFlag);        // Get this flag from uefiplat.cfg. Depending on the flag, process call to this function
    if ((Status != EFI_SUCCESS) || ((WinsecappFlag & WIN_SEC_APP_FLAG) != WIN_SEC_APP_FLAG))
    {
        DEBUG((EFI_D_INFO, "WinsecappFlag not found in uefiplat.cfg. Defaulting to 0.\r\n"));
        // Default to 0. Does not matter what the flag is
        WinsecappFlag = 0;
        Status = EFI_SUCCESS;   // Continue for Hawker
    }
    else
    {
        Status = LoadSecureImageFromFileSystem(This,
            &TzAppsPartitionType,
            WINSECAPP_MBN_FILE,
            FALSE,
            &AppId);
        DEBUG((EFI_D_INFO, "LoadSecureApps: Load from filesystem result Winsecapp: 0x%p\n", Status));

        if (EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR, "WinSecApp failed to load! Status = 0x%x", Status));
            print_tz_diag();
        }
    }

    Status = GetConfigValue("SecurityFlag", &MSsecappFlag);        // Get this flag from uefiplat.cfg. Depending on the flag, process call to this function
    if ((Status != EFI_SUCCESS) || ((MSsecappFlag & MS_SEC_APP_FLAG) != MS_SEC_APP_FLAG))
    {
        DEBUG((EFI_D_INFO, "MSsecappFlag not found in uefiplat.cfg. Defaulting to 0.\r\n"));
        // Default to 0. Does not matter what the flag is
        DEBUG((EFI_D_ERROR, "MSsecapp failed to load! Status = 0x%x\n", Status));
        MSsecappFlag = 0;
        Status = EFI_SUCCESS;
    }
    else
    {
        Status = LoadSecureImageFromFileSystem(This,
            &TzAppsPartitionType,
            MSSECAPP_MBN_FILE,
            FALSE,
            &AppId);
        DEBUG((EFI_D_INFO, "LoadSecureApps: Load from filesystem result MSsecapp: 0x%p\n", Status));

        if (EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR, "MSsecapp failed to load! Status = 0x%x", Status));
            print_tz_diag();
        }
    }

    //send appid back for now; is this OK?
    if (AppIdPtr)
        *AppIdPtr = AppId;

ErrorExit:
    Status = EFI_SUCCESS;
    return Status;
}
