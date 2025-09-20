/** @file FwFlagsAndStates.c
   
  Change Flags and States for FwUpdate.

  Copyright (c) 2011-2012, 2014-2015, 2018 - 2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2019/04/01     vkasilya    Modified functions for Bootup time optimization
 2018/07/20     gparam      KW Fixes
 2018/07/11     prema       Changes to move GPT backup from file to NV
 2018/05/21     gparam      Changes to move all file based state-machine data to NV +
                            Cleanup of unused functions after the file to NV migration
 2014/12/08     mic         Deprecated FwGetOsType API
 2012/08/03     mic         Added maximum update attempts support
 2012/06/12     rs          Added support for BreakChangeNumber
 2012/06/05     rs          Added GetDebugModeState(), GetSecureBootState() and FwGetOSType()
 2012/01/25     jd          Enhanced GetOrSetFlagState to support FactoryTool commit and result files
 2011/10/12     jthompso    Initial version
=============================================================================*/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BootConfig.h>
#include <Library/UefiLib.h>
#include <Library/FwCommonLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/QcomLib.h>

#include <Guid/GlobalVariable.h>
#include "FwCheck_FlagsStates.h"

static EFI_GUID DebugModeVariableGuid = {0x77fa9abd, 0x0359, 0x4d32, { 0xbd, 0x60, 0x28, 0xf4, 0xe7, 0x8f, 0x78, 0x4b}};

static const QCOM_FW_UPDATE_NV_IDENTIFIER NVConfigData[QCOM_FW_UPDATE_NV_MAX] = {
  {QCOM_FW_UPDATE_NV_CAPSULE_QC_RESULT,    QCOM_FW_UPDATE_NV_CAPSULE_QC_RESULT_NAME,   QCOM_FW_UPDATE_CAPSULE_QC_RESULT_SIZE,   (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS)},
  {QCOM_FW_UPDATE_NV_UPDATE_STATE,         QCOM_FW_UPDATE_NV_UPDATE_STATE_NAME,        QCOM_FW_UPDATE_UPDATE_STATE_SIZE,        (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)},
  {QCOM_FW_UPDATE_NV_BP_GPT,               QCOM_FW_UPDATE_NV_BP_GPT_NAME,              QCOM_FW_UPDATE_NV_SIZE_UNKNOWN,          (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)},
  {QCOM_FW_UPDATE_NV_GPP_GPT,              QCOM_FW_UPDATE_NV_GPP_GPT_NAME,             QCOM_FW_UPDATE_NV_SIZE_UNKNOWN,          (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)},
  {QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM,  QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM_NAME, QCOM_FW_UPDATE_BREAKING_CHANGE_NUM_SIZE, (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)},
  {QCOM_FW_UPDATE_NV_ATTEMPT_COUNT,        QCOM_FW_UPDATE_NV_FILE_ATTEMPT_COUNT_NAME,  QCOM_FW_UPDATE_ATTEMPT_COUNT_SIZE,       (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)}
    
};

extern EFI_GUID gQcomTokenSpaceGuid;

/**
 * This function returns the default compressed size of GPT data
 * 
 * @param BpGPTSize  - State of the system that needs to be 
 *                     updated
 * @param GppGPTSize - State of the system that needs to be 
 *                     updated
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
GetDefaultGPTSize(
  UINTN *BpGPTSize,
  UINTN *GppGPTSize
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if ((NULL == BpGPTSize) ||
      (NULL == GppGPTSize))
  {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (boot_from_emmc())
  {
    *BpGPTSize = QCOM_FW_UPDATE_EMMC_BP_GPT_NV_SIZE;
    *GppGPTSize = QCOM_FW_UPDATE_EMMC_GPP_GPT_NV_SIZE;
  }
  else if (boot_from_ufs())
  {
    *BpGPTSize = QCOM_FW_UPDATE_UFS_BP_GPT_NV_SIZE;
    *GppGPTSize = QCOM_FW_UPDATE_UFS_GPP_GPT_NV_SIZE;
  }
  else if (boot_from_spi_nor())
  {
    *BpGPTSize = QCOM_FW_UPDATE_NOR_BP_GPT_NV_SIZE;
    *GppGPTSize = QCOM_FW_UPDATE_NVME_GPP_GPT_NV_SIZE;
  }
  else
  {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

Exit:
  return Status;
}

/**
 * Gets or Sets NV data of given type 
 *
 * @param NVType - NV to Read/Modify
 * @param Value - Value to set/Value Returned from read. 
 * @param ValueSize - Size of the Value to set/Size of the Value Returned from read. 
 * @param Operation - Get, Set with flush or Set with no flush NV Data
 *
 @retval EFI_SUCCESS            On successful operation
 @retval EFI_BUFFER_TOO_SMALL   Requested NV type holds larger data than expected.
 */
EFI_STATUS
GetOrSetNVData(
  IN     QCOM_FW_UPDATE_NV_TYPE       NVType,
  IN OUT VOID                       **Value,
  IN OUT UINTN                       *ValueSize,
  IN     QCOM_FW_UPDATE_GET_SET       Operation
  )
{
  EFI_STATUS     Status            = EFI_SUCCESS;

  // Check the expected file size
  if ((NVType >= QCOM_FW_UPDATE_NV_MAX) || 
      (NULL == Value) ||
      (NULL == ValueSize))
  {
    return EFI_INVALID_PARAMETER;
  }

  if(Operation == QCOM_FW_UPDATE_GET_NV_DATA)
  {
    Status = GetVariable2(
      NVConfigData[NVType].Name,
      &gQcomTokenSpaceGuid,
      Value,
      ValueSize
      );

    ERR_HANDLE_LABEL(Status,GetOrSetNVDataExit);

    if (*Value == NULL)
    {
      Status = EFI_LOAD_ERROR;
      DEBUG((EFI_D_WARN,"Error: GetVariable2 returned invalid buffer\n"));

      goto GetOrSetNVDataExit;
    }

    if (*ValueSize > NVConfigData[NVType].Size)
    {
      Status = EFI_BUFFER_TOO_SMALL;
      DEBUG((EFI_D_WARN,"Error: NV size larger than expected value. Expected: %d. Actual Size: %d.\n", NVConfigData[NVType].Size, *ValueSize));

      if (*Value != NULL)
      {
          FreePool (*Value);
          *Value = NULL;
      }
      goto GetOrSetNVDataExit;
    }
  }
  else
  {
    if (NULL == *Value)
    {
      Status = EFI_INVALID_PARAMETER;
    
      goto GetOrSetNVDataExit;
    }

    Status = gRT->SetVariable(
      NVConfigData[NVType].Name,
      &gQcomTokenSpaceGuid,
      NVConfigData[NVType].Attributes,
      *ValueSize,
      *Value
      );

    ERR_HANDLE_LABEL(Status,GetOrSetNVDataExit);

    if (Operation == QCOM_FW_UPDATE_SET_NV_DATA_FLUSH)
    {
      FlushVariableServicesIfSupported();
    }
  }

GetOrSetNVDataExit:

  return Status;
}

/**
 * Get value of the QC result flag.
 * 
 * @param FlagValue - Value read from flag.
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
GetQCResultFlag(        
  OUT QCFW_UPDATE_FLAG_VALUE      *FlagValue
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN NVSize = 0;
  VOID *NVData = NULL;

  Status = GetOrSetNVData(
    QCOM_FW_UPDATE_NV_CAPSULE_QC_RESULT,
    &NVData,
    &NVSize,
    QCOM_FW_UPDATE_GET_NV_DATA
    );

  if (EFI_SUCCESS == Status)
  {
    if (NVSize > sizeof (*FlagValue))
    {
      DEBUG((EFI_D_WARN, "Current Size=%d : Max Expected Size=%d",
        NVSize,
        NVConfigData[QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM].Size
        ));
      FreePool (NVData);
      NVData = NULL;

      Status = EFI_BUFFER_TOO_SMALL;
      goto Exit;
    }

    ZeroMem(FlagValue, sizeof(*FlagValue));
    CopyMem(FlagValue, NVData, NVSize);
  }
  else
  {
    goto Exit;
  }

Exit:

  if (NULL != NVData)
  {
    FreePool (NVData);
    NVData = NULL;
  }
  return Status;
}

/**
 * Read the current state from non-volatile memory
 *
 * @param  State      -  Output STATE_VARIABLE
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
GetUpdateState ( 
  OUT QCOM_FW_UPDATE_STATE_VARIABLES *State
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN NVSize = 0;
  VOID *NVData = NULL;

  Status = GetOrSetNVData(
    QCOM_FW_UPDATE_NV_UPDATE_STATE,
    &NVData,
    &NVSize,
    QCOM_FW_UPDATE_GET_NV_DATA
    );

  if (EFI_SUCCESS == Status)
  {
    if (NVSize > sizeof (*State))
    {
      DEBUG((EFI_D_WARN, "Current Size=%d : Max Expected Size=%d",
        NVSize,
        NVConfigData[QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM].Size
        ));
      FreePool (NVData);
      NVData = NULL;

      Status = EFI_BUFFER_TOO_SMALL;
      goto Exit;
    }

    ZeroMem(State, sizeof(*State));
    CopyMem(State, NVData, NVSize);
  }
  else
  {
    goto Exit;
  }

Exit:

  if (NULL != NVData)
  {
    FreePool (NVData);
    NVData = NULL;
  }

  return Status;
}

/**
 * Read the attempt count from the file in non-volatile memory
 *
 * @param  cnt      -  Output attempt count
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
GetAttemptCount ( 
  OUT UINT8 *cnt
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN NVSize = 0;
  VOID *NVData = NULL;

  Status = GetOrSetNVData(
    QCOM_FW_UPDATE_NV_ATTEMPT_COUNT,
    &NVData,
    &NVSize,
    QCOM_FW_UPDATE_GET_NV_DATA
    );

  if (EFI_SUCCESS == Status)
  {
    if (NVSize > sizeof (*cnt))
    {
      DEBUG((EFI_D_WARN, "Current Size=%d : Max Expected Size=%d",
        NVSize,
        NVConfigData[QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM].Size
        ));
      FreePool (NVData);
      NVData = NULL;
  
      Status = EFI_BUFFER_TOO_SMALL;
      goto Exit;
    }
  
    ZeroMem(cnt, sizeof(*cnt));
    CopyMem(cnt, NVData, NVSize);
  }
  else
  {
    goto Exit;
  }
  
Exit:
  
  if (NULL != NVData)
  {
    FreePool (NVData);
    NVData = NULL;
  }

  return Status;
}

/**
 * Read the BreakingChangeNumber from the file in non-volatile memory
 *
 * @param BreakingChangeNumber  - The BreakingChangeNumber from 
 *                                the non-volatile memory
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
GetBreakingChangeNumber ( 
  OUT UINT32 *BreakingChangeNumber
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN NVSize = 0;
  VOID *NVData = NULL;

  Status = GetOrSetNVData(
    QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM,
    &NVData,
    &NVSize,
    QCOM_FW_UPDATE_GET_NV_DATA
    );

  if (EFI_SUCCESS == Status)
  {
    if (NVSize > sizeof (*BreakingChangeNumber))
    {
      DEBUG((EFI_D_WARN, "Current Size=%d : Max Expected Size=%d",
        NVSize,
        NVConfigData[QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM].Size
        ));
      FreePool (NVData);
      NVData = NULL;
  
      Status = EFI_BUFFER_TOO_SMALL;
      goto Exit;
    }
  
    ZeroMem(BreakingChangeNumber, sizeof(*BreakingChangeNumber));
    CopyMem(BreakingChangeNumber, NVData, NVSize);
  }
  else
  {
    goto Exit;
  }
  
Exit:
  
  if (NULL != NVData)
  {
    FreePool (NVData);
    NVData = NULL;
  }

  return Status;
}

/**
 * Sets a flag used by FwCommonLib.
 * 
 * @param FlagValue - Value to be set. 
 *  
 * @return EFI_STATUS 
 */
EFI_STATUS 
SetQCResultFlag(       
  IN QCFW_UPDATE_FLAG_VALUE       FlagValue
  )
{
  UINTN NVSize = NVConfigData[QCOM_FW_UPDATE_NV_CAPSULE_QC_RESULT].Size;
  VOID *NVValue = &FlagValue;

  return GetOrSetNVData(
    QCOM_FW_UPDATE_NV_CAPSULE_QC_RESULT,
    &NVValue,
    &NVSize,
    QCOM_FW_UPDATE_SET_NV_DATA_FLUSH
    );
}

/**
 * This function updates the state flag in non-volatile memory for future retrieval. 
 * 
 * @param State       - State of the system that needs to be 
 *                      updated
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
SetUpdateState(
  IN QCOM_FW_UPDATE_STATE_VARIABLES State
  )
{
  UINTN NVSize = NVConfigData[QCOM_FW_UPDATE_NV_UPDATE_STATE].Size;
  VOID *NVValue = &State;

  return GetOrSetNVData(
    QCOM_FW_UPDATE_NV_UPDATE_STATE,
    &NVValue,
    &NVSize,
    QCOM_FW_UPDATE_SET_NV_DATA_FLUSH
    );
}

/**
 * *********This function should be called during boot time only**************
 * ******************for boot time optimization*******************************
 * This function updates the state flag in non-volatile memory for future retrieval 
 * without flushing.
 *
 * @param State       - State of the system that needs to be 
 *                      updated
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
SetUpdateStateDonotFlush(
  IN QCOM_FW_UPDATE_STATE_VARIABLES State
  )
{
  UINTN NVSize = NVConfigData[QCOM_FW_UPDATE_NV_UPDATE_STATE].Size;
  VOID *NVValue = &State;

  return GetOrSetNVData(
    QCOM_FW_UPDATE_NV_UPDATE_STATE,
    &NVValue,
    &NVSize,
    QCOM_FW_UPDATE_SET_NV_DATA_NO_FLUSH
    );
}

/**
 * This function updates the attempt count in non-volatile memory. 
 * 
 * @param AttemptCnt       - attempt count to update in the non-volatile memory
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
SetAttemptCount(
  IN UINT8 AttemptCnt
  )
{
  UINTN NVSize = NVConfigData[QCOM_FW_UPDATE_NV_ATTEMPT_COUNT].Size;
  VOID *NVValue = &AttemptCnt;

  return GetOrSetNVData(
    QCOM_FW_UPDATE_NV_ATTEMPT_COUNT,
    &NVValue,
    &NVSize,
    QCOM_FW_UPDATE_SET_NV_DATA_FLUSH
    );
}

/**
 * *********This function should be called during boot time only**************
 * ******************for boot time optimization*******************************
 * This function updates the attempt count in non-volatile memory 
 * without flushing.
 * 
 * @param AttemptCnt       - attempt count to update in the non-volatile memory
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
SetAttemptCountDonotFlush(
  IN UINT8 AttemptCnt
  )
{
  UINTN NVSize = NVConfigData[QCOM_FW_UPDATE_NV_ATTEMPT_COUNT].Size;
  VOID *NVValue = &AttemptCnt;

  return GetOrSetNVData(
    QCOM_FW_UPDATE_NV_ATTEMPT_COUNT,
    &NVValue,
    &NVSize,
    QCOM_FW_UPDATE_SET_NV_DATA_NO_FLUSH
    );
}

/**
 * Write the BreakingChangeNumber to the file in non-volatile memory
 *
 * @param BreakingChangeNumber  - The BreakingChangeNumber to 
 *                                write to the non-volatile memory
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
SetBreakingChangeNumber( 
  IN UINT32 BreakingChangeNumber
  )
{
  UINTN NVSize = NVConfigData[QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM].Size;
  VOID *NVValue = &BreakingChangeNumber;

  return GetOrSetNVData(
    QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM,
    &NVValue,
    &NVSize,
    QCOM_FW_UPDATE_SET_NV_DATA_FLUSH
    );
}


/**
 * *********This function should be called during boot time only**************
 * ******************for boot time optimization*******************************
 * Write the BreakingChangeNumber to the file in non-volatile memory
 * without flushing
 *
 * @param BreakingChangeNumber  - The BreakingChangeNumber to 
 *                                write to the non-volatile memory
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
SetBreakingChangeNumberDonotFlush ( 
  IN UINT32 BreakingChangeNumber
  )
{
  UINTN NVSize = NVConfigData[QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM].Size;
  VOID *NVValue = &BreakingChangeNumber;

  return GetOrSetNVData(
    QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM,
    &NVValue,
    &NVSize,
    QCOM_FW_UPDATE_SET_NV_DATA_NO_FLUSH
    );
}

/**
    Variable GUID:  77fa9abd-0359-4d32-bd60-28f4e78f784b 
    Variable Name: "CurrentPolicy"
 
    If the variable exist, and its value size is just one byte
    then it is a dummy policy and the system is in production
    mode. If the variable exist and its size is larger, then the
    system may either have a custom production policy or a debug
    policy. The former is unlikely as MSFT do not intend to
    create any custom production policies and so you will be
    able to determine that the system is in debug mode.
 
    @param DebugModeEnabled - Update the current Debug Mode
                              state.
    @return EFI_STATUS 
 
*/
EFI_STATUS
GetDebugModeState(
    OUT BOOLEAN *DebugModeEnabled
    )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT8       DebugMode ;
    UINTN       VarSize;

    VarSize = sizeof(DebugMode);
    Status = gRT->GetVariable (L"CurrentPolicy", &DebugModeVariableGuid,
                               NULL, &VarSize, &DebugMode);

    *DebugModeEnabled = Status==EFI_BUFFER_TOO_SMALL?TRUE:FALSE;

    return EFI_SUCCESS;
}

/**
 * Check if secure boot is enabled or disabled.
 * 
 * @param SecureBootEnabled - A caller allocated buffer to store secure boot state.
 *
 * @return EFI_STATUS
 *
 */

EFI_STATUS
GetSecureBootState(
  OUT BOOLEAN *SecureBootEnabled
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       SecureBoot;
  UINTN       VarSize;

  VarSize = sizeof(SecureBoot);
  Status = gRT->GetVariable (L"SecureBoot", &gEfiGlobalVariableGuid,
                             NULL, &VarSize, &SecureBoot);

  if(EFI_ERROR(Status))
  {   
      DEBUG((EFI_D_ERROR,": gRT->GetVariable returned %r\n", Status));
      goto GetSecureBootStateExit;
  }   

  *SecureBootEnabled = SecureBoot == 0 ? FALSE : TRUE;

GetSecureBootStateExit:

  return Status;
}

/**
 * Check if the NV variables related to firmware update
 * is present. Else create those with default values
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
CheckAndCreateNVData(
        VOID
      )
{
  EFI_STATUS Status ;
  UINT8     *GptNvDataPtr               = NULL;
  UINTN      BpGptNvDataDefaultSize     = 0;
  UINTN      GppGptNvDataDefaultSize    = 0;
  UINT8      AttemptCount               = 0;
  UINT32     BreakingChangeNum          = 0;
  QCOM_FW_UPDATE_STATE_VARIABLES UpdateState  = FW_STATE_NOT_IN_PROGRESS ;

  Status = GetUpdateState(&UpdateState);
  if((Status == EFI_NOT_FOUND) ||
     (Status == EFI_BUFFER_TOO_SMALL))
  {
    UpdateState = FW_STATE_NOT_IN_PROGRESS;
    
    Status = SetUpdateStateDonotFlush(FW_STATE_NOT_IN_PROGRESS);
    HANDLE_ERROR_LABEL(Status,SetUpdateStateDonotFlush(FW_STATE_NOT_IN_PROGRESS),CheckAndCreateNVDataExit);
  }
  HANDLE_ERROR_LABEL(Status,GetUpdateState,CheckAndCreateNVDataExit);

  // If we are starting the update freshly, then reserve space for GPT NVs
  if(FW_STATE_NOT_IN_PROGRESS == UpdateState)
  {
    Status = GetDefaultGPTSize (&BpGptNvDataDefaultSize, &GppGptNvDataDefaultSize);
    HANDLE_ERROR_LABEL(Status,GetDefaultGPTSize(),CheckAndCreateNVDataExit);

    // Some devices might choose not to have BOOT or GPP backup. Ex: UFS will not take BOOT GPT backup
    if (0 != BpGptNvDataDefaultSize)
    {
      GptNvDataPtr = AllocateZeroPool (BpGptNvDataDefaultSize);
      Status = (NULL == GptNvDataPtr) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
      HANDLE_ERROR_LABEL(Status, AllocateZeroPool (GptNvDataPtr),  CheckAndCreateNVDataExit);

      Status = SetGPTNvDataDonotFlush(QCOM_FW_UPDATE_NV_BP_GPT, GptNvDataPtr, BpGptNvDataDefaultSize);
      HANDLE_ERROR_LABEL(Status,SetGPTNvDataDonotFlush(QCOM_FW_UPDATE_NV_BP_GPT),CheckAndCreateNVDataExit);

      FreePool (GptNvDataPtr);
      GptNvDataPtr = NULL;
    }
    
    // Some devices might choose not to have BOOT or GPP backup. Ex: UFS will not take BOOT GPT backup
    if (0 != GppGptNvDataDefaultSize)
    {
      GptNvDataPtr = AllocateZeroPool (GppGptNvDataDefaultSize);
      Status = (NULL == GptNvDataPtr) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
      HANDLE_ERROR_LABEL(Status, AllocateZeroPool (GptNvDataPtr),  CheckAndCreateNVDataExit);

      Status = SetGPTNvDataDonotFlush(QCOM_FW_UPDATE_NV_GPP_GPT, GptNvDataPtr, GppGptNvDataDefaultSize);
      HANDLE_ERROR_LABEL(Status,SetGPTNvDataDonotFlush(QCOM_FW_UPDATE_NV_GPP_GPT),CheckAndCreateNVDataExit);

      FreePool (GptNvDataPtr);
      GptNvDataPtr = NULL;
    }      
  }    

  Status = GetAttemptCount(&AttemptCount);
  if((Status == EFI_NOT_FOUND) ||
     (Status == EFI_BUFFER_TOO_SMALL))
  {
    Status = SetAttemptCountDonotFlush(0);
    HANDLE_ERROR_LABEL(Status,SetAttemptCountDonotFlush(0),CheckAndCreateNVDataExit);
  }
  HANDLE_ERROR_LABEL(Status,GetAttemptCount,CheckAndCreateNVDataExit);

  Status = GetBreakingChangeNumber(&BreakingChangeNum);
  if((Status == EFI_NOT_FOUND) ||
     (Status == EFI_BUFFER_TOO_SMALL))
  {
    Status = SetBreakingChangeNumberDonotFlush(0);
    HANDLE_ERROR_LABEL(Status,SetBreakingChangeNumberDonotFlush(0),CheckAndCreateNVDataExit);
  }
  HANDLE_ERROR_LABEL(Status,GetBreakingChangeNumber,CheckAndCreateNVDataExit);

CheckAndCreateNVDataExit:

  if (NULL != GptNvDataPtr)
  {
    FreePool (GptNvDataPtr);
    GptNvDataPtr = NULL;
  }

  return Status;
}

/**
 * Get the current state to non-volatile memory
 *
 * @param  GptNvData      -  Output GptNvData
 * @param  GptNvDataSize  -  Output GptNvDataSize
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
GetGPTNvData (
  IN  QCOM_FW_UPDATE_NV_TYPE       NVType,
  OUT VOID                       **GptNvData,
  OUT UINTN                       *GptNvDataSize
  )
{
  return GetOrSetNVData(
    NVType,
    GptNvData,
    GptNvDataSize,
    QCOM_FW_UPDATE_GET_NV_DATA
    );
}

/**
 * Set the GPT NV data to non-volatile memory(NVRAM)
 *
 * @param  GptNvData      -  Output GptNvData
 * @param  GptNvDataSize  -  Output GptNvDataSize
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
SetGPTNvData (
  IN  QCOM_FW_UPDATE_NV_TYPE       NVType,
  OUT VOID                        *GptNvData,
  IN  UINTN                        GptNvDataSize
  )
{
  return GetOrSetNVData(
    NVType,
    &GptNvData,
    &GptNvDataSize,
    QCOM_FW_UPDATE_SET_NV_DATA_FLUSH
    );
}

/**
 * *********This function should be called during boot time only**************
 * ******************for boot time optimization*******************************
 * Set the GPT NV data to non-volatile memory(NVRAM) without flushing
 *
 * @param  GptNvData      -  Output GptNvData 
 * @param  GptNvDataSize  -  Output GptNvDataSize
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
SetGPTNvDataDonotFlush (
  IN  QCOM_FW_UPDATE_NV_TYPE       NVType,
  OUT VOID                        *GptNvData,
  IN  UINTN                        GptNvDataSize
  )
{
  return GetOrSetNVData(
    NVType,
    &GptNvData,
    &GptNvDataSize,
    QCOM_FW_UPDATE_SET_NV_DATA_NO_FLUSH
    );
}
