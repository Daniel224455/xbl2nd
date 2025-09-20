/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
$Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/SDMPkg/1000/Library/DSFTargetLib/common/mc/mc40/header/mc_common.h#1 $
$DateTime: 2021/04/29 02:01:56 $
$Author: pwbldsvc $
================================================================================*/

#ifndef _MC_COMMON_H
#define _MC_COMMON_H

#include "cabo_seq_hwiobase.h"
#include "cabo_seq_hwioreg.h"
#include "HAL_SNS_DDR.h"
#include "ddrss_common.h"

#define REG_OFFSET_MC_CH(ch)            (SEQ_DDR_SS_CABO0_CABO_OFFSET + (ch * 0x80000)) 
#define REG_OFFSET_MC_ISU_CH(ch)        (SEQ_DDR_SS_CABO0_CABO_ISU_OFFSET + (ch * 0x80000)) 
#define REG_OFFSET_MC_MPE_CH(ch)        (SEQ_DDR_SS_CABO0_CABO_MPE_OFFSET + (ch * 0x80000)) 
#define REG_OFFSET_MC_DTC_CH(ch)        (SEQ_DDR_SS_CABO0_CABO_DTC_OFFSET + (ch * 0x80000))
#define REG_OFFSET_MC_OSU_CH(ch)        (SEQ_DDR_SS_CABO0_CABO_OSU_OFFSET + (ch * 0x80000)) 
#define REG_OFFSET_MC_PXI_CH(ch)        (SEQ_DDR_SS_CABO0_CABO_PXI_OFFSET + (ch * 0x80000)) 
#define REG_OFFSET_MC_SHKE_CH(ch)       (SEQ_DDR_SS_CABO0_CABO_SHKE_OFFSET + (ch * 0x80000))
#define REG_OFFSET_MC_GLOBAL_CH(ch)     (SEQ_DDR_SS_CABO0_CABO_GLOBAL_OFFSET + (ch * 0x80000 ))
#define REG_OFFSET_MC_CBU_TOP_CH(ch)    (SEQ_DDR_SS_CABO0_CABO_CBU_TOP_OFFSET + (ch * 0x80000)) 
#define REG_OFFSET_MC_PERFMON_CH(ch)    (SEQ_DDR_SS_CABO0_CABO_PERFMON_OFFSET + (ch * 0x80000)) 
#define REG_OFFSET_MC_INTERRUPT_CH(ch)  (SEQ_DDR_SS_CABO0_CABO_INTERRUPT_OFFSET +(ch * 0x80000))
#define REG_OFFSET_MC_GLOBAL_SEC_CH(ch) (SEQ_DDR_SS_CABO0_CABO_GLOBAL_SEC_OFFSET + (ch * 0x80000)) 
#define REG_OFFSET_MC_PXI_SECURE_CH(ch) (SEQ_DDR_SS_CABO0_CABO_PXI_SECURE_OFFSET + (ch * 0x80000)) 

#if REDIRECT_BROADCAST_TO_CH0
//Re-assign BROADCAST macro to CH0 address offset 

#define REG_OFFSET_MC_BROADCAST            SEQ_DDR_SS_CABO0_CABO_OFFSET
#define REG_OFFSET_MC_ISU_BROADCAST        SEQ_DDR_SS_CABO0_CABO_ISU_OFFSET
#define REG_OFFSET_MC_MPE_BROADCAST        SEQ_DDR_SS_CABO0_CABO_MPE_OFFSET 
#define REG_OFFSET_MC_DTC_BROADCAST        SEQ_DDR_SS_CABO0_CABO_DTC_OFFSET
#define REG_OFFSET_MC_OSU_BROADCAST        SEQ_DDR_SS_CABO0_CABO_OSU_OFFSET
#define REG_OFFSET_MC_PXI_BROADCAST        SEQ_DDR_SS_CABO0_CABO_PXI_OFFSET
#define REG_OFFSET_MC_SHKE_BROADCAST       SEQ_DDR_SS_CABO0_CABO_SHKE_OFFSET
#define REG_OFFSET_MC_GLOBAL_BROADCAST     SEQ_DDR_SS_CABO0_CABO_GLOBAL_OFFSET
#define REG_OFFSET_MC_CBU_TOP_BROADCAST    SEQ_DDR_SS_CABO0_CABO_CBU_TOP_OFFSET
#define REG_OFFSET_MC_PERFMON_BROADCAST    SEQ_DDR_SS_CABO0_CABO_PERFMON_OFFSET 
#define REG_OFFSET_MC_INTERRUPT_BROADCAST  SEQ_DDR_SS_CABO0_CABO_INTERRUPT_OFFSET
#define REG_OFFSET_MC_GLOBAL_SEC_BROADCAST SEQ_DDR_SS_CABO0_CABO_GLOBAL_SEC_OFFSET
#define REG_OFFSET_MC_PXI_SECURE_BROADCAST SEQ_DDR_SS_CABO0_CABO_PXI_SECURE_OFFSET

#else

#define REG_OFFSET_MC_BROADCAST            SEQ_DDR_SS_CABO_BROADCAST_CABO_OFFSET
#define REG_OFFSET_MC_ISU_BROADCAST        SEQ_DDR_SS_CABO_BROADCAST_CABO_ISU_OFFSET
#define REG_OFFSET_MC_MPE_BROADCAST        SEQ_DDR_SS_CABO_BROADCAST_CABO_MPE_OFFSET 
#define REG_OFFSET_MC_DTC_BROADCAST        SEQ_DDR_SS_CABO_BROADCAST_CABO_DTC_OFFSET
#define REG_OFFSET_MC_OSU_BROADCAST        SEQ_DDR_SS_CABO_BROADCAST_CABO_OSU_OFFSET
#define REG_OFFSET_MC_PXI_BROADCAST        SEQ_DDR_SS_CABO_BROADCAST_CABO_PXI_OFFSET
#define REG_OFFSET_MC_SHKE_BROADCAST       SEQ_DDR_SS_CABO_BROADCAST_CABO_SHKE_OFFSET
#define REG_OFFSET_MC_GLOBAL_BROADCAST     SEQ_DDR_SS_CABO_BROADCAST_CABO_GLOBAL_OFFSET
#define REG_OFFSET_MC_CBU_TOP_BROADCAST    SEQ_DDR_SS_CABO_BROADCAST_CABO_CBU_TOP_OFFSET
#define REG_OFFSET_MC_PERFMON_BROADCAST    SEQ_DDR_SS_CABO_BROADCAST_CABO_PERFMON_OFFSET 
#define REG_OFFSET_MC_INTERRUPT_BROADCAST  SEQ_DDR_SS_CABO_BROADCAST_CABO_INTERRUPT_OFFSET
#define REG_OFFSET_MC_GLOBAL_SEC_BROADCAST SEQ_DDR_SS_CABO_BROADCAST_CABO_GLOBAL_SEC_OFFSET
#define REG_OFFSET_MC_PXI_SECURE_BROADCAST SEQ_DDR_SS_CABO_BROADCAST_CABO_PXI_SECURE_OFFSET

#endif

#define XO_PERIOD_IN_PS                 (52080)   //52.08ns, based on XO = 19.2MHz

#define MC_MPE_OFFSET  0x3000
#define MC_DTC_OFFSET  0x4000
#define MC_SHKE_OFFSET 0x5000
#define MC_PXI_OFFSET  0x8000
#define MC_PXI_SECURE_OFFSET  0xE000

///////////////////////////// PXI defines/////////////////////////////////////////////////////
#define PRBS_SEED_BYTE0             0x5106D633
#define PRBS_SEED_BYTE1             0xE85334E1

#define PXI_WR_BROADCAST_EN         1
#define PXI_RD_BROADCAST_EN         1

                                    //2092 MC clock cycles - 1
#define PXI_TCKCKEH_MAX             0x1  //max(1.75ns, 3nCK)
#define PXI_TXP_MAX                 0x8  //max(7.5ns, 5nCK)
#define PXI_TRCD_MAX                0x12  //max(18ns, 4nCK)
#define PXI_ACT_DELAY               (PXI_TCKCKEH_MAX + PXI_TXP_MAX + PXI_TRCD_MAX) 
#define PXI_BL32_COMMAND_DELTA      0x8 //must be >= PXI_BL32_TASK_CNT but can't be PXI_BL32_TASK_CNT + 2
#define PXI_BL32_TASK_CNT           0x8
#define PXI_2_CYCLE_CMD             0x1
#define PXI_4_CYCLE_CMD             0x2

#define PXI_PRBS                    1
#define PXI_INC_DEC                 0x20

#define PXI_WR_CS0_START_ADDR       0x0
#define PXI_WR_CS1_START_ADDR       0x8
#define PXI_RD_CS0_START_ADDR       0x10
#define PXI_RD_CS1_START_ADDR       0x18
#define PXI_RD_WR_CS0_START_ADDR    0x0
#define PXI_RD_WR_CS1_START_ADDR    0x8

#define PXI_UD_PATTERN_0            0x40408080
#define PXI_UD_PATTERN_1            0x04040808
#define PXI_UD_PATTERN_2            0x0A0A0A0A
#define PXI_UD_PATTERN_3            0xA0A0A0A0
#define PXI_UD_PATTERN_4            0xFFFF0000
#define PXI_UD_PATTERN_5            0xFFFF0000
#define PXI_UD_PATTERN_6            0x0000FFFF
#define PXI_UD_PATTERN_7            0x0000FFFF
#define PXI_UD_PATTERN_8            0xF0F00F0F
#define PXI_UD_PATTERN_9            0xF0F00F0F
#define PXI_UD_PATTERN_10           0x0F0FF0F0
#define PXI_UD_PATTERN_11           0x0F0FF0F0
#define PXI_UD_PATTERN_12           0xF0F0F1F1
#define PXI_UD_PATTERN_13           0xF0F0F1F1
#define PXI_UD_PATTERN_14           0xF1F1F0F0
#define PXI_UD_PATTERN_15           0xF1F1F0F0
#define PXI_UD_PATTERN_16           0xAAAA5555
#define PXI_UD_PATTERN_17           0xAAAA5555
#define PXI_UD_PATTERN_18           0x5555AAAA
#define PXI_UD_PATTERN_19           0x5555AAAA
#define PXI_UD_PATTERN_20           0x5A5AA5A5
#define PXI_UD_PATTERN_21           0x5A5AA5A5
#define PXI_UD_PATTERN_22           0xA5A55A5A
#define PXI_UD_PATTERN_23           0xA5A55A5A
#define PXI_UD_PATTERN_24           0x5A5A5B5B
#define PXI_UD_PATTERN_25           0x5A5A5B5B
#define PXI_UD_PATTERN_26           0x5B5B5A5A
#define PXI_UD_PATTERN_27           0x5B5B5A5A
#define PXI_UD_PATTERN_28           0x0101FEFE
#define PXI_UD_PATTERN_29           0xFEFE0101
#define PXI_UD_PATTERN_30           0x0202FDFD
#define PXI_UD_PATTERN_31           0xFDFD0202
#define PXI_UD_PATTERN_32           0x0404FBFB
#define PXI_UD_PATTERN_33           0xFBFB0404
#define PXI_UD_PATTERN_34           0x0808F7F7
#define PXI_UD_PATTERN_35           0xF7F70808
#define PXI_UD_PATTERN_36           0x1010EFEF
#define PXI_UD_PATTERN_37           0xEFEF1010
#define PXI_UD_PATTERN_38           0x2020DFDF
#define PXI_UD_PATTERN_39           0xDFDF2020
#define PXI_UD_PATTERN_40           0x4040BFBF
#define PXI_UD_PATTERN_41           0xBFBF4040
#define PXI_UD_PATTERN_42           0x80807F7F
#define PXI_UD_PATTERN_43           0x7F7F8080
#define PXI_UD_PATTERN_44           0x32321010
#define PXI_UD_PATTERN_45           0xBABA9898
#define PXI_UD_PATTERN_46           0x19190808
#define PXI_UD_PATTERN_47           0x5D5D4C4C
#define PXI_UD_PATTERN_48           0x0C0C8484
#define PXI_UD_PATTERN_49           0x2E2EA6A6
#define PXI_UD_PATTERN_50           0x86864242
#define PXI_UD_PATTERN_51           0x97975353
#define PXI_UD_PATTERN_52           0x43432121
#define PXI_UD_PATTERN_53           0xCBCBA9A9
#define PXI_UD_PATTERN_54           0x51516060
#define PXI_UD_PATTERN_55           0x15152424
#define PXI_UD_PATTERN_56           0x2828B0B0
#define PXI_UD_PATTERN_57           0x0A0A9292
#define PXI_UD_PATTERN_58           0x94945858
#define PXI_UD_PATTERN_59           0x85854949
#define PXI_UD_PATTERN_60           0x4A4A2C2C
#define PXI_UD_PATTERN_61           0xC2C2A4A4
#define PXI_UD_PATTERN_62           0x25251616
#define PXI_UD_PATTERN_63           0x61615252

#define PXI_DATA_ADDRESS_0          0x0
#define PXI_DATA_ADDRESS_1          0x8
#define PXI_DATA_ADDRESS_2          0x10
#define PXI_DATA_ADDRESS_3          0x18

#if PXI_PRBS
    #define PXI_REPEAT_CNT          29 //31
#else
    #define PXI_REPEAT_CNT          29
#endif

#define COLUMN_ADDRESS_PRECHARGE    PXI_INC_DEC * PXI_REPEAT_CNT

#define PXI_ACT_1_H                 0x1
#define PXI_ACT_1_L                 0x0
#define PXI_ACT_2_H                 0x3
#define PXI_ACT_2_L                 0x0
#define PXI_WRITE_1_H               0x24
#define PXI_WRITE_1_L               0x0
#define PXI_READ_1_L                0x0
#define PXI_READ_1_H                0x22
#define PXI_CAS_2_H                 0x12
#define PXI_CAS_2_L                 0x0
#define PXI_WRITE_1_L_PC            PXI_WRITE_1_L | (1 << 5) | (((COLUMN_ADDRESS_PRECHARGE >> 9) & 1) << 4)
#define PXI_READ_1_L_PC             PXI_READ_1_L | (1 << 5) | (((COLUMN_ADDRESS_PRECHARGE >> 9) & 1) << 4)
#define PXI_CAS_2_H_PC              PXI_CAS_2_H | (((COLUMN_ADDRESS_PRECHARGE >> 8) & 1) << 5)
#define PXI_CAS_2_L_PC              ((COLUMN_ADDRESS_PRECHARGE >> 2) & 0x3F)

/** PXI blocks enumeration */
typedef enum
{
   PXI_DQDM = 0x0,
   PXI_DQRD = 0x1,
   PXI_DQWD = 0x2,
   PXI_DQOP = 0x4,
   PXI_CAWD = 0x8,
   PXI_CAOP = 0x10
} PXI_BLOCK;

/** PXI byte/bit mode enumeration */
typedef enum
{
   PXI_BYTE_ERR_MODE = 0x0,
   PXI_BIT_ERR_MODE  = 0x1
} PXI_ERR_MODE;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    WAIT_XO_CLOCK    = 0x0,
    WAIT_SLEEP_CLOCK = 0x1,
    WAIT_CORE_CLOCK  = 0x2
} MC_Wait_Timer_Domain;

typedef enum
{
    RL_DBI_OFF = 0x0 ,
    RL_DBI_ON  = 0x1 ,
    WL         = 0x2 ,
    MR2_WR_VAL = 0x3
} RL_WL_Table_Sel;

typedef enum
{
    DEVICE_WIDTH_IS_x16 = 0x0 ,
    DEVICE_WIDTH_IS_x8  = 0x1 
} Device_Io_Width_sel;

// #defines for all valid MRs
#define JEDEC_MR_0   0x0
#define JEDEC_MR_1   0x1
#define JEDEC_MR_2   0x2
#define JEDEC_MR_3   0x3
#define JEDEC_MR_4   0x4
#define JEDEC_MR_5   0x5
#define JEDEC_MR_6   0x6
#define JEDEC_MR_7   0x7
#define JEDEC_MR_8   0x8
#define JEDEC_MR_9   0x9
#define JEDEC_MR_11  0xB
#define JEDEC_MR_12  0xC
#define JEDEC_MR_13  0xD
#define JEDEC_MR_14  0xE
#define JEDEC_MR_15  0xF
#define JEDEC_MR_16  0x10
#define JEDEC_MR_17  0x11
#define JEDEC_MR_18  0x12
#define JEDEC_MR_19  0x13
#define JEDEC_MR_20  0x14
#define JEDEC_MR_22  0x16
#define JEDEC_MR_23  0x17
#define JEDEC_MR_24  0x18
#define JEDEC_MR_32  0x20
#define JEDEC_MR_39  0x27
#define JEDEC_MR_40  0x28

typedef enum
{
   CMD_REFRESH_AB            = 0x40,
   CMD_SELF_REFRESH_SRE      = 0x50,
   CMD_SELF_REFRESH_SRE_ASAP = 0x51,
   CMD_SELF_REFRESH_SRX      = 0x54,
   CMD_POWER_DOWN_PDE        = 0x60,
   CMD_POWER_DOWN_PDX        = 0x64,
   CMD_POWER_DOWN_CSPDE      = 0x68,
   CMD_POWER_DOWN_CSPDX      = 0x6C,
   CMD_MRR                   = 0x70,
   CMD_MRR_MR4               = 0x71,
   CMD_MRW                   = 0x74,
   CMD_MRW_FSP_OP            = 0x75,
   CMD_MRW_VRCG_EN           = 0x76,
   CMD_MRW_VRCG_DIS          = 0x77,
   CMD_MPC_WRFIFO            = 0x80,
   CMD_MPC_RDFIFO            = 0x84,
   CMD_MPC_RDDQCAL           = 0x88, 
   CMD_MPC_ZQCAL_LATCH       = 0x8C,
   CMD_MPC_ZQCAL_START       = 0x90,
   CMD_MPC_DQS_OSC_START     = 0x94,
   CMD_MPC_DQS_OSC_STOP      = 0x98,  
   CMD_INIT_CK_ON            = 0xA0,
   CMD_INIT_CK_OFF           = 0xA4,
   CMD_CUSTOM_CA             = 0xB0,
   CMD_CUSTOM_DQ             = 0xB4,
   CMD_MCPHY_FCS_STALL       = 0xC0,
   CMD_MCPHY_FCS_STALL_EXIT  = 0xC1,
   CMD_MC_SW_STALL           = 0xC4,
   CMD_MC_SW_STALL_EXIT      = 0xC5,
   CMD_NOP                   = 0x00
} MC_SHKE_CMD_EVENT;


/*==============================================================================
                                  FUNCTIONS
==============================================================================*/
/* =============================================================================
**  MC Common Functions
** =============================================================================
*/
/**
 * @brief Configure SHKE_CMD_SET_CFG_n register
 * 
 * @param[in] uint32 ch_offset:   memory controller channel/instance address offset
 * @param[in] uint32 chip_select: rank selection value  (SHKE_CMD_SET_CFG_n[13:12], RANK_SEL) 
 * @param[in] uint8  reg_num:     n value in SHKE_CMD_SET_CFG_n
 * @param[in] uint32 event_sel:   event selection value (SHKE_CMD_SET_CFG_n[9:0], EVENT_SEL) 
 * @param[in] uint32 mr_addr:     MR address value      (SHKE_CMD_SET_CFG_n[23:16], MR_ADDR) 
 * @param[in] uint32 mr_data:     MR data value         (SHKE_CMD_SET_CFG_n[31:24], MR_DATA) 
 * 
 * @retval None
 */   
void MC_Cmd_Set_Cfg (uint32 ch_offset, uint32 chip_select, uint8 reg_num,\
                     uint32 event_sel, uint32 mr_addr, uint32 mr_data);

/**
 * @brief Configure SHKE_CMD_SET_STAGGER_CFG_n register
 * 
 * @param[in] uint32  ch_offset:         memory controller channel/instance address offset
 * @param[in] uint8  reg_num:    n value in SHKE_CMD_SET_STAGGER_CFG_n
 * @param[in] uint32 stagger_en: 0->concurrent issue, 1->two separate commands to Rank0 and Rank1
 *                                           (SHKE_CMD_SET_STAGGER_CFG_n[20]: STAGGER_RANKS_EN)
 * @param[in] uint32 clk_sel:    0->XO clock (19.2MHz), 1->Sleep clock (32KHz), 2->Core clock 
 *                                           (SHKE_CMD_SET_STAGGER_CFG_n[17:16], CLK_SELECT) 
 * @param[in] uint32 interval:   timer value (SHKE_CMD_SET_STAGGER_CFG_n[15:0], INTERVAL) 
 * 
 * @retval None
 */  
void MC_Cmd_Stagger_Cfg (uint32 ch_offset, uint8 reg_num, uint32 stagger_en,\
                         uint32 clk_sel, uint32 interval);

/** 
 * @brief Trigger SHKE Command and wait for completion
 * 
 * @param[in] uint32  ch_offset:      memory controller channel/instance address offset
 * @param[in] uint32 cmd_sel: command select (SHKE_SW_CMD_SEQ[23:0], CMD_SELECT)
 * 
 * @retval None
 */						 
void MC_Cmd_Trigger (uint32 ch_offset, uint32 cmd_sel);

/**
 * @brief Create MR1 value from fields in RL_WL_lpddr_struct and mc_freq_switch_params_struct 
 *
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to shrm_ddr configuration struct       
 * @param[in] uint32 ch_offset:              memory controller channel/instance address offset 
 * @param[in] uint8 RL_WL_idx:               index number of RL_WL_lpddr_struct
 * @param[in] uint8 params_idx:              index number of mc_freq_switch_params_struct
 *
 * @retval uint8 MR1 value
 */ 
uint8 MC_MR1_Value (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint32 ch_offset, uint8 RL_WL_idx, uint8 params_idx);

/**
 * @brief Based on device RL/WL/ODTLon frequency band, 
          get an index for selecting in RL/WL/ODTLon table
 *
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to shrm_ddr configuration struct         
 * @param[in] uint32 clk_in_ps:              Current clock frequency in pico second
 *
 * @retval uint8 Index for selecting in RL/WL/ODTLon table
 */
uint8 MC_DRAM_Latency_Index (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint32 clk_in_ps);

/**
 * @brief Based on Freq_Switch_Params band, 
 *        get an index for selecting in Freq_Switch_Params table
 *
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to ddr configuration struct         
 * @param[in] uint32 clk_in_ps:              Current clock frequency in pico second
 *
 * @retval uint8 Index for selecting in Freq_Switch_Params table
 */
uint8 MC_Freq_Switch_Params_Index (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint32 clk_in_ps);

/**
 * @brief Enter SW self refresh
 *
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to shrm ddr configuration struct
 *
 * @retval None
 */  
void MC_Enter_Self_Refresh (SHRM_DDR_STRUCT *shrm_ddr_ptr);

/**
 * @brief Exit SW self refresh
 *
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to shrm ddr configuration struct 
 *
 * @retval None
 *
 * @note  When execute this function, if HW self refresh is enabled, 
 *        MC will still be in self refresh until traffic comes        
 */ 
void MC_Exit_Self_Refresh (SHRM_DDR_STRUCT *shrm_ddr_ptr);

/**
 * @brief Enable/Disable HW activity based self refresh
 * 
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to shrm ddr configuration struct
 * @param[in] DDR_CHIPSELECT chip_select: rank selection
 * @param[in] uint8  enable:              0x0->disable, 0x1->enable
 * 
 * @retval None
 */
void MC_HW_Self_Refresh_Ctrl(SHRM_DDR_STRUCT *shrm_ddr_ptr, DDR_CHIPSELECT chip_select, uint8 enable);

/**
 * @brief QFI clock gating control function
 *
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to shrm ddr configuration struct 
 * @param[in] uint8 enable:                  0x0->disable, 0x1->enable       
 *
 * @retval None 
 */
void MC_QFI_Clock_Gating_Ctrl (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint8 enable);

/**
 * @brief power down control function
 *
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to shrm ddr configuration struct 
 * @param[in] uint8 enable:                  0x0->disable, 0x1->enable       
 *
 * @retval None 
 */
void MC_Power_Down_Ctrl (SHRM_DDR_STRUCT *shrm_ddr_ptr, DDR_CHIPSELECT chip_select, uint8 enable);

/**
 * @brief clock stop control function
 *
 * @param[in] SHRM_DDR_STRUCT *shrm_ddr_ptr: Pointer to shrm ddr configuration struct 
 * @param[in] uint8 enable:                  0x0->disable, 0x1->enable       
 *
 * @retval None 
 */
void MC_Clock_Stop_Ctrl (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint8 enable);


#define MAX_MR_CMD_LOG_INDEX 32

/**
 * @brief MR Cmd logging function
 *
 * @param[in] mr_addr: MR Register address 
 * @param[in] mr_data: MR Register value
 * 
 * @retval None 
 */
void MC_MR_Cmd_Log(uint32 mr_addr, uint32 mr_data);

/**
 * @brief Mode register read function only returns a single MR value, it reads on per channel per rank basis
 * 
 * @param[in] uint32 ch_offset:        channel selection address offset
 * @param[in] DDR_CHIPSELECT chip_select: rank selection
 * @param[in] uint32 mr_addr:             MR address value
 * 
 * @retval uint32 MR read back value
 * 
 * @note Both rank option (DDR_CS_BOTH) is not supported for DDR_CHIPSELECT
 */
uint32 MC_MR_Read (uint32 ch_offset, DDR_CHIPSELECT chip_select, uint32 mr_addr);

void MC_PXI_Payload_Config (PXI_BLOCK block, uint8 num,
                            uint32 rpt_cntr, uint32 nop_cntr, uint32 task_cntr,
                            uint32 task_ptr, uint32 error_clr);
                            
void MC_PXI_Latency_Config (void);

void MC_PXI_Update_Config (SHRM_DDR_STRUCT *shrm_ddr_ptr);



#endif

