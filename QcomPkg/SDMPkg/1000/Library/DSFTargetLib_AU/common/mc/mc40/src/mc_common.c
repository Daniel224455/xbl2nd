/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
#define V2notHana 0
#include "mc_common.h"
#include "shrm_common.h"

uint32 *shrm_mr_cmd_log_ptr = (uint32 *)(SHRM_MR_DUMP_ADDR + 0x4);
uint8 *shrm_mr_cmd_log_curr_indx = (uint8 *) (SHRM_MR_DUMP_ADDR);
uint8 *shrm_mr_cmd_list_ptr = (uint8 *)(SHRM_MR_DUMP_ADDR + 0x84);

void MC_MR_Cmd_Log(uint32 mr_addr, uint32 mr_data)
{
    uint8 current_idx = 0;

    current_idx = *shrm_mr_cmd_log_curr_indx & 0xFF;   

    //If current is MAX value 32 then reset index   
    if (current_idx == MAX_MR_CMD_LOG_INDEX)
    {
        current_idx = 0;
        *shrm_mr_cmd_log_curr_indx = 0;
    }

    //Log <mr_addr << 16> ORing with mr_data  
    shrm_mr_cmd_log_ptr[current_idx] = (mr_addr << 16) | mr_data;


    //Store the latest MR value for given MR CMD
    shrm_mr_cmd_list_ptr[mr_addr] = (uint8)mr_data;
 
 
    //Increment circular buffer current index
    *shrm_mr_cmd_log_curr_indx = *shrm_mr_cmd_log_curr_indx + 1;

}


void MC_Cmd_Set_Cfg (uint32 ch_offset, uint32 chip_select, uint8 reg_num,\
                     uint32 event_sel, uint32 mr_addr, uint32 mr_data)

{
   uint32 reg_value = 0;
   uint32 reg_offset = 0;
   // Combine field values to create single register write
   reg_value = ((mr_data << 24)     & 0xFF000000) /* MR_DATA   [31:24] */ | \
               ((mr_addr << 16)     & 0x00FF0000) /* MR_ADDR   [23:16] */ | \
               ((chip_select << 12) & 0x00003000) /* RANK_SEL  [13:12] */ | \
               ((event_sel)         & 0x000003FF) /* EVENT_SEL [9:0]   */ ;

   reg_offset = ch_offset + MC_SHKE_OFFSET;
   HWIO_OUTXI (reg_offset, SHKE_CMD_SET_CFG_n, reg_num, reg_value);

   MC_MR_Cmd_Log(mr_addr, mr_data);
}
    
void MC_Cmd_Stagger_Cfg (uint32 ch_offset, uint8 reg_num, uint32 stagger_en,\
                         uint32 clk_sel, uint32 interval)
{
   uint32 reg_value = 0;
   uint32 reg_offset = 0;

   // Combine field values to create single register write
   reg_value = ((stagger_en << 20) & 0x00100000) /* STAGGER_RANKS_EN [20]    */ | \
               ((clk_sel    << 16) & 0x00030000) /* CLK_SELECT       [17:16] */ | \
               ((interval)         & 0x0000FFFF) /* INTERVAL         [15:0]  */ ;
   
   reg_offset = ch_offset + MC_SHKE_OFFSET;
   HWIO_OUTXI (reg_offset, SHKE_CMD_SET_STAGGER_CFG_n, reg_num, reg_value);
}

void MC_Cmd_Trigger (uint32 ch_offset, uint32 cmd_sel)
{
   uint32 reg_offset = 0;

   reg_offset = ch_offset + MC_SHKE_OFFSET;

   // Set the trigger for HW to execute the commands selected by CMD_SELECT
   HWIO_OUTX (reg_offset, SHKE_SW_CMD_SEQ, cmd_sel); // Assume PRIORITY field [29:28] is the same across the channels
   HWIO_OUTX (reg_offset, SHKE_SW_CMD_TRIGGER, 0x1); 
      
   // Poll for MC_SW_CMD_TRIGGER[SEQ_TRIGGER] and wait for the read value to become zero
   while (HWIO_INX (reg_offset, SHKE_SW_CMD_TRIGGER) != 0x0);
}
 
uint8 MC_MR1_Value (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint32 ch_offset, uint8 RL_WL_idx, uint8 params_idx)
{
   uint8 mr1_value = 0;
   uint32 reg_offset = ch_offset + MC_MPE_OFFSET;
   mr1_value = (HWIO_INX (reg_offset, MPE_CCP_OPT_CFG) & 0x03)                           /* MR1[1:0] = Burst Length */   |\
               ((shrm_ddr_ptr->ddr_runtime.mc_freq_switch[params_idx].wr_pre << 2) & 0x04) /* MR1[2]   = WR-PRE       */ |\
               ((shrm_ddr_ptr->ddr_runtime.mc_freq_switch[params_idx].rd_pre << 3) & 0x08) /* MR1[3]   = RD-PRE       */ |\
               ((RL_WL_idx << 4) & 0x70) /* nWR located in RL_WL_lpddr_struct, use RL_WL_idx */     /* MR1[6:4] = nWR */ |\
               ((shrm_ddr_ptr->ddr_runtime.mc_freq_switch[params_idx].rpst << 7) & 0x80);  /* MR1[7]   = RPST         */

   return mr1_value;
}

uint8 MC_DRAM_Latency_Index (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint32 clk_in_ps)
{
   uint8 clk_idx = 0;

   /* Cover the case of clk_in_ps with initial value 0 */
   if (clk_in_ps == 0) { 
      return 0;
   }
   else {
      for (clk_idx = 0; clk_idx < NUM_DRAM_LATENCY_STRUCTS; clk_idx++) {
       if (clk_in_ps >= shrm_ddr_ptr->ddr_runtime.dram_latency[clk_idx].clk_in_ps)
          break;
      }
      // Make sure we don't return an index past the end of the table
      return MIN(clk_idx, (NUM_DRAM_LATENCY_STRUCTS-1));
   }
}

uint8 MC_Freq_Switch_Params_Index (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint32 clk_in_ps)
{
   uint8 clk_idx = 0;

   /* Cover the case of clk_in_ps with initial value 0 */
   if (clk_in_ps == 0) { 
      return 0;
   }
   else {   
      for (clk_idx = 0; clk_idx < NUM_FREQ_SWITCH_STRUCTS; clk_idx++) {
          if (clk_in_ps >= shrm_ddr_ptr->ddr_runtime.mc_freq_switch[clk_idx].clk_in_ps)
             break;
      }
      // Make sure we don't return an index past the end of the table
      return MIN(clk_idx, (NUM_FREQ_SWITCH_STRUCTS-1));
   }
}

void MC_Enter_Self_Refresh(SHRM_DDR_STRUCT *shrm_ddr_ptr)
{
    uint8  ch                = 0;
    uint32 channels          = shrm_ddr_ptr->misc.enabled_channels;
    uint32 chip_selects      = shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].populated_chipselect;
    
    uint32 reg_offset_shke   = 0;
    uint32 reg_offset_global = 0;
    uint32 reg_offset_osu    = 0;
    
    reg_offset_shke   = REG_OFFSET_MC_SHKE_BROADCAST;
    reg_offset_global = REG_OFFSET_MC_GLOBAL_BROADCAST;
    reg_offset_osu    = REG_OFFSET_MC_OSU_BROADCAST;
    
    // 2 Disable the clock gating for the power state clock so that the
    //   idle counter CGC enables get enough clocks to be resetted
    HWIO_OUTXF(reg_offset_osu, OSU_CLK_CTRL, PWRS_CTRL_CLK_CG_EN, 0);
    
    // 3 Configure the below register to error out on the new RD/WR traffic
    //   that is being sent to MC SW has to ensure that no new traffic comes
    //   to MC before doing this step
    HWIO_OUTX(reg_offset_shke, SHKE_INIT_CONFIG, 0);

#if TARGET_MC_MIN_VER == 0x04010000 // workaround
    // Disable ZQ_CG_EN and AYSNC_CG_EN before disabling periodic ZQ
    HWIO_OUTXF2(reg_offset_shke, SHKE_CLK_CTRL, ZQ_CG_EN, EVENT_ASYNC_CG_EN, 0, 0);
#endif
    
    // 4 Disable the periodic ZQ calibration, periodic MR4 reads
    HWIO_OUTXF(reg_offset_shke, SHKE_ZQSTART_EVENT_CTRL, RANK_EN, 0);
    HWIO_OUTXF(reg_offset_shke, SHKE_ZQLATCH_EVENT_CTRL, RANK_EN, 0);
    
    HWIO_OUTXFI(reg_offset_shke, SHKE_INTERVAL_EVENT_CTRL_n, 0, RANK_EN, 0);
    HWIO_OUTXF(reg_offset_shke, SHKE_PDT_TRAC_EVENT_CTRL, RANK_EN, 0);
    HWIO_OUTXF(reg_offset_shke, SHKE_PDT_ACQ_EVENT_CTRL, RANK_EN, 0);
    
    // 5 Set the below register for the above settings to take effect
    HWIO_OUTX(reg_offset_shke, SHKE_LOAD_CONFIG, 1);

    // 6 Set the SW self refresh vote to put the DRAM into self refresh
    HWIO_OUTX(reg_offset_shke, SHKE_SW_SELF_REFRESH_VOTE, chip_selects);
    
    for(ch = 0; ch < NUM_CH; ch++)
    {
        // Skip channels which are not enabled
        if(((channels >> ch) & 0x1) == 0x0) continue;
        
        reg_offset_shke   = REG_OFFSET_MC_SHKE_CH(ch);
        reg_offset_global = REG_OFFSET_MC_GLOBAL_CH(ch);
        reg_offset_osu    = REG_OFFSET_MC_OSU_CH(ch);
        
        // 7 Poll for SW self refresh status (RANKS_IN_SELFRFRSH (bits[9:8]))
        //   and CSPD status (RANKS_IN_CSPD (bits[5:4])) of each rank present to be 1
        if(chip_selects & DDR_CS0)
        {
            while((HWIO_INX(reg_offset_global, GLOBAL_DEVICE_STATUS) & 0x110) != 0x110);
        }
        if(chip_selects & DDR_CS1)
        {
            while((HWIO_INX(reg_offset_global, GLOBAL_DEVICE_STATUS) & 0x220) != 0x220);
        }
        
        // 8 Poll for RCH FIFO to be empty ensuring that upstream has accepted all the read data
        while((HWIO_INX(reg_offset_global, GLOBAL_ISU_RCH_STATUS) & 0x1) != 0x1);
	
        // 9 Poll for ZQ Calibration and ZQ Latch FSM to be in idle state
#if TARGET_MC_MIN_VER >= 0x04010002 // new feature
        while((HWIO_INX(reg_offset_shke, SHKE_ZQCAL_STATUS) & 0x7) != 0x0){} 
#endif
        
		// 10 Poll for idle registers to be set to ensure that the clocks are not needed anymore by CABO.
        //    Once set, it indicates that the power collapse entry can now be initiated
#if TARGET_MC_MIN_VER == 0x04010000 // workaround
        if(shrm_ddr_ptr->mc_core_version > 0x04010000)
#endif
        {
#if TARGET_MC_MIN_VER >= 0x04010002 // new feature
            while(HWIO_INXF(reg_offset_global, GLOBAL_IDLE_STATUS, IDLE) != 0x1);
            while(HWIO_INXF(reg_offset_osu, OSU_IDLE_STATUS, PWRS_CTRL_IDLE) != 0x0);
#else
            while(HWIO_INXF(reg_offset_global, GLOBAL_IDLE_STATUS, RANK_IDLE) != 0x3);
            while(HWIO_INXF(reg_offset_osu, OSU_IDLE_STATUS, PWRS_CTRL_IDLE) != 0x1);
#endif
        }
    }
}
       
void MC_Exit_Self_Refresh (SHRM_DDR_STRUCT *shrm_ddr_ptr)
{
   uint8  ch                          = 0;
   uint32 channels          = shrm_ddr_ptr->misc.enabled_channels;
   uint32 chip_selects      = shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].populated_chipselect;
   uint8  shke_interval_event_en      = 0;
   uint8  shke_ZQStart_event_en       = 0;
   uint8  shke_ZQLatch_event_en       = 0;
   uint8  shke_pdt_trac_event_en      = 0;
   uint32 periodic_events_enable_flag = 0; 
   uint32 reg_offset_shke             = 0;

   /* periodic_event_enable_flag bit mapping as 
    * bit 0 =1 if SHKE_INTERVAL_EVENT_CTRL_0 is enable  
    * bit 1 =1 if SHKE_ZQSTART_EVENT_CTRL is enable
    * bit 2 =1 if SHKE_ZQLATCH_EVENT_CTRL is enable
    * bit 3 =1 if SHKE_PDT_TRAC_EVENT_CTRL is enable
    */
   periodic_events_enable_flag = shrm_ddr_ptr->periodic_events_enable_flag;
   shke_interval_event_en = (periodic_events_enable_flag >> SHKE_INTERVAL_EVENT_CTRL_0_EN )& 0x01 ;
   shke_ZQStart_event_en  = (periodic_events_enable_flag >> SHKE_ZQSTART_EVENT_CTRL_EN ) & 0x01;
   shke_ZQLatch_event_en  = (periodic_events_enable_flag >> SHKE_ZQLATCH_EVENT_CTRL_EN ) & 0x01;
   shke_pdt_trac_event_en = (periodic_events_enable_flag >> SHKE_PDT_TRAC_EVENT_CTRL_EN) & 0x01;

    reg_offset_shke   = REG_OFFSET_MC_SHKE_BROADCAST;   
   
  
         /* 9. Clear the SW self refresh vote */
         //Below implementation casuse hang when chip selects is 1 
         //HWIO_OUTX (reg_offset_shke, SHKE_SW_SELF_REFRESH_VOTE, (~chip_selects & 0x3));
         HWIO_OUTX (reg_offset_shke, SHKE_SW_SELF_REFRESH_VOTE, (0));

         /* 10. Program the MR4 read command for two ranks respectively. */
         /* a. Program the MR4 read command to Rank0. */
         MC_Cmd_Set_Cfg(REG_OFFSET_MC_BROADCAST, DDR_CS0, 10, CMD_MRR_MR4, 0x0, 0x0); 
         MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_BROADCAST, 10, 0x0, 0x0, 0x0);
         /* b. Set the trigger for HW to execute the commands programmed above */
         /* c. Poll for the below register and wait for the read value to become zero */
         MC_Cmd_Trigger(REG_OFFSET_MC_BROADCAST, 0x400); // Selecting command 10
         

            /* d. Poll for the below register and wait for the read value to become zero */
            while ((HWIO_INXF (reg_offset_shke, SHKE_DTC_CTRL_STATUS, FSM_STATE)) != 0x0);
         
         if (chip_selects == DDR_CS_BOTH) {
            /* e. Program the MR4 read command to Rank0. */
            MC_Cmd_Set_Cfg(REG_OFFSET_MC_BROADCAST, DDR_CS1, 10, CMD_MRR_MR4, 0x0, 0x0); 
            MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_BROADCAST, 10, 0x0, 0x0, 0x0);
            /* f. Set the trigger for HW to execute the commands programmed above */
            /* g. Poll for the below register and wait for the read value to become zero */
            MC_Cmd_Trigger(REG_OFFSET_MC_BROADCAST, 0x400); // Selecting command 10
         }

         /* 11. Program for the ZQ Calibration Start command and configure HW to wait for tZQCAL time 
            before executing ZQ Calibration Latch command */
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_selects, 11, CMD_MPC_ZQCAL_START, 0x0, 0x0);
         // clk_sel = 0x0 -> XO clk (19.2MHz), interval = 0x14 -> 20*(1/19.2MHz) = 1us = tZQCAL_in_ps
         MC_Cmd_Stagger_Cfg(REG_OFFSET_MC_BROADCAST, 11, 0x1/* for shared ZQ resistor */, WAIT_XO_CLOCK, 0x14); 

         /* 12. Program for the ZQ Calibration Latch command */
         /* ZQCAL_LATCH does not need wait timer. tZQLAT timing is already taken care while
            executing ZQCAL_LATCH. Hence set the wait timer to 0 */
         MC_Cmd_Set_Cfg (REG_OFFSET_MC_BROADCAST, chip_selects, 12, CMD_MPC_ZQCAL_LATCH, 0x0, 0x0);
         MC_Cmd_Stagger_Cfg (REG_OFFSET_MC_BROADCAST, 12, 0, 0, 0);

   // Disable MC broadcast
   //DDRSS_Broadcast_MC_Config(DDR_CH_NONE/* disable broadcast */, NO_RDATA);

   // CMD triggering will not support broadcast, need to use for loop to trigger each channel in series   
    for(ch = 0; ch < NUM_CH; ch++)
    {
        // Skip channels which are not enabled
        if(((channels >> ch) & 0x1) == 0x0) continue;
         /* 13. Select the commands to be executed */
         /* 14. Set the trigger for HW to execute the commands programmed above */
         /* 15. Poll for CABO_SHKE_SW_CMD_TRIGGER[SEQ_TRIGGER] and wait for the read value to become zero */
         MC_Cmd_Trigger(REG_OFFSET_MC_CH(ch), 0x1800); // selecting commands 11-12
      
   }
   

         /* 16. Enable the periodic ZQ calibration and periodic MR4 reads */
          
            if(shke_ZQStart_event_en) {
               HWIO_OUTXF (reg_offset_shke, SHKE_ZQSTART_EVENT_CTRL, RANK_EN, chip_selects);
            } 
            if(shke_ZQLatch_event_en) {
               HWIO_OUTXF (reg_offset_shke, SHKE_ZQLATCH_EVENT_CTRL, RANK_EN, chip_selects);
            }
        

         if(shke_interval_event_en) {
            HWIO_OUTXFI (reg_offset_shke, SHKE_INTERVAL_EVENT_CTRL_n, 0, RANK_EN, chip_selects);
         }

         if(shke_pdt_trac_event_en) {
            HWIO_OUTXF (reg_offset_shke, SHKE_PDT_TRAC_EVENT_CTRL, RANK_EN, chip_selects);
           /*Disable HW DIT Collection when in SW ACQ */
            HWIO_OUTXF (reg_offset_shke, SHKE_PDT_ACQ_EVENT_CTRL, RANK_EN, 0x0);
         }
         
         /* 17. Enable MC to accept RD/WR traffic */
         HWIO_OUTX (reg_offset_shke, SHKE_INIT_CONFIG, chip_selects & shrm_ddr_ptr->misc.not_frd);
       
         /* 18. Set the below register for the above settings to take effect */
         HWIO_OUTX (reg_offset_shke, SHKE_LOAD_CONFIG, 0x1); 
      
   // Disable MC broadcast
   //DDRSS_Broadcast_MC_Config(DDR_CH_NONE/* disable broadcast */, NO_RDATA);
}

void MC_HW_Self_Refresh_Ctrl(SHRM_DDR_STRUCT *shrm_ddr_ptr, DDR_CHIPSELECT chip_select, uint8 enable)
{
    uint32 reg_offset_shke = REG_OFFSET_MC_SHKE_BROADCAST;
    uint8 rank_en = 0;
    
    // Check if HW self refresh feature enable flag is set
    if(shrm_ddr_ptr->ddr_runtime.hw_self_refresh_enable == 1)
    {
        // Enable by setting RANK_EN bits based on chip_select
        if(enable == 1)
        {
            rank_en = chip_select;
        }
        
        // Set SHKE_HW_SELF_REFRESH_CTRL[RANK_EN] to a rank mask for enabled ranks
        HWIO_OUTXF(reg_offset_shke, SHKE_HW_SELF_REFRESH_CTRL, RANK_EN, rank_en);
        
        // Set SHKE_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect 
        HWIO_OUTX(reg_offset_shke, SHKE_LOAD_CONFIG, 0x1);
    }
}

void MC_QFI_Clock_Gating_Ctrl (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint8 enable)
{
   uint32 reg_offset_osu = 0;


   reg_offset_osu    = REG_OFFSET_MC_OSU_BROADCAST;
           
   HWIO_OUTXF2 (reg_offset_osu, OSU_QFI_CLK_CTRL, QFI_CLK_SET, QFI_CLK_OVERRIDE_EN, (~enable & 0x1), (~enable & 0x1));
   HWIO_OUTX (reg_offset_osu, OSU_LOAD_CONFIG, 0x1);

}

void MC_Power_Down_Ctrl (SHRM_DDR_STRUCT *shrm_ddr_ptr, DDR_CHIPSELECT chip_select, uint8 enable)
{
    uint8   index               = 0;
    uint32  reg_offset_mpe      = REG_OFFSET_MC_MPE_BROADCAST;
    
    if(shrm_ddr_ptr->ddr_runtime.power_down_enable == 1)
    {
        for(index = 0; index < 4; index++)
        {
            if((chip_select & DDR_CS0) != 0)
            {
                HWIO_OUTXFI2(reg_offset_mpe, MPE_APM_OPT_CFG_m_CS_n, index, 0, IDLE_POWER_DOWN_EN, enable);
                HWIO_OUTXFI2(reg_offset_mpe, MPE_APM_OPT_CFG_m_CS_n, index, 0, ACTIVE_POWER_DOWN_EN, enable);
            }
            
            if((chip_select & DDR_CS1) != 0)
            {
                HWIO_OUTXFI2(reg_offset_mpe, MPE_APM_OPT_CFG_m_CS_n, index, 1, IDLE_POWER_DOWN_EN, enable);
                HWIO_OUTXFI2(reg_offset_mpe, MPE_APM_OPT_CFG_m_CS_n, index, 1, ACTIVE_POWER_DOWN_EN, enable);
            }
        }
        
        // Set MPE_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect 
        HWIO_OUTX(reg_offset_mpe, MPE_LOAD_CONFIG, 0x1);
        
        // Poll for LOAD_CONFIG to become zero
        while(HWIO_INX(reg_offset_mpe, MPE_LOAD_CONFIG) != 0x0);
    }
}

void MC_Clock_Stop_Ctrl (SHRM_DDR_STRUCT *shrm_ddr_ptr, uint8 enable)
{
    uint8   index               = 0;
    uint32  reg_offset_mpe      = REG_OFFSET_MC_MPE_BROADCAST;
    
    if(shrm_ddr_ptr->ddr_runtime.clock_stop_enable == 1)
    {
		for(index = 0; index < 4; index++)
        {
			
			HWIO_OUTXFI(reg_offset_mpe, MPE_APM_OPT_CTRL_m, index, CLOCK_STOP_EN, enable);
        
        // Set MPE_LOAD_CONFIG[LOAD_CONFIG] = 1 for the above settings to take effect 
        HWIO_OUTX(reg_offset_mpe, MPE_LOAD_CONFIG, 0x1);
        
			// Poll for LOAD_CONFIG to become zero
			while(HWIO_INX(reg_offset_mpe, MPE_LOAD_CONFIG) != 0x0);
		}
    }
}

uint32 MC_MR_Read (uint32 ch_offset, DDR_CHIPSELECT chip_select, uint32 mr_addr)
{
   uint32 reg_data        = 0;
   uint32 reg_offset_shke = 0;
 
   reg_offset_shke = ch_offset + MC_SHKE_OFFSET;
   
   // Single command triggerring, only use SHKE_CMD_SET_CFG_0
   MC_Cmd_Set_Cfg (ch_offset, chip_select, 0, CMD_MRR, mr_addr, 0x0);
   // No wait time needed
   MC_Cmd_Stagger_Cfg (ch_offset, 0, 0, 0, 0);
   // Trigger command and wait for completion
   MC_Cmd_Trigger(ch_offset, 0x1); // Choose CMD_SET_CFG_0 in bit 0

   if((chip_select & DDR_CS0) != 0) {
      reg_data = HWIO_INX (reg_offset_shke, SHKE_MR_RDATA_RANK0);
   }
   else if((chip_select & DDR_CS1) != 0) {
      reg_data = HWIO_INX (reg_offset_shke, SHKE_MR_RDATA_RANK1);
   }
   return (reg_data);
}

void MC_PXI_Payload_Config (PXI_BLOCK block, uint8 num,\
                            uint32 rpt_cntr, uint32 nop_cntr, uint32 task_cntr,\
                            uint32 task_ptr, uint32 error_clr)
{
   uint32 value          = 0;
   uint32 reg_offset_pxi_secure = 0;
      
   value = ((error_clr << 30) & 0x40000000) /* PAYLOAD[30]    = ERROR_CLR */ |\
           ((task_ptr  << 25) & 0x3E000000) /* PAYLOAD[29:25] = TASK_PTR  */ |\
           ((task_cntr << 20) & 0x01F00000) /* PAYLOAD[24:20] = TASK_CNTR */ |\
           ((nop_cntr  << 12) & 0x0003F000) /* PAYLOAD[17:12] = NOP_CNTR  */ |\
           ((rpt_cntr  <<  0) & 0x000003FF) /* PAYLOAD[9:0]   = RPT_CNTR  */ ;  

   reg_offset_pxi_secure = REG_OFFSET_MC_PXI_SECURE_BROADCAST; 

   switch (block) {
      case PXI_CAOP:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CAOP_n_PAYLOAD, num, value);
         break;
      case PXI_CAWD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CADATA_n_PAYLOAD, num, value);
         break;	 
      case PXI_DQOP:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQOP_n_PAYLOAD, num, value);
         break;	 
      case PXI_DQWD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_PAYLOAD, num, value);
         break;	 
      case PXI_DQRD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_PAYLOAD, num, value);
         break;	 
      default:
         break;
   }
}

void MC_PXI_Latency_Config (void)
{
   uint32 rl             = 0;
   uint32 wl             = 0;
   uint32 value          = 0;
   uint32 reg_offset_pxi = 0;
   uint32 reg_offset_dtc = 0;
 
   /* Read MC latency settings and assume the same settings apply to all channels */
   reg_offset_dtc = REG_OFFSET_MC_DTC_BROADCAST;         
   rl = HWIO_INXF(reg_offset_dtc, DTC_COL_TIMING_CFG_RL, IN_CK);
   wl = HWIO_INXF(reg_offset_dtc, DTC_COL_TIMING_CFG_WL, IN_CK); 

   value = (((wl + 4) << 8) & 0x00003F00) /* RDWR_LATENCY[13:8] = WR_LATENCY */ |\
            ((rl + 2)       & 0x0000003F) /* RDWR_LATENCY[5:0]  = RD_LATENCY */ ;

   reg_offset_pxi = REG_OFFSET_MC_PXI_BROADCAST;
   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_RDWR_LATENCY, value);
}

//anything that needs to be updated after frequency switch
void MC_PXI_Update_Config(SHRM_DDR_STRUCT *shrm_ddr_ptr)
{   
    uint32 reg_offset_osu            = REG_OFFSET_MC_OSU_BROADCAST;
    
	uint8 sw_status = 0;
    
    uint8 cs = 0;
    uint8 qfi_rd_delay = 0;
    uint8 qfi_rd_delay_mc_cycles = 0;
    uint8 qfi_rd_delay_phy_cycles = 0;
    uint8 qfi_rd_delay_p0_p1 = 0;
    uint8 qfi_wr_delay = 0;
    uint8 qfi_wr_delay_mc_cycles = 0;
    uint8 qfi_wr_delay_phy_cycles = 0;
    uint8 qfi_wr_delay_p0_p1 = 0;
    
    uint32 curr_clk_khz = shrm_ddr_ptr->misc.current_clk_in_kHz; //unused variable
    DDR_CHIPSELECT qualified_cs = shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].populated_chipselect;
    
    MC_PXI_Latency_Config ();
	
	//Required as the o_ch*_cabo_caphy_qfi_clk gets enabled one cycle late so SSP stage does not propagate cmd in 1st clk cycle
    //early_cmd_valid, early_data_valid is 0 in case outbound_sel=1 and anyways mpe stuff is used and not the one after pxi_mux
    HWIO_OUTXF2 (reg_offset_osu,OSU_QFI_CLK_CTRL,QFI_CLK_SET,QFI_CLK_OVERRIDE_EN,0x1,0x1);
		 
    HWIO_OUTX (reg_offset_osu,OSU_LOAD_CONFIG, 0x1);

    do {
        sw_status = HWIO_INX (reg_offset_osu, OSU_LOAD_CONFIG);
    } while(sw_status);
    ///////////////////////////////////////////////
    
    //Set IE/OE depending on QFI delay
    for (cs = 0; cs < NUM_CS; cs++)
    {
        if ((qualified_cs >> cs) & 0x1)
        {
            qfi_rd_delay = (HWIO_INXI (REG_OFFSET_MC_DTC_BROADCAST, DTC_OSU_TIMING_CFG_QFI_2P0_RD_OPCODE_OFFSET_n, cs) >> 20) & 0xFF;
            qfi_wr_delay = (HWIO_INXI (REG_OFFSET_MC_DTC_BROADCAST, DTC_OSU_TIMING_CFG_QFI_2P0_WR_OPCODE_OFFSET_n, cs) >> 20) & 0xFF;
            
            qfi_wr_delay_phy_cycles = qfi_wr_delay + 2;
           
            qfi_wr_delay_mc_cycles = qfi_wr_delay_phy_cycles >> 1;
            qfi_wr_delay_p0_p1 = qfi_wr_delay_phy_cycles % 2;
            
            if (curr_clk_khz > F_RANGE_6)
                qfi_rd_delay_phy_cycles = qfi_rd_delay + 4;
            else if (curr_clk_khz > F_RANGE_5)
                qfi_rd_delay_phy_cycles = qfi_rd_delay + 3;
            else if (curr_clk_khz > F_RANGE_4)
                qfi_rd_delay_phy_cycles = qfi_rd_delay + 3;
            else if (curr_clk_khz > F_RANGE_3)
                qfi_rd_delay_phy_cycles = qfi_rd_delay + 3;
            else if (curr_clk_khz > F_RANGE_2)
                qfi_rd_delay_phy_cycles = qfi_rd_delay + 2;
            else if (curr_clk_khz > F_RANGE_1)
                qfi_rd_delay_phy_cycles = qfi_rd_delay + 6;
            else if (curr_clk_khz > F_RANGE_0)
                qfi_rd_delay_phy_cycles = qfi_rd_delay + 6;
            else
                qfi_rd_delay_phy_cycles = qfi_rd_delay + 6;
            
            qfi_rd_delay_mc_cycles = qfi_rd_delay_phy_cycles >> 1;
            qfi_rd_delay_p0_p1 = qfi_rd_delay_phy_cycles % 2;
           
            if (cs)
            {
                MC_PXI_Payload_Config(PXI_DQOP, PXI_RD_CS1_START_ADDR, 0x0, PXI_ACT_DELAY + qfi_rd_delay_mc_cycles, PXI_2_CYCLE_CMD, 0x0, 0x0); //Dummy Read
                
                if (qfi_rd_delay_p0_p1) //P1R1
                {
                    MC_PXI_Payload_Config(PXI_DQOP, (PXI_RD_CS1_START_ADDR + 1), PXI_REPEAT_CNT, (PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD), PXI_2_CYCLE_CMD, 0x8, 0x0); //32 BL32 P1R1 read opcodes
                }
                else  //P0R1
                {
                    MC_PXI_Payload_Config(PXI_DQOP, (PXI_RD_CS1_START_ADDR + 1), PXI_REPEAT_CNT, (PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD), PXI_2_CYCLE_CMD, 0x4, 0x0); //32 BL32 P0R1 read opcodes
                }
                
                MC_PXI_Payload_Config(PXI_DQOP, PXI_WR_CS1_START_ADDR, 0x0, (PXI_ACT_DELAY + qfi_wr_delay_mc_cycles), PXI_2_CYCLE_CMD, 0x0, 0x0); //Dummy Write R1
                
                if (qfi_wr_delay_p0_p1) //P1R1
                {
                    MC_PXI_Payload_Config(PXI_DQOP, (PXI_WR_CS1_START_ADDR + 1), PXI_REPEAT_CNT, (PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD), PXI_2_CYCLE_CMD, 0x6, 0x0); //32 BL32 P1R1 Write opcodes
                }
                else
                {
                    MC_PXI_Payload_Config(PXI_DQOP, (PXI_WR_CS1_START_ADDR + 1), PXI_REPEAT_CNT, (PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD), PXI_2_CYCLE_CMD, 0x2, 0x0); //32 BL32 P0R1 Write opcodes
                }
            }
            else
            {
                MC_PXI_Payload_Config(PXI_DQOP, PXI_RD_CS0_START_ADDR, 0x0, PXI_ACT_DELAY + qfi_rd_delay_mc_cycles, PXI_2_CYCLE_CMD, 0x0, 0x0); //Dummy Read
                
                if (qfi_rd_delay_p0_p1) //P1R0
                {
                    MC_PXI_Payload_Config(PXI_DQOP, (PXI_RD_CS0_START_ADDR + 1), PXI_REPEAT_CNT, (PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD), PXI_2_CYCLE_CMD, 0x7, 0x0); //32 BL32 P1R0 read opcodes
                }
                else  //P0R0
                {
                    MC_PXI_Payload_Config(PXI_DQOP, (PXI_RD_CS0_START_ADDR + 1), PXI_REPEAT_CNT, (PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD), PXI_2_CYCLE_CMD, 0x3, 0x0); //32 BL32 P0R0 read opcodes
                }
                
                MC_PXI_Payload_Config(PXI_DQOP, PXI_WR_CS0_START_ADDR, 0x0, (PXI_ACT_DELAY + qfi_wr_delay_mc_cycles), PXI_2_CYCLE_CMD, 0x0, 0x0); //Dummy Write R0
                
                if (qfi_wr_delay_p0_p1) //P1R0
                {
                    MC_PXI_Payload_Config(PXI_DQOP, (PXI_WR_CS0_START_ADDR + 1), PXI_REPEAT_CNT, (PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD), PXI_2_CYCLE_CMD, 0x5, 0x0); //32 BL32 P1R0 Write opcodes
                }
                else  //P0R0
                {
                    MC_PXI_Payload_Config(PXI_DQOP, (PXI_WR_CS0_START_ADDR + 1), PXI_REPEAT_CNT, (PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD), PXI_2_CYCLE_CMD, 0x1, 0x0); //32 BL32 P0R0 Write opcodes
                }
            }
        }
    }

//below commented because one DQWD delay value seems to work for all freqs    
/*    
    // (uint8 num, uint8 rpt_ctr, uint8 nop_ctr, uint8 task_ctr, uint8 task_ptr, uint8 error_clr) 
    if (curr_clk_khz > F_RANGE_6)
    {
        MC_PXI_Payload_Config(PXI_DQWD, 0, 0x0, (PXI_ACT_DELAY + 10), PXI_2_CYCLE_CMD, 0x0, 0x0);//NOP
    }
    else if (curr_clk_khz > F_RANGE_5)
    {
        MC_PXI_Payload_Config(PXI_DQWD, 0, 0x0,  (PXI_ACT_DELAY + 9), PXI_2_CYCLE_CMD, 0x0, 0x0);//NOP
    }
    else if (curr_clk_khz > F_RANGE_4)
    {
        MC_PXI_Payload_Config(PXI_DQWD, 0, 0x0,  (PXI_ACT_DELAY + 8), PXI_2_CYCLE_CMD, 0x0, 0x0);//NOP
    }
    else if (curr_clk_khz > F_RANGE_3)
    {
        MC_PXI_Payload_Config(PXI_DQWD, 0, 0x0,  (PXI_ACT_DELAY + 7), PXI_2_CYCLE_CMD, 0x0, 0x0);//NOP
    }
    else if (curr_clk_khz > F_RANGE_2)
    {
        MC_PXI_Payload_Config(PXI_DQWD, 0, 0x0,  (PXI_ACT_DELAY + 6), PXI_2_CYCLE_CMD, 0x0, 0x0);//NOP
    }
    else if (curr_clk_khz > F_RANGE_1)
    {
        MC_PXI_Payload_Config(PXI_DQWD, 0, 0x0,  (PXI_ACT_DELAY + 5), PXI_2_CYCLE_CMD, 0x0, 0x0);//NOP
    }
    else if (curr_clk_khz > F_RANGE_0)
    {
        MC_PXI_Payload_Config(PXI_DQWD, 0, 0x0,  (PXI_ACT_DELAY + 4), PXI_2_CYCLE_CMD, 0x0, 0x0);//NOP
    }
    else
    {
        MC_PXI_Payload_Config(PXI_DQWD, 0, 0x0,  (PXI_ACT_DELAY + 3), PXI_2_CYCLE_CMD, 0x0, 0x0);//NOP
    }
*/
}





