/** @file ChargerLibDisplay.c

  Helper functions for displaying images on screen

  Copyright (c) 2016 - 2019, Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when         who     what, where, why
 --------     ---     -----------------------------------------------------------
 01/28/19     kpa     Support loading bmp src's from guided fv
 07/12/17     cs      added debug board icons
 03/29/17     cs      added thermal error symbol
 03/09/17     ll      Updated include path
 07/06/16     sm      Enabled all the APIs
                      Replaced RenderBGRTImage() with DrawBmpFile()
 05/26/16     va      Update for Profile load and struct mis match 
 05/24/16     sm      Removed unused global variables.
 01/27/16     sm      Initial revision

=============================================================================*/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/QcomLib.h>
#include <Library/BMPLib.h>
#include "Library/SerialPortShLib.h"
#include <Library/QcomTargetLib.h>
#include <Library/FuseControlLib.h>
#include <api/pmic/pm/pm_uefi.h>
#include <api/pmic/charger/ChargerLibCommon.h>
#include <Library/MountFvUtils.h>

/**
  Protocol Dependencies
*/
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFIPmicFg.h>
#include <Protocol/EFIPmicVersion.h>
#include <Protocol/EFIPmicPwrOn.h>
#include <Protocol/EFIVariableServices.h>
#include <Protocol/EFITsens.h>
#include <Protocol/EFIPmicRTC.h>

#include <Protocol/EFIDisplayPwr.h>
#include <Protocol/EFIDisplayPwrCtrl.h>
#include <Protocol/EFIClock.h>

#include <Protocol/GraphicsOutput.h>

#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ParserLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/FileSystemInfo.h>
#include <Guid/FileInfo.h>

#include "DDIPlatformInfo.h"
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/EFIDisplayUtils.h>
#include <Protocol/EFIPIL.h>

/*
This file will implement the functions to setup timer and implment the timer callback, 
also it implement functions to register for key and LPM events and callback functions for key events and LPM events. 
It also implement functions to turn display on/off, and render charging/battery images.
*/

#define CHARGER_BATTERY_SYMBOL_NOBATTERY            "battery_symbol_NoBattery.bmp"
#define CHARGER_BATTERY_SYMBOL_NOCHARGER            "battery_symbol_Nocharger.bmp"
#define CHARGER_BATTERY_SYMBOL_ABOVE_THRESHOLD      "battery_symbol_Soc10.bmp"
#define CHARGER_BATTERY_SYMBOL_LOWBATTERYCHARGING   "battery_symbol_LowBatteryCharging.bmp"
#define CHARGER_BATTERY_SYMBOL_LOWBATTERY           "battery_symbol_LowBattery.bmp"
#define CHARGER_TSENS_THERMAL_SYMBOL                "tsens_thermal_symbol.bmp"
#define CHARGER_TSENS_CRITICAL_SYMBOL               "tsens_thermal_err_symbol.bmp"
#define CHARGER_BATTERY_SYMBOL_DEBUG_BOARD_BOOT     "battery_symbol_DebugBoot.bmp"
#define CHARGER_BATTERY_SYMBOL_DEBUG_BOARD_STAY     "battery_symbol_DebugStay.bmp"

extern EFI_GUID gEfiImageFvNameGuid;
STATIC BOOLEAN ImageFvLoadedFlag = FALSE;

#define ANIM_TIMER_DURATION           5000000 /*500 milli sec*/
#define SEVEN_SEC_WAIT                70000000 /*tick is 100ns; 100 * 70000000 = 7sec*/
#define SEVEN_SEC                     7
#define ENTER_LPM                     TRUE
#define EXIT_LPM                      FALSE

typedef enum _CHGAPP_DISP_ANIM_IMG{
  CHGAPP_DISP_ANIM_IMG_LOW_BATTERY,
  CHGAPP_DISP_ANIM_IMG_LOW_BATT_CHARGING,
  CHGAPP_DISP_ANIM_IMG_NUM_MAX = 0x7fffffff
}CHGAPP_DISP_ANIM_IMG_NUM;

/******************************************************************************
            LOCAL FUNCTIONS
******************************************************************************/
extern EFI_STATUS
LoadFromFV(IN   CHAR8 *Name,
           OUT  UINT8 **FileBuffer,
           OUT  UINTN *FileSize);
           
VOID EFIAPI DisplayOffTimer(IN EFI_EVENT Event, IN VOID *Context);
VOID EFIAPI ChargerLibEvent_AnimImgTimer(IN EFI_EVENT Event, IN VOID *Context);
STATIC EFI_STATUS ChargerLibEvent_RenderImage(char   *str);
STATIC EFI_STATUS ChargerLibEvent_ClearScreen( void );

/**
  Duplicates ASCII string

  @param  Source  Null Terminated string of characters
  @param  Length  source String length
  @return  Duplicated String  if successful otherwise return NULL

  note: Caller responsibility to Free returned String
**/
static CHAR8 * ChargerLibEvent_AsciiStrNDup(
  CONST CHAR8               *Source,
  UINTN                     Length
)
{
  CHAR8 *Dest = NULL;
  if(NULL == Source || 0 == Length)
    return NULL;

  Dest = AllocatePool (Length + 1);
  if (NULL == Dest)
  {
    return NULL;
  }

  AsciiStrnCpy(Dest, Source, Length + 1);

  return Dest;
}

/* API to clear the display screen */
EFI_STATUS ChargerLibEvent_ClearScreen( void )
{
  EFI_STATUS                              Status = EFI_SUCCESS;
  STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL    *pGfxProtocol = NULL;
  EFI_GUID                                sGfxOutputGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *pModeInfo = NULL;
  UINTN                                   nModeInfoSize = 0;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL           bgPixel = {0, 0, 0, 0};
  UINT32                                  mode = 0;

  if(!pGfxProtocol)
  {
    Status = gBS->LocateProtocol ( &sGfxOutputGUID, 
                                   NULL, 
                                   (VOID **)&pGfxProtocol);
  }

  if (EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_ERROR, "ChargerLib:: %a ChgAppClearScreen LocateProtocol(pGfxProtocol) failed. Status = %r \r\n", __FUNCTION__, Status));
    return Status;
  }

  if (EFI_SUCCESS != (Status = pGfxProtocol->SetMode(pGfxProtocol, mode)))
  {
    DEBUG((EFI_D_ERROR, "ChargerLib:: %a ChgAppClearScreen SetMode() failed. Status = %r \r\n",__FUNCTION__, Status));
    return Status;
  }

  if (EFI_SUCCESS != (Status = pGfxProtocol->QueryMode(pGfxProtocol, 0, &nModeInfoSize, &pModeInfo)))
  {
    DEBUG((EFI_D_ERROR, "ChargerLib:: %a ChgAppClearScreen QueryMode() failed.Status = %r  \r\n",__FUNCTION__, Status));
    return Status;
  }

  /* Fill background as black */
  bgPixel.Blue     = 0x00;
  bgPixel.Green    = 0x00;
  bgPixel.Red      = 0x00;
  bgPixel.Reserved = 0x00;

  Status = pGfxProtocol->Blt( pGfxProtocol, 
                              (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)&bgPixel,
                              EfiBltVideoFill,
                              0, 0,
                              0, 0, 
                              pModeInfo->HorizontalResolution,
                              pModeInfo->VerticalResolution,
                              0);

  if (EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_ERROR, "ChargerLib:: %a ChgAppClearScreen Blt(EfiBltVideoFill)  failed.Status = %r \r\n",__FUNCTION__, Status));
    return Status;
  }

  return Status;
  
}

EFI_STATUS ChargerLibEvent_RenderImage(char   *str)
{
  EFI_STATUS                              Status                = EFI_SUCCESS;
  UINTN                                   FileSizeFv            = 0;
  UINT8                                  *FileBuffer            = NULL;
  STATIC EFI_QCOM_DISPLAY_UTILS_PROTOCOL *pDisplayUtilsProtocol = NULL;
  EFI_DISPLAY_UTILS_RENDERLOGO_CONFIG     RenderLogoCfg         = {0};
  
  if(!str)
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = LoadFromFV(str, &FileBuffer, &FileSizeFv);
  if (EFI_SUCCESS == Status)
  {
    if (NULL == pDisplayUtilsProtocol)
    {
      Status = gBS->LocateProtocol(&gQcomDisplayUtilsProtocolGuid, NULL, (void**)&pDisplayUtilsProtocol);
    }

    if ((EFI_SUCCESS == Status) && (NULL != pDisplayUtilsProtocol))
    {
      RenderLogoCfg.uFlags        |= DISPLAY_UTILS_RENDERLOGO_RENDER_BGRT_IMAGE;
      RenderLogoCfg.BGRTImage      = FileBuffer;
      RenderLogoCfg.BGRTImageSize  = (UINT32)FileSizeFv;

      Status = pDisplayUtilsProtocol->DisplayUtilsRenderLogo(&RenderLogoCfg);
      if(EFI_SUCCESS != Status)
      {
        DEBUG((EFI_D_ERROR, "ChargerLib:: %a Error from RenderBGRTImage Status = %r\r\n", __FUNCTION__, Status));
      }
    }
    else
    {
      DEBUG(( EFI_D_WARN, "ChargerLib:: %a Failed to Locate Display Utils Protocol Status = %r\r\n", __FUNCTION__, Status));
    }
  }
  else
  {
     DEBUG((EFI_D_ERROR, "ChargerLib:: %a Error from LoadFromFV Status = %r\r\n", __FUNCTION__, Status));
  }

  if (FileBuffer){
    FreePool(FileBuffer);
    FileBuffer = NULL;
  }

  return Status;
}

EFI_STATUS ChargerLibEvent_DispBattSymbol(CHARGERLIB_EVENT_DISP_IMAGE_TYPE DispImage, BOOLEAN ClearScreen)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PIL_PROTOCOL  *PILProtocol = NULL;
  char   *str = NULL;  

  switch (DispImage)
  {
  case CHARGERLIB_EVENT_DISP_IMAGE_ABOVE_THRESHOLD:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_BATTERY_SYMBOL_ABOVE_THRESHOLD, AsciiStrLen(CHARGER_BATTERY_SYMBOL_ABOVE_THRESHOLD));
    break;
  case CHARGERLIB_EVENT_DISP_IMAGE_NOBATTERY:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_BATTERY_SYMBOL_NOBATTERY, AsciiStrLen(CHARGER_BATTERY_SYMBOL_NOBATTERY));
    break;
  case CHARGERLIB_EVENT_DISP_IMAGE_NOCHARGER:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_BATTERY_SYMBOL_NOCHARGER, AsciiStrLen(CHARGER_BATTERY_SYMBOL_NOCHARGER));
    break;
  case CHARGERLIB_EVENT_DISP_IMAGE_LOWBATTERYCHARGING:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_BATTERY_SYMBOL_LOWBATTERYCHARGING, AsciiStrLen(CHARGER_BATTERY_SYMBOL_LOWBATTERYCHARGING));
    break;
  case CHARGERLIB_EVENT_DISP_IMAGE_LOWBATTERY:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_BATTERY_SYMBOL_LOWBATTERY, AsciiStrLen(CHARGER_BATTERY_SYMBOL_LOWBATTERY));
    break;
  case CHARGERLIB_EVENT_DISP_IMAGE_TSENS_THERMAL_SYMBOL:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_TSENS_THERMAL_SYMBOL, AsciiStrLen(CHARGER_TSENS_THERMAL_SYMBOL));
    break;
  case CHARGERLIB_EVENT_DISP_IMAGE_TSENS_CRITICAL_SYMBOL:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_TSENS_CRITICAL_SYMBOL, AsciiStrLen(CHARGER_TSENS_CRITICAL_SYMBOL));
    break;
  case CHARGERLIB_EVENT_DISP_IMAGE_DEBUG_BOOT_SYMBOL:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_BATTERY_SYMBOL_DEBUG_BOARD_BOOT, AsciiStrLen(CHARGER_BATTERY_SYMBOL_DEBUG_BOARD_BOOT));
    break;
  case CHARGERLIB_EVENT_DISP_IMAGE_DEBUG_LOW_SYMBOL:
    str = ChargerLibEvent_AsciiStrNDup(CHARGER_BATTERY_SYMBOL_DEBUG_BOARD_STAY, AsciiStrLen(CHARGER_BATTERY_SYMBOL_DEBUG_BOARD_STAY));
    break;
  default:
    break;
  }

  /*Clear Screen when requested*/
  if(TRUE == ClearScreen)
  {
    if (EFI_SUCCESS != (Status = ChargerLibEvent_ClearScreen()))
    {
      DEBUG((EFI_D_ERROR, "ChargerLib:: %a ChgAppClearScreen() returned error = %r\n\r", __FUNCTION__,Status));
    }
    /* CHARGER_DEBUG((EFI_D_ERROR, "ChargerLib: ChargerLibEvent_DispBattSymbol ChgAppClearScreen Cleared: %d\n\r", Status)); */
  }

  if(ImageFvLoadedFlag == FALSE)
  {
    Status = gBS->LocateProtocol (&gEfiPilProtocolGuid,NULL, (VOID **) &PILProtocol);
    if ((EFI_SUCCESS != Status) || (NULL == PILProtocol))
    {
      CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a %r \r\n", __FUNCTION__, Status));
      return Status;
    }

    Status = PILProtocol->ProcessPilImage(L"ImageFv");
    if ((EFI_SUCCESS != Status))
    {
      CHARGER_DEBUG(( EFI_D_ERROR, "ChargerLib:: %a %r \r\n", __FUNCTION__, Status));
      return Status;
    }
    ImageFvLoadedFlag = TRUE;
  }

  /* Draw BMP image with default options, the screen will be cleared and the image
     will be drawn at the center of the screen*/
  Status = DrawBmpFile(str, NULL, 0, &gEfiImageFvNameGuid);

  if(NULL != str )
  {
    FreePool(str);
    str = NULL;
  }

  return Status;
}

