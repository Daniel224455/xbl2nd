#ifndef __EFIPSLEEP_H__
#define __EFIPSLEEP_H__

/** 
  @file  EFIPsleep.h
  @brief PSleep API.
*/
/*=============================================================================
  Copyright (c) 2018 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY

 when       who     what, where, why
 --------   ---     ----------------------------------------------------------

=============================================================================*/

extern EFI_GUID gEfiPsleepProtocolGuid;

//--------------------------------------------------------------------------
// Sleep Stats Defines 
//--------------------------------------------------------------------------
typedef struct
{
  uint32 stat_type;	  //type of sleep stat (ascii little endian)
  uint32 count;           //# times this mode has been entered 
  uint64 last_entered;    //last timestamp we entered this mode
  uint64 last_exited;     //last timestamp we exited this mode
  uint64 total_duration;  //total amount of time we've spent in this mode
} sleep_stats_t;

typedef enum power_sleep_stats_enum_t
{
  POWER_SLEEP_STATS_AOSD = 0, // AOSS Sleep
  POWER_SLEEP_STATS_CXSD = 1, // CX Collapse
  POWER_SLEEP_STATS_C4D4 = 2, // Apps subsystem full PC
  POWER_SLEEP_STATS_LAST = POWER_SLEEP_STATS_C4D4,
  
  POWER_SLEEP_STATS_NUM_STATS = POWER_SLEEP_STATS_LAST + 1
} power_sleep_stats_enum_t;

//--------------------------------------------------------------------------
// Sleep Callback Defines 
//--------------------------------------------------------------------------
typedef enum
{
  PSLEEP_CB_C4D4_ENTER,
  PSLEEP_CB_C4D4_EXIT,
}psleep_cb_mode_enum;

//--------------------------------------------------------------------------
// Protocol Defines 
//--------------------------------------------------------------------------

typedef void (*psleep_cb_fnc)(psleep_cb_mode_enum mode, void * data);
typedef void (*PsleepRegisterSleepCb) (psleep_cb_fnc fnc, void *data);
typedef sleep_stats_t *(*PsleepGetLpmCount) (power_sleep_stats_enum_t sleep_mode);

typedef struct 
{
  UINT64           Version;	
  PsleepRegisterSleepCb RegisterSleepCb; //registers a callback to be invoked on the way in/out of system sleep
  PsleepGetLpmCount     GetLpmCount;     //retrieves the stats for a given low power mode 
}EFI_PSLEEP_PROTOCOL;


#endif //__EFIPSLEEP_H__
