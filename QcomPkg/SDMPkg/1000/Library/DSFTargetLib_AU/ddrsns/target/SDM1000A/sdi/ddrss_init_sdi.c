/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
                                EDIT HISTORY

================================================================================*/

#include "ddrss_init_sdi.h"
#include "ddrss.h"





/*********************************************************************************
**********************************************************************************
Code from ddrss.c
**********************************************************************************
*********************************************************************************/

//================================================================================================//
// Pass 1 and Pass 2 DDR Initialization
//================================================================================================//
boolean HAL_DDR_SDI_Recover (SHRM_DDR_STRUCT *shrm_ddr_ptr,
                             DDR_CHANNEL channel, 
                             DDR_CHIPSELECT chip_select,
                             uint32 clk_freq_khz)
{
   // As described by the watchdog reset documentation at go/ddrss -> Napali -> Watchdog the Napali watchdog reset process
   //    is much simpler than previous chips.

   // For Pass 1 warm boot, we need to send a SHRM command via the RSI that brings the DDRSS from Powerstate D3 (collapsed)
   //    to Powerstate 0 (active), at the 200 MHz boot frequency.  We can then take the DDR out of (watchdog) self-refresh,
   //    and run recovery traffic.
   
   // Disable downtime requests to proceed with frequency switch included in powerstate change
   DDRSS_Downtime_Request_Ctrl_sdi(0);

   
   // Request that the SHRM move the DDRSS to Powerstate D0
   /* Enabling RPMH port override control*/
   SHRM_rpmh_port_override_control_sdi(1);
 
   /* Powerstate D0 (active) Work request */
   SHRM_rsi_cmd_inject_sdi(RSI_SDI_POWERSTATE_D0_WORK_CMD);
   
   /* LB Powerup work request */
   SHRM_rsi_cmd_inject_sdi(RSI_SDI_POWERSTATE_LB0_WORK_CMD);

   /* Exit PASR */
   SHRM_rsi_cmd_inject_sdi(RSI_SDI_PASR_EXIT_CMD);

   /* Disabling RPMH port override control*/
   SHRM_rpmh_port_override_control_sdi(0);
   

   return TRUE;
}


//================================================================================================//
// Enable or disable downtime requests in DDRSS DT Aggregator
//================================================================================================//
void DDRSS_Downtime_Request_Ctrl_sdi (uint8 enable)
{ 
   // enable = 1 -> DT_REQ_DISABLE = 0, enable DT aggregator
   // enable = 0 -> DT_REQ_DISABLE = 1, disable DT aggregator
   HWIO_OUTXFI (SEQ_DDR_SS_DDR_REG_DDR_SS_REGS_OFFSET, DDR_SS_REGS_DT_AGG_REQ_CFG_n,\
                0, DT_REQ_DISABLE, ~enable);
   HWIO_OUTXFI (SEQ_DDR_SS_DDR_REG_DDR_SS_REGS_OFFSET, DDR_SS_REGS_DT_AGG_REQ_CFG_n,\
                1, DT_REQ_DISABLE, ~enable);
   HWIO_OUTXFI (SEQ_DDR_SS_DDR_REG_DDR_SS_REGS_OFFSET, DDR_SS_REGS_DT_AGG_REQ_CFG_n,\
                2, DT_REQ_DISABLE, ~enable);
   HWIO_OUTXFI (SEQ_DDR_SS_DDR_REG_DDR_SS_REGS_OFFSET, DDR_SS_REGS_DT_AGG_REQ_CFG_n,\
                3, DT_REQ_DISABLE, ~enable);				
   
   // Current workaround in MC block to enable/disable Task 5 (Downtime Request) for boot time freq switch
   // TODO: may be removed once downtime request could be disabled correctly in DDRSS level
   // Enable/disable MC freq switch task 5 (Downtime Request)
   HWIO_OUTXFI (REG_OFFSET_MC_SHKE(BROADCAST_CH_NUM), SHKE_FSW_TASK_CFG_n, 5, TASK_EN, enable);
}






/*********************************************************************************
**********************************************************************************
Code from ddrss_shrm.c
**********************************************************************************
*********************************************************************************/

//================================================================================================//
// usage: 
// 1. call SHRM_rpmh_port_override_control_sdi(uint8 en) with en = 1
// 2. call SHRM_rsi_cmd_inject with task_sdi(cmd_payload), like ddr_clk_switch.
//    work function call be called multiple times here
// 3. call SHRM_rpmh_port_override_control_sdi(uint8 en) with en = 0
//================================================================================================//
void SHRM_rpmh_port_override_control_sdi(uint8 en)
{
	// change Override RPMh option in MUX sel to CSR, SHRM_RSI_PATH[SEL] PoR value = 0(enable)
	HWIO_OUTXF (SEQ_DDR_SS_SHRM_CSR_SHRM_CSR_OFFSET, SHRM_CSR_SHRM_RSI_PATH, SEL, !en);

	// Override RPMh in MUX sel
	HWIO_OUTXF (SEQ_DDR_SS_SHRM_CSR_SHRM_CSR_OFFSET, SHRM_CSR_SHRM_RPM_PORT_OVERRIDE_EN, EN, en);
}

void SHRM_rsi_cmd_inject_sdi(uint32 cmd_payload)
{
	// specify payload before issuing trigger cmd
	HWIO_OUTXF (SEQ_DDR_SS_SHRM_CSR_SHRM_CSR_OFFSET, SHRM_CSR_SHRM_RPM_PORT_OVERRIDE_CMD, CMD_PAYLOAD, cmd_payload);

	// issuing cmd trigger
	HWIO_OUTXF (SEQ_DDR_SS_SHRM_CSR_SHRM_CSR_OFFSET, SHRM_CSR_SHRM_RPM_PORT_OVERRIDE_TRIGGER, TRIGGER, 1);
	
	// Poll SHRM_CSR_SHRM_SPROC_STATUS to ensure task completed and processor in WAITI state
	while (HWIO_INXF (SEQ_DDR_SS_SHRM_CSR_SHRM_CSR_OFFSET, SHRM_CSR_SHRM_SPROC_STATUS, STATE) != 1);
	
	// clearing cmd trigger
	HWIO_OUTXF (SEQ_DDR_SS_SHRM_CSR_SHRM_CSR_OFFSET, SHRM_CSR_SHRM_RPM_PORT_OVERRIDE_TRIGGER, TRIGGER, 0);
}

