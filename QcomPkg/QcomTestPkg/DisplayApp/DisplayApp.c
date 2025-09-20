/** @file DisplayApp.c
   
  Test application for DisplayDxe graphics output protocol

  Copyright (c) 2010-2019 Qualcomm Technologies, Inc.  
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  
**/


/*=========================================================================
      Include Files
==========================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include "Testcases.h"
#include "Utils.h"

/*=========================================================================
   Defines
==========================================================================*/
#define MAX_STRING      128

/*=========================================================================
      Type Definitions
==========================================================================*/


/*=========================================================================
     Local Function Declarations
==========================================================================*/


/*=========================================================================
     Helper Functions
==========================================================================*/


/* DisplayApp Help
*/
static void DisplayAppHelp(void)
{
  AsciiPrint ("Usage:\n");
  AsciiPrint (" DisplayApp -[option]:[value]\n\n");
  AsciiPrint (" options:\n");  
  // Print section specific help
  DisplayUtilsHelp();
  DisplayBVTHelp();

}


/* DisplayApp Command line parser
 * 
 * Parse a string in the format -<Param>:<Value>
 */
static UINTN DisplayParseCommand(CHAR8 *pCommandLine, CHAR8 *pParam, CHAR8 *pValue)
{
  UINTN params = 0;
  UINTN strlength = AsciiStrLen(pCommandLine);

  // Initialize output to null
  pParam[0] = '\0';
  pValue[0] = '\0';

  // If we detect the swich and at least 1 valid character
  if ((pCommandLine[0] == '-') &&
      (pCommandLine[0] != ':') &&
      (pCommandLine[0] != ' ') &&
      (strlength > 2))
  {
    UINTN uCount;

    // At least one param detected
    params++;

    // Find the length of the parameter
    for (uCount = 1; uCount<strlength; uCount++)
    {
      if ((pCommandLine[uCount] == ':') ||
        (pCommandLine[uCount] == ' '))
      {
        break;
      }
    }

    // Copy the paramter
    AsciiStrnCpy(pParam, &pCommandLine[1], uCount - 1);
    pParam[uCount-1] = '\0';

    // Copy the optional value
    if ((pCommandLine[uCount] == ':') &&
      ((uCount + 1)<strlength))
    {
      params++;

      AsciiStrnCpy(pValue, &pCommandLine[uCount + 1], strlength - uCount - 1);
      pValue[strlength-uCount-1] = '\0';
    }
  }

  return params;
}


/*=========================================================================
     Main Entry Point
==========================================================================*/

/**
  The user entry point for the display application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
DisplayAppMain (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS   eStatus  = EFI_INVALID_PARAMETER;
  UINTN        Argc     = 0;
  UINTN        NumArgs  = 0;
  CHAR8      **Argv;
  CHAR8        Param[MAX_STRING];
  CHAR8        Value[MAX_STRING];

  AsciiPrint("DisplayApp (Compiled on: %a)\n\n", __DATE__);
  
  // Get the command line arguments
  if ((EFI_SUCCESS == GetCmdLineArgs (ImageHandle, &Argc, &Argv)) &&
      (Argc > 0))
  {
    // Parse the arguments
    NumArgs = DisplayParseCommand((CHAR8*)Argv[0], (CHAR8*)&Param, (CHAR8*)&Value);

    // If we have at least one parameter
    if (NumArgs)
    {
      // Command line passed -<Param>:<Value>, we really just want to know if <Value> was passed for the function below so subtract the number of
      // parased commands in the command line.
      NumArgs--;
      eStatus = EFI_SUCCESS;
    }
  }

  // Print help on failure
  if (EFI_SUCCESS != eStatus)
  {
    DisplayAppHelp();
    return 0;
  }

  // Check for which test case / utility to execute
  if (DisplayBVTParser(Param, Value))
  {
    eStatus = EFI_SUCCESS;
  }
  else if (DisplayUtilsParser(Param, Value, NumArgs))
  {
    eStatus = EFI_SUCCESS;
  }
  else
  {
    AsciiPrint("Error: %a is an invalid parameter\n", Param);
  }

  return eStatus;
}

