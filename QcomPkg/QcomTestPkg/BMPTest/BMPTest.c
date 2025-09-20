/** @file BMPTest.c

  Run through list of BMPs

  Copyright (c) 2015,2019 Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 01/28/19   kpa     Enable DrawBmpFile to load bmp's using fv guid
 09/09/15   ai      Initial version


=============================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TestInterface.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BMPLib.h>
#include <Protocol/EFIPIL.h>
#include <Library/GuidedFvLib.h>

#ifndef ARRAY_SIZE
#define  ARRAY_SIZE(a) (sizeof((a))/sizeof((a)[0]))
#endif

extern EFI_GUID gEfiImageFvNameGuid;
extern EFI_GUID gEfiPilProtocolGuid;

CHAR8* FilePaths[] = {
   "battery_symbol_NoBattery.bmp",
   "battery_symbol_Nocharger.bmp",
   "battery_symbol_LowBattery.bmp",
   "battery_symbol_Soc10.bmp",
   "battery_symbol_LowBatteryCharging.bmp",
   "battery_symbol_DebugBoot.bmp",
   "battery_symbol_DebugStay.bmp",
   "tsens_thermal_symbol.bmp",
   "tsens_thermal_err_symbol.bmp"
};

/**
 *  Make non-const duplicate of strings.
 *  Caller must FreePool the new string.
 */
static CHAR8 * AsciiStrnDup(
  CONST CHAR8               *Source,
  UINTN                     Length
)
{
  CHAR8 *Dest = NULL;

  if(Source == NULL || Length == 0) {
    return NULL;
  }

  Dest = AllocatePool (Length + 1);
  if (Dest == NULL) {
    return NULL;
  }

  AsciiStrnCpy(Dest, Source, Length + 1);

  return Dest;
}

/**
 *
 */
EFI_STATUS
BmpTestNoOptions()
{
  UINTN           Index        = 0;
  UINTN           NumFiles     = ARRAY_SIZE(FilePaths);
  EFI_STATUS      Status;
  CHAR8           *FileName    = NULL;

  TEST_START("BmpTest-NoOptions");

  Index = 0;
  do {
    /* Make non-const copy of FileName string */
    FileName = AsciiStrnDup(
      FilePaths[Index],
      AsciiStrLen(FilePaths[Index])
      );
    if (!FileName) {
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    /* Draw bitmap */
    Status = DrawBmpFile(FileName, NULL, 0, &gEfiImageFvNameGuid);
    if(EFI_ERROR (Status)) {
      break;
    }

    /* Stall for 1.5s */
    gBS->Stall(1500000);

    /* Free resources */
    FreePool(FileName);
    FileName   = NULL;
  } while (Index++ < NumFiles-1);


  if (FileName != NULL) {
    FreePool(FileName);
  }

  TestStatus("BmpTest-NoOptions", Status);
  TEST_STOP("BmpTest-NoOptions");
  return Status;
}

/**
 *
 */
EFI_STATUS
BmpTestLocation()
{
  EFI_STATUS                    Status;
  CHAR8                         *FileName  = NULL;
  UINTN                         Index      = 0;
  BMPLIB_OPTION_LOCATION_COORD  Location[] = {
    BmplibOptionLocationTopLeft,
    BmplibOptionLocationTopCenter,
    BmplibOptionLocationTopRight,
    BmplibOptionLocationCenterLeft,
    BmplibOptionLocationCenter,
    BmplibOptionLocationCenterRight,
    BmplibOptionLocationBottomLeft,
    BmplibOptionLocationBottomCenter,
    BmplibOptionLocationBottomRight,
    BmplibOptionLocationCustom,
  };
  UINTN                         NumLocations = ARRAY_SIZE(Location);

  BMPLIB_OPTION_LOCATION_PARAMS Params = {BmplibOptionLocationCustom, 200, 200};
  BMPLIB_OPTION                 Options[1];

  Options[0].Type = BmplibOptionTypeLocation;
  Options[0].Params = &Params;

  TEST_START("BmpTest-Location");

  FileName = AsciiStrnDup(
                FilePaths[0],
                AsciiStrLen(FilePaths[0])
                );
  if (!FileName) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ExitHandler;
  }

  Index = 0;
  do {
    Params.Location = Location[Index];

    /* Draw bitmap */
    Status = DrawBmpFile(FileName, Options, ARRAY_SIZE(Options), &gEfiImageFvNameGuid);
    if(EFI_ERROR (Status)) {
      break;
    }

    /* Stall for 1.5s */
    gBS->Stall(1500000);
  } while (Index++ < NumLocations-1);

/* Legal for GCC; Illegal for RVCT */
#if 0
  /* Test invalid Location */
  Params.Location = 999;

  /* Draw bitmap */
  Status = DrawBmpFile(FileName, Options, ARRAY_SIZE(Options), &gEfiImageFvNameGuid);
  if(!EFI_ERROR (Status)) {
    Status = EFI_ABORTED;
  }
  else {
    Status = EFI_SUCCESS;
  }
#endif

ExitHandler:
  if (FileName != NULL) {
    FreePool(FileName);
  }

  TestStatus("BmpTest-Location", Status);
  TEST_STOP("BmpTest-Location");
  return Status;
}

/**
 *
 */
EFI_STATUS
BmpTestClearScreen()
{
  EFI_STATUS                    Status;
  CHAR8                         *FileName  = NULL;
  UINTN                         Index      = 0;
  BMPLIB_OPTION_LOCATION_COORD  Location[] = {
    BmplibOptionLocationTopLeft,
    BmplibOptionLocationTopRight,
    BmplibOptionLocationCenter,
    BmplibOptionLocationBottomLeft,
    BmplibOptionLocationBottomRight,
    BmplibOptionLocationCustom,
  };
  UINTN                         NumLocations = ARRAY_SIZE(Location);

  BMPLIB_OPTION_LOCATION_PARAMS     LocationParams = {BmplibOptionLocationCustom, 200, 200};
  BMPLIB_OPTION_CLEARSCREEN_PARAMS  ScreenParams   = {FALSE};

  BMPLIB_OPTION                 Options[2];

  Options[0].Type = BmplibOptionTypeLocation;
  Options[0].Params = &LocationParams;

  Options[1].Type = BmplibOptionTypeClearScreen;
  Options[1].Params = &ScreenParams;

  TEST_START("BmpTest-ClearScreen");

  FileName = AsciiStrnDup(
                FilePaths[0],
                AsciiStrLen(FilePaths[0])
                );
  if (!FileName) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ExitHandler;
  }

  Index = 0;
  do {
    LocationParams.Location = Location[Index];

    /* Draw bitmap */
    Status = DrawBmpFile(FileName, Options, ARRAY_SIZE(Options), &gEfiImageFvNameGuid);
    if(EFI_ERROR (Status)) {
      break;
    }

    /* Stall for 1.5s */
    gBS->Stall(1500000);
  } while (Index++ < NumLocations-1);

ExitHandler:
  if (FileName != NULL) {
    FreePool(FileName);
  }

  TestStatus("BmpTest-ClearScreen", Status);
  TEST_STOP("BmpTest-ClearScreen");
  return Status;
}


/**
 *
 */
EFI_STATUS
MountImageFv( VOID )
{
  EFI_STATUS        Status;
  EFI_PIL_PROTOCOL  *PILProtocol = NULL;  
  
  //Check if Image fv is already mounted
  if (GuidedFvIsMounted(&gEfiImageFvNameGuid))
    return EFI_SUCCESS;
  
  Status = gBS->LocateProtocol (&gEfiPilProtocolGuid,NULL, (VOID **) &PILProtocol);
  if ((EFI_SUCCESS != Status) || (NULL == PILProtocol))
  {
    DEBUG(( EFI_D_ERROR, "BmpTest:: %a %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = PILProtocol->ProcessPilImage(L"ImageFv");
  if ((EFI_SUCCESS != Status))
  {
    DEBUG(( EFI_D_ERROR, "BmpTest:: %a %r \r\n", __FUNCTION__, Status));
    return Status;
  }  
  
  return EFI_SUCCESS;
}

/**
 *
 */
typedef EFI_STATUS (* BMPTESTCASE)(VOID);
BMPTESTCASE BmpTests[] = {
  BmpTestNoOptions,
  BmpTestLocation,
  BmpTestClearScreen
};

/**
 *
 */
EFI_STATUS
EFIAPI
BMPTestMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  UINTN      NumTests = ARRAY_SIZE(BmpTests);
  UINTN      Index    = 0;

  Status = MountImageFv();
  if (Status != EFI_SUCCESS)
    return Status;
  
  do {
    Status = BmpTests[Index]();
    if(EFI_ERROR (Status)) {
      break;
    }
  } while (Index++ < NumTests-1);

  return Status;
}


