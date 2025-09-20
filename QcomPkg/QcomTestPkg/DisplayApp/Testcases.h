/** @file Testcases.h
   
  Test application for DisplayDxe graphics output protocol

  Copyright (c) 2019 Qualcomm Technologies, Inc.  
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  
**/
#include <Uefi.h>

/*=========================================================================
  Public Structures and Definitions
 ==========================================================================*/



/*=========================================================================
  Public API's
 ==========================================================================*/
/* Display BVT Command Line Parser */
UINTN DisplayBVTParser(CHAR8* pCommandLine, CHAR8* pArgs);

/* Display BVT Help */
void DisplayBVTHelp();

