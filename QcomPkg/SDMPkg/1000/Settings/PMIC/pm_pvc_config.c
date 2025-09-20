/*! \file
*  \n
*  \brief  pm_pvc_config.c
*  \n
*  \n This file contains pmic configuration data specific for SPMI Controller's
      Peripheral for SDM845 device..
*  \n
*  \n &copy; Copyright 2015-2018 Qualcomm Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who    what, where, why
--------   ---    ----------------------------------------------------------
031/22/18  aab    Updated MGPI2 configuration
01/15/18   aab    Updated Poipu target PVC configuration
12/19/17   aab    Updated MGPI configuration for SDM855 Target 
02/01/17   aab    Supported SDM845 target.  Added SPMI,PVC, MGPI and VIOCTL configurations
08/25/15   vtw    Created.
========================================================================== */

/*-------------------------------------------------------------------------
* Include Files
* ----------------------------------------------------------------------*/

#include "SpmiCfg.h"

/*-------------------------------------------------------------------------
* Preprocessor Definitions and Constants
* ----------------------------------------------------------------------*/

/* Converts a PVC port index to an overall PMIC Arbiter port index */
#define PVC_PORT_TO_PMIC_ARB_PORT(p) (p+1)

/*   Port assignment in 8998   */
#define APPS0_PORT      1   /* APPS Port 0  */
#define APPS1_PORT      2   /* APPS Port 1  */
#define MGPI_PVC_PORT   6   /* UICC MGPI port 6  */

/*-------------------------------------------------------------------------
* Static Variable Definitions
* ----------------------------------------------------------------------*/

/*
 * pm_pvc_apps0_addr
 *
 * PVC port addr for APPS0 Gold cluster.
 */
SpmiCfg_Ppid pm_pvc_apps0_addr[] =
{
  {0x1, 0x1A40}, /* PM855, S3A_CTRL_VSET_LB */
  {0x1, 0x1A41}, /* PM855, S3A_CTRL_VSET_UB */
  {0x1, 0x1A45}, /* PM855, S3A_CTRL_MODE_CTL */
  {0x1, 0x1A46}  /* PM855, S3A_CTRL_EN_CTL */
};

const SpmiCfg_PvcPortCfg pm_pvc_apps0_cfg = {APPS0_PORT, SPMI_ACCESS_PRIORITY_LOW, pm_pvc_apps0_addr, sizeof(pm_pvc_apps0_addr)/sizeof(SpmiCfg_Ppid)};

/*
 * pm_pvc_apps1_addr
 *
 * PVC port addr for APPS1 Silver cluster block.
 */
SpmiCfg_Ppid pm_pvc_apps1_addr[] =
{
  {0x9, 0x2F40}, /* PM855, S10E_CTRL_VSET_LB */
  {0x9, 0x2F41}, /* PM855, S10E_CTRL_VSET_UB */
  {0x9, 0x2F45}, /* PM855, S10E_CTRL_MODE_CTL */
  {0x9, 0x2F46}  /* PM855, S10E_CTRL_EN_CTL */
};

const SpmiCfg_PvcPortCfg pm_pvc_apps1_cfg = {APPS1_PORT, SPMI_ACCESS_PRIORITY_LOW, pm_pvc_apps1_addr, sizeof(pm_pvc_apps1_addr)/sizeof(SpmiCfg_Ppid)};

/*
 * pm_pvc_mgpi_addr
 *
 * PVC port addr for UICC MGPI block.
 */
SpmiCfg_Ppid pm_pvc_mgpi_addr[] =
{
  {0x9, 0x4146}, /* MGPI0 , L2E_EN_CTL UICC1 */
  {0x9, 0x4C46}, /* MGPI1 , L13E_EN_CTL UICC2 */
  {0x0, 0x5349}, /* MGPI2 , NFC, LNBBCLK3_PC */
  {0x0, 0x096A}, /* MGPI3 , WLAN/BT(WLAN_SW_EN_CTRL), PBS Seq */
  {0x9, 0x5046}, /* MGPI4 , SD card removal detection */
  {0x6, 0x096B}, /* MGPI5 , WiGig, RFCLK3_PC */
  {0x0, 0x0969}, /* MGPI6 , Wcss_pwr_req, RFCLK2 PC and PBS Seq */
  {0x0, 0x0000}  /* MGPI7 Not used */  
};

const SpmiCfg_PvcPortCfg pm_pvc_uicc_mgpi_cfg = {MGPI_PVC_PORT, SPMI_ACCESS_PRIORITY_LOW, pm_pvc_mgpi_addr, sizeof(pm_pvc_mgpi_addr)/sizeof(SpmiCfg_Ppid)};

/*
 * pm_arb_pvc_cfg
 *
 * PMIC Arbiter PVC ports config.
 */
const SpmiCfg_PvcPortCfg* pm_arb_pvc_port_cfg[] = {&pm_pvc_apps0_cfg, &pm_pvc_apps1_cfg, &pm_pvc_uicc_mgpi_cfg};

uint32 pm_arb_pvc_port_cfg_size[] = {sizeof(pm_arb_pvc_port_cfg) / sizeof(SpmiCfg_PvcPortCfg*)};



/*
 * pm_mgpi_port_cfg
 *
 * PMIC MGPI ports config.
 */
const SpmiBusCfg_MgpiPortCfg pm_mgpi_port_0_cfg = 
{
  0,                 /* MGPI Port id */
  MGPI_PVC_PORT,     /* PVC Port id */
  FALSE,             /* Positive edge enable */
  0,                 /* Positive edge PVC port address index */
  0,                 /* Positive edge data */
  TRUE,              /* Negative edge enable */
  0,                 /* Negative edge PVC port address index */
  0,                 /* Negative edge data */
  FALSE,             /* MUX Sel Valid Flag*/
  0,                 /* MUX Sel Value */
};

const SpmiBusCfg_MgpiPortCfg pm_mgpi_port_1_cfg = 
{
  1,                /* MGPI Port id */
  MGPI_PVC_PORT,    /* PVC Port id */
  FALSE,            /* Positive edge enable */
  0,                /* Positive edge PVC port address index */
  0,                /* Positive edge data */
  TRUE,             /* Negative edge enable */
  1,                /* Negative edge PVC port address index */
  0,                /* Negative edge data */
  TRUE,             /* MUX Sel Valid Flag*/
  0                 /* MUX Sel Value */
};

const SpmiBusCfg_MgpiPortCfg pm_mgpi_port_2_cfg = 
{
  2,                 /* MGPI Port id */
  MGPI_PVC_PORT,     /* PVC Port id */
  TRUE,              /* Positive edge enable */
  2,                 /* Positive edge PVC port address index */
  0xC0,              /* Positive edge data */
  TRUE,              /* Negative edge enable */
  2,                 /* Negative edge PVC port address index */
  0x0,               /* Negative edge data */
  TRUE,              /* MUX Sel Valid Flag*/
  5                  /* MUX Sel Value */
};

const SpmiBusCfg_MgpiPortCfg pm_mgpi_port_3_cfg = 
{
  3,                 /* MGPI Port id */
  MGPI_PVC_PORT,     /* PVC Port id */
  TRUE,              /* Positive edge enable */
  3,                 /* Positive edge PVC port address index */
  1,                 /* Positive edge data */
  TRUE,              /* Negative edge enable */
  3,                 /* Negative edge PVC port address index */
  0,                 /* Negative edge data */
  TRUE,              /* MUX Sel Valid Flag*/
  1                  /* MUX Sel Value */
};

const SpmiBusCfg_MgpiPortCfg pm_mgpi_port_4_cfg = 
{
  4,                 /* MGPI Port id */
  MGPI_PVC_PORT,     /* PVC Port id */
  FALSE,              /* Positive edge enable */
  4,                 /* Positive edge PVC port address index */
  0,                 /* Positive edge data */
  FALSE,              /* Negative edge enable */
  4,                 /* Negative edge PVC port address index */
  0,                 /* Negative edge data */
  FALSE,              /* MUX Sel Valid Flag*/
  4                  /* MUX Sel Value */
};

const SpmiBusCfg_MgpiPortCfg pm_mgpi_port_5_cfg = 
{
  5,                 /* MGPI Port id */
  MGPI_PVC_PORT,     /* PVC Port id */
  TRUE,              /* Positive edge enable */
  5,                 /* Positive edge PVC port address index */
  1,                 /* Positive edge data */
  TRUE,              /* Negative edge enable */
  5,                 /* Negative edge PVC port address index */
  0,                 /* Negative edge data */
  TRUE,              /* MUX Sel Valid Flag*/
  5                  /* MUX Sel Value */
};

const SpmiBusCfg_MgpiPortCfg pm_mgpi_port_6_cfg = 
{
  6,                 /* MGPI Port id */
  MGPI_PVC_PORT,     /* PVC Port id */
  TRUE,              /* Positive edge enable */
  6,                 /* Positive edge PVC port address index */
  1,                 /* Positive edge data */
  TRUE,              /* Negative edge enable */
  6,                 /* Negative edge PVC port address index */
  0,                 /* Negative edge data */
  FALSE,             /* MUX Sel Valid Flag*/
  0                  /* MUX Sel Value */
};
const SpmiBusCfg_MgpiPortCfg* pm_mgpi_port_cfg[] = {&pm_mgpi_port_0_cfg, &pm_mgpi_port_1_cfg, &pm_mgpi_port_2_cfg, &pm_mgpi_port_3_cfg, &pm_mgpi_port_4_cfg, &pm_mgpi_port_5_cfg, &pm_mgpi_port_6_cfg};

uint32 pm_mgpi_port_cfg_size[] = {sizeof(pm_mgpi_port_cfg) / sizeof(SpmiBusCfg_MgpiPortCfg*)};

