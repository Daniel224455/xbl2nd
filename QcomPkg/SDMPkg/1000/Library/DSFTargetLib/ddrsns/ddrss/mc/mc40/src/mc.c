/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

#include "mc.h"
#include "ddrss.h"

void MC_MR_Write (uint32 ch_offset, DDR_CHIPSELECT chip_select, uint32 mr_addr, uint32 mr_data)
{
   // Single command triggerring, only use SHKE_CMD_SET_CFG_0
   MC_Cmd_Set_Cfg (ch_offset, chip_select, 0, CMD_MRW, mr_addr, mr_data);
   // No wait time needed
   MC_Cmd_Stagger_Cfg (ch_offset, 0, 0, 0, 0);
   // Trigger command and wait for completion
   MC_Cmd_Trigger(ch_offset, 0x1); // Choose CMD_SET_CFG_0 in bit 0
}
 
void MC_Timing_Recalc_Update(uint32 ch_offset)
{
   uint32 reg_offset_dtc = 0;

   reg_offset_dtc = ch_offset + MC_DTC_OFFSET;

   // Kick off timing parameter calculation and wait until done
   // Set MC_DTC_SW_TRIGGER_CFG[RECALC_TIMERS] = 1
   HWIO_OUTX(reg_offset_dtc, DTC_SW_TRIGGER_CFG, 0x1);
   // Poll for MC_DTC_SW_TRIGGER_CFG[RECALC_TIMERS] and wait for the read value to become zero.
   while(HWIO_INX(reg_offset_dtc, DTC_SW_TRIGGER_CFG) != 0x0);

   // Update all the calculated DTC timing values and wait until done
   // Set MC_DTC_SW_TRIGGER_CORE[UPDATE_TIMERS] = 1
   HWIO_OUTX (reg_offset_dtc, DTC_SW_TRIGGER_CORE, 0x1);
   // Poll for MC_DTC_SW_TRIGGER_CORE[UPDATE_TIMERS] and wait for the read value to become zero.
   while(HWIO_INX(reg_offset_dtc, DTC_SW_TRIGGER_CORE) != 0x0);  
}

void MC_Periodic_Auto_Refresh_Ctrl(BOOT_DDR_STRUCT *ddr, DDR_CHIPSELECT chip_select, uint8 enable)
{
    uint8  rank_en          = 0;
    uint32 reg_offset_shke  = REG_OFFSET_MC_SHKE_BROADCAST;


         if (enable == 0x1) {
            // Set rank enable bit(s) based on chip_select
            rank_en = chip_select;
         }
         else {
            // Clear rank enable bit(s) to 0
            rank_en = 0;
         }

         // Enable/disable periodic refresh
         HWIO_OUTXF2 (reg_offset_shke, SHKE_REFRESH_EVENT_CTRL, REFRESH_PB_RANK_EN, RANK_EN, rank_en, rank_en);

    // Set SHKE_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect 
    HWIO_OUTX (reg_offset_shke, SHKE_LOAD_CONFIG, 0x1);

    // Poll for LOAD_CONFIG to become zero
    while (HWIO_INX(reg_offset_shke, SHKE_LOAD_CONFIG) != 0x0);
}

void MC_All_Periodic_Ctrl(BOOT_DDR_STRUCT *ddr, DDR_CHIPSELECT chip_select, uint8 enable)
{
    uint8  rank_en          = 0;
    uint32 reg_offset_shke  = REG_OFFSET_MC_SHKE_BROADCAST;


         if (enable == 0x1) {
            // Set rank enable bit(s) based on chip_select
            rank_en = chip_select;
         }
         else {
            // Clear rank enable bit(s) to 0
            rank_en = 0;
         }

    // 1. Enable/disable periodic refresh
    HWIO_OUTXF2(reg_offset_shke, SHKE_REFRESH_EVENT_CTRL, REFRESH_PB_RANK_EN, RANK_EN, rank_en, rank_en);

    // 2. Enable/disable periodic MR4 read
    if(ddr->shrm_ddr_ptr->ddr_runtime.MR4_polling_enable == 1)
    {
        HWIO_OUTXFI(reg_offset_shke, SHKE_INTERVAL_EVENT_CTRL_n, 0, RANK_EN, rank_en);

        /*Store event enabling info in shrm_ddr_ptr->periodic_events_enable_flag    
        * required while power wakeup sequence in SHRM
        *  periodic_event_enable_flag bit mapping as 
        * bit 0 =1 if SHKE_INTERVAL_EVENT_CTRL_0 is enable  
        * bit 1 =1 if SHKE_ZQSTART_EVENT_CTRL is enable
        * bit 2 =1 if SHKE_ZQLATCH_EVENT_CTRL is enable
        * bit 3 =1 if SHKE_PDT_TRAC_EVENT_CTRL is enable
        */
        ddr->shrm_ddr_ptr->periodic_events_enable_flag |= ((enable & 0x1) << SHKE_INTERVAL_EVENT_CTRL_0_EN);
        ddr->shrm_ddr_ptr->periodic_events_enable_flag &= ~((~enable & 0x1) << SHKE_INTERVAL_EVENT_CTRL_0_EN);
    }

    // 3. Enable/disable periodic ZQ calibration
    if(ddr->shrm_ddr_ptr->ddr_runtime.periodic_zq_calibration_enable == 1)
    {
        HWIO_OUTXF(reg_offset_shke, SHKE_ZQSTART_EVENT_CTRL, RANK_EN, rank_en);
        ddr->shrm_ddr_ptr->periodic_events_enable_flag |= ((enable & 0x1) << SHKE_ZQSTART_EVENT_CTRL_EN);
        ddr->shrm_ddr_ptr->periodic_events_enable_flag &= ~((~enable & 0x1) << SHKE_ZQSTART_EVENT_CTRL_EN);

        HWIO_OUTXF(reg_offset_shke, SHKE_ZQLATCH_EVENT_CTRL, RANK_EN, rank_en);
        ddr->shrm_ddr_ptr->periodic_events_enable_flag |= ((enable & 0x1) << SHKE_ZQLATCH_EVENT_CTRL_EN);
        ddr->shrm_ddr_ptr->periodic_events_enable_flag &= ~((~enable & 0x1) << SHKE_ZQLATCH_EVENT_CTRL_EN);
    }

    // 4. Enable/disable periodic DIT (DQS Oscillator Interval Timer)
    // Moved to HAL_DDR_Periodic_Training API

    // Set SHKE_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect 
    HWIO_OUTX (reg_offset_shke, SHKE_LOAD_CONFIG, 0x1);
    
    // Poll for LOAD_CONFIG to become zero
    while (HWIO_INX(reg_offset_shke, SHKE_LOAD_CONFIG) != 0x0);
}

void MC_Power_Saving_Ctrl(BOOT_DDR_STRUCT *ddr, DDR_CHIPSELECT chip_select, uint8 enable)
{
    uint8   idx_m               = 0;
    uint8   idx_n               = 0;
    uint8   clk_idx             = 0;
    uint32  reg_offset_osu      = REG_OFFSET_MC_OSU_BROADCAST;
    uint32  reg_offset_global   = REG_OFFSET_MC_GLOBAL_BROADCAST;

    clk_idx = DDRSS_Find_Clk_Index(ddr, ddr->shrm_ddr_ptr->misc.current_clk_in_kHz);

    // 1. Enable/disable CSPD
    // TODO: set all QoS registers to the same value for now. May update to use different values if needed.
    MC_Power_Down_Ctrl(ddr->shrm_ddr_ptr, chip_select, enable);
    MC_Clock_Stop_Ctrl(ddr->shrm_ddr_ptr, enable);

    // 2. Enable/disable HW activity based self-refresh
    MC_HW_Self_Refresh_Ctrl(ddr->shrm_ddr_ptr, chip_select, enable);

    // 3. Enable/disable root clock gating (except Napali V1)
    HWIO_OUTXF(reg_offset_global, GLOBAL_RTCG_CFG, RTCG_EN, enable);
    
    // Set GLOBAL_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect
    HWIO_OUTX(reg_offset_global, GLOBAL_LOAD_CONFIG, 0x1);
    
    // Poll for LOAD_CONFIG to become zero
    while(HWIO_INX(reg_offset_global, GLOBAL_LOAD_CONFIG) != 0x0)
    {
    }
    

         // 4. Enable QFI power state transitions
         if (enable == 1) {
            for (idx_m = 0; idx_m < 4; idx_m++) {
               for (idx_n = 0; idx_n < 4; idx_n++) {
                  HWIO_OUTXI2 (reg_offset_osu, OSU_PWRS_m_n_CFG, idx_m, idx_n,\
                               ddr->shrm_ddr_ptr->ddr_runtime.mc_pwrs_cfg[clk_idx][idx_m][idx_n]);
               }
            }
         }
         else if (enable == 0) {
            for (idx_m = 0; idx_m < 4; idx_m++) {
               for (idx_n = 0; idx_n < 4; idx_n++) {
                  HWIO_OUTXI2 (reg_offset_osu, OSU_PWRS_m_n_CFG, idx_m, idx_n, 0x0);
               }
            }
         }	 
    // Set OSU_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect 
    HWIO_OUTX(reg_offset_osu, OSU_LOAD_CONFIG, 0x1);
    
    // Poll for LOAD_CONFIG to become zero
    while(HWIO_INX(reg_offset_osu, OSU_LOAD_CONFIG) != 0x0);
}

void MC_ZQ_Calibration(BOOT_DDR_STRUCT *ddr, DDR_CHIPSELECT chip_select)
{
    uint8  ch               = 0;
    uint32 channels         = ddr->shrm_ddr_ptr->misc.enabled_channels;

    for(ch = 0; ch < NUM_CH; ch++)
    {
        // Skip channels which are not enabled
        if(((channels >> ch) & 0x1) == 0x0) continue;

        /* 1. Program for the ZQ Calibration Start command and configure HW to 
        wait for tZQCAL time before executing ZQ Calibration Latch command. */
        MC_Cmd_Set_Cfg(REG_OFFSET_MC_CH(ch), chip_select, 0, CMD_MPC_ZQCAL_START, 0x0, 0x0);
        
        // tZQCAL has 1ps resolution, convert to XO_PERIOD. 
        // clk_sel = 0x0 -> XO clk (19.2MHz), interval = 0x14 -> 20*(1/19.2MHz) = 1us = tZQCAL_in_ps
        MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_CH(ch), 0, 0x1/* for shared ZQ resistor */, WAIT_XO_CLOCK, 0x14); 

        // ZQCAL_LATCH does not need wait timer. tZQLAT timing is already taken care while
        // executing ZQCAL_LATCH. Hence set the wait timer to 0.
        MC_Cmd_Set_Cfg(REG_OFFSET_MC_CH(ch), chip_select, 1, CMD_MPC_ZQCAL_LATCH, 0x0, 0x0);
        MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_CH(ch), 1, 0, 0, 0);

        // Trigger command and wait for completion
        MC_Cmd_Trigger(REG_OFFSET_MC_CH(ch), 0x3); /* selecting commands 0-1 */
    }
}

void MC_Watchdog_Reset_Setup(BOOT_DDR_STRUCT *ddr, DDR_CHIPSELECT chip_select)
{
    uint32 reg_offset_shke  = REG_OFFSET_MC_SHKE_BROADCAST;
#if DSF_LPDDR4Y_SUPPORTED
    uint8  MR51_wrdata      = 0;
#endif // #if DSF_LPDDR4Y_SUPPORTED
  
    //using channel0 to get the chip_select info.   
//    chip_select = ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].populated_chipselect;
   
    /* 1. Configure the MRW execution to switch the FSP-WR to update the settings of non-operating set. 
    For the MR_DATA, OP[7-6] will be overwritten by HW and the programmed value doesn't matter.
    OP[5-0] should be the DRAM defaults/correspond to low DDR frequency. */
    // RRO(OP[4]) = 0x1, all other OPs in MR13 are set as default value of 0x0
    MC_Cmd_Set_Cfg(REG_OFFSET_MC_BROADCAST, chip_select, 13, CMD_MRW, JEDEC_MR_13, (MR13_BASE)|0x10);
    MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_BROADCAST, 13, 0, 0, 0);

         /* 2. Configure the MRW execution to update nWR, RL/WL in the DRAM as per the low DDR frequency. */
         // Choose lowest DDR frequency index 0x0 for extended_cdt_runtime.dram_latency and extended_cdt_runtime.mc_freq_switch
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_select, 14, CMD_MRW, JEDEC_MR_2,\
                         ddr->shrm_ddr_ptr->ddr_runtime.dram_latency[0].MR2);
         MC_Cmd_Stagger_Cfg (REG_OFFSET_MC_BROADCAST, 14, 0, 0, 0);
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_select, 15, CMD_MRW, JEDEC_MR_1,\
                         MC_MR1_Value(ddr->shrm_ddr_ptr, REG_OFFSET_MC_BROADCAST, 0, 0));
         MC_Cmd_Stagger_Cfg (REG_OFFSET_MC_BROADCAST, 15, 0, 0, 0);
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_select, 16, CMD_MRW, JEDEC_MR_3,\
                         ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[0].MR3);
         MC_Cmd_Stagger_Cfg (REG_OFFSET_MC_BROADCAST, 16, 0, 0, 0);

         /* 3. Configure the MRW execution to setup ODT values as per the low DDR frequency */
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_select, 17, CMD_MRW, JEDEC_MR_11,\
                         0 /* TODO: CA/DQ ODT values */);
         MC_Cmd_Stagger_Cfg (REG_OFFSET_MC_BROADCAST, 17, 0, 0, 0);
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_select, 18, CMD_MRW, JEDEC_MR_22,\
                         0 /* TODO: SoC ODT etc */);
         MC_Cmd_Stagger_Cfg (REG_OFFSET_MC_BROADCAST, 18, 0, 0, 0);

         /* 4. Configure the MRW execution to setup CA/DQ Vref values as per the low DDR frequency */
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_select, 19, CMD_MRW, JEDEC_MR_12,\
                         0x4D/* TODO: CA Vref value */);
         MC_Cmd_Stagger_Cfg (REG_OFFSET_MC_BROADCAST, 19, 0, 0, 0);
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_select, 20, CMD_MRW, JEDEC_MR_14,\
                         0x4D/* TODO: DQ Verf value */);
         MC_Cmd_Stagger_Cfg (REG_OFFSET_MC_BROADCAST, 20, 0, 0, 0);
		 
#if DSF_LPDDR4Y_SUPPORTED		 
    if(ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support & LPDDR4Y_SINGLE_ENDED_WRITE_DQS)
    {
        MR51_wrdata = 0x4;   
    }

    if(ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support & LPDDR4Y_SINGLE_ENDED_READ_DQS)
    {
        /* Enable at low frequency (Phy LPRX, below 300 MHz) in Mode Register 51 (single-ended Read DQS). */
        MR51_wrdata |= 0x2;
    }

    // If anything is active for LPDDR4Y, single-ended clock will be.
    // So use the SE Clock flag to force update of MR51.
    if(ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support & LPDDR4Y_SINGLE_ENDED_CLOCK)
    {
        /* Enable at low frequency in Mode Register 51 (single-ended clock). */
        MR51_wrdata |= 0x8;
    }

    /* b. Program the MRW execution to update
    LPDDR4Y support registers in the DRAM as per the target DDR frequency */
    MC_Cmd_Set_Cfg(REG_OFFSET_MC_BROADCAST, chip_select, 21, CMD_MRW, 51, MR51_wrdata);
    MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_BROADCAST, 21, 0x0, 0x0, 0x0);
#endif		 

    /* 5. Configure to do FSP-OP switch. 
    For the MR_DATA, OP[7-6] will be overwritten by HW and the programmed value doesn't matter. 
    OP[5-0] should be the DRAM defaults/correspond to low DDR frequency with high current mode enabled. */
    MC_Cmd_Set_Cfg(REG_OFFSET_MC_BROADCAST, chip_select, 22, CMD_MRW_FSP_OP, JEDEC_MR_13, (MR13_BASE)|0x18);
    MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_BROADCAST, 22, 0, 0, 0);

    /* 6. Configure to enter CSPD. */
    MC_Cmd_Set_Cfg(REG_OFFSET_MC_BROADCAST, DDR_CS_BOTH, 23, CMD_POWER_DOWN_CSPDE, 0x0, 0x0);
    /* Stagger interval is equal to the cycles required for CK_OFF opcode propagated and processed in MC clock */
    /* Clock plan index 2 (mc_pwrs_extend_cycle[2]) is equal to boot frequency 200MHz */
    MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_BROADCAST, 23, 0, WAIT_XO_CLOCK, 5);
    
    /* 7. Select the commands to be executed when watchdog reset occurs. */
    HWIO_OUTX(reg_offset_shke, SHKE_WDOG_CMD_SEQ, 0xFFE000); /* selecting command 13-23 */
}


