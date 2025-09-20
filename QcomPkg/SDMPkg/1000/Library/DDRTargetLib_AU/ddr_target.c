/**
 * @file ddr_target.c
 * @brief
 * Target specific DDR drivers.
 */
/*==============================================================================
                                EDIT HISTORY

================================================================================
when       who     what, where, why
--------   ---     -------------------------------------------------------------
12/20/18   dj      change DSF version printing and pass1 cookie
12/18/18   dj      change register used for getting serial ID of the MSM 
09/26/17   daison  Use DDR_CH_ALL instead of interface_name parameter
07/28/15   rp      Added ddr_post_training function to support conditional loading of ddr training
07/20/15   tw      enable one time training
07/01/15   tw      added bimc remapper api call
06/20/14   tw      added ddr_pre_init api to capture any target specific 
                   workarounds that needs to be applied prior to ddr init
05/28/14   tw      cleaned up sbl <-> ddr driver dependencies around ddr training
                   implementation of cx\mx\cpr hash to force retraining
03/12/14   sr      Initial version.
================================================================================
                   Copyright 2018 Qualcomm Technologies Incorporated
                              All Rights Reserved
                     Qualcomm Confidential and Proprietary
==============================================================================*/
/*==============================================================================
                                  INCLUDES
==============================================================================*/
#include "ddr_common.h"
#include "ddr_drivers.h"
#include "ddr_internal.h"
#include "ddr_sync.h"
#include "ddr_log.h"
#include "ddr_params.h"
#include "ddr_target.h"
#include "HAL_SNS_DDR.h"
#include "ddr_config.h"
#include "ClockBoot.h"
#include "icbcfg.h"
#include <stddef.h>
#include "ddr_external.h"
#include "pm_ldo.h"
#include "CoreVerify.h"
#include "cpr.h"
#include "crc.h"
#include "boot_extern_platforminfo_interface.h"
#include "pm_smps.h"
#include "pm_resources_and_types.h"
#include "PlatformInfo.h"
#include "ddr_hwio.h"
#include "Target_cust.h"
#include "llcc.h"
#include "boot_util.h"
#include "ddr_sdi_log.h"
#include "boothw_target.h"
#include "ddrss_init_sdi.h"
#include "mccc_seq_hwioreg.h"

#define TRAINING_ENABLED TRUE
#define ONE_TIME_TRAINING TRUE

/*==============================================================================
                                  MACROS
==============================================================================*/
/* Macro for round-up division */
#define div_ceil(n, d)  (((n) + (d) - 1) / (d))

#define EIGHT_SEGMENT_MASK 0xFF
#define FOUR_SEGMENT_MASK  0xF

#define DDR_COLLAPSE_CP    0x2

/*==============================================================================
                                  DATA
==============================================================================*/

/* DDR interface status, keeps track of active interfaces and their status */
extern ddr_interface_state ddr_status;

extern ddr_info ddr_physical_size;
extern ddr_size_info ddr_system_size;

extern DDR_SDI_LOGS *ddr_sdi_logs_ptr;

#ifdef ENABLE_AUTO_BOOT_OPTIMIZATION
uint32 ddr_boot_up_freq = 1804800;
#else
uint32 ddr_boot_up_freq = 1353000;
#endif

static railway_voter_t ebi_pin = NULL;


/*==============================================================================
                                  FUNCTIONS
==============================================================================*/

void print_ddr_frequency(void);
void ddr_freq_set_min_max(uint8 min, uint8 max);

void ddr_freq_set_min_max(uint8 min, uint8 max)
{
  uint8 i;
  uint8 num_levels = (sizeof(ddrsns_share_data->aop_params.mc_states)/sizeof(ddrsns_share_data->aop_params.mc_states[0]));
  if (max > num_levels)
    max = num_levels;
  for (i = 0; i < num_levels; i++)
  {
    if (i >= min && i <= max)
      ddrsns_share_data->aop_params.mc_states[i].freq_enable = TRUE;
    else
      ddrsns_share_data->aop_params.mc_states[i].freq_enable = FALSE;
  }
    
}

/* ============================================================================
**  Function : ddr_target_init
** ============================================================================
*/
/*!
*   @brief
*   This function is called at the end of ddr init to initialize any
*   target specific configurations
*
*   @details
*   Target specific configurations such as override of timing registers and
*   calling target specific init functions will be captured in this api
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
void ddr_target_init()
{
  // uint8 ddr_min_freq_idx = 0;
  // uint8 ddr_max_freq_idx = 4;
  uint8 i;
  char ddr_log_string[200];
  // char ddr_max_freq[50];
  uint32 chip_plat_ver_info;
  uint32 err;
  DalPlatformInfoPlatformType platform = DalPlatformInfo_Platform();
  
  if (platform != DALPLATFORMINFO_TYPE_RUMI)
  {
    ddrsns_share_data->shrm_ddr_ptr->misc.target_silicon = 1;

    if (PcdGet32 (PcdBuildType) == 0)
    {
      int rail = rail_id("vdda_ebi");
      CORE_VERIFY(rail!=RAIL_NOT_SUPPORTED_BY_RAILWAY);    
      ebi_pin = railway_create_voter(rail, RAILWAY_DDR_TRAINING_VOTER_ID);
      railway_corner_vote(ebi_pin, RAILWAY_TURBO);
      railway_transition_rails();
      err = pm_smps_sw_mode(0, PM_SMPS_1, PM_SW_MODE_NPM); // VDDA_EBI SMPS1 to PWM
      CORE_VERIFY(err == PM_ERR_FLAG__SUCCESS);    
    }
  }
  ddrsns_share_data->jtag_connected = HWIO_INF(AOSS_CC_CDBGPWRUPREQ, STATUS);
  
  chip_plat_ver_info =  HWIO_IN(TCSR_SOC_HW_VERSION);

  /* extract upper 16 bit and store it in platform field of ddr_misc */
  ddrsns_share_data->shrm_ddr_ptr->misc.platform_id  = chip_plat_ver_info >> 16 /*& 0xFFFF0000*/ ;
  /* extract lower 16 bits and store it in version field of ddr_misc */
  ddrsns_share_data->shrm_ddr_ptr->misc.chip_version  = chip_plat_ver_info & 0x0000FFFF ;
  
  /* Extract the MSM_PKG_TYPE from QFPROM register */
  ddrsns_share_data->shrm_ddr_ptr->misc.package_type = HWIO_INF(QFPROM_RAW_FEAT_CONFIG_ROW4_MSB , MSM_PKG_TYPE);
  
  ddrsns_share_data->shrm_ddr_ptr->misc.misc_cfg[0] = 1;

  ddrsns_share_data->shrm_ddr_ptr->misc.total_channels = 0xFF;
  ddrsns_share_data->shrm_ddr_ptr->misc.enabled_channels = 0xFF;
  /*This check is specifically for Poipu V2 .
    return number of channels based on the value in TCSR_TIMEOUT_INTR_STATUS register
    TCSR_TIMEOUT_INTR_STATUS = 0 --> return 8 channels
                               1 --> return 4 channels*/
    
  if((ddrsns_share_data->shrm_ddr_ptr->misc.platform_id == MSM_VERSION_POIPU) && ((ddrsns_share_data->shrm_ddr_ptr->misc.chip_version == 0x0200)||(ddrsns_share_data->shrm_ddr_ptr->misc.chip_version == 0x0201)))
  {
    //This check is only for Poipu V2 and to determine number of channel based on TCSR register 
    if(HWIO_TCSR_TIMEOUT_INTR_STATUS_INM(HWIO_TCSR_TIMEOUT_INTR_STATUS_DDR_CH4_OR_CH8_SEL_BMSK))
    {
      ddrsns_share_data->shrm_ddr_ptr->misc.num_channel = 4;
    }
	else
	{
	  ddrsns_share_data->shrm_ddr_ptr->misc.num_channel = 8;
	}
  } 
  else
  {
	  ddrsns_share_data->shrm_ddr_ptr->misc.num_channel = ddr_get_max_channels();
  }
  
  snprintf(ddr_log_string, 200, "%d channel DDR configuration",ddrsns_share_data->shrm_ddr_ptr->misc.num_channel);
  boot_log_message(ddr_log_string);
  /* This call is made in order to know the voltage level , 
   * for determining whether we have LP4 or LP4x on the chip */

  if (platform != DALPLATFORMINFO_TYPE_RUMI)
  {
      for (i = 0; i < ddr_get_max_channels(); i++)
      {
        ddrsns_share_data->shrm_ddr_ptr->detected_ddr_device[i].device_type = DDR_TYPE_LPDDR4X;
      }
  }	
}


/* ============================================================================
**  Function : ddr_target_sdi_pass_init
** ============================================================================
*/
/*!
*   @brief
*   This function is called by ddr_initialize_device, handles none cold boot DDR init pass
*
*   @details
*   None
*
*   @param
*   clk_speed: target clock to be initialized
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
void ddr_target_sdi_pass_init(uint32 clk_speed)
{
  char ddr_log_string[200];
  
  snprintf(ddr_log_string, 200, "PASS1 Enter Cookie = 0x%x", ddr_sdi_logs_ptr->pass1_enter_cookie->p_cookie_value);
  boot_log_message(ddr_log_string);
  
  //First pass complete && bimc alt_ares set) or first_pass_expire -> full ddr init     
  /*
  SDI is skipped      :AOSS REG pass1 complete = 0  , AOSS REG pass1 expire = 0 , we check for AOSS REG pass1 complete = 0 only and then call HAL DDR INIT 
  SDI is hanged       :AOSS REG pass1 expire = 1 ,  in this case ALT ARES will be set to 1 by HW , so we check for EXPIRE only and then call HAL DDR INIT 
                      ( no need to  check for ALT ARES specially for this case in order to call HAL DDR INIT) 
  SDI failed in DDR SDI ( ddr sdi init ):AOSS REG pass1 complete = 1  , AOSS REG pass1 expire = 0  , TZ SDI set ALT ARES , so we check for AOSS pass1 complete = 1 & ALT ARES to call HAL DDR INIT 

  */
  if ((HWIO_INF(AOSS_CC_RESET_FSM_CTRL, FIRST_PASS_COMPLETE) && HWIO_AOSS_CC_ALT_RESET_ENABLE_INM(HWIO_AOSS_CC_ALT_RESET_ENABLE_ALLOW_BIMC_ALT_ARES_BMSK)) ||
            HWIO_INF(AOSS_CC_RESET_FSM_STATUS, FIRST_PASS_EXPIRE)||(!HWIO_INF(AOSS_CC_RESET_FSM_CTRL, FIRST_PASS_COMPLETE)))
  {
    ddr_sdi_logs_ptr->pass2_cookie->p_cookie_value = DDR_FULLY_INIT_PASS2_START;
    snprintf(ddr_log_string, 200, "ALT_ARES Set, Before HAL_DDR_Init()  : Freeze IOs = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, DBG PDC Tr = 0x%x, First Pass expire = 0x%x, First Pass complete = 0x%x , BIMC Alt ARES  = 0x%x , PASS2 Cookie = 0x%x ", (HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(AOSS_CC_RESET_FSM_STATUS, DBG_PDC_TRIGGER)), (HWIO_INF(AOSS_CC_RESET_FSM_STATUS, FIRST_PASS_EXPIRE)), (HWIO_INF(AOSS_CC_RESET_FSM_CTRL, FIRST_PASS_COMPLETE))  , HWIO_AOSS_CC_ALT_RESET_ENABLE_INM(HWIO_AOSS_CC_ALT_RESET_ENABLE_ALLOW_BIMC_ALT_ARES_BMSK), ddr_sdi_logs_ptr->pass2_cookie->p_cookie_value);
    boot_log_message(ddr_log_string);
    
    HAL_DDR_Init(ddrsns_share_data, DDR_CH_ALL, DDR_CS_BOTH, clk_speed);
    ddr_sdi_logs_ptr->pass2_cookie->p_cookie_value = DDR_FULLY_INIT_PASS2_COMPLETE;
    
   snprintf(ddr_log_string, 200, "ALT_ARES Set,HAL_DDR_Init() completed: Freeze IOs = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, DBG PDC Tr = 0x%x, First Pass expire = 0x%x, First Pass complete = 0x%x , BIMC Alt ARES  = 0x%x , PASS2 Cookie = 0x%x ", (HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(AOSS_CC_RESET_FSM_STATUS, DBG_PDC_TRIGGER)), (HWIO_INF(AOSS_CC_RESET_FSM_STATUS, FIRST_PASS_EXPIRE)), (HWIO_INF(AOSS_CC_RESET_FSM_CTRL, FIRST_PASS_COMPLETE))  , HWIO_AOSS_CC_ALT_RESET_ENABLE_INM(HWIO_AOSS_CC_ALT_RESET_ENABLE_ALLOW_BIMC_ALT_ARES_BMSK), ddr_sdi_logs_ptr->pass2_cookie->p_cookie_value);
    boot_log_message(ddr_log_string);
  }
  else
  {
    ddr_sdi_logs_ptr->pass2_cookie->p_cookie_value = DDR_RECOVER_PASS2_START;
   
    snprintf(ddr_log_string, 200, "HAL_DDR_SDI_Recover() start: Freeze IOs = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, DBG PDC Tr = 0x%x, First Pass expire = 0x%x, First Pass complete = 0x%x , BIMC Alt ARES  = 0x%x , PASS2 Cookie = 0x%x ", (HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(AOSS_CC_RESET_FSM_STATUS, DBG_PDC_TRIGGER)), (HWIO_INF(AOSS_CC_RESET_FSM_STATUS, FIRST_PASS_EXPIRE)), (HWIO_INF(AOSS_CC_RESET_FSM_CTRL, FIRST_PASS_COMPLETE))  , HWIO_AOSS_CC_ALT_RESET_ENABLE_INM(HWIO_AOSS_CC_ALT_RESET_ENABLE_ALLOW_BIMC_ALT_ARES_BMSK), ddr_sdi_logs_ptr->pass2_cookie->p_cookie_value);
    boot_log_message(ddr_log_string);
      
    //do HAL_DDR_SDI_Recover 
    HAL_DDR_SDI_Recover(ddrsns_share_data->shrm_ddr_ptr, DDR_CH_ALL, DDR_CS_BOTH, clk_speed); 
    ddr_sdi_logs_ptr->pass2_cookie->p_cookie_value = DDR_RECOVER_PASS2_COMPLETE;
   
    snprintf(ddr_log_string, 200, "HAL_DDR_SDI_Recover() end: Freeze IOs = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, DBG PDC Tr = 0x%x, First Pass expire = 0x%x, First Pass complete = 0x%x , BIMC Alt ARES  = 0x%x , PASS2 Cookie = 0x%x ", (HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH01_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH23_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH45_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_0)), (HWIO_INF(DDRCC_CH67_DDRCC_PCC_PC_STA, PC_FREEZE_IO_1)), (HWIO_INF(AOSS_CC_RESET_FSM_STATUS, DBG_PDC_TRIGGER)), (HWIO_INF(AOSS_CC_RESET_FSM_STATUS, FIRST_PASS_EXPIRE)), (HWIO_INF(AOSS_CC_RESET_FSM_CTRL, FIRST_PASS_COMPLETE))  , HWIO_AOSS_CC_ALT_RESET_ENABLE_INM(HWIO_AOSS_CC_ALT_RESET_ENABLE_ALLOW_BIMC_ALT_ARES_BMSK), ddr_sdi_logs_ptr->pass2_cookie->p_cookie_value);
    boot_log_message(ddr_log_string);
  }
}

/* ============================================================================
**  Function : ddr_pre_init
** ============================================================================
*/
/*!
*   @brief
*   This function is called before ddr is initialized. It will take care of any
*   pre initialization workarounds.
*
*   @details
*   This function is called before ddr is initialized. It will take care of any
*   pre initialization workarounds.
*
*   @param
*   boolean -
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
boolean ddr_pre_init()
{
  return TRUE;
}

/* ============================================================================
**  Function : ddr_post_init
** ============================================================================
*/
/*!
*   @brief
*   This function is called after ddr is initialized. It will take care of any
*   post initialization activities such as ddr training.
*
*   @details
*   This function is called after ddr is initialized. It will take care of any
*   post initialization activities such as ddr training.
*
*   @param
*   boolean -
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
boolean ddr_post_init()
{
  char ddr_log_string[50];
  uint32 i , size_cs0 = 0, size_cs1 = 0;
  DalPlatformInfoPlatformType platform = DalPlatformInfo_Platform();

  snprintf (ddr_log_string, 50, "DSF version = %d.%d.%d, DSF SHRM version = %d.%d", 
            ((ddrsns_share_data->shrm_ddr_ptr->boot_dsf_version) >> 16),(((ddrsns_share_data->shrm_ddr_ptr->boot_dsf_version) & 0x0000FF00)>>8),((ddrsns_share_data->shrm_ddr_ptr->boot_dsf_version) & 0x000000FF),
             ((ddrsns_share_data->shrm_ddr_ptr->shrm_dsf_version) >> 16),((ddrsns_share_data->shrm_ddr_ptr->shrm_dsf_version) & 0x0000FFFF));
  boot_log_message(ddr_log_string);
  
  snprintf (ddr_log_string, 50, "Manufacturer ID = %x, Device Type = %x", ddrsns_share_data->shrm_ddr_ptr->detected_ddr_device[0].manufacturer_id,
            ddrsns_share_data->shrm_ddr_ptr->detected_ddr_device[0].device_type);
  boot_log_message(ddr_log_string);
  
  snprintf (ddr_log_string, 50, "LPDDR4Y  Enabled = %x", ddrsns_share_data->shrm_ddr_ptr->ddr_runtime.lpddr4y_support);
  boot_log_message(ddr_log_string);
  
  for (i = 0; i < ddr_get_max_channels(); i++)
  {
    size_cs0+= ddrsns_share_data->shrm_ddr_ptr->ddr_size_info.ddr_cs0_mb[i];
    size_cs1+= ddrsns_share_data->shrm_ddr_ptr->ddr_size_info.ddr_cs1_mb[i];
  }
  snprintf (ddr_log_string, 50, "Rank 0 size = %d MB, Rank 1 size = %d MB", size_cs0, size_cs1);
  boot_log_message(ddr_log_string);
  
  if (platform != DALPLATFORMINFO_TYPE_RUMI)
  {
    if (PcdGet32 (PcdBuildType) == 0)
    {
      railway_corner_vote(ebi_pin, RAILWAY_NO_REQUEST);
      railway_transition_rails();
    }
    // Clock_SetBIMCSpeed(300000);
    // print_ddr_frequency();
  }
    //HAL_DDR_Post_Training_Init(ddrsns_share_data, DDR_CH_ALL, DDR_CS_BOTH);
	return TRUE;
}

void print_ddr_frequency()
{
  uint32 clk_period;
  char ddr_freq[50];
  
  clk_period = HWIO_IN(CABO_BROADCAST_CABO_GLOBAL_DDR_CLK_PERIOD_CFG);
  snprintf (ddr_freq, 50, "Frequency = %d MHz", 1000000/clk_period);
  boot_log_message(ddr_freq);
}

/* ============================================================================
**  Function : ddr_pre_clock_switch
** ============================================================================
*/
/*!
*   @brief
*   This function is called right before clock switching occures.
*   The function will configure the ddr such that no data loss will occur
*
*   @details
*   DDR will be stalled and new timing parameter is loaded into shadow.
*   Depending on bumping up or stepping down clock speed, we will load the
*   shadow register value to actual registers before or after clock switch
*   has occurred.
*
*   @param curr_clk   -   [IN] the current clock speed
*   @param new_clk    -  [IN] the clock speed we are switching to
*   @param new_clk    -  [IN] interface to switch clock for
*
*   @par Dependencies
*
*
*   @par Side Effects
*   None
*
*   @retval  None
*
*   @sa None
*/

void ddr_pre_clock_switch(uint32 curr_clk, uint32 new_clk)
{
  HAL_DDR_Pre_Clock_Switch(ddrsns_share_data, DDR_CH_ALL, curr_clk, new_clk);
} /* ddr_pre_clock_switch */


/* ============================================================================
**  Function : ddr_clock_switch
** ============================================================================
*/
/*!
*   @brief
*   This function is called right after pre clock switching function.
*   The function will actually do the real clock switch
*
*   @details
*   DDR will be stalled and new timing parameter is loaded into shadow.
*   Depending on bumping up or stepping down clock speed, we will load the
*   shadow register value to actual registers before or after clock switch
*   has occurred.
*
*   @param curr_clk   -   [IN] the current clock speed
*   @param new_clk    -  [IN] the clock speed we are switching to
*   @param new_clk    -  [IN] interface to switch clock for
*
*   @par Dependencies
*
*
*   @par Side Effects
*   None
*
*   @retval  None
*
*   @sa None
*/

void ddr_clock_switch(uint32 curr_clk, uint32 new_clk)
{
  HAL_DDR_Clock_Switch(ddrsns_share_data, DDR_CH_ALL, curr_clk, new_clk);
} /* ddr_clock_switch */


/* ============================================================================
**  Function : ddr_post_clock_switch
** ============================================================================
*/
/*!
*   @brief
*   This function is called right after clock switching occurs.
*   The function will configure the ddr such that no data loss will occur
*
*   @details
*   DDR will be unstalled.
*   Depending on bumping up or stepping down clock speed, we will load the
*   shadow register value to actual registers before or after clock switch
*   has occurred.
*
*   @param curr_clk          -  [IN] the current clock speed
*   @param new_clk           -  [IN] the clock speed we are switching to
*   @param interface_name    -  [IN] interface to switch clock for
*
*   @par Dependencies
*   This code has to be on IRAM because ddr is unavailable during clock switching
*
*   @par Side Effects
*   None
*
*   @retval  None
*
*   @sa None
*/

void ddr_post_clock_switch(uint32 curr_clk, uint32 new_clk)
{
     HAL_DDR_Post_Clock_Switch(ddrsns_share_data, DDR_CH_ALL, curr_clk, new_clk);
} /* ddr_post_clock_switch */

/* ============================================================================
**  Function : ddr_pre_vddmx_switch
** ============================================================================
*/
/*!
*   @brief
*   This function is called right before voltage switch occurs.
*
*   @param vddmx_microvolts - vddmx voltage in microvolts
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval None
*
*   @sa None
*/

void ddr_pre_vddmx_switch(uint32 vddmx_microvolts)
{
  /* Stepping Down in VDDCX voltage */
  ddr_status.vddmx_voltage = vddmx_microvolts;
} /* ddr_pre_vddmx_switch */

/* ============================================================================
**  Function : ddr_post_vddmx_switch
** ============================================================================
*/
/*!
*   @brief
*   This function is called right after voltage switch occurs.
*
*   @param vddmx_microvolts - vddmx voltage in microvolts
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval None
*
*   @sa None
*/

void ddr_post_vddmx_switch(uint32 vddmx_microvolts)
{

} /* ddr_post_vddmx_switch */

/* ============================================================================
**  Function : ddr_pre_vddcx_switch
** ============================================================================
*/
/*!
*   @brief
*   This function is called right before vddcx switch.
*
*   @param settings - contains the VDDCX voltage level we just switched to
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval None
*
*   @sa None
*/
void ddr_pre_vddcx_switch(uint32 vddcx_microvolts)
{
} /* ddr_pre_vddcx_switch */

/* ============================================================================
**  Function : ddr_post_vddcx_switch
** ============================================================================
*/
/*!
*   @brief
*   This function is called right after VDDCX is switched
*
*   @param none
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval None
*
*   @sa None
*/

void ddr_post_vddcx_switch(uint32 vddcx_microvolts)
{
} /* ddr_post_vddcx_switch */

/* ============================================================================
**  Function : ddr_pre_xo_shutdown
** ============================================================================
*/
/**
*   @brief
*   Called right before XO shutdown. Puts DDR into self refresh mode and
*   disables CDC and I/O calibration.
*
*   @param[in]  clk_speed    Current clock speed
*
*   @return
*   None
*
*   @dependencies
*   None
*
*   @sideeffects
*   None
*
*   @sa
*   ddr_post_xo_shutdown
*/

void ddr_pre_xo_shutdown(uint32 clk_speed)
{
  ddr_enter_self_refresh_all(clk_speed);
} /* ddr_pre_xo_shutdown */

/* ============================================================================
**  Function : ddr_post_xo_shutdown
** ============================================================================
*/
/**
*   @brief
*   Called right after XO wakeup. Takes DDR out of self refresh mode and enables
*   CDC and I/O calibration.
*
*   @param[in]  clk_speed    Current clock speed
*
*   @return
*   None
*
*   @dependencies
*   None
*
*   @sideeffects
*   None
*
*   @sa
*   ddr_pre_xo_shutdown
*/

void ddr_post_xo_shutdown(uint32 clk_speed)
{
  ddr_exit_self_refresh_all(clk_speed);

} /* ddr_post_xo_shutdown */

/* ============================================================================
**  Function : ddr_check_partition
** ============================================================================
*/
/*!
*   @brief
*   A helper function for ddr_dmm_partition to check the given interface and partition
*   whether we can put into dpd or full\partial self refresh
*
*   @details
*   Given the retention and active state, put the corresponding ddr interface
*   into self refresh, or deep power down when possible
*
*   Truth table for active and retention state:
*                        Active State:0         |      Active_state:1
*   Retention_state:0    self refresh/dpd       |      Invalid config, assume to be
*                                               |      Active retention
*   Retention_state:1    self refresh retention |      Active Retention
*
*   @param interface_name   -  [IN] the interface to check for
*   @param chip_sel         -  [IN] the chip select on the interface to check for
*   @param retention_state  -  [IN] the retention state for the partitions given
*   @param active_state     -  [IN] the active state for the partitions given
*   @param num_partitions   -  [IN] the number of partitions on this interface
*
*   @par Dependencies
*   None
*
*   @par Side Effects
*   None
*
*   @retval
*   None
*
*   @sa None
*/
void ddr_check_partition(DDR_CHANNEL interface_name, DDR_CHIPSELECT chip_sel, uint32 retention_state, uint32 active_state, uint8 num_partitions)
{

} /* ddr_check_partition */

/* ============================================================================
**  Function : ddr_dmm_partition
** ============================================================================
*/
/*!
*   @brief
*   Given the retention and active state, put the corresponding ddr interface
*   into self refresh, or deep power down when possible
*
*   @details
*   Given the retention and active state, put the corresponding ddr interface
*   into self refresh, or deep power down when possible
*
*   Truth table for active and retention state:
*                        Active State:0         |      Active_state:1
*   Retention_state:0    self refresh/dpd       |      Invalid config, assume to be
*                                               |      Active retention
*   Retention_state:1    self refresh retention |      Active Retention
*
*   @param retention_state  -  [IN] the retention state for the partitions given
*   @param active_state     -  [IN] the active state for the partitions given
*
*   @par Dependencies
*   Caller of this API has to take care not to put ddr interface that is in use
*   into self refresh or deep power down.
*
*   @par Side Effects
*   None
*
*   @retval
*  None
*
*   @sa None
*/

void ddr_dmm_partition(uint32 retention_state, uint32 active_state)
{
}
/* =============================================================================
**  Function : ddr_params_is_training_required
** =============================================================================
*/
/**
*   @brief
*   Indicate whether DDR parameter training is required or not. Training is
*   required if and only if DDR itself (e.g. PCDDR3) requires parameter training
*   and DDR parameter partition is invalid. LPDDR3 will always require training syncronization
*   to be done between rpm and sbl
*
*   @param  None
*
*   @retval  TRUE   Training required
*   @retval  FALSE  Training not required
*
*   @dependencies
*   None
*
*   @sideeffects
*   None
*
*   @sa
*   None
*/
boolean ddr_params_is_training_required( void )
{
  DalPlatformInfoPlatformType platform;
  platform = boot_DalPlatformInfo_Platform();
  boolean training_required;
  
  if ((platform != DALPLATFORMINFO_TYPE_RUMI) &&
      (platform != DALPLATFORMINFO_TYPE_VIRTIO))
  {
    training_required = TRAINING_ENABLED;
  }
  else
  {
    training_required = FALSE;
  }
  return training_required;
} /* ddr_params_is_training_required */


/* ============================================================================
**  Function : ddr_is_training_required
** ============================================================================
*/
/*!
*   @brief
*   This function will parse the crc hash and determine if training is required
*   based on serial number, mx\cx\rbcpr hash
*
*   @details
*   This function will parse the crc hash and determine if training is required
*   based on serial number, mx\cx\rbcpr hash
*
*   @param
*   None
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  boolean - Training required/Not required
*
*   @sa None
*/
boolean ddr_is_training_required(void)
{
  uint32 serial_number;

  /* cx/mx/vdda hash variables */
  uint32 combined_checksum = cpr_cx_mx_settings_checksum();

  /* compare checksum for training data in our partition with DDR_STRUCT to see if training is required */
  serial_number = BOOT_HWIO_IN(SERIAL_NUM , 0);

  ddr_printf ( DDR_NORMAL, "DDR: The serial number is %d", serial_number);

  /* combine serial number with voltage checksum for a new seed */
  combined_checksum = serial_number ^ combined_checksum;

  ddr_printf (DDR_NORMAL, "DDR: Checksum on flash is %d", ddrsns_share_data->training_results.checksum);
  ddr_printf (DDR_NORMAL, "DDR: Recomputed checksum is %d", ddr_get_training_checksum(combined_checksum));


  if (ddrsns_share_data->training_results.checksum  != ddr_get_training_checksum(combined_checksum))
  {
    ddr_printf (DDR_NORMAL, "DDR: Training is required");

    return TRUE;
  }

  ddr_printf (DDR_NORMAL, "DDR: Training is not required");
  return FALSE;
} /* ddr_is_training_required */

/* =============================================================================
**  Function : ddr_do_phy_training_init
** =============================================================================
*/
/**
*   @brief
*   Indicates that PHY training needs to be done in SBL1.
*
*   @param  None
*
*   @retval  None
*
*   @dependencies
*   None
*
*   @sideeffects
*   None
*
*   @sa
*   None
*/
void ddr_do_phy_training_init( void )
{
  ddr_printf (DDR_NORMAL, "DDR: Start of HAL DDR Boot Training");
  HAL_DDR_Boot_Training_Init(ddrsns_share_data, DDR_CH_ALL, DDR_CS_BOTH);  
  Clock_SetBIMCSpeed(ddr_boot_up_freq);
  print_ddr_frequency();
	ddr_printf (DDR_NORMAL, "DDR: End of HAL DDR Boot Training");
} /* ddr_do_phy_training */

/* =============================================================================
**  Function : ddr_do_phy_training_restore
** =============================================================================
*/
/**
*   @brief
*   Restore training data results, if phy training is already done
*
*   @param  None
*
*   @retval  TRUE   Training required
*   @retval  FALSE  Training not required
*
*   @dependencies
*   None
*
*   @sideeffects
*   None
*
*   @sa
*   None
*/
boolean ddr_do_phy_training_restore( void )
{
#if ONE_TIME_TRAINING  
  if (ddr_is_training_required())
  {
#endif
    memset(&ddrsns_share_data->training_results, 0x0, sizeof(struct ddr_params_partition));
    return TRUE;
#if ONE_TIME_TRAINING
  }
  else if (!HAL_DDR_Boot_Training_Restore(ddrsns_share_data, DDR_CH_ALL, DDR_CS_BOTH))
  {
    memset(&ddrsns_share_data->training_results, 0x0, sizeof(struct ddr_params_partition));
    return TRUE;
  }
  else
  {
    Clock_SetBIMCSpeed(ddr_boot_up_freq);
    print_ddr_frequency();

    return FALSE;
  }
#endif

} /* ddr_do_phy_training */

/* =============================================================================
**  Function : ddr_post_training
** =============================================================================
*/
/**
*   @brief
*   This function is called after ddr training. It will take care of all post
*   training activities such as computing checksum over training data.
*
*   @param  None
*
*   @retval  TRUE If one time training is enabled
*   @retval  FALSE If one time training is disabled
*
*   @dependencies
*   None
*
*   @sideeffects
*   None
*
*   @sa
*   None
*/
boolean ddr_post_training(void)
{
  uint32 serial_number;
  // uint8 min_ddr_freq = 0;
  // uint8 max_ddr_freq = 8;
  uint32 combined_checksum = cpr_cx_mx_settings_checksum();

  serial_number = BOOT_HWIO_IN(SERIAL_NUM , 0);

  /* combine serial number with voltage checksum for a new seed */
  combined_checksum = serial_number ^ combined_checksum;

  /* update training data checksum */
  ddr_set_training_checksum(combined_checksum);

  /* update training data and log size */
  ddrsns_share_data->training_results.training_data_size = sizeof(ddrsns_share_data->training_results.training_data);
  ddrsns_share_data->training_results.training_log_size = ddr_external_get_training_log_size();
  
  // ddr_freq_set_min_max(min_ddr_freq, max_ddr_freq);

  return ONE_TIME_TRAINING;
}

/*===========================================================================

**  Function :  ddr_post_setup

** ==========================================================================
*/
/*!
* 
* @brief
*    Function which gets executed after DDR setup operations are complete
* 
* @par Dependencies
*   None
*   
* @retval. 
*    None
* @par Side Effects
*   None
*/

void ddr_post_setup()
{
#if 0
   char ddr_log_string[50];
   unsigned int feature_id = 0;
   uint8 num_levels = (sizeof(ddrsns_share_data->aop_params.mc_states)/sizeof(ddrsns_share_data->aop_params.mc_states[0]));
                 
   feature_id = BOOT_HWIO_IN(QFPROM_CORR_PTE_ROW0_LSB,0);
   feature_id = feature_id >> 20;
   feature_id = feature_id&(0xff);
   snprintf(ddr_log_string, 50, "feature ID is %p",feature_id);
   boot_log_message(ddr_log_string);
   if (feature_id == 0x05)
   ddrsns_share_data->aop_params.mc_states[num_levels-1].freq_enable = FALSE;
#endif

   /* Some training failures are seen on 768MHz with Micron 6GB/8GB,
      So training is ignored for 768Mhz and now remove this freq from clock plan as well */
   ddrsns_share_data->aop_params.mc_states[7].freq_enable = FALSE;
/* Put LLCC in cacheable mode. Do this WA in DDR driver till DSF moves it inside their code */
  HAL_DDR_LLCC_cacheable_mode_override_ctrl(ddrsns_share_data, DDR_CH_ALL, CACHEABLE);
}

/*  Remapper Table 
    This table and code will need to be updated based on newer DDR configurations 
*/

RemapperTable RemapTable[] =
{
/* remapping configurations */
  
/*                    SCMO CS0  SCMO CS1           rank_detected  remap_size  BIMC Remapper CS0   BIMC Remapper CS1 */
  /* 2GB Single Rank  { 0x0      N/A         } */   {DDR_CS0,       0x800,     0x80000000,                0},
  /* 2GB Dual Rank    { 0x0      0x80000000  } */   {DDR_CS_BOTH,   0x800,     0x80000000,      0x100000000},
  /* 3GB Single Rank  { 0x0      N/A         } */   {DDR_CS0 ,      0xC00,     0x80000000,                0},
  /* 3GB Dual Rank    { 0x0      0x80000000  } */   {DDR_CS_BOTH,   0xC00,     0x80000000,      0x100000000},
  /* 4GB Single Rank  { 0x0      N/A         } */   {DDR_CS0,      0x1000,     0x80000000,                0},
  /* 4GB Dual Rank    { 0x0      0x80000000  } */   {DDR_CS_BOTH,  0x1000,     0x80000000,      0x100000000},
  /* 6GB Single Rank  { 0x0      N/A         } */   {DDR_CS0,      0x1800,     0x80000000,                0}, 
  /* 6GB Dual Rank    { 0x0      0x100000000 } */   {DDR_CS_BOTH,  0x1800,     0x80000000,      0x140000000},
  /* 8GB Single Rank  { 0x0      N/A         } */   {DDR_CS0,      0x2000,     0x80000000,                0},
  /* 8GB Dual Rank    { 0x0      0x100000000 } */   {DDR_CS_BOTH,  0x2000,     0x80000000,      0x180000000},
  /* 12GB Single Rank { 0x0      N/A         } */   {DDR_CS0,      0x3000,     0x80000000,                0},
  /* 12GB Dual Rank   { 0x0      0x180000000 } */   {DDR_CS_BOTH,  0x3000,     0x80000000,      0x200000000},
  /* 16GB Single Rank { 0x0      N/A         } */   {DDR_CS0,      0x4000,     0x80000000,                0},
  /* 16GB Dual Rank   { 0x0      0x200000000 } */   {DDR_CS_BOTH,  0x4000,     0x80000000,      0xC00000000},
  /* 24GB Dual Rank   { 0x0      0x300000000 } */   {DDR_CS_BOTH,  0x6000,     0x80000000,      0xC00000000},
  /* 32GB Dual Rank   { 0x0      0x400000000 } */   {DDR_CS_BOTH,  0x8000,     0x80000000,      0xC00000000}

};


/* =============================================================================
**  Function : ddr_remapper
** =============================================================================
*/
/**
*   @brief
*   configures ddr to be remapped based on density
*
*   @param void
*
*   @retval  void
*
*   @dependencies
*   ddr initialization has already finished
*
*   @sideeffects
*   None
*
*   @sa
*   None
*/
void ddr_remapper(void)
{
  icbcfg_remap_info_type remap_info;
  uint64 remap_size = 0;

  uint8 cs=0, index;
  RemapperTable *Remap_Tbl_ptr = RemapTable;
  uint8 remap_table_size = (sizeof(RemapTable)/sizeof(RemapTable[0]));
  DDR_CHIPSELECT chip_select = ddrsns_share_data->shrm_ddr_ptr->detected_ddr_device[0].populated_chipselect;
  uint8 cs_loop_inx = 0;
  uint8 i = 0;
  uint64 src_address = 0x0;
  uint64 dest_address = 0x0;

  /*
    remapping configurations
    
                      SCMO CS0   SCMO CS1      BIMC Remapper CS0  BIMC Remapper CS1
    3GB Single Rank   0x0        N/A           0x80000000         N/A
    3GB Dual Rank     0x0        0x80000000    0x100000000        N/A
    4GB Dual Rank     0x0        0x100000000   0x80000000         N/A
    4GB Single Rank   0x0        N/A           0x80000000         N/A
    6GB Dual Rank     0x0        0x100000000   0x40000000         N/A
    8GB Dual Rank     0x0        0x100000000   0x80000000         0x180000000
  */
  
  for (i = 0; i < ddr_get_max_channels(); i++)
  {
    remap_size += ddr_system_size.ddr_cs0[i] + ddr_system_size.ddr_cs1[i];
  }

    /* Find the correct index in the remapper table */
  for (index = 0; index < remap_table_size; index++)
  {
     /* If size and rank matches, use the index to get the correct entry */
     if((Remap_Tbl_ptr[index].remap_size == remap_size) && (Remap_Tbl_ptr[index].rank_detected == chip_select)) 
	 	break;
  }

  /* Remap 16GB single Rank DDR to 2 region of 8GB and remap same as 16GB dual Rank DDR */
  if((Remap_Tbl_ptr[index].remap_size == 0x4000) && (Remap_Tbl_ptr[index].rank_detected == DDR_CS0))
  {
	  ddr_system_size.ddr_cs0[0] = ddr_system_size.ddr_cs0[1] = remap_size / 2;
	  ddr_system_size.ddr_cs0_addr[1] = 0xC00000000;
  }

  /* Remap 32GB Dual Rank DDR - Divide first rank into 2 part with 14GB+2GB and than 2nd rank as 3rd region of 16GB */
  if((Remap_Tbl_ptr[index].remap_size == 0x8000) && (Remap_Tbl_ptr[index].rank_detected == DDR_CS_BOTH))
  {
	  ddr_system_size.ddr_cs0[0] = 0x3800;
	  ddr_system_size.ddr_cs0[1] = 0x800;
	  ddr_system_size.ddr_cs0_addr[1] = 0x800000000;
  }

  if(index >= remap_table_size)
    ddr_printf (DDR_ERROR, "**ERROR Can't find a matching entry in the DDR remapper table**");
  else
  {
    for(cs = DDR_CS1; cs >= DDR_CS0; cs--) {
      remap_info.interleaved = ICBCFG_REMAP_INTERLEAVE_DEFAULT;
      remap_info.deinterleave = FALSE;

      if (cs == DDR_CS0)
	  {
		src_address = ddrsns_share_data->shrm_ddr_ptr->ddr_size_info.ddr_cs0_addr[0];
		dest_address = ddr_system_size.ddr_cs0_addr[0] = Remap_Tbl_ptr[index].bimc_remapper_cs0;
	  }
	  else if(cs == DDR_CS1)
	  {
		src_address = ddrsns_share_data->shrm_ddr_ptr->ddr_size_info.ddr_cs1_addr[0];
		dest_address = ddr_system_size.ddr_cs1_addr[0] = Remap_Tbl_ptr[index].bimc_remapper_cs1;
	  }

      /* if dest addr is 0, will skip mapping */
      if(0 == dest_address) {
         if(cs == DDR_CS1) {
              for (i = 0; i < ddr_get_max_channels(); i++)
              {
                ddrsns_share_data->shrm_ddr_ptr->ddr_size_info.ddr_cs1_remapped_addr[i] = ddrsns_share_data->shrm_ddr_ptr->ddr_size_info.ddr_cs1_addr[i];
              }
        }
        continue;
      }

      for (i = 0; i < ddr_get_max_channels(); i++)
	  {
		  if (ddr_system_size.ddr_cs0[i] && (cs == DDR_CS0))
		  {
		    remap_info.src_addr = src_address;
			remap_info.dest_addr = ddr_system_size.ddr_cs0_addr[i];
			remap_info.size = (uint64)ddr_system_size.ddr_cs0[i] * 1024 * 1024;
		    ICB_RemapEx("/dev/icbcfg/boot", &ddr_physical_size, cs_loop_inx, &remap_info);
		    cs_loop_inx++;
			src_address = src_address + remap_info.size;
		  }
		  if (ddr_system_size.ddr_cs1[i] && (cs == DDR_CS1))
		  {
			remap_info.src_addr = src_address;
			remap_info.dest_addr = ddr_system_size.ddr_cs1_addr[i];
			remap_info.size = (uint64)ddr_system_size.ddr_cs1[i] * 1024 * 1024;
		    ICB_RemapEx("/dev/icbcfg/boot", &ddr_physical_size, cs_loop_inx, &remap_info);
		    cs_loop_inx++; 
			src_address = src_address + remap_info.size;
		  }
	  }

      if(cs == DDR_CS0) {
        /* update internal variable to track new mapping */
        for (i = 0; i < ddr_get_max_channels(); i++)
        {
          ddrsns_share_data->shrm_ddr_ptr->ddr_size_info.ddr_cs0_remapped_addr[i] = dest_address;
        }
      }
      else if(cs == DDR_CS1) {
        for (i = 0; i < ddr_get_max_channels(); i++)
        {
          ddrsns_share_data->shrm_ddr_ptr->ddr_size_info.ddr_cs1_remapped_addr[i] = dest_address;
        }
      }
    }
    // ddr_printf (DDR_NORMAL, "Remapped addresses ddr0_cs0 is %X, ddr1_cs0 is %X, ddr0_cs1 is %X, ddr1_cs1 is %X", 
	  	                              // ddrsns_share_data->ddr_size_info.ddr0_cs0_remapped_addr,
	  	                              // ddrsns_share_data->ddr_size_info.ddr1_cs0_remapped_addr,
	  	                              // ddrsns_share_data->ddr_size_info.ddr0_cs1_remapped_addr,
	  	                              // ddrsns_share_data->ddr_size_info.ddr1_cs1_remapped_addr);
  }

} /* ddr_remapper */

/* ============================================================================
**
**  get_clk_period_regaddr
**
**  Description:
**    Get the DDR clock period register.
**
**  Parameters:
**    None
**
**  Return:
**    clk_period_regaddr
**
**  Dependencies:
**    None
**
** ========================================================================= */

uint32* get_clk_period_regaddr()
{
	uint32 *clock_addr = 0;
	
	clock_addr = (uint32*)HWIO_MCCC_MSTR_CLK_PERIOD_ADDR(SEQ_DDR_SS_MCCC_MCCC_MSTR_OFFSET);
	return clock_addr;
}