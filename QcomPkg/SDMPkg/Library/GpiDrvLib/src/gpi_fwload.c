/**
 * @file  gpi_fwload.c
 * @brief Implements the firmware loading procedure for GSI and associated SEs 
 */
/*
===============================================================================

                               Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------ 
04/11/17   ts      Created to load qupv3 firmware from flash

===============================================================================
              Copyright (c) 2017 QUALCOMM Technologies, Inc.
                         All Rights Reserved.
            Qualcomm Technologies Proprietary and Confidential.
===============================================================================
*/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/QcomLib.h>
#include <Include/scm_sip_interface.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Protocol/EFIScm.h>
#include <Library/DebugLib.h>
#include <Library/ShmBridgeLib.h>
#include <Protocol/EFIShmBridge.h>
#include "gpi_ee.h"

/* SCM param value to be passed for loading GSI / QUP FW */
#define TZBSP_BLSP_SCM_CALL_FROM_GPI_DXE 0xC3C3C3C3

/* Global static variables */
STATIC UINT64 ScmParam[SCM_MAX_NUM_PARAMETERS];
STATIC UINT64 ScmResult[SCM_MAX_NUM_RESULTS];
STATIC QCOM_SCM_PROTOCOL * QcomScmProtocol = NULL;  

extern EFI_STATUS  efi_split_elf_file(void * elf_base, uint8 * meta_base, void ** prog_base, void * prog_size);
extern EFI_STATUS  prep_elf_parsing(void * elf_base, size_t * meta_buffer_size);

STATIC inline EFI_STATUS gpi_tz_pil_init_id(UINT64 ELFMetaBase)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;

  /* Subsys ID */
  ScmParam[0] = 0x13;
  /* Memory address of ELF meta data */
  ScmParam[1] = ELFMetaBase;

  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  /*
   * Authenticate ELF meta data with PIL
   */
  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_PIL_INIT_ID,
                                 TZ_PIL_INIT_ID_PARAM_ID,
                                 ScmParam,
                                 ScmResult);

  return Status;
}

STATIC inline EFI_STATUS gpi_tz_pil_mem_id(UINT64 imageBase, UINTN imageSize)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;

  /* Subsys ID */
  ScmParam[0] = 0x13;
  /* Start address of the image to be loaded */
  ScmParam[1] = imageBase; 
  /* Length of the image to be loaded */
  ScmParam[2] = (UINT64)imageSize;

  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  /*
   * Confirms the memory address to be loaded is different
   * than in the ELF
   */
  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_PIL_MEM_ID,
                                 TZ_PIL_MEM_ID_PARAM_ID,
                                 ScmParam,
                                 ScmResult);
  return Status;
}

STATIC inline EFI_STATUS gpi_tz_pil_auth_reset_id(void)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;

  /* Subsys ID */
  ScmParam[0] = 0x13;

  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  /*
   * Authenticate and load the image
   */
  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_PIL_AUTH_RESET_ID,
                                 TZ_PIL_AUTH_RESET_ID_PARAM_ID,
                                 ScmParam,
                                 ScmResult);

  return Status;
}

STATIC inline EFI_STATUS gpi_tz_pil_unlock_xpu_id(void)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;

  /* Subsys ID */
  ScmParam[0] = 0x13;

  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  /*
   * Unlock the XPU and free the memory
   */
  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_PIL_UNLOCK_XPU_ID,
                                 TZ_PIL_UNLOCK_XPU_ID_PARAM_ID,
                                 ScmParam,
                                 ScmResult);
  
  return Status;
}

#ifdef ENABLE_AUTO_BOOT_OPTIMIZATION

STATIC inline EFI_STATUS gpi_tz_pil_load_qup_fw(void)
{
  EFI_STATUS Status = EFI_LOAD_ERROR;

  /* Subsys ID */
  ScmParam[0] = TZ_PIL_AUTH_GSI_QUP_PROC;
  ScmParam[1] = TZBSP_BLSP_SCM_CALL_FROM_GPI_DXE;

  if (QcomScmProtocol == NULL)
  {
    return Status;
  }

  /*
   * Load the QUP FW Image in TZ
   */
  Status = QcomScmProtocol->ScmSipSysCall(QcomScmProtocol,
                                 TZ_RESTORE_SEC_CFG,
                                 TZ_RESTORE_SEC_CFG_PARAM_ID,
                                 ScmParam,
                                 ScmResult);
  
  return Status;
}

#endif /* ENABLE_AUTO_BOOT_OPTIMIZATION */


EFI_STATUS gpi_load_firmware(void)
{

#ifdef ENABLE_AUTO_BOOT_OPTIMIZATION

  EFI_STATUS                Status = EFI_SUCCESS;

  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID**)&QcomScmProtocol);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed to locate QcomScmProtocol\r\n", __func__));
    return Status;
  }

  return gpi_tz_pil_load_qup_fw();

#else /* ENABLE_AUTO_BOOT_OPTIMIZATION */

  VOID                    * ElfBase = NULL;
  UINT8                   * MetaBase = NULL;
  size_t                    MetaBufferSize = 0;
  VOID                    * ProgSegBase = NULL;
  UINT32                    ProgSegSize = 0;
  EFI_BLOCK_IO_PROTOCOL   * pBlkIo;
  EFI_SHMBRIDGE_PROTOCOL  * ShmBridgeProtocol = NULL;
  UINTN                     ImgSize;
  UINT32                    Attribs = 0;
  HandleInfo                HandleInfoList[4];
  UINT32                    MaxHandles;
  PartiSelectFilter         HandleFilter;
  EFI_STATUS                Status = EFI_SUCCESS;

  /* Select the BlkIo handle that represents the partition by the
   * referenced GUID type in GPT partition on Non removable media
   * ie UFS device */
  Attribs |= BLK_IO_SEL_MEDIA_TYPE_NON_REMOVABLE;
  Attribs |= BLK_IO_SEL_PARTITIONED_GPT;
  Attribs |= BLK_IO_SEL_MATCH_PARTITION_TYPE_GUID;

  MaxHandles = sizeof(HandleInfoList)/sizeof(*HandleInfoList);
  HandleFilter.PartitionType = &gEfiQupv3FirmwareElfTypeGuid;
  HandleFilter.VolumeName = 0;

#ifdef ENABLE_AUTO_PLAT
  /* Use GetBlkIOHandlesExt to load in sepfic LUN for UFS */
  Status = GetBlkIOHandlesExt (Attribs, &HandleFilter, HandleInfoList, &MaxHandles);
  
#else
  Status = GetBlkIOHandles (Attribs, &HandleFilter, HandleInfoList, &MaxHandles);

#endif
  
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: GetBockIOHandles Failed\r\n", __func__));
    return Status;
  }
  /* Having more than one partition is considered ambiguity, so return
   * invalid param */
  if (MaxHandles > 1)
  {
    DEBUG(( EFI_D_WARN, "%a: QUPV3 partition handle invalid\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  if (MaxHandles == 0)
  {
    DEBUG(( EFI_D_WARN, "%a: QUPV3 ELF partition not found\r\n", __func__));
    return EFI_NOT_FOUND;
  }

  /* Get the Block IO protocol instance from the handle */
  pBlkIo = HandleInfoList[0].BlkIo;

  /* Get the partition size and round it up to EFI_PAGE_SIZE */
  ImgSize = (pBlkIo->Media->LastBlock + 1) * pBlkIo->Media->BlockSize;

  /* Allocate page aligned buffer for ELF buffer from flash */

  Status = gBS->LocateProtocol(&gEfiShmBridgeProtocolGuid,
                               NULL,
                               (VOID **)&ShmBridgeProtocol);
  if (Status != EFI_SUCCESS)
  {
    DEBUG(( EFI_D_WARN,
            "%a: Failed to locate EFI_SHMBRIDGE_PROTOCOL\r\n", __func__));
    return Status;
  }

  ElfBase = ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &ImgSize, ShmBridgeBootSvcData);
  if (ElfBase == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Could not allocate uncached mem\r\n", __func__));
    return EFI_OUT_OF_RESOURCES;
  }

  /*
   * Load the ELF image from flash partition to DDR
   */
  Status = pBlkIo->ReadBlocks (pBlkIo, pBlkIo->Media->MediaId, 0, ImgSize, ElfBase);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Could not read ELF from UFS partition\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    return Status;
  }

  /*
   * This function finds the meta buffer size
   */
  Status = prep_elf_parsing(ElfBase, &MetaBufferSize);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed to get meta buffer size\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    return Status;
  }

  if (MetaBufferSize == 0)
  {
    DEBUG(( EFI_D_WARN, "%a: Invalid Meta Buffer Size\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    return EFI_OUT_OF_RESOURCES;
  }

  MetaBase = (UINT8 *)ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol,
                                                           &MetaBufferSize,
                                                           ShmBridgeBootSvcData);
  if (MetaBase == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Could not allocate uncached mem\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    return EFI_OUT_OF_RESOURCES;
  }

  /*
   * This function will take ELF buffer base as input and perform:
   *
   * 1. Parse the ELF buffer
   * 2. Create Meta buffer from the ELF buffer 
   * 3. Provide the base address for program segment within ELF buffer
   */
  Status = efi_split_elf_file(ElfBase, MetaBase, &ProgSegBase, &ProgSegSize);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed to parse ELF successfully\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, (VOID *)MetaBase);
    return Status;
  }

  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID**)&QcomScmProtocol);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed to locate QcomScmProtocol\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, (VOID *)MetaBase);
    return Status;
  }

  /* TZ PIL init */
  Status = gpi_tz_pil_init_id((UINT64)MetaBase);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed TZ_PIL_INIT\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, (VOID *)MetaBase);
    return Status;
  }

  /*
   * Assign prog seg size till the end of ELF from the first prog seg base.
   * We are calculating this here because, the program segment should start
   * and end at 4K aligned address for XPU protection reasons
   */
  ProgSegSize = (ImgSize - ((uint8 *)ProgSegBase - (uint8 *)ElfBase));

  /* TZ PIL mem */
  Status = gpi_tz_pil_mem_id((UINT64)ProgSegBase, ProgSegSize);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed TZ_PIL_MEM\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, (VOID *)MetaBase);
    return Status;
  }

  /* TZ PIL auth reset */
  Status = gpi_tz_pil_auth_reset_id();
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed TZ_PIL_AUTH_RESET\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, (VOID *)MetaBase);
    return Status;
  }

  /* TZ PIL unlock xpu */
  Status = gpi_tz_pil_unlock_xpu_id();
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed TZ_PIL_UNLOCK_XPU\r\n", __func__));
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, ElfBase);
    ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, (VOID *)MetaBase);
    return Status;
  }

  return Status;

#endif /* ENABLE_AUTO_BOOT_OPTIMIZATION */

}
