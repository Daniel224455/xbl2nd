/*=============================================================================
 
  File: MDPSystem.c
 
  Source file for MDP functions
  
 
  Copyright (c) 2011-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ArmLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/RamPartitionTableLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/GuidedFvLib.h>
#include <Library/KernelLib.h>
#include <Protocol/EFIHALIOMMUProtocol.h>
#include <Protocol/EFIDDRGetConfig.h>
#include <Protocol/EFIPIL.h>
#include "MDPSystem.h"
#include "HALhwio.h"
#include "MDPLib_i.h"
#include "MDPSystem.h"
#include "MDPPlatformLib.h"
#include "DisplayUtils.h"
#include "displaydiag.h"

/*=========================================================================
     Default Defines
==========================================================================*/
// Maximum debug output string length
#define DISP_MAX_DEBUG_MESSAGE_LENGTH   100

// .DevObjectName for MDP in Settings\SMMU\iort.h
#define MMU_SID_MDP_OBJECT_NAME         "\\_SB_.MDP"

// ARID value for MDP: .SIDMappings[0].InputBase for MDP in Settings\SMMU\iort.h
#define MMU_SID_MDP_ARID                0

// SMR Mask for MDP to route all MDP SIDs to a single Context Bank (from sid.mapping in ipcat)
#define MMU_SID_MDP_SMR_MASK            0x420

/* -----------------------------------------------------------------------
** Types
** ----------------------------------------------------------------------- */

/*=========================================================================
     Global Variables
==========================================================================*/
/* Display Variable Table
 */
DisplayVariableType gDisplayVariableTable[MDP_VAR_TABLE_MAX_SIZE];

/*=========================================================================
     Local Static Variables
==========================================================================*/

/*=========================================================================
     Local Static Functions
==========================================================================*/
/*
Reserved memory allocation
*/
static MDP_Status MDPAllocateReservedMemory(EFIChipInfoFamilyType eChipId, uint64 *pReservedMemoryAddress, uint64 *pReservedMemorySize);

/* Locks used to prevent re-entrance */
static MDP_LockInfo sLockInfo;

/* Display diagnostic buffer */
DisplayDiagLogType  DISPLAY_DIAG_LOG_BUFFER;


/* Query number of DDR channels for the platform */
static void GetMaxDDRChannels(uint32 *pNumChannels);

/* Flag to set multi thread state */
static bool32 bDisplayMultiThreadded = FALSE;

/* Function to cache display variables */
static MDP_Status DisplayCacheVariables(CHAR16                *pVariableName,
                                        void                  *pVariableValue,
                                        UINTN                  uDataSize,
                                        MDPVariableAttribType  eAttrib);

/* Function to save display variables */
static EFI_STATUS DisplaySaveVariables(CHAR16                *pVariableName,
                                       void                  *pVariableValue,
                                       UINTN                  uDataSize,
                                       MDPVariableAttribType  eAttrib,
                                       UINT32                 uFlags);


/*=========================================================================
      Public APIs
==========================================================================*/
extern EFI_GUID gQcomTokenSpaceGuid;

/****************************************************************************
*
** FUNCTION: MDP_Log_Message()
*/
/*!
* \brief
*   This function will perform the logging of Debug messages
*
* \param [in] uLogType   - Log level
*        [in] pLogString - Log String
*
* \retval void
*
****************************************************************************/
void MDP_Log_Message(MDP_LogLevel uLogType, char *pLogString, ...)
{
  CHAR8    buffer[DISP_MAX_DEBUG_MESSAGE_LENGTH];
  VA_LIST  marker;
  UINT32   uLogLevel;
  switch (uLogType)
  {
  case MDP_LOGLEVEL_INFO:
    uLogLevel = EFI_D_INFO;
    break;
  case MDP_LOGLEVEL_WARN:
    uLogLevel = EFI_D_WARN;
    break;
  case MDP_LOGLEVEL_ERROR:
  default:
    uLogLevel = EFI_D_ERROR;
    break;
  }
  //
  // Convert the DEBUG() message to a Unicode String
  //
  VA_START (marker, pLogString);
  AsciiVSPrint(buffer, DISP_MAX_DEBUG_MESSAGE_LENGTH, pLogString, marker);
  VA_END (marker);

  DEBUG ((uLogLevel, buffer));
}

/****************************************************************************
*
** FUNCTION: MDP_ReadUnaligned_U32()
*/
/*!
* \brief
* This function reads the uint16 values from aligned boundary and packs them into uint32
*
* if pSrc is at offset of 1 or 3 from the uint32 aligned address
*            reads uint16 from aligned (pSrc -1) address and extracts the LSB 
*            reads next uint16 addresses and 
*            reads the next +1 uint16 address and extracts the MSB
*            Packs these into uint32 pDest address
*
*if pSrc is at offset 2 from the the uint32 aligned address
*            reads uint16 from aligned (pSrc -1) address 
*            reads next uint16 addresses 
*            Packs these into uint32 pDest addr
*
* Otherwise pSrc is on aligned address boundary

*
* \param [in] pDest   - pointer to Uint32
*        [in] pSrc    - pointer to Uint8 
*
* \retval void
*
****************************************************************************/
void MDP_ReadUnaligned_U32(uint32 *pDest, uint8 * pSrc)
{
    // if pSrc is at offset of 1 or 3 from the uint32 aligned address
    if (0 != ((UINTN)pSrc & 0x1) ) 
    {
        uint16 *pTemp = (uint16*)((uint8 *)(pSrc) - 1); 

         // Pack these into uint32 pDest address
        *pDest = (uint8)(*(pTemp + 2) &  0xff) << 24 | (uint16)(*(pTemp + 1) << 16) | (uint8)(*pTemp >> 8); 
    } 
    // if pSrc is at offset 2 from the the uint32 aligned address
    else if (0 != ((UINTN)(pSrc) & 0x2) )
    { 
       uint16 *pTemp = (uint16*)pSrc; 

       // Packs these into uint32 pDest addr
       *pDest =((uint16)*(pTemp + 1) << 16) | (uint16)*pTemp ;
    }
    // Otherwise pSrc is on aligned address boundary
    else 
    {
        *pDest = *(uint32 *)pSrc; 
    }
}


/****************************************************************************
*
** FUNCTION: MDP_OSAL_MB()
*/
/*!
* \brief
*   This function creates a hardware memory barrier (fence) that prevents the CPU from 
*  re-ordering read and write operations.
*
* \param void
*        
*
* \retval void
*
****************************************************************************/
void MDP_OSAL_MB(void)
{
    ArmDataMemoryBarrier();
}


/****************************************************************************
*
** FUNCTION: MDP_SetFirmwareEnvironmentVariable()
*/
/*!
* \brief
*   Program the platform environment variable
*
* \param [in] pValue         - Environment variable 
*        [in] uSize          - Environment variable size 
*
* \retval MDP_STATUS_OK on success
*
****************************************************************************/
MDP_Status MDP_SetFirmwareEnvironmentVariable(void *pValue, UINT32 uSize)
{
  MDP_Status  eStatus  = MDP_STATUS_OK;

  if (EFI_SUCCESS == DisplaySaveVariables(DISPVAR_UEFI_DISPLAY_INFO,
                                          pValue,
                                          uSize,
                                          MDP_VARIABLE_ATTRIB_FIRMWARE_ENV,
                                          0))
  {
    eStatus = MDP_STATUS_OK;
  }
  else if (MDP_STATUS_OK == DisplayCacheVariables(DISPVAR_UEFI_DISPLAY_INFO, pValue, uSize, MDP_VARIABLE_ATTRIB_FIRMWARE_ENV))
  {
    // Cache display variable successfully
    eStatus = MDP_STATUS_OK;
  }
  else
  {
    DEBUG ((EFI_D_WARN, "MDPSystem: Failed to update UEFI Environment variable (UEFIDisplayInfo)!\n"));
    eStatus = MDP_STATUS_FAILED;
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: MDP_GetEnvironmentVariable()
*/
/*!
* \brief
*   Helper to retrieve a system environment variable.
*
* \param [in]  pVariableName - Environment variable name
*        [out] pValue        - Variable storage
*        [in]  uSize         - Size of the environment variable
*
* \retval MDP_STATUS_OK on success
*
****************************************************************************/
MDP_Status MDP_GetEnvironmentVariable(CHAR16 *pVariableName, void *pValue, UINT32 uSize)
{
  MDP_Status eStatus      = MDP_STATUS_OK;
  UINT32     uAttributes  = 0;
  UINT8      uStorage[MDP_OSAL_MAX_ENVVAR_SIZE];
  UINTN      uStorageSize = MDP_OSAL_MAX_ENVVAR_SIZE;  

  if (uSize > MDP_OSAL_MAX_ENVVAR_SIZE)
  {
   eStatus = MDP_STATUS_FAILED;  
  }
  else if (EFI_SUCCESS == gRT->GetVariable((CHAR16*)pVariableName, 
                                      &gQcomTokenSpaceGuid,
                                      &uAttributes,
                                      (UINTN*)&uStorageSize,
                                      (void*)uStorage))
  {
    MDP_OSAL_MEMCPY(pValue, &uStorage, uSize);
  }
  else
  {
    eStatus = MDP_STATUS_FAILED;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_GetBootServiceVariableSize()
*/
/*!
* \brief
*   Helper to retrieve data size for system variable from the QcomTokenSpace GUID.
*
* \param [in]  pVariableName - Environment variable name
*        [out] puSize        - Size of the environment variable
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS MDP_GetBootServiceVariableSize(CHAR16 *pVariableName, UINTN *puSize)
{
  void       *pValue       = NULL;
  UINTN       uSize        = 0;
  EFI_STATUS  eStatus;

  if ((NULL == pVariableName) ||
      (NULL == puSize))
  {
    eStatus = EFI_INVALID_PARAMETER;
  }
  else
  {
    /* 
    * GetVariable call with NULL pointer for data will return the actual size
    * and EFI_BUFFER_TOO_SMALL error, if the variable exists
    */
    eStatus = gRT->GetVariable(pVariableName, 
                              &gQcomTokenSpaceGuid,
                               0,
                              &uSize,
                               pValue);

    if (EFI_BUFFER_TOO_SMALL == eStatus)
    {
      /* This is the expected result. Set the size and update return status */
      *puSize = uSize;
      eStatus = EFI_SUCCESS;
    }
    else
    {
      DEBUG((EFI_D_INFO, "MDPSystem: Failed to get size for variable <%s>. Error: %r\n", pVariableName, eStatus));
    }
    
  }
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_GetBootServiceVariable()
*/
/*!
* \brief
*   Helper to retrieve a system environment variable from the QcomTokenSpace GUID.
*
* \param 
*        [in]   pVariableName - Environment variable name
*        [out]  pValue        - Variable storage
*        [out]  uSize         - Size of the environment variable
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS MDP_GetBootServiceVariable(CHAR16 *pVariableName, void *pValue, UINTN *puSize)
{
  UINT32     uAttributes  = 0;
  EFI_STATUS eStatus;

  if ((NULL == pVariableName) ||
      (NULL == pValue)        ||
      (NULL == puSize))
  {
    DEBUG((EFI_D_ERROR, "MDPSystem: Null parameter\n"));
    eStatus = EFI_INVALID_PARAMETER;
  }
  else if (EFI_SUCCESS != (eStatus = gRT->GetVariable(pVariableName,
                                                     &gQcomTokenSpaceGuid,
                                                     &uAttributes,
                                                      puSize,
                                                      pValue)))
  {
    /* No error message if variable not found */
    if (EFI_NOT_FOUND != eStatus)
    {
      DEBUG((EFI_D_WARN, "MDPSystem: Failed to get variable <%s>. Error: %r\n", pVariableName, eStatus));
    }
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: MDP_SetBootServiceVariable()
*/
/*!
* \brief
*   Helper to set a system environment variable using the QcomTokenSpace GUID.
*
* \param [in]  pVariableName - Environment variable name
*        [in]  pValue        - Variable storage
*        [in]  uSize         - Size of the environment variable
*        [in]  uFlags        - the MDP variable flags
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS MDP_SetBootServiceVariable(CHAR16 *pVariableName, void *pValue, UINTN uSize, UINT32 uFlags)
{
  EFI_STATUS eStatus = EFI_SUCCESS;

  if (EFI_SUCCESS == DisplaySaveVariables(pVariableName, pValue, uSize, MDP_VARIABLE_ATTRIB_BOOT_SERVICE, 0))
  {
    eStatus = EFI_SUCCESS;
  }
  else if (MDP_STATUS_OK == DisplayCacheVariables(pVariableName, pValue, uSize, MDP_VARIABLE_ATTRIB_BOOT_SERVICE))
  {
    // Cache display variable successfully
    eStatus = EFI_SUCCESS;
  }
  else
  {
    DEBUG((EFI_D_WARN, "MDPSystem: Failed to set display variable <%s>\n", pVariableName));
    eStatus = EFI_LOAD_ERROR;
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: MDP_FlushCachedVariables()
*/
/*!
* \brief
*   Function to flush the cached variables.
*
  @param[in]   None
*
* \retval      None
*
****************************************************************************/
void MDP_FlushCachedVariables(void)
{
  UINT32  uVarIndex  = 0;

  // Variable service is not ready, cache the variable.
  for (uVarIndex=0; uVarIndex<MDP_VAR_TABLE_MAX_SIZE; uVarIndex++)
  {
    if ((gDisplayVariableTable[uVarIndex].pVariableName   != NULL)  &&
        (gDisplayVariableTable[uVarIndex].pVariableValue  != NULL)  &&
        (gDisplayVariableTable[uVarIndex].uDataSize       != 0))
    {
      DisplaySaveVariables(gDisplayVariableTable[uVarIndex].pVariableName,
                           gDisplayVariableTable[uVarIndex].pVariableValue,
                           gDisplayVariableTable[uVarIndex].uDataSize,
                           gDisplayVariableTable[uVarIndex].eAttrib,
                           0);
    }
  }
}


/****************************************************************************
*
** FUNCTION: MDP_SetupMMUSIDs()
*/
/*!
* \brief
*   Helper to program Display MMU SID to bypass
*
* \param None
*
* \retval MDP_STATUS_OK on success
*
****************************************************************************/
MDP_Status MDP_SetupMMUSIDs(void)
{
  MDP_Status                   eStatus        = MDP_STATUS_FAILED;
  EFI_HAL_IOMMU_PROTOCOL      *pIOMMUProtocol = NULL;
  HAL_IOMMU_fptable_t         *pSMMUTable     = NULL;
  void                        *pMDPDomain     = NULL;

  if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiHalIommuProtocolGuid, NULL, (void **)&pIOMMUProtocol))
  {
    DEBUG((EFI_D_WARN, "MDPLib: Unable to locate IOMMU protocol.\n"));
  }
  else
  {
    pIOMMUProtocol->_HAL_IOMMU_protocol_init((HAL_IOMMU_fptable_t **)&pSMMUTable);
    if (NULL == pSMMUTable) {
      DEBUG((EFI_D_WARN, "MDPLib: Unable to initialize IOMMU protocol.\n"));
    }
    else if (HAL_IOMMU_ERR_OK != pSMMUTable->domain_create_func(&pMDPDomain))
    {
      DEBUG((EFI_D_WARN, "MDPLib: Unable to create domain in IOMMU\n"));
    }
    else if (HAL_IOMMU_ERR_OK != pSMMUTable->config_bypass_domain_func(pMDPDomain,
                                                                       HAL_IOMMU_AARCH64_NON_CCA_DOMAIN_TYPE))
    {
      DEBUG((EFI_D_WARN, "MDPLib: Unable to configure bypass domain in IOMMU\n"));
    }
    else if (HAL_IOMMU_ERR_OK != pSMMUTable->domain_attach_func(pMDPDomain,
                                                                (void *) MMU_SID_MDP_OBJECT_NAME,
                                                                MMU_SID_MDP_ARID,
                                                                MMU_SID_MDP_SMR_MASK))
    {
      DEBUG((EFI_D_WARN, "MDPLib: Unable to attach MDP to IOMMU\n"));
    }
    else
    {
      eStatus = MDP_STATUS_OK;
    }
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_OSAL_MEMZERO()
*/
/*!
* \brief
*   Fills a target buffer with 0 value
*
* \param [in] pBuffer   - The memory to set
*        [in] uLength   - The number of bytes to set
*
* \retval void
*
****************************************************************************/
void MDP_OSAL_MEMZERO(void *pBuffer, UINTN uLength)
{
  gBS->SetMem(pBuffer, uLength, 0x0);
}


/****************************************************************************
*
** FUNCTION: MDP_OSAL_MEMCPY()
*/
/*!
* \brief
*   Copies a source buffer to a destination buffer
*
* \param [in] pDest   - The pointer to the destination buffer of the memory copy
*        [in] pSrc    - The pointer to the source buffer of the memory copy.
*        [in] uLength - The number of bytes to copy from SourceBuffer to DestinationBuffer.

*
* \retval void
*
****************************************************************************/
void MDP_OSAL_MEMCPY(void *pDest, void *pSrc, UINTN uLength)
{
  CopyMem(pDest, pSrc, uLength);
}

/****************************************************************************
*
** FUNCTION: MDP_OSAL_MEMSET()
*/
/*!
* \brief
*   Fills a target buffer with a byte value
*
* \param [in] pBuffer - The memory to set
*        [in] uLength - The number of bytes to set
*        [in] uValue  - The value with which to fill Length bytes of Buffer
*
* \retval void
*
****************************************************************************/
void MDP_OSAL_MEMSET(void  *pBuffer,  UINT8  uValue, UINTN  uLength)
{
  gBS->SetMem(pBuffer, uLength, uValue);
}


/****************************************************************************
*
** FUNCTION: MDP_OSAL_CALLOC()
*/
/*!
* \brief
*   Allocate memory from local heap
*
* \param [in] uLength   - Requested size of memory to be allocated
*
* \retval point to the block of allocated memory
*
****************************************************************************/
void* MDP_OSAL_CALLOC(UINTN uLength)
{
  return UncachedAllocateZeroPool(uLength);
}

/****************************************************************************
*
** FUNCTION: MDP_OSAL_FREE()
*/
/*!
* \brief
*   Free allocated memory from local heap
*
* \param [in] pBuffer   - Pointer to block of memory allocated
*
* \retval void
*
****************************************************************************/
void MDP_OSAL_FREE(void* pBuffer)
{
  UncachedSafeFreePool(pBuffer);
}


/****************************************************************************
*
** FUNCTION: MDP_OSAL_DELAYMS()
*/
/*!
* \brief
*   This function will sleep for a given time in milliseconds
*
* \param [in] uDelayMs   - Sleep time in milliseconds
*
* \retval void
*
****************************************************************************/
void MDP_OSAL_DELAYMS(UINTN uDelayMs)
{
  if (TRUE == bDisplayMultiThreadded)
  {
    ThreadSleep (uDelayMs);
  }
  else
  {
    gBS->Stall((uint32)(uDelayMs)* 1000);
  }
}


/****************************************************************************
*
** FUNCTION: MDP_OSAL_DELAYUS()
*/
/*!
* \brief
*   This function will sleep for a given time in microsecond
*
* \param [in] uDelayUs   - Sleep time in microsecond
*
* \retval void
*
****************************************************************************/
void MDP_OSAL_DELAYUS(UINTN uDelayUs)
{
  gBS->Stall((uint32)(uDelayUs));
}

/****************************************************************************
*
** FUNCTION: MDP_SaveFirmwareEnvironmentVariable()
*/
/*!
* \brief
*   This function will save display info into UEFI environment variable
*
* \param [in] eDisplayId        - Display ID
*
* \retval void
*
****************************************************************************/
MDP_Status MDP_SaveFirmwareEnvironmentVariable(MDP_Display_IDType eDisplayId)
{
  MDP_Status          eStatus          = MDP_STATUS_OK;
  MDP_Panel_AttrType *pDisplayInfo     = MDP_GET_DISPLAYINFO(eDisplayId);
  MDPSurfaceInfo     *pFrameBufferInfo = &pDisplayInfo->sFrameBuffer;
  uint32              uBacklightLevel  = pDisplayInfo->uBacklightLevel;
  MDPPlatformParams   sPlatformParams;

  if (MDP_STATUS_OK == (eStatus = MDPPlatformConfigure(eDisplayId, MDPPLATFORM_CONFIG_GETPLATFORMINFO, &sPlatformParams)))
  {
    UINT32                      FrameBufferSize       = 0;
    UINT8                       uHighestBankBit       = 0;           // 0 is defined to be error value with GFX
    UINT32                      uMaxDDRChannels       = 0;           // 0 is defined to be error value 
    CHAR8*                      MemLabel              = "Display Reserved";
    MemRegionInfo               DisplayMemRegionInfo;
    MDPFirmwareEnvType          sFirmwareEnvInfo;
    EFI_STATUS                  Status;
    
    MDP_OSAL_MEMZERO(&sFirmwareEnvInfo, sizeof(MDPFirmwareEnvType));

    // Get the Display region.
    Status = GetMemRegionInfoByName(MemLabel, &DisplayMemRegionInfo);
    if (Status != EFI_SUCCESS)
    {
      DEBUG((EFI_D_WARN, "MDPLib: Could not find the Display memory region. Defaulting to 8MB.\n"));
      FrameBufferSize = 0x00800000; /* Default to 8MB */
    }
    else
    {
      FrameBufferSize = DisplayMemRegionInfo.MemSize;
    }

    if (EFI_SUCCESS != (Status = GetHighestBankBit(&uHighestBankBit)))
    {
      DEBUG((EFI_D_WARN, "MDPLib: GetHighestBankBit failed.\n"));
    }

    // Query DDR channel count
    GetMaxDDRChannels(&uMaxDDRChannels);

    // Populate firmware environment information
    sFirmwareEnvInfo.uVersionInfo = (MDP_FIRMWARE_ENV_VERSION_MAGIC << 16) |
                                    (MDP_FIRMWARE_ENV_VERSION_MAJOR << 8) |
                                    (MDP_FIRMWARE_ENV_VERSION_MINOR);
    sFirmwareEnvInfo.uPlatformId      = sPlatformParams.sPlatformInfo.sEFIPlatformType.platform;
    sFirmwareEnvInfo.uChipsetId       = sPlatformParams.sPlatformInfo.sEFIChipSetId;
    sFirmwareEnvInfo.uPlatformVersion = 0; // Reserved
                                           // Frame buffer information comes from the currently cached frame buffer
    sFirmwareEnvInfo.uFrameBufferAddress      = (UINTN)pFrameBufferInfo->pPlane0Offset;
    // We report the frame buffer size as the carve out memory size which is sum of PcdFrameBufferSize and PcdHDMISize
    sFirmwareEnvInfo.uFrameBufferSize         = FrameBufferSize;
    sFirmwareEnvInfo.uFrameBufferPixelDepth   = (8 * pFrameBufferInfo->uPlane0Stride) / pFrameBufferInfo->uWidth;
    sFirmwareEnvInfo.uFrameBufferImagePxWidth = pFrameBufferInfo->uWidth;
    sFirmwareEnvInfo.uFrameBufferImagePxHeight= pFrameBufferInfo->uHeight;
    sFirmwareEnvInfo.uFrameBufferImageStride  = pFrameBufferInfo->uPlane0Stride;
    sFirmwareEnvInfo.uHighestBankBit          = uHighestBankBit;
    sFirmwareEnvInfo.uMaxDDRChannels          = uMaxDDRChannels;

    // Populate panel information
    if (MDP_DISPLAY_INTERNAL(eDisplayId))
    {
      // Allocate any required reserved regions, if not allocated already.
      if (0 == pDisplayInfo->uReservedAllocationSize)
      {
        if (MDP_STATUS_OK == MDPAllocateReservedMemory(sPlatformParams.sPlatformInfo.sEFIChipSetFamily, &sFirmwareEnvInfo.uReservedMemoryAddress, &sFirmwareEnvInfo.uReservedMemorySize))
        {
          // Keep track of the allocation size so we don't allocate it again
          pDisplayInfo->uReservedAllocationSize = sFirmwareEnvInfo.uReservedMemorySize;
        }
        else
        {
          // Reset values on failure.
          sFirmwareEnvInfo.uReservedMemoryAddress = 0x0;
          sFirmwareEnvInfo.uReservedMemorySize    = 0x0;
        }
      }
    }

    // If panel configuration has provided the panel id, then override panel id with this value
    // This is required when panel supports multiple modes with same panel id, by overriding 
    // the panel id for a particular configuration HLOS can identify that specifc mode of panel
    if (MDP_DISPLAY_PRIMARY == eDisplayId)
    {
      sFirmwareEnvInfo.uPrimaryPanelId               = (0 != pDisplayInfo->uPanelId) ? pDisplayInfo->uPanelId : sPlatformParams.sPlatformInfo.uPrimaryPanelId;
      sFirmwareEnvInfo.uPrimaryPanelFlags            = MDP_FIRMWARE_ENV_FLAG_DISPLAY_INITIALIZED;
      sFirmwareEnvInfo.uPrimaryPanelBacklightLevel   = uBacklightLevel;
    }
    else if (MDP_DISPLAY_SECONDARY == eDisplayId)
    {
      sFirmwareEnvInfo.uSecondaryPanelId             = (0 != pDisplayInfo->uPanelId) ? pDisplayInfo->uPanelId : sPlatformParams.sPlatformInfo.uPrimaryPanelId;
      sFirmwareEnvInfo.uSecondaryPanelFlags          = MDP_FIRMWARE_ENV_FLAG_DISPLAY_INITIALIZED;
      sFirmwareEnvInfo.uSecondaryPanelBacklightLevel = uBacklightLevel;
    }

    eStatus = MDP_SetFirmwareEnvironmentVariable(&sFirmwareEnvInfo, sizeof(sFirmwareEnvInfo));
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDPAcquireLockOrFail()
*/
/*!
* \brief
*   This function will try to get the specified lock.
*
* The function will acquire the lock if not already acquired
* It will return FAIL if lock is already acquired
*
* \param  eLock              - Lock to acquire
*
* \retval MDP_STATUS_OK      - Lock acquired successfully
* \retval MDP_STATUS_FAILED  - Failed to acquire Lock
*
****************************************************************************/
MDP_Status  MDPAcquireLockOrFail(MDP_Lock_Type eLock)
{
  MDP_Status eStatus = MDP_STATUS_OK;

  if (MDP_LOCK_TYPE_MAX <= eLock)
  {
    eStatus = MDP_STATUS_BAD_PARAM;
  }
  else
  {
    volatile uint32 *pLock = &sLockInfo.uLock[eLock];

    if (*pLock)
    {
      // Failed to acquire lock
      eStatus = MDP_STATUS_FAILED;
    }
    else
    {
      *pLock = 1;
    }
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDPReleaseLock()
*/
/*!
* \brief
*   This function will release the specified lock.
*
* \param  eLock          - Lock to release
*
* \retval MDP_STATUS_OK  - Lock released successfully
*
****************************************************************************/
MDP_Status  MDPReleaseLock(MDP_Lock_Type eLock)
{
  MDP_Status eStatus = MDP_STATUS_OK;

  if (MDP_LOCK_TYPE_MAX <= eLock)
  {
    eStatus = MDP_STATUS_BAD_PARAM;
  }
  else
  {
    volatile uint32 *pLock = &sLockInfo.uLock[eLock];

    // Always be able to release
    *pLock = 0;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_GetConfigValue()
*/
/*!
* \brief
*   Get config value created by parsing the platform config file
*
* \param [in] Key         - Pointer to ASCII parameter to search for in config file
*        [in] Value       - On input pointer to buffer used to return value
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status MDP_GetConfigValue(IN CHAR8* Key, IN OUT UINT32* Value)
{
  MDP_Status eStatus = MDP_STATUS_OK;

  if (EFI_SUCCESS != GetConfigValue(Key, Value))
  {
    eStatus = MDP_STATUS_FAILED;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_GetTimerCountMS()
*/
/*!
* \brief
*    Running time in mS based on the Arch Timer Counter.
*    This could get wrapped around based on the resolution and Speed.So
*    use with caution.
*    If the Timer is 32 bit running at 19.2 MHz, it would wrap around, to 0
*    after 223 Sec
*
* \param NONE  - Log level
*
* \retval Timer count
*
****************************************************************************/
UINT32  MDP_GetTimerCountMS(void)
{
  return GetTimerCountms();
}

/****************************************************************************
*
** FUNCTION: MDP_GetTimerCountUS()
*/
/*!
* \brief
*    Running time in uS based on the Arch Timer Counter.
*    This could get wrapped around based on the resolution and Speed.So
*    use with caution.
*    If the Timer is 32 bit running at 19.2 MHz, it would wrap around, to 0
*    after 223 Sec
*
* \param NONE  - Log level
*
* \retval Timer count
*
****************************************************************************/
UINT32     MDP_GetTimerCountUS(void)
{
  return GetTimerCountus();
}

/****************************************************************************
*
** FUNCTION: MDP_SetMultiThreadState()
*/
/*!
* \brief
*   This function will set multithreadded state to be enabled or not.
*
* \param [in]  bEnable            - Multithreadded state.
*
* \retval None
*
****************************************************************************/
MDP_Status MDP_SetMultiThreadState(bool32 bEnable)
{
  MDP_Status  eStatus          = MDP_STATUS_OK;
  UINT32      uMultiThreadded  = 0;

  if (EFI_SUCCESS != GetConfigValue ("EnableDisplayThread", &uMultiThreadded))
  {
    eStatus = MDP_STATUS_NOT_SUPPORTED;
  }
  else if (0 == uMultiThreadded)
  {
    eStatus = MDP_STATUS_NOT_SUPPORTED;
  }
  else
  {
    bDisplayMultiThreadded = bEnable;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_Display_SetVariable()
*/
/*!
* \brief
*   Function to set the display variable.
*
  @param[in]  pVariableName              The variable name that need to set.
  @param[in]  pVariableValue             The variable value.
  @param[in]  uDataLength                The length of the variable value.
  @param[in]  uFlags                     The flag to set variable.
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status MDP_Display_SetVariable (CHAR16 *pVariableName, UINT8 *pVariableValue, UINTN uDataLength, UINTN uFlags)
{
  MDP_Status  eStatus = MDP_STATUS_OK;

  if (EFI_SUCCESS != Display_Utils_SetVariable(pVariableName, pVariableValue, uDataLength, uFlags))
  {
    eStatus = MDP_STATUS_FAILED;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_Display_GetVariable()
*/
/*!
* \brief
*   Function to get the display variable.
*
  @param[in]   pVariableName              The variable name that need to get.
  @param[out]  pVariableValue             The variable value.
  @param[out]  pDataLength                The length of the variable value.
  @param[in]   uFlags                     The flag to get variable.
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status MDP_Display_GetVariable (CHAR16 *pVariableName, UINT8 *pVariableValue, UINTN *pDataLength, UINTN uFlags)
{
  MDP_Status  eStatus = MDP_STATUS_OK;

  if (EFI_SUCCESS != Display_Utils_GetVariable(pVariableName, pVariableValue, pDataLength, uFlags))
  {
    eStatus = MDP_STATUS_FAILED;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_Display_SetVariable_Integer()
*/
/*!
* \brief
*   Function to set the display variable.
*
  @param[in]  pVariableName              The variable name that need to set.
  @param[in]  uVariableValue             The variable value.
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status MDP_Display_SetVariable_Integer (CHAR16 *pVariableName, UINT32 uVariableValue)
{
  MDP_Status  eStatus       = MDP_STATUS_OK;
  CHAR8       VarValue[64]  =  "";

  AsciiSPrint (VarValue, sizeof(VarValue), "%x", uVariableValue);

  if (EFI_SUCCESS  != Display_Utils_SetVariable(pVariableName, (UINT8 *)VarValue, AsciiStrLen (VarValue), 0))
  {
    eStatus = MDP_STATUS_FAILED;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: MDP_Display_GetVariable_Integer()
*/
/*!
* \brief
*   Function to get the display variable.
*
  @param[in]   pVariableName              The variable name that need to get.
  @param[out]  pVariableValue             The variable value.
*
* \retval MDP_Status
*
****************************************************************************/
MDP_Status MDP_Display_GetVariable_Integer (CHAR16 *pVariableName, UINT32 *pVariableValue)
{
  MDP_Status  eStatus       = MDP_STATUS_OK;
  CHAR8       VarValue[64]  =  "";
  UINTN       uVarLength    =  sizeof(VarValue) / sizeof(CHAR8);

  if ((EFI_SUCCESS  == Display_Utils_GetVariable(pVariableName, (UINT8 *)VarValue, &uVarLength, 0)) &&
      (uVarLength  > 0))
  {
    *pVariableValue = AsciiStrHexToUintn(VarValue);
  }
  else
  {
    *pVariableValue = 0;
    eStatus         = MDP_STATUS_FAILED;
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: MDP_MountFv()
*/
/*!
* \brief
*   Mount fv for display.
*
* \param [in]  gFvNameGuid        - The fv name guid
*        [in]  pFvName            - The fv name
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS MDP_MountFv(EFI_GUID gFvNameGuid, CHAR16 *pFvName)
{
  EFI_STATUS         eStatus       = EFI_SUCCESS;
  EFI_PIL_PROTOCOL  *pPILProtocol  = NULL;

  if (GuidedFvIsMounted(&gFvNameGuid))
  {
    // Do nothing here if imagefv is already mounted.
  }
  else
  {
    eStatus = gBS->LocateProtocol (&gEfiPilProtocolGuid, NULL, (VOID **) &pPILProtocol);

    if ((EFI_SUCCESS  != eStatus) ||
        (NULL         == pPILProtocol))
    {
      DEBUG ((EFI_D_WARN, "MDPLib: Locate pil protocol failed with status(%d)!\r\n", eStatus));
    }
    else
    {
      eStatus = pPILProtocol->ProcessPilImage(pFvName);
    }
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: MDP_InitDiagnosticInfo()
*/
/*!
* \brief
*   Initialize the display diagnostic buffer structure
*
* \param 
*     none
*
* \retval 
*     none
*
****************************************************************************/
void MDP_InitDiagnosticInfo()
{
   DiagLog_Init(DISPLAY_DIAG_GETBUFFER());
}


/****************************************************************************
*
** FUNCTION: MDP_GetDiagnosticInfo()
*/
/*!
* \brief
*   Retrieve the display diagnostic buffer information
*
* \param 
*     pBuffer     - [in] Callers diagnostic log buffer
*     pBufferSize - [in/out] Size of the input buffer and size of bytes consumed
*
* \retval 
*     MDP_STATUS_OK on success
*
****************************************************************************/
MDP_Status MDP_GetDiagnosticInfo(void *pBuffer, UINT32 *pBufferSize)
{
   MDP_Status    eStatus            = MDP_STATUS_OK;

   if ((NULL == pBuffer) ||
       (NULL == pBufferSize))
   {
     eStatus = MDP_STATUS_BAD_PARAM;
   }
   else if (*pBufferSize < sizeof(DisplayDiagLogType))
   {
     eStatus = MDP_STATUS_BAD_PARAM;
   }
   else if (NULL == DISPLAY_DIAG_GETBUFFER())
   {
     // Nothing to copy
     *pBufferSize = 0;
   }
   else
   {
     MDP_OSAL_MEMCPY(pBuffer, DISPLAY_DIAG_GETBUFFER(), sizeof(DisplayDiagLogType));
     *pBufferSize = sizeof(DisplayDiagLogType);
   }
    
   return eStatus;
}


/****************************************************************************
*
** FUNCTION: MDPAllocateReservedMemory()
*/
/*!
* \brief
*   This function will allocate a reserved memory region. Target specific
*
* \param [in]  eChipId                 - Chipset family
*        [out] pReservedMemoryAddress  - pointer to the allocated memory address
*        [out] pReservedMemorySize     - pointer to the allocated memory size
*
* \retval MDP_Status PcdReservedMemory alignment
*
****************************************************************************/
static MDP_Status MDPAllocateReservedMemory(EFIChipInfoFamilyType eChipId, uint64 *pReservedMemoryAddress, uint64 *pReservedMemorySize)
{
  MDP_Status       eStatus        = MDP_STATUS_OK;
  uint32           uMemSize       = PcdGet32(PcdReservedMemorySize);
  uint32           uMemAlignment  = PcdGet32(PcdReservedMemoryAlignment);

  if ((NULL == pReservedMemoryAddress) ||
      (NULL == pReservedMemorySize))
  {
    eStatus = MDP_STATUS_BAD_PARAM;
  }
  else if ((0 == uMemSize)    ||
           (0 == uMemAlignment))
  {
    //No need to allocate memory
    eStatus = MDP_STATUS_OK;
  }
  else
  {
    UINTN   uReservedMemorySize       = 0;
    UINTN   uReservedMemoryAlignment  = 0;
    void   *pMemoryAddress            = NULL;

    switch (eChipId)
    {
    case EFICHIPINFO_FAMILY_MSM8998:
      uReservedMemorySize       = MDP_MEGA(uMemSize);
      uReservedMemoryAlignment  = MDP_KILOBYTES(uMemAlignment);
      break;
    default:
      uReservedMemorySize       = 0;
      uReservedMemoryAlignment  = 0;
      break;
    }

    if (uReservedMemorySize > 0)
    {
      UINTN uPages = EFI_SIZE_TO_PAGES(uReservedMemorySize);

      /* Allocate reserved memory */
      if (NULL == (pMemoryAddress = AllocateAlignedReservedPages(uPages, uReservedMemoryAlignment)))
      {
        DEBUG((EFI_D_ERROR, "MDPAllocateReservedMemory() failed to allocate %d bytes (%d pages).\n", uReservedMemorySize, uPages));
        eStatus = MDP_STATUS_BAD_PARAM;
      }
      else
      {
        /* Update the memory size after a successful allocation */
        uReservedMemorySize = EFI_PAGES_TO_SIZE(uPages);
        DEBUG((EFI_D_INFO, "MDPAllocateReservedMemory() allocated %d bytes.\n", uReservedMemorySize));
      }
    }

    *pReservedMemoryAddress = (uint64)pMemoryAddress;
    *pReservedMemorySize    = uReservedMemorySize;
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: GetMaxDDRChannels()
*/
/*!
* \brief
*   This function will query DDR channel info from DDRGetInfo protocol
*
* \param [out] pNumChannels   - Number of DDR channels
*
* \retval void
*
****************************************************************************/
static void GetMaxDDRChannels(uint32 *pNumChannels)
{
  EFI_DDRGETINFO_PROTOCOL        *pDDRInfoProtocol = NULL;
  struct ddr_details_entry_info   DDRInfo;

  if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiDDRGetInfoProtocolGuid, NULL, (void **)&pDDRInfoProtocol))
  {
    DEBUG((EFI_D_WARN, "MDPLib: Unable to locate DDRInfo protocol.\n"));
  }
  else if (EFI_SUCCESS != pDDRInfoProtocol->GetDDRDetails(pDDRInfoProtocol, &DDRInfo))
  {
    DEBUG((EFI_D_WARN, "MDPLib: GetDDRDetails failed\n"));
  }
  else
  {
    //GetDDRDetails was successful
    *pNumChannels = DDRInfo.num_channels;
  }
}


/****************************************************************************
*
** FUNCTION: DisplayCacheVariables()
*/
/*!
* \brief
*   This function will cache the display variables.
*
* \param [in] pVariableName   - Variable name
*        [in] pVariableValue  - Variable value
*        [in] uDataSize       - Data size
*        [in] eAttrib         - Variable attrib
*
* \retval MDP_Status
*
****************************************************************************/
static MDP_Status DisplayCacheVariables(CHAR16                *pVariableName,
                                        void                  *pVariableValue,
                                        UINTN                  uDataSize,
                                        MDPVariableAttribType  eAttrib)
{
  MDP_Status    eStatus      = MDP_STATUS_OK;
  UINT32        uVarIndex    = 0;
  UINT32        uNewIndex    = MDP_VAR_TABLE_MAX_SIZE;

  if ((pVariableName   == NULL)  ||
      (pVariableValue  == NULL)  ||
      (uDataSize       ==  0))
  {
    DEBUG((EFI_D_WARN, "MDPSystem: Null parameter for variable name or value\n"));
    eStatus = MDP_STATUS_BAD_PARAM;
  }
  else
  {
    for (uVarIndex=0; uVarIndex<MDP_VAR_TABLE_MAX_SIZE; uVarIndex++)
    {
      if (NULL == gDisplayVariableTable[uVarIndex].pVariableName)
      {
        // Variable table free spot has been found.
        uNewIndex = uVarIndex;
        break;
      }
      else if (0 == StrCmp(gDisplayVariableTable[uVarIndex].pVariableName, pVariableName))
      {
        // Existing variable name is same as pVariableName, overwrite the index value.
        uNewIndex = uVarIndex;
        break;
      }
    }

    if (uNewIndex < MDP_VAR_TABLE_MAX_SIZE)
    {
      gDisplayVariableTable[uNewIndex].pVariableName    = pVariableName;
      gDisplayVariableTable[uNewIndex].pVariableValue   = pVariableValue;
      gDisplayVariableTable[uNewIndex].uDataSize        = uDataSize;
      gDisplayVariableTable[uNewIndex].eAttrib          = eAttrib;
      eStatus                                           = MDP_STATUS_OK;
    }
    else
    {
      eStatus = MDP_STATUS_FAILED;
    }
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: DisplaySaveVariables()
*/
/*!
* \brief
*   This function will save the display variables.
*
* \param [in] pVariableName   - Variable name
*        [in] pVariableValue  - Variable value
*        [in] uDataSize       - Data size
*        [in] eAttrib         - Variable attrib
*        [in] uFlags          - Variable flags
*
* \retval EFI_STATUS
*
****************************************************************************/
static EFI_STATUS DisplaySaveVariables(CHAR16                *pVariableName,
                                       void                  *pVariableValue,
                                       UINTN                  uDataSize,
                                       MDPVariableAttribType  eAttrib,
                                       UINT32                 uFlags)
{
  EFI_STATUS    eStatus = EFI_SUCCESS;

  if ((NULL == pVariableName) ||
      (NULL == pVariableValue))
  {
    DEBUG((EFI_D_WARN, "MDPSystem: Null parameter for variable name or value\n"));
    eStatus = EFI_INVALID_PARAMETER;
  }
  else
  {
    UINT32        uAttribFlags   = EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    EFI_GUID      sQcomTokenGUID = gQcomTokenSpaceGuid;
    EFI_GUID      sOutputGUID    = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    if (uFlags & MDP_VARIABLE_FLAG_NON_VOLATILE)
    {
      uAttribFlags |= EFI_VARIABLE_NON_VOLATILE;
    }

    if (MDP_VARIABLE_ATTRIB_FIRMWARE_ENV == eAttrib)
    {
      eStatus = gRT->SetVariable(pVariableName, &sOutputGUID, uAttribFlags, uDataSize, pVariableValue);
    }
    else
    {
      eStatus = gRT->SetVariable(pVariableName, &sQcomTokenGUID, uAttribFlags, uDataSize, pVariableValue);
    }
  }

  return eStatus;
}
