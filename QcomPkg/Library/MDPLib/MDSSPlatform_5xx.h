#ifndef __MDSSPlatform_5xx_H__
#define __MDSSPlatform_5xx_H__
/*=============================================================================
 
  File: MDSSPlatform_5xx.h
 
  Internal header file for MDSS 5xx library
  
 
  Copyright (c) 2011-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "MDPLib_i.h"

/*===========================================================================
                                Defines 
===========================================================================*/

#define RES_MAX_FAMILY_MAJOR                      6
#define RES_MAX_FAMILY_MINOR                      2

#define XO_DEFAULT_FREQ_IN_HZ                     19200000


 /*=========================================================================
     MDSS 5.x.x Tables
 ==========================================================================*/


 MDPExternalClockEntry            sDSI0ExtClocks_5xx[] = 
  {
    {"disp_cc_mdss_esc0_clk",      0, 1, 0, 0, 0, 0},     // Index 0 : primary source :XO , Secondary source : dsi pll
    {"disp_cc_mdss_pclk0_clk",     1, 0, 0, 0, 0, 0},     // Index 1 : Source DSI0_PLL
    {"disp_cc_mdss_byte0_clk",     1, 0, 0, 0, 0, 0},     // Index 2 : Source DSI0_PLL
    {"disp_cc_mdss_byte0_intf_clk",1, 0, 2, 0, 0, 0, 2},  // Index 3 : Source DSI0_PLL
    {"\0",                         0, 0, 0, 0, 0, 0},  
  };

  MDPExternalClockEntry            sDSI1ExtClocks_5xx[] = 
  {
    {"disp_cc_mdss_esc1_clk",      0, 1, 0, 0, 0, 0},     // Index 0 : primary source :XO , Secondary source : dsi pll
    {"disp_cc_mdss_pclk1_clk",     2, 0, 0, 0, 0, 0},     // Index 1 : Source DSI1_PLL
    {"disp_cc_mdss_byte1_clk",     2, 0, 0, 0, 0, 0},     // Index 2 : Source DSI1_PLL
    {"disp_cc_mdss_byte1_intf_clk",2, 0, 2, 0, 0, 0, 2},  // Index 3 : Source DSI1_PLL
    {"\0",                         0, 0, 0, 0, 0, 0},  
  };

  /*For Dual DSI Split display, DSI0/DSI1 share same PLL-- DSI0_PLL */
 MDPExternalClockEntry            sDSI1SharedSourceExtClocks_5xx[] = 
  {
    {"disp_cc_mdss_esc1_clk",      0, 1, 0, 0, 0, 0},     // Index 0 : primary source :XO , Secondary source : dsi pll
    {"disp_cc_mdss_pclk1_clk",     1, 0, 0, 0, 0, 0},     // Index 1 : Source DSI0_PLL
    {"disp_cc_mdss_byte1_clk",     1, 0, 0, 0, 0, 0},     // Index 2 : Source DSI0_PLL
    {"disp_cc_mdss_byte1_intf_clk",1, 0, 2, 0, 0, 0, 2},  // Index 3 : Source DSI0_PLL
    {"\0",                         0, 0, 0, 0, 0, 0},  
  }; 
 
  /* MDSS Clock list
   * Note: Clocks are ordered in order to ensure Ahb access is available prior to accessing the core.
   */
  const MDPClockEntry MDSSClocks_5xx[] =
  {
      {"gcc_disp_hf_axi_clk",           240000000, NULL, 0x0},
      {"disp_cc_mdss_ahb_clk",                  0, NULL, 0x0},
      {"disp_cc_mdss_mdp_clk",          345000000, NULL, 0x0},
      {"disp_cc_mdss_rot_clk",                  0, NULL, 0x0},
      {"disp_cc_mdss_vsync_clk",                0, NULL, 0x0},
      {"\0",                                    0, NULL}
  };
  
  /* DSI0 Clock list
   */
  const MDPClockEntry DSI0Clocks_5xx[] =
  {
      {"disp_cc_mdss_esc0_clk",         0, NULL, 0x0},
      {"disp_cc_mdss_byte0_clk",        0, NULL, 0x0},
      {"disp_cc_mdss_byte0_intf_clk",   0, NULL, 0x0},
      {"disp_cc_mdss_pclk0_clk",        0, NULL, 0x0},
      {"\0", 0, NULL}
  };
  
  /* DSI1 Clock list
   */
  const MDPClockEntry DSI1Clocks_5xx[] =
  {
      {"disp_cc_mdss_esc1_clk",         0, NULL, 0x0},
      {"disp_cc_mdss_byte1_clk",        0, NULL, 0x0},
      {"disp_cc_mdss_byte1_intf_clk",   0, NULL, 0x0},
      {"disp_cc_mdss_pclk1_clk",        0, NULL, 0x0},
      {"\0", 0, NULL}
  };

  /*
   * disp_cc_mdss_dp_Pixel_clk  is for DP0 SST
   * disp_cc_mdss_dp_Pixel1_clk is for DP0 + DP1 MST
   * disp_cc_mdss_dp_Pixel2_clk is for DP1 SST
   */
  
  /* DP 0 Clock list
   */
  const MDPClockEntry DP0Clocks_5xx[] =
  {
      {"disp_cc_mdss_dp_pixel_clk",     0, NULL, 0x0},
      {"disp_cc_mdss_dp_link_clk",      0, NULL, 0x0},
      {"disp_cc_mdss_dp_crypto_clk",    0, NULL, 0x0},
      {"disp_cc_mdss_dp_aux_clk",       0, NULL, 0x0},
      {"disp_cc_mdss_dp_link_intf_clk", 0, NULL, 0x0},
      {"\0", 0, NULL}
  
  };

  /* DP 1 Clock list
   */
  const MDPClockEntry DP1Clocks_5xx[] =
  {
      {"disp_cc_mdss_dp_pixel2_clk",     0, NULL, 0x0},
      {"disp_cc_mdss_dp_link1_clk",      0, NULL, 0x0},
      {"disp_cc_mdss_dp_crypto1_clk",    0, NULL, 0x0},
      {"disp_cc_mdss_dp_aux1_clk",       0, NULL, 0x0},
      {"disp_cc_mdss_dp_link1_intf_clk", 0, NULL, 0x0},
      {"\0", 0, NULL}
  
  };
  
  /* eDP Clock list
   */
  const MDPClockEntry EDPClocks_5xx[] =
  {
      {"disp_cc_mdss_edp_aux_clk",       0, NULL, 0x0},
      {"disp_cc_mdss_edp_pixel_clk",     0, NULL, 0x0},
      {"disp_cc_mdss_edp_link_clk",      0, NULL, 0x0},
      {"disp_cc_mdss_edp_link_intf_clk", 0, NULL, 0x0},
      {"disp_cc_mdss_edp_gtc_clk",       0, NULL, 0x0},
      {"\0", 0, NULL}
  };

  /*
   * DP0 controller is hard mapped to Standalone DP Phy
   * PLL sources are dptx2_phy_pll_link_clk (index:5) &
   * dptx2_phy_pll_vco_div_clk (index:6)
   * #TODO#IMPLEMENT run-time mapping
   */
  MDPExternalClockEntry            sDP0ExtClocks_5xx[] = 
  {
    {"disp_cc_mdss_dp_pixel_clk",     0, 0, 0, 0, 0, 0},    // Index 0: Pixel clock
    {"disp_cc_mdss_dp_link_clk",      0, 0, 0, 0, 0, 0},    // Index 1: Link clock
    {"disp_cc_mdss_dp_crypto_clk",    0, 0, 0, 0, 0, 0},    // Index 2: Crypto clock
    {"disp_cc_mdss_dp_aux_clk",       0, 0, 0, 0, 0, 0},    // Index 3: Aux clock
    {"disp_cc_mdss_dp_link_intf_clk", 0, 0, 0, 0, 0, 0},    // Index 4: Link intf clock
    {"\0",                            0, 0, 0, 0, 0, 0}
  };

  /*
   * DP1 controller is hard mapped to Type-C USB 0 PHY
   * PLL sources are dptx1_phy_pll_link_clk (index:3) &
   * dptx1_phy_pll_vco_div_clk (index:4)
   * #TODO#IMPLEMENT run-time mapping
   */
  MDPExternalClockEntry            sDP1ExtClocks_5xx[] = 
  {
    {"disp_cc_mdss_dp_pixel2_clk",     0, 0, 0, 0, 0, 0},
    {"disp_cc_mdss_dp_link1_clk",      0, 0, 0, 0, 0, 0},
    {"disp_cc_mdss_dp_crypto1_clk",    0, 0, 0, 0, 0, 0},
    {"disp_cc_mdss_dp_aux1_clk",       0, 0, 0, 0, 0, 0},
    {"disp_cc_mdss_dp_link1_intf_clk", 0, 0, 0, 0, 0, 0},
    {"\0",                             0, 0, 0, 0, 0, 0}
  };

  MDPExternalClockEntry            sEDPExtClocks_5xx[] = 
  {
    {"disp_cc_mdss_edp_pixel_clk",     2, 0, 0, 0, 0, 0},
    {"disp_cc_mdss_edp_link_clk",      1, 0, 0, 0, 0, 0},
    {"disp_cc_mdss_edp_gtc_clk",       0, 0, 0, 0, 0, 0},
    {"disp_cc_mdss_edp_aux_clk",       0, 0, 0, 0, 0, 0},
    {"disp_cc_mdss_edp_link_intf_clk", 1, 0, 0, 0, 0, 0},
    {"\0",                             0, 0, 0, 0, 0, 0}
  };

  /* Disp CC XO clock
  */
  const MDPClockEntry DispCcXoClk_5xx[] =
  {
      {"gcc_disp_ahb_clk",      0, NULL, MDP_CLOCKFLAG_SHARED},
      {"disp_cc_xo_clk",        0, NULL, MDP_CLOCKFLAG_SHARED},
      {"\0", 0, NULL}
  };
  
  /* MDSS power domain list
   */
  const MDPPowerDomainEntry MDSS5xxPowerDomain[] = 
  {
      {"mdss_core_gdsc",        0x0},
      {"\0",                    0x0}
  };
  
  /* MDSS resource list for MDSS 5.0.0
  */
  const MDP_ResourceList sMDP50xResources =
  {
      (MDPPowerDomainEntry*)  &MDSS5xxPowerDomain,   /* Power domain    */
      (MDPClockEntry*)        &DispCcXoClk_5xx,      /* DISP_CC clocks  */
      (MDPClockEntry*)        &MDSSClocks_5xx,       /* MDP clocks      */
      (MDPClockEntry*)        &DSI0Clocks_5xx,       /* DSI clocks      */
      (MDPClockEntry*)        &DSI1Clocks_5xx,       /* DSI clocks      */
      (MDPClockEntry*)        &DP0Clocks_5xx,        /* DP0 clocks      */
  };
 

 /* MDSS resource list for MDSS 5.1.0
 */
 const MDP_ResourceList sMDP51xResources =
 {
     (MDPPowerDomainEntry*)  &MDSS5xxPowerDomain,   /* Power domain    */
     (MDPClockEntry*)        &DispCcXoClk_5xx,      /* DISP_CC clocks  */
     (MDPClockEntry*)        &MDSSClocks_5xx,       /* MDP clocks      */
     (MDPClockEntry*)        &DSI0Clocks_5xx,       /* DSI clocks      */
     (MDPClockEntry*)        &DSI1Clocks_5xx,       /* DSI clocks      */
     (MDPClockEntry*)        &DP0Clocks_5xx,        /* DP0 clocks      */
     (MDPClockEntry*)        &DP1Clocks_5xx,        /* DP1 clocks      */
     (MDPClockEntry*)        &EDPClocks_5xx,        /* EDP clocks      */
 };
 
  
  /* MDP External resource list for MDSS 50x
  */
  const MDP_ExtClockResourceList sMDP50xExtClockResources =
  {
      (MDPExternalClockEntry*)    &sDSI0ExtClocks_5xx,               /* DSI0 Ext clocks      */
      (MDPExternalClockEntry*)    &sDSI1ExtClocks_5xx,               /* DSI1 Ext clocks      */
      (MDPExternalClockEntry*)    &sDSI1SharedSourceExtClocks_5xx,   /* DSI shared clocks for dual DSI */
      (MDPExternalClockEntry*)    &sDSI0ExtClocks_5xx,               /* DSI0 Ext clocks      */
  };
 
 
 /* MDP External resource list for MDSS 51x
 */
 const MDP_ExtClockResourceList sMDP51xExtClockResources =
 {
     (MDPExternalClockEntry*)    &sDSI0ExtClocks_5xx,               /* DSI0 Ext clocks      */
     (MDPExternalClockEntry*)    &sDSI1ExtClocks_5xx,               /* DSI1 Ext clocks      */
     (MDPExternalClockEntry*)    &sDSI1SharedSourceExtClocks_5xx,   /* DSI shared clocks for dual DSI */
     (MDPExternalClockEntry*)    &sDP0ExtClocks_5xx,                /* DP0 Ext clocks      */
     (MDPExternalClockEntry*)    &sDP1ExtClocks_5xx,                /* DP1 Ext clocks      */
     (MDPExternalClockEntry*)    &sEDPExtClocks_5xx,                /* EDP Ext clocks      */
 };
 

 /* Display resource list - MDSS 5.0.0
 */
 const DisplayResourceList sDisplayMDP50xResources =
 {
    (MDP_ResourceList*)            &sMDP50xResources,               /* MDP Resources          */
    (MDP_ExtClockResourceList*)    &sMDP50xExtClockResources,       /* MDP Ext Resources      */
 };

 /* Display resource list - MDSS 5.1.0
 */
 const DisplayResourceList sDisplayMDP51xResources =
 {
    (MDP_ResourceList*)            &sMDP51xResources,               /* MDP Resources          */
    (MDP_ExtClockResourceList*)    &sMDP51xExtClockResources,       /* MDP Ext Resources      */
 };

 /*=========================================================================
       Top level Tables
 ==========================================================================*/

/* Reesource list for MDSS 5xx
*/
const MDP_HwMinorFamilyResourceList asHarwareFamilyMinor5xx[RES_MAX_FAMILY_MINOR] = {
    {MDSS_DEVICE_VERSION_05_00, (DisplayResourceList*)&sDisplayMDP50xResources},     /* MDSS Family 5.0.x, Hana */
    {MDSS_DEVICE_VERSION_05_01, (DisplayResourceList*)&sDisplayMDP51xResources},     /* MDSS Family 5.1.x, Poipu */
};



/* MDP resource list  based on MDP major version.
*/
const MDP_HwMajorFamilyResourceList asHarwareFamilyMajor[RES_MAX_FAMILY_MAJOR] = {
    {NULL},                                                     /* MDSS Family 0xx   */
    {NULL},                                                     /* MDSS Fmaily 1xx (Badger Family)       */
    {NULL},                                                     /* MDSS Family 2xx   */ 
    {NULL},                                                     /* MDSS Family 3xx   */ 
    {NULL},                                                     /* MDSS Fmaily 4xx (Napali Family)  */
    {(MDP_HwMinorFamilyResourceList*)&asHarwareFamilyMinor5xx}, /* MDSS Fmaily 5xx (Hana/Poipu Family)  */
}; 


#endif // __MDSSPlatform_5xx_H__

