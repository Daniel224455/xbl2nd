/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
#ifndef __PHY_COMMON_H__
#define __PHY_COMMON_H__

#include "phy_target.h"
#include "target_config.h"
#include "ddr_aop_params.h"
#include "ddr_cc_seq_hwiobase.h"
#include "ddr_cc_seq_hwioreg.h"
#include "ddr_phy_seq_hwiobase.h"
#include "ddr_phy_seq_hwioreg.h"

//RCW related
#define   RCW_COARSE_DELAY        1
#define   RCW_FINE_DELAY          0

#define LOG2(i) (( (i)>=8u)? ( (i)>=32u)?5 :  ( (i)>=16u)?4 : 3 :  ( (i)>=4u)?2 :  ( (i)>=2u)?1 : 0 )

// Defines for boot training status (shrm_ddr_ptr->misc.boot_training_status)
#define TRAINING_NOT_STARTED    (0)
#define TRAINING_IN_PROGRESS    (1)
#define TRAINING_COMPLETED      (2)
#define CA_TRAINING_IN_PROGRESS (3)

#define NUM_CS                    2 // Number of ranks (chip selects)
#define NUM_DQ_PCH                2 // Number of DQ PHYs
#define NUM_CA_PCH                1 // Number of CA PHYs Per Channel
#define NUM_DDR_CC                2 // Number of DDR CCs
#define NUM_PLL                   2 // Number of PLLs in DDRCC
#define NUM_DIE_PCH               2 // Number of DRAM Die per Channel
#define NUM_FSP                   2 // Number of FSP

extern uint32 freq_range[NUM_PRFS_BANDS]; //for 1353MHz expansion. 
extern uint8 connected_bit_mapping_no_DBI_A  [PINS_PER_PHY_CONNECTED_NO_DBI];
extern uint8 connected_bit_mapping_no_DBI_B  [PINS_PER_PHY_CONNECTED_NO_DBI];
extern uint8 connected_bit_mapping_with_DBI_A [PINS_PER_PHY_CONNECTED_WITH_DBI];
extern uint8 connected_bit_mapping_with_DBI_B [PINS_PER_PHY_CONNECTED_WITH_DBI];
//extern uint8 connected_bit_mapping_CA [PINS_PER_PHY_CONNECTED_CA];
// Define DDR base address in the chip address map from APPS viewpoint.
//    From the RPM viewpoint, DDR is also at 0x80000000, but this address
//    is remapped to 0x60000000 by setting RPM_PAGE_SELECT to 2.
#define DDR_BASE                         0x80000000


// Size of statically allocated area in ZI data area that training functions will
// use in place of local variables to reduce stack growth pressure.
// Used in the function HAL_DDR_Boot_Training().
#define LOCAL_VARS_AREA_SIZE            0x5000 /* 20KB */

//#define DSF_SCR_EN                                          0
enum ddr_Training_enable_index
{
    _RD_P       = 0,
    _WR_P       = 1,
  _DCC_Mon  = 0,
  _WRLVL_CF = 0,
  _C_CF     = 0, 
  _C_PB     = 1, 
  _DCC      = 2, 
  _V_FSP1   = 3, 
  _V_FSP0   = 4, 
};

#define DDR_PHY_SS_BASE          SEQ_DDR_SS_DDRPHY_CH0_CA_DDR_PHY_OFFSET

#define DDR_PHY_OFFSET           0x1000      // DDR PHY Address offset (2k Bytes)
#define CA0_DDR_PHY_OFFSET       0x0000
#define DQ0_DDR_PHY_OFFSET       0x1000
#define DQ1_DDR_PHY_OFFSET       0x2000
#define DDR_CC_OFFSET            0x6000

#define REG_OFFSET_DDR_PHY_CH(ch) (DDR_PHY_SS_BASE + (((ch) % 2) * 0x03000) + (((ch) / 2) * 0x10000))
#define REG_OFFSET_DDR_CC_CH(ch)  (DDR_PHY_SS_BASE + (((ch) / 2) * 0x10000))

typedef enum
{
    TRAINING_TYPE_CA_VREF   = 0,
    TRAINING_TYPE_WR_DQDQS  = 1,
    TRAINING_TYPE_RD_DQDQS  = 2,
    TRAINING_TYPE_MAX       = 0x7f
} TRAINING_TYPE;

typedef enum
 {
 	RCW_DIS,
	RCW_ENA  
 }RCW_EN_DIS;
/***************************************************************************************************
 Training Params Struct
 ***************************************************************************************************/
typedef struct
{
   struct
   {
      uint8 max_loopcnt;
   } dcc;

   struct
   {
      uint8 max_loopcnt;
   } dit;

   struct
   {
      uint8 max_loopcnt;
      uint8 cdc_training_enable;
      uint8 vref_start_value;
      uint8 vref_max_value;
      uint8 vref_step;
      uint8 coarse_cdc_start_value;
      uint8 coarse_cdc_max_value;
      uint8 coarse_cdc_step;
      uint8 fine_cdc_max_value;
      uint8 fine_perbit_start_value;
   } ca_vref;

   struct
   {
      uint8 max_loopcnt;
      uint8 max_coarse_cdc;
      uint8 max_fine_cdc;
      uint8 coarse_cdc_step;
      uint8 fine_cdc_step;
      uint8 feedback_percent;
   } wrlvl;

   struct
   {
       uint8  max_loopcnt;
       uint8  max_coarse_cdc;
       uint8  max_fine_cdc;
	   uint8  max_num_cycle;
       uint8  coarse_cdc_step;
       uint8  fine_cdc_step;
   } rcw;

   struct
   {
      uint8 max_loopcnt;
      uint8 coarse_vref_start_mprx;
      uint8 coarse_vref_min_mprx;
      uint8 coarse_vref_max_mprx;
      uint8 coarse_vref_start_hprx;
      uint8 coarse_vref_min_hprx;
      uint8 coarse_vref_max_hprx;
      uint8 vref_start_value;
      uint8 vref_max_value;
      uint8 vref_step;
      uint8 coarse_cdc_start_value;
      uint8 coarse_cdc_max_value;
      uint8 fine_cdc_start_value;
      uint8 fine_cdc_max_value;
      uint8 cdc_step_2D;
   } rd_dqdqs;

   struct
   {
      uint8 max_loopcnt;
      uint8 vref_start_value;
      uint8 vref_max_value;
      uint8 vref_step;
      uint8 coarse_cdc_start_value;
      uint8 coarse_cdc_max_value;
      uint8 fine_cdc_start_value;
      uint8 fine_cdc_max_value;
      uint8 dbi_offset;
      uint8 cdc_step_2D;
   } wr_dqdqs;
   struct
   {	  
	  uint8 g_sed_flag;
	  uint8 rcw_range_enable;
      uint16 min_tDQSCK;
      uint16 max_tDQSCK;
	  uint8 tdqs2dq_range_enable;
      uint16 min_tdqs2dq;
      uint16 max_tdqs2dq;
	  uint8 wrlvl_delta_enable;
	  uint8 max_dqs_cm_dcc_enable;
	  uint8 max_dqs_cm_dcc;
      uint8 max_dqs_io_dcc_enable;
      uint8 max_dqs_io_dcc;
	  uint8 max_ck_cm_dcc_enable;
	  uint8 max_ck_cm_dcc;
      uint8 max_ck_io_dcc_enable;
      uint8 max_ck_io_dcc;
	  uint8 max_rd_dqs_dcc_enable;
      uint8 max_rd_dqs_dcc;
	  uint8 min_rd_eye_width_enable;
      uint8 min_rd_eye_width;
      uint8 min_rd_eye_width_mask_top_enable;
      uint8 min_rd_eye_width_mask_top;
      uint8 min_rd_eye_width_mask_bottom_enable;
      uint8 min_rd_eye_width_mask_bottom;
	  uint8 min_rd_eye_height_HP_enable;
      uint8 min_rd_eye_height_HP;
	  uint8 min_rd_eye_height_MP_enable;
      uint8 min_rd_eye_height_MP;
	  uint8 min_rd_setup_enable;
      uint8 min_rd_setup;
	  uint8 min_rd_hold_enable;
      uint8 min_rd_hold;
	  uint8 min_wr_eye_width_enable;
      uint8 min_wr_eye_width;
      uint8 min_wr_eye_width_mask_top_enable;
      uint8 min_wr_eye_width_mask_top;
      uint8 min_wr_eye_width_mask_bottom_enable;
      uint8 min_wr_eye_width_mask_bottom;
	  uint8 min_wr_eye_height_enable;
      uint8 min_wr_eye_height;
	  uint8 min_wr_setup_enable;
      uint8 min_wr_setup;
	  uint8 min_wr_hold_enable;
      uint8 min_wr_hold;
	  uint8 rx_io_cal_enable;
      uint8 rx_cal_min;
      uint8 rx_cal_max;

   } ddr_abort;
   uint8 PLL_Pre_Cal_En;
   uint8 RX_Cal_En;
   uint8 RCW_Training_En; 
   uint8 wrlvl_pass1_en;
   // Training switchboards
   uint8 Training_pivot_plots_Switchboard[NUM_PRFS_BANDS][2];   // _RD_P,_WR_P
   uint8 DCC_Training_Switchboard[NUM_PRFS_BANDS][1];           // _DCC_Mon
   uint8 WRLVL_Training_Switchboard[NUM_PRFS_BANDS][1];         // _WRLVL_CF
   uint8 WR_Training_Switchboard[NUM_PRFS_BANDS][5];            // _C_CF,_C_PB,_DCC,_V_FSP1,_V_FSP0
   uint8 RD_Training_Switchboard[NUM_PRFS_BANDS][5];            // _C_CF,_C_PB,_DCC,_V_HP,_V_MP

#if TARGET_PHY_MIN_VER >= 0x03020100
   uint8 CA_in_PRFS[NUM_PRFS_BANDS][5];                         // _C_CF,_C_PB,_DCC,_V_HP,_V_MP
#endif

   // Flag indicating small eye abort detected
   uint8 small_eye_abort;
   
   // Low frequency used during training
   uint32 low_freq_in_khz;
} training_params_t;


/***************************************************************************************************
 Training data struct
 This is the organization of ddr->flash_params.training_data struct
 ***************************************************************************************************/
#define DDRSS_SAVE_RESTORE_MAX_DATA_PCH (1344) // 1024 + 256 + 64 bytes per channel
#define DDRSS_SAVE_RESTORE_MAX_DATA (DDRSS_SAVE_RESTORE_MAX_DATA_PCH * NUM_CH)
#define DDRSS_SAVE_RESTORE_MAX_ADDR 512

 struct training_results
{
   struct
   {
     uint16 dit_count[NUM_CH][NUM_DIT_FREQS][NUM_CS][NUM_DIE_PCH]; // Napali: 2 * (4 * 2 * 1) = 16.
     uint16 dit_runtime_count;                      // Napali: 2.
     uint32 mpc_start_to_mrr;
	 uint8  write_training_window_start;
     uint8  write_training_window_start_calculated;
     uint16 dit_fine_step_size[NUM_CH][NUM_DIT_FREQS][NUM_CS][NUM_DIE_PCH];
     uint8  dit_loop_count;                         // Napali: 1. 
     uint8  dit_acq_ndx;                           // Napali: 2. 
     uint8  dit_trac_prfs_ndx;                     // Napali: 2.
     uint8  dit_dual_tracking;
     uint32 tdqs2dq_in_ps[NUM_CH][NUM_CS][NUM_DQ_PCH];// Napali: 4 * 2 * 2* 4 = 64
   //  uint8  wr_half_cycle_start [MAX_TRAINING_FREQ_NUM][NUM_CH][NUM_CS][NUM_DQ_PCH]; // Napali: (5 *4 * 2 * 2)  bytes = 80 bytes
   //  uint8  wr_coarse_cdc_start [MAX_TRAINING_FREQ_NUM][NUM_CH][NUM_CS][NUM_DQ_PCH]; // Napali: (5 *4 * 2 * 2)  bytes = 80 bytes
   } dit; // Napali: Total = 96.
   
   struct
   {
     uint32 rd_opcode_offset[MAX_NUM_CLOCK_PLAN][NUM_CS];
   } rcw;
   
   struct
   {
     uint8 wr_dram_vref[NUM_CH][NUM_CS][NUM_DIE_PCH][NUM_FSP];
   } wr_dram;
   
   uint8 pll_precal_completed; // flag indicating PLL pre-calibration has been completed
   
   uint32 save_restore_data[DDRSS_SAVE_RESTORE_MAX_DATA / sizeof(uint32)];
   
}; // Napali: Sum Total = old: 4751 new: 4896

typedef struct
{
   struct training_results results; // Napali : Total = 4992 uint8s.
} training_data;

void DDRSS_set_training_params (training_params_t *training_params_ptr);

/***************************************************************************************************
 CA Vref training
 ***************************************************************************************************/

extern uint8 ca_training_pattern_lpddr3[CA_PATTERN_NUM][6];
extern uint8 ca_training_pattern_lpddr4[CA_PATTERN_NUM][3];

typedef struct {
  uint8 vref_fail_count_table [NUM_CA_PCH][80];  // 80B
} ddrss_ca_vref_local_vars; //Napali total size: 80B.


/***************************************************************************************************
 Write Leveling
 ***************************************************************************************************/
// Structure for wrlvl convert routine
typedef struct {
  uint32 coarse_dqs_delay;
  uint32 fine_dqs_delay;
  uint32 dqs_half_cycle;
} wrlvl_params_struct;

/**************************************************************************************************
RCW training local Vars
****************************************************************************************************/
//This Structure contains the local variables for RCW training used by Training_DDRSS_RCW() function and its callees.

typedef struct
{
	uint8 rcw_num_cycles[NUM_CH][NUM_CS][NUM_DQ_PCH] ; // 4 * 2 * 2 = 16 bytes
	uint8 rcw_coarse_cdc[NUM_CH][NUM_CS][NUM_DQ_PCH];   // 4 * 2 * 2 = 16 bytes
	uint8 rcw_fine_cdc[NUM_CH][NUM_CS][NUM_DQ_PCH];     // 4 * 2 * 2 = 16 bytes
}ddrss_rcw_local_vars; // 48 bytes


/***************************************************************************************************
 Write and Read training
 ***************************************************************************************************/
// Structure containing what would otherwise be local variables used by the ddrss_rd_dqdqs(), ddrss_wr_dqdqs() functions and its callees.
typedef struct {
    struct 
    {
      uint8 coarse_dq_passband_info[NUM_DQ_PCH][COARSE_VREF][COARSE_CDC];        // 2 * 57 * 25    = 2850 bytes
      uint8 coarse_dq_full_half_cdc[NUM_DQ_PCH][NUM_FULL][NUM_HALF][COARSE_CDC]; // 2 * 3 * 2 * 25 =  300 bytes
    } coarse_schmoo;
    
//    uint32 fail_hist_2d [MAXVREF][MAXCDC];                                      // 4 * 80 * 24 * 6     = 46080 bytes
    uint8            dq_dcc_adj   [NUM_DQ_PCH][NUM_CS];                         // 2 * 2               =     4 bytes
    uint8            dqsdcc_adj   [NUM_DQ_PCH][NUM_CS];                         // 2 * 2               =     4 bytes
    uint8  failcount_histogram [3][NUM_DQ_PCH][PINS_PER_PHY][HISTOGRAM_SIZE];   // 3 * 2 * 10 * 24 * 6 =  8640 bytes
    uint8             pass_reg [3][NUM_DQ_PCH][PINS_PER_PHY][4];                // 3 * 2 * 10 * 4      =   240 bytes
    uint8        vref_pass_reg [3][NUM_DQ_PCH][NUM_CS][PINS_PER_PHY][4];        // 3 * 2 *  2 * 10 * 4 =   480 bytes
    uint32             cdc_ref                              [HISTOGRAM_SIZE][4];// 4 * 24 * 6 * 4      =  2304 bytes
    uint8           dq_half_cycle    [NUM_DQ_PCH];                              // 2                   =     2 bytes
    uint8           dq_full_cycle    [NUM_DQ_PCH];                              // 2                   =     2 bytes

} ddrss_rdwr_dqdqs_local_vars; // 60906 bytes with fail_hist_2d, 14826 without fail_hist_2d

typedef union  {
	uint32 w;						//32 bit whole
	uint8 b [PHASE][NUM_DQ_PCH];   //8bit for each dq and each phase
	uint16 p[PHASE];				//16bit for each phase
} FAILCELL;

typedef struct {
	uint8 dbi_flag;  // Coarse vref of SMALLER vref CA
	uint8 dq_pin_num_max;    // Difference value of two CA vref
	uint8 *connected_bit_mapping_A;
} dbi_struct;

// Disables PLL band select calibration during frequency switch
void DDR_PHY_Disable_Band_Sel_Cal (void);
void DDR_PHY_DDRCC_PLL_Lock_Workaround (void);


#endif/* __PHY_COMMON_H__ */
