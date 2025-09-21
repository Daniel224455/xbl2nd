/*! \file
*  \n
*  \brief  pm_spmi_config.c
*  \n
*  \n This file contains pmic configuration data specific for SPMI Controller's
      Peripheral for MSM8996 device..
*  \n
*  \n &copy; Copyright 2015-2017 QUALCOMM Technologies, Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/boot.xf/2.2/QcomPkg/SDM845Pkg/Settings/PMIC/pm_spmi_config.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/02/17   aab     Changed Access Control config for APC0/APC1 and Coincell peripherals to TZ
08/24/17   aab     Update Access Control Parsing script that groups the output by peripheral owner.  so that minimum number of RGs is allocated for TZ and Modem owned peripherals.
03/27/17   aab     Updated it based on reviewed Access control documentt. Generated from reviewed document and done some manual grouping
03/06/17   aab     Updated it to use new structure: SpmiBusCfg_OwnerMask
01/16/17   aab     Created to support SDM845
========================================================================== */

/*-------------------------------------------------------------------------
* Include Files
* ----------------------------------------------------------------------*/

#include "pm_device.h"
#include "SpmiCfg.h"
#include "SpmiBusCfg.h" 

/*-------------------------------------------------------------------------
* Preprocessor Definitions and Constants
* ----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
* Static Variable Definitions
* ----------------------------------------------------------------------*/

/* PMIC MMU/SMMU configuration table. */

SpmiCfg_ChannelCfg pm_spmi_channel_cfg [] =
{
/*SID, Periph ID, IRQ Owner, Periph Owner */ 
  /* PM8998 */
  /* PMI8998 */
  /* PM8005 */
  {0, 0x04, SPMI_OWNER_AOP,   SPMI_OWNER_TZ},   /* BUS	 */
  {0, 0x08, SPMI_OWNER_APPS,  SPMI_OWNER_TZ},   /* PON	 */
  {0, 0x60, SPMI_OWNER_TZ,    SPMI_OWNER_TZ},   /* RTC_RW	 */
  {0, 0x70, SPMI_OWNER_TZ,    SPMI_OWNER_TZ},   /* PBS_CORE	 */
  {1, 0x35, SPMI_OWNER_TZ,    SPMI_OWNER_TZ},   /* S12_CTRL	VDD_APC1 */
  {1, 0x38, SPMI_OWNER_TZ,    SPMI_OWNER_TZ},   /* S13_CTRL	VDD_APC0 */
  {2, 0x04, SPMI_OWNER_AOP,   SPMI_OWNER_TZ},   /* BUS	 */
  {2, 0x08, SPMI_OWNER_APPS,  SPMI_OWNER_TZ},   /* PON	 */
  {2, 0x70, SPMI_OWNER_TZ,    SPMI_OWNER_TZ},   /* PBS_CORE	 */
  {4, 0x04, SPMI_OWNER_AOP,   SPMI_OWNER_TZ},   /* BUS	 */
  {4, 0x08, SPMI_OWNER_APPS,  SPMI_OWNER_TZ},   /* PON	 */
  {4, 0x70, SPMI_OWNER_TZ,    SPMI_OWNER_TZ},   /* PBS_CORE	 */
  {0, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	 */
  {0, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {0, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {0, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_AOP},   /* PON	 */
  {0, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	 */
  {0, 0x0B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREF_MSM	 */
  {0, 0x0C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREF_WWAN_DAC	 */
  {0, 0x1C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUA_EXT_CHARGER	BATT_ALARM */
  {0, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG1	 */
  {0, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VADC_HC4_CAL	VADC CALIBRATION */
  {0, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VADC_HM10_CMN	VADC COMMON SETTINGS */
  {0, 0x3C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VDDGEN	 */
  {0, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* XO	 */
  {0, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK1	 */
  {0, 0x52, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK2	 */
  {0, 0x53, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK3	 */
  {0, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK1	 */
  {0, 0x55, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK2	 */
  {0, 0x56, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK3	 */
  {0, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	 */
  {0, 0x5A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SLEEP_CLK1	 */
  {0, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	 */
  {0, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	 */
  {0, 0xCE, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO15	Ext Buck Option for LPDDR4X VDD2: Clock for Naper external PA. */
  {0, 0xD3, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO20	Wipower (Stark) - Reset (from PMI8998) and EUD (embedded USB debug) */
  {0, 0xD5, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO22	PMIC Boot Options */
  {0, 0xD7, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO24	Tie to MSM SP block through votlage divider */
  {0, 0xD8, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO25	PMIC Boot Options */
  {0, 0xD9, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO26	Handshake - Single Line Bus (SLB) (for LP-DDR4X, otherwise 0-ohm PU to VPH) */
  {1, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID1	 */
  {1, 0x10, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUCK_CMN_MOD	 */
  {1, 0x14, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_CTRL	VDD_Cx (S1A_4A) */
  {1, 0x15, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_PS	 */
  {1, 0x16, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_FREQ	 */
  {1, 0x17, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_CTRL	LPDDR4X Memory */
  {1, 0x18, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_PS	 */
  {1, 0x19, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_FREQ	 */
  {1, 0x1A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_CTRL	 */
  {1, 0x1B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_PS	 */
  {1, 0x1C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_FREQ	 */
  {1, 0x1D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_CTRL	VDD_PX3 */
  {1, 0x1E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_PS	 */
  {1, 0x1F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_FREQ	 */
  {1, 0x20, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_CTRL	 */
  {1, 0x21, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_PS	 */
  {1, 0x22, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_FREQ	 */
  {1, 0x23, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_CTRL	VDDMX_1 */
  {1, 0x24, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_PS	 */
  {1, 0x25, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_FREQ	 */
  {1, 0x26, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_CTRL	 */
  {1, 0x27, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_PS	 */
  {1, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_FREQ	 */
  {1, 0x29, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_CTRL	 */
  {1, 0x2A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_PS	 */
  {1, 0x2B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_FREQ	 */
  {1, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_CTRL	VDDCX_1 */
  {1, 0x2D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_PS	 */
  {1, 0x2E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_FREQ	 */
  {1, 0x2F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_CTRL	 */
  {1, 0x30, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_PS	 */
  {1, 0x31, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_FREQ	 */
  {1, 0x32, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S11_CTRL	 */
  {1, 0x33, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S11_PS	 */
  {1, 0x34, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S11_FREQ	 */
  {1, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S12_PS	 */
  {1, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S12_FREQ	 */
  {1, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S13_PS	 */
  {1, 0x3A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S13_FREQ	 */
  {1, 0x40, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO01	 */
  {1, 0x41, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO02	 */
  {1, 0x42, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO03	 */
  {1, 0x43, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO04	VDD_LPI_MX */
  {1, 0x44, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO05	VDD_WCSS_MX */
  {1, 0x45, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO06	 */
  {1, 0x46, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO07	 */
  {1, 0x47, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO08	 */
  {1, 0x48, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO09	VDD_UIM2; VDDPX_5 */
  {1, 0x49, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO10	VDD_UIM2; VDDPX_6 */
  {1, 0x4A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO11	 */
  {1, 0x4B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO12	 */
  {1, 0x4C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO13	VDDPX_2 */
  {1, 0x4D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO14	 */
  {1, 0x4E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO15	 */
  {1, 0x4F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO16	 */
  {1, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO17	 */
  {1, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO18	 */
  {1, 0x52, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO19	 */
  {1, 0x53, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO20	 */
  {1, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO21	MEM_SD_MMC_VDD */
  {1, 0x55, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO22	 */
  {1, 0x56, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO23	WCN3990_3P3_CHAIN1 */
  {1, 0x57, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO24	VDDA_QUSB_HS0_3P1 */
  {1, 0x58, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO25	WCN3990_3P3_CHAIN0 */
  {1, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO26	 */
  {1, 0x5A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO27	VDD_SSC_CX */
  {1, 0x5B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO28	 */
  {1, 0x5C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREF_SP	 */
  {1, 0x80, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LVS1	 */
  {1, 0x81, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LVS2	 */
  {14, 0x05, SPMI_OWNER_AOP,   SPMI_OWNER_AOP},   /* INT	Used for broadcast write access */
  {14, 0x06, SPMI_OWNER_AOP,   SPMI_OWNER_AOP},   /* SPMI	Used for broadcast write access */
  {14, 0x72, SPMI_OWNER_AOP,   SPMI_OWNER_AOP},   /* PBS_CLIENT1	Used for broadcast write access */
  {2, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {2, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	 */
  {2, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {2, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {2, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_AOP},   /* PON	 */
  {2, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	Need Discussion */
  {2, 0x1C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUA	 */
  {2, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG1	 */
  {2, 0x3C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VDDGEN	 */
  {2, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	 */
  {2, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	 */
  {2, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	 */
  {2, 0xC3, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO04	Boot Option: uUSB (GND)/ Type C (High Z) Connector Type Selector */
  {2, 0xC6, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO07	Boot Option: LCD (HighZ)/AMOLED(GND) */
  {2, 0xCC, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO13	Boot Option: SPMI (high Z)/I2C (GND) Select Pin */
  {3, 0xA0, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BOB_CONFIG	 */
  {4, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	 */
  {4, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {4, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {4, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_AOP},   /* PON	 */
  {4, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	 */
  {4, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG1	 */
  {4, 0x3C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VDDGEN	 */
  {4, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	 */
  {4, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	 */
  {4, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	 */
  {4, 0xC2, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO03	Handshake - Single Line Bus (SLB) (for LP-DDR4X, otherwise spare) */
  {4, 0xC3, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO04	Boot Option: LPDDR4X (VPH_PWR)/LP-DDR4 (GND) */
  {5, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID1	 */
  {5, 0x10, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUCK_CMN_MOD	 */
  {5, 0x14, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_CTRL	 */
  {5, 0x15, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_PS	 */
  {5, 0x16, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_FREQ	 */
  {5, 0x17, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_CTRL	 */
  {5, 0x18, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_PS	 */
  {5, 0x19, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_FREQ	 */
  {5, 0x1A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_CTRL	 */
  {5, 0x1B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_PS	 */
  {5, 0x1C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_FREQ	 */
  {5, 0x1D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_CTRL	 */
  {5, 0x1E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_PS	 */
  {5, 0x1F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_FREQ	 */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	 */
  {0, 0x32, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* VADC_HC2_MDM	VADC (SINGLE-SHOT) */
  {0, 0x38, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* VADC_HC9_BTM_2	VADC BTM (THRESHOLDS) */
  {0, 0x3F, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* BCL_COMP_MDM	 */
  {0, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* XO	 */
  {0, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* RF_CLK1	 */
  {0, 0x55, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* RF_CLK2	 */
  {0, 0x56, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* RF_CLK3	 */
  {0, 0x74, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* PBS_CLIENT3	 */
  {0, 0xC2, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* GPIO03	Cherokee - 38.4MHz Clock/PWR Enable (from Cherokee):  If assignment changed, PBS trigger would need to be updated */
  {2, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	 */
  {2, 0x73, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* PBS_CLIENT2	 */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	 */
  {0, 0x04, SPMI_OWNER_AOP,     SPMI_OWNER_APPS},   /* BUS	 */
  {0, 0x08, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* PON	 */
  {0, 0x24, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* TEMP_ALARM	ALARM */
  {0, 0x28, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* COIN	CHARGER */
  {0, 0x31, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* VADC_HC1_USR	VADC (SINGLE-SHOT) */
  {0, 0x34, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* VADC_HC7_BTM8	VADC BTM (THRESHOLDS) */
  {0, 0x35, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* VADC_HC5_VBAT	VADC VBATT MIN */
  {0, 0x3D, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BCL_COMP	 */
  {0, 0x3E, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BCL_PLM	 */
  {0, 0x5B, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* DIV_CLK1	 */
  {0, 0x5C, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* DIV_CLK2	 */
  {0, 0x5D, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* DIV_CLK3	 */
  {0, 0x61, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* RTC_ALARM	 */
  {0, 0x62, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* RTC_TIMER	 */
  {0, 0x73, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* PBS_CLIENT2	 */
  {0, 0xC0, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO01	PMI8998 - Battery/UICC Removal (backup) */
  {0, 0xC1, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO02	PM845 RCM Marker */
  {0, 0xC3, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO04	 */
  {0, 0xC4, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO05	Home Key */
  {0, 0xC5, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO06	Volume + Key */
  {0, 0xC6, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO07	Snapshot Key */
  {0, 0xC7, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO08	Focus Key */
  {0, 0xC8, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO09	Camera Front DVDD LDO Enable */
  {0, 0xC9, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO10	 */
  {0, 0xCA, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO11	NFC - UIM2 Power Control (required only for dual SIM NFC) */
  {0, 0xCB, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO12	Camera Rear DVDD LDO Enable */
  {0, 0xCC, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO13	 */
  {0, 0xCD, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO14	WiPower (Stark) - 32kHz OWI CLK */
  {0, 0xCF, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO16	Ext LDO Option for Napier PA */
  {0, 0xD0, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO17	SMB1380 System OK */
  {0, 0xD1, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO18	SMB1380 Status */
  {0, 0xD2, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO19	WiGig - 38.4MHz Clock Enable (11AD) */
  {0, 0xD4, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO21	NFC - 19.2MHz Clock Enable */
  {0, 0xD6, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO23	WCSS - Power Request (from MSM) */
  {14, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* PON	Used for broadcast write access */
  {2, 0x04, SPMI_OWNER_AOP,     SPMI_OWNER_APPS},   /* BUS	 */
  {2, 0x08, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* PON	 */
  {2, 0x10, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* SCHG8998_CHGR	 */
  {2, 0x11, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* SCHG8998_OTG	 */
  {2, 0x12, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* SCHG8998_BATIF	 */
  {2, 0x13, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* SCHG8998_USB	 */
  {2, 0x14, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* SCHG8998_DC	 */
  {2, 0x15, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* QNOVO	 */
  {2, 0x16, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* SCHG8998_MISC	 */
  {2, 0x17, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* USB_PD_PHY	 */
  {2, 0x18, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* SCHG8998_OTP_HDR	 */
  {2, 0x1B, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BSI	 */
  {2, 0x24, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* TEMP_ALARM	 */
  {2, 0x40, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_BATT_SOC	 */
  {2, 0x41, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_BATT_INFO	 */
  {2, 0x42, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_BCL	 */
  {2, 0x43, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_LMH	 */
  {2, 0x44, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_MEM_IF	 */
  {2, 0x45, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_ADC_RR	 */
  {2, 0x46, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_FT_HDR	 */
  {2, 0x48, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_DMA0	 */
  {2, 0x49, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_DMA1	 */
  {2, 0x4A, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_DMA2	 */
  {2, 0x4B, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* FG_DMA3	 */
  {2, 0x74, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* PBS_CLIENT3	 */
  {2, 0xC0, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO01	AMOLED: WLED Discharge, LCD: WLED Short Circuit Protection */
  {2, 0xC1, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO02	use for 2nd usb port */
  {2, 0xC2, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO03	Camera Rear 3rd LED Enable */
  {2, 0xC4, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO05	Backlight PWM Mode (external IC for tablet) */
  {2, 0xC5, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO06	QNOVO External FET Control */
  {2, 0xC7, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO08	use for 2nd usb port */
  {2, 0xC8, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO09	ID for second USB port */
  {2, 0xC9, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO10	5V Boost + OVP Enable */
  {2, 0xCA, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO11	Wipower (Stark) - Control external FET for WiPwr LBET */
  {2, 0xCB, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO12	19.2MHz Clock Input - from PM845 */
  {2, 0xCD, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO14	I2C Interrupt (connects to MSM) */
  {3, 0xA1, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BOB_MONITORING	 */
  {3, 0xB0, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* LPG_LUT	 */
  {3, 0xB1, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* LPG_CHAN1	 */
  {3, 0xB2, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* LPG_CHAN2	 */
  {3, 0xB3, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* LPG_CHAN3	 */
  {3, 0xB4, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* LPG_CHAN4	 */
  {3, 0xB5, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* LPG_CHAN5	 */
  {3, 0xB6, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* LPG_CHAN6	 */
  {3, 0xC0, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* HAPTICS	 */
  {3, 0xD0, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* TRI_LED	 */
  {3, 0xD3, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* MVFLASH1	 */
  {3, 0xD8, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* WLED1_CTRL	 */
  {3, 0xD9, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* WLED1_SINK	 */
  {3, 0xDC, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* IBB	 */
  {3, 0xDE, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* LAB	 */
  {4, 0x04, SPMI_OWNER_AOP,     SPMI_OWNER_APPS},   /* BUS	 */
  {4, 0x08, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* PON	 */
  {4, 0x24, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* TEMP_ALARM	 */
  {4, 0xC0, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO01	 */
  {4, 0xC1, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* GPIO02	PM8005 RCM Marker */
};


/* Number of spmi channels config entries. . */
uint32 pm_spmi_channel_cfg_size[] = {sizeof(pm_spmi_channel_cfg) / sizeof(SpmiCfg_ChannelCfg)};

