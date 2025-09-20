/*=============================================================================
                              DRE INIT.C
GENERAL DESCRIPTION
This is the target header file for DRE Initializtion .

Copyright 2020 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who        what, where, why
--------   ---        --------------------------------------------------------
07/13/20   adawar     Added DRE change to poipuAU from hanaAU
=============================================================================

*/
#include "dre_hwio.h"
#include "ddr_drivers.h"
#include "ddr_params.h"
#include "boot_logger.h"
#include "ddr_log.h"
#include "ddr_ss_seq_hwioreg.h"

#define INIT_AT_BOOT 0

#define SYNDROME_START_LOW_ADDRESS 0x20000000
#define SYNDROME_START_HIGH_ADDRESS 0x1
#define SYNDROME_END_LOW_ADDRESS 0x2C000000
#define SYNDROME_END_HIGH_ADDRESS 0x1
#define SIZE_TG_8MB 0x800000
#define ECC_FAULT_INJ_ADDR 0x900000000
#define ECC_SYNDROME_ADDRESS 0x128000000
#define ECC_FAULT_INJ_DATA 0x400

int reg_status=0;
boolean ecc_syndrome_region_init();

boolean test_ecc(uint64 pDataAddressDRE, uint64 pSyndromeAddress);


/* ============================================================================
**  Function : dre_enabled
** ============================================================================
*/
/**
*   @brief
*   This function returns TRUE if DRE is enabled 
*   @param[in]  None
*                         
*
*   @return
*   TRUE if DRE is enabled
*
*   @dependencies
*   None
*
*   @sa
*   None
*
*   @sa
*   None
*/

boolean dre_enabled(void)
{
  if (ddrsns_share_data->shrm_ddr_ptr->misc.chip_version >= 0x200)
    return TRUE;
  else
    return FALSE;
}

/* ============================================================================
**  Function : dre_init
** ============================================================================
*/
/**
*   @brief
*   This function initializes 
*   AHB Clock: 
*   DRE Core clock: Div-2 of SHUB clock
*   ATB clock
*
*   @param[in]  None
*                         
*
*   @return
*   TRUE if all clocks are initialized and syndrome region is configured , else return FALSE
*
*   @dependencies
*   None
*
*   @sa
*   None
*
*   @sa
*   None
*/


boolean dre_init()
{ 
 if (dre_enabled())
 {
	 
	
      /* Enable clocks required for DRE */
      HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_OUTM(HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ENABLE_BMSK, HWIO_DPC_REG_DPCC_GEMNOC_DRE_CBCR_CLK_ENABLE_ENABLE_FVAL);  
      HWIO_DPC_REG_DPCC_DRE_CBCR_OUTM(HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ENABLE_BMSK, HWIO_DPC_REG_DPCC_DRE_CBCR_CLK_ENABLE_ENABLE_FVAL);
      HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_OUTM(HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ENABLE_BMSK, HWIO_DPC_REG_DPCC_DRE_CFG_CBCR_CLK_ENABLE_ENABLE_FVAL);
      HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_OUTM(HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ENABLE_BMSK, HWIO_DPC_REG_DPCC_DRE_ATB_CBCR_CLK_ENABLE_ENABLE_FVAL); 
     
   /*
      while((HWIO_DPC_REG_DPCC_DRE_GDSCR_INM(HWIO_DPC_REG_DPCC_DRE_GDSCR_PWR_ON_BMSK)) == 0); */
  //Disable Clock Gating for DRE
	  HWIO_OUTXF(SEQ_DDR_SS_DDR_REG_DDR_SS_REGS_OFFSET, DDR_SS_REGS_DRE_ROOT_CLOCK_GATING_1, HW_RCG_EN, 0);
    
	  //DRE GDSCR Out of SW Collapse
	  HWIO_OUTX(SEQ_DDR_SS_DPC_REG_DPCC_OFFSET,  DPCC_REG_DRE_GDSCR,  0x222000);
	  while(!(reg_status==0x1f))
	  {
		  reg_status=(HWIO_INX(SEQ_DDR_SS_DPC_REG_DPCC_OFFSET, DPCC_REG_DRE_GDSCR) & 0xF8000000) >> 27;
	  }

      HWIO_DPC_REG_DPCC_DRE_GDSCR_OUTM(HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_FF_ENABLE_BMSK,HWIO_DPC_REG_DPCC_DRE_GDSCR_RETAIN_FF_ENABLE_ENABLE_FVAL);
      
      /* Disable Bypass mode  */
      HWIO_DRE_DRE_BYPASS_MODE_OUTM(HWIO_DRE_DRE_BYPASS_MODE_BYPASS_MODE_BMSK,0x0);

      /* Configure the syndrome region */
      HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_OUTM(HWIO_DRE_DRE_SYNDROME_REGION_BASE_LOWER_BASE_31_12_BMSK,SYNDROME_START_LOW_ADDRESS);
      HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_OUTM(HWIO_DRE_DRE_SYNDROME_REGION_BASE_UPPER_BASE_35_32_BMSK,SYNDROME_START_HIGH_ADDRESS);

      /* Defining size : Total DDR Size 8GB/32  */
      HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_OUTM(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_LOWER_MASK_31_12_BMSK,0xF0000000);
      HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_OUTM(HWIO_DRE_DRE_SYNDROME_ADDR_MASK_UPPER_MASK_35_32_BMSK,0xF);

      /* Enable ECC */
      HWIO_DRE_DRE_ECC_CONTROL_OUTM(HWIO_DRE_DRE_ECC_CONTROL_ENABLE_ECC_BMSK,0xA);
      /*Enable Interrupt*/
      //HWIO_DRE_DRE_ECC_IRQ_SEC_OUTM (HWIO_DRE_DRE_ECC_IRQ_SEC_ENABLE_SEC_IRQ_BMSK, 0x1);

	 /* The syndrome initialization is taking 400+ms so for boot optimization it is deferred to HLOS boot*/
#if INIT_AT_BOOT
      if(ecc_syndrome_region_init() == FALSE)
      {
        return FALSE;
      }
#endif	
 }
  return TRUE;
}

/* ============================================================================
**  Function : dre_test
** ============================================================================
*/
/**
*   @brief
*   This function tests DRE
*   @param[in]  None
*                         
*
*   @return
*   TRUE if test passes
*
*   @dependencies
*   None
*
*   @sa
*   None
*
*   @sa
*   None
*/


boolean dre_test()
{ 
 /*As syndrome region initialiazation is deferred to HLOS boot and hence dre_test*/
#if INIT_AT_BOOT
  boolean dre_test_status = 0;
  volatile uint64 ecc_addr = ECC_FAULT_INJ_ADDR;
  uint64 syndrome_address = ECC_SYNDROME_ADDRESS;
  if (ddrsns_share_data->shrm_ddr_ptr->misc.chip_version >= 0x200)
  {
      /* Run DRE test*/
      ddr_printf(DDR_ERROR, "dre_test, Start");
      dre_test_status = test_ecc(ecc_addr, syndrome_address); 
      ddr_printf(DDR_ERROR, "dre_test, End");
      if(dre_test_status == FALSE)
      {
          return FALSE;
      }
  }
#endif
  return TRUE;  
}

/* ============================================================================
**  Function : ecc_syndrome_region_init
** ============================================================================
*/
/**
*   @brief
*   This function sets the DDR driver's device table to an external device table.
*   The Syndrome DRAM region need to be initialized to “0”. The traffic generator
*   modules in DDRSS infrastructure are to be used.
*
*   @param[in]  None
*                         
*
*   @return
*   TRUE if DDR_TG0_TG_STREAM_WR_STATUS0 is SET, else return FALSE
*
*   @dependencies
*   None
*
*   @sa
*   None
*
*   @sa
*   None
*/
boolean ecc_syndrome_region_init()
{
  volatile uint32 i=0;
  uint32 syndrome_lower_address = SYNDROME_START_LOW_ADDRESS;
  /* One time setting */
  HWIO_DDR_SS_REGS_DDRSS_CTG_SEL_OUTM(SEQ_DDR_SS_DDR_REG_DDR_SS_REGS_OFFSET, HWIO_DDR_SS_REGS_DDRSS_CTG_SEL_GPU0_CTG_SEL_BMSK, 0x1);
  HWIO_DDR_TG0_TG_STREAM_WR_CFG4_OUT(0x00000000);
  HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG0_OUT(0x00014C00);
  HWIO_DDR_TG0_TG_STREAM_WR_QSB_CFG1_OUT(0x00000000);
  HWIO_DDR_TG0_TG_STREAM_WR_CFG1_OUT(0x000FFFFF);
  
  /* Following sequence needs to be repeated by incrementing start syndrome address by 8MB till end of syndrome region*/
  while(syndrome_lower_address < SYNDROME_END_LOW_ADDRESS)
  {   
      /* 36-bit Start address to be configured for syndrome region */
      HWIO_DDR_TG0_TG_STREAM_WR_CFG2_OUTM(HWIO_DDR_TG0_TG_STREAM_WR_CFG2_SADDR_LOW_BMSK,syndrome_lower_address);
      HWIO_DDR_TG0_TG_STREAM_WR_CFG3_OUTM(HWIO_DDR_TG0_TG_STREAM_WR_CFG3_SADDR_HIGH_BMSK,SYNDROME_START_HIGH_ADDRESS);

      HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OUT(0xA0080D0);     
      /*Triggering the operation */
      HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_OUT(0x00000001); 
      /*Turn off the trigger*/
      HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_OUT(0x00000000);  
      HWIO_DDR_TG0_TG_STREAM_WR_CFG0_OUT(0xA0080D1);
      /*Triggering the operation */
      HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_OUT(0x00000001);
      /* Turn off the trigger */
      HWIO_DDR_TG0_TG_STREAM_WR_OPERATION_OUT(0x00000000); 
      /*Delay required */
      for(i=0;i<6000000;i++)
      {
      } 
      /* Read register for completion status */
      if(HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_INM(HWIO_DDR_TG0_TG_STREAM_WR_STATUS0_DONE_BMSK) != 0x1)
      {
        ddr_printf(DDR_ERROR, "Data not cleared");
        return FALSE;
      }   
      syndrome_lower_address = syndrome_lower_address + SIZE_TG_8MB;
  }
  HWIO_DDR_SS_REGS_DDRSS_CTG_SEL_OUTM(SEQ_DDR_SS_DDR_REG_DDR_SS_REGS_OFFSET, HWIO_DDR_SS_REGS_DDRSS_CTG_SEL_GPU0_CTG_SEL_BMSK, 0x0);
  return TRUE;  
}
