/** @file FwESRT.h

   Header for functions to maintain the ESRT , including ESRT
   header and ESRT entry .
  
  Copyright (c) 2012-2015, 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who      what, where, why
 --------       ---      --------------------------------------------------
 2018/01/31     prema    Updated System Firmware Resource GUID for 845
 2017/05/08     gparam   Changes to centralize ESRT update to DPP to the end of
                         SetImage API
 2017/04/18     gparam   Changes to centralize the recording of update status
                         in DPP for future use
 2017/04/18     gparam   Updated function names to match the latest capsule flow +
                         Removed unused/redundant definitions/parameters not
                         used in capsule flow
 2017/02/27     pkn      Added changes required to support new update type - FWCLASS_GUID
 2017/02/24     gparam   Changes to correct the prefilled known SystemFirmware
                         GUIDs for the default Qualcomm form-factors
 2017/02/22     gparam   Added the default 8998 specific system firmware GUIDs and
                         removed 8996 GUID
 2017/02/13     gpavithr Changes to use ESRTDxe inplace of local functions
                         to directly create and/or operate on ESRT table
 2015/03/31     mic      Added 8994 platform types
 2013/04/11     rsb      Added 8084 platform types
 2012/11/30     mic      Added ESRT entry print function
 2012/11/09     jmb      Adding CBH platform
 2012/10/21     mic      Code review
 2012/08/14     ash      Added 8064 WB292 and V1 variants
 2012/06/18     jd       rename OEM_UPDATE_EVENT_GUID
 2012/06/05     rs       Added UXCapsule OEM Update Event GUID
 2012/05/09     rsb      Platform run-time type detection for FW Resource GUID
 2012/05/07     jd       Migrated to DPP Protocol.
 2012/02/01     mic      Initial version

=============================================================================*/
#ifndef __QCOM_FW_ESRT_H_
#define __QCOM_FW_ESRT_H_

#include <Guid/SystemResourceTable.h>

#define QCOM_FW_UPDATE_PRODUCT_VERSION_10 0x00010000
#define QCOM_FW_UPDATE_PRODUCT_VERSION_11 0x00010001
#define QCOM_FW_UPDATE_PRODUCT_VERSION_12 0x00010002

/* System Firmware Resource GUID for 845 MTP: {250A9F97-22DE-4008-895B-452797FAB3CF} */
#define QCOM_FW_UPDATE_SYSTEM_FW_845_MTP_GUID \
{ \
  0x250A9F97, 0x22DE, 0x4008, { 0x89, 0x5B, 0x45, 0x27, 0x97, 0xFA, 0xB3, 0xCF } \
}

/* System Firmware Resource GUID for 845 CLS: {AC9C5007-A27E-4CC8-B031-63354F926F56} */
#define QCOM_FW_UPDATE_SYSTEM_FW_845_CLS_GUID \
{ \
  0xAC9C5007, 0xA27E, 0x4CC8, { 0xB0, 0x31, 0x63, 0x35, 0x4F, 0x92, 0x6F, 0x56 } \
}

/* System Firmware Resource GUID for 845 CLS: {451EF112-CFD6-44E5-BEC6-DED2E6C26127} */
#define QCOM_FW_UPDATE_SYSTEM_FW_845_QRD_GUID \
{ \
  0x451EF112, 0xCFD6, 0x44E5, { 0xBE, 0xC6, 0xDE, 0xD2, 0xE6, 0xC2, 0x61, 0x27 } \
}

/* System Firmware Resource GUID for UNKNOWN platform type: {06CE6651-B594-4AC3-B57A-D9637CC335A8} */

#define QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_GUID \
{ \
  0x06CE6651, 0xB594, 0x4AC3, { 0xB5, 0x7A, 0xD9, 0x63, 0x7C, 0xC3, 0x35, 0xA8 } \
}

/* UX Capsule GUID : {3b8c8162-188c-46a4-aec9-be43f1d65697}  */

#define QCOM_FW_UPDATE_UX_CAPSULE_GUID \
{ \
  0x3B8C8162, 0x188C, 0x46A4, { 0xAE, 0xC9, 0xBE, 0x43, 0xF1, 0xD6, 0x56, 0x97 } \
}

#define QCOM_FW_UPDATE_ESRT_GUID \
{ \
  0xB122A263, 0x3661, 0x4F68, { 0x99, 0x29, 0x78, 0xf8, 0xb0, 0xd6, 0x21, 0x80 } \
}

/*{592515FE-F062-4AD0-89C8-9118F0F2D6BF}*/
#define QCOM_FW_UPDATE_OEM_UPDATE_EVENT_GUID \
{ \
  0x592515FE, 0xF062, 0x4AD0, { 0x89, 0xC8, 0x91, 0x18, 0xF0, 0xF2, 0xD6, 0xBF } \
}

typedef enum {
  FW_TYPE_UNKNOWN,
  FW_TYPE_SYSTEM,
  FW_TYPE_DEVICE,
  FW_TYPE_DRIVER
} ESRT_FWTYPE;

typedef enum {
  FW_LAST_ATMPT_STATUS_SUCCESS,
  FW_LAST_ATMPT_STATUS_UNSUCCESSFUL,
  FW_LAST_ATMPT_STATUS_INSUFF_RESOURCE,
  FW_LAST_ATMPT_STATUS_INCORRECT_VERSION,
  FW_LAST_ATMPT_STATUS_INVALID_IMG_FMT,
  FW_LAST_ATMPT_STATUS_AUTHEN_ERROR,
  FW_LAST_ATMPT_STATUS_POWER_AC_NOT_CONNECTED,
  FW_LAST_ATMPT_STATUS_POWER_INSUFFICIENT_BATTERY

} ESRT_LAST_ATTEMPT_STATUS;

/* The list of System Firmware Resource types for each platform, this enumeration must match
   one to one with qcomFwResourceGUIDs. */
typedef enum _QCOM_FW_RESOURCE_TYPE
{
  QCOM_FW_UPDATE_SYSTEM_FW_845_MTP_TYPE,
  QCOM_FW_UPDATE_SYSTEM_FW_845_CLS_TYPE,
  QCOM_FW_UPDATE_SYSTEM_FW_845_QRD_TYPE,
  QCOM_FW_UPDATE_SYSTEM_FW_UNKNOWN_TYPE

} QCOM_FW_RESOURCE_TYPE;

typedef struct
{
  UINT32 FWResrcCnt;
  UINT32 FWResrcMax;
  UINT64 FWResrcVer;

}ESRT_TABLE_HEADER;

/**
 * Function to print ESRT entry
 *
 * @param ent  - ESRT entry
 * @DebugLevel - Debug level
 *
 * @return : EFI_STATUS
 */
EFI_STATUS
QcomPrintEsrtEntry(
  IN EFI_SYSTEM_RESOURCE_ENTRY *ent,
  IN UINTN      DebugLevel
  );

/**
* Get the ESRT entry info
*
* @param  ent         - Copy of current ESRT entry
* @return EFI_STATUS
**/
EFI_STATUS
QcomGetEsrtEntryCache(
  OUT EFI_SYSTEM_RESOURCE_ENTRY  *ent
  );

/**
 * This Function try to update ESRT in both DPP and system table
 *
 * @param  ent            - Copy of new ESRT entry to update in cache and DPP
 * @param cleanBackupFile - TRUE indicates to delete the backup file if found
 *                          instead of processing the file
 *                          FALSE indicates to process the file if found
 *
 * @return EFI_STATUS
 */

EFI_STATUS
QcomUpdateEsrtEntry(
  IN EFI_SYSTEM_RESOURCE_ENTRY  *ent,
  IN BOOLEAN                     cleanBackupFile
  );

/**
 * This Function kicks off the ESRT  Phase of
 * the Firmware update process.
 *
 * @return EFI_STATUS
 **/
EFI_STATUS
QcomESRTPhaseMain(
  );

#endif
