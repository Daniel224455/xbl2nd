/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

#ifndef __PHY_TARGET_H__
#define __PHY_TARGET_H__

#include "ddr_ss_seq_hwiobase.h"
#include "ddr_ss_seq_hwioreg.h"

#define VREF_MP_HP                  2
#define PINS_PER_PHY_CONNECTED_CA   6
#define NUM_CA_PHY_BIT             10

#define DSF_LPDDR4Y_SUPPORTED       1    //project specific

#define CA_PATTERN_NUM              4 

#define COARSE_VREF_MAX_VALUE       0x37
#define COARSE_VREF_MAX_VALUE_WRITE 0x32 //Limit VREF to max number of possible steps.
#define COARSE_CDC_MAX_VALUE        0x18
#define MIN_WIDTH                   0x1
#define COARSE_VREF                 (COARSE_VREF_MAX_VALUE + 1)
#define COARSE_CDC                  (COARSE_CDC_MAX_VALUE + 1)

#define FINE_VREF_MAX_VALUE         0x8 
#define FINE_CDC_MAX_VALUE          0x8
#define FINE_VREF                   (FINE_VREF_MAX_VALUE + 1)
#define FINE_CDC                    (FINE_CDC_MAX_VALUE + 1)
#define FINE_RD_CDC                 0x10
#define PERBIT_CDC_MAX_VALUE        0xF 
#define DBI_WR_PBIT_OFFSET          0

#define COARSE_STEP_IN_PS           60 // 62ps at high voltage from Hana PTE data
#define FINE_STEP_IN_PS             8  // 7.7ps at high voltage from Hana PTE data
#define FINE_STEPS_PER_COARSE_DIT   6  // changed for periodic training purposes, was 7
#define FINE_STEPS_PER_COARSE       8  // RD:0-7, WR:4-11
#define TRAIN_WR_CDC_FINE_STEPS_OFFSET 4

#define WRITE_FINE_CDC_MIN          5
#define WRITE_FINE_CDC_MAX          10
#define WRITE_TERMINATED_MR14_VREF  0x19
#define WRITE_UNTERMINATED_MR14_VREF 0x4B
#define MAX_TDQS2DQ_IN_PS           800
#define DCC_ADJ_RANGE               12

#define VREF_RD_MP_STEP_IN_MV 9 
#define VREF_RD_MP_OFFSET_IN_MV 40 
#define VREF_RD_HP_STEP_IN_MV 21 
#define VREF_RD_HP_OFFSET_IN_MV 55 
#define VREF_WR_LP4x_STEP_IN_MV (3.6) // LP4x DRAM side: (0.6% * VDDQ) 
#define VREF_RANGE_THRESHOLD      30
#define WR_RANGE0_OFFSET_MV          90 // 15% first value // only LP4x  need to add also LP4
#define WR_RANGE1_OFFSET_MV          197    
#define HPVREF_OFFSET_MV          66

//#define MIN_DTTS_TRACKING_PRFS      0x6

#define LOW_SPEED_HALF_CYC_CONV_THRESHOLD       500  //((COARSE_CDC_MAX_VALUE * COARSE_STEP_IN_PS)/2) //=600ps.
#define LOW_SPEED_NO_HALF_CYC_COARSE_CDC_VALUE  0x18 //0x12 //0x14 //0x10
#define LOW_SPEED_NO_HALF_CYC_FINE_CDC_VALUE    0x0
#define LOW_SPEED_RD_COARSE_CDC_VALUE           0xA

#define VREF_STEPS                80
#define HISTOGRAM_SIZE            (COARSE_CDC_MAX_VALUE * FINE_STEPS_PER_COARSE)
#define MAXCDC                    (COARSE_CDC_MAX_VALUE * FINE_STEPS_PER_COARSE)
#define MAXVREF            		  80
#define COARSE_L                   0
#define FINE_L                     1
#define DELAY_L                    2
#define CDC_L                      3

#define  STRT 0
#define  STOP 1
#define  MIDP 2
#define  EYEW 3
#define  ACPT 4
#define  NUM_PR_PARAMS 5
//#define MAX_BUS_WIDTH 32
#define BYTE_SIZE     8
#define ALL_DQ_FAIL_PAT      0xFFFFFFFF
#define ALL_DQ_PASS_PAT      0x00000000
#define DQ_TRAINING_PATTERN_SIZE(CH_MASK) ((CH_MASK == 0xFF)? 64 : 128)  //Channel Specific. Depends on interleaving size . 64*4 = 256 interleaving size for poipu 
#define NUM_SCALE_BANDS           1
                                              
#define SCALE_FREQ(freq)          ((freq==2) ? (768000) \
                                             : (547200))       

#define  MASTER_IOCAL(CH)     	   ( (CH==0) || (CH==2) || (CH==5) || (CH==7)) ? 1 : 0 //This needs to be programed PER-PROJECT
											 
#define HIGHEST_TRAINED_FPRS_FREQ F_RANGE_7

#define ODT_FPRS_THRESHOLD_FREQ   F_RANGE_2
#define HIGHEST_MPRX_FPRS_FREQ    F_RANGE_4
#define HIGHEST_MPVREF_PRFS_FREQ  F_RANGE_2  
#define HIGHEST_GCC_PLL_FREQ      F_RANGE_1

#define RCW_THRESHOLD_FREQ        F_RANGE_3
#define LOWEST_PRFS_FREQ          F_RANGE_0//lowest PRFS threshold

#define HIGHEST_TRAINING_TABLE_INDX      7//4 
#define RCW_TRAINING_TABLE_START_INDX    3//1 

#define LOWEST_TRAINED_FPRS_FREQ  F_RANGE_2
#define SCALED_FREQ_EN  		   0
#define HIGHEST_RANK_EN_FREQ      F_RANGE_4 

#define LOWEST_SCALED_FPRS_FREQ   F_RANGE_1
#define LOWEST_FPRS_FREQ          F_RANGE_0 

#define LOWEST_DCC_COPY_PRFS_DRRCC   2
#define LOWEST_WRLVL_COPY_PRFS       2

#define NUM_DQ_TRAINING_STACK        120
#define NUM_CA_TRAINING_STACK        100

#define MRW_BIST_EN 1
#define MRW_MC_EN 0

#define BROADCAST_CH_NUM	       8     //channel specific 

 
#define NUM_CA_BITS                 6
#define NUM_CA_PHY_BIT             10
#define NUM_DQ_PHY_BIT             10
#define BYTE_SIZE                   8
#define READ_PERBIT_INIT            0xF
#define PHYBIT_NC                   0xFF

#define DDR_MAX_NUM_CH  8 					 //channel specific 
#define DDR_CH_ALL      0xFF				 //channel specific 
#define NUM_CH                    8 // Number of DDR channels				 //channel specific 
#define NUM_DIT_FREQS             3

#define DQ_PER_BYTE               8 // Number of bits per BYTE

#define DIT_INTERRUPT_SHIFT 28												 //channel specific 
#define DIT_INTERRUPT_SHIFT_2 24											 //channel specific 

#define NUM_PRFS_BANDS            8
#define TRAINING_THRESHOLD        533000 

#define TRAINING_START_PRFS       3
#define FSP_THRESHOLD_PRFS_BAND   3
#define MAX_TRAINING_FREQ_NUM     8
#define MAX_TRAINING_FREQ_INDEX   7

#define BOOT_DIT_FACTORY_FLAG       0xDEADCAFE

#define RX_DCC                      10
#define PHASE                       2
#define PHASES                      3
#define CLK_DCC_OFFSET_RANGE        7     //2*3(range) + 1

#define ODT_FSP_EN_FREQ_THRESHOLD 1000000 //Match with settings in bimc_data.c
#define LOWEST_PRFS_FREQ          F_RANGE_0//lowest PRFS threshold
#define HIGHEST_PRFS_FREQ         F_RANGE_7 //Highest PRFS threshold

#define FEATURE_DISABLE            0
#define FEATURE_ENABLE             1
#define DCC_TRAINING_WRLVL_WR90_IO 7
#define DCC_TRAINING_WRLVL_WR90    3
#define DCC_TRAINING_WR90          2
#define DCC_TRAINING_WRLVL         1
#define DCC_TRAINING_WR90_IO       6

#define FSP_OP_BOOT               0x0
#define FSP_OP_HIGH               0x1

#define MPVREF                    0
#define HPVREF                    1

#define MR23_data                 0x3F

#define IO_QUAL_OFFSET_DOUBLE_STEP_SIZE 0x5
#define LOCAL_VREF_MID_VALUE            0x04
#define LOCAL_VREF_MAX_VALUE            0x07

#define LP_MODE                   0
#define HP_MODE                   1
#define SWEEP_LEFT                0
#define SWEEP_RIGHT               1

#define DCC_TRAINING_SEL          7         // Selecting all three DCC training flows
#define MR12_VREF_RANGE_BIT       0         // MR12 OP6 -> 0: Vref Range 0; 1: MR12 Vref Range 1
#define VREF_WR_LP4_MR14_RANGE_BIT_N 6
#define VREF_WR_LP4_MR14_VALUE_MASK 0x3F
#define VREF_WR_LP4_MAX_VALUE 50

#define DIR_RD 0
#define DIR_WR 1

// PHY Power Collapse Broadcast for dual channel devices
#define PC_PHY_CA_DQ_BROADCAST    0x1FBF
#define PC_CC_BROADCAST           0x2040

#define PINS_PER_PHY                    10
#define PINS_PER_PHY_CONNECTED_NO_DBI    8
#define PINS_PER_PHY_CONNECTED_WITH_DBI  9
#define NUM_FULL                  3
#define NUM_HALF                  2

#define DDR_SS_BASE                 0x09000000
// The flat 32-bit system address from APPS viewpoint where PHY training code will write
// training patterns to and read back from. Training addresses currently are hard-coded to assume
// that System Address Bit 10 is the interleave bit, hence the address for channel 1 is 0x00000400.
#define TRAINING_BASE_ADDRESS_OFFSET        0x10000000   
#define TRAINING_INTERLEAVE_ADDRESS_OFFSET 0x00000100  //256Bytes interleaving    //channel specific 
#define MIN_EYE_WIDTH    100
#define MAX_SKEW_RANGE   1200

#define DSF_MIN_SUPPORTED_DDR_TRAINING_FREQ     547200

#define RX_CAL_EN                                    1
#define DSF_MIN_PERIODIC_TRAINING_FREQ          1804800
#define MAX_UNTERMINATED_FREQ					 768000
#define WRLVL_PASS1_FREQ                        1017600
#define WRLVL_PASS1_PRFS_INDEX                       3

#define DSF_PERIODIC_TRAINING_EN                     1
#define DSF_OTA_TRAINING_EN                          0
#define DSF_PXI_TRAINING_EN                          0

#define DSF_PLLPRECAL_TRAINING_LOG_IN                0
#define DSF_CA_VREF_TRAINING_EN                      0
#define DSF_WRLVL_TRAINING_EN                        1
#define DSF_WRLVL_TRAINING_LOG                       1
#define DSF_RCW_TRAINING_EN                          1
#define DSF_RCW_TRAINING_LOG                         1

#define DSF_DCC_TRAINING_DQ_DCC_LOG                  0    
#define DSF_DCC_TRAINING_CA_DCC_LOG                  0 

#define DC 										 0xFFFFFFFF
#define DSF_TRAINING_LOG_EN                          1   
#define DSF_RD_DQDQS_TRAINING_EN                     1
#define DSF_RD_DQDQS_TRAINING_LOG_IN                 1 & DSF_RD_DQDQS_TRAINING_EN & DSF_TRAINING_LOG_EN
#define DSF_RD_DQDQS_TRAINING_LOG_HIST_IN            0 & DSF_RD_DQDQS_TRAINING_EN & DSF_TRAINING_LOG_EN
#define DSF_RD_DQDQS_TRAINING_LOG_BEFORE             0 & DSF_RD_DQDQS_TRAINING_EN & DSF_TRAINING_LOG_EN
#define DSF_RD_DQDQS_TRAINING_LOG_AFTER              0 & DSF_RD_DQDQS_TRAINING_EN & DSF_TRAINING_LOG_EN

#define DSF_RD_DQDQS_COARSE_FINE_PLOT_PIVOT          0  
#define DSF_RD_DQDQS_COARSE_FINE_PLOT_FREQ           2092800

#define DSF_RD_DQDCC_EN                              0                     
#define DSF_RD_HPVREF_TRAINING_EN                    1
#define DSF_RD_HPVREF_TRAINING_LOG_IN                1 & DSF_RD_HPVREF_TRAINING_EN  & DSF_TRAINING_LOG_EN                                  
#define DSF_RD_HPVREF_TRAINING_LOG_HIST_IN	         0 & DSF_RD_HPVREF_TRAINING_EN  & DSF_TRAINING_LOG_EN & DSF_RD_HPVREF_TRAINING_LOG_IN  
#define DSF_RD_HPVREF_TRAINING_LOG_BEFORE            0 & DSF_RD_HPVREF_TRAINING_EN  & DSF_TRAINING_LOG_EN                                  
#define DSF_RD_HPVREF_TRAINING_LOG_AFTER             0 & DSF_RD_HPVREF_TRAINING_EN  & DSF_TRAINING_LOG_EN                                  

#define DSF_RD_MPVREF_TRAINING_EN                    1
#define DSF_RD_MPVREF_TRAINING_LOG_IN                0 & DSF_RD_MPVREF_TRAINING_EN & DSF_TRAINING_LOG_EN                                   
#define DSF_RD_MPVREF_TRAINING_LOG_HIST_IN           0 & DSF_RD_MPVREF_TRAINING_EN & DSF_TRAINING_LOG_EN  & DSF_RD_MPVREF_TRAINING_LOG_IN  
                                                                                                          
#define DSF_RD_DQDQS_EYE_MASK                        0 //1                                                
#define DSF_WR_DQDQS_TRAINING_EN                     1                                                    
#define DSF_WR_DQDQS_TRAINING_LOG_IN                 1  & DSF_WR_DQDQS_TRAINING_EN & DSF_TRAINING_LOG_EN                                   
#define DSF_WR_DQDQS_TRAINING_LOG_HIST_IN            0  & DSF_WR_DQDQS_TRAINING_EN & DSF_TRAINING_LOG_EN  & DSF_WR_DQDQS_TRAINING_LOG_IN   
#define DSF_WR_DQDQS_TRAINING_LOG_AFTER              0  & DSF_WR_DQDQS_TRAINING_EN & DSF_TRAINING_LOG_EN                                   
#define DSF_WR_DQDQS_TRAINING_LOG_BEFORE             0  & DSF_WR_DQDQS_TRAINING_EN & DSF_TRAINING_LOG_EN                                   

#define DSF_WR_DQDQS_COARSE_FINE_PLOT_PIVOT          0  // Warning: The Pivot plot takes ~5 min
#define DSF_WR_DQDQS_COARSE_FINE_PLOT_FREQ           2092800 

#define DSF_WR_DQDQS_TRAINING_TDQS2DQ_CAL_EN         1    //WR dqdqs training for band0.
#define DSF_WR_DQDQS_TRAINING_TDQS2DQ_CAL_LOG        0  
#define DSF_WR_DQDQS_EYE_MASK                        0 //1
														
#define BROADCAST_BASE SEQ_DDR_SS_DDRSS_AHB2PHY_BROADCAST_AHB2PHY_BROADCAST_SWMAN1_OFFSET
//DDRSS_AHB2PHY0_AHB2PHY_BROADCAST_EN_CFG_LOWER
//BROADCAST MASK0
// ----------------------------------------------------------------------
// |CH_01 CC|     CH1     |      CH0     |
// ----------------------------------------------------------------------
//     { CC}| {DQ1|DQ0|CA}| {DQ1|DQ0| CA}|
// ----------------------------------------------------------------------
//     {  6}|{  5|  4 | 3}| {  2|  1|  0}|

//DDRSS_AHB2PHY1_AHB2PHY_BROADCAST_EN_CFG_LOWER
//BROADCAST MASK1
// ----------------------------------------------------------------------
// |CH_23 CC|     CH3     |      CH2     |
// ----------------------------------------------------------------------
//     { CC}| {DQ1|DQ0|CA}| {DQ1|DQ0| CA}|
// ----------------------------------------------------------------------
//     {  6}|{  5|  4 | 3}| {  2|  1|  0}|

#define PHY_BC_DISABLE         0x0000

#define DQ_DBI_BIT     0xFF
#define DQ_UNCONNECTED 0x1F
#define CA_UNCONNECTED 0x1F

//Channel 0                 
#define CS_CH0_DDRPHY0_CA0      0x0001 
#define CS_CH0_DDRPHY0_DQ0      0x0002 
#define CS_CH0_DDRPHY0_DQ1      0x0004 

//Channel 1                               
#define CS_CH1_DDRPHY0_CA0      0x0008  
#define CS_CH1_DDRPHY0_DQ0      0x0010  
#define CS_CH1_DDRPHY0_DQ1      0x0020  

#define CS_CH01_DDRCC          0x0040  

//Channel 2                 
#define CS_CH2_DDRPHY1_CA0      0x0001 
#define CS_CH2_DDRPHY1_DQ0      0x0002 
#define CS_CH2_DDRPHY1_DQ1      0x0004 

//Channel 3                 
#define CS_CH3_DDRPHY1_CA0      0x0008 
#define CS_CH3_DDRPHY1_DQ0      0x0010 
#define CS_CH3_DDRPHY1_DQ1      0x0020 

#define CS_CH23_DDRCC          0x0040 

//Channel 4                 
#define CS_CH4_DDRPHY2_CA0      0x0001 
#define CS_CH4_DDRPHY2_DQ0      0x0002 
#define CS_CH4_DDRPHY2_DQ1      0x0004 

//Channel 5                               
#define CS_CH5_DDRPHY2_CA0      0x0008  
#define CS_CH5_DDRPHY2_DQ0      0x0010  
#define CS_CH5_DDRPHY2_DQ1      0x0020  

#define CS_CH45_DDRCC          0x0040  

//Channel 6                 
#define CS_CH6_DDRPHY3_CA0      0x0001 
#define CS_CH6_DDRPHY3_DQ0      0x0002 
#define CS_CH6_DDRPHY3_DQ1      0x0004 

//Channel 7                 
#define CS_CH7_DDRPHY3_CA0      0x0008 
#define CS_CH7_DDRPHY3_DQ0      0x0010 
#define CS_CH7_DDRPHY3_DQ1      0x0020 

#define CS_CH67_DDRCC          0x0040 


//All Channels

#define ALL_PHY0_CAs    (CS_CH0_DDRPHY0_CA0 | CS_CH1_DDRPHY0_CA0)

#define ALL_PHY1_CAs    (CS_CH2_DDRPHY1_CA0 | CS_CH3_DDRPHY1_CA0)                     
                    
#define ALL_PHY2_CAs    (CS_CH4_DDRPHY2_CA0 | CS_CH5_DDRPHY2_CA0)

#define ALL_PHY3_CAs    (CS_CH6_DDRPHY3_CA0 | CS_CH7_DDRPHY3_CA0)                     
                    
#define ALL_PHY0_DQs    (CS_CH0_DDRPHY0_DQ0 | CS_CH0_DDRPHY0_DQ1 | CS_CH1_DDRPHY0_DQ0 | CS_CH1_DDRPHY0_DQ1 )  

#define ALL_PHY1_DQs    (CS_CH2_DDRPHY1_DQ0 | CS_CH2_DDRPHY1_DQ1 | CS_CH3_DDRPHY1_DQ0 | CS_CH3_DDRPHY1_DQ1 )
                    
#define ALL_PHY2_DQs    (CS_CH4_DDRPHY2_DQ0 | CS_CH4_DDRPHY2_DQ1 | CS_CH5_DDRPHY2_DQ0 | CS_CH5_DDRPHY2_DQ1 )  

#define ALL_PHY3_DQs    (CS_CH6_DDRPHY3_DQ0 | CS_CH6_DDRPHY3_DQ1 | CS_CH7_DDRPHY3_DQ0 | CS_CH7_DDRPHY3_DQ1 )
                    
#define ALL_PHY0_CCs    (CS_CH01_DDRCC )

#define ALL_PHY1_CCs    (CS_CH23_DDRCC )

#define ALL_PHY2_CCs    (CS_CH45_DDRCC )

#define ALL_PHY3_CCs    (CS_CH67_DDRCC )


#define ALL_CAsDQs  (CS_CH0_DDRPHY0_CA0 | CS_CH1_DDRPHY0_CA0 | \
                     CS_CH2_DDRPHY1_CA0 | CS_CH3_DDRPHY1_CA0 | \
					 CS_CH4_DDRPHY2_CA0 | CS_CH5_DDRPHY2_CA0 | \
					 CS_CH6_DDRPHY3_CA0 | CS_CH7_DDRPHY3_CA0 | \
                     CS_CH0_DDRPHY0_DQ0 | CS_CH0_DDRPHY0_DQ1 | CS_CH1_DDRPHY0_DQ0 | CS_CH1_DDRPHY0_DQ1 | \
                     CS_CH2_DDRPHY1_DQ0 | CS_CH2_DDRPHY1_DQ1 | CS_CH3_DDRPHY1_DQ0 | CS_CH3_DDRPHY1_DQ1 | \
					 CS_CH4_DDRPHY2_DQ0 | CS_CH4_DDRPHY2_DQ1 | CS_CH5_DDRPHY2_DQ0 | CS_CH5_DDRPHY2_DQ1 | \
					 CS_CH6_DDRPHY3_DQ0 | CS_CH6_DDRPHY3_DQ1 | CS_CH7_DDRPHY3_DQ0 | CS_CH7_DDRPHY3_DQ1  )  

#define ALL_PHY0_CAsDQs  (CS_CH0_DDRPHY0_CA0 | CS_CH1_DDRPHY0_CA0 | \
                          CS_CH0_DDRPHY0_DQ0 | CS_CH0_DDRPHY0_DQ1 | CS_CH1_DDRPHY0_DQ0 | CS_CH1_DDRPHY0_DQ1)  

#define ALL_PHY1_CAsDQs  (CS_CH2_DDRPHY1_CA0 | CS_CH3_DDRPHY1_CA0 | \
                          CS_CH2_DDRPHY1_DQ0 | CS_CH2_DDRPHY1_DQ1 | CS_CH3_DDRPHY1_DQ0 | CS_CH3_DDRPHY1_DQ1 )    
						  
#define ALL_PHY2_CAsDQs  (CS_CH4_DDRPHY2_CA0 | CS_CH5_DDRPHY2_CA0 | \
                          CS_CH4_DDRPHY2_DQ0 | CS_CH4_DDRPHY2_DQ1 | CS_CH5_DDRPHY2_DQ0 | CS_CH5_DDRPHY2_DQ1)  

#define ALL_PHY3_CAsDQs  (CS_CH6_DDRPHY3_CA0 | CS_CH7_DDRPHY3_CA0 | \
                          CS_CH6_DDRPHY3_DQ0 | CS_CH6_DDRPHY3_DQ1 | CS_CH7_DDRPHY3_DQ0 | CS_CH7_DDRPHY3_DQ1 )   
                      






#define F_RANGE_0                   250000
#define F_RANGE_1                   600000
#define F_RANGE_2                   900000
#define F_RANGE_3                   1100000
#define F_RANGE_4                   1500000
#define F_RANGE_5                   1700000
#define F_RANGE_6                   2000000
#define F_RANGE_7                   2200000

typedef enum 
{
  DQ = (uint8) 2,
  CA = (uint8) 1
} PHY_BUS;

static inline uint8 DDRSS_Channel_Mask(void)
{
	uint8 mask = 0xFF;
	if (HWIO_INXF (SEQ_DDR_SS_DDR_LAGG_DDR_SS_LAGG_OFFSET, DDR_SS_LAGG_LLCC_INFO_0, LLCC_NUM_CTRLRS) == 4)
    {
       mask = 0xF0;
    } 
	return mask;
}

static inline void DDRSS_PHY_Broadcast_DQs_All_Enable()
{
	// Enable broadcast mode for all DQ PHYs on both channels
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY0_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY0_DQs);
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY1_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY1_DQs);
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY2_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY2_DQs);
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY3_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY3_DQs);	
}

static inline void DDRSS_PHY_Broadcast_CAs_All_Enable()
{
 // Enable broadcast mode for all CA PHYs on both channels
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY0_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY0_CAs);
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY1_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY1_CAs);
	HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY2_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY2_CAs); 
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY3_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY3_CAs); 
}

static inline void DDRSS_PHY_Broadcast_CCs_All_Enable()
{
  // Enable broadcast mode for all CCs on both channels
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY0_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY0_CCs);
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY1_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY1_CCs);
	HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY2_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY2_CCs); 
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY3_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY3_CCs);
}
static inline void DDRSS_PHY_Broadcast_CAsDQs_All_Enable()
{
// Enable broadcast mode for all DQ and CA PHYs on both channels
     HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY0_AHB2PHY_SWMAN_OFFSET,
               AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY0_CAsDQs);
     HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY1_AHB2PHY_SWMAN_OFFSET,
               AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY1_CAsDQs);      
	 HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY2_AHB2PHY_SWMAN_OFFSET,
               AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY2_CAsDQs);
     HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY3_AHB2PHY_SWMAN_OFFSET,
               AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, ALL_PHY3_CAsDQs);  
}	
static inline void DDRSS_PHY_Broadcast_All_Disable()
{
    // Disable broadcast mode 
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY0_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, PHY_BC_DISABLE);
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY1_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, PHY_BC_DISABLE);
 	HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY2_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, PHY_BC_DISABLE); 
    HWIO_OUTX(SEQ_DDR_SS_DDRSS_AHB2PHY3_AHB2PHY_SWMAN_OFFSET,
        AHB2PHY_SWMAN_AHB2PHY_BROADCAST_EN_CFG_LOWER, PHY_BC_DISABLE);
      
}

#endif // __PHY_TARGET_H__
