/*! \file
*  \n
*  \brief  pm_spmi_config.c
*  \n
*  \n This file contains pmic configuration data specific for SPMI Controller's
      Peripheral for SDM855 AU device..
*  \n
*  \n &copy; Copyright 2015-2019 QUALCOMM Technologies, Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/17/18   aab     Created to support SDM855AU
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
  /* PM855_AU(Wailua #1) */
  /* PM855_AU(Wailua #2) */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_TZ},   /* BUS	 */
  {0, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_TZ},   /* PON	 */
  {0, 0x60, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* RTC_RW	 */
  {0, 0x70, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PBS_CORE	 */
  {0, 0xB0, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* SDAM1	Shared Info TZ:  TDOS:Table 3-5 RAM size for SDAMM on PM854 */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_TZ},   /* BUS	 */
  {4, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_TZ},   /* PON	 */
  {4, 0x60, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* RTC_RW	 */
  {4, 0x70, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PBS_CORE	 */
  {4, 0xB0, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* SDAM1	Shared Info TZ:  TDOS:Table 3-5 RAM size for SDAMM on PM854 */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_TZ},   /* BUS	 */
  {8, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_TZ},   /* PON	 */
  {8, 0x60, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* RTC_RW	 */
  {8, 0x70, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* PBS_CORE	 */
  {8, 0xB0, SPMI_OWNER_TZ,     SPMI_OWNER_TZ},   /* SDAM1	Shared Info TZ:  TDOS:Table 3-5 RAM size for SDAMM on PM854 */
  {0, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	 */
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
  {0, 0x3D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SPMI	 */
  {0, 0x3E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SP	 */
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
  {0, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	 */
  {0, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	 */
  {0, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {0, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {0, 0x75, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS3	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {0, 0xB2, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM3	Shared Info AOP */
  {0, 0xB4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM5	BTM 4 channel; ATE PBS CAL  :  Spare:  To be used for testing */
  {0, 0xB5, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM6	BTM 2 channel1: Spare:  To be used for testing */
  {0, 0xB7, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM8	AMOLED SWIRE Decode: PM855A; Spare PM855L: Spare:  To be used for testing */
  {0, 0xC4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO05	PMIC Boot Options */
  {0, 0xC7, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO08	Battery removal alarm */
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
  {4, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	 */
  {4, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {4, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {4, 0x07, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MASTER	 */
  {4, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_AOP},   /* PON	 */
  {4, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	 */
  {4, 0x0B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREF_MSM	 */
  {4, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* COIN	CHARGER */
  {4, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG1	 */
  {4, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE3_PBS	 */
  {4, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE4_CAL	VADC CALIBRATION */
  {4, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE6_CMN	VADC COMMON SETTINGS */
  {4, 0x3C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VDDGEN	 */
  {4, 0x3D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SPMI	 */
  {4, 0x3E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SP	 */
  {4, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* XO	 */
  {4, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK1	 */
  {4, 0x52, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK2	 */
  {4, 0x53, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK3	 */
  {4, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK1	 */
  {4, 0x55, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK2	 */
  {4, 0x56, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK3	 */
  {4, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	 */
  {4, 0x5A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SLEEP_CLK1	 */
  {4, 0x66, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MONITOR	 */
  {4, 0x67, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RCS_MSGR	 */
  {4, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	 */
  {4, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	 */
  {4, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {4, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {4, 0x75, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS3	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {4, 0xB2, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM3	Shared Info AOP */
  {4, 0xB4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM5	BTM 4 channel; ATE PBS CAL  :  Spare:  To be used for testing */
  {4, 0xB5, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM6	BTM 2 channel1: Spare:  To be used for testing */
  {4, 0xB7, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM8	AMOLED SWIRE Decode: PM855A; Spare PM855L: Spare:  To be used for testing */
  {4, 0xC4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO05	PMIC Boot Options */
  {5, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID1	 */
  {5, 0x10, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUCK_CMN_MOD	 */
  {5, 0x14, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_CTRL	 */
  {5, 0x15, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_PS	 */
  {5, 0x16, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_FREQ	 */
  {5, 0x17, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_CTRL	 */
  {5, 0x18, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_PS	 */
  {5, 0x19, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_FREQ	 */
  {5, 0x1A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_CTRL	 */
  {5, 0x1B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_PS	 */
  {5, 0x1C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S1_FREQ	 */
  {5, 0x1D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_CTRL	 */
  {5, 0x1E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S4_PS	 */
  {5, 0x1F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S2_FREQ	 */
  {5, 0x20, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_CTRL	 */
  {5, 0x21, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S5_PS	 */
  {5, 0x22, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_FREQ	 */
  {5, 0x23, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_CTRL	 */
  {5, 0x24, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_PS	 */
  {5, 0x25, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_FREQ	 */
  {5, 0x26, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_CTRL	 */
  {5, 0x27, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_PS	 */
  {5, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S7_FREQ	 */
  {5, 0x29, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_CTRL	 */
  {5, 0x2A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_PS	 */
  {5, 0x2B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S8_FREQ	 */
  {5, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_CTRL	 */
  {5, 0x2D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_PS	 */
  {5, 0x2E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S9_FREQ	 */
  {5, 0x2F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_CTRL	 */
  {5, 0x30, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_PS	 */
  {5, 0x31, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S10_FREQ	 */
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
  {5, 0x4B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO12	 */
  {5, 0x4C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO13	 */
  {5, 0x4D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO14	 */
  {5, 0x4E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO15	 */
  {5, 0x4F, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO16	 */
  {5, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO17	 */
  {5, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LDO18	 */
  {8, 0x01, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* REVID	 */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* BUS	 */
  {8, 0x05, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* INT	 */
  {8, 0x06, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI	 */
  {8, 0x07, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MASTER	 */
  {8, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_AOP},   /* PON	 */
  {8, 0x09, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC	 */
  {8, 0x0B, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREF_MSM	 */
  {8, 0x28, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* COIN	CHARGER */
  {8, 0x2C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MBG1	 */
  {8, 0x36, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE3_PBS	 */
  {8, 0x37, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE4_CAL	VADC CALIBRATION */
  {8, 0x39, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* ADC_CORE6_CMN	VADC COMMON SETTINGS */
  {8, 0x3C, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VDDGEN	 */
  {8, 0x3D, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SPMI	 */
  {8, 0x3E, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* VREG_SP	 */
  {8, 0x58, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* XO	 */
  {8, 0x51, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK1	 */
  {8, 0x52, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK2	 */
  {8, 0x53, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* LN_BB_CLK3	 */
  {8, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK1	 */
  {8, 0x55, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK2	 */
  {8, 0x56, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RF_CLK3	 */
  {8, 0x59, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* CLK_DIST	 */
  {8, 0x5A, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SLEEP_CLK1	 */
  {8, 0x66, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SPMI_MONITOR	 */
  {8, 0x67, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* RCS_MSGR	 */
  {8, 0x71, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT0	 */
  {8, 0x72, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* PBS_CLIENT1	 */
  {8, 0x73, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS1	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {8, 0x74, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS2	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {8, 0x75, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* MISC_PBS3	TDOS: Table 3-1 MISC_PBS SW Trigger Assignment */
  {8, 0xB2, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM3	Shared Info AOP */
  {8, 0xB4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM5	BTM 4 channel; ATE PBS CAL  :  Spare:  To be used for testing */
  {8, 0xB5, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM6	BTM 2 channel1: Spare:  To be used for testing */
  {8, 0xB7, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* SDAM8	AMOLED SWIRE Decode: PM855A; Spare PM855L: Spare:  To be used for testing */
  {8, 0xC0, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO01 AOSS (Always on sleep indicator) -- Debug, OEMs don't need */
  {8, 0xC4, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* GPIO05	PMIC Boot Options */
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
  {9, 0x22, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S3_FREQ	 */
  {9, 0x23, SPMI_OWNER_AOP,    SPMI_OWNER_AOP},   /* S6_CTRL	 */
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
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	 */
  {0, 0x32, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE2_MDM	VADC (SINGLE-SHOT) */
  {0, 0x38, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE5_BTM2	VADC BTM (THRESHOLDS) */
  {0, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* XO	 */
  {0, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* RF_CLK1	 */
  {0, 0xB3, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* SDAM4	Shared Info MPSS */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	 */
  {4, 0x32, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE2_MDM	VADC (SINGLE-SHOT) */
  {4, 0x38, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE5_BTM2	VADC BTM (THRESHOLDS) */
  {4, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* XO	 */
  {4, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* RF_CLK1	 */
  {4, 0xB3, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* SDAM4	Shared Info MPSS */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* BUS	 */
  {8, 0x32, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE2_MDM	VADC (SINGLE-SHOT) */
  {8, 0x38, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* ADC_CORE5_BTM2	VADC BTM (THRESHOLDS) */
  {8, 0x50, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* XO	 */
  {8, 0x54, SPMI_OWNER_AOP,    SPMI_OWNER_MSS},   /* RF_CLK1	 */
  {8, 0xB3, SPMI_OWNER_MSS,    SPMI_OWNER_MSS},   /* SDAM4	Shared Info MPSS */
  {0, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	 */
  {0, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* PON	 */
  {0, 0x24, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* TEMP_ALARM	ALARM */
  {0, 0x31, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE1_USR	VADC (SINGLE-SHOT) */
  {0, 0x35, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE5_BTM4	VADC BTM (THRESHOLDS) */
  {0, 0x5B, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK1	 */
  {0, 0x5C, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK2	 */
  {0, 0x61, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* RTC_ALARM	 */
  {0, 0x62, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* RTC_TIMER	 */
  {0, 0xB1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* SDAM2	Shared Info HLOS */
  {0, 0xB6, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* SDAM7	PBS Pattern Generator1: Spare:  To be used for testing */
  {0, 0xC0, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO01	SPARE BUTTON 1 */
  {0, 0xC1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO02	SPARE GPIO */
  {0, 0xC2, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO03	SDC4 card det */
  {0, 0xC3, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO04	SDC2 card det */
  {0, 0xC5, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO06	SPARE BUTTON 2 */
  {0, 0xC6, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO07	USB PORT 3 FAULT */
  {0, 0xC8, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO09	USB PORT 2 POWER ENABLE */
  {0, 0xC9, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO10	USB PORT 2 FAULT */
  {4, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	 */
  {4, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* PON	 */
  {4, 0x24, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* TEMP_ALARM	ALARM */
  {4, 0x31, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE1_USR	VADC (SINGLE-SHOT) */
  {4, 0x35, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE5_BTM4	VADC BTM (THRESHOLDS) */
  {4, 0x5B, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK1	 */
  {4, 0x5C, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK2	 */
  {4, 0x61, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* RTC_ALARM	 */
  {4, 0x62, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* RTC_TIMER	 */
  {4, 0xB1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* SDAM2	Shared Info HLOS */
  {4, 0xB6, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* SDAM7	PBS Pattern Generator1: Spare:  To be used for testing */
  {4, 0xC0, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO01	Spare ? */
  {4, 0xC1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO02	Spare */
  {4, 0xC2, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO03	USB PORT 0 ID */
  {4, 0xC3, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO04	USB PORT 3 ID */
  {4, 0xC5, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO06	USB PORT 3 DETECT */
  {4, 0xC6, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO07	USB PORT 3 POWER ENABLE */
  {4, 0xC7, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO08	Spare */
  {4, 0xC8, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO09	USB PORT 1 DETECT */
  {4, 0xC9, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO10	USB PORT 2 DETECT */
  {8, 0x04, SPMI_OWNER_AOP,    SPMI_OWNER_APPS},   /* BUS	 */
  {8, 0x08, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* PON	 */
  {8, 0x24, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* TEMP_ALARM	ALARM */
  {8, 0x31, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE1_USR	VADC (SINGLE-SHOT) */
  {8, 0x35, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* ADC_CORE5_BTM4	VADC BTM () */
  {8, 0x5B, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK1	 */
  {8, 0x5C, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* DIV_CLK2	 */
  {8, 0x61, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* RTC_ALARM	 */
  {8, 0x62, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* RTC_TIMER	 */
  {8, 0xB1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* SDAM2	Shared Info HLOS */
  {8, 0xB6, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* SDAM7	PBS Pattern Generator1: Spare:  To be used for testing */
  {8, 0xC1, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO02	SPARE GPIO */
  {8, 0xC2, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO03	USB PORT 1 ID */
  {8, 0xC3, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO04	USB PORT 2 ID */
  {8, 0xC5, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO06	USB PORT 0 DETECT */
  {8, 0xC6, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO07	USB PORT 0 FAULT */
  {8, 0xC7, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO08	USB PORT 0 POWER ENABLE */
  {8, 0xC8, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO09	USB PORT 1 POWER ENABLE */
  {8, 0xC9, SPMI_OWNER_APPS,   SPMI_OWNER_APPS},   /* GPIO10	USB PORT 1 FAULT */
};


/* Number of spmi channels config entries. . */
uint32 pm_spmi_channel_cfg_size[] = {sizeof(pm_spmi_channel_cfg) / sizeof(SpmiCfg_ChannelCfg)};

