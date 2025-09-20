/*                                                                    
                                                                   
                     TpmLocal.h   

  TPM definitions, copied from fTPM code.
 
  Copyright (c) 2012-2014 Qualcomm Technologies, Inc.  All Rights Reserved.

*/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 12/20/18   shl      Cleanup
 12/03/14   sk       Updated copyright
 12/10/13   sm       Updated variable types
 05/22/12   shl      Fixed the compiler upgrade issue. Make pact() change
 02/12/12   shl      Fixed a warning of without end line
 01/18/12   shl      Initial version

=============================================================================*/
#include <IndustryStandard/Tpm20.h>

#pragma pack (1)

typedef struct
{
  UINT16    size;
  UINT8     buffer[20];
} DIGEST;

typedef struct {
    UINT32        sessionHandle;
    DIGEST        nonce;
    TPMA_SESSION  sessionAttributes;
    DIGEST        auth;
} AUTH_SESSION_COMMAND;

typedef struct 
{
  UINT32  authHandle;
} Clear_In;

typedef struct
{
  UINT32   auth;
  UINT8    disable;
} ClearControl_In;

typedef struct 
{
  TPM2_COMMAND_HEADER    header;
  AUTH_SESSION_COMMAND   authSessionCmd;
  Clear_In               inputParameters;
} TPM2ClearCmdType;

typedef struct 
{
  TPM2_RESPONSE_HEADER    header;
  AUTH_SESSION_COMMAND    authSessionCmd;
} TPM2ClearRespType;

typedef struct 
{
  TPM2_COMMAND_HEADER     header;
  AUTH_SESSION_COMMAND    authSessionCmd;
  ClearControl_In         inputParameters;
} TPM2ClearControlCmdType;

typedef struct 
{
  TPM2_RESPONSE_HEADER    header;
  AUTH_SESSION_COMMAND    authSessionCmd;
} TPM2ClearControlRespType;

#pragma pack ()

