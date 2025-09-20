/** @file Utils.h
   
  Test utilities for DisplayDxe graphics output protocol

  Copyright (c) 2019 Qualcomm Technologies, Inc.  
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  
**/
#include <Uefi.h>

/*=========================================================================
  Public helper APIs
 ==========================================================================*/


/* DisplayUtilsParser
*/
UINTN DisplayUtilsParser(CHAR8* pCommandLine, CHAR8* pArgs, UINTN uNumArgs);
    


/* DisplayUtilsHelp
*/
void DisplayUtilsHelp();



