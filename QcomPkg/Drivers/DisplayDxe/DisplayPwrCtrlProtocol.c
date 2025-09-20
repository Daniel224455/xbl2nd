/*=============================================================================
 
  File: DisplayPwrCtrlProtocol.c
 
  UEFI display power control protocol 
  
 
    Copyright (c) 2011-2018 Qualcomm Technologies, Inc.
    All Rights Reserved
    Qualcomm Confidential and Proprietary
=============================================================================*/

#include <PiDxe.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/QcomLib.h>
#include <Protocol/EFIPlatformInfo.h>
#include "Protocol/EFIDisplayPwrCtrl.h"
#include <Pi/PiStatusCode.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Protocol/StatusCode.h>
#include <Library/ReportStatusCodeLib.h>
#include <Lpm.h>
#include "MDPLib.h"
#include "MDPPlatformLib.h"
#include "DisplayDxe.h"
#include "MDPSystem.h"

/**********************************************************************************************************************
    Static/Globals
**/

static 
EFI_STATUS 
DisplayPanelPwrCtrlOp
(
  IN  EFI_DISPLAY_TYPE     eDisplayType,
  IN  BOOLEAN              bDisplayState
);

static 
EFI_STATUS 
DisplayPanelSetMode
(
  IN  MDP_Display_IDType   eDisplayId
);

static
BOOLEAN
IsDisplayIdSupported
(
  IN  EFI_DISPLAY_TYPE     eDisplayType,
  IN  MDP_Display_IDType   eDisplayId
);

static
MDP_Display_IDType
GetFirstAvailableDisplayId
(
  IN  EFI_DISPLAY_TYPE     eDisplayType
);

static EFI_STATUS EFIAPI DisplayLPMCB (
    IN EFI_STATUS_CODE_TYPE     CodeType,
    IN EFI_STATUS_CODE_VALUE    Value,
    IN UINT32                   Instance,
    IN EFI_GUID                 *CallerId,
    IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
    );


/**********************************************************************************************************************
    Public APIs
**/


/**
  Sets the display panel power state 

  @param[IN] DisplayType   : PRIMARY/EXTERNAL/ALL
  @param[IN] DisplayState  : ON/OFF

  @return
  EFI_SUCCESS              : Function returned successfully.
  EFI_INVALID_PARAMETER    : Parameter passed is incorrect.  
  EFI_UNSUPPORTED          : Parameter passed is not supported. 
  EFI_DEVICE_ERROR         : Physical device reported an error.

*/
EFI_STATUS
EFIAPI
EFI_DisplayPanelPowerControl
(
  IN EFI_DISPLAY_TYPE                DisplayType,
  IN EFI_DISPLAY_POWER_CTRL_STATE    DisplayState
)
{
  EFI_STATUS   eStatus   = EFI_SUCCESS;

  /* Display type is supported */
  if (DisplayType >= EFI_DISPLAY_TYPE_MAX)
  {
    eStatus = EFI_INVALID_PARAMETER; 
  }
  /* Display power state is supported */ 
  else if (DisplayState > EFI_DISPLAY_POWER_STATE_ON)
  {
    eStatus = EFI_INVALID_PARAMETER; 
  }
  else if (MDP_STATUS_OK != MDPAcquireLockOrFail(MDP_LOCK_TYPE_DISPLAY_PWR_CTRL))
  {
    /* Fail this call as an instance of the function is currently running */
    DEBUG((EFI_D_ERROR, "DisplayPanelPowerControl: Failed as concurrency is not supported!\n"));
    eStatus = EFI_UNSUPPORTED;
  }
  else 
  {
    BOOLEAN                  bDisplayState;
    BOOLEAN                  bPanelDetected;
    BOOLEAN                  bPanelPwrState;
    MDP_Display_IDType       eDisplayId = MDP_DISPLAY_PRIMARY;
    MDP_PropertiesParamType  sDisplayProp;    

    /* Assign the display state */
    bDisplayState = (EFI_DISPLAY_POWER_STATE_ON == DisplayState) ? TRUE : FALSE;

    /* Get the available display to detect power state */
    eDisplayId = GetFirstAvailableDisplayId(DisplayType);

    /* Get the panel detection info */
    MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));
    MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_DETECTION_INFO, &sDisplayProp);
    bPanelDetected = sDisplayProp.bDisplayDetected;

    /* Get the panel power state info */
    MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));
    MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp);
    bPanelPwrState = sDisplayProp.bDisplayPwrState;

    /* Panel detected */
    if (TRUE == bPanelDetected)
    {
      /* Display state has changed */
      if (bDisplayState != bPanelPwrState)
      {
        eStatus = DisplayPanelPwrCtrlOp(DisplayType, bDisplayState);
      }
    }

    if ((EFI_SUCCESS                 == eStatus)  &&
        (EFI_DISPLAY_POWER_STATE_OFF == DisplayState))
    {
      if (MDP_STATUS_OK != MDPTerm(0x0))
      {
        eStatus = EFI_DEVICE_ERROR; 
      }
    }

    MDPReleaseLock(MDP_LOCK_TYPE_DISPLAY_PWR_CTRL); 
  }

  return eStatus;
}


/**
  Retrieves the display panel power state 

  @param[IN] DisplayType    : PRIMARY/EXTERNAL
  @param[OUT] DisplayState  : ON/OFF

  @return
  EFI_SUCCESS               : Function returned successfully.
  EFI_UNSUPPORTED           : Parameter passed is not supported. 
  EFI_DEVICE_ERROR          : Physical device reported an error.

*/
EFI_STATUS
EFIAPI
EFI_DisplayPanelPowerStatus
(
  IN  EFI_DISPLAY_TYPE                DisplayType,
  OUT EFI_DISPLAY_POWER_CTRL_STATE   *DisplayState
)
{
  EFI_STATUS               eStatus  = EFI_SUCCESS; 
  
  /* Display type is supported */
  if (DisplayType >= EFI_DISPLAY_TYPE_MAX)
  {
    eStatus = EFI_UNSUPPORTED; 
  }
  else 
  {
    MDP_Display_IDType       eDisplayId;
    MDP_PropertiesParamType  sDisplayProp;
         
    MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));

    /* Get the available display to detect power state */
    eDisplayId = GetFirstAvailableDisplayId(DisplayType);

    /* Get the power state for the primary/external display */
    if (MDP_STATUS_OK != MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp))
    {
      eStatus = EFI_DEVICE_ERROR; 
    }
    else 
    {
      *DisplayState = (TRUE == sDisplayProp.bDisplayPwrState) ? EFI_DISPLAY_POWER_STATE_ON : EFI_DISPLAY_POWER_STATE_OFF;
    }
  }

  return eStatus;
}


/**
  Sets the display backlight brightness level 

  @param[IN] DisplayType       : PRIMARY/SECONDARY/EXTERNAL
  @param[IN] BrightnessLevel   : 0% to 100%

  @return
  EFI_SUCCESS              : Function returned successfully.
  EFI_INVALID_PARAMETER    : Parameter passed is incorrect.  
  EFI_UNSUPPORTED          : Parameter passed is not supported. 
  EFI_DEVICE_ERROR         : Physical device reported an error.

*/
EFI_STATUS
EFIAPI
EFI_DisplayBackLightBrightnessLevelControl
(
  IN EFI_DISPLAY_TYPE     DisplayType,
  IN UINT32               BrightnessLevel
)
{
  EFI_STATUS               eStatus  = EFI_SUCCESS; 
  
  /* Display type is supported */
  if (DisplayType >= EFI_DISPLAY_TYPE_EXTERNAL)
  {
    eStatus = EFI_UNSUPPORTED; 
  }
  /* Brightness level is within the max range */
  else if (BrightnessLevel > DISPLAY_MAX_BACKLIGHT_LEVEL)
  {
    eStatus = EFI_INVALID_PARAMETER;    
  }
  else if (MDP_STATUS_OK != MDPAcquireLockOrFail(MDP_LOCK_TYPE_DISPLAY_PWR_CTRL))
  {     
    /* Fail this call as an instance of the function is currently running */
    DEBUG((EFI_D_ERROR, "DisplayBackLightBrightnessLevelControl: Failed as concurrency is not supported!\n"));
    eStatus = EFI_UNSUPPORTED;
  }
  else 
  {
    MDP_Display_IDType       eDisplayId;
    MDP_PropertiesParamType  sDisplayProp;

    for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId <= MDP_DISPLAY_SECONDARY; eDisplayId++)
    {
      if (FALSE == IsDisplayIdSupported(DisplayType, eDisplayId))
      {
        continue;
      }
      else
      {
        MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));

        /* Assign the backlight brightness level */
        sDisplayProp.uBacklightLevel = BrightnessLevel;

        /* Adjust the panel backlight brightness level */      
        if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_BACKLIGHT, &sDisplayProp))
        {
          eStatus = EFI_DEVICE_ERROR; 
        }
      }
    }

    MDPReleaseLock(MDP_LOCK_TYPE_DISPLAY_PWR_CTRL);    
  }

  return eStatus;
}


/**
  Retrieves the display backlight brightness level  

  @param[IN]   DisplayType        : PRIMARY/EXTERNAL
  @@param[OUT] BrightnessLevel    : 0% to 100%

  @return
  EFI_SUCCESS                : Function returned successfully.  
  EFI_UNSUPPORTED            : Parameter passed is not supported. 
  EFI_DEVICE_ERROR           : Physical device reported an error.

*/
EFI_STATUS
EFIAPI
EFI_DisplayBackLightBrightnessLevelStatus
(
  IN  EFI_DISPLAY_TYPE     DisplayType,
  OUT UINT32              *BrightnessLevel
)
{
  EFI_STATUS               eStatus  = EFI_SUCCESS; 

  /* Display type is supported */
  if (DisplayType >= EFI_DISPLAY_TYPE_EXTERNAL)
  {
    eStatus = EFI_UNSUPPORTED; 
  }
  else 
  {
    MDP_Display_IDType       eDisplayId;
    MDP_PropertiesParamType  sDisplayProp;
         
    MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));

    /* Assign the MDP display ID */
    eDisplayId = GetFirstAvailableDisplayId(DisplayType);

    /* Get the backlight for the display */
    if (MDP_STATUS_OK != MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_BACKLIGHT, &sDisplayProp))
    {
      eStatus = EFI_DEVICE_ERROR; 
    }
    else 
    {
      /* Assign the backlight level */ 
      *BrightnessLevel = sDisplayProp.uBacklightLevel;
    }  
  }

  return eStatus;
}


/**
  Sets the display panel power state 

  @param[IN] DisplayType   : PRIMARY/EXTERNAL
  @param[IN] DisplayState  : TRUE/FALSE

  @return
  EFI_SUCCESS              : Function returned successfully.
  EFI_DEVICE_ERROR         : Physical device reported an error.

*/
static 
EFI_STATUS 
DisplayPanelPwrCtrlOp
(
  IN  EFI_DISPLAY_TYPE     eDisplayType,
  IN  BOOLEAN              bDisplayState
)
{
  EFI_STATUS               eStatus  = EFI_SUCCESS;
  MDP_PowerParamsType      sPowerParams; 
  MDP_PropertiesParamType  sDisplayProp;            
  MDP_Display_IDType       eDisplayId = MDP_DISPLAY_PRIMARY;

  MDP_OSAL_MEMZERO(&sPowerParams, sizeof(MDP_PowerParamsType));        
  MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));    

  /* Assign the display power state */
  sPowerParams.bPowerOn = bDisplayState;

  /* Display power state is set to on */
  if (TRUE == sPowerParams.bPowerOn)
  {
    MDP_DetectParamType  sDetectParams; 
    MDP_InitParamsType   sInitParam;
    
    MDP_OSAL_MEMZERO(&sDetectParams, sizeof(MDP_DetectParamType));
    MDP_OSAL_MEMZERO(&sInitParam, sizeof(MDP_InitParamsType));
    MDP_OSAL_MEMCPY(&sInitParam.aSupportedDisplays, &gSupportedDisplayList, sizeof(sInitParam.aSupportedDisplays));

    /* MDSS configuration needs to be restored as it can lose its
       configuration if there is power collapse.*/
    if (MDP_STATUS_OK != MDPInit(&sInitParam, MDP_INIT_FLAG_MINIMAL_INIT))
    {
      eStatus = EFI_DEVICE_ERROR; 
    }
    else
    {
      for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId <= MDP_DISPLAY_EXTERNAL; eDisplayId++)
      {
        if (FALSE == IsDisplayIdSupported(eDisplayType, eDisplayId))
        {
          continue;
        }
        else
        {
          MDP_OSAL_MEMZERO(&sDetectParams, sizeof(MDP_DetectParamType));
          sPowerParams.bPowerOn = TRUE;

          /* Request the MDP hardware to power on the display */
          if (MDP_STATUS_OK != MDPPower(eDisplayId, &sPowerParams, 0x0))
          {
            DEBUG ((EFI_D_WARN, "Common display panel power up failed!\n"));
            eStatus = EFI_DEVICE_ERROR; 
          }
          /* Detect the presence of a display */
          else if (MDP_STATUS_OK != MDPDetect(eDisplayId, &sDetectParams, 0x0))
          {
            eStatus = EFI_DEVICE_ERROR;
          }
          else if (MDP_STATUS_OK != MDPPower(eDisplayId, &sPowerParams, POWERCONFIG_FLAGS_PANEL_POWER))
          {
            DEBUG ((EFI_D_WARN, "Specific display panel power up failed!\n")); 
            eStatus = EFI_DEVICE_ERROR; 
          }
          /* Display is not detected */
          else if (FALSE == sDetectParams.bDisplayDetected)
          {
            MDP_OSAL_MEMZERO(&sPowerParams, sizeof(MDP_PowerParamsType));          

            /* Power off the display that was turned on */
            sPowerParams.bPowerOn = FALSE;       

            /* Request the MDP hardware to power off the display */
            MDPPower(eDisplayId, &sPowerParams, 0x0);
            MDPPower(eDisplayId, &sPowerParams, POWERCONFIG_FLAGS_PANEL_POWER);

            /* Indicate that the operation was not successful */
            eStatus = EFI_DEVICE_ERROR;
          }
          else
          { 
            sDisplayProp.bDisplayPwrState = bDisplayState;

            /* Set the display panel power state */              
            if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp))
            {
              eStatus = EFI_DEVICE_ERROR; 
            } 

            /* Set the display mode that was cached */
            eStatus = DisplayPanelSetMode(eDisplayId);
          }
        }
      }
    }
  }
  /* Display power state is set to OFF */
  else
  {
    for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId <= MDP_DISPLAY_EXTERNAL; eDisplayId++)
    {
      if (FALSE == IsDisplayIdSupported(eDisplayType, eDisplayId))
      {
        continue;
      }
      else
      {
        /* De-initialize the interfaces */
        if(MDP_STATUS_OK != MDPDeInit(eDisplayId, 0x0))
        {
          eStatus = EFI_DEVICE_ERROR; 
        }
        /* Request the MDP hardware to power off the display */
        else if (MDP_STATUS_OK != MDPPower(eDisplayId, &sPowerParams, 0x0))
        {
          DEBUG ((EFI_D_WARN, "Common display panel power down failed!\n"));
          eStatus = EFI_DEVICE_ERROR; 
        }
        else if (MDP_STATUS_OK != MDPPower(eDisplayId, &sPowerParams, POWERCONFIG_FLAGS_PANEL_POWER))
        {
          DEBUG ((EFI_D_WARN, "Specific display panel power down failed!\n")); 
          eStatus = EFI_DEVICE_ERROR; 
        }
        else 
        {
          /* Set the display panel power state */
          sDisplayProp.bDisplayPwrState = bDisplayState;
          if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp))
          {
            eStatus = EFI_DEVICE_ERROR;
          }
        }
      }
    }  
  }

  return eStatus;
}


/**********************************************************************************************************************
    Helper function
**/

/**
  IsDisplayIdSupported
  Helper function to detect whether the display id is supported and required to get controlled.

  @param[IN] eDisplayType   : PRIMARY/SECONDARY/EXTERNAL/INTERNAL_ALL/ALL
  @param[IN] eDisplayId     : Display ID

  @return
  TRUE                      : The display is supported.
  FALSE                     : The display is not supported.  

*/
static
BOOLEAN
IsDisplayIdSupported
(
  IN EFI_DISPLAY_TYPE                eDisplayType,
  IN MDP_Display_IDType              eDisplayId
)
{
  BOOLEAN DisplaySupportFlag = FALSE;

  EFI_DISPLAY_TYPE aDisplayTypeMapping[MDP_DISPLAY_MAX] =
  {
    EFI_DISPLAY_TYPE_PRIMARY,
    EFI_DISPLAY_TYPE_SECONDARY,
    EFI_DISPLAY_TYPE_EXTERNAL
  };

  if (FALSE == MDP_DISPLAY_INTERNAL(eDisplayId))
  {
    DisplaySupportFlag = FALSE;
  }
  else if ((EFI_DISPLAY_TYPE_INTERNAL_ALL == eDisplayType) &&
           (MDP_DISPLAY_EXTERNAL          == eDisplayId))
  {
    /* If only need to control internal display, then skip the external operations */
    DisplaySupportFlag = FALSE;
  }
  else if ((EFI_DISPLAY_TYPE_INTERNAL_ALL   == eDisplayType) ||
           (EFI_DISPLAY_TYPE_ALL            == eDisplayType) ||
           (aDisplayTypeMapping[eDisplayId] == eDisplayType))
  {
    DisplaySupportFlag = TRUE;
  }

  return DisplaySupportFlag;
}


/**
  GetFirstAvailableDisplayId
  Helper function to get the first available display id.

  @param[IN] eDisplayType   : PRIMARY/SECONDARY/EXTERNAL/INTERNAL_ALL/ALL

  @return eDisplayId          Valid display id can be detected.

*/
static
MDP_Display_IDType
GetFirstAvailableDisplayId
(
  IN  EFI_DISPLAY_TYPE     eDisplayType
)
{
  EFI_STATUS         eStatus    = EFI_DEVICE_ERROR;
  MDP_Display_IDType eDisplayId = MDP_DISPLAY_PRIMARY;

  for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId <= MDP_DISPLAY_EXTERNAL; eDisplayId++)
  {
    if ((TRUE == MDP_DISPLAY_INTERNAL(eDisplayId)) &&
        (TRUE == IsDisplayIdSupported(eDisplayType, eDisplayId)))
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
  Sets the panel mode
  
  @param[IN] DisplayType     : PRIMARY/EXTERNAL

  @return
  EFI_SUCCESS                : Graphics mode specified was selected.
  EFI_DEVICE_ERROR           : Physical device reported an error.
  EFI_UNSUPPORTED            : ModeNumber is not supported by this device.
  EFI_INVALID_PARAMETER      : Parameter passed is incorrect.  
  EFI_OUT_OF_RESOURCES       : Memory allocation failed  
**/
static 
EFI_STATUS 
DisplayPanelSetMode
(
  IN  MDP_Display_IDType   eDisplayId
)
{
  MDP_SetModeParamType     sModeParams;
  MDP_PropertiesParamType  sDisplayProp;
  EFI_STATUS               eStatus    = EFI_SUCCESS;  

  MDP_OSAL_MEMZERO(&sModeParams, sizeof(MDP_SetModeParamType));
  MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));
    
  /* Display type is supported */
  if (eDisplayId >= MDP_DISPLAY_MAX)
  {
    eStatus = EFI_INVALID_PARAMETER; 
  }  
  /* Retrieve the cached mode information */
  else if (MDP_STATUS_OK != MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_MODE_INFO, &sDisplayProp))
  {
    eStatus = EFI_DEVICE_ERROR; 
  }  
  else 
  {
    sModeParams.uModeIndex = sDisplayProp.sModeParams.uModeIndex;
    MDP_OSAL_MEMCPY(&sModeParams.sSurfaceInfo, &sDisplayProp.sModeParams.sSurfaceInfo, sizeof(MDPSurfaceInfo));

    if (MDP_STATUS_OK != MDPSetMode(eDisplayId, &sModeParams, 0x0))
    {
      eStatus = EFI_DEVICE_ERROR;
    }
    else
    {
      MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));

      /* Assign the default brightness */
      sDisplayProp.uBacklightLevel = MDP_DISPLAY_DEFAULT_BRIGHTNESS;

      /* Set the default backlight level */
      if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_BACKLIGHT, &sDisplayProp))
      {
        eStatus = EFI_DEVICE_ERROR;
      }
      /* Populate the display environment information, used to inform the OS driver of firmware state */
      else if(MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_FIRMWAREENV, NULL))
      {
        eStatus = EFI_DEVICE_ERROR;
      }
    }
  }

  return eStatus;
}

/**
  Register for LPM support to get LPM entry/exit status in CB
  
  @return
  EFI_SUCCESS                : CB registered .
  EFI_DEVICE_ERROR           : device reported an error.
  EFI_UNSUPPORTED            : not supported by this device.
  EFI_INVALID_PARAMETER      : Parameter passed is incorrect.  
**/
EFI_STATUS DisplayPwr_InitLPMSupport(BOOLEAN InitLpm)
{
  EFI_STATUS                       Status              = EFI_SUCCESS;
  static EFI_RSC_HANDLER_PROTOCOL *mRscHandlerProtocol = NULL;

  if ( NULL == mRscHandlerProtocol)
  { 
    Status = gBS->LocateProtocol(
                    &gEfiRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &mRscHandlerProtocol
                    );
  }
  if((EFI_SUCCESS != Status) || (mRscHandlerProtocol == NULL))
  {
    DEBUG(( EFI_D_WARN, "DisplayDxe::InitLPMSupport Failure = %r \r\n",Status));
    return Status;
  }

  if(InitLpm)
  {
    Status |= mRscHandlerProtocol->Register(DisplayLPMCB, TPL_CALLBACK);
  }
  else
  {
    Status |= mRscHandlerProtocol->Unregister(DisplayLPMCB);
  }
  return Status;

}

/**
  LPM feature - Display CB 
  
  @param[IN] CodeType        :EFI_PROGRESS_CODE
  @param[IN] Value           :PcdLpm refer QcomPkg.dec for the value details
  @param[IN] Instance        : 0
  @param[IN] *CallerId       : Caller Id who initiated this CB 
  @param[IN] *Data OPTIONAL  : Extended data refer LPM_EVENT_EXT_DATA in lpm.h

  @return
  EFI_SUCCESS                : CB handled 
  EFI_UNSUPPORTED            : CB not intended for this module 
**/
EFI_STATUS EFIAPI DisplayLPMCB (
    IN EFI_STATUS_CODE_TYPE     CodeType,
    IN EFI_STATUS_CODE_VALUE    Value,
    IN UINT32                   Instance,
    IN EFI_GUID                 *CallerId,
    IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
    )
{

  EFI_STATUS          status       = EFI_UNSUPPORTED;
  BOOLEAN             bMatch       = FALSE;
  LPM_EVENT_EXT_DATA *LpmEventData = NULL;

  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) &&
      (Value == PcdGet32 (PcdLpm)) && 
      (Data != NULL))
  {
    /* Map LpmEventData */
    LpmEventData = (LPM_EVENT_EXT_DATA*)( Data + 1);

    bMatch = CompareGuid(&(LpmEventData->CalleeGuid), &gQcomDisplayPwrCtrlProtocolGuid);
    if(TRUE == bMatch)
    {
      switch (LpmEventData->LPMState)
      {
        case LPM_ENTRY:
          status = EFI_DisplayPanelPowerControl(EFI_DISPLAY_TYPE_ALL, EFI_DISPLAY_POWER_STATE_OFF);
          break;
        case LPM_EXIT:
          status = EFI_DisplayPanelPowerControl(EFI_DISPLAY_TYPE_ALL, EFI_DISPLAY_POWER_STATE_ON);
        break;
        default:
          DEBUG(( EFI_D_WARN, "Display_LowPowerModeHandler: Invalid LPM state %d \r\n", LpmEventData->LPMState));
        break;
      }
      /* Error log */
      if (status != EFI_SUCCESS)
      {
      DEBUG(( EFI_D_ERROR, "Display_LowPowerModeHandler: LPM state = %d Error %r\r\n", LpmEventData->LPMState, status));
      }
    }
  }
  return status;
}


/**
  Display power UEFI Protocol implementation
 */
EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL gDisplayPwrCtrlProtocolImplementation =
{
  DISPLAY_PWR_CTRL_REVISION,
  EFI_DisplayPanelPowerControl,
  EFI_DisplayPanelPowerStatus,
  EFI_DisplayBackLightBrightnessLevelControl,
  EFI_DisplayBackLightBrightnessLevelStatus,
}; 
