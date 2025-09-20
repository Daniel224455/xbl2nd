/**
 * @file esp_dpp.h
 *
 * Public header for EFI System Partition and Device Provisioning for Windows on ARM.
 *
 * This utility library provides interfaces for accessing the EFI System Partition
 * and files stored in Device Provisioning area.
 *
 * !!!IMPORTANT!!!
 *
 * Currently this header is shared between UEFI OsAppsPkg and FactoryTool/UpdateTool.
 * Due to build restriction, two identical copies of esp_dpp.h are maintained at:
 *
 * <<APSS>>/QCDK/main/latest/inc/esp_dpp.h
 * <<UEFI>>/QcomPkg/Library/FwCommonLib/esp_dpp.h
 *
 * MAKE SURE THESE TWO COPIES ARE ALWAYS IN-SYNC WITH EACH OTHER!!!
 *
 */

/*==============================================================================
     Copyright (c) 2011-2013,2016,2018 Qualcomm Technologies, Inc.  All Rights Reserved.
     Qualcomm Technologies Proprietary and Confidential.
==============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/21/18   gparam  Clean up unused definitions
08/27/16   ltg     Updates to add new error types for flash type errors
05/22/13   rsb     Added support for HDCK 2.x key provisioning
10/23/12   mic     Added support for firmware update return code
06/04/12   mic     Added support for OPM public key and encrypted OPM private key provisioning
02/23/12   rsb     Added support for UEFI secure boot provisioning
01/25/12   jd      Made it compatible with UEFI build environment
12/02/11   rsb     Clean up API
09/20/11   rsb     Created initial library for ESP/DPP on WoA

===========================================================================*/

#ifndef ESP_DPP_H
#define ESP_DPP_H

/*===========================================================================
                                INCLUDE FILES
===========================================================================*/
/*=========================================================================== 
                           DEVICE UPDATE DEFINITIONS
===========================================================================*/
  
/*!
 * Enum defining the different return codes for the QCFirmwareUpdate.exe, 
 * Please note this flag is only 8 bits.
 */

typedef enum
{
   QCFW_UPDATE_SUCCESS                                        = 0x00,
   QCFW_UPDATE_UNKNOWN                                        = 0x01,
   QCFW_UPDATE_INIT                                           = 0x02,
   QCFW_UPDATE_ERR_FLAG_GET_STAMACHINE_STATE                  = 0x03,

   /*State and flag related error*/
   QCFW_UPDATE_ERR_FLAG_STATE_EXCEEDED_UPDATE_ATTEMPT_MAX     = 0x20,  
 
   /*ESRT related error*/
   QCFW_UPDATE_ERR_ESRT_GET_ENTRY                             = 0x30,

   /*Capsule related error*/
   QCFW_UPDATE_ERR_CAPSULE_REGISTER_SYSTEM_CAPSULE            = 0x40,

   /*UEFI API related error*/
   QCFW_UPDATE_ERR_UEFI_API_PRINT                             = 0x50,
   QCFW_UPDATE_ERR_UEFI_API_ALLOCATE_POOL                     = 0x51,

   /*Payload related error*/
   QCFW_UPDATE_ERR_PAYLOAD_LOWEST_SUPPORT_VERSION             = 0x60,
   QCFW_UPDATE_ERR_PAYLOAD_AUTHENTICATION                     = 0x61,
   QCFW_UPDATE_ERR_PAYLOAD_IMG_FORMAT                         = 0x62,
   QCFW_UPDATE_ERR_PAYLOAD_LOADING                            = 0x63,
   QCFW_UPDATE_ERR_PAYLOAD_LOAD_FROM_ESP                      = 0x64,
   QCFW_UPDATE_ERR_PAYLOAD_LOAD_FROM_ESP_LOCATE_PROTOCOL      = 0x65,
   QCFW_UPDATE_ERR_PAYLOAD_LOAD_FROM_ESP_HANDLE_PROTOCOL      = 0x66,
   QCFW_UPDATE_ERR_PAYLOAD_LOAD_FROM_ESP_GET_FILE_NAME        = 0x67,
   QCFW_UPDATE_ERR_PAYLOAD_LOAD_FROM_ESP_ALLOCATE_POOL        = 0x68,
   QCFW_UPDATE_ERR_PAYLOAD_LOAD_FROM_ESP_APPEND_DEV_NODE      = 0x69,

   /*Resource related error*/
   QCFW_UPDATE_ERR_RESOUCE_POWER_INSUFFICEIENT_BATTERY        = 0x70,

   /*Handle related error*/
   QCFW_UPDATE_ERR_HANDLE_REGISTRY                            = 0x90,

   /*MetaData related error*/
   QCFW_UPDATE_ERR_METADATA_INVALID_PARAM                     = 0xA0,
   QCFW_UPDATE_ERR_METADATA_GET_FV_HANDLE                     = 0xA1,
   QCFW_UPDATE_ERR_METADATA_FV_READ                           = 0xA2,
   QCFW_UPDATE_ERR_METADATA_BAD_SIGNATURE                     = 0xA3,
   QCFW_UPDATE_ERR_METADATA_BAD_HEADER_SIZE                   = 0xA4,
   QCFW_UPDATE_ERR_METADATA_BAD_V1_HEADER_SIZE                = 0xA5,
   QCFW_UPDATE_ERR_METADATA_BAD_V2_HEADER_SIZE                = 0xA6,
   QCFW_UPDATE_ERR_METADATA_BAD_V3_HEADER_SIZE                = 0xA7,
   /*Parser related error*/
   QCFW_UPDATE_ERR_PARSER_SET_PARTITION_HANDLES               = 0xB0,

   /*Crypto related error*/
   QCFW_UPDATE_ERR_CRYPTO_SSD_CHECKING                        = 0xC0,
   QCFW_UPDATE_ERR_CRYPTO_SSD_DECRYPT                         = 0xC1,
   QCFW_UPDATE_ERR_CRYPTO_OPM_PRIV_KEY_CRYPTO                 = 0xC2,
   QCFW_UPDATE_ERR_CRYPTO_MSM_KEY_ENCRYPT                     = 0xC3,

   /*FwEntry related error*/
   QCFW_UPDATE_ERR_FWENTRY_READ_ENTRY_FROM_FV                 = 0xD0,
   QCFW_UPDATE_ERR_FWENTRY_UNKNOWN_UPDATE_TYPE                = 0xD1,
   QCFW_UPDATE_ERR_FWENTRY_UNKNOWN_UPDATE_BACKUP_TYPE         = 0xD2,
   QCFW_UPDATE_ERR_FWENTRY_NULL_ENTRY                         = 0xD3,
   QCFW_UPDATE_ERR_FWENTRY_VALUE_CHECKING                     = 0xD4,
   QCFW_UPDATE_ERR_FWENTRY_BAD_VALUE                          = 0xD5,
   QCFW_UPDATE_ERR_FWENTRY_COMBINATION_CKECKING               = 0xD6,
   QCFW_UPDATE_ERR_FWENTRY_BAD_COMBINATION                    = 0xD7,
   QCFW_UPDATE_ERR_FWENTRY_FLASHTYPE_CHECKING                 = 0xD8,
   QCFW_UPDATE_ERR_FWENTRY_BAD_FLASHTYPE                      = 0xD9,

   /*FwEntry list related error*/
   QCFW_UPDATE_ERR_LIST_ADD_NODE                              = 0xE0,
   QCFW_UPDATE_ERR_LIST_SSD_EXCLUSIVE_CHECKING                = 0xE1,
   QCFW_UPDATE_ERR_LIST_SSD_NOT_EXCLUSIVE                     = 0xE2,
   QCFW_UPDATE_ERR_LIST_PARTITION_DEVPATH_EXCLUSIVE_CHECKING  = 0xE3,
   QCFW_UPDATE_ERR_LIST_PARTITION_DEVPATH_NOT_EXCLUSIVE       = 0xE4,
   QCFW_UPDATE_ERR_LIST_QCOM_DPP_DEVPATH_EXCLUSIVE_CHECKING   = 0xE5,
   QCFW_UPDATE_ERR_LIST_QCOM_DPP_DEVPATH_NOT_EXCLUSIVE        = 0xE6,
   QCFW_UPDATE_ERR_LIST_OEM_DPP_DEVPATH_EXCLUSIVE_CHECKING    = 0xE7,
   QCFW_UPDATE_ERR_LIST_OEM_DPP_DEVPATH_NOT_EXCLUSIVE         = 0xE8,
   QCFW_UPDATE_ERR_LIST_FAT_DEVPATH_EXCLUSIVE_CHECKING        = 0xE9,
   QCFW_UPDATE_ERR_LIST_FAT_DEVPATH_NOT_EXCLUSIVE             = 0xEA,
   QCFW_UPDATE_ERR_LIST_NULL_NODE                             = 0xEB,
   QCFW_UPDATE_MAX 

} QCFW_UPDATE_FLAG_VALUE;


/*=========================================================================== 
                         FIRMWARE UPDATE DEFINITIONS
===========================================================================*/

/*=========================================================================== 
                         PUBLIC FUNCTION DECLARATIONS
===========================================================================*/

#endif /* ESP_DPP_H */
