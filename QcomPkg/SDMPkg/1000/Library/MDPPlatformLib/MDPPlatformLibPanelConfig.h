/*=============================================================================
 
  File: MDPPlatformLibPanelConfig.h

  MDP platform specific types

  Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
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

extern MDP_Status Panel_CLS_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_CLS_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_CLS_Peripheral_Power(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp);
extern MDP_Status Panel_CLS_Brightness_Enable(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_CLS_Brightness_Level(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig);
extern MDP_Status Panel_CLS_Reset(MDPPlatformPanelResetInfo *pResetInfo);

extern MDP_Status Panel_ADP_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_ADP_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);

extern MDP_Status Panel_BondedDP_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
extern MDP_Status Panel_BondedDP_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);

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
    MDPPLATFORM_PANEL_SHARP_4K_EDP,                         // ePanelSelected
    "Panel_sharp_4k_edp.xml",                               // pPanelXmlConfig
    Panel_CLS_PowerUp,                                      // pPanel_PowerUp
    Panel_CLS_PowerDown,                                    // pPanel_PowerDown
    Panel_CLS_Reset,                                        // pPanel_Reset
    Panel_CLS_Peripheral_Power,                             // pPanel_Peripheral_Power
    Panel_CLS_Brightness_Enable,                            // pPanel_Brightness_Enable
    Panel_CLS_Brightness_Level                              // pPanel_Brightness_Level
  },
  {
    MDPPLATFORM_PANEL_SHARP_1080P_CMD,                      // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_TRULY_1080P_CMD,                      // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_TRULY_1080P_VIDEO,                    // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_BOE_AMOLED_WQHD_DSC_CMD,              // ePanelSelected
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
  },
  {
    MDPPLATFORM_PANEL_AUO_1080P_EDP,                        // ePanelSelected
    "Panel_auo_1080p_edp.xml",                              // pPanelXmlConfig
    Panel_CLS_PowerUp,                                      // pPanel_PowerUp
    Panel_CLS_PowerDown,                                    // pPanel_PowerDown
    Panel_CLS_Reset,                                        // pPanel_Reset
    Panel_CLS_Peripheral_Power,                             // pPanel_Peripheral_Power
    Panel_CLS_Brightness_Enable,                            // pPanel_Brightness_Enable
    Panel_CLS_Brightness_Level                              // pPanel_Brightness_Level
  },
  { /* Poipu ADP Primary Panel */
    MDPPLATFORM_PRI_PANEL_ANX7625_DSI2DP_FHD_VIDEO,         // ePanelSelected
    "Panel_primary_anx7625_dsi2dp_fhd_vid.xml",             // pPanelXmlConfig
    Panel_ADP_PowerUp,                                      // pPanel_PowerUp
    Panel_ADP_PowerDown,                                    // pPanel_PowerDown
    Panel_Default_Reset,                                    // pPanel_Reset
    NULL, NULL, NULL                     
  },
  { /* Poipu ADP Secondary Panel */
    MDPPLATFORM_SEC_PANEL_ANX7625_DSI2DP_FHD_VIDEO,         // ePanelSelected
    "Panel_secondary_anx7625_dsi2dp_fhd_vid.xml",           // pPanelXmlConfig
    Panel_ADP_PowerUp,                                      // pPanel_PowerUp
    Panel_ADP_PowerDown,                                    // pPanel_PowerDown
    Panel_Default_Reset,                                    // pPanel_Reset
    NULL, NULL, NULL
  },
};

#endif // __MDP_PLATFORM_LIB_PANEL_CONFIG__H__
