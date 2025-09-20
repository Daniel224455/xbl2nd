/**
* @file     UFS.c
* @brief   Universal Flash Storage (UFS) Dxe Driver
*
Copyright (c) 2013 - 2019, 2020-2021 Qualcomm Technologies, Inc. All rights reserved.
Copyright (c) 2004 - 2009, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
bbe
*/

/*=============================================================================
                              EDIT HISTORY


when            who   what, where, why
----------      ---   ----------------------------------------------------------- 
2021-04-13      jt    Initialize write booster parameters for provisioning
2019-07-02      jt    Turn off regulators if UFS is not the boot device
2019-03-04      jt    Add UFS 3.0 support
2018-12-12      jt    Add ability to provision secondary UFS  
2018-11-29      jt    Secondary UFS enablement for HLOS 
2018-08-16      jt    Register for Sleep callback to trigger UFS LPM 
2018-06-26      jt    Allow UFS SMMU config even for other boot devices
2018-06-21      jt    Support IOC enablement 
2018-06-04      jt    Turn off clocks if UFS is not the boot device  
2018-05-29      jt    Use recursive lock 
2018-05-11      jt    Add LPM callback   
2018-04-24      jt    Changes to RegisterListener  
2018-02-16      jt    Add HAL IOMMU configuration  
2017-02-01      jt    Use wrapper functions for BlkIo 
2016-11-30      jt    Return all entries in UFSGetLUWriteProtectInfo
2016-09-21      jt    Secure write protect changes   
2016-09-20      jt    Correct serial number length  
2016-06-28      jt    Add get/set boot LUN APIs 
2016-01-26      jt    Card info changes 
2016-01-21      rh    Adding BlockIO2 
2015-10-19      jt    Pass MediaId as argument for GPTListenerInit
2015-05-13      jb    Change device path type name to not conflict with standard one
2015-04-03      rm    Add check for boot device
2015-03-26      rm    Add RPMB protocols  
10-09-2014      rh    Adding RPMB APIs
09-23-2014      rh    Use unique GUID for LUs, different from eMMC
05-05-2013      ai    Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/ArmLib.h>
#include <Library/SerialPortShLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/StorSecApp.h>
#include <Library/UefiCfgLib.h>
#include <Library/QcomLib.h>
#include <Library/ParserLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/BlockIo2.h>
#include <Protocol/DevicePath.h>
#include <Protocol/EFICardInfo.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFIHWIO.h>

#include <Library/GPTListener.h>
#include <Library/RpmbListener.h>
#include <Library/RpmbLib.h>
#include <Protocol/EFIRpmb.h>
#include <Protocol/EFIEraseBlock.h>
#include <Protocol/EFIStorageWriteProtect.h>
#include <Protocol/EFIHALIOMMUProtocol.h>
#include <Protocol/EFIUfsProvision.h>
#include <Protocol/EFIAcpiPlatform.h>
#include <Library/KernelLib.h>

#include "UfsBlockIO2.h"
#include "UFS.h"
#include "Library/LockLib.h"
#include <api/storage/ufs_api.h>

#include <BootConfig.h>
#include <Library/HobLib.h>

#include <Library/QcomBaseLib.h>
#include <stdio.h>
#include <Lpm.h>
#include <Protocol/ReportStatusCodeHandler.h>

#define UFS_TPL TPL_CALLBACK

/* RPMB transfer limit */
#define RPMB_MAX_BLOCK_TRANSFER_SIZE 0xffff

VOID* UfsLockPtr;

/* UFS Device Path */
typedef struct {
   VENDOR_DEVICE_PATH  Ufs;
   EFI_DEVICE_PATH     End;
} QCUFS_DEVICE_PATH;

static QCUFS_DEVICE_PATH gUfsDevicePath [UFS_UEFI_MAX_LUN_ACCESS];
#ifdef ENABLE_AUTO_PLAT
static QCUFS_DEVICE_PATH gUfs1DevicePath [UFS_UEFI_MAX_LUN_ACCESS];
#endif

static VENDOR_DEVICE_PATH UfsPath = 
{ 
   HARDWARE_DEVICE_PATH,
   HW_VENDOR_DP,
   (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
   (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8),
   0 
};

static EFI_DEVICE_PATH UfsEndPath = 
{ 
   END_DEVICE_PATH_TYPE,
   END_ENTIRE_DEVICE_PATH_SUBTYPE,
   sizeof (EFI_DEVICE_PATH_PROTOCOL),
   0
}; 

#define _UFS_DEV_PROTOTYPE    { UFS_DEV_SIGNATURE, NULL, NULL, 0, 0, 0, 0, 0, 0}

static UFS_DEV gUfsDevice[UFS_UEFI_MAX_LUN_ACCESS] = {
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE
};

#ifdef ENABLE_AUTO_PLAT
static UFS_DEV g_Ufs1Device[UFS_UEFI_MAX_LUN_ACCESS] = {
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE,
   _UFS_DEV_PROTOTYPE
};
#endif

/* UFS Block IO's Media */
static EFI_BLOCK_IO_MEDIA UFSMedia = {
   SIGNATURE_32('u','f','s',' '),            /* MediaId */
   FALSE,                                    /* RemovableMedia */
   FALSE,                                    /* MediaPresent */
   FALSE,                                    /* LogicalPartition */
   FALSE,                                    /* ReadOnly */
   FALSE,                                    /* WriteCaching */
   4096,                                     /* BlockSize */
   8,                                        /* IoAlign */
   0,                                        /* LastBlock */
   0,                                        /* LowestAlignedLba */
   0,                                        /* LogicalBlocksPerPhysicalBlock */
   0                                         /* OptimalTransferLengthGranularity */  
}; 

static EFI_BLOCK_IO_MEDIA gMediaUfs[UFS_UEFI_MAX_LUN_ACCESS]; 

/* UFS Block IO Protocol */
EFI_BLOCK_IO_PROTOCOL gBlockIoUfs = {
   EFI_BLOCK_IO_PROTOCOL_REVISION3,          /* Revision */
   0,                                        /* Media */
   UFSReset,                                 /* Reset */
   UFSReadBlocksWrapper,                     /* ReadBlocks */
   UFSWriteBlocksWrapper,                    /* WriteBlocks */
   UFSFlushBlocks                            /* FlushBlocks */
};

/* UFS Block IO2 Protocol */
EFI_BLOCK_IO2_PROTOCOL gBlockIo2Ufs = {
   0,                                        /* Media */
   UFSResetEx,                               /* Reset */
   UFSReadBlocksEx,                          /* ReadBlocksEx */
   UFSWriteBlocksEx,                         /* WriteBlocksEx */
   UFSFlushBlocksEx                          /* FlushBlocksEx */
};

/* Device Paths for all the UFS LUNs */
static EFI_GUID UfsDevicePathGuids[UFS_UEFI_MAX_LUN_ACCESS] = {
   EFI_UFS_LUN_0_GUID,
   EFI_UFS_LUN_1_GUID,
   EFI_UFS_LUN_2_GUID,
   EFI_UFS_LUN_3_GUID,
   EFI_UFS_LUN_4_GUID,
   EFI_UFS_LUN_5_GUID,
   EFI_UFS_LUN_6_GUID,
   EFI_UFS_LUN_7_GUID
}; 

#ifdef ENABLE_AUTO_PLAT
/* Device Paths for all the UFS LUNs */
static EFI_GUID Ufs1DevicePathGuids[UFS_UEFI_MAX_LUN_ACCESS] = {
   EFI_UFS1_LUN_0_GUID,
   EFI_UFS1_LUN_1_GUID,
   EFI_UFS1_LUN_2_GUID,
   EFI_UFS1_LUN_3_GUID,
   EFI_UFS1_LUN_4_GUID,
   EFI_UFS1_LUN_5_GUID,
   EFI_UFS1_LUN_6_GUID,
   EFI_UFS1_LUN_7_GUID

};
#endif

/* RPMB Protocol */
static const EFI_SDCC_RPMB_PROTOCOL gUfsRpmbProtocol = {
   EFI_RPMB_PROTOCOL_REVISION,               /* Revision */
   0,                                        /* Reliable Write Count */
   0,                                        /* RPMB Partition Size */
   RPMBReadCounterPkt,                       /* Read Counter*/ 
   RPMBProgProvisionKey,                     /* Program Provision Key */
   RPMBReadBlocks,                           /* Read Sectors */
   RPMBWriteBlocks,                          /* Write Sectors */
   0                                         /* Max RMPB Read/Write Transfer Size */
};

/* UFS Card Info Protocol */
static EFI_MEM_CARDINFO_PROTOCOL gUfsCardInfoProtocol = {
   EFI_MEM_CARD_INFO_PROTOCOL_REVISION,      /* Revision */
   UFSGetCardInfo,                           /* GetCardInfo */
   UFSGetActiveBootLU,                       /* GetBootLU */
   UFSSetActiveBootLU                        /* SetBootLU */
};

/* Storage Write Protect Protocol */
static EFI_STORAGE_WP_PROTOCOL gUfsWriteProtectProtocol = {
   EFI_STORAGE_WP_PROTOCOL_REVISION,      /* Revision */
   UFSGetSupportedLbaWriteProtectTypes,   /* GetSupportedLbaWriteProtectTypes */                      
   UFSSetLUWriteProtect,                  /* SetLUWriteProtect */    
   UFSSetLbaWriteProtect,                 /* SetLbaWriteProtect */
   UFSClearWriteProtect,                  /* ClearWriteProtect */
   UFSGetLUWriteProtectInfo               /* GetLUWriteProtectInfo */              
};

/* Erase Blocks Protocol */
static const EFI_ERASE_BLOCK_PROTOCOL gUfsEraseBlkProtocol = {
   EFI_ERASE_BLOCK_PROTOCOL_REVISION,        /* Revision */
   4096,                                     /* Erase Granularity */
   UFSEraseBlocks                            /* Erase Blocks */
};

/* UFS provision Protocol */
static const EFI_UFS_PROV_PROTOCOL gUfsProvProtocol = {
   EFI_UFS_PROV_PROTOCOL_REVISION,        /* Revision */
   UFSProvisionHandler                    /* ProvisionUfs*/ 
};

static UFS_DEV gUfsRpmbDevice = _UFS_DEV_PROTOTYPE;
static UFS_DEV gUfsBootDevice = _UFS_DEV_PROTOTYPE; 
static UFS_DEV gUfsWlunDevice = _UFS_DEV_PROTOTYPE; 
static UFS_DEV gUfs1WlunDevice = _UFS_DEV_PROTOTYPE; 
static UFS_DEV gUfs1Device = _UFS_DEV_PROTOTYPE;
static QCUFS_DEVICE_PATH gUfsRpmbDevicePath;

/* SCM registration event */
static EFI_EVENT ScmInitialized = NULL; 
static VOID *ScmInitializedToken = NULL;
extern EFI_GUID gQcomScmProtocolGuid;

/* Exit Boot services event */
extern EFI_GUID gEfiEventExitBootServicesGuid;
static EFI_EVENT ExitBootServicesEvent = NULL; 

static EFI_RSC_HANDLER_PROTOCOL *RscHandlerProtocol = NULL; 

extern EFI_GUID gEfiHalIommuProtocolGuid; 
HAL_IOMMU_fptable_t *fp_table = NULL;

static EFI_QCOM_ACPIPLATFORM_PROTOCOL *AcpiPlatProtocol = NULL; 
static EFI_EVENT SecondaryUfsInit = NULL; 
static VOID *SecondaryUfsInitToken = NULL;
static EFI_EVENT SecondaryUfs30Init = NULL; 
static VOID *SecondaryUfs30InitToken = NULL;
static EFI_EVENT Ufs30Init = NULL; 
static VOID *Ufs30InitToken = NULL; 

#define DEFAULT_PROV_CFG_FILE  "UfsProvision.cfg"

/** Connection to blockIO2 **/
#define  REQ_LIST_SIZE  128
EFI_EVENT         gUfsBlkIo2TimerEvent;
BLKIO2_REQ_LIST   gUfsReqList;
DRIVER_INFO       gUfsTransferInfo;
EFI_EVENT         gUfsPurgeTimerEvent;
EFI_BLOCK_IO_PROTOCOL *gUfsCurrentErase;

VOID PurgeTimerEventNotify (
            IN  EFI_EVENT  Event,
            IN  VOID      *Context);

EFI_STATUS UFSIoWrapper (
            IN VOID                     *pThis,
            IN UINT32                    MediaId,
            IN EFI_LBA                   Lba,
            IN OUT EFI_BLOCK_IO2_TOKEN  *Token,
            IN UINTN                     BufferSize,
            IN OUT VOID                 *Buffer);

/**
   Reset the Block Device.
**/
EFI_STATUS EFIAPI UFSReset (
            IN EFI_BLOCK_IO_PROTOCOL   *This,
            IN BOOLEAN                 ExtendedVerification)
{
   return EFI_SUCCESS; 
}

/**
   Parse the UFS Provision cfg file.
**/ 
EFI_STATUS 
ParseUfsConfigParameters (INTN Pd, UINT8 *MaxCoreId,
                          struct ufs_lun_config_descr **UfsCfgParams)
{
   INT8 LUNum = -1; 
   struct ufs_lun_config_descr *UfsCoreCfg = NULL; 
   UINT8 *Section = NULL, *Key = NULL, *Value = NULL; 
   UINT8 CoreId = 0, CoreIdIndex = 0;

   if ((NULL == MaxCoreId) || (NULL == UfsCfgParams))
   {
      return EFI_INVALID_PARAMETER;
   }

   while (GetNextKeyValue (Pd, &Section, &Key, &Value) == 0) 
   {
      if (NULL == Key)
      {
         return EFI_INVALID_PARAMETER; 
      }

      if (AsciiStrnCmp((CHAR8*)Section, "UFS", 3) == 0) 
      {
         if ((NULL == UfsCfgParams) || (NULL == *UfsCfgParams))
         {
            return EFI_INVALID_PARAMETER;  
         }

         /* First 3 characters of section are "UFS". The 4th character will indicate
            the UFS instance */
         CoreId = AsciiStrDecimalToUintn((CHAR8 *)Section+3); 
         UfsCoreCfg = *UfsCfgParams + CoreId; 

         if (AsciiStriCmp ((CHAR8*)Key, "bNumberLU") == 0)
         {
            UfsCoreCfg->bNumberLU = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "bBootEnable") == 0)
         {
            UfsCoreCfg->bBootEnable = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "bDescrAccessEn") == 0)
         {
            UfsCoreCfg->bDescrAccessEn = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "bInitPowerMode") == 0)
         {
            UfsCoreCfg->bInitPowerMode = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "bHighPriorityLUN") == 0)
         {
            UfsCoreCfg->bHighPriorityLUN = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "bSecureRemovalType") == 0)
         {
            UfsCoreCfg->bSecureRemovalType = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "bInitActiveICCLevel") == 0)
         {
            UfsCoreCfg->bInitActiveICCLevel = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "wPeriodicRTCUpdate") == 0)
         {
            UfsCoreCfg->wPeriodicRTCUpdate = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "bConfigDescrLock") == 0)
         {
            UfsCoreCfg->bConfigDescrLock = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "LUNtoGrow") == 0)
         {
            UfsCoreCfg->lun_to_grow = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else if (AsciiStriCmp ((CHAR8*)Key, "LUNum") == 0)
         {
            LUNum = AsciiStrDecimalToUintn((CHAR8*)Value); 
         }
         else
         {
            /* The keys below correspond to LUN characteristics, so make sure we
               have a valid LUN number */
            if (LUNum == -1)
            {
               return EFI_INVALID_PARAMETER; 
            }

            if (AsciiStriCmp ((CHAR8*)Key, "bLUEnable") == 0)
            {
               UfsCoreCfg->unit[LUNum].bLUEnable = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
            else if (AsciiStriCmp ((CHAR8*)Key, "bBootLunID") == 0)
            {
               UfsCoreCfg->unit[LUNum].bBootLunID = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
            else if (AsciiStriCmp ((CHAR8*)Key, "bLUWriteProtect") == 0)
            {
               UfsCoreCfg->unit[LUNum].bLUWriteProtect = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
            else if (AsciiStriCmp ((CHAR8*)Key, "bMemoryType") == 0)
            {
               UfsCoreCfg->unit[LUNum].bMemoryType = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
            else if (AsciiStriCmp ((CHAR8*)Key, "qSizeInKb") == 0)
            {
               UfsCoreCfg->unit[LUNum].qSizeInKb = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
            else if (AsciiStriCmp ((CHAR8*)Key, "bDataReliability") == 0)
            {
               UfsCoreCfg->unit[LUNum].bDataReliability = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
            else if (AsciiStriCmp ((CHAR8*)Key, "bLogicalBlockSize") == 0)
            {
               UfsCoreCfg->unit[LUNum].bLogicalBlockSize = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
            else if (AsciiStriCmp ((CHAR8*)Key, "bProvisioningType") == 0)
            {
               UfsCoreCfg->unit[LUNum].bProvisioningType = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
            else if (AsciiStriCmp ((CHAR8*)Key, "wContextCapabilities") == 0)
            {
               UfsCoreCfg->unit[LUNum].wContextCapabilities = AsciiStrDecimalToUintn((CHAR8*)Value); 
            }
         }
      }
      else
      {
         if (AsciiStriCmp ((CHAR8*)Key, "MaxUfsInstancesForProvision") == 0)
         {
            *MaxCoreId = AsciiStrDecimalToUintn((CHAR8*)Value); 

            *UfsCfgParams = (struct ufs_lun_config_descr *)AllocateZeroPool(*MaxCoreId * sizeof(struct ufs_lun_config_descr)); 
            if (NULL == *UfsCfgParams)
            {
               return EFI_OUT_OF_RESOURCES;
            }

            /* Initialize Write Booster paramaters to 0xFF */
            for (CoreIdIndex = 0; CoreIdIndex < *MaxCoreId; CoreIdIndex++) 
            {
                UfsCoreCfg = *UfsCfgParams + CoreIdIndex;
                
                UfsCoreCfg->bWriteBoosterBufferType = 0xFF; 
                UfsCoreCfg->bWriteBoosterBufferPreserveUserSpaceEn = 0xFF; 
            }
         }
      }
   }

   return EFI_SUCCESS; 
}

/**
   Provisions the UFS device based on the settings in ConfigFileName.
**/
EFI_STATUS EFIAPI UFSProvisionHandler (
   IN EFI_UFS_PROV_PROTOCOL *This, 
   IN CONST CHAR8           *ConfigFileName
)
{
   CHAR16 ConfigFileNameUnicode[128] = {0};
   EFI_STATUS Status = EFI_SUCCESS;  
   INT32 rc;
   INTN Pd;  
   struct ufs_handle *hUFS = NULL;  
   struct ufs_lun_config_descr *UfsCfgParams = NULL; 
   UINT8 *CfgBuffer = NULL;
   UINT8 i = 0, MaxCoreId = 0; 
   UINTN FileSize = 0;  

   if (NULL == This)
   {
      return EFI_INVALID_PARAMETER;
   }

   if (0 == AsciiStriCmp (ConfigFileName, "default"))
   {
      AsciiStrToUnicodeStr(DEFAULT_PROV_CFG_FILE, ConfigFileNameUnicode);
   }
   else
   {
      AsciiStrToUnicodeStr(ConfigFileName, ConfigFileNameUnicode);
   }

   /* Read the config file */
   Status = ReadFromFV(ConfigFileNameUnicode, (void **) &CfgBuffer, &FileSize);
   if (EFI_SUCCESS != Status)
   {
      DEBUG((EFI_D_ERROR, "Failed to load UFS Provision cfg file, status 0x%x\n", Status));
      return Status;  
   }

   /* Open the parser for the config file */
   Pd = OpenParser (CfgBuffer, (UINT32)FileSize, NULL);
   if (Pd < 0)
   {
      DEBUG((EFI_D_ERROR, "UfsProvisionHandler: OpenParser failed\n"));
      return EFI_DEVICE_ERROR; 
   }

   /* Parse out the provisioning parameters */
   Status = ParseUfsConfigParameters (Pd, &MaxCoreId, &UfsCfgParams); 
   if (EFI_SUCCESS != Status)
   {
      DEBUG((EFI_D_ERROR, "UfsProvisionHandler: ParseUfsConfigParameters failed, status 0x%x\n", Status));
      goto ProvisionExit;   
   }

   if (NULL == UfsCfgParams) 
   {
      Status = EFI_INVALID_PARAMETER;
      goto ProvisionExit; 
   }

   /* Do the actual UFS provisioning */
   for (i = 0; i < MaxCoreId; i++)
   {
      if (UfsCfgParams[i].bNumberLU != 0)
      {
         hUFS = ufs_open(i, UFS_WLUN_DEVICE); 
         if (NULL == hUFS)
         {
            DEBUG((EFI_D_ERROR, "UfsProvisionHandler: ufs_open failed\n"));
            Status = EFI_DEVICE_ERROR; 
            goto ProvisionExit; 
         }

         rc = ufs_configure_device(hUFS, UfsCfgParams + i); 
         if (UFS_EOK != rc) {
            DEBUG ((EFI_D_ERROR, "ufs_configure_luns with return value of %d\n", rc));
            Status = EFI_DEVICE_ERROR;  
            goto ProvisionExit; 
         }
      }
   }

ProvisionExit: 
   CloseParser(Pd);
   return Status; 
}

/**
   Return supported write protect types
**/
EFI_STATUS EFIAPI UFSGetSupportedLbaWriteProtectTypes(
   IN EFI_STORAGE_WP_PROTOCOL *This, 
   OUT UINT32 *LbaWpTypes
)
{
   INT32 rc; 
   UFS_DEV *UfsDevice;
   struct ufs_handle *hUFS;
   struct ufs_info_type info;
   if ((NULL == This) || (NULL == LbaWpTypes)) 
   {
      return EFI_INVALID_PARAMETER; 
   }

   UfsDevice = UFS_DEV_FROM_WRITE_PROTECT (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;   

   *LbaWpTypes = WP_NOT_PROTECTED; 

   Lock (UfsLockPtr);
   /* Get device info to determine bLUWriteProtect value */
   rc = ufs_get_device_info (hUFS, &info);
   UnLock (UfsLockPtr);
   if (UFS_EOK != rc) 
   {
      DEBUG ((EFI_D_ERROR, "GetSupportedLbaWriteProtectTypes: Get device "
                           "info failed, status 0x%x\n", rc));
      return EFI_DEVICE_ERROR;
   }

   /* Set options based on bLUWriteProtect */
   if (2 == info.bLUWriteProtect)
   {
      *LbaWpTypes |= WP_PERMANENT; 
   }
   else if (1 == info.bLUWriteProtect) 
   {
      *LbaWpTypes |= WP_POWERON; 
   }
   else if (0 == info.bLUWriteProtect) 
   {
      *LbaWpTypes |= (WP_SECURE_NV | WP_SECURE_P | WP_SECURE_P_EN);
   }
   else
   {
      DEBUG ((EFI_D_ERROR, "GetSupportedLbaWriteProtectTypes: Invalid value "
                           "for bLUWriteProtect\n")); 
      return EFI_INVALID_PARAMETER; 
   }

   return EFI_SUCCESS; 
}

/**
   Set write protection for the entire LU
**/
EFI_STATUS EFIAPI UFSSetLUWriteProtect(
   IN EFI_STORAGE_WP_PROTOCOL *This
)
{
   INT32 rc; 
   UFS_DEV *UfsDevice;
   struct ufs_handle *hUFS;

   /* Validate passed-in pointers are not NULL */
   if (NULL == This)
   {
      return EFI_INVALID_PARAMETER;
   }

   UfsDevice = UFS_DEV_FROM_WRITE_PROTECT (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;       

   /* Set fPowerOnWPEn field in the flags */
   Lock (UfsLockPtr);
   rc = ufs_set_pon_write_protect(hUFS); 
   UnLock (UfsLockPtr);
   if (UFS_EOK != rc) 
   {
      DEBUG ((EFI_D_ERROR, "ufs_set_pon_write_protect with return value of %d\n", rc));
      return EFI_DEVICE_ERROR;
   }

   return EFI_SUCCESS; 
}

/**
   Get write protection information for the LU specified
**/
EFI_STATUS EFIAPI UFSGetLUWriteProtectInfoInternal(
   IN EFI_STORAGE_WP_PROTOCOL *This, 
   OUT EFI_WP_CONFIG *ProtectInfo
)
{
   EFI_STATUS Status; 
   INT32 rc; 
   SdMgrSecureWpInfo WpInfo; 
   UFS_DEV *UfsDevice;
   struct ufs_handle *hUFS;
   struct ufs_info_type info;
   UINT8 i = 0, index = 0; 

   if ((NULL == This) || (NULL == ProtectInfo))
   {
      return EFI_INVALID_PARAMETER; 
   }

   UfsDevice = UFS_DEV_FROM_WRITE_PROTECT (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;  

   /* Get device info to determine bLUWriteProtect value */
   rc = ufs_get_device_info (hUFS, &info);
   if (UFS_EOK != rc) 
   {
      DEBUG ((EFI_D_ERROR, "GetLbaWriteProtectType: Get device info failed, status 0x%x\n", rc));
      return EFI_DEVICE_ERROR;
   }

   /* If fPermanentWPEn = 1, then permanent write protection will be enabled for
      those LUNs where bLUWriteProtect = 2, regardless of LBA */
   if (2 == info.bLUWriteProtect)
   {
      if (info.fPermanentWPEn) 
      {
         ProtectInfo->NumEntries = 1;
         ProtectInfo->WpEntries[0].WpType = WP_PERMANENT; 
         ProtectInfo->WpEntries[0].Lba = 0; 
         ProtectInfo->WpEntries[0].NumBlocks = info.dLuTotalBlocks;  
         return EFI_SUCCESS; 
      }
   }
   /* If fPowerOnWPEn = 1, then power on write protection will be enabled for
      those LUNs where bLUWriteProtect = 1, regardless of LBA */
   else if (1 == info.bLUWriteProtect)
   {
      if (info.fPowerOnWPEn) 
      {
         ProtectInfo->NumEntries = 1;
         ProtectInfo->WpEntries[0].WpType = WP_POWERON;  
         ProtectInfo->WpEntries[0].Lba = 0; 
         ProtectInfo->WpEntries[0].NumBlocks = info.dLuTotalBlocks; 
         return EFI_SUCCESS; 
      }
   }
   /* If bLUWriteProtect = 0, then check to see if the LBA is part of a secure
      write protect configuration block */
   else if (0 == info.bLUWriteProtect) 
   {
      for (i = 0; i < UFS_UEFI_MAX_LUN_ACCESS; i++) 
      {
         if (UfsDevice == &gUfsDevice[i]) 
         {
            WpInfo.LunNumber = i; 
         }
      }

      ProtectInfo->NumEntries = 0; 

      /* Get Secure Write Protect Configuration Block */
      Status = UFSGetSecureWpConfigBlock(&WpInfo); 
      if (EFI_SUCCESS != Status)
      {
         DEBUG ((EFI_D_ERROR, "GetLbaWriteProtect: UFSGetSecureWpConfigBlockt failed "
                              "with return value of 0x%x\n", Status));
         return Status; 
      }

      for (i = 0; i < WpInfo.NumEntries; i++) 
      {
         ProtectInfo->NumEntries++; 
         ProtectInfo->WpEntries[index].Lba = WpInfo.WpEntries[i].Addr; 
         ProtectInfo->WpEntries[index].NumBlocks = WpInfo.WpEntries[i].NumBlocks; 

         /* NV-type when WPT = 0 */
         if (0 == WpInfo.WpEntries[i].WpTypeMask) 
         {
            ProtectInfo->WpEntries[index].WpType = WP_SECURE_NV; 
         }
         /* P-type when WPT = 1 */
         else if (1 == WpInfo.WpEntries[i].WpTypeMask) 
         {
            ProtectInfo->WpEntries[index].WpType = WP_SECURE_P; 
         }
         /* NV-AWP-type when WPT = 2 */
         else if (2 == WpInfo.WpEntries[i].WpTypeMask) 
         {
            ProtectInfo->WpEntries[index].WpType = WP_SECURE_P_EN;
         }

         index++; 
      }
   }
   else
   {
      DEBUG ((EFI_D_ERROR, "GetLbaWriteProtect: Invalid bLUWriteProtect "
                           "value 0x%x\n", info.bLUWriteProtect));  
      return EFI_INVALID_PARAMETER; 
   }
 
   return EFI_SUCCESS;  
}

EFI_STATUS EFIAPI UFSGetLUWriteProtectInfo(
   IN EFI_STORAGE_WP_PROTOCOL *This, 
   OUT EFI_WP_CONFIG *ProtectInfo
)
{
  EFI_STATUS Status;

  Lock (UfsLockPtr);
  Status = UFSGetLUWriteProtectInfoInternal (This, ProtectInfo);
  UnLock (UfsLockPtr);

  return Status;
}
/**
   Set write protection for the LBA ranges
**/
EFI_STATUS EFIAPI UFSSetLbaWriteProtect(
   IN EFI_STORAGE_WP_PROTOCOL *This,      
   IN EFI_WP_CONFIG *WpConfig
)
{
   EFI_STATUS Status; 
   INT32 rc; 
   SdMgrSecureWpInfo WpInfo;
   UFS_DEV *UfsDevice; 
   struct ufs_handle *hUFS;
   struct ufs_info_type info;
   UINT8 i;

   if ((NULL == This) || (NULL == WpConfig)) 
   {
      return EFI_DEVICE_ERROR; 
   }

   UfsDevice = UFS_DEV_FROM_WRITE_PROTECT (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;

   /* Get device info to determine bLUWriteProtect value */
   Lock (UfsLockPtr);
   rc = ufs_get_device_info (hUFS, &info);
   UnLock (UfsLockPtr);
   if (UFS_EOK != rc) 
   {
      DEBUG ((EFI_D_ERROR, "SetLbaWriteProtect: Get device info failed, status 0x%x\n", rc));
      return EFI_DEVICE_ERROR;
   }
 
   if (0 != info.bLUWriteProtect)
   {
      DEBUG ((EFI_D_ERROR, "LBA write protect only allowed for bLUWriteProtect = 0\n", rc));
      return EFI_INVALID_PARAMETER; 
   }

   for (i = 0; i < UFS_UEFI_MAX_LUN_ACCESS; i++) 
   {
      if (UfsDevice == &gUfsDevice[i]) 
      {
         WpInfo.LunNumber = i; 
      }
   }
 
   Status = UFSGetSecureWpConfigBlock(&WpInfo);
   if (EFI_SUCCESS != Status) 
   {
      DEBUG ((EFI_D_ERROR, "UFSSetLbaWriteProtect: UFSGetSecureWpConfigBlock failed "
                           "with return value of 0x%x\n", Status));
      return Status; 
   }

   /* Once the configuration block is updated, we won't have NumEntries = 0.
      If we see that NumEntries = 1 and the first entry disabled, then
      we know that the ClearWriteProtect has happened and so we essentially
      have NumEntries = 0 */
   if ((WpInfo.NumEntries == 1) && (WpInfo.WpEntries[0].WpEnable == 0))
   {
      WpInfo.NumEntries = 0; 
   }

   if ((MAX_SECURE_WP_ENTRIES - WpInfo.NumEntries) < WpConfig->NumEntries) 
   {
      DEBUG ((EFI_D_ERROR, "Number of configuration entries too large\n"));
      return EFI_INVALID_PARAMETER; 
   }

   /* Set up the configuration structure */
   for (i = WpInfo.NumEntries; i < (WpInfo.NumEntries + WpConfig->NumEntries); i++) 
   {
      WpInfo.WpEntries[i].WpEnable = 1; 

      if (WP_SECURE_NV == WpConfig->WpEntries[i-WpInfo.NumEntries].WpType) 
      {
         WpInfo.WpEntries[i].WpTypeMask = 0; 
      }
      else if (WP_SECURE_P == WpConfig->WpEntries[i-WpInfo.NumEntries].WpType)  
      {
         WpInfo.WpEntries[i].WpTypeMask = 1; 
      }
      else if (WP_SECURE_P_EN == WpConfig->WpEntries[i-WpInfo.NumEntries].WpType)  
      {
         WpInfo.WpEntries[i].WpTypeMask = 2; 
      }
      else
      {
         DEBUG ((EFI_D_ERROR, "UFSSetLbaWriteProtect: Invalid WpType"));
         return EFI_INVALID_PARAMETER;
      }

      WpInfo.WpEntries[i].Addr = WpConfig->WpEntries[i-WpInfo.NumEntries].Lba;
      WpInfo.WpEntries[i].NumBlocks = WpConfig->WpEntries[i-WpInfo.NumEntries].NumBlocks;

   }

   WpInfo.NumEntries += WpConfig->NumEntries;

   /* Send request to secure app */
   Status = UFSSetSecureWpConfigBlock(&WpInfo);   
   if (EFI_SUCCESS != Status) 
   {
      DEBUG ((EFI_D_ERROR, "UFSSetLbaWriteProtect: UFSSetSecureWpConfigBlock failed "
                           "with return value of 0x%x\n", Status));
      return Status; 
   }

   return EFI_SUCCESS; 
}

/**
   Clear secure write protection entries for the LU specified
**/
EFI_STATUS EFIAPI UFSClearWriteProtectInternal(
   IN EFI_STORAGE_WP_PROTOCOL *This
)
{
   EFI_STATUS Status;
   INT32 rc; 
   SdMgrSecureWpInfo WpInfo; 
   UFS_DEV *UfsDevice; 
   struct ufs_handle *hUFS;
   struct ufs_info_type info;
   UINT8 i = 0; 

   if (NULL == This) 
   {
      return EFI_INVALID_PARAMETER; 
   }

   UfsDevice = UFS_DEV_FROM_WRITE_PROTECT (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;

   /* Get device info to determine bLUWriteProtect value */
   rc = ufs_get_device_info (hUFS, &info);
   if (UFS_EOK != rc) 
   {
      DEBUG ((EFI_D_ERROR, "ClearWriteProtect: Get device info failed, status 0x%x\n", rc));
      return EFI_DEVICE_ERROR;
   }
 
   if (0 != info.bLUWriteProtect)
   {
      DEBUG ((EFI_D_ERROR, "Clear write protect only allowed for bLUWriteProtect = 0\n"));
      return EFI_INVALID_PARAMETER; 
   }

   for (i = 0; i < UFS_UEFI_MAX_LUN_ACCESS; i++) 
   {
      if (UfsDevice == &gUfsDevice[i]) 
      {
         WpInfo.LunNumber = i; 
      }
   }

   /* Get Secure Write Protect Configuration Block */
   Status = UFSGetSecureWpConfigBlock(&WpInfo); 
   if (EFI_SUCCESS != Status)
   {
      DEBUG ((EFI_D_ERROR, "ClearWriteProtect: UFSGetSecureWpConfigBlock failed "
                           "with return value of 0x%x\n", Status));
      return Status; 
   }

   /* NumEntries > 0, so we clear the entries, but set NumEntries = 1 */
   SetMem(WpInfo.WpEntries, MAX_SECURE_WP_ENTRIES * sizeof(SdMgrSecureWpInfoEntry), 0x00);
   WpInfo.NumEntries = 0x1; 

   /* Update the Secure Write Protect Configuration Block */
   Status = UFSSetSecureWpConfigBlock(&WpInfo);
   if (EFI_SUCCESS != Status)
   {
      DEBUG ((EFI_D_ERROR, "ClearWriteProtect: UFSSetSecureWpConfigBlock failed "
                           "with return value of 0x%x\n", Status));
      return Status; 
   }

   return EFI_SUCCESS; 
}

EFI_STATUS EFIAPI UFSClearWriteProtect(
   IN EFI_STORAGE_WP_PROTOCOL *This
)
{
  EFI_STATUS Status;

  Lock (UfsLockPtr);
  Status = UFSClearWriteProtectInternal (This);
  UnLock (UfsLockPtr);

  return Status;
}
/**
   Read BufferSize bytes from Lba into Buffer. 
**/
EFI_STATUS EFIAPI UFSReadBlocks (
            IN EFI_BLOCK_IO_PROTOCOL   *This,
            IN UINT32                  MediaId,
            IN EFI_LBA                 Lba,
            IN UINTN                   BufferSize,
            OUT VOID                   *Buffer)
{
   INT32  rc; 
   UFS_DEV    *UfsDevice;
   struct ufs_handle *hUFS;
   UINT32 transfer_size = 0, block_count = 0, block_size = 0;
   UINT8 *temp_buffer;

   /* Validate passed-in pointers are not NULL */
   if ((NULL == Buffer) || (NULL == This) || (NULL == This->Media)) {
      return EFI_INVALID_PARAMETER;
   }

   /* Validate if buffer is multiple of block size */
   if ((BufferSize%(This->Media->BlockSize)) != 0 ) {
      return EFI_BAD_BUFFER_SIZE;
   }

   /* Validate block size and access range */
   if ((Lba > This->Media->LastBlock) || (This->Media->BlockSize == 0)) {
      return EFI_INVALID_PARAMETER;
   }
   
   if (This->Media->MediaId != MediaId) {
      return EFI_MEDIA_CHANGED;
   }

   UfsDevice = UFS_DEV_FROM_BLOCKIO (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;       
   
   /* If BufferSize is zero, the host is pinging the card for its presence */
   if (0 == BufferSize)
   {
      if (!hUFS)
      {
         (void) ufs_close(hUFS);       
         UfsDevice->DeviceHandle = NULL;   
         UfsDevice->BlkIo.Media->MediaPresent = FALSE;        
         return EFI_NO_MEDIA;
      }
      else
      {
         return EFI_SUCCESS;
      }
   }

   block_size = UfsDevice->BlkIo.Media->BlockSize;
   block_count = (BufferSize/block_size);

   /* Check if LBA plus the total sectors trying to access would exceed the */
   /* total size of the partition */
   if ((Lba + (BufferSize/block_size)) > (UfsDevice->BlkIo.Media->LastBlock + 1)) {
      return EFI_INVALID_PARAMETER;
   }

   /* Break transfer into smaller transfers to UFS_MAX_BLOCK_TRANSFER_SIZE transfer size */
   temp_buffer = (UINT8 *)Buffer;

   while (block_count > 0)
   {
      transfer_size = (block_count > UFS_MAX_BLOCK_TRANSFERS) ? 
         UFS_MAX_BLOCK_TRANSFERS : block_count;

      rc = ufs_read( hUFS, temp_buffer, Lba, transfer_size);

      Lba = Lba + transfer_size;
      block_count = block_count - transfer_size;
      temp_buffer = temp_buffer + (transfer_size * block_size);
   
      if (UFS_EOK != rc) {
         DEBUG ((EFI_D_ERROR, "UFS READ with return value of %d\n", rc));
         return EFI_DEVICE_ERROR;
      }
   }
   
   return EFI_SUCCESS;
}

/**
   Write BufferSize bytes from Lba into Buffer. 
**/
EFI_STATUS EFIAPI UFSWriteBlocks (
            IN EFI_BLOCK_IO_PROTOCOL   *This,
            IN UINT32                  MediaId,
            IN EFI_LBA                 Lba,
            IN UINTN                   BufferSize,
            IN VOID                    *Buffer)
{
   INT32  rc;   
   UFS_DEV    *UfsDevice;
   struct ufs_handle *hUFS;
   UINT32 transfer_size = 0, block_count = 0, block_size = 0;
   UINT8 *temp_buffer;

   /* Validate passed-in pointers are not NULL */
   if ((NULL == Buffer) || (NULL == This) || (NULL == This->Media)) {
      return EFI_INVALID_PARAMETER;
   }

   /* Validate if buffer is multiple of block size */   
   if (((BufferSize%(This->Media->BlockSize)) != 0)) {
      return EFI_BAD_BUFFER_SIZE;
   }

   /* Validate block size and access range */
   if ((Lba > This->Media->LastBlock) || (This->Media->BlockSize == 0)) {
      return EFI_INVALID_PARAMETER;
   }
   
   /* Validate if LBA plus the total sectors trying to access would exceed the */
   /* total size of the partition */
   if ((Lba + (BufferSize/This->Media->BlockSize)) > (This->Media->LastBlock + 1)) {
      return EFI_INVALID_PARAMETER;
   }
     
   if (This->Media->MediaId != MediaId) {
      return EFI_MEDIA_CHANGED;
   }

   UfsDevice = UFS_DEV_FROM_BLOCKIO (This);
   hUFS = UfsDevice->DeviceHandle;
   
   if (NULL == hUFS) {
      return EFI_INVALID_PARAMETER;
   }
  
   /* Break transfer into smaller transfers to UFS_MAX_BLOCK_TRANSFER_SIZE transfer size */
   temp_buffer = (UINT8 *)Buffer;
   block_size = UfsDevice->BlkIo.Media->BlockSize;
   block_count = (BufferSize/block_size);

   while (block_count > 0)
   {
     transfer_size = (block_count > UFS_MAX_BLOCK_TRANSFERS) ? 
                         UFS_MAX_BLOCK_TRANSFERS : block_count;

     rc = ufs_write( hUFS, temp_buffer, Lba, transfer_size);

     Lba = Lba + transfer_size;
     block_count = block_count - transfer_size;
     temp_buffer = temp_buffer + (transfer_size * block_size);
   
     if (UFS_EOK != rc) {
        DEBUG ((EFI_D_ERROR, "UFS WRITE with return value of %d\n", rc));
        return EFI_DEVICE_ERROR;
     }
   }
   return EFI_SUCCESS;
}


/**
   Flush the Block Device.   
**/
EFI_STATUS EFIAPI UFSFlushBlocks (
            IN EFI_BLOCK_IO_PROTOCOL  *This)
{
   return EFI_SUCCESS;
}

VOID EFIAPI RegisterListener (
            IN EFI_EVENT        Event,
            IN VOID             *Context)
{
   EFI_STATUS EfiStatus = EFI_UNSUPPORTED; 
   EFI_BLOCK_IO_PROTOCOL *BlkIo = (EFI_BLOCK_IO_PROTOCOL *)Context; 

   if ((NULL == BlkIo) || (NULL == BlkIo->Media))
   {
      return; 
   }

   EfiStatus = RPMBListenerInit(BlkIo->Media->MediaId); 
   if (EFI_SUCCESS != EfiStatus)
   {
      DEBUG ((EFI_D_ERROR, "Failed to initialize RPMB Listener, Status 0x%08x\n", EfiStatus));
   }

   EfiStatus = GPTListenerInit(BlkIo); 
   if (EFI_SUCCESS != EfiStatus)
   {
      DEBUG ((EFI_D_ERROR, "Failed to initialize GPT Listener, Status 0x%08x\n", EfiStatus));
   }

   EfiStatus = InitPartitionConfig();
   if (EFI_SUCCESS != EfiStatus)
   {
      DEBUG ((EFI_D_ERROR, "Failed to initialize Partition config, Status 0x%08x\n", EfiStatus));
   }  
}

EFI_STATUS EFIAPI UFSSmmuConfig ()
{
   EFI_STATUS Status = EFI_SUCCESS; 
   VOID *ufs_domain_hdl;
   EFI_HAL_IOMMU_PROTOCOL *Iommu = NULL; 
   enum hal_iommu_bypass_domain_type IommuType = HAL_IOMMU_AARCH64_NON_CCA_DOMAIN_TYPE; 

   /* Locate UEFI IOMMU protocol */
   if (fp_table == NULL)
   {
      Status = gBS->LocateProtocol(&gEfiHalIommuProtocolGuid,
                                   NULL,
                                   (VOID **)&Iommu); 
      if (Status != EFI_SUCCESS)
      {
         DEBUG((EFI_D_ERROR, "UFS IOMMU LocateProtocol failed 0x%x\n", Status));
         return Status;
      }

      Iommu->_HAL_IOMMU_protocol_init(&fp_table);
   }

   /* Create Domain for UFS */
   if (HAL_IOMMU_ERR_OK != fp_table->domain_create_func(&ufs_domain_hdl)) 
   {
      DEBUG((EFI_D_ERROR, "UFS IOMMU domain create failed\n"));
      return EFI_NOT_STARTED;
   }

   Status = GetConfigValue ("EnableUfsIOC", &IommuType);
   if (EFI_SUCCESS != Status)
   {
      IommuType = HAL_IOMMU_AARCH64_NON_CCA_DOMAIN_TYPE; 
   }

   if (HAL_IOMMU_ERR_OK != fp_table->config_bypass_domain_func(ufs_domain_hdl, IommuType)) 
   {
      DEBUG((EFI_D_ERROR, "UFS IOMMU domain configure failed\n"));
      return EFI_UNSUPPORTED;
   }

   /* Attach UFS device to the domain */
   if (HAL_IOMMU_ERR_OK != fp_table->domain_attach_func(ufs_domain_hdl, (void *)"\\_SB_.UFS0", 0x0, 0x0))
   {
      DEBUG((EFI_D_ERROR, "UFS IOMMU domain attach ARID 0x0 failed\n"));
      return EFI_DEVICE_ERROR;
   }

   if (HAL_IOMMU_ERR_OK != fp_table->domain_attach_func(ufs_domain_hdl, (void *)"\\_SB_.UFS0", 0x10, 0x0))
   {
      DEBUG((EFI_D_ERROR, "UFS IOMMU domain attach ARID 0x10 failed\n"));
      return EFI_DEVICE_ERROR;
   }

   if (HAL_IOMMU_ERR_OK != fp_table->domain_attach_func(ufs_domain_hdl, (void *)"\\_SB_.UFS0", 0x20, 0x0))
   {
      DEBUG((EFI_D_ERROR, "UFS IOMMU domain attach ARID 0x20 failed\n"));
      return EFI_DEVICE_ERROR;
   }

   return EFI_SUCCESS; 
}

/**
   Update ACPI AML variable to enable update UFS features
**/
EFI_STATUS EFIAPI AmlUpdateUFSFeatures (
   IN OUT VOID **AmlVariableBuffer, 
   IN OUT UINTN  AmlVariableBufferSize
   )
{
   if ((NULL == AmlVariableBuffer) || (NULL == *AmlVariableBuffer))
   {
      DEBUG ((EFI_D_WARN, "AmlUpdateUFSFeatures: Could not find the AML variable in DSDT\r\n"));
      return EFI_SUCCESS; 
   }

   *((UINTN*) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = 0x1; 

   return EFI_SUCCESS; 
}

VOID EFIAPI RegisterAmlVariable (
   IN EFI_EVENT        Event,
   IN VOID             *Context)
{
   CHAR8 AmlVariableSecondaryUFS[AML_NAMESTRING_LENGTH] =  {'S','U','F','S'}; 
   CHAR8 AmlVariableUFS30[AML_NAMESTRING_LENGTH] = {'P','U','S','3'}; 
   CHAR8 AmlVariablesSecondaryUFS30[AML_NAMESTRING_LENGTH] = {'S', 'U', 'S', '3'}; 
   CHAR8 *AmlVariable = NULL; 
   EFI_STATUS Status = EFI_UNSUPPORTED; 

   if (NULL == AcpiPlatProtocol)
   {
      Status = gBS->LocateProtocol(&gQcomAcpiPlatformProtocolGuid, NULL, 
                                (VOID **)&AcpiPlatProtocol); 
      ASSERT_EFI_ERROR(Status); 
   }

   if (Event == SecondaryUfsInit)
   {
      AmlVariable = AmlVariableSecondaryUFS;
   }
   else if (Event == Ufs30Init)
   {
      AmlVariable = AmlVariableUFS30;
   }
   else if (Event == SecondaryUfs30Init)
   {
      AmlVariable = AmlVariablesSecondaryUFS30;
   }

   Status = AcpiPlatProtocol->AmlVariableRegister(AcpiPlatProtocol, AmlVariable, GETMODE_PREFIX, 
                                                  NULL, AmlUpdateUFSFeatures); 
   ASSERT_EFI_ERROR(Status); 
}

volatile UINT32 LastPwrEvt = 0;
static void UFSSleepCb (enum PwrTxnType Evt, VOID* Arg)
{
  struct ufs_handle *hUFS = (struct ufs_handle *)Arg;
  
   INT32 rc = 0; 

   LastPwrEvt = Evt;

   switch(Evt)
   {
      case DevicePwrOFF:
         rc = ufs_clock_gate (hUFS); 
         if (UFS_EOK != rc) {
            // WARNING: NO Debug logs during sleep transitions...!!
            // DEBUG ((EFI_D_ERROR, "ufs_clock_gate with return value of %d\n", rc));
         }
         break;

      case DevicePwrON:
         rc = ufs_clock_ungate (hUFS); 
         if (UFS_EOK != rc) {
            // WARNING: NO Debug logs during sleep transitions...!!
            // DEBUG ((EFI_D_ERROR, "ufs_clock_ungate with return value of %d\n", rc));
         }
         break;

      default:
         break;
   }
}

EFI_STATUS EFIAPI UfsLpmCb (
   IN EFI_STATUS_CODE_TYPE     CodeType,
   IN EFI_STATUS_CODE_VALUE    Value,
   IN UINT32                   Instance,
   IN EFI_GUID                 *CallerId,
   IN EFI_STATUS_CODE_DATA     *Data OPTIONAL)
{
   EFI_STATUS Status = EFI_SUCCESS; 
   LPM_EVENT_EXT_DATA *LpmEventData = NULL;

   if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) &&
       (Value == PcdGet32 (PcdLpm)) && (Data != NULL))
   {
      /* Map LpmEventData */
      LpmEventData = (LPM_EVENT_EXT_DATA*)(Data + 1);

      if(TRUE == CompareGuid(&(LpmEventData->CalleeGuid), &gEfiUfsLU0Guid))
      {
         switch (LpmEventData->LPMState)
         {
            case LPM_ENTRY:
               Status = ufs_autoH8(gUfsDevice[0].DeviceHandle);
               break;
            default:
               break;
         }

         if (Status != EFI_SUCCESS)
         {
            DEBUG(( EFI_D_ERROR, "UfsLpmCb: LPM state = %d, Status 0x%x\r\n", LpmEventData->LPMState, Status));
         }
      }
   }

   return Status; 
}

VOID
EFIAPI
UfsDxeExitBSCallback (
   IN EFI_EVENT        Event,
   IN VOID             *Context
  )
{
   EFI_STATUS Status = EFI_SUCCESS; 

   /* Unregister LPM callback in Exit Boot Services */
   if (NULL != RscHandlerProtocol)
   {
      Status = RscHandlerProtocol->Unregister(UfsLpmCb); 
      if (EFI_SUCCESS != Status)
      {
         DEBUG ((EFI_D_ERROR, "Unregister UfsLpmCb failed, status 0x%x\n", Status));
      }
   }
}

/**
   Driver initialization entry point.
**/
EFI_STATUS EFIAPI UFSDxeInitialize (
            IN EFI_HANDLE         ImageHandle,
            IN EFI_SYSTEM_TABLE   *SystemTable)
{
   EFI_STATUS Status;
   UINT32 lun = 0;
   UINT32 lun_enabled_mask = 0;
   UINT8  total_luns_enabled = 0;
   VOID *ScmProtocol = NULL;
   INT32 rc = 0;
   struct ufs_info_type ufs_info;

   UINT32 AlwaysDoSmmuConfig = 0; 
   UINT32 SecondaryUFSInit = 0; 
#ifdef ENABLE_AUTO_PLAT
   UINT32 EnableSecondaryUFS = 0;
#endif
   UfsLockPtr = InitRecursiveLock ("UFS_DRVR");

   /* Don't do anything if it doesn't boot from UFS */   
   if (!boot_from_ufs ()) {
      /* Turn off the UFS clocks */
      ufs_clk_ctrl (0, UFS_CLK_ON); 
      ufs_clk_ctrl (0, UFS_CLK_OFF);

      /* Turn off the UFS regulators */
      ufs_volt_ctrl (0, UFS_VOLT_OFF); 

      /* Configure SMMU if HLOS needs it to detect UFS */
      Status = GetConfigValue ("UfsSmmuConfigForOtherBootDev", &AlwaysDoSmmuConfig);
      if (EFI_SUCCESS != Status)
      {
         AlwaysDoSmmuConfig = 0; 
      }

      if (AlwaysDoSmmuConfig)
      {
         Status = UFSSmmuConfig(); 
         if (EFI_SUCCESS != Status)
         {
            DEBUG ((EFI_D_ERROR, "UFSSmmuConfig failed, status 0x%x\n", Status));
            return Status; 
         }
      }

      return EFI_SUCCESS;
   }
   
// TODO: Remove Timer Profiling Code
#if 0
   {
      Timetick_timer_Type Timer;
      UINT32 call_count = 0;
      UINT64 starting_tick = 0, ending_tick = 0, total_ticks = 0;       
      UINT32 TimetickFreq = 0;
      UINT64 uSeconds = 0;
 
      if(PcdGet32(PcdQTimerEnabled))
      {
        Timer = TIMETICK_QTIMER;
      }
      else
      {
        Timer = TIMETICK_TIMER_ADGT;
      }
 
      /* Enable TimeTick for collecting performance data */
      Timetick_Init();
      Timetick_Enable(Timer, TRUE);
      
      Timetick_GetCount(Timer, &starting_tick);
      //Delay of 5s
      for (call_count=0; call_count<5000; call_count++)
      {
        ufs_osal_stall_us(1);
      }       
      Timetick_GetCount(Timer, &ending_tick);
      total_ticks = ending_tick - starting_tick;

      /* Convert ticks to micro seconds */
      Timetick_GetFreq(Timer, &TimetickFreq);
      uSeconds = MultU64x32(total_ticks, 1000000);
      uSeconds = DivU64x32(uSeconds, TimetickFreq);  
      DEBUG ((EFI_D_ERROR, "uSeconds = %ld\n", uSeconds));
   }
#endif

   Status = UFSSmmuConfig(); 
   if (EFI_SUCCESS != Status)
   {
      DEBUG ((EFI_D_ERROR, "UFSSmmuConfig failed, status 0x%x\n", Status));
      return Status; 
   }

   /* Initialize UFS */
   gUfsWlunDevice.DeviceHandle = ufs_open (0, UFS_WLUN_DEVICE);
   if (!gUfsWlunDevice.DeviceHandle)
      ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);

   /* Get valid LUNs */
   rc = ufs_get_device_info ((struct ufs_handle *)gUfsWlunDevice.DeviceHandle, &ufs_info); 
   if (UFS_EOK != rc)
      ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
   lun_enabled_mask = ufs_info.dLunEnabled;
   total_luns_enabled = ufs_info.bNumberLu;
  
   for (lun = 0; lun < total_luns_enabled; lun++)
   { 
      // Check if LUN is enabled
      if (!(lun_enabled_mask & ((UINT32) 1) << lun))
         continue;

      gUfsDevicePath[lun].Ufs = UfsPath; 
      gUfsDevicePath[lun].Ufs.Guid = UfsDevicePathGuids[lun]; 
      gUfsDevicePath[lun].End = UfsEndPath;
      
      gUfsDevice[lun].Signature    = UFS_DEV_SIGNATURE;
      gUfsDevice[lun].BlkIo        = gBlockIoUfs;
      gUfsDevice[lun].BlkIo.Media  = &gMediaUfs[lun];
      gUfsDevice[lun].BlkIo2        = gBlockIo2Ufs;
      gUfsDevice[lun].BlkIo2.Media  = &gMediaUfs[lun];
      gUfsDevice[lun].CardInfo     = gUfsCardInfoProtocol;
      gUfsDevice[lun].EraseBlk     = gUfsEraseBlkProtocol;
      CopyMem(gUfsDevice[lun].BlkIo.Media, 
                 &UFSMedia, sizeof(EFI_BLOCK_IO_MEDIA));  
      gUfsDevice[lun].WriteProtect = gUfsWriteProtectProtocol;

      /* Initialize LUN */
      gUfsDevice[lun].DeviceHandle = ufs_open (0, lun);
      if (!gUfsDevice[lun].DeviceHandle) {
         gUfsDevice[lun].BlkIo.Media->MediaPresent = FALSE;
         continue;
      }
      gUfsDevice[lun].BlkIo.Media->MediaPresent = TRUE;
      rc = ufs_get_device_info ((struct ufs_handle *)gUfsDevice[lun].DeviceHandle, &ufs_info); 
      if (UFS_EOK != rc)
         ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
      gUfsDevice[lun].BlkIo.Media->LastBlock = ufs_info.dLuTotalBlocks - 1;

      /* Assume LUN0 and install the following protocols: */ 
      /* BlkIO */
      Status = gBS->InstallMultipleProtocolInterfaces (
         &gUfsDevice[lun].ClientHandle,  
         &gEfiBlockIoProtocolGuid, &gUfsDevice[lun].BlkIo, 
         &gEfiDevicePathProtocolGuid, &gUfsDevicePath[lun],
         &gEfiMemCardInfoProtocolGuid, &gUfsDevice[lun].CardInfo,
         &gEfiBlockIo2ProtocolGuid, &gUfsDevice[lun].BlkIo2,
         &gEfiEraseBlockProtocolGuid, &gUfsDevice[lun].EraseBlk,
         &gEfiStorageWpProtocolGuid, &gUfsDevice[lun].WriteProtect, 
         NULL
      );
      ASSERT_EFI_ERROR(Status);
   }

   if (ufs_info.wSpecVersion >= 0x300) {
      /* Create the RegisterAmlVariable callback */
      Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL,
                                TPL_CALLBACK,
                                RegisterAmlVariable,
                                NULL, 
                                &Ufs30Init);
      ASSERT_EFI_ERROR (Status); 
   
      Status = gBS->RegisterProtocolNotify(&gQcomAcpiPlatformProtocolGuid,
                                           Ufs30Init, 
                                           (VOID *)&Ufs30InitToken); 
      ASSERT_EFI_ERROR(Status);
   }
   
   gUfsRpmbDevicePath.Ufs = UfsPath; 
   gUfsRpmbDevicePath.Ufs.Guid = (EFI_GUID)EFI_UFS_RPMB_LUN_GUID;
   gUfsRpmbDevicePath.End = UfsEndPath;
   
   gUfsRpmbDevice.RPMB = gUfsRpmbProtocol;
   gUfsRpmbDevice.CardInfo = gUfsCardInfoProtocol;
  
   gUfsRpmbDevice.DeviceHandle = ufs_open (0, UFS_WLUN_RPMB);
   if (!gUfsRpmbDevice.DeviceHandle) {
      DEBUG ((EFI_D_ERROR, "Cannot open UFS RPMB!\n"));
   }   
   rc = ufs_get_device_info ((struct ufs_handle *)gUfsRpmbDevice.DeviceHandle, &ufs_info); 
   if (UFS_EOK != rc)  ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
   
   gUfsRpmbDevice.RPMB.RPMBMaxTransferSize = ufs_info.qLogicalBlockCount * 256;
   gUfsRpmbDevice.RPMB.RPMBPartitionSizeInBytes = ufs_info.qLogicalBlockCount * 256;
   gUfsRpmbDevice.RPMB.ReliableWriteCount = ufs_info.bRPMB_ReadWriteSize;
   
   Status = gBS->InstallMultipleProtocolInterfaces (
      &gUfsRpmbDevice.ClientHandle,  
      &gEfiSdccRpmbProtocolGuid, &gUfsRpmbDevice.RPMB,
      &gEfiDevicePathProtocolGuid, &gUfsRpmbDevicePath,
      &gEfiMemCardInfoProtocolGuid, &gUfsRpmbDevice.CardInfo,
      NULL
   );  

#ifdef ENABLE_AUTO_PLAT
   /* Check if we need to do the Full secondary UFS init */
   Status = GetConfigValue ("EnableSecondaryUFS", &EnableSecondaryUFS);

   if ((Status == EFI_SUCCESS) && (EnableSecondaryUFS == 1)) {
     /* Initialize Secondary UFS */
     gUfs1WlunDevice.DeviceHandle = ufs_open (1, UFS_WLUN_DEVICE);
     if (!gUfs1WlunDevice.DeviceHandle) {
	    DEBUG ((EFI_D_ERROR, "Cannot open Secondary UFS!\n"));
        ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
     }

     /* Get valid LUNs */
     rc = ufs_get_device_info ((struct ufs_handle *)gUfs1WlunDevice.DeviceHandle, &ufs_info); 
     if (UFS_EOK != rc)
        ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
     lun_enabled_mask = ufs_info.dLunEnabled;
     total_luns_enabled = ufs_info.bNumberLu;
     
     for (lun = 0; lun < total_luns_enabled; lun++)
     { 
        // Check if LUN is enabled
        if (!(lun_enabled_mask & ((UINT32) 1) << lun))
           continue;
     
        gUfs1DevicePath[lun].Ufs = UfsPath; 
        gUfs1DevicePath[lun].Ufs.Guid = Ufs1DevicePathGuids[lun]; 
        gUfs1DevicePath[lun].End = UfsEndPath;
        
        g_Ufs1Device[lun].Signature    = UFS_DEV_SIGNATURE;
        g_Ufs1Device[lun].BlkIo        = gBlockIoUfs;
        g_Ufs1Device[lun].BlkIo.Media  = &gMediaUfs[lun];
        g_Ufs1Device[lun].BlkIo2        = gBlockIo2Ufs;
        g_Ufs1Device[lun].BlkIo2.Media  = &gMediaUfs[lun];
        g_Ufs1Device[lun].CardInfo     = gUfsCardInfoProtocol;
        g_Ufs1Device[lun].EraseBlk     = gUfsEraseBlkProtocol;
        CopyMem(g_Ufs1Device[lun].BlkIo.Media, 
                   &UFSMedia, sizeof(EFI_BLOCK_IO_MEDIA));  
        g_Ufs1Device[lun].WriteProtect = gUfsWriteProtectProtocol;
     
        /* Initialize LUN */
        g_Ufs1Device[lun].DeviceHandle = ufs_open (1, lun);
        if (!g_Ufs1Device[lun].DeviceHandle) {
           g_Ufs1Device[lun].BlkIo.Media->MediaPresent = FALSE;
           continue;
        }
        g_Ufs1Device[lun].BlkIo.Media->MediaPresent = TRUE;
        rc = ufs_get_device_info ((struct ufs_handle *)g_Ufs1Device[lun].DeviceHandle, &ufs_info); 
        if (UFS_EOK != rc)
           ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
        g_Ufs1Device[lun].BlkIo.Media->LastBlock = ufs_info.dLuTotalBlocks - 1;

        /* Install the following protocols: */ 
        /* BlkIO */
        Status = gBS->InstallMultipleProtocolInterfaces (
           &g_Ufs1Device[lun].ClientHandle,  
           &gEfiBlockIoProtocolGuid, &g_Ufs1Device[lun].BlkIo, 
           &gEfiDevicePathProtocolGuid, &gUfs1DevicePath[lun],
           &gEfiMemCardInfoProtocolGuid, &g_Ufs1Device[lun].CardInfo,
           &gEfiBlockIo2ProtocolGuid, &g_Ufs1Device[lun].BlkIo2,
           &gEfiEraseBlockProtocolGuid, &g_Ufs1Device[lun].EraseBlk,
           &gEfiStorageWpProtocolGuid, &g_Ufs1Device[lun].WriteProtect, 
           NULL
        );
        ASSERT_EFI_ERROR(Status);
     }
      if (ufs_info.wSpecVersion >= 0x300)
      {
         /* Create the RegisterAmlVariable callback */
         Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL,
                                   TPL_CALLBACK,
                                   RegisterAmlVariable,
                                   NULL, 
                                   &SecondaryUfs30Init);
         ASSERT_EFI_ERROR (Status); 
   
         Status = gBS->RegisterProtocolNotify(&gQcomAcpiPlatformProtocolGuid,
                                              SecondaryUfs30Init, 
                                              (VOID *)&SecondaryUfs30InitToken); 
         ASSERT_EFI_ERROR(Status);
      }

      /* Create the RegisterAmlVariable callback */
      Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL,
                                TPL_CALLBACK,
                                RegisterAmlVariable,
                                NULL, 
                                &SecondaryUfsInit);
      ASSERT_EFI_ERROR (Status); 
   
      Status = gBS->RegisterProtocolNotify(&gQcomAcpiPlatformProtocolGuid,
                                           SecondaryUfsInit, 
                                           (VOID *)&SecondaryUfsInitToken); 
      ASSERT_EFI_ERROR(Status);
   } else {
#endif
	  /* Check if we need to do the partial init of secondary UFS for HLOS */
      GetConfigValue ("EnableSecondaryUFSforHLOS", &SecondaryUFSInit);
	  
	  if (SecondaryUFSInit == 1) {
        gUfs1WlunDevice.DeviceHandle = ufs_open(1, UFS_WLUN_DEVICE); 
        if (gUfs1WlunDevice.DeviceHandle == NULL)
        {
            DEBUG ((EFI_D_ERROR, "Cannot open Secondary UFS!\n"));
        }
        else
        {
           gUfs1Device.DeviceHandle = ufs_open(1, 0);
           if (gUfs1Device.DeviceHandle == NULL)
           {
              DEBUG ((EFI_D_ERROR, "Failed to open LUN0 of Secondary UFS!\n"));
              DEBUG ((EFI_D_ERROR, "Did you remember to provision the secondary UFS?\n"));
           }
        }
        
        rc = ufs_get_device_info (gUfs1WlunDevice.DeviceHandle, &ufs_info);
        if (UFS_EOK != rc)
        {
           ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
        }
        
        if (ufs_info.wSpecVersion >= 0x300)
        {
           /* Create the RegisterAmlVariable callback */
           Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL,
                                     TPL_CALLBACK,
                                     RegisterAmlVariable,
                                     NULL, 
                                     &SecondaryUfs30Init);
           ASSERT_EFI_ERROR (Status); 
        
           Status = gBS->RegisterProtocolNotify(&gQcomAcpiPlatformProtocolGuid,
                                                SecondaryUfs30Init, 
                                                (VOID *)&SecondaryUfs30InitToken); 
           ASSERT_EFI_ERROR(Status);
        }
        
        /* Create the RegisterAmlVariable callback */
        Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL,
                                  TPL_CALLBACK,
                                  RegisterAmlVariable,
                                  NULL, 
                                  &SecondaryUfsInit);
        ASSERT_EFI_ERROR (Status); 
        
        Status = gBS->RegisterProtocolNotify(&gQcomAcpiPlatformProtocolGuid,
                                             SecondaryUfsInit, 
                                             (VOID *)&SecondaryUfsInitToken); 
        ASSERT_EFI_ERROR(Status);
        
        gUfs1WlunDevice.UfsProvision = gUfsProvProtocol;
        Status = gBS->InstallMultipleProtocolInterfaces (
           &gUfs1WlunDevice.ClientHandle, 
           &gEfiUfsProvProtocolGuid, &gUfs1WlunDevice.UfsProvision, 
           NULL); 
        ASSERT_EFI_ERROR(Status);
	  }
#ifdef ENABLE_AUTO_PLAT
   }
#endif

   /* Allocate memory for RPMB listener */
   Status = RPMBListenerAllocMem();
   if(Status != EFI_SUCCESS)
   {
      DEBUG ((EFI_D_ERROR, "Failed to allocate memory for RPMB listener\n"));
      return Status; 
   }

   /* Allocate memory for GPT listener */
   Status = GPTListenerAllocMem();
   if(Status != EFI_SUCCESS)
   {
      DEBUG ((EFI_D_ERROR, "Failed to allocate memory for GPT listener\n"));
      return Status; 
   }  

   Status = gBS->LocateProtocol(&gQcomScmProtocolGuid, NULL,
                                (VOID **)&ScmProtocol);
   if (Status == EFI_SUCCESS)
   {
      RegisterListener(ScmInitialized, (VOID *)&gUfsDevice[0].BlkIo); 
   }
   else
   {
      /* Create the RegisterListener callback */
      Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL,
                                TPL_CALLBACK,
                                RegisterListener,
                                (VOID *)&gUfsDevice[0].BlkIo,
                                &ScmInitialized
                             );
      ASSERT_EFI_ERROR (Status); 
   
      Status = gBS->RegisterProtocolNotify(&gQcomScmProtocolGuid,
                                           ScmInitialized, 
                                           (VOID *)&ScmInitializedToken); 

      ASSERT_EFI_ERROR(Status);
   }

   /* Initialize request list for Block IO 2 */ 
   gUfsTransferInfo.BlkIO2Initialized = TRUE;
   Status = BlkIo2ReqListInit(&gUfsReqList, REQ_LIST_SIZE);

   if(Status == EFI_SUCCESS)
   {
      /* Create Internal Event for Block IO 2 */
      Status = gBS->CreateEvent (
                       EVT_TIMER | EVT_NOTIFY_SIGNAL,
                       UFS_TPL,
                       BlkIo2TimerEventNotify,
                       NULL,
                       &gUfsBlkIo2TimerEvent
                       );
      
      if(EFI_ERROR (Status)) 
      {
         gUfsTransferInfo.BlkIO2Initialized = FALSE;
         DEBUG ((EFI_D_WARN, "Block IO 2 timer creation failed, cannot use Block IO 2.\n"));
      }
   }   
   else 
   {
      gUfsTransferInfo.BlkIO2Initialized = FALSE;
      DEBUG ((EFI_D_WARN, "Initialization failed, cannot use Block IO 2.\n"));
   }

   /* Crate an event for Erase, Purge */
   gUfsTransferInfo.PurgeEventInitialized = TRUE;
   Status = gBS->CreateEvent (
                    EVT_TIMER | EVT_NOTIFY_SIGNAL,
                    UFS_TPL,
                    PurgeTimerEventNotify,
                    NULL,
                    &gUfsPurgeTimerEvent
                    );
   
   if(EFI_ERROR (Status)) 
   {
      gUfsTransferInfo.PurgeEventInitialized = FALSE;
      DEBUG ((EFI_D_WARN, "Purge event timer creation failed, cannot use non-blocking block erase.\n"));
   }

   /* Register for Sleep callback for UFS LPM */
   RegisterDevicePowerTransitionCallback (UFSSleepCb, (struct ufs_handle *)gUfsWlunDevice.DeviceHandle);

   /* Locate RscHandlerProtocol and register for LPM callback */
   Status = gBS->LocateProtocol(&gEfiRscHandlerProtocolGuid,
                                NULL,
                                (VOID **) &RscHandlerProtocol);
   if((EFI_SUCCESS == Status) && (RscHandlerProtocol != NULL))
   {
      Status = RscHandlerProtocol->Register(UfsLpmCb, TPL_CALLBACK);
      if(Status != EFI_SUCCESS)
      {
         DEBUG ((EFI_D_ERROR, "Failed to register UfsLpmCb, status 0x%x\n", Status));
         return Status; 
      } 

      /* Register for Exit Boot Service Event to Unregister LPM callback */
      Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                                   TPL_CALLBACK,
                                   UfsDxeExitBSCallback,
                                   NULL,
                                   &gEfiEventExitBootServicesGuid,
                                   &ExitBootServicesEvent);

      ASSERT_EFI_ERROR (Status);
   }
   else
   {
      DEBUG(( EFI_D_WARN, "InitLPMSupport Failure = 0x%x\r\n", Status));
      return Status; 
   }
   
   return EFI_SUCCESS;
}


/**
   Get the active UFS Boot LU  
**/
EFI_STATUS EFIAPI UFSGetActiveBootLUInternal (
   EFI_MEM_CARDINFO_PROTOCOL  *This,
   UINT32                     *ActiveBootLU)
{
   INT32 rc;
   struct ufs_info_type info;

   if ((NULL == This) || (NULL == ActiveBootLU))
   {
      return EFI_INVALID_PARAMETER;
   }

   /* Open WLUN BOOT and get Card Info */
   if (!gUfsBootDevice.DeviceHandle) 
   {
      gUfsBootDevice.DeviceHandle = ufs_open(0, UFS_WLUN_BOOT);
      if (!gUfsBootDevice.DeviceHandle)
      {
         DEBUG ((EFI_D_ERROR, "Cannot open UFS WLUN BOOT!\n"));
         return EFI_DEVICE_ERROR; 
      }
   }

   rc = ufs_get_device_info ((struct ufs_handle *)gUfsBootDevice.DeviceHandle, &info); 
   if (UFS_EOK != rc)  
   {
      DEBUG ((EFI_D_ERROR, "Get Card Info FAILED %x\n\n\n", rc)); 
      return EFI_DEVICE_ERROR;
   }

   /* bLUN tells us which BOOT LUN is current */
   *ActiveBootLU = info.bLUN;     

   return EFI_SUCCESS; 
}

EFI_STATUS EFIAPI UFSGetActiveBootLU (
   EFI_MEM_CARDINFO_PROTOCOL  *This,
   UINT32                     *ActiveBootLU)
{
  EFI_STATUS Status;

  Lock (UfsLockPtr);
  Status = UFSGetActiveBootLUInternal (This, ActiveBootLU);
  UnLock (UfsLockPtr);

  return Status;
}
/**
   Set the active UFS Boot LU 
**/
EFI_STATUS EFIAPI UFSSetActiveBootLU (
   EFI_MEM_CARDINFO_PROTOCOL *This,
   UINT32                    BootLU)
{
   INT32 rc;
   UFS_DEV *UfsDevice;
   struct ufs_handle *hUFS;

   if ((NULL == This) || (BootLU > UFS_LUN_BOOT_B)) 
   {
      return EFI_INVALID_PARAMETER;
   }

   UfsDevice = UFS_DEV_FROM_CARD_INFO (This);
   hUFS = UfsDevice->DeviceHandle;

   if (NULL == hUFS) 
   {
      return EFI_INVALID_PARAMETER;
   }

   Lock (UfsLockPtr);
   rc =  ufs_set_bootable (hUFS, BootLU); 
   UnLock (UfsLockPtr);
   if (UFS_EOK != rc)
   {
      DEBUG ((EFI_D_ERROR, "Set Active Boot LU FAILED. %x\n\n\n", rc));
      return EFI_DEVICE_ERROR;
   }

   return EFI_SUCCESS; 
}

/**
   Get the card information 
**/
EFI_STATUS EFIAPI UFSGetCardInfoInternal (
            EFI_MEM_CARDINFO_PROTOCOL   *This,
            MEM_CARD_INFO               *card_info)
{
   char serial_num_str[254] = {0}; // 126 unicode chars + length + IDN 
   INT32 rc;
   UFS_DEV    *UfsDevice;
   struct ufs_handle *hUFS;
   struct ufs_info_type info;
   int i;

   if ((NULL == card_info) || (NULL == This)) {
      return EFI_INVALID_PARAMETER;
   }

   UfsDevice = UFS_DEV_FROM_CARD_INFO (This);
   hUFS = UfsDevice->DeviceHandle;
   if (NULL == hUFS) {
      return EFI_INVALID_PARAMETER;
   }

   /* Get Card Info */
   rc = ufs_get_device_info (hUFS, &info);
   if (UFS_EOK != rc) {
      DEBUG ((EFI_D_ERROR, "Get Card Info FAILED. %x\n\n\n", rc));
      return EFI_DEVICE_ERROR;
   }

   /* Copy the information out */
   card_info->card_type[0] = 'U';
   card_info->card_type[1] = 'F';
   card_info->card_type[2] = 'S';
   card_info->card_type[3] = ' ';

   card_info->mfr_id = info.wManufacturerID;
   rc = ufs_get_device_info_str (hUFS, info.iSerialNumber, 
                                 serial_num_str, sizeof(serial_num_str)); 
   if (UFS_EOK != rc || serial_num_str[1] != 0x05)
   {
      DEBUG ((EFI_D_ERROR, "Get Card Serial Number FAILED. %x\n\n\n", rc));
      return EFI_DEVICE_ERROR;
   }

   card_info->serial_num_len = serial_num_str[0] - 2;  // Get length and reduce by 2 bytes (length, IDN)

   CopyMem(card_info->product_serial_num, &serial_num_str[2], card_info->serial_num_len);

   for (i = 0; i < 29; i++) 
   {
      card_info->inquiry_str[i] = info.inquiry_str[i];
   }

   rc = ufs_get_device_info ((struct ufs_handle *)gUfsRpmbDevice.DeviceHandle, &info); 
   if (UFS_EOK != rc)
   {
      DEBUG ((EFI_D_ERROR, "Get Card Info FAILED. %x\n\n\n", rc));
      return EFI_DEVICE_ERROR;
   }
   card_info->rpmb_size_in_byte = info.qLogicalBlockCount * 256;
   card_info->reliable_write_count = info.bRPMB_ReadWriteSize;

   return EFI_SUCCESS;
}

EFI_STATUS EFIAPI UFSGetCardInfo (
            EFI_MEM_CARDINFO_PROTOCOL   *This,
            MEM_CARD_INFO               *card_info)
{
  EFI_STATUS Status;

  Lock (UfsLockPtr);
  Status = UFSGetCardInfoInternal (This, card_info);
  UnLock (UfsLockPtr);

  return Status;
}


/**
   This function retrieves Read Counter Response packet from flash device.
   The Response packet includes the Write Counter as well as the MAC which
   is used later to validate the Response packet
  
   @param  RespPktBuffer [OUT]    Pointer to the response from the Read
                                  Counter command
    
   @retval  RPMB_NO_ERROR           Successfully read the Counter packet.
   @retval  RPMB_ERR_INVALID_PARAM  NULL pointer is passed in to the function.
   @retval  RPMB_ERR_READ           Failed to read the Counter packet.
**/
INT32 EFIAPI RPMBReadCounterPkt (
            EFI_SDCC_RPMB_PROTOCOL       *This,
            UINT32                       *RespPktBuffer)
{
   UFS_DEV    *UfsDevice;
   struct ufs_handle *hUFS;
   INT32  rc = 0;

   if ((NULL == This) || (NULL == RespPktBuffer)) {
      return RPMB_ERR_INVALID_PARAM;
   }
   
   UfsDevice = UFS_DEV_FROM_RPMB (This);
   hUFS = UfsDevice->DeviceHandle;
  
   if (NULL == hUFS) {
      return RPMB_ERR_INVALID_PARAM;
   }
 
   Lock (UfsLockPtr);
   rc = ufs_rpmb_read_cnt_pkt(hUFS, (UINT8 *)RespPktBuffer);
   UnLock (UfsLockPtr);

   if (UFS_EOK != rc) {
      return RPMB_ERR_READ;
   }
   return RPMB_NO_ERROR;
}

/**
 This function programs the RPMB's key using the provided packet
 formatted as ProvisionKey command. 

 @param CmdPktBuffer  [IN] : Pointer to a ProvisionKey command packet
                             to be sent to RPMB to program its key
 @param RespPktBuffer [OUT]: Pointer to the response packet for the
                             ProvisionKey command
 
 @retval  RPMB_NO_ERROR            Successfully provision the RPMB's Key.
 @retval  RPMB_ERR_INVALID_PARAM   NULL pointer is passed in to the function.
 @retval  RPMB_ERR_GENERAL         Failed to provision the key (Key has been
                                   provisioned).
**/
INT32 EFIAPI RPMBProgProvisionKey (
            EFI_SDCC_RPMB_PROTOCOL       *This,
            UINT32                       *CmdPktBuffer,
            UINT32                       *RespPktBuffer )
{
   UFS_DEV    *UfsDevice;
   struct ufs_handle *hUFS = NULL;
   INT32  rc = UFS_EOK; 

   if ((NULL == CmdPktBuffer) || (NULL == RespPktBuffer) || (NULL == This)) {
      return RPMB_ERR_INVALID_PARAM;
   }

   UfsDevice = UFS_DEV_FROM_RPMB (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;       
   
   if (NULL == hUFS) {
      return RPMB_ERR_INVALID_PARAM;
   }

   Lock (UfsLockPtr);
   rc = ufs_rpmb_write_blocks(hUFS, (UINT8 *)CmdPktBuffer, 
                              1, (UINT8 *)RespPktBuffer);
   UnLock (UfsLockPtr);

   if (UFS_EOK != rc) {
      return RPMB_ERR_GENERAL;
   }

   return RPMB_NO_ERROR;
}

/**
  This function reads the sectors from RPMB partition using the
  cmd packet buffers provided.
 
  @param   CmdPktBuffer  [IN] : Pointer to a formatted packet for Read Request 
  @param   PktCount      [IN] : How many half sectors to read   
  @param   DataBuffer    [OUT]: Pointer to data read from PRMB
 
  @retval  RPMB_NO_ERROR            Successfully read the Counter packet
  @retval  RPMB_ERR_INVALID_PARAM   NULL pointer is passed in to the function
  @retval  RPMB_ERR_READ            Failed to read the sectors
     
**/ 
INT32 EFIAPI RPMBReadBlocks (
            EFI_SDCC_RPMB_PROTOCOL       *This,
            UINT32                       *CmdPktBuffer,
            UINT32                       PktCount,
            UINT32                       *DataBuffer)
{
   UFS_DEV    *UfsDevice;
   struct ufs_handle *hUFS;
   INT32 rc = UFS_EOK;

   if ((NULL == CmdPktBuffer) || (NULL == DataBuffer) || 
       (NULL == This) || (PktCount == 0))
   {
      return RPMB_ERR_INVALID_PARAM;
   }
   
   if (PktCount > RPMB_MAX_BLOCK_TRANSFER_SIZE) {
      return RPMB_ERR_INVALID_PARAM;
   }
   
   UfsDevice = UFS_DEV_FROM_RPMB (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;       
   
   if (NULL == hUFS) {
      return RPMB_ERR_INVALID_PARAM;
   }

   Lock (UfsLockPtr);
   rc = ufs_rpmb_read_blocks(hUFS, (UINT8 *)CmdPktBuffer, 
                             PktCount, (UINT8 *)DataBuffer); 
   UnLock (UfsLockPtr);
   if (UFS_EOK != rc) {
      return RPMB_ERR_READ;
   }
     
   return RPMB_NO_ERROR;

}

/**
  This function writes the sectors to the RPMB partition using
  the provided cmd packet buffers
                       
  @param  CmdPktBuffer  [IN] :  Pointer to a formatted packets to be sent
                                to PRMB to write to RPMB partition
  @param  PktCount      [IN] :  Number of half sectors to write
  @param  RespPktBuffer [OUT]:  Pointer to the response packet from PRMB
 
  @retval  RPMB_NO_ERROR            Successfully Write the RPMB sectors.
                                    Caller should authenticate the Response
                                    packet and validate the Write Counter
  @retval  RPMB_ERR_INVALID_PARAM   NULL pointer is passed in to the function
  @retval  RPMB_ERR_WRITE           Failed to write the sectors
**/
INT32 EFIAPI RPMBWriteBlocks (
            EFI_SDCC_RPMB_PROTOCOL       *This,
            UINT32                       *CmdPktBuffer,
            UINT32                       PktCount,
            UINT32                       *RespPktBuffer )
{
   UFS_DEV    *UfsDevice;
   struct ufs_handle *hUFS = NULL;
   INT32  rc = UFS_EOK;

   if ((NULL == CmdPktBuffer) || (NULL == RespPktBuffer) || 
      (NULL == This) || (PktCount == 0)) 
   {
      return RPMB_ERR_INVALID_PARAM;
   }

   if (PktCount > RPMB_MAX_BLOCK_TRANSFER_SIZE) {
      return RPMB_ERR_INVALID_PARAM;
   }

   UfsDevice = UFS_DEV_FROM_RPMB (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;       
   
   if (NULL == hUFS) {
      return RPMB_ERR_INVALID_PARAM;
   }

   Lock (UfsLockPtr);
   rc = ufs_rpmb_write_blocks(hUFS, (UINT8 *)CmdPktBuffer, 
                              PktCount, (UINT8 *)RespPktBuffer);
   UnLock (UfsLockPtr);

   if (UFS_EOK != rc) {
      return RPMB_ERR_WRITE;
   }

   return RPMB_NO_ERROR;
}

/* Block IO2 supporting functions */

/**
   A wrapper for Block IO and Block IO 2. whether Block IO or Block IO 2 protocol
   will be used depending on caller and device status.
   
   @param  This       Indicates a pointer to the calling context.
   @param  MediaId    Id of the media, changes every time the media is replaced.
   @param  Lba        The starting Logical Block Address to read from
   @param  Token    A pointer to the token associated with the transaction.
   @param  BufferSize Size of Buffer, must be a multiple of device block size.
   @param  Buffer     A pointer to the destination buffer for the data.
                      The caller is responsible for either having implicit or
                      explicit ownership of the buffer.
   
   @retval EFI_SUCCESS           The data was read correctly from the device.
   @retval EFI_DEVICE_ERROR      The device reported an error while
                                 performing the read.
   @retval EFI_NO_MEDIA          There is no media in the device.
   @retval EFI_MEDIA_CHANGED     The MediaId does not matched the
                                 current device.
   @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block
                                 size of the device.
   @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not
                                 valid,or the buffer is not on proper alignment.
**/
EFI_STATUS UFSIoWrapper (
            IN VOID                     *pThis,
            IN UINT32                    MediaId,
            IN EFI_LBA                   Lba,
            IN OUT EFI_BLOCK_IO2_TOKEN  *Token,
            IN UINTN                     BufferSize,
            IN OUT VOID                 *Buffer)
{
   EFI_STATUS        Status = EFI_SUCCESS;
   UFS_DEV           *UfsDevice;
   UINT32            block_size = 0;
   BLKIO2_REQ_LIST  *pReqList = &gUfsReqList;
   
   if (gUfsTransferInfo.TransferType == BLOCKIO_READ || 
       gUfsTransferInfo.TransferType == BLOCKIO_WRITE)
   {
      EFI_BLOCK_IO_PROTOCOL  *This = pThis;
      
      // Cancel timer for Block IO 
      if (gUfsTransferInfo.TimerSet == TRUE) {
         gBS->SetTimer (gUfsBlkIo2TimerEvent, TimerCancel, 0); 
      }

      // If no Block IO 2 tranfer is in progress 
      if (!pReqList->CurReq->IsStart)
      {
         if (gUfsTransferInfo.TransferType == BLOCKIO_READ) {
            Status = UFSReadBlocks(This, MediaId, Lba, BufferSize, Buffer);
         } else {           
            Status = UFSWriteBlocks(This, MediaId, Lba, BufferSize, Buffer);
         }

         // Set timer if necessary
         if (TRUE == gUfsTransferInfo.TimerSet) {
            gBS->SetTimer (gUfsBlkIo2TimerEvent, TimerPeriodic, BLKIO2_POLL_PERIOD);
         }
         goto WrapperExit;
      }
      
      // If Block IO 2 is in progress, Block IO will take over the transfer 
      // and has priority to run. Block IO 2 resumes after Block IO 
      if (pReqList->CurReq->IsStart) {         
         Status = BlkIoTakeOverTransfer(This, MediaId, Lba, BufferSize, Buffer);
      }

      // Set timer to call Block IO 2 
      if (TRUE  == gUfsTransferInfo.TimerSet) {
         gBS->SetTimer (gUfsBlkIo2TimerEvent, TimerPeriodic, BLKIO2_POLL_PERIOD);
      }
   }
   // If caller calls Block IO 2 
   else 
   {  
      EFI_BLOCK_IO2_PROTOCOL *This = pThis;

      // Validate passed-in pointers are not NULL 
      if ((NULL == Buffer) || (NULL == This) || (NULL == This->Media) || 
          (NULL == Token) || (NULL == Token->Event)) {
         Status = EFI_INVALID_PARAMETER;
         goto WrapperExit;
      }
      
      // Validate if buffer is multiple of block size 
      if ((BufferSize%(This->Media->BlockSize) != 0) || (BufferSize == 0)) {
         Status = EFI_BAD_BUFFER_SIZE;
         goto WrapperExit;
      }

      // Validate block size and access range 
      if ((Lba > This->Media->LastBlock) || (This->Media->BlockSize == 0)) {
         Status = EFI_INVALID_PARAMETER;
         goto WrapperExit;
      }
   
      if (This->Media->MediaId != MediaId) {
         Status = EFI_MEDIA_CHANGED;
         goto WrapperExit;
      }
     
      UfsDevice = UFS_DEV_FROM_BLOCKIO2 (This);       
      block_size = UfsDevice->BlkIo.Media->BlockSize;

      if ((Lba + (BufferSize/block_size)) > (UfsDevice->BlkIo.Media->LastBlock + 1)) {
         Status = EFI_INVALID_PARAMETER;
         goto WrapperExit;
      }   
      
      // Append request to queue 
      EfiAcquireLock (&pReqList->ReqListLock);

      if (pReqList->Full == TRUE) { 
         EfiReleaseLock (&pReqList->ReqListLock);      
         Status = EFI_OUT_OF_RESOURCES;
         goto WrapperExit;
      }
  
      pReqList->LastReq->Token = Token;
      pReqList->LastReq->This = This;
      pReqList->LastReq->MediaId = MediaId;
      pReqList->LastReq->Lba = Lba;
      pReqList->LastReq->BufferSize = BufferSize;
      pReqList->LastReq->Buffer = Buffer;
      
      // Append opeartion type
      if (gUfsTransferInfo.TransferType == BLOCKIO2_READ) {        
         pReqList->LastReq->Type = BLOCKIO2_READ;  
      } else {        
         pReqList->LastReq->Type = BLOCKIO2_WRITE;
      }

      pReqList->LastReq ++;
  
      if(pReqList->LastReq == pReqList->Tail) {
         pReqList->LastReq = pReqList->Head;  
      }
      
      if(pReqList->LastReq == pReqList->CurReq) { 
         pReqList->Full = TRUE;                    
      }

      EfiReleaseLock (&pReqList->ReqListLock);           
      
      // Call Notity function to do real Block IO 2 transfer 
      BlkIo2TimerEventNotify (NULL, NULL);   
   }

WrapperExit:
   return Status;
}

/**
   Read BufferSize bytes from Lba into Buffer.
   
   @param  This       Indicates a pointer to the calling context.
   @param  MediaId    Id of the media, changes every time the media is replaced.
   @param  Lba        The starting Logical Block Address to read from
   @param  BufferSize Size of Buffer, must be a multiple of device block size.
   @param  Buffer     A pointer to the destination buffer for the data.
                      The caller is responsible for either having implicit or
                      explicit ownership of the buffer.
   
   @retval EFI_SUCCESS           The data was read correctly from the device.
   @retval EFI_DEVICE_ERROR      The device reported an error while
                                 performing the read.
   @retval EFI_NO_MEDIA          There is no media in the device.
   @retval EFI_MEDIA_CHANGED     The MediaId does not matched the
                                 current device.
   @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block
                                 size of the device.
   @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not
                                 valid,or the buffer is not on proper alignment.
**/
EFI_STATUS UFSReadBlocksWrapper (
            IN EFI_BLOCK_IO_PROTOCOL  *This,
            IN UINT32                  MediaId,
            IN EFI_LBA                 Lba,
            IN UINTN                   BufferSize,
            OUT VOID                  *Buffer)
{
   EFI_STATUS Status = EFI_UNSUPPORTED;

   // UFS accesses has NOTIFY priority 
   EFI_TPL PrevTPL;
   PrevTPL = gBS->RaiseTPL (UFS_TPL);

   Lock (UfsLockPtr);
   gUfsTransferInfo.TransferType = BLOCKIO_READ;
   Status = UFSIoWrapper (This, MediaId, Lba, NULL, BufferSize, Buffer);
   UnLock (UfsLockPtr);

   gBS->RestoreTPL (PrevTPL);

   return Status;
}

/**
   Write BufferSize bytes from Buffer to Lba.
   
   @param  This       Indicates a pointer to the calling context.
   @param  MediaId    The media ID that the write request is for.
   @param  Lba        The starting logical block address to be 
                      written. The   caller is responsible for
                      writing to only legitimate locations.
   @param  BufferSize Size of Buffer, must be a multiple of device block size.
   @param  Buffer     A pointer to the source buffer for the data.
   
   @retval EFI_SUCCESS           The data was written correctly to the device.
   @retval EFI_WRITE_PROTECTED   The device can not be written to.
   @retval EFI_DEVICE_ERROR      The device reported an error 
                                 while performing the write.
   @retval EFI_NO_MEDIA          There is no media in the device.
   @retval EFI_MEDIA_CHNAGED     The MediaId does not matched 
                                 the current device.
   @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple 
                                 of the block size of the device.
   @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not
                                 valid or the buffer is not on proper
                                 alignment.
**/
EFI_STATUS UFSWriteBlocksWrapper (
            IN EFI_BLOCK_IO_PROTOCOL   *This,
            IN UINT32                  MediaId,
            IN EFI_LBA                 Lba,
            IN UINTN                   BufferSize,
            IN VOID                    *Buffer)
{
   EFI_STATUS Status = EFI_UNSUPPORTED;

   // UFS accesses has NOTIFY priority 
   EFI_TPL PrevTPL;
   PrevTPL = gBS->RaiseTPL (UFS_TPL);

   Lock (UfsLockPtr);
   gUfsTransferInfo.TransferType = BLOCKIO_WRITE;
   Status = UFSIoWrapper(This, MediaId, Lba, NULL, BufferSize, Buffer);
   UnLock (UfsLockPtr);

   gBS->RestoreTPL (PrevTPL);

   return Status;
}
 
/**
  Read BufferSize bytes from Lba into Buffer.
  
  This function reads the requested number of blocks from the device. All the
  blocks are read, or an error is returned.
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_or EFI_MEDIA_CHANGED is returned and
  non-blocking I/O is being used, the Event associated with this request will
  not be signaled.

  @param[in]       This       Indicates a pointer to the calling context.
  @param[in]       MediaId    Id of the media, changes every time the media is 
                              replaced.
  @param[in]       Lba        The starting Logical Block Address to read from.
  @param[in, out]  Token       A pointer to the token associated with the transaction.
  @param[in]       BufferSize Size of Buffer, must be a multiple of device block size.  
  @param[out]      Buffer     A pointer to the destination buffer for the data. The 
                              caller is responsible for either having implicit or 
                              explicit ownership of the buffer.

  @retval EFI_SUCCESS           The read request was queued if Token->Event is
                                not NULL.The data was read correctly from the
                                device if the Token->Event is NULL.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing
                                the read.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId is not for the current media.
  @retval EFI_BAD_BUFFER_SIZE   The BufferSize parameter is not a multiple of the
                                intrinsic block size of the device.
  @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack
                                of resources.
**/ 
EFI_STATUS UFSReadBlocksEx (
            IN     EFI_BLOCK_IO2_PROTOCOL *This,
            IN     UINT32                 MediaId,
            IN     EFI_LBA                Lba,
            IN OUT EFI_BLOCK_IO2_TOKEN    *Token,
            IN     UINTN                  BufferSize,
            OUT VOID                      *Buffer)
{
   EFI_STATUS Status = EFI_UNSUPPORTED;

   /* UFS accesses has NOTIFY priority */
   EFI_TPL PrevTPL;
   PrevTPL = gBS->RaiseTPL(UFS_TPL);
   Lock (UfsLockPtr);

   /* Do not use Block IO 2 if not initialized succesfully */
   if (gUfsTransferInfo.BlkIO2Initialized == FALSE)
   {
      Status = EFI_OUT_OF_RESOURCES;
      goto ReadBlocksExExit;
   }

   gUfsTransferInfo.TransferType = BLOCKIO2_READ;
   Status = UFSIoWrapper(This, MediaId, Lba, Token, BufferSize, Buffer);

ReadBlocksExExit:
   UnLock (UfsLockPtr);
   gBS->RestoreTPL (PrevTPL);
   
   return Status;
}

/**
  Write BufferSize bytes from Buffer to Lba.

  This function writes the requested number of blocks to the device. All blocks
  are written, or an error is returned.If EFI_DEVICE_ERROR, EFI_NO_MEDIA,
  EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED is returned and non-blocking I/O is
  being used, the Event associated with this request will not be signaled.

  @param[in]       This       Indicates a pointer to the calling context.
  @param[in]       MediaId    The media ID that the write request is for.
  @param[in]       Lba        The starting logical block address to be written. The
                              caller is responsible for writing to only legitimate
                              locations.
  @param[in, out]  Token      A pointer to the token associated with the transaction.
  @param[in]       BufferSize Size of Buffer, must be a multiple of device block size.
  @param[in]       Buffer     A pointer to the source buffer for the data.

  @retval EFI_SUCCESS           The write request was queued if Event is not NULL.
                                The data was written correctly to the device if
                                the Event is NULL.
  @retval EFI_WRITE_PROTECTED   The device can not be written to.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHNAGED     The MediaId does not matched the current device.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing the write.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.
  @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack
                                of resources.

**/
EFI_STATUS UFSWriteBlocksEx (
            IN     EFI_BLOCK_IO2_PROTOCOL *This,
            IN     UINT32                 MediaId,
            IN     EFI_LBA                Lba,
            IN OUT EFI_BLOCK_IO2_TOKEN    *Token,
            IN     UINTN                  BufferSize,
            OUT VOID                      *Buffer)
{
   EFI_STATUS Status = EFI_UNSUPPORTED;

   EFI_TPL PrevTPL;
   PrevTPL = gBS->RaiseTPL(UFS_TPL);
   Lock (UfsLockPtr);

   /* Do not use Block IO 2 if not initialized succesfully */
   if (gUfsTransferInfo.BlkIO2Initialized == FALSE)
   {
      Status = EFI_OUT_OF_RESOURCES;
      goto WriteBlocksExExit;
   }  

   gUfsTransferInfo.TransferType = BLOCKIO2_WRITE;
   Status = UFSIoWrapper(This, MediaId, Lba, Token, BufferSize, Buffer);

WriteBlocksExExit:
   UnLock (UfsLockPtr);
   gBS->RestoreTPL (PrevTPL);
   
   return Status;
}

/**
  Reset the block device hardware.

  @param[in]  This                 Indicates a pointer to the calling context.
  @param[in]  ExtendedVerification Indicates that the driver may perform a more
                                   exhausive verfication operation of the device
                                   during reset.

  @retval EFI_SUCCESS          The device was reset.
  @retval EFI_DEVICE_ERROR     The device is not functioning properly and could
                               not be reset.
**/
EFI_STATUS EFIAPI UFSResetEx (
            IN EFI_BLOCK_IO2_PROTOCOL  *This,
            IN BOOLEAN                 ExtendedVerification)
{
   return EFI_SUCCESS;
}

/**
  Flush the Block Device.
 
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED
  is returned and non-blocking I/O is being used, the Event associated with
  this request will not be signaled.  

  @param[in]      This     Indicates a pointer to the calling context.
  @param[in,out]  Token    A pointer to the token associated with the transaction

  @retval EFI_SUCCESS          The flush request was queued if Event is not NULL.
                               All outstanding data was written correctly to the
                               device if the Event is NULL.
  @retval EFI_DEVICE_ERROR     The device reported an error while writting back
                               the data.
  @retval EFI_WRITE_PROTECTED  The device cannot be written to.
  @retval EFI_NO_MEDIA         There is no media in the device.
  @retval EFI_MEDIA_CHANGED    The MediaId is not for the current media.
  @retval EFI_OUT_OF_RESOURCES The request could not be completed due to a lack
                               of resources.
**/
EFI_STATUS EFIAPI UFSFlushBlocksEx (
            IN     EFI_BLOCK_IO2_PROTOCOL   *This,
            IN OUT EFI_BLOCK_IO2_TOKEN      *Token)
{
   return EFI_SUCCESS;
}


/**
  Erase blocks in the Block Device
 
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED
  is returned and non-blocking I/O is being used, the Event associated with
  this request will not be signaled.  

  @param[in]      This     Indicates a pointer to the calling context.
  @param[in,out]  Token    A pointer to the token associated with the transaction

  @retval EFI_SUCCESS          The flush request was queued if Event is not NULL.
                               All outstanding data was written correctly to the
                               device if the Event is NULL.
  @retval EFI_DEVICE_ERROR     The device reported an error while writting back
                               the data.
  @retval EFI_WRITE_PROTECTED  The device cannot be written to.
  @retval EFI_NO_MEDIA         There is no media in the device.
  @retval EFI_MEDIA_CHANGED    The MediaId is not for the current media.
  @retval EFI_OUT_OF_RESOURCES The request could not be completed due to a lack
                               of resources.
**/
EFI_STATUS EFIAPI UFSEraseBlocksInternal (
   IN     EFI_BLOCK_IO_PROTOCOL *This,
   IN     UINT32 MediaId,
   IN     EFI_LBA LBA,
   IN OUT EFI_ERASE_BLOCK_TOKEN *Token,
   IN     UINTN Size
   )
{
   INT32  rc; 
   UFS_DEV    *UfsDevice;
   struct ufs_handle *hUFS;
   UINT32 blocks;
   EFI_STATUS Status;

   /* Validate passed-in size are not zero */
   if (0 == Size) {
      return EFI_INVALID_PARAMETER;
   }

   /* Validate if size is multiple of block size */
   if ((Size%(This->Media->BlockSize)) != 0 ) {
      return EFI_INVALID_PARAMETER;
   }

   /* Validate block size and access range */
   if ((LBA > This->Media->LastBlock) || (This->Media->BlockSize == 0)) {
      return EFI_INVALID_PARAMETER;
   }
   
   if (This->Media->MediaId != MediaId) {
      return EFI_MEDIA_CHANGED;
   }

   gUfsCurrentErase = This;
   UfsDevice = UFS_DEV_FROM_BLOCKIO (This);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;       
   
   /* Check if LBA plus the total sectors trying to erase would exceed the */
   /* total size of the partition */
   if ((LBA + (Size/This->Media->BlockSize)) > (UfsDevice->BlkIo.Media->LastBlock + 1)) {
      return EFI_INVALID_PARAMETER;
   }

   blocks = Size/(This->Media->BlockSize);

   rc = ufs_unmap (hUFS, LBA, blocks);
   if (UFS_EOK != rc) {
      DEBUG ((EFI_D_ERROR, "UFS UNMAP with return value of %d\n", rc));
      return EFI_DEVICE_ERROR;
   }

   /* Setup non-block purge sequence if purge event is initialized */
   if (gUfsTransferInfo.PurgeEventInitialized && Token != NULL) {
      rc = ufs_purge_nonblock_setup (hUFS);
      if (rc == -UFS_EBUSY) {
         // Device in busy, continue the non-blocking wait
         gUfsTransferInfo.TransferType = BLOCKERASE_PURGE;
      }
      else if (rc == UFS_EOK) {
         // Device is out of busy already, no need to wait
         gUfsTransferInfo.TransferType = 0;
         if (Token != NULL) {
            gUfsTransferInfo.EraseToken = NULL;
            Token->TransactionStatus = EFI_SUCCESS;
         }
         return EFI_SUCCESS;
      }
      else {
         DEBUG ((EFI_D_WARN, "UFS non-blocking purge setup failed with value of %d\n", rc));
      }
   }

   rc = ufs_purge_and_wait (hUFS);
   if (rc == -UFS_EBUSY) {
      /* Pending transfer is taking place, setting up a timer */
      gBS->SetTimer (gUfsPurgeTimerEvent, TimerPeriodic, BLKIO2_POLL_PERIOD);
      if (Token != NULL) {
         gUfsTransferInfo.EraseToken = Token;
         Status = gBS->CreateEvent (EVT_TIMER,
                                    TPL_APPLICATION,
                                    NULL,
                                    NULL,
                                    &Token->Event );
         ASSERT_EFI_ERROR (Status); 
         Token->TransactionStatus = EFI_SUCCESS;
      }
      return EFI_SUCCESS;
   }
   else if (rc != UFS_EOK) {
      DEBUG ((EFI_D_ERROR, "UFS purge with return value of %d\n", rc));
      if (Token != NULL) {
         Token->Event = NULL;
         Token->TransactionStatus = EFI_DEVICE_ERROR;
      }
      return EFI_DEVICE_ERROR;
   }

   /* Purge completed, set the token to NULL */
   if (Token != NULL) {
      Token->Event = NULL;
      Token->TransactionStatus = EFI_SUCCESS;
   }
   
   return EFI_SUCCESS;
}

EFI_STATUS EFIAPI UFSEraseBlocks (
   IN     EFI_BLOCK_IO_PROTOCOL *This,
   IN     UINT32 MediaId,
   IN     EFI_LBA LBA,
   IN OUT EFI_ERASE_BLOCK_TOKEN *Token,
   IN     UINTN Size
   )
{
  EFI_STATUS Status;

  Lock (UfsLockPtr);
  Status = UFSEraseBlocksInternal (This, MediaId, LBA, Token, Size);
  UnLock (UfsLockPtr);

  return Status;
}

/******************************************************************************
*  Handle the timer event for purge
*
* Arguments:
*    Event              [IN] : UEFI event method, not used
*    Context            [IN] : Can be any forms of input parameter, not used
******************************************************************************/
VOID PurgeTimerEventNotify (
                     IN  EFI_EVENT  Event,
                     IN  VOID      *Context)
{  
   INT32 rc = -UFS_ENOENT;  
   EFI_ERASE_BLOCK_TOKEN  *temp_token;
   UFS_DEV                *UfsDevice;
   struct ufs_handle      *hUFS;

   UfsDevice = UFS_DEV_FROM_BLOCKIO (gUfsCurrentErase);
   hUFS = (struct ufs_handle *) UfsDevice->DeviceHandle;       
   Lock (UfsLockPtr);
   rc = ufs_purge_and_wait (hUFS);
   UnLock (UfsLockPtr);
   
   if (rc == UFS_EOK) {
      gBS->SetTimer (gUfsPurgeTimerEvent, TimerCancel, 0);
      temp_token = gUfsTransferInfo.EraseToken;
      gBS->SignalEvent(temp_token->Event);   
      return;
   }
   if (rc == -UFS_EBUSY) {
      /* Still busy, wait more */
      return;
   }
   else {
      DEBUG ((EFI_D_ERROR, "UFS purge with return value of %d\n", rc));
      return;
   }

   return;
}

