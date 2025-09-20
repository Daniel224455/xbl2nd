/*! \file
*  
*  \brief  pm_wled.c ---WLED Driver implementation.
*  \details WLED Driver implementation.
*  &copy; Copyright 2012-2018 QUALCOMM Technologies Inc, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE



when       	who     	 what, where, why
--------   	---    	     ---------------------------------------------------------- 
06/02/18    al          Add WA- CR 2211149 and 2197043
10/19/16    al          Adding APIs for configuring SWIRE default value 
03/29/16    al          Updating for newer target
10/13/15    al          Writing overwrite bit for ILIM so that default values can be overwritten 
28/05/14    al          Adding APIs for configuring various AMOLED param.
04/06/15    al          Mask Fix
09/22/14    al          PSM shift fix 
08/20/14    al          Updating comm lib 
08/29/14    al          KW fixes
08/12/14    al          Mask and data fix
05/20/14    al          Architecture update 
06/06/14    al          Fixed mask and data for sink write
05/19/14    al          AMOLED, PSM and PFM support 
03/26/14    al          8994 WLED support 
03/15/13    al          Rewriting according to new architecture 
02/11/13    al          Removing warning
01/14/13    al          Replacing enums with uint32   
11/30/12    al          Accommodating SWITCHING FREQUENCY change in pmic version 3  
11/30/12    al          Fixed vref control and enable modulator 
10/10/12    al          impelemented sleepb and softstart api
08/27/12    al          Removed necessity for additional API call after setting brightness
08/27/12    al          made changes for update condition
08/09/12    al          added api for brightness
08/09/12    al          updated with latest offtarget
07/24/12    al          New file

===========================================================================*/

/*===========================================================================

INCLUDE FILES 

===========================================================================*/
#include "pm_wled.h"
#include "pm_wled_driver.h"


#define ADDRESS_SINK(x) (wled_ptr->wled_reg_table->wled_sink_reg->base_address + wled_ptr->wled_reg_table->wled_sink_reg->x)
#define ADDRESS_CTRL(x) (wled_ptr->wled_reg_table->wled_ctrl_reg->base_address + wled_ptr->wled_reg_table->wled_ctrl_reg->x)
#define SLAVE_ID        (wled_ptr->comm_ptr->slave_id )

#define MAX_AVDD_MV          7900
#define MIN_AVDD_MV          5650
#define AVDD_STEP_SIZE_MV     150

#define PM_LED_MAX_FULL_SCALE_CURRENT_uAmp (30*1000)
#define PM_LED_FULL_SCALE_CURRENT_STEP_SIZE 2500 //2.5milliamp
#define PM_MAX_FULL_SCALE_CURRENT_V1_HW (20*1000) //20milliamp
#define PM_V1_PMIC  1
#define PM_V3_PMIC  3
#define WAIT_5_MILLI_SEC        (5*1000)

#define PM_WLED_BRIGHTNESS_15BIT 0x7FFF
#define PM_WLED_BRIGHTNESS_12BIT 0xFFF
#define PM_WLED_MODULE_ENABLE    0x1
#define PM_WLED_MODULE_DISABLED  0x0

static pm_wled_led_type current_strings_arr[] = {PM_WLED_NONE, PM_WLED_LED1, PM_WLED_LED2, PM_WLED_LED3, PM_WLED_LED4};
static boolean strings_detection_complete = FALSE;

/*************************************
NOTE: VIRTUAL METHOD IMPLEMENTATION
**************************************/
static pm_err_flag_type pm_wled_sink_write(uint32 pmic_device_index, pm_wled_led_type led_type, pm_register_address_type led1_reg_addr, pm_register_data_type data, pm_register_data_type mask);

static pm_err_flag_type pm_wled_sample_and_hold_wa( uint32 pmic_device_index)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;

  uint16 max_brightness = 0xFFFF;

  uint16 current_brightness = 0xFFFF ;

  uint32 brightness_percenatge = 100;

  pm_register_data_type brightness_data[3] = { 0xFF, 0xFF, 0xFF };

  pm_register_data_type sample_hold_enable, sample_hold_disable;

  pm_register_address_type soft_start_reg;

  pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

  if (wled_ptr == NULL)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  pm_register_address_type modulators[4] = { ADDRESS_SINK(modulatora_brightness_width_sel), ADDRESS_SINK(modulatora_brightness_settings_lsb),
                                             ADDRESS_SINK(modulatorb_brightness_width_sel), ADDRESS_SINK(modulatorb_brightness_settings_lsb) };

  pm_register_address_type module_enable = ADDRESS_CTRL(module_enable);

  /*check for both modulator A and B. If for either brightness is < 1% then run WA*/
  for (int i = 0; i < 4; i = i + 2)
  {
    err_flag = pm_comm_read_byte(SLAVE_ID, modulators[i], brightness_data, 0);

    err_flag = pm_comm_read_byte_array(SLAVE_ID, modulators[i+1], 2, &brightness_data[1], 0);

    max_brightness = (brightness_data[0] & 0x01) ? PM_WLED_BRIGHTNESS_15BIT : PM_WLED_BRIGHTNESS_12BIT;

    current_brightness = brightness_data[1] | (brightness_data[2] << 8);

    brightness_percenatge = (current_brightness * 100) / max_brightness;

    if (brightness_percenatge < 1) 
    {
      break;
    }
  }
  
  /*if brightness is <1% then only run WA*/
  if (brightness_percenatge < 1)
  {
    /*S&H enable register has changed after revision 2*/
    if (pm_get_pmic_revision(pmic_device_index) < PM_V3_PMIC)
    {
      soft_start_reg = ADDRESS_CTRL(test4);
      sample_hold_enable = 0xF2;
      sample_hold_disable = 0x52;
    }
    else
    {
      soft_start_reg = ADDRESS_CTRL(sh_for_softstart);
      sample_hold_enable = 0x01;
      sample_hold_disable = 0x00;
    }

    err_flag = pm_comm_write_byte(SLAVE_ID, soft_start_reg, sample_hold_disable, 0);
    err_flag = pm_comm_write_byte(SLAVE_ID, module_enable, PM_WLED_MODULE_ENABLE << 7, 0);
    err_flag = pm_busy_wait(WAIT_5_MILLI_SEC);
    err_flag = pm_comm_write_byte(SLAVE_ID, soft_start_reg, sample_hold_enable, 0);
  }
  else
  {
    err_flag = pm_comm_write_byte(SLAVE_ID, module_enable, PM_WLED_MODULE_ENABLE << 7, 0);
  }

  return err_flag;
}

pm_err_flag_type pm_wled_enable_module( uint32 pmic_device_index, uint32 resource_index, boolean enable )
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);
  (void)resource_index;
  
  pm_register_data_type data = 0;

  if (wled_ptr == NULL)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  pm_register_address_type module_enable = ADDRESS_CTRL(module_enable);

  err_flag = pm_comm_read_byte(SLAVE_ID, module_enable, &data, 0);

  /*if PMIC is 855L, currently module is disabled but the request is to enable module then run work around*/
  if ((PMIC_IS_PM855L == pm_get_pmic_model(pmic_device_index)) && (PM_WLED_MODULE_ENABLE == enable) && (data == PM_WLED_MODULE_DISABLED))
  {
    err_flag |= pm_wled_sample_and_hold_wa(pmic_device_index);
  }
  else
  {
    err_flag |= pm_comm_write_byte(SLAVE_ID, module_enable, enable << 7, 0);
  }

  return err_flag;
}

pm_err_flag_type pm_wled_enable_modulator(uint32 pmic_device_index, pm_wled_modulator_type modulator, boolean enable)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_register_address_type modulator_reg;
  pm_register_data_type data = (enable) ? 0x80 : 0x00;
  pm_register_data_type mask = 0x80;

  pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

  if (wled_ptr == NULL)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }

  if (modulator == PM_MODULATOR_TYPE_A)
  {
    modulator_reg = ADDRESS_SINK(modulatora_en);
  }
  else if (modulator == PM_MODULATOR_TYPE_B)
  {
    modulator_reg = ADDRESS_SINK(modulatorb_en);
  }
  else
  {
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  err_flag = pm_comm_write_byte_mask(SLAVE_ID, modulator_reg, mask, data, 0);

  return err_flag;
}

pm_err_flag_type 
pm_wled_modulator_src_sel( uint32 pmic_device_index, pm_wled_modulator_type modulator, pm_wled_modulator_src_type src )
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type modulator_src_sel_reg;
    pm_register_data_type mask = 0x3;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    
    if (modulator == PM_MODULATOR_TYPE_A) 
    {
      modulator_src_sel_reg = ADDRESS_SINK(modulatora_src_sel);
    }
    else if(modulator == PM_MODULATOR_TYPE_B) 
    {
      modulator_src_sel_reg = ADDRESS_SINK(modulatorb_src_sel);
    }
    else
    {
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, modulator_src_sel_reg, mask, src, 0);

    return err_flag;
}

pm_err_flag_type 
pm_wled_modulator_brightness_width( uint32 pmic_device_index,  pm_wled_modulator_type modulator, pm_wled_modulator_brightness_width_type  width )
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type modulator_width_reg;
    pm_register_data_type mask = 0x1;
    pm_register_data_type data = 0;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if (width >= PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_INVALID)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if (modulator == PM_MODULATOR_TYPE_A) 
    {
      modulator_width_reg = ADDRESS_SINK(modulatora_brightness_width_sel);
    }
    else if(modulator == PM_MODULATOR_TYPE_B) 
    {
      modulator_width_reg = ADDRESS_SINK(modulatorb_brightness_width_sel);
    }
    else
    {
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }

   data = (width == PM_WLED_MODULATOR_BRIGHTNESS_WIDTH_15BIT) ? 1 : 0;

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, modulator_width_reg, mask, data, 0);

    return err_flag;
}


pm_err_flag_type 
pm_wled_set_led_duty_cycle( uint32 pmic_device_index,  pm_wled_modulator_type modulator, uint16  duty_cycle)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type modulator_brightness_lsb_reg;
    pm_register_address_type modulator_sync_bit_reg;
    pm_register_data_type data[2] = {duty_cycle & 0xFF, duty_cycle >> 8};

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if (modulator == PM_MODULATOR_TYPE_A) 
    {
      modulator_brightness_lsb_reg = ADDRESS_SINK(modulatora_brightness_settings_lsb);
    }
    else if(modulator == PM_MODULATOR_TYPE_B) 
    {
      modulator_brightness_lsb_reg = ADDRESS_SINK(modulatorb_brightness_settings_lsb);
    }
    else
    {
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    err_flag = pm_comm_write_byte_array(SLAVE_ID, modulator_brightness_lsb_reg, 2, data, 0);

    /*write 0 and 1 to sync bit for changes to take effect*/
    modulator_sync_bit_reg = ADDRESS_SINK(modulator_sync_bit);
    err_flag = pm_comm_write_byte(SLAVE_ID, modulator_sync_bit_reg, 0x00, 0);
    err_flag = pm_comm_write_byte(SLAVE_ID, modulator_sync_bit_reg, 0xFF, 0);

    return err_flag;
}


pm_err_flag_type pm_wled_enable_current_sink(uint32 pmic_device_index, pm_wled_led_type led_type)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type en_current_sink;

    pm_register_data_type mask, data;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if(led_type >= PM_WLED_INVALID)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if (led_type >= PM_WLED_INVALID) 
    {
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else if (led_type == PM_WLED_NONE)
    {
      mask = 0xF0; 
      data = 0x00;
    }
    else
    {
      data = (led_type << 4);
      mask = data;
    }

    en_current_sink =  ADDRESS_SINK(current_sink_en);

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, en_current_sink, mask, data, 0);

    return err_flag;
}

pm_err_flag_type pm_wled_set_led_full_scale_current(uint32 pmic_device_index, pm_wled_led_type led_type, uint32 current_microamp)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type led1_full_scale_current;

    pm_register_data_type data, mask;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if(led_type >= PM_WLED_INVALID)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }
    /*WA for 855L,A V1 HW to limit full scale current to 20milliamp*/
    if(current_microamp > PM_MAX_FULL_SCALE_CURRENT_V1_HW)
    {
      if (pm_get_pmic_model(pmic_device_index) == PMIC_IS_PM855L) 
      {
        if (pm_get_pmic_revision(pmic_device_index) == PM_V1_PMIC)
        {
          current_microamp = PM_MAX_FULL_SCALE_CURRENT_V1_HW;
        }
      }
    }

    led1_full_scale_current = ADDRESS_SINK(led1_full_scale_current);
    /*data = current_milliamp/2.5 ; current_milliamp range: 0 to 30*/
    if (current_microamp > PM_LED_MAX_FULL_SCALE_CURRENT_uAmp)
    {
       current_microamp = PM_LED_MAX_FULL_SCALE_CURRENT_uAmp;
    }

    data = current_microamp / PM_LED_FULL_SCALE_CURRENT_STEP_SIZE;
    mask = 0x0F;

    err_flag = pm_wled_sink_write(pmic_device_index, led_type, led1_full_scale_current, data, mask);

    return err_flag;
}



pm_err_flag_type pm_wled_led_modulator_src_sel(uint32 pmic_device_index, pm_wled_led_type led_type, pm_wled_modulator_type modulator)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;

    pm_register_address_type led_src_sel;
    pm_register_data_type data = 0;
    pm_register_data_type mask = 0x3;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (led_type >= PM_WLED_INVALID)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else if ( modulator >= PM_MODULATOR_TYPE_INVALID )
    {
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    data = (pm_register_data_type)modulator;

    led_src_sel = ADDRESS_SINK(led1_src_sel);

    err_flag = pm_wled_sink_write(pmic_device_index, led_type, led_src_sel, data, mask);

    return err_flag;
}

pm_err_flag_type pm_wled_en_sync(uint32 pmic_device_index, pm_wled_led_type led_type, boolean enable)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;

    pm_register_address_type iled_sync_bit;
    pm_register_data_type mask, data;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    if(led_type >= PM_WLED_INVALID)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    iled_sync_bit = ADDRESS_SINK(iled_sync_bit);

    mask = led_type;

    if (enable)
    {
        data = 0xFF;
        //making sure the transition will be 0 -> 1 , as required for update
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, iled_sync_bit, mask, 0x00, 0);
    }
    else
    {
        data = 0x00;
    }

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, iled_sync_bit, mask, data, 0);

    return err_flag;
}


pm_err_flag_type pm_wled_set_fsw_ctrl(uint32 pmic_device_index, uint32 resource_index, uint32   fsw_khz)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;

    pm_register_data_type data;

    pm_register_address_type switching_frequency;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    
    switching_frequency = ADDRESS_CTRL(switching_frequency);
    
    if (fsw_khz > 9600)
        {
            fsw_khz = 9600;
        }
        else if (fsw_khz < 600)
        {
            fsw_khz = 300;
        }
        //FSW = 9600KHz/(data+1)
        data = (9600/fsw_khz)-1;

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, switching_frequency, 0x0F, data, 0);

    return err_flag;
}



pm_err_flag_type pm_wled_sel_ovp_thr(uint32 pmic_device_index, uint32 resource_index, pm_wled_ovp_threshold_type ovp_threshold)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data = 0;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if (ovp_threshold >= PM_WLED_OVP_THR_INVALID)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if (resource_index >= ((pm_wled_data_type *)wled_ptr)->num_of_peripherals)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    pm_register_address_type wled_ovp = ADDRESS_CTRL(wled_ovp);

    data = (pm_register_data_type)ovp_threshold;

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, wled_ovp, 0x0F, data, 0);

    return err_flag;
}


pm_err_flag_type pm_wled_sel_bst_ilimit(uint32 pmic_device_index, uint32 resource_index, pm_wled_bst_ilimit_type bst_ilimit_type)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;

  pm_register_data_type data;

  pm_register_address_type wled_ilim;

  pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

  if (wled_ptr == NULL)
  {
    return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (bst_ilimit_type >= PM_WLED_BST_ILIMIT_INVALID)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
    switch (bst_ilimit_type)
    {
    case PM_WLED_BST_ILIMIT_525mA :
      data = 0; break;
    case PM_WLED_BST_ILIMIT_700mA :
      data = 1; break;
    case PM_WLED_BST_ILIMIT_875mA :
      data = 2; break;
    case PM_WLED_BST_ILIMIT_1050mA:
      data = 3; break;
    case PM_WLED_BST_ILIMIT_1225mA:
      data = 4; break;
    case PM_WLED_BST_ILIMIT_1400mA:
      data = 5; break;
    case PM_WLED_BST_ILIMIT_1575mA:
      data = 6; break;
    case PM_WLED_BST_ILIMIT_1750mA:
      data = 7; break;
    default:
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    wled_ilim = ADDRESS_CTRL(wled_ilim);

    err_flag = pm_comm_write_byte_mask(SLAVE_ID, wled_ilim, 0x7, data, 0);
  }

  return err_flag;
}


pm_err_flag_type pm_wled_get_status(uint32 pmic_device_index, pm_wled_status_type *wled_status)
{
   pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;

   pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

   if (wled_ptr == NULL)
   {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }

   pm_register_data_type    data;

   pm_register_address_type module_enable = ADDRESS_CTRL(module_enable);
   err_flag |= pm_comm_read_byte(SLAVE_ID, module_enable, &data, 0);
   wled_status->module_enable = (data & 0x80) ? TRUE : FALSE;

   pm_register_address_type wled_ovp = ADDRESS_CTRL(wled_ovp);
   err_flag |= pm_comm_read_byte(SLAVE_ID, wled_ovp, &data, 0);
   wled_status->ovp_threshold = (pm_wled_ovp_threshold_type)(data & 0x0F);

   pm_register_address_type wled_ilim = ADDRESS_CTRL(wled_ilim);
   err_flag |= pm_comm_read_byte(SLAVE_ID, wled_ilim, &data, 0);
   wled_status->bst_ilimit_type = (pm_wled_bst_ilimit_type)(data & 0x7);

   pm_register_address_type switching_frequency = ADDRESS_CTRL(switching_frequency);
   err_flag |= pm_comm_read_byte(SLAVE_ID, switching_frequency, &data, 0);
   wled_status->fsw_ctr = 9600 / ((data & 0xF) + 1);


   pm_register_data_type modulator_data[5] = { 0 };
   pm_register_address_type modulator_reg = 0;
   pm_register_address_type i= 0;

   for (i = 0; i < 2; i++)
   {
      modulator_reg = ADDRESS_SINK(modulatora_en) + (i * 0x10);
      err_flag |= pm_comm_read_byte_array(SLAVE_ID, modulator_reg, 4, modulator_data, 0);

      wled_status->modulator[i].enabled = (modulator_data[0] & 0x80) ? TRUE : FALSE;
      wled_status->modulator[i].src = (pm_wled_modulator_src_type)(modulator_data[1] & 0x03);
      wled_status->modulator[i].width = (pm_wled_modulator_brightness_width_type)(modulator_data[2] & 0x1);
      wled_status->modulator[i].duty_cycle = modulator_data[3] << 8 | modulator_data[4];
   }

   pm_register_address_type en_current_sink =  ADDRESS_SINK(current_sink_en);
   err_flag |= pm_comm_read_byte(SLAVE_ID, en_current_sink, &data, 0);

   wled_status->led[0].enable_current_sink = (data & 0x10) ? TRUE : FALSE;
   wled_status->led[1].enable_current_sink = (data & 0x20) ? TRUE : FALSE;
   wled_status->led[2].enable_current_sink = (data & 0x40) ? TRUE : FALSE;
   wled_status->led[3].enable_current_sink = (data & 0x80) ? TRUE : FALSE;

   pm_register_address_type led_full_scale_current;
   pm_register_data_type data_led[2] = { 0 };

   for (i=0; i < 4; i++)
   {
      led_full_scale_current = ADDRESS_SINK(led1_full_scale_current) + (i * 0x10);
      err_flag |= pm_comm_read_byte_array(SLAVE_ID, led_full_scale_current, 2, data_led, 0);
      wled_status->led[i].current_milliamp  = (data_led[0] & 0x0F) * PM_LED_FULL_SCALE_CURRENT_STEP_SIZE ;
      wled_status->led[i].led_modulator_src = (pm_wled_modulator_type)(data_led[1] & 0x03);
   }

   return err_flag;
}

pm_err_flag_type pm_wled_irq_enable(uint32 pmic_device_index, pm_wled_irq_type irq, boolean enable)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type irq_reg;
    uint8 data = 1 << irq;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);
    
    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_WLED_IRQ_INVALID)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if (enable)
    {
        irq_reg = ADDRESS_CTRL(int_en_set);
    }
    else
    {
        irq_reg = ADDRESS_CTRL(int_en_clr);
    }
    err_flag = pm_comm_write_byte(SLAVE_ID, irq_reg, data, 0);
    return err_flag;
}

pm_err_flag_type pm_wled_irq_clear(uint32 pmic_device_index, pm_wled_irq_type irq)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    uint8 data = 1 << irq;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);
   
    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_WLED_IRQ_INVALID)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    pm_register_address_type int_latched_clr = ADDRESS_CTRL(int_latched_clr);

    err_flag = pm_comm_write_byte(SLAVE_ID, int_latched_clr, data, 0);
    return err_flag;
}


pm_err_flag_type pm_wled_irq_set_trigger(uint32 pmic_device_index, pm_wled_irq_type irq, pm_irq_trigger_type trigger)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    uint8 data = 1 << irq;

    pm_register_data_type set_type, polarity_high, polarity_low;
    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);
    
    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_WLED_IRQ_INVALID)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    pm_register_address_type int_set_type = ADDRESS_CTRL(int_set_type);
    pm_register_address_type int_polarity_high = ADDRESS_CTRL(int_polarity_high);
    pm_register_address_type int_polarity_low = ADDRESS_CTRL(int_polarity_low);

    switch (trigger)
    {
    case PM_IRQ_TRIGGER_ACTIVE_LOW:
        set_type = 0x00;
        polarity_high = 0x00;
        polarity_low = 0xFF;
        break;
    case PM_IRQ_TRIGGER_ACTIVE_HIGH:
        set_type = 0x00;
        polarity_high = 0xFF;
        polarity_low = 0x00;
        break;
    case PM_IRQ_TRIGGER_RISING_EDGE:
        set_type = 0xFF;
        polarity_high = 0xFF;
        polarity_low = 0x00;
        break;
    case PM_IRQ_TRIGGER_FALLING_EDGE:
        set_type = 0xFF;
        polarity_high = 0x00;
        polarity_low = 0xFF;
        break;
    case PM_IRQ_TRIGGER_DUAL_EDGE:
        set_type = 0xFF;
        polarity_high = 0xFF;
        polarity_low = 0xFF;
        break;
    default:
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    err_flag = pm_comm_write_byte_mask(SLAVE_ID,  int_set_type, data, set_type, 0);
    err_flag |= pm_comm_write_byte_mask(SLAVE_ID, int_polarity_high, data, polarity_high, 0);
    err_flag |= pm_comm_write_byte_mask(SLAVE_ID, int_polarity_low, data, polarity_low, 0);

    return err_flag;
}

pm_err_flag_type pm_wled_irq_status(uint32 pmic_device_index, pm_wled_irq_type irq, pm_irq_status_type type, boolean *status)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_data_type data;
    uint8 mask = 1 << irq;    
    pm_register_address_type int_sts;
    
    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (irq >= PM_WLED_IRQ_INVALID)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    switch (type)
    {
    case PM_IRQ_STATUS_RT:
        int_sts = ADDRESS_CTRL(int_rt_sts);
        break;
    case PM_IRQ_STATUS_LATCHED:
        int_sts = ADDRESS_CTRL(int_latched_sts);
        break;
    case PM_IRQ_STATUS_PENDING:
        int_sts = ADDRESS_CTRL(int_pending_sts);
        break;
    default:
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    err_flag = pm_comm_read_byte_mask(SLAVE_ID, int_sts, mask, &data, 0);
    *status = data ? TRUE : FALSE;

    return err_flag;
}


static pm_err_flag_type pm_wled_sink_write(uint32 pmic_device_index, pm_wled_led_type led_type, pm_register_address_type led1_reg_addr, pm_register_data_type data, pm_register_data_type mask)
{
    pm_err_flag_type      err_flag    = PM_ERR_FLAG_SUCCESS;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
      return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else if (led_type >= PM_WLED_INVALID)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    /*led4_reg_addr=led1_reg_addr+0x30; led3_reg_addr=led1_reg_addr+0x20; led2_reg_addr=led1_reg_addr+0x10;*/
    if (led_type & PM_WLED_LED1)
    {
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, led1_reg_addr, mask, data, 0);
    }

    if (led_type & PM_WLED_LED2)
    {
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, (pm_register_address_type)(led1_reg_addr + 0x10), mask, data, 0);
    }

    if (led_type & PM_WLED_LED3)
    {
        err_flag |= pm_comm_write_byte_mask(SLAVE_ID, (pm_register_address_type)(led1_reg_addr + 0x20), mask, data, 0);
    }

    if (led_type & PM_WLED_LED4)
    {
        err_flag = pm_comm_write_byte_mask(SLAVE_ID, (pm_register_address_type)(led1_reg_addr + 0x30), mask, data, 0);
    }

    return err_flag;
}

static pm_err_flag_type pm_wled_enable_detected_current_strings( uint32 pmic_device_index )
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    uint8 current_strings = 0x1;

    for (current_strings = 1; current_strings < (sizeof(current_strings_arr) / sizeof(current_strings_arr[0])); current_strings++)
    {
        if (current_strings_arr[current_strings] != PM_WLED_NONE)
        {
            err_flag |= pm_wled_enable_current_sink(pmic_device_index, current_strings_arr[current_strings]);
        }
    }

    return err_flag;

}

pm_err_flag_type pm_wled_autoselect_current_strings(uint32 pmic_device_index)
{
    pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
    pm_register_address_type feedback_control = 0x0;
    boolean ovp_status = FALSE;
    uint8 current_strings = 0;

    pm_wled_data_type *wled_ptr = pm_wled_get_data(pmic_device_index);

    if (wled_ptr == NULL)
    {
        return PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }

    if (strings_detection_complete == TRUE)
    {
        err_flag = pm_wled_enable_detected_current_strings(pmic_device_index);
        return err_flag;
    }

    feedback_control = ADDRESS_CTRL(feedback_control);

    /* 
       To enable module follow:
       i.disable OVP IRQ,
       ii.enable module
       iii.wait 10millisec
       iv.enable OVP IRQ
       */
    err_flag |= pm_wled_irq_enable(pmic_device_index, PM_WLED_IRQ_OVP_FAULT, FALSE);

    err_flag |= pm_wled_enable_module(pmic_device_index, 0, TRUE);

    pm_busy_wait(10 * 1000);

    err_flag |= pm_wled_irq_enable(pmic_device_index, PM_WLED_IRQ_OVP_FAULT, TRUE);

    /* 
      Autodetection algo:
      i.disable all current strings at the begining of each iteration
      ii. enable feedback register for each string to force output from stromg
      iii. enable a current string
      iv. wait 10millisec
      v.check for OVP status
      vi. if OVP then discard that string otherwise save string number. Repeat for all strings.
     */
    for (current_strings = 1; current_strings < (sizeof(current_strings_arr) / sizeof(current_strings_arr[0])); current_strings++)
    {
        err_flag |= pm_wled_enable_current_sink(pmic_device_index, PM_WLED_NONE);

        err_flag |= pm_comm_write_byte(SLAVE_ID, feedback_control, current_strings, 0);

        err_flag |= pm_wled_enable_current_sink(pmic_device_index, current_strings_arr[current_strings]);

        pm_busy_wait(10 * 1000);

        err_flag |= pm_wled_irq_status(pmic_device_index, PM_WLED_IRQ_OVP_FAULT, PM_IRQ_STATUS_RT, &ovp_status);

        if (ovp_status == TRUE)
        {
            current_strings_arr[current_strings] = PM_WLED_NONE;
        }
    }

    /*revert feedback control to default value 0*/
    err_flag |= pm_comm_write_byte(SLAVE_ID, feedback_control, 0, 0);

    if (err_flag == PM_ERR_FLAG_SUCCESS)
    {
        err_flag |= pm_wled_enable_current_sink(pmic_device_index, PM_WLED_NONE);

        err_flag |= pm_wled_enable_detected_current_strings(pmic_device_index);

        strings_detection_complete = (err_flag == PM_ERR_FLAG_SUCCESS) ? TRUE : FALSE;
    }

    return err_flag;
}
