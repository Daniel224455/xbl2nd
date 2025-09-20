/*=============================================================================
 
  File: DisplayDxe.c
 
  Display driver for UEFI
  
 
    Copyright (c) 2011-2021 Qualcomm Technologies, Inc.All Rights Reserved
    Portions Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
	This program and the accompanying materials
	are licensed and made available under the terms and conditions of the BSD License
	which accompanies this distribution.  The full text of the license may be found at
	http://opensource.org/licenses/bsd-license.php
	
	THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
	WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
=============================================================================*/
#include <PiDxe.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/EFIChargerEx.h>
#include <Protocol/EFIPIL.h>
#include <Library/BaseLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/QcomLib.h>
#include "Protocol/EFIDisplayPwrCtrl.h"
#include "Protocol/EFIDisplayPwr.h"
#include "Protocol/EFIDisplayUtils.h"
#include "DisplayDxe.h"
#include "DisplayMultiCore.h"
#include "MDPLib.h"
#include "MDPSystem.h"
#include "DisplayUtils.h"

/**********************************************************************************************************************
 **   Macros/Defines
 **/
#define DISPLAY_TABLE_SIZE(_table_)        (sizeof(_table_)/sizeof((_table_)[0]))


/**********************************************************************************************************************
 **   Static/Globals
 **/

EFI_EVENT EfiExitBootServicesEvent      = (EFI_EVENT)NULL;

/* Block IO Refresh Event */
EFI_EVENT gBlockIoRefreshEvt            = (EFI_EVENT) NULL;

/* Pil Services Event */
EFI_EVENT gPilServicesEvt               = (EFI_EVENT)NULL;

/* Pil services callback registration */
VOID     *gPilServicesRegistration      = NULL;

/* Pil GUID */
extern EFI_GUID gEfiPilProtocolGuid;

/* Variable Services Event */
EFI_EVENT gVariableServicesEvt          = (EFI_EVENT)NULL;

/* Variable services callback registration */
VOID     *gVariableServicesRegistration = NULL;

/* Static information about the current display mode */
DisplayDxe_ModeInfo   gDxeModeInfo;

/* Initialize the gSupportedDisplayList */
static bool32* DisplaySupportTableInit(void);

/* Function table pointer to the supported protocol functions */
static EFI_GRAPHICS_OUTPUT_PROTOCOL gDisplayDxeOutputProtocol = {
    &DisplayDxe_QueryMode,
    &DisplayDxe_SetMode,
    &DisplayDxe_Blt,
    &gDxeModeInfo.sProtocolInfo
};

/* DisplayDXE Device path */
EFI_DISPLAY_DEVICE_PATH DisplayDevicePath = 
{
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID
  },
  { END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, {sizeof (EFI_DEVICE_PATH_PROTOCOL), 0}}
};

/* Supported display list - TRUE: Enabled, FALSE: Disabled. */
bool32 gSupportedDisplayList[MDP_DISPLAY_MAX] =
{
  TRUE,   // MDP_DISPLAY_PRIMARY
#ifdef DUAL_DSI_CONFIG
  TRUE,   // MDP_DISPLAY_SECONDARY
#else
  FALSE,  // MDP_DISPLAY_SECONDARY
#endif
  FALSE,  // MDP_DISPLAY_EXTERNAL
  FALSE,  // MDP_DISPLAY_EXTERNAL2
  FALSE,  // MDP_DISPLAY_EXTERNAL3
  FALSE,  // MDP_DISPLAY_EXTERNAL4
  FALSE,  // MDP_DISPLAY_EXTERNAL5
  FALSE   // MDP_DISPLAY_EXTERNAL6
};

/* Frame buffer Resolution Limit Table, table is landscape but check is done for both orientations. */
const MDP_ResolutionType gFBResLimit[] = 
{
    {4096, 2160},   // Index 0: 4k    ~35.3MB @ 32bpp
    {3840, 2160},   // Index 1: UHD   ~33.1MB  @ 32bpp
    {2560, 1440},   // Index 2: WQHD  ~14.7MB  @ 32bpp
    {1920, 1280},   // Index 3: WUXGA  ~9.8MB @ 32bpp
    {1920, 1080},   // Index 4: FHD    ~8.2MB @ 32bpp
    {1280, 720},    // Index 5: HD     ~3.6MB @ 32bpp
};

/* Virtual mode table, list of additonal virtual modes exposed by the DisplayDxe */
const MDP_ResolutionType gVirtualResTable[] =
{
    {640,  480},   
    {800,  600},   
    {1280, 720},  
    {1024, 768},  
    {1280, 768},  
    {1366, 768}
};

/**********************************************************************************************************************
 **   Local function prototypes
 **/
    
/* Display power control protocol definition */
extern EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL  gDisplayPwrCtrlProtocolImplementation;

/* Display power protocol definition */
extern EFI_DISPLAY_POWER_PROTOCOL  gDisplayPwrProtocolImplementation;

/* Display Utils protocol definition */
extern EFI_QCOM_DISPLAY_UTILS_PROTOCOL  gQcomDisplayUtilsProtocolImplementation;

/* Block IO Refresh GUID */
extern EFI_GUID gBlockIoRefreshGuid;

/* Get charger power status */
static EFI_STATUS GetChargerPowerStatus(void);

/* Display Blit function */
static void DisplayDxeBltInternal (
  UINT8   *pSrc,
  UINT8   *pDst,
  UINTN    uSrcX,
  UINTN    uSrcY,
  UINTN    uSrcWidth,
  UINTN    uSrcHeight,
  UINTN    uSrcStride,
  UINTN    uDstX,
  UINTN    uDstY,
  UINTN    uDstStride,
  UINTN    uBytesPerPixel
  );

/**
  ClampResolution
  
  Clamps a resolution to a specific limit.  Handles cases where input & clamp are in different orientation (porait vs landscape)
    
  @param  uSourceWidth          Source width
  @param  uSourceHeight         Source height
  @param  uClampWidth           Clamp width
  @param  uClampHeight          Clamp height
 
@retval MDP_ResolutionType      Clamped resolution

**/
static MDP_ResolutionType ClampResolution(UINT32 uSourceWidth, UINT32 uSourceHeight, UINT32 uClampWidth, UINT32 uClampHeight);

/* Register events callbacks */
static VOID DisplayDxeRegisterEvents(void);

/* Unregister events */
static VOID DisplayDxeUnRegisterEvents(void);

//-------------------------------------------------------------------------------------------------
//  DisplayDxeExitBootServicesEvent
//
//  @brief
//      Callback function when UEFI Exit, free resources.
//
//  @params
//      [IN] Event
//          Event handle.
//      [IN] Context
//          Callback parameter context.
//
//  @return
//      None
//-------------------------------------------------------------------------------------------------
//
VOID EFIAPI DisplayDxeExitBootServicesEvent( IN EFI_EVENT  Event,  IN VOID *Context )
{
  // Unregister events
  DisplayDxeUnRegisterEvents();

  //UEFI is shutting down - Handle it in MDP
  MDPExitBoot(gDxeModeInfo.uExitFlags);
}


//-------------------------------------------------------------------------------------------------
//  BlockIoCallback
//
//  @brief
//      Callback function when GPT partition table is updated to invalidate all Block Io handles.
//
//  @params
//      [IN] Event
//          Event handle.
//      [IN] Context
//          Callback parameter context.
//
//  @return
//      None
//-------------------------------------------------------------------------------------------------
//
VOID EFIAPI BlockIoCallback(IN EFI_EVENT  Event, IN VOID *Context)
{
  /* GPT has been updated and the splash partition is invalid. So skip writing to it */
  gDxeModeInfo.uExitFlags |= MDP_EXIT_FLAG_INVALIDATE_GPT;
}


//-------------------------------------------------------------------------------------------------
//  UIActiveEventCallBack
//
//  @brief
//      Callback function when UI_Active event is signaled, turn on display
//
//  @params
//      [IN] Event
//          Event handle.
//      [IN] Context
//          Callback parameter context.
//
//  @return
//      None.
//-------------------------------------------------------------------------------------------------
//
VOID EFIAPI UIActiveEventCallBack( IN EFI_EVENT  Event, IN VOID *Context)
{
  EFI_DISPLAY_TYPE               eDisplayType = EFI_DISPLAY_TYPE_ALL;
  EFI_DISPLAY_POWER_CTRL_STATE   ePowerState  = EFI_DISPLAY_POWER_STATE_ON;

  /* Turn on the display */
  if (EFI_SUCCESS !=  EFI_DisplayPanelPowerControl(eDisplayType, ePowerState))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: EFI_DisplayPanelPowerControl() failed to turn on display!\n"));
  }

}

//-------------------------------------------------------------------------------------------------
//  UIIdleEventCallBack
//
//  @brief
//      Callback function when UI_Idle event is signaled, turn off display
//
//  @params
//      [IN] Event
//          Event handle.
//      [IN] Context
//          Callback parameter context.
//
//  @return
//      None.
//-------------------------------------------------------------------------------------------------
//
VOID EFIAPI UIIdleEventCallBack( IN EFI_EVENT  Event, IN VOID *Context)
{
  EFI_DISPLAY_TYPE               eDisplayType = EFI_DISPLAY_TYPE_ALL;
  EFI_DISPLAY_POWER_CTRL_STATE   ePowerState  = EFI_DISPLAY_POWER_STATE_OFF;

  /* Turn off the display */
  if (EFI_SUCCESS !=  EFI_DisplayPanelPowerControl(eDisplayType, ePowerState))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: EFI_DisplayPanelPowerControl() failed to turn off display!\n"));
  }

}

//-------------------------------------------------------------------------------------------------
//  PilServicesCallBack
//
//  @brief
//      Callback function to mount imagefv
//
//  @params
//      [IN] Event
//          Event handle.
//      [IN] Context
//          Callback parameter context.
//
//  @return
//      None.
//-------------------------------------------------------------------------------------------------
//
VOID EFIAPI PilServicesCallBack (IN EFI_EVENT  Event, IN VOID *Context)
{
  EFI_STATUS  eStatus      = EFI_SUCCESS;
  EFI_GUID    gFvNameGuid  = gEfiImageFvNameGuid;
  CHAR16     *pFvName      = L"ImageFv";

  if (EFI_SUCCESS != (eStatus = MDP_MountFv(gFvNameGuid, pFvName)))
  {
    DEBUG ((EFI_D_WARN, "DisplayDxe: Mount imagefv failed with status(%d)!\r\n", eStatus));
  }
}


//-------------------------------------------------------------------------------------------------
//  VariableServicesCallBack
//
//  @brief
//      Callback function when variable services is available
//
//  @params
//      [IN] Event
//          Event handle.
//      [IN] Context
//          Callback parameter context.
//
//  @return
//      None.
//-------------------------------------------------------------------------------------------------
//
VOID EFIAPI VariableServicesCallBack( IN EFI_EVENT  Event, IN VOID *Context)
{
  // Flush cached variables.
  MDP_FlushCachedVariables();
}


//-------------------------------------------------------------------------------------------------
//  DisplayDxeRegisterEvents
//
//  @brief
//      Register for event call backs
//
//  @params
//      none
//
//  @return
//      None.
//-------------------------------------------------------------------------------------------------
//
static VOID DisplayDxeRegisterEvents( void )
{
  EFI_GUID UIActiveEventGuid  = EFI_UI_ACTIVE_EVENT_GUID;
  EFI_GUID UIIdleEventGuid    = EFI_UI_IDLE_EVENT_GUID;
  UINT32   uImageFvSetting    = 0;

  /* Register for an ExitBootServicesEvent */
  if (EFI_SUCCESS != gBS->CreateEvent(EVT_SIGNAL_EXIT_BOOT_SERVICES, TPL_CALLBACK, DisplayDxeExitBootServicesEvent, NULL, &EfiExitBootServicesEvent))
  {
    DEBUG((EFI_D_INFO, "DisplayDxe: Failed to create EXIT_BOOT_SERVICES event!\n"));
  }

  /* Register for BlockIoRefreshEvent */
  if (EFI_SUCCESS != gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, BlockIoCallback, NULL, &gBlockIoRefreshGuid, &gBlockIoRefreshEvt))
  {
    DEBUG((EFI_D_INFO, "DisplayDxe: Failed to create BlockIoRefresh event!\n"));
  }

  /* Create and Register for UI_Active Event*/
  if (EFI_SUCCESS != gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, UIActiveEventCallBack, NULL, &UIActiveEventGuid, &gDxeModeInfo.UIActiveEvent))
  {
    DEBUG((EFI_D_INFO, "DisplayDxe: Failed to create UI_Active event!\n"));
  }
  
  /* Create and Register for UI_Idle Event*/
  if (EFI_SUCCESS != gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, UIIdleEventCallBack, NULL, &UIIdleEventGuid, &gDxeModeInfo.UIIdleEvent))
  {
    DEBUG((EFI_D_INFO, "DisplayDxe: Failed to create UI_Idle event!\n"));
  }

  if ((MDP_STATUS_OK == MDP_GetConfigValue("EnableDisplayImageFv", &uImageFvSetting)) &&
      (1             == uImageFvSetting))
  {
    /* Create and Register for pil services event */
    if (EFI_SUCCESS == gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PilServicesCallBack, NULL, &gPilServicesEvt))
    {
      if (EFI_SUCCESS != gBS->RegisterProtocolNotify(&gEfiPilProtocolGuid, gPilServicesEvt, (VOID *)&gPilServicesRegistration))
      {
        DEBUG((EFI_D_INFO, "DisplayDxe: Failed to register for pil services event!\n"));
      }
    }
  }

  /* Create and Register for a Variable services event */
  if (EFI_SUCCESS == gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, VariableServicesCallBack, NULL, &gVariableServicesEvt))
  {
    // Cache request if this is not available.
    if (EFI_SUCCESS != gBS->RegisterProtocolNotify (&gEfiVariableServicesProtocolGuid, gVariableServicesEvt, (VOID *)&gVariableServicesRegistration))
    {
      DEBUG((EFI_D_INFO, "DisplayDxe: Failed to register for variable services event!\n"));
    }
  }
  else
  {
    DEBUG((EFI_D_INFO, "DisplayDxe: Failed to create variable services event!\n"));
  }

  if (EFI_SUCCESS != DisplayPwr_InitLPMSupport(TRUE))
  {
    DEBUG((EFI_D_WARN, "DisplayDxe: Failed to register LPM CB!\n"));
  }
}


//-------------------------------------------------------------------------------------------------
//  DisplayDxeUnRegisterEvents
//
//  @brief
//      Unregister events
//
//  @params
//      none
//
//  @return
//      None.
//-------------------------------------------------------------------------------------------------
//
static VOID DisplayDxeUnRegisterEvents( void )
{
  if ((NULL != gDxeModeInfo.UIActiveEvent) &&
      (EFI_SUCCESS != gBS->CloseEvent(gDxeModeInfo.UIActiveEvent)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to Close event UI_Active\n"));
  }

  if ((NULL != gDxeModeInfo.UIIdleEvent) &&
      (EFI_SUCCESS != gBS->CloseEvent(gDxeModeInfo.UIIdleEvent)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to Close event UI_Idle\n"));
  }

  if ((NULL != gBlockIoRefreshEvt) &&
      (EFI_SUCCESS != gBS->CloseEvent(gBlockIoRefreshEvt)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to Close event UI_Active\n"));
  }

  if ((NULL        != gPilServicesEvt) &&
      (EFI_SUCCESS != gBS->CloseEvent(gPilServicesEvt)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to close event for Pil Services\n"));
  }

  if ((NULL != gVariableServicesEvt) &&
      (EFI_SUCCESS != gBS->CloseEvent(gVariableServicesEvt)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to close event for Variable Services\n"));
  }

  if (EFI_SUCCESS != DisplayPwr_InitLPMSupport(FALSE))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to Close event LPM\n"));
  }
}


/**********************************************************************************************************************
     Public APIs
**/
/**
  Initialize the state information for the Display Dxe

  @param  ImageHandle   of the loaded driver
  @param  SystemTable   Pointer to the System Table

  @retval EFI_SUCCESS           Protocol registered
  @retval EFI_OUT_OF_RESOURCES  Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR      Hardware problems

**/
EFI_STATUS
EFIAPI
DisplayDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              eStatus            = EFI_SUCCESS;
  EFI_GUID                sOutputGUID        = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_HANDLE              hUEFIDisplayHandle = NULL;
  MDP_InitParamsType      sInitParam;
  MDP_PropertiesParamType sDisplayProp;
  MDPPlatformParams       sPlatformParams;

  // Clear out global structures
  MDP_OSAL_MEMZERO(&gDxeModeInfo, sizeof(DisplayDxe_ModeInfo));
  MDP_OSAL_MEMZERO(&sInitParam,      sizeof(MDP_InitParamsType));
  MDP_OSAL_MEMZERO(&sDisplayProp,    sizeof(MDP_PropertiesParamType));
  MDP_OSAL_MEMZERO(&sPlatformParams, sizeof(MDPPlatformParams));

  /* Register for all event callbacks */
  DisplayDxeRegisterEvents();

  eStatus = DisplayMCDxe_ReadMultiCoreInfo();

  sDisplayProp.pSupportedDisplayList = (bool32 (*)[MDP_DISPLAY_MAX])DisplaySupportTableInit();

  if (MDP_STATUS_OK != (eStatus = MDPPlatformConfigure(MDP_DISPLAY_PRIMARY, MDPPLATFORM_CONFIG_INIT, &sPlatformParams)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: MDPPlatformConfigure(MDPPLATFORM_CONFIG_INIT) failed!\n"));
  }
  else
  {
    // Initialize Apps BootLoader (ABL) interface
    // It checks for panel override, if any, set by ABL and sets up parameters which will be used by MDPInit
    if (MDP_STATUS_OK != MDPSetProperty(MDP_DISPLAY_PRIMARY, MDP_DISPLAY_PROPERTY_ABL_INTERFACE_INIT, &sDisplayProp))
    {
      DEBUG((EFI_D_INFO, "DisplayDxe: Failed to initialize ABL!\n"));
    }

    MDP_OSAL_MEMCPY(&sInitParam.aSupportedDisplays, &gSupportedDisplayList, sizeof(sInitParam.aSupportedDisplays));

    if (1 == gDxeModeInfo.uDedicatedCoreEnabled)
    {
      eStatus = DisplayMCDxe_DisplayInitialize();
    }
    else if (MDP_STATUS_OK != MDPInit(&sInitParam, MDP_INIT_FLAG_MMU_INIT))
    {
      // MDP Initialization failed
      DEBUG ((EFI_D_ERROR, "DisplayDxe: MDP init failed!\n"));
      eStatus = EFI_DEVICE_ERROR;
    }
    else
    {
      MDP_PowerParamsType      sPowerParams;
      MDP_DetectParamType      sDetectParams;
      MDP_Display_IDType       eDisplayId = MDP_DISPLAY_PRIMARY;

      MDP_OSAL_MEMZERO(&sPowerParams, sizeof(MDP_PowerParamsType));
      MDP_OSAL_MEMZERO(&sDetectParams, sizeof(MDP_DetectParamType));      

      sPowerParams.bPowerOn         = TRUE;
      sDisplayProp.bDisplayPwrState = TRUE;

      /////////////////////////////////
      // Primary / Secondary Display //
      /////////////////////////////////

      // If the primary / secondary display is supported initialize it
      for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId < MDP_DISPLAY_MAX; eDisplayId++)
      {
        sDisplayProp.bIsSupported = sInitParam.aSupportedDisplays[eDisplayId];

        if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_SUPPORTED_DISPLAY, &sDisplayProp))
        {
          DEBUG ((EFI_D_ERROR, "DisplayDxe: Failed to set property!\n"));
        }
        if (FALSE == sInitParam.aSupportedDisplays[eDisplayId])
        {
          continue;
        }
        else if (MDP_STATUS_OK != MDPPower(eDisplayId, &sPowerParams, 0x0))
        {
          DEBUG ((EFI_D_WARN, "DisplayDxe: Common display panel power up failed!\n"));
        }
        else if (MDP_STATUS_OK != MDPDetect(eDisplayId, &sDetectParams, 0x0))
        {
          eStatus = EFI_DEVICE_ERROR;
        }
        else if (MDP_STATUS_OK != MDPPower(eDisplayId, &sPowerParams, POWERCONFIG_FLAGS_PANEL_POWER))
        {
          DEBUG ((EFI_D_WARN, "DisplayDxe: Specific display panel power up failed!\n")); 
        }
        else if (TRUE == sDetectParams.bDisplayDetected)
        {
          // Add/filter any new modes according to resolution table and max resolution table.
          if (MDP_STATUS_OK != DisplayDxe_FilterMode(eDisplayId, &sDetectParams, 0x0))
          {
            DEBUG ((EFI_D_WARN, "DisplayDxe: Failed to add/filter new DisplayDxe modes!\n"));
          }
          
           // Set the display power state to on 
           
          if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp))
          {
            eStatus = EFI_DEVICE_ERROR;
          }
        }
      }


      // Default at some dummy mode
      gDxeModeInfo.sCurrentModeInfo.Version                       = GRAPHICS_OUTPUT_PROTOCOL_REVISION;
      gDxeModeInfo.sCurrentModeInfo.PixelFormat                   = DISPLAYDXE_DEFAULT_PIXEL_FORMAT;
      gDxeModeInfo.sCurrentModeInfo.HorizontalResolution          = 0;
      gDxeModeInfo.sCurrentModeInfo.VerticalResolution            = 0;
      gDxeModeInfo.sCurrentModeInfo.PixelInformation.RedMask      = DISPLAYDXE_RED_MASK;
      gDxeModeInfo.sCurrentModeInfo.PixelInformation.GreenMask    = DISPLAYDXE_GREEN_MASK;
      gDxeModeInfo.sCurrentModeInfo.PixelInformation.BlueMask     = DISPLAYDXE_BLUE_MASK;
      gDxeModeInfo.sCurrentModeInfo.PixelInformation.ReservedMask = DISPLAYDXE_ALPHA_MASK;
      gDxeModeInfo.sCurrentModeInfo.PixelsPerScanLine             = 0;

      // Setup the protocol information, set the current mode to an invalid mode forcing a set mode
      gDxeModeInfo.sProtocolInfo.MaxMode                          = gDxeModeInfo.uNumModes[MDP_DISPLAY_PRIMARY];
      gDxeModeInfo.sProtocolInfo.Mode                             = (UINT32)-1;
      gDxeModeInfo.sProtocolInfo.SizeOfInfo                       = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
      gDxeModeInfo.sProtocolInfo.Info                             = &gDxeModeInfo.sCurrentModeInfo;

      if (sInitParam.aSupportedDisplays[MDP_DISPLAY_EXTERNAL]  == TRUE || 
          sInitParam.aSupportedDisplays[MDP_DISPLAY_EXTERNAL2] == TRUE || 
          sInitParam.aSupportedDisplays[MDP_DISPLAY_EXTERNAL3] == TRUE)
      {
        gDxeModeInfo.bExtSupported            = TRUE;
      }
      else
      {
        gDxeModeInfo.bExtSupported            = FALSE;
      }
      
      //Update ABL with selected panel info
      Display_Utils_SetPanelConfiguration(gSupportedDisplayList);      

      // Install display protocols only if panel is detected.
      // Make a new handle with EFI Graphics Protocol
      if (EFI_SUCCESS != (eStatus = gBS->InstallMultipleProtocolInterfaces (&hUEFIDisplayHandle,
                                                                            &gEfiDevicePathProtocolGuid,
                                                                            &DisplayDevicePath,
                                                                            &sOutputGUID,
                                                                            &gDisplayDxeOutputProtocol,
                                                                            &gQcomDisplayPwrCtrlProtocolGuid,
                                                                            &gDisplayPwrCtrlProtocolImplementation,
                                                                            &gEfiDisplayPowerStateProtocolGuid,
                                                                            &gDisplayPwrProtocolImplementation,
                                                                            &gQcomDisplayUtilsProtocolGuid,
                                                                            &gQcomDisplayUtilsProtocolImplementation,
                                                                             NULL)))
      {
        DEBUG ((EFI_D_INFO, "DisplayDxe: InstallMultipleProtocolInterfaces failed!\n"));
      }
    }
  }

  if (1 == gDxeModeInfo.uDedicatedCoreEnabled)
  {
    /* In case of success, do nothing
     * In case of error, unregister the event callbacks in else condition
     */
  }
  if (eStatus != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN, "DisplayDxe: Unregister Event Callbacks due to Error\n"));

    if (EFI_SUCCESS != gBS->CloseEvent(EfiExitBootServicesEvent))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to Close event ExitBoot\n"));
    }

    DisplayDxeUnRegisterEvents();
  }

  return eStatus;
}


/**
  Display DXE Protocol Query function
  
  @param  This             The EFI_GRAPHICS_OUTPUT_PROTOCOL instance.
  @param  ModeNumber       The mode number to return information on.
  @param  SizeOfInfo       A pointer to the size, in bytes, of the Info buffer.
  @param  Info             A pointer to callee allocated buffer that returns information about ModeNumber.

  @retval EFI_SUCCESS               Mode returned successfully
  @retval EFI_DEVICE_ERROR          Hardware error querying the mode
  @retval EFI_INVALID_PARAMETER     Invalid parameters/null parameters

**/
EFI_STATUS
DisplayDxe_QueryMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  UINT32                                ModeNumber,
  UINTN                                 *SizeOfInfo,
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
  )
{
  EFI_STATUS                            eStatus   = EFI_SUCCESS;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pModeInfo = NULL;

  if ((NULL == SizeOfInfo) || (NULL == Info) || (NULL == This))
  {
    eStatus = EFI_INVALID_PARAMETER;
  }
  else
  {
    MDP_Display_IDType eDisplayId = MDP_DISPLAY_PRIMARY;

    eDisplayId = DisplayDxe_GetFirstAvailableDisplayId();

    if (ModeNumber >= gDxeModeInfo.uNumModes[eDisplayId])
    {
      // Mode is out of range
      eStatus = EFI_INVALID_PARAMETER;
    }
    else if (NULL == (pModeInfo = AllocatePool(sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION))))
    {
      // Allocation failed
      eStatus = EFI_INVALID_PARAMETER;
    }
    else
    {
      // Return results (single supported mode)
      MDP_OSAL_MEMCPY(pModeInfo, &gDxeModeInfo.aDxeModeList[eDisplayId][ModeNumber], sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
       
      *Info       = pModeInfo;
      *SizeOfInfo = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
    }
  }
   
  return eStatus;
}



/**
  Display DXE Protocol set mode
  
  @param  This                      The EFI_GRAPHICS_OUTPUT_PROTOCOL instance.
  @param  ModeNumber                Abstraction that defines the current video mode.

  @retval EFI_SUCCESS               The graphics mode specified by ModeNumber was selected.
  @retval EFI_DEVICE_ERROR          The device had an error and could not complete the request.
  @retval EFI_UNSUPPORTED           ModeNumber is not supported by this device.
  @retval EFI_OUT_OF_RESOURCES      Memory allocation failed
  @retval EFI_INVALID_PARAMETER     Invalid protocol handle passed in
**/
EFI_STATUS
DisplayDxe_SetMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  UINT32                       ModeNumber
  )
{
  EFI_STATUS          eStatus    = EFI_SUCCESS;
  MemRegionInfo       sMemRegionInfo;

  if (NULL == This)
  {
    eStatus = EFI_INVALID_PARAMETER;
  }
    else if (ModeNumber >= gDxeModeInfo.uNumModes[MDP_DISPLAY_PRIMARY])
    {  
       eStatus = RETURN_UNSUPPORTED;
    }
    else if (TRUE == gDxeModeInfo.bModeSet)
    {
        // Enter this path if the mode has already been set once.  As all subsequent mode sets are virtual mode changes (scaled mode)
        if (ModeNumber != gDxeModeInfo.uModeId)
        {   
            EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pMode = &gDxeModeInfo.aDxeModeList[MDP_DISPLAY_PRIMARY][ModeNumber];
            MDP_PropertiesParamType               sModeInfo;
            
            // Setup the virtual mode by changing the source cropping rectangle.
            MDP_OSAL_MEMZERO(&sModeInfo, sizeof(MDP_PropertiesParamType));
            sModeInfo.sModeParams.sRectInfo.uWidth  = pMode->HorizontalResolution;
            sModeInfo.sModeParams.sRectInfo.uHeight = pMode->VerticalResolution;
            
            if (MDP_STATUS_OK == MDPSetProperty(MDP_DISPLAY_PRIMARY, MDP_DISPLAY_PROPERTY_SURFACE_RECT, &sModeInfo))
            {
              // Update the Dxe information there is an new mode configured
              MDP_OSAL_MEMCPY(&gDxeModeInfo.sCurrentModeInfo, pMode, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

              // Update both the internal and external mode number we report              
              gDxeModeInfo.uModeId             = ModeNumber;
              gDxeModeInfo.sProtocolInfo.Mode  = ModeNumber;
              eStatus                          = EFI_SUCCESS;            
            }
            else
            {
              DEBUG ((EFI_D_ERROR, "DisplayDxe: MDPSetMode() fail to change to a virtual mode!\n"));
              eStatus = EFI_DEVICE_ERROR;
            }
        
        }
        else
        {
          // This mode is already set, just continue
          eStatus = EFI_SUCCESS;
        }
    }
  else if (EFI_SUCCESS != (eStatus = GetMemRegionInfoByName("Display Reserved", &sMemRegionInfo)))
  {
    // Failed to get memory region info
    eStatus = EFI_DEVICE_ERROR;
  }
  else 
  {
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pMode            = NULL;
    UINTN                                 uFrameBufferSize = 0;
    UINTN                                 uMemBase;
    VOID                                 *pFrameBufferBase;
    MDP_SetModeParamType                  sModeParams;
    MDP_Display_IDType                    eDisplayId = MDP_DISPLAY_PRIMARY;
    MDP_Status                            eMDPStatus[MDP_DISPLAY_MAX];
    MDP_PropertiesParamType               sDisplayProp;

    MDP_OSAL_MEMZERO(&eMDPStatus,   sizeof(eMDPStatus));
    MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));

    //////////////////////////
    // Allocate frame buffer//
    /////////////////////////
    /****************************************************************************************/
    uMemBase                   = (UINTN)sMemRegionInfo.MemBase;
    pFrameBufferBase           = (VOID *)uMemBase;
    gDxeModeInfo.eDisplayFBAttrib = sMemRegionInfo.CacheAttributes;

    // 
    // If primary display exists use the primary display resolution for the frame buffer allocation
    // If only external display exists use the selected mode resolution for the frame buffer allocation
    //
    eDisplayId = DisplayDxe_GetFirstAvailableDisplayId();

    pMode = &gDxeModeInfo.aDxeModeList[eDisplayId][ModeNumber];

    // Clear the frame buffer
    uFrameBufferSize = pMode->HorizontalResolution * pMode->VerticalResolution * DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL;
    MDP_OSAL_MEMZERO(pFrameBufferBase, uFrameBufferSize);

    /* write through frame buffer */
    DisplayDxe_FlushStaleLines(pFrameBufferBase, uFrameBufferSize);

    // Setup the mode params
    MDP_OSAL_MEMZERO(&sModeParams, sizeof(MDP_SetModeParamType));
    sModeParams.sSurfaceInfo.ePixelFormat  = DISPLAYDXE_DEFAULT_MDP_FORMAT;
    sModeParams.sSurfaceInfo.uWidth        = pMode->HorizontalResolution;
    sModeParams.sSurfaceInfo.uHeight       = pMode->VerticalResolution;
    sModeParams.sSurfaceInfo.pPlane0Offset = pFrameBufferBase;
    sModeParams.sSurfaceInfo.uPlane0Stride = pMode->HorizontalResolution * DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL;

    /****************************************************************************************/

    /////////////////////////////////
    // Primary / Secondary Display //
    /////////////////////////////////
    for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId < MDP_DISPLAY_MAX; eDisplayId++)
    {
      if (0 == gDxeModeInfo.uNumModes[eDisplayId])
      {
        continue;
      }
      else
      {
        //Set Mode index to selected Mode
        sModeParams.uModeIndex = gDxeModeInfo.uSelectedModeIndex[eDisplayId];
         
        if (MDP_STATUS_OK != (eMDPStatus[eDisplayId] = MDPSetMode(eDisplayId, &sModeParams, 0x0)))
        {
          DEBUG ((EFI_D_ERROR, "DisplayDxe: MDPSetMode() failed!\n"));
        }
      }
    }


    //
    // Setup the default backlight level
    //
    if (EFI_SUCCESS == GetChargerPowerStatus())
    {
      /* Assign the default brightness */
      sDisplayProp.uBacklightLevel = MDP_DISPLAY_DEFAULT_BRIGHTNESS;
    }
    else
    {
      // Charger power status is not okay or at off mode charging, so turn off backlight as default
      sDisplayProp.uBacklightLevel = 0;
      DEBUG ((EFI_D_ERROR, "DisplayDxe: Charger power status is not okay or at off mode charging, so turn off backlight!\n"));
    }

    for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId < MDP_DISPLAY_MAX; eDisplayId++)
    {
      if (0 == gDxeModeInfo.uNumModes[eDisplayId])
      {
        continue;
      }
      else if (MDP_STATUS_OK == eMDPStatus[eDisplayId])
      {
        MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_BACKLIGHT, &sDisplayProp);

        //
        // Populate the display environment information, used to inform the OS driver of firmware state.
        //
        MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_FIRMWAREENV, NULL);
      }
    }


    // Save the current mode
    if ((EFI_SUCCESS == eStatus) && 
        (NULL != pMode))
    {
      MDP_OSAL_MEMCPY(&gDxeModeInfo.sCurrentModeInfo, pMode, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
      gDxeModeInfo.uModeId = ModeNumber;
         
      //
      // Set the protocol information
      //
      gDxeModeInfo.sProtocolInfo.FrameBufferBase = (EFI_PHYSICAL_ADDRESS)pFrameBufferBase;
      gDxeModeInfo.sProtocolInfo.FrameBufferSize = uFrameBufferSize;
      gDxeModeInfo.sProtocolInfo.Mode            = ModeNumber;
      gDxeModeInfo.sProtocolInfo.Info            = &gDxeModeInfo.sCurrentModeInfo;
      gDxeModeInfo.uDisplayFBWidth               = pMode->HorizontalResolution;
      gDxeModeInfo.uDisplayFBHeight              = pMode->VerticalResolution;      
      gDxeModeInfo.bModeSet                      = TRUE;
    }
  }

  return eStatus;
}

/**
  Display DXE Protocol set mode for external display
  
  @param  DisplayId                 display id, can be either primary, secondary or external
  @param  ModeNumber                Abstraction that defines the current video mode.
  @param  Flags                     Other info, set to DISPLAY_UTILS_EXT_FORCE_MODE if force
                                    specific mode from the table of supported modes

  @retval EFI_SUCCESS               The graphics mode specified by ModeNumber was selected.
  @retval EFI_DEVICE_ERROR          The device had an error and could not complete the request.
  @retval EFI_UNSUPPORTED           ModeNumber is not supported by this device.
  @retval EFI_OUT_OF_RESOURCES      Memory allocation failed
  @retval EFI_INVALID_PARAMETER     Invalid protocol handle passed in
**/
EFI_STATUS
DisplayDxe_SetModeExt (
  UINT32                       DisplayId,
  UINT32                       ModeNumber,
  UINT32                       Flags
)
{
    MDP_PowerParamsType                  sPowerParams;
    MDP_DetectParamType                  sDetectParams;
    MDP_PropertiesParamType              sDisplayProp;
    MDP_SetModeParamType                 sModeParams;
    VOID                                 *pFrameBufferBase = (VOID *)gDxeModeInfo.sProtocolInfo.FrameBufferBase;
    EFI_STATUS                           eStatus           = EFI_SUCCESS;

   if (((MDP_DISPLAY_EXTERNAL == DisplayId)   ||       /* miniDP */
        (MDP_DISPLAY_EXTERNAL2 == DisplayId)  ||       /* DP over usb-0 */
        (MDP_DISPLAY_EXTERNAL3 == DisplayId)) &&       /* DP over usb-1 */
        (TRUE == gDxeModeInfo.bExtSupported))
   {
      EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pMode = &gDxeModeInfo.aDxeModeList[MDP_DISPLAY_PRIMARY][0]; /* primary display with mode 0 */

      if ((pMode->HorizontalResolution > 0) &&      /* primary display had presented */
          (pMode->VerticalResolution > 0))
      {
           //////////////////////
           // External Display //
           //////////////////////

          if (gDxeModeInfo.bModeSet)                   //re-init if mode has previously being set
          {
            MDPDeInit(DisplayId, 0x0);
          }

          MDP_OSAL_MEMZERO(&sPowerParams, sizeof(MDP_PowerParamsType));
          sPowerParams.bPowerOn = TRUE;
          sDisplayProp.bDisplayPwrState = TRUE;

          if (MDP_STATUS_OK != MDPPower(DisplayId, &sPowerParams, 0x0))
          {
            DEBUG ((EFI_D_ERROR, "DisplayDxe: External panel power up failed!\n"));    
          }              
          else 
          {
            MDP_OSAL_MEMZERO(&sDetectParams, sizeof(MDP_DetectParamType));
                  
            // Default reporting of External display
            if ((MDP_STATUS_OK != MDPDetect(DisplayId, &sDetectParams, Flags)) || 
                (FALSE == sDetectParams.bDisplayDetected))
            {
                // External display not detected, turn off and continue on
                DEBUG ((EFI_D_ERROR, "DisplayDxe: External panel not detected\n"));    
                eStatus = EFI_NOT_FOUND; 
            }
            else
            {
              sDetectParams.uModeSelected = ModeNumber; /* which mode to display */
    
              if (MDP_STATUS_OK != (eStatus = DisplayDxe_FilterMode(DisplayId, &sDetectParams, Flags)))
              {
                DEBUG ((EFI_D_ERROR, "DisplayDxe: failed to select external display mode!\n"));
              }
              else if (MDP_STATUS_OK != (eStatus = MDPSetProperty(DisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp)))
              {
                DEBUG ((EFI_D_ERROR, "DisplayDxe: failed to set external display power state to on!\n"));
              }
              else
              {
                // Setup the mode params
                MDP_OSAL_MEMZERO(&sModeParams, sizeof(MDP_SetModeParamType));
                sModeParams.sSurfaceInfo.ePixelFormat  = DISPLAYDXE_DEFAULT_MDP_FORMAT;
                sModeParams.sSurfaceInfo.uWidth        = pMode->HorizontalResolution;
                sModeParams.sSurfaceInfo.uHeight       = pMode->VerticalResolution;
                sModeParams.sSurfaceInfo.pPlane0Offset = pFrameBufferBase;
                sModeParams.sSurfaceInfo.uPlane0Stride = pMode->HorizontalResolution * DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL;

                //Set Mode index to selected Mode
                sModeParams.uModeIndex                 = gDxeModeInfo.uSelectedModeIndex[DisplayId];
          
                // Setup the mode
                if (MDP_STATUS_OK != MDPSetMode(DisplayId, &sModeParams, 0x0))
                {
                  DEBUG ((EFI_D_ERROR, "DisplayDxe: MDPSetModeExt() failed!\n"));
                  eStatus = EFI_DEVICE_ERROR;
                }
              }
            }
          }
       }
       else 
       {
          eStatus = EFI_DEVICE_ERROR;
          DEBUG ((EFI_D_ERROR, "DisplayDxe: Cannot change modes on the external if the primary has not been set yet\n"));
       }
   }
   else
   {
      eStatus = EFI_DEVICE_ERROR;
   }
    return eStatus;
}


/**
  Display DXE Protocol Blt function
  
  @param  This         Protocol instance pointer.
  @param  BltBuffer    Buffer containing data to blit into video buffer. This
                       buffer has a size of Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
  @param  BltOperation Operation to perform on BlitBuffer and video memory
  @param  SourceX      X coordinate of source for the BltBuffer.
  @param  SourceY      Y coordinate of source for the BltBuffer.
  @param  DestinationX X coordinate of destination for the BltBuffer.
  @param  DestinationY Y coordinate of destination for the BltBuffer.
  @param  Width        Width of rectangle in BltBuffer in pixels.
  @param  Height       Hight of rectangle in BltBuffer in pixels.
  @param  Delta        OPTIONAL

  @retval EFI_SUCCESS           The Blt operation completed.
  @retval EFI_INVALID_PARAMETER BltOperation is not valid or source/destination parameters are invalid
  @retval EFI_DEVICE_ERROR      A hardware error occured writting to the video buffer.


**/
EFI_STATUS
DisplayDxe_Blt (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL            *This,
  IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL           *BltBuffer,
  IN  EFI_GRAPHICS_OUTPUT_BLT_OPERATION       BltOperation,
  IN  UINTN                                   SourceX,
  IN  UINTN                                   SourceY,
  IN  UINTN                                   DestinationX,
  IN  UINTN                                   DestinationY,
  IN  UINTN                                   Width,
  IN  UINTN                                   Height,
  IN  UINTN                                   Delta)
{
    EFI_STATUS        eStatus = EFI_SUCCESS;

    if (NULL == This)
    {
        eStatus = EFI_INVALID_PARAMETER;
    }
    else if (FALSE == gDxeModeInfo.bModeSet)
    {
         eStatus = EFI_NOT_STARTED;
    }
    else
    {
      // Resolution limits should come from the current mode (gDxeModeInfo.sCurrentModeInfo), but the framework does not handle
      // dynamic changes to resolution to restrict the text width/height.  Keep original FB dimensions for blt operations when in a 
      // actual mode or a virtual (scaled) mode.
      UINT32 uFrameBufferWidth  = gDxeModeInfo.uDisplayFBWidth; 
      UINT32 uFrameBufferHeight = gDxeModeInfo.uDisplayFBHeight;
      UINT32 uStride            = uFrameBufferWidth * DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL;
    
      switch (BltOperation)
      {
      case EfiBltBufferToVideo:
        {
          UINT8 *pSrcBuffer = (UINT8*)BltBuffer;
          UINT8 *pDstBuffer = (UINT8*)DISPLAYDXE_PHYSICALADDRESS32(gDxeModeInfo.sProtocolInfo.FrameBufferBase);
          UINTN SrcStride, DstStride, CopyWidth, CopyHeight;

          if (((DestinationX + Width) > gDxeModeInfo.sCurrentModeInfo.HorizontalResolution) ||
              ((DestinationY + Height) > gDxeModeInfo.sCurrentModeInfo.VerticalResolution))
          {
             return EFI_INVALID_PARAMETER;
          }

          CopyWidth  = Width;
          CopyHeight = Height;

          /* Video buffer stride in bytes, consider padding as well */
          DstStride = uStride;

          /* Src buffer stride in bytes. Delta is valid when X or Y is not 0 */
          SrcStride = Width * DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL;
          if (Delta != 0)
          {  
            SrcStride = Delta;
          }

          DisplayDxeBltInternal (pSrcBuffer,
                                 pDstBuffer,
                                 SourceX, 
                                 SourceY, 
                                 CopyWidth, 
                                 CopyHeight,
                                 SrcStride,
                                 DestinationX,
                                 DestinationY,
                                 DstStride,
                                 DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL);
          
          /* If display frame buffer is cached, need to call cache maintenance function */
          DisplayDxe_FlushStaleLines(((UINT8*)pDstBuffer) + (DstStride * DestinationY), (DstStride * CopyHeight));

          eStatus = EFI_SUCCESS;
        }
        break;

      case EfiBltVideoToBltBuffer:
        {  
          UINT8 *pDstBuffer = (UINT8*)BltBuffer;
          UINT8 *pSrcBuffer = (UINT8*)DISPLAYDXE_PHYSICALADDRESS32(gDxeModeInfo.sProtocolInfo.FrameBufferBase);
          UINTN SrcStride, DstStride, CopyWidth, CopyHeight;

          if (((SourceX + Width) > uFrameBufferWidth) ||
              ((SourceY + Height) > uFrameBufferHeight))
          {
            return EFI_INVALID_PARAMETER;
          }

          CopyWidth  = Width;
          CopyHeight = Height;

          /* Video buffer stride in bytes, consider padding as well */
          SrcStride = uStride;

          /* Buffer stride in bytes. Delta is valid when X or Y is not 0 */
          DstStride = Width * DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL;
          if (Delta != 0)
          {
            DstStride = Delta;
          }

          DisplayDxeBltInternal (pSrcBuffer,
                                 pDstBuffer,
                                 SourceX, 
                                 SourceY, 
                                 CopyWidth, 
                                 CopyHeight,
                                 SrcStride,
                                 DestinationX,
                                 DestinationY,
                                 DstStride,
                                 DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL);

          eStatus = EFI_SUCCESS;
        }
        break; 

      case EfiBltVideoFill:
        {
          UINT32 SrcPixel = *(UINT32*)BltBuffer;
          UINT32 *pDstBuffer = (UINT32*)DISPLAYDXE_PHYSICALADDRESS32(gDxeModeInfo.sProtocolInfo.FrameBufferBase);
          UINTN  DstStride, CopyWidth, CopyHeight;
          UINT32 x,y;

          if (((DestinationX + Width) > uFrameBufferWidth) ||
              ((DestinationY + Height) > uFrameBufferHeight))
          {
            return EFI_INVALID_PARAMETER;
          }

          CopyWidth  = Width;
          CopyHeight = Height;

          /* Video buffer stride in bytes, consider padding as well */
          DstStride = uStride;

          /* Adjust Destination location */
          pDstBuffer = (UINT32*)(((UINT8*)pDstBuffer) + (DestinationY * DstStride) + DestinationX * DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL); 

          /* Do the actual blitting */
          for (y = 0; y < CopyHeight; y++)
          {
              for (x = 0; x < CopyWidth; x++)
              {
                  pDstBuffer[x] = SrcPixel;
              }
              /* Increment by stride number of bytes */
              pDstBuffer = (UINT32*)((UINT8*)pDstBuffer + DstStride);
          }

          /* If display frame buffer is cached, need to call cache maintenance function */
          pDstBuffer = (UINT32*)DISPLAYDXE_PHYSICALADDRESS32(gDxeModeInfo.sProtocolInfo.FrameBufferBase);
          DisplayDxe_FlushStaleLines(((UINT8*)pDstBuffer) + (DstStride * DestinationY), (DstStride * CopyHeight));

          eStatus = EFI_SUCCESS;
         }
         break;

       case EfiBltVideoToVideo:
         {
           UINT8 *pSrcBuffer = (UINT8*)DISPLAYDXE_PHYSICALADDRESS32(gDxeModeInfo.sProtocolInfo.FrameBufferBase);
           UINT8 *pDstBuffer = (UINT8*)DISPLAYDXE_PHYSICALADDRESS32(gDxeModeInfo.sProtocolInfo.FrameBufferBase);
           UINTN  Stride, CopyWidth, CopyHeight;

           if (((SourceX + Width) > uFrameBufferWidth))
             Width = uFrameBufferWidth - SourceX;

           if (((SourceY + Height) > uFrameBufferHeight))
             Height = uFrameBufferHeight - SourceY;

           if (((DestinationX + Width) > uFrameBufferWidth))
             Width = uFrameBufferWidth - DestinationX;

           if (((DestinationY + Height) > uFrameBufferHeight))
             Height = uFrameBufferHeight - DestinationY;

           CopyWidth  = Width;
           CopyHeight = Height;

           /* Video buffer stride in bytes, consider padding as well */
           Stride = uStride;

           DisplayDxeBltInternal (pSrcBuffer,
                                  pDstBuffer,
                                  SourceX, 
                                  SourceY, 
                                  CopyWidth, 
                                  CopyHeight,
                                  Stride,
                                  DestinationX,
                                  DestinationY,
                                  Stride,
                                  DISPLAYDXE_DEFAULT_BYTES_PER_PIXEL);

           /* If display frame buffer is cached, need to call cache maintenance function */
           DisplayDxe_FlushStaleLines(((UINT8*)pDstBuffer) + (Stride * DestinationY), (Stride * CopyHeight));
           eStatus = EFI_SUCCESS;

         }
         break;

       default:
          DEBUG ((EFI_D_INFO, "DisplayDxe: BltOperation not supported\n"));
          eStatus = RETURN_INVALID_PARAMETER;
          break;
      }    
    }


    return eStatus;    
}

/**
DisplayDxe_FlushStaleLines

Check if Frame buffer is cached, and perform cache maintenance.

@param  pAddress            Start of frame buffer address
@param  uLength             Length of frame buffer need to write back from cache

@retval None

**/
void DisplayDxe_FlushStaleLines(VOID *pAddress, uint32 uLength)
{
  if ((ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK_XN == gDxeModeInfo.eDisplayFBAttrib) ||
      (ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK    == gDxeModeInfo.eDisplayFBAttrib))
  {
    WriteBackDataCacheRange(pAddress, uLength);
  }
}

/**
DisplayDxe_GetFirstAvailableDisplayId

Get the first available display id.

@param  None

@retval eDisplayId        Valid display id can be detected

**/
MDP_Display_IDType DisplayDxe_GetFirstAvailableDisplayId(void)
{
  EFI_STATUS         eStatus    = EFI_DEVICE_ERROR;
  MDP_Display_IDType eDisplayId = MDP_DISPLAY_PRIMARY;

  for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId <= MDP_DISPLAY_EXTERNAL; eDisplayId++)
  {
    if (gDxeModeInfo.uNumModes[eDisplayId] > 0)
    {
      eStatus = EFI_SUCCESS;
      break;
    }
  }

  if (EFI_SUCCESS != eStatus)
  {
    eDisplayId = MDP_DISPLAY_PRIMARY;
  }

  return eDisplayId;
}

/**
  DisplayDxe_FilterMode
  
  Display DXE Local function to select the mode out of supported modes. Function will also generate a list of
  virtual modes for the internal display.
  
  @param  eDisplayId        Display ID
  @param  pDisplayModes     List of modes reported by the sink
  @param  uFlags            Mode flags 
  
  @retval EFI_SUCCESS       Execution successful
  @retval other             Error occurred

**/
EFI_STATUS DisplayDxe_FilterMode(MDP_Display_IDType eDisplayId, MDP_DetectParamType  *pDisplayModes, uint32 uFlags)
{
   EFI_STATUS     Status = EFI_SUCCESS;

   if (NULL == pDisplayModes)
   {
      Status = EFI_INVALID_PARAMETER;
   }
   else if (0 == pDisplayModes->uSupportedModes)
   {
      Status = EFI_NOT_FOUND;
   }
   else
   {
      MDP_PropertiesParamType sDisplayProp;  
      
      // Handle each display
      switch (eDisplayId)
      {
         //For Primary always default to the first mode which is native.
        case MDP_DISPLAY_PRIMARY:
        case MDP_DISPLAY_SECONDARY:            
        {
            // Store the mode that the caller requested   
            gDxeModeInfo.uSelectedModeIndex[eDisplayId] = pDisplayModes->uModeSelected;

            // Internal displays are limited to select only the physical resolution (index 0)
            MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType)); 
            sDisplayProp.sModeParams.uModeIndex = 0;   

            // Notify the driver that we have selected a particular mode
            if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_MODE_INFO, &sDisplayProp))
            {
              DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to set property while forcing mode\n"));
            }
        }
        break;

        // Select the best mode out of the list of supported modes available on the external monitor
        case MDP_DISPLAY_EXTERNAL:
        case MDP_DISPLAY_EXTERNAL2:
        case MDP_DISPLAY_EXTERNAL3:
        {
            // Store the mode that the caller requested
            gDxeModeInfo.uSelectedModeIndex[eDisplayId] = pDisplayModes->uModeSelected;            
            
            // The FORCE_MODE flag forces a specific resolution from the drivers table, irrespetive of wha the EDID reports.
            if (uFlags & DISPLAY_UTILS_EXT_FORCE_MODE)
            {
                // Use MDP_DISPLAY_PROPERTY_SELECT_MODE_INDEX property to select a specific mode
                MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType)); 
                sDisplayProp.uIndex = pDisplayModes->uModeSelected;
                
                if (MDP_STATUS_OK == MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_SELECT_MODE_INDEX,  &sDisplayProp))
                {
                  // Mode is from the drvier, resolution is not available so we can only print the index
                  DEBUG ((EFI_D_ERROR, "DisplayDxe: External Mode#%d selected.\n",
                              pDisplayModes->uModeSelected));
                
                }
                else
                {
                   DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to set property while forcing mode\n"));
                   Status = RETURN_INVALID_PARAMETER;                   
                }            
            }
            else
            {
                // Use MDP_DISPLAY_PROPERTY_MODE_INFO property to select a specific mode            
                MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType)); 
                sDisplayProp.sModeParams.uModeIndex = gDxeModeInfo.uSelectedModeIndex[eDisplayId];               

                // Notify the driver that we have selected a particular mode
                if (MDP_STATUS_OK == MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_MODE_INFO, &sDisplayProp))
                {
                   uint32 uModeID = gDxeModeInfo.uSelectedModeIndex[eDisplayId];
                   
                   DEBUG ((EFI_D_ERROR, "DisplayDxe: External Panel Mode#%d %dx%d %dHz selected.\n",
                           uModeID,
                           pDisplayModes->aModeList[uModeID].uWidth,
                           pDisplayModes->aModeList[uModeID].uHeight,
                          (pDisplayModes->aModeList[uModeID].uRefreshRate / 0x10000)));

                }
                else
                {
                  DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to set property while forcing mode\n"));
                  Status = RETURN_INVALID_PARAMETER;
                }
            }
        }
        break;

        default:
            Status = EFI_INVALID_PARAMETER;
        break;
      }

      // If we haven't built the mode table for this display, build it now
      if ((EFI_SUCCESS == Status) &&
          (0 == gDxeModeInfo.uNumModes[eDisplayId]))
      {
          // Mode set on the display is the native panel resolution, which can be different from what the Dxe reports.
         UINT32                  uPhysicalResolution = gDxeModeInfo.uSelectedModeIndex[eDisplayId];
         UINT32                  uResLimitIndex      = (PcdGet32(PcdFrameBufMaxRes) < DISPLAY_TABLE_SIZE(gFBResLimit))?PcdGet32(PcdFrameBufMaxRes):0;
         UINT32                  uModeIndex          = 0;
         UINT32                  uVirtualModeIndex   = 0;

         while (uModeIndex < DISPLAYDXE_MAX_MODES)
         {
           MDP_ResolutionType                    sResolution;
           EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pMode = &gDxeModeInfo.aDxeModeList[eDisplayId][uModeIndex];

           // Mode 0 is a special case where the resolution comes from the panel native resolution, all other modes are virtual.
           if (0 == uModeIndex)
           {
                sResolution.uWidth  = pDisplayModes->aModeList[uPhysicalResolution].uWidth;
                sResolution.uHeight = pDisplayModes->aModeList[uPhysicalResolution].uHeight;
           }
           else if ((MDP_DISPLAY_PRIMARY == eDisplayId) &&
                    (uVirtualModeIndex < DISPLAY_TABLE_SIZE(gVirtualResTable)))
           {
                // Virtual resolutions are only applicable to the primary               
                // Add virtual resolutions from hardcoded gVirtualResTable (table is in portait.  Switch if the panel is landscape)
                if (pDisplayModes->aModeList[uPhysicalResolution].uWidth >= pDisplayModes->aModeList[uPhysicalResolution].uHeight)
                {
                  sResolution.uWidth  = gVirtualResTable[uVirtualModeIndex].uWidth;
                  sResolution.uHeight = gVirtualResTable[uVirtualModeIndex].uHeight;
                }
                else
                {
                  sResolution.uWidth  = gVirtualResTable[uVirtualModeIndex].uHeight;
                  sResolution.uHeight = gVirtualResTable[uVirtualModeIndex].uWidth;
                }

                // Clamp to ensure virtual resolution is not larger than native, required because only upscaled modes are supported.
                sResolution = ClampResolution(sResolution.uWidth, 
                                              sResolution.uHeight, 
                                              pDisplayModes->aModeList[uPhysicalResolution].uWidth, 
                                              pDisplayModes->aModeList[uPhysicalResolution].uHeight);
                
                uVirtualModeIndex++;
           }
           else
           {
              // No more resolutions or virtual resolutions to add
              break;
           }
         
           // Populate standard fields that are not resolution dependent
           pMode->Version                              = GRAPHICS_OUTPUT_PROTOCOL_REVISION;
           pMode->PixelFormat                          = DISPLAYDXE_DEFAULT_PIXEL_FORMAT;
           pMode->PixelInformation.RedMask             = DISPLAYDXE_RED_MASK;
           pMode->PixelInformation.GreenMask           = DISPLAYDXE_GREEN_MASK;
           pMode->PixelInformation.BlueMask            = DISPLAYDXE_BLUE_MASK;
           pMode->PixelInformation.ReservedMask        = DISPLAYDXE_ALPHA_MASK;

           // Ensure we clamp the resolution to not exceed the limits set by the resolution limit PCD
           // Note: After clamping we don't check if the same resolution is in the list already.
           sResolution = ClampResolution(sResolution.uWidth, sResolution.uHeight, gFBResLimit[uResLimitIndex].uWidth, gFBResLimit[uResLimitIndex].uHeight);
           
           pMode->HorizontalResolution                 = sResolution.uWidth;
           pMode->VerticalResolution                   = sResolution.uHeight;
           pMode->PixelsPerScanLine                    = pMode->HorizontalResolution;
           gDxeModeInfo.uNumModes[eDisplayId]++;
           uModeIndex++;
         }
      }
   }

   return Status;
}

/**********************************************************************************************************************
     Local Functions
**/


/**
  ClampResolution
  
  Clamps a resolution to a specific limit.  Handles cases where input & clamp are in different orientation (porait vs landscape)
    
  @param  uSourceWidth          Source width
  @param  uSourceHeight         Source height
  @param  uClampWidth           Clamp width
  @param  uClampHeight          Clamp height
 
@retval MDP_ResolutionType      Clamped resolution

**/
static MDP_ResolutionType ClampResolution(UINT32 uSourceWidth, UINT32 uSourceHeight, UINT32 uClampWidth, UINT32 uClampHeight)
{
    MDP_ResolutionType sResult;
    
    // Default is to return the input source width and height
    sResult.uWidth  = uSourceWidth;
    sResult.uHeight = uSourceHeight;    
       
    // Limit by total number of pixels
    if ((uSourceWidth * uSourceHeight) > (uClampWidth * uClampHeight)) 
    {
       if (uSourceHeight > uSourceWidth)
       {
         // Portrait mode panel, clamp to height x width
         sResult.uHeight = uClampWidth;  
         sResult.uWidth  = uClampHeight;
       }
       else
       {
         // Landscape mode panel, clamp to width x height
         sResult.uWidth  = uClampWidth;  
         sResult.uHeight = uClampHeight;
       }
    }
    
    return sResult;
}

/**
  Display DXE Internal Blit function
  
  @param  pSrc              Pointer (non NULL) to source buffer.
  @param  pdst              Pointer (non NULL) to destination buffer.                        

  @param  uSrcX             X coordinate of source.
  @param  uSrcY             Y coordinate of source.
  @param  uSrcWidth         Width of source rectangle in pixels.
  @param  uSrcHeight        Height of source rectangle in pixels.
  @param  uSrcStride        Stride of source rectangle in bytes.
  
  @param  uDstX             X coordinate of destination.
  @param  uDstY             Y coordinate of destination.
  @param  uDstStride        Stride of destination rectangle in bytes.

  @param  uBytesPerPixel    Number of bytes per pixel. 
**/

static void DisplayDxeBltInternal (
  UINT8   *pSrc,
  UINT8   *pDst,
  UINTN    uSrcX,
  UINTN    uSrcY,
  UINTN    uSrcWidth,
  UINTN    uSrcHeight,
  UINTN    uSrcStride,
  UINTN    uDstX,
  UINTN    uDstY,
  UINTN    uDstStride,
  UINTN    uBytesPerPixel
  )
{
  UINT32 uI = 0;
  UINT32 uSrcWidthBytes = uSrcWidth * uBytesPerPixel;  
  
  /* move src pointer to start of src rectangle */
  pSrc += (uSrcY * uSrcStride) + (uSrcX * uBytesPerPixel);

  /* move dest pointer to start of dest rectangle */
  pDst += (uDstY * uDstStride) + (uDstX * uBytesPerPixel); 

  /* Blit Operation 
   *
   *  We use MDP_OSAL_MEMCPY which invokes Neon memcpy (kr_memcpy.asm) 
   *  This memcpy supports overlapped src and dst buffers but copying may not be optimal in the overlap case 
   */  
  for (uI = 0; uI < uSrcHeight; ++uI)
  {
    MDP_OSAL_MEMCPY((void*)pDst, (void*)pSrc, uSrcWidthBytes);

    pDst += uDstStride;
    pSrc += uSrcStride;
  }
}

/**
GetChargerPowerStatus

Check charger power status using charger protocol.

@param  None

@retval EFI_SUCCESS       Charger power status is okay or no charger policy used for this platform
@retval other             Charger power status is not okay or at off mode charging

**/
static EFI_STATUS GetChargerPowerStatus(void)
{
  EFI_STATUS                eStatus            = EFI_SUCCESS;
  BOOLEAN                   bOffModeCharging   = FALSE;
  EFI_CHARGER_EX_PROTOCOL   *ChargerExProtocol = NULL;
  EFI_CHARGER_EX_FLASH_INFO sPowerTypeInfo     = {0};

  if (0 == PcdGet32(PcdDisplayChargerPolicy))
  {
    // Charger policy is not enforced on this platform
    eStatus = EFI_SUCCESS;
  }
  else
  {
    eStatus = gBS->LocateProtocol(&gChargerExProtocolGuid,
                                  NULL,
                                  (void**)&ChargerExProtocol);

    if ((EFI_SUCCESS != eStatus)                                                 ||
        (EFI_SUCCESS != ChargerExProtocol->IsOffModeCharging(&bOffModeCharging)) ||
        (EFI_SUCCESS != ChargerExProtocol->IsPowerOk(EFI_CHARGER_EX_POWER_FLASH_BATTERY_VOLTAGE_TYPE, &sPowerTypeInfo)))
    {
      // Unable to locate charger protocol, just continue with power okay status
      eStatus = EFI_SUCCESS;
    }
    else if((TRUE  == bOffModeCharging) ||
            (FALSE == sPowerTypeInfo.bCanFlash))
    {
      // Charger power status is not ok or at off mode charging
      eStatus = EFI_OUT_OF_RESOURCES;
    }
    else
    {
      // Charger power status is ok
      eStatus = EFI_SUCCESS;
    }
  }
  return eStatus;
}

/*
  Helper function to initialize the gSupportedDisplayList
**/
static bool32* DisplaySupportTableInit()
{
  uint32 uSupportedDisplays =  PcdGet32(PcdSupportedDisplays);

 if (uSupportedDisplays > 0)
 {
   MDP_Display_IDType eDisplayId = MDP_DISPLAY_PRIMARY;

   for (eDisplayId = 0; eDisplayId < MDP_DISPLAY_MAX; eDisplayId++)
   {
     if (uSupportedDisplays & (1 << eDisplayId))
     {
       gSupportedDisplayList[eDisplayId] = TRUE;
     }
     else
     {
       gSupportedDisplayList[eDisplayId] = FALSE;
     }
   }
 }

 return &gSupportedDisplayList[0];
}
