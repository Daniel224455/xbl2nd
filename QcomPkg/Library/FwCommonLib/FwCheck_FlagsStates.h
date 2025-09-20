/** @file FwFlagsAndStates.h
   
  Header for FwFlagsAndStates.c, part of FwUpdateLib.

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
 2019/03/21     vkasilya    Added FW_STATE_PREPARE_UPDATE to find FW Backup phase started
 2018/07/11     prema       Changes to move GPT backup from file to NV
 2018/05/21     gparam      Changes to move all file based state-machine data to NV +
                            Cleanup of unused functions after the file to NV migration
 2014/12/08     mic         Deprecated FwGetOsType API
 2012/08/03     mic         Added maximum update attempts support
 2012/06/12     rs          Added support for BreakChangeNumber
 2012/06/05     rs          Added GetDebugModeState(), GetSecureBootState() and FwGetOSType() 
 2012/05/10     rs          Added support for decryption of bootloaders
 2011/10/12     jthompso    Initial version

=============================================================================*/

#ifndef __QCOM_FW_CHECK_FLAGSSTATES_H__
#define __QCOM_FW_CHECK_FLAGSSTATES_H__


#include "Uefi.h"
#include "esp_dpp.h"
#include "FwHandle_Partition.h"

// NV Names
#define QCOM_FW_UPDATE_NV_CAPSULE_QC_RESULT_NAME            L"QCRESULT_FLAG"
#define QCOM_FW_UPDATE_NV_UPDATE_STATE_NAME                 L"UPDATE_STATE"
#define QCOM_FW_UPDATE_NV_BP_GPT_NAME                       L"BP_TABLE"
#define QCOM_FW_UPDATE_NV_GPP_GPT_NAME                      L"GPP_TABLE"
#define QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM_NAME          L"BREAKING_CHANGE_NUMBER"
#define QCOM_FW_UPDATE_NV_FILE_ATTEMPT_COUNT_NAME           L"ATTEMPT_COUNT"


#define QCOM_FW_UPDATE_CAPSULE_QC_RESULT_SIZE               (sizeof (QCFW_UPDATE_FLAG_VALUE))
#define QCOM_FW_UPDATE_UPDATE_STATE_SIZE                    (sizeof (QCOM_FW_UPDATE_STATE_VARIABLES))
#define QCOM_FW_UPDATE_BREAKING_CHANGE_NUM_SIZE             (sizeof (UINT32))
#define QCOM_FW_UPDATE_ATTEMPT_COUNT_SIZE                   (sizeof (UINT8))

// Size of BP and GPP GPT are calculated based on details for UFS and SPINOR-NVMe
#define QCOM_FW_UPDATE_UFS_BP_GPT_NV_SIZE                   0

// TODO: prem: Need to calculate this data using console app
// Rough compressed size of (EMMC_BOOT_MAX_ENTRIES * sizeof (ACTIVE_GPT_ENTRY_SUB_DATA))
#define QCOM_FW_UPDATE_EMMC_BP_GPT_NV_SIZE                  2123

// TODO: prem: Need to calculate this data using console app
// Rough compressed size of (EMMC_BOOT_MAX_ENTRIES * sizeof (ACTIVE_GPT_ENTRY_SUB_DATA))
#define QCOM_FW_UPDATE_EMMC_GPP_GPT_NV_SIZE                 2128

// Rough compressed size of (UFS_GPP_MAX_ENTRIES * sizeof (ACTIVE_GPT_ENTRY_SUB_DATA))
#define QCOM_FW_UPDATE_UFS_GPP_GPT_NV_SIZE                  2128

// Rough compressed size of (SPINOR_BOOT_MAX_ENTRIES * sizeof (ACTIVE_GPT_ENTRY_SUB_DATA))
#define QCOM_FW_UPDATE_NOR_BP_GPT_NV_SIZE                   2123

// Rough compressed size of (NVME_GPP_MAX_ENTRIES * sizeof (ACTIVE_GPT_ENTRY_SUB_DATA))
#define QCOM_FW_UPDATE_NVME_GPP_GPT_NV_SIZE                 1495

#define QCOM_FW_UPDATE_NV_SIZE_UNKNOWN                      ((UINT32)(-1))


typedef enum
{
  QCOM_FW_UPDATE_QC_RESULT_FLAG = 0,
  QCOM_FW_UPDATE_FLAG_MAX
}QCOM_FW_UPDATE_FLAG;

typedef enum {
  FW_STATE_NOT_IN_PROGRESS,
  FW_STATE_PREPARE_UPDATE,
  FW_STATE_BACKUP_DONE,
  FW_STATE_UPDATE_SUCCESS,
  FW_STATE_UPDATE_FAIL,   
  FW_STATE_ROLLBACK_DONE

} QCOM_FW_UPDATE_STATE_VARIABLES;

typedef enum UPDATE_STATUS{
  UPDATE_SUCCESS,
  UPDATE_FAILED
}UPDATE_STATUS;

typedef enum
{
    QCOM_FW_UPDATE_NV_CAPSULE_QC_RESULT,
    QCOM_FW_UPDATE_NV_UPDATE_STATE,
    QCOM_FW_UPDATE_NV_BP_GPT,
    QCOM_FW_UPDATE_NV_GPP_GPT,
    QCOM_FW_UPDATE_NV_BREAKING_CHANGE_NUM,
    QCOM_FW_UPDATE_NV_ATTEMPT_COUNT,
    QCOM_FW_UPDATE_NV_MAX

}QCOM_FW_UPDATE_NV_TYPE;

typedef enum
{
  QCOM_FW_UPDATE_GET_NV_DATA,
  QCOM_FW_UPDATE_SET_NV_DATA_FLUSH,
  QCOM_FW_UPDATE_SET_NV_DATA_NO_FLUSH
}QCOM_FW_UPDATE_GET_SET;

typedef struct{
    QCOM_FW_UPDATE_NV_TYPE    Type;
    UINT16                   *Name;
    UINTN                     Size;
    UINT32                    Attributes;

}QCOM_FW_UPDATE_NV_IDENTIFIER;

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
);

/**
 * Gets or Sets NV data of given type 
 *
 * @param NVType - NV to Read/Modify
 * @param Value - Value to set/Value Returned from read. 
 * @param ValueSize - Size of the Value to set/Size of the Value Returned from read. 
 * @param Operation - Get or Set with flush or Set with no flush NV Data
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
  );

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
  );

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
  );

/**
 * Read the update attempt count from non-volatile memory
 *
 * @param  cnt      -  Output attempt count
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
GetAttemptCount ( 
  OUT UINT8 *cnt
  );

/**
 * Read the BreakingChangeNumber from the file in non-volatile memory
 *
 * @param BreakingChangeNumber  - The BreakingChangeNumber from 
 *  							  the FS
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
GetBreakingChangeNumber ( 
  OUT UINT32 *BreakingChangeNumber
  );

/**
 * This function updates the state flag in non-volatile memory for future retrieval. 
 * 
 * @param State       - State of the system that needs to be 
 *  				    updated
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
SetUpdateState(
  IN QCOM_FW_UPDATE_STATE_VARIABLES State
  );

/**
 * *********This function should be called during boot time only**************
 * ******************for boot time optimization*******************************
 * This function updates the state flag in non-volatile memory for future retrieval 
 * without flushing.
 *
 * @param State       - State of the system that needs to be 
 *  				    updated
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
SetUpdateStateDonotFlush(
  IN QCOM_FW_UPDATE_STATE_VARIABLES State
  );


/**
 * This function updates the attempt count in non-volatile memory. 
 * Make sure after the write a flush command is issued to clear 
 * the FS cache 
 * 
 * @param AttemptCnt       - attempt count to update in the FS
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
SetAttemptCount(
  IN UINT8 AttemptCnt
  );

/**
 * *********This function should be called during boot time only**************
 * ******************for boot time optimization*******************************
 * This function updates the attempt count in non-volatile memory. 
 * Make sure after the write a flush command is issued to clear 
 * the FS cache without flushing
 * 
 * @param AttemptCnt       - attempt count to update in the FS
 *  
 * @return EFI_STATUS 
 */

EFI_STATUS 
SetAttemptCountDonotFlush(
  IN UINT8 AttemptCnt
  );

/**
 * Sets a flag used by FwUpdateLib.
 * 
 * @param FlagValue - Value to be set. 
 *  
 * @return EFI_STATUS 
 */
EFI_STATUS 
SetQCResultFlag(       
  IN QCFW_UPDATE_FLAG_VALUE       FlagValue
  );

/**
 * Write the BreakingChangeNumber to the file in non-volatile memory
 *
 * @param BreakingChangeNumber  - The BreakingChangeNumber to 
 *  							  write to the FS
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
SetBreakingChangeNumber ( 
  IN UINT32 BreakingChangeNumber
  );

/**
 * *********This function should be called during boot time only**************
 * ******************for boot time optimization*******************************
 * Write the BreakingChangeNumber to the file in non-volatile memory  
 * without flushing
 *
 * @param BreakingChangeNumber  - The BreakingChangeNumber to 
 *  							  write to the FS
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
SetBreakingChangeNumberDonotFlush ( 
  IN UINT32 BreakingChangeNumber
  );


/**
 * This Function gets the state of secure boot
 * 
 * 
 * @param SecureBootEnabled - A caller allocated buffer to store 
 *  						  secure boot state.
 * @return EFI_STATUS 
 */

EFI_STATUS
GetSecureBootState(
  OUT BOOLEAN *SecureBootEnabled
  );

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
    );

/**
 * Check if the NV variables related to firmware update
 * is present. Else create those with default values
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
CheckAndCreateNVData(
  VOID
  );

/**
 * Get the current state to non-volatile memory(NVRAM)
 *
 * @param  GptNvData      -  Output GptNvData 
 * @param  GptNvDataSize  -  Output GptNvDataSize
 *
 * @return EFI_STATUS 
 *
 */

EFI_STATUS 
GetGPTNvData (
  IN  QCOM_FW_UPDATE_NV_TYPE      NVType,
  OUT VOID                      **GptNvData,
  OUT UINTN                      *GptNvDataSize
  );

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
  IN  VOID                        *GptNvData,
  IN  UINTN                        GptNvDataSize
  );

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
SetGPTNvDataDonotFlush(
  IN  QCOM_FW_UPDATE_NV_TYPE       NVType,
  IN  VOID                        *GptNvData,
  IN  UINTN                        GptNvDataSize
  );


#endif

