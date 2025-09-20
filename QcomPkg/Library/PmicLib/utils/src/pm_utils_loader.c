/*! \file
*  
*  \brief  pm_utils_loader.c 
*  &copy; Copyright 2016-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/15/17   aab      Added pm_sbl_set_batt_id_forced_mode()
02/02/17   cs       SBL needs to do a fresh reading of batt id
08/03/16   akm      Created

===========================================================================*/

/*===========================================================================

INCLUDE FILES 

===========================================================================*/

#include "DALSys.h"
#include "adc.h"
#include "DDIAdc.h"
#include "busywait.h"
#include "boothw_target.h"

#include "pm_app_pon.h"
#include "pm_utils.h"
#include "pm_log_utils.h"



pm_err_flag_type
pm_busy_wait ( uint32 uS )
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  if ( uS > 0 )
  {  
     (void) DALSYS_BusyWait(uS);
  }
  else
  {
     err_flag = PM_ERR_FLAG_INVALID_PARAMETER;
  }

  return err_flag;
}

pm_err_flag_type 
pm_sbl_get_batt_id(pm_batt_id_pull_up_type pu_type, uint32 *batt_id_ohms)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   adc_status_type adc_status;
   adc_result_type adc_result;
   const char *channel = NULL;

   if(NULL == batt_id_ohms || pu_type >= PM_BATT_ID_PU_INVALID)
   {
       return PM_ERR_FLAG_INVALID_PARAMETER;
   }
 
   switch(pu_type)
   {
   case PM_BATT_ID_PU_30K:
       channel = ADC_INPUT_BATT_ID_OHMS_PU_30K;
       break;
   case PM_BATT_ID_PU_100K:
       channel = ADC_INPUT_BATT_ID_OHMS;
       break;
   case PM_BATT_ID_PU_400K:
       channel = ADC_INPUT_BATT_ID_OHMS_PU_400K;
       break;
   default:
       return PM_ERR_FLAG_INVALID_PARAMETER;
   }

   adc_status = adc_read(channel, &adc_result);
   if (adc_status != ADC_STATUS_SUCCESS)
   {
      pm_log_message("ERROR: adc_read()");
      return PM_ERR_FLAG_PERIPHERAL_ERR;
   }

   *batt_id_ohms = adc_result.physical;      
   
   return err_flag;
}

pm_err_flag_type
pm_get_adc_code_of_battery_id(pm_batt_id_pull_up_type type, uint32 ohms, uint32* code)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    DALResult result = DAL_SUCCESS;
    AdcResultType adcResult;
    static boolean batt_id_channel_init = FALSE;
    static DalDeviceHandle *phAdcDev = NULL;
    static AdcInputPropertiesType adcInputProps;

    if(NULL == code || type >= PM_BATT_ID_PU_INVALID)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if(!batt_id_channel_init)
    {
        /* Attach to ADC */
        result = DAL_AdcDeviceAttach(DALDEVICEID_ADC, &phAdcDev);
        if(result != DAL_SUCCESS)
        {
            pm_log_message("ERROR: DAL_AdcDeviceAttach()");
            return err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
        }

        char *channel = NULL;
		uint32 chan_size;
        switch(type)
        {
        case PM_BATT_ID_PU_30K:
            channel = ADC_INPUT_BATT_ID_OHMS_PU_30K;
            chan_size = sizeof(ADC_INPUT_BATT_ID_OHMS_PU_30K);
            break;
        case PM_BATT_ID_PU_100K:
            channel = ADC_INPUT_BATT_ID_OHMS;
            chan_size = sizeof(ADC_INPUT_BATT_ID_OHMS);
            break;
        case PM_BATT_ID_PU_400K:
            channel = ADC_INPUT_BATT_ID_OHMS_PU_400K;
            chan_size = sizeof(ADC_INPUT_BATT_ID_OHMS_PU_400K);
            break;
        default:
            return PM_ERR_FLAG_INVALID_PARAMETER;
        }

        /* Get the channel's input properties */
        result = DalAdc_GetAdcInputProperties(phAdcDev,
                                            channel,
                                            chan_size,
                                            &adcInputProps);
        if(result != DAL_SUCCESS)
        {
            pm_log_message("ERROR: DalAdc_GetAdcInputProperties()");
            return PM_ERR_FLAG_PERIPHERAL_ERR;
        }

        batt_id_channel_init = TRUE;
    }

    result = DalAdc_ProcessConversionResultInverse(phAdcDev, &adcInputProps, ohms, &adcResult);
    if(result != DAL_SUCCESS)
    {
        pm_log_message("ERROR: DalAdc_Read()");
        return PM_ERR_FLAG_PERIPHERAL_ERR;
    }

    *code = adcResult.nCode;

    return err_flag;
}

pm_err_flag_type
pm_get_adc_code_of_battery_temp(uint32 celsius, uint32* code)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    DALResult result = DAL_SUCCESS;
    AdcResultType adcResult;
    static boolean inited = FALSE;
    static DalDeviceHandle *phAdcDev = NULL;
    static AdcInputPropertiesType adcInputProps;

    if(NULL == code)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    if(!inited)
    {
        /* Attach to ADC */
        result = DAL_AdcDeviceAttach(DALDEVICEID_ADC, &phAdcDev);
        if(result != DAL_SUCCESS)
        {
            pm_log_message("ERROR: DAL_AdcDeviceAttach()");
            return err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
        }

        char *channel = ADC_INPUT_BATT_THERM;
		uint32 chan_size = sizeof(ADC_INPUT_BATT_THERM);

        /* Get the channel's input properties */
        result = DalAdc_GetAdcInputProperties(phAdcDev,
                                            channel,
                                            chan_size,
                                            &adcInputProps);
        if(result != DAL_SUCCESS)
        {
            pm_log_message("ERROR: DalAdc_GetAdcInputProperties()");
            return PM_ERR_FLAG_PERIPHERAL_ERR;
        }

        inited = TRUE;
    }

    result = DalAdc_ProcessConversionResultInverse(phAdcDev, &adcInputProps, celsius, &adcResult);
    if(result != DAL_SUCCESS)
    {
        pm_log_message("ERROR: DalAdc_Read()");
        return PM_ERR_FLAG_PERIPHERAL_ERR;
    }

    *code = adcResult.nCode;

    return err_flag;
}



pm_err_flag_type 
pm_device_shutdown(void)
{
  boot_hw_powerdown();   
  
  /*should not return*/
  return PM_ERR_FLAG_SUCCESS;
}

  
pm_err_flag_type  
pm_sbl_daladc_fg_set_temp_threshold(uint8 pmic_index,  AdcFGThresholdType adc_fg_threshold, int temp_value) 
{ 
   pm_err_flag_type err_flag         = PM_ERR_FLAG_SUCCESS; 

   //TODO: Need to revisit this fn
   return PM_ERR_FLAG_SUCCESS;

/*
   DALResult result                  = DAL_SUCCESS; 
   static DALBOOL bInit              = FALSE; 
   static DalDeviceHandle *phAdcDev  = NULL; 
  
   if (!bInit) 
   { 
      // Attach to ADC 
      result = DAL_AdcDeviceAttach(DALDEVICEID_ADC, &phAdcDev); 
      if (result != DAL_SUCCESS) 
      { 
         pm_log_message("ERROR: DAL_AdcDeviceAttach()"); 
         return err_flag = PM_ERR_FLAG_PERIPHERAL_ERR; 
      } 
  
      bInit = TRUE; 
   } 
  
   //Configure charger / skin temp threshold 
   result = DalAdc_FGSetThreshold(phAdcDev, adc_fg_threshold, temp_value); 
   if (result != DAL_SUCCESS) 
   { 
      pm_log_message("ERROR: DalAdc_FGSetThreshold()"); 
      err_flag = PM_ERR_FLAG_PERIPHERAL_ERR; 
   } 
  */
   return err_flag; 
}




pm_err_flag_type 
pm_sbl_set_batt_id_forced_mode(boolean enable)
{
   pm_err_flag_type err_flag         = PM_ERR_FLAG_SUCCESS;

   return PM_ERR_FLAG_SUCCESS;

   /*
   DALResult result                  = DAL_SUCCESS;
   static DALBOOL bInit              = FALSE;
   static DalDeviceHandle *phAdcDev  = NULL;

   if (!bInit)
   {
      // Attach to ADC
      result = DAL_AdcDeviceAttach(DALDEVICEID_ADC, &phAdcDev);
      if (result != DAL_SUCCESS)
      {
         pm_log_message("ERROR: DAL_AdcDeviceAttach()");
         return err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
      }
	  bInit = TRUE;
   }
   
   // Configure bat_id forced mode 
   result = DalAdc_FGSetBattIdForced(phAdcDev, enable);
   if (result != DAL_SUCCESS)
   {
      pm_log_message("ERROR: DalAdc_FGSetBattIdForced()");
      return err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
   }
   */
 
   return err_flag;
}
