/** @file ScmCmdVar.h
   
  Defines library functions for SCM commands used for Variable Storage.

  Copyright (c) 2011,2013,2016,2018-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Portions Copyright (c) 2006 - 2011, Intel Corporation. All rights reserved.<BR> 
  This program and the accompanying materials are licensed and made available under 
  the terms and conditions of the BSD License that accompanies this distribution. 
  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php. 

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS, 
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED. 
    
**/
/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/08/19   kpa     remove support to delete secure boot variables 
 02/05/19   kpa     SCM call for deleting secure boot variables
 03/07/16   bh      Update copyright
 03/14/13   niting  Added dump of UefiSecApp log buffer
 01/10/13   niting  Updated to use SCM protocol. Removed unused functions.
 12/15/11   niting  Cleanup.
 10/20/11   niting  Initial revision.

=============================================================================*/
#ifndef _SCMCMDVAR_H_
#define _SCMCMDVAR_H_

#include <Uefi.h>
#include <Library/ScmCmdLib.h>
#include <Library/VariablesHelperLib.h>
#include <Protocol/VariablePolicy.h>

#define  VARIABLE_INIT_PARAMS_REVISION      POLICY_ENGINE_REVISION
#define  POLICY_ENGINE_REVISION             0x00010001
#define  INITIAL_REVISION                   0x00010000

/* PolicyFlags bits */
#define  BIT_POLICY_ENGINE_NS_ENABLE        0x00000001     /* NS-world enable/disable bit */
#define  BIT_DISABLE_SECURE_BOOT            0x00000002     /* PE impacts Secure Boot config? */
#define  BIT_DISABLE_PRIV_AUTH_VAR          0x00000004     /* PE impacts Priv Auth Var protections? */

typedef enum {
  TABLE_ID_RESERVED    = 0,
  TABLE_ID_BS_NV_S     = 1,       /* Unused */
  TABLE_ID_BS_NV_NS    = 2,
  TABLE_ID_RT_NV_S     = 3,
  TABLE_ID_RT_NV_NS    = 4,

  TABLE_ID_COUNT       = 5,

  /* Add any extra tables here */

  TABLE_TYPE_MAX         = 6      /* Should be the last entry in this enum */
}TableType;

typedef struct  
{ 
  UINT32 DeviceId;          /* Root partition id - see qsee_stor_device_id_type */
  UINT8  PartitionGuid[16]; /* Guid corresponding to physical partition on storage */
  UINT32 PartitionType;     /* Soft partition id - see TableType*/ 
} TableInfoType; 

typedef struct {
  UINT32              Version;                           /* Version info */
  UINT32              TableInfoArraySize;                /* Num of elements in TableInfoArray */
  TableInfoType       TableInfoArray[TABLE_ID_COUNT];    /* See TableInfoType */
  UINT32              PolicyFlags;                       /* Added in POLICY_ENGINE_REVISION
                                                            - Policy Engine configuration */
} VARIABLE_INIT_PARAMS;

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
/**

  This code loads the SCM protocol instance to use for any SCM calls. This
  also returns the loaded UefiSecApp App ID.
 
  @param  UefiSecAppId              App ID corresponding to loaded UefiSecApp
                         
  @return EFI_SUCCESS               If app has been loaded and
                                    QCOM_SCM_PROTOCOL in loaded.

**/
EFI_STATUS
EFIAPI
ScmCmdInitUefiSecApp ( 
  OUT     UINT32            *UefiSecAppId
);

/**

  This code finds variable in storage blocks (Volatile or Non-Volatile).
   
  @param VariableName               Name of variable to be retrieved.
  @param VendorGuid                 Vendor Guid corresponding to the variable to
                                    be retrieved.
  @param Attributes                 Pointer to the attributes of the variable 
                                    found. If this is NULL, then this buffer
                                    will not be updated.
  @param DataSize                   Size of data found. If size is less than the
                                    size of the retrieved data, this value
                                    contains the required size.
  @param Data                       Pointer to the data.
                      
  @return EFI_SUCCESS               Found the specified variable successfully.
  @return EFI_INVALID_PARAMETER     Invalid parameter.
  @return EFI_NOT_FOUND             Variable was not found.
  @return EFI_BUFFER_TO_SMALL       DataSize is too small for the result.
  @return EFI_DEVICE_ERROR          The variable could not be retrieved due to 
                                    a hardware error.

**/
EFI_STATUS
EFIAPI
ScmCmdGetVariable (
  IN      CHAR16            *VariableName,
  IN      EFI_GUID          *VendorGuid,
  OUT     UINT32            *Attributes OPTIONAL,
  IN OUT  UINTN             *DataSize,
  OUT     VOID              *Data
  );

/**

  Sets the value of a variable.

  @param VariableName               A Null-terminated string that is the name of 
                                    the vendor's variable. Each VariableName is
                                    unique for each VendorGuid. VariableName
                                    must contain 1 or more characters. If
                                    VariableName is an empty string, then
                                    EFI_INVALID_PARAMETER is returned.
  @param VendorGuid                 A unique identifier for the vendor.
  @param Attributes                 Attributes bitmask to set for the variable.
  @param DataSize                   The size in bytes of the Data buffer. Unless 
                                    the EFI_VARIABLE_APPEND_WRITE,
                                    EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS, or
                                    EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS
                                    attribute is set, a size of zero causes the
                                    variable to be deleted. When the
                                    EFI_VARIABLE_APPEND_WRITE attribute is set,
                                    then a SetVariable() call with a DataSize of
                                    zero will not cause any change to the
                                    variable value (the timestamp associated
                                    with the variable may be updated).
  @param Data                       The contents for the variable. 
  @param TableId                    Unique identifier used to indicate which 
                                    table was updated by this call to
                                    SetVariable.
  @param SyncSize                   Size in bytes of the table that was updated.
 
  @return EFI_SUCCESS               The firmware has successfully stored the
                                    variable and its data as defined by the
                                    Attributes.
  @return EFI_INVALID_PARAMETER     Invalid parameter.
  @return EFI_INVALID_PARAMETER     An invalid combination of attribute bits
                                    was supplied, or the DataSize exceeds the
                                    maximum allowed.
  @return EFI_INVALID_PARAMETER     VariableName is an empty string.
  @return EFI_DEVICE_ERROR          The variable could not be saved due to 
                                    a hardware error.
  @return EFI_WRITE_PROTECTED       The variable in question is read-only.
  @return EFI_OUT_OF_RESOURCES      Not enough storage is available to hold the
                                    variable and its data.
  @return EFI_SECURITY_VIOLATION    The variable could not be written due
                                    to EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS or 
                                    EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS
                                    being set, but the AuthInfo does NOT pass
                                    the validation check carried out by the
                                    firmware.
  @return EFI_NOT_FOUND             The variable trying to be updated or deleted
                                    was not found.

**/
EFI_STATUS
EFIAPI
ScmCmdSetVariable (
  IN      CHAR16            *VariableName,
  IN      EFI_GUID          *VendorGuid,
  IN      UINT32            Attributes,
  IN      UINTN             DataSize,
  IN      VOID              *Data,
  OUT     UINT32            *TableId,
  OUT     UINT32            *SyncSize
  );

/**

  Enumerates the current variable names.

  @param VariableNameSize           Size in bytes of the VariableName buffer.
  @param VariableName               On input, supplies the last VariableName 
                                    that was returned by GetNextVariableName().
                                    On output, returns the Null-terminated
                                    string of the current variable.
  @param VendorGuid                 On input, supplies the last VendorGuid that 
                                    was returned by GetNextVariableName(). On
                                    output, returns the VendorGuid of the
                                    current variable.

  @return EFI_SUCCESS               The function completed successfully.
  @return EFI_NOT_FOUND             The next variable was not found.
  @return EFI_BUFFER_TO_SMALL       The VariableNameSize is too small for the 
                                    result. VariableNameSize has been updated
                                    with the size needed to complete the
                                    request.
  @return EFI_INVALID_PARAMETER     Invalid parameter.
  @return EFI_DEVICE_ERROR          The variable name could not be retrieved 
                                    due to a hardware error.

**/
EFI_STATUS
EFIAPI
ScmCmdGetNextVariableName (
  IN OUT  UINTN             *VariableNameSize,
  IN OUT  CHAR16            *VariableName,
  IN OUT  EFI_GUID          *VendorGuid
  );

/**

  Returns information about the EFI variables.

  @param Attributes                     Attributes bitmask to specify the type 
                                        of variables on which to return
                                        information.
  @param MaximumVariableStorageSize     Pointer to the maximum size of the 
                                        storage space available for the EFI
                                        variables associated with the attributes
                                        specified.
  @param RemainingVariableStorageSize   Pointer to the remaining size of the 
                                        storage space available for EFI
                                        variables associated with the attributes
                                        specified.
  @param MaximumVariableSize            Pointer to the maximum size of an 
                                        individual EFI variables associated with
                                        the attributes specified.

  @return EFI_SUCCESS                   Valid answer returned.
  @return EFI_INVALID_PARAMETER         An invalid combination of attributes 
                                        bits was supplied.
  @return EFI_UNSUPPORTED               The attribute is not supported on this 
                                        platform, and the
                                        MaximumVariableStorageSize,
                                        RemainingVariableStorageSize,
                                        MaximumVariableSize are undefined.

**/
EFI_STATUS
EFIAPI
ScmCmdQueryVariableInfo (
  IN      UINT32            Attributes,
  OUT     UINT64            *MaximumVariableStorageSize,
  OUT     UINT64            *RemainingVariableStorageSize,
  OUT     UINT64            *MaximumVariableSize
  );

/**

  Initializes the Variable storage in TZ.

  @param InitParams                 Pointer to parameters used to initialize the
                                    tables in TZ.
  @param InitParamsSize             Size in bytes of InitParams. 

  @return EFI_SUCCESS               Successfully returned the table.
  @return EFI_INVALID_PARAMETER     Invalid parameter passed.
  @return EFI_OUT_OF_RESOURCES      Not enough memory available to store the 
                                    tables.

**/
EFI_STATUS
EFIAPI
ScmCmdVariableInitialize (
  IN      UINT8                              *InitParams,
  IN      UINT32                             InitParamsSize
  );
  
/**

  Notifies UEFI in TZ that ExitBootServices has occurred.

  @return EFI_SUCCESS               Successfully notified. 
  @return EFI_INVALID_PARAMETER     Invalid parameter passed.

**/
EFI_STATUS
EFIAPI
ScmCmdExitBootServices (
  VOID
  );

/**

  Syncs tables based on ID. If TABLE_ID_RESERVED is passed, all tables are synced.

  @param TableId                    ID corresponding to the tables to sync.

  @return EFI_SUCCESS               Successfully synced the tables.
  @return EFI_INVALID_PARAMETER     An invalid parameter was supplied.

**/
EFI_STATUS
EFIAPI
ScmCmdSyncVarTables (
  IN      UINT32                             TableId
  );

/**
  Copies UefiSecApp Log Buffer into input buffer if size is valid. 
   
  @param DumpLogBuffer           Buffer pointer.
  @param DumpLogBufferSize       Pointer to size of buffer in bytes. 
                                 On input, size of input buffer.
                                 On output, size of data copied or required
                                 if input DumpLogBufferSize is too small.

  @retval EFI_SUCCESS            Log buffer successfully copied. 
  @retval EFI_INVALID_PARAMETER  Invalid parameter supplied. 
  @retval EFI_BUFFER_TOO_SMALL   To small of a buffer supplied.  

**/
EFI_STATUS
EFIAPI
ScmCmdDumpLogBuffer (
  IN OUT  UINT8                     *DumpLogBuffer,
  IN OUT  UINT32                    *DumpLogBufferSize
  );
  
/**

  Trigger SCM call to UEFI SECAPP to delete secure boot variables.
  Note: Deprecated
  
  @return EFI_UNSUPPORTED           Function is not supported in this 
                                    context.  

**/
EFI_STATUS
EFIAPI
ScmCmdDelSecBootVars (
  VOID
  );  
  
/**

  Send Mile stone call;After this Delete secboot SCM call can not be sent
  Note: Deprecated  

  @return EFI_UNSUPPORTED           Function is not supported in this 
                                    context.    

**/
EFI_STATUS
EFIAPI
ScmCmdSendDisableDelSecBootVarsMSCall (
  VOID
  );  

/**
   Un/Marshalling layer for RegisterVariablePolicy

   @see  VARIABLE_POLICY_PROTOCOL::RegisterVariablePolicy
**/
EFI_STATUS
EFIAPI
ScmCmdRegisterVariablePolicy(
  IN VARIABLE_POLICY_ENTRY *PolicyEntry
  );

/**
   Un/Marshalling layer for DumpVariablePolicy

   @see VARIABLE_POLICY_PROTOCOL::DumpVariablePolicy
**/
EFI_STATUS
EFIAPI
ScmCmdDumpVariablePolicy(
  IN OUT UINT8  *Policy,
  IN OUT UINT32 *Size
  );

/**
   Un/Marshalling layer for IsVariablePolicyEnabled
   NOTE: BOOLEAN parameter substitued to UINT8 to avoid any issues with BOOLEAN
         representation between different compilers for UEFI and TZ

   @see  VARIABLE_POLICY_PROTOCOL::IsVariablePolicyEnabled
**/
EFI_STATUS
EFIAPI
ScmCmdIsVariablePolicyEnabled(
  IN OUT UINT8*   State
  );


/**
   Un/Marshalling layer for LockVariablePolicy

   @see  VARIABLE_POLICY_PROTOCOL::LockVariablePolicy
**/
EFI_STATUS
EFIAPI
ScmCmdLockVariablePolicy(
  );


/**
   Un/Marshalling layer for DisableVariablePolicy

   @see  VARIABLE_POLICY_PROTOCOL::DisableVariablePolicy
**/
EFI_STATUS
EFIAPI
ScmCmdDisableVariablePolicy(
  );

#endif /* _SCMCMDVAR_H_ */
