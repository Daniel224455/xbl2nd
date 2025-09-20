/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

#ifndef _MC_PXI_H
#define _MC_PXI_H

#include "mc_common.h"
#include "ddrss.h"

struct pxi_sel_struct
{
   uint8 raw_open;
   uint8 in_sel;
   uint8 out_sel;
};

struct pxi_opcode_sel_struct
{
  uint8 dly_caop_one_clk;
  uint8 dly_cawd_one_clk;
  uint8 dly_dqop_one_clk;
  uint8 dm_done_in_mc_sel;
  uint8 cq_timestamp_sel;
  uint8 dq_timestamp_sel;
  uint8 ca_opcode_sel;
  uint8 dq_opcode_sel;
};
 
struct pxi_start_addr_struct
{
   uint8 caop_addr;
   uint8 cawd_addr;
   uint8 dqop_addr;
   uint8 dqwd_addr;
   uint8 dqrd_addr;
};

struct pxi_setting_struct
{
   uint8                        clk_on;
   struct pxi_sel_struct        pxi_sel;
   struct pxi_opcode_sel_struct opcode_sel;
   struct pxi_start_addr_struct start_addr; 
};

extern struct pxi_setting_struct pxi_setting_table[];

 
/*==============================================================================
                                  FUNCTIONS
==============================================================================*/
void MC_PXI_Task_Config (PXI_BLOCK block, uint8 num,\
                         uint32 alu_cfg, uint32 data0_ptr, uint32 data1_ptr,\
                         uint32 data0_sel, uint32 data1_sel, uint32 sel_spg,\
                         uint32 spg_mode, uint32 txvalid, uint32 mux_mode);

void MC_PXI_Opcode_Config (PXI_BLOCK block, uint8 num,\
                           uint32 precalc_tstamp, uint32 precalc_opcode, uint32 phase,\
                           uint32 bl, uint32 cs, uint32 rnw_or_cke, uint32 op_type, uint32 ck_en);

void MC_PXI_DQ_Data_Config (PXI_BLOCK block, void *data);

void MC_PXI_CA_Data_Config (PXI_BLOCK block, uint8 num,\
                            uint32 ca_p0, uint32 ca_p1, uint32 cs_p0, uint32 cs_p1);

void MC_PXI_GPR_Config (PXI_BLOCK block, uint8 num, uint32 gpr);

void MC_PXI_Setting_Config (uint8 setting_idx);

void MC_PXI_Control_Config (uint32 ch_offset, uint32 go, uint32 step, uint32 abort);

void MC_PXI_RD_Beat_Status_Clear (uint32 ch_offset);

void MC_PXI_ELOG_Config (uint8 enable, PXI_ERR_MODE cnt_mode, uint32 cnt_wdw, uint32 threshold);

void MC_PXI_MISR_Config (uint8 enable, uint32 seed_msb, uint32 seed_lsb);

void MC_PXI_MISR_Calc_1 (uint32 * misr_data, uint32 misr_input);

uint32 MC_PXI_MISR_Calc_2 (uint32 * misr_data, uint32 misr_input);

uint8 MC_PXI_MISR_Compare (uint32 ch_offset, uint32 beats, uint32 * misr_in_lsb, uint32 * misr_in_msb);

void MC_PXI_Payload_Start_Address_Config(uint32 ch_offset, uint8 cs, uint8 wr_rd);

void MC_PXI_Execute_Tables(uint32 ch_offset, uint8 cs, uint8 wr_rd);

void MC_PXI_Pre_Training_Config();

void MC_PXI_Post_Training_Config();

#endif
