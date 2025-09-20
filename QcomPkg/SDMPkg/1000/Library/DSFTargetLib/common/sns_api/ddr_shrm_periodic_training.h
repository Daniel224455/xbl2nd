/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
#ifndef DDR_SHRM_PERIODIC_TRAINING_H
#define DDR_SHRM_PERIODIC_TRAINING_H

#include "phy_common.h"
#include "ddr_phy_seq_hwioreg.h"



#define DIT_ACQUIRE_MODE   0x1
#define DIT_TRACKING_MODE  0x2
#define DIT_SCALING_MODE   0x3

#define DIT_INTERRUPT_MASK  0xF
#define DIT_INTERRUPT_MASK_SDM1000  0xFF

#define DIT_NORMAL_DELTA                20
#define DIT_FINE_STEP_LIMIT             (FINE_STEPS_PER_COARSE_DIT - 1) //6-1=5
#define DIT_INTERVAL_PER_SEC            4
#define DIT_CNT_1SEC                    32000
#define DIT_WATCHDOG                    16
#define DIT_MAX_FINE_STEPS_FROM_PREV    1

#define DIT_NUM_SLABS                   (FINE_STEPS_PER_COARSE_DIT * 2) //12

#define DIT_NUM_SAMPLES_ACQ             8
#define DIT_NUM_SAMPLES_TRAC            8

#define DIT_DIE1_BIT_SHIFT              8


#define DSF_PERIODIC_TRAINING_FORCE_UPDATE_EN   0
#define DSF_PERIODIC_TRAINING_ALGO_DEBUG_EN     0


typedef struct
{
    uint8  dit_acq_ndx;
    uint8  dit_trac_prfs_ndx;
    uint8  dit_dual_tracking;
    uint8  write_training_window_start;
    uint32 dit_runtime_count;
    uint32 mpc_start_to_mrr;

} SHRM_BOOT_DIT_STRUCT;


typedef struct
{
    SHRM_BOOT_DIT_STRUCT boot_dit;
    uint16 osc_slab [NUM_CH][NUM_DIT_FREQS][NUM_CS][NUM_DIE_PCH][DIT_NUM_SLABS];
    uint8 boot_cdc_fine [NUM_CH][NUM_DIT_FREQS-1][NUM_CS][NUM_DQ_PCH];
    uint8 prev_cdc_fine_step [NUM_CH][NUM_CS][NUM_DIE_PCH];
    
#if DSF_PERIODIC_TRAINING_ALGO_DEBUG_EN
    uint8 ch_to_be_serviced;
    uint8 acq_trac_mode;
    uint8 shrm_execution_flag;
    uint32 dit_die_count_acc[NUM_CH][NUM_CS][NUM_DIE_PCH];
	uint16 dit_die_count_min[NUM_CH][NUM_CS][NUM_DIE_PCH];
	uint16 dit_die_count_max[NUM_CH][NUM_CS][NUM_DIE_PCH];
    uint16 osc_avg[NUM_CS][NUM_DIE_PCH];
#endif
} SHRM_PERIODIC_TRAINING_DATA_STRUCT;

extern SHRM_PERIODIC_TRAINING_DATA_STRUCT *shrm_periodic_training_data_ptr;

#endif
