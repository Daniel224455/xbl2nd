/** @file
   
 Declaration of fTPM command defintions, structures. 

  Copyright (c) 2012-2017 Copyright Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.   
**/

/*=============================================================================
                              EDIT HISTORY


 when        who     what, where, why
 --------    ---    -----------------------------------------------------------
 05/19/15   sk       Added support for TCG 2.0
 08/15/14   sm      Switched to new SCM API
 04/22/13   bmuthuku Send signal to fTPM to disable NV flushing.
 02/21/13    shl     ScmDxe cleanup.
 01/02/13    shl     Reverted the change, will check in later once winsecapp is checked in
 12/21/12    shl     Moved tpm command to this file, adapted to TZ1.4 interface
 07/18/11    shl     Initial version.

=============================================================================*/

#ifndef _TZFTPMCMD_H_
#define _TZFTPMCMD_H_

#include <Uefi.h>
#include <Include/IndustryStandard/UefiTcgPlatform.h>

/** Common Definitions for fTPM command */
#define HASH_MAX_SIZE    64

typedef enum
{
  HASH_SHA1_LEN   = 20,
  HASH_SHA256_LEN = 32, 
  HASH_SHA384_LEN = 32, 
  HASH_SHA512_LEN = 32 
} HashLenType;

#define    TPM_CC_Startup                 (TPM_CC)(0x00000144)    
#define    TPM_CC_PCR_Extend              (TPM_CC)(0x00000182)    
#define    TPM_ST_NO_SESSIONS             (TPM_ST)(0x8001)    
#define    TPM_ST_SESSIONS                (TPM_ST)(0x8002)    
#define    TPM_SU_CLEAR                   (TPM_SU)(0x0000)    
#define    TPM_SU_STATE                   (TPM_SU)(0x0001)    
#define    TPM_ALG_SHA1                   (TPM_ALG_ID)(0x0004)    
#define    TPM_ALG_SHA256                 (TPM_ALG_ID)(0x000B)   
#define    TPM_RS_PW                      (TPM_RH)(0x40000009)    

#define    TPM_ACPI_TABLE_SIGNATURE       0x324D5054

#define TZEXEC_TPM_SVC_ID           0x0004000
#define SERVICE_TPM_CREATE_CMD(x)          (TZEXEC_TPM_SVC_ID | x)

#define TPM_SIGNAL_POWER_ON                SERVICE_TPM_CREATE_CMD(1)
#define TPM_SIGNAL_POWER_OFF               SERVICE_TPM_CREATE_CMD(2)
#define TPM_SIGNAL_PHYS_PRES_ON          SERVICE_TPM_CREATE_CMD(3)
#define TPM_SIGNAL_PHYS_PRES_OFF         SERVICE_TPM_CREATE_CMD(4)
#define TPM_SIGNAL_HASH_START          SERVICE_TPM_CREATE_CMD(5)
#define TPM_SIGNAL_HASH_DATA           SERVICE_TPM_CREATE_CMD(6)
#define TPM_SIGNAL_HASH_END            SERVICE_TPM_CREATE_CMD(7)
#define TPM_SEND_COMMAND             SERVICE_TPM_CREATE_CMD(8)
#define TPM_SHUTDOWN                       SERVICE_TPM_CREATE_CMD(9)
#define TPM_STARTUP                        SERVICE_TPM_CREATE_CMD(10)
#define TPM_SET_CONTROL_AREA               SERVICE_TPM_CREATE_CMD(11)
#define TPM_GET_CONTROL_AREA               SERVICE_TPM_CREATE_CMD(12)

#define TPM_UEFI_EXIT_BOOT_SERVICE_DONE    SERVICE_TPM_CREATE_CMD(19)
#define TPM_HLOS_SCM_IS_UP                 SERVICE_TPM_CREATE_CMD(20)


#define MAX_TPM_SVC_INPUT_SIZE           1024
#define MAX_TPM_SVC_OUTPUT_SIZE          1024

//typedef UINT16 TPM_ST;
//typedef UINT32 TPM_CC;
//typedef UINT32 TPM_RC;
//typedef UINT32 TPM_RH;
//typedef UINT16 TPM_SU;
//typedef UINT16 TPM_ALG_ID;

//Adding TPM req and rsp used for tz1.4 interface
typedef struct _TPM_SEND_CMD_REQ
{
    UINT32                   commandId;
    UINT32                   inputSize;
    UINT8                    input[MAX_TPM_SVC_INPUT_SIZE];    
}  __attribute__ ((packed)) TPM_SEND_CMD_REQ, *PTPM_SEND_CMD_REQ;

typedef struct _TPM_SEND_CMD_RSP
{
    UINT32                   outputSize;
    UINT8                    output[MAX_TPM_SVC_OUTPUT_SIZE];    
}  __attribute__ ((packed)) TPM_SEND_CMD_RSP, *PTPM_SEND_CMD_RSP;


#endif /* _TZFTPMCMD_H_ */

