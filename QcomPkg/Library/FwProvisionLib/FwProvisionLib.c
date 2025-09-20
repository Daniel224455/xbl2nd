/** @file FwProvisionLib.c
   
  Firmware provisioning functions used for factory provisioning for 
  Windows-on-Snapdragon (WoS) 
  
  Copyright (c) 2011-2015, 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  
  Copyright (C) 2012 Apple, Inc. All rights reserved. 

  All rights reserved. This program and the accompanying materials 
  are licensed and made available under the terms and conditions of the BSD License 
  which accompanies this distribution. The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php 

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS, 
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED. 

  **/

/*=============================================================================
                              EDIT HISTORY


when         who       what, where, why
--------     ---       ----------------------------------------------------------- 
2018/07/17   gapram    Removed redundant code
2015/04/15   mic       Added 64bit support
2015/03/22   mic       Updated log message
2014/12/08   mic       Removed fuse blowing support
2014/01/26   mic       Fixed KW warnings
2013/08/01   mic       Added WP provisioning support
2013/08/01   wuc       Updated HDCP 2.x provisioning
2013/05/23   rsb       Add support for HDCP 2.x provisioning
2013/04/09   rsb       Switch from RPMBProvisionNoPrompt to RPMBProvision
2012/12/11   mic       Fixed KW warnings
2012/10/08   mic       Moved common macro to FwCommonLib
2012/09/04   rsb       Add reset for provisioning operation
2012/08/10   mic       Changed debug message level for critical errors
2012/07/17   mic       Added ToolSupportProtocol for OPM handshake and removed 
                       ToolSupportLib
2012/07/13   mic       Added WinSecApp OPM handshake support
2012/07/09   mic       Re-organized debug message logic for privision flag check
2012/07/06   mic       Added OPM Provisioning support, removed ReadKeystoreFile function. 
2012/07/02   rsb       Removed EFI_INVALID_PARAMETER check from Debug Policy
                       provisioning since it is no longer required
2012/06/15   mic       Adjusted the return status check for the result of Debug 
                       Policy provisioning
2012/06/05   rs        Replaced AsciiPrints with DEBUG Macro
2012/05/07   jd        Migrated to DPP Protocol. Cleanup
2012/03/12   rs        Added SSD Provisioning changes
2012/02/26   jd        Added support for raw-partition based device provisioning  
2012/01/25   jd        Use a separate commit interface for FactoryTool provisioning
2011/09/09   jthompso  first revision
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathUtilities.h>
#include <Protocol/BlockIo.h>
#include <Protocol/EFIEncryption.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/EFIDpp.h>
#include <Protocol/EFIToolSupport.h>
#include <Protocol/LoadedImage.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Guid/FileInfo.h>
#include <Guid/GlobalVariable.h>
#include <Guid/ImageAuthentication.h>
#include <Library/FwCommonLib.h>
#include <Library/DebugLib.h>
#include <Library/QcomLib.h>
#include "FwProvFuseLib.h"

/**
 * The user Entry Point for provisioning. 
 * 
 * 
 * @param ImageHandle - Passed from calling application
 * @param SystemTable - Passed from calling application
 * 
 * @return EFI_STATUS 
 */
EFI_STATUS
RunFwProvisionMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status            = EFI_SUCCESS;
  
  DEBUG((EFI_D_INFO,"Entering RunFwProvisionMain:\n"));

  /* QRDProvStage1 */
  Status = QrdProvStage1();
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR,"Warning: QrdProvStage1 returned error: %r\n", Status));
    Status = EFI_SUCCESS;
    goto RunFwProvisionMainExit;
  }  

RunFwProvisionMainExit:

  return Status;
}
  
