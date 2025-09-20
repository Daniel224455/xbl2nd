/*=============================================================================
                            
                       Boot PBL Version 1 Header File

GENERAL DESCRIPTION
  This file access to routines and data exported by the PBL to other boot
  loaders.
  
Copyright 2014,2017-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
=============================================================================*/


/*=============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ------------------------------------------------------------
06/18/18   vk      Update to new timestamp struct
04/20/18   yps     Added PBL clock
06/18/17   dp      ROM shared data structure & logging updates
05/28/14   ck      Added boot_pbl_log_milestones
05/28/14   ck      Added boot_pbl_milestone_names
02/03/12   dh      Initial revision.
=============================================================================*/


/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
#include BOOT_PBL_H
#include "boot_error_if.h"
#include "boot_util.h"
#include "boot_logger.h"

/*=============================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/
/* Shared data coming from PBL */
static boot_pbl_shared_data_type   boot_pbl_shared_data;

static char * boot_log_pbl_milestone_names[] = 
{
  "shared_data_version",
  "pbl_apps_init_timestamp",
  "bootable_media_detect_timestamp",
  "bl_elf_metadata_loading_timestamp",
  "bl_hash_seg_auth_timestamp",
  "bl_elf_loadable_segment_loading_timestamp",
  "bl_elf_segs_hash_verify_timestamp",
  "bl_sec_hash_seg_auth_timestamp",
  "bl_sec_segs_hash_verify_timestamp",
  "pbl_populate_shared_data_and_exit_timestamp"
};


/*=============================================================================   
                              FUNCTION DEFINITIONS
=============================================================================*/

/*=========================================================================

**  Function :  boot_pbl_get_pbl_version

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns the PBL version from PBL shared data.
* 
* @par Dependencies
*   None
*   
* @retval
*   uint32
* 
* @par Side Effects
*   None
* 
*/
uint32 boot_pbl_get_pbl_version
(
  void
)
{
  return boot_pbl_shared_data.pbl_version;
}

/*=========================================================================

**  Function :  boot_pbl_get_clock_speed

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns the clock speed set by PBL.
* 
* @par Dependencies
*   None
*   
* @retval
*   uint32
* 
* @par Side Effects
*   None
* 
*/
uint32 boot_pbl_get_clock_speed
(
  void
)
{
  return ((pbl_clock_shared_data_type *)(boot_pbl_shared_data.pbl_clock_shared_data))->nProcSpeedMHz;
} /* boot_pbl_get_clock_speed */


/*===========================================================================

**  Function :  boot_pbl_get_flash_type

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns the flash type detected by PBL
* 
* @par Dependencies
*   None
* 
* @retval
*   boot_flash_type
* 
* @par Side Effects
*   None.
* 
*/
boot_flash_type boot_pbl_get_flash_type
(
  void
)
{
  BL_VERIFY( boot_pbl_shared_data.flash_shared_data != NULL, 
             BL_ERR_NULL_PTR_PASSED );
	
  return ( ((boot_flash_shared_dev_info_type*)boot_pbl_shared_data.flash_shared_data)->type );
} /* boot_pbl_get_flash_type */


/*===========================================================================

**  Function :  boot_pbl_get_flash_shared_data

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns the pointer to the flash related information found
*   by PBL.
* 
* @par Dependencies
*   None
* 
* @retval
*   Pointer of boot_flash_shared_dev_info_type type
* 
* @par Side Effects
*   None.
* 
*/
boot_flash_shared_dev_info_type* boot_pbl_get_flash_shared_data
(
  void
)
{
  BL_VERIFY( boot_pbl_shared_data.flash_shared_data != NULL, 
             BL_ERR_NULL_PTR_PASSED );

  return (boot_pbl_shared_data.flash_shared_data);
} /* boot_pbl_get_flash_shared_data */


/*===========================================================================

**  Function :  boot_pbl_is_auth_enabled

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns whether PBL detected that auth was enabled.
* 
* @par Dependencies
*   None
* 
* @retval
*   boot_boolean
* 
* @par Side Effects
*   None.
* 
*/
boot_boolean boot_pbl_is_auth_enabled(void)
{
  uint32 is_auth_enabled = 0;
  ((pbl_secboot_shared_info_type*)boot_pbl_shared_data.secboot_shared_data)->pbl_secboot_hw_ftbl.
                        secboot_hw_is_auth_enabled(SECBOOT_HW_APPS_CODE_SEGMENT, 
                                                   &is_auth_enabled);
  return is_auth_enabled;
}

/*===========================================================================

**  Function :  boot_pbl_interface_init

** ==========================================================================
*/
/*!
* 
* @brief
*   Function that will initialize the target interface so that generic SBL
*   code can use it.
* 
* @par Dependencies
*   None
* 
* @retval
*   boot_boolean
* 
* @par Side Effects
*   None
* 
*/
boot_boolean boot_pbl_interface_init
(
  boot_pbl_shared_data_type   *pbl_shared_data  /* Pointer to data from PBL */
)
{
  if (pbl_shared_data == NULL)
  {
    return FALSE;
  }

  qmemcpy(&boot_pbl_shared_data, pbl_shared_data, sizeof(boot_pbl_shared_data_type));

  return TRUE;
} /* boot_pbl_interface_init */


/*===========================================================================

**  Function :  boot_log_insert_pbl_milestones

** ==========================================================================
*/
/*!
* 
* @brief
*   This funcion will parse the PBL timestamp milestones passed to SBL
*   and insert them into the boot log.  Currently PBL's unit of measure is
*   clock ticks.  PBL does not pass the clock frequency yet so it is hard
*   wired to 19.2 Mhz.  Also PBL does not pass the names of each field so for
*   now reference a structure of our own to get the names which will have
*   logic ready for the day PBL starts passing them.
*
* @param[in]
*   boot_pbl_shared_data_type
*
* @par Dependencies
*   None
*   
* @retval
*   None
* 
* @par Side Effects
*   None
* 
*/
void boot_pbl_log_milestones(boot_pbl_shared_data_type * pbl_shared_data)
{
  int count = 0;
  uint64 pbl_us_value = 0;
  int num_entries = 0;
  uint32 total_time = 0;
   
  /* PBL timestamp struct is nothing but uint32's */
  uint32 * pbl_timestamp = (uint32 *)(pbl_shared_data->timestamps);
  pbl_timestamp++; /* Skip first entry - Revision */

  num_entries = sizeof(pbl_apps_timestamp_type) / sizeof(*pbl_timestamp);
  for (count = 1; count < num_entries; count++, pbl_timestamp++)
  {
    pbl_us_value = ( ( (uint64)(*pbl_timestamp) ) * PS_PER_PBL_TIMESTAMP_TICK ) / 1000000;
    
    total_time += (uint32)pbl_us_value;
    /*Logs message and time. */
    boot_log_message_raw(boot_log_pbl_milestone_names[count],
                         (uint32)pbl_us_value,
                         LOG_MSG_TYPE_DELTA,
                         NULL);

  }

  boot_log_message_raw("PBL, End",
                       total_time,
                       LOG_MSG_TYPE_STATISTIC,
                       NULL);
} /* boot_pbl_log_milestones */
