/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

#include "mc_pxi.h"
#include "mc.h"

void MC_PXI_Task_Config(PXI_BLOCK block, uint8 num,\
                         uint32 alu_cfg, uint32 data0_ptr, uint32 data1_ptr,\
                         uint32 data0_sel, uint32 data1_sel, uint32 sel_spg,\
                         uint32 spg_mode, uint32 txvalid, uint32 mux_mode)
{
   uint32 value          = 0;
   uint32 reg_offset_pxi_secure = REG_OFFSET_MC_PXI_SECURE_BROADCAST; 
      
   value = ((mux_mode  << 27) & 0x18000000) /* TASK[28:27] = MUXING_MODE */ |\
           ((txvalid   << 26) & 0x04000000) /* TASK[26]    = TXAVLID     */ |\
           ((spg_mode  << 25) & 0x02000000) /* TASK[25]    = SPG_MODE    */ |\
           ((sel_spg   << 24) & 0x01000000) /* TASK[24]    = SELECT_SPG  */ |\
           ((data1_sel << 22) & 0x00C00000) /* TASK[23:22] = DATA1_SEL   */ |\
           ((data0_sel << 20) & 0x00300000) /* TASK[21:20] = DATA0_SEL   */ |\
           ((data1_ptr << 12) & 0x000FF000) /* TASK[19:12] = DATA1_PTR   */ |\
           ((data0_ptr <<  4) & 0x00C00FF0) /* TASK[11:4]  = DATA0_PTR   */ |\
           ((alu_cfg)         & 0x0000000F) /* TASK[3:0]   = ALU_CFG     */ ;

   switch (block) {
      case PXI_CAOP:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CAOP_n_TASK, num, value);
         break;
      case PXI_CAWD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CADATA_n_TASK, num, value);
         break;	 
      case PXI_DQOP:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQOP_n_TASK, num, value);
         break;	 
      case PXI_DQWD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_TASK, num, value);
         break;	 
      case PXI_DQRD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_TASK, num, value);
         break;	 
      default:
         break;
   }
}

void MC_PXI_Opcode_Config (PXI_BLOCK block, uint8 num,\
                           uint32 precalc_tstamp, uint32 precalc_opcode, uint32 phase,\
                           uint32 bl, uint32 cs, uint32 rnw_or_cke, uint32 op_type, uint32 ck_en)
{
   uint32 value          = 0;
   uint32 reg_offset_pxi_secure = REG_OFFSET_MC_PXI_SECURE_BROADCAST; 
    
   if (block == PXI_CAOP) {
      value = ((ck_en          << 20) & 0x00100000) /* OPCODE[20]    = CK_EN          */ |\
              ((op_type        << 16) & 0x000F0000) /* OPCODE[19:16] = OP_TYPE        */ |\
              ((rnw_or_cke     << 14) & 0x0000C000) /* OPCODE[15:14] = CKE            */ |\
              ((cs             << 12) & 0x00003000) /* OPCODE[13:12] = CS             */ |\
              ((bl             << 11) & 0x00000800) /* OPCODE[11]    = BL             */ |\
              ((phase          << 10) & 0x00000400) /* OPCODE[10]    = PHASE          */ |\
              ((precalc_opcode <<  5) & 0x000003E0) /* OPCODE[9:5]   = PRECALC_OPCODE */ |\
              ((precalc_tstamp)       & 0x00C0001F) /* OPCODE[4:0]   = PRECALC_TSTAMP */ ;
   }
   else if (block == PXI_DQOP) {
      value = ((rnw_or_cke     << 14) & 0x00004000) /* OPCODE[14]    = RNW            */ |\
              ((cs             << 12) & 0x00003000) /* OPCODE[13:12] = CS             */ |\
              ((bl             << 11) & 0x00000800) /* OPCODE[11]    = BL             */ |\
              ((phase          << 10) & 0x00000400) /* OPCODE[10]    = PHASE          */ |\
              ((precalc_opcode <<  5) & 0x000003E0) /* OPCODE[9:5]   = PRECALC_OPCODE */ |\
              ((precalc_tstamp)       & 0x00C0001F) /* OPCODE[4:0]   = PRECALC_TSTAMP */ ;
   }

   switch (block) {
      case PXI_CAOP:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CAOP_n_DATA, num, value);
         break;
      case PXI_DQOP:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQOP_n_DATA, num, value);
         break;	 
      default:
         break;
   }
}

void MC_PXI_DQ_Data_Config (PXI_BLOCK block, void *data)
{
   uint8  max_idx        = 0;   
   uint8  i              = 0;
   uint32 reg_offset_pxi_secure = REG_OFFSET_MC_PXI_SECURE_BROADCAST; 
   
   max_idx = 32;

   switch (block) {
      case PXI_DQWD:
         for (i = 0; i < max_idx; i++) {
            HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, i, (*((uint64 *)data + i)) & 0xFFFFFFFF);
            HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, i, (*(uint64 *)data + i) >> 32);
         }
         break;
      case PXI_DQRD:
         for (i = 0; i < max_idx; i++) {	
            HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, i, (*((uint64 *)data + i)) & 0xFFFFFFFF);
            HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, i, (*(uint64 *)data + i) >> 32);
         }
         break;			   
      case PXI_DQDM:
         for (i = 0; i < max_idx; i++) {			
            HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DMDATA_n_DATA, i, *((uint8 *)data + i));
         }				  
         break;
      default:
         break;
   }
}

void MC_PXI_CA_Data_Config (PXI_BLOCK block, uint8 num,\
                            uint32 ca_p0, uint32 ca_p1, uint32 cs_p0, uint32 cs_p1)
{
   uint16 data           = 0;
   uint32 reg_offset_pxi_secure = REG_OFFSET_MC_PXI_SECURE_BROADCAST; 

   switch (block) {
      case PXI_CAWD:
            data = ((cs_p1 << 14) & 0xC000) /* CAWD[15:14] = CS_P1 */ |\
                   ((cs_p0 << 12) & 0x3000) /* CAWD[13:12] = CS_P0 */ |\
                   ((ca_p1 <<  6) & 0x0FC0) /* CAWD[11: 6] = CA_P1 */ |\
				   ((ca_p0      ) & 0x003F) /* CAWD[ 5: 0] = CA_P0 */ ;
            HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CADATA_n_DATA, num, data);
         break;
      default:
         break;
   }
}

void MC_PXI_GPR_Config (PXI_BLOCK block, uint8 num, uint32 gpr)
{
   uint32 reg_offset_pxi_secure = REG_OFFSET_MC_PXI_SECURE_BROADCAST; 

   switch (block) {
      case PXI_CAWD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CADATA_n_GPR, num, gpr);
         break;
      case PXI_DQWD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_GPR, num, gpr);
         break;
      case PXI_DQDM:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DMDATA_n_GPR, num, gpr);
         break;       
      case PXI_DQRD:
         HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_GPR, num, gpr);	  
      default:
         break;
   }
}

void MC_PXI_Setting_Config (uint8 setting_idx)
{
   uint32 value                       = 0;
   uint32 reg_offset_pxi              = REG_OFFSET_MC_PXI_BROADCAST;
   struct pxi_setting_struct *setting = NULL;
 
   setting = &pxi_setting_table[setting_idx];

   value = setting->clk_on;
   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_CLK_ON, value);

   value = ((setting->pxi_sel.raw_open << 2) & 0x00000004) /* PXI_SEL[2] = PXI_RAW_OPEN */ |\
           ((setting->pxi_sel.in_sel   << 1) & 0x00000002) /* PXI_SEL[1] = PXI_IN_SEL   */ |\
           ((setting->pxi_sel.out_sel      ) & 0x00000001) /* PXI_SEL[0] = PXI_OUT_SEL  */ ;
   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_SEL, value);

   value = ((setting->opcode_sel.dly_caop_one_clk  << 18) & 0x00040000) /* PXI_OPCODE_SEL[18] = DLY_CAOP_ONE_CLK  */ |\
           ((setting->opcode_sel.dly_cawd_one_clk  << 17) & 0x00020000) /* PXI_OPCODE_SEL[17] = DLY_CAWD_ONE_CLK  */ |\
           ((setting->opcode_sel.dly_dqop_one_clk  << 16) & 0x00010000) /* PXI_OPCODE_SEL[16] = DLY_DQOP_ONE_CLK  */ |\
           ((setting->opcode_sel.dm_done_in_mc_sel <<  8) & 0x00000100) /* PXI_OPCODE_SEL[8]  = DM_DONE_IN_MC_SEL */ |\
           ((setting->opcode_sel.cq_timestamp_sel  <<  5) & 0x00000020) /* PXI_OPCODE_SEL[5]  = CQ_TIMESTAMP_SEL  */ |\
           ((setting->opcode_sel.dq_timestamp_sel  <<  4) & 0x00000010) /* PXI_OPCODE_SEL[4]  = DQ_TIMESTAMP_SEL  */ |\
           ((setting->opcode_sel.ca_opcode_sel     <<  1) & 0x00000002) /* PXI_OPCODE_SEL[1]  = CQ_OPCODE_SEL     */ |\
           ((setting->opcode_sel.dq_opcode_sel          ) & 0x00000001) /* PXI_OPCODE_SEL[0]  = DQ_OPCODE_SEL     */ ;
   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, value);

   value = ((setting->start_addr.caop_addr << 24) & 0xFF000000) /* PXI_WR_START_ADDR[31:24] = CAOP */ |\
           ((setting->start_addr.cawd_addr << 16) & 0x00FF0000) /* PXI_WR_START_ADDR[23:16] = CAWD */ |\
           ((setting->start_addr.dqop_addr <<  8) & 0x0000FF00) /* PXI_WR_START_ADDR[15:8]  = DQOP */ |\
           ((setting->start_addr.dqwd_addr      ) & 0x000000FF) /* PXI_WR_START_ADDR[7:0]   = DQWD */ ;
   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, value);

   value = (setting->start_addr.dqrd_addr & 0x000000FF) /* PXI_RD_START_ADDR[7:0] = DQRD */ ;
   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_RD_START_ADDR, value);
}

/* e.g. uint8 go = PXI_DQRD | PXI_DQWD | PXI_DQOP | PXI_CAWD | PXI_CAOP; */
void MC_PXI_Control_Config (uint32 ch_offset, uint32 go, uint32 step, uint32 abort)
{
   uint32 value          = 0;
   uint32 reg_offset_pxi = 0;

   value = ((abort << 16) & 0x001F0000) /* PXI_CTL[20:16] = CAOW/CAWD/DQOP/DQWD/DQRD_ABORT */ |\
           ((step  << 8 ) & 0x00001F00) /* PXI_CTL[12:8]  = CAOW/CAWD/DQOP/DQWD/DQRD_STEP  */ |\
           ((go)          & 0x0000001F) /* PXI_CTL[4:0]   = CAOW/CAWD/DQOP/DQWD/DQRD_GO    */ ;

   reg_offset_pxi =  ch_offset + MC_PXI_OFFSET;

   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_CTL, value);
   
   if (go != 0)
       HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_CGC_CFG, CFG_CLK_HOLD_DELAY, 0x1);
   else
       HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_CGC_CFG, CFG_CLK_HOLD_DELAY, 0x0);
   
}

void MC_PXI_RD_Beat_Status_Clear (uint32 ch_offset)
{
   uint32 reg_offset_pxi = 0;

   reg_offset_pxi =  ch_offset + MC_PXI_OFFSET;
   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_RD_BEAT_CNTL, 0x1);
   HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_RD_BEAT_CNTL, 0x0);
}

/* cnt_mode = PXI_BIT_ERR_MODE or PXI_BYTE_ERR_MODE */
void MC_PXI_ELOG_Config (uint8 enable, PXI_ERR_MODE cnt_mode,\
                         uint32 cnt_wdw, uint32 threshold)
{
   uint32 value                 = 0;
   uint32 reg_offset_pxi        = REG_OFFSET_MC_PXI_BROADCAST;
   uint32 reg_offset_pxi_secure = REG_OFFSET_MC_PXI_SECURE_BROADCAST; 
   
   if (enable == 1) {
      // Disable MISR mode		 
      HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_RD_PATH_CTRL, 0x10);

      // Set error count mode and window position
      value = ((cnt_mode << 3) & 0x00000008) |\
              ((cnt_wdw      ) & 0x00000007) ;		    
      HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_ELOG_TABLE_RW, 0, value);

      // Set syndrome threshold
      HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_ELOG_TABLE_RW, 1, threshold);
   }	    
}

void MC_PXI_MISR_Config (uint8 enable, uint32 seed_msb, uint32 seed_lsb)
{
   uint32 reg_offset_pxi = REG_OFFSET_MC_PXI_BROADCAST;
   
   if (enable == 1) {
      // Disable ELOG mode		 
      HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_RD_PATH_CTRL, 0x1);
      // Enable and configure MISR	 
      HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_MISR_SEED_MSB, seed_msb);
      HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_MISR_SEED_LSB, seed_lsb);
      HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_MISR_CNTL, 0x3);
   }
   else {
      // Disable MISR
      HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_MISR_CNTL, 0x0);
   }	    
}

void MC_PXI_MISR_Calc_1 (uint32 * misr_data, uint32 misr_input)
{

   *(misr_data)     = misr_input & 1;
   *(misr_data + 1) = (misr_input >> 1)  & 1;
   *(misr_data + 2) = (misr_input >> 2)  & 1;
   *(misr_data + 3) = (misr_input >> 22) & 1;
   *(misr_data + 4) = (misr_input & 0x3FFFFF) >> 3;
   *(misr_data + 5) = (misr_input & 0xFF800000) >> 23;
}

uint32 MC_PXI_MISR_Calc_2 (uint32 * misr_data, uint32 misr_input)
{
   uint32 result;

   result = (*(misr_data) ^ *(misr_data + 1))         |\
            (*(misr_data) ^ *(misr_data + 2))         |\
            (*(misr_data + 4) << 2)                   |\
            ((*(misr_data + 3) ^ *(misr_data)) << 21) |\
            (*(misr_data + 5) << 22)                  |\
            (*(misr_data) << 31)                      ;

   return (result ^ misr_input);
}


/* Only support single channel */
uint8 MC_PXI_MISR_Compare (uint32 ch_offset, uint32 beats, uint32 * misr_in_lsb, uint32 * misr_in_msb)
{
   uint32 i              = 0;
   uint32 misr_out_lsb   = 0;
   uint32 misr_out_msb   = 0;
   uint32 data_lsb[6]    = {0};
   uint32 data_msb[6]    = {0};
   uint32 reg_offset_pxi = 0;

   reg_offset_pxi = ch_offset + MC_PXI_OFFSET;
   misr_out_lsb = HWIO_INX (reg_offset_pxi, DTTS_CSR_PXI_MISR_SEED_LSB);
   misr_out_msb = HWIO_INX (reg_offset_pxi, DTTS_CSR_PXI_MISR_SEED_MSB);
   
   MC_PXI_MISR_Calc_1(data_lsb, misr_out_lsb);
   MC_PXI_MISR_Calc_1(data_msb, misr_out_msb);

   for (i = 0; i < beats; i++) {     
      misr_out_lsb = MC_PXI_MISR_Calc_2(data_lsb, misr_in_lsb[i]);
      MC_PXI_MISR_Calc_1(data_lsb, misr_out_lsb);

      misr_out_msb = MC_PXI_MISR_Calc_2(data_msb, misr_in_msb[i]);
      MC_PXI_MISR_Calc_1(data_msb, misr_out_lsb);
   }

   // Check beat status
   while (HWIO_INX (reg_offset_pxi, DTTS_CSR_PXI_RD_BEAT_STATUS) != beats);

   // Compare calculated MISR results with design generated MISR
   if ((misr_out_lsb == HWIO_INX (reg_offset_pxi, DTTS_CSR_PXI_MISR_STATUS_LSB)) && \
       (misr_out_msb == HWIO_INX (reg_offset_pxi, DTTS_CSR_PXI_MISR_STATUS_MSB))) {
      return 0; // Return zero to indicate no error
   } 
   else {
      return 1; // Return non-zero value to indicate error
   }
}

//Set Payload Start address depending on WR/RD, CS
void MC_PXI_Payload_Start_Address_Config(uint32 ch_offset, uint8 cs, uint8 wr_rd)
{
    uint32 reg_offset_pxi              =  ch_offset + MC_PXI_OFFSET;
    uint32 reg_offset_pxi_secure       =  ch_offset + MC_PXI_SECURE_OFFSET;

    
    if (cs)
    {
        HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CADATA_n_GPR, 4, 0x80000000);   //Set GPR for CS1
        HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, CAOP, PXI_WR_CS1_START_ADDR);  //Point to WR/RD CAOP payload CS1
        
        if (wr_rd)
        {
            HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, CAWD, PXI_RD_CS1_START_ADDR);  //Point to RD CAWD payload CS1
            HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, DQOP, PXI_RD_CS1_START_ADDR);  //Point to RD DQOP payload CS1
        }
        else
        {
            HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, CAWD, PXI_WR_CS1_START_ADDR);  //Point to WR CAWD payload CS1
            HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, DQOP, PXI_WR_CS1_START_ADDR);  //Point to WR DQOP payload CS1
        }
    }
    else
    {
        HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_CADATA_n_GPR, 4, 0x40000000);   //Set GPR for CS0
        HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, CAOP, PXI_WR_CS0_START_ADDR);  //Point to WR/RD CAOP payload CS0
        
        if (wr_rd)
        {
            HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, CAWD, PXI_RD_CS0_START_ADDR);  //Point to RD CAWD payload CS0
            HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, DQOP, PXI_RD_CS0_START_ADDR);  //Point to RD DQOP payload CS0
        }
        else
        {   
            HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, CAWD, PXI_WR_CS0_START_ADDR);  //Point to WR CAWD payload CS0
            HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_WR_START_ADDR, DQOP, PXI_WR_CS0_START_ADDR);  //Point to WR DQOP payload CS0
        }
    }
}

//Execute PXI Tables
void MC_PXI_Execute_Tables(uint32 ch_offset, uint8 cs, uint8 wr_rd)
{
    uint32 reg_offset_pxi              =  ch_offset + MC_PXI_OFFSET;
    uint32 reg_offset_pxi_secure       =  ch_offset + MC_PXI_SECURE_OFFSET;
    
    MC_PXI_Payload_Start_Address_Config(ch_offset, cs, wr_rd); //Set Payload start addr
    
    if (wr_rd)
    {
        //set PRBS seed (reset RD PRBS generator)
        HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_GPR, 0, PRBS_SEED_BYTE0);
        HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_GPR, 1, PRBS_SEED_BYTE1);
        
        //Clear read counter counting number of read valid beats
        //MC_PXI_RD_Beat_Status_Clear (ch_offset);
    }
    else
    {   
        //set PRBS seed (reset WR PRBS generator)
        HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_GPR, 0, PRBS_SEED_BYTE0);
        HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_GPR, 1, PRBS_SEED_BYTE1);
    }
      
    HWIO_OUTXFI (reg_offset_pxi, DTTS_CSR_PXI_ALU_n_CTRL, 0, RD_WR, wr_rd); //set ALU to Write/Read
	  
	//PXI_RAW_OPEN, PXI_IN_SEL, PXI_OUT_SEL (Get control of bus)
    HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_SEL, 0x7);

    if (wr_rd)
        MC_PXI_Control_Config (ch_offset, 0x1D, 0x0, 0x0);  //execute PXI RD
    else
        MC_PXI_Control_Config (ch_offset, 0x1E, 0x0, 0x0);  //execute PXI_WR

    //wait for all tables to go idle
    while ((HWIO_INX (reg_offset_pxi, DTTS_CSR_PXI_STATUS) & 0x0000001F) != 0x0000001F);

	MC_PXI_Control_Config (ch_offset, 0x0, 0x0, 0x0);

	//Give control back to MC
	HWIO_OUTX (reg_offset_pxi, DTTS_CSR_PXI_SEL, 0x0);
}

//Fill All PXI tables for PXI Operation
void MC_PXI_Pre_Training_Config()
{   
    uint32 reg_offset_pxi              =  REG_OFFSET_MC_PXI_BROADCAST;
    uint32 reg_offset_pxi_secure       =  REG_OFFSET_MC_PXI_SECURE_BROADCAST;
    
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_CGC_CFG, CFG_CLK_GATE_DISABLE, 1);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_CGC_CFG, CLK_GATE_DISABLE, 1);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, DQ_OPCODE_SEL, 0x3);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, CA_OPCODE_SEL, 0x3);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, DQ_TIMESTAMP_SEL, 0x0);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, CA_TIMESTAMP_SEL, 0x0);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, DM_DONE_IN_MC_SEL, 0x1);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, DLY_DQOP_ONE_CLK, 0x0);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, DLY_CAOP_ONE_CLK, 0x0);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_OPCODE_SEL, DLY_CAWD_ONE_CLK, 0x0);
    HWIO_OUTXFI (reg_offset_pxi, DTTS_CSR_PXI_ALU_n_CTRL, 0, RRBC_SEL, 0x2);
    HWIO_OUTXFI (reg_offset_pxi, DTTS_CSR_PXI_ALU_n_VALUE, 0, INC_DEC, PXI_INC_DEC);
    HWIO_OUTXFI (reg_offset_pxi, DTTS_CSR_PXI_ALU_n_CTRL, 0, BL, 0x1);
    
    //hard coded rrbc  
    //row  = rrbc_val[16:0]
    //col  = rrbc_val[26:17]
    //ba   = rrbc_val[29:27]
    //rank = rrbc_val[31:30]
    
    // bit error count mode
    HWIO_OUTXI (reg_offset_pxi_secure ,DTTS_RAM_PXI_RD_n_ELOG_TABLE_RW, 0, 0x00000008);

    //capture first miscompare
    HWIO_OUTXI (reg_offset_pxi_secure ,DTTS_RAM_PXI_RD_n_ELOG_TABLE_RW, 1, 0x00000001);

    //disable MISR
    HWIO_OUTXF  (reg_offset_pxi, DTTS_CSR_PXI_RD_PATH_CTRL, MISR_DISABLE , 0x1); 

    // (uint8 num, uint8 rpt_ctr, uint8 nop_ctr, uint8 task_ctr, uint8 task_ptr, uint8 error_clr)
    MC_PXI_Payload_Config(PXI_CAOP, PXI_RD_WR_CS0_START_ADDR    ,            0x0,                          PXI_TCKCKEH_MAX, PXI_2_CYCLE_CMD, 0, 0x0);// CLK_ON
    MC_PXI_Payload_Config(PXI_CAOP, PXI_RD_WR_CS0_START_ADDR + 1,            0x0,                              PXI_TXP_MAX, PXI_2_CYCLE_CMD, 1, 0x0);// CKE0/1 on
    MC_PXI_Payload_Config(PXI_CAOP, PXI_RD_WR_CS0_START_ADDR + 2,            0x0,                             PXI_TRCD_MAX, PXI_2_CYCLE_CMD, 2, 0x0);// ACT R0
    MC_PXI_Payload_Config(PXI_CAOP, PXI_RD_WR_CS0_START_ADDR + 3, PXI_REPEAT_CNT, PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD, PXI_2_CYCLE_CMD, 2, 0x0);//32 BL32 RD/WR R0
    MC_PXI_Payload_Config(PXI_CAOP, PXI_RD_WR_CS1_START_ADDR    ,            0x0,                          PXI_TCKCKEH_MAX, PXI_2_CYCLE_CMD, 0, 0x0);// CLK_ON
    MC_PXI_Payload_Config(PXI_CAOP, PXI_RD_WR_CS1_START_ADDR + 1,            0x0,                              PXI_TXP_MAX, PXI_2_CYCLE_CMD, 1, 0x0);// CKE0/1 on
    MC_PXI_Payload_Config(PXI_CAOP, PXI_RD_WR_CS1_START_ADDR + 2,            0x0,                             PXI_TRCD_MAX, PXI_2_CYCLE_CMD, 3, 0x0);// ACT R1
    MC_PXI_Payload_Config(PXI_CAOP, PXI_RD_WR_CS1_START_ADDR + 3, PXI_REPEAT_CNT, PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD, PXI_2_CYCLE_CMD, 3, 0x0);//31 BL32 RD/WR R1

    // (uint8 num, uint8 alu_cfg, uint32 data0_ptr, uint32 data1_ptr uint8 data0_sel, uint8 data1_sel, uint8 sel_spg, uint8 spg_mode, uint8 txvalid,uint8 muxing_mode) 
    MC_PXI_Task_Config(PXI_CAOP, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_CAOP, 1, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_CAOP, 2, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_CAOP, 3, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);

    //(uint8 num, uint8 precalc_tstamp, uint8 precalc_opcode, uint8 phase, uint8 bl, uint8 cs, uint8 rnw_or_cke, uint8 op_type, uint8 ck_en)
    MC_PXI_Opcode_Config(PXI_CAOP, 0, 0x0,        CMD_CK_ON, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); //CLK_ON opcode
    MC_PXI_Opcode_Config(PXI_CAOP, 1, 0x0,   CMD_CKE_ALL_ON, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); //CMD_CKE_ALL_ON
    MC_PXI_Opcode_Config(PXI_CAOP, 2, 0x0, OCR_WR_P0_R0_BL1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); //OCR_WR_P0R0_BL1 //4 cycle cmd (act, write, read)
    MC_PXI_Opcode_Config(PXI_CAOP, 3, 0x0, OCR_WR_P0_R1_BL1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); //OCR_WR_P0R1_BL1

    
    // (uint8 num, uint8 rpt_ctr, uint8 nop_ctr, uint8 task_ctr, uint8 task_ptr, uint8 error_clr) 
    MC_PXI_Payload_Config(PXI_DQOP, PXI_WR_CS0_START_ADDR    ,            0x0,                        PXI_ACT_DELAY + 2, PXI_2_CYCLE_CMD, 0, 0x0); //Dummy Write R0
    MC_PXI_Payload_Config(PXI_DQOP, PXI_WR_CS0_START_ADDR + 1, PXI_REPEAT_CNT, PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD, PXI_2_CYCLE_CMD, 5, 0x0); //32 BL32 Write opcodes. tccd apart
    MC_PXI_Payload_Config(PXI_DQOP, PXI_WR_CS1_START_ADDR    ,            0x0,                        PXI_ACT_DELAY + 2, PXI_2_CYCLE_CMD, 0, 0x0); //Dummy Write R1
    MC_PXI_Payload_Config(PXI_DQOP, PXI_WR_CS1_START_ADDR + 1, PXI_REPEAT_CNT, PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD, PXI_2_CYCLE_CMD, 6, 0x0); //32 BL32 Write opcodes. tccd apart
    MC_PXI_Payload_Config(PXI_DQOP, PXI_RD_CS0_START_ADDR    ,            0x0,                        PXI_ACT_DELAY + 3, PXI_2_CYCLE_CMD, 0, 0x0); //Dummy Read R0
    MC_PXI_Payload_Config(PXI_DQOP, PXI_RD_CS0_START_ADDR + 1, PXI_REPEAT_CNT, PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD, PXI_2_CYCLE_CMD, 7, 0x0); //32 BL32 read opcodes. tccd apart    
    MC_PXI_Payload_Config(PXI_DQOP, PXI_RD_CS1_START_ADDR    ,            0x0,                        PXI_ACT_DELAY + 3, PXI_2_CYCLE_CMD, 0, 0x0); //Dummy Read R1
    MC_PXI_Payload_Config(PXI_DQOP, PXI_RD_CS1_START_ADDR + 1, PXI_REPEAT_CNT, PXI_BL32_COMMAND_DELTA - PXI_2_CYCLE_CMD, PXI_2_CYCLE_CMD, 8, 0x0); //32 BL32 read opcodes. tccd apart    
    
    // (uint8 num, uint8 alu_cfg, uint32 data0_ptr, uint32 data1_ptr uint8 data0_sel, uint8 data1_sel, uint8 sel_spg, uint8 spg_mode, uint8 txvalid, uint8 muxing_mode ) 
    MC_PXI_Task_Config(PXI_DQOP, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); //muxing_mode =0 . BL1 Do not send valid out
    MC_PXI_Task_Config(PXI_DQOP, 1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1); //BL32 write P0R0
    MC_PXI_Task_Config(PXI_DQOP, 2, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1); //BL32 write P0R1
    MC_PXI_Task_Config(PXI_DQOP, 3, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1); //BL32 read P0R0
    MC_PXI_Task_Config(PXI_DQOP, 4, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1); //BL32 read P0R1
    MC_PXI_Task_Config(PXI_DQOP, 5, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1); //BL32 write P1R0
    MC_PXI_Task_Config(PXI_DQOP, 6, 0x0, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1); //BL32 write P1R1
    MC_PXI_Task_Config(PXI_DQOP, 7, 0x0, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1); //BL32 read P1R0
    MC_PXI_Task_Config(PXI_DQOP, 8, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1); //BL32 read P1R1
    
    //(uint8 num, uint8 precalc_tstamp, uint8 precalc_opcode, uint8 phase, uint8 bl, uint8 cs, uint8 rnw_or_cke, uint8 op_type, uint8 ck_en)
    MC_PXI_Opcode_Config(PXI_DQOP, 0, 0x0, OCR_WR_P0_R0_BL1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); // OCR_WR_P0R0_BL1
    MC_PXI_Opcode_Config(PXI_DQOP, 1, 0x0, OCR_WR_P0_R1_BL1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); // OCR_WR_P0R1_BL1 
    MC_PXI_Opcode_Config(PXI_DQOP, 2, 0x0, OCR_RD_P0_R0_BL1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0); // OCR_RD_P0R0_BL1  
    MC_PXI_Opcode_Config(PXI_DQOP, 3, 0x0, OCR_RD_P0_R1_BL1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0); // OCR_RD_P0R1_BL1      
    MC_PXI_Opcode_Config(PXI_DQOP, 4, 0x0, OCR_WR_P1_R0_BL1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); // OCR_WR_P1R0_BL1 
    MC_PXI_Opcode_Config(PXI_DQOP, 5, 0x0, OCR_WR_P1_R1_BL1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); // OCR_WR_P1R1_BL1 
    MC_PXI_Opcode_Config(PXI_DQOP, 6, 0x0, OCR_RD_P1_R0_BL1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0); // OCR_RD_P1R0_BL1
    MC_PXI_Opcode_Config(PXI_DQOP, 7, 0x0, OCR_RD_P1_R1_BL1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0); // OCR_RD_P1R1_BL1 

    
    // (uint8 num, uint8 rpt_ctr, uint8 nop_ctr, uint8 task_ctr, uint8 task_ptr, uint8 error_clr) 
	MC_PXI_Payload_Config(PXI_CAWD, PXI_WR_CS0_START_ADDR    ,                0x0,                        PXI_ACT_DELAY + 3, PXI_4_CYCLE_CMD, 0, 0x0); //Activate cmd. CS0
    MC_PXI_Payload_Config(PXI_CAWD, PXI_WR_CS0_START_ADDR + 1, PXI_REPEAT_CNT - 1, PXI_BL32_COMMAND_DELTA - PXI_4_CYCLE_CMD, PXI_4_CYCLE_CMD, 2, 0x0); //31 BL32 Write cmd. CS0   
    MC_PXI_Payload_Config(PXI_CAWD, PXI_WR_CS0_START_ADDR + 2,                0x0, PXI_BL32_COMMAND_DELTA - PXI_4_CYCLE_CMD, PXI_4_CYCLE_CMD, 6, 0x0); //1 BL32 Write cmd. CS0 auto-pre
    MC_PXI_Payload_Config(PXI_CAWD, PXI_WR_CS1_START_ADDR    ,                0x0,                        PXI_ACT_DELAY + 3, PXI_4_CYCLE_CMD, 1, 0x0); //Activate cmd. CS1
    MC_PXI_Payload_Config(PXI_CAWD, PXI_WR_CS1_START_ADDR + 1, PXI_REPEAT_CNT - 1, PXI_BL32_COMMAND_DELTA - PXI_4_CYCLE_CMD, PXI_4_CYCLE_CMD, 3, 0x0); //31 BL32 Write cmd. CS1   
    MC_PXI_Payload_Config(PXI_CAWD, PXI_WR_CS1_START_ADDR + 2,                0x0, PXI_BL32_COMMAND_DELTA - PXI_4_CYCLE_CMD, PXI_4_CYCLE_CMD, 7, 0x0); //1 BL32 Write cmd. CS1 auto-pre
    MC_PXI_Payload_Config(PXI_CAWD, PXI_RD_CS0_START_ADDR    ,                0x0,                        PXI_ACT_DELAY + 3, PXI_4_CYCLE_CMD, 0, 0x0); //Activate cmd. CS0
    MC_PXI_Payload_Config(PXI_CAWD, PXI_RD_CS0_START_ADDR + 1, PXI_REPEAT_CNT - 1, PXI_BL32_COMMAND_DELTA - PXI_4_CYCLE_CMD, PXI_4_CYCLE_CMD, 4, 0x0); //31 BL32 Read cmd. CS0   
    MC_PXI_Payload_Config(PXI_CAWD, PXI_RD_CS0_START_ADDR + 2,                0x0, PXI_BL32_COMMAND_DELTA - PXI_4_CYCLE_CMD, PXI_4_CYCLE_CMD, 8, 0x0); //1 BL32 Read cmd. CS0 auto-pre 
    MC_PXI_Payload_Config(PXI_CAWD, PXI_RD_CS1_START_ADDR    ,                0x0,                        PXI_ACT_DELAY + 3, PXI_4_CYCLE_CMD, 1, 0x0); //Activate cmd. CS1
    MC_PXI_Payload_Config(PXI_CAWD, PXI_RD_CS1_START_ADDR + 1, PXI_REPEAT_CNT - 1, PXI_BL32_COMMAND_DELTA - PXI_4_CYCLE_CMD, PXI_4_CYCLE_CMD, 5, 0x0); //31 BL32 Read cmd. CS1   
    MC_PXI_Payload_Config(PXI_CAWD, PXI_RD_CS1_START_ADDR + 2,                0x0, PXI_BL32_COMMAND_DELTA - PXI_4_CYCLE_CMD, PXI_4_CYCLE_CMD, 9, 0x0); //1 BL32 Read cmd. CS1 auto-pre
    
    // (uint8 num, uint8 alu_cfg, uint32 data0_ptr, uint32 data1_ptr uint8 data0_sel, uint8 data1_sel, uint8 sel_spg, uint8 spg_mode, uint8 txvalid, uint8 muxing_mode ) 
    MC_PXI_Task_Config(PXI_CAWD, 0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);  //activate CS0
    MC_PXI_Task_Config(PXI_CAWD, 1, 0x0,  0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);  //activate CS1
    MC_PXI_Task_Config(PXI_CAWD, 2, 0x1,  0x4, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 0x1);  //WR CS0
    MC_PXI_Task_Config(PXI_CAWD, 3, 0x1,  0x6, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 0x1);  //WR CS1
    MC_PXI_Task_Config(PXI_CAWD, 4, 0x1,  0x8, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 0x1);  //RD CS0
    MC_PXI_Task_Config(PXI_CAWD, 5, 0x1,  0xA, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 0x1);  //RD CS1
    MC_PXI_Task_Config(PXI_CAWD, 6, 0x0,  0xC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);  //WR CS0 auto-pre
    MC_PXI_Task_Config(PXI_CAWD, 7, 0x0,  0xE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);  //WR CS1 auto-pre
    MC_PXI_Task_Config(PXI_CAWD, 8, 0x0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);  //RD CS0 auto-pre
    MC_PXI_Task_Config(PXI_CAWD, 9, 0x0, 0x12, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);  //RD CS1 auto-pre
    
    MC_PXI_CA_Data_Config (PXI_CAWD,  0,    PXI_ACT_1_H,      PXI_ACT_1_L, 0x1, 0x0);// ACTIVATE CS0
    MC_PXI_CA_Data_Config (PXI_CAWD,  1,    PXI_ACT_2_H,      PXI_ACT_2_L, 0x1, 0x0);// ACTIVATE CS0
    MC_PXI_CA_Data_Config (PXI_CAWD,  2,    PXI_ACT_1_H,      PXI_ACT_1_L, 0x2, 0x0);// ACTIVATE CS1
    MC_PXI_CA_Data_Config (PXI_CAWD,  3,    PXI_ACT_2_H,      PXI_ACT_2_L, 0x2, 0x0);// ACTIVATE CS1
    MC_PXI_CA_Data_Config (PXI_CAWD,  4,  PXI_WRITE_1_H,    PXI_WRITE_1_L, 0x1, 0x0);// WRITE-1 on the fly BL32 CS0
    MC_PXI_CA_Data_Config (PXI_CAWD,  5,    PXI_CAS_2_H,      PXI_CAS_2_L, 0x1, 0x0);// WRITE CAS-2 CS0
    MC_PXI_CA_Data_Config (PXI_CAWD,  6,  PXI_WRITE_1_H,    PXI_WRITE_1_L, 0x2, 0x0);// WRITE-1 on the fly BL32 CS1
    MC_PXI_CA_Data_Config (PXI_CAWD,  7,    PXI_CAS_2_H,      PXI_CAS_2_L, 0x2, 0x0);// WRITE CAS-2 CS1
    MC_PXI_CA_Data_Config (PXI_CAWD,  8,   PXI_READ_1_H,     PXI_READ_1_L, 0x1, 0x0);// READ-1 on the fly BL32 CS0
    MC_PXI_CA_Data_Config (PXI_CAWD,  9,    PXI_CAS_2_H,      PXI_CAS_2_L, 0x1, 0x0);// READ CAS-2 CS0
    MC_PXI_CA_Data_Config (PXI_CAWD, 10,   PXI_READ_1_H,     PXI_READ_1_L, 0x2, 0x0);// READ-1 on the fly BL32 CS1
    MC_PXI_CA_Data_Config (PXI_CAWD, 11,    PXI_CAS_2_H,      PXI_CAS_2_L, 0x2, 0x0);// READ CAS-2 CS1
    MC_PXI_CA_Data_Config (PXI_CAWD, 12,  PXI_WRITE_1_H, PXI_WRITE_1_L_PC, 0x1, 0x0);// WRITE-1 on the fly BL32 CS0 auto-pre
    MC_PXI_CA_Data_Config (PXI_CAWD, 13, PXI_CAS_2_H_PC,   PXI_CAS_2_L_PC, 0x1, 0x0);// WRITE CAS-2 CS0 auto-pre
    MC_PXI_CA_Data_Config (PXI_CAWD, 14,  PXI_WRITE_1_H, PXI_WRITE_1_L_PC, 0x2, 0x0);// WRITE-1 on the fly BL32 CS1 auto-pre
    MC_PXI_CA_Data_Config (PXI_CAWD, 15, PXI_CAS_2_H_PC,   PXI_CAS_2_L_PC, 0x2, 0x0);// WRITE CAS-2 CS1 auto-pre
    MC_PXI_CA_Data_Config (PXI_CAWD, 16,   PXI_READ_1_H,  PXI_READ_1_L_PC, 0x1, 0x0);// READ-1 on the fly BL32 CS0 auto-pre
    MC_PXI_CA_Data_Config (PXI_CAWD, 17, PXI_CAS_2_H_PC,   PXI_CAS_2_L_PC, 0x1, 0x0);// READ CAS-2 CS0 auto-pre
    MC_PXI_CA_Data_Config (PXI_CAWD, 18,   PXI_READ_1_H,  PXI_READ_1_L_PC, 0x2, 0x0);// READ-1 on the fly BL32 CS1 auto-pre
    MC_PXI_CA_Data_Config (PXI_CAWD, 19, PXI_CAS_2_H_PC,   PXI_CAS_2_L_PC, 0x2, 0x0);// READ CAS-2 CS1 auto-pre

    
    // (uint8 num, uint8 rpt_ctr, uint8 nop_ctr, uint8 task_ctr, uint8 task_ptr, uint8 error_clr)
    MC_PXI_Payload_Config(PXI_DQRD,  0,            0x0, 0x0,               0x0,  0, 0x1); // clears error log
#if PXI_PRBS
    //MC_PXI_Payload_Config(PXI_DQRD,  1, PXI_REPEAT_CNT, 0, PXI_BL32_TASK_CNT,  0, 0x0);//32 BL32 read
    MC_PXI_Payload_Config(PXI_DQRD,  1,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  2,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  3,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  4,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  5,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  6,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  7,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  8,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  9,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 10,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 11,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 12,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 13,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 14,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 15,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 16,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 17,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 18,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 19,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 20,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 21,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 22,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 23,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 24,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 25,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 26,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 27,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 28,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 29,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 30,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
#else
    MC_PXI_Payload_Config(PXI_DQRD,  1,              0, 0, PXI_BL32_TASK_CNT,  0, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  2,              0, 0, PXI_BL32_TASK_CNT,  1, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  3,              0, 0, PXI_BL32_TASK_CNT,  2, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  4,              0, 0, PXI_BL32_TASK_CNT,  3, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  5,              0, 0, PXI_BL32_TASK_CNT,  4, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  6,              0, 0, PXI_BL32_TASK_CNT,  5, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  7,              0, 0, PXI_BL32_TASK_CNT,  6, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  8,              0, 0, PXI_BL32_TASK_CNT,  7, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD,  9,              0, 0, PXI_BL32_TASK_CNT,  8, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 10,              0, 0, PXI_BL32_TASK_CNT,  9, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 11,              0, 0, PXI_BL32_TASK_CNT, 10, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 12,              0, 0, PXI_BL32_TASK_CNT, 11, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 13,              0, 0, PXI_BL32_TASK_CNT, 12, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 14,              0, 0, PXI_BL32_TASK_CNT, 13, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 15,              0, 0, PXI_BL32_TASK_CNT, 14, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 16,              0, 0, PXI_BL32_TASK_CNT, 15, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 17,              0, 0, PXI_BL32_TASK_CNT, 16, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 18,              0, 0, PXI_BL32_TASK_CNT, 17, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 19,              0, 0, PXI_BL32_TASK_CNT, 18, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 20,              0, 0, PXI_BL32_TASK_CNT, 19, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 21,              0, 0, PXI_BL32_TASK_CNT, 20, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 22,              0, 0, PXI_BL32_TASK_CNT, 21, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 23,              0, 0, PXI_BL32_TASK_CNT, 22, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 24,              0, 0, PXI_BL32_TASK_CNT, 23, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 25,              0, 0, PXI_BL32_TASK_CNT, 24, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 26,              0, 0, PXI_BL32_TASK_CNT, 25, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 27,              0, 0, PXI_BL32_TASK_CNT, 26, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 28,              0, 0, PXI_BL32_TASK_CNT, 27, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 29,              0, 0, PXI_BL32_TASK_CNT, 28, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQRD, 30,              0, 0, PXI_BL32_TASK_CNT, 29, 0x0);//BL32 read 15
#endif
        
    // (uint8 num, uint8 alu_cfg, uint32 data0_ptr, uint32 data1_ptr uint8 data0_sel, uint8 data1_sel, uint8 sel_spg, uint8 spg_mode, uint8 txvalid, uint8 muxing_mode ) 
#if PXI_PRBS
    MC_PXI_Task_Config(PXI_DQRD,  0, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x2, 0x2, 0x0, 0x0, 0x1, 0x1);  //prbs
#else
    MC_PXI_Task_Config(PXI_DQRD,  0, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  1, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  2, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  3, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  4, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  5, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  6, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  7, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  8, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD,  9, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 10, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 11, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 12, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 13, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 14, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 15, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 16, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 17, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 18, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 19, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 20, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 21, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 22, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 23, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 24, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 25, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 26, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 27, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 28, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQRD, 29, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  0,  PXI_UD_PATTERN_0); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  1,  PXI_UD_PATTERN_1); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  2,  PXI_UD_PATTERN_2); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  3,  PXI_UD_PATTERN_3); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  4,  PXI_UD_PATTERN_4); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  5,  PXI_UD_PATTERN_5); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  6,  PXI_UD_PATTERN_6); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  7,  PXI_UD_PATTERN_7); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  8,  PXI_UD_PATTERN_8); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY,  9,  PXI_UD_PATTERN_9); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 10, PXI_UD_PATTERN_10); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 11, PXI_UD_PATTERN_11); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 12, PXI_UD_PATTERN_12); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 13, PXI_UD_PATTERN_13); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 14, PXI_UD_PATTERN_14); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 15, PXI_UD_PATTERN_15); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 16, PXI_UD_PATTERN_16); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 17, PXI_UD_PATTERN_17); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 18, PXI_UD_PATTERN_18); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 19, PXI_UD_PATTERN_19); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 20, PXI_UD_PATTERN_20); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 21, PXI_UD_PATTERN_21); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 22, PXI_UD_PATTERN_22); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 23, PXI_UD_PATTERN_23); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 24, PXI_UD_PATTERN_24); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 25, PXI_UD_PATTERN_25); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 26, PXI_UD_PATTERN_26); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 27, PXI_UD_PATTERN_27); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 28, PXI_UD_PATTERN_28); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 29, PXI_UD_PATTERN_29); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 30, PXI_UD_PATTERN_30); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_0_2_15_ENTRY, 31, PXI_UD_PATTERN_31); 
	
	
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  0, PXI_UD_PATTERN_32); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  1, PXI_UD_PATTERN_33); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  2, PXI_UD_PATTERN_34); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  3, PXI_UD_PATTERN_35); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  4, PXI_UD_PATTERN_36); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  5, PXI_UD_PATTERN_37); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  6, PXI_UD_PATTERN_38); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  7, PXI_UD_PATTERN_39); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  8, PXI_UD_PATTERN_40); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY,  9, PXI_UD_PATTERN_41); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 10, PXI_UD_PATTERN_42); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 11, PXI_UD_PATTERN_43); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 12, PXI_UD_PATTERN_44); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 13, PXI_UD_PATTERN_45); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 14, PXI_UD_PATTERN_46); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 15, PXI_UD_PATTERN_47); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 16, PXI_UD_PATTERN_48); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 17, PXI_UD_PATTERN_49); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 18, PXI_UD_PATTERN_50); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 19, PXI_UD_PATTERN_51); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 20, PXI_UD_PATTERN_52); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 21, PXI_UD_PATTERN_53); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 22, PXI_UD_PATTERN_54); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 23, PXI_UD_PATTERN_55); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 24, PXI_UD_PATTERN_56); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 25, PXI_UD_PATTERN_57); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 26, PXI_UD_PATTERN_58); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 27, PXI_UD_PATTERN_59); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 28, PXI_UD_PATTERN_60); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 29, PXI_UD_PATTERN_61); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 30, PXI_UD_PATTERN_62); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_RD_n_16_2_31_ENTRY, 31, PXI_UD_PATTERN_63); 
#endif
    
    MC_PXI_Payload_Config(PXI_DQWD,  0,            0x0,                          PXI_ACT_DELAY + 9,   PXI_2_CYCLE_CMD,  0, 0x0);//NOP
#if PXI_PRBS
    MC_PXI_Payload_Config(PXI_DQWD,  1, PXI_REPEAT_CNT, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  1, 0x0);//32 BL32 write
#else
    MC_PXI_Payload_Config(PXI_DQWD,  1,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  1, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD,  2,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  2, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD,  3,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  3, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD,  4,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  4, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD,  5,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  5, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD,  6,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  6, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD,  7,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  7, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD,  8,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  8, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD,  9,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT,  9, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 10,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 10, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 11,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 11, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 12,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 12, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 13,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 13, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 14,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 14, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 15,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 15, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 16,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 16, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 17,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 17, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 18,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 18, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 19,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 19, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 20,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 20, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 21,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 21, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 22,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 22, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 23,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 23, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 24,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 24, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 25,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 25, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 26,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 26, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 27,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 27, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 28,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 28, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 29,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 29, 0x0);//BL32 read 15
    MC_PXI_Payload_Config(PXI_DQWD, 30,              0, PXI_BL32_COMMAND_DELTA - PXI_BL32_TASK_CNT, PXI_BL32_TASK_CNT, 30, 0x0);//BL32 read 15
#endif

    // (uint8 num, uint8 alu_cfg, uint32 data0_ptr, uint32 data1_ptr uint8 data0_sel, uint8 data1_sel, uint8 txvalid, uint8 muxing_mode) 
    MC_PXI_Task_Config(PXI_DQWD,  0, 0x0,                0x0,                0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0);//Dummy to delay dq_op vs dq_wd and to wait while ca is sending activate cmd
#if PXI_PRBS
    MC_PXI_Task_Config(PXI_DQWD,  1, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x2, 0x2, 0x0, 0x0, 0x1, 0x1);  //prbs
#else
    MC_PXI_Task_Config(PXI_DQWD,  1, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD,  2, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD,  3, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD,  4, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD,  5, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD,  6, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD,  7, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD,  8, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD,  9, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 10, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 11, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 12, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 13, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 14, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 15, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 16, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 17, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 18, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 19, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 20, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 21, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 22, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 23, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 24, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 25, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 26, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 27, 0x0, PXI_DATA_ADDRESS_2, PXI_DATA_ADDRESS_2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 28, 0x0, PXI_DATA_ADDRESS_3, PXI_DATA_ADDRESS_3, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 29, 0x0, PXI_DATA_ADDRESS_0, PXI_DATA_ADDRESS_0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    MC_PXI_Task_Config(PXI_DQWD, 30, 0x0, PXI_DATA_ADDRESS_1, PXI_DATA_ADDRESS_1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1);
    
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  0,  PXI_UD_PATTERN_0); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  1,  PXI_UD_PATTERN_1); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  2,  PXI_UD_PATTERN_2); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  3,  PXI_UD_PATTERN_3); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  4,  PXI_UD_PATTERN_4); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  5,  PXI_UD_PATTERN_5); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  6,  PXI_UD_PATTERN_6); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  7,  PXI_UD_PATTERN_7); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  8,  PXI_UD_PATTERN_8); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY,  9,  PXI_UD_PATTERN_9); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 10, PXI_UD_PATTERN_10); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 11, PXI_UD_PATTERN_11); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 12, PXI_UD_PATTERN_12); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 13, PXI_UD_PATTERN_13); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 14, PXI_UD_PATTERN_14); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 15, PXI_UD_PATTERN_15); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 16, PXI_UD_PATTERN_16); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 17, PXI_UD_PATTERN_17); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 18, PXI_UD_PATTERN_18); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 19, PXI_UD_PATTERN_19); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 20, PXI_UD_PATTERN_20); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 21, PXI_UD_PATTERN_21); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 22, PXI_UD_PATTERN_22); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 23, PXI_UD_PATTERN_23); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 24, PXI_UD_PATTERN_24); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 25, PXI_UD_PATTERN_25); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 26, PXI_UD_PATTERN_26); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 27, PXI_UD_PATTERN_27); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 28, PXI_UD_PATTERN_28); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 29, PXI_UD_PATTERN_29); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 30, PXI_UD_PATTERN_30); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_0_2_15_ENTRY, 31, PXI_UD_PATTERN_31); 

    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  0, PXI_UD_PATTERN_32); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  1, PXI_UD_PATTERN_33); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  2, PXI_UD_PATTERN_34); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  3, PXI_UD_PATTERN_35); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  4, PXI_UD_PATTERN_36); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  5, PXI_UD_PATTERN_37); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  6, PXI_UD_PATTERN_38); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  7, PXI_UD_PATTERN_39); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  8, PXI_UD_PATTERN_40); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY,  9, PXI_UD_PATTERN_41); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 10, PXI_UD_PATTERN_42); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 11, PXI_UD_PATTERN_43); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 12, PXI_UD_PATTERN_44); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 13, PXI_UD_PATTERN_45); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 14, PXI_UD_PATTERN_46); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 15, PXI_UD_PATTERN_47); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 16, PXI_UD_PATTERN_48); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 17, PXI_UD_PATTERN_49); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 18, PXI_UD_PATTERN_50); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 19, PXI_UD_PATTERN_51); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 20, PXI_UD_PATTERN_52); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 21, PXI_UD_PATTERN_53); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 22, PXI_UD_PATTERN_54); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 23, PXI_UD_PATTERN_55); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 24, PXI_UD_PATTERN_56); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 25, PXI_UD_PATTERN_57); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 26, PXI_UD_PATTERN_58); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 27, PXI_UD_PATTERN_59); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 28, PXI_UD_PATTERN_60); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 29, PXI_UD_PATTERN_61); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 30, PXI_UD_PATTERN_62); 
    HWIO_OUTXI (reg_offset_pxi_secure, DTTS_RAM_PXI_DQDATA_n_16_2_31_ENTRY, 31, PXI_UD_PATTERN_63); 
#endif
}

void MC_PXI_Post_Training_Config()
{
    uint32 reg_offset_pxi              =  REG_OFFSET_MC_PXI_BROADCAST;
    
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_CGC_CFG, CFG_CLK_GATE_DISABLE, 0);
    HWIO_OUTXF (reg_offset_pxi, DTTS_CSR_PXI_CGC_CFG, CLK_GATE_DISABLE, 0);
    
}





