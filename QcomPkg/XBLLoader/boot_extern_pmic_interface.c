/*===========================================================================

                    BOOT EXTERN PMIC DRIVER DEFINITIONS

DESCRIPTION
  Contains wrapper definition for external pmic drivers

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None
  
Copyright 2011-2019 by Qualcomm Technologies, Incorporated.
                All Rights Reserved.
============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.
    
    
when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/15/18   pxm     Using haptics API instead pm_vib for vibration on PM855B
12/19/17   pxm     Export WLED/LAB/IBB/LDO APIs for display.
05/09/17   sj      Added Api to qurey the battery status in XBL ramdump
03/07/17   ds      Added boot_pm_aop_pre_init
01/26/17   yps     Added boot_pm_pbs_get_pon_reason_hist
02/18/16   aab     Added boot_pm_system_init()
10/05/15   kpa     Changed boot_pm_ps_hold_cfg() API to boot_pm_app_pon_pshold_cfg() to match pmic API name
09/29/15   kpa     Added support to check for warm reset status
07/11/15   rp	   Changed boot_pm_enable_led api to have LED color as an additional parameter
05/31/15   aab     Replace pm_oem_init() with pm_sbl_chg_init()  
03/27/15   kpa     Added boot_pm_device_programmer_init
01/06/15   kt      Changed PMIC PS_HOLD config API call for multi-PMIC support.
08/20/14   aab     Updated LED blinking intensity to PM_RGB_DIM_LEVEL_HIGH
09/24/14   ck      Removed FEATURE_BOOT_PON_VIBRATION flag
05/22/13   yp      Add boot_pm_vib_on and boot_pm_vib_off
05/22/13   aus     Updated RAM dump LED to support multiple targets
04/02/13   dh      Add boot_pm_pon_ps_hold_cfg.
                   Change LED light to blue and brightness to high
03/29/13   plc     Add LED Functions
02/26/13   dh      Add boot_pm_init_smem
04/18/12   dh      change boot_pm_get_power_on_status to boot_pm_dev_get_power_on_reason
09/08/11   dh      Initial Creation.

===========================================================================*/

/*==========================================================================

                               INCLUDE FILES

===========================================================================*/
#include "boot_extern_pmic_interface.h"
#include "boot_logger.h"
#include "boot_target.h"
#include "adc.h"


/*battery threshold for enable the disply in RAMDUMP mode*/
#define BATT_DISPLAY_THRESHOLD   (3700*2) //2S devices

/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/ 
/*===========================================================================

**  Function :  boot_pm_device_init

** ==========================================================================
*/
/*! 
 * @brief This function executes the pmic device initialization
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_device_init(void)
{
  return pm_device_init();
}



boolean boot_check_display_vbatt(void)
{  
   adc_status_type  adc_status = ADC_STATUS_SUCCESS;
   adc_result_type  adc_result;
   uint32           vbat = 0;

   adc_status = adc_read(ADC_INPUT_VPH_PWR, &adc_result);

   if (adc_status == ADC_STATUS_SUCCESS)
   {
      vbat = adc_result.physical * 2; //2S
   }

   if ((vbat >= BATT_DISPLAY_THRESHOLD) && (adc_status == ADC_STATUS_SUCCESS))
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }

}

/*===========================================================================

**  Function :  boot_pm_dev_get_power_on_reason

** ==========================================================================
*/
/*!
 * @brief  This function returns the phone power-on reason. Typically used in boot
    during early bootup and stored in memory for later access.
 *
 * INPUT PARAMETERS
 * @param pmic_device_index When the target has more than
 *          one pmic, this is the index of the PMIC in which
 *          the power on module is physically located. The device
 *          index starts at zero.
 *@param pwr_on_reason
 *    - pointer to 64-bit unsigned integer that stores the all PON reasons
 *   including PON power on, Warm Reset Reason and POFF_REASON,
 *   SOFT_RESET_REASON.
 *    PON_REASON
 *    PON_WARM_RESET_REASON
 *    PON_POFF_REASON
 *    PON_SOFT_RESET_REASON
 *
 * @return pm_err_flag_type.
 *         PM_ERR_FLAG__PAR1_OUT_OF_RANGE     = Input Parameter one is out of range.
 *         PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this 
 *                                       version of the PMIC.
 *         PM_ERR_FLAG__SBI_OPT_ERR           = The SBI driver failed to communicate
 *                                       with the PMIC.
 *         PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 *@par Dependencies
 *      and pm_init() must have been called.
===========================================================================*/
pm_err_flag_type boot_pm_dev_get_power_on_reason(unsigned pmic_device_index, uint64* pwr_on_reason)
{
  return pm_pon_get_all_pon_reasons(pmic_device_index, pwr_on_reason);
}

/*=========================================================================== 
  
**  Function :  boot_pm_pbs_get_pon_reason_history 
  
** ========================================================================== 
*/ 
/*! 
 * @brief  This function returns the pon reason history for past three resets 
 * 
  
* INPUT PARAMETERS 
 *@param pon_reason_history_ptr 
 *    - pointer to 64-bit unsigned integer that stores the all PON reasons 
 *   for the past three resets 
 * 
 * @return pm_err_flag_type. 
 *         PM_ERR_FLAG__PAR1_OUT_OF_RANGE     = Input Parameter one is out of range. 
 *         PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this  
 *                                       version of the PMIC. 
 *         PM_ERR_FLAG__SBI_OPT_ERR           = The SBI driver failed to communicate 
 *                                       with the PMIC. 
 *         PM_ERR_FLAG__SUCCESS               = SUCCESS. 
 * 
 *@par Dependencies 
 *      and pm_init() must have been called. 
===========================================================================*/ 
pm_err_flag_type boot_pm_pbs_get_pon_reason_history(pm_pbs_pon_reason_history_data_type* pon_reason_history_ptr) 
{ 
  return pm_pbs_get_pon_reason_history(pon_reason_history_ptr); 
} 

/*===========================================================================

**  Function :  boot_pm_driver_init

** ==========================================================================
*/
/*! 
 * @brief This function executes the pmic device initialization
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_driver_init(void)
{
  return pm_driver_init();
}


/*===========================================================================

**  Function :  boot_pm_sbl_chg_init

** ==========================================================================
*/
/*! 
 * @brief This function executes the pmic sbl charging initialization
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_sbl_chg_init(void)
{
  return pm_sbl_chg_init();  
}

/*===========================================================================

**  Function :  boot_pm_system_init

** ==========================================================================
*/
/*! 
 * @brief This function executes the pmic post initialization
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_system_init(void)
{
  return pm_system_init();  
}


/*===========================================================================

**  Function :  boot_pm_init_smem

** ==========================================================================
*/
/*! 
 * @brief This function executes the pmic smem initialization
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_init_smem(void)
{
  return pm_smem_init();
}


/*===========================================================================

**  Function :  boot_pm_enable_led

** ==========================================================================
*/
/*! 
 * @brief This function executes the pmic rgb led functions to turn on LED
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_enable_led(uint32 led_color, boolean current_led_status)
{
  /* Enable LED based on input color parameter */
  return pm_rgb_led_config(2, PM_RGB_1, led_color,  
                           PM_RGB_DIM_LEVEL_HIGH, 
                           current_led_status);
}


/*===========================================================================

**  Function :  boot_pm_app_pon_pshold_cfg

** ==========================================================================
*/
/**
 * @brief Configures PMIC to act on MSM PS_HOLD toggle. This
 *        is an app level API which handles all the required
 *        PS_HOLD config for all the applicable PMICs
 *        internally.
 *  
 * @details Configure PMIC to act on MSM PS_HOLD state. 
 * 
 * @param ps_hold_cfg 
 *          PM_APP_PON_CFG_WARM_RESET,
 *          PM_APP_PON_CFG_HARD_RESET,
 *          PM_APP_PON_CFG_NORMAL_SHUTDOWN,
 *          
 *   
 *                   
 * @return pm_err_flag_type PM_ERR_FLAG__SUCCESS = SUCCESS else 
 *         ERROR.
 */
pm_err_flag_type boot_pm_app_pon_pshold_cfg
(
pm_app_pon_reset_cfg_type ps_hold_cfg
)
{
  return pm_app_pon_pshold_cfg(ps_hold_cfg);
}


/*===========================================================================

**  Function :  boot_pm_vib_on

** ==========================================================================
*/
/*! 
 * @brief This function initializes vibration driver voltage and turn on it
 * @return Error flag.
 *
 */ 

pm_err_flag_type boot_pm_vib_on(void)
{
    return pm_app_haptics_vib_ctrl(PM_ON);
}

/*===========================================================================

**  Function :  boot_pm_vib_off

** ==========================================================================
*/
/*! 
 * @brief This function turn off vibration
 * @return Error flag.
 *
 */ 

pm_err_flag_type boot_pm_vib_off(void)
{
    return pm_app_haptics_vib_ctrl(PM_OFF);
}


/*===========================================================================

**  Function :  boot_pm_pon_warm_reset_status

** ==========================================================================
*/
/*! 
 * @brief This function provides warm reset status information
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_pon_warm_reset_status
(
  unsigned pmic_device_index, 
  boolean *status
)
{
  return pm_pon_warm_reset_status(pmic_device_index, status);
}


/*===========================================================================

**  Function :  boot_pm_pon_warm_reset_status_clear

** ==========================================================================
*/
/*! 
 * @brief This function clears the warm reset status
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_pon_warm_reset_status_clear
(
  unsigned pmic_device_index
)
{
  return pm_pon_warm_reset_status_clear(pmic_device_index);
}


/*===========================================================================

**  Function :  boot_pm_pon_wdog_cfg

** ==========================================================================
*/
/*!
 *@brief  This function will configure the wdog with the given timer value 
 * and with the given reset configuration type.
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param s1_timer
 *@param s2_timer
 *@param reset_cfg_type
 *
 *
 *@return pm_err_flag_type.
 *
 *
 @par Dependencies
 *api boot_pm_device_init() must have been called before calling this API
 *
 */
pm_err_flag_type boot_pm_pon_wdog_cfg
(
	uint8 pmic_device_index, 
	uint32 s1_timer, 
	uint32 s2_timer, 
	pm_pon_reset_cfg_type reset_cfg_type
)
{
	return pm_pon_wdog_cfg(pmic_device_index,s1_timer, s2_timer, reset_cfg_type);
}

/*===========================================================================

**  Function :  boot_pm_pon_wdog_enable

** ==========================================================================
*/
/*!
 *@brief  This function will enable the wdog with the given pmic_device_index.
 *
 * INPUT PARAMETERS
 *@param pmic_device_index
 *@param enable flag
 *
 *@return pm_err_flag_type
 *
 *
 *@par Dependencies
 * api boot_pm_pon_wdog_cfg() must have been called before calling this API
 *
 */
pm_err_flag_type boot_pm_pon_wdog_enable
(
	uint8 pmic_device_index,
	pm_on_off_type enable
)
{
	return pm_pon_wdog_enable(pmic_device_index,enable);
}
 
/*===========================================================================

**  Function :  boot_pm_pon_wdog_disable

** ==========================================================================
*/
/*!
 *@brief  This function will disable the wdog with the given pmic_device_index.
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param disable flag
 *
 *@return pm_err_flag_type.
 *
 *@par Dependencies
 * api boot_pm_pon_wdog_cfg() must have been called before calling this API
 *
 */
pm_err_flag_type boot_pm_pon_wdog_disable
(
	uint8 pmic_device_index,
	pm_on_off_type disable
)
{
	return pm_pon_wdog_enable(pmic_device_index,disable);
}

/*===========================================================================

**  Function :  boot_pm_aop_pre_init

** ==========================================================================
*/
/*! 
 * @brief This function executes the pmic pre AOP initialization
 * @return Error flag.
 *
 */ 
pm_err_flag_type boot_pm_aop_pre_init(void)
{
  return pm_aop_pre_init();
}

/*===========================================================================

**  Function :  boot_pm_ldo_sw_enable

** ==========================================================================
*/
/*!
 *@brief Enables or disables a regulator or voltage switcher.
 *
 *  @param[in] pmic_chip            Primary PMIC: 0, Secondary PMIC: 1.
 *
 *  @param[in] ldo_peripheral_index LDO peripheral index. The first LDO
 *                                   peripheral starts at 0. See
 *                                  @xnameref{hdr:ldoPeripheralIndex}.
 *
 *  @param[in] on_off               Turns the regulator on and off.
 *                                  See #pm_on_off_type.
 *
 *  @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type boot_pm_ldo_sw_enable (uint8 pmic_chip, uint8  ldo_peripheral_index, pm_on_off_type  on_off, boolean wait_for_settle)
{
  return pm_ldo_sw_enable(pmic_chip, ldo_peripheral_index, on_off, wait_for_settle);
}

/*!
 *@brief Enables or disables a regulator or voltage switcher.
 *
 *  @param[in] pmic_chip            Primary PMIC: 0, Secondary PMIC: 1.
 *
 *  @param[in] smps_peripheral_index SMPS peripheral index. The first SMPS
 *                                   peripheral starts at 0. See
 *                                  @xnameref{hdr:ldoPeripheralIndex}.
 *
 *  @param[in] on_off               Turns the regulator on and off.
 *                                  See #pm_on_off_type.
 *
 *  @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */

pm_err_flag_type boot_pm_smps_sw_enable (uint8 pmic_chip, uint8  smps_peripheral_index, pm_on_off_type  on_off, boolean wait_for_settle)
{
  return pm_smps_sw_enable(pmic_chip, smps_peripheral_index, on_off, wait_for_settle);
}

/*===========================================================================

**  Function :  boot_pm_ldo_volt_level

** ==========================================================================
*/
/*!
 *@brief  Sets the voltage level for an LDO.
 * 
 * @param[in] pmic_chip            Primary PMIC: 0, Secondary PMIC: 1.
 *
 * @param[in] ldo_peripheral_index LDO peripheral index. Starts from 0
 *                                 (for the first LDO peripheral).
 *                                 See @xnameref{hdr:ldoPeripheralIndex}.
 *
 * @param[in] volt_level           Voltage level.
 * 
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type boot_pm_ldo_volt_level(uint8 pmic_chip, uint8 ldo_peripheral_index, pm_volt_level_type volt_level, boolean wait_for_settle)
{
  return pm_ldo_volt_level(pmic_chip, ldo_peripheral_index, volt_level, wait_for_settle);
}

/*===========================================================================

**  Function :  boot_pm_smps_volt_level

** ==========================================================================
*/
/*!
 *@brief  Sets the voltage level for an SMPS.
 * 
 * @param[in] pmic_chip            Primary PMIC: 0, Secondary PMIC: 1.
 *
 * @param[in] ldo_peripheral_index SMPS peripheral index. Starts from 0
 *                                 (for the first SMPS peripheral).
 *                                 See @xnameref{hdr:SMPS PeripheralIndex}.
 *
 * @param[in] volt_level           Voltage level.
 * 
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type boot_pm_smps_volt_level(uint8 pmic_chip, uint8 smps_peripheral_index, pm_volt_level_type volt_level, boolean wait_for_settle)
{
  return pm_smps_volt_level(pmic_chip, smps_peripheral_index, volt_level, wait_for_settle);
}


/*===========================================================================

**  Function :  boot_pm_ibb_ibb_module_rdy

** ==========================================================================
*/
/*!
 * @brief This function prepares module to be enabled or not
 * 
 * @details
 *  This API prepares module to be enabled or not.
 *  When this is set to low, the IBB module cannot be enabled 
 *  (powered-up) through S-Wire or IBB_ENABLE register
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ibb_rdy             TRUE if ready to be enabled else FALSE
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_ibb_ibb_module_rdy(uint32 device_index, boolean ibb_rdy)
{
  return pm_ibb_ibb_module_rdy(device_index, ibb_rdy);
}

/*===========================================================================

**  Function :  boot_pm_ibb_config_ibb_ctrl

** ==========================================================================
*/
/*!
 * @brief This function enables/disables module and makes S-wire ready for listening
 * 
 * @details
 *  This API enables/disables module and makes S-wire ready for listening
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] en_module      TRUE enables module and false disables module
 * @param[in] swire_rdy      TRUE ready to liste to to S-wire and FALSE not ready to listen to S-Wire
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_ibb_config_ibb_ctrl(uint32 device_index, boolean en_module, boolean swire_rdy)
{
  return pm_ibb_config_ibb_ctrl(device_index, en_module, swire_rdy);
}

/*===========================================================================

**  Function :  boot_pm_ibb_get_ibb_status

** ==========================================================================
*/
/*!
 * @brief This reads the IBB module setting status
 * 
 * @details
 *  This API reads the IBB module setting status
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ibb_status          Refer struct #pm_ibb_status_type for more info
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_ibb_get_ibb_status(uint32 device_index, pm_ibb_status_type* ibb_status)
{
  return pm_ibb_get_ibb_status(device_index, ibb_status);
}

/*===========================================================================

**  Function :  boot_pm_lab_config_current_sense

** ==========================================================================
*/
/*!
 * @brief This configures LAB current sense 
 * 
 * @details
 *  This API configures current sense tapping point and current
 *  sense gain
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] isense_tap          : Current sense tapping point. Refer #pm_lab_isense_type for more info
 * @param[in] isense_gain         : Current sense gain. Refer #pm_lab_isense_type for more info
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_lab_config_current_sense(uint32 device_index, pm_lab_isense_type isense_tap, pm_lab_isense_type isense_gain)
{
  return pm_lab_config_current_sense(device_index, isense_tap, isense_gain);
}

/*===========================================================================

**  Function :  boot_pm_lab_config_pulse_skip_ctrl

** ==========================================================================
*/
/*!
* @brief This configures LAB pulse skipping parameters  
 * 
 * @details
 *  This API configures pulse skipping parameters
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ps_threshold      : Threshold when pulse skipping mode is entered. Range is 20 to 50 mAmps
 * @param[in] select_ps_table   : TRUE to slected PS table, otherwise FALSE
 * @param[in] select_digital_ps : TRUE to slected DIGITAL PS, otherwise FALSE for ANALOG
 * @param[in] enable_ps         : TRUE to enable PS, otherwise FALSE to disable it
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_lab_config_pulse_skip_ctrl(uint32 device_index, uint32 ps_threshold, boolean select_ps_table, boolean select_digital_ps, boolean enable_ps)
{
  return pm_lab_config_pulse_skip_ctrl(device_index, ps_threshold, select_ps_table, select_digital_ps, enable_ps);
}

/*===========================================================================

**  Function :  boot_pm_lab_lcd_amoled_sel

** ==========================================================================
*/
/*!
 * @brief This function configures module for AMOLED or LCD
 * 
 * @details
 *  This API configures module for AMOLED or LCD
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] en_amoled_mode      TRUE enables for AMOLED and FALSE for LCD 
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_lab_lcd_amoled_sel(uint32 device_index, boolean en_amoled_mode)
{
  return pm_lab_lcd_amoled_sel(device_index, en_amoled_mode);
}

/*===========================================================================

**  Function :  boot_pm_lab_lab_module_rdy

** ==========================================================================
*/
/*!
 * @brief This function configures module to be enabled or not enabled
 * 
 * @details
 *  This API configures module to be enabled or not enabled
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] lab_rdy             TRUE if ready to be enabled else FALSE
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_lab_lab_module_rdy(uint32 device_index, boolean lab_rdy)
{
  return pm_lab_lab_module_rdy(device_index, lab_rdy);
}

/*===========================================================================

**  Function :  boot_pm_lab_ibb_rdy_en

** ==========================================================================
*/
/*!
 * @brief This function sets LAB to be enabled by IBB or not
 * 
 * @details
 *  This API allows LAB to be enabled by IBB or not
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] ibb_en_rdyTRUE      TRUE allows IBB module to enable LAB and FALSE won't 
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_lab_ibb_rdy_en(uint32 device_index, boolean ibb_rdy_en)
{
  return pm_lab_ibb_rdy_en(device_index, ibb_rdy_en);
}

/*===========================================================================

**  Function :  boot_pm_lab_get_lab_status

** ==========================================================================
*/
/*!
 * @brief This reads the LAB module setting status
 * 
 * @details
 *  This API reads the LAB module setting status
 * 
 * @param[in] pmic_device_index.  Primary: 0 Secondary: 1
 * @param[in] lab_status          Refer struct #pm_lab_status_type for more info
 *       
 *                                
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_lab_get_lab_status(uint32 device_index, pm_lab_status_type* lab_status)
{
  return pm_lab_get_lab_status(device_index, lab_status);
}

/*===========================================================================

**  Function :  boot_pm_wled_sel_ovp_thr

** ==========================================================================
*/
/*!
 * @brief This function selects OVP threshold
 * 
 * @param[in] ovp_threshold_type
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 */
pm_err_flag_type boot_pm_wled_sel_ovp_thr(uint32 pmic_device_index, uint32 resource_index, pm_wled_ovp_threshold_type ovp_threshold_type)
{
  return pm_wled_sel_ovp_thr(pmic_device_index, resource_index, ovp_threshold_type);
}

/*===========================================================================

**  Function :  boot_pm_wled_sel_bst_ilimit

** ==========================================================================
*/
/*!
 * @brief This function selects boost current limit
 * 
 * @param[in] bst_ilimit_type
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG__SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type boot_pm_wled_sel_bst_ilimit(uint32 pmic_device_index, uint32 resource_index, pm_wled_bst_ilimit_type bst_ilimit_type)
{
  return pm_wled_sel_bst_ilimit(pmic_device_index, resource_index, bst_ilimit_type);
}

/*===========================================================================

**  Function :  boot_pm_wled_set_fsw_ctrl

** ==========================================================================
*/
/*!
 *@brief  This function is used to set FSW.
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param resource_index
 *@param fsw_khz
 *
 *@return pm_err_flag_type.
 *
 *@par Dependencies
 * api boot_pm_pon_wdog_cfg() must have been called before calling this API
 *
 */
pm_err_flag_type boot_pm_wled_set_fsw_ctrl(uint32 pmic_device_index, uint32 resource_index, uint32 fsw_khz)
{
  return pm_wled_set_fsw_ctrl(pmic_device_index, resource_index, fsw_khz);
}

/*===========================================================================

**  Function :  boot_pm_wled_set_led_duty_cycle

** ==========================================================================
*/
/*!
 *@brief  This function is uesd to set duty cycle.
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param pm_wled_modulator_type
 *@param duty cycle data
 *
 *@return pm_err_flag_type.
 *
 *@par Dependencies
 * api boot_pm_pon_wdog_cfg() must have been called before calling this API
 *
 */
pm_err_flag_type boot_pm_wled_set_led_duty_cycle(uint32 pmic_device_index, pm_wled_modulator_type modulator, uint16 data)
{
  return pm_wled_set_led_duty_cycle(pmic_device_index, modulator, data);
}

/*===========================================================================

**  Function :  boot_pm_wled_enable_current_sink

** ==========================================================================
*/
/*!
 *@brief  This function is uesd to enable WLED current source.
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param led_type
 *
 *@return pm_err_flag_type.
 */
pm_err_flag_type boot_pm_wled_enable_current_sink(uint32 pmic_device_index, pm_wled_led_type led_type)
{
  return pm_wled_enable_current_sink(pmic_device_index, led_type);
}

/*===========================================================================

**  Function :  boot_pm_wled_enable_modulator

** ==========================================================================
*/
/*!
 *@brief  This function is uesd to enable WLED modulator
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param led_type
 *@param disable flag
 *
 *@return pm_err_flag_type.
 *
 */
pm_err_flag_type boot_pm_wled_enable_modulator(uint32 pmic_device_index, pm_wled_modulator_type modulator, boolean enable)
{
  return pm_wled_enable_modulator(pmic_device_index, modulator, enable);
}

/*===========================================================================

**  Function :  boot_pm_wled_enable_module

** ==========================================================================
*/
/*!
 *@brief  This function is used to enable WLED module.
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param resource_index
 *@param disable flag
 *
 *@return pm_err_flag_type.
 *
 */
pm_err_flag_type boot_pm_wled_enable_module(uint32 pmic_device_index, uint32 resource_index, boolean enable)
{
  return pm_wled_enable_module(pmic_device_index, resource_index, enable);
}

/*===========================================================================

**  Function :  boot_pm_wled_en_sync

** ==========================================================================
*/
/*!
 *@brief  This function is used to enable WLED sync.
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param led_type
 *@param disable flag
 *
 *@return pm_err_flag_type.
 *
 */
pm_err_flag_type boot_pm_wled_en_sync(uint32 pmic_device_index, pm_wled_led_type led_type, boolean enable)
{
  return pm_wled_en_sync(pmic_device_index, led_type, enable);
}


pm_err_flag_type boot_pm_wled_modulator_src_sel( uint32 pmic_device_index, pm_wled_modulator_type modulator, pm_wled_modulator_src_type src )
{
  return pm_wled_modulator_src_sel( pmic_device_index, modulator, src );
}

pm_err_flag_type boot_pm_pwm_enable_pwm(uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, boolean enable)
{
   return pm_pwm_enable_pwm(pmic_chip, pwm_rsrc, enable);
}

pm_err_flag_type boot_pm_pwm_set_pwm_value( uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, uint16 pwm_value)
{
   return pm_pwm_set_pwm_value( pmic_chip, pwm_rsrc, pwm_value);
}

pm_err_flag_type boot_pm_pwm_clock_config( uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, pm_pwm_bit_size_type bit_size, pm_pwm_clock_freq_type clk_freq, uint32 pre_div, uint32 exp)
{
   return pm_pwm_clock_config( pmic_chip, pwm_rsrc, bit_size, clk_freq, pre_div, exp);
}

pm_err_flag_type boot_pm_pwm_glitch_removal(uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, boolean glitch_removal)
{
   return pm_pwm_glitch_removal(pmic_chip, pwm_rsrc, glitch_removal);
}

pm_err_flag_type boot_pm_pwm_config_dtest(uint32 pmic_chip, pm_pwm_resource_type pwm_rsrc, pm_pwm_dtest dtest, uint32 set_value)
{
   return pm_pwm_config_dtest( pmic_chip, pwm_rsrc, dtest, set_value);
}

/*===========================================================================

**  Function :  boot_pm_fg_batt_info_get_vbatt

** ==========================================================================
*/
/*!
 *@brief  This function is uesd to get battery voltage
 *
 *INPUT PARAMETERS
 *@param pmic_device_index
 *@param pVBat
 *
 *@return pm_err_flag_type.
 *
 */
pm_err_flag_type boot_pm_fg_batt_info_get_vbatt(uint32 pmic_device, uint32 *pvbat)
{
  return pm_fg_batt_info_get_vbatt(pmic_device, pvbat);
}

pm_err_flag_type boot_pm_lcdb_config_enable_ctrl(uint32 device_index, pm_lcdb_enable_ctl_type ctl_type, boolean enable)
{
 return pm_lcdb_config_enable_ctrl(device_index, ctl_type, enable);
}


pm_err_flag_type boot_pm_lcdb_set_output_volt(uint32 device_index, int32 ldo_mv, int32 ncp_mv, boolean symmetry_ok)
{
  return pm_lcdb_set_output_volt(device_index, ldo_mv, ncp_mv, symmetry_ok);
}

pm_err_flag_type boot_pm_lcdb_vreg_ok(uint32 device_index, boolean *vreg_ok)
{
  return pm_lcdb_vreg_ok(device_index, vreg_ok);
}

