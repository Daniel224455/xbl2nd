#ifndef __PM_WLED_H__
#define __PM_WLED_H__

/*! \file  pm_uefi_wled.h 
 *  \n
 *  \brief  PMIC-WLED MODULE RELATED DECLARATION
 *  \details  This file contains functions and variable declarations to support 
 *   the PMIC WLED module.
 *
 *  \n &copy; Copyright (c) 2009-2018 QUALCOMM Technologies Incorporated. All Rights Reserved.
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/19/16   al      Adding APIs for configuring SWIRE default value
03/29/16   al      Updating for newer target
28/05/14   al      Adding APIs for configuring various AMOLED param.
05/20/14   al      Architecture update
05/16/14   al      Add AMOLED, PFM and PSM
03/26/14   al      8994 WLED support
09/17/13   al      Adding WLED IRQ and updating get_status 
01/14/13   al      Replacing enums with type uint32 and removing 'dev_' from api name
12/21/12   al      Updating eumn for SWITCHING FREQUENCY  
10/29/12   al      removed register structs to make it generic and added wled get status api
08/15/12   al	   Changed according to naming conventions
7/27/12    al	   Renamed all Enums,added device index and resource index
03/05/12   sm      Changes parameters for pm_uefi_wled_en_sync
02/08/12   sm      New file
===========================================================================*/

/*===========================================================================
                INCLUDE FILES FOR MODULE
===========================================================================*/
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"
#include "com_dtypes.h"

/*===========================================================================
                     GLOBAL TYPE DEFINITIONS
===========================================================================*/

typedef enum {
  PM_WLED_NONE = 0,
  PM_WLED_LED1 = 1,
  PM_WLED_LED2 = 2,
  PM_WLED_LED3 = 4,
  PM_WLED_LED4 = 8,
  PM_WLED_ALL = PM_WLED_LED1 | PM_WLED_LED2 |PM_WLED_LED3 | PM_WLED_LED4,
  PM_WLED_INVALID
} pm_wled_led_type;


typedef enum
{
  PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_12BIT,
  PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_15BIT,
  PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_INVALID,
}pm_wled_modulator_brightness_width_type;

typedef enum {
  PM_WLED_OVP_THR_38P5V = 0,   // 38.5V 
  PM_WLED_OVP_THR_37V,         // 37V   
  PM_WLED_OVP_THR_35p5V,       // 35.5V 
  PM_WLED_OVP_THR_34V,         // 34V   
  PM_WLED_OVP_THR_32p5V,       // 32.5V 
  PM_WLED_OVP_THR_31V,         // 31V   
  PM_WLED_OVP_THR_29P5V,       // 29.5V 
  PM_WLED_OVP_THR_28V,         // 28V   
  PM_WLED_OVP_THR_26P5V,       // 26.5V 
  PM_WLED_OVP_THR_25V,         // 25V   
  PM_WLED_OVP_THR_23P5V,       // 23.5V 
  PM_WLED_OVP_THR_22V,         // 22V   
  PM_WLED_OVP_THR_20P5V,       // 20.5V 
  PM_WLED_OVP_THR_19V,         // 19V   
  PM_WLED_OVP_THR_17P5V,       // 17.5V 
  PM_WLED_OVP_THR_16V,         // 16V   
  PM_WLED_OVP_THR_INVALID
} pm_wled_ovp_threshold_type;


typedef enum {
  PM_WLED_BST_ILIMIT_525mA ,   //525mA 
  PM_WLED_BST_ILIMIT_700mA ,   //700mA 
  PM_WLED_BST_ILIMIT_875mA ,   //875mA 
  PM_WLED_BST_ILIMIT_1050mA,   //1050mA
  PM_WLED_BST_ILIMIT_1225mA,   //1225mA
  PM_WLED_BST_ILIMIT_1400mA,   //1400mA
  PM_WLED_BST_ILIMIT_1575mA,   //1575mA
  PM_WLED_BST_ILIMIT_1750mA,   //1750mA
  PM_WLED_BST_ILIMIT_INVALID
} pm_wled_bst_ilimit_type;


typedef enum {
  PM_WLED_SLEW_RATE_1X = 0,  //20 nS
  PM_WLED_SLEW_RATE_1p5X ,
  PM_WLED_SLEW_RATE_2X ,
  PM_WLED_SLEW_RATE_2p5X ,
  PM_WLED_SLEW_RATE_INVALID
} pm_wled_slew_rate_type;

typedef enum
{
  PM_WLED_LED_DIGITAL_DTEST_SRC,
  PM_WLED_LED_ANALOG_DTEST_SRC,
  PM_WLED_LED_DTEST_SRC_INVALID,
}pm_wled_led_dtest_src_type;


typedef enum {
  PM_WLED_IRQ_ILIM_FAULT ,
  PM_WLED_IRQ_OVP_FAULT,
  PM_WLED_IRQ_SC_FAULT,
  PM_WLED_IRQ_FLASH_SAFETY_TIMER,
  PM_WLED_IRQ_PRE_FLASH_DONE,
  PM_WLED_IRQ_FLASH_DONE,
  PM_WLED_IRQ_FLASH_EXIT,
  PM_WLED_IRQ_INVALID
} pm_wled_irq_type;

typedef enum
{
  PM_MODULATOR_TYPE_A,
  PM_MODULATOR_TYPE_B,
  PM_MODULATOR_TYPE_INVALID,
}pm_wled_modulator_type;

typedef enum
{
  PM_MODULATOR_SRC_DRV_HIGH,
  PM_MODULATOR_SRC_CABC1,
  PM_MODULATOR_SRC_CABC2,
  PM_MODULATOR_SRC_EXT,
  PM_MODULATOR_SRC_INVALID,
}pm_wled_modulator_src_type; 

typedef struct
{
  boolean                                  enabled;
  pm_wled_modulator_src_type               src;
  pm_wled_modulator_brightness_width_type  width;
  uint16                                   duty_cycle;
}pm_moulator_status_type;

typedef struct
{
  boolean                enable_current_sink;
  uint32                 current_milliamp;
  pm_wled_modulator_type led_modulator_src;
}pm_led_status_type;

typedef struct
{
  boolean                    module_enable;
  pm_wled_ovp_threshold_type ovp_threshold;
  pm_wled_bst_ilimit_type    bst_ilimit_type;
  uint32                     fsw_ctr;
  pm_moulator_status_type    modulator[2];
  pm_led_status_type         led[4];
}pm_wled_status_type; 

/*===========================================================================
                          FUNCTION DEFINITIONS
===========================================================================*/
/**
 * @brief This function enables WLED module
 * 
 * @param[in] enable
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_enable_module(uint32 pmic_device_index, uint32 resource_index, boolean enable);


/**
 * @brief This function enables digital modulator generator 
 * 
 * @param[in] pmic_device_index : PMIC index
 * 
 * @param[in] modulator: Type of modulator. See #pm_wled_modulator_type for more info
 *
 * @param[in] enable :TRUE to enabled. FALSE otherwise.
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_enable_modulator( uint32 pmic_device_index, pm_wled_modulator_type modulator, boolean enable );



/**
 * @brief This function selects the modulator input source
 * 
 * @param[in] pmic_device_index : PMIC index
 * 
 * @param[in] modulator: Type of modulator. See #pm_wled_modulator_type for more info
 *
 * @param[in] src : Input source for modulator. See #pm_wled_modulator_src_type for more info
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_modulator_src_sel( uint32 pmic_device_index, pm_wled_modulator_type modulator, pm_wled_modulator_src_type src );


/**
 * @brief This function configures modulator brightness width
 * 
 * @param[in] pmic_device_index : PMIC index
 * 
 * @param[in] modulator: Type of modulator. See #pm_wled_modulator_type for more info
 *
 * @param[in] width : Width for modulator brightness. See #pm_wled_modulator_brightness_width_type for more info
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_modulator_brightness_width( uint32 pmic_device_index,  pm_wled_modulator_type modulator, pm_wled_modulator_brightness_width_type  width );


/**
 * @brief This function enables control for LED current sink outputs
 * 
 * @param[in] led_type
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_enable_current_sink(uint32 pmic_device_index, pm_wled_led_type led_type);

/**
 * @brief  Sets the LED full scale current. 
 *
 * @param[in]  pmic_device_index  Primary: 0.
 *
 * @param[in]  led_type           LED type; see #pm_wled_led_type for details.
 *
 * @param[in]  CurrentMicroAmp    LED current 2500 to 30000 uAmp.
 *
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_set_led_full_scale_current(uint32 pmic_device_index, pm_wled_led_type led_type, uint32 current_microamp);


/**
 * @brief This function selects LED modulator source
 * 
 * @param[in]  pmic_device_index: 0 for primary pmic
 *			        
 * @param[in]  led_type: refer enum #pm_wled_led_type
 *				    
 * @param[in]  modulator: modulator type. Refer enum #pm_wled_modulator_type
 *                 
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_led_modulator_src_sel(uint32 pmic_device_index, pm_wled_led_type led_type, pm_wled_modulator_type modulator);


/**
 * @brief This function sets 12bit duty cycle control for WLED brightness     
 * 
 * 
 * @param[in]  pmic_device_index  Primary: 0, Secondary: 1, etc.
 * @param[in]  modulator:         Type of modulator. See #EFI_PM_WLED_MODULATOR_TYPE for more info
 * @param[in]  duty_cycle:        Data is between 0x000 to 0xFFF. This 12-bit data 
 *                                is used to set the current value for the LED 
 *                                duty cycle. The final current for the duty 
 *                                cycle = 12-bit data * LSB value.
 *                                At FSC = 30mA, LSB = 1.875mA
 *                                AT FSC = 25mA, LSB = 1.563mA
 *                                AT FSC = 15mA, LSB = 0.938mA
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_set_led_duty_cycle(uint32 pmic_device_index, pm_wled_modulator_type modulator, uint16 duty_cycle);


/**
 * @brief This function enables LED sync
 * 
 * @param[in] led_type
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_en_sync(uint32 pmic_device_index, pm_wled_led_type led_type, boolean enable);


/**
 * @brief This function selects boost switching frequency control
 * 
 * @param[in]  pmic_device_index:
 *			        0 for primary pmic
 * @param[in]  resource_index:
 *				    valid value 0 
 * @param[in] fsw_khz:
 *                  boost switching frequency in KHz.
 *                  Acceptable values are: 9600, 4800, 3200, 2400, 1920, 1600, 1370, 1200, 1060,  960,  872,  800, 738, 685, 640, 600
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_set_fsw_ctrl(uint32 pmic_device_index, uint32 resource_index, uint32   fsw_khz);

/**
 * @brief This function selects OVP threshold
 * 
 * @param[in] ovp_threshold_type
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_sel_ovp_thr(uint32 pmic_device_index, uint32 resource_index, pm_wled_ovp_threshold_type ovp_threshold);


/**
 * @brief This function selects boost current limit
 * 
 * @param[in] bst_ilimit_type
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_sel_bst_ilimit(uint32 pmic_device_index, uint32 resource_index, pm_wled_bst_ilimit_type bst_ilimit_type);

/**
 * @brief This function detects and enables supported current strings
 * 
 * @param[in] device_index. Primary: 0. Secondary: 1
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_autoselect_current_strings(uint32 pmic_device_index);

/**
 * @brief This function enables or disables WLED irq
 * 
 * @param[in] device_index. Primary: 0. Secondary: 1
 * @param[in] irq:  
 *                WLED irq type. Refer enum pm_wled_irq_type.
 *  
 * @param[in] enable:  
 *                TRUE: Enable corresponding WLED interrupt 
 *                FALSE:Disable corresponding WLED interrupt
 *
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_irq_enable(uint32 pmic_device_index, pm_wled_irq_type irq, boolean enable);


/**
 * @brief This function clears the WLED irq 
 * 
 * @param[in] device_index. Primary: 0. Secondary: 1
 * @param[in] irq:  
 *                WLED irq type. Refer enum pm_wled_irq_type.  
 *
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_irq_clear(uint32 pmic_device_index, pm_wled_irq_type irq);

/**
 * @brief This function configures the WLED irq trigger 
 * 
 * @param[in] device_index. Primary: 0. Secondary: 1
 * @param[in] irq:  
 *                WLED irq type. Refer enum pm_wled_irq_type.  
 * @param[in] trigger:  
 *                One of different irq triggers. Refer enum pm_irq_trigger_type 
 *                from pm_uefi_irq.h for more details
 *
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_irq_set_trigger(uint32 pmic_device_index, pm_wled_irq_type irq, pm_irq_trigger_type trigger);

/**
 * @brief This function reads the WLED irq 
 * 
 * @param[in] device_index. Primary: 0. Secondary: 1
 * @param[in] irq:  
 *                WLED irq type. Refer enum pm_wled_irq_type. 
 * @param[in] type:  
 *                Type of IRQ status to read. Refer enum pm_irq_status_type 
 *                from pm_uefi_irq.h for more details
 * @param[out] status:  
 *                IRQ status.
 *
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_irq_status(uint32 pmic_device_index, pm_wled_irq_type irq, pm_irq_status_type type, boolean *status);

/**
 * @brief This function gets the current status for wled:

 * 
 * @param[in] pmic_device_index : Pmic Chip device index.
 *                                For Primary :0
 *      
 *             
 * @param[out] wled_status       :refer pm_wled_status_type for more info  
 *                                pm_wled_led_type set for enabling modulator;
 *                                pm_wled_led_type set for enabling current sink;
 *                                value set for slope trim;
 *                                pm_wled_pwm_mode_type set for pwm mode;
 *                                pm_wled_mod_clk_type set for mod clk;
 *                                pm_wled_mod_dim_mode_type set for mod dim mode;
 *                                value set for fsw ctrl;
 *                                value set for ramp time;
 *                                pm_wled_bst_ilimit_type set for bst ilimit;
 *                                pm_wled_slew_rate_type set for slew rate;
 *                                pm_wled_vref_ctrl_type set for vref ctrl;
 *                                pm_wled_warmup_dly_type set for warmup dly;
 *                                pm_wled_ovp_threshold_type set for ovp thr; 
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG_FEATURE_NOT_SUPPORTED = Feature not available on this
 *          version of the PMIC.
 *          PM_ERR_FLAG_SUCCESS               = SUCCESS.
 *
 */
pm_err_flag_type pm_wled_get_status(uint32 pmic_device_index, pm_wled_status_type *wled_status);

#endif /* __PM_WLED_H__ */

