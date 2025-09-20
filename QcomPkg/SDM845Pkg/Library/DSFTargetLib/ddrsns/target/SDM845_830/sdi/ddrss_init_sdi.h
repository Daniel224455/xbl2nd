/****************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017, Qualcomm Technologies Incorporated. All rights reserved.
 ****************************************************************************/
/*==============================================================================
                                EDIT HISTORY

$Header: //components/rel/boot.xf/2.0/QcomPkg/SDM845Pkg/Library/DSFTargetLib/ddrsns/target/SDM845_830/sdi/ddrss_init_sdi.h#8 $
$DateTime: 2017/11/21 22:18:18 $
$Author: pwbldsvc $
================================================================================
when       who          what, where, why
--------   ---          --------------------------------------------------------
05/04/14   arindamm     First edit history header. Add new entries at top.
================================================================================*/

#include "cabo_seq_hwioreg_sdi.h"
#include "ddr_ss_seq_hwioreg_sdi.h"
#include "shrm_csr_seq_hwioreg_sdi.h"
#include "ddr_phy_seq_hwioreg_sdi.h"
#include "mccc_seq_hwioreg_sdi.h"

#include "HAL_SNS_DDR.h"

// Code from ddrss.h
#define BROADCAST_CH_NUM	         4

#define RSI_SDI_CLOCKSWITCH_PREWORK_CMD  0x09010002
#define RSI_SDI_CLOCKSWITCH_WORK_CMD     0x09090002
#define RSI_SDI_CLOCKSWITCH_POSTWORK_CMD 0x09110002
#define RSI_SDI_POWERSTATE_D0_WORK_CMD   0x09300000  // Request, No resp required, Last, Powerstate Work, DDRSS, State D0
#define RSI_SDI_POWERSTATE_LB0_WORK_CMD  0x09310000  // Request, No resp required, Last, Powerstate Work, LB, LB powerstate 0

#define SDI_RESET_WAIT_IN_XO_CLKS 10000

/*SHKE periodic events enabling*/
typedef enum
{
SHKE_SDI_INTERVAL_EVENT_CTRL_0_EN = 0x0,
SHKE_SDI_ZQSTART_EVENT_CTRL_EN    = 0x1,
SHKE_SDI_ZQLATCH_EVENT_CTRL_EN    = 0x2,
SHKE_SDI_PDT_TRAC_EVENT_CTRL_EN   = 0x3
} SHKE_SDI_PERIODIC_EVENT_CTRL;


boolean HAL_DDR_SDI_Recover (SHRM_DDR_STRUCT *shrm_ddr_ptr, DDR_CHANNEL channel, DDR_CHIPSELECT chip_select, uint32 clk_freq_khz);
void wait_for_xo_clocks_sdi(uint32 xo_clocks);
void DDRSS_Downtime_Request_Ctrl_sdi (uint8 enable);


// Code from mc_common.h
#define REG_OFFSET_MC_MPE(uint8)      ((uint8 == 0) ? \
                                       (SEQ_DDR_SS_CABO0_CABO_MPE_OFFSET) : \
                                       (uint8 == 1) ? \
                                       (SEQ_DDR_SS_CABO1_CABO_MPE_OFFSET) : \
                                       (uint8 == 2) ? \
                                       (SEQ_DDR_SS_CABO2_CABO_MPE_OFFSET) : \
                                       (uint8 == 3) ? \
                                       (SEQ_DDR_SS_CABO3_CABO_MPE_OFFSET) : \
                                       (SEQ_DDR_SS_CABO_BROADCAST_CABO_MPE_OFFSET)) 
#define REG_OFFSET_MC_SHKE(uint8)     ((uint8 == 0) ? \
                                       (SEQ_DDR_SS_CABO0_CABO_SHKE_OFFSET) : \
                                       (uint8 == 1) ? \
                                       (SEQ_DDR_SS_CABO1_CABO_SHKE_OFFSET) : \
                                       (uint8 == 2) ? \
                                       (SEQ_DDR_SS_CABO2_CABO_SHKE_OFFSET) : \
                                       (uint8 == 3) ? \
                                       (SEQ_DDR_SS_CABO3_CABO_SHKE_OFFSET) : \
                                       (SEQ_DDR_SS_CABO_BROADCAST_CABO_SHKE_OFFSET)) 



// Code from ddrss.h
#define DDR_PHY_OFFSET           0x1000      // DDR PHY Address offset (2k Bytes)
#define CA0_DDR_PHY_OFFSET       0x0000
#define DQ0_DDR_PHY_OFFSET       0x1000
#define DQ1_DDR_PHY_OFFSET       0x2000
#define DDR_CC_OFFSET            0x6000

#define REG_OFFSET_DDR_PHY_CH(ch) ((ch == 0)  ? DDR_PHY_SS_BASE :\
                                   (ch == 1)  ? DDR_PHY_SS_BASE + 0x03000 :\
                                   (ch == 2)  ? DDR_PHY_SS_BASE + 0x10000 :\
                                                DDR_PHY_SS_BASE + 0x13000)

#define REG_OFFSET_DDR_CC_CH(ch)  ((ch == 0)  ? DDR_PHY_SS_BASE :\
                                   (ch == 1)  ? DDR_PHY_SS_BASE :\
                                   (ch == 2)  ? DDR_PHY_SS_BASE + 0x10000 :\
                                                DDR_PHY_SS_BASE + 0x10000)

// Code from ddrss_mccc.h
#define REG_OFFSET_MCCC_SLV(uint8)      ((uint8 == 0) ? \
                                        (SEQ_DDR_SS_MCCC_CH0_MCCC_SLV_OFFSET) : \
                                        (uint8 == 1) ? \
                                        (SEQ_DDR_SS_MCCC_CH1_MCCC_SLV_OFFSET) : \
                                        (uint8 == 2) ? \
                                        (SEQ_DDR_SS_MCCC_CH2_MCCC_SLV_OFFSET) : \
                                        (uint8 == 3) ? \
                                        (SEQ_DDR_SS_MCCC_CH3_MCCC_SLV_OFFSET) : \
                                        (SEQ_DDR_SS_MCCC_BROADCAST_MCCC_SLV_OFFSET)) 

										
										
// Code from ddrss_shrm.h
void SHRM_rpmh_port_override_control_sdi(uint8 en);
void SHRM_rsi_cmd_inject_sdi(uint32 cmd_payload);
