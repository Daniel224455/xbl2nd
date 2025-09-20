/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

/*==============================================================================

                                EDIT HISTORY



================================================================================*/



#include "ddrss_mccc.h"



void MCCC_Config(BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel)

{

    // MCCC MSTR config

    DDRSS_set_config(ddr, SEQ_DDR_SS_MCCC_MCCC_MSTR_OFFSET,
                     (uint32(*)[2])((size_t)(ddr->dcb_ptr) + (*ddr->dcb_ptr)[MCCC_MSTR_CFG_IDX][0]));



    // Enable MCCC broadcast for MCCC SLV config

    DDRSS_Broadcast_MCCC_Config(channel, NO_RDATA);

    DDRSS_set_config(ddr, SEQ_DDR_SS_MCCC_BROADCAST_MCCC_SLV_OFFSET,
                      (uint32(*)[2])((size_t)(ddr->dcb_ptr) + (*ddr->dcb_ptr)[MCCC_SLV_CFG_IDX][0]));

    // Disable MCCC broadcast

    DDRSS_Broadcast_MCCC_Config(DDR_CH_NONE/* disable broadcast */, NO_RDATA);

    

    // Enable MCCC HW clock gating as the last step in MCCC one-time settings

    HWIO_OUTXF (SEQ_DDR_SS_MCCC_MCCC_MSTR_OFFSET, MCCC_MSTR_CLOCK_GATE_CFG, MCCC_HW_CLKGATE_EN, 0x1);

}

  



