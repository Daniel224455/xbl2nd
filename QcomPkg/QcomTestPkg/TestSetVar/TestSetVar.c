/** @file TestSetVar.c

#How to run
1.  SetVariable(TIME_BASED, KEKA_T1)  Result = SUCCESS. Variable = KEKA. TimeStamp = T1
2.  Program PK and ensure SecureBoot is enabled
3.  Reboot
4.  SetVariable(TIME_BASED, KEKD_T4)  Result = SUCCESS. Variable = KEKD. TimeStamp = T4   (NOTE*)
5.  SetVariable(TIME_BASED | APPEND_WRITE, KEKB_T2)  Result = SUCCESS. Variable = KEKD||KEKB. TimeStamp = T4  (NOTE**)
6.  SetVariable(TIME_BASED, KEKC_T3)  Result = FAIL  (NOTE***)


  Copyright (c) 2017 Qualcomm Technologies Inc. All rights reserved.
  Portions Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved. 
  This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/21/17   bd      Initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TestInterface.h>
#include <Library/ScmCmdLib.h>
#include <Protocol/EFIScm.h>
#include <Protocol/EFIVariableServices.h>
#include <Drivers/VariableDxe/ScmCmdVar.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomLib.h>
#include <Guid/ImageAuthentication.h>
 
#define RETURN_UINTN_EFI_STATUS(a)                       \
        (((a) == (0)) ? (a) : (ENCODE_ERROR((a)&0xFF)))

        
#define KEKA_T1        "fv1:\\OEM_KEK_May.bin"
#define KEKB_T2        "fv1:\\OEM_KEK_Append.bin"
#define KEKC_T3        "fv1:\\OEM_KEK_Sep.bin"
#define KEKD_T4        "fv1:\\OEM_KEK_Nov.bin"
#define OEM_PK         "fv1:\\OEM_PK.bin"

typedef enum _TestOptions { Default, SetKEK } TestOptions;
TestOptions TestOption = Default;

EFI_STATUS
ProcessCommand (UINTN Argc, CHAR8** Argv)
{
  CHAR8 *CmdStr, *ArgStr = NULL;

  CmdStr = Argv[0];
  --Argc;

  if (Argc)
    ArgStr = Argv[1];

  AsciiPrint ("Processing Cmd: %a", CmdStr);
  if (ArgStr)
     AsciiPrint (", Arg: %a", ArgStr);
  AsciiPrint ("\r\n");

  if (AsciiStriCmp ((CHAR8*)CmdStr, "KEK") == 0)
    TestOption = SetKEK;
  else 
    TestOption = Default;
  	  
  return EFI_SUCCESS;
}

EFI_STATUS
ParseCommandLine (EFI_HANDLE *ImageHandle)
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;	
  UINTN      Argc = 0;
  CHAR8      **Argv;

  Status = GetCmdLineArgs (*ImageHandle, &Argc, &Argv);
  if (EFI_ERROR(Status))
    return Status;

  if ((Argc != 0) && (Argv != NULL))
    Status = ProcessCommand (Argc, Argv);

  return Status;
}

/**
  Read a secure UEFI variable from MainFv and write to disk

  @param[in]  Attr      Additional attributes to OR with default values
  @param[in]  FileName  Filename of the UEFI secure key
   
  @retval EFI_SUCCESS     The data is found from the firmware volume and written
                          to the UEFI NV variable.
  @retval EFI_UNSUPPORTED The given key type is not supported.
  @retval EFI_NOT_FOUND        The key data cannot be found in the firmware volume
  @retval EFI_OUT_OF_RESOURCES There were not enough resources to allocate a 
                                data buffer or complete the operations
  @retval EFI_ACCESS_DENIED The operation is not allowed.
  @retval EFI_SECURITY_VIOLATION The operation is a security violation.
**/
EFI_STATUS
TestSetVariable(
  UINT32           KeyType,
  UINT32            Attr,
  IN   CHAR8               *FileName
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  UINT8            *DataBuffer = NULL;
  UINTN             DataSize = 0;
  EFI_GUID         *VarGuid = NULL;
  CHAR16           *VarName = NULL;
  STATIC EFI_GUID   GlobalVariableGuid = EFI_GLOBAL_VARIABLE;

  UINT32            Attributes = (EFI_VARIABLE_RUNTIME_ACCESS | 
                                  EFI_VARIABLE_BOOTSERVICE_ACCESS |
                                  EFI_VARIABLE_NON_VOLATILE |
                                  EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS) | Attr;

  VarGuid = &GlobalVariableGuid;  

  Status = LoadFromFV(FileName, &DataBuffer, &DataSize);

  if (EFI_ERROR(Status))
  {
    goto CleanUp;
  }

  if (KeyType == 0x0)
  {
    VarName = EFI_KEY_EXCHANGE_KEY_NAME;
  }
  else if (KeyType == 0x1)
  {
    VarName = EFI_PLATFORM_KEY_NAME;
  } 
  else
  {
    AsciiPrint("KeyType not specified\n");
    return EFI_INVALID_PARAMETER;
  }

  /* Attempt to write the data to the specified variable. */
  Status = gRT->SetVariable(VarName, VarGuid, Attributes, DataSize, DataBuffer);
  if (Status == EFI_NOT_FOUND)
  {
    Status = EFI_SUCCESS;
  }
  else if (EFI_ERROR(Status))
  {
    goto CleanUp;
  }
  else
  {
    /* On successful write to variable services, trigger a flush to ensure it is written to disk without
       requring a reset. */
    Status = FlushVariableServices();
  }
  
CleanUp:

  /* Free up the key buffer */
  if (DataBuffer != NULL)
  {
      gBS->FreePool(DataBuffer);
      DataBuffer = NULL;
  }

  return Status;
}


/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
TestSetVarMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  
  EFI_STATUS  Status= EFI_SUCCESS;  
  
  TEST_START("TestSetVar");

  Status = ParseCommandLine (&ImageHandle);
  if (Status != EFI_SUCCESS)
    return Status;
  
  AsciiPrint ("\n --------------------------- \n");
  
  if(TestOption == Default)
  {
    Status = TestSetVariable(0, 0,KEKA_T1);
    if(Status == EFI_SUCCESS)
    {
      AsciiPrint ("Setting variable KEKA_T1, status = %r \n", Status);
    }
    else
    {
      AsciiPrint ("Setting variable KEKA_T1, status = %r \n", Status);
      goto Done;
    }
    
    Status = TestSetVariable(1, 0, OEM_PK);
    if(Status == EFI_SUCCESS)
    {
      AsciiPrint ("Setting variable OME_PK, status = %r \n", Status);
    }
    else
    {
      AsciiPrint ("Setting variable OME_PK, status = %r \n", Status);
      goto Done;
    }
  }
  else if(TestOption == SetKEK)
  {
    /* Before running below tests, reboot and ensure secureboot is enabled */
    Status = TestSetVariable(0, 0, KEKD_T4);
    if(Status == EFI_SUCCESS)
    {
      AsciiPrint ("Setting variable KEKD_T4 status = %r \n", Status);
    }
    else
    {
      AsciiPrint ("Setting variable KEKD_T4 status = %r \n", Status); 
      goto Done;
    }

    Status = TestSetVariable(0, EFI_VARIABLE_APPEND_WRITE  , KEKB_T2);
    if(Status == EFI_SUCCESS)
    {
      AsciiPrint ("Appending variable KEKB_T2 status = %r \n", Status);
    }
    else
    {
      AsciiPrint ("Appending variable KEKB_T2 status = %r \n", Status);
      goto Done;
    }

    Status = TestSetVariable(0, 0, KEKC_T3);
    if(Status == EFI_SUCCESS)
    {
      AsciiPrint ("Setting variable KEKC_T3 status = %r \n", Status);
    }
    else
    {
      AsciiPrint ("Setting variable KEKC_T3 status = %r \n", Status);
      goto Done;
    }
  }
  
  AsciiPrint ("\n --------------------------- \n");  
  Done:

  TestStatus("TestSetVar", Status);
  TEST_STOP("TestSetVar");
  return Status;
}
