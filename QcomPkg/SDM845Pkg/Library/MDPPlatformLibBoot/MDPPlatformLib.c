/*=============================================================================
 
  File: MDPPlatformLib.c
 
  MDP platform specific functions
  
  Copyright (c) 2016-2017 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#include <PlatformInfo.h>
#include <Library/PcdLib.h>
#include <api/pmic/pm/pm_ibb.h>
#include <api/pmic/pm/pm_lab.h>
#include <api/pmic/pm/pm_wled.h>
#include <api/pmic/pm/pm_ldo.h>
#include <api/pmic/pm/pm_boot.h>
#include <api/dal/DALStdErr.h>
#include "MDPTypes.h"
#include "MDPPlatformLib.h"
#include "MDPSystem.h"
#include "DDITlmm.h"
#include "pmapp_npa.h"
#include "npa.h"

/* -----------------------------------------------------------------------
** Defines
** ----------------------------------------------------------------------- */

#define PCD_PANEL_TYPE_OEM                1

#define DSI_READ_ADDRESS_SIZE             2
#define DSI_READ_READBACK_SIZE            8

#define PLATFORM_PANEL_ID_MAX_COMMANDS    3       // maximum panel ID read commands

// PMIC Device Indices
#define PMIC_DEV_INDEX                    0       // PMIC device (Vreg, LDO, ect)
#define PMIC_PMI_DEV_INDEX                1       // PMIC interface device (IBB/LAB, GPIO,  WLED)

#define PMIC_IBBLAB_READYTIME             25      // Maximum of milliseconds to wait for IBB/LAB to stabilize (Can be up to 8.8ms +/-16%)

/* 8998 Main Panel CDP specific defines
*/
#define CDP_DISP_RESET_GPIO                6      //GPIO number for LCD0_RESET
#define CDP_DISP_TE_GPIO                  10      //GPIO number for TE
#define CDP_DISP_MODE_SEL                 52      //GPIO for panel mode select 0 = Dual Port, 1 = Single Port (DSC)

/* -----------------------------------------------------------------------
** Types
** ----------------------------------------------------------------------- */

/* Panel command ID list for dynamic panel detection
*/

typedef struct {
  uint8      address[DSI_READ_ADDRESS_SIZE];                                        // DCS command for panel ID
  uint8      expectedReadback[DSI_READ_READBACK_SIZE];                              // expected readback
} PlatformPanelIDCommandInfo;

/* Dynamic panel ID detection entry 
 */
typedef struct {
  uint8                         uCmdType;                                             // data type for panel ID DSI read
  uint32                        uTotalRetry;                                          // number of retry if DSI read fails
  PlatformPanelIDCommandInfo    panelIdCommands[PLATFORM_PANEL_ID_MAX_COMMANDS];      // commands for panel ID and expected readback
  uint32                        uLaneRemapOrder;                                      // Lane remap order
  const int8                   *psPanelCfg;                                           // pointer to the panel configuration
  uint32                        uPanelCfgSize;                                        // size of panel configuration data
  MDPPlatformPanelType          eSelectedPanel;                                       // index of the selected panel
  uint32                        uFlags;                                               // flags to set clock config for now, can set other config in future
} PlatformDSIDetectParams;

/* Power configuration 
 */
typedef struct
{
  MDP_PmicModuleControlType     ePMICSecondaryPower[MDP_DISPLAY_MAX];                 // Configuration for PMIC based secondary power source
  uint32                        uResetGpio;
} Panel_PowerCtrlParams;

/* NPA Client info
*/
typedef struct
{
  char*                         cResourceName;
  char*                         cClientName;
}NPAClientName;

/* -----------------------------------------------------------------------
** Local functions
** ----------------------------------------------------------------------- */

/* Platform detection 
*/
static MDP_Status ReadPlatformIDAndChipID(EFI_PLATFORMINFO_PLATFORM_INFO_TYPE *pPlatformInfo, EFIChipInfoIdType *pChipSetId, EFIChipInfoFamilyType *pChiSetFamily);


/* 8998 Main Panel CDP Functions
*/
static MDP_Status Panel_CDP_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
static MDP_Status Panel_CDP_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
static MDP_Status Panel_CDP_Reset(MDPPlatformPanelResetInfo *pResetInfo);
static MDP_Status Panel_CDP_BacklightLevel(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig);
static MDP_Status Panel_CDP_PeripheralPower(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp);
static void Panel_IBBLAB_WaitForReady();

/* Main Panel CDP/MTP Functions
*/

/* 8996 HDMI Panel Functions
*/

/* General helper functions */
MDP_Status        SetDefaultGPIOState(GpioStateType *pGPIOList);
static MDP_Status GetPmicIBBLABMode(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);

/* -----------------------------------------------------------------------
**  declaration for pmic shared fucntion 
** ----------------------------------------------------------------------- */

/************************************************************************
NOTE: 
1) Only panels with dual dsi controller are supported here since CDP_DISP_MODE_SEL is set to low.
2) For single dsi controller panel, CDP_DISP_MODE_SEL must be set to high
***************************************************************************/

/*===========================================================================
 Local Configuration Definitions
 ===========================================================================*/
/******************************************************************
 *                   Sharp 5.5" 4k Panel                            *
 *                                                                *
 *                   nt35950 DDIC                                 *
 *                   2160x3840                                    *
 *                   Dual-DSI(DSI0 and DSI1)                      *
 *                   Command mode DSC                             *
 ******************************************************************/
const static int8 Sharp_4k_cmd_dsc_xmldata[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"<PanelName>LS055D1SX04</PanelName>"
"<PanelDescription>Sharp Dual DSI Command Mode DSC Panel (2160x3840 24bpp)</PanelDescription>"
"<Group id=\"Active Timing\">"
" <HorizontalActive>2160</HorizontalActive>"
" <HorizontalFrontPorch>30</HorizontalFrontPorch>"
" <HorizontalBackPorch>100</HorizontalBackPorch>"
" <HorizontalSyncPulse>4</HorizontalSyncPulse>"
" <HorizontalSyncSkew>0</HorizontalSyncSkew>"
" <HorizontalLeftBorder>0</HorizontalLeftBorder>"
" <HorizontalRightBorder>0</HorizontalRightBorder>"
" <VerticalActive>3840</VerticalActive>"
" <VerticalFrontPorch>8</VerticalFrontPorch>"
" <VerticalBackPorch>7</VerticalBackPorch>"
" <VerticalSyncPulse>1</VerticalSyncPulse>"
" <VerticalSyncSkew>0</VerticalSyncSkew>"
" <VerticalTopBorder>0</VerticalTopBorder>"
" <VerticalBottomBorder>0</VerticalBottomBorder>"
" <InvertDataPolarity>False</InvertDataPolarity>"
" <InvertVsyncPolairty>False</InvertVsyncPolairty>"
" <InvertHsyncPolarity>False</InvertHsyncPolarity>"
" <BorderColor>0x0</BorderColor>"
"</Group>"
"<Group id=\"Display Interface\">"
" <InterfaceType>9</InterfaceType>"
" <InterfaceColorFormat>3</InterfaceColorFormat>"
"</Group>"
"<Group id=\"DSI Interface\">"
" <DSIChannelId>2</DSIChannelId>"
" <DSIVirtualId>0</DSIVirtualId>"
" <DSIColorFormat>36</DSIColorFormat>"
" <DSITrafficMode>1</DSITrafficMode>"
" <DSILanes>4</DSILanes>"
" <DSIRefreshRate>0x3C0000</DSIRefreshRate>"
" <DSICmdSwapInterface>False</DSICmdSwapInterface>"
" <DSICmdUsingTrigger>False</DSICmdUsingTrigger>"
" <DSIEnableAutoRefresh>True</DSIEnableAutoRefresh>"
" <DSIAutoRefreshFrameNumDiv>2</DSIAutoRefreshFrameNumDiv>"
" <DSITECheckEnable>True</DSITECheckEnable>"
" <DSITEUsingDedicatedTEPin>True</DSITEUsingDedicatedTEPin>"
" <DSITEvSyncStartPos>3840</DSITEvSyncStartPos>"
" <DSITEvSyncInitVal>3840</DSITEvSyncInitVal>"
" <DSICmdModePixelAlignment>1</DSICmdModePixelAlignment>"
" <DSIDSCEnable>True</DSIDSCEnable>"
" <DSIDSCMajor>1</DSIDSCMajor>"
" <DSIDSCMinor>1</DSIDSCMinor>"
" <DSIDSCScr>0</DSIDSCScr>"
" <DSIDSCProfileID>4</DSIDSCProfileID>"
" <DSIDSCSliceWidth>1080</DSIDSCSliceWidth>"
" <DSIDSCSliceHeight>32</DSIDSCSliceHeight>"
" <DSIControllerMapping>\n"
"  00 01\n"
" </DSIControllerMapping>\n" 
"</Group>"
"<DSIInitSequence>"
" 39 91 09 20 00 20 02 00 03 1c 04 21 00 0f 03 19 01 97\n"
" 39 92 10 f0\n"
" 15 90 03\n"
" 15 03 01\n"
" 39 f0 55 aa 52 08 04\n"
" 15 c0 03\n"
" 39 f0 55 aa 52 08 07\n"
" 15 ef 01\n"
" 39 f0 55 aa 52 08 00\n"
" 15 b4 01\n"
" 15 35 00\n"
" 39 f0 55 aa 52 08 01\n"
" 39 ff aa 55 a5 80\n"
" 15 6f 01\n"
" 15 f3 10\n"
" 39 ff aa 55 a5 00\n"
" 05 11\n"
" ff 78\n"
" 05 29\n"
" ff 78\n"
"</DSIInitSequence>"
"<TLMMGPIODefaultLow>34</TLMMGPIODefaultLow>"
"<Group id='Backlight Configuration'>"
" <BacklightType>1</BacklightType>\n"
" <BacklightPmicControlType>2</BacklightPmicControlType>\n"  
"</Group>\n";

/******************************************************************
 *           Truly 5.5in 2k  TFT2P2827-E                          *
 *                                                                *
 *                  nt35597 DDIC                                  *
 *                   1440x2560                                    *
 *                   Dual-DSI                                     *
 *                   Command mode                                   *
 ******************************************************************/
const static int8 Truly_TFT2P2827_E_cmd_xmldata[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"<PanelName>TFT2P2827-E</PanelName>"
"<PanelDescription>Truly Dual DSI Command Mode Panel (1440x2560 24bpp)</PanelDescription>"
"<Group id=\"Active Timing\">"
" <HorizontalActive>1440</HorizontalActive>"
" <HorizontalFrontPorch>100</HorizontalFrontPorch>"
" <HorizontalBackPorch>32</HorizontalBackPorch>"
" <HorizontalSyncPulse>16</HorizontalSyncPulse>"
" <HorizontalSyncSkew>0</HorizontalSyncSkew>"
" <HorizontalLeftBorder>0</HorizontalLeftBorder>"
" <HorizontalRightBorder>0</HorizontalRightBorder>"
" <VerticalActive>2560</VerticalActive>"
" <VerticalFrontPorch>8</VerticalFrontPorch>"
" <VerticalBackPorch>7</VerticalBackPorch>"
" <VerticalSyncPulse>1</VerticalSyncPulse>"
" <VerticalSyncSkew>0</VerticalSyncSkew>"
" <VerticalTopBorder>0</VerticalTopBorder>"
" <VerticalBottomBorder>0</VerticalBottomBorder>"
" <InvertDataPolarity>False</InvertDataPolarity>"
" <InvertVsyncPolairty>False</InvertVsyncPolairty>"
" <InvertHsyncPolarity>False</InvertHsyncPolarity>"
" <BorderColor>0x0</BorderColor>"
"</Group>"
"<Group id=\"Display Interface\">"
" <InterfaceType>9</InterfaceType>"
" <InterfaceColorFormat>3</InterfaceColorFormat>"
"</Group>"
"<Group id=\"DSI Interface\">"
" <DSIChannelId>2</DSIChannelId>"
" <DSIVirtualId>0</DSIVirtualId>"
" <DSIColorFormat>36</DSIColorFormat>"
" <DSITrafficMode>1</DSITrafficMode>"
" <DSILanes>4</DSILanes>"
" <DSIRefreshRate>0x3C0000</DSIRefreshRate>"
" <DSICmdSwapInterface>False</DSICmdSwapInterface>"
" <DSICmdUsingTrigger>False</DSICmdUsingTrigger>"
" <DSIEnableAutoRefresh>True</DSIEnableAutoRefresh>"
" <DSIAutoRefreshFrameNumDiv>2</DSIAutoRefreshFrameNumDiv>"
" <DSIClockHSForceRequest>1</DSIClockHSForceRequest>"
" <DSITECheckEnable>True</DSITECheckEnable>"
" <DSITEUsingDedicatedTEPin>True</DSITEUsingDedicatedTEPin>"
" <DSITEvSyncStartPos>2560</DSITEvSyncStartPos>"
" <DSITEvSyncInitVal>2560</DSITEvSyncInitVal>"
" <DSICmdModePixelAlignment>1</DSICmdModePixelAlignment>"
" <DSIControllerMapping>\n"
"  00 01\n"
" </DSIControllerMapping>\n" 
"</Group>"
"<DSIInitSequence>"
" 15 FF 20\n"
" 15 fb 01\n"
" 15 00 01\n"
" 15 01 55\n"
" 15 02 45\n"
" 15 05 40\n"
" 15 06 19\n"
" 15 07 1E\n"
" 15 0B 73\n"
" 15 0C 73\n"
" 15 0E B0\n"
" 15 0F AE\n"
" 15 11 B8\n"
" 15 13 00\n"
" 15 58 80\n"
" 15 59 01\n"
" 15 5A 00\n"
" 15 5B 01\n"
" 15 5C 80\n"
" 15 5D 81\n"
" 15 5E 00\n"
" 15 5F 01\n"
" 15 72 31\n"
" 15 68 03\n"
" 15 ff 24\n"
" 15 fb 01\n"
" 15 00 1C\n"
" 15 01 0B\n"
" 15 02 0C\n"
" 15 03 01\n"
" 15 04 0F\n"
" 15 05 10\n"
" 15 06 10\n"
" 15 07 10\n"
" 15 08 89\n"
" 15 09 8A\n"
" 15 0A 13\n"
" 15 0B 13\n"
" 15 0C 15\n"
" 15 0D 15\n"
" 15 0E 17\n"
" 15 0F 17\n"
" 15 10 1C\n"
" 15 11 0B\n"
" 15 12 0C\n"
" 15 13 01\n"
" 15 14 0F\n"
" 15 15 10\n"
" 15 16 10\n"
" 15 17 10\n"
" 15 18 89\n"
" 15 19 8A\n"
" 15 1A 13\n"
" 15 1B 13\n"
" 15 1C 15\n"
" 15 1D 15\n"
" 15 1E 17\n"
" 15 1F 17\n"
" 15 20 40\n"
" 15 21 01\n"
" 15 22 00\n"
" 15 23 40\n"
" 15 24 40\n"
" 15 25 6D\n"
" 15 26 40\n"
" 15 27 40\n"
" 15 E0 00\n"
" 15 DC 21\n"
" 15 DD 22\n"
" 15 DE 07\n"
" 15 DF 07\n"
" 15 E3 6D\n"
" 15 E1 07\n"
" 15 E2 07\n"
" 15 29 D8\n"
" 15 2A 2A\n"
" 15 4B 03\n"
" 15 4C 11\n"
" 15 4D 10\n"
" 15 4E 01\n"
" 15 4F 01\n"
" 15 50 10\n"
" 15 51 00\n"
" 15 52 80\n"
" 15 53 00\n"
" 15 56 00\n"
" 15 54 07\n"
" 15 58 07\n"
" 15 55 25\n"
" 15 5B 43\n"
" 15 5C 00\n"
" 15 5F 73\n"
" 15 60 73\n"
" 15 63 22\n"
" 15 64 00\n"
" 15 67 08\n"
" 15 68 04\n"
" 15 72 02\n"
" 15 7A 80\n"
" 15 7B 91\n"
" 15 7C D8\n"
" 15 7D 60\n"
" 15 7F 15\n"
" 15 75 15\n"
" 15 B3 C0\n"
" 15 B4 00\n"
" 15 B5 00\n"
" 15 78 00\n"
" 15 79 00\n"
" 15 80 00\n"
" 15 83 00\n"
" 15 93 0A\n"
" 15 94 0A\n"
" 15 8A 00\n"
" 15 9B FF\n"
" 15 9D B0\n"
" 15 9F 63\n"
" 15 98 10\n"
" 15 EC 00\n"
" 15 ff 10\n"
" 39 3B 03 0A 0A\n"
" 15 35 00\n"
" 15 E5 01\n"
" 15 BB 10\n"
" 15 FB 01\n"
" 05 11 00\n"
" ff ff\n"
" 05 29 00\n"
"</DSIInitSequence>"
"<TLMMGPIODefaultLow>34</TLMMGPIODefaultLow>"
"<Group id='Backlight Configuration'>"
" <BacklightType>1</BacklightType>\n"
" <BacklightPmicControlType>2</BacklightPmicControlType>\n"  
"<DisplayPrimaryFlags units='integer'>0x400</DisplayPrimaryFlags>\n"
"</Group>\n";

/*
* Dummy panel configuration, default fallback mode.
*
*/
const static int8 dummy_xmldata[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"    <PanelName>VirtioDSI</PanelName>"
"    <PanelDescription>DSI Model (640x480 24bpp)</PanelDescription>"
"    <Group id=\"Active Timing\">"
"        <HorizontalActive units=\"Dot Clocks\">640</HorizontalActive>"
"        <HorizontalFrontPorch units=\"Dot Clocks\">13</HorizontalFrontPorch>"
"        <HorizontalBackPorch units=\"Dot Clocks\">14</HorizontalBackPorch>"
"        <HorizontalSyncPulse units=\"Dot Clocks\">15</HorizontalSyncPulse>"
"        <HorizontalSyncSkew units=\"Dot Clocks\">0</HorizontalSyncSkew>"
"        <HorizontalLeftBorder units=\"Dot Clocks\">0</HorizontalLeftBorder>"
"        <HorizontalRightBorder units=\"Dot Clocks\">0</HorizontalRightBorder>"
"        <VerticalActive units=\"Dot Clocks\">480</VerticalActive>"
"        <VerticalFrontPorch units=\"Lines\">11</VerticalFrontPorch>"
"        <VerticalBackPorch units=\"Lines\">12</VerticalBackPorch>"
"        <VerticalSyncPulse units=\"Lines\">13</VerticalSyncPulse>"
"        <VerticalSyncSkew units=\"Lines\">0</VerticalSyncSkew>"
"        <VerticalTopBorder units=\"Lines\">0</VerticalTopBorder>"
"        <VerticalBottomBorder units=\"Lines\">0</VerticalBottomBorder>"
"        <InvertDataPolarity>False</InvertDataPolarity>"
"        <InvertVsyncPolairty>False</InvertVsyncPolairty>"
"        <InvertHsyncPolarity>False</InvertHsyncPolarity>"
"        <BorderColor>0x0</BorderColor>"   
"    </Group>"
"    <Group id=\"Display Interface\">"
"        <InterfaceType units=\"QDI_DisplayConnectType\">8</InterfaceType>"
"        <InterfaceColorFormat units=\"QDI_PixelFormatType\">3</InterfaceColorFormat>"
"    </Group>"
"    <Group id=\"DSI Interface\">"
"        <DSIChannelId units=\"DSI_Channel_IDType\">1</DSIChannelId>"     
"        <DSIVirtualId units=\"DSI_Display_VCType\">0</DSIVirtualId>"     
"        <DSIColorFormat units=\"DSI_ColorFormatType\">36</DSIColorFormat>"     
"        <DSITrafficMode units=\"DSI_TrafficModeType\">0</DSITrafficMode>"
"        <DSILanes units=\"integer\">2</DSILanes>"
"        <DSIHsaHseAfterVsVe units=\"Bool\">False</DSIHsaHseAfterVsVe>"
"        <DSILowPowerModeInHFP units=\"Bool\">False</DSILowPowerModeInHFP>"
"        <DSILowPowerModeInHBP units=\"Bool\">False</DSILowPowerModeInHBP>"
"        <DSILowPowerModeInHSA units=\"Bool\">False</DSILowPowerModeInHSA>"
"        <DSILowPowerModeInBLLPEOF units=\"Bool\">False</DSILowPowerModeInBLLPEOF>"
"        <DSILowPowerModeInBLLP units=\"Bool\">False</DSILowPowerModeInBLLP>"
"        <DSIRefreshRate>0x3c0000</DSIRefreshRate>" 
"    </Group>"
"        <DisplayPrimaryFlags>0x2</DisplayPrimaryFlags>"
"    <DSIInitSequence>"
"    32 00 00\n"
"    </DSIInitSequence>";

/***************************************************************************
Dynamic panel detect list:
    -- first entry contains default panel

At SDM845 panel detection base on platform subtype
   -- subtype_0 use Truly 2k panel
       --- dual dsi (without dsc) video and cmd mode
       --- single dsi with DSC video and cmd mode

   -- subtype_1 use Sharp 4k panel
      --- dual dsi DSC with video and cmd mode
***************************************************************************/
static PlatformDSIDetectParams panelList_0[] = {    /* subtype == 0 */
    // Panel #1 - Truly DSI Command Mode Panel (1440x2560 24bpp
    {  
      0x06,                                                  // uCmdType
      0x05,                                                  // total number of retry on failures
      {
        {{0xDA, 0x00},                                       // address to read ID1
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }, 
        {{0xDB, 0x00},                                       // address to read ID2
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        },
        {{0xFE, 0x00},                                       // address to read ID3
        {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }
      },
      0,                                                     // Lane remap order {0, 1, 2, 3}
      Truly_TFT2P2827_E_cmd_xmldata,
      sizeof(Truly_TFT2P2827_E_cmd_xmldata),             // uPanelCfgSize
      MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_CMD,                 // eSelectedPanel 
      0                                                      // uFlags
    }
};

static PlatformDSIDetectParams panelList_1[] = {    /* subtype == 1 */
    // Panel #2 - Sharp 4k DSC Command Mode Panel (2160x3840 24bpp) [ Place holder, ID are not valid ]
    {  
      0x06,                                                  // uCmdType
      0x05,                                                  // total number of retry on failures
      {
        {{0xDA, 0x00},                                       // address to read ID1
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }, 
        {{0xDB, 0x00},                                       // address to read ID2
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        },
        {{0xDC, 0x00},                                       // address to read ID3
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }
      },
      0,                                                     // Lane remap order {0, 1, 2, 3}
      Sharp_4k_cmd_dsc_xmldata,                              // psPanelCfg (panel configuration)
      sizeof(Sharp_4k_cmd_dsc_xmldata),                      // uPanelCfgSize
      MDPPLATFORM_PANEL_SHARP_4K_DSC_CMD,                    // eSelectedPanel 
      0                                                      // uFlags
    }
};

/*===========================================================================
Function Definitions
===========================================================================*/

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: PlatformClientInit()
** 
** DESCRIPTION:
**        Initialize NPA client
**
*//* -------------------------------------------------------------------- */
static MDP_Status PlatformClientInit(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status    eStatus = MDP_STATUS_OK;

  return eStatus;
}



/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_PowerUp()
** 
** DESCRIPTION:
**        Panel power up sequence for CDP
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status Panel_CDP_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status        eStatus        = MDP_STATUS_OK;
  int ret;

  if (MDP_STATUS_OK != (eStatus =  PlatformClientInit(eDisplayId, pPowerParams)))
  {
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Failed to initialize handle for Primary display NPA node.\n");
  }
  else
  {
    DALGpioSignalType uGPIOCfg = 0;

    /* LDO1, LDO2 for DSI PHY/PLL are already turned on by system driver.*/

    /*Turn on LDO14 for V_LCD_VDDI_1P8 which is used by panel */
    ret = pm_ldo_volt_level(PMIC_DEV_INDEX, PM_LDO_14, 1800000);

    if (PM_ERR_FLAG__SUCCESS != ret)
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error configuring LDO to 1.8V, err=%d\n", ret);
    }

    ret = pm_ldo_sw_enable(PMIC_DEV_INDEX, PM_LDO_14, PM_ON);

    if (PM_ERR_FLAG__SUCCESS != ret)
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enabling LDO14, err=%d\n", ret);
    }

    MDP_OSAL_DELAYMS(10);   /* delay 10ms to allow power grid to settle */

    /* only support dual dsi control case */
    uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(CDP_DISP_MODE_SEL, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_LOW_VALUE);

    Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

    uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG(CDP_DISP_TE_GPIO, 1, DAL_GPIO_INPUT, DAL_GPIO_NO_PULL, DAL_GPIO_LOW_VALUE);
    Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);
      
  }

  return eStatus;
}


/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_PowerDown()
** 
** DESCRIPTION:
**        Panel power down sequence for CDP
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status Panel_CDP_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status          eStatus      = MDP_STATUS_OK;
  DALGpioSignalType   uGPIOCfg     = 0;
  /*Turn OFF LDO14 for V_LCD_VDDI_1P8 which is used by panel */
  if (PM_ERR_FLAG__SUCCESS != pm_ldo_sw_enable(PMIC_DEV_INDEX, PM_LDO_14, PM_OFF))
  {
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enabling LDO2\n");
  }


  uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(pPowerParams->uResetGpio, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_LOW_VALUE);
  Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_ResetStateExec()
** 
** DESCRIPTION:
**        toggle reset gpio base on states transition
**
*/ /* -------------------------------------------------------------------- */
static void Panel_ResetStateExec(MDPPlatformPanelResetInfo *pResetInfo, uint32 state0, uint32 state1, uint32 state2)
{
  DALGpioSignalType   uGPIOCfg    = 0;

  uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(pResetInfo->uResetGpio, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, state0);
  Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

   // Pull reset pin high with same time as assert time
   MDP_OSAL_DELAYUS(pResetInfo->uPreResetTimeUs);

  uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(pResetInfo->uResetGpio, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, state1);
  Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

  // Wait 10us, Reset_N(RESX) Need to be at least 10us to really trigger the (as per the panel spec)
  MDP_OSAL_DELAYUS(pResetInfo->uResetAssertTimeUs);
    
  uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(pResetInfo->uResetGpio, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, state2);
  Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_Reset()
** 
** DESCRIPTION:
**        Panel reset sequence for CDP
**
*/ /* -------------------------------------------------------------------- */
MDP_Status Panel_CDP_Reset(MDPPlatformPanelResetInfo *pResetInfo)
{
  MDP_Status          eStatus     = MDP_STATUS_OK;

    switch (pResetInfo->uAssertState)
    {
      case MDP_RESET_STATE_EDGE_LOW2HIGH:
         Panel_ResetStateExec(pResetInfo, DAL_GPIO_LOW_VALUE, DAL_GPIO_HIGH_VALUE, DAL_GPIO_HIGH_VALUE);
       break;
      case MDP_RESET_STATE_EDGE_HIGH2LOW:
         Panel_ResetStateExec(pResetInfo, DAL_GPIO_HIGH_VALUE, DAL_GPIO_LOW_VALUE, DAL_GPIO_LOW_VALUE);
       break;
      case MDP_RESET_STATE_LEVEL_HIGH:
         Panel_ResetStateExec(pResetInfo, DAL_GPIO_LOW_VALUE, DAL_GPIO_HIGH_VALUE, DAL_GPIO_LOW_VALUE);
       break;
      case MDP_RESET_STATE_LEVEL_LOW:
      default:
         Panel_ResetStateExec(pResetInfo, DAL_GPIO_HIGH_VALUE, DAL_GPIO_LOW_VALUE, DAL_GPIO_HIGH_VALUE);
       break;
    }

  MDP_OSAL_DELAYUS(pResetInfo->uPostResetTimeUs);

   /* 
    * post reset time: Timing space between end of reset to start sending init dcs cmds
    * NOTE:
    *      use DSIInitMasterTime to extend post reset time control if required
    */

  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_PeripheralPower()
** 
** DESCRIPTION:
**        Secondary power sequence for other PMIC modules such as IBB/LAB
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status Panel_CDP_PeripheralPower(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp)
{
  MDP_Status eStatus = MDP_STATUS_OK;

  if (MDP_STATUS_OK != (eStatus = GetPmicIBBLABMode(eDisplayId, pPowerParams)))
  {
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Getting PMIC mode failed with error(%d), skipping peripheral power configuration!\n", eStatus);
  }
  else if (MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD == pPowerParams->ePMICSecondaryPower[eDisplayId])
  {
    if (TRUE == bPowerUp)
    {
      // ********** Brightness Power-up Sequence **********
      // Power up
      /************************************************************************/
      /* Turn On IBB(+5.5v) first, wait for 8ms, turn on LAB(-5.5v)           */
      /************************************************************************/
      //Enable LCD mode
      if (PM_ERR_FLAG__SUCCESS != pm_ibb_lcd_amoled_sel(PMIC_PMI_DEV_INDEX, FALSE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to config IBB to LCD mode\n");
      }
      // Soft start charging register = 64k Ohms
      if (PM_ERR_FLAG__SUCCESS != pm_ibb_set_soft_strt_chgr_resistor(PMIC_PMI_DEV_INDEX, 64))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to set start charging register with ChgrResistor\n");
      }
      // Finish VDISP Config
      if (PM_ERR_FLAG__SUCCESS != pm_ibb_ibb_module_rdy(PMIC_PMI_DEV_INDEX, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to config IBB to Ready\n");
      }
      // Turn on IBB
      if (PM_ERR_FLAG__SUCCESS != pm_ibb_config_ibb_ctrl(PMIC_PMI_DEV_INDEX, TRUE, FALSE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to enable IBB\n");
      }


      //Change default Lab Current Sense setting, ISense_Tap = 1x, ISense_Gain = 1.5x
      if (PM_ERR_FLAG__SUCCESS != pm_lab_config_current_sense(PMIC_PMI_DEV_INDEX, PM_LAB_ISENSE_1P0, PM_LAB_ISENSE_1P5))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to config LAB Current Sense\n");
      }
      //Change default Lab Pulse skip setting, Enabled, 40mA threshold.
      if (PM_ERR_FLAG__SUCCESS != pm_lab_config_pulse_skip_ctrl(PMIC_PMI_DEV_INDEX, 40, FALSE, FALSE, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to config LAB Pulse skip setting\n");
      }

      //Enable LCD mode
      if (PM_ERR_FLAG__SUCCESS != pm_lab_lcd_amoled_sel(PMIC_PMI_DEV_INDEX, FALSE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to config LAB to LCD mode\n");
      }
      // Finish VDISN Config
      if (PM_ERR_FLAG__SUCCESS != pm_lab_lab_module_rdy(PMIC_PMI_DEV_INDEX, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to config LAB to Ready\n");
      }
      //Turn on LAB for VDISN
      if (PM_ERR_FLAG__SUCCESS != pm_lab_ibb_rdy_en(PMIC_PMI_DEV_INDEX, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to enable LAB\n");
      }

      // Wait for IBB/LAB to become ready
      Panel_IBBLAB_WaitForReady();

      /************************************************************************
        *  Enable secondary power source
        ************************************************************************/

      //0xD84A             MODULATION   0x0C      1.2 MHz modulator clock (default)
      //if (PM_ERR_FLAG__SUCCESS != PmicWledProtocol->SetModClkCtrl(PMIC_PMI_DEV_INDEX, 0, PM_WLED_MOD_CLK_1P2MHz))
      //{
      //  MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled SetModClkCtrl failed\n");
      //}

      //0xD84D             WLED_OVP         0x11       For this panel, 29.6V OVP is sufficient
      if (PM_ERR_FLAG__SUCCESS != pm_wled_sel_ovp_thr(PMIC_PMI_DEV_INDEX, 0, PM_WLED_OVP_THR_29P6V))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled SetOvpThreshold failed\n");
      }

      //0xD84E             WLED_ILIM         0x02       525 mA current limit setting for fsw = 1.6 MHz
      if (PM_ERR_FLAG__SUCCESS != pm_wled_sel_bst_ilimit(PMIC_PMI_DEV_INDEX, 0, PM_WLED_BST_ILIMIT_450mA))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled SetBoostIlimit failed\n");
      }

      //0xD84F             EN_CURRENT_SINK         0xE0       Enable string 1
      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_current_sink(PMIC_PMI_DEV_INDEX, PM_WLED_LED2))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled EnableCurrentSink PM_WLED_LED2 failed\n");
      }

      //0xD84F             EN_CURRENT_SINK         0xE0       Enable string 2
      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_current_sink(PMIC_PMI_DEV_INDEX, PM_WLED_LED3))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled EnableCurrentSink PM_WLED_LED3 failed\n");
      }

      //0xD854             SLEW_RATE_CTRL            0x02       At default
      if (PM_ERR_FLAG__SUCCESS != pm_wled_sel_slew_rate(PMIC_PMI_DEV_INDEX, PM_WLED_SLEW_RATE_10nS))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled SelectSlewRate failed\n");
      }

      //0xD84C             SWITCHING_FREQUENCY       0x11      Set boost switching frequency to 800KHz.
      if (PM_ERR_FLAG__SUCCESS != pm_wled_set_fsw_ctrl(PMIC_PMI_DEV_INDEX, 0, 800))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled SetFswCtrl failed\n");
      }

      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_modulator(PMIC_PMI_DEV_INDEX, PM_WLED_ALL, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled EnableModulator failed\n");
      }

      // 12bit mode
      //if (PM_ERR_FLAG__SUCCESS != pm_wled_sel_pwm_mode(PMIC_PMI_DEV_INDEX, 0, PM_WLED_MODE_12b_SDM))
      //{
      //  MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled SelectPwmMode failed\n");
      //}

      if (PM_ERR_FLAG__SUCCESS != pm_wled_en_sync(PMIC_PMI_DEV_INDEX, PM_WLED_ALL, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled EnableSync failed\n");
      }

      //0xD846             MODULE_EN      0x80       Enable the entire module
      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_module(PMIC_PMI_DEV_INDEX, 0, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled EnableWled failed\n");
      }
    }
    else
    {
      // ********** Brightness Power-down Sequence **********
      if (PM_ERR_FLAG__SUCCESS != pm_ibb_config_ibb_ctrl(PMIC_PMI_DEV_INDEX, FALSE, FALSE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to enable IBB\n");
      }

      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_module(PMIC_PMI_DEV_INDEX, 0, FALSE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled EnableWled failed\n");
      }
    }
  }
  else if (MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED == pPowerParams->ePMICSecondaryPower[eDisplayId])
  {
    if (TRUE == bPowerUp)
    {
      // Enable AMOLED mode for IBB
      if (PM_ERR_FLAG__SUCCESS != pm_ibb_lcd_amoled_sel(PMIC_PMI_DEV_INDEX, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to config IBB to AMOLED mode\n");
      }

      // Enable AMOLED mode for LAB
      if (PM_ERR_FLAG__SUCCESS != pm_lab_lcd_amoled_sel(PMIC_PMI_DEV_INDEX, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to config LAB to AMOLED mode\n");
      }

      // Enable AMOLED mode for WLED
      if (PM_ERR_FLAG__SUCCESS != pm_wled_en_amoled(PMIC_PMI_DEV_INDEX, PM_WLED_DISPLAY_TYPE_AMOLED))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled Enable AMOLED mode failed\n");
      }

      // Set AMOLED WLED Controlling mode as Swire
      if (PM_ERR_FLAG__SUCCESS != pm_wled_set_amoled_vout(PMIC_PMI_DEV_INDEX, PM_WLED_AMOLED_CTRL_TYPE_SWIRE, 7600))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Wled Set AMOLED Vout failed\n");
      }
    }
    else
    {
      // Turn off IBB 
      if (PM_ERR_FLAG__SUCCESS != pm_ibb_config_ibb_ctrl(PMIC_PMI_DEV_INDEX, FALSE, FALSE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Error to disable IBB\n");
      }
    }
  }
  
  return eStatus;
}


/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_BacklightLevel()
**
** DESCRIPTION:
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status Panel_CDP_BacklightLevel(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig)
{
  MDP_Status                    eStatus = MDP_STATUS_OK;


  if (MDP_PANEL_BACKLIGHTTYPE_PMIC == pBacklightConfig->eBacklightType)
  {
    // Configure the power grid based on the module type
    switch (pBacklightConfig->uBacklightCntrl.eBacklightCtrl)
    {
    case MDP_PMIC_BACKLIGHT_CONTROLTYPE_WLED:
      {

        uint32 uWLEDValue = (0xFFF * pBacklightConfig->uLevel) / 100; // Calculate duty cycle based on 12 bit mode

        if (PM_ERR_FLAG__SUCCESS != pm_wled_set_led_duty_cycle(PMIC_PMI_DEV_INDEX, PM_WLED_ALL, uWLEDValue))
        {
          MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Wled pm_wled_set_led_duty_cycle failed\n");
        }
      }
      break;
    default:
      break;
    }
  }
  else
  {
    // Nothing to do for other configurations
  }

  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_IBBLAB_WaitForReady()
** 
** DESCRIPTION:
**  Polling loop to wait for IBB/LAB modules to be ready.
**  VDISN, VDISP will reach the proper voltage upon ready.
**/
static void Panel_IBBLAB_WaitForReady()
{

  pm_ibb_status_type    sIBBStatus;
  pm_lab_status_type    sLABStatus;
  uint32                uCount;

  for (uCount = 0; uCount < PMIC_IBBLAB_READYTIME; uCount++)
  {
    if ((PM_ERR_FLAG__SUCCESS == pm_ibb_get_ibb_status(PMIC_PMI_DEV_INDEX, &sIBBStatus)) &&
        (PM_ERR_FLAG__SUCCESS == pm_lab_get_lab_status(PMIC_PMI_DEV_INDEX, &sLABStatus)))
    {
      if ((TRUE == sLABStatus.lab_vreg_ok) &&
          (TRUE == sIBBStatus.ibb_vreg_ok))
      {
        break;
      }
      else if (PMIC_IBBLAB_READYTIME == (uCount + 1))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: LAB/IBB GetStatus(LabVregOk) failed after %dms\n", PMIC_IBBLAB_READYTIME);
      }

      // Wait 1ms between checks
      MDP_OSAL_DELAYMS(1);
    }
    else
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: IBB/LAB GetStatus() Failed\n");
      break;
    }
  }

}


/* ---------------------------------------------------------------------- */
/**
** FUNCTION: panel_ConfigGPIO()

** 
** DESCRIPTION:
**   Initialize LCD panel GPIOs and PMICs
**
*/ /* -------------------------------------------------------------------- */
MDP_Status MDPPlatformConfigure(MDP_Display_IDType eDisplayId, MDPPlatformConfigType eConfig, MDPPlatformParams *pPlatformParams)
{
  MDP_Status  eStatus  = MDP_STATUS_OK;

  /* Static information, initial once during the first call */
  static bool32                 bInitFlag           = FALSE;
  static MDPPlatformInfo        sPlatformInfo;
  static Panel_PowerCtrlParams  sPanelPowerCtrl;
  PlatformDSIDetectParams       *pPanelList         = NULL;
  uint32                        uPanelCnt;

  if (FALSE == bInitFlag)
  {
    MDP_OSAL_MEMZERO(&sPlatformInfo,    sizeof(MDPPlatformInfo));
    MDP_OSAL_MEMZERO(&sPanelPowerCtrl,  sizeof(Panel_PowerCtrlParams));
    bInitFlag = TRUE;
  }


  // Read the platform ID once
  if (FALSE == sPlatformInfo.bPlatformDetected)
  {
    if (MDP_STATUS_OK == ReadPlatformIDAndChipID(&sPlatformInfo.sEFIPlatformType, &sPlatformInfo.sEFIChipSetId, &sPlatformInfo.sEFIChipSetFamily))
    {
      uint32 uTemp                    = 0;
      sPlatformInfo.bPlatformDetected = TRUE;

      // Read the display panel ID override
      if (MDP_STATUS_OK == MDP_GetEnvironmentVariable(L"DISPLAYID", &uTemp, sizeof(uTemp)))
      {
        if (uTemp > 0)
        {
          MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Panel override enabled (ID:0x%x)\n", uTemp);
          sPlatformInfo.uPanelIdOverride = uTemp;
        }
      }
    }
  }

  switch (eConfig)
  {
  case MDPPLATFORM_CONFIG_SW_RENDERER:

    if ((EFI_PLATFORMINFO_TYPE_UNKNOWN == sPlatformInfo.sEFIPlatformType.platform) ||
        (EFI_PLATFORMINFO_TYPE_VIRTIO  == sPlatformInfo.sEFIPlatformType.platform) ||
        (EFI_PLATFORMINFO_TYPE_RUMI    == sPlatformInfo.sEFIPlatformType.platform) ||
        (TRUE == PcdGetBool(PcdDisplayForceSwRenderer)))
    {
      // Treat unknown platforms as a SW model only
      pPlatformParams->sPlatformInfo.bSWRender = TRUE;
      MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayLib: SW renderer only\n");
    }
    else
    {
      pPlatformParams->sPlatformInfo.bSWRender = FALSE;
    }
    break;
  case MDPPLATFORM_CONFIG_GETPANELCONFIG:
    {
      pPlatformParams->sPlatformPanel.uDefaultVendor   = 0;
      pPlatformParams->sPlatformPanel.uDefaultRevision = 0;
      pPlatformParams->sPlatformPanel.uPanelId         = 0;

      // Retrieve panel configuration (could be dependent on the interface)
      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:
        // Report the proper information depending on the display.
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
            case EFI_PLATFORMINFO_TYPE_CDP:
            case EFI_PLATFORMINFO_TYPE_MTP:
            case EFI_PLATFORMINFO_TYPE_QRD:
              if (sPlatformInfo.sEFIPlatformType.subtype == 1)
              {
                 pPanelList = panelList_1;
                 uPanelCnt  = sizeof(panelList_1)/sizeof(PlatformDSIDetectParams);
              }
              else
              {
                 pPanelList = panelList_0;
                 uPanelCnt  = sizeof(panelList_0)/sizeof(PlatformDSIDetectParams);
              }

              /* use default panel */
              pPlatformParams->sPlatformPanel.pPanelXMLConfig = (int8 *)pPanelList->psPanelCfg;
              pPlatformParams->sPlatformPanel.uConfigSize     = pPanelList->uPanelCfgSize;
              pPlatformParams->sPlatformPanel.eSelectedPanel  = pPanelList->eSelectedPanel;
              break;
            default:
               MDP_Log_Message(MDP_LOGLEVEL_WARN, "Ramdump:DisplayLib: Platform id=%d NOT supported\n", sPlatformInfo.sEFIPlatformType.platform);
              break;
        }
        break;
      case MDP_DISPLAY_EXTERNAL:
          pPlatformParams->sPlatformPanel.pPanelXMLConfig = (int8*)dummy_xmldata;
          pPlatformParams->sPlatformPanel.uConfigSize     = sizeof(dummy_xmldata);
          break;
      default:
        break;
      }
    }
    break;
  case MDPPLATFORM_CONFIG_POWERUP:
    {
      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:
        // Config based on the platform
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_QRD:
          // Primary Power Sequence
          if (MDP_STATUS_OK != (eStatus = Panel_CDP_PowerUp(eDisplayId, &sPanelPowerCtrl)))
          {
            MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayLib: Primary Power Up Sequence Failed (%d)\n", eStatus);
          }
          else if (MDP_STATUS_OK != (eStatus = Panel_CDP_PeripheralPower(eDisplayId, &sPanelPowerCtrl, TRUE)))  // Secondary Power Sequence
          {
            MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayLib: Secondary Power Up Sequence Failed (%d)\n", eStatus);
          }
          break;

        default:
          break;
        }

        break;

      case MDP_DISPLAY_EXTERNAL:

        // Config based on the platform
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
          break;
        default:
          break;
        }
        break;

      default:
        break;

      }

    }
    break;

  case MDPPLATFORM_CONFIG_POWERDOWN:
    {
        MDP_Log_Message(MDP_LOGLEVEL_WARN, "Ramdump:DisplayLib: Primary Power Down NOT supported\n");

    }
    break;
  case MDPPLATFORM_CONFIG_SETBACKLIGHT:
    {
      // Handle backlight level
      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
          eStatus = Panel_CDP_BacklightLevel(eDisplayId, &pPlatformParams->sBacklightConfig);
        default:
          break;
        }
        break;
      case MDP_DISPLAY_EXTERNAL:
        eStatus = MDP_STATUS_NOT_SUPPORTED;
        break;
      default:
        break;
      }
    }
    break;
  case MDPPLATFORM_CONFIG_GETPANELID:
    {
    }
    break;
  case MDPPLATFORM_CONFIG_GETPLATFORMINFO:
    {
      //
      // Return platform information
      //
      MDP_OSAL_MEMCPY(&pPlatformParams->sPlatformInfo, &sPlatformInfo, sizeof(MDPPlatformInfo));
    }
    break;
  case MDPPLATFORM_CONFIG_RESETPANEL:
    {
      if (pPlatformParams->sPlatformPanelReset.uResetGpio == 0)
      {
           sPanelPowerCtrl.uResetGpio = CDP_DISP_RESET_GPIO;
           pPlatformParams->sPlatformPanelReset.uResetGpio = CDP_DISP_RESET_GPIO;
      }
      else 
      {
           sPanelPowerCtrl.uResetGpio = pPlatformParams->sPlatformPanelReset.uResetGpio;
      }

      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:
        // Config based on the platform
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_QRD:
           eStatus = Panel_CDP_Reset(&pPlatformParams->sPlatformPanelReset);
           break;
        default:
          break;
        }

        break;
      case MDP_DISPLAY_EXTERNAL:
        eStatus = MDP_STATUS_NOT_SUPPORTED;
        break;
      default:
        break;
      }
    }
    break;

  case MDPPLATFORM_CONFIG_SETGPIOSTATE:
    {
      eStatus = SetDefaultGPIOState(&pPlatformParams->sGPIOState);
    }
    break;
      
  default:
    eStatus = MDP_STATUS_BAD_PARAM;
    break;
  }
  
  return eStatus;
}



/*===========================================================================
Private Function Definitions
===========================================================================*/

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: ReadPlatformIDAndChipID()
** 
** DESCRIPTION:
**
*/ /* -------------------------------------------------------------------- */

DALResult boot_DalPlatformInfo_Platforminfo(void *pPlatforminfo);
DALResult boot_DalPlatformInfo_ChipId(void * pChipSetId);
DALResult boot_DalPlatformInfo_ChipFamily(void * pChipsetfamily);


static MDP_Status ReadPlatformIDAndChipID(EFI_PLATFORMINFO_PLATFORM_INFO_TYPE *pPlatformInfo, EFIChipInfoIdType *pChipSetId, EFIChipInfoFamilyType *pChipSetFamily)
{
  DalPlatformInfoPlatformInfoType      dalPlatformInfo;
  DALResult                            eDalStatus = DAL_SUCCESS;
  MDP_Status                           eStatus    = MDP_STATUS_OK;

  eDalStatus = boot_DalPlatformInfo_Platforminfo(&dalPlatformInfo);
  if (eDalStatus != DAL_SUCCESS)
  {
     MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump:Display:ReadPlatformIDAndChipId: Failed\n");

     eStatus    = MDP_STATUS_FAILED;
  }
  else 
  {
    pPlatformInfo->platform = (EFI_PLATFORMINFO_PLATFORM_TYPE)dalPlatformInfo.platform;
    pPlatformInfo->version  = dalPlatformInfo.version;
    pPlatformInfo->subtype  = dalPlatformInfo.subtype;
    pPlatformInfo->fusion   = dalPlatformInfo.fusion;

    eDalStatus = boot_DalPlatformInfo_ChipId(pChipSetId);
    eDalStatus = boot_DalPlatformInfo_ChipFamily(pChipSetFamily);
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: SetDefaultGPIOState()
*/
/*!
* \brief
*   The \b SetDefaultGPIOState function toggles a list of GPIO's based on the requested configuration
**
* \retval MDP_Status
*
****************************************************************************/
MDP_Status SetDefaultGPIOState(GpioStateType *pGPIOList)
{
  MDP_Status          eStatus = MDP_STATUS_OK;

  if ((NULL == pGPIOList) ||
      (NULL == pGPIOList->pGPIOList))
  {
    eStatus = MDP_STATUS_BAD_PARAM;
  }
  else
  {
    // Handle each type of GPIO differently
    switch (pGPIOList->eGPIOType)
    {
    case MDPPLATFORM_GPIOTYPE_TLMM:
    {
      uint32              uCount;

      // Loop the list to configure all GPIOs.
      for (uCount = 0;uCount < pGPIOList->uListSize;uCount++)
      {
        // Only try to program GPIOs that are within range, TLMM Macro (EFI_GPIO_CFG) supports up to 0x3FF
        if (pGPIOList->pGPIOList[uCount] < 0x3FF)
        {

          DALGpioSignalType   uGPIOCfg = 0;
          uint32              uValue  = (pGPIOList->uState > 0) ? DAL_GPIO_HIGH_VALUE : DAL_GPIO_LOW_VALUE;

          uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(pGPIOList->pGPIOList[uCount], 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, uValue);
          Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);
        }
      }
    }
    break;
    default:
      // Others (PMIC, PMI) not supported
      break;
    }
  }

  return eStatus;
}


/* ---------------------------------------------------------------------- */
/**
** FUNCTION: GetPmicIBBLABMode()
** 
** DESCRIPTION:
**        This function can get the PMIC AMOLED/LCD mode configuration.
**
*/
static MDP_Status GetPmicIBBLABMode(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  pm_ibb_status_type sPmicIBBStatus;
  pm_lab_status_type sPmicLABStatus;
  MDP_Status         eStatus = MDP_STATUS_OK;

  if (NULL == pPowerParams)
  {
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump: DisplayLib: GetPmicIBBLABMode pPowerParams is NULL!\n");
    eStatus = MDP_STATUS_NO_RESOURCES;
  }
  else if (PM_ERR_FLAG__SUCCESS != pm_ibb_get_ibb_status(PMIC_PMI_DEV_INDEX, &sPmicIBBStatus))
  {
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump: DisplayLib: GetPmicIBBLABMode Get IBB Status Failed!\n");
    eStatus = MDP_STATUS_FAILED;
  }
  else if (PM_ERR_FLAG__SUCCESS != pm_lab_get_lab_status(PMIC_PMI_DEV_INDEX, &sPmicLABStatus))
  {
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump: DisplayLib: GetPmicIBBLABMode Get LAB Status Failed!\n");
    eStatus = MDP_STATUS_FAILED;
  }
  else
  {
    if ((TRUE == sPmicIBBStatus.en_amoled_mode) &&
        (TRUE == sPmicLABStatus.en_amoled_mode))
    {
      // Get AMOLED mode by reading IBB/LAB status, configure PMIC as AMOLED mode.
      pPowerParams->ePMICSecondaryPower[eDisplayId] = MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED;
    }
    else if ((FALSE == sPmicIBBStatus.en_amoled_mode) &&
             (FALSE == sPmicLABStatus.en_amoled_mode))
    {
      // Get LCD mode by reading IBB/LAB status, configure PMIC as LCD mode.
      pPowerParams->ePMICSecondaryPower[eDisplayId] = MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD;
    }
    else
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump: DisplayLib: GetPmicIBBLABMode IBB/LAB get different modes, skip power configuration!\n");
      eStatus = MDP_STATUS_FAILED;
    }
  }

  return eStatus;
}

#ifdef __cplusplus
}
#endif
