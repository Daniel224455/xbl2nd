/** @file
                    ButtonTest.c

  This is a simple application to test physical hardware button functionality.

  Copyright (c) 2017, 2018 by Qualcomm Technologies, Inc. All Rights Reserved.

**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 07/27/17   ianr     Initial Version

=============================================================================*/

#include <DDITF.h>
#include <Library/TestInterface.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleTextInEx.h>
#include <Uefi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


BOOLEAN allPressed(boolean *allPressedArray, uint32 length) {
  BOOLEAN retValue = TRUE;
  for (int i = 0; i < length; i++) {
    if (allPressedArray[i] == FALSE) {
      retValue = FALSE;
      break;
    }
  }
  return retValue;
}

//This sets up the protocol for reading keyboard input, stores and parses user input
UINT32 ButtonTestMain (uint32 dwParam, char *apszParam[])
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINTN         WaitCount;
  UINTN         WaitIndex;
  EFI_EVENT     WaitList[2];
  EFI_EVENT     TimerEvent;
  UINT64        Timeout = 600000000; //timeout after 1 min
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_KEY_DATA testKeyData;
  WaitCount   = 2;
  Status = gBS->OpenProtocol (
                  gST->ConsoleInHandle,
                  &gEfiSimpleTextInputExProtocolGuid,
                  (VOID**)&SimpleEx,
                  gImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if ((Status != EFI_SUCCESS) || (NULL == SimpleEx)) {
    AsciiPrint("ERROR: ButtonsTest - Failed to locate SimpleTextInputProtocol! Error Code = 0x%08X\r\n", Status);
    Status = TF_FAILURE;
    return Status;
  }
  AsciiPrint("ButtonTest: SimpleTextInput Protocol located \n");


  //create array with size = inputsize initalized to FALSE
  boolean allPressedArray[32] = {FALSE};

  Print(L"\nPlease press all hardware buttons individually to test their functionality\n");

  while(!(allPressed(allPressedArray, dwParam))){
    Status = gBS->CreateEvent(EVT_TIMER, 0, NULL, NULL, &TimerEvent);
    gBS->SetTimer (TimerEvent, TimerRelative, Timeout);
    
    WaitList[0] = gST->ConIn->WaitForKey;
    WaitList[1] = TimerEvent;

    Status = gBS->WaitForEvent (WaitCount, WaitList, &WaitIndex);
    
    gBS->CloseEvent(TimerEvent);
    
    if (EFI_ERROR(Status) && WaitIndex == 1) {
      Status = EFI_TIMEOUT;
      AsciiPrint("Buttons Test: gBS->WaitForEvent FAILED because a button was not pressed within 1 min\n");
      return Status;
    }

    // Key event signaled
    Status = SimpleEx->ReadKeyStrokeEx (SimpleEx, &testKeyData);
    if (Status != EFI_SUCCESS) {
      AsciiPrint("Buttons Test: SimpleEx->ReadKeyStrokeEx FAILED\n");
      return Status;
    }

    AsciiPrint("The button pressed was: %X\n", testKeyData.Key.UnicodeChar);
    
    for (int i=0; i<dwParam; i++) {
      int val = (int)strtol(apszParam[i], NULL, 16);
      if (val == testKeyData.Key.UnicodeChar || val == testKeyData.Key.ScanCode) {
        allPressedArray[i] = TRUE;
        break;
      }
    }    
  }
  
  Status = EFI_SUCCESS;
  return Status;
}