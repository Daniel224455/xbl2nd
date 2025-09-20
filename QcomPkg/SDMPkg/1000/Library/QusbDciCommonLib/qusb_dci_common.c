/*=======================================================================*//**
 * @file        qusb_dci_common.c
 * @author:     kameya
 * @date        28-Dec-2015
 *
 * @brief       QUSB common parts of DCI shared by QusbFedlLib & QusbTargetLib.
 *
 * @details     This file contains the implementation of basic APIs to be used
 *              by both QusbTargetLib and QusbFedlLib
 *
 * @note        
 *
 * 
*//*========================================================================*/
//============================================================================
/**
  Copyright (c) 2015, 2017-2018 Qualcomm Technologies, Inc. All rights reserved.
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
// when          who     what, where, why
// ----------   -----    ----------------------------------------------------------
// 02/13/18      gk	 Port changes from boot.xf.2.0 and boot.xf.2.2
// 2017-02-02    pm      Add UsbSharedLib
// 2015-12-28    kameya  First Draft
// 
// ===========================================================================

//----------------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------------
#include "comdef.h"    // basic types as byte word uint32 etc
#include "busywait.h"  // busywait APIs
#include "DALSys.h"    // To get phy settings
#include "qusb_dci_common.h"
#include "HalusbHWIO.h"
#include "qusb_log.h"
#include "PlatformInfo.h"
#include "UsbSharedLib.h"
#include "pm_ldo.h"

#define QUSB_DCI_PMD8998_INDEX      (0)

#define USB_FEDL_MAX_LS_TRIES       (50)
#define USB_FEDL_DP_BMSK            (0x1)

// ---------------------------------------------------------------------------
// EUD Tuning Parameter register table ++
// ---------------------------------------------------------------------------

#define USB2PHY_OFF(a) ((a)-(AHB2PHY_SOUTH_USB2PHY_PRIM_USB2PHY_PRIM_CM_DWC_USB2_REG_BASE))
#define EUD_USB_PHY_EMPTY (0x00)

#define QUSB_EUD_TUNE_TABLE_SIZE (sizeof(qusb_dci_eud_phy_reg_array)/sizeof(qusb_dci_eud_phy_reg_array_t))

typedef struct 
{
  uint8   address;
  uint8   value;
} qusb_dci_eud_phy_reg_array_t;

static qusb_dci_eud_phy_reg_array_t qusb_dci_eud_phy_reg_array[] =
{
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_ADDR ),                      (0x02) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL5_ADDR ),                (0x02) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR ),     (HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X0_POR) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR ),     (0x43) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR ),     (HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X2_POR) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR ),     (HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X3_POR) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_HS_PHY_CTRL2_ADDR ),              (0x0E) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL5_ADDR ),                (0x00) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_HS_PHY_CTRL2_ADDR ),              (0x06) },
  { USB2PHY_OFF( HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_ADDR ),                      (0x00) },
};
// ---------------------------------------------------------------------------
// EUD Tuning Parameter register table --
// ---------------------------------------------------------------------------

//============================================================================

/**
* @function  qusb_dci_get_platform
*
* @brief Read platforminfo from DAL API and cache it.
*
* @Note : None.
*
* @param None.
*
* @return None.
*
*/
//============================================================================
qusb_dci_platform_type qusb_dci_get_platform(void)
{
  static qusb_dci_platform_type qusb_platform = QUSB_DCI_PLATFORM_UNKNOWN;
  DalPlatformInfoPlatformType platform;

  if (qusb_platform == QUSB_DCI_PLATFORM_UNKNOWN)
  {
    platform = PlatformInfo_Platform();

    switch(platform)
    {
      case DALPLATFORMINFO_TYPE_RUMI:
      {
        qusb_platform = QUSB_DCI_PLATFORM_RUMI;
      }
      break;
      
      case DALPLATFORMINFO_TYPE_VIRTIO:
      {
        qusb_platform = QUSB_DCI_PLATFORM_VIRTIO;
      }
      break;

      default:
        qusb_platform = QUSB_DCI_PLATFORM_SILICON;
    }
  }

  return qusb_platform;
}


//============================================================================

/**
* @function  qusb_dci_delay_ms
*
* @brief Perform delay in milliseconds.
*
* @Note : The USB Timers can not be used before the core is initialized.
*
* @param milliseconds
*
* @return none
*
*/
//============================================================================
void qusb_dci_delay_ms(uint32 msecs)
{
  int counter;

  for (counter = 0; counter < msecs; counter++)
  {
    busywait(1000); // 1 ms
  }   
}


//============================================================================
/**
* @function  qusb_dci_delay_us
*
* @brief Perform delay in microseconds.
*
* @Note : The USB Timers can not be used before the core is initialized.
*
* @param microseconds
*
* @return none
*
*/
//============================================================================
void qusb_dci_delay_us(uint32 usecs)
{
  busywait( usecs );
}

// ===========================================================================
/**
 * @function    qusb_hs_phy_gcc_reset
 * 
 * @brief   API used for resetting High Speed QUSB2 PHY using GCC control
 *  
 * @details API is used for resetting High Speed QUSB2 PHY using GCC control
 * 
 * @param   None.
 * 
 * @return  None.
 * 
 */
// ===========================================================================
void qusb_hs_phy_gcc_reset(void)
{
  HWIO_GCC_QUSB2PHY_PRIM_BCR_OUTM(HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK,(0x1 << HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_SHFT));

  qusb_dci_delay_us(100);

  HWIO_GCC_QUSB2PHY_PRIM_BCR_OUTM(HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK,(0x0 << HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_SHFT));
}

// ===========================================================================
/**
 * @function    qusb_dci_usb30_gcc_reset
 * 
 * @brief   API used for resetting the Link and PHYs using GCC control
 *  
 * @details This API is used for resetting the Link and PHYs using clock control 
 * 
 * @param   None.
 * 
 * @return  None.
 * 
 */
// ===========================================================================
void qusb_dci_usb30_gcc_reset(void)
{
  boolean is_eud_active = usb_shared_is_eud_active();
  
  /* Reset SNPS Link controller */
  HWIO_GCC_USB30_PRIM_BCR_OUTM(HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_BMSK,(0x1 << HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_SHFT));
  
  qusb_dci_delay_us(100);
  
  HWIO_GCC_USB30_PRIM_BCR_OUTM(HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_BMSK,(0x0 << HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_SHFT));
  
  /* Reset USB 2.0 and QMP (USB 3.0) PHYs */
  
  // SNPS Femto GCC RESET
  if (FALSE == is_eud_active)
  {
    HWIO_GCC_QUSB2PHY_PRIM_BCR_OUTM(HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK,(0x1 << HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_SHFT));
  }
  HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OUTM(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK,(0x1 << HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_SHFT));
  
  HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OUTM(HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK, (0x1 << HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_SHFT));
  
  qusb_dci_delay_us(100);
  
  HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OUTM(HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK, (0x0 << HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_SHFT));
  HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OUTM(HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK,(0x0 << HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_SHFT));
  if (FALSE == is_eud_active)
  {
    HWIO_GCC_QUSB2PHY_PRIM_BCR_OUTM(HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK,(0x0 << HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_SHFT));
  }
  
  qusb_dci_delay_us(100);  

  return; 
}

// ===========================================================================
/**
 * @function    qusb_hs_phy_refclk_enable
 * 
 * @brief   This function will be used to enable / disable HS PHY reference clock.
 * 
 * @param  TRUE or FALSE depending on enable or disable.
 * 
 * @return  None.
 * 
 */
// ===========================================================================
void qusb_hs_phy_refclk_enable(boolean enable)
{
  // For future reference
}

// ===========================================================================
/**
 * @function    qusb_dci_select_utmi_clk
 * 
 * @brief   This is used for configuring the core to UTMI clock instead of pipe
 *          clock.  This needs to be called when there is no SS USB PHY.
 * 
 * @param   None.
 * 
 * @return  None.
 * 
 */
// ===========================================================================
void qusb_dci_select_utmi_clk(void)
{
  qusb_error_log(QUSB_DCI_SELECT_UTMI_CLK_LOG, 0, 0);
  
  // If operating without SS PHY, follow this sequence to disable 
  // pipe clock requirement
  HWIO_USB3_PRI_GENERAL_CFG_OUTM(HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_DIS_BMSK,
    0x1 << HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_DIS_SHFT);
  
  qusb_dci_delay_us(100);
  
  HWIO_USB3_PRI_GENERAL_CFG_OUTM(HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_SEL_BMSK,
    0x1 << HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_SEL_SHFT);
  
  qusb_dci_delay_us(100);
  
  HWIO_USB3_PRI_GENERAL_CFG_OUTM(HWIO_USB3_PRI_GENERAL_CFG_PIPE3_PHYSTATUS_SW_BMSK,
    0x1 << HWIO_USB3_PRI_GENERAL_CFG_PIPE3_PHYSTATUS_SW_SHFT);
  
  qusb_dci_delay_us(100);
  
  HWIO_USB3_PRI_GENERAL_CFG_OUTM(HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_DIS_BMSK,
    0x0 << HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_DIS_SHFT);
}

// ===========================================================================
/**
 * @function    qusb_dci_hs_phy_update_hstx_trim
 * 
 * @brief   This function will update TUNE2 HSTX_TRIM register bits if feature is enabled.
 * 
 * @param   None.
 * 
 * @return  None.
 * 
 */
// ===========================================================================
void qusb_dci_hs_phy_update_hstx_trim(void)
{
  // For future reference
}

// =============================================================================
/**
 * @function    qusb_dci_ahb2phy_cfg
 * 
 * @brief   Configures AHB2PHY read/write states to be 1 wait cycle each.
 * 
 * @param   None.
 * 
 * @return  None.
 * 
 */
// =============================================================================
void qusb_dci_ahb2phy_cfg(void)
{
  qusb_dci_delay_us(10);
  
  // Write 0x11 to AHB2PHY bridge CSR PERIPH_SS_AHB2PHY_TOP_CFG so that 
  // writes and reads to/from the PHY use one wait state.
  // This is essential to operate at nominal freq with lower CX rail voltages.
  // HAL_USB_WRITE (AHB2PHY_SOUTH_AHB2PHY_SOUTH_BASE, HWIO_AHB2PHY_SOUTH_AHB2PHY_SOUTH_AHB2PHY_TOP_CFG_OFFS, 0x11);
}

// ===========================================================================
/**
 * @function    qusb_hs_phy_init
 * 
 * @brief   API used to initialize the High Speed PHY.
 * 
 * @param   None.
 * 
 * @return  TRUE if PHY initializes successfully, FALSE otherwise.
 * 
 */
// ===========================================================================
boolean qusb_hs_phy_init(void)
{
  if (usb_shared_is_eud_active())
  {
    return TRUE;
  }
  return usb_shared_hs_phy_init();
}

// ===========================================================================
/**
 * @function    qusb_dci_dp_gnd_check
 * 
 * @brief   API used to check if D+ is grounded.
 * 
 * @param   None.
 * 
 * @return  None.
 * 
 */
// ===========================================================================
boolean qusb_dci_dp_gnd_check(void)
{
  uint8 linestate = 0;
  uint16 ls_tries = 0;
  boolean enter_fedl = FALSE;

  qusb_error_log(DCI_SKIP_FEDL_LOG, 0, FALSE);

  // Set UTMI_PHY_CMN_CNTRL_OVERRIDE_EN
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_UTMI_PHY_CMN_CTRL_OVERRIDE_EN_BMSK, 
    0x1 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_UTMI_PHY_CMN_CTRL_OVERRIDE_EN_SHFT);

  // Set UTMI_PHY_DATAPATH_CTRL_OVERRIDE_EN
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_UTMI_PHY_DATAPATH_CTRL_OVERRIDE_EN_BMSK,
    0x1 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_UTMI_PHY_DATAPATH_CTRL_OVERRIDE_EN_SHFT);

  // Set OPMODE to normal i.e. 0x00
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_OPMODE_BMSK,
    0x0 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_OPMODE_SHFT);

  // Set XCVRSEL and TERMSEL to FS.
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL1_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL1_XCVRSEL_BMSK,
    0x1 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL1_XCVRSEL_SHFT);

  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_TERMSEL_BMSK,
    0x1 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_TERMSEL_SHFT);

  usb_shared_delay_us(20);

  // Enter forced EDL mode if D+ is grounded for 50ms.
  for(ls_tries = 0; (linestate == 0) && (ls_tries < USB_FEDL_MAX_LS_TRIES);  ls_tries++)
  {
    linestate = (HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_RX_PORT_STATUS_INM(
      HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_RX_PORT_STATUS_LINESTATE_BMSK) 
      >> HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_RX_PORT_STATUS_LINESTATE_SHFT) & USB_FEDL_DP_BMSK;

    if(linestate == USB_FEDL_DP_BMSK)
    {
      break;
    }
    usb_shared_delay_us(1000);
  }

  if((linestate == 0) && (ls_tries == USB_FEDL_MAX_LS_TRIES))
  {
    enter_fedl = TRUE;
  }

  // Clear TERMSEL and XCVRSEL
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_TERMSEL_BMSK,
    0x0 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_TERMSEL_SHFT);

  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL1_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL1_XCVRSEL_BMSK,
    0x0 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL1_XCVRSEL_SHFT);

  // Clear UTMI_PHY_CMN_CTRL_OVERRIDE_EN
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_UTMI_PHY_CMN_CTRL_OVERRIDE_EN_BMSK,
    0x0 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_UTMI_PHY_CMN_CTRL_OVERRIDE_EN_SHFT);

  // Clear UTMI_PHY_DATAPATH_CTRL_OVERRIDE_EN
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_OUTM(
    HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_UTMI_PHY_DATAPATH_CTRL_OVERRIDE_EN_BMSK,
    0x0 << HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_CFG0_UTMI_PHY_DATAPATH_CTRL_OVERRIDE_EN_SHFT);

  usb_shared_delay_us(20);

  qusb_error_log(DCI_ENTER_FEDL_LOG, 0, enter_fedl);

  return enter_fedl;
}

// =============================================================================
/**
 * @function    qusb_hs_phy_nondrive_cfg
 * 
 * @brief   Function used to configure High Speed PHY in non-drive mode.
 * 
 * @param   None.
 * 
 * @return  None.
 * 
 */
// =============================================================================
void qusb_hs_phy_nondrive_cfg(void)
{
  // Not needed
}


// ===========================================================================
/**
 * @function    qusb_dci_eud_phy_tune
 * 
 * @brief       This function will populate QUSB2PHY registers onto EUD mode 
 *              manager CSR.
 * 
 * @param   none.
 * 
 * @return  none.
 * 
 */
// ===========================================================================
void qusb_dci_eud_phy_tune(void)
{
  uint32 addr   = 0 ;
  uint32 index  = 0;

  for (index = 0, addr = HWIO_AHB2PHY_SOUTH_EUD_Q2_TUNE_ADR00_ADDR; 
      (index < QUSB_EUD_TUNE_TABLE_SIZE) && (addr <= HWIO_AHB2PHY_SOUTH_EUD_Q2_TUNE_ADR15_ADDR); 
      index++)
  {
    if (qusb_dci_eud_phy_reg_array[index].address == EUD_USB_PHY_EMPTY) continue;

    out_dword(addr, qusb_dci_eud_phy_reg_array[index].address);
    addr+=4;
    out_dword(addr, qusb_dci_eud_phy_reg_array[index].value);
    addr+=4;
  }

  HWIO_AHB2PHY_SOUTH_EUD_PBUS_CTRL_EN_OUTM(HWIO_AHB2PHY_SOUTH_EUD_PBUS_CTRL_EN_PBUS_CTRL_EN_0_0_BMSK,
    0x1 <<HWIO_AHB2PHY_SOUTH_EUD_PBUS_CTRL_EN_PBUS_CTRL_EN_0_0_SHFT);
}


// ===========================================================================
/**
 * @function    qusb_dci_eud_init
 * 
 * @brief       This function wirte to sw_attch bit of EUD to prevent 
 *              transition to safe mode.
 * 
 * @param   none.
 * 
 * @return  none.
 * 
 */
// ===========================================================================
void qusb_dci_eud_init(void)
{
  // Program EUD sw_attach to prevent transition to safe mode
  if (HWIO_AHB2PHY_SOUTH_EUD_SW_ATTACH_DET_INM(
        HWIO_AHB2PHY_SOUTH_EUD_SW_ATTACH_DET_SW_ATTACH_DET_0_0_BMSK))
  {
    HWIO_AHB2PHY_SOUTH_EUD_SW_ATTACH_DET_OUTM(
      HWIO_AHB2PHY_SOUTH_EUD_SW_ATTACH_DET_SW_ATTACH_DET_0_0_BMSK,
      (0x0 << HWIO_AHB2PHY_SOUTH_EUD_SW_ATTACH_DET_SW_ATTACH_DET_0_0_SHFT));
    qusb_dci_delay_ms(1);
  }
  
  HWIO_AHB2PHY_SOUTH_EUD_SW_ATTACH_DET_OUTM(
    HWIO_AHB2PHY_SOUTH_EUD_SW_ATTACH_DET_SW_ATTACH_DET_0_0_BMSK,
    (0x1 << HWIO_AHB2PHY_SOUTH_EUD_SW_ATTACH_DET_SW_ATTACH_DET_0_0_SHFT));
}

// ===========================================================================
/**
 * @function    qusb_dci_enable_usb2_ldo
 * 
 * @brief   This function will be used to turn ON the USB2 LDO
 * 
 * @param   None
 * 
 * @return  TRUE or FALSE depending on success or failure.
 * 
 */
// ===========================================================================
void qusb_dci_enable_usb2_ldo(boolean enable)
{ 
#if  !defined(BUILD_HOSTDL) && !defined(BUILD_EHOSTDL) && !defined(FEATURE_EMMCBLD)
  pm_err_flag_type  pm_err;
  pm_on_off_type    pm_on_off = PM_OFF;
  
  if (enable)
  {
    pm_on_off = PM_ON;
  }

  pm_err = PM_ERR_FLAG__SUCCESS;
#if 0 // TODO [JC] - 855
  pm_err = pm_ldo_sw_enable(QUSB_DCI_PMD8998_INDEX, PM_LDO_24, pm_on_off);
#endif

  if (PM_ERR_FLAG__SUCCESS != pm_err)
  {
    qusb_uart_log("3V LDO failed", (uint32)pm_err);
  }

  // Enable in case of debugging
  // qusb_uart_log("3V LDO", enable);
#else
  (void)enable;  // Suppress warning
#endif
}

uint32 qusb_dci_get_chip_serial_number(void)
{
  return HWIO_SERIAL_NUM_INM(HWIO_SERIAL_NUM_SERIAL_NUM_BMSK) >> HWIO_SERIAL_NUM_SERIAL_NUM_SHFT;
}

void qusb_dci_eud_update_serial_number(uint32 serial_num)
{
  // ID_0 is LSB
  HWIO_AHB2PHY_SOUTH_EUD_DEV_ID_0_OUT( (serial_num &  0x000000FF)       );
  HWIO_AHB2PHY_SOUTH_EUD_DEV_ID_1_OUT( (serial_num &  0x0000FF00) >> 8  );
  HWIO_AHB2PHY_SOUTH_EUD_DEV_ID_2_OUT( (serial_num &  0x00FF0000) >> 16 );
  HWIO_AHB2PHY_SOUTH_EUD_DEV_ID_3_OUT( (serial_num &  0xFF000000) >> 24 );
}


