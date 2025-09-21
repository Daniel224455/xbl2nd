/** @file
                    CommandLineReferenceApp.c

  This is a simple demonstration program of an interface to call the HWDiagnostics libraries.

  Copyright (c) 2017, 2018 by Qualcomm Technologies, Inc. All Rights Reserved.

**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 07/17/17   ianr     Initial Version

=============================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TestInterface.h>
#include <Protocol/SimpleTextInEx.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <DDITF.h>
#include <string.h>   
#include <QcomLib.h>
#include <api/services/memheap_lite.h>

//This is what the LogFS diagnostic file will be named. Can be changed if needed.
#define LOG_FILE_STORAGE_LOCATION L"LLD_Diagnostic_Logs"
//This is the partition (in this case LogFS) where the log file will be placed.
#define LOG_PARTITION L"logfs"

static char TemporaryOutputBuffer[1000];
extern const TF_TestFunction_Descr LLD_test_list[]; //defined in ..\LLD\LLDTestList.c
     
/* 
 * This is a wrapper for the AsciiPrint function. Allows for us to overwrite the AsciiPrint Function
 * to do logging, without having to change the AsciiPrint function in every library we use.
 * To use normal AsciiPrint, a function in this file needs to call __real_AsciiPrint. A normal
 * AsciiPrint call will default to calling __wrap_AsciiPrint. The flag --wrap=AsciiPrint needs to be
 * added to the Linker so it knows how to replace AsciiPrint (currently included in the inf). 
 */
void __real_AsciiPrint(const char *data, ...);
void __wrap_AsciiPrint(const char *data, ...){
  UINT32 SizeOfLogFile = 0;
  va_list InputList;
  va_start(InputList, data);
  char TempDataStorage[256];
  TempDataStorage[0] = data[0];
  for (int i = 1; (i < strlen(data)+1) && (i < 256); i++){
    TempDataStorage[i] = data[i];
    if(data[i-1] == '%'){
      if(data[i] == 'a'){
        TempDataStorage[i] = 's';
      }
    }
    
  }
  memset(TemporaryOutputBuffer, 0x0, sizeof(TemporaryOutputBuffer));
  vsnprintf(TemporaryOutputBuffer, sizeof(TemporaryOutputBuffer), TempDataStorage, InputList);
  UINTN BytesToWriteLength = strlen(TemporaryOutputBuffer);
  //Gets the file size of the current LogFS file, so it doesn't overwrite what it has currently
  // written in the run cycle.
  GetFileSize(LOG_FILE_STORAGE_LOCATION,
              NULL,
              NULL,
              LOG_PARTITION,
              TRUE,
              NULL,
              &SizeOfLogFile);
  //Writes the contents of the ascii print to the LogFS file
  WriteFile(LOG_FILE_STORAGE_LOCATION,
            NULL,
            NULL,
            LOG_PARTITION,
            TRUE,
            NULL,
            &BytesToWriteLength,
            SizeOfLogFile,
            (UINT8 *)TemporaryOutputBuffer,
            SizeOfLogFile+BytesToWriteLength);
  va_end(InputList);
}

//executes each test
void ExecuteTestCases(int CurrentEntry){
  __real_AsciiPrint("\x1b[32m%a test started..................\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
  if(0 == strcmp("Button", LLD_test_list[CurrentEntry].pszTestName)){
    uint32 dwParam = 4;
    char* apszParam[4] = {"0D", "02", "01", "102"};
    if(0 == LLD_test_list[CurrentEntry].pTestFunc(dwParam, apszParam)){
      __real_AsciiPrint("\x1b[32m%a passed\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
    } else {
       __real_AsciiPrint("\x1b[31m%a failed\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
    }
    __real_AsciiPrint("\x1b[32m%a test ended..................\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
  } else if (0 == strcmp("Display", LLD_test_list[CurrentEntry].pszTestName)){
    uint32 dwParam = 3;
    char* apszParam[3] = {"0", "1", "2"};
    if(0 == LLD_test_list[CurrentEntry].pTestFunc(dwParam, apszParam)){
      __real_AsciiPrint("\x1b[32m%a passed\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
    } else {
       __real_AsciiPrint("\x1b[31m%a failed\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
    }
    __real_AsciiPrint("\x1b[32m%a test ended..................\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
  } else {
    if(0 == LLD_test_list[CurrentEntry].pTestFunc(0, NULL)){
      __real_AsciiPrint("\x1b[32m%a passed\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
    } else {
       __real_AsciiPrint("\x1b[31m%a failed\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
    }
    __real_AsciiPrint("\x1b[32m%a test ended..................\x1b[0m\n", LLD_test_list[CurrentEntry].pszTestName);
  }

}


//Checks to see if the user input is a valid test name, else returns failure
TF_Result CheckInput(char *UserInput){
  int CurrentEntry = 0;
  while(NULL != LLD_test_list[CurrentEntry].pszTestName){
    if(0 == strcmp(UserInput, LLD_test_list[CurrentEntry].pszTestName)){
      ExecuteTestCases(CurrentEntry);
      return TF_SUCCESS;
    }
    CurrentEntry++;
  }
  return TF_FAILURE;
}

//The main app, sets up the menu UI, begins taking user input.
EFI_STATUS EFIAPI LLDReferenceAppMain (IN EFI_HANDLE        ImageHandle,
                                               IN EFI_SYSTEM_TABLE  *SystemTable)
{
  EFI_STATUS    Status;
  UINTN         WaitCount;
  UINTN         WaitIndex;
  EFI_EVENT     WaitList[2];
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_KEY_DATA testKeyData;
  WaitCount   = 1;
  WaitList[0] = gST->ConIn->WaitForKey;

  //Deletes the log file if it already exists, so that the details don't get appended to an already
  // existing file and cause the size to balloon after several runs.
  DeleteFile (LOG_FILE_STORAGE_LOCATION,
              NULL,
              NULL,
              LOG_PARTITION,
              TRUE,
              NULL);
  //This sets up the protocol for reading keyboard input, stores and parses user input
  Status = gBS->OpenProtocol (
                  gST->ConsoleInHandle,
                  &gEfiSimpleTextInputExProtocolGuid,
                  (VOID**)&SimpleEx,
                  gImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  ASSERT_EFI_ERROR (Status);

  char UserInput[128];
  int index = 0;

  //Sets up the menu UI, parses input
  __real_AsciiPrint("================ Command Line Interface Begin ================\n");
  __real_AsciiPrint("Available Tests:\n");
  int CurrentEntry = 0;
  while(NULL != LLD_test_list[CurrentEntry].pszTestName){
    __real_AsciiPrint("    %a\n", LLD_test_list[CurrentEntry].pszTestName);
    CurrentEntry++;
  }
  __real_AsciiPrint("\nEnter \x1b[31;1mAll\x1b[0m to run all listed tests, else enter \x1b[31;1m<test_name>\x1b[0m for specific test:\n> ");
  while(index < 127){
    Status = gBS->WaitForEvent (WaitCount, WaitList, &WaitIndex);
    ASSERT_EFI_ERROR (Status);
    // Key event signaled
    Status = SimpleEx->ReadKeyStrokeEx (SimpleEx, &testKeyData);
    ASSERT_EFI_ERROR (Status);
    if(0x08 != testKeyData.Key.ScanCode){
      __real_AsciiPrint("%c", testKeyData.Key.UnicodeChar);
    }
    //Enter key breaks loop, submits user entry
    if (0x0D == testKeyData.Key.UnicodeChar){
      break;
    //Backspace support
    } else if(0x08 == testKeyData.Key.ScanCode){
      if (index > 0){
        __real_AsciiPrint("%c", 0x7F);
        index--;
      }
    } else {
      UserInput[index++] = (char)testKeyData.Key.UnicodeChar;
    }
  }
  UserInput[index] = '\0';
  __real_AsciiPrint("\n");

  //Executes all tests, else sends to helper function to check input if not executing all.
  if (0 == strcmp(UserInput, "All") || !strcmp(UserInput, "all") || !strcmp(UserInput, "ALL")){
    CurrentEntry = 0;
    while(NULL != LLD_test_list[CurrentEntry].pszTestName){
      ExecuteTestCases(CurrentEntry);
      CurrentEntry++;
    }
  } else {
    TF_Result Result = CheckInput(UserInput);
    if (TF_FAILURE == Result){
      __real_AsciiPrint("\nTest not found in list. Please check your entry and run again.\n");
    }
  }
  __real_AsciiPrint("================ Command Line Interface End ================\n");
  return Status;
}