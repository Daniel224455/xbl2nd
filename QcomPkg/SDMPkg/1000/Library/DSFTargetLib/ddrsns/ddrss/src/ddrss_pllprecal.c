/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

#include "ddrss.h"
#include "ddrss_mccc.h"
#define DDR_CC_PRFS_OFFSET 0X54

void Training_DDRSS_PLLPRECAL      (BOOT_DDR_STRUCT *ddr, 
                                    DDR_CHANNEL channel, 
                                    DDR_CHIPSELECT chip_select, 
                                    uint32 freq,  
                                    uint8 prfs_index, 
                                    training_params_t *training_params_ptr,
                                    uint8             *local_vars)
{
#if TARGET_PHY_MIN_VER >= 0x03020100 // new feature
    uint8 ch				        = 0;
    uint32 channels     	        = 0;
    uint32 active_pll               = 0;
    uint32 reg_offset_ddrcc         = 0;  
    uint32 DDRCC_threshold          = 0;
    uint32 PLL0_Cal[NUM_CH]         ={0};
    uint32 PLL1_Cal[NUM_CH]         ={0};
    uint32 DDRCC_prfs_indx_fmax[8]  ={0};
    uint8 index                     = 0;
    uint32 curr_clk_freq_khz        = 0;
    uint8 clock_plan_idx            = 0;
    
    training_data *training_data_ptr;
    training_data_ptr = (training_data *)(&ddr->training_results.training_data);
    
    // Skip PLL pre-calibration if not enabled in training_params
    //if(training_params_ptr->PLL_Pre_Cal_En == 0)
    //    return;
    channels = ddr->shrm_ddr_ptr->misc.enabled_channels;    
        for(ch = 0; ch < NUM_CH; ch += 2) 
    {
        // Skip channels which are not enabled
        if(((channels >> ch) & 0x1) == 0x0) continue;

    reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;
    break; 
    }
    //Enumerate DDRCC thresholds for each PRFS.
    Get_LUT_band_DDRCC(reg_offset_ddrcc, DDRCC_prfs_indx_fmax);
    
    // Initialize the pre cal settings:
    // Change frequency to 200MHz
    //ddr_external_set_clk_speed (200000); // This freq switch can be commented if curr freq is in GCC mode.
    
    for(ch = 0; ch < NUM_CH; ch += 2) 
    {
        // Skip channels which are not enabled
        if(((channels >> ch) & 0x1) == 0x0) continue;
        
        reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;			
        HWIO_OUTX (reg_offset_ddrcc , DDR_CC_DDRCC_QLINKPLL0_SYSTEM_MUXES, 0xC0);
        HWIO_OUTX (reg_offset_ddrcc , DDR_CC_DDRCC_QLINKPLL1_SYSTEM_MUXES, 0xC0); 
        HWIO_OUTX (reg_offset_ddrcc , DDR_CC_DDRCC_QLINKPLL0_CALIBRATION_SETTINGS,0x40);
        HWIO_OUTX (reg_offset_ddrcc , DDR_CC_DDRCC_QLINKPLL1_CALIBRATION_SETTINGS,0x40);
    }
    
    DDRCC_threshold = (CONVERT_CYC_TO_PS /HWIO_INXF (reg_offset_ddrcc , DDR_CC_DDRCC_TOP_CTRL_CFG1, GCC_MODE_THRESHOLD));
    
    // Do PLL pre cal training for all DDRCC frequencies
    for(clock_plan_idx = ddr->shrm_ddr_ptr->ddr_runtime.ddr_num_clock_levels - 1;
         ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[clock_plan_idx].clk_freq_in_khz > DDRCC_threshold;
         clock_plan_idx--)
    {
        curr_clk_freq_khz = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[clock_plan_idx].clk_freq_in_khz;
    
#if DSF_PLLPRECAL_TRAINING_LOG_IN        
        ddr_printf(DDR_NORMAL, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
#endif   
        //Switch to target frequency
        ddr_external_set_clk_speed (curr_clk_freq_khz);
    
        for(ch = 0; ch < NUM_CH; ch += 2) 
        {
            // Skip channels which are not enabled
            if(((channels >> ch) & 0x1) == 0x0) continue;
            
            reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;
    
            // Determine the current active pll
            active_pll = HWIO_INXF (reg_offset_ddrcc, DDR_CC_DDRCC_PLLCTRL_STATUS, ACTIVE_PLL);	
            if (active_pll == 0)
            {
                PLL0_Cal[ch] = HWIO_INX (reg_offset_ddrcc, DDR_CC_DDRCC_QLINKPLL0_BAND_SEL_CAL);
            }
            else
            {
                PLL1_Cal[ch] = HWIO_INX (reg_offset_ddrcc, DDR_CC_DDRCC_QLINKPLL1_BAND_SEL_CAL);
            }
        }
        
        //Switch to other PLL
        ddr_external_set_clk_speed (curr_clk_freq_khz);    
        
        for(ch = 0; ch < NUM_CH; ch += 2)
        {
            // Skip channels which are not enabled
            if(((channels >> ch) & 0x1) == 0x0) continue;
            
            reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;
    
            // Determine the current active pll
            active_pll = HWIO_INXF (reg_offset_ddrcc, DDR_CC_DDRCC_PLLCTRL_STATUS, ACTIVE_PLL);
            
            if (active_pll == 0)
            {
                PLL0_Cal[ch] = HWIO_INX (reg_offset_ddrcc, DDR_CC_DDRCC_QLINKPLL0_BAND_SEL_CAL);
            }
            else
            {
                PLL1_Cal[ch] = HWIO_INX (reg_offset_ddrcc, DDR_CC_DDRCC_QLINKPLL1_BAND_SEL_CAL);
            }
        }
        
        //Determine current clk freq's DDRCC PRFS index
        for(index = 0; index < NUM_PRFS_BANDS; index++)
        {
            if(curr_clk_freq_khz < DDRCC_prfs_indx_fmax[index])
                break;
#if DSF_PLLPRECAL_TRAINING_LOG_IN
            ddr_printf(DDR_NORMAL,"++Freq: %d MHz,  DDRCC PRFS Band: %d; PRFS_threshold %d Mhz\n",curr_clk_freq_khz, index, DDRCC_prfs_indx_fmax[index]); 
#endif    
    
        }
        
        for(ch = 0; ch < NUM_CH; ch += 2) 
        {
            // Skip channels which are not enabled
            if(((channels >> ch) & 0x1) == 0x0) continue;
            
            reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;
#if DSF_PLLPRECAL_TRAINING_LOG_IN            
            ddr_printf(DDR_NORMAL,"++Channel: %d, PLL Calibration value: PLL0: %x,  PLL1: %x \n",ch, PLL0_Cal[ch], PLL1_Cal[ch]);
#endif                    
            // in DDRCC offest between each PRFS band is 0x54 (This is not same as DDRPHY offset)
            HWIO_OUTX(reg_offset_ddrcc +(index*DDR_CC_PRFS_OFFSET), DDR_CC_DDRCC_FPMEXT_PERF0_CTL_12_CFG, PLL0_Cal[ch]);
            HWIO_OUTX(reg_offset_ddrcc +(index*DDR_CC_PRFS_OFFSET), DDR_CC_DDRCC_FPMEXT_PERF0_CTL_13_CFG, PLL1_Cal[ch]);
        }
        
#if DSF_PLLPRECAL_TRAINING_LOG_IN        
        ddr_printf(DDR_NORMAL, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
#endif
    
    }

   // for(ch = 0; ch < NUM_CH; ch += 2) 
   // { 
   //     // Skip channels which are not enabled
   //     if(((channels >> ch) & 0x1) == 0x0) continue;
   //     
   //     reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;
        DDR_PHY_Disable_Band_Sel_Cal();  //channel loop inside the function           
   // }
    
    // Indicate PLL pre-calibration has been completed
    training_data_ptr->results.pll_precal_completed = 1;
#endif // TARGET_PHY_MIN_VER >= 0x03020100
}

