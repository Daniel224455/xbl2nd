//============================================================================
/**
 * @file        qusb_dci_845_hw_apis_sbl1.c
 * @author      shreyasr
 * @date        19-June-2013
 *
 * @brief       QUSB (Qualcomm High-Speed USB) DCI (Device-Controller-Interface) HW 
 *              interface for the MDM9x45 platform. Implementation for the SBL1 image.
 * 
 *              Copyright (c) 2013,2015 QUALCOMM Technologies Incorporated.
 *              All Rights Reserved.
 *              Qualcomm Confidential and Proprietary
 * 
 */
//============================================================================

// ===========================================================================
// 
//                            EDIT HISTORY FOR FILE
//   This section contains comments describing changes made to the module.
//   Notice that changes are listed in reverse chronological order.
// 
// 
// when         who         what, where, why
// ----------   -----       ----------------------------------------------------------
// 2015-09-16   kedara       added cache api's  
// 2013-06-19   shreyasr     Porting the code for 9x45
// 2012-07-18   rthoorpu     Porting for 9x25
// 2012-05-24 	shreyasr	Porting the code for 8974
// 2011-07-23   dpatnaik     Re-factored clock ON/OFF and USB Reset routines.
// 
// ===========================================================================

//----------------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------------
#include "busywait.h"  // busywait APIs
#include "HalusbHWIO.h"
#include "qusb_log.h"
#include "qusb_dci_private.h"
#include "boot_cache.h"
#include "qusb_dci_common.h" 
#include "stdio.h"

//----------------------------------------------------------------------------
// Preprocessor Definitions and Constants
//----------------------------------------------------------------------------
#define QUSB_QTIMER_ACCESS_CONTROL      0x3F
#define QUSB_FRAME_NUMBER               0x00
#define QUSB_QTIMER_FREQUENCY_IN_HZ    19200000       //its 19.2MHz 

#define QUSB_FD_PRODUCT_STRING_LENGTH_MAX (60)
#define USB_DESCTYPE_STRING               (3)

//----------------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Externalized Function Definitions
//----------------------------------------------------------------------------

//============================================================================
/**
 * @function  qusb_dci_get_hw_revision
 *
 * @brief Reads HW revision from TCSR register
 *
 *
 * @param None.
 *
 * @return HW major revision.
 *
 */
//============================================================================
qusb_hw_rev qusb_dci_get_hw_rev_major(void)
{
  static qusb_hw_rev hw_rev_major = HW_REV_INVALID;

#if 0 // [JC]
  if(HW_REV_INVALID == hw_rev_major)
  {
    hw_rev_major = (qusb_hw_rev)
                    (HWIO_TCSR_SOC_HW_VERSION_INM(HWIO_TCSR_SOC_HW_VERSION_MAJOR_VERSION_BMSK) 
                     >> HWIO_TCSR_SOC_HW_VERSION_MAJOR_VERSION_SHFT);
  }
#endif
  return hw_rev_major;
}

//============================================================================
/**
 * @function  qusb_dci_dcache_flush
 *
 * @brief Flush Data-Cache
 *
 *
 * @param address
 * @param length
 *
 * @return none
 *
 */
//============================================================================
void qusb_dci_dcache_flush(void* addr, unsigned int length)
{
  dcache_flush_region(addr, length);
}

//============================================================================
/**
 * @function  qusb_dci_dcache_inval
 *
 * @brief Flush Data-Cache
 *
 *
 * @param address
 * @param length
 *
 * @return none
 *
 */
//============================================================================
void qusb_dci_dcache_inval(void* addr, unsigned int length)
{
  dcache_inval_region(addr, length);
}

//============================================================================
/**
* @function  qusb_timer_start
*
* @brief Call the API to start the timer 
*
* @Note : These timers are based on QTIMERS in APPS. This is a count down timer
*
* @param timer_value in uSec. 
*
* @return None
*
*/
//============================================================================
void qusb_timer_start(uint32 timer_value)
{
#if 0 // [JC]
  uint32 timer_tick;
  
  qusb_log(QUSB_START_TIMER_LOG, 0, timer_value);

  // time_tick = uS * 19.2Mhz 
  timer_tick = (timer_value * 19) + (timer_value * 2 / 10);

  //set the timer value in the counter 
  HWIO_APSS_QTMR0_F0V1_QTMR_V1_CNTP_TVAL_OUT(timer_tick);
  return; 
#endif
}

//============================================================================
/**
* @function  qusb_timer_stop
*
* @brief Call the API to stop the timer 
*
* @Note : These timers are based on QTIMERS in APPS. This is a count down timer
*
* @param  None 
*
* @return None
*
*/
//============================================================================
void qusb_timer_stop(void)
{
  // Nothing to do here
  qusb_log(QUSB_STOP_TIMER_LOG, 0, 0);
}

//============================================================================
/**
* @function  qusb_timer_check_for_expiry
*
* @brief Call the API to stop the timer 
*
* @Note : These timers are based on QTIMERS in APPS. This is a count down timer
*
* @param  None 
*
* @return boolean: TRUE if expired else FALSE
*
*/
//============================================================================
boolean qusb_timer_check_for_expiry(void)
{
#if 0 // [JC]
  uint32 timer_value;

  timer_value = HWIO_APSS_QTMR0_F0V1_QTMR_V1_CNTP_TVAL_INM(HWIO_APSS_QTMR0_F0V1_QTMR_V1_CNTP_TVAL_CNTP_TVAL_BMSK);

  // Check if MSB becomes 1b (negative) 
    if (0x80000000 & timer_value)
  {
    qusb_log(QUSB_CHECK_QTIMER_FOR_EXPIRY_LOG, 0, TRUE);
    return TRUE;  
  }
#endif
  return FALSE;
}

