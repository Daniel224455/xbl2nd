/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

#include "ddrss.h"
#include "ddrss_mccc.h"
#include "phy_common.h"
#include "mc.h"
#include "target_config.h"
#include "ddr_shrm_periodic_training.h"
#include "mc_pxi.h"

  uint32  training_prfs_table[MAX_TRAINING_FREQ_NUM]={(NUM_PRFS_BANDS-1),(NUM_PRFS_BANDS-2),(NUM_PRFS_BANDS-3),(NUM_PRFS_BANDS-4),(NUM_PRFS_BANDS-5),(NUM_PRFS_BANDS-6),(NUM_PRFS_BANDS-7),(NUM_PRFS_BANDS-8)};
  uint32  training_freq_idx[MAX_TRAINING_FREQ_NUM]={0,1,2,3,4,5,6,7};
  uint32 PRFS_BAND_THRESHOLD[NUM_PRFS_BANDS]  = {F_RANGE_0,  F_RANGE_1,    F_RANGE_2,    F_RANGE_3,    F_RANGE_4,     F_RANGE_5,    F_RANGE_6,    F_RANGE_7 }; 
  uint8 g_channelMask =0xFF;

// Numbers used in calculating PLL fractional-divider ratios
#define TWO_TO_THE_18TH  ((double) 262144.0)  // Like it says on the box.
#define VCO_MIN_IN_KHZ 1000000

// =============================================================================
// Bit-byte remapping per the bump map. 
// =============================================================================


uint8  bit_remapping_phy2bimc_DQ[NUM_CH][NUM_DQ_PCH][PINS_PER_PHY] = {
       {//channel 0
         //0  1  2  3  4  5  6  7  8
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte0
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte1
       } ,
       {//channel 1
         //0  1  2  3  4  5  6  7  8
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte0
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte1
       } ,
       {//channel 2
         //0  1  2  3  4  5  6  7  8
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte1
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte0
       } ,
       {//channel 3
         //0  1  2  3  4  5  6  7  8
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte0
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED } //byte1
       } ,
	   {//channel 4	
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte0
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte1
       } ,
       {//channel 5
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte0
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte1
       } ,
       {//channel 6
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte0
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte1
       } ,
       {//channel 7
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED }, //byte0
         { 3, 1, 2, 0, 4, 6, 7, 5, DQ_DBI_BIT, DQ_UNCONNECTED } //byte1
       }        
      };
//                                       2          4           8
uint8  bit_remapping_bimc2phy_DQ[NUM_CH][NUM_DQ_PCH][DQ_PER_BYTE ] = {
       {//channel 0
	   //  0  1  2  3  4  5  6  7
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte0
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte1
       },
       {//channel 1
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte0
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte1
       },
       {//channel 2
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte0
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte1
       },                         
       {//channel 3               
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte0
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte1
       } ,
       {//channel 0
       //  0  1  2  3  4  5  6  7
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte0
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte1
       },
       {//channel 1
       //  0  1  2  3  4  5  6  7
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte0
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte1
       },
       {//channel 2
       //  0  1  2  3  4  5  6  7
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte0
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte1
       },                         
       {//channel 3               
       //  0  1  2  3  4  5  6  7 
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte0
         { 3, 1, 2, 0, 4, 7, 5, 6}, //byte1
       }
       };

uint8  bit_remapping_bimc2phy_CA[NUM_CH][NUM_CA_PHY_BIT]  = {
      //channel 0
      //0  1  2  3  4  5  6  7    8         9
      { 4, 7, 0, 2, 1, 3, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED}, //ch0, bit

      //channel 1
      //0  1  2  3  4  5  6  7
      { 4, 7, 0, 2, 1, 3, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED}, //ch1, bit
      
	  //channel 2
      //0  1  2  3  4  5  6  7
      { 4, 7, 0, 2, 1, 3, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED}, //ch2, bit
      
	  //channel 3
      //0  1  2  3  4  5  6  7
      { 4, 7, 0, 2, 1, 3, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED}, //ch3, bit  
      
	  //channel 4
      //0  1  2  3  4  5  6  7      
	  { 4, 7, 0, 2, 1, 3, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED}, //ch4, bit
      
	  //channel 5
      //0  1  2  3  4  5  6  7
	  { 4, 7, 0, 2, 1, 3, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED}, //ch5, bit
      
	  //channel 6
      //0  1  2  3  4  5  6  7      
	  { 4, 7, 0, 2, 1, 3, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED}, //ch6, bit
      
	  //channel 7
      //0  1  2  3  4  5  6  7      
	  { 4, 7, 0, 2, 1, 3, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED, CA_UNCONNECTED} //ch7, bit      
     };

uint8  bit_remapping_phy2bimc_CA[NUM_CH][PINS_PER_PHY] = {
      //channel 0
      //0  1  2  3  4  5  6  7    8         9
      { 2, 4, 3, 5, 0, CA_UNCONNECTED, CA_UNCONNECTED, 1, CA_UNCONNECTED, CA_UNCONNECTED}, //ch0, bit

      //channel 1
      //0  1  2  3  4  5  6  7
      { 2, 4, 3, 5, 0, CA_UNCONNECTED, CA_UNCONNECTED, 1, CA_UNCONNECTED, CA_UNCONNECTED}, //ch1, bit
      
	  //channel 2
      //0  1  2  3  4  5  6  7
      { 2, 4, 3, 5, 0, CA_UNCONNECTED, CA_UNCONNECTED, 1, CA_UNCONNECTED, CA_UNCONNECTED}, //ch2, bit
      
	  //channel 3
      //0  1  2  3  4  5  6  7
      { 2, 4, 3, 5, 0, CA_UNCONNECTED, CA_UNCONNECTED, 1, CA_UNCONNECTED, CA_UNCONNECTED}, //ch3, bit    
      
	  //channel 4
      //0  1  2  3  4  5  6  7      
	  { 2, 4, 3, 5, 0, CA_UNCONNECTED, CA_UNCONNECTED, 1, CA_UNCONNECTED, CA_UNCONNECTED}, //ch4, bit
      
	  //channel 5
      //0  1  2  3  4  5  6  7
	  { 2, 4, 3, 5, 0, CA_UNCONNECTED, CA_UNCONNECTED, 1, CA_UNCONNECTED, CA_UNCONNECTED}, //ch5, bit
      
	  //channel 6
      //0  1  2  3  4  5  6  7
	  { 2, 4, 3, 5, 0, CA_UNCONNECTED, CA_UNCONNECTED, 1, CA_UNCONNECTED, CA_UNCONNECTED}, //ch6, bit
      
	  //channel 7
      //0  1  2  3  4  5  6  7
	  { 2, 4, 3, 5, 0, CA_UNCONNECTED, CA_UNCONNECTED, 1, CA_UNCONNECTED, CA_UNCONNECTED} //ch7, bit      
     };    
     
       
//uint8 byte_remapping_table [NUM_CH][NUM_DQ_PCH] = {{0, 1}, {0, 1},{0, 1},{0, 1}};
uint8 byte_remapping_table [NUM_CH][NUM_DQ_PCH] = {{0, 1}, {1, 0},{0, 1},{0, 1},{0, 1}, {0, 1},{1, 0},{0, 1}};

uint8 connected_bit_mapping_no_DBI_A [PINS_PER_PHY_CONNECTED_NO_DBI]     = {0, 1, 2, 3, 4, 5, 6, 8   }; // These PHY DQ pads are connected to BIMC DQ pads
uint8 connected_bit_mapping_with_DBI_A [PINS_PER_PHY_CONNECTED_WITH_DBI] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; // PHY DQ[8] is DBI
uint8 connected_bit_mapping_CA [PINS_PER_PHY_CONNECTED_CA]               = {0, 1, 2, 3, 5, 8}; // These PHY CA pads are connected to BIMC CA pads

            // CA value sent back on DQ pads: CA: {0, 1, 2, 3, 4, 5} = DQ: {8, 9, 10, 11, 12, 13}
uint8 connected_bit_mapping_CA_PHY [PINS_PER_PHY_CONNECTED_CA]           = {0, 1, 2, 3, 4, 6}; //These are the odd DQ byte pads in PHY that contain data sent on the CA bus

uint8 dq_dbi_bit    = 8;
uint8 dq_spare_bit  = 9;

uint8 dll_analog_freq_range[]= {
   15 ,  /*  <300MHz  */
    7 ,  /*  <400MHz  */   
    3 ,  /*  <500MHz  */
    1 ,  /*  <700MHz  */
    0    /* < 1.89GHz  */
};

uint32 dll_analog_freq_range_table[]    = {300000, 400000, 500000, 700000, 1890000};
uint8  dll_analog_freq_range_table_size = sizeof(dll_analog_freq_range_table)/sizeof(uint8);

//================================================================================================//
// DDR PHY and CC one-time settings
//================================================================================================//
void DDR_PHY_CC_Config(BOOT_DDR_STRUCT *ddr)
{
    uint8 ch = 0, phy = 0;
    uint32 channels          = ddr->shrm_ddr_ptr->misc.enabled_channels;
    
    // QFI2 per channel timestamp counter offsets
    uint8 ts_v1[NUM_CH] = {45, 45, 46, 45, 45, 46, 46, 44};
    uint8 ts_v2[NUM_CH] = {45, 45, 46, 45, 45, 46, 46, 44};

    
    uint8 *ts_ptr = NULL;
    uint8 prfs_band = 0;
    uint32 reg_offset_ddr_phy;
    
    // Enable broadcast mode for all DQ PHYs on both channels
    DDRSS_PHY_Broadcast_DQs_All_Enable();

    DDRSS_set_config(ddr, BROADCAST_BASE,
        (uint32(*)[2])((size_t)(ddr->dcb_ptr) + (*ddr->dcb_ptr)[DQ_PHY_CFG_IDX][0]));

    // Enable broadcast mode for all CA PHYs on both channels
    DDRSS_PHY_Broadcast_CAs_All_Enable();

    DDRSS_set_config(ddr, BROADCAST_BASE,
        (uint32(*)[2])((size_t)(ddr->dcb_ptr) + (*ddr->dcb_ptr)[CA_PHY_CFG_IDX][0]));
    
    DDRSS_PHY_Broadcast_CCs_All_Enable();
    DDRSS_set_config(ddr, BROADCAST_BASE,
        (uint32(*)[2])((size_t)(ddr->dcb_ptr) + (*ddr->dcb_ptr)[DDR_CC_CFG_IDX][0]));
    
    DDRSS_PHY_Broadcast_All_Disable();
    
    // Per channel settings
    for(ch = 0; ch < NUM_CH; ch++)
    {
        // Skip channels which are not enabled
        if(((channels >> ch) & 0x1) == 0x0) continue;
        
        reg_offset_ddr_phy = REG_OFFSET_DDR_PHY_CH(ch);
        
        // Master PHY (odd channels only)
        if(MASTER_IOCAL(ch))
        {
            HWIO_OUTX(reg_offset_ddr_phy + CA0_DDR_PHY_OFFSET, DDR_PHY_DDRPHY_UPDATE_INTF_CFG, 0x1);
        }
        
        // Select QFI2.0 per channel timestamp counter offsets based on chip version
        if(ddr->shrm_ddr_ptr->misc.chip_version == 0x0100) // V1
        {
            ts_ptr = ts_v1;
        }
        else // V2
        {
            ts_ptr = ts_v2;
        }
        
        // Apply QFI2.0 per channel timestamp counter offsets to all PHYs and PRFS band 7 only
        for(phy = 0; phy < (NUM_CA_PCH + NUM_DQ_PCH); phy++)
        {
            HWIO_OUTXF(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_QFI2EXT_PRFS_7_CNTL_0_CFG, TIMESTAMP_COUNTER_OFFSET, ts_ptr[ch]);
        }

        // WRLVL settings @GCC mode to offset QFI2.0 propagation delay
        for(prfs_band = 0; prfs_band < LOWEST_WRLVL_COPY_PRFS; prfs_band++)
        {
            // WRLVL settings @GCC mode to offset QFI2.0 propagation delay
            HWIO_OUTX(reg_offset_ddr_phy + CA0_DDR_PHY_OFFSET + (4 * prfs_band), DDR_PHY_DDRPHY_CDCEXT_WRLVL_0_CTL_CFG, 0x1C07);
            HWIO_OUTX(reg_offset_ddr_phy + DQ0_DDR_PHY_OFFSET + (4 * prfs_band), DDR_PHY_DDRPHY_CDCEXT_WRLVL_0_CTL_CFG, 0x0);
            HWIO_OUTX(reg_offset_ddr_phy + DQ1_DDR_PHY_OFFSET + (4 * prfs_band), DDR_PHY_DDRPHY_CDCEXT_WRLVL_0_CTL_CFG, 0x3C0F);
        }
		
		//Reverting reg settings for CK_OE pad leakage current fix in V2.0. Changes for V2.1 in SWC
        if(ddr->shrm_ddr_ptr->misc.chip_version == 0x0200)
        {
            for(phy = 0; phy < NUM_CA_PCH; phy++)
            {               
               HWIO_OUTXF(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_CMIO_PAD_MODE_CFG, SW_CA_PAD_OE_OVR_MODE_DQS, 0x0);
            }
            
         
        }
		
				//Reg settings for CK_OE pad leakage current fix in V2.1
        if(ddr->shrm_ddr_ptr->misc.chip_version == 0x0200)
        {
            for(phy = 1; phy <= NUM_DQ_PCH; phy++)
            {
               HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_3_PWRS_0_CFG, 0x005592C8);
			   HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_4_PWRS_0_CFG, 0x005592C8);
			   HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_5_PWRS_0_CFG, 0x0055BCC8);
			   HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_6_PWRS_0_CFG, 0x0055BCC8);
			   HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_7_PWRS_0_CFG, 0x0055BCC8);              
            }
			
			for(phy = 0; phy < NUM_CA_PCH; phy++)
            {
               HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_3_PWRS_0_CFG, 0x00558001);
			   HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_4_PWRS_0_CFG, 0x00558001);
			   HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_5_PWRS_0_CFG, 0x00558001);
			   HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_6_PWRS_0_CFG, 0x00558001);
			   HWIO_OUTX(reg_offset_ddr_phy + (DDR_PHY_OFFSET * phy), DDR_PHY_DDRPHY_FPM_PRFS_7_PWRS_0_CFG, 0x00558001);              
            }                     
         
        }
    }  //ch 
}



//================================================================================================//
// Pre training set-up
//================================================================================================//

uint32   reg_offset_ddr_phy;


//================================================================================================//
// Used for saving and restoring registers modified by training
//================================================================================================//
uint32 dq_address_list[] =
{
    HWIO_DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG_ADDR(0),
    HWIO_DDR_PHY_DDRPHY_CMCDCWRLVL_MODE_CFG_ADDR(0),
    HWIO_DDR_PHY_DDRPHY_CMIO_PAD_MODE_CFG_ADDR(0),
    0
};
#define NUM_DQ_STACK (sizeof(dq_address_list) / sizeof(uint32))
uint32 dq_value_stack[NUM_DQ_STACK] = {0};

uint32 ca_address_list[] =
{
    HWIO_DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG_ADDR(0),
    HWIO_DDR_PHY_DDRPHY_CMCDCWRLVL_MODE_CFG_ADDR(0),
    HWIO_DDR_PHY_DDRPHY_CMIO_PAD_MODE_CFG_ADDR(0),
    HWIO_DDR_PHY_DDRPHY_FPM_PRFS_4_PWRS_0_CFG_ADDR(0),
    HWIO_DDR_PHY_DDRPHY_FPM_PRFS_5_PWRS_0_CFG_ADDR(0),
    HWIO_DDR_PHY_DDRPHY_FPM_PRFS_6_PWRS_0_CFG_ADDR(0),
    HWIO_DDR_PHY_DDRPHY_FPM_PRFS_7_PWRS_0_CFG_ADDR(0),
    0
};
#define NUM_CA_STACK (sizeof(ca_address_list) / sizeof(uint32))
uint32 ca_value_stack[NUM_CA_STACK] = {0};

/*  Write all the mode register settings needed before initial training or training restore */
void pre_training_mode_reg_writes (BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel, DDR_CHIPSELECT chip_select)
{   
    //Range 1 - unTerm range selected
    uint16 CA_unterm_vref= 0x5D;
    uint16 DQ_unterm_vref= 0x5D;
	
    //Range 0 - unTerm range selected
    uint16 CA_term_vref= 0x18;
    //uint16 DQ_term_vref= 0x11;
    //TBD DBI WrVref Change
    uint16 DQ_term_vref= 0x20;
    // DRAM MR22 Setup
    
    /* Set FPS-WR back to 0 */                 
    MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_13, (MR13_BASE)|0x0); 
    /* Initialize MR22 for SoC ODT */
    /* FSP0, MR22 = 0x0 */
	MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_12, CA_unterm_vref);
	MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_14, DQ_unterm_vref);
    MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_22, 0x0);
    
    /* Set FPS-WR to 1 */
    MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_13,(MR13_BASE)| 0x40);
    
    if (ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_type == DDR_TYPE_LPDDR4X) 
    {
        if (chip_select == DDR_CS_BOTH) 
        {
              MC_MR_Write(REG_OFFSET_MC_BROADCAST, DDR_CS0, JEDEC_MR_22, 0x04); // Ca, CS terminated prev val = 0x34
              MC_MR_Write(REG_OFFSET_MC_BROADCAST, DDR_CS1, JEDEC_MR_22, 0x2C); //  CS terminated. Prev val = 0x3C                                     
        }
        else if (chip_select == DDR_CS0) 
        {
        	MC_MR_Write(REG_OFFSET_MC_BROADCAST, DDR_CS0, JEDEC_MR_22, 0x04);//Ca, CS terminated. Prev val = 0x34
        }
    }
    
    else if (ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_type == DDR_TYPE_LPDDR4) 
    {
        MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_22, 0x23);
    }
    MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_12, CA_term_vref);
    MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_14, DQ_term_vref);
    /* Set FPS-WR back to 0 */                 
    MC_MR_Write(REG_OFFSET_MC_BROADCAST, chip_select, JEDEC_MR_13,(MR13_BASE)| 0x0); 	
}

void pre_training_setup(BOOT_DDR_STRUCT *ddr, 
                        DDR_CHANNEL channel, 
                        DDR_CHIPSELECT chip_select, 
                        uint32 freq, uint8 prfs_index, 
                        training_params_t *training_params_ptr,
                        uint8             *local_vars
                        ) 

{
#if DSF_PERIODIC_TRAINING_EN
    uint8 dit_freq_indx = 0;
#endif
	uint8 ch = 0;
	uint32 reg_offset_ddr_phy; 
	uint32 channels = ddr->shrm_ddr_ptr->misc.enabled_channels;
	
    training_data *training_data_ptr;
    training_data_ptr = (training_data *)(&ddr->training_results.training_data);

    //Enable PHY XO Clock
    DDRSS_PHY_XO_Clk_Ctrl(1);
    
  //uint32 reg_offset_osu;
  //uint32 reg_offset_mpe;
        
    /* Disable periodic SHKE events */
    MC_All_Periodic_Ctrl (ddr, chip_select, 0/* 1 for enable, 0 for disable */);
    
    /* Disable power saving features */
    MC_Power_Saving_Ctrl (ddr, chip_select, 0/* 1 for enable, 0 for disable */);
	
	pre_training_mode_reg_writes(ddr, channel, chip_select);
    
    // Save registers modified by for training purposes
    DDRSS_Register_Save_Restore(
                    SAVE,
                    NUM_DQ_STACK,
                    REG_OFFSET_DDR_PHY_CH(7) + DQ0_DDR_PHY_OFFSET,
                    dq_address_list,
                    dq_value_stack);
    DDRSS_Register_Save_Restore(
                    SAVE,
                    NUM_CA_STACK,
                    REG_OFFSET_DDR_PHY_CH(7) + CA0_DDR_PHY_OFFSET,
                    ca_address_list,
                    ca_value_stack);
       
     // Enable broadcast mode for all DQ and CA PHYs on both channels
     DDRSS_PHY_Broadcast_CAsDQs_All_Enable();

     // Set TRAFFIC_CFG registers to bypass clock gating for training
     HWIO_OUTX (BROADCAST_BASE, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, 0x7FFF);// 0x0067FFFF);
    // HWIO_OUTX (BROADCAST_BASE, DDR_PHY_DDRPHY_CMCDCWRLVL_MODE_CFG, 0x00300);//wrlvl mode cfg : CGC overide mode   
    
    // Enable broadcast mode for CA only
    DDRSS_PHY_Broadcast_CAs_All_Enable();
        
    // Enable MP Vref for IO qualifier
    HWIO_OUTX (BROADCAST_BASE, DDR_PHY_DDRPHY_FPM_PRFS_3_PWRS_0_CFG, 0x7D8003); // 1017MHz is Frange3
    HWIO_OUTX (BROADCAST_BASE, DDR_PHY_DDRPHY_FPM_PRFS_4_PWRS_0_CFG, 0x7D8003);
    HWIO_OUTX (BROADCAST_BASE, DDR_PHY_DDRPHY_FPM_PRFS_5_PWRS_0_CFG, 0x7D8003);
    HWIO_OUTX (BROADCAST_BASE, DDR_PHY_DDRPHY_FPM_PRFS_6_PWRS_0_CFG, 0x7D8003);
    HWIO_OUTX (BROADCAST_BASE, DDR_PHY_DDRPHY_FPM_PRFS_7_PWRS_0_CFG, 0x7D8003);
	
    //Disabling auto IOCAL before training 
    for(ch = 0; ch < NUM_CH; ch++)
    {
      // Skip channels which are not enabled
      if(((channels >> ch) & 0x1) == 0x0) continue;
      reg_offset_ddr_phy = REG_OFFSET_DDR_PHY_CH(ch);
      if(MASTER_IOCAL(ch))
      {
        // Disable Auto IOCAL beofre training
        HWIO_OUTXF(reg_offset_ddr_phy + CA0_DDR_PHY_OFFSET, DDR_PHY_DDRPHY_IOCTLR_CTRL_CFG, IO_CAL_AUTO, 0);
      }
    }
#if DSF_PXI_TRAINING_EN
    MC_PXI_Pre_Training_Config();
#else
    // Load the DRAM physical training addresses
    DDRSS_Get_Training_Address(ddr);
    
    // Repopulate write_in_pattern with x8 training data
    DDRSS_Create_Training_Data(ddr->shrm_ddr_ptr->misc.bus_width_pch);
#endif

#if DSF_PERIODIC_TRAINING_EN
  //may need to change following to determine ACQ and TRAC indexes
  if (ddr->shrm_ddr_ptr->ddr_runtime.max_ddr_frequency >= DSF_MIN_PERIODIC_TRAINING_FREQ)
  {
    for (dit_freq_indx = 0; dit_freq_indx < ddr->shrm_ddr_ptr->ddr_runtime.ddr_num_clock_levels; dit_freq_indx++)
    { 
      if ((ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[dit_freq_indx].clk_freq_in_khz) == ddr->shrm_ddr_ptr->ddr_runtime.max_ddr_frequency)
      {
	     break;
      }
    }
    
    if (ddr->shrm_ddr_ptr->ddr_runtime.max_ddr_frequency > F_RANGE_6)
    {
        training_data_ptr->results.dit.dit_acq_ndx = dit_freq_indx - 2;
        training_data_ptr->results.dit.dit_dual_tracking = 1;
    }
    else
    {
        training_data_ptr->results.dit.dit_acq_ndx = dit_freq_indx - 1;
        training_data_ptr->results.dit.dit_dual_tracking = 0;
    }
  }
  else //no periodic training required
  {
    training_data_ptr->results.dit.dit_acq_ndx = 0;
    training_data_ptr->results.dit.dit_dual_tracking = 0;
  }  
#else
  training_data_ptr->results.dit.dit_acq_ndx = 0;
  training_data_ptr->results.dit.dit_dual_tracking = 0;
#endif

  training_data_ptr->results.dit.write_training_window_start = DIT_FINE_STEP_LIMIT;
  training_data_ptr->results.dit.dit_loop_count = training_params_ptr->dit.max_loopcnt;

}

void post_training_setup(BOOT_DDR_STRUCT *ddr, 
                         DDR_CHANNEL channel, 
                         DDR_CHIPSELECT chip_select, 
                         uint32 freq, 
                         uint8 prfs_index, 
                         training_params_t *training_params_ptr,
                         uint8             *local_vars
                         ) 

{
   uint32 dq0_ddr_phy_base             = 0;     // DDR PHY DQ0 Base Address
   uint32 ca0_ddr_phy_base             = 0;     // DDR PHY CA0 Base Address
   uint8 iocal_ca_pcnt[NUM_CA_PCH] ={0};
   uint8 iocal_ca_ncnt[NUM_CA_PCH] ={0};
   uint8 rxcal_ca[NUM_CA_PCH] ={0};
   uint8 iocal_dq_pcnt[NUM_DQ_PCH] ={0};
   uint8 iocal_dq_ncnt[NUM_DQ_PCH] ={0};
   uint8 rxcal_dq[NUM_DQ_PCH] ={0};
   uint8 ca                            = 0;  // CA Loop counter
   uint8 dq                            = 0;  // DQ Loop counter
   uint8 ch                      = 0;	
   uint32 channels               = ddr->shrm_ddr_ptr->misc.enabled_channels;
    training_data *training_data_ptr;
    training_data_ptr = (training_data *)(&ddr->training_results.training_data);
    
#if DSF_PXI_TRAINING_EN
    MC_PXI_Post_Training_Config();
#endif

        // ============================================================================================================================
        // ==========================================             S P E C   C h e c k            ======================================
        // ============================================================================================================================ 
    for(ch = 0; ch < NUM_CH; ch++)
    {  
      if(((channels >> ch) & 0x1) == 0x0) continue;
      
   dq0_ddr_phy_base = REG_OFFSET_DDR_PHY_CH(ch) + DQ0_DDR_PHY_OFFSET;
   ca0_ddr_phy_base = REG_OFFSET_DDR_PHY_CH(ch) + CA0_DDR_PHY_OFFSET;
   reg_offset_ddr_phy = REG_OFFSET_DDR_PHY_CH(ch);
	 for (ca=0;ca<NUM_CA_PCH;ca++) 
     {	
	 rxcal_ca[ca] = HWIO_INXF (ca0_ddr_phy_base + (ca * DDR_PHY_OFFSET), DDR_PHY_DDRPHY_IOCS_DQ_STA, IOCS_HPRXCNT);
	 iocal_ca_pcnt[ca] = HWIO_INXF (ca0_ddr_phy_base + (ca * DDR_PHY_OFFSET), DDR_PHY_DDRPHY_IOCS_DQ_STA, IOCS_DQ_PCNT);
	 iocal_ca_ncnt[ca] = HWIO_INXF (ca0_ddr_phy_base + (ca * DDR_PHY_OFFSET), DDR_PHY_DDRPHY_IOCS_DQ_STA, IOCS_DQ_NCNT);
	              g_sed_struct_val.training = "RX_IO_CAL";
                  g_sed_struct_val.param_name = "RXCAL_CA";
                  g_sed_struct_val.param_steps = rxcal_ca[ca];
                  g_sed_struct_val.param_value = rxcal_ca[ca];
                  g_sed_struct_val.param_units = "NA";
                  g_sed_struct_val.vref_type = "NA";
                  g_sed_struct_val.vref_steps = DC;
                  g_sed_struct_val.vref_value = DC;
                  g_sed_struct_val.freq = freq;
                  g_sed_struct_val.ch = ch;
                  g_sed_struct_val.cs = ca;
                  g_sed_struct_val.byte_lane = DC;
                    //abort for rxcal_ca
                    if ((rxcal_ca[ca] < training_params_ptr->ddr_abort.rx_cal_min) || (rxcal_ca[ca] > training_params_ptr->ddr_abort.rx_cal_max))
                   // if ((rxcal_ca[ca] <=0) || (rxcal_ca[ca] >= 31))    
                    {  
                     g_abort_flag = 1;
                     training_params_ptr->small_eye_abort = 1;
                    }
	                if (((g_abort_flag) &&(training_params_ptr->ddr_abort.rx_io_cal_enable)) ||(g_sed_flag))
					//if (((g_abort_flag) &&(0x1)) ||(g_sed_flag))
				    {
				   DDRSS_SED_prints_struct();  
				    }
				  g_sed_struct_val.param_name = "IOCAL_CA_PCNT";	
				  g_sed_struct_val.param_steps = iocal_ca_pcnt[ca];
				  g_sed_struct_val.param_value = iocal_ca_pcnt[ca];
				     //abort for iocal_ca_pcnt
                    if ((iocal_ca_pcnt[ca] < training_params_ptr->ddr_abort.rx_cal_min) || (iocal_ca_pcnt[ca] > training_params_ptr->ddr_abort.rx_cal_max))
                    //if ((iocal_ca_pcnt[ca] <= 0) || (rxcal_ca[ca] >= 31))
                           
                    {  
                     g_abort_flag = 1;
                     training_params_ptr->small_eye_abort = 1;
                    }
	                //if (((g_abort_flag) &&(training_params_ptr->ddr_abort.rx_io_cal_enable)) ||(g_sed_flag))
					if (((g_abort_flag) &&(0x1)) ||(g_sed_flag))
				    {
				   DDRSS_SED_prints_struct();  
				    }
				  g_sed_struct_val.param_name = "IOCAL_CA_NCNT";	
				  g_sed_struct_val.param_steps = iocal_ca_ncnt[ca];
				  g_sed_struct_val.param_value = iocal_ca_ncnt[ca];
				  
				     //abort for iocal_ca_pcnt
                    if ((iocal_ca_ncnt[ca] < training_params_ptr->ddr_abort.rx_cal_min) || (iocal_ca_ncnt[ca] > training_params_ptr->ddr_abort.rx_cal_max))
					//if ((iocal_ca_ncnt[ca] <= 0) || (rxcal_ca[ca] >= 31))
                    {  
                     g_abort_flag = 1;
                     training_params_ptr->small_eye_abort = 1;
                    }
	                //if (((g_abort_flag) &&(training_params_ptr->ddr_abort.rx_io_cal_enable)) ||(g_sed_flag))
					if (((g_abort_flag) &&(0x1)) ||(g_sed_flag))
				    {
				   DDRSS_SED_prints_struct();  
				    }
	 }
     for (dq = 0; dq < NUM_DQ_PCH; dq++) 
     {	
	 rxcal_dq[dq] = HWIO_INXF (dq0_ddr_phy_base + (dq * DDR_PHY_OFFSET), DDR_PHY_DDRPHY_IOCS_DQ_STA, IOCS_HPRXCNT);
	 iocal_dq_pcnt[dq] = HWIO_INXF (dq0_ddr_phy_base + (dq * DDR_PHY_OFFSET), DDR_PHY_DDRPHY_IOCS_DQ_STA, IOCS_DQ_PCNT);
	 iocal_dq_ncnt[dq] = HWIO_INXF (dq0_ddr_phy_base + (dq * DDR_PHY_OFFSET), DDR_PHY_DDRPHY_IOCS_DQ_STA, IOCS_DQ_NCNT);
                  g_sed_struct_val.param_name = "RXCAL_DQ";
                  g_sed_struct_val.param_steps = rxcal_dq[dq];
				  g_sed_struct_val.param_value = rxcal_dq[dq];
                  g_sed_struct_val.cs = DC;
                  g_sed_struct_val.byte_lane = dq;
                    //abort for rxcal_ca
                    if ((rxcal_dq[dq] < training_params_ptr->ddr_abort.rx_cal_min) || (rxcal_dq[dq] > training_params_ptr->ddr_abort.rx_cal_max))
                    // if ((rxcal_dq[dq] <= 0) || (rxcal_ca[ca] >= 31))                        
                    {  
                     g_abort_flag = 1;
                     training_params_ptr->small_eye_abort = 1;
                    }
	                if (((g_abort_flag) &&(training_params_ptr->ddr_abort.rx_io_cal_enable)) ||(g_sed_flag))
				    {
				   DDRSS_SED_prints_struct();  
				    }
				  g_sed_struct_val.param_name = "IOCAL_DQ_PCNT";	
				  g_sed_struct_val.param_steps = iocal_dq_pcnt[dq];
				  g_sed_struct_val.param_value = iocal_dq_pcnt[dq];
				     //abort for iocal_ca_pcnt
                    if ((iocal_dq_pcnt[dq] < training_params_ptr->ddr_abort.rx_cal_min) || (iocal_dq_pcnt[dq] > training_params_ptr->ddr_abort.rx_cal_max))
                   // if ((iocal_dq_pcnt[dq] <= 0) || (rxcal_ca[ca] >= 31))                         
                    {  
                     g_abort_flag = 1;
                     training_params_ptr->small_eye_abort = 1;
                    }
					if (((g_abort_flag) &&(0x1)) ||(g_sed_flag))
				    {
				   DDRSS_SED_prints_struct();  
				    }
				  g_sed_struct_val.param_name = "IOCAL_DQ_NCNT";	
				  g_sed_struct_val.param_steps = iocal_dq_ncnt[dq];
				  g_sed_struct_val.param_value = iocal_dq_ncnt[dq];
				     //abort for iocal_ca_pcnt
                    if ((iocal_dq_ncnt[dq] < training_params_ptr->ddr_abort.rx_cal_min) || (iocal_dq_ncnt[dq] > training_params_ptr->ddr_abort.rx_cal_max))
                    //if ((iocal_dq_ncnt[dq] <= 0) || (rxcal_ca[ca] >= 31))                         
                    {  
                     g_abort_flag = 1;
                     training_params_ptr->small_eye_abort = 1;
                    }
	                //if (((g_abort_flag) &&(training_params_ptr->ddr_abort.rx_io_cal_enable)) ||(g_sed_flag))
					if (((g_abort_flag) &&(0x1)) ||(g_sed_flag))
				    {
				   DDRSS_SED_prints_struct();  
				    }
	 
	 }
           // ============================================================================================================================
           // ==========================================         S P E C   C h e c k  E n d      =========================================
           // ============================================================================================================================    
	}
    if (training_params_ptr->small_eye_abort == 1)
    {
       ddr_abort(); 
    }
    
	//Enabling auto IOCAL after training
    for(ch = 0; ch < NUM_CH; ch++)
    {
      if(((channels >> ch) & 0x1) == 0x0) continue;
      reg_offset_ddr_phy = REG_OFFSET_DDR_PHY_CH(ch);
      if(MASTER_IOCAL(ch))
      {
        // Enable Auto IOCAL After training
        HWIO_OUTXF(reg_offset_ddr_phy + CA0_DDR_PHY_OFFSET, DDR_PHY_DDRPHY_IOCTLR_CTRL_CFG, IO_CAL_AUTO, 1);
        DDR_PHY_hal_cfg_sw_iocal(reg_offset_ddr_phy + CA0_DDR_PHY_OFFSET);
      }
    }	
    // Restore registers modified for training purposes
    DDRSS_PHY_Broadcast_DQs_All_Enable();

		        
    DDRSS_Register_Save_Restore(
                    RESTORE,
                    NUM_DQ_STACK,
                    BROADCAST_BASE,
                    dq_address_list,
                    dq_value_stack);

   DDRSS_PHY_Broadcast_CAs_All_Enable();
                 
    DDRSS_Register_Save_Restore(
                    RESTORE,
                    NUM_CA_STACK,
                    BROADCAST_BASE,
                    ca_address_list,
                    ca_value_stack);
    
    // Save the restore data onto the stack
    ddr_printf (DDR_NORMAL,"\nSaving the DDR PHY Training Results\n\n");
    DDRSS_Training_Save_Restore(ddr, channel, SAVE);
    
    // Save read opcode offset for RCW
    ddr_external_memscpy(training_data_ptr->results.rcw.rd_opcode_offset,
        sizeof(training_data_ptr->results.rcw.rd_opcode_offset),
        ddr->shrm_ddr_ptr->ddr_runtime.mc_rd_opcode_offset,
        sizeof(ddr->shrm_ddr_ptr->ddr_runtime.mc_rd_opcode_offset));
	
    // Post Boot Training setup.
    DDRSS_Post_Boot_Training(ddr, channel, chip_select);
    
	/* Enable periodic SHKE events */
    MC_All_Periodic_Ctrl (ddr, chip_select, 1/* 1 for enable, 0 for disable */);
    
    /* Enable power saving features */ 
    MC_Power_Saving_Ctrl (ddr, chip_select, 1/* 1 for enable, 0 for disable */);
    
	//After training revert PHY_XO_CLK gating back to one time settings 
    DDRSS_PHY_XO_Clk_Ctrl(ddr->shrm_ddr_ptr->ddr_runtime.phy_xoclk);
    ddr_printf (DDR_NORMAL,"\nEND of DDR PHY Training\n\n");
}

void DDRSS_Pre_Init(BOOT_DDR_STRUCT *ddr)
{
    
    ddr->shrm_ddr_ptr->misc.num_channel = 8;
    ddr->shrm_ddr_ptr->misc.total_channels = DDR_CH0 | DDR_CH1 | DDR_CH2 | DDR_CH3
                                            | DDR_CH4 | DDR_CH5 | DDR_CH6 | DDR_CH7;
    ddr->shrm_ddr_ptr->misc.num_pcb = 6;
    ddr->shrm_ddr_ptr->misc.all_pcb_mask = 0x3F;
    ddr->shrm_ddr_ptr->misc.all_way_mask = 0xFFF;
    ddr->shrm_ddr_ptr->misc.ram_status0_mask = 0xFFFFFFFF;
    ddr->shrm_ddr_ptr->misc.ram_status1_mask = 0x0FFFFFFF;
    ddr->shrm_ddr_ptr->misc.pcb_status_mask  = 0x00FFFFFF;
    
    // Core versions
    if(ddr->shrm_ddr_ptr->misc.chip_version == 0x0100)
    {
        ddr->shrm_ddr_ptr->phy_core_version = TARGET_PHY_VER_V1;
        ddr->shrm_ddr_ptr->mc_core_version = TARGET_MC_VER_V1;
        ddr->shrm_ddr_ptr->llcc_core_version = TARGET_LLCC_VER_V1;
    }
    else
    {
        ddr->shrm_ddr_ptr->phy_core_version = TARGET_PHY_VER_V2;
        ddr->shrm_ddr_ptr->mc_core_version = TARGET_MC_VER_V2;
        ddr->shrm_ddr_ptr->llcc_core_version = TARGET_LLCC_VER_V2;
    }

    // Enable broadcast mode for all CCs on both channels
    DDRSS_PHY_Broadcast_CCs_All_Enable();

	// Workaround for DDRCC PLL lock issue
	if (((ddr->shrm_ddr_ptr->misc.platform_id == MSM_VERSION_HANA) || (ddr->shrm_ddr_ptr->misc.platform_id == MSM_VERSION_POIPU)) && (ddr->shrm_ddr_ptr->misc.chip_version == 0x100))
	{
    DDR_PHY_DDRCC_PLL_Lock_Workaround();		
	}
		
    // Disable broadcast mode 
    DDRSS_PHY_Broadcast_All_Disable();


}

//================================================================================================//
// void DDRSS_Pre_Init2(BOOT_DDR_STRUCT *ddr)
//================================================================================================//
void DDRSS_Pre_Init2(BOOT_DDR_STRUCT *ddr)
{
    if (HWIO_INXF (SEQ_DDR_SS_DDR_LAGG_DDR_SS_LAGG_OFFSET, DDR_SS_LAGG_LLCC_INFO_0, LLCC_NUM_CTRLRS) == 4)
    {
         HWIO_OUTX(SEQ_DDR_SS_MCCC_MCCC_MSTR_OFFSET, MCCC_MSTR_CHANNEL_DISABLE, 0xF);
	     HWIO_OUTXF(SEQ_DDR_SS_LLCC_BROADCAST_LLCC_BEAC_OFFSET, LLCC_BEAC_CFG2, LLCC_HALF_CH_CNT_EN,0x1);
         ddr->shrm_ddr_ptr->misc.enabled_channels = 0xF0;
         g_channelMask =0xF0;
            ddr->shrm_ddr_ptr->misc.num_channel = 4;

    }   
}





void DDRSS_Post_Config(BOOT_DDR_STRUCT *ddr)
{

if (ddr->shrm_ddr_ptr->misc.target_silicon == 1)
{
	uint8 ch = 0;
	//uint32 channel          = ddr->shrm_ddr_ptr->misc.enabled_channels;
	//uint32 ch_1hot = channel;
	//uint32 ch_tmp = DDR_CH_NONE;
	//uint32 dq0_ddr_phy_base;
	//uint32 dq1_ddr_phy_base;
	//uint32 ca0_ddr_phy_base;
	uint32 reg_cabo_shke_offset;
	//DDR_CHIPSELECT qualified_cs = DDR_CS_NONE;

   for (ch = 0; ch < NUM_CH; ch++)
   {
   	   		if ((ch==1) || (ch == 2) || (ch == 5) || (ch == 6))
		{
     		reg_cabo_shke_offset       = REG_OFFSET_MC_SHKE_CH(ch);
      		HWIO_OUTXF2 (reg_cabo_shke_offset, SHKE_DQ_BYTE_MAP, DQ0, DQ1, 1, 0);
	  	}
   }  

}
#if 0 

	
	ch = 0;
	ch_tmp = shrm_ddr_ptr->misc.enabled_channels;
	while(ch_tmp) 
	{
		if(ch_1hot & (0x1<<ch)) 
		{	
			dq0_ddr_phy_base  = REG_OFFSET_DDR_PHY_CH(ch) + DQ0_DDR_PHY_OFFSET;
			ca0_ddr_phy_base  = REG_OFFSET_DDR_PHY_CH(ch) + CA0_DDR_PHY_OFFSET;
			dq1_ddr_phy_base  = REG_OFFSET_DDR_PHY_CH(ch) + DQ1_DDR_PHY_OFFSET;
			
			if(ch == 0)
			{
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 0
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_1_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 1
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_2_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 2
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_3_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 3
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_4_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 4
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_5_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 5
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_6_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 6
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_7_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 7
				
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 0
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_1_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 1
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_2_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 2
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_3_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 3
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_4_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 4
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_5_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 5
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_6_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 6
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_7_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 7
				
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 0
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_1_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 1
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_2_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 2
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_3_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 3
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_4_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 4
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_5_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 5
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_6_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 6
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_7_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 7
			}
			
			else if(ch == 7)
			{
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 0
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_1_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 1
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_2_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 2
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_3_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 3
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_4_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 4
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_5_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 5
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_6_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 6
				HWIO_OUTXF(dq0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_7_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 7
				
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 0
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_1_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 1
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_2_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 2
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_3_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 3
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_4_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 4
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_5_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 5
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_6_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 6
				HWIO_OUTXF(ca0_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_7_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 7
				
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 0
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_1_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 1
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_2_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 2
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_3_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 3
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_4_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 4
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_5_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 5
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_6_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);     //PRFS 6
				HWIO_OUTXF(dq1_ddr_phy_base, DDR_PHY_DDRPHY_PADEXT_PRFS_7_FPM_CNTL0_CFG, /*SET A VALUE HERE*/);		//PRFS 7
			}
			
			else{}
		}
		ch_tmp &= ch_tmp-1;
		ch++;
	}
#endif 	
#if 0
    uint8 clk_idx = 0, qos = 0;
   
    // Apply settings for V2.0 which are different from V2.1
    if(ddr->shrm_ddr_ptr->misc.chip_version == 0x0200)
    {
        //Enable PHY XO Clock
        DDRSS_PHY_XO_Clk_Ctrl(1);
        
        // Enable broadcast mode for all CCs on both channels
        DDRSS_PHY_Broadcast_CCs_All_Enable();
        
        // Apply DDR_CC settings
        HWIO_OUTX(BROADCAST_BASE, DDR_CC_DDRCC_SPM_PWRS_0_FEA_VAL_MODE0_CFG, 0xFFC7FFFF);
        HWIO_OUTX(BROADCAST_BASE, DDR_CC_DDRCC_SPM_PWRS_1_FEA_VAL_MODE0_CFG, 0xFFC7FFFE);
        HWIO_OUTX(BROADCAST_BASE, DDR_CC_DDRCC_SPM_PWRS_2_FEA_VAL_MODE0_CFG, 0xFFC7FFFF);
        HWIO_OUTX(BROADCAST_BASE, DDR_CC_DDRCC_SPM_PWRS_3_FEA_VAL_MODE0_CFG, 0xFFC7DFFF);
        HWIO_OUTX(BROADCAST_BASE, DDR_CC_DDRCC_SPM_CTRL_CFG1, 0x00003000);
        HWIO_OUTX(BROADCAST_BASE, DDR_CC_DDRCC_SPM_FAL_INDX_STATE_CFG0, 0x30303030);
        
        // Disable broadcast mode 
        DDRSS_PHY_Broadcast_All_Disable();
        
        //Set PHY_XO_CLK gating according to one time settings
        DDRSS_PHY_XO_Clk_Ctrl(ddr->shrm_ddr_ptr->ddr_runtime.phy_xoclk);
        
        // Disable S2
        for(clk_idx = 0; clk_idx < MAX_NUM_CLOCK_PLAN; clk_idx++)
        {
            for(qos = 0; qos < 4; qos++)
            {
                ddr->shrm_ddr_ptr->ddr_runtime.mc_pwrs_cfg[clk_idx][qos][2] = 0;
            }
        }
        
        // Set S3 exit state to S0
        HWIO_OUTXF(REG_OFFSET_MC_OSU_BROADCAST, OSU_PWRS_CTRL, PWRS3_EXIT_STATE, 0x00);
        
        // Set OSU_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect 
        HWIO_OUTX(REG_OFFSET_MC_OSU_BROADCAST, OSU_LOAD_CONFIG, 0x1);

        // Poll for LOAD_CONFIG to become zero
        while(HWIO_INX(REG_OFFSET_MC_OSU_BROADCAST, OSU_LOAD_CONFIG) != 0x0);
        
        // 
        for(qos = 0; qos < 4; qos++)
        {
            HWIO_OUTXFI(REG_OFFSET_MC_MPE_BROADCAST, MPE_FCS_OPT_CFG_m, qos, CMD_LEN_CTRL, 0x3);
        }
        
        // Set MPE_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect 
        HWIO_OUTX(REG_OFFSET_MC_MPE_BROADCAST, MPE_LOAD_CONFIG, 0x1);
      
        // Poll for LOAD_CONFIG to become zero
        while(HWIO_INX(REG_OFFSET_MC_MPE_BROADCAST, MPE_LOAD_CONFIG) != 0x0);
        
        // 
        HWIO_OUTX(REG_OFFSET_MC_DTC_BROADCAST, DTC_TCMD_EXTEND_CFG, 0x02020202);
       
    }
	
#endif
}


