/*=============================================================================
 
  File: MDPPlatformLibPanelConfig.h

  MDP platform specific types

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#ifndef __MDP_PLATFORM_LIB_PANEL_CONFIG__H__
#define __MDP_PLATFORM_LIB_PANEL_CONFIG__H__

#include "MDPPlatformLibPanelCommon.h"



/* Default platform panel functions - From MDPPlatformLibPanelCommon.c */
extern MDP_Status Panel_Default_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_Default_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_Default_Peripheral_Power(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp);
extern MDP_Status Panel_Default_Brightness_Enable(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_Default_Brightness_Level(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig);
extern MDP_Status Panel_Default_Reset(MDPPlatformPanelResetInfo *pResetInfo);

/* CLS Sn65dsix6_Wqhd_Vid panel functions - From Panel_cls_sn65dsix6_wqhd_vid.c */
extern MDP_Status Panel_PowerUp_Cls_Sn65dsix6_Wqhd_Vid(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_PowerDown_Cls_Sn65dsix6_Wqhd_Vid(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_Brightness_Enable_Cls_Sn65dsix6_Wqhd_Vid(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_Brightness_Level_Cls_Sn65dsix6_Wqhd_Vid(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig);
extern MDP_Status Panel_Reset_Cls_Sn65dsix6_Wqhd_Vid(MDPPlatformPanelResetInfo *pResetInfo);

/* MDPPlatformPanelFunctionTable Definition :
 * typedef struct
 * {
 *   // The default panel selected when setting up platform panel configuration.
 *   MDPPlatformPanelType ePanelSelected;
 *
 *   // The name of the configuration xml file for the panel selected.
 *   CHAR8               *pPanelXml;
 *
 *   // Panel power up function
 *   MDP_Status (*pPanel_PowerUp)           (MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
 *
 *   // Panel power down function
 *   MDP_Status (*pPanel_PowerDown)         (MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
 *
 *   // Panel reset function
 *   MDP_Status (*pPanel_Reset)             (MDPPlatformPanelResetInfo *pResetInfo);
 *
 *   // Panel peripheral power function
 *   MDP_Status (*pPanel_Peripheral_Power)  (MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp);
 *
 *   // Panel brightness enable function
 *   MDP_Status (*pPanel_Brightness_Enable) (MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
 *
 *   // Panel brightness level control function
 *   MDP_Status (*pPanel_Brightness_Level)  (MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig);
 * } MDPPlatformPanelFunctionTable;
 */
MDPPlatformPanelFunctionTable sMDPPlatformPanelFunction[MDPPLATFORM_PANEL_MAX] =
{
  {
    MDPPLATFORM_PANEL_NONE,                                 // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SIM_VIDEO_PANEL,                      // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SIM_DUALDSI_VIDEO_PANEL,              // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SIM_CMD_PANEL,                        // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SIM_DUALDSI_CMD_PANEL,                // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SIM_DSC_375_CMD_PANEL,                // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SIM_DUAL_DSC_375_CMD_PANEL,           // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SHARP_4K_DSC_CMD,                     // ePanelSelected
    "Panel_sharp_4k_dsc_cmd.xml",                           // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_SHARP_4K_DSC_VIDEO,                   // ePanelSelected
    "Panel_sharp_4k_dsc_vid.xml",                           // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_TRULY_WQXGA_DSC_CMD,                  // ePanelSelected
    "Panel_truly_wqxga_dsc_cmd.xml",                        // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_TRULY_WQXGA_DSC_VIDEO,                // ePanelSelected
    "Panel_truly_wqxga_dsc_vid.xml",                        // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_CMD,              // ePanelSelected
    "Panel_truly_wqxga_dual_cmd.xml",                       // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_VIDEO,            // ePanelSelected
    "Panel_truly_wqxga_dual_vid.xml",                       // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_SHARP_1080P_CMD,                      // ePanelSelected
    "Panel_sharp_1080p_cmd.xml",                            // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_SHARP_120HZ_1080P_CMD,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SAMSUNG_WQHD_AMOLED_DSC_CMD,          // ePanelSelected
    "Panel_ss_mg2_wqhd_dsc_cmd.xml",                        // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_SAMSUNG_1080P_AMOLED_DSC_CMD,         // ePanelSelected
    "Panel_ss_mg2_1080p_dsc_cmd.xml",                       // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_SAMSUNG_720P_AMOLED_CMD,              // ePanelSelected
    "Panel_ss_mg2_720p_cmd.xml",                            // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_SAMSUNG_WQHD_AMOLED_DUAL_CMD,         // ePanelSelected
    "Panel_ss_mg1_wqhd_dual_cmd.xml",                       // pPanelXmlConfig
    Panel_Default_PowerUp,                                  // pPanel_PowerUp
	Panel_Default_PowerDown,                                // pPanel_PowerDown
	Panel_Default_Reset,                                    // pPanel_Reset
	Panel_Default_Peripheral_Power,                         // pPanel_Peripheral_Power
	Panel_Default_Brightness_Enable,                        // pPanel_Brightness_Enable
	Panel_Default_Brightness_Level                          // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_SHARP_WQHD_DUALDSI_CMD,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_SHARP_WQHD_DUALDSI_VIDEO,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_CLS_SN65DSIx6_WQHD_VIDEO,             // ePanelSelected
    "Panel_cls_sn65dsix6_wqhd_vid.xml",                     // pPanelXmlConfig
    Panel_PowerUp_Cls_Sn65dsix6_Wqhd_Vid,                   // pPanel_PowerUp
    Panel_PowerDown_Cls_Sn65dsix6_Wqhd_Vid,                 // pPanel_PowerDown
    Panel_Reset_Cls_Sn65dsix6_Wqhd_Vid,                     // pPanel_Reset
    NULL,                                                   // pPanel_Peripheral_Power
    Panel_Brightness_Enable_Cls_Sn65dsix6_Wqhd_Vid,         // pPanel_Brightness_Enable
    Panel_Brightness_Level_Cls_Sn65dsix6_Wqhd_Vid           // pPanel_Brightness_Level
  },
};

#endif // __MDP_PLATFORM_LIB_PANEL_CONFIG__H__
