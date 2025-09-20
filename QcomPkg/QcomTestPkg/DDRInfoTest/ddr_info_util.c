/*
===========================================================================
*/
/**
  @file ddr_info_util.c

*/
/*
  ====================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated.  All Rights Reserved.

  ====================================================================

  when       who     what, where, why
  --------   ---     -------------------------------------------------
  10/16/18   dais    Initial revision.

  ====================================================================
*/

/*=========================================================================
      Include Files
==========================================================================*/
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIDisplayPwrCtrl.h>
#include <Protocol/EFIQcomEnv.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>

/*=========================================================================
      Variables
==========================================================================*/
EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL *DispCtrl = NULL;

/* =========================================================================
**  Functions
** =========================================================================*/

/*==============================================================================

  FUNCTION      ddr_uefi_display_init

  DESCRIPTION   Locate Display Control protocol

==============================================================================*/

UINT32 ddr_uefi_display_init()
{
  EFI_STATUS status = EFI_SUCCESS;
  status = gBS->LocateProtocol(&gQcomDisplayPwrCtrlProtocolGuid,
                               NULL,
                               (VOID **)&DispCtrl);
  if (status != EFI_SUCCESS)
  {
	  AsciiPrint("DispPwrCtrl protocol load failed\n");
      return 0;
  }
  return 1;
}

/*==============================================================================

  FUNCTION      ddr_uefi_display_off

  DESCRIPTION   Function to change primary display state as OFF

==============================================================================*/

UINT32 ddr_uefi_display_off()
{
  EFI_STATUS status = EFI_SUCCESS;

  if (DispCtrl ==  NULL) {
	  return 0;
  }

  status = DispCtrl->DisplayPanelPowerControl(EFI_DISPLAY_TYPE_PRIMARY, EFI_DISPLAY_POWER_STATE_OFF);
  if (status != EFI_SUCCESS)
  {
	  AsciiPrint("Could not change primary display state\n");
      return 0;
  }
  return 1;
}

/*==============================================================================

  FUNCTION      ddr_uefi_display_on

  DESCRIPTION   Function to change primary display state as ON

==============================================================================*/

UINT32 ddr_uefi_display_on()
{
  EFI_STATUS status = EFI_SUCCESS;

  if (DispCtrl ==  NULL) {
	  return 0;
  }

  status = DispCtrl->DisplayPanelPowerControl(EFI_DISPLAY_TYPE_PRIMARY, EFI_DISPLAY_POWER_STATE_ON);
  if (status != EFI_SUCCESS)
  {
	  AsciiPrint("Could not change primary display state\n");
      return 0;
  }
  return 1;
}

/*==============================================================================

  FUNCTION      ddr_setlowbandwidth

  DESCRIPTION   Function to set low bandwidth for DDR

==============================================================================*/

EFI_STATUS ddr_setlowbandwidth(void)
{
  EFI_STATUS Status;
  EFI_QCOMENV_PROTOCOL  *EnvProtocol = NULL;    
    
  Status = gBS->LocateProtocol(&gEfiQcomEnvProtocolGuid, NULL, (void**)&EnvProtocol);
  if(Status == EFI_SUCCESS)
  {
    if (EFI_SUCCESS != (Status = EnvProtocol->SetDdrThroughputLow(EnvProtocol)))
    {
		AsciiPrint("SetDdrThroughLow Failed\n");
    }
    else
    {
		AsciiPrint("SetDdrThroughLow Success\n");
    }
  } 
  return Status; 
}
