/*=============================================================================
 
  File: DisplayUtils.c
 
  Source file for MDP Utils functions

   
  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

/* Copyright (c) 2013-2016, The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*  * Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*  * Neither the name of The Linux Foundation nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

#include <Uefi.h>
#include <libfdt.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/RamPartitionTableLib.h>
#include <Protocol/EFIScm.h>
#include <Protocol/EFIDisplayUtils.h>
#include <Protocol/BootLogo.h>
#include <Include/scm_sip_interface.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BootConfig.h>
#include <IndustryStandard/Acpi50.h>
#include <IndustryStandard/Bmp.h>
#include <Library/GuidedFvLib.h>
#include <Library/LockLib.h>
#include "MDPLib.h"
#include "MDPLib_i.h"
#include "MDPSystem.h"
#include "MDPPlatformLib.h"
#include "DisplayUtils.h"
#include "displaydiag.h"
#include "HALDSILib.h"


/*=========================================================================
     Default Defines
==========================================================================*/

#define PANEL_LIST_STR_LEN_MAX                1024                   // maximum length for the panel list string
#define PANEL_CONFIG_STR_LEN_MAX              256                    // maximum length for panel configuration string

#define PANEL_CMD_MARKER_PRIM                 "prim:"                // Panel override: primary panel marker
#define PANEL_CMD_MARKER_PRIM_LEN             5                      //.Length

#define PANEL_CMD_MARKER_SEC                  "sec:"                 // Panel override: secondary panel marker
#define PANEL_CMD_MARKER_SEC_LEN              4                      // Length 

#define PANEL_CMD_MARKER_CONFIG               ":cfg"                 // Panel override: topology selection marker
#define PANEL_CMD_MARKER_CONFIG_LEN           4                      // Length

#define PANEL_CMD_MARKER_TIMING               ":timing"              // Panel override: timing selection marker
#define PANEL_CMD_MARKER_TIMING_LEN           7                      // Length

#define PANEL_CMD_MARKER_SKIP                 ":skip"                // Panel override: skip override in bootloader
#define PANEL_CMD_MARKER_DISABLE              ":disable"             // Panel override: disable continuous splash
#define PANEL_CMD_MARKER_SWAP                 ":swap"                // Panel override: swap DSI controllers
#define PANEL_CMD_MARKER_PLL0                 ":pll0"                // Panel override: use pll0
#define PANEL_CMD_MARKER_PLL1                 ":pll1"                // Panel override: use pll1
#define PANEL_CMD_MARKER_SPLASH_OFF           ":cs0"                 // Panel override: SEAMLESS_SPLASH OFF
#define PANEL_CMD_MARKER_SPLASH_ON            ":cs1"                 // Panel override: SEAMLESS_SPLASH ON

#define PANEL_CMD_MARKER_SIM                  "#sim"                 // Panel override: simulator panel
#define PANEL_CMD_MARKER_SIM_SWTE             "#sim-swte"            // Panel override: software tear check
#define PANEL_CMD_MARKER_SIM_HWTE             "#sim-hwte"            // Panel override: hardware tear check

#define NO_PANEL_CONFIG                       "none"                 // Panel override: skip display in bootloader


#define DISPLAY_CMDLINE_DSI_PRIMARY           " msm_drm.dsi_display0="   // Panel config prefix
#define DISPLAY_CMDLINE_DSI_SECONDARY         " msm_drm.dsi_display1="   // Panel config prefix


#define SIM_OVERRIDE                          "#override:"           // Panel config: override marker
#define DSI_0_STRING                          ":0:"                  // Panel config: dsi controller 0
#define DSI_1_STRING                          ":1:"                  // Panel config: dsi controller 1
#define DSI_CFG_STRING                        ":cfg:"                // Panel config: topology selection marker

#define HDMI_CONTROLLER_STRING                "hdmi:0"               // Panel config: HDMI controller marker
#define DSI_CONTROLLER_0_STRING               "dsi:0:"               // Panel config: DSI0 controller marker
#define DSI_CONTROLLER_1_STRING               "dsi:1:"               // Panel config: DSI1 controller marker

#define SPLIT_DSI_STRING                      "split_dsi"            // Panel config: ping pong split mode 
#define DUAL_DSI_STRING                       "dual_dsi"             // Panel config: dual dsi mode
#define SINGLE_DSI_STRING                     "single_dsi"           // Panel config: single dsi_mode

#define SIM_MODE_STRING                       "sim"                  // Panel config: simulator panel
#define SIM_HWTE_STRING                       "sim-hwte"             // Panel config: software tear check
#define SIM_SWTE_STRING                       "sim-swte"             // Panel config: hardware tear check

//Device tree related constants
#define DTB_PAD_SIZE                           1024                              // Device tree buffer padding size
#define SPLASH_MEM_DT_NODE_NAME                "/reserved-memory/splash_region"  // Name of the splash buffer device tree node
#define SPLASH_MEM_DT_REG                      "reg"                             // Splash Node's sub node name
#define SPLASH_MEM_PROP_SIZE                   (4 * sizeof(UINT32))              // Size of splash node in bytes
/* Helper to get display params struct*/
#define GET_DISPLAY_CONTEXT()                 (&gsDisplayContext)

//Variable name to store custom splash image storage info
#define QCOMLIBVAR_SPLASH_IMAGE_STORAGE_INFO  L"SplashImageStorageInfo"

// The display panel configuration file macro definition
#define DISP_FILE_EXT                         L".cfg"
#define DISP_FILE_NAME_MAX_SIZE               64
#define DISP_VARIABLE_VALUE_MAX_SIZE          256
#define DISP_LOGFS                            L"logfs"

/* -----------------------------------------------------------------------
** Local Types
** ----------------------------------------------------------------------- */

/* Display Utils Context
 */
typedef struct
{
  Display_UtilsParamsType  sDisplayParams;                           /* Params parsed from override command */
  bool32                   bOverride;                                /* Flag panel override */
  const PanelDTInfoType   *pDTInfoArray;                             /* DT info for all supported panels */
  CHAR8                    pPanelConfig[PANEL_CONFIG_STR_LEN_MAX];   /* Panel configuration command to kernel */
  uint32                   uDynamicRefreshDataAddress;               /* Memory address for PLL calibration data */
  uint32                   uContinuousSplashAddress;                 /* Memory address for continuous splash buffer */
  uint32                   uContinuousSplashDataSize;                /* Size of data in the continous splash buffer */
  CHAR8                    pSupportedPanels[PANEL_LIST_STR_LEN_MAX]; /* concatenated list of supported panels*/
} Display_UtilsContextType;


/* External references */

// Physical Partition Guids
extern EFI_GUID gEfiEmmcGppPartition1Guid;
extern EFI_GUID gEfiEmmcUserPartitionGuid;
extern EFI_GUID gEfiUfsLU4Guid;
extern EFI_GUID gMainFvGuid;
extern EFI_GUID gEfiImageFvNameGuid;

// Partition Type
extern EFI_GUID gEfiPlatPartitionTypeGuid;

/*=========================================================================
     Local Functions Prototype Headers
==========================================================================*/
/* CalculateLogoPosition() - Calculate the location of the logo image within the screen
 */
static VOID CalculateLogoPosition(
    IN  UINT32                LogoWidth,
    IN  UINT32                LogoHeight,
    OUT UINT32               *pPosX,
    OUT UINT32               *pPosY,
    RenderLogoParamsType     *pRenderLogoParams);

/* ConversionPixelCopy() - Pixel copy with fomat conversion
 */
static VOID ConversionPixelCopy(
    IN UINT32     SourceBitDepth,
    IN VOID      *ColorMap,
    IN UINT8     *SourceBuffer,
    IN UINT32     DestBitDepth,
    IN UINT8     *DestBuffer,
    IN UINT32     NumPixels);

/* Display_Utils_SetProperty
 */
EFI_STATUS Display_Utils_SetProperty (
    EFI_DISPLAY_UTILS_PROPERTY_TYPE ePropertyType,
    void                           *pData,
    UINT32                          uDataSize);

/* Display_Utils_GetProperty
 */
EFI_STATUS Display_Utils_GetProperty (
    EFI_DISPLAY_UTILS_PROPERTY_TYPE ePropertyType,
    void                           *pData,
    UINT32                         *pDataSize);

/* Display_Utils_RenderLogo
 */
EFI_STATUS Display_Utils_RenderLogo(IN  EFI_DISPLAY_UTILS_RENDERLOGO_CONFIG *psRenderLogoConfig);

/* Display_Utils_SetMode
 */
EFI_STATUS Display_Utils_SetMode (
    UINT32                         DisplayId,
    UINT32                         ModeNum,
    UINT32                         Flags);

/*=========================================================================
     Global Variables
==========================================================================*/

/* Display Utils Context
 */
Display_UtilsContextType gsDisplayContext;

/* Display Utils Protocol Implementation
 */
EFI_QCOM_DISPLAY_UTILS_PROTOCOL  gQcomDisplayUtilsProtocolImplementation = 
{
  DISPLAY_UTILS_REVISION,
  Display_Utils_SetProperty,
  Display_Utils_GetProperty,
  Display_Utils_RenderLogo,
  Display_Utils_SetMode,
  Display_Utils_SetVariable,
  Display_Utils_GetVariable
};


/*=========================================================================
     Local Static Functions
==========================================================================*/

/****************************************************************************
*
** FUNCTION: LocalAsciiStrnCat()
*/
/*!
* \brief
*   Helper function to concatenate strings upto limit and ignore further
*   characters.
*
* \param [in] pDest          - Destination string
*        [in] uLimit         - Max length for dest
*        [in] pSrc           - Source string
*
* \retval available chars in dest
*
****************************************************************************/
static uint32 LocalAsciiStrnCat(CHAR8 *pDest, uint32 uLimit, CHAR8 *pSrc)
{
  uint32 uSize  = AsciiStrLen(pSrc);
  uint32 uStart = AsciiStrLen(pDest);
  uint32 uAvail = 0;

  if (uLimit > (uStart + 1))
  {
    uAvail = uLimit - (uStart + 1); // 1 termination char
  }

  /* Check if there is enough size available */
  if ( uSize > uAvail )
  {
    //pSrc too big - will be truncated
    DEBUG((EFI_D_WARN, "DisplayUtils: Not enough room available for strcat: will truncate <%a> avail %d reqd %d\n",
           pSrc, uAvail, uSize));

    uSize = uAvail;
  }

  if (0 < uAvail)
  {
    /* AsciiStnCpy will copy pSrc and the fill the next byte with zero */
    AsciiStrnCpy(pDest+uStart, pSrc, uSize+1);
    
    uAvail -= uSize;
  }
  
  return uAvail;
}


/****************************************************************************
*
** FUNCTION: LocalAsciiPrintConfig()
*/
/*!
* \brief
*   Helper function to append configuration string. Converts num to ascii and
*   then concatenates to pDest
*
* \param [in] pDest          - Destination string
*        [in] uLimit         - Max length for pDest
*        [in] iNum           - configuration index
*
* \retval None
*
****************************************************************************/
static void LocalAsciiPrintConfig(CHAR8 *pDest, uint32 uLimit, int32 iNum)
{
  /* config ranges from 0 - 99 */
  if ((0 <= iNum)  && 
      (100 > iNum)) 
  {
    uint32 i       = 0;
    uint32 uDigit;
    CHAR8  str[3];

    MDP_OSAL_MEMZERO(str, sizeof(str));
    
    uDigit = iNum / 10;
    
    if (0 < uDigit)
    {
      str[i++] = '0'+uDigit;
    }
    
    uDigit   = iNum - (10 * uDigit);
    str[i++] = '0' + uDigit;
    str[i]   = 0;

    LocalAsciiStrnCat(pDest, uLimit, ":config");
    LocalAsciiStrnCat(pDest, uLimit, str);
  } 
}


/****************************************************************************
*
** FUNCTION: LocalAsciiTrim()
*/
/*!
* \brief
*   Helper function to remove leading spaces in ascii string
*
* \param [in] pSrc     - pointer to string
*
* \retval Trimmed ascii string or NULL if input is NULL
*
****************************************************************************/
static CHAR8* LocalAsciiTrim(CHAR8* pStr)
{
  if (NULL != pStr)
  {
    /* remove leading spaces, if any */
    while (' ' == *pStr)
    {
      pStr++;
    }
  }
  
  return pStr;
}


/****************************************************************************
*
** FUNCTION: GetTokenEnd()
*/
/*!
* \brief
*   Helper function to find the index of specific tokens inside string
*
* \param [in] pString        - input string to look for delimiters
*
* \retval pointer to delimiter or NULL if not found
*
****************************************************************************/
static CHAR8* GetTokenEnd(const CHAR8 *pString)
{
  CHAR8 *pHash  = NULL;
  CHAR8 *pColon = NULL;
  CHAR8 *pOut   = NULL;

  /* ':' and '#' are delimiters in the string */
  pColon = AsciiStrStr((CHAR8 *) pString, ":");
  pHash  = AsciiStrStr((CHAR8 *) pString, "#");

  if ((NULL != pColon) && 
      (NULL != pHash))
  {
    pOut = ((pColon < pHash) ? pColon : pHash);
  }
  else if (NULL != pColon)
  {
    pOut = pColon;
  }
  else if (NULL != pHash)
  {
    pOut = pHash;
  }
  
  return pOut;
}


/****************************************************************************
*
** FUNCTION: LookupPanelByName()
*/
/*!
* \brief
*   Helper function to look up panel name inside the global DT array 
*
* \param [in] pShortName   - short name of the panel
*
* \retval pointer to DT info struct or NULL if not found
*
****************************************************************************/
static const PanelDTInfoType* LookupPanelByName(CHAR8 *pShortName)
{
  const PanelDTInfoType    *pDT             = NULL;
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();
    
  if ((NULL != pShortName) &&
      (NULL != pDisplayContext->pDTInfoArray))
  {
    uint32      i     = 0;
    uint32      uLen  = AsciiStrLen(pShortName);

    while (MDPPLATFORM_PANEL_NONE != pDisplayContext->pDTInfoArray[i].ePanel)
    {
      if((uLen == AsciiStrLen(pDisplayContext->pDTInfoArray[i].name)) &&
         (0    == AsciiStrnCmp(pDisplayContext->pDTInfoArray[i].name, pShortName, uLen)))
      {
        /* match found */
        pDT = &(pDisplayContext->pDTInfoArray[i]);
        break;
      }
      i++;
    }
  }
 
  return pDT;
}


/****************************************************************************
*
** FUNCTION: UpdateDisplayParams()
*/
/*!
* \brief
*   Helper function to update display params struct with selected panel info
*
* \param Index of display interface
*
* \retval None
*
****************************************************************************/
static void UpdateDisplayParams(MDP_Display_IDType eDisplayId)
{
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();
  MDP_Panel_AttrType       *pDisplayInfo    = MDP_GET_DISPLAYINFO(eDisplayId);
  PanelOverrideParams      *pPanelParams;
  uint32                    i               = 0;

  if (NULL == pDisplayContext->pDTInfoArray)
  {
    //This is NULL during XBL RAMDUMP. So just ignore and return.
    MDP_Log_Message(MDP_LOGLEVEL_INFO, "DisplayDxe: DT info is null pointer\n");
  }
  else
  {
    if (MDP_DISPLAY_PRIMARY == eDisplayId)
    {
      pPanelParams = &pDisplayContext->sDisplayParams.sPrimary;
    }
    else
    {
      pPanelParams = &pDisplayContext->sDisplayParams.sSecondary;
    }

    while (MDPPLATFORM_PANEL_NONE != pDisplayContext->pDTInfoArray[i].ePanel)
    {
      if (pDisplayInfo->eSelectedPanel == pDisplayContext->pDTInfoArray[i].ePanel)
      {
        MDPPlatformParams    sPlatformParams;
        MDP_Status           eMDPStatus;

        /* match found */
        pPanelParams->psDTInfo = &(pDisplayContext->pDTInfoArray[i]);
        AsciiStrnCpy(pPanelParams->shortName, pDisplayContext->pDTInfoArray[i].name, DISPLAY_PANEL_NAME_MAX_LEN);

        if (DISP_TOPOLOGY_CONFIG_NONE == pPanelParams->eTopologyCfg)
        {
          /* Using default topology index */
          pPanelParams->eTopologyCfg = pDisplayContext->pDTInfoArray[i].uConfigNum;
        }

        MDP_OSAL_MEMZERO(&sPlatformParams, sizeof(MDPPlatformParams));
        //get panel support flags
        sPlatformParams.sPanelSupport.ePanel     = pPanelParams->psDTInfo->ePanel;
        sPlatformParams.sPanelSupport.uModeIndex = pPanelParams->eTopologyCfg;

        if (MDP_STATUS_OK != (eMDPStatus = MDPPlatformConfigure(eDisplayId,
                                                                MDPPLATFORM_CONFIG_GETPANELSUPPORTFLAGS,
                                                               &sPlatformParams)))
        {
          DEBUG((EFI_D_ERROR, "DisplayUtils: MDPPlatformConfigure(MDPPLATFORM_CONFIG_GETPANELSUPPORTFLAGS) failed with returned error %d\n", eMDPStatus));
        }
        else
        {
          pDisplayContext->sDisplayParams.uFlags   |= sPlatformParams.sPanelSupport.uFlags;
          pPanelParams->uSelectedModeFlags         |= sPlatformParams.sPanelSupport.uFlags;
        }
        break;
      }
      i++;
    }
  }
}


/****************************************************************************
*
** FUNCTION: SetupSupportedDisplayInterface()
*/
/*!
* \brief
*   Helper function to setup display interfaces according to fastboot oem override string
*
* \param pSupportedDisplayList  - Supported display interface list.
*        pPanelOverrideCmd      - panel override command.
*
* \retval None
*
****************************************************************************/
static void SetupSupportedDisplayInterface(bool32 (*pSupportedDisplayList)[], char *pPanelOverrideCmd)
{
  char    *pStr      = NULL;
  char    *pOverride = (char*) pPanelOverrideCmd;

  if ((NULL == pOverride) ||
      (NULL == pSupportedDisplayList))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: Invalid panel override command or supported display list\n"));
  }
  else
  {
    if ((NULL != (pStr = AsciiStrStr((char *) pOverride, PANEL_CMD_MARKER_PRIM))) &&
        (NULL != (pStr = AsciiStrStr((char *) pOverride, PANEL_CMD_MARKER_SEC))))
    {
      /* Both primary and secondary string can be detected */
      (*pSupportedDisplayList)[MDP_DISPLAY_PRIMARY]   = TRUE;
      (*pSupportedDisplayList)[MDP_DISPLAY_SECONDARY] = TRUE;
    }
    else if ((NULL == (pStr = AsciiStrStr((char *) pOverride, PANEL_CMD_MARKER_PRIM))) &&
             (NULL != (pStr = AsciiStrStr((char *) pOverride, PANEL_CMD_MARKER_SEC))))
    {
      /* Only secondary string can be detected */
      (*pSupportedDisplayList)[MDP_DISPLAY_PRIMARY]   = FALSE;
      (*pSupportedDisplayList)[MDP_DISPLAY_SECONDARY] = TRUE;
    }
    else
    {
      (*pSupportedDisplayList)[MDP_DISPLAY_PRIMARY]   = TRUE;
      (*pSupportedDisplayList)[MDP_DISPLAY_SECONDARY] = FALSE;
    }
  }
}


/****************************************************************************
*
** FUNCTION: ParsePanelOverrideCommand()
*/
/*!
* \brief
*   Helper function to parse and extract params from panel override string
*
* \param [in]  pPanelOverrideCmd - panel override command from ABL
*        [out] pDisplayParams    - display override params
*
* \retval bool32 - TRUE: panel override enabled, FALSE: disable panel override, using default panel
*
****************************************************************************/
static bool32 ParsePanelOverrideCommand(bool32 (*pSupportedDisplayList)[], const char *pPanelOverrideCmd, Display_UtilsParamsType *pDisplayParams)
{
  char    *pStr           = NULL;
  char    *pStrTmp        = NULL;
  char    *pOverride      = (char*) pPanelOverrideCmd;
  int32    len;

  if ((NULL == pDisplayParams) ||
      (NULL == (pOverride = LocalAsciiTrim(pOverride))))
  {
    DEBUG((EFI_D_ERROR, "DisplayUtils: Invalid arguments\n"));
  }
  /* check for special configuration */
  else if(0 == AsciiStrnCmp(pOverride, NO_PANEL_CONFIG, AsciiStrLen(NO_PANEL_CONFIG)))
  {
    /* special case: if fastboot oem panel name is "none" then enable SW render and pass empty string to kernel */
    pDisplayParams->uFlags |= DISP_MODE_SKIP_BOOTLOADER|DISP_MODE_PANEL_NONE;
  }
  else
  {
    MDP_OSAL_MEMZERO(pDisplayParams, sizeof(Display_UtilsParamsType));

    SetupSupportedDisplayInterface(pSupportedDisplayList, pOverride);

    /* Primary panel string */
    pStr = AsciiStrStr((char *) pOverride, PANEL_CMD_MARKER_PRIM);
    if (NULL != pStr) 
    {
      /*
       * Parse the primary panel for cases where 'prim' prefix
       * is present in the fastboot oem command before primary
       * panel string.
       * Examples:
       * 1.) fastboot oem select-display-panel prim:jdi_1080p_video:sec:sharp_1080p_cmd
       * 2.) fastboot oem select-display-panel prim:jdi_1080p_video:skip:sec:sharp_1080p_cmd
       * 3.) fastboot oem select-display-panel prim:jdi_1080p_video:disable:sec:sharp_1080p_cmd
       * 4.) fastboot oem select-display-panel prim:jdi_1080p_video:skip#sim:sec:sharp_1080p_cmd
       */
      pStr += PANEL_CMD_MARKER_PRIM_LEN;
      pStrTmp = GetTokenEnd((const char*) pStr);
      if (NULL == pStrTmp)
      {
        pStrTmp = pStr + AsciiStrLen(pStr);
      }
      len = pStrTmp - pStr;
      if (len > (DISPLAY_PANEL_NAME_MAX_LEN - 1) )
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Panel name is too long %d vs max %d\n", len, DISPLAY_PANEL_NAME_MAX_LEN - 1)); 
        len = DISPLAY_PANEL_NAME_MAX_LEN - 1;
      }
      
      AsciiStrnCpy(pDisplayParams->sPrimary.shortName,pStr, len);
      
      pDisplayParams->sPrimary.shortName[DISPLAY_PANEL_NAME_MAX_LEN-1] = 0;
    } 
    else 
    {
      /*
       * Check if secondary panel string is present.
       * The 'prim' prefix definitely needs to be present
       * to specify primary panel for cases where secondary panel
       * is also specified in fastboot oem command. Otherwise, it
       * becomes tough to parse the fastboot oem command for primary
       * panel. If 'sec' prefix is used without 'prim' prefix, it
       * means the default panel needs to be picked as primary panel.
       * Example:
       * fastboot oem select-display-panel sec:sharp_1080p_cmd
       */
      pStr = AsciiStrStr((char *) pOverride, PANEL_CMD_MARKER_SEC);
      if (NULL == pStr) 
      {
        /*
         * This code will be executed for cases where the
         * secondary panel is not specified i.e., single/split
         * DSI cases.
         * Examples:
         * 1.) fastboot oem select-display-panel jdi_1080p_video
         * 2.) fastboot oem select-display-panel sharp_1080p_cmd:skip
         * 3.) fastboot oem select-display-panel sharp_1080p_cmd:disable
         * 4.) fastboot oem select-display-panel sim_cmd_panel#sim-swte
         */
        pStr = GetTokenEnd(pOverride);
        if (NULL != pStr) 
        {
          len = pStr - pOverride;
        }
        else
        {
          len = AsciiStrLen(pOverride);
        }
        
        if (len > (DISPLAY_PANEL_NAME_MAX_LEN - 1) )
        {
          DEBUG((EFI_D_WARN, "DisplayUtils: Panel name is too long %d vs max %d\n", len, DISPLAY_PANEL_NAME_MAX_LEN - 1));
          len = DISPLAY_PANEL_NAME_MAX_LEN - 1;
        }
        AsciiStrnCpy(pDisplayParams->sPrimary.shortName, pOverride, len);
      }
    }

    /* Use short names to look up DT info params */
    pDisplayParams->sPrimary.psDTInfo = LookupPanelByName(pDisplayParams->sPrimary.shortName);

    /*
     * Secondary panel string.
     * This is relatively simple. The secondary panel string gets
     * parsed if the 'sec' prefix is present.
     */
    pStr = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_SEC);
    if (NULL != pStr)
    {

      pStr += PANEL_CMD_MARKER_SEC_LEN;
      pStrTmp = GetTokenEnd((const char*)pStr);
      if (NULL == pStrTmp)
      {
        pStrTmp = pStr + AsciiStrLen(pStr);
      }
      len = pStrTmp - pStr;
      if (len > (DISPLAY_PANEL_NAME_MAX_LEN - 1))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Panel name is too long %d vs max %d\n", len, DISPLAY_PANEL_NAME_MAX_LEN - 1));
        len = DISPLAY_PANEL_NAME_MAX_LEN - 1;
      }
      AsciiStrnCpy(pDisplayParams->sSecondary.shortName, pStr, len);
      pDisplayParams->sSecondary.shortName[DISPLAY_PANEL_NAME_MAX_LEN - 1] = 0;

      /* Use short names to look up DT info params */
      pDisplayParams->sSecondary.psDTInfo = LookupPanelByName(pDisplayParams->sSecondary.shortName);

      /* Topology configuration for secondary panel */
      pDisplayParams->sSecondary.eTopologyCfg = DISP_TOPOLOGY_CONFIG_NONE;

      pStrTmp = AsciiStrStr((char *)pStr, PANEL_CMD_MARKER_CONFIG);
      if (NULL != pStrTmp)
      {
        pDisplayParams->sSecondary.eTopologyCfg = AsciiStrDecimalToUintn((const char*)(pStrTmp + PANEL_CMD_MARKER_CONFIG_LEN)) + 1;
      }
    }
    else
    {
      pDisplayParams->sSecondary.shortName[0] = '\0';
    }

    /* Topology configuration for primary panel */
    pDisplayParams->sPrimary.eTopologyCfg = DISP_TOPOLOGY_CONFIG_NONE;

    pStrTmp = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_CONFIG);
    if ((NULL != pStrTmp) &&
        ((NULL == pStr) ||
         ((NULL != pStr) &&
          (pStrTmp < pStr))))
    {
      pDisplayParams->sPrimary.eTopologyCfg = AsciiStrDecimalToUintn((const char*)(pStrTmp + PANEL_CMD_MARKER_CONFIG_LEN)) + 1;
    }

    pStrTmp = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_TIMING);
    if ((NULL != pStrTmp) &&
        ((NULL == pStr) ||
         ((NULL != pStr) &&
          (pStrTmp < pStr))))
    {
      pDisplayParams->sPrimary.eTimingCfg = AsciiStrDecimalToUintn((const char*)(pStrTmp + PANEL_CMD_MARKER_TIMING_LEN)) + 1;
    }

    /* Skip LK configuration */
    pStr = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_SKIP);
    if (NULL != pStr)
    {
      pDisplayParams->uFlags |= DISP_MODE_SKIP_BOOTLOADER;
    }

   /* SEAMLESS SPLASH COntrol */
    pStr = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_SPLASH_ON);
    if (NULL != pStr)
    {
      pDisplayParams->uFlags |= DISP_MODE_SEAMLESS_SPLASH_ON;
    }

    pStr = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_SPLASH_OFF);
    if (NULL != pStr)
    {
      pDisplayParams->uFlags |= DISP_MODE_SEAMLESS_SPLASH_OFF;
    }

    /* Cont. splash status */
    pStr = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_DISABLE);
    if (NULL != pStr)
    {
      pDisplayParams->uFlags &= ~DISP_MODE_SEAMLESS_SPLASH_ON;
      pDisplayParams->uFlags |= DISP_MODE_SEAMLESS_SPLASH_OFF;
    }

    /* Interposer card to swap DSI0 and DSI1 lanes */
    pStr = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_SWAP);
    if (NULL != pStr)
    {
      pDisplayParams->uFlags |= DISP_MODE_SWAP_DSI_CONTROLLER;
    }

    /* DSI PLL source */
    pDisplayParams->ePllOverride = PLL_OVERRIDE_NONE;
    pStr = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_PLL0);
    if (NULL != pStr)
    {
      pDisplayParams->ePllOverride = PLL_OVERRIDE_PLL0;
    }
    else
    {
      pStr = AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_PLL1);
      if (NULL != pStr)
      {
        pDisplayParams->ePllOverride = PLL_OVERRIDE_PLL1;
      }
    }

    /* Simulator status */
    pDisplayParams->eSimMode = PANELSIM_NONE;
    if (NULL != AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_SIM_HWTE))
    {
      pDisplayParams->eSimMode = PANELSIM_HWTE;
    }
    else if (NULL != AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_SIM_SWTE))
    {
      pDisplayParams->eSimMode = PANELSIM_SWTE;
    }
    else if (NULL != AsciiStrStr((char *)pOverride, PANEL_CMD_MARKER_SIM))
    {
      pDisplayParams->eSimMode = PANELSIM_MODE;
    }

    /* disable cont splash when booting in simulator mode */
    if (PANELSIM_NONE != pDisplayParams->eSimMode)
    {
      pDisplayParams->uFlags &= ~DISP_MODE_SEAMLESS_SPLASH_ON;
      pDisplayParams->uFlags |= DISP_MODE_SEAMLESS_SPLASH_OFF;
    }
  }

  return TRUE;
}


/****************************************************************************
*
** FUNCTION: panelConfigOutput()
*/
/*!
* \brief
*   Helper to build panel configuration string
*
* \param [in]  eDisplayId       - ID of display interface
         [in]  psOverrideParam  - display override params 
*        [out] ppStr            - Panel configuration string
*
* \retval None
*
****************************************************************************/
void panelConfigOutput(MDP_Display_IDType eDisplayId, PanelOverrideParams *psOverrideParam, CHAR8 **ppStr)
{
  uint32 uShortNameLen = 0;
  uint32 uTopology     = 0;
  uint32 uTiming       = 0;
  CHAR8  DTLabel[DISPLAY_PANEL_DT_LABEL_MAX_LEN];

  /* Update panel attribute for PP Split info */
  if(DISP_MODE_PING_PONG_SPLIT & psOverrideParam->uSelectedModeFlags)
  {
    MDP_Panel_AttrType *pDisplayInfo = MDP_GET_DISPLAYINFO(eDisplayId);
    pDisplayInfo->eDisplayFlags |= MDP_PANEL_FLAG_ENABLE_PINGPONG_SPLIT;
  }

  MDP_OSAL_MEMZERO(DTLabel, sizeof(DTLabel));
  uShortNameLen = AsciiStrLen(psOverrideParam->shortName);
  uTopology     = (uint32)psOverrideParam->eTopologyCfg;


  /* Decide how to construct the DTLabel */
  if((NULL == psOverrideParam->psDTInfo) &&
     (uShortNameLen > 0))
  {
    /* use short name to construct DTLabel */
    LocalAsciiStrnCat(DTLabel, DISPLAY_PANEL_DT_LABEL_MAX_LEN, (CHAR8 *)psOverrideParam->shortName);
    LocalAsciiStrnCat(DTLabel, DISPLAY_PANEL_DT_LABEL_MAX_LEN, ":");
  }
  else if(NULL != psOverrideParam->psDTInfo)
  {
    /* always use psOverrideParam->psDTInfo->DTLabel if it is not NULL */
    LocalAsciiStrnCat(DTLabel, DISPLAY_PANEL_DT_LABEL_MAX_LEN, (CHAR8 *)psOverrideParam->psDTInfo->DTLabel);

    /* Update the topology if the config has been specified */
    if (DISP_TIMING_CONFIG_NONE == psOverrideParam->eTimingCfg )
    {
      psOverrideParam->eTimingCfg = psOverrideParam->psDTInfo->uTimingNum;
    }
  }

  if(uShortNameLen > 0)
  {
    CHAR8 *pDisplayCmdLine;
    uTiming = (uint32)psOverrideParam->eTimingCfg;

    if (MDP_DISPLAY_PRIMARY == eDisplayId)
    {
      pDisplayCmdLine = DISPLAY_CMDLINE_DSI_PRIMARY;
    }
    else
    {
      pDisplayCmdLine = DISPLAY_CMDLINE_DSI_SECONDARY;
    }



    LocalAsciiStrnCat(*ppStr, PANEL_CONFIG_STR_LEN_MAX, pDisplayCmdLine);
    (*ppStr) += AsciiStrLen(pDisplayCmdLine);

    LocalAsciiStrnCat(*ppStr, PANEL_CONFIG_STR_LEN_MAX, (CHAR8 *)DTLabel);
    (*ppStr) += AsciiStrLen(DTLabel);

    if (uTopology)
    {
      LocalAsciiStrnCat(*ppStr, PANEL_CONFIG_STR_LEN_MAX, "config");
      (*ppStr) += 6;
      **ppStr   = uTopology - 1 + '0';
      (*ppStr) += 1;
      **ppStr   = 0;
    }

    if (uTiming)
    {
      LocalAsciiStrnCat(*ppStr, PANEL_CONFIG_STR_LEN_MAX, ":timing");
      (*ppStr) += 7;
      **ppStr = uTiming - 1 + '0';
      (*ppStr) += 1;
      **ppStr = 0;
    }
  }
}


/****************************************************************************
*
** FUNCTION: UpdatePanelConfiguration()
*/
/*!
* \brief
*   Helper to build panel configuration string
*
* \param [in]  aSupportedDisplayList - Supported display list
*        [out] pConfigStr            - Panel configuration string
*
* \retval None
*
****************************************************************************/
static void UpdatePanelConfiguration(const bool32 aSupportedDisplayList[], CHAR8 *pConfigStr)
{
  Display_UtilsContextType  *pDisplayContext = GET_DISPLAY_CONTEXT();
  Display_UtilsParamsType   *pDisplayParams  = &pDisplayContext->sDisplayParams;
  CHAR8                     *pSimModeStr     = NULL;
  MDP_Display_IDType         eDisplayId      = MDP_DISPLAY_PRIMARY;

  /* Get updated display params */
  for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId <= MDP_DISPLAY_SECONDARY; eDisplayId++)
  {
    if (FALSE == aSupportedDisplayList[eDisplayId])
    {
      continue;
    }
    else
    {
      UpdateDisplayParams(eDisplayId);
    }
  }
  
  if (DISP_MODE_PANEL_NONE & pDisplayParams->uFlags)
  {
    /* Panel override with NONE, no string will be generated */
  }
  else
  {
    CHAR8 *pStr = pConfigStr;;

    /*
     * Not make change to pConfigStr 
     * instead of have pStr move to end of string after every panelConfigOutput() called.
     */
    for (eDisplayId = MDP_DISPLAY_PRIMARY; eDisplayId <= MDP_DISPLAY_SECONDARY; eDisplayId++)
    {
      if (FALSE == aSupportedDisplayList[eDisplayId])
      {
        continue;
      }
      else if (MDP_DISPLAY_PRIMARY   == eDisplayId)
      {
        panelConfigOutput(MDP_DISPLAY_PRIMARY,   &pDisplayParams->sPrimary,   &pStr);
      }
      else if (MDP_DISPLAY_SECONDARY == eDisplayId)
      {
        panelConfigOutput(MDP_DISPLAY_SECONDARY, &pDisplayParams->sSecondary, &pStr);
      }
    }

    switch(pDisplayParams->eSimMode)
    {
    case PANELSIM_MODE:
      pSimModeStr = SIM_MODE_STRING;
      break;
    case PANELSIM_HWTE:
      pSimModeStr = SIM_HWTE_STRING;
      break;
    case PANELSIM_SWTE:
      pSimModeStr = SIM_SWTE_STRING;
      break;
    case PANELSIM_NONE:
    default:
      pSimModeStr = NULL;
      break;
    }

    if (NULL != pSimModeStr)
    {
      LocalAsciiStrnCat(pConfigStr, PANEL_CONFIG_STR_LEN_MAX, SIM_OVERRIDE);
      LocalAsciiStrnCat(pConfigStr, PANEL_CONFIG_STR_LEN_MAX, pSimModeStr);
    }
  }
}


/****************************************************************************
*
** FUNCTION: CheckTargetPanelSupport()
*/
/*!
* \brief
*   Helper function to query Platform Lib for a list of supported panels and
*   set boot service variables to inform ABL
*
* \param None
*
* \retval None
*
****************************************************************************/
static void CheckTargetPanelSupport()
{
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();
  MDP_Status                eMDPStatus;
  MDPPlatformParams         sPlatformParams;
  

  MDP_OSAL_MEMZERO(&sPlatformParams, sizeof(MDPPlatformParams));
 
  if (MDP_STATUS_OK != (eMDPStatus = MDPPlatformConfigure(MDP_DISPLAY_PRIMARY, MDPPLATFORM_CONFIG_GETPANELDTINFO, &sPlatformParams)))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: MDPPlatformConfigure(MDPPLATFORM_CONFIG_GETPANELDTINFO) failed! Returned %d\n", eMDPStatus));
  }
  else
  {
    CHAR8 *pPanels                 = pDisplayContext->pSupportedPanels;
    CHAR8 *szSeparator             = ",";
    uint32 uPanelCount             = 0;
    PanelDTInfoType *psPanelDTInfo = sPlatformParams.psPanelDTInfo;
    /*
    The received panel list is sorted by panel IDs
    Go through the panel mapping to lookup names.
    */
    if (NULL != psPanelDTInfo)
    {
      /* Initialize to Zero */
      MDP_OSAL_MEMZERO(pPanels, PANEL_LIST_STR_LEN_MAX);

      while (MDPPLATFORM_PANEL_NONE != psPanelDTInfo[uPanelCount].ePanel)
      {
        if (0 < uPanelCount)
        {
          /* separator */
          LocalAsciiStrnCat(pPanels, PANEL_LIST_STR_LEN_MAX, szSeparator);
        }

        /* append it to string */
        LocalAsciiStrnCat(pPanels, PANEL_LIST_STR_LEN_MAX, (CHAR8*)psPanelDTInfo[uPanelCount].name);
        uPanelCount++;
      }

      /* Save Panel DT info to Display Context */
      pDisplayContext->pDTInfoArray = psPanelDTInfo;
    }

    MDP_SetBootServiceVariable(DISPVAR_SUPPORTED_PANEL_COUNT, &uPanelCount, sizeof(uPanelCount),   0);
    MDP_SetBootServiceVariable(DISPVAR_SUPPORTED_PANEL_LIST,  pPanels,      AsciiStrSize(pPanels), 0);
  }
}


/****************************************************************************
*
** FUNCTION: CheckPanelOverride()
*/
/*!
* \brief
*   Helper function to check if panel override is set and parse the override
*   string, if set.
*
* \param  None
*
* \retval None
*
****************************************************************************/
static void CheckPanelOverride(bool32 (*pSupportedDisplayList)[])
{

  Display_UtilsContextType *pDisplayContext   = GET_DISPLAY_CONTEXT();
  UINTN                     uLength           = 0;
  CHAR8                     panelOverrideStr[PANEL_OVERRIDE_STR_LEN_MAX];

  MDP_OSAL_MEMZERO(panelOverrideStr, sizeof(panelOverrideStr));
  uLength = PANEL_OVERRIDE_STR_LEN_MAX;

  /* Check Panel Override
  */
  if (MDP_STATUS_OK == MDP_Display_GetVariable(DISPVAR_PANEL_OVERRIDE, (UINT8 *)panelOverrideStr, &uLength, 0))
  {
    /* Parse String - Since this is an override scenario print the command to log*/
    DEBUG((EFI_D_ERROR, "DisplayUtils: Panel override command: <%a>\n", panelOverrideStr));
    
    /*Panel override string */
    pDisplayContext->bOverride  = ParsePanelOverrideCommand(pSupportedDisplayList, panelOverrideStr, &pDisplayContext->sDisplayParams);
  }
}


/****************************************************************************
*
** FUNCTION: ConversionPixelCopy()
*/
/*!
* \brief
*   Helper function to Convert a source 24bpp image to 32bpp with a dynamically
*   allocated buffer.
*
* \param[in]  SourceBitDepth    Source image bit depth
*       [in]  ColorMap          Pointer to BMP indexed color map table
*       [in]  SourceBuffer      Source image buffer
*       [in]  DestBitDepth      Destination image bit depth
*       [in]  DestBuffer        Destination image buffer
*       [in]  NumPixels         Number of pixels to process
*
* \retval None
*
****************************************************************************/
static
VOID
ConversionPixelCopy(
    IN UINT32     SourceBitDepth,
    IN VOID      *ColorMap,
    IN UINT8     *SourceBuffer,
    IN UINT32     DestBitDepth,
    IN UINT8     *DestBuffer,
    IN UINT32     NumPixels)
{

  // Check for valid input parameters
  if (SourceBitDepth == DestBitDepth)
  {
    CopyMem(DestBuffer, SourceBuffer, NumPixels*(SourceBitDepth/8));
  }
  else if ((24 == SourceBitDepth) && (32 == DestBitDepth))
  {
    UINT32  Count;
    UINT32 *Buffer32BPP = (UINT32*)DestBuffer;
    UINT8  *Buffer24BPP = (UINT8*)SourceBuffer;

    for (Count=0;Count<NumPixels;Count++)
    {
       Buffer32BPP[Count] = (UINT32) (0x00000000 | (Buffer24BPP[2]<<16) | (Buffer24BPP[1]<<8) | (Buffer24BPP[0]));

       // Increment to the next pixel
       Buffer24BPP+=3;
    }
  }
  else if ((8 == SourceBitDepth) && (32 == DestBitDepth))
  {
    UINT32  Count;
    UINT32 *Buffer32BPP = (UINT32*)DestBuffer;
    UINT8  *Buffer8BPP  = (UINT8*)SourceBuffer;
  
    for (Count=0;Count<NumPixels;Count++)
    {
       UINT8          uIndex = *Buffer8BPP;     
       BMP_COLOR_MAP *pColor = (BMP_COLOR_MAP*)ColorMap;
       
       Buffer32BPP[Count] = (UINT32) (0x00000000 | (pColor[uIndex].Red<<16) | (pColor[uIndex].Green<<8) | (pColor[uIndex].Blue));

       // Increment to the next pixel
       Buffer8BPP+=1;
    }
  }
  else
  {
    // All other conversion are not supported, fill with an solid color (gray)
    UINT32  Count;
    UINT32 *Buffer8BPP = (UINT32*)DestBuffer;

    for (Count=0;Count<NumPixels*(DestBitDepth/8);Count++)
    {
        Buffer8BPP[Count] = 0x5A;
    }
  }
}


/****************************************************************************
*
** FUNCTION: CalculateLogoPosition()
*/
/*!
* \brief
*   Helper function to Calculate the x,y position of the logo based on logo size,
*   screen size and placement rules.
*
* \param[in]   LogoWidth             Logo width in pixels
*       [in]   LogoHeight            Logo height in pixels
*       [out]  pPosX                 Horizontal placement of logo top left corder
*       [out]  pPoxY                 Vertical placement of logo top left corder
*       [in]   pRenderLogoParams     Render logo parameters
*
* \retval None
*
****************************************************************************/
static 
VOID 
CalculateLogoPosition(
    IN  UINT32                LogoWidth,
    IN  UINT32                LogoHeight,
    OUT UINT32               *pPosX,
    OUT UINT32               *pPosY,
    RenderLogoParamsType     *pRenderLogoParams)
{
  EFI_STATUS                    Status          = EFI_SUCCESS;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput = NULL;    

  Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,
                               NULL,
                               (VOID **)&GraphicsOutput);

  if ((EFI_SUCCESS != Status) ||
      (NULL        == GraphicsOutput))
  {
    DEBUG((EFI_D_ERROR, "DisplayUtils: Cannot locate GraphicsOutput.\r\n"));
  }
  else if ((NULL == pPosX)                                            ||
           (NULL == pPosY)                                            ||
           (0    == GraphicsOutput->Mode->Info->HorizontalResolution) ||
           (0    == GraphicsOutput->Mode->Info->VerticalResolution))
  {
    DEBUG((EFI_D_ERROR, "DisplayUtils: Invalid inputs of pPos and resolution.\r\n"));
  }
  else
  {
    UINT32  PanelWidth   = GraphicsOutput->Mode->Info->HorizontalResolution;
    UINT32  PanelHeight  = GraphicsOutput->Mode->Info->VerticalResolution;
    INT32   LogoPosX     = 0;
    INT32   LogoPosY     = 0;

    if ((NULL                      != pRenderLogoParams) &&
        (pRenderLogoParams->uFlags &  RENDER_LOGO_POSITION_OVERRIDE))
    {
      LogoPosX = pRenderLogoParams->uXPosition;
      LogoPosY = pRenderLogoParams->uYPosition;
    }
    else
    {
      // Calculate LogoPosX and LogoPosY based on platform panel resolution.
      // Center of BGRT image should be at the horizontal center of the panel.
      if (PanelWidth > LogoWidth)
      {
        LogoPosX = ((PanelWidth - LogoWidth)/2);
      }

      // Center of BGRT image should be at 38.2 percent of panel height from the top.
      if (PanelHeight > LogoHeight)
      {
        LogoPosY = (0.382*PanelHeight) - (UINT32)(LogoHeight/2);
      }
    }

    // If LogoPosX and LogoPosY does not have valid values, set them to (0,0)
    if ((LogoPosX         < 0)              ||
        (LogoPosY         < 0)              ||
        ((UINT32)LogoPosX > (PanelWidth-1)) ||
        ((UINT32)LogoPosY > (PanelHeight-1)))
    {
      LogoPosX = 0;
      LogoPosY = 0;
    }

    // Update the image offsets
    *pPosX = (UINT32)LogoPosX;
    *pPosY = (UINT32)LogoPosY;
  }
}


/****************************************************************************
*
** FUNCTION: Display_Utils_LoadFile()
*/
/*!
* \brief
*   Helper function to read a file from the GPP partition in to a caller
*   specified buffer.
*
* \param[in]       FileName     Filename including path in ascii.
*       [in][out]  Buffer       Pointer to a pointer to a buffer allocated by the caller.
*                               If buffer is NULL or smaller in size than required,
*                               then dynamically allocate buffer.
*       [in][out]  BufferSize   If input buffer is provided, then BufferSize indicates the size
*                               of callers buffer in bytes.
*                               If input buffer is null or buffersize is smaller than the filesize,
*                               size of a newly allocated buffer is stored here.
*       [out]      RetSize      The returned size of the image file that has been loaded in to memory.
*       [in]       eFlags       Flags to load file.
*
* \retval EFI_STATUS
* EFI_SUCCESS                 : Function returned successfully.
* EFI_SUCCESS                 : Image loaded successfully in to memory.
* EFI_INVALID_PARAMETER       : Invalid input parameters passed in
* EFI_OUT_OF_RESOURCES        : Input buffer is too small to hold the incoming image and dynamic allocation
*                               for a bigger memory failed.
*
****************************************************************************/
EFI_STATUS
Display_Utils_LoadFile(
    IN      CHAR8                   *FileName,
    IN OUT  UINT8                  **Buffer,
    IN OUT  UINT32                  *BufferSize,
    OUT     UINT32                  *RetSize,
    IN      DisplayLoadFileFlags     eFlags)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_GUID   *EFIParition = NULL;
  UINT32      Setting = 0;
  CHAR16      LogoFile[128];
  UINT32      LogoFileSize = 0;

  if ((NULL == FileName)   ||
      (NULL == Buffer)     ||
      (NULL == BufferSize) ||
      (NULL == RetSize))
  {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }
  else if ((NULL != *Buffer) &&
           (0 == *BufferSize))
  {
    // Buffersize cannot be zero if buffer is non-null
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (DISP_LOAD_FILE_FLAG_IMAGEFV & eFlags)
  {
    UINTN BufferSizeN = 0;

    /* Load file into buffer */
    Status = LoadFileFromGuidedFv(&gEfiImageFvNameGuid,
                                   FileName,
                                   Buffer,
                                  &BufferSizeN);
    if((EFI_SUCCESS != Status) ||
       (NULL        == Buffer))
    {
       DEBUG((EFI_D_WARN, "DisplayUtils: load file from imagefv failed with status(%d)!\n", Status));
    }
    else
    {
      *RetSize = (UINT32) BufferSizeN;
    }
  }
  else
  {
    // Plat partition is enabled as a compile time ConfigValue see uefiplat.cfg.
    Status = GetConfigValue("EnablePlatPartition", &Setting);
    if ((Status == EFI_SUCCESS) &&
        (Setting == 1))
    {
      // Load Plat partition
      if (EFI_SUCCESS != (Status = MountFatPartition(L"plat")))
      {
         // Warn and continue to try and load file
         DEBUG((EFI_D_WARN, "DisplayUtils: Could not mount plat partition.\n"));
         Status = EFI_SUCCESS;
      }
  
      // Convert input ascii to unicode
      AsciiStrToUnicodeStr(FileName, (CHAR16*)&LogoFile);

      if (boot_from_emmc())
      {
        // Check for the filesize before continuing, need to search the GPP partition and User partition.
        if (EFI_SUCCESS == (Status = GetFileSize((CHAR16*)&LogoFile,
                                                  &gEfiEmmcGppPartition1Guid,
                                                  &gEfiPlatPartitionTypeGuid,
                                                  NULL,
                                                  TRUE,
                                                  NULL,
                                                  &LogoFileSize)))
        {
          // Image found in GPP partition (WA), short cut the path and don't check the next partition.
          EFIParition = &gEfiEmmcGppPartition1Guid;
        }
        else if (EFI_SUCCESS == (Status = GetFileSize((CHAR16*)&LogoFile,
                                                       &gEfiEmmcUserPartitionGuid,
                                                       &gEfiPlatPartitionTypeGuid,
                                                       NULL,
                                                       TRUE,
                                                       NULL,
                                                       &LogoFileSize)))
        {
          // Image found in User partition (WP),
          EFIParition = &gEfiEmmcUserPartitionGuid;
        }
      }
      else if (boot_from_ufs())
      {
        if (EFI_SUCCESS == (Status = GetFileSize((CHAR16*)&LogoFile,
                                                  &gEfiUfsLU4Guid,
                                                  &gEfiPlatPartitionTypeGuid,
                                                  NULL,
                                                  TRUE,
                                                  NULL,
                                                  &LogoFileSize)))
        {
          // Image found in UFS LUN 4
          EFIParition = &gEfiUfsLU4Guid;
        }
      }
    }

    if (NULL != EFIParition)
    {
      //Cache the original provided buffer
      UINT8 *pOriginalBuffer     = *Buffer;
      UINT32 uOriginalBufferSize = *BufferSize;

      // If no buffer is provided or buffersize is smaller than filesize, allocate a new buffer dynamically.
      if ((NULL == pOriginalBuffer) ||
          (LogoFileSize > uOriginalBufferSize))
      {
        UINT8 *pNewBuffer;

        //Allocate memory to a new buffer
        pNewBuffer = (UINT8 *)AllocatePool(LogoFileSize);

        if (NULL != pNewBuffer)
        {
          if (NULL != pOriginalBuffer)
          {
            // Free the passed original buffer as it is too small and a bigger buffer has been allocated.
            FreePool(pOriginalBuffer);
          }

          //Update the provided buffer with new buffer address and size.
          *Buffer     = pNewBuffer;
          *BufferSize = LogoFileSize;
        }
        else
        {
          /* Not enough memory available for dynamic memory allocation */
          Status  = EFI_OUT_OF_RESOURCES;
        }
      }

      if (EFI_SUCCESS == Status)
      {
        UINTN  BytesToRead = LogoFileSize;

         // Read the entire buffer in to memory
         if (EFI_SUCCESS == (Status = ReadFile(LogoFile,
                                               EFIParition,
                                               &gEfiPlatPartitionTypeGuid,
                                               NULL,
                                               TRUE,
                                               NULL,
                                               &BytesToRead,
                                               0,
                                               *Buffer,
                                               LogoFileSize)))
         {
           // Return the result of the read
           *RetSize = BytesToRead;
         }
      }
    }
    else
    {
      UINTN BufferSizeN = 0;

      AsciiStrToUnicodeStr(FileName, (CHAR16*)&LogoFile);

      Status = ReadFromFV(LogoFile, (void **)Buffer, &BufferSizeN);
      if (EFI_ERROR(Status) ||
         (Buffer == NULL))
      {
        Status = EFI_LOAD_ERROR;
      }

      *RetSize = (UINT32) BufferSizeN;
    }
  }

  return Status;
}


/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetUpdatedCmdLine()
**
** DESCRIPTION:
**   Append the display command line to the existing command line
**
** ---------------------------------------------------------------------- */
static bool32 GetUpdatedCmdLine(CHAR8* pOrig, uint32 uOrigSize, CHAR8 **ppUpdatedStr, int *piUpdatedSize)
{

  CHAR8 *pCmdLine = NULL;
  UINT32 uLen     = 0;
  UINT32 uSize    = 0;
  bool32 bUpdate  = FALSE;
  CHAR8* pStr     = NULL;

  /* Get the display command line */
  Display_Utils_GetDisplayCommandLine(&pCmdLine, &uLen);

  /* calculate size including termination*/
  uSize = sizeof(CHAR8) * (uLen + 1);

  if ((NULL == pCmdLine) ||
      (0 == uLen))
  {
    /* No panel configuration available */
  }
  else if (NULL == (pStr = MDP_OSAL_CALLOC(uOrigSize + uSize)))
  {
    DEBUG ((EFI_D_ERROR, "Display: Failed to allocate memory for display command line\n"));
  }
  /* Replace null termination with space */
  else
  {
    /* uOrigSize is checked for zero in the calling function */
    UINT32 uOrigLen = uOrigSize - sizeof(CHAR8);

    /* Copy original string - without termination character*/
    MDP_OSAL_MEMCPY(pStr, pOrig, uOrigLen * sizeof(CHAR8));

    /* space to separate */
    pStr[uOrigLen] = ' ';

    /* copy display command line - has termination at end*/
    MDP_OSAL_MEMCPY(&pStr[uOrigLen + 1], pCmdLine, uSize);

    /* Return updated data & size */
    *ppUpdatedStr = pStr;
    *piUpdatedSize = (int) uOrigSize + uSize;

    /* Flag property update */
    bUpdate = TRUE;
  }
  
  return bUpdate;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: UpdateDisplayCmdLine()
**
** DESCRIPTION:
**   Update the display command line in the device tree node
**
** ---------------------------------------------------------------------- */
static void UpdateDisplayCmdLine(void *pFdt)
{
  int           iRet     = 0;
  const CHAR8  *pOrigStr = NULL;
  CHAR8        *pNewStr  = NULL;
  int           iOrigSize= 0;
  int           iNewSize = 0;
  int           iOffset;

  /* Get offset of the chosen node */
  if (0 > (iOffset = fdt_path_offset(pFdt, "/chosen")))
  {
    DEBUG ((EFI_D_ERROR, "ERROR: Could not find chosen node ...\n"));
  }
  else if ((NULL == (pOrigStr = (const CHAR8*) fdt_getprop(pFdt, iOffset, (CONST char*)"bootargs", &iOrigSize))) ||
           (iOrigSize <= 0 ))
  {
    DEBUG ((EFI_D_ERROR, "Display: Could not get bootargs from device tree\n"));
  }
  else if (FALSE == GetUpdatedCmdLine((CHAR8*) pOrigStr, iOrigSize, &pNewStr, &iNewSize))
  {
    /* Nothing to update - Ignore and quit */
  }
  /* Set property */
  else if (0 != (iRet = fdt_setprop(pFdt, iOffset, (const char*)"bootargs", (const void*)pNewStr, iNewSize)))
  {
    DEBUG ((EFI_D_ERROR, "Display: Failed to update display command line on device tree. Return code: %d\n", iRet));
  }

  /* Free memory if allocated */
  if (NULL != pNewStr)
  {
    MDP_OSAL_FREE(pNewStr);
  }
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: UpdateSplashNode()
**
** DESCRIPTION:
**   Update the continuous splash device tree node
**
** ---------------------------------------------------------------------- */
static void UpdateSplashNode(void *pFdt)
{
  CONST struct fdt_property  *pProperty     = NULL;
  int                         iPropertySize = 0;
  int                         iRet          = 0;
  int                         iOffset;
  
  /* Get offset of the splash memory reservation node */
  if (0 > (iOffset = fdt_path_offset(pFdt, SPLASH_MEM_DT_NODE_NAME)))
  {
    DEBUG ((EFI_D_ERROR, "Display: Could not get splash memory region node\n"));
  }
  /* Get the property that specifies the splash memory details */
  else if (0 == (pProperty = fdt_get_property(pFdt, iOffset, SPLASH_MEM_DT_REG, &iPropertySize)))
  {
    DEBUG ((EFI_D_ERROR, "Display: Could not find the splash reg property\n"));
  }
  else if (iPropertySize != SPLASH_MEM_PROP_SIZE)
  {
    DEBUG ((EFI_D_ERROR, "Display: Splash memory node size mismatch in device tree. Expected: %d, Actual: %d\n",
            SPLASH_MEM_PROP_SIZE, iPropertySize));
  }
  else 
  {
    UINT32* pData = (UINT32*) pProperty->data;
    UINT32  uAddr = 0;
    UINT32  uSize = 0;

    /* Get splash buffer info */
    Display_Utils_GetContinuousSplashInfo(&uAddr, &uSize);
    
    pData[0] = 0;
    pData[1] = cpu_to_fdt32(uAddr);
    pData[2] = 0;
    pData[3] = cpu_to_fdt32(uSize);
    
    /* Update the property value in place */
    if (0 > (iRet = fdt_setprop_inplace(pFdt, iOffset, SPLASH_MEM_DT_REG, pProperty->data, iPropertySize)))
    {
      DEBUG ((EFI_D_ERROR, "Display: Could not update splash mem device tree node\n"));
    } 
  }
}

/****************************************************************************
*
** FUNCTION: UpdateDeviceTree()
*/
/*!
* \brief
*   Update display specific devicee tree nodes
*
* \param [in] pFdt         - Device tree address
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS UpdateDeviceTree(void *pFdt)
{
  EFI_STATUS                  eStatus       = EFI_NOT_FOUND;
  int                         iRet           = 0;

  if (0 != fdt_check_header(pFdt))
  {
      DEBUG ((EFI_D_ERROR, "Display: Failed to validate device tree header\n"));
  }
  /* Add padding to make space for new nodes and properties. */
  else if (0 != (iRet = fdt_open_into(pFdt, pFdt, fdt_totalsize(pFdt) + DTB_PAD_SIZE)))
  {
    DEBUG ((EFI_D_ERROR, "Display: Failed to move/resize device tree buffer\n"));
  }
  else 
  {
    /* Update display command line*/
    UpdateDisplayCmdLine(pFdt);

    /* Update continuous splash node */
    UpdateSplashNode(pFdt);
    
    if (0 != (iRet = fdt_pack(pFdt)))
    {
      DEBUG((EFI_D_ERROR, "Display: Device tree write failed: %d\n", iRet));
    }
    eStatus = EFI_SUCCESS;
  }
  
  return eStatus;
}

/****************************************************************************
*
** FUNCTION: WaitForEvent()
*/
/*!
* \brief
*   Wait for an event completion
*
* \param [in] pShortName  - Lock name to wait on
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS WaitForEvent(CHAR8 *pShortName)
{
  VOID* Init_Lock;
  UINTN Wait_Start;

  Init_Lock = InitLock(pShortName);
  Wait_Start = GetTimerCountms ();

  Lock (Init_Lock);
  UnLock (Init_Lock);

  return EFI_SUCCESS;
}

/*=========================================================================
      Public APIs
==========================================================================*/

/****************************************************************************
*
** FUNCTION: Display_Utils_Initialize()
*/
/*!
* \brief
*   Initialize Display Utils interface
*
* \param None
*
* \retval None
*
****************************************************************************/
void Display_Utils_Initialize(bool32 (*pSupportedDisplayList)[])
{ 
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();

  /* initialize override flag */
  MDP_OSAL_MEMZERO(pDisplayContext, sizeof(Display_UtilsContextType));

  /* get list of supported panels & build DT info array */
  CheckTargetPanelSupport();
  
  /* Check panel override */
  CheckPanelOverride(pSupportedDisplayList);

}


/****************************************************************************
*
** FUNCTION: Display_Utils_GetParams()
*/
/*!
* \brief
*   Helper to get display override params
*
* \param None
*
* \retval pointer to Display_UtilsParamsType or NULL if not set
*
****************************************************************************/
Display_UtilsParamsType* Display_Utils_GetParams(void)
{
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();
  Display_UtilsParamsType  *pDisplayParams  = NULL;

  if (pDisplayContext->bOverride)
  {
    pDisplayParams = &pDisplayContext->sDisplayParams;
  }
  return pDisplayParams;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_SetPanelConfiguration()
*/
/*!
* \brief
*   Send panel configuration string to ABL
*
* \param Supported display list
*
* \retval None
*
****************************************************************************/
void Display_Utils_SetPanelConfiguration(const bool32 aSupportedDisplayList[])
{

  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();
  CHAR8*                    pConfigStr      = pDisplayContext->pPanelConfig;
  MDP_Panel_AttrType       *pDisplayInfo    = MDP_GET_DISPLAYINFO(MDP_DISPLAY_PRIMARY);

  MDP_OSAL_MEMZERO(pConfigStr, sizeof(pDisplayContext->pPanelConfig));

  UpdatePanelConfiguration(aSupportedDisplayList, pConfigStr);

  /* Set panel configuration variable */
  MDP_SetBootServiceVariable(DISPVAR_PANEL_CONFIGURATION, pConfigStr, AsciiStrLen(pConfigStr)+1, 0);

  if (pDisplayContext->sDisplayParams.uFlags & DISP_MODE_SEAMLESS_SPLASH_ON)
  {
       pDisplayInfo->eDisplayFlags &= ~MDP_PANEL_FLAG_DISABLE_SEAMLESS_SPLASH ;
  }
  else if (pDisplayContext->sDisplayParams.uFlags & DISP_MODE_SEAMLESS_SPLASH_OFF)
  {
       pDisplayInfo->eDisplayFlags |= MDP_PANEL_FLAG_DISABLE_SEAMLESS_SPLASH ;
  }
}


/****************************************************************************
*
** FUNCTION: Display_Utils_CheckPanelSkip()
*/
/*!
* \brief
*   Check for panel override and if the overridden panel is supported in 
*   bootloader
*
* \param None
*
* \retval TRUE if panel configuration is to be skipped in bootloader
*
****************************************************************************/
bool32 Display_Utils_CheckPanelSkip()
{
  bool32                    bSkip           = FALSE;
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();

  if (TRUE == pDisplayContext->bOverride)
  {
    if (pDisplayContext->sDisplayParams.uFlags & DISP_MODE_SKIP_BOOTLOADER)
    {
      bSkip = TRUE;
    }
    else if (NULL != pDisplayContext->sDisplayParams.sPrimary.psDTInfo)
    {
      MDPPlatformParams    sPlatformParams;
      MDP_Status           eMDPStatus;
      
      MDP_OSAL_MEMZERO(&sPlatformParams, sizeof(MDPPlatformParams));

      // skip = FALSE only if a valid panel is found and is supported by bootloader
      // check if skip is forced by the panel override command

      //get panel support flags
      sPlatformParams.sPanelSupport.ePanel     = pDisplayContext->sDisplayParams.sPrimary.psDTInfo->ePanel;
      sPlatformParams.sPanelSupport.uModeIndex = pDisplayContext->sDisplayParams.sPrimary.eTopologyCfg;

      if (MDP_STATUS_OK != (eMDPStatus = MDPPlatformConfigure(MDP_DISPLAY_PRIMARY, 
                                                              MDPPLATFORM_CONFIG_GETPANELSUPPORTFLAGS,
                                                              &sPlatformParams)))
      {
        DEBUG ((EFI_D_ERROR, "MDPLib: MDPPlatformConfigure(MDPPLATFORM_CONFIG_GETPANELSUPPORTFLAGS) failed! Returned %d\n", 
                eMDPStatus)); 
      }
      else
      {

        /* set override flags if any */
        pDisplayContext->sDisplayParams.uFlags |= sPlatformParams.sPanelSupport.uFlags;
        if (DISP_MODE_SKIP_BOOTLOADER & pDisplayContext->sDisplayParams.uFlags)
        {
          bSkip = TRUE;
        }
      }
    }
  }
  return bSkip;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_GetDisplayCommandLine()
*/
/*!
* \brief
*   Gets the pointer to the panel configuration string & string length 
*
* \param [out] ppCmdLine - pointer to command line string
*        [out] pLen      - Ascii length of the string
*
* \retval None
*
****************************************************************************/
void Display_Utils_GetDisplayCommandLine(CHAR8 **ppCmdLine, UINT32 *pLen)
{
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();

  /* To make sure we dont go overrun the string */
  pDisplayContext->pPanelConfig[PANEL_CONFIG_STR_LEN_MAX - 1] = '\0';

  /* Set command line & len */
  *ppCmdLine = pDisplayContext->pPanelConfig;
  *pLen      = AsciiStrLen(pDisplayContext->pPanelConfig);
  
}


/****************************************************************************
*
** FUNCTION: Display_Utils_SetContinuousSplashInfo()
*/
/*!
* \brief
*   Saves the continuous splash address in Display context
*
* \param [in] uAddr - Splash buffer address
*        [in] uSize - Splash buffer size in bytes
*
* \retval None
*
****************************************************************************/
void Display_Utils_SetContinuousSplashInfo(UINT32 uAddr, UINT32 uSize)
{
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();

  pDisplayContext->uContinuousSplashAddress  = uAddr;
  pDisplayContext->uContinuousSplashDataSize = uSize;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_GetContinuousSplashInfo()
*/
/*!
* \brief
*   Retrieves the continuous splash buffer info
*
* \param [out] pAddr - Splash buffer address
*        [out] pSize - Splash buffer size in bytes
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS Display_Utils_GetContinuousSplashInfo(UINT32 *pAddr, UINT32 *pSize)
{
  EFI_STATUS                eStatus         = EFI_SUCCESS;
  Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();

  if ((NULL == pAddr) ||
      (NULL == pSize))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: Invalid arguments to continous splash configuration.\n"));
    eStatus = EFI_INVALID_PARAMETER;
  }
  else
  {
    *pAddr = pDisplayContext->uContinuousSplashAddress;
    *pSize = pDisplayContext->uContinuousSplashDataSize;
  }
  
  return eStatus;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_SetProperty()
*/
/*!
* \brief
*   Set a property on Display UEFI
*
* \param [in] PropertyType - Property identifier
*        [in] Data         - Pointer to data buffer
*        [in] Size         - Size of valid data in bytes
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS Display_Utils_SetProperty (
    EFI_DISPLAY_UTILS_PROPERTY_TYPE ePropertyType,
    void                           *pData,
    UINT32                          uDataSize)
{
  EFI_STATUS eStatus = EFI_SUCCESS;
  switch(ePropertyType)
  {
  case EFI_DISPLAY_UTILS_DEVICE_TREE_ADDR:
    {
      /* Locate device tree at the address provided and update display specific nodes */
      if ((NULL == pData) ||
          (uDataSize != sizeof(void*)))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Set property (id=%d) with invalid parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else if(EFI_SUCCESS != (eStatus = UpdateDeviceTree(pData)))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Device tree update failed with %d\n", eStatus));
      }
    }
    break;

  case EFI_DISPLAY_UTILS_PANEL_OVERRIDE:
    {
      if ((NULL == pData) ||
          (0 == uDataSize))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Set property (id=%d) with invalid parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else if ( uDataSize > (PANEL_OVERRIDE_STR_LEN_MAX *sizeof(CHAR8)))
      {
        DEBUG((EFI_D_WARN,"DisplayUtils: Panel override string is too long. Input %d, Max allowed %d\n", 
              uDataSize/sizeof(CHAR8), PANEL_OVERRIDE_STR_LEN_MAX));

        eStatus = EFI_INVALID_PARAMETER;
      }
      else if (EFI_SUCCESS != (eStatus = MDP_Display_SetVariable (DISPVAR_PANEL_OVERRIDE,
                                                                  (UINT8 *) pData,
                                                                  (uDataSize / (sizeof(CHAR8))),
                                                                  0)))
      {
        DEBUG((EFI_D_WARN,"DisplayUtils: Failed to set boot service variable <%s>. Returned %d\n", DISPVAR_PANEL_OVERRIDE, eStatus));
      }
      else
      {
        eStatus = EFI_SUCCESS;
      }
    }
    break;

  case EFI_DISPLAY_UTILS_DYNAMIC_REFRESH:
    {
      if ((NULL == pData) ||
          (0 == uDataSize))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Set property (id=%d) with invalid parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else
      {
        if(MDP_STATUS_OK == DSIDriver_SetDynamicRefreshFreqs(pData, uDataSize))
        {
          eStatus = EFI_SUCCESS;
        }
        else
        {
          eStatus = EFI_UNSUPPORTED;
        }
      }   
    }
    break;
  case EFI_DISPLAY_UTILS_DISPLAY_ORIENTATION:
    {
      if ((NULL == pData) ||
          (0 == uDataSize))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Set property (id=%d) with invalid parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else
      {
        MDP_Panel_AttrType                           *pDisplayInfo = MDP_GET_DISPLAYINFO(MDP_DISPLAY_PRIMARY);
        EFI_DISPLAY_UTILS_DISPLAY_ORIENTATION_PARAMS *pOrientation = (EFI_DISPLAY_UTILS_DISPLAY_ORIENTATION_PARAMS*)pData;
        MDP_PropertiesParamType                       sModeInfo;

        // Change orientation
        pDisplayInfo->ePhysPanelOrientation = (TRUE==pOrientation->bInverted)?MDP_ROTATE_180:MDP_ROTATE_NONE;

        // Don't change the current mode, use the current information.
        MDP_OSAL_MEMZERO(&sModeInfo, sizeof(MDP_PropertiesParamType));
        MDP_OSAL_MEMCPY(&sModeInfo.sModeParams.sSurfaceInfo, &pDisplayInfo->sFrameBuffer, sizeof(sModeInfo.sModeParams.sSurfaceInfo));
        sModeInfo.sModeParams.sRectInfo.uWidth  = pDisplayInfo->sFrameBuffer.uWidth;
        sModeInfo.sModeParams.sRectInfo.uHeight = pDisplayInfo->sFrameBuffer.uHeight;

            
        if (MDP_STATUS_OK == MDPSetProperty(MDP_DISPLAY_PRIMARY, MDP_DISPLAY_PROPERTY_SURFACE_RECT, &sModeInfo))
        {
          eStatus = EFI_SUCCESS;
        }
        else
        {
          eStatus = EFI_UNSUPPORTED;
        }
      } 
    }
    break;

  case EFI_DISPLAY_UTILS_WAIT_FOR_EVENT:
    {
      if ((NULL == pData) ||
          (0 == uDataSize))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Set property (id=%d) with invalid parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else if(EFI_SUCCESS != (eStatus = WaitForEvent(pData)))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Wait for the event failed with %d\n", eStatus));
      }
    }
    break;

  default:
    eStatus = EFI_UNSUPPORTED;
  }
  return eStatus;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_GetProperty()
*/
/*!
* \brief
*   Set a property on Display UEFI
*
* \param [in]     Type            Property type
*        [out]    Data            Pointer to data structure
*        [in/out] DataSize        [in] Max size of data 
*                                 [out] Actual size of data 
*
* \retval EFI_STATUS
* EFI_SUCCESS              : Function returned successfully.
* EFI_INVALID_PARAMETER    : Parameter passed is incorrect.  
* EFI_UNSUPPORTED          : Parameter passed is not supported. 
* EFI_DEVICE_ERROR         : Physical device reported an error.
* EFI_BUFFER_TOO_SMALL     : Buffer to small to copy data into
*
****************************************************************************/
EFI_STATUS Display_Utils_GetProperty(
    EFI_DISPLAY_UTILS_PROPERTY_TYPE ePropertyType,
    void                           *pData,
    UINT32                         *pDataSize)
{
  EFI_STATUS eStatus = EFI_SUCCESS;

  switch(ePropertyType)
  {
  case EFI_DISPLAY_UTILS_SUPPORTED_PANELS:
    {
      Display_UtilsContextType *pDisplayContext = GET_DISPLAY_CONTEXT();
      UINT32 Size                         = 0;

      if (NULL == pDataSize)
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Get property (id=%d) with Null parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else if ( *pDataSize < (Size = AsciiStrSize(pDisplayContext->pSupportedPanels)))
      {
        *pDataSize = Size;
        eStatus   = EFI_BUFFER_TOO_SMALL;
      }
      else if (NULL == pData)
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Get property (id=%d) with Null parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else 
      {
        MDP_OSAL_MEMCPY(pData, pDisplayContext->pSupportedPanels, Size);

        *pDataSize = Size;
        eStatus   = EFI_SUCCESS;
      }
    }
    break;
  case EFI_DISPLAY_UTILS_PANEL_CONFIG:
    {
      if ((NULL == pDataSize) ||
          (NULL == pData))
      {
        eStatus = EFI_INVALID_PARAMETER;
      }
      else if (*pDataSize < sizeof(EFI_DISPLAY_UTILS_PANEL_CONFIG))
      {
        eStatus = EFI_BUFFER_TOO_SMALL;
      }
      else
      {
        MDPPlatformParams                      sPlatformParams;
        EFI_DISPLAY_UTILS_PANEL_CONFIG_PARAMS *pPanelConfig = (EFI_DISPLAY_UTILS_PANEL_CONFIG_PARAMS*)pData;

        MDP_OSAL_MEMZERO(&sPlatformParams, sizeof(MDPPlatformParams));
        sPlatformParams.sPanelXMLInfo.uIndex = pPanelConfig->uPanelIndex;

        if (MDP_STATUS_OK == MDPPlatformConfigure(MDP_DISPLAY_PRIMARY, MDPPLATFORM_CONFIG_GETPANELXMLINFO, &sPlatformParams))
        {
          // Return the details
          pPanelConfig->pPanelConfig       = (CHAR8*)sPlatformParams.sPanelXMLInfo.pPanelXMLConfig;
          pPanelConfig->uPanelConfigLength = sPlatformParams.sPanelXMLInfo.uPanelXMLSize;

          eStatus = EFI_SUCCESS;
        }
        else
        {
          eStatus = EFI_UNSUPPORTED;
        }
      }
    }
    break;

    case EFI_DISPLAY_UTILS_DYNAMIC_REFRESH:
    {
      if ((NULL == pDataSize) ||
          (NULL == pData))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Get property (id=%d) with invalid parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else
      {
        if(MDP_STATUS_OK == DSIDriver_CalibratePLL(pData, pDataSize))
        {
          eStatus = EFI_SUCCESS;
        }
        else
        {
          eStatus = EFI_UNSUPPORTED;
        }
      }
    }
    break;

    case EFI_DISPLAY_UTILS_DISPLAY_DIAG_LOGS:
    {
      if ((NULL == pDataSize) ||
          (NULL == pData))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Get property (id=%d) with invalid parameter!\n", ePropertyType));
        eStatus = EFI_INVALID_PARAMETER;
      }
      else
      {
        if (MDP_STATUS_OK == MDP_GetDiagnosticInfo(pData, pDataSize))
        {
          eStatus = EFI_SUCCESS;
        }
        else 
        {
          eStatus = EFI_INVALID_PARAMETER;
        }
      }        
    }
    
    break;
  case EFI_DISPLAY_UTILS_GET_PANEL_RESOLUTION:
    {
       MDP_Display_IDType          eDisplayId    = MDP_DISPLAY_EXTERNAL;
       MDP_PropertiesParamType     sDisplayProp;
       MDP_PanelResolutionType    *pResolution   = (MDP_PanelResolutionType *)pData;

       if ((NULL == pDataSize) ||
           (NULL == pData)     ||
           (*pDataSize < sizeof(MDP_PanelResolutionType)))
       {
         DEBUG((EFI_D_WARN, "DisplayUtils: Get property (id=%d) with invalid parameter!\n", ePropertyType));
         eStatus = EFI_INVALID_PARAMETER;
       }
       else
       { 
           eStatus = MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_GETPANELRESOLUTION, &sDisplayProp);
           if (eStatus == EFI_SUCCESS)
           {
              MDP_OSAL_MEMCPY(pResolution, &sDisplayProp.sResolution, sizeof(MDP_PanelResolutionType));
           }
       }
    }
    break;
  
  case EFI_DISPLAY_UTILS_GET_FRAME_CRC:
    {
       MDP_Display_IDType         eDisplayId    = MDP_DISPLAY_EXTERNAL;
       MDP_PropertiesParamType    sDisplayProp;
       uint64                    *pCRC = (uint64 *)pData;

       if ((NULL == pDataSize) ||
           (NULL == pData)     ||
           (*pDataSize < sizeof(uint64)))
       {
         DEBUG((EFI_D_WARN, "DisplayUtils: Get property (id=%d) with invalid parameter!\n", ePropertyType));
         eStatus = EFI_INVALID_PARAMETER;
       }
       else
       { 
           eStatus = MDPGetProperty(eDisplayId, MDP_DISPLAY_PROPERTY_GETFRAMECRC, &sDisplayProp);
           if (eStatus == EFI_SUCCESS)
           {
              *pCRC = sDisplayProp.uCRC;
           }
       }
    }
    break;

  default:
    eStatus = EFI_UNSUPPORTED;
  }
  
  return eStatus;
}


/****************************************************************************
*
** FUNCTION: UpdateLogoParameters()
*/
/*!
* \brief
*   Recording the boot logo information to EFI_BOOT_LOGO_PROTOCOL/EDKII_BOOT_LOGO2_PROTOCOL
*   and the same can be retrieved during capsule update to render progress bar.
*
* \param[in]  BltBuffer        The BLT buffer for logo drawn on screen. If BltBuffer
*                              is set to NULL, it indicates that logo image is no
*                              longer on the screen.
*       [in]  DestinationX     X coordinate of destination for the BltBuffer.
*       [in]  DestinationY     Y coordinate of destination for the BltBuffer.
*       [in]  Width            Width of rectangle in BltBuffer in pixels.
*       [in]  Height           Hight of rectangle in BltBuffer in pixels.
*
****************************************************************************/
static VOID
UpdateLogoParameters(
  IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL           *BltBuffer,
  IN  UINTN                                   DestinationX,
  IN  UINTN                                   DestinationY,
  IN  UINTN                                   Width,
  IN  UINTN                                   Height
  )
{
  EFI_BOOT_LOGO_PROTOCOL    *BootLogo = NULL;
  EFI_STATUS                 Status   = EFI_SUCCESS;

  //
  // Try to open Boot Logo Protocol.
  //
  Status = gBS->LocateProtocol (&gEfiBootLogoProtocolGuid, NULL, (VOID **) &BootLogo);

  if (!EFI_ERROR (Status)) 
  {
    BootLogo->SetBootLogo (BootLogo, BltBuffer, DestinationX, DestinationY, Width, Height);
  }

}


/****************************************************************************
*
** FUNCTION: Display_Utils_RenderBGRTImage()
*/
/*!
* \brief
*   Render an image on to the screen from the BGRT buffer.
*
* \param[in]  BGRTImage         BGRT Source image buffer.
*       [in]  BGRTImageSize     BGRT image buffer size.
*       [in]  pRenderLogoParams Render logo parameters.
*
* \retval EFI_STATUS
* EFI_SUCCESS                 : Image loaded successfully in to memory.
* EFI_INVALID_PARAMETER       : Invalid input parameters passed in.
* EFI_OUT_OF_RESOURCES        : Not enough resources for buffer allocations.
*
****************************************************************************/
EFI_STATUS
Display_Utils_RenderBGRTImage(
    IN UINT8                 *BGRTImage,
    IN UINT32                 BGRTImageSize,
    RenderLogoParamsType     *pRenderLogoParams)
{

  EFI_STATUS                     Status         = EFI_SUCCESS;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput = NULL;
  BMP_IMAGE_HEADER              *ImageHeader    = (BMP_IMAGE_HEADER*)BGRTImage;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL  bgPixel        = {0};

  //
  // Try to open GOP(Graphics output protocol) for blt operations
  //
  Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,
                               NULL,
                               (VOID **)&GraphicsOutput);

  if ((EFI_SUCCESS != Status) ||
      (NULL        == GraphicsOutput))
  {
    DEBUG((EFI_D_ERROR, "DisplayUtils: Cannot locate GraphicsOutput.\r\n"));
    Status = EFI_UNSUPPORTED;
  }
  // Validate all inputs
  else if ((0 == GraphicsOutput->Mode->Info->HorizontalResolution) ||
           (0 == GraphicsOutput->Mode->Info->VerticalResolution))
  {
    DEBUG((EFI_D_ERROR, "DisplayUtils: Invalid inputs of horizontal / vertical resolution.\r\n"));
    Status = EFI_INVALID_PARAMETER;
  }
  else if ((NULL == ImageHeader) ||
           (0 == BGRTImageSize)  ||
           (NULL == BGRTImage)   ||
           (BGRTImageSize < sizeof(BMP_IMAGE_HEADER)))
  {
      // Invalid input parameters
      Status = EFI_INVALID_PARAMETER;
  }
  else if (('B' != ImageHeader->CharB) ||
           ('M' != ImageHeader->CharM) ||
           (0 != ImageHeader->CompressionType))
  {
      // Unsupported image format
      Status = EFI_UNSUPPORTED;

  }
   else if ((32 != ImageHeader->BitPerPixel) &&
            (24 != ImageHeader->BitPerPixel) &&
            (8  != ImageHeader->BitPerPixel))
   {
      // Unsupported pixel format
      Status = EFI_UNSUPPORTED;
   }
   else if (((ImageHeader->PixelWidth * ImageHeader->PixelHeight * ImageHeader->BitPerPixel) / 8) > (BGRTImageSize - sizeof(BMP_IMAGE_HEADER)))
   {
     // Resolution cannot be larger than the file itself minus the header
     Status = EFI_UNSUPPORTED;
   }
   else
   {
      UINT32         ImageHeight;
      UINT32         ImageWidth;
      UINT32         ImageStride;
      UINT8         *SrcAddress;
      INT32          SrcStride;
      UINT32         DestStride;
      UINT32        *ConversionBuffer;
      BMP_COLOR_MAP *ColorMap        = (BMP_COLOR_MAP *) (BGRTImage + sizeof (BMP_IMAGE_HEADER));

      //
      // This section of code handles conversion from the native BMP format to a GOP compatible format, this includes
      // - Conversion from 24bpp to 32bpp
      // - Handling an inverted image
      // - Handling BMP stride alignment of 4 bytes.
      //
      // Potential optimization to perform conversion using MDPBlt() API to use hardware to perform this conversion and blt
      // directly to the frame buffer.
      //
      //

      // Populate the image width and height
      ImageWidth = ImageHeader->PixelWidth;
      // Image source is 24bpp, but is always aligned to a dword (4 bytes)
      ImageStride = (ImageWidth * (ImageHeader->BitPerPixel/8));
      // Align stride on a dword boundary
      ImageStride = ((ImageStride)+3) & ~3;

      // Populate starting address, height and stride information, the source image could be stored
      // upside down, need to handle the case of an inverted image
      if ((INT32)ImageHeader->PixelHeight < 0)
      {
        // Image height is negative, indicating the image is in raster order, setup the pointers so we can
        // copy the image directly
        ImageHeight = (UINT32)- ImageHeader->PixelHeight;
        // Calculate the starting address which starts after the header
        SrcAddress = (UINT8*)((UINTN)BGRTImage + (UINT32)ImageHeader->ImageOffset);

        // Source stride is incremented by the image stride directly
        SrcStride = ImageStride;
      }
      else
      {
        // by default BMP images are stored upside down, setup the buffers to copy in the reverse direction
        ImageHeight = (UINT32)ImageHeader->PixelHeight;
        // Image source is 24bpp, but is always aligned to a dword (4 bytes)
        // Source image starts on the last line and we copy from the last line to the top
        SrcAddress =  (UINT8*)((UINTN)BGRTImage + (UINT32)ImageHeader->ImageOffset + (UINT32)((ImageHeight-1)*ImageStride));
        // Reverse stride since we are subtracting from the bottom
        SrcStride = -ImageStride;
      }

      // Calcualte the destination output stride.
      DestStride  = ImageWidth * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL);

      // Allocate a temporary buffer that is compatible with the GOP protocol
      ConversionBuffer = AllocatePool(ImageHeight * DestStride);

      if (NULL == ConversionBuffer)
      {
        // Cannot allocate a conversion buffer
        Status = EFI_OUT_OF_RESOURCES;
      }
      else
      {
        UINT32 Count;
        //
        // If the OEM supports two logos (logo1.bmp & logo2.bmp), they should use the BGRT table to override the offsets for logo2.bmp.
        //
        UINT32 LogoPosX     = 0;
        UINT32 LogoPosY     = 0;
        UINT8 *DestAddress  = (UINT8*)ConversionBuffer;

        CalculateLogoPosition(ImageWidth, ImageHeight, &LogoPosX, &LogoPosY, pRenderLogoParams);

        // Copy line by line to handle inverted images and images that have padding (due to odd widths)
        //
        for (Count=0;Count<ImageHeight;Count++)
        {
          // Perform version, all pixel depths are in bits, so multiply byte size by 8
          ConversionPixelCopy(ImageHeader->BitPerPixel, 
                              ColorMap,
                              SrcAddress, 
                              (sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)*8), 
                              DestAddress, 
                              ImageWidth);

          // Increment the source and destination pointers by their respective strides.
          DestAddress += DestStride;
          SrcAddress += SrcStride;
        }

        //
        // Setup blt of conversion buffer to the screen
        //
        if (EFI_SUCCESS != (Status = GraphicsOutput->Blt(GraphicsOutput,
                                                        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)&bgPixel,
                                                        EfiBltVideoFill,
                                                        0, 0,
                                                        0, 0,
                                                        ImageWidth,
                                                        ImageHeight,
                                                        0)))
        {
          DEBUG((EFI_D_ERROR, "DisplayUtils: Blt(EfiBltVideoFill) failed.\r\n"));
          return Status;
        }

        Status = GraphicsOutput->Blt (GraphicsOutput,
                                      (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)ConversionBuffer,
                                      EfiBltBufferToVideo,
                                      0,
                                      0,
                                      (UINTN) LogoPosX,
                                      (UINTN) LogoPosY,
                                      ImageWidth,
                                      ImageHeight,
                                      ImageWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

        if (!EFI_ERROR (Status)) {
          //
          // Recording the boot logo information to EFI_BOOT_LOGO_PROTOCOL/EDKII_BOOT_LOGO2_PROTOCOL for capsule update process
          // This is a void function because it will not block capsule update process
          //
          UpdateLogoParameters ((EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)ConversionBuffer,(UINTN) LogoPosX,(UINTN) LogoPosY,(UINTN) ImageWidth,(UINTN) ImageHeight);
        }

        //
        // Free the conversion buffer
        //
        FreePool(ConversionBuffer);
      }
  }

  return Status;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_RenderSplashScreen()
*/
/*!
* \brief
*   Load, process and display OEM logo bitmaps including processing needed to
*   support ACPI2.0 BGRT.
*
*   - Display_Utils_RenderSplashScreen is called with NULL which will render the logo
*     (found in FV or HLOS partitions).
*   - Display_Utils_RenderSplashScreen is called with valid BGRT will load the 2nd logo
*     and update the BGRT table.  
*
* \param[out]  BGRTTable         Pointer to the BGRT table in ACPI, if NULL the
*                                primary logo is rendered with no BGRT information returned.
*       [in]   pRenderLogoParams Render logo parameters.
*
* \retval EFI_STATUS
* EFI_SUCCESS                  : Image loaded successfully in to memory.
* EFI_INVALID_PARAMETER        : Invalid input parameters passed in.
* EFI_OUT_OF_RESOURCES         : Not enough resources for buffer allocations.
*
****************************************************************************/
EFI_STATUS
EFIAPI
Display_Utils_RenderSplashScreen(
  OUT VOID             *BGRTTable OPTIONAL,
  RenderLogoParamsType *pRenderLogoParams)
{
  EFI_STATUS                                        Status          = EFI_SUCCESS;
  static EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE *pLocalBGRTTable = NULL;

  // Handle cases where the BGRT is not yet loaded or not applicable
  if(NULL == BGRTTable)
  {
     // Allocate space for the local cached copy of the BGRT table if it doesn't exist
     if (NULL == pLocalBGRTTable)
     {
        if (NULL == (pLocalBGRTTable  = (EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE*) AllocateZeroPool(sizeof(EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE))))
        {
            DEBUG ((EFI_D_ERROR, "Display_Utils_RenderSplashScreen: Unable to allocate memory for local BGRT table!\n"));
            Status = EFI_OUT_OF_RESOURCES;
        }
     }

     // Load and render the logo1.bmp, cache the BGRT information till a valid BGRT is passed in.
     if (EFI_SUCCESS == Status)
     {
       UINT8        *BGRTAddress     = NULL;
       CHAR8        *pLogoFileName   = NULL;
       UINT32        ImageSize       = 0;
       UINT32        BGRTBufferSize  = 0;
       UINT32        uFlags          = 0;
       UINT32        uImageFvSetting = 0;

       if ((MDP_STATUS_OK == MDP_GetConfigValue("EnableDisplayImageFv", &uImageFvSetting)) &&
           (1             == uImageFvSetting))
       {
         // Set imagefv flag if logo policy is 1, which indicates loading logo from imagefv system.
         uFlags = DISP_LOAD_FILE_FLAG_IMAGEFV;
         pLogoFileName = "logo_custom.bmp";
       }
       else
       {
         pLogoFileName = "logo1.bmp";
       }

       // Load logo1, from plat partition or the main FV
       Status = Display_Utils_LoadFile(pLogoFileName, &BGRTAddress, &BGRTBufferSize, &ImageSize, uFlags);

       if (EFI_SUCCESS == Status)
       {
         // Render logo1.bmp after loading it.
         if (EFI_SUCCESS == (Status = Display_Utils_RenderBGRTImage(BGRTAddress, ImageSize, pRenderLogoParams)))
         {
           BMP_IMAGE_HEADER *ImageHeader = (BMP_IMAGE_HEADER*)BGRTAddress;
           UINT32            ImageWidth  = ImageHeader->PixelWidth;
           UINT32            ImageHeight = ((INT32)ImageHeader->PixelHeight < 0) ? ((UINT32)- ImageHeader->PixelHeight):ImageHeader->PixelHeight;

           DEBUG ((EFI_D_INFO, "Display_Utils_RenderSplashScreen: OEM Logo1 Successfully Loaded\n"));
         
           // Update the BGRT position, store it in the cached version of the BGRT
           CalculateLogoPosition(ImageWidth, ImageHeight, &pLocalBGRTTable->ImageOffsetX, &pLocalBGRTTable->ImageOffsetY, pRenderLogoParams);
           
           // As logo1.bmp is rendered to the panel, set the status field to 1.
           pLocalBGRTTable->Status       = 0x1;
           // Update the BGRT address
           pLocalBGRTTable->ImageAddress = (UINT64)BGRTAddress;
         }      
       } 
     }
  }
  else  // Valid BGRT passed in, validate is and process it.
  {
    EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE  *pBGRTTable = (EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE *)BGRTTable;  
    
    if ((EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE_SIGNATURE != pBGRTTable->Header.Signature) || // "BGRT" Signature
        (EFI_ACPI_5_0_BGRT_VERSION                           != pBGRTTable->Version)          || // Valid version 1.0
        (EFI_ACPI_5_0_BGRT_IMAGE_TYPE_BMP                    != pBGRTTable->ImageType))          // Valid type BMP
    {
      DEBUG ((EFI_D_ERROR, "Display_Utils_RenderSplashScreen: BGRT table header corrupt or invalid!\n"));
      Status = EFI_LOAD_ERROR;
    }
    else
    {
      UINT32        ImageSize      = 0;
      UINT8        *BGRTAddress    = NULL;
      UINT32        BGRTBufferSize = 0;

      if (EFI_SUCCESS == Display_Utils_LoadFile("logo2.bmp", &BGRTAddress, &BGRTBufferSize, &ImageSize, 0))
      {
        BMP_IMAGE_HEADER *ImageHeader    = (BMP_IMAGE_HEADER*)BGRTAddress;
        UINT32            ImageWidth     = ImageHeader->PixelWidth;
        UINT32            ImageHeight    = ((INT32)ImageHeader->PixelHeight < 0) ? ((UINT32)- ImageHeader->PixelHeight):ImageHeader->PixelHeight;

        DEBUG ((EFI_D_INFO, "Display_Utils_RenderSplashScreen: OEM Logo2 Successfully Loaded\n"));

        // Update the BGRT position
        CalculateLogoPosition(ImageWidth, ImageHeight, &pBGRTTable->ImageOffsetX, &pBGRTTable->ImageOffsetY, pRenderLogoParams);

        // As logo2.bmp is loaded into the BGRT address, set the status to 0 to give an indication to OS to render this image.
        pBGRTTable->Status       = 0x0;
        // Update the BGRT address
        pBGRTTable->ImageAddress = (UINT64)BGRTAddress;
      }
      else
      {
        // logo2 not found, pass information loaded during logo1 back to the caller's BGRT table
        if (NULL != pLocalBGRTTable)
        {
          pBGRTTable->ImageOffsetX = pLocalBGRTTable->ImageOffsetX;
          pBGRTTable->ImageOffsetY = pLocalBGRTTable->ImageOffsetY;
          pBGRTTable->Status       = pLocalBGRTTable->Status;
          pBGRTTable->ImageAddress = pLocalBGRTTable->ImageAddress;
        }
        else
        {
          DEBUG ((EFI_D_ERROR, "Display_Utils_RenderSplashScreen: BGRT was not loaded correctly, table cannot be updated!\n"));
        }
      }
    }
  }

  return Status;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_RenderLogo()
*/
/*!
* \brief
*   Display Utils Protocol Render logo function.
*
* \param[in]  psRenderLogoConfig  Configurations of clearing screen and overriding position.
*
* \retval EFI_STATUS
* EFI_SUCCESS                   : Function returned successfully.
* EFI_INVALID_PARAMETER         : Parameter passed is incorrect.  
* EFI_UNSUPPORTED               : Parameter passed is not supported. 
* EFI_DEVICE_ERROR              : Physical device reported an error.
*
****************************************************************************/
EFI_STATUS
Display_Utils_RenderLogo(
  IN  EFI_DISPLAY_UTILS_RENDERLOGO_CONFIG *psRenderLogoConfig)
{
  EFI_STATUS                     eStatus           = EFI_SUCCESS;

  if (NULL == psRenderLogoConfig)
  {
    // No configurations for rendering logo, just render the default logo
    if (EFI_SUCCESS != (eStatus = Display_Utils_RenderSplashScreen(NULL, NULL)))
    {
      DEBUG((EFI_D_WARN, "DisplayUtils: Display_Utils_RenderSplashScreen failed with Status(%d)!\n", eStatus));
    }
  }
  else
  {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL  bgPixel;
    RenderLogoParamsType           sRenderLogoParams;
    EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput = NULL;

    MDP_OSAL_MEMZERO(&bgPixel,           sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    MDP_OSAL_MEMZERO(&sRenderLogoParams, sizeof(RenderLogoParamsType));

    // Clear Screen
    if (psRenderLogoConfig->uFlags & DISPLAY_UTILS_RENDERLOGO_CLEARSCREEN)
    {
      eStatus = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,
                                    NULL,
                                   (void**)&GraphicsOutput);
      if ((eStatus != EFI_SUCCESS)                                      ||
          (NULL    == GraphicsOutput)                                   ||
          (0       == GraphicsOutput->Mode->Info->HorizontalResolution) ||
          (0       == GraphicsOutput->Mode->Info->VerticalResolution))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Locate Protocol or Get Resolution failed!\n"));
      }
      else
      {
        GraphicsOutput->Blt(GraphicsOutput,
                            (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)&bgPixel,
                            EfiBltVideoFill,
                            0, 0,
                            0, 0,
                            GraphicsOutput->Mode->Info->HorizontalResolution,
                            GraphicsOutput->Mode->Info->VerticalResolution,
                            0);
      }
    }

    // Override Logo Position
    if (psRenderLogoConfig->uFlags & DISPLAY_UTILS_RENDERLOGO_POSITION_OVERRIDE)
    {
      sRenderLogoParams.uXPosition  = psRenderLogoConfig->xPosition;
      sRenderLogoParams.uYPosition  = psRenderLogoConfig->yPosition;
      sRenderLogoParams.uFlags     |= RENDER_LOGO_POSITION_OVERRIDE;
    }

    // Render BGRT Image
    if(psRenderLogoConfig->uFlags & DISPLAY_UTILS_RENDERLOGO_RENDER_BGRT_IMAGE)
    {
      if (EFI_SUCCESS != (eStatus = Display_Utils_RenderBGRTImage(psRenderLogoConfig->BGRTImage,
                                                                  psRenderLogoConfig->BGRTImageSize,
                                                                  &sRenderLogoParams)))
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Render BGRT Image failed with Status(%d)!\n", eStatus));
      }
    }
    // Render Splash Screen
    else if (EFI_SUCCESS != (eStatus = Display_Utils_RenderSplashScreen(psRenderLogoConfig->BGRTTable,
                                                                        &sRenderLogoParams)))
    {
      DEBUG((EFI_D_WARN, "DisplayUtils: Render Splash Screen failed with Status(%d)!\n", eStatus));
    }
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: Display_Utils_SetMode()
*/
/*!
* \brief
*   Display Utils Protocol to set display mode
*
  @param[in]  DisplayId                 display id, can be either primary, secondary or external
  @param[in]  ModeNumber                Abstraction that defines the current video mode.
  @param[in]  Flags                     More info
*
* \retval EFI_STATUS
* EFI_SUCCESS                   : Function returned successfully.
* EFI_INVALID_PARAMETER         : Parameter passed is incorrect.  
* EFI_UNSUPPORTED               : Parameter passed is not supported. 
* EFI_DEVICE_ERROR              : Physical device reported an error.
*
****************************************************************************/
EFI_STATUS
Display_Utils_SetMode(
  IN  UINT32           DisplayId,
  IN  UINT32           ModeNum,
  IN  UINT32           Flags)
{
    EFI_STATUS    eStatus        = EFI_NOT_FOUND;

     switch(DisplayId)
     {
       case MDP_DISPLAY_EXTERNAL:  /* mini DP */
       case MDP_DISPLAY_EXTERNAL2: /* DP over USB-0 type-c */
       case MDP_DISPLAY_EXTERNAL3: /* DP over USB-1 type-c */
          eStatus = DisplayDxe_SetModeExt(DisplayId, ModeNum, Flags);
          break;
       default:
          DEBUG((EFI_D_WARN, "Display_Utils_SetMode: DisplayId=%d NOT Supported\n", DisplayId));
          break;
     }

    return eStatus;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_StorePLLCodes()
*/
/*!
* \brief
*   Display Utils to store pll codes
*
  @param  None
*
* \retval None
*
****************************************************************************/
void
Display_Utils_StorePLLCodes(void)
{
  UINT32               uSplashAddr          = 0;
  UINT32               uSplashSize          = 0;
  uintPtr              uPllCodesAddr        = 0;
  UINT32               uPllCodesMaxSize     = MDP_MAX_PLLCODES_SIZE;
  void                *pBuffer              = NULL;
  MDP_PLLCodesInfo     sPllCodesInfo[MDP_MAX_NUM_DSI_DEVICE];
  
  /* Save PLL codes to last 4K of framebuffer for kernel to read */
  if((EFI_SUCCESS == Display_Utils_GetContinuousSplashInfo(&uSplashAddr, &uSplashSize)) &&
    (uSplashSize   > uPllCodesMaxSize))
  {
    uPllCodesAddr = uSplashAddr + uSplashSize - uPllCodesMaxSize;
    pBuffer       = (void*)uPllCodesAddr;

    if (sizeof(sPllCodesInfo) > uPllCodesMaxSize)
    {
      MDP_Log_Message(MDP_LOGLEVEL_WARN, "Error: Not enough splash memory to store pll codes\n");
    }
    else
    {
      uint32 i;
    
      MDP_OSAL_MEMSET(&sPllCodesInfo[0], 0, sizeof(sPllCodesInfo));
    
      for (i = 0; i < MDP_MAX_NUM_DSI_DEVICE; i++)
      {
        if (MDP_STATUS_OK == DSIDriver_GetCalibrationCodes(i, &sPllCodesInfo[i]))
        {
          MDP_PLLCodesInfo *pPllCodes = (MDP_PLLCodesInfo *)pBuffer;
        
          MDP_OSAL_MEMCPY(&pPllCodes[i], (void*)&sPllCodesInfo[i], sizeof(sPllCodesInfo[0]));
        }
      }
    }
  }
}


/****************************************************************************
*
** FUNCTION: Display_Utils_SetVariable()
*/
/*!
* \brief
*   Display Utils Protocol to set the variable value
*
  @param[in]  pVariableName              The variable name that need to set.
  @param[in]  pVariableValue             The variable value.
  @param[in]  uDataLength                The length of the variable value.
  @param[in]  uFlags                     The flag to set variable.
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS Display_Utils_SetVariable (CHAR16 *pVariableName, UINT8 *pVariableValue, UINTN uDataLength, UINTN uFlags)
{
  EFI_STATUS eStatus = EFI_SUCCESS;

  if (RETAIL)
  {
    // Do nothing here but just return EFI_NOT_FOUND for RETAIL to indicate nothing to be set.
    eStatus = EFI_NOT_FOUND;
  }
  else if ((NULL == pVariableName) ||
           (NULL == pVariableValue))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: variable name or value buffer is NULL!\n"));
    eStatus = EFI_INVALID_PARAMETER;
  }
  else if ((StrLen(pVariableName) > (DISP_FILE_NAME_MAX_SIZE - StrLen(DISP_FILE_EXT) - 1)) ||
           (uDataLength           >  DISP_VARIABLE_VALUE_MAX_SIZE))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: variable name or value size is out of limit!\n"));
    eStatus = EFI_INVALID_PARAMETER;
  }
  else if (EFI_SUCCESS != (eStatus = MountFatPartition(DISP_LOGFS)))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: mount logfs system failed with status %d!\n", eStatus));
  }
  else
  {
    CHAR16      DisplayConfigFile[DISP_FILE_NAME_MAX_SIZE];
    UINT32      uFileNameSize   = 0;
    UINTN       uWriteDataSize  = uDataLength * sizeof (UINT8);

    uFileNameSize = StrLen(pVariableName) * sizeof (CHAR16);

    MDP_OSAL_MEMZERO(DisplayConfigFile, sizeof(DisplayConfigFile));
    MDP_OSAL_MEMCPY(DisplayConfigFile, pVariableName, uFileNameSize);

    StrCat(DisplayConfigFile, DISP_FILE_EXT);

    /* Clear the configuration file before writing it if already exists */
    DeleteFile (DisplayConfigFile,
                NULL,
                NULL,
                DISP_LOGFS,
                TRUE,
                NULL);

    if ((NULL != pVariableValue) &&
        (0    != uWriteDataSize))
    {  
      eStatus = WriteFile (DisplayConfigFile,
                           NULL,
                           NULL,
                           DISP_LOGFS,
                           TRUE,
                           NULL,
                           &uWriteDataSize,
                           0,
                           pVariableValue,
                           uWriteDataSize);

      if(eStatus != EFI_SUCCESS)
      {
        DEBUG((EFI_D_WARN, "DisplayUtils: Saving variable(%s) failed (status:%d)\n", pVariableName, eStatus));
      }
    } 
  }

  return eStatus;
}


/****************************************************************************
*
** FUNCTION: Display_Utils_GetVariable()
*/
/*!
* \brief
*   Display Utils Protocol to get the variable value
*
  @param[in]   pVariableName              The variable name that need to get.
  @param[out]  pVariableValue             The variable value.
  @param[out]  pDataLength                The length of the variable value.
  @param[in]   uFlags                     The flag to get variable.
*
* \retval EFI_STATUS
*
****************************************************************************/
EFI_STATUS Display_Utils_GetVariable(CHAR16 *pVariableName, UINT8 *pVariableValue, UINTN *pDataLength, UINTN uFlags)
{
  EFI_STATUS  eStatus = EFI_SUCCESS;

  if (RETAIL)
  {
    // Do nothing here but just return EFI_NOT_FOUND for RETAIL to indicate nothing to be read.
    eStatus = EFI_NOT_FOUND;
  }
  else if ((NULL == pVariableName)  ||
           (NULL == pVariableValue) ||
           (NULL == pDataLength))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: variable name or value buffer is NULL!\n"));
    eStatus = EFI_INVALID_PARAMETER;
  }
  else if (StrLen(pVariableName) > (DISP_FILE_NAME_MAX_SIZE - StrLen(DISP_FILE_EXT) - 1))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: variable name is too long!\n"));
    eStatus = EFI_INVALID_PARAMETER;
  }
  else if (EFI_SUCCESS != (eStatus = MountFatPartition(DISP_LOGFS)))
  {
    DEBUG((EFI_D_WARN, "DisplayUtils: mount logfs system failed with status %d!\n", eStatus));
  }
  else
  {
    CHAR16      DisplayConfigFile[DISP_FILE_NAME_MAX_SIZE];
    UINT32      uFileNameSize  = 0;
    UINTN       uReadDataSize  = (*pDataLength) * sizeof (UINT8);

    uFileNameSize = StrLen(pVariableName) * sizeof (CHAR16);

    MDP_OSAL_MEMZERO(DisplayConfigFile, sizeof(DisplayConfigFile));
    MDP_OSAL_MEMCPY(DisplayConfigFile, pVariableName, uFileNameSize);

    StrCat(DisplayConfigFile, DISP_FILE_EXT);

    eStatus = ReadFile (DisplayConfigFile,
                        NULL,
                        NULL,
                        DISP_LOGFS,
                        TRUE,
                        NULL,
                        &uReadDataSize,
                        0,
                        pVariableValue,
                        uReadDataSize);

    *pDataLength = AsciiStrLen((CHAR8 *)pVariableValue);

    if ((EFI_NOT_FOUND == eStatus)     ||
        (0             == *pDataLength))
    {
      // Variable not found
      eStatus = EFI_NOT_FOUND;
    }
    else if(EFI_SUCCESS != eStatus)
    {
      DEBUG((EFI_D_WARN, "DisplayUtils: Reading variable(%s) failed (status:%d)\n", pVariableName, eStatus));
    }
  }

  return eStatus;
}
