/*=============================================================================
 
  File: DisplayPwrCtrlProtocol.c
 
  UEFI display power control protocol 
  
  Copyright (c) 2011-2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc. 
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
#include "Protocol/EFIDisplayPwr.h"
#include "Protocol/EFIDisplayPwrCtrl.h"
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

/**********************************************************************************************************************
    Public APIs
**/


/** 
  Modifies the power state of the display and backlight.

  @param[in]  This         : Pointer to the EFI_DISPLAY_POWER_PROTOCOL interface                     
  @param[in] PowerState    : EfiDisplayPowerStateMaximum (ON) 
                           : EfiDisplayPowerStateOff (OFF)
  
  @return
  EFI_SUCCESS              : Function returned successfully.
  EFI_INVALID_PARAMETER    : Parameter passed is incorrect.  
  EFI_DEVICE_ERROR         : Physical device reported an error.
*/
EFI_STATUS
EFIAPI 
EFI_DisplayPowerSetDisplayPowerState
(
  IN EFI_DISPLAY_POWER_PROTOCOL       *This,
  IN EFI_DISPLAY_POWER_STATE           PowerState
)
{
  EFI_STATUS   eStatus   = EFI_SUCCESS;

  /* Power state is supported */
  if ((PowerState != EfiDisplayPowerStateMaximum) && (PowerState != EfiDisplayPowerStateOff))
  {
    eStatus = EFI_INVALID_PARAMETER; 
  } 
  else if (MDP_STATUS_OK != MDPAcquireLockOrFail(MDP_LOCK_TYPE_DISPLAY_PWR_CTRL))
  {
    /* Fail this call as an instance of the function is currently running */
    DEBUG((EFI_D_ERROR, "DisplayPowerSetDisplayPowerState: Failed as concurrency is not supported!\n"));
    eStatus = EFI_UNSUPPORTED;
  }
  else 
  {
    BOOLEAN                  bDisplayState;
    BOOLEAN                  bPanelDetected;
    BOOLEAN                  bPanelPwrState;
    MDP_Display_IDType       eDisplayId   = MDP_DISPLAY_PRIMARY;
    EFI_DISPLAY_TYPE         eDisplayType = EFI_DISPLAY_TYPE_ALL;
    MDP_PropertiesParamType  sDisplayProp;    
       
    /* Assign the display state */
    bDisplayState = (EfiDisplayPowerStateMaximum == PowerState) ? TRUE : FALSE;

    /* Get the available display to detect power state */
    eDisplayId = GetFirstAvailableDisplayId(eDisplayType);

    /* Get the panel detection info */
    MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));
    MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_DETECTION_INFO, &sDisplayProp);
    bPanelDetected = sDisplayProp.bDisplayDetected;

    /* Get the panel power state info */
    MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));
    MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp);
    bPanelPwrState = sDisplayProp.bDisplayPwrState;

    /* Panel detected - turn on/off the primary panel */      
    if (TRUE == bPanelDetected)
    {
      /* Display state has changed */
      if (bDisplayState != bPanelPwrState)
      {
        eStatus = DisplayPanelPwrCtrlOp(eDisplayType, bDisplayState);
      }
    }

    /* Make sure that all the displays are powered down inorder to de-initialize the MDP core and free all the resources */
    if ((EFI_SUCCESS             == eStatus)  &&
        (EfiDisplayPowerStateOff == PowerState))
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
  Retrieves the current power state of the display and backlight.

  @param[in]  This         : Pointer to the EFI_DISPLAY_POWER_PROTOCOL interface      
  @param[out] PowerState   : EfiDisplayPowerStateMaximum (ON)  
                           : EfiDisplayPowerStateOff (OFF)
  
  @return
  EFI_SUCCESS              : Function returned successfully.
  EFI_INVALID_PARAMETER    : Parameter passed is incorrect.  
  EFI_DEVICE_ERROR         : Physical device reported an error.
*/
EFI_STATUS
EFIAPI 
EFI_DisplayPowerGetDisplayPowerState
(
  IN EFI_DISPLAY_POWER_PROTOCOL       *This,
  OUT EFI_DISPLAY_POWER_STATE         *PowerState
)
{
  EFI_STATUS               eStatus      = EFI_SUCCESS; 
  MDP_PropertiesParamType  sDisplayProp;
  MDP_Display_IDType       eDisplayId   = MDP_DISPLAY_PRIMARY;
  EFI_DISPLAY_TYPE         eDisplayType = EFI_DISPLAY_TYPE_ALL;
         
  /* Get the available display to detect power state */
  eDisplayId = GetFirstAvailableDisplayId(eDisplayType);

  MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));

  /* Get the power state for the primary display */
  if (MDP_STATUS_OK != MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp))
  {
    eStatus = EFI_DEVICE_ERROR; 
  }
  else 
  {
    *PowerState = (TRUE == sDisplayProp.bDisplayPwrState) ? EfiDisplayPowerStateMaximum : EfiDisplayPowerStateOff;
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
  EFI_STATUS               eStatus    = EFI_SUCCESS;
  MDP_PowerParamsType      sPowerParams; 
  MDP_PropertiesParamType  sDisplayProp;
  MDP_Display_IDType       eDisplayId = MDP_DISPLAY_PRIMARY;
 
  MDP_OSAL_MEMZERO(&sPowerParams, sizeof(MDP_PowerParamsType));        
  MDP_OSAL_MEMZERO(&sDisplayProp, sizeof(MDP_PropertiesParamType));    

  /* Assign the display power state */
  sPowerParams.bPowerOn = sDisplayProp.bDisplayPwrState = bDisplayState;

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
            /* Set the display panel power state */              
            if (MDP_STATUS_OK != MDPSetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_POWER_STATE, &sDisplayProp))
            {
              eStatus = EFI_DEVICE_ERROR; 
            } 
            else 
            {
              /* Set the display mode that was cached */
              eStatus = DisplayPanelSetMode(eDisplayId);
            }
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
    if (TRUE == MDP_DISPLAY_INTERNAL(eDisplayId))
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
  
  /* Retrieve the cached mode information */
  if (MDP_STATUS_OK != MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_MODE_INFO, &sDisplayProp))
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
    else if ((MDP_DISPLAY_PRIMARY   == eDisplayId) ||
             (MDP_DISPLAY_SECONDARY == eDisplayId))
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
  Display power UEFI Protocol implementation
 */
EFI_DISPLAY_POWER_PROTOCOL gDisplayPwrProtocolImplementation =
{
  EFI_DISPLAY_POWER_PROTOCOL_REVISION,
  EFI_DisplayPowerSetDisplayPowerState,
  EFI_DisplayPowerGetDisplayPowerState 
}; 
