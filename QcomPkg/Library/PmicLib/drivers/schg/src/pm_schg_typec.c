/*! \file
*  
*  \brief  pm_schg_typec.c driver implementation.
*  \details charger driver implementation.
*  &copy;
*  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
*  Qualcomm Technologies Proprietary and Confidential.
*/

/*===========================================================================

EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---    ------------------------------------------------------------- 
04/18/18    dc          Update dfp mask for not consider dfp in case of RA-Open
05/14/18    ra          Support for Debug Accessory Mode
04/06/18    dc          Fix source/dfp mode detection
02/20/18    richaa      Removing obsolete code
10/18/17    richaa      Initial revision
=============================================================================== */

/*=============================================================================

					INCLUDE FILES

===============================================================================*/

#include "pm_schg_driver.h"
#include "pm_schg_typec.h"
#include "pm_resource_manager.h"

/*===========================================================================
                       MACRO DECLARATIONS
===========================================================================*/

#define ADDRESS(x) (schg_ptr->schg_register->typec_register->base_address + schg_ptr->schg_register->typec_register->x)
#define SLAVE_ID   (schg_ptr->comm_ptr->slave_id)

/*===========================================================================

FUNCTION DEFINITIONS 

===========================================================================*/
pm_err_flag_type pm_schg_usb_get_typec_status(uint32 pmic_device, pm_schg_usb_typec_status_type *typec_status)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
  uint8 typec_port_status[6] = {0};
  pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);

  if (NULL == schg_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else if (typec_status == NULL)
  {
    err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }
  else
  {
	  pm_register_address_type address = ADDRESS(typec_snk_status);
    err_flag = pm_comm_read_byte_array(SLAVE_ID, address, 6, (uint8 *)&typec_port_status, 0);

    if(typec_port_status[0] & 0x02)
    {
      typec_status->dfp_curr_adv = PM_SCHG_USB_TYPEC_CURR_ADV_3A;
    }
    else if(typec_port_status[0] & 0x04)
    {
      typec_status->dfp_curr_adv = PM_SCHG_USB_TYPEC_CURR_ADV_1P5A;
    }
    else if(typec_port_status[0] & 0x08)
    {
      typec_status->dfp_curr_adv = PM_SCHG_USB_TYPEC_CURR_ADV_STD;
    }
    else
    {
      typec_status->dfp_curr_adv = PM_SCHG_USB_TYPEC_CURR_ADV_INVALID;
    }

    if(typec_port_status[2] & 0x08)
    {//DFP mode - RD/Open on CC1/CC2
      typec_status->ufp_conn_type = PM_SCHG_TYPEC_UFP_ATTACHED;
    }
    else if(typec_port_status[2] & 0x04)
    {//DFP mode - RD/RA on CC1/CC2
      typec_status->ufp_conn_type = PM_SCHG_TYPEC_UFP_ATTACHED_POWER_CABLE;
    }
    else if(typec_port_status[2] & 0x10)
    {//	DFP mode - RD/RD on CC1/CC2
      typec_status->ufp_conn_type = PM_SCHG_TYPEC_UFP_DEBUG_ACCESSORY;
    }
    else if(typec_port_status[2] & 0x01)
    {//DFP mode - RA/RA on CC1/CC2
      typec_status->ufp_conn_type = PM_SCHG_TYPEC_UFP_AUDIO_ADAPTER;
    }
    else
    {
      typec_status->ufp_conn_type = PM_SCHG_TYPEC_UFP_INVALID;
    }

    typec_status->vbus_sts          = (typec_port_status[5] & 0x20) ? TRUE : FALSE;
    typec_status->vbus_err_sts      = (typec_port_status[5] & 0x10) ? TRUE : FALSE;
    typec_status->debounce_done_sts = (typec_port_status[5] & 0x08) ? TRUE : FALSE;
    typec_status->vconn_oc_sts      = (typec_port_status[5] & 0x04) ? TRUE : FALSE;
    typec_status->ccout_out_hiz_sts = (typec_port_status[5] & 0x01) ? TRUE : FALSE;
    typec_status->cc_out_sts        = (typec_port_status[5] & 0x02) ? PM_TYPEC_PLUG_FLIP : PM_TYPEC_PLUG_UNFLIP;
  }

  return err_flag;
}


pm_err_flag_type pm_schg_usb_typec_get_port_role(uint32 pmic_device, pm_schg_usb_typec_port_role_type *port_role)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
  uint8 typec_port_status[3] = {0};
  uint8 current_adv_mask = 0x0E;
  //uint8 ufp_types_open_open_mask = 0x01;
  uint8 dfp_mask = 0x0D;

  pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);

  if(NULL == port_role)
  {
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  *port_role = PM_SCHG_USB_TYPEC_ROLE_INVALID;

  if (NULL == schg_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
	  pm_register_address_type address = ADDRESS(typec_snk_status);

    err_flag = pm_comm_read_byte_array(SLAVE_ID, address, sizeof(typec_port_status), typec_port_status, 0);
    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
      return err_flag;
    }

    /*Workaround for false factory cable detection on PMI 8998 */
    /*if(ufp_types_open_open_mask & typec_port_status[0])
    {
      *port_role = PM_SCHG_USB_TYPEC_ROLE_UFP;
      return err_flag;
    }*/

    if(current_adv_mask & typec_port_status[0])
    {
      *port_role = PM_SCHG_USB_TYPEC_ROLE_UFP;
    }
    else if(dfp_mask & typec_port_status[2])
    {
      *port_role = PM_SCHG_USB_TYPEC_ROLE_DFP;
    }
    else
    {
      *port_role = PM_SCHG_USB_TYPEC_ROLE_NONE;
    }
  }

  return err_flag;
}

pm_err_flag_type pm_schg_usb_typec_set_port_role(uint32 pmic_device, pm_schg_usb_typec_port_role_type port_role)
{
  pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_register_data_type data = 0x00;
  uint8 mask = 0x06;
  pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);

  if (NULL == schg_ptr)
  {
    err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
  }
  else
  {
	  pm_register_address_type type_c_mode_cfg = ADDRESS(typec_mode_cfg);
	  if (port_role >= PM_SCHG_USB_TYPEC_ROLE_INVALID || port_role <= PM_SCHG_USB_TYPEC_ROLE_NONE)
    {
      return PM_ERR_FLAG_INVALID_PARAMETER;
    }
    else 
    {
		if (port_role == PM_SCHG_USB_TYPEC_ROLE_DFP)
		{
			data = port_role << 2;
		}
    }
    
	  err_flag = pm_comm_write_byte_mask(SLAVE_ID, type_c_mode_cfg, mask, data, 0);
  }
  return err_flag;
}

pm_err_flag_type pm_schg_usb_typec_vconn_enable(uint32 pmic_device, pm_schg_usb_typec_vconn_enable_type enable_type)
{
   pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
   pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);
   pm_register_data_type data = 0x00;
   uint8 mask = 0x03;

   if (NULL == schg_ptr)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else if (enable_type >= PM_SCHG_USB_TYPEC_VCONN_INVALID)
   {
      err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
   }
    else
   {
		pm_register_address_type type_c_intrpt_enb_software_ctrl = ADDRESS(typec_vconn_control);
      data = enable_type;
	  err_flag = pm_comm_write_byte_mask(SLAVE_ID, type_c_intrpt_enb_software_ctrl, mask, data, 0);
   }

   return err_flag;
}

pm_err_flag_type pm_schg_usb_typec_disable(uint32 pmic_device, boolean disable)
{
   pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
   pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);
   uint8 mask = 0x01;

   if (NULL == schg_ptr)
   {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
   }
   else
   {
	   pm_register_address_type type_c_intrpt_enb_software_ctrl = ADDRESS(typec_mode_cfg);
      err_flag = pm_comm_write_byte_mask(SLAVE_ID, type_c_intrpt_enb_software_ctrl, mask, disable, 0);
   }

   return err_flag;
}

pm_err_flag_type pm_schg_usb_typec_enable_try_sink_mode(uint32 pmic_device, boolean enable)
{
   pm_err_flag_type    err_flag    = PM_ERR_FLAG_SUCCESS;
   pm_schg_data_type *schg_ptr  = pm_schg_get_data(pmic_device);
   pm_register_data_type data = enable << 4;
   pm_register_data_type mask = 0x10;

    if (NULL == schg_ptr)
    {
      err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
    }
    else
    {
		pm_register_address_type type_c_cfg_3 = ADDRESS(typec_mode_cfg);

      err_flag = pm_comm_write_byte_mask(SLAVE_ID, type_c_cfg_3, mask, data, 0);
    }

   return err_flag;
}

pm_err_flag_type pm_schg_typec_en_dbg_access_snk(uint32 pmic_device, boolean EnDbgAccessSnk)
{
	pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
	pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);
	pm_register_data_type data = EnDbgAccessSnk;
	pm_register_data_type mask = 0x01;

	if (NULL == schg_ptr)
	{
		err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else
	{
		pm_register_address_type type_c_snk_cfg = ADDRESS(typec_debug_access_snk_cfg);
		err_flag = pm_comm_write_byte_mask(SLAVE_ID, type_c_snk_cfg, mask, data, 0);
	}

	return err_flag;
}

pm_err_flag_type pm_schg_typec_en_fmb(uint32 pmic_device, boolean EnFmb)
{
	pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
	pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);
	pm_register_data_type data = EnFmb << 4;
	pm_register_data_type mask = 0x10;

	if (NULL == schg_ptr)
	{
		err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else
	{
		pm_register_address_type type_c_snk_cfg = ADDRESS(typec_debug_access_snk_cfg);
		err_flag = pm_comm_write_byte_mask(SLAVE_ID, type_c_snk_cfg, mask, data, 0);
	}

	return err_flag;

}

pm_err_flag_type pm_schg_typec_get_dam_connect_sts(uint32 pmic_device, boolean *ConnectSts)
{
	pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
	pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);
	pm_register_data_type data = 0;
	uint8 mask = 0x3F;

	if (NULL == schg_ptr)
	{
		err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else
	{
		pm_register_address_type type_c_access_status = ADDRESS(typec_debug_access_status);
		err_flag = pm_comm_read_byte_mask(SLAVE_ID, type_c_access_status, mask, &data, 0);
		if (ConnectSts != NULL)
		{
			*ConnectSts = data ? 1 : 0;
		}
	}

	return err_flag;

}

pm_err_flag_type pm_schg_typec_set_dam_icl(uint32 pmic_device, pm_schg_usb_typec_dam_icl_sel icl_sel)
{
	pm_err_flag_type    err_flag = PM_ERR_FLAG_SUCCESS;
	pm_schg_data_type *schg_ptr = pm_schg_get_data(pmic_device);
	pm_register_data_type data = icl_sel << 2;
	pm_register_data_type mask = 0x04;

	if (NULL == schg_ptr)
	{
		err_flag = PM_ERR_FLAG_FEATURE_NOT_SUPPORTED;
	}
	else
	{
		pm_register_address_type type_c_snk_cfg = ADDRESS(typec_debug_access_snk_cfg);
		err_flag = pm_comm_write_byte_mask(SLAVE_ID, type_c_snk_cfg, mask, data, 0);
	}

	return err_flag;

}
