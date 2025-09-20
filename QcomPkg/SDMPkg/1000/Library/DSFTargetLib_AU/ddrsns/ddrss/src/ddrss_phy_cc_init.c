/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================
when       who          what, where, why
--------   ---          --------------------------------------------------------
05/30/14   jeasley      Create separate CA and DQ PHY settings routines
05/20/14   jeasley      Correct SW handshake and Broadcast
05/08/14   jeasley      Move initialization of polling variables to the inside of the loop.
05/06/14   jeasley      Invert the polarity of sw_handshake_complete poll.
05/06/14   jeasley      Updated DDR_PHY_CC_init to use broadcast.
05/04/14   arindamm     First edit history header. Add new entries at top.
================================================================================*/

#include "ddrss.h"
#include "ddrss_mccc.h"

//================================================================================================//
// Pre training set-up
//================================================================================================//

//uint32   reg_offset_ddr_phy;

//================================================================================================//
// DDR PHY and CC one-time settings
//================================================================================================//
//void DDR_PHY_CC_Config(BOOT_DDR_STRUCT *ddr, void *data)
//{   
//    uint32   dq0_ddr_phy_base; 
//    uint32   ca0_ddr_phy_base;
//    uint32   cc_ddr_phy_base;
//    uint8    ch;
//    uint8    byte_lane;
//    uint8    ca_lane;
//    PHY_BUS  dq     = DQ;
//    PHY_BUS  ca     = CA;
//    
//    for (ch = 0; ch < NUM_CH; ch++)
//    {
//        dq0_ddr_phy_base  = REG_OFFSET_DDR_PHY_CH(ch) + DQ0_DDR_PHY_OFFSET;
//        ca0_ddr_phy_base  = REG_OFFSET_DDR_PHY_CH(ch) + CA0_DDR_PHY_OFFSET;        
//        
//        for (byte_lane=0;byte_lane<dq;byte_lane++)
//        {
//            DDRSS_set_config_base_delta(ddr,
//                dq0_ddr_phy_base + (byte_lane * DDR_PHY_OFFSET),
//                (uint32(*)[2])((size_t)data + ((uint16*)data)[DDR_PHY_DQ_CFG_BASE_IDX]),
//                (uint32(*)[2])((size_t)data + ((uint16*)data)[DDR_PHY_DQ_CFG_DELTA_IDX]));
//        }
//        
//        for (ca_lane=0;ca_lane<ca;ca_lane++)
//        {
//            DDRSS_set_config_base_delta(ddr, 
//                ca0_ddr_phy_base + (ca_lane * DDR_PHY_OFFSET),
//                (uint32(*)[2])((size_t)data + ((uint16*)data)[DDR_PHY_CA_CFG_BASE_IDX]),
//                (uint32(*)[2])((size_t)data + ((uint16*)data)[DDR_PHY_CA_CFG_DELTA_IDX]));
//        }
//        
//        cc_ddr_phy_base  =  REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;
//        
//        DDRSS_set_config_base_delta(ddr,
//                cc_ddr_phy_base,
//                (uint32(*)[2])((size_t)data + ((uint16*)data)[DDR_CC_CFG_BASE_IDX]),
//                (uint32(*)[2])((size_t)data + ((uint16*)data)[DDR_CC_CFG_DELTA_IDX]));
//    }
//}



//================================================================================================//
// DDR PHY and CC Initialization
//================================================================================================//
void DDR_PHY_CC_init (BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel)
{
    uint8 ch = 0;
    uint32 reg_offset_ddr_phy;
    uint32 reg_offset_ddrcc     = 0;
    DDR_CHIPSELECT qualified_cs = DDR_CS_NONE;
    uint32 channels          = ddr->shrm_ddr_ptr->misc.enabled_channels;
	//uint32  Master_IOCAL[NUM_CH];

    for(ch = 0; ch < NUM_CH; ch++)
    {
        // Skip channels which are not enabled
        if(((channels >> ch) & 0x1) == 0x0) continue;
        
        qualified_cs = (DDR_CHIPSELECT)ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].populated_chipselect;
        reg_offset_ddrcc = REG_OFFSET_DDR_CC_CH(ch) + DDR_CC_OFFSET;
        
        if((ch % 2) == 0)
        {
            // Even channles are assigned to Ch0 register
            HWIO_OUTXF(reg_offset_ddrcc, DDR_CC_DDRCC_UPDATE_INTF_CFG0, UPDT_REQ_TYPE_CH0, qualified_cs);
        }
        else
        {
            // Odd channles are assigned to Ch1 register
            HWIO_OUTXF(reg_offset_ddrcc, DDR_CC_DDRCC_UPDATE_INTF_CFG0, UPDT_REQ_TYPE_CH1, qualified_cs);
        }
        
        reg_offset_ddr_phy = REG_OFFSET_DDR_PHY_CH(ch);

        if(MASTER_IOCAL(ch))
        {
            // Set master CA PHY, enable automatic IO calibration, and perform manual IO calibration
            HWIO_OUTX(reg_offset_ddr_phy + CA0_DDR_PHY_OFFSET, DDR_PHY_DDRPHY_UPDATE_INTF_CFG, 1);
            HWIO_OUTXF(reg_offset_ddr_phy + CA0_DDR_PHY_OFFSET, DDR_PHY_DDRPHY_IOCTLR_CTRL_CFG, IO_CAL_AUTO, 1);
            
            //if(ddr->training_params.RX_Cal_En)
            if(1)
            {
                DDR_PHY_hal_cfg_rx_cal(ddr, ch);
            }
            
            DDR_PHY_hal_cfg_sw_iocal(reg_offset_ddr_phy + CA0_DDR_PHY_OFFSET);
        }
    }//ch
}

