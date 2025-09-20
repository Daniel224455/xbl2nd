/*=======================================================================*//**
 * @file        UsbSharedLib.h
 * @author:     pmahajan
 * @date        01-feb-2017
 *
 * @brief       USB common USB core init implementation Header.
 *
 * @details     This file contains API's the Init sequence for HS and SS core implementation.
 *
 * @note        
 *
 * 
*//*========================================================================*/
//============================================================================
/**
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2007 - 2013 Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php
  
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 */
//============================================================================
// ===========================================================================
// 
//                            EDIT HISTORY FOR FILE
//   This section contains comments describing changes made to the module.
//   Notice that changes are listed in reverse chronological order.
// 
// 
// when         who        what, where, why
// --------     ---        ----------------------------------------------------------
// 04/19/18     amitg  Poipu Multi-Port Support
// 03/19/18     gk     Updated SS and HS Phy sequence for Multiport from latest HPG 
// 04/26/17     amitg  Updated QUSB2 Phy confirguration for Host mode low speed devices
// 03/30/17     pm     Added configuration for sec Clock and core
// 02/01/17     pm 	   Initial revision
//
// ===========================================================================

#ifndef _USB_SHARED_LIB__H_
#define _USB_SHARED_LIB__H_

#include "comdef.h"    // basic types as byte word uint32 etc

#define QUSB_HWIO_ADDR_EMPTY                    (0xFFFFFFFF)
#define QUSB_HWIO_ADDR_END                      (0x0)

//----------------------------------------------------------------------------
// Static Variable Definitions
//----------------------------------------------------------------------------

#define QUSB_DCI_HS_PHY_CFG_ARRAY_ENTRY_CNT           	(4)
#define QUSB_DCI_SS_PHY_COMMON_CFG_ARRAY_ENTRY_CNT      (200)

#define QUSB_DCI_HS_PHY_SEC_CFG_ARRAY_ENTRY_CNT         (4)
#define QUSB_DCI_SS_PHY_SEC_COMMON_CFG_ARRAY_ENTRY_CNT  (200)

#define QUSB_DCI_HS_PHY_MP0_CFG_ARRAY_ENTRY_CNT         (4)
#define QUSB_DCI_SS_PHY_MP0_COMMON_CFG_ARRAY_ENTRY_CNT  (100)

#define QUSB_DCI_HS_PHY_MP1_CFG_ARRAY_ENTRY_CNT         (4)
#define QUSB_DCI_SS_PHY_MP1_COMMON_CFG_ARRAY_ENTRY_CNT  (100)

// Assume a 300 ohm SMB charger resistor on D+/D- which corresponds to HSTX_TRIM offset of value 2
// Subtract QUSB_HSTX_TRIM_OFFSET to make amplitude overshoot by 400mV to account for longer cables and hubs.

#define QUSB_HSTX_TRIM_OFFSET                     (0)
#define QUSB_HS_PHY_PLL_BMSK                      (0x1)
#define QUSB_HS_PHY_PLL_MAX_CNT                   (20000)
#define QUSB_HS_PHY_PLL_CHECK_DELAY_PER_LOOP      (5)

#define MICRO_STALL_UNITS(x)  ((x) * 10)

#define USB_SHARED_ASSERT(val)  usb_shared_error_log(val, 0, __LINE__);

#define QUSB_FD_PRODUCT_STRING_LENGTH_MAX (60)
#define USB_DESCTYPE_STRING               (3)

#define USB_SHARED_CHIP_REVISION(major, minor)  (((major) << 16) | (minor))

typedef struct 
{
  uint32 address;
  uint8  value;
} qusb_phy_cfg_type;

//============================================================================
/**
* @function USB_SHARED_HS_PHY_INIT
*
* @brief API used to initialize the High Speed Primary PHY
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_hs_phy_init(void);

//============================================================================
/**
* @function USB_SHARED_HS_PHY_INIT_SEC
*
* @brief API used to initialize the High Speed Secondary PHY
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_hs_phy_init_sec(void);

//============================================================================
/**
* @function USB_SHARED_HS_PHY_INIT_MP0
*
* @brief API used to initialize the High Speed MultiPort 0 PHY
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_hs_phy_init_MP0(void);

//============================================================================
/**
* @function USB_SHARED_HS_PHY_INIT_MP1
*
* @brief API used to initialize the High Speed MultiPort 1 PHY
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_hs_phy_init_MP1(void);


//============================================================================
/**
* @function USB_SHARED_HS_PHY_INIT
*
* @brief API used to initialize the High Speed RUMI PHY
*
* @Note : 
*
* @param    core_num
*               1st core (primary) = 0, 2nd = 1, etc
*           port_num
*               If core supports multiple ports, 0..4. (0 default)
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_hs_phy_rumi_init(uint32 core_num, uint32 phy_num);

//============================================================================
/**
* @function USB_SHARED_SS_PHY_INIT  
*
* @brief API used to initialize the Super Speed Primary PHY
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_ss_phy_init(uint32 usb_ss_phy_lane);

//============================================================================
/**
* @function USB_SHARED_SS_PHY_INIT_SEC  
*
* @brief API used to initialize the Super Speed Secondary UNI_PHY
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_ss_phy_init_sec();

//============================================================================
/**
* @function USB_SHARED_SS_PHY_INIT_SECDP  
*
* @brief API used to initialize the Super Speed Secondary DP_PHY 
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_ss_phy_init_secDP(uint32 usb_ss_phy_lane);

/**
* @function USB_SHARED_SS_PHY_INIT_MP0  
*
* @brief API used to initialize the Super Speed MultiPort 1 PHY
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_ss_phy_init_MP0();

/**
* @function USB_SHARED_SS_PHY_INIT_MP1  
*
* @brief API used to initialize the Super Speed MultiPort 1 PHY
*
* @Note : 
*
* @param    None 
*
* @return   None
*
*/
//============================================================================
boolean usb_shared_ss_phy_init_MP1();

//============================================================================
/**
* @function usb_shared_poll_ss_phy_training
*
* @brief    Polls USB SS PHY for training
*
*/
//============================================================================
void usb_shared_poll_ss_phy_training(void);

//============================================================================
/**
* @function usb_shared_poll_ss_phy_training_sec
*
* @brief    Polls USB SS PHY for training
*
*/
//============================================================================
void usb_shared_poll_ss_phy_training_sec(void);

//============================================================================
/**
* @function  usb_dci_get_chip_serial_number
*
* @brief Call API to uint32 unique MSM serial string
*
* @Note : None
*
* @param  None 
*
* @return uint32: Chip serial number
*
*/
//============================================================================
uint32 usb_dci_get_chip_serial_number(void);

//============================================================================
/** 
 * API is used to add chip serial number to the string and return 
 * Product String Descriptor
 * 
 * @param pointer to product string
 *              Pointer to the buffer to be updated with Product string desc
 *              Length of the buffer
 *
 * @return Appended string length
 */ 
//============================================================================ 
uint32 usb_get_product_str_desc_with_chip_serial(char* product_str, char* prod_str_desc, uint32 desc_len);

//============================================================================
/**
* @function  usb_shared_is_eud_active
*
* @brief Returns status whether EUD is active 
*        and has already initialized High Speed PHY
*
* @Note : None
*
* @param  None 
*
* @return boolean: TRUE is EUD is active, FALSE otherwise
*
*/
//============================================================================
boolean usb_shared_is_eud_active(void);

// Wrapper functions for Core/Loader specific implementaion
void usb_shared_delay_us(uint32 usecs);
void usb_shared_loop_here(void); 
void usb_shared_uart_log_info(char *message);
void usb_shared_uart_log_error(char *message, uint32 value);
void usb_shared_error_log(uint32 id_0, uint8 offset, uint32 param);
uint32 usb_shared_get_chip_revision(void);
#endif /*_USB_SHARED_LIB__H_*/
