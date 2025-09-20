/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
#include "ddr_phy.h"
#include "ddrss.h"
#include <string.h>

void Training_DDRSS_DCC(BOOT_DDR_STRUCT *ddr, 
                        DDR_CHANNEL channel, 
                        DDR_CHIPSELECT chip_select, 
                        uint32 freq, 
                        uint8 prfs_index, 
                        training_params_t *training_params_ptr,
                        uint8             *local_vars
                        )
{
    
    PHY_BUS  dq     = DQ;
    PHY_BUS  ca     = CA;
 
    uint8  pll = 0;	
    uint8 freq_eq_max_training;

    freq_eq_max_training    = training_params_ptr->DCC_Training_Switchboard[prfs_index][_DCC_Mon];

    if (freq_eq_max_training)
    { 
      ddr_printf(DDR_NORMAL, "START: DCC training \n"); 




      for (pll=0;pll<NUM_PLL;pll++)
      {
        // DCC Training for WRLVL CDC, WR90 CDC and IO DCC at max freq
        DDRSS_dcc_boot (ddr, 
                        channel,
                        chip_select,                        
                        dq, 
                        ca,
						training_params_ptr,
                        DCC_TRAINING_WRLVL_WR90_IO,
                        freq,
                        prfs_index);
      
        // Switch to the opposite PLL and repeat the DCC training
        ddr_external_set_clk_speed (freq);  
      }            
      ddr_printf(DDR_NORMAL, "\nEND: DCC training \n");
     
    }
     // DDRSS_MC_cnt_clk_dis(ddr,channel,chip_select);  
 	 
}

void DDRSS_dcc_boot (BOOT_DDR_STRUCT *ddr, 
                     DDR_CHANNEL channel, 
                     DDR_CHIPSELECT chip_select,
                     PHY_BUS dq, 
                     PHY_BUS ca,
                     training_params_t *training_params_ptr,					 
                     uint32 training_sel, 
                     uint32 clk_freq_khz,
                     uint8 prfs_index)
{
    uint32 ch               = 0;
    chip_select             = (DDR_CHIPSELECT) (chip_select & (DDR_CHIPSELECT)ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].populated_chipselect);
    uint32 channels          = ddr->shrm_ddr_ptr->misc.enabled_channels;       
	
    uint8  byte_lane        = 0;
    uint32 dq0_ddr_phy_base = 0;
    // uint32 ca0_ddr_phy_base = 0;
    uint16 wrlvl_stat_dq    = 0; 
    uint16 t90_stat_dq      = 0;
    uint16 iodqs_stat_dq    = 0;
    int32 wrlvl_dcc_adjust = 0;  
    int32 t90_dcc_adjust   = 0;
    int32 io_dcc_adjust    = 0;
    //uint8 cs;    

    uint8  ca_lane          = 0;
    uint16 wrlvl_stat_ca    = 0; 
    uint16 t90_stat_ca      = 0;
    uint16 iodqs_stat_ca    = 0;

    uint32 inst_offset      = 0;
    uint32 inst_offset_freq = 0;
	
    uint8  pll              = 0;	
    uint8  index            = 0;	
    uint32 reg_offset_ddrcc = 0;
    uint32 inst_offset_lut  = 0;
    uint32 lut              =0;  
    uint32 TIMESTAMP_COUNTER_OFFSET_temp  = 0;
   
    // Pointer for training data structure
	//Commented due to unused variable Makefile error.
   // training_data *training_data_ptr;
   // training_data_ptr = (training_data *)(&ddr->training_results.training_data);    

    // Loop through selected channel(s)
  for(ch = 0; ch < NUM_CH; ch++)
    {
        // Skip channels which are not enabled
        if(((channels >> ch) & 0x1) == 0x0) continue;
            #if (DSF_DCC_TRAINING_DQ_DCC_LOG || DSF_DCC_TRAINING_CA_DCC_LOG)
            ddr_printf(DDR_NORMAL,"\n");
            #endif 
      
            // Base pointer to DDR_PHY
            dq0_ddr_phy_base = REG_OFFSET_DDR_PHY_CH(ch) + DQ0_DDR_PHY_OFFSET;
            // ca0_ddr_phy_base = REG_OFFSET_DDR_PHY_CH(ch) + CA0_DDR_PHY_OFFSET;
            
            // Base pointer to DDR_CC
            reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;

            // Determine the current active pll
            pll = HWIO_INXF (reg_offset_ddrcc, DDR_CC_DDRCC_PLLCTRL_STATUS, ACTIVE_PLL);
            
            #if DSF_DCC_TRAINING_DQ_DCC_LOG
                ddr_printf(DDR_NORMAL,"    Active DCC PLL %x\n",pll);
            #endif 

            // Loop through all PHY DQs (byte_lanes)
            for (byte_lane=0;byte_lane<dq;byte_lane++) 
            {
                // Set individual PHY DQ base address
                inst_offset = dq0_ddr_phy_base + (byte_lane * DDR_PHY_OFFSET);
                
                // Set all DCC registers to "0" CM as well as FPM based for trageted frequncy
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_0_CFG, CM_MAN_ADJ, 0x0);
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_3_CFG, CM_ADJ_DQ_SW_OVRD_VAL, 0x0);
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_1_CFG, IO_MAN_ADJ , 0x0);
                HWIO_OUTXF2 (inst_offset + (4 * index), DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG, CMDCC_ADJ_WRLVL, CMDCC_ADJ_WR, 0x0, 0x0);
                HWIO_OUTXF2 (inst_offset + (4 * index), DDR_PHY_DDRPHY_DCCEXT_PADDCC_0_CTL_CFG,PADDCC_ADJ_DQS, PADDCC_ADJ_DQS_R1, 0x0, 0x0);
                HWIO_OUTXF (inst_offset + (4 * index), DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG, CMDCC_ADJ_WRLVL_PLL1, 0x0);  

                //Turn on continuous DDRCC clock
                HWIO_OUTXF2(reg_offset_ddrcc, DDR_CC_DDRCC_TOP_CTRL_CFG, DDRCC_LVDS_CLK_ENABLE_OVRD_SEL,
                                                                       DDRCC_LVDS_CLK_ENABLE_OVRD_VAL,
                                                                       0x1, 0x1);        
                // Turn on continuous GCC clock (per byte lane)
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_CMHUB_TOP_0_CFG, TRAFFIC_CGC_EN, 0x1);
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, TMIN_WR_DATA_CGC_BYPASS,
                                                                                TMIN_WR_DATA_CGC_BYPASS_MUX_SEL,
                                                                                0x1,0x1); 
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, T90_WR_DATA_CGC_BYPASS,
				                                                                  T90_WR_DATA_CGC_BYPASS_MUX_SEL,
                                                                                  0x1,0x1); 
																				
                //*new* Enable DCC internal clock for CM DCC ///only for IO --doesn't hurt
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, CM_SW_OVRRD_UPDT_INT_CLK_CGC_EN_SEL, 
                                                                      CM_SW_OVRRD_UPDT_INT_CLK_CGC_EN,
                                                                      0x1,0x1);               
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, IO_SW_OVRRD_UPDT_INT_CLK_CGC_EN_SEL,
                                                                      IO_SW_OVRRD_UPDT_INT_CLK_CGC_EN,
                                                                      0x1,0x1);     
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, DCC_MON_CLK_DIS,
                                                                                RD_DQS_DCC_CLK_SEL,
                                                                                0x0, 0x0);
            
                // Turn off CM and IO FPM
                HWIO_OUTXF2(inst_offset,DDR_PHY_DDRPHY_DCC_TOP_4_CFG, CM_DCC_ADJ_FPM_CNTL, 
                                                                    PAD_DCC_ADJ_FPM_CNTL, 
                                                                    0x0, 0x0);
            
                // WRLVL DCC training
                if (training_sel & 0x1) 
                {      
                    // Start WRLVL DCC Training and poll for completion
                    wrlvl_stat_dq = DDR_PHY_rtn_training_dcc_wrlvl(ddr,inst_offset);               
                    wrlvl_dcc_adjust = DDRSS_wr_csr2dcc(wrlvl_stat_dq);
                   
                     
                    #if DSF_DCC_TRAINING_DQ_DCC_LOG
                        ddr_printf(DDR_NORMAL,"    Channel %u Byte %u WRLVL DQS DCC Adjustment: %x\n",ch,byte_lane,wrlvl_stat_dq);
                    #endif
                  // ============================================================================================================================
                  // ==========================================             S P E C   C h e c k            ======================================
                  // ============================================================================================================================

                    if (((prfs_index == MAX_TRAINING_FREQ_INDEX) || (prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))) && pll == 0)
                    {
                        g_sed_struct_val.training = g_str_dcc_pll0;
                    }
                    else
                    {
                        g_sed_struct_val.training = g_str_dcc_pll1;
                    }                         


                        //g_sed_struct_val.training = tmp_string_p;
                        g_sed_struct_val.param_name = "WRLVL_DQS_DCC";
                        g_sed_struct_val.param_steps = wrlvl_dcc_adjust;
                        g_sed_struct_val.param_value = wrlvl_dcc_adjust * FINE_STEP_IN_PS;
                        g_sed_struct_val.param_units = g_str_unit_ps;
                        g_sed_struct_val.vref_type = g_str_default;
                        g_sed_struct_val.vref_steps = DC;
                        g_sed_struct_val.vref_value = DC;
                        g_sed_struct_val.freq = clk_freq_khz;
                        g_sed_struct_val.ch = ch;
                        g_sed_struct_val.cs = DC;
                        g_sed_struct_val.byte_lane = byte_lane;
						if(wrlvl_dcc_adjust < 0)
								wrlvl_dcc_adjust = wrlvl_dcc_adjust*-1;
                        if((wrlvl_dcc_adjust >= training_params_ptr->ddr_abort.max_dqs_cm_dcc)&&
                             (prfs_index == MAX_TRAINING_FREQ_INDEX))  
                         {
                              g_abort_flag = 1;
                              training_params_ptr->small_eye_abort = 1;                        
                         }
                        if ((((g_abort_flag)&&(training_params_ptr->ddr_abort.max_dqs_cm_dcc_enable)) || 
                              (g_sed_flag)) && 
                              ((prfs_index==MAX_TRAINING_FREQ_INDEX)||(prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))))
                         {    
                          DDRSS_SED_prints_struct();
                         } 
                         //reset SED print struct
                        memset(&g_sed_struct_val,0,sizeof(sed_print_struct_t));                 
                  // ============================================================================================================================
                  // ==========================================         S P E C   C h e c k  E n d      =========================================
                  // ============================================================================================================================                   
                   
                }
                
                // WR90 DCC training
                if ((training_sel & 0x2) && pll==0) 
                {
                    // Start WR90 DCC Training and poll for completion
                    t90_stat_dq = DDR_PHY_rtn_training_dcc_t90(ddr, inst_offset);
                    t90_dcc_adjust = DDRSS_wr_csr2dcc(t90_stat_dq);
                    //if (t90_dcc_adjust >= 32)
                    //    t90_dcc_adjust = t90_dcc_adjust>>5;
                                  
                    #if DSF_DCC_TRAINING_DQ_DCC_LOG
                        ddr_printf(DDR_NORMAL,"    Channel %u Byte %u WR90 DQS DCC Adjustment: %x\n",ch,byte_lane,t90_stat_dq);
                    #endif
                  // ============================================================================================================================
                  // ==========================================             S P E C   C h e c k            ======================================
                  // ============================================================================================================================

                    if (((prfs_index == MAX_TRAINING_FREQ_INDEX) || (prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))) && pll == 0)
                    {
                        g_sed_struct_val.training = g_str_dcc_pll0;
                    }
                    else
                    {
                        g_sed_struct_val.training = g_str_dcc_pll1;
                    }                         


                        g_sed_struct_val.param_name = "WR90_DQS_DCC";
                        g_sed_struct_val.param_steps = t90_dcc_adjust;
                        g_sed_struct_val.param_value = t90_dcc_adjust * FINE_STEP_IN_PS;
                        g_sed_struct_val.param_units = g_str_unit_ps;
                        g_sed_struct_val.vref_type = g_str_default;
                        g_sed_struct_val.vref_steps = DC;
                        g_sed_struct_val.vref_value = DC;
                        g_sed_struct_val.freq = clk_freq_khz;
                        g_sed_struct_val.ch = ch;
                        g_sed_struct_val.cs = DC;
                        g_sed_struct_val.byte_lane = byte_lane;
						if(t90_dcc_adjust < 0)
								t90_dcc_adjust = t90_dcc_adjust*-1;
                        if((t90_dcc_adjust >= training_params_ptr->ddr_abort.max_dqs_cm_dcc)&&
                             (prfs_index == MAX_TRAINING_FREQ_INDEX))
                         {
                              g_abort_flag = 1;
                              training_params_ptr->small_eye_abort = 1;                        
                         }
                        if ((((g_abort_flag)&&(training_params_ptr->ddr_abort.max_dqs_cm_dcc_enable)) || 
                              (g_sed_flag)) && 
                              ((prfs_index==MAX_TRAINING_FREQ_INDEX)||(prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))))
                         {    
                          DDRSS_SED_prints_struct();
                         } 
                        //reset SED print struct
                        memset(&g_sed_struct_val,0,sizeof(sed_print_struct_t));
                  // ============================================================================================================================
                  // ==========================================         S P E C   C h e c k  E n d      =========================================
                  // ============================================================================================================================
                }  
              
                // IO DCC training
                //if (0)
                if ((training_sel & 0x4) && pll==0)
                {         

                    TIMESTAMP_COUNTER_OFFSET_temp = HWIO_INXF (inst_offset+(prfs_index*4), DDR_PHY_DDRPHY_QFI2EXT_PRFS_0_CNTL_0_CFG, TIMESTAMP_COUNTER_OFFSET);
                    HWIO_OUTXF (inst_offset+(prfs_index*4), DDR_PHY_DDRPHY_QFI2EXT_PRFS_0_CNTL_0_CFG, TIMESTAMP_COUNTER_OFFSET, 0x3A);

                    // Toggle DQS with BIST
                    DDRSS_BIST_DCC (ddr, ch, inst_offset, 0,clk_freq_khz);
                    // Run IO DCC and read DQS DCC status
                    iodqs_stat_dq  = DDR_PHY_rtn_training_dcc_io_dqs(ddr, inst_offset);
                      
                    //BIST exit sequence
                    HWIO_OUTXF (inst_offset ,DDR_PHY_DDRPHY_BIST_CTRL_ACTIVE_CFG, BIST_START,0x0);
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_CTRL_ACTIVE_CFG, BIST_ERR_CLR, 0x1);
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_CTRL_ACTIVE_CFG, BIST_ERR_CLR, 0x0); 
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_QFI2_CNTL_5_CFG, FSM_BIST_STATE_EN, 0x0); 
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_TOP_CGC_ACTIVE_CFG, BIST_TOP_CGC_SND_SW_CLK_ON,0x0); //Set back to default value 0
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_TOP_CGC_ACTIVE_CFG, BIST_TOP_CGC_CHK_SW_CLK_ON, 0x0); //Set back to default value 0 
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_CTRL_CFG, BIST_MODE,0x0);
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_QFI2_CNTL_5_CFG, WPRE_OFF, 0x0); //Set back to default value 0  
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_CTRL_CFG, BIST_DATA_DLY_CNT_LOAD_VAL,0x0);
                    HWIO_OUTXF3 (inst_offset,DDR_PHY_DDRPHY_BIST_CTRL_CFG, BIST_OPCODE_INT_CNT_LOAD_VAL,
                                                                            BIST_LOOP_CNT_LOAD_VAL, 
                                                                            BIST_CHK_TIMER_LOAD_VAL, 
                                                                            0x0,0,0x7);

                    HWIO_OUTXF (inst_offset+(prfs_index*4), DDR_PHY_DDRPHY_QFI2EXT_PRFS_0_CNTL_0_CFG, TIMESTAMP_COUNTER_OFFSET, TIMESTAMP_COUNTER_OFFSET_temp);
                   
                    ddr_external_set_clk_speed(clk_freq_khz); 
                    ddr_external_set_clk_speed(clk_freq_khz); 
                                      
                    #if DSF_DCC_TRAINING_DQ_DCC_LOG
                        ddr_printf(DDR_NORMAL,"    Channel %u Byte %u IO    DQS DCC Adjustment: %x\n",ch,byte_lane,iodqs_stat_dq);
                        ddr_printf(DDR_NORMAL,"\n");
                    #endif
                
                    io_dcc_adjust = DDRSS_wr_csr2dcc(iodqs_stat_dq);
					//if (io_dcc_adjust >= 32)
					//	io_dcc_adjust = io_dcc_adjust>>5;
                    
                  // ============================================================================================================================
                  // ==========================================             S P E C   C h e c k            ======================================
                  // ============================================================================================================================
                    if (((prfs_index == MAX_TRAINING_FREQ_INDEX) || (prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))) && pll == 0)
                    {
                        g_sed_struct_val.training = g_str_dcc_pll0;
                    }
                    else
                    {
                        g_sed_struct_val.training = g_str_dcc_pll1;
                    }                         

                        g_sed_struct_val.param_name = "IO_DQS_DCC";
                        g_sed_struct_val.param_steps = io_dcc_adjust;
                        g_sed_struct_val.param_value = io_dcc_adjust * FINE_STEP_IN_PS;
                        g_sed_struct_val.param_units = g_str_unit_ps;
                        g_sed_struct_val.vref_type = g_str_default;
                        g_sed_struct_val.vref_steps = DC;
                        g_sed_struct_val.vref_value = DC;
                        g_sed_struct_val.freq = clk_freq_khz;
                        g_sed_struct_val.ch = ch;
                        g_sed_struct_val.cs = DC;
                        g_sed_struct_val.byte_lane = byte_lane;
						if(io_dcc_adjust < 0)
								io_dcc_adjust = io_dcc_adjust*-1;
                        if((io_dcc_adjust >= training_params_ptr->ddr_abort.max_dqs_io_dcc)&&
                             (prfs_index == MAX_TRAINING_FREQ_INDEX))
                         {
                              g_abort_flag = 1;
                              training_params_ptr->small_eye_abort = 1;                        
                         }
                        if ((((g_abort_flag)&&(training_params_ptr->ddr_abort.max_dqs_io_dcc_enable)) || 
                              (g_sed_flag)) && 
                              ((prfs_index==MAX_TRAINING_FREQ_INDEX)||(prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))))
                         {    
                          DDRSS_SED_prints_struct();
                         } 
                         //reset SED print struct
                        memset(&g_sed_struct_val,0,sizeof(sed_print_struct_t));
                  
                  // ============================================================================================================================
                  // ==========================================         S P E C   C h e c k  E n d      =========================================
                  // ============================================================================================================================

                } // if training_sel == 4
          
                // Write the EXT DCC registers including and below the trained frequency
                if (prfs_index < LOWEST_DCC_COPY_PRFS_DRRCC)
                {
                    for (index = prfs_index+1; index > 0; )
                    {
                        index--;
                        // Add the frequency band offset to the base address
                        inst_offset_freq = inst_offset + (4 * index);
                        
                        // Write the EXT DCC registers
                        if (pll == 0)
                        {
                          HWIO_OUTXF2 (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG,
                                      CMDCC_ADJ_WRLVL, CMDCC_ADJ_WR, wrlvl_stat_dq, t90_stat_dq
                                      );
                          HWIO_OUTXF2  (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_PADDCC_0_CTL_CFG,
                                      PADDCC_ADJ_DQS,PADDCC_ADJ_DQS_R1, iodqs_stat_dq, iodqs_stat_dq
                                      );		  
                        }
                        else
                        {
                          HWIO_OUTXF  (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG,
                                      CMDCC_ADJ_WRLVL_PLL1, wrlvl_stat_dq
                                      );              
                        }
                    } // index                     
                }
                else
                {
                    for (index = prfs_index; index >= LOWEST_DCC_COPY_PRFS_DRRCC; index--)
                    {
                        // Add the frequency band offset to the base address
                        inst_offset_freq = inst_offset + (4 * index);
                        
                        // Write the EXT DCC registers
                        if (pll == 0)
                        {
                          HWIO_OUTXF2 (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG,
                                      CMDCC_ADJ_WRLVL, CMDCC_ADJ_WR, wrlvl_stat_dq, t90_stat_dq
                                      );
                          HWIO_OUTXF2  (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_PADDCC_0_CTL_CFG,
                                      PADDCC_ADJ_DQS, PADDCC_ADJ_DQS_R1, iodqs_stat_dq, iodqs_stat_dq
                                      );		  
                        }
                        else
                        {
                          HWIO_OUTXF  (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG,
                                      CMDCC_ADJ_WRLVL_PLL1, wrlvl_stat_dq
                                      );              
                        }
                    } // index                 
                }

                //Turn off continuous DDRCC clock
                HWIO_OUTXF2(reg_offset_ddrcc, DDR_CC_DDRCC_TOP_CTRL_CFG, DDRCC_LVDS_CLK_ENABLE_OVRD_SEL,
                                                                         DDRCC_LVDS_CLK_ENABLE_OVRD_VAL,
                                                                         0x0,0x0);
                
                //Traffic Bypass
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_CMHUB_TOP_0_CFG, TRAFFIC_CGC_EN, 0x0);
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, TMIN_WR_DATA_CGC_BYPASS, 
                                                                                  TMIN_WR_DATA_CGC_BYPASS_MUX_SEL,
                                                                                  0x0,0x0);																				  
				HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, T90_WR_DATA_CGC_BYPASS,
				                                                                  T90_WR_DATA_CGC_BYPASS_MUX_SEL,
                                                                                  0x0,0x0); 																				  
                  //*new* Disable DCC internal clock for CM DCC
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, CM_SW_OVRRD_UPDT_INT_CLK_CGC_EN_SEL, 
                                                                        CM_SW_OVRRD_UPDT_INT_CLK_CGC_EN,
                                                                        0x0,0x0);               
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, IO_SW_OVRRD_UPDT_INT_CLK_CGC_EN_SEL,
                                                                        IO_SW_OVRRD_UPDT_INT_CLK_CGC_EN,
                                                                        0x0,0x0);             
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, DCC_MON_CLK_DIS, // CSR is inverted inside DCC
                                                                                RD_DQS_DCC_CLK_SEL,
                                                                                0x1, 0x0);
              
                // Turn on CM and IO FPM
                HWIO_OUTXF2(inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, CM_DCC_ADJ_FPM_CNTL, 
                                                                       PAD_DCC_ADJ_FPM_CNTL, 
                                                                       0x1, 0x1); 

                 //BIST to settings before DCC    
                HWIO_OUTXF2 (inst_offset,DDR_PHY_DDRPHY_QFI2_CNTL_5_CFG, WPRE_OFF, FSM_BIST_STATE_EN, 0x0,0x0); //Set back to default value 0
                HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_TOP_CGC_ACTIVE_CFG, BIST_TOP_CGC_SND_SW_CLK_ON,0x0); //Set back to default value 0
                HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_TOP_CGC_ACTIVE_CFG, BIST_TOP_CGC_CHK_SW_CLK_ON, 0x0); //Set back to default value 0
                HWIO_OUTX (inst_offset,DDR_PHY_DDRPHY_BIST_OPCODE_LUT_0_CFG, 0x0);
                HWIO_OUTX (inst_offset,DDR_PHY_DDRPHY_BIST_OPCODE_LUT_1_CFG, 0x0); 
                
                //BIST USR data
                    
                for (lut=0;lut<16;lut++) 
                {       
                    // Add LUT offset to the base address
                    inst_offset_lut = inst_offset + (lut * 0x4);
                    HWIO_OUTX (inst_offset_lut,DDR_PHY_DDRPHY_BIST_USR_DATA_LUT_0_CFG, 0x0);
                }
            
               HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_BIST_CTRL_CFG,  BIST_DATA_DLY_CNT_LOAD_VAL,0x0); 
               HWIO_OUTXF3 (inst_offset,DDR_PHY_DDRPHY_BIST_CTRL_CFG, BIST_OPCODE_INT_CNT_LOAD_VAL,    
                                                                      BIST_LOOP_CNT_LOAD_VAL, 
                                                                      BIST_CHK_TIMER_LOAD_VAL,
                                                                      0x0,0,0x7);
            } // for byte_lane
         
           // Base pointer to DDR_CC
           reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;
            
                // Determine the current active PLL
           pll = HWIO_INXF (reg_offset_ddrcc, DDR_CC_DDRCC_PLLCTRL_STATUS, ACTIVE_PLL);
           
            // Train each CA clock (CA-DQS) 
            for (ca_lane=0;ca_lane<ca;ca_lane++) 
            { 
                // Set CA Base Address
                inst_offset = REG_OFFSET_DDR_PHY_CH (ch) + CA0_DDR_PHY_OFFSET;
                 
                // Set all DCC registers to "0" CM as well as FPM based for trageted frequncy
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_0_CFG, CM_MAN_ADJ, 0x0);
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_3_CFG, CM_ADJ_DQ_SW_OVRD_VAL, 0x0);
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_1_CFG, IO_MAN_ADJ , 0x0);
                HWIO_OUTXF2 (inst_offset + (4 * index), DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG, CMDCC_ADJ_WRLVL, CMDCC_ADJ_WR, 0x0, 0x0);
                HWIO_OUTXF  (inst_offset + (4 * index), DDR_PHY_DDRPHY_DCCEXT_PADDCC_0_CTL_CFG,PADDCC_ADJ_DQS, 0x0);
                HWIO_OUTXF (inst_offset + (4 * index), DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG, CMDCC_ADJ_WRLVL_PLL1, 0x0);   

                //Turn on continuous DDRCC clock
                HWIO_OUTXF2(reg_offset_ddrcc, DDR_CC_DDRCC_TOP_CTRL_CFG, DDRCC_LVDS_CLK_ENABLE_OVRD_SEL,
                                                                      DDRCC_LVDS_CLK_ENABLE_OVRD_VAL,
                                                                      0x1,0x1);  

                // Traffic Bypass
                HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_CMHUB_TOP_0_CFG, TRAFFIC_CGC_EN, 0x1);
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, TMIN_WR_DATA_CGC_BYPASS, 
                                                                               TMIN_WR_DATA_CGC_BYPASS_MUX_SEL,
                                                                               0x1,0x1);
                 HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, T90_WR_DATA_CGC_BYPASS, 
                                                                                T90_WR_DATA_CGC_BYPASS_MUX_SEL,
                                                                                0x1,0x1);

                //*new* disable DCC internal clock for CM DCC
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, CM_SW_OVRRD_UPDT_INT_CLK_CGC_EN_SEL, 
                                                                     CM_SW_OVRRD_UPDT_INT_CLK_CGC_EN,
                                                                     0x1,0x1);               
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, IO_SW_OVRRD_UPDT_INT_CLK_CGC_EN_SEL,
                                                                     IO_SW_OVRRD_UPDT_INT_CLK_CGC_EN,
                                                                     0x1,0x1);             
                HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, DCC_MON_CLK_DIS, // CSR is inverted inside DCC
                                                                               RD_DQS_DCC_CLK_SEL,
                                                                               0x0, 0x0);

                // Turn off CM and IO FPM
                HWIO_OUTXF2(inst_offset,DDR_PHY_DDRPHY_DCC_TOP_4_CFG, CM_DCC_ADJ_FPM_CNTL, PAD_DCC_ADJ_FPM_CNTL, 0x0, 0x0); 

                // WRLVL DCC Training
                if (training_sel & 0x1) 
                { 
                    // Call WRLVL training routine
                    wrlvl_stat_ca = DDR_PHY_rtn_training_dcc_wrlvl(ddr, inst_offset);
                                          
                    #if DSF_DCC_TRAINING_CA_DCC_LOG                
                        ddr_printf(DDR_NORMAL,"    Channel %u CA %u WRLVL CK DCC Adjustment: %x\n",ch,ca_lane,wrlvl_stat_ca);
                    #endif
                    wrlvl_dcc_adjust = DDRSS_wr_csr2dcc(wrlvl_stat_ca);
                    //if (wrlvl_dcc_adjust >= 32)
                    //    wrlvl_dcc_adjust = wrlvl_dcc_adjust>>5;

                  //  #if DSF_SMALL_EYE_DETECTION_LOG
                  //  if ((prfs_index == MAX_TRAINING_FREQ_INDEX) && pll==0)
                  //  {
                  //      tmp_string_p = g_str_dcc_pll0;
                  //  }
                  //  else
                  //  {
                  //      tmp_string_p = g_str_dcc_pll1;
                  //  }                    
                  //    DDRSS_SED_prints(tmp_string_p, "WRLVL_CK_DCC", wrlvl_dcc_adjust, wrlvl_dcc_adjust * FINE_STEP_IN_PS, g_str_unit_ps, g_str_default,1024, 1024, clk_freq_khz, ch, 1024, 1024);
                  //  #endif
                  // ============================================================================================================================
                  // ==========================================             S P E C   C h e c k            ======================================
                  // ============================================================================================================================

                    if (((prfs_index == MAX_TRAINING_FREQ_INDEX) || (prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))) && pll == 0)
                    {
                        tmp_string_p = g_str_dcc_pll0;
                    }
                    else
                    {
                        tmp_string_p = g_str_dcc_pll1;
                    }                         


                        g_sed_struct_val.training = tmp_string_p;
                        g_sed_struct_val.param_name = "WRLVL_CK_DCC";
                        g_sed_struct_val.param_steps = wrlvl_dcc_adjust;
                        g_sed_struct_val.param_value = wrlvl_dcc_adjust * FINE_STEP_IN_PS;
                        g_sed_struct_val.param_units = g_str_unit_ps;
                        g_sed_struct_val.vref_type = g_str_default;
                        g_sed_struct_val.vref_steps = DC;
                        g_sed_struct_val.vref_value = DC;
                        g_sed_struct_val.freq = clk_freq_khz;
                        g_sed_struct_val.ch = ch;
                        g_sed_struct_val.cs = ca_lane;
                        g_sed_struct_val.byte_lane = DC;
                        if(wrlvl_dcc_adjust < 0)
								wrlvl_dcc_adjust = wrlvl_dcc_adjust*-1;	
                        if((wrlvl_dcc_adjust >= training_params_ptr->ddr_abort.max_ck_cm_dcc)&&
                            (prfs_index == MAX_TRAINING_FREQ_INDEX))  
                        {
                             g_abort_flag = 1;
                             training_params_ptr->small_eye_abort = 1;                        
                        }
                        if ((((g_abort_flag)&&(training_params_ptr->ddr_abort.max_ck_cm_dcc_enable)) || 
                              (g_sed_flag)) && 
                              ((prfs_index==MAX_TRAINING_FREQ_INDEX)||(prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))))
                        {
                        DDRSS_SED_prints_struct();
                        }
                        //reset SED print struct
                        memset(&g_sed_struct_val,0,sizeof(sed_print_struct_t));                   
                  
                  // ============================================================================================================================
                  // ==========================================         S P E C   C h e c k  E n d      =========================================
                  // ============================================================================================================================
                }
               
                // WR90 DCC Training
                if ((training_sel & 0x2) && pll==0) 
                {
                // Call WR90 training routine
                    t90_stat_ca = DDR_PHY_rtn_training_dcc_t90(ddr, inst_offset);            
                                        
                    t90_dcc_adjust = DDRSS_wr_csr2dcc(t90_stat_ca);
                    //if (t90_dcc_adjust >= 32)
                    //    t90_dcc_adjust = t90_dcc_adjust>>5;
                    #if DSF_DCC_TRAINING_CA_DCC_LOG
                        ddr_printf(DDR_NORMAL,"    Channel %u CA %u WR90  CK  DCC Adjustment: %x\n",ch, ca_lane,t90_stat_ca);
                    #endif
                 // ============================================================================================================================
                  // ==========================================             S P E C   C h e c k            ======================================
                  // ============================================================================================================================
                    if (((prfs_index == MAX_TRAINING_FREQ_INDEX) || (prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))) && pll == 0)
                    {
                        tmp_string_p = g_str_dcc_pll0;
                    }
                    else
                    {
                        tmp_string_p = g_str_dcc_pll1;
                    }                         


                        g_sed_struct_val.training = tmp_string_p;
                        g_sed_struct_val.param_name = "WR90_CK_DCC";
                        g_sed_struct_val.param_steps = t90_dcc_adjust;
                        g_sed_struct_val.param_value = t90_dcc_adjust * FINE_STEP_IN_PS;
                        g_sed_struct_val.param_units = g_str_unit_ps;
                        g_sed_struct_val.vref_type = g_str_default;
                        g_sed_struct_val.vref_steps = DC;
                        g_sed_struct_val.vref_value = DC ;
                        g_sed_struct_val.freq = clk_freq_khz;
                        g_sed_struct_val.ch = ch;
                        g_sed_struct_val.cs = ca_lane;
                        g_sed_struct_val.byte_lane = DC;
                        if(t90_dcc_adjust < 0)
								t90_dcc_adjust = t90_dcc_adjust*-1;	
                        if((t90_dcc_adjust >= training_params_ptr->ddr_abort.max_ck_cm_dcc)&&
                            (prfs_index == MAX_TRAINING_FREQ_INDEX))  
                        {
                             g_abort_flag = 1;
                             training_params_ptr->small_eye_abort = 1;                        
                        }
                        if ((((g_abort_flag)&&(training_params_ptr->ddr_abort.max_ck_cm_dcc_enable)) || 
                              (g_sed_flag)) && 
                              ((prfs_index==MAX_TRAINING_FREQ_INDEX)||(prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))))
                        {
                        DDRSS_SED_prints_struct();
                        }

                        //reset SED print struct
                        memset(&g_sed_struct_val,0,sizeof(sed_print_struct_t));
                  
                  // ============================================================================================================================
                  // ==========================================         S P E C   C h e c k  E n d      =========================================
                  // ============================================================================================================================
                 
                }
           
                // IO DCC Training
                if ((training_sel & 0x4) && pll==0) 
                //if (0) 
                {              
                    // Enable PAD SW override 
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_CMIO_PAD_MODE_CFG, SW_CA_PAD_OE_OVR_MODE_DQS, 1);

                    // Disable PAD OE 
                    //HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_CMIO_PAD_OE_CFG, SW_CA_PAD_OE_OVR_DQS, 0);

                    // Enable DQS PAD OE 
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_CMIO_PAD_OE_CFG, SW_CA_PAD_OE_OVR_DQS, 1);

                    // Run IO DCC and read DQS DCC status
                    iodqs_stat_ca  = DDR_PHY_rtn_training_dcc_io_dqs(ddr, inst_offset);

                    // Disable PAD SW override 
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_CMIO_PAD_MODE_CFG, SW_CA_PAD_OE_OVR_MODE_DQS, 0);

                    // Disable DQS PAD OE 
                    HWIO_OUTXF (inst_offset,DDR_PHY_DDRPHY_CMIO_PAD_OE_CFG, SW_CA_PAD_OE_OVR_DQS, 0); 

                    io_dcc_adjust = DDRSS_wr_csr2dcc(iodqs_stat_ca) ;
                    //if (io_dcc_adjust >= 32)
                    //    io_dcc_adjust = io_dcc_adjust>>5;
                    #if DSF_DCC_TRAINING_CA_DCC_LOG
                        ddr_printf(DDR_NORMAL,"    Channel %u CA %u IO    CK  DCC Adjustment: %x\n",ch,ca_lane,iodqs_stat_ca);
                    #endif
                                   // ============================================================================================================================
                  // ==========================================             S P E C   C h e c k            ======================================
                  // ============================================================================================================================

                    if (((prfs_index == MAX_TRAINING_FREQ_INDEX) || (prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))) && pll == 0)
                    {
                        tmp_string_p = g_str_dcc_pll0;
                    }
                    else
                    {
                        tmp_string_p = g_str_dcc_pll1;
                    }                         


                        g_sed_struct_val.training = tmp_string_p;
                        g_sed_struct_val.param_name = "IO_CK_DCC";
                        g_sed_struct_val.param_steps = io_dcc_adjust;
                        g_sed_struct_val.param_value = io_dcc_adjust * FINE_STEP_IN_PS;
                        g_sed_struct_val.param_units = g_str_unit_ps;
                        g_sed_struct_val.vref_type = g_str_default;
                        g_sed_struct_val.vref_steps = DC;
                        g_sed_struct_val.vref_value = DC;
                        g_sed_struct_val.freq = clk_freq_khz;
                        g_sed_struct_val.ch = ch;
                        g_sed_struct_val.cs = ca_lane;
                        g_sed_struct_val.byte_lane = DC;
                        if(io_dcc_adjust < 0)
								io_dcc_adjust = io_dcc_adjust*-1;	
                        if((io_dcc_adjust >= training_params_ptr->ddr_abort.max_ck_io_dcc)&&
                            (prfs_index == MAX_TRAINING_FREQ_INDEX))  
                        {
                             g_abort_flag = 1;
                             training_params_ptr->small_eye_abort = 1;                        
                        }
                        if ((((g_abort_flag)&&(training_params_ptr->ddr_abort.max_ck_io_dcc_enable)) || 
                              (g_sed_flag)) && 
                              ((prfs_index==MAX_TRAINING_FREQ_INDEX)||(prfs_index == (LOWEST_DCC_COPY_PRFS_DRRCC-1))))
                        {
                        DDRSS_SED_prints_struct();
                        }
                        
                        //reset SED print struct
                        memset(&g_sed_struct_val,0,sizeof(sed_print_struct_t));                   
                  
                  // ============================================================================================================================
                  // ==========================================         S P E C   C h e c k  E n d      =========================================
                  // ============================================================================================================================
                }
               
            
                // Write the EXT DCC registers including and below the trained frequency
                if (prfs_index < LOWEST_DCC_COPY_PRFS_DRRCC)
                {
                    for (index = prfs_index+1; index > 0; )
                    {
                        index--;
                        // Add the frequency band offset to the base address
                        inst_offset_freq = inst_offset + (4 * index);
                              
                        // Write the EXT DCC registers
                        if (pll == 0)
                        {
                            HWIO_OUTXF2 (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG,
                                        CMDCC_ADJ_WRLVL, CMDCC_ADJ_WR, wrlvl_stat_ca, t90_stat_ca
                                        );
                             HWIO_OUTXF2  (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_PADDCC_0_CTL_CFG,
                                         PADDCC_ADJ_DQS, PADDCC_ADJ_DQS_R1, iodqs_stat_ca, iodqs_stat_ca
                                        );
                        }
                        else
                        {
                            HWIO_OUTXF (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG,
                                        CMDCC_ADJ_WRLVL_PLL1, wrlvl_stat_ca
                                        );              
                        }
                    } // index
                }
                else
                {
                    for (index = prfs_index; index >= LOWEST_DCC_COPY_PRFS_DRRCC; index--)
                    {
                        // Add the frequency band offset to the base address
                        inst_offset_freq = inst_offset + (4 * index);
                              
                        // Write the EXT DCC registers
                        if (pll == 0)
                        {
                            HWIO_OUTXF2 (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG,
                                        CMDCC_ADJ_WRLVL, CMDCC_ADJ_WR, wrlvl_stat_ca, t90_stat_ca
                                        );
                             HWIO_OUTXF2  (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_PADDCC_0_CTL_CFG,
                                         PADDCC_ADJ_DQS, PADDCC_ADJ_DQS_R1, iodqs_stat_ca, iodqs_stat_ca
                                        );
                        }
                        else
                        {
                            HWIO_OUTXF (inst_offset_freq, DDR_PHY_DDRPHY_DCCEXT_CMDCC_0_CTL_CFG,
                                        CMDCC_ADJ_WRLVL_PLL1, wrlvl_stat_ca
                                        );              
                        }
                    } // index 
                }
                 //Turn on continuous DDRCC clock
                 HWIO_OUTXF2(reg_offset_ddrcc, DDR_CC_DDRCC_TOP_CTRL_CFG, DDRCC_LVDS_CLK_ENABLE_OVRD_SEL,
                                                                          DDRCC_LVDS_CLK_ENABLE_OVRD_VAL,
                                                                          0x0,0x0); 
               
                 // Traffic Bypass
                 HWIO_OUTXF (inst_offset, DDR_PHY_DDRPHY_CMHUB_TOP_0_CFG, TRAFFIC_CGC_EN, 0x0);
                 HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, TMIN_WR_DATA_CGC_BYPASS, 
                                                                                   TMIN_WR_DATA_CGC_BYPASS_MUX_SEL,
                                                                                    0x0,0x0);
                 HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, T90_WR_DATA_CGC_BYPASS, 
                                                                                T90_WR_DATA_CGC_BYPASS_MUX_SEL,
                                                                                0x0,0x0);
                 
                   //*new* disable DCC internal clock for CM DCC
                 HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, CM_SW_OVRRD_UPDT_INT_CLK_CGC_EN_SEL, 
                                                                         CM_SW_OVRRD_UPDT_INT_CLK_CGC_EN,
                                                                          0x0,0x0);               
                 HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_DCC_TOP_4_CFG, IO_SW_OVRRD_UPDT_INT_CLK_CGC_EN_SEL,
                                                                         IO_SW_OVRRD_UPDT_INT_CLK_CGC_EN,
                                                                          0x0,0x0);             
                 HWIO_OUTXF2 (inst_offset, DDR_PHY_DDRPHY_CMCDCWR_TRAFFIC_BYP_CFG, DCC_MON_CLK_DIS, // CSR is inverted inside DCC
                                                                                   RD_DQS_DCC_CLK_SEL,
                                                                                   0x1, 0x0);
                   // Turn on CM and IO FPM 
                 HWIO_OUTXF2(inst_offset,DDR_PHY_DDRPHY_DCC_TOP_4_CFG, CM_DCC_ADJ_FPM_CNTL, 
                                                                       PAD_DCC_ADJ_FPM_CNTL, 
                                                                       0x1, 0x1);
             
            } // for ca
    } // for ch  

} // DDRSS_dcc_boot

