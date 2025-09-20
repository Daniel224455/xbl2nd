/*=============================================================================
Copyright (c) 2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/
#include <Library/UefiCfgLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Library/QcomBaseLib.h>
#include <Library/QcomLib.h>
#include "Protocol/EFIDisplayUtils.h"
#include "DisplayDxe.h"
#include "DisplayMultiCore.h"
#include "MDPLib.h"
#include "MDPSystem.h"
#include "DisplayUtils.h"

/**********************************************************************************************************************
 **   Macros/Defines
 **/

#define DISPLAY_STACK_SIZE       (DEFAULT_STACK_SIZE * 15)
/**********************************************************************************************************************
    Static/Globals
**/

/* Function table pointer to the supported protocol functions */
EFI_GRAPHICS_OUTPUT_PROTOCOL gDisplayMultiCoreOutputProtocol = {
    &DisplayMCDxe_QueryMode,
    &DisplayMCDxe_SetMode,
    &DisplayDxe_Blt,
    &gDxeModeInfo.sProtocolInfo
};

/**********************************************************************************************************************
 **   Local function prototypes
 **/

/* Thread function to Initialize and set display mode */
static INT32 SetModeThread(void *sArgs);

/**********************************************************************************************************************
     Public APIs
**/

/**
DisplayMCDxe_DisplayInitialize

Install Display Dxe protocols and create thread

  @param  None

  @retval EFI_SUCCESS           Protocol registered
  @retval EFI_UNSUPPORTED       Error during protocol registration or thread create

**/
EFI_STATUS DisplayMCDxe_DisplayInitialize(void)
{
  EFI_STATUS              eStatus            = EFI_SUCCESS;
  EFI_HANDLE              hUEFIDisplayHandle = NULL;
  EFI_GUID                sOutputGUID        = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

  // Default to dummy mode
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
    
  //Update ABL with selected panel info
  Display_Utils_SetPanelConfiguration(gSupportedDisplayList);

  /* Update DisplayUtils Protocol version as 0x20000
   * DisplayUtils protocol 0x20000 or above supports Setvariable & Getvariable
   */
  gQcomDisplayUtilsProtocolImplementation.Revision = 0x20000;

  // Install display protocols
  if (EFI_SUCCESS != (eStatus = gBS->InstallMultipleProtocolInterfaces (&hUEFIDisplayHandle,
                                                                        &gEfiDevicePathProtocolGuid,
                                                                        &DisplayDevicePath,
                                                                        &sOutputGUID,
                                                                        &gDisplayMultiCoreOutputProtocol,
                                                                        &gQcomDisplayPwrCtrlProtocolGuid,
                                                                        &gDisplayPwrCtrlProtocolImplementation,
                                                                        &gEfiDisplayPowerStateProtocolGuid,
                                                                        &gDisplayPwrProtocolImplementation,
                                                                        &gQcomDisplayUtilsProtocolGuid,
                                                                        &gQcomDisplayUtilsProtocolImplementation,
                                                                        NULL)))
  {
    DEBUG ((EFI_D_INFO, "DisplayDxe MultiCore: InstallMultipleProtocolInterfaces failed!\n"));
  }

  if (eStatus == EFI_SUCCESS)
  {
    gDxeModeInfo.uDedicatedThreadStatus = 1;

    // Spawn off thread to initialize and mode set displays
    if (NULL != (gDxeModeInfo.hModeLock = InitLock ("DispInit")))
    {
      if (NULL != (gDxeModeInfo.hDispThread = ThreadCreate ("Disp", &SetModeThread, 0, UEFI_THREAD_PRIORITY + 1, DISPLAY_STACK_SIZE)))
      {  
        if(MpcoreIsCpuActive(gDxeModeInfo.uDedicatedCore))
        {
          ThreadSetPinnedCpu(gDxeModeInfo.hDispThread, gDxeModeInfo.uDedicatedCore);
          DEBUG ((EFI_D_ERROR, "DisplayDxe MultiCore: Set mode thread execution on core %d\n", gDxeModeInfo.uDedicatedCore));
        }
        else
        {
          DEBUG ((EFI_D_ERROR, "DisplayDxe MultiCore: Cpu core [%d] is not active\n", gDxeModeInfo.uDedicatedCore));
        }

        if (MDP_STATUS_OK != MDP_SetMultiThreadState(TRUE))
        {
          DEBUG ((EFI_D_WARN, "DisplayDxe MultiCore: Failed to set multiThread state to be enabled!\n"));    
          gDxeModeInfo.uDedicatedThreadStatus = 0;
          eStatus = EFI_UNSUPPORTED;
        }
        else
        {
          ThreadDetach (gDxeModeInfo.hDispThread);
          ThreadResume (gDxeModeInfo.hDispThread);
        }
      }
      else /* ThreadCreate*/
      {
        DEBUG ((EFI_D_ERROR, "DisplayDxe MultiCore: Failed to create thread!\n"));
        gDxeModeInfo.uDedicatedThreadStatus = 0;
        eStatus = EFI_UNSUPPORTED;
      }
    }
    else /* InitLock */
    {
      DEBUG ((EFI_D_ERROR, "DisplayDxe MultiCore: Failed to initialize lock!\n"));
      gDxeModeInfo.uDedicatedThreadStatus = 0;
      eStatus = EFI_UNSUPPORTED;
    }
  }

  return eStatus;
}

/**
  Helper function to read the Display Dxe execution mode
**/
EFI_STATUS DisplayMCDxe_ReadMultiCoreInfo(void)
{
  EFI_STATUS              eStatus            = EFI_SUCCESS;

  if (EFI_SUCCESS != GetConfigValue ("DisplayDedicatedCoreExecution", &gDxeModeInfo.uDedicatedCoreEnabled))
  {
    gDxeModeInfo.uDedicatedCoreEnabled = 0;
    eStatus = EFI_NOT_FOUND;
  }
  else if (gDxeModeInfo.uDedicatedCoreEnabled == 1)
  {
    if (EFI_SUCCESS != GetConfigValue ("DisplayDedicatedCore", &gDxeModeInfo.uDedicatedCore))
    {
      gDxeModeInfo.uDedicatedCore        = 0;
      gDxeModeInfo.uDedicatedCoreEnabled = 0;
      eStatus                            = EFI_NOT_FOUND;
    }
  }

  return eStatus;
}

/**
DisplayMCDxe_SetMode

EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE Handler

  @param  This                  pointer to the DisplayMCDxe protocol function
  @param  ModeNumber            Mode index

  @retval EFI_SUCCESS           return success, As set mode is done internally

**/
EFI_STATUS
DisplayMCDxe_SetMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  UINT32                       ModeNumber)
{
  EFI_STATUS eStatus    = EFI_SUCCESS;

  // Mode set will be done in separate thread independent to UEFI framework
  eStatus = EFI_SUCCESS;

  return eStatus;
}

/**
DisplayMCDxe_QueryMode

EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE Handler

  @param  This                  pointer to the DisplayMCDxe protocol function
  @param  ModeNumber            Mode index
  @param  SizeOfInfo            size of mode information
  @param  Info                  Mode information

  @retval EFI_UNSUPPORTED       return unsupported, As set mode handling is done internally

**/
EFI_STATUS
DisplayMCDxe_QueryMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  UINT32                                ModeNumber,
  UINTN                                 *SizeOfInfo,
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info)
{
  EFI_STATUS eStatus   = EFI_SUCCESS;

  // To Do: Report modes to UEFI framework
  eStatus = EFI_UNSUPPORTED;

  return eStatus;
}


/**********************************************************************************************************************
     Local Functions
**/

/**
  Thread function for display init and mode set

  @param  Args              Optional Args

**/
static INT32 SetModeThread(void *sArgs)
{
  EFI_STATUS              eStatus    = EFI_SUCCESS;
  MDP_InitParamsType      sInitParam;

  /*
   * ABL needs to wait for the completion of display thread.
   * Acquire the lock here and unlock after completion.
   */
  Lock(gDxeModeInfo.hModeLock);
  gDxeModeInfo.uDedicatedThreadStatus = 1;

  MDP_OSAL_MEMZERO(&sInitParam,      sizeof(MDP_InitParamsType));

  MDP_OSAL_MEMCPY(&sInitParam.aSupportedDisplays, &gSupportedDisplayList, sizeof(sInitParam.aSupportedDisplays));

  if (MDP_STATUS_OK != MDPInit(&sInitParam, MDP_INIT_FLAG_MMU_INIT))
  {
    DEBUG ((EFI_D_ERROR, "DisplayDxe MultiCore: MDP init failed!\n"));
    eStatus = EFI_DEVICE_ERROR;
  }
  else
  {
    MDP_Display_IDType      eDisplayId = MDP_DISPLAY_PRIMARY;
    MDP_PowerParamsType     sPowerParams;
    MDP_PropertiesParamType sDisplayProp;
    MDP_DetectParamType     sDetectParams;

    MDP_OSAL_MEMZERO(&sPowerParams,  sizeof(MDP_PowerParamsType));
    MDP_OSAL_MEMZERO(&sDisplayProp,  sizeof(MDP_PropertiesParamType));
    MDP_OSAL_MEMZERO(&sDetectParams, sizeof(MDP_DetectParamType));

    sPowerParams.bPowerOn         = TRUE;
    sDisplayProp.bDisplayPwrState = TRUE;

    // Display Initialization
    for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId < MDP_DISPLAY_MAX; eDisplayId++)
    {
      if (FALSE == gSupportedDisplayList[eDisplayId])
      {
        continue;
      }
      else if (MDP_STATUS_OK != MDPPower(eDisplayId, &sPowerParams, 0x0))
      {
        DEBUG ((EFI_D_WARN, "DisplayDxe MultiCore: display %d panel power up failed!\n", eDisplayId));
      }
      else if (MDP_STATUS_OK != MDPDetect(eDisplayId, &sDetectParams, 0x0))
      {
        eStatus = EFI_DEVICE_ERROR;
      }
      else if (TRUE == sDetectParams.bDisplayDetected)
      {
        // Add/filter any new modes according to resolution table and max resolution table.
        if (MDP_STATUS_OK != DisplayDxe_FilterMode(eDisplayId, &sDetectParams, 0x0))
        {
           DEBUG ((EFI_D_WARN, "DisplayDxe MultiCore: Failed to add/filter new DisplayDxe modes!\n"));
        }
          
        // Set the display power state to on 
        if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp))
        {
          eStatus = EFI_DEVICE_ERROR; 
        }
      }
    }

    // Display Set Mode
    if (eStatus != EFI_SUCCESS)
    {
      DEBUG ((EFI_D_WARN, "DisplayDxe MultiCore: Initilization failed!\n"));
    }
    else
    {
      MemRegionInfo sMemRegionInfo;

      if (EFI_SUCCESS != (eStatus = GetMemRegionInfoByName("Display Reserved", &sMemRegionInfo)))
      {
        // Failed to get memory region info
        eStatus = EFI_DEVICE_ERROR;
      }
      else 
      {
        UINT32                                ModeNumber       = 0;  //default modenumber is '0'
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pMode            = NULL;
        UINTN                                 uFrameBufferSize = 0;
        UINTN                                 uMemBase;
        VOID                                 *pFrameBufferBase;
        MDP_SetModeParamType                  sModeParams;
        MDP_Display_IDType                    eDisplayId = MDP_DISPLAY_PRIMARY;
        MDP_Status                            eMDPStatus[MDP_DISPLAY_MAX];
        MDP_PropertiesParamType               sDisplayProp;
        RenderLogoParamsType                  sRenderLogoParams;

        MDP_OSAL_MEMZERO(&eMDPStatus,   sizeof(eMDPStatus));
        MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));
        MDP_OSAL_MEMZERO(&sRenderLogoParams, sizeof(RenderLogoParamsType));


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
              DEBUG ((EFI_D_ERROR, "DisplayDxe MultiCore: MDPSetMode() failed for display %d!\n", eDisplayId));
            }
          }
        }

        /* Assign the default brightness */
        sDisplayProp.uBacklightLevel = MDP_DISPLAY_DEFAULT_BRIGHTNESS;

        for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId <= MDP_DISPLAY_EXTERNAL; eDisplayId++)
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

        // Render Splash logo
        sRenderLogoParams.uXPosition  = 0;
        sRenderLogoParams.uYPosition  = 0;
        sRenderLogoParams.uFlags      = 0;

        if (EFI_SUCCESS != (eStatus = Display_Utils_RenderSplashScreen(NULL, &sRenderLogoParams)))
        {
          DEBUG((EFI_D_WARN, "DisplayDxe MultiCore: Render Splash Screen failed with Status(%d)!\n", eStatus));
        }
      }
    }
  }

  /*
   * release the lock so that ABL can lock and unlock this lock
   * to know if thread finished execution.
   */
  UnLock(gDxeModeInfo.hModeLock);

  // Done, disable multi-threadding
  gDxeModeInfo.uDedicatedCoreEnabled = 0;
  gDxeModeInfo.uDedicatedThreadStatus = 0;

  if (MDP_STATUS_OK != MDP_SetMultiThreadState(FALSE))
  {
    DEBUG ((EFI_D_WARN, "DisplayDxe MultiCore: Failed to set multiThread state to be disabled!\n"));    
  }

  return 0;
}
