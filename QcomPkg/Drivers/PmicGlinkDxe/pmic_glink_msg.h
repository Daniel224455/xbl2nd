#ifndef PMIC_GLINK_MSG_H
#define PMIC_GLINK_MSG_H

/*===========================================================================
* Copyright (c) 2018 Qualcomm Technologies Incorporated. All Rights Reserved.
* QUALCOMM Confidential and Proprietary
*
=============================================================================
EDIT HISTORY

when           who     what, where, why
--------       ---     -----------------------------------------------------------
10/21/2018     ivy     Updated pmicglink to polling based and support fetching usb info from charger FW
03/07/2018     cs      added ucsi interface support.
=============================================================================*/



#ifdef __cplusplus
extern "C" {
#endif



/** ============================================================================
 constant definitions
============================================================================ */
#define BATT_MNGR_POWER_ON_LINE 0x00000001
#define BATT_MNGR_DISCHARGING 0x00000002
#define BATT_MNGR_CHARGING 0x00000004

/**  all power state used in BATT_MNGR_GET_CHARGER_STATUS_RESP */
#define BATT_MNGR_CRITICAL1 0x00000008
#define BATT_MNGR_TEST_REQ_FLASH 0x00000000
#define BATT_MNGR_TEST_REQ_SOH 0x00000001
#define BATT_MNGR_TEST_REQ_GET_SRAM_DUMP 0x00000002

/**  request type for BATT_MNGR_GET_TEST_INFO */
#define BATT_MNGR_TEST_REQ_SET_SRAM_DUMP 0x00000003

/** ============================================================================ */
#define BATT_MNGR_BATT_CHEMISTRY_MAX_LEN 4
#define BATT_MNGR_MAX_STRING_LEN 128

/** ============================================================================ */
#define BATT_MNGR_MAX_GENERIC_MSG_LEN 8

/** ============================================================================ */
#define BATT_MNGR_MAX_TEST_INPUT_LEN 256
#define BATT_MNGR_MAX_TEST_OUTPUT_LEN 256

/** ============================================================================ */
#define UCSI_BUFFER_SIZE           48
#define USBC_WRITE_BUFFER_SIZE     8
#define USBC_READ_BUFFER_SIZE      32

/**
@}
*/

/** @addtogroup PMIC GLINK MSG OWNER DEFS
@{
*/
#define PMIC_GLINK_MSG_OWNER_CHARGER     32778
#define	PMIC_GLINK_MSG_OWNER_USB_TYPE_C  32779


/** @addtogroup pmicGlink_msg_type_enums
@{
*/
#define PMIC_GLINK_MSG_TYPE_REQ_RESP      1
#define	PMIC_GLINK_MSG_TYPE_NOTIFICATION  2
/**
@}
*/


/*opcode definition maps to battmngr API calls*/
#define BATT_MNGR_GET_BATT_ID_REQ                   0x0000
#define BATT_MNGR_GET_CHARGER_STATUS_REQ            0x0001
#define BATT_MNGR_CONTROL_CHARGING_REQ              0x0002
#define BATT_MNGR_SET_OPERATIONAL_MODE_REQ          0x0003
#define BATT_MNGR_SET_NOTIFICATION_CRITERIA_REQ     0x0004
#define BATT_MNGR_DISABLE_NOTIFICATION_REQ          0x0005
#define BATT_MNGR_SET_CHARGE_RATE_REQ               0x0006
#define BATT_MNGR_NOTIFY_IND                        0x0007
#define BATT_MNGR_GET_SBS_REQ                       0x0008
#define BATT_MNGR_GET_BATTERY_INFORMATION_REQ       0x0009
#define BATT_MNGR_SET_AVERAGING_INTERVAL_REQ        0x000A 
#define BATT_MNGR_SET_SAMPLING_REQ                  0x000B 
#define BATT_MNGR_GET_DISCHARGE_TIME_REQ            0x000C 
#define BATT_MNGR_GET_CHARGE_TIME_REQ               0x000D 
#define BATT_MNGR_GET_BMD_REQ                       0x000E 
#define BATT_MNGR_SET_BMC_REQ                       0x000F 
#define BATT_MNGR_GET_GENERIC_INFO_REQ              0x0010 
#define BATT_MNGR_GET_TEST_INFO_REQ                 0x0020
#define BATT_MNGR_NUM_RSP_TYPE                      0x0021                            

/*opcode definition maps to UCSI and USBC calls*/
#define UCSI_USBC_NOTIFY_RECEIVER_UCSI         0x0
#define UCSI_USBC_NOTIFY_RECEIVER_USBC         0x1

/*opcode definition maps to UCSI calls*/
#define UCSI_READ_BUFFER_REQ                   0x0011
#define UCSI_WRITE_BUFFER_REQ                  0x0012
#define UCSI_USBC_NOTIFY_IND                   0x0013

/*opcode definition maps to USBC calls*/
#define USBC_CMD_READ_REQ                      0x0014
#define USBC_CMD_WRITE_REQ                     0x0015


/** @addtogroup PMIC_GLINK_HEADER_TYPE
@{
*/
typedef struct
{
  uint32 msg_owner;  //PMIC_GLINK_MSGTYPE_USB_TYPE_C, PMIC_GLINK_MSGTYPE_CHARGER
  uint32 msg_type;   //reqest, response, notification
  uint32 opcode;    //opcode will be associated with each request/response or notification
}PMIC_GLINK_HEADER_TYPE;

/**
@}
*/

/**
@}
*/
/** @addtogroup BATT_MNGR_SVC_qmi_enums
    @{
  */
typedef enum {
  BATT_MNGR_CHARGING_SOURCE_REQ_TYPE_E_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  BATT_MNGR_CHARGING_SOURCE_TYPE_AC_V01 = 1, /**<  AC charger */
  BATT_MNGR_CHARGING_SOURCE_TYPE_USB_V01 = 2, /**<  usb charger */
  BATT_MNGR_CHARGING_SOURCE_TYPE_WIRELESS_V01 = 3, /**<  wireless charger  */
  BATT_MNGR_CHARGING_SOURCE_TYPE_MAX_V01 = 4, 
  BATT_MNGR_CHARGING_SOURCE_REQ_TYPE_E_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}batt_mngr_charging_source_req_type_e;
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_enums
    @{
  */
typedef enum {
  BATT_MNGR_CHG_CMD_REQ_TYPE_E_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  BATTERY_CRITICAL_BIAS_V01 = 0, /**<  command for set reserved capacity for 0% */
  BATTERY_CHARGE_V01 = 1, /**<  command for charge */
  BATTERY_DISCHARGE_V01 = 2, /**<  command for discharge  */
  BATTERY_CHARGE_MAX_CURRENT_V01 = 3, /**<  command for set max charge current  */
  BATTERY_CHARGE_PROPRIETARY_CHARGER_MAX_CURRENT_V01 = 4, /**<  command for set max charge current for proprietary charger  */
  BATTERY_CHARGE_GENERIC_V01 = 5, /**<  generic charge command  */
  BATT_MNGR_CHG_CMD_REQ_TYPE_E_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}batt_mngr_chg_cmd_req_type_e;
/**
    @}
  */

///** @addtogroup BATT_MNGR_SVC_qmi_messages
//    @{
//  */
///** Request Message; Get the battery tag. */
//typedef struct {
//  /* This element is a placeholder to prevent the declaration of
//     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
//  char __placeholder;
//}batt_mngr_get_batt_id_req_msg;
//
//  /* Message */
///**
//    @}
//  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; Get the battery tag. */
typedef struct {
  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */
  /* Mandatory */
  uint32 battery_id;
  /**<   response message for battery ID */
}batt_mngr_get_batt_id_resp_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; Get the charger status. maps to _BST */
typedef struct {
  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */
  /* Mandatory */
  uint32 battery_id;
  /**<   battery id for which the status is to be retrieved */
}batt_mngr_get_charger_status_req_msg;  /* Message */

/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; Get the charger status. maps to _BST */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_state;
  /**<   battery state bit 0 = 1 battery discharging, bit 1 = 1 battery is charging, bit 2 = 1 battery critical low.*/

  /* Mandatory */
  uint32 capacity;
  /**<   battery capacity in mwh(mili watts hour) or mAh specified in power unit*/

  /* Mandatory */
  int32 rate;
  /**<   battery charging rate positive charging, negative discharging*/

  /* Mandatory */
  uint32 battery_voltage;
  /**<   battery voltage in mv
 mini port specific fields*/

  /* Mandatory */
  uint32 power_state;
  /**<   bttery charging power state, bit 0 = 1 power online, bit 1 = 1 discharging, bit 2 = 1 charging, bit 3 = 1 battery critical*/

  /* Mandatory */
  batt_mngr_charging_source_req_type_e charging_source;
  /**<   battery charging source*/

  /* Mandatory */
  uint32 battery_temperature;
  /**<   battery temperature in 10th of Kelvin*/
}batt_mngr_get_charger_status_resp_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; Set charging parameter */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery id to control charging */

  /* Mandatory */
  batt_mngr_chg_cmd_req_type_e battery_command;
  /**<   command type */

  /* Mandatory */
  uint32 battery_critical_bias_mv;
  /**<   reserved battery capacity for 0% */

  /* Mandatory */
  uint32 battery_max_charge_current;
  /**<   max input current */

  /* Mandatory */
  uint32 battery_charger_id;
  /**<   used for hvdcp or other charger type indication */
}batt_mngr_control_charging_req_msg;  /* Message */
/**
    @}
  */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; Set operational mode: 0 for factory test, 1 for normal mode */
typedef struct {
  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 operational_mode;
  /**<   command type */
}batt_mngr_set_operational_mode_req_msg;  /* Message */

/** Request Message; for generic glink calls only need to return status*/
typedef struct {
  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 return_status;
  /**<  response message*/
}batt_mngr_generic_resp_msg;  /* Message */

/**
    @}
  */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; Set capacity threshold when charging/discharging to send notification maps to _BTP */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery tag */

  /* Mandatory */
  uint32 power_state;
  /**<   power state flags: power online, discharging, charging, critical */

  /* Mandatory */
  uint32 low_capacity;
  /**<   capacity below which to send a notification */

  /* Mandatory */
  uint32 high_capacity;
  /**<   capacity above which to send a notification */
}batt_mngr_set_status_notification_criteria_req_msg;  /* Message */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; set FCC TO a percentage, maps to _BTH */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery ID*/

  /* Mandatory */
  uint32 charge_rate;
  /**<   percentage of full charge current */

  /* Mandatory */
  uint32 charging_path;
  /**<    0 for usb, 1 for wireless etc.*/
}batt_mngr_set_charge_rate_req_msg;  /* Message */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Indication Message; send notification when the notification criteria is met */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 notification;
  /**<    0x81 for battery insert or removal, 0x80 trip point crossed, 0x82 when status_flag of _BMD change.*/
}batt_mngr_notify_ind_msg;  /* Message */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; Get configuration of the smart battery system, maps to _SBS method. */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 number_batt;
  /**<   response message for number of smart batterys */
}batt_mngr_get_sbs_resp_msg;  /* Message */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; Get the battery information maps to _BIF and _BIX */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery id for which the information is to be retrieved */
}batt_mngr_get_battery_information_req_msg;  /* Message */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; Get the battery information maps to _BIF and _BIX */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /**<   standard response type indicating success or failure 
 ACPI items for _BIX*/

  /* Mandatory */
  uint32 power_unit;
  /**<   unit for capacity, 0-mwh, 1-mAh*/

  /* Mandatory */
  uint32 design_capacity;
  /**<   designed capacity for the new battery*/

  /* Mandatory */
  uint32 last_full_capacity;
  /**<   predicted battery capacity when fully charged*/

  /* Mandatory */
  uint32 battery_tech;
  /**<   battery technology 0-primary, nonrechargable, 1-secondary, rechargable*/

  /* Mandatory */
  uint32 design_voltage;
  /**<   nominal voltage of a new battery in mV e.g. 3700*/

  /* Mandatory */
  uint32 capacity_warning;
  /**<   designed warning capacity level for low battery default_alert2 in mini port*/

  /* Mandatory */
  uint32 capacity_low;
  /**<   designed low capacity level, default_alert1 in mini port*/

  /* Mandatory */
  uint32 cycle_count;
  /**<   charge cycle count, 0xFFFFFFFF unknown cycle count*/

  /* Mandatory */
  uint32 accuracy;
  /**<   battery capacity accuracy in thousandth of percent 800000 = 80% accuracy*/

  /* Mandatory */
  uint32 max_sample_time;
  /**<   max sample time between two _BST in miliseconds, 0xFFFFFFFF if not available*/

  /* Mandatory */
  uint32 min_sample_time;
  /**<   min sample time between two _BST in miliseconds, 0xFFFFFFFF if not available*/

  /* Mandatory */
  uint32 max_average_interval;
  /**<   max average interval in msec, 0xFFFFFFFF if not available*/

  /* Mandatory */
  uint32 min_average_interval;
  /**<   min average interval in msec, 0xFFFFFFFF if not available*/

  /* Mandatory */
  uint32 capacity_granularity1;
  /**<   between low and warning, smallest increment in capacity is capable of measuring*/

  /* Mandatory */
  uint32 capacity_granularity2;
  /**<   between warning and Full, smallest increment in capacity is capable of measuring*/

  /* Mandatory */
  uint32 swappable;
  /**<   0-non-swappable, 1-cold swappable, 2-hot swappable
 additional fields for miniport*/

  /* Mandatory */
  uint32 capabilities;
  /**<   flags capacity_relative, is_short_term, set_charge_supported, system_battery*/

  /* Mandatory */
  char model_number[BATT_MNGR_MAX_STRING_LEN];
  /**<   ASCII string of modle number*/

  /* Mandatory */
  char serial_number[BATT_MNGR_MAX_STRING_LEN];
  /**<   ASCII string of serial number*/

  /* Mandatory */
  char battery_type[BATT_MNGR_MAX_STRING_LEN];
  /**<   ASCII string of OEM specific control method battery type*/

  /* Mandatory */
  char oem_info[BATT_MNGR_MAX_STRING_LEN];
  /**<   ASCII string of battery type of manufactor name*/

  /* Mandatory */
  char battery_chemistry[BATT_MNGR_BATT_CHEMISTRY_MAX_LEN];
  /**<   ASCII string of battery chemistry for mini_port, PbAc, LION etc.*/


  /* Mandatory */
  char uid[BATT_MNGR_MAX_STRING_LEN];
  /**<   string for the battery unique ID for specific battery for mini port*/

  /* Mandatory */
  uint32 critical_bias;
  /**<   a bias from 0 in mwh applied to battery reporting.*/

  /* Mandatory */
  uint8 day;
  /**<   manufacture day for mini port*/

  /* Mandatory */
  uint8 month;
  /**<   manufacture month for mini port*/

  /* Mandatory */
  uint16 year;
  /**<   manufacture year for mini port*/

  /* Mandatory */
  uint32 battery_id;
  /**<   for packing purpose*/

}batt_mngr_get_battery_information_resp_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; set the capacity measurment averaging interval in msec. maps to _BMA */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 interval;

}batt_mngr_set_averaging_interval_req_msg;  /* Message */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; set the capacity measurment sampling time in msec. maps to _BMS */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 sampling_time;
}batt_mngr_set_sampling_time_req_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; get the time to empty in seconds, TTE,  maps to _BTM */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery ID */

  /* Mandatory */
  uint32 rate;
  /**<   discharging rate in mWh or mAh depending on power units 0-current rate, 1-0x7FFFFFFF specified discharge rate*/

  /* Mandatory */
  uint32 reserved;
  
}batt_mngr_get_discharge_time_req_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; get the time to empty in seconds, TTE,  maps to _BTM */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 discharge_time;
  /**<   discharge time in seconds */
}batt_mngr_get_discharge_time_resp_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; get the charge time to the specified percentage in seconds, TTF. maps to _BCT */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery ID */

  /* Mandatory */
  uint32 battery_percent;
  /**<   charge to specific battery percent, valid values 1-100*/

  uint32 reserved;
}batt_mngr_get_charge_time_req_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; get the charge time to the specified percentage in seconds, TTF. maps to _BCT */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 charge_time;
  /**<   charge time in seconds */
}batt_mngr_get_charge_time_resp_msg;  /* Message */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; get the battery maintenance data. maps to _BMD */
typedef struct {
  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery ID */
}batt_mngr_get_bmd_req_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; get the battery maintenance data. maps to _BMD */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 status_flag;
  /**<   bit values see ACPI spec 6.1 table 10-287 */

  /* Mandatory */
  uint32 capability_flag;
  /**<   bit values see ACPI spec 6.1 table 10-287 */

  /* Mandatory */
  uint32 recalib_count;
  /**<   used to recomend that the battery should be calibrated after certain num of cycles.  */

  /* Mandatory */
  uint32 quick_recalib_time;
  /**<   estimated time will take to calibrate the battery if fstatus_flag bit 4 is set  */

  /* Mandatory */
  uint32 slow_recalib_time;
  /**<   estimated time will take to calibrate the battery if status_flag bit 4 is not set  */
}batt_mngr_get_bmd_resp_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; when _BMD capabilties flag field has bit 0, 1, 2 set, use to initiate calibration cycles. maps to _BMC */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery ID */

  /* Mandatory */
  uint32 control_flag;
  /**<   feature control flags defined in ACPI spec 6.1 10.2.2.12 */

  uint32 reserved;
}batt_mngr_set_bmc_req_msg;  /* Message */


/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; this is a generic and expandable method to get, APSD result, flash related info or SOH info */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 charger_id;
  /**<   charger ID */

  /* Mandatory */
  uint32 request_type;
  /**<   request type 0 = flash related info, 1 = SOH info,  2 = APSD result*/

  /* Mandatory */
  uint32 input_info_len;  /**< Must be set to # of elements in input_info */
  uint32 input_info[BATT_MNGR_MAX_GENERIC_MSG_LEN];
  /**<   variable length buffer */
}batt_mngr_get_generic_info_req_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; this is a generic and expandable method to get, APSD result, flash related info or SOH info */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 resp_buffer_len;  /**< Must be set to # of elements in resp_buffer */
  int32 resp_buffer[BATT_MNGR_MAX_GENERIC_MSG_LEN];
}batt_mngr_get_generic_info_resp_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Request Message; this is a generic and expandable method to get test info, such as read/write sram */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 battery_id;
  /**<   battery ID */

  /* Mandatory */
  uint32 request_type;
  /**<   request type 0 = write sram, buffer will include , 1 = read sram  */

  /* Mandatory */
  uint32 req_buffer_len;  /**< Must be set to # of elements in req_buffer */
  uint32 req_buffer[BATT_MNGR_MAX_TEST_INPUT_LEN];
  /**<   variable length buffer, write sram will contain element 0 = address, element 1 = num of words to write,  
   then followed by the buffer to write. Read sram will contain element 0 = address, element 1 = num of words to read*/
}batt_mngr_get_test_info_req_msg;  /* Message */
/**
    @}
  */

/** @addtogroup BATT_MNGR_SVC_qmi_messages
    @{
  */
/** Response Message; this is a generic and expandable method to get test info, such as read/write sram */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 resp_buffer_len;  /**< Must be set to # of elements in resp_buffer */
  uint32 resp_buffer[BATT_MNGR_MAX_TEST_OUTPUT_LEN];
  /**<   for sram read, will return the sram buffer read. */
}batt_mngr_get_test_info_resp_msg;  /* Message */
 

/** ucsi read buffer request  message*/
typedef struct {
    PMIC_GLINK_HEADER_TYPE header;
}ucsi_read_buffer_req_msg;

/** ucsi read buffer response message*/
typedef struct {
    PMIC_GLINK_HEADER_TYPE header;
    uint8 read_buffer[UCSI_BUFFER_SIZE];
    uint32 return_code;
}ucsi_read_buffer_resp_msg;

/** ucsi write buffer request  message*/
typedef struct {
    PMIC_GLINK_HEADER_TYPE header;
    uint8 write_buffer[UCSI_BUFFER_SIZE];
    uint32 reserved;
}ucsi_write_buffer_req_msg;

/** ucsi write buffer resp  message*/
typedef struct {
    PMIC_GLINK_HEADER_TYPE header;
    uint32 return_code;
}ucsi_usbc_write_buffer_resp_msg;

/** Indication Message; send notification when data buffer changes. */
typedef struct {

  PMIC_GLINK_HEADER_TYPE header;
  /**< pmic glink header */

  /* Mandatory */
  uint32 notification;
  uint32 receiver;
  uint32 reserved;
  /* Reserved for padding */
}ucsi_notify_ind_msg;  /* Message */

//usbc read buffer request  message
typedef struct {
    PMIC_GLINK_HEADER_TYPE header;
    uint32 reserved;
}usbc_read_buffer_req_msg;

//usbc read buffer response message
typedef struct {
    PMIC_GLINK_HEADER_TYPE header;
    uint8 read_buffer[USBC_READ_BUFFER_SIZE];
    uint32 return_code;
}usbc_read_buffer_resp_msg;

//usbc write buffer request  message
typedef struct {
    PMIC_GLINK_HEADER_TYPE header;
    uint8 write_buffer[USBC_WRITE_BUFFER_SIZE];
    uint32 reserved;
}usbc_write_buffer_req_msg;

//ucsi write buffer resp message, 
//use ucsi_usbc_write_buffer_resp_msg

#ifdef __cplusplus
}
#endif
#endif

