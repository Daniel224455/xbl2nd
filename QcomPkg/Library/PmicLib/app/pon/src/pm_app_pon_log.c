/*! \file pm_app_pon_log.c
*  
*  \brief Implementation file for PON APP level logger APIs.
*    
*  &copy; Copyright 2016-2017 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/*===================================================================
EDIT HISTORY FOR MODULE
 This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Date  2018/07/02 $ 
$Change  16554216 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/10/17   aab     Updated PON logging Text
09/13/16   aab     Updated PON logging
08/03/16   akm     Creation
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_pon.h"
#include "pm_device.h"
#include "pm_app_pon.h"
#include "pmio_pon.h"
#include "pm_err_flags.h"
#include "device_info.h"
#include "hw_module_type.h"
#include "pm_config_target.h"
#include "pm_sbl_boot_target.h"
#include "pm_pon_target.h"
#include "pm_target_information.h"
#include "CoreVerify.h"
#include "pm_log_utils.h"
#include <string.h> 

/*===========================================================================

                     Global Variables

===========================================================================*/
typedef struct
{
  uint8 pon_reason1;
  uint8 pon_reason2;
  uint8 poff_reason1;
  uint8 poff_reason2;
  uint8 warm_reset_reason1;
  uint8 warm_reset_reason2;
  uint8 soft_reset_reason1;
  uint8 soft_reset_reason2;
}pm_reason_status_type;

typedef struct
{
  uint8 pon_reason1;
  uint8 reserved1;
  uint8 warm_reset_reason1;
  uint8 reserved2;
  uint8 on_reason;
  uint8 poff_reason1;
  uint8 reserved3;
  uint8 off_reason;
}pm_pon_gen2_reason_status_type;

typedef struct
{
  uint8 fault_reason1;
  uint8 fault_reason2;
  uint8 s3_reset_reason;
  uint8 soft_reset_reason1;
}pm_pon_gen2_fault_status_type;

#define PON_REASON_SIZE 16

/*===========================================================================

                   Function Prototypes

===========================================================================*/

static pm_err_flag_type pm_app_log_verbose_pon_reason_gen2(void);
static uint8 find_first_non_zero_bit(uint8 reason1, uint8 reason2);

/*===========================================================================

                   API Implementation

===========================================================================*/

pm_err_flag_type
pm_log_pon_reasons(void)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  pm_model_type pmic_model = PMIC_IS_INVALID;
  uint8 pmic_index = 0;
  uint8 sid_index = 0;
  uint32 pon_peripheral_index = 8;
  uint64 pon_reasons = 0x00;
  uint64 fault_reasons = 0x00;
  peripheral_info_type pon_peripheral_info;

  memset(&pon_peripheral_info, 0, sizeof(peripheral_info_type));
  pon_peripheral_info.base_address = pon_peripheral_index * 0x0100;

  //Log PON reasons for all PMIC
  for (pmic_index = 0; pmic_index < PM_MAX_NUM_PMICS; pmic_index++)
  {
    pmic_model = pm_get_pmic_model(pmic_index);
    if((pmic_model != PMIC_IS_INVALID) && (pmic_model != PMIC_IS_UNKNOWN))
    {
      pon_reasons = 0x00;
      err_flag |= pm_pon_get_all_pon_reasons(pmic_index, &pon_reasons);
      if(err_flag != PM_ERR_FLAG_SUCCESS)
      {
        return err_flag;
      }

      //Get PON perepheral type and perepheral sub type for each valid PMIC on target
      sid_index = pmic_index * 2;
      pm_get_peripheral_info(sid_index, &pon_peripheral_info);

      //PON GEN2
      if(pon_peripheral_info.peripheral_subtype >= PM_HW_MODULE_PON_LV_PON_GEN2_PRIMARY)
      {
        fault_reasons = 0x00;
        err_flag |= pm_pon_get_fault_reasons(pmic_index, &fault_reasons);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
          return err_flag;
        }
        pm_log_message("PM %d=0x%llx:0x%llx ", pmic_index, (uint64)pon_reasons, (uint64)fault_reasons);
      }
    }
  }

  if(pon_peripheral_info.peripheral_subtype >= PM_HW_MODULE_PON_LV_PON_GEN2_PRIMARY)
  { //PON GEN2
    err_flag |= pm_app_log_verbose_pon_reason_gen2();
  }

  return err_flag;
}

pm_err_flag_type 
pm_app_log_verbose_pon_reason_gen2(void)
{
  uint8 pon_on_bit_num = 0;
  uint8 pon_off_bit_num = 0;
  uint8 other_off_bit_num = 0;
  uint8 fault_bit_num = 0;
  uint8 s3_reset_bit_num = 0;
  uint8 warm_reset_bit_num = 0;
  uint64 pon_reasons = 0x00;
  uint64 fault_reason = 0x00;
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  pm_pon_gen2_reason_status_type *reasons_ptr = 0;
  pm_pon_gen2_fault_status_type *fault_reasons_ptr = 0;
  uint8 pmic_index = PMIC_A;
  char power_on_str[] = "POWER ON by";
  char power_off_str[] = "POWER OFF by";
  char *pon_off_reasons[PON_REASON_SIZE] = {"KPDPWR",
                                            "RESIN",
                                            "KPDPWR_AND_RESIN",
                                            "GP2","GP1",
                                            "PMIC_WD",
                                            "PS_HOLD",
                                            "SOFT"};
  char *warm_reset_reasons[PON_REASON_SIZE] = {"KPDPWR",
                                               "RESIN",
                                               "KPDPWR_AND_RESIN",
                                               "GP2","GP1",
                                               "PMIC WD",
                                               "PS_HOLD",
                                               "SOFT"};
  char *pon_on_reasons[PON_REASON_SIZE] = {"KPDPWR",
                                           "CBLPWR",
                                           "PON1",
                                           "USB_CHG",
                                           "DC_CHG",
                                           "RTC",
                                           "SMPL", 
                                           "HARD_RESET"};
  char *fault_reasons[PON_REASON_SIZE] = {"AVDD_RB",
                                          "UVLO",
                                          "OVLO",
                                          "MBG_FAULT",
                                          "GP_FAULT3",
                                          "GP_FAULT2",
                                          "GP_FAULT1",
                                          "GP_FAULT0",
                                          "OTST3",
                                          "RESTART_PON",
                                          "PBS_NACK",
                                          "PBS_WD_TO",
                                          "FAULT_N"};
  char *s3_reset_reasons[PON_REASON_SIZE] = {"KPDPWR_ANDOR_RESIN",
                                             "PBS_NACK",
                                             "PBS_WATCHDOG_TO",
                                             "FAULT_N"};
  char *other_off_reasons[PON_REASON_SIZE] = {"IMM_XVDD_SD",
                                              "RAW_DVDD_SHD",
                                              "RAW_XVDD_SHD"};

  err_flag |= pm_pon_get_all_pon_reasons(pmic_index, &pon_reasons);
  reasons_ptr = (pm_pon_gen2_reason_status_type*)(&pon_reasons);

  err_flag |= pm_pon_get_fault_reasons(pmic_index, &fault_reason);
  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    return err_flag;
  }
  fault_reasons_ptr = (pm_pon_gen2_fault_status_type*) (&fault_reason);

  //ON Reason
  if((uint8)reasons_ptr->on_reason & PMIO_PON_GEN2_ON_REASON_PON_SEQ_BMSK)
  {
    if((uint8)reasons_ptr->pon_reason1 & PMIO_PON_PON_REASON1_HARD_RESET_BMSK)
    {
      pon_on_bit_num = find_first_non_zero_bit((uint8)reasons_ptr->poff_reason1, 0);
      if(pon_on_bit_num != -1)
      {
        pm_log_message("HARD_RESET by %s", pon_off_reasons[pon_on_bit_num]);
      }
    }
    else
    {
      pon_on_bit_num = find_first_non_zero_bit((uint8)reasons_ptr->pon_reason1, 0);

      if((uint8)reasons_ptr->off_reason & PMIO_PON_GEN2_OFF_REASON_POFF_SEQ_BMSK)
      {
        pon_off_bit_num = find_first_non_zero_bit((uint8)reasons_ptr->poff_reason1, 0);
        if((pon_off_bit_num != -1) && (pon_on_bit_num != -1))
        {
          pm_log_message("%s %s, %s %s", power_on_str, pon_on_reasons[pon_on_bit_num], power_off_str, pon_off_reasons[pon_off_bit_num]);
        }
      }
      else if((uint8)reasons_ptr->off_reason & PMIO_PON_GEN2_OFF_REASON_FAULT_SEQ_BMSK)
      {
        if((PMIO_PON_GEN2_FAULT_REASON2_FAULT_N_BMSK & fault_reasons_ptr->fault_reason2) != 0)
        {
          // FAULT_REASON2
          // For the scenario of FAULT triggerred from non-primary PMIC, iterate the following PMICs to find out the fault reason
          fault_bit_num = 0;
          for (pmic_index = 1; pmic_index < PM_MAX_NUM_PMICS; pmic_index++)
          {
            fault_reason = 0x00;
            if(pm_is_pmic_present(pmic_index) == FALSE)
            {
              continue;
            }
            err_flag |= pm_pon_get_fault_reasons(pmic_index, &fault_reason);
            if(err_flag != PM_ERR_FLAG_SUCCESS)
            {
              return err_flag;
            }

            fault_reasons_ptr = (pm_pon_gen2_fault_status_type*) (&fault_reason);
            if((PMIO_PON_GEN2_FAULT_REASON2_FAULT_N_BMSK & fault_reasons_ptr->fault_reason2) != 0)
            {
              continue;
            }
            else
            {
              fault_bit_num = find_first_non_zero_bit((uint8)fault_reasons_ptr->fault_reason1, (uint8)fault_reasons_ptr->fault_reason2);
              if((fault_bit_num != -1) && (pon_on_bit_num != -1))
              {
                pm_log_message("%s %s, POWER OFF due to FAULT %s of PM %d", power_on_str, pon_on_reasons[pon_on_bit_num], fault_reasons[fault_bit_num], pmic_index);
                break;
              }
            }
          }
        }
        else
        {
          fault_bit_num = find_first_non_zero_bit((uint8)fault_reasons_ptr->fault_reason1, (uint8)fault_reasons_ptr->fault_reason2);
          if((fault_bit_num != -1) && (pon_on_bit_num != -1))
          {
            pm_log_message("%s %s, POWER OFF due to FAULT %s", power_on_str, pon_on_reasons[pon_on_bit_num], fault_reasons[fault_bit_num]);
          }
        }
      }
      else if((uint8)reasons_ptr->off_reason & PMIO_PON_GEN2_OFF_REASON_S3_RESET_BMSK)
      {
        s3_reset_bit_num = find_first_non_zero_bit((uint8)fault_reasons_ptr->s3_reset_reason, 0);
        if((s3_reset_bit_num != -1) && (pon_on_bit_num != -1))
        {
          pm_log_message("%s %s, %s S3 reset %s", power_on_str, pon_on_reasons[pon_on_bit_num], power_off_str, s3_reset_reasons[s3_reset_bit_num]);
        }
      }
      else if((uint8)fault_reasons_ptr->soft_reset_reason1 != 0x00)
      {
        pm_log_message("SOFT_RESET_REASON1:SOFT");
      }
      else
      {
        if((uint8)reasons_ptr->off_reason & PMIO_PON_GEN2_OFF_REASON_IMM_XVDD_SD_BMSK)
        {
          other_off_bit_num = 0;
        }
        else if((uint8)reasons_ptr->off_reason & PMIO_PON_GEN2_OFF_REASON_RAW_DVDD_SHD_BMSK)
        {
          other_off_bit_num = 1;
        }
        else if((uint8)reasons_ptr->off_reason & PMIO_PON_GEN2_OFF_REASON_RAW_XVDD_SHD_BMSK)
        {
          other_off_bit_num = 2;
        }
        if((other_off_bit_num != -1) && (pon_on_bit_num != -1))
        {
          pm_log_message("%s %s,%s %s", power_on_str, pon_on_reasons[pon_on_bit_num], power_off_str, other_off_reasons[other_off_bit_num]);
        }
      }
    }
  }
  else
  {

    warm_reset_bit_num = find_first_non_zero_bit((uint8)reasons_ptr->warm_reset_reason1, 0);
    if(warm_reset_bit_num != -1)
    {
      pm_log_message("WRM_reset by %s", warm_reset_reasons[warm_reset_bit_num]);
    }
  }

  return err_flag;
}

uint8 find_first_non_zero_bit(uint8 reason1, uint8 reason2)
{
  uint16 i = 0x8000;
  uint8 pon_on_bit_num = 0;
  uint16 reasons = ((reason1 << 8) | reason2);

  for (pon_on_bit_num = 0; pon_on_bit_num < 16; pon_on_bit_num++)
  {
    if((reasons & i) != 0)
    {
      break;
    }
    i = (i >> 1);
  }

  if(16 == pon_on_bit_num)
  {
    return -1;
  }

  return pon_on_bit_num;
}
