/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/
#include "phy_common.h"
#include "target_config.h"
// ============================================================================
// Disables PLL band select calibration during frequency switch
// ============================================================================
void DDR_PHY_Disable_Band_Sel_Cal(void) //TODO: change to broadcast later
{

#if TARGET_PHY_MIN_VER >= 0x03020100 // new feature
    uint32 reg_offset_ddrcc = 0;
    uint8 ch = 0;
    
    for(ch=0; ch< NUM_CH; ch+= 2) 
    {
     reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET; 
    // Enable broadcast mode for all DQ PHYs on both channels
    //DDRSS_PHY_Broadcast_CCs_All_Enable();
    // Set FPMEXT_CTL_12_ADDRESS to DDRCC_QLINKPLL0_ANALOG_CONTROLS_THREE
    uint32 addr = HWIO_DDR_CC_DDRCC_QLINKPLL0_ANALOG_CONTROLS_THREE_ADDR(0) / 4;
    HWIO_OUTXF(reg_offset_ddrcc, DDR_CC_DDRCC_FPM_EXT_ADDR_CFG0, FPMEXT_PLL_CTL_12_ADDRESS, addr);
    
    // New feature
    HWIO_OUTXF(reg_offset_ddrcc, DDR_CC_DDRCC_FPM_EXT_DATA_CFG0, FPMEXT_PLL0_CTL_12_DATA_INDEX, 13);  //was 12
    HWIO_OUTXF(reg_offset_ddrcc, DDR_CC_DDRCC_FPM_EXT_DATA_CFG0, FPMEXT_PLL1_CTL_12_DATA_INDEX, 14);  //was 13

	//PLL outputs need to be unclamped 
    HWIO_OUTXF2 (reg_offset_ddrcc , DDR_CC_DDRCC_TOP_CTRL_CFG,PLL0_SLEEP_B,PLL1_SLEEP_B, 0x1, 0x1);  
    
    // Select band select derived from FPMEXT_PLL_CTL_12_ADDRESS
    HWIO_OUTX(reg_offset_ddrcc, DDR_CC_DDRCC_QLINKPLL0_SYSTEM_MUXES, 0xE0);   
    HWIO_OUTX(reg_offset_ddrcc, DDR_CC_DDRCC_QLINKPLL1_SYSTEM_MUXES, 0xE0);
    
    // Skip band select calibration
    HWIO_OUTX(reg_offset_ddrcc, DDR_CC_DDRCC_QLINKPLL0_CALIBRATION_SETTINGS, 0x44);
    HWIO_OUTX(reg_offset_ddrcc, DDR_CC_DDRCC_QLINKPLL1_CALIBRATION_SETTINGS, 0x44);

	//PLL outputs need to be clamped 
    HWIO_OUTXF2 (reg_offset_ddrcc , DDR_CC_DDRCC_TOP_CTRL_CFG,PLL0_SLEEP_B,PLL1_SLEEP_B, 0x0, 0x0);  
	
	}
	 
#endif // TARGET_PHY_MIN_VER >= 0x03020100
}

void DDR_PHY_DDRCC_PLL_Lock_Workaround(void) //TODO: change to broadcast later
{

#if TARGET_PHY_MIN_VER >= 0x03020100
   
    uint32 reg_offset_ddrcc = 0;
    uint8 ch = 0;
    
    for(ch=0; ch< NUM_CH; ch+=2) 
    {
        reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET; 
		
	//PLL outputs need to be unclamped 
        HWIO_OUTXF2 (reg_offset_ddrcc , DDR_CC_DDRCC_TOP_CTRL_CFG,PLL0_SLEEP_B,PLL1_SLEEP_B, 0x1, 0x1);  
        
        // Disable Binnary search for Gear Band select  and    Enable Linear search for Gear Band select
        HWIO_OUTXF2 (reg_offset_ddrcc,DDR_CC_DDRCC_QLINKPLL0_GEAR_BAND_SELECT_CONTROLS , BAS_GEAR_BS_ENABLE, LNS_GEAR_BS_ENABLE, 0,1);
        HWIO_OUTXF2 (reg_offset_ddrcc,DDR_CC_DDRCC_QLINKPLL1_GEAR_BAND_SELECT_CONTROLS , BAS_GEAR_BS_ENABLE ,LNS_GEAR_BS_ENABLE, 0,1);
	
        //Software BIAS Enable and Select
        HWIO_OUTXF2 (reg_offset_ddrcc,DDR_CC_DDRCC_QLINKPLL0_SYSTEM_MUXES , CM_BIAS_EN_MUX, CM_BIAS_EN, 1, 1);
        HWIO_OUTXF2 (reg_offset_ddrcc,DDR_CC_DDRCC_QLINKPLL1_SYSTEM_MUXES , CM_BIAS_EN_MUX, CM_BIAS_EN, 1, 1);
		
		//Software BIAS Control Setting (0x00000070)
        HWIO_OUTXF (reg_offset_ddrcc,DDR_CC_DDRCC_QLINKPLL0_BIAS_CONTROL_1 , CM_BIAS_IE_CTRL, 0x38);
        HWIO_OUTXF (reg_offset_ddrcc,DDR_CC_DDRCC_QLINKPLL1_BIAS_CONTROL_1 , CM_BIAS_IE_CTRL, 0x38);
		
		//PLL outputs need to be clamped 
        HWIO_OUTXF2 (reg_offset_ddrcc , DDR_CC_DDRCC_TOP_CTRL_CFG,PLL0_SLEEP_B,PLL1_SLEEP_B, 0x0, 0x0); 
    }

#endif


   
}

