/**
 * @file ddr_sdi_wrapper.c
 * @brief
 * Target specific DDR drivers.
 */
/*==============================================================================
                                EDIT HISTORY

================================================================================
when       who     what, where, why
--------   ---     -------------------------------------------------------------
12/20/18   dj      change pass1 cookie
07/20/15   tw      updated sanity ddr test to check across interleave
02/20/15   tw      Initial version.
================================================================================
                   Copyright 2018 Qualcomm Technologies Incorporated
                              All Rights Reserved
                     Qualcomm Confidential and Proprietary
==============================================================================*/
/*==============================================================================
                                  INCLUDES
==============================================================================*/
#include "ddrss_init_sdi.h"
#include "ddr_drivers.h"
#include <stdbool.h>
#include "DALSys.h"
#include "ddr_hwio.h"
#include "Target_cust.h"
#include "ddr_sdi_log.h"
#include "ddr_target.h"
#include "ddr_sdi_log.h"

/*==============================================================================
                                  MACROS
==============================================================================*/
#define INTERLEAVE_SIZE 0x200

/*==============================================================================
                                  DATA
==============================================================================*/
static boolean ddr_init_status = FALSE;
#define test_limit  128

//SHRM_DDR_STRUCT *shrm_share_data_sdi = (SHRM_DDR_STRUCT *)SCL_SBL1_DDR_SHRM_STRUCT_BASE;
DDR_SDI_LOGS *ddr_sdi_logs_ptr_sdi = (DDR_SDI_LOGS *)SDI_COOKIES_STRUCT_BASE;

// volatile boolean pass1_test = TRUE;

/*==============================================================================
                                  FUNCTIONS
==============================================================================*/

boolean Clock_ConfigureDDR( void );
bool icb_sdi_restore( void );

static boolean ddr_test_read_write
(
  uint64 base
)
{
  volatile uint32 *base_addr;
  uint32 limit;
  static uint32 data[test_limit];
  uint32 offset;
  base_addr = (uint32 *)base;
  limit = test_limit >> 2;
  for (offset = 0; offset < limit; offset++)
  {
	data[offset] = base_addr[offset];
	base_addr[offset] = ~(~data[offset]);
  }
  for (offset = 0; offset < limit; offset++)
  {
    if (base_addr[offset] != data[offset])
    {
      return FALSE;
    }
  }
  return TRUE;
}

void ddr_sdi_save_reg_log (SHRM_DDR_STRUCT *shrm_share_data_sdi)
{
  uint32 i = 0;
  uint32 *addr = (uint32 *)SDI_REG_LOG_START_ADDR;

  if (shrm_share_data_sdi->misc.num_channel == 4)
  {
    while (ddr_sdi_reg_log_addr_4ch[i] != 0)
    {
      *addr = *((uint32*)(ddr_sdi_reg_log_addr_4ch[i++]));
      addr++;
    }
  }
  else
  {
    while (ddr_sdi_reg_log_addr_8ch[i] != 0)
    {
      *addr = *((uint32*)(ddr_sdi_reg_log_addr_8ch[i++]));
      addr++;
    }
  }
}

/* ============================================================================
**  Function : ddr_sdi_init
** ============================================================================
*/
/*!
*   @brief
*   This function is called to reinitialize ddr in SDI context. 
*
*   @details
*   This function is called to reinitialize ddr in SDI context. 
*   lite library for both clock and bus will be called first to
*   prepare for data access on the ddr, then sdi version of ddr driver
*   will be called to take ddr out from self refresh.
*
*   @param
*   None
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  None
*
*   @sa None
*/

boolean ddr_sdi_init(void)
{ 
  boolean status = TRUE, freeze_io_status;
  uint8 i;
  SHRM_DDR_STRUCT *shrm_share_data_sdi = (SHRM_DDR_STRUCT *)SCL_SBL1_DDR_SHRM_STRUCT_BASE;
  
  ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = DDR_PASS1_START;

  if (ddr_init_status == FALSE)
  {    
    /* initialize clock for ddr */
    Clock_ConfigureDDR();
    
    /* initialize bus driver */
    icb_sdi_restore();
    
    ddr_sdi_save_reg_log(shrm_share_data_sdi);
    
    if((!HWIO_DPC_REG_DPCC_MC_GDSCR_INM(HWIO_DPC_REG_DPCC_MC_GDSCR_PWR_ON_BMSK)) || (!HWIO_DPC_REG_DPCC_PHY_GDSCR_INM(HWIO_DPC_REG_DPCC_PHY_GDSCR_PWR_ON_BMSK)) )
    {
      if(!HWIO_DPC_REG_DPCC_MC_GDSCR_INM(HWIO_DPC_REG_DPCC_MC_GDSCR_PWR_ON_BMSK))
      {
        ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = DPCC_MC_GDSCR_PWR_ON_NOT_SET;
      }
      else
      {
        ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = DPCC_PHY_GDSCR_PWR_ON_NOT_SET;
      }
      return FALSE;
    }

	if (shrm_share_data_sdi->misc.num_channel == 4)
		freeze_io_status = HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0) && HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1) && HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0) && HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1);
	else
		freeze_io_status = HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0) && HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1) && HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0) && HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1) && HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0) && HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1) && HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0) && HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1);

    if (!freeze_io_status) 
    {
        if (HWIO_INF(CABO_BROADCAST_CABO_GLOBAL_SHKE_STATUS, WDOG_SELF_REFRESH))
        {
          ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = FREEZE_IO_NOT_SET_WDOG_SR_SET;
        }
        else
        {
          ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = FREEZE_IO_NOT_SET_WDOG_SR_NOT_SET;
        }
        return FALSE;
    }
    /* DDR correctly went into watchdog self refresh */
    ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = DDR_WENT_INTO_WDOG_SR;
    
    /* recover ddr driver */
    HAL_DDR_SDI_Recover(shrm_share_data_sdi, DDR_CH_ALL, DDR_CS_BOTH, 200000);

    if (!(HWIO_INF(CABO_BROADCAST_CABO_GLOBAL_SHKE_STATUS, WDOG_SELF_REFRESH)))
    {
      /* DDR was correctly brought out of self refresh */
      ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = DDR_BROUGHT_OUT_OF_SR;
    }
    else
    {
      ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = DDR_NOT_BROUGHT_OUT_OF_SR;
      return FALSE;
    }
    
    ddr_init_status = TRUE;
  }
  for (i = 0; i < shrm_share_data_sdi->misc.num_channel; i++)
  {
    status &= ddr_test_read_write((shrm_share_data_sdi->ddr_size_info.ddr_cs0_remapped_addr[0]+(i*INTERLEAVE_SIZE)));
    if (shrm_share_data_sdi->ddr_size_info.ddr_cs1_remapped_addr[0])
    {
      status &= ddr_test_read_write((shrm_share_data_sdi->ddr_size_info.ddr_cs1_remapped_addr[0]+(i*INTERLEAVE_SIZE)));
    }
  }
  
  if (status)
  {
    ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = DDR_TEST_PASSED;
  }
  else
  {
    ddr_sdi_logs_ptr_sdi->pass1_enter_cookie->p_cookie_value = DDR_TEST_FAILED;
  }  
  
  return status;   
}

boolean ddr_sdi_exit(void)
{
  volatile uint32 i;

  HWIO_OUTF(AOSS_CC_RESET_DEBUG_ENABLE, SW_BASED_PRE_ARES, 0x1 );
  ddr_sdi_logs_ptr_sdi->pass1_exit_cookie->p_cookie_value = DDR_PASS1_EXIT_PREARES_SET;
  for (i = 0; i < 100000; i++)
  {
  }
  // Deassert the PRE_ARES signal again
  HWIO_OUTF(AOSS_CC_RESET_DEBUG_ENABLE, SW_BASED_PRE_ARES, 0);
  ddr_sdi_logs_ptr_sdi->pass1_exit_cookie->p_cookie_value = DDR_PASS1_EXIT_PREARES_NOT_SET;
  
  return TRUE;
}

/*===========================================================================
FUNCTION
    ddr_mpm_config_ebi1_freeze_io_sdi
DESCRIPTION
    Freeze/unfreeze the IO to the DDR devices.  This implementation is required by the DSF drivers 
    (in file ddr_external.h) to allow Power Collapse or other operating modes where the output 
    to the DDR is disabled.
===========================================================================*/

void ddr_mpm_config_ebi1_freeze_io_sdi(boolean flag)
{
#if 0
   HWIO_OUTF(MPM2_MPM_LOW_POWER_CFG, SW_EBI1_CTL_VALUE, (flag ? 1:0));
   HWIO_OUTF(MPM2_MPM_LOW_POWER_CFG, SW_EBI1_CTL_ENABLE, 0x1);

   // Freeze/unfreeze IO as requested by flag
   HWIO_OUTF(MPM2_MPM_DDR_PHY_FREEZEIO_EBI1, DDR_PHY_FREEZEIO_EBI1, (flag ? 1:0) );
#endif
}
