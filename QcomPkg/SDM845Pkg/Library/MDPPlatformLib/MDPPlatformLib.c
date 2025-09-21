/*=============================================================================
 
  File: MDPPlatformLib.c
 
  MDP platform specific functions
  
  Copyright (c) 2011-2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/EFIPmicGpio.h>
#include <Protocol/EFIPmicClkBuff.h>
#include <Protocol/EFITlmm.h>
#include <Protocol/EFIPmicLpg.h>
#include <Protocol/EFIPmicGpio.h>
#include <Protocol/EFIPmicVreg.h>
#include <Protocol/EFIPmicWled.h>
#include <Protocol/EFIPmicIbb.h>
#include <Protocol/EFIPmicLab.h>
#include "DDIChipInfo.h"
#include "MDPTypes.h"
#include "MDPPlatformLib.h"
#include "MDPPlatformLibPanelConfig.h"
#include "MDPSystem.h"
#include "MDPPeripherals.h"
#include "DisplayUtils.h"
#include "npa.h"
#include "pmapp_npa.h"
#include "DDITlmm.h"
#include "HALDSILib.h"

/* -----------------------------------------------------------------------
** Local functions
** ----------------------------------------------------------------------- */

/* Platform detection 
 */
static MDP_Status ReadPlatformIDAndChipID(EFI_PLATFORMINFO_PLATFORM_INFO_TYPE *pPlatformInfo, EFIChipInfoIdType *pChipSetId, EFIChipInfoFamilyType *pChiSetFamily);
static MDP_Status DynamicDSIPanelDetection(MDPPlatformPanelInfo *pPlatformPanel, uint32 *puPanelID, PlatformDSIDetectParams *pPanelList, uint32 uPanelCnt);

/* General helper functions
 */
static void       SetupPlatformPanelConfig(MDP_Display_IDType eDisplayId, MDPPlatformPanelInfo *pPlatformPanel, MDPPlatformInfo *pPlatformInfo);
static MDP_Status GetPanelXmlConfig(MDPPlatformPanelInfo *pPlatformPanel);
static MDP_Status SetDefaultGPIOState(GpioStateType *pGPIOList);

/* External display default functions
 */
extern MDP_Status ExternalPanel_Default_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status ExternalPanel_Default_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern bool32     DP_USBPlugInvertedOrientation(void);

/*===========================================================================
 Local Configuration Definitions
 ===========================================================================*/
const static int8 dummy_xmldata[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"    <PanelName>VirtioDSI</PanelName>"
"    <PanelDescription>DSI Model (640x480 24bpp)</PanelDescription>"
"    <Group id=\"Active Timing\">"
"        <HorizontalActive>640</HorizontalActive>"
"        <HorizontalFrontPorch>13</HorizontalFrontPorch>"
"        <HorizontalBackPorch>14</HorizontalBackPorch>"
"        <HorizontalSyncPulse>15</HorizontalSyncPulse>"
"        <HorizontalSyncSkew>0</HorizontalSyncSkew>"
"        <HorizontalLeftBorder>0</HorizontalLeftBorder>"
"        <HorizontalRightBorder>0</HorizontalRightBorder>"
"        <VerticalActive>480</VerticalActive>"
"        <VerticalFrontPorch>11</VerticalFrontPorch>"
"        <VerticalBackPorch>12</VerticalBackPorch>"
"        <VerticalSyncPulse>13</VerticalSyncPulse>"
"        <VerticalSyncSkew>0</VerticalSyncSkew>"
"        <VerticalTopBorder>0</VerticalTopBorder>"
"        <VerticalBottomBorder>0</VerticalBottomBorder>"
"        <InvertDataPolarity>False</InvertDataPolarity>"
"        <InvertVsyncPolairty>False</InvertVsyncPolairty>"
"        <InvertHsyncPolarity>False</InvertHsyncPolarity>"
"        <BorderColor>0x0</BorderColor>"   
"    </Group>"
"    <Group id=\"Display Interface\">"
"        <InterfaceType>8</InterfaceType>"
"        <InterfaceColorFormat>3</InterfaceColorFormat>"
"    </Group>"
"    <Group id=\"DSI Interface\">"
"        <DSIChannelId>1</DSIChannelId>"     
"        <DSIVirtualId>0</DSIVirtualId>"     
"        <DSIColorFormat>36</DSIColorFormat>"     
"        <DSITrafficMode>0</DSITrafficMode>"
"        <DSILanes>2</DSILanes>"
"        <DSIHsaHseAfterVsVe>False</DSIHsaHseAfterVsVe>"
"        <DSILowPowerModeInHFP>False</DSILowPowerModeInHFP>"
"        <DSILowPowerModeInHBP>False</DSILowPowerModeInHBP>"
"        <DSILowPowerModeInHSA>False</DSILowPowerModeInHSA>"
"        <DSILowPowerModeInBLLPEOF>False</DSILowPowerModeInBLLPEOF>"
"        <DSILowPowerModeInBLLP>False</DSILowPowerModeInBLLP>"
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
static PlatformDSIDetectParams uefiPanelList[] = { 

    /* CDT Subtype == 0 : 2.5k panels */

    // Panel #0 - Truly DSC DSI Command Mode Panel (1440x2560 24bpp) ID:0x8012
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
        {0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }
      },
      0,                                                     // Lane remap order {0, 1, 2, 3}
      NULL,                                                  // psPanelCfg (panel configuration)
      0,                                                     // uPanelCfgSize
      MDPPLATFORM_PANEL_TRULY_WQXGA_DSC_CMD,                 // eSelectedPanel 
      0                                                      // uFlags
    },

    // Panel #1 - Truly DSC DSI Video Mode Panel (1440x2560 24bpp) ID:0x8013
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
        {0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }
      },
      0,                                                     // Lane remap order {0, 1, 2, 3}
      NULL,                                                  // psPanelCfg (panel configuration)
      0,                                                     // uPanelCfgSize
      MDPPLATFORM_PANEL_TRULY_WQXGA_DSC_VIDEO,               // eSelectedPanel 
      0                                                      // uFlags
    },

    // Panel #2 - Truly Dual DSI Command Mode Panel (1440x2560 24bpp) ID:0x8010
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
      NULL,                                                  // psPanelCfg (panel configuration)
      0,                                                     // uPanelCfgSize
      MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_CMD,             // eSelectedPanel 
      0                                                      // uFlags
    },      

    // Panel #3 - Truly Dual DSI Video Mode Panel (1440x2560 24bpp) ID:0x8011
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
        {0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }
      },
      0,                                                     // Lane remap order {0, 1, 2, 3}
      NULL,                                                  // psPanelCfg (panel configuration)
      0,                                                     // uPanelCfgSize
      MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_VIDEO,           // eSelectedPanel 
      0                                                      // uFlags
    },  
      
      
    /* CDT subtype == 1 (4k panels) */

    // Panel #4 - Sharp 4k DSC Command Mode Panel (2160x3840 24bpp) ID:0x8000
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
      NULL,                                                  // psPanelCfg (panel configuration)
      0,                                                     // uPanelCfgSize
      MDPPLATFORM_PANEL_SHARP_4K_DSC_CMD,                    // eSelectedPanel 
      0                                                      // uFlags
    },

    // Panel #5 - Sharp 4k DSC Video Mode Panel (2160x3840 24bpp) ID:0x8056
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
        {0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }
      },
      0,                                                     // Lane remap order {0, 1, 2, 3}
      NULL,                                                  // psPanelCfg (panel configuration)
      0,                                                     // uPanelCfgSize
      MDPPLATFORM_PANEL_SHARP_4K_DSC_VIDEO,                  // eSelectedPanel 
      0                                                      // uFlags
    },


    /* subtype == 3 (No panel panels) */
    
    // Panel #6 - Dummy 640x480 video mode panel  ID:0xFFFF
    {  
      0xFF,                                                  // uCmdType
      0xFF,                                                  // total number of retry on failures
      {
        {{0xff, 0x00},                                       // address to read ID1
        {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }, 
        {{0xff, 0x00},                                       // address to read ID2
        {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        },
        {{0xff, 0x00},                                       // address to read ID3
        {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // expected readback
        }
      },
      0,                                                     // Lane remap order {0, 1, 2, 3}
      NULL,                                                  // psPanelCfg (panel configuration)
      0,                                                     // uPanelCfgSize
      MDPPLATFORM_PANEL_SIM_VIDEO_PANEL,                     // eSelectedPanel 
      0                                                      // uFlags
    }
};


/*
 * List of supported panels. The ones with XML data have UEFI support and the rest are only supported by kernel.  These are applicable for fastboot overrides of the
 * panel configuration.
 */
const PanelDTInfoType fastBootPanelList[] =
{
 /*Supported Panels*/
 /*4k cmd*/
 PANEL_CREATE_ENTRY("sharp_4k_dsc_cmd",     MDPPLATFORM_PANEL_SHARP_4K_DSC_CMD,             "dsi_sharp_4k_dsc_cmd_display:",            DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI,                                DISP_MODE_DUAL_DSI,                                   DISP_MODE_DUAL_DSI),
 /*4k video*/
 PANEL_CREATE_ENTRY("sharp_4k_dsc_vid",     MDPPLATFORM_PANEL_SHARP_4K_DSC_VIDEO,           "dsi_sharp_4k_dsc_video_display:",          DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI,                                DISP_MODE_DUAL_DSI,                                   DISP_MODE_DUAL_DSI),
 
 
 /*Truly 2k NT35597 dsc cmd */
 PANEL_CREATE_ENTRY("truly_wqxga_dsc_cmd",  MDPPLATFORM_PANEL_TRULY_WQXGA_DSC_CMD,          "dsi_nt35597_truly_dsc_cmd_display:",        DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_1,  DISP_TIMING_CONFIG_NONE,   PLL_OVERRIDE_PLL1, DISP_MODE_SINGLE_DSI | DISP_MODE_USE_DSI1,         DISP_MODE_SINGLE_DSI | DISP_MODE_USE_DSI1,            DISP_MODE_SINGLE_DSI | DISP_MODE_USE_DSI1 | DISP_MODE_DSC_MERGE),
 
 /*Truly 2k NT35597 dsc video */
 PANEL_CREATE_ENTRY("truly_wqxga_dsc_vid",  MDPPLATFORM_PANEL_TRULY_WQXGA_DSC_VIDEO,        "dsi_nt35597_truly_dsc_video_display:",      DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_1,  DISP_TIMING_CONFIG_NONE,   PLL_OVERRIDE_PLL1, DISP_MODE_SINGLE_DSI | DISP_MODE_USE_DSI1,         DISP_MODE_SINGLE_DSI | DISP_MODE_USE_DSI1,            DISP_MODE_SINGLE_DSI | DISP_MODE_USE_DSI1 | DISP_MODE_DSC_MERGE),
 
 
 /*Truly 2k NT35597 cmd*/
 PANEL_CREATE_ENTRY("truly_wqxga_dual_cmd", MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_CMD,      "dsi_dual_nt35597_truly_cmd_display:",       DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI,                                DISP_MODE_DUAL_DSI,                                   DISP_MODE_DUAL_DSI),
 
 /*Truly 2k NT35597 video*/
 PANEL_CREATE_ENTRY("truly_wqxga_dual_vid", MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_VIDEO,    "dsi_dual_nt35597_truly_video_display:",     DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI,                                DISP_MODE_DUAL_DSI,                                   DISP_MODE_DUAL_DSI),
 
 /* Sharp 1080p cmd */
 PANEL_CREATE_ENTRY("sharp_1080p_cmd",      MDPPLATFORM_PANEL_SHARP_1080P_CMD,              "dsi_sharp_1080_cmd_display:",               DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_SINGLE_DSI,                                DISP_MODE_SINGLE_DSI,                                   DISP_MODE_SINGLE_DSI),
 
 /* OLED Panels */
 PANEL_CREATE_ENTRY("ss_mg2_wqhd_dsc_cmd",  MDPPLATFORM_PANEL_SAMSUNG_WQHD_AMOLED_DSC_CMD,  "dsi_samsung_wqhd_amoled_cmd_display:",      DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_1,     DISP_TIMING_CONFIG_0,    PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI,                                DISP_MODE_DUAL_DSI,                                   DISP_MODE_DUAL_DSI),
 PANEL_CREATE_ENTRY("ss_mg2_1080p_dsc_cmd", MDPPLATFORM_PANEL_SAMSUNG_1080P_AMOLED_DSC_CMD, "dsi_samsung_wqhd_amoled_cmd_fhd_display:",  DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_1,     DISP_TIMING_CONFIG_1,    PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI,                                DISP_MODE_DUAL_DSI,                                   DISP_MODE_DUAL_DSI),
 PANEL_CREATE_ENTRY("ss_mg2_720p_cmd",      MDPPLATFORM_PANEL_SAMSUNG_720P_AMOLED_CMD,      "dsi_samsung_wqhd_amoled_cmd_hd_display:",   DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_2,    PLL_OVERRIDE_NONE, DISP_MODE_SINGLE_DSI,                                DISP_MODE_SINGLE_DSI,                                   DISP_MODE_SINGLE_DSI),
 PANEL_CREATE_ENTRY("ss_mg1_wqhd_dual_cmd", MDPPLATFORM_PANEL_SAMSUNG_WQHD_AMOLED_DUAL_CMD, "dsi_samsung_wqhd_amoled_dual_cmd_display:", DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_SINGLE_DSI,                                DISP_MODE_SINGLE_DSI,                                   DISP_MODE_SINGLE_DSI),
 
 /*Simulation panels */
 /*sim video*/
 PANEL_CREATE_ENTRY("sim_vid_panel",        MDPPLATFORM_PANEL_SIM_VIDEO_PANEL,             "dsi_sim_vid_display:",                DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER,  DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER,     DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER),
 /*sim dualdsi video*/ 
 PANEL_CREATE_ENTRY("sim_dual_vid_panel",   MDPPLATFORM_PANEL_SIM_DUALDSI_VIDEO_PANEL,     "dsi_dual_sim_vid_display:",           DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,  DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,     DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER),
 /*sim CMD*/
 PANEL_CREATE_ENTRY("sim_cmd_panel",        MDPPLATFORM_PANEL_SIM_CMD_PANEL,               "dsi_sim_cmd_display:",                  DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER,  DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER,     DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER),
 /*sim dualdsi CMD*/
 PANEL_CREATE_ENTRY("sim_dual_cmd_panel",   MDPPLATFORM_PANEL_SIM_DUALDSI_CMD_PANEL,       "dsi_dual_sim_cmd_display:",             DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,  DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,     DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER),
 
 /* sim single dsc  with dsc compression 3.75 cmd */
 PANEL_CREATE_ENTRY("sim_dsc_375_cmd_panel",      MDPPLATFORM_PANEL_SIM_DSC_375_CMD_PANEL,               "dsi_sim_dsc_375_cmd_display:",  DISP_INTF_DSI,   DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER,  DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER,     DISP_MODE_SINGLE_DSI | DISP_MODE_SKIP_BOOTLOADER),
 
 /* sim dual dsi with dsc compression 3.75 cmd */
 PANEL_CREATE_ENTRY("sim_dual_dsc_375_cmd_panel", MDPPLATFORM_PANEL_SIM_DUAL_DSC_375_CMD_PANEL,       "dsi_dual_sim_dsc_375_cmd_display:",             DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,  DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,     DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER),
 
 
 /*Skip mode only panels, SW render in UEFI*/
 /* Sharp 1080p cmd, 120Hz */
 PANEL_CREATE_ENTRY("sharp_120hz_1080p_cmd",      MDPPLATFORM_PANEL_SHARP_120HZ_1080P_CMD,   "dsi_dual_sharp_1080_120hz_cmd_display:",DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,   DISP_TIMING_CONFIG_NONE,   PLL_OVERRIDE_NONE, DISP_MODE_SINGLE_DSI,                                DISP_MODE_SINGLE_DSI,                                   DISP_MODE_SINGLE_DSI),

 /* Sharp wqhd cmd and video */
 PANEL_CREATE_ENTRY("sharp_wqhd_dual_cmd",        MDPPLATFORM_PANEL_SHARP_WQHD_DUALDSI_CMD,  "dsi_dual_nt35597_cmd_display:",         DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,   DISP_TIMING_CONFIG_NONE,   PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,    DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,       DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER),
 PANEL_CREATE_ENTRY("sharp_wqhd_dual_vid",        MDPPLATFORM_PANEL_SHARP_WQHD_DUALDSI_VIDEO,"dsi_dual_nt35597_video_display:",       DISP_INTF_DSI, DISP_TOPOLOGY_CONFIG_NONE,   DISP_TIMING_CONFIG_NONE,   PLL_OVERRIDE_NONE, DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,    DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,       DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER),
 
 /*End of Table, DO NOT ADD PANEL BELOW THIS*/
 PANEL_CREATE_ENTRY("",                           MDPPLATFORM_PANEL_NONE,                         "",                          DISP_INTF_NONE,        DISP_TOPOLOGY_CONFIG_NONE,  DISP_TIMING_CONFIG_NONE, PLL_OVERRIDE_NONE, DISP_MODE_NONE,                                    DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER,     DISP_MODE_DUAL_DSI   | DISP_MODE_SKIP_BOOTLOADER),
};

/*===========================================================================
Function Definitions
===========================================================================*/

/**********************************************************************************************
*
* FUNCTION: PlatformClientInit()
*
* DESCRIPTION:
*   Initialize NPA client.
*
***********************************************************************************************/
MDP_Status PlatformClientInit(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status    Status = MDP_STATUS_OK;

  NPAClientName aNPAClientName[MDP_DISPLAY_MAX] =
  {
    {PMIC_NPA_GROUP_ID_DISP_PRIM,     "DisplayPrim"},
    {PMIC_NPA_GROUP_ID_DISP_SEC,      "DisplaySec"},
    {PMIC_NPA_GROUP_ID_DISP_EXT_DP,   "DisplayExt"},
  };

  if (eDisplayId >= MDP_DISPLAY_MAX )
  {
    DEBUG ((EFI_D_ERROR, "DisplayDxe: Unsupported Display ID for power init.\n"));
    Status =  MDP_STATUS_FAILED;
  }
  else if (NULL == pPowerParams->sNPAClient[eDisplayId])
  {
    pPowerParams->sNPAClient[eDisplayId] = npa_create_sync_client( aNPAClientName[eDisplayId].cResourceName, aNPAClientName[eDisplayId].cClientName, NPA_CLIENT_REQUIRED); 
    
    if (NULL == pPowerParams->sNPAClient[eDisplayId])
    {
      DEBUG ((EFI_D_ERROR, "DisplayDxe: failed to retrieve NPA Display Handle for Display ID %x.\n", eDisplayId ));
      Status =  MDP_STATUS_FAILED;
    }
  }      

  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_CheckOverride()
*
* DESCRIPTION:
*   Check if the panel was overridden by ABL (apps boot loader) and if so check 
*   if the overridden panel is supported by this platform.
*
*   This function has 1 of 3 outcomes
*   1. Panel is supported by both bootloader & kernel 
*      - Set matching xmlconfig
*   2. panel is only supported in kernel 
*      - Return dummyconfig, since sw rendering was already selected during initialization in this scenario
*   3. panel override is not set or the panel is not supported by bootloader & kernel
*      - Return FALSE to trigger panel detection
*
***********************************************************************************************/
static bool32 Panel_CheckOverride(MDP_Display_IDType eDisplayId, MDPPlatformPanelInfo *pPlatformPanel, MDPPlatformInfo *pPlatformInfo)
{
  Display_UtilsParamsType* pDisplayOverride = Display_Utils_GetParams();
  bool32                   bRet             = FALSE;

  if ((NULL != pDisplayOverride)                           && 
      (NULL != pDisplayOverride->sPrimary.psDTInfo)        &&
      (MDPPLATFORM_PANEL_NONE < pDisplayOverride->sPrimary.psDTInfo->ePanel))
  {
    // Lookup panel
    int32 i = PANEL_LIST_LENGTH(fastBootPanelList) - 1;

    while (0 <= i)
    {
      if (pDisplayOverride->sPrimary.psDTInfo->ePanel == fastBootPanelList[i].ePanel)
      {
        // Match found
        bRet = TRUE;
        pPlatformPanel->eSelectedPanel = fastBootPanelList[i].ePanel;

        if (pDisplayOverride->uFlags & DISP_MODE_SKIP_BOOTLOADER)
        {
          pPlatformInfo->bSWRender = TRUE;
          DEBUG((EFI_D_ERROR, "DisplayDxe: Panel override <%a> in skip mode\n", pDisplayOverride->sPrimary.shortName));
        }
        else
        {
          DEBUG((EFI_D_ERROR, "DisplayDxe: Panel override <%a>\n", pDisplayOverride->sPrimary.shortName));
        }
        break;
      }
      i--;
    }
  }
  
  return bRet;
}
  

/**********************************************************************************************
*
* FUNCTION: DynamicDSIPanelDetection()
*
* DESCRIPTION:
*   Detect DSI panels by doing a DSI read specific to each panels.
*   This function could be used as sample for OEM to detect DSI panels, 
*   it is not a complete implementation of all possible combinations of read
*   commands that could be needed for this detection.
*   
*   Return success only if a DSI panel was correctly detected and the information 
*   is updated in pPlatformParams->sPlatformPanel
*
***********************************************************************************************/
static MDP_Status DynamicDSIPanelDetection(MDPPlatformPanelInfo *pPlatformPanel, uint32 *puPanelID, PlatformDSIDetectParams *pPanelList, uint32 uPanelCnt)
{
  MDP_Status             Status                = MDP_STATUS_NOT_SUPPORTED;
  bool32                 bDumpPanelId          = FALSE;

  if (MDP_STATUS_OK == DSIDriver_MinimalInit())          // do minimal DSI init
  {
    uint8       panelID[PLATFORM_PANEL_ID_MAX_COMMANDS];
    uint32      uPanelIndex;
    bool32      bMatch         = FALSE;
    uint32      uPrevClkConfig = 0; 

    // go through all possible panels
    for (uPanelIndex = 0; uPanelIndex < uPanelCnt; uPanelIndex++)
    {
      uint8     readback[DSI_READ_READBACK_SIZE];
      uint32    readSize                          = sizeof(readback);
      int       iCommandIndex                     = 0;
      uint32    uClkConfig                        = (MDPPLATFORM_PANEL_DETECT_FLAG_CLOCK_FORCEHS & 
                                                     pPanelList[uPanelIndex].uFlags);

      // Check if there is any change in the clock config and set it accordingly
      if (uPrevClkConfig != uClkConfig)
      {
        if (MDP_STATUS_OK != DSIDriver_ConfigClockLane(uClkConfig))
        {
          DEBUG((EFI_D_ERROR, "Display: DSIDriver_ConfigClockLane failed\n"));
        }
        
        uPrevClkConfig = uClkConfig;
      }

      // Reprogram the DSI lane swap based on remap order
      if (MDP_STATUS_OK != DSIDriver_RemapLane(pPanelList[uPanelIndex].uLaneRemapOrder))
      {
        DEBUG((EFI_D_WARN, "Display: DSIDriver_RemapLane failed\n"));
      }

      // Allow debug option to scan panel registers (used to help generate a uniquie panel ID for detection)
      if (TRUE == bDumpPanelId)
      {
        DsiPanelDumpRegisters();
        // Dump just once
        bDumpPanelId = FALSE;
      }

      // clear the panel ID
      MDP_OSAL_MEMZERO(panelID, PLATFORM_PANEL_ID_MAX_COMMANDS);

      // for each possible panel ID read
      for(iCommandIndex = 0; iCommandIndex<PLATFORM_PANEL_ID_MAX_COMMANDS; iCommandIndex++)
      {
        uint32 uRetryCount = 0;

        // if read command is 0, then stop reading panel ID
        if ((0 == pPanelList[uPanelIndex].panelIdCommands[iCommandIndex].address[0]) &&
            (0 == pPanelList[uPanelIndex].panelIdCommands[iCommandIndex].address[1]) )
        {
          break;
        }
        // DSI read
        bMatch = FALSE;

        uRetryCount = 0;
        do
        {
          // clear the readback buffer
          MDP_OSAL_MEMZERO(&readback[0], readSize);
          readSize = sizeof(readback);
          Status = DSIDriver_Read(pPanelList[uPanelIndex].uCmdType, 
                                  pPanelList[uPanelIndex].panelIdCommands[iCommandIndex].address, 
                                  sizeof(pPanelList[uPanelIndex].panelIdCommands[iCommandIndex].address), 
                                  readback, 
                                  &readSize);

          uRetryCount++;
        } while((uRetryCount < pPanelList[uPanelIndex].uTotalRetry) && 
               ((MDP_STATUS_OK != Status)                           ||
                (0 == readSize)));

        if ((uRetryCount <= pPanelList[uPanelIndex].uTotalRetry) &&
            (0 != readSize))
        {
          // Read was successful, now check the data is what we expect
          if (0 == CompareMem(readback, pPanelList[uPanelIndex].panelIdCommands[iCommandIndex].expectedReadback, readSize))
          {
            panelID[iCommandIndex] = readback[0];    // store the first byte of readback as panel ID
            bMatch                 = TRUE;                       // mark one panel ID matched
          }
        }

        // if any panel ID is not matched, then go to detect next panel in the list
        if(FALSE == bMatch)
        {
          break;
        }
      }

      // if all panel IDs are matched for a specific panel, store settings and stop
      if(TRUE == bMatch)
      {
        // store matched panel configuration xml data
        pPlatformPanel->eSelectedPanel = pPanelList[uPanelIndex].eSelectedPanel;

        // return the final combined panel ID
        *puPanelID = (panelID[0]<<16) | (panelID[1]<<8) | (panelID[2]);
        Status     = MDP_STATUS_OK;
        DEBUG((EFI_D_INFO, "Detected panel id:%08x\n", *puPanelID));
        break;
      }
      else
      {
        Status = MDP_STATUS_FAILED;
      }
    }

    // Close the DSI context opened in DSIDriver_MinimalInit()
    DSIDriver_Close();
  }
  return Status;
}


/**********************************************************************************************
*
* FUNCTION: PlatformConfigRemap()
*
* DESCRIPTION:
*   Remapping of the platform configuration enumeration based on platform properties.
*
***********************************************************************************************/
static MDPPlatformConfigType PlatformConfigRemap(MDPPlatformInfo *pPlatformConfig, MDPPlatformConfigType eConfig)
{
  MDPPlatformConfigType eRemappedConfig;

  // In SW render mode bypass all configuration that manipulate the hardware
  if ((NULL !=  pPlatformConfig) && 
      (TRUE == pPlatformConfig->bSWRender))
  {
    switch (eConfig)
    {
      case MDPPLATFORM_CONFIG_POWERUP:
      case MDPPLATFORM_CONFIG_POWERDOWN:
      case MDPPLATFORM_CONFIG_SETBACKLIGHT:
      case MDPPLATFORM_CONFIG_RESETPANEL:
      case MDPPLATFORM_CONFIG_SETGPIOSTATE:
        
        // In SW render mode don't execute these hardware configurations
        eRemappedConfig = MDPPLATFORM_CONFIG_BYPASS;
        
        break;
     default:
      
        // Default mapping (output = input)
        eRemappedConfig = eConfig;
        
        break;
   }
 }
 else
 {
    // Default mapping (output = input)
    eRemappedConfig = eConfig;
 }

 return eRemappedConfig;
}


/**********************************************************************************************
*
* FUNCTION: MDPPlatformConfigure()
*
* DESCRIPTION:
*   The function is used to configure display, control power and brightness etc.
*
***********************************************************************************************/
MDP_Status MDPPlatformConfigure(MDP_Display_IDType eDisplayId, MDPPlatformConfigType eConfig, MDPPlatformParams *pPlatformParams)
{
  MDP_Status               eStatus               = MDP_STATUS_OK;
  uint32                   uPlatformSubtype      = 0;
  uint32                   uHardwareVersionMajor = 0;
  uint32                   uHardwareVersionMinor = 0;
  MDPPlatformConfigType    ePlatformConfig;

  /* Static information, initial once during the first call */
  static bool32                 bInitFlag        = FALSE;
  static MDPPlatformInfo        sPlatformInfo;
  static Panel_PowerCtrlParams  sPanelPowerCtrl;
  static MDP_I2C_Configuration  sI2CConfig;
  static MDPPlatformPanelInfo   sPlatformPanel;

  if (FALSE == bInitFlag)
  {
    MDP_OSAL_MEMZERO(&sPlatformInfo,    sizeof(MDPPlatformInfo));
    MDP_OSAL_MEMZERO(&sPanelPowerCtrl,  sizeof(Panel_PowerCtrlParams));
    MDP_OSAL_MEMZERO(&sI2CConfig,       sizeof(MDP_I2C_Configuration));
    MDP_OSAL_MEMZERO(&sPlatformPanel,   sizeof(MDPPlatformPanelInfo));
    bInitFlag = TRUE;
  }

  // Get remapped platform configuration enum
  ePlatformConfig = PlatformConfigRemap(&sPlatformInfo, eConfig);
      
  // Read the platform ID once
  if (FALSE == sPlatformInfo.bPlatformDetected)
  {
    if (MDP_STATUS_OK == ReadPlatformIDAndChipID(&sPlatformInfo.sEFIPlatformType, &sPlatformInfo.sEFIChipSetId, &sPlatformInfo.sEFIChipSetFamily))
    {
      uint32 uTemp                    = 0;
      sPlatformInfo.bPlatformDetected = TRUE;

      // Read the display panel ID override, it will be checked later during detection
      if (MDP_STATUS_OK == MDP_GetEnvironmentVariable(PANEL_OVERRIDE_STRINGNAME, &uTemp, sizeof(uTemp)))
      {
          sPlatformInfo.uPanelIdOverride =  uTemp;
      }
    }

    /*
     * Napali QRD device panels need 180 inverted
     */
    if ((sPlatformInfo.sEFIChipSetFamily == EFICHIPINFO_FAMILY_SDM845) &&
        (sPlatformInfo.sEFIPlatformType.platform == EFI_PLATFORMINFO_TYPE_QRD))
    {
          sPlatformInfo.uPanelOrientationOverride = MDP_ROTATE_180;
    }

    // Parse sub-revision specific info to determine the QRD platform type
    uPlatformSubtype      = sPlatformInfo.sEFIPlatformType.subtype         & 0xFF;
    uHardwareVersionMajor = (sPlatformInfo.sEFIPlatformType.version >> 16) & 0xFF;
    uHardwareVersionMinor = sPlatformInfo.sEFIPlatformType.version         & 0xFF;

    /* Setup display panel configurations according to CDT */
    SetupPlatformPanelConfig(eDisplayId, &sPlatformPanel, &sPlatformInfo);
  }

  switch (ePlatformConfig)
  {
  case MDPPLATFORM_CONFIG_SW_RENDERER:
  
    if (NULL == pPlatformParams)
    {
      eStatus = MDP_STATUS_BAD_PARAM;
    }
    else if (TRUE == sPlatformInfo.bSWRender)
    {
      // SW Render mode is enabled already, just return the status
      pPlatformParams->sPlatformInfo.bSWRender = TRUE;
    }
    else if ((EFI_PLATFORMINFO_TYPE_UNKNOWN == sPlatformInfo.sEFIPlatformType.platform) ||
             (EFI_PLATFORMINFO_TYPE_VIRTIO  == sPlatformInfo.sEFIPlatformType.platform) ||
             (EFI_PLATFORMINFO_TYPE_RUMI    == sPlatformInfo.sEFIPlatformType.platform) ||
             (TRUE == pPlatformParams->sPlatformInfo.bSWRenderOverrride)                ||
             (TRUE == PcdGetBool(PcdDisplayForceSwRenderer)))
    {
      // Force SW render mode for emulation and unknown platforms
      pPlatformParams->sPlatformInfo.bSWRender = TRUE;
      // Store that an override has been requested by the caller
      sPlatformInfo.bSWRenderOverrride         = pPlatformParams->sPlatformInfo.bSWRenderOverrride;
      // Cache that SW Rendering is enabled
      sPlatformInfo.bSWRender                  = TRUE;
      DEBUG((EFI_D_ERROR, "DisplayDxe: SW renderer mode enabled!\n"));
    }
    else
    {
      // Force SW render mode off
      sPlatformInfo.bSWRender                  = FALSE;      
      // Report SW render mode is disabled
      pPlatformParams->sPlatformInfo.bSWRender = FALSE;
    }
    break;
  case MDPPLATFORM_CONFIG_GETPANELCONFIG:
    {
      if (NULL == pPlatformParams)
      {
        eStatus = MDP_STATUS_BAD_PARAM;
      }
      else
      {
        // Retrieve panel configuration (could be dependent on the interface)
        switch (eDisplayId)
        {
        case MDP_DISPLAY_PRIMARY:
          {
            uint32                    uPanelCnt  = 0;
            uint32                    uPanelID   = 0;
            PlatformDSIDetectParams  *pPanelList = NULL;

            if (MDPPLATFORM_PANEL_NONE != sPlatformPanel.eSelectedPanel)
            {
              /* Panel has been selected, use the selected panel */
            }
            else if (MDP_STATUS_OK != DynamicDSIPanelDetection(&sPlatformPanel, &uPanelID, pPanelList, uPanelCnt))
            {
              /* Not detected, use defualt panel */
            }
            else
            {
              /* Detected */
              sPlatformPanel.uPanelId       = uPanelID;
              sPlatformInfo.uPrimaryPanelId = uPanelID;
            }

            // Get the panel xml configurations.
            if(MDP_STATUS_OK != GetPanelXmlConfig(&sPlatformPanel))
		    {
              pPlatformParams->sPlatformPanel.pPanelXMLConfig = (int8 *)dummy_xmldata;
              pPlatformParams->sPlatformPanel.uConfigSize     = sizeof (dummy_xmldata);
            }
            else
            {
              /* Copy the panel configurations to pPlatformParams->sPlatformPanel */
              MDP_OSAL_MEMCPY(&pPlatformParams->sPlatformPanel, &sPlatformPanel, sizeof(MDPPlatformPanelInfo));
            }
          }
          break;
  
        case MDP_DISPLAY_EXTERNAL: 
          switch (PcdGet32(PcdExtDisplayType))
          {
            case PCD_EXTERNAL_DISPLAY_DP:
              pPlatformParams->sDPConfig.bInvertPlugPolarity = DP_USBPlugInvertedOrientation();
              
              // For Nazgul, DP Phy lane to pad connection is the reverse of DP alt mode over usb type-c spec defined mapping
              pPlatformParams->sDPConfig.bReversePhyDataPath = TRUE;
              break;
          
            default:
              break;
          }
          break;
  
        default:
          break;
        }
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
        case EFI_PLATFORMINFO_TYPE_CLS:
          // Primary Power Sequence
          if (NULL != sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_PowerUp)
          {
            if (MDP_STATUS_OK != (eStatus = sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_PowerUp(eDisplayId, &sPanelPowerCtrl)))
            {
              DEBUG((EFI_D_WARN, "DisplayDxe: Primary Power Up Sequence failed (%d)\n", eStatus));
            }
          }

          if (NULL != sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Peripheral_Power)
          {
            if (MDP_STATUS_OK != (eStatus = sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Peripheral_Power(eDisplayId, &sPanelPowerCtrl, TRUE)))
            {
              DEBUG((EFI_D_WARN, "DisplayDxe: Secondary Power Up Sequence failed (%d)\n", eStatus));
            }
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
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_QRD:
          eStatus = ExternalPanel_Default_PowerUp(eDisplayId, &sPanelPowerCtrl);
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

  case MDPPLATFORM_CONFIG_GETPANELDTINFO:
    if (NULL == pPlatformParams)
    {
      eStatus = MDP_STATUS_BAD_PARAM;
    }
    else
    {
      pPlatformParams->psPanelDTInfo = (PanelDTInfoType*)&fastBootPanelList;
    }
    break;

  case MDPPLATFORM_CONFIG_GETPANELLIST:
      {
        if (NULL == pPlatformParams)
        {
          eStatus = MDP_STATUS_BAD_PARAM;
        }
        else
        {
          uint32 i = 0;
          
          while (MDPPLATFORM_PANEL_NONE != fastBootPanelList[i].ePanel)
          {
            //Copy Panel ID
            pPlatformParams->sPanelList.ePanel[i] = fastBootPanelList[i].ePanel;
            i++; 
          }
  
          pPlatformParams->sPanelList.uCount = i;
        }
      }
      break;

  case MDPPLATFORM_CONFIG_GETPANELSUPPORTFLAGS:
    {
      if (NULL == pPlatformParams)
      {
        eStatus = MDP_STATUS_BAD_PARAM;
      }
      else
      {
        //Lookup panel
        int32 i = PANEL_LIST_LENGTH(fastBootPanelList) - 1;
        while (0 <= i)
        {
          if (pPlatformParams->sPanelSupport.ePanel == fastBootPanelList[i].ePanel)
          {
            if ((DISP_TOPOLOGY_CONFIG_NONE == pPlatformParams->sPanelSupport.uModeIndex) ||
                (pPlatformParams->sPanelSupport.uModeIndex > DISPLAY_MODE_MAX))
            {
              pPlatformParams->sPanelSupport.uFlags = fastBootPanelList[i].uModeFlags[0];
            }
            else
            {
              pPlatformParams->sPanelSupport.uFlags = fastBootPanelList[i].uModeFlags[pPlatformParams->sPanelSupport.uModeIndex-1];
            }
            break;
          }
          i--;
        }
      }
    }
    break;

  case MDPPLATFORM_CONFIG_POWERDOWN:
    {
      // Handle power down
      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:
  
        // Config based on the platform
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_QRD:
        case EFI_PLATFORMINFO_TYPE_CLS:
          if (NULL != sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_PowerDown)
          {
            if (MDP_STATUS_OK != (eStatus = sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_PowerDown(eDisplayId, &sPanelPowerCtrl)))
            {
              DEBUG((EFI_D_WARN, "DisplayDxe: Primary Power Down Sequence failed (%d)\n", eStatus));
            }
          }

          /* Secondary Power Sequence */
          if (NULL != sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Peripheral_Power)
          {
            if (MDP_STATUS_OK != (eStatus = sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Peripheral_Power(eDisplayId, &sPanelPowerCtrl, FALSE)))
            {
              DEBUG((EFI_D_WARN, "DisplayDxe: Secondary Power Down Sequence failed (%d)\n", eStatus));
            }
          }

          sPanelPowerCtrl.bBacklightEnabled = FALSE; /* wled is turned off */
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
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_QRD:
          eStatus = ExternalPanel_Default_PowerDown(eDisplayId, &sPanelPowerCtrl);
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

  case MDPPLATFORM_CONFIG_SETBACKLIGHT:
    {
      if (NULL == pPlatformParams)
      {
        eStatus = MDP_STATUS_BAD_PARAM;
      }
      else
      {
        // Handle backlight level
        switch (eDisplayId)
        {
        case MDP_DISPLAY_PRIMARY:
          switch (sPlatformInfo.sEFIPlatformType.platform)
          {
          case EFI_PLATFORMINFO_TYPE_CDP:
          case EFI_PLATFORMINFO_TYPE_MTP:
          case EFI_PLATFORMINFO_TYPE_QRD:
          case EFI_PLATFORMINFO_TYPE_CLS:
            /* Copy the I2C configurations pPlatformParams->sBacklightConfig to sPanelPowerCtrl */
            MDP_OSAL_MEMCPY(&sPanelPowerCtrl.sI2CConfig, &pPlatformParams->sBacklightConfig.uBacklightCntrl.sI2CConfig, sizeof(MDP_I2C_Configuration));

            if (FALSE == sPanelPowerCtrl.bBacklightEnabled)
            {
              if (NULL != sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Brightness_Enable)
              {
                if (MDP_STATUS_OK != (eStatus = sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Brightness_Enable(eDisplayId, &sPanelPowerCtrl)))
                {
                  DEBUG((EFI_D_WARN, "MDPPlatformConfig: Backlight enabled failed\n"));
                }
              }

              sPanelPowerCtrl.bBacklightEnabled = TRUE;
            }

            if (NULL != sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Brightness_Level)
            {
              if (MDP_STATUS_OK != (eStatus = sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Brightness_Level(eDisplayId, &pPlatformParams->sBacklightConfig)))
              {
                DEBUG((EFI_D_WARN, "MDPPlatformConfig: Backlight level control failed\n"));
              }
            }
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
    }
    break;

  case MDPPLATFORM_CONFIG_GETPANELID:
    {
    }
    break;

  case MDPPLATFORM_CONFIG_GETPLATFORMINFO:
    {
      if (NULL == pPlatformParams)
      {
        eStatus = MDP_STATUS_BAD_PARAM;
      }
      else
      {
        //
        // Return platform information
        //
        MDP_OSAL_MEMCPY(&pPlatformParams->sPlatformInfo, &sPlatformInfo, sizeof(MDPPlatformInfo));
      }
    }
    break;

  case MDPPLATFORM_CONFIG_RESETPANEL:
    {
      if (NULL == pPlatformParams)
      {
        eStatus = MDP_STATUS_BAD_PARAM;
      }
      else
      {
         if (pPlatformParams->sPlatformPanelReset.uResetGpio == 0)
         {
           sPanelPowerCtrl.uResetGpio = Default_DISP_RESET_GPIO;
           pPlatformParams->sPlatformPanelReset.uResetGpio = Default_DISP_RESET_GPIO;
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
           case EFI_PLATFORMINFO_TYPE_CLS:
             if (NULL != sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Reset)
             {
               if (MDP_STATUS_OK != (eStatus = sMDPPlatformPanelFunction[sPlatformPanel.eSelectedPanel].pPanel_Reset(&pPlatformParams->sPlatformPanelReset)))
               {
                DEBUG((EFI_D_WARN, "MDPPlatformConfig: Panel reset failed (%d)\n", eStatus));
               }
             }
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
    }
    break;

  case MDPPLATFORM_CONFIG_SETGPIOSTATE:
    {
      if (NULL == pPlatformParams)
      {
        eStatus = MDP_STATUS_BAD_PARAM;
      }
      else
      {
        eStatus = SetDefaultGPIOState(&pPlatformParams->sGPIOState);
      }
    }
    break;

  case MDPPLATFORM_CONFIG_GETPANELXMLINFO:
    {
      if (NULL == pPlatformParams)
      {
        eStatus = MDP_STATUS_BAD_PARAM;
      }
      else
      {
        // Return the raw xml information if it is within range.
        if (pPlatformParams->sPanelXMLInfo.uIndex >= PANEL_LIST_LENGTH(uefiPanelList))
        {
          eStatus = MDP_STATUS_FAILED;
        }
        else
        {
          MDPPlatformPanelInfo sPlatformPanelInfo;

          MDP_OSAL_MEMZERO(&sPlatformPanelInfo, sizeof(MDPPlatformPanelInfo));
          sPlatformPanelInfo.eSelectedPanel = uefiPanelList[pPlatformParams->sPanelXMLInfo.uIndex].eSelectedPanel;

          if(MDP_STATUS_OK == GetPanelXmlConfig(&sPlatformPanelInfo))
		  {
            pPlatformParams->sPanelXMLInfo.pPanelXMLConfig = (int8*)sPlatformPanelInfo.pPanelXMLConfig;
            pPlatformParams->sPanelXMLInfo.uPanelXMLSize   = sPlatformPanelInfo.uConfigSize;
          }
          else
          {
            pPlatformParams->sPanelXMLInfo.pPanelXMLConfig = (int8 *)dummy_xmldata;
            pPlatformParams->sPanelXMLInfo.uPanelXMLSize   = sizeof (dummy_xmldata);
          }
        }
      }
    }
    break;

  case MDPPLATFORM_CONFIG_BYPASS:
    {
      // SW Render Bypass mode
      eStatus = MDP_STATUS_OK;
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

/**********************************************************************************************
*
* FUNCTION: ReadPlatformIDAndChipID()
*
* DESCRIPTION:
*   Read the platform info.
*
***********************************************************************************************/
static MDP_Status ReadPlatformIDAndChipID(EFI_PLATFORMINFO_PLATFORM_INFO_TYPE *pPlatformInfo, EFIChipInfoIdType *pChipSetId, EFIChipInfoFamilyType *pChipSetFamily)
{
  MDP_Status                           eStatus                = MDP_STATUS_OK;
  EFI_PLATFORMINFO_PROTOCOL            *hPlatformInfoProtocol;
  EFI_CHIPINFO_PROTOCOL                *hChipInfoProtocol;

  if (EFI_SUCCESS == gBS->LocateProtocol(&gEfiPlatformInfoProtocolGuid,
                                         NULL,
                                         (VOID **)&hPlatformInfoProtocol))
  {
    if (EFI_SUCCESS != hPlatformInfoProtocol->GetPlatformInfo(hPlatformInfoProtocol, pPlatformInfo))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: gEfiPlatformInfoProtocolGuid->GetPlatformInfo() failed.\n"));

      eStatus = MDP_STATUS_FAILED;
    }
  }
  else
  {
    DEBUG((EFI_D_WARN, "DisplayDxe: gEfiPlatformInfoProtocolGuid protocol failed.\n"));

    eStatus = MDP_STATUS_FAILED;
  }

  if (EFI_PLATFORMINFO_TYPE_UNKNOWN == pPlatformInfo->platform)
  {
    pPlatformInfo->platform = EFI_PLATFORMINFO_TYPE_VIRTIO;
  }

  // Read the chipset ID
  if (EFI_SUCCESS == gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,
                                         NULL,
                                         (VOID **)&hChipInfoProtocol))
  {
    if (EFI_SUCCESS != hChipInfoProtocol->GetChipId(hChipInfoProtocol, pChipSetId))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: gEfiChipInfoProtocolGuid->GetChipId() failed. \n"));

      eStatus = MDP_STATUS_FAILED;
    }
    if (EFI_SUCCESS != hChipInfoProtocol->GetChipFamily(hChipInfoProtocol, pChipSetFamily))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: gEfiChipInfoProtocolGuid->GetChipFamily() failed. \n"));

      eStatus = MDP_STATUS_FAILED;
    }

  }
  else
  {
    DEBUG((EFI_D_WARN, "DisplayDxe: gEfiChipInfoProtocolGuid protocol failed.\n"));

    eStatus = MDP_STATUS_FAILED;
  }

  return eStatus;
}


/**********************************************************************************************
*
* FUNCTION: SetDefaultGPIOState()
*
* DESCRIPTION:
*   The function toggles a list of GPIO's based on the requested configuration.
*
***********************************************************************************************/
static MDP_Status SetDefaultGPIOState(GpioStateType *pGPIOList)
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
           EFI_TLMM_PROTOCOL   *TLMMProtocol;        

           if (EFI_SUCCESS == gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void**)&TLMMProtocol))
           {
             uint32 uCount;          

             // Loop the list to configure all GPIOs.
             for (uCount=0;uCount<pGPIOList->uListSize;uCount++)
             {
                // Only try to program GPIOs that are within range, TLMM Macro (EFI_GPIO_CFG) supports up to 0x3FF
                if (pGPIOList->pGPIOList[uCount] < 0x3FF) 
                {
                  uint32 uValue = (pGPIOList->uState>0)?GPIO_HIGH_VALUE:GPIO_LOW_VALUE;
                  
                  // Setup GPIO
                  if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(pGPIOList->pGPIOList[uCount], 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), TLMM_GPIO_ENABLE))
                  {
                    DEBUG((EFI_D_ERROR, "DisplayDxe: Error configuring TLMM GPIO%d\n", uCount));

                  }

                  // Setup Output
                  if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(pGPIOList->pGPIOList[uCount], 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), uValue))
                  {
                    DEBUG((EFI_D_ERROR, "DisplayDxe: Error configuring TLMM GPIO%d\n", uCount));
                  }              
                }
             }
           }
           else
           {
             eStatus = MDP_STATUS_NO_RESOURCES;
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


/**********************************************************************************************
*
* FUNCTION: GetPanelXmlConfig()
*
* DESCRIPTION:
*   The function can get the panel xml configurations.
*
***********************************************************************************************/
static MDP_Status GetPanelXmlConfig (MDPPlatformPanelInfo *pPlatformPanel)
{
  MDP_Status    eStatus            = MDP_STATUS_OK;
  UINT32        uRetSize           = 0;
  UINT8        *pFileBuffer        = NULL;
  UINT32        uBufferSize        = 0;

  if (NULL == pPlatformPanel)
  {
    eStatus = MDP_STATUS_NO_RESOURCES;
  }
  else if (NULL == sMDPPlatformPanelFunction[pPlatformPanel->eSelectedPanel].pPanelXml)
  {
    eStatus = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != Display_Utils_LoadFile(sMDPPlatformPanelFunction[pPlatformPanel->eSelectedPanel].pPanelXml,
                                                &pFileBuffer,
                                                &uBufferSize,
                                                &uRetSize))
  {
    eStatus = MDP_STATUS_FAILED;
  }
  else if (NULL != pFileBuffer)
  {
    pPlatformPanel->pPanelXMLConfig = (int8 *)pFileBuffer;
    pPlatformPanel->uConfigSize     = uRetSize;
  }
  else
  {
    /* Read null from the file, just return with fail status */
    eStatus = MDP_STATUS_FAILED;
  }

  return eStatus;
}


/**********************************************************************************************
*
* FUNCTION: SetupPlatformPanelConfig()
*
* DESCRIPTION:
*   The function can get the display panel that is being used.
*
***********************************************************************************************/
static void SetupPlatformPanelConfig(MDP_Display_IDType eDisplayId, MDPPlatformPanelInfo *pPlatformPanel, MDPPlatformInfo *pPlatformInfo)
{
  PlatformDSIDetectParams *pPanelList = NULL;
  uint32                   uPanelCnt  = 0;
  uint32                   uPanelID   = 0;

  if (NULL == pPlatformPanel)
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: The platform panel info is NULL\n"));
  }
  else if (TRUE == Panel_CheckOverride(eDisplayId, pPlatformPanel, pPlatformInfo))
  {
    //Use configuration set by panel override
  }
  else if (TRUE == pPlatformInfo->bSWRender)
  {
    //use dummy panel for sw rendering
    pPlatformPanel->eSelectedPanel   = MDPPLATFORM_PANEL_NONE;
  }
  else
  {
    // Report the proper information depending on the display.
    switch (pPlatformInfo->sEFIPlatformType.platform)
    {
    case EFI_PLATFORMINFO_TYPE_CDP:
    case EFI_PLATFORMINFO_TYPE_MTP:
    case EFI_PLATFORMINFO_TYPE_QRD:
      if (pPlatformInfo->sEFIPlatformType.subtype == 3)
      {
        pPanelList                      = &uefiPanelList[6]; // Panel #6 - Dummy 640x480 video mode panel  ID:0xFFFF
        uPanelCnt                       = 1;
        pPlatformInfo->uPanelIdOverride =  0xffffff;
      }
      else if (pPlatformInfo->sEFIPlatformType.subtype == 1)
      {
        pPanelList                      = &uefiPanelList[4]; // Panel #4 - Sharp 4k DSC Command Mode Panel (2160x3840 24bpp) ID:0x8000;
        uPanelCnt                       = 1;
      }
      else
      {
        pPanelList                      = &uefiPanelList[0]; // Panel #0 - Truly DSC DSI Command Mode Panel (1440x2560 24bpp) ID:0x8012
        uPanelCnt                       = 1;
      }

      pPlatformPanel->eSelectedPanel    = pPanelList->eSelectedPanel;
      uPanelID                          = ((pPanelList->panelIdCommands[1].expectedReadback[0] << 8) |
                                            pPanelList->panelIdCommands[2].expectedReadback[0]);
      pPlatformPanel->uPanelId          = uPanelID;
      pPlatformInfo->uPrimaryPanelId    = uPanelID;

      // If the panel ID override is enabled and within range, force that panel configuration.
      if ((pPlatformInfo->uPanelIdOverride & PANEL_OVERRIDE_ENABLE_MASK) &&
          (PANEL_OVERRIDE_PANELID(pPlatformInfo->uPanelIdOverride) < PANEL_LIST_LENGTH(uefiPanelList)))
      {
        pPanelList                                      = &uefiPanelList[PANEL_OVERRIDE_PANELID(pPlatformInfo->uPanelIdOverride)];

        pPlatformPanel->eSelectedPanel  = pPanelList->eSelectedPanel;
        uPanelID                        = ((pPanelList->panelIdCommands[1].expectedReadback[0] << 8) |
                                            pPanelList->panelIdCommands[2].expectedReadback[0]);
        pPlatformPanel->uPanelId        = uPanelID;
        pPlatformInfo->uPrimaryPanelId  = uPanelID;
        DEBUG((EFI_D_ERROR, "DisplayDxe: Panel override enabled (Index: %d ID:0x%x)\n", PANEL_OVERRIDE_PANELID(pPlatformInfo->uPanelIdOverride), uPanelID));
      }
      break;

    case EFI_PLATFORMINFO_TYPE_CLS:
      pPlatformPanel->eSelectedPanel    = MDPPLATFORM_PANEL_CLS_SN65DSIx6_WQHD_VIDEO;
      pPlatformPanel->uPanelId          = 0x000000C3;
      pPlatformInfo->uPrimaryPanelId    = pPlatformPanel->uPanelId;
      break;

    default:
      pPlatformPanel->eSelectedPanel    = MDPPLATFORM_PANEL_NONE;
      break;
    }
  }
}

#ifdef __cplusplus
}
#endif
