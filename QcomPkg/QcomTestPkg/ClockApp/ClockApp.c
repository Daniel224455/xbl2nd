/** @file ClockApp.c

  This file contains test clock driver entry point.  On UEFI,
  there are two different ways to use clock driver: (1) DAL
  framework (2) EFI clock protocol.  This application shows
  sample code for both options.

  Copyright (c) 2017 - 2019 Qualcomm Technologies, Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 04/02/19   gbedi     Created.

=============================================================================*/



/*=========================================================================
      Include Files
==========================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/QcomLib.h>
#include <Library/TestInterface.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/EFIClock.h>

/*=========================================================================
      Type Definitions
==========================================================================*/


/*=========================================================================
      Function Prototypes
==========================================================================*/
extern EFI_GUID gQcomTokenSpaceGuid;

/*=========================================================================
      Functions
==========================================================================*/


/**
  Test clock application entry point.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/

EFI_STATUS
EFIAPI
ClockAppMain (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
	
  EFI_CLOCK_PROTOCOL *ClockProtocol = NULL;
  CONST CHAR8         *ClockName = "gcc_sys_noc_cpuss_ahb_clk";
  EFI_STATUS          Status = EFI_SUCCESS;
  UINTN               ClockId;
  UINT32              FreqHz = 0;
  BOOLEAN             bClockOn = FALSE;

  // Initializing clock protocol
  if(!ClockProtocol)
  {
    Status = gBS->LocateProtocol(&gEfiClockProtocolGuid, NULL, (VOID **)&ClockProtocol);
    if (EFI_ERROR(Status)) 
    {
      ClockProtocol = NULL;
	    AsciiPrint ("Failed to locate CLOCK protocol\r\n");    
    }
  } 

  if (ClockProtocol && (EFI_SUCCESS == Status))
  {
    // Get Clock ID
    Status = ClockProtocol->GetClockID (ClockProtocol, ClockName, &ClockId);
    if(EFI_SUCCESS != Status)
    {
	     AsciiPrint ("ERROR: Failed to Get clock ID for %a\r\n", ClockName);
       return Status;
    }
    else
    {
	      AsciiPrint ("Successfully got the clock ID for %a\r\n", ClockName);
    }
	
    // Get IsClockOn Status
    Status = ClockProtocol->IsClockOn(ClockProtocol, ClockId, &bClockOn);
    if(EFI_SUCCESS == Status && FALSE == bClockOn)
    { 
      // Enable clock.
      Status = ClockProtocol->EnableClock(ClockProtocol, ClockId);
      // Update the status of bClockOn
      Status = ClockProtocol->IsClockOn(ClockProtocol, ClockId, &bClockOn);
      if(EFI_SUCCESS !=Status )
      {  
        AsciiPrint ("ERROR: Failed to enable the clock: of %a\n", ClockName);
      }   
      else
      {
	      AsciiPrint ("Successfully enabled the clock: %a\r\n", ClockName);
      }
    }
    else if (EFI_SUCCESS == Status && TRUE == bClockOn)
    {
 	    AsciiPrint ("Successfully got the clock status for %a\r\n", ClockName);
    }
    else
    {
      AsciiPrint ("ERROR: Failed to query the clock status for %a\r\n", ClockName);
    }  

    // Get Clock Frequency
    Status = ClockProtocol->CalcClockFreqHz(ClockProtocol, ClockId, &FreqHz);
    if(EFI_SUCCESS != Status )
    {
      AsciiPrint ("ERROR: Failed to calculate clock frequency of %a\n", ClockName);
    }
    else
    {
	    AsciiPrint("%-30a %3a    %dHz\n", ClockName, (bClockOn)?"ON":"OFF", (bClockOn)?FreqHz:0);
    }
  }

  return EFI_SUCCESS;
}
