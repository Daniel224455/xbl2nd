/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
                                EDIT HISTORY

================================================================================
when       who          what, where, why
--------   ---          --------------------------------------------------------
05/04/14   arindamm     First edit history header. Add new entries at top.
================================================================================*/

#include "cabo_seq_hwioreg_sdi.h"
#include "ddr_ss_seq_hwioreg_sdi.h"
#include "shrm_csr_seq_hwioreg_sdi.h"

#include "HAL_SNS_DDR.h"

// Code from ddrss.h

#define RSI_SDI_CLOCKSWITCH_PREWORK_CMD  0x09010002
#define RSI_SDI_CLOCKSWITCH_WORK_CMD     0x09090002
#define RSI_SDI_CLOCKSWITCH_POSTWORK_CMD 0x09110002
#define RSI_SDI_POWERSTATE_D0_WORK_CMD   0x09300000  // Request, No resp required, Last, Powerstate Work, DDRSS, State D0
#define RSI_SDI_POWERSTATE_LB0_WORK_CMD  0x09310000  // Request, No resp required, Last, Powerstate Work, LB, LB powerstate 0
#define RSI_SDI_PASR_EXIT_CMD            0x09600000  // Request, No resp required, Last, PASR, segmask=0x00

/*SHKE periodic events enabling*/
typedef enum
{
SHKE_SDI_INTERVAL_EVENT_CTRL_0_EN = 0x0,
SHKE_SDI_ZQSTART_EVENT_CTRL_EN    = 0x1,
SHKE_SDI_ZQLATCH_EVENT_CTRL_EN    = 0x2,
SHKE_SDI_PDT_TRAC_EVENT_CTRL_EN   = 0x3
} SHKE_SDI_PERIODIC_EVENT_CTRL;


// Code from mc_common.h
#define REG_OFFSET_MC_SHKE(uint8)     ((uint8 == 0) ? \
                                       (SEQ_DDR_SS_CABO0_CABO_SHKE_OFFSET) : \
                                       (uint8 == 1) ? \
                                       (SEQ_DDR_SS_CABO1_CABO_SHKE_OFFSET) : \
                                       (uint8 == 2) ? \
                                       (SEQ_DDR_SS_CABO2_CABO_SHKE_OFFSET) : \
                                       (uint8 == 3) ? \
                                       (SEQ_DDR_SS_CABO3_CABO_SHKE_OFFSET) : \
									    (uint8 == 4) ? \
                                       (SEQ_DDR_SS_CABO4_CABO_SHKE_OFFSET) : \
									    (uint8 == 5) ? \
                                       (SEQ_DDR_SS_CABO5_CABO_SHKE_OFFSET) : \
									    (uint8 == 6) ? \
                                       (SEQ_DDR_SS_CABO6_CABO_SHKE_OFFSET) : \
									    (uint8 == 7) ? \
                                       (SEQ_DDR_SS_CABO7_CABO_SHKE_OFFSET) : \
                                       (SEQ_DDR_SS_CABO_BROADCAST_CABO_SHKE_OFFSET)) 



// Code from ddrss.h
boolean HAL_DDR_SDI_Recover (SHRM_DDR_STRUCT *shrm_ddr_ptr, DDR_CHANNEL channel, DDR_CHIPSELECT chip_select, uint32 clk_freq_khz);
void DDRSS_Downtime_Request_Ctrl_sdi (uint8 enable);


// Code from ddrss_shrm.h
void SHRM_rpmh_port_override_control_sdi(uint8 en);
void SHRM_rsi_cmd_inject_sdi(uint32 cmd_payload);
