#ifndef __DEV_EXTENDED_CDT_PARAMS_H__
#define __DEV_EXTENDED_CDT_PARAMS_H__

/*=============================================================================

DDR runtime parameters
Header File

GENERAL DESCRIPTION
This file defines the DDR runtime paramters
=============================================================================*/
/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
/*==========================================================================

                               INCLUDE FILES
===========================================================================*/
#include "HALcomdef.h"
#include "ddr_aop_params.h"
#include "phy_common.h"

// ===============================================================================
// This file contains structure declarations for the Extended CDT. In this MSM8996
// DSF, the Extended CDT is not being processed. It has been put in purely to freeze
// the data structures and APIs. Application of Extended CDT parameters into DDR
// subsystem and boot-time and run-time will be implemented in a future DSF release.
// ===============================================================================

#define NUM_DRAM_LATENCY_STRUCTS   8
#define NUM_FREQ_SWITCH_STRUCTS    7
// #define MAX_NUM_CLOCK_PLAN 13
// #define MAX_NUM_SHUB_CLOCK_PLAN 8
#define NUM_QOS_CLOCK_BAND_PLAN 2
#define MAX_NUM_SHRM_CLOCK_PLAN 5

// --------------------------------- DRAM Latency --------------------------------------------------
struct dram_latency_runtime_struct
{
    uint8   RL_DBI_Off_x16;
    uint8   RL_DBI_On_x16;
    uint8   RL_DBI_Off_x8;
    uint8   RL_DBI_On_x8;
    uint8   WL;
    uint8   nWR_x16;
    uint8   nWR_x8;
    uint8   nRTP;
    uint8   ODTLon;
    uint8   ODTLoff;
    uint8   MR2;
    uint32  clk_in_ps;
};

// --------------------------------- BIMC Frequency Switch Params -------------------------------------
struct mc_freq_switch_params_runtime_struct
{
    uint8   rd_dbi;
    uint8   odt;
    uint8   fsp;
	uint8   wr_pre;
	uint8   rd_pre;
	uint8   rpst;
    uint8   MR3;
    uint8   MR11;
    uint32  clk_in_ps;
};

// --------------------------------- Read and Write Timing Extensions --------------------------------------------------
struct mc_timing_extend_struct
{
    uint32  dr_extend;
    uint32  sr_extend;
};

// --------------------------------- PWRS Extend Cycles --------------------------------------------------
struct mc_pwrs_extend_struct
{
    uint8  pwrs_extend;
    uint8  opcode_extend;
    uint8  ca_act_pwrs_extend;
    uint8  dq_act_pwrs_extend;
};

/**********************/
/*** DDR Clock Plan ***/
/**********************/
/**
 * ddr_clock_plan: DDR Clock Plan Data Structure.
 */
struct ddr_clock_plan
{
  uint32 clk_index;
  uint32 clk_freq_in_khz; /**< Clock freq in khz*/
  uint32 clk_mode;        /**< Clocking mode: 0=GCC 1=DDRCC*/
  uint32 clk_period_in_psec; /**< Clock freq in psec*/
};

/***********************/
/*** SHUB Clock Plan ***/
/***********************/
/**
 * shub_clock_plan: SHUB Clock Plan Data Structure.
 */
struct shub_clock_plan
{
  uint32 shub_clk_index;
  uint32 shub_clk_freq_in_khz; /**< Clock freq in khz*/
  uint32 shub_clk_freq_in_psec; /**< Clock freq in psec*/

};


/***********************/
/*** SHRM Clock Plan ***/
/***********************/
/**
 * shrm_clock_plan: SHRM Clock Plan Data Structure.
 */
struct shrm_clock_plan
{
  uint32 shrm_clk_index;
  uint32 shrm_clk_freq_in_khz; /**< Clock freq in khz*/

};


struct shrm_debug
{
   uint32 shrm_timeout_in_usec;
   uint8 crash_shrm_after_timeout_en;
   uint8 delayed_freq_switch_en;
   uint8 skip_wdog_shrm_dump_en;
   uint8 timeout_detected_status;
};

/* Root Clock Gating */
struct shub_clk_rcg
{
    uint8 gemnoc_shub_rcg_en;
    uint8 llcc_shub_rcg_en;
    uint8 mc_shub_rcg_en;
};

struct shrm_clk_rcg
{
    uint8 shrm_rcg_en;
    uint8 mccc_rcg_en;
};

struct dre_clk_rcg
{
    uint8 gemnoc_dre_rcg_en;
    uint8 dre_dre_rcg_en;
};

struct uptree_clk_rcg {
    uint8 shrm_uptree_rcg_en;
    uint8 shub_uptree_rcg_en;
    uint8 dre_uptree_rcg_en;
};

struct ddrss_root_clock_gating
{
    struct shub_clk_rcg     shub_rcg;
    struct shrm_clk_rcg     shrm_rcg;
    struct dre_clk_rcg      dre_rcg;
    struct uptree_clk_rcg   uptree_rcg;
    uint8  gladiator_rcg_en;
    uint8  dre_rcg_en;
    uint8  ddrss_rcg_en;
};


// ----------------------------------- Extended CDT Runtime Structure -------------------------------------
// This structure is a member of SHRM_DDR_STRUCT for use during run-time operations. It is populated by DSF
// code processing the extended_cdt_input_struct{} members with their apply_override knobs.
// ---------------------------------------------------------------------------------------------------------
struct ddr_runtime_struct
{
    uint8   hw_self_refresh_enable;                          /* Default 1 to enable. 0 used for debug. */
    uint8   MR4_polling_enable;                              /* Default 1 to enable. 0 used for debug. */
    uint8   periodic_zq_calibration_enable;                  /* 1: Enable periodic ZQ calibration, 0: Disable periodic ZQ calibration */
    uint8   periodic_training_enable;                        /* 1: Enable periodic training, 0: Disable periodic training */
    uint8   partial_array_self_refresh_enable;               /* 1: Enable partial array self refresh, 0: Disable partial array self refresh */
    uint8   power_down_enable;                               /* 1: Enable power down, 0: Disable power down */
    uint8   clock_stop_enable;                               /* 1: Enable clock stop, 0: Disable clock stop */
    uint8   auto_precharge_enable;                           /* 1: Enable auto precharge, 0: Disable auto precharge */
    uint8   phy_xoclk;                                       /* 1: Enable PHY XO CLK, 0: Disable PHY XO CLK */
    uint8   base_logging_enable;                             /* 1: Enable logging, 0: Disable logging */
    uint8   llcc_extended_logging_enable;                    /* 1: Enable logging, 0: Disable logging */
    uint8   mc_extended_logging_enable;                      /* 1: Enable logging, 0: Disable logging */
    uint8   phy_extended_logging_enable;                     /* 1: Enable logging, 0: Disable logging */
    uint8   shrm_extended_logging_enable;                    /* 1: Enable logging, 0: Disable logging */
    uint16  page_close_timer;                                /* PG_OPEN_TIMER is 12bit wide and is in DDR clock cycles */
    uint32  lpddr4y_support;                                 /* Mask populated using LPDDR4Y_SUPPORT_TYPE to indicate which LPDDR4Y features are supported */
    
    struct  dram_latency_runtime_struct                      dram_latency[NUM_DRAM_LATENCY_STRUCTS];
    struct  mc_freq_switch_params_runtime_struct             mc_freq_switch[NUM_FREQ_SWITCH_STRUCTS];
    
    // Moved from struct ddr_misc and SHRM mc_data.c
    struct  ddr_clock_plan                                   clock_plan[MAX_NUM_CLOCK_PLAN]; /**< Struct of valid clock plan frequencies for DDR */
    struct  mc_timing_extend_struct                          mc_timing_extend[MAX_NUM_CLOCK_PLAN]; /**< Read and Write timer extensions table */
    uint32                                                   mc_rd_opcode_offset[MAX_NUM_CLOCK_PLAN][NUM_CS]; /**< Read opcode offset table */
    uint32                                                   mc_wr_opcode_offset[MAX_NUM_CLOCK_PLAN][NUM_CS]; /**< Write opcode offset table */
    uint8                                                    mc_pwrs_latency_cycle[MAX_NUM_CLOCK_PLAN][4][4]; /**< Powerstates transition latency cycles table */
    uint8                                                    mc_pwrs_cfg[MAX_NUM_CLOCK_PLAN][4][4]; /**< Powerstates configuration table */
    struct  mc_pwrs_extend_struct                            mc_pwrs_extend_cycle[MAX_NUM_CLOCK_PLAN]; /**< Powerstates transition QFI clock extension cycles table */
    uint32                                                   mc_rd_batch_cfg[NUM_QOS_CLOCK_BAND_PLAN][4]; /**< MC CBU_TOP read batch configuration */
    uint32                                                   mc_wr_batch_cfg[NUM_QOS_CLOCK_BAND_PLAN][4]; /**< MC CBU_TOP write batch configuration */
    uint32                                                   mc_rd_timeout_cfg[NUM_QOS_CLOCK_BAND_PLAN][4][2]; /**< MC CBU_TOP read timeout configuration */
    uint8                                                    mc_auto_precharge[NUM_QOS_CLOCK_BAND_PLAN][4]; /**< MC CBU_TOP_RWOPT_CFG auto-precharge configuration */
    struct  shub_clock_plan                                  shub_clk_plan[MAX_NUM_SHUB_CLOCK_PLAN]; /**< Struct of valid clock plan frequencies for SHUB */
    uint32                                                   llcc_ram_timing[MAX_NUM_SHUB_CLOCK_PLAN][6];
    uint32                                                   llcc_credit_th0_cfg[NUM_QOS_CLOCK_BAND_PLAN][3];
    uint32                                                   llcc_credit_th1_cfg[NUM_QOS_CLOCK_BAND_PLAN][3];
    uint32                                                   llcc_credit_th2_cfg[NUM_QOS_CLOCK_BAND_PLAN][3];
    uint32                                                   llcc_credit_th3_cfg[NUM_QOS_CLOCK_BAND_PLAN][3];
    uint8                                                    llcc_qactive_hyst[MAX_NUM_CLOCK_PLAN][MAX_NUM_SHUB_CLOCK_PLAN]; /**< Array of qactive hyst cycles per MC & SHUB combination */
    uint32  ddr_num_clock_levels;
    uint32  max_ddr_frequency;                               /** < Requested max frequency at boot time; actual supported max freq after boot */
    struct  ddrss_root_clock_gating                          ddrss_rcg;  
    struct shrm_clock_plan                                   shrm_clk_plan[MAX_NUM_SHRM_CLOCK_PLAN];
    struct shrm_debug                                        shrm_dbg;
};

#endif /* __DEV_EXTENDED_CDT_PARAMS_H__ */
