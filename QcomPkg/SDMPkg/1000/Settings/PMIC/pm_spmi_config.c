/*! \file
*  \n
*  \brief  pm_spmi_config.c
*  \n
*  \n This file contains pmic configuration data specific for SPMI Controller's
      Peripheral for SDM855 device..
*  \n
*  \n &copy; Copyright 2015-2020 QUALCOMM Technologies, Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/03/20    ms     Updated ownership for PMICC GPIO7
03/22/18   aab     Updated to support PMK8002
02/09/18   aab     Created to support SDM1000
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
  /* PMC1000_#1(Wailua) */
  /* SMB2351#1 */
  /* PM855L(Makua) */
  /* PMK8002(Timex) */
  /* PMC1000_#2(Wailua) for GPU */
  /* SMB2351#2 */
  /* SMB2351#3 */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_TZ},   /* BUS	PBUS Logger */
  {0, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_TZ},   /* PON	 */
  {0, 0x60, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* RTC_RW	 */
  {0, 0x70, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PBS_CORE	 */
  {0, 0xB0, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* SDAM1	Shared Info TZ:  TDOS:Table 3-5 RAM size for SDAMM on PM854 */
  {2, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_TZ},   /* BUS	intbus_arb_logger_dig */
  {2, 0x0A, SPMI_OWNER_APPS,  SPMI_OWNER_TZ},   /* PON	smb2351_pon */
  {2, 0x70, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PBS_CORE	pbs_core */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_TZ},   /* BUS	 */
  {4, 0x08, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PON	 */
  {4, 0x70, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PBS_CORE	 */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_TZ},   /* BUS	 */
  {8, 0x08, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PON	 */
  {8, 0x70, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PBS_CORE	ATEST Diagnostic / Spare */
  {8, 0xB0, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* SDAM1	 */
  {10, 0x04, SPMI_OWNER_AOP,   SPMI_OWNER_TZ},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {10, 0x0A, SPMI_OWNER_APPS,  SPMI_OWNER_TZ},   /* PON	smb2351_pon */
  {10, 0x70, SPMI_OWNER_TZ,    SPMI_OWNER_TZ},   /* PBS_CORE	pbs_core */
  {12, 0x04, SPMI_OWNER_AOP,   SPMI_OWNER_TZ},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {12, 0x0A, SPMI_OWNER_APPS,  SPMI_OWNER_TZ},   /* PON	smb2351_pon */
  {12, 0x70, SPMI_OWNER_TZ,    SPMI_OWNER_TZ},   /* PBS_CORE	pbs_core */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_ADSP},   /* BUS	PBUS Logger */
  {0, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_ADSP},   /* PON	 */
  {0, 0xB1, SPMI_OWNER_APPS,   SPMI_OWNER_ADSP},   /* SDAM2	Shared Info HLOS */
  {2, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_ADSP},   /* BUS	intbus_arb_logger_dig */
  {2, 0x10, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* SCHG_CHGR	schg_2s_chgr: */
  {2, 0x11, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* SCHG_DCDC	schg_2s_dcdc */
  {2, 0x12, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* SCHG_BATIF	schg_2s_batif */
  {2, 0x13, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* SCHG_USB	schg_2s_usb */
  {2, 0x15, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* SCHG_TYPE_C	schg_2s_typec */
  {2, 0x16, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* SCHG_MISC	schg_2s_misc */
  {2, 0x17, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* USB_PD_PHY	usb_pd_phy */
  {2, 0x19, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* SCHG_BOB	schg_2s_bob :  Do we need to control it from the SW? */
  {2, 0x1A, SPMI_OWNER_APPS,   SPMI_OWNER_ADSP},   /* BMD	bmd */
  {2, 0x32, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* ADC_CORE2_MDM	adc_mdm */
  {2, 0x35, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* ADC_CORE5_BTM2	adc_btm */
  {2, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_ADSP},   /* ADC_CORE4_CAL adc_cal */
  {5, 0xB0, SPMI_OWNER_APPS,    SPMI_OWNER_ADSP},   /* LPG_LUT  */
  {5, 0xB1, SPMI_OWNER_APPS,    SPMI_OWNER_ADSP},   /* LPG_CHAN1  */
  {5, 0xB2, SPMI_OWNER_APPS,    SPMI_OWNER_ADSP},   /* LPG_CHAN2  */
  {5, 0xB3, SPMI_OWNER_APPS,    SPMI_OWNER_ADSP},   /* LPG_CHAN3  */
  {5, 0xD0, SPMI_OWNER_APPS,    SPMI_OWNER_ADSP},   /* LED  */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_ADSP},   /* BUS	 */
  {8, 0x32, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* ADC_CORE2_MDM	VADC CALIBRATION */
  {8, 0x38, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* ADC_CORE5_BTM2	 */
  {8, 0xB1, SPMI_OWNER_APPS,   SPMI_OWNER_ADSP},   /* SDAM2	 */
  {8, 0xC2, SPMI_OWNER_APPS,   SPMI_OWNER_ADSP},   /* GPIO03	 */
  {8, 0xC7, SPMI_OWNER_ADSP,   SPMI_OWNER_ADSP},   /* GPIO08	 */
  {10, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_ADSP},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {10, 0x10, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_CHGR	schg_2s_chgr: */
  {10, 0x11, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_DCDC	schg_2s_dcdc */
  {10, 0x12, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_BATIF	schg_2s_batif */
  {10, 0x13, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_USB	schg_2s_usb */
  {10, 0x15, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_TYPE_C	schg_2s_typec */
  {10, 0x16, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_MISC	schg_2s_misc */
  {10, 0x17, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* USB_PD_PHY	usb_pd_phy */
  {10, 0x19, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_BOB	schg_2s_bob */
  {10, 0x1A, SPMI_OWNER_APPS,    SPMI_OWNER_ADSP},   /* BMD	bmd */
  {10, 0x32, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* ADC_CORE2_MDM	adc_mdm */
  {10, 0x35, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* ADC_CORE5_BTM2	adc_btm */
  {10, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_ADSP},   /* ADC_CORE4_CAL adc_cal */
  {12, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_ADSP},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {12, 0x10, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_CHGR	schg_2s_chgr: */
  {12, 0x11, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_DCDC	schg_2s_dcdc */
  {12, 0x12, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_BATIF	schg_2s_batif */
  {12, 0x13, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_USB	schg_2s_usb */
  {12, 0x15, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_TYPE_C	schg_2s_typec */
  {12, 0x16, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_MISC	schg_2s_misc */
  {12, 0x17, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* USB_PD_PHY	usb_pd_phy */
  {12, 0x19, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* SCHG_BOB	schg_2s_bob */
  {12, 0x1A, SPMI_OWNER_APPS,    SPMI_OWNER_ADSP},   /* BMD	bmd */
  {12, 0x32, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* ADC_CORE2_MDM	adc_mdm */
  {12, 0x35, SPMI_OWNER_ADSP,    SPMI_OWNER_ADSP},   /* ADC_CORE5_BTM2	adc_btm */
  {12, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_ADSP},   /* ADC_CORE4_CAL adc_cal */
  {0, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	PBUS Logger */
  {0, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {0, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {0, 0x07, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MASTER	 */
  {0, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_AOP},   /* PON	 */
  {0, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	 */
  {0, 0x0B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREF_MSM	 */
  {0, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* COIN	CHARGER */
  {0, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG1	 */
  {0, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE3_PBS	 */
  {0, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE4_CAL	VADC CALIBRATION */
  {0, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE6_CMN	VADC COMMON SETTINGS */
  {0, 0x3C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VDDGEN	 */
  {0, 0x3D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SPMI	*/
  {0, 0x3E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SP	Secure Proc Rail */
  {0, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* XO	 */
  {0, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK1	 */
  {0, 0x52, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK2	 */
  {0, 0x53, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK3	 */
  {0, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK1	 */
  {0, 0x55, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK2	 */
  {0, 0x56, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK3	 */
  {0, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	 */
  {0, 0x5A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SLEEP_CLK1	 */
  {0, 0x66, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MONITOR	 */
  {0, 0x67, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RCS_MSGR	 */
  {0, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	*/
  {0, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	Used to trigger the sleep sequence */
  {0, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {0, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {0, 0x75, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS3	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {0, 0xB2, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM3	Shared Info AOP */
  {0, 0xB4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM5	BTM 4 channel; ATE PBS CAL  :  Spare:  To be used for testing */
  {0, 0xB5, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM6	BTM 2 channel1: Spare:  To be used for testing */
  {0, 0xB6, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM7	PBS Pattern Generator1: Spare:  To be used for testing */
  {0, 0xB7, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM8	AMOLED SWIRE Decode: PM855A; Spare PM855L: Spare:  To be used for testing */
  {0, 0xC1, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO02	power delivery clock out (back up, 1S only)/ Option 2 backup */
  {0, 0xC3, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO04 PMK8002 (Timex) sleep clock (Backup) or PD Clk back up */
  {0, 0xC4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO05	PMIC Boot Options */
  {0, 0xC6, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO07	Handshake - Single Line Bus (SLB) (back up) */
  {0, 0xC7, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO08	Tie to MSM SP block through voltage divider */
  {0, 0xC9, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO10	Ship mode exit (1S), PMC1000H LCM (2S) */
  {1, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID1	 */
  {1, 0x10, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUCK_CMN_MOD	 */
  {1, 0x14, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_CTRL	 */
  {1, 0x15, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_PS	 */
  {1, 0x16, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_FREQ	 */
  {1, 0x17, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_CTRL	 */
  {1, 0x18, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_PS	 */
  {1, 0x19, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_FREQ	 */
  {1, 0x1A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_CTRL	 */
  {1, 0x1B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_PS	 */
  {1, 0x1C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_FREQ	 */
  {1, 0x1D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_CTRL	 */
  {1, 0x1E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_PS	 */
  {1, 0x1F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_FREQ	 */
  {1, 0x20, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_CTRL	 */
  {1, 0x21, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_PS	 */
  {1, 0x22, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_FREQ	 */
  {1, 0x23, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_CTRL	 */
  {1, 0x24, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_PS	 */
  {1, 0x25, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_FREQ	 */
  {1, 0x26, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_CTRL	 */
  {1, 0x27, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_PS	 */
  {1, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_FREQ	 */
  {1, 0x29, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_CTRL	 */
  {1, 0x2A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_PS	 */
  {1, 0x2B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_FREQ	 */
  {1, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_CTRL	 */
  {1, 0x2D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_PS	 */
  {1, 0x2E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_FREQ	 */
  {1, 0x2F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_CTRL	 */
  {1, 0x30, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_PS	 */
  {1, 0x31, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_FREQ	 */
  {1, 0x40, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO01	 */
  {1, 0x41, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO02	 */
  {1, 0x42, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO03	 */
  {1, 0x43, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO04	 */
  {1, 0x44, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO05	 */
  {1, 0x45, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO06	 */
  {1, 0x46, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO07	 */
  {1, 0x47, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO08	 */
  {1, 0x48, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO09	 */
  {1, 0x49, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO10	 */
  {1, 0x4A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO11	 */
  {1, 0x4B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO12	 */
  {1, 0x4C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO13	 */
  {1, 0x4D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO14	 */
  {1, 0x4E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO15	 */
  {1, 0x4F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO16	 */
  {1, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO17	 */
  {1, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO18	 */
  {2, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	revid_smb2351 */
  {2, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	intbus_arb_logger_dig */
  {2, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	intr_dig */
  {2, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	spmi_p_dig */
  {2, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	misc_smb2351 */
  {2, 0x1E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BCLBIG_PLM bclbig_plm */
  {2, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE3_PBS	adc_pbs */
  {2, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE4_CAL	adc_cal */
  {2, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE6_CMN	adc_cmn */
  {2, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	clk_dist */
  {2, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	pbs_client */
  {2, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	pbs_client */
  {2, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	 */
  {2, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	misc_pbs */
  {4, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	 */
  {4, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {4, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {4, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	 */
  {4, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG1	 */
  {4, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE3_PBS	 */
  {4, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE4_CAL	VADC CALIBRATION */
  {4, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE6_CMN	VADC COMMON SETTINGS */
  {4, 0x3C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VDDGEN	 */
  {4, 0x3E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BCLBIG_PLM  */
  {4, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	 */
  {4, 0x66, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MONITOR	 */
  {4, 0x67, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RCS_MSGR	RCS Messenger (Request Capable Slave)  :   This allows data to be transfered between PMICs using interrupt messages. */
  {4, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	 */
  {4, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	 */
  {4, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {4, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {4, 0x75, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS3	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {4, 0xC6, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},  /*GPIO ADC channel for Compal customer */
  {5, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID1	 */
  {5, 0x10, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUCK_CMN_MOD	 */
  {5, 0x14, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_CTRL	 */
  {5, 0x15, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_PS	 */
  {5, 0x16, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_FREQ	 */
  {5, 0x17, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_CTRL	 */
  {5, 0x18, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_PS	 */
  {5, 0x19, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_FREQ	 */
  {5, 0x1A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_CTRL	 */
  {5, 0x1B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_PS	 */
  {5, 0x1C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_FREQ	 */
  {5, 0x1D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_CTRL	 */
  {5, 0x1E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_PS	 */
  {5, 0x1F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_FREQ	 */
  {5, 0x20, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_CTRL	 */
  {5, 0x21, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_PS	 */
  {5, 0x22, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_FREQ	 */
  {5, 0x23, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_CTRL	 */
  {5, 0x24, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_PS	 */
  {5, 0x25, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_FREQ	 */
  {5, 0x26, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_CTRL	 */
  {5, 0x27, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_PS	 */
  {5, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_FREQ	 */
  {5, 0x29, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_CTRL	 */
  {5, 0x2A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_PS	 */
  {5, 0x2B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_FREQ	 */
  {5, 0x40, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO01	 */
  {5, 0x41, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO02	 */
  {5, 0x42, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO03	 */
  {5, 0x43, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO04	 */
  {5, 0x44, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO05	 */
  {5, 0x45, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO06	 */
  {5, 0x46, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO07	 */
  {5, 0x47, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO08	 */
  {5, 0x48, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO09	 */
  {5, 0x49, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO10	 */
  {5, 0x4A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO11	 */
  {5, 0xA0, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BOB_CONFIG	BOB (Buck-Or-Boost): non-inverting buck-boost converter;  For VIN > VSET, VOUT = VSET V (bucking), else VOUT = VSET (boosting) */
  {6, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {6, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {6, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {6, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	 */
  {6, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG	 */
  {6, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* XO	 */
  {6, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK1	 */
  {6, 0x55, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK2	 */
  {6, 0x56, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK3	 */
  {6, 0x57, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK4	 */
  {6, 0x58, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK	 */
  {6, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	 */
  {6, 0x5A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SLEEP_CLK1	 */
  {7, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_OPTIONS	 */
  {8, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	 */
  {8, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {8, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {8, 0x07, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MASTER	 */
  {8, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	 */
  {8, 0x0B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREF_MSM	 */
  {8, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* COIN	VADC (SINGLE-SHOT) */
  {8, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG1	VADC BTM (THRESHOLDS) */
  {8, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE3_PBS	 */
  {8, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE4_CAL	BCL: Battery current limiting */
  {8, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE6_CMN	 */
  {8, 0x3C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VDDGEN	 */
  {8, 0x3D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SPMI	 */
  {8, 0x3E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SP	 */
  {8, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* XO	RCS Messenger (Request Capable Slave)  :   This allows data to be transfered between PMICs using interrupt messages. */
  {8, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK1	 */
  {8, 0x52, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK2	 */
  {8, 0x53, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK3	 */
  {8, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK1	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {8, 0x55, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK2	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {8, 0x56, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK3	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {8, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	AOSS (Always ON Sleep Indication) :  For internal RCM usage */
  {8, 0x5A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SLEEP_CLK1	 */
  {8, 0x5B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* DIV_CLK1	 */
  {8, 0x5C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* DIV_CLK2	 */
  {8, 0x60, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RTC_RW	 */
  {8, 0x61, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RTC_ALARM	Backlight PWM Mode (external IC for tablet) */
  {8, 0x62, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RTC_TIMER	Handshake - Single Line Bus (SLB) (for LP-DDR4X) for initial bring-ups. (test diagrnosis - PMIC message triggers) */
  {8, 0x66, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MONITOR	ATEST Diagnostic / Spare */
  {8, 0x67, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RCS_MSGR	Wipower Reset from WLAN chip */
  {8, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	Spare */
  {8, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	Spare */
  {8, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	 */
  {8, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	 */
  {8, 0x75, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS3	 */
  {8, 0xB2, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM3	 */
  {8, 0xB4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM5	 */
  {8, 0xB5, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM6	 */
  {8, 0xB6, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM7	 */
  {8, 0xB7, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM8	 */
  {8, 0xC1, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO02	 */
  {8, 0xC4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO05	 */
  {8, 0xC6, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO07	 */
  {9, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID1	 */
  {9, 0x10, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUCK_CMN_MOD	 */
  {9, 0x14, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_CTRL	 */
  {9, 0x15, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_PS	 */
  {9, 0x16, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_FREQ	 */
  {9, 0x17, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_CTRL	 */
  {9, 0x18, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_PS	 */
  {9, 0x19, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_FREQ	 */
  {9, 0x1A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_CTRL	 */
  {9, 0x1B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_PS	 */
  {9, 0x1C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_FREQ	 */
  {9, 0x1D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_CTRL	 */
  {9, 0x1E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_PS	 */
  {9, 0x1F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_FREQ	 */
  {9, 0x20, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_CTRL	 */
  {9, 0x21, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_PS	 */
  {9, 0x22, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_FREQ	BOB (Buck-Or-Boost): non-inverting buck-boost converter;  For VIN > VSET, VOUT = VSET V (bucking), else VOUT = VSET (boosting) */
  {9, 0x23, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_CTRL	Used for thermal use case */
  {9, 0x24, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_PS	 */
  {9, 0x25, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_FREQ	 */
  {9, 0x26, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_CTRL	 */
  {9, 0x27, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_PS	 */
  {9, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_FREQ	 */
  {9, 0x29, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_CTRL	 */
  {9, 0x2A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_PS	 */
  {9, 0x2B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_FREQ	 */
  {9, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_CTRL	 */
  {9, 0x2D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_PS	 */
  {9, 0x2E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_FREQ	 */
  {9, 0x2F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_CTRL	 */
  {9, 0x30, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_PS	 */
  {9, 0x31, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_FREQ	 */
  {9, 0x40, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO01	 */
  {9, 0x41, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO02	 */
  {9, 0x42, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO03	 */
  {9, 0x43, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO04	 */
  {9, 0x44, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO05	 */
  {9, 0x45, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO06	 */
  {9, 0x46, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO07	 */
  {9, 0x47, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO08	 */
  {9, 0x48, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO09	 */
  {9, 0x49, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO10	 */
  {9, 0x4A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO11	 */
  {9, 0x4B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO12	 */
  {9, 0x4C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO13	 */
  {9, 0x4D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO14	 */
  {9, 0x4E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO15	 */
  {9, 0x4F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO16	 */
  {9, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO17	 */
  {9, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO18	 */
  {10, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	revid_smb2351 */
  {10, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {10, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	intr_dig */
  {10, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	spmi_p_dig */
  {10, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	misc_smb2351 */
  {10, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE3_PBS	adc_pbs */
  {10, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE4_CAL	adc_cal */
  {10, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE6_CMN	adc_cmn */
  {10, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	clk_dist */
  {10, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	pbs_client */
  {10, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	pbs_client */
  {10, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	 */
  {10, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	misc_pbs */
  {12, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	revid_smb2351 */
  {12, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {12, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	intr_dig */
  {12, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	spmi_p_dig */
  {12, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	misc_smb2351 */
  {12, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE3_PBS	adc_pbs */
  {12, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE4_CAL	adc_cal */
  {12, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE6_CMN	adc_cmn */
  {12, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	clk_dist */
  {12, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	pbs_client */
  {12, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	pbs_client */
  {12, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	misc_pbs */
  {12, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	misc_pbs */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	PBUS Logger */
  {0, 0x32, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE2_MDM	VADC (SINGLE-SHOT) */
  {0, 0x38, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE5_BTM2	VADC BTM (THRESHOLDS) */
  {0, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* XO	 */
  {0, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* RF_CLK1	 */
  {0, 0xB3, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* SDAM4	Shared Info MPSS */
  {2, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	intbus_arb_logger_dig */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	 */
  {4, 0x32, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE2_MDM	VADC (SINGLE-SHOT) */
  {4, 0x3F, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* BCL_COMP_MDM	 */
  {6, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* XO	 */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	 */
  {8, 0xB3, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* SDAM4	 */
  {10, 0x04, SPMI_OWNER_AOP,   SPMI_OWNER_MSS},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {12, 0x04, SPMI_OWNER_AOP,   SPMI_OWNER_MSS},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	PBUS Logger */
  {0, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* PON	 */
  {0, 0x24, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* TEMP_ALARM	ALARM */
  {0, 0x31, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE1_USR	VADC (SINGLE-SHOT) */
  {0, 0x35, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE5_BTM4	VADC BTM (THRESHOLDS) */
  {0, 0x5B, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK1	 */
  {0, 0x5C, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK2	 */
  {0, 0x61, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* RTC_ALARM	 */
  {0, 0x62, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* RTC_TIMER	 */
  {0, 0xB1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* SDAM2	Shared Info HLOS */
  {0, 0xC0, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO01	Fingerprint Sensor wake up Interrupt */
  {0, 0xC2, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO03	NFC 32kHz SLEEP CLK (external xtal back up) */
  {0, 0xC5, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO06	Volume + Key */
  {0, 0xC8, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO09	USB2 */
  {1, 0x40, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* LDO01	 */
  {1, 0x41, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* LDO02	 */
  {1, 0x49, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* LDO10	 */
  {1, 0x4D, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* LDO14	 */
  {1, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* LDO17	 */
  {2, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	intbus_arb_logger_dig */
  {2, 0x10, SPMI_OWNER_ADSP,   SPMI_OWNER_APPS},   /* SCHG_CHGR	schg_2s_chgr: */
  {2, 0x15, SPMI_OWNER_ADSP,   SPMI_OWNER_APPS},   /* SCHG_TYPE_C	schg_2s_typec */
  {2, 0x19, SPMI_OWNER_ADSP,   SPMI_OWNER_APPS},   /* SCHG_BOB	schg_2s_bob :  Do we need to control it from the SW? */
  {2, 0x1A, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* BMD	bmd */
  {2, 0x1D, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* BCLBIG_COMP	bclbig_comp */
  {2, 0x31, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE1_USR	adc_usr */
  {2, 0x5B, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK1	divclk */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	 */
  {4, 0x24, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* TEMP_ALARM	 */
  {4, 0x31, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE1_USR	VADC (SINGLE-SHOT) */
  {4, 0x35, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE5_BTM4	VADC BTM (THRESHOLDS) */
  {4, 0x3D, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* BCLBIG_COMP	BCL: Battery current limiting */
  {4, 0x5B, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK1	 */
  {4, 0xC0, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO01	AOSS (Always ON Sleep Indication) :  For internal RCM usage */
  {4, 0xC1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO02	 */
  {4, 0xC2, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO03	 */
  {4, 0xC3, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO04	 */
  {4, 0xC4, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO05	 */
  {4, 0xC5, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO06	Backlight PWM Mode (external IC for tablet) */
  {4, 0xC7, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO08	ATEST Diagnostic / Spare */
  {4, 0xC8, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO09	Wipower Reset from WLAN chip */
  {4, 0xC9, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO10	ATEST Diagnostic / Spare */
  {4, 0xCA, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO11	Spare */
  {4, 0xCB, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO12	Spare */
  {5, 0xA1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* BOB_MONITORING	Used for thermal use case */
  {5, 0xB0, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* LPG_LUT	 */
  {5, 0xB1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* LPG_CHAN1	 */
  {5, 0xB2, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* LPG_CHAN2	 */
  {5, 0xB3, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* LPG_CHAN3	 */
  {5, 0xBC, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* PWM1	 */
  {5, 0xBD, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* PWM2	 */
  {5, 0xD0, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* LED	 */
  {5, 0xD3, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* MVFLASH1	 */
  {5, 0xD8, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* WLED1_CTRL	 */
  {5, 0xD9, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* WLED1_SINK	 */
  {5, 0xEC, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* LCDB	 */
  {6, 0x5B, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK1	 */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	 */
  {8, 0x24, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* TEMP_ALARM	VADC (SINGLE-SHOT) */
  {8, 0x31, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE1_USR	 */
  {8, 0x35, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE5_BTM4	VADC COMMON SETTINGS */
  {8, 0xB1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* SDAM2	 */
  {8, 0xC0, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO01	 */
  {8, 0xC2, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO03	 */
  {8, 0xC3, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO04	 */
  {8, 0xC5, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO06	 */
  {8, 0xC8, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO09	 */
  {8, 0xC9, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO10	 */
  {10, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {10, 0x10, SPMI_OWNER_ADSP,    SPMI_OWNER_APPS},   /* SCHG_CHGR	schg_2s_chgr: */
  {10, 0x15, SPMI_OWNER_ADSP,    SPMI_OWNER_APPS},   /* SCHG_TYPE_C	schg_2s_typec */
  {10, 0x19, SPMI_OWNER_ADSP,    SPMI_OWNER_APPS},   /* SCHG_BOB	schg_2s_bob */
  {10, 0x1A, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BMD	bmd */
  {10, 0x1D, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BCLBIG_COMP	bclbig_comp */
  {10, 0x1E, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BCLBIG_PLM	bclbig_plm */
  {10, 0x31, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* ADC_CORE1_USR	adc_usr */
  {10, 0x5B, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* DIV_CLK1	divclk */
  {12, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	intbus_arb_logger_dig:  Pbus Logger */
  {12, 0x10, SPMI_OWNER_ADSP,    SPMI_OWNER_APPS},   /* SCHG_CHGR	schg_2s_chgr: */
  {12, 0x15, SPMI_OWNER_ADSP,    SPMI_OWNER_APPS},   /* SCHG_TYPE_C	schg_2s_typec */
  {12, 0x19, SPMI_OWNER_ADSP,    SPMI_OWNER_APPS},   /* SCHG_BOB	schg_2s_bob */
  {12, 0x1A, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BMD	bmd */
  {12, 0x1D, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BCLBIG_COMP	bclbig_comp */
  {12, 0x1E, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* BCLBIG_PLM	bclbig_plm */
  {12, 0x31, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* ADC_CORE1_USR	adc_usr */
  {12, 0x5B, SPMI_OWNER_APPS,    SPMI_OWNER_APPS},   /* DIV_CLK1	divclk */
};

/* Number of spmi channels config entries. . */
uint32 pm_spmi_channel_cfg_size[] = {sizeof(pm_spmi_channel_cfg) / sizeof(SpmiCfg_ChannelCfg)};

