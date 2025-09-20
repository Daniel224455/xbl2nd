/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
#include "ddrss.h"
#include "mc_common.h"
#include "mc.h"
#include <string.h>
#include "ddr_shrm_periodic_training.h"

void Training_DDRSS_DIT(BOOT_DDR_STRUCT *ddr, 
                        DDR_CHANNEL channel, 
                        DDR_CHIPSELECT chip_select, 
                        uint32 clk_freq_khz, 
                        uint8 prfs_index, 
                        training_params_t *training_params_ptr,
                        uint8             *local_vars)
{  

    uint8 ch                  = 0;
    uint8 cs;
    uint8 die                 = 0;
    uint32 dit_count[NUM_CH][NUM_CS][NUM_DIE_PCH] = {{{0}}};
    uint8 device_width				= 0;
  
    training_data *training_data_ptr;
    training_data_ptr = (training_data *)(&ddr->training_results.training_data);
    uint8 freq_indx = 0;
    uint32 acq_freq     = 0;
    uint32 track_freq   = 0;
    uint32 channels          = ddr->shrm_ddr_ptr->misc.enabled_channels;
    uint8 channel_id    = 0;
    
   for(ch = 0; ch < NUM_CH; ch++)
   {
     // Skip channels which are not enabled
     if(((channels >> ch) & 0x1) == 0x0) 
     {
        channel_id ++;
     }
     else 
     {
       break;
     }
   }

    acq_freq = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[training_data_ptr->results.dit.dit_acq_ndx].clk_freq_in_khz;
    track_freq = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[training_data_ptr->results.dit.dit_acq_ndx + 1].clk_freq_in_khz;
       
    //================================================================================================//
    // DIT Training
    //================================================================================================//
    if (ddr->shrm_ddr_ptr->ddr_runtime.periodic_training_enable && track_freq >= DSF_MIN_PERIODIC_TRAINING_FREQ && clk_freq_khz >= acq_freq)
    {   
        if (training_data_ptr->results.dit.dit_dual_tracking == 1)
        {
            if (clk_freq_khz > track_freq)
            {   
                ddr_printf(DDR_NORMAL, "START: DIT runtime calculation \n");
                DDRSS_DIT_Runtime(ddr,  channel_id , 0, clk_freq_khz, 0);
			    ddr_printf(DDR_NORMAL, "DIT runtime count: %u \n", training_data_ptr->results.dit.dit_runtime_count);
                ddr_printf(DDR_NORMAL, "END: DIT runtime calculation  \n");
                freq_indx = 2;
            }
            
            else if (clk_freq_khz == track_freq)
            {
                freq_indx = 1;
            }
        }
        else if (clk_freq_khz == track_freq)
        {
            ddr_printf(DDR_NORMAL, "START: DIT runtime calculation \n");
            DDRSS_DIT_Runtime(ddr,  channel_id, 0, clk_freq_khz, 0);
			ddr_printf(DDR_NORMAL, "DIT runtime count: %u \n", training_data_ptr->results.dit.dit_runtime_count);
            ddr_printf(DDR_NORMAL, "END: DIT runtime calculation  \n");
            freq_indx = 1;
        }
        
        for(ch = 0; ch < NUM_CH; ch++)
        {
            // Skip channels which are not enabled
            if(((channels >> ch) & 0x1) == 0x0) continue;
                for(cs = 0; cs < NUM_CS; cs++)
                {
                    if((chip_select >> cs) & 0x1)
                    {
                        ddr_printf(DDR_NORMAL, "START: DIT training on Ch: %u Rank: %u\n", ch, cs);
                        DDRSS_DIT_Read (ddr, ch, cs, training_params_ptr, dit_count, clk_freq_khz);

                        device_width = ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_io_width[cs];

                        for (die=0; die <= device_width; die++)
                        {
                            training_data_ptr->results.dit.dit_count[ch][freq_indx][cs][die] = dit_count[ch][cs][die];
                            DDRSS_DIT_Calculate_Parameters(ddr, ch, cs, die, clk_freq_khz);
                            ddr_printf(DDR_NORMAL, "Die %u DIT count: %u\n", die, dit_count[ch][cs][die]);
                        }

                        ddr_printf(DDR_NORMAL, "END: DIT training on Ch: %u Rank: %u\n", ch, cs);
                    }
                }
        } //ch
            
        if (clk_freq_khz == acq_freq)
        {
            ddr->training_results.boot_dit_factory_flag = BOOT_DIT_FACTORY_FLAG;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
// DIT Training Routines
//-------------------------------------------------------------------------------------------------------------

uint8 DDRSS_DIT_Capture(BOOT_DDR_STRUCT *ddr,
                       uint8 ch,
                       uint8 cs,
                       uint32 mpc_start_to_mrr,
                       uint32 (*dit_die_count))
{
    uint8 device_width			= 0;
    uint8 die                   = 0;
    uint32 MR18_data            = 0;
    uint32 MR19_data            = 0;
    uint8 invalid_count_flag    = 0;
    uint32 ch_offset            = REG_OFFSET_MC_CH(ch);
    DDR_CHIPSELECT cs_1hot = DDR_CS_NONE;

    // Convert the channel to one-hot for BIMC functions
    cs_1hot = CS_1HOT(cs);
    
    device_width = ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_io_width[cs];
    
    //Send MPC Start Command
    MC_Cmd_Set_Cfg(ch_offset, cs_1hot, 0, CMD_MPC_DQS_OSC_START, 0, 0);
    MC_Cmd_Stagger_Cfg(ch_offset, 0, 0x0, 0x0, mpc_start_to_mrr);
    MC_Cmd_Trigger (ch_offset, 1);
    
    //MR 18/19 Read
    MR18_data = MC_MR_Read (ch_offset, cs_1hot, JEDEC_MR_18);
    MR19_data = MC_MR_Read (ch_offset, cs_1hot, JEDEC_MR_19);
    
    for (die=0; die <= device_width; die++)
    {
        dit_die_count[die] = ((MR18_data & (0xFF << (DIT_DIE1_BIT_SHIFT * die))) >> (DIT_DIE1_BIT_SHIFT * die)) | 
            (((MR19_data & (0xFF << (DIT_DIE1_BIT_SHIFT * die))) >> (DIT_DIE1_BIT_SHIFT * die)) << DIT_DIE1_BIT_SHIFT);
    }
    
    // Discard illegal count results
    if (device_width == DEVICE_WIDTH_IS_x8)
	{
        invalid_count_flag = dit_die_count[0] <= 0x40 || dit_die_count[0] == 0xFFFF ||
                             dit_die_count[1] <= 0x40 || dit_die_count[1] == 0xFFFF;
    }
    else
    {
        invalid_count_flag = dit_die_count[0] <= 0x40 || dit_die_count[0] == 0xFFFF;       
    }
    
    return invalid_count_flag;
}

void DDRSS_DIT_Read (BOOT_DDR_STRUCT *ddr,
                     uint8  ch,
                     uint8  cs,
                     training_params_t *training_params_ptr,
                     uint32 (*dit_count)[NUM_CS][NUM_DIE_PCH],
                     uint32 clk_freq_khz)
{
    uint32 dit_die_count[2]     = {0};
    uint32 dit_die_count_acc[2] = {0};
    uint32 dit_die_count_max[2] = {0};
    uint32 dit_die_count_min[2] = {0xffffffff,0xffffffff};
    uint16 dit_runtime_count    = 0;
    uint8 loopcnt               = 0;
    uint8 die                   = 0;
    uint8 max_loopcnt           = 0;
    uint8 device_width          = 0;
    uint8 invalid_count_flag    = 0;
    uint8 dit_watchdog          = 0;
    uint32 mpc_start_to_mrr     = 0;
    
    training_data *training_data_ptr;
    training_data_ptr = (training_data *)(&ddr->training_results.training_data);

    dit_runtime_count = training_data_ptr->results.dit.dit_runtime_count;
    mpc_start_to_mrr = training_data_ptr->results.dit.mpc_start_to_mrr;

    max_loopcnt = training_params_ptr->dit.max_loopcnt;

    device_width = ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_io_width[cs];
    
    DDRSS_DIT_Runtime_MR_Write(ddr, ch, cs, dit_runtime_count);

    // Reset the loop count
    loopcnt = 0;

    // Read and average multiple DRAM Oscillator counts
    while ((loopcnt < max_loopcnt) && (dit_watchdog < DIT_WATCHDOG))
    {
        // Read the DRAM DQS oscillator counter
        invalid_count_flag = DDRSS_DIT_Capture(ddr, ch, cs, mpc_start_to_mrr, dit_die_count);

        if(!invalid_count_flag)
        {
            // Calculate the minimum and maximum counts
            for (die=0; die <= device_width; die++)
            {
                // Accumulate the per die counter results
                dit_die_count_acc[die] += dit_die_count[die];

                // Check for the maximum
                if (dit_die_count[die] > dit_die_count_max[die])
                    dit_die_count_max[die] = dit_die_count[die];

                // Check for the minimum
                if (dit_die_count[die] < dit_die_count_min[die])
                    dit_die_count_min[die] = dit_die_count[die];
            }
            loopcnt++;
        }
        dit_watchdog++;
    } // while
    
    if (dit_watchdog == DIT_WATCHDOG)
    {
        ddr_printf(DDR_ERROR, "\nDDR ABORT: DIT READ FAIL, Frequency = %d, Channel = %d, Rank = %d\n", clk_freq_khz, ch, cs );
        ddr_abort();
    }

    // Process the accumulated count results
    for (die=0; die <= device_width; die++)
    {
        // Remove the minimum and maximum counts
        dit_die_count[die] = dit_die_count_acc[die] - (dit_die_count_max[die] + dit_die_count_min[die]);

        // Average the accumulated counts (after removing the min and max)
        dit_die_count[die] = dit_die_count[die]/(max_loopcnt-2);

        dit_count[ch][cs][die] = dit_die_count[die];
    }
}

void DDRSS_DIT_Slope(uint16  dit_count,
                     uint16 dit_fine_step_size,
                     uint16 (*dit_mid_count))
{
    uint32  dit_delta_count[DIT_NUM_SLABS+1]  = {0};
    uint8   range                             = 0;
    uint8   index                             = 0;
    uint8   mid                               = 0;
    uint8   num_upper_buckets                 = 0;
    uint8   center                            = DIT_NUM_SLABS/2;
    
    num_upper_buckets = DIT_NUM_SLABS - center;
    
    // Populate the center of the array with the boot DIT count
    dit_delta_count[center] = dit_count;
    
    // Calculate the upper range of the array
    for (range=1;range<=num_upper_buckets;range++)
    {
        // Determine the index for the delta count array
        index = range + center;

        // Calculate the Delta count when the clock tree changes in units of FINE_STEP_IN_PS (FINE_STEP_IN_PS * two ring oscillator half cycles)
        dit_delta_count[index] = dit_delta_count[index-1] - dit_fine_step_size;
    }

    index = center-1;

    // Calculate the lower range of the array
    for (range=1;range<=center;range++)
    {
        // Calculate the Delta count when the clock tree changes in units of FINE_STEP_IN_PS (FINE_STEP_IN_PS * two ring oscillator half cycles)
        dit_delta_count[index] = dit_delta_count[index+1] + dit_fine_step_size;
        index--;
    }

    // Populate the return array with the midpoints between the delta counts
    for (mid=0;mid<=DIT_NUM_SLABS-1;mid++)
    {
        // Calculate the midpoints
        dit_mid_count[mid] = (dit_delta_count[mid] + dit_delta_count[mid+1])/2;
    }
}

void DDRSS_DIT_Calculate_Parameters(BOOT_DDR_STRUCT *ddr,
                     uint8  ch,
                     uint8  cs,
                     uint8  die,
                     uint32 clk_freq_khz)
{

    uint16  dit_count                         = 0;
    uint16  boot_dit_count                    = 0;
    uint16  dit_runtime_count                 = 0;
    uint8   dit_bucket_index                  = 0;
    uint32  dit_acquisition_time              = 0;
    uint32  dit_dram_clk_period               = 0;
    uint16  dit_fine_step_size                = 0;

    uint32  dit_ps_per_count                  = 0;
    uint16  dit_boot_mid_count[DIT_NUM_SLABS]  = {0};
    
    uint8 freq_indx = 0;
    uint8 trac_ndx = 0;
    uint32 trac_freq = 0;

    // Training data structure pointer
    training_data *training_data_ptr;
    training_data_ptr = (training_data *)(&ddr->training_results.training_data);
    
    trac_ndx    = training_data_ptr->results.dit.dit_acq_ndx + 1;
    trac_freq   = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[trac_ndx].clk_freq_in_khz;
    
    if (clk_freq_khz == trac_freq)
        freq_indx = 1;
    else if (training_data_ptr->results.dit.dit_dual_tracking == 1 && clk_freq_khz > trac_freq)
        freq_indx = 2;

    // Fetch the boot training DIT count result
    dit_count = training_data_ptr->results.dit.dit_count[ch][freq_indx][cs][die];
    dit_runtime_count = training_data_ptr->results.dit.dit_runtime_count;

    // Calculate the clock period
    dit_dram_clk_period =  CONVERT_CYC_TO_PS/clk_freq_khz;
    
    // Calculate the acquisition time
    dit_acquisition_time = dit_runtime_count * dit_dram_clk_period;
    
    if (DSF_OTA_TRAINING_EN && ddr->training_results.boot_dit_factory_flag == BOOT_DIT_FACTORY_FLAG)
    {    
        boot_dit_count = ddr->training_results.boot_dit_count[ch][freq_indx][cs][die];
        
        // Calculate the time (ps) per DIT count for original boot DIT
        dit_ps_per_count = dit_acquisition_time/boot_dit_count;
        
        //Calculate bucket DIT delta according to original boot
        dit_fine_step_size = boot_dit_count - dit_acquisition_time / (dit_ps_per_count + (FINE_STEP_IN_PS * 2));
        
        if (training_data_ptr->results.dit.write_training_window_start_calculated == 0)
        {
            //recalculate original boot DIT array
            DDRSS_DIT_Slope(boot_dit_count, dit_fine_step_size, dit_boot_mid_count);
        
            //determine start point for write training CDC fine window
            for (dit_bucket_index = 0; dit_bucket_index < DIT_NUM_SLABS; dit_bucket_index++)
            {
                if (dit_count > dit_boot_mid_count[dit_bucket_index])
                    break;
            }
            
            if (dit_bucket_index == 0)
                training_data_ptr->results.dit.write_training_window_start = 0;
            else if (dit_bucket_index > DIT_NUM_SLABS - 2)
                training_data_ptr->results.dit.write_training_window_start = DIT_NUM_SLABS - 2;
            else
                training_data_ptr->results.dit.write_training_window_start = dit_bucket_index - 1;
            
            training_data_ptr->results.dit.write_training_window_start_calculated = 1;
        }
        
        //overwrite new DIT count with original factory boot DIT count to create identical arrays 
        training_data_ptr->results.dit.dit_count[ch][freq_indx][cs][die] = boot_dit_count;
    }
    else  //this is the original factory boot (or OTA training disabled).  store factory boot dit
    {
        ddr->training_results.boot_dit_count[ch][freq_indx][cs][die] = dit_count;
    
        // Calculate the time (ps) per DIT count
        dit_ps_per_count = dit_acquisition_time/dit_count;
    
        //Calculate bucket DIT delta
        dit_fine_step_size = dit_count - dit_acquisition_time / (dit_ps_per_count + (FINE_STEP_IN_PS * 2));
        
        if (training_data_ptr->results.dit.write_training_window_start_calculated == 0)
        {
            training_data_ptr->results.dit.write_training_window_start = DIT_NUM_SLABS/2 - 1;
            training_data_ptr->results.dit.write_training_window_start_calculated = 1;
        }
    }

    training_data_ptr->results.dit.dit_fine_step_size[ch][freq_indx][cs][die] = dit_fine_step_size;
}



void DDRSS_DIT_Runtime(BOOT_DDR_STRUCT *ddr,
                     uint8  ch,
                     uint8  cs,
                     uint32 clk_freq_khz,
                     uint8  wr_tdqs2dq)
{
    uint32 dit_die_count[2]     = {0};
    uint32 dit_dram_clk_period  = 0;
    uint16 dit_runtime_table[4] = {1008, 2048, 4096, 8192};
    uint16 dit_runtime_count    = 0;
    uint32 dit_delta_count[2]   = {0};
    uint32 dit_ps_per_count[2]  = {0};
    uint32 dit_acquisition_time = 0;
    uint8 loopcnt               = 0;
    uint8 die                   = 0;
    uint8 device_width			= 0;
    uint8 dit_delta_count_flag  = 1;
    uint8 invalid_count_flag    = 0;
    uint8 dit_watchdog          = 0;
    uint32 mpc_start_to_mrr     = 0;
    uint32 acq_freq             = 0;

    // Training data structure pointer
    training_data *training_data_ptr;
    training_data_ptr = (training_data *)(&ddr->training_results.training_data);
    
    if (wr_tdqs2dq)
        acq_freq = clk_freq_khz;
    else
        acq_freq = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[training_data_ptr->results.dit.dit_acq_ndx].clk_freq_in_khz;
    
    if (wr_tdqs2dq || !(DSF_OTA_TRAINING_EN && ddr->training_results.boot_dit_factory_flag == BOOT_DIT_FACTORY_FLAG))
    {

        // Calculate the clock period
        dit_dram_clk_period =  CONVERT_CYC_TO_PS/clk_freq_khz;

        device_width = ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_io_width[cs];

        // Search for the optimal run time count for the first pass of the DIT Read function
        // Normalize the results if needed by increasing the acquisition count
        while (dit_delta_count_flag && loopcnt < 4 && dit_watchdog < DIT_WATCHDOG)
        {
            // Calculate the MR23 acquisition count
            dit_runtime_count    = dit_runtime_table[loopcnt];
        
            //dit_read_cnt based on this equation, which is in core clock cycles (1555200):
            //  dit_read_cnt = runtime cnt in MC cycles + tOSCO (40ns)
            //      dit_runtime_count/2 = runtime cnt in MC cycles
            //      (40/1000)*((acq_freq/2)/1000) = tOSCO (40ns)
            //then convert dit_read_cnt into XO clock cycles
            mpc_start_to_mrr = (dit_runtime_count * 19200 / acq_freq) + 1;
        
            DDRSS_DIT_Runtime_MR_Write(ddr, ch, cs, dit_runtime_count);

            invalid_count_flag =  DDRSS_DIT_Capture(ddr, ch, cs, mpc_start_to_mrr, dit_die_count);

            if(!invalid_count_flag)
            {
                dit_acquisition_time = dit_runtime_count * dit_dram_clk_period;

                // Calculate the dit delta count
                for (die=0; die <= device_width; die++)
                {
                    dit_ps_per_count[die] = dit_acquisition_time / dit_die_count[die];
                    dit_delta_count[die]  = dit_acquisition_time / (dit_ps_per_count[die] + (FINE_STEP_IN_PS * 2));
                    dit_delta_count[die]  = dit_die_count[die] - dit_delta_count[die];
                }

                // Update the results data structure with the revised runtime count
                if (training_data_ptr->results.dit.dit_runtime_count < dit_runtime_count)
                {
                    training_data_ptr->results.dit.dit_runtime_count = dit_runtime_count;
                    training_data_ptr->results.dit.mpc_start_to_mrr = mpc_start_to_mrr;
                }

                if (device_width == DEVICE_WIDTH_IS_x8)
                {
                    dit_delta_count_flag = (dit_delta_count[0] < DIT_NORMAL_DELTA) && (dit_delta_count[1] < DIT_NORMAL_DELTA)  ;
                }
                else
                {
                    dit_delta_count_flag = (dit_delta_count[0] < DIT_NORMAL_DELTA) ;
                }
                loopcnt++;
            }
            dit_watchdog++;
        }
    
        if (dit_watchdog == DIT_WATCHDOG)
        {
            ddr_printf(DDR_ERROR, "\nDDR ABORT: DIT RUNTIME READ FAIL, Frequency = %d, Channel = %d, Rank = %d\n", clk_freq_khz, ch, cs );
            ddr_abort();
        }
    
        if (ddr->training_results.boot_dit_factory_flag != BOOT_DIT_FACTORY_FLAG)
        {
            ddr->training_results.boot_dit_runtime_count = dit_runtime_count;
        }
    }
    else //OTA training, restore original factory Runtime Count
    {
        dit_runtime_count = ddr->training_results.boot_dit_runtime_count;
        
        DDRSS_DIT_Runtime_MR_Write(ddr, ch, cs, dit_runtime_count);
        
        training_data_ptr->results.dit.dit_runtime_count = dit_runtime_count;
        
        mpc_start_to_mrr = (dit_runtime_count * 19200 / acq_freq) + 1;
        training_data_ptr->results.dit.mpc_start_to_mrr = mpc_start_to_mrr;
    }
}

void DDRSS_DIT_Runtime_MR_Write(BOOT_DDR_STRUCT *ddr,
                     uint8  ch,
                     uint8  cs,
                     uint16 dit_runtime_count)
{
    uint32 MR23_value           = 0;
    uint32 ch_offset       =  REG_OFFSET_MC_CH(ch);
    DDR_CHIPSELECT cs_1hot = DDR_CS_NONE;

    // Convert the channel to one-hot for BIMC functions
    cs_1hot = CS_1HOT(cs);
    
    // Decode the runtime count for the DRAM MR23 register
    if (dit_runtime_count < 2048)
    {
        MR23_value = dit_runtime_count / 16;
    }
    else
    {
        if (dit_runtime_count == 2048)
            MR23_value = 0x40;
        else if (dit_runtime_count == 4096)
            MR23_value = 0x80;
        else if (dit_runtime_count == 8192)
            MR23_value = 0xC0;
    }
        
    // Set DQS interval timer runtime setting
    MC_MR_Write (ch_offset, cs_1hot, JEDEC_MR_23, MR23_value);
        
    return;
}
