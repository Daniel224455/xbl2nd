/*=======================================================================*//**
 * @file        qusb_phy_settings.c
 * @author:     kameya
 * @date        19-Jan-2016
 *
 * @brief       QUSB (Qualcomm High-Speed USB) High Speed(HS) and 
 *              Super Speed (SS) PHY settings.
 *
 * @details     This file contains settings of the HS PHY.
 *
 * @note        
 *
 * 
*//*========================================================================*/
//============================================================================
/**
  Copyright (c) 2016-2019 Qualcomm Technologies, Inc. All rights reserved.
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
// 2018-12-31	 gk		 Updated the piopu SS sequence for V2 target
// 2018-07-16    jaychoi Revert Poipu PHY tune sequence to 2.4
// 2018-07-02    kameya  Add support for SuperSpeed-Plus enumeration
// 2018-05-04    psapin  Poipu latest PHY tune sequence (secondary)
// 2018-04-25    jaychoi Poipu latest PHY tune sequence
// 2018-04-19    amitg   Poipu Multi-Port Support
// 2018-04-10    amitg   Poipu BU Fixes/Phy Tuning for Primary and Secondary
// 2017-06-06    pm      Updated SS Phy sequence for primary port from Latest HPG 
// 2017-04-26    amitg   Updated QUSB2 Phy confirguration for Host mode low speed devices
// 2017-03-30    pm      Added Configuration for Secondary Core
// 2016-01-19    kameya  First Draft
// 
// =================================================================


//----------------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------------
#include "comdef.h" // basic types as byte word uint32 etc
#include "../../Include/HalusbHWIO.h"

#include "qusb_dci_common.h"

//----------------------------------------------------------------------------
// Static Variable Definitions
//----------------------------------------------------------------------------

//============================================================================
// QUSB High-Speed Primary PHY Configuration Array
//============================================================================

ALIGN(4)
static const uint32 qusb_dci_hs_phy_cfg_address[QUSB_DCI_HS_PHY_CFG_ARRAY_ENTRY_CNT] = 
{
  //-------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR, //  4 : (0x61),
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR, //  5 : (0x43),
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR, //  6 : (0x2E),
  HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR, //  7 : (0x03),
  //-------------------------------------------------------------------------------------------
};

ALIGN(4)
static const uint8 qusb_dci_hs_phy_cfg_value[QUSB_DCI_HS_PHY_CFG_ARRAY_ENTRY_CNT] = 
{
  //-------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR, 4 */ (0x61),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR, 5 */ (0x43),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR, 6 */ (0x2E),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR, 7 */ (0x03),
  //-------------------------------------------------------------------------------------------
};

//============================================================================
// QUSB High-Speed Secondary PHY Configuration Array
//============================================================================

ALIGN(4)
static const uint32 qusb_dci_hs_phy_sec_cfg_address[QUSB_DCI_HS_PHY_SEC_CFG_ARRAY_ENTRY_CNT] = 
{
  //-------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR,  //  4 : (0x61),
  HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR,  //  5 : (0x43),
  HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR,  //  6 : (0x2E),
  HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR,  //  7 : (0x03),
  //-------------------------------------------------------------------------------------------
};

ALIGN(4)
static const uint8 qusb_dci_hs_phy_sec_cfg_value[QUSB_DCI_HS_PHY_SEC_CFG_ARRAY_ENTRY_CNT] = 
{
  //-------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR,  4 */ (0x61),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR,  5 */ (0x43),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR,  6 */ (0x2E),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR,  7 */ (0x03),
  //-------------------------------------------------------------------------------------------
};

//============================================================================
// QUSB High-Speed MultiPort 0 PHY Configuration Array
//============================================================================

ALIGN(4)
static const uint32 qusb_dci_hs_phy_MP0_cfg_address[QUSB_DCI_HS_PHY_MP0_CFG_ARRAY_ENTRY_CNT] = 
{
  //-------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB2PHY_MP0_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR,  //  4 : (0x61),
  HWIO_AHB2PHY_SOUTH_USB2PHY_MP0_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR,  //  5 : (0x43),
  HWIO_AHB2PHY_SOUTH_USB2PHY_MP0_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR,  //  6 : (0x2E),
  HWIO_AHB2PHY_SOUTH_USB2PHY_MP0_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR,  //  7 : (0x03),
  //-------------------------------------------------------------------------------------------
};

ALIGN(4)
static const uint8 qusb_dci_hs_phy_MP0_cfg_value[QUSB_DCI_HS_PHY_MP0_CFG_ARRAY_ENTRY_CNT] = 
{
  //-------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_MP0_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR,  4 */ (0x61),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_MP0_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR,  5 */ (0x43),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_MP0_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR,  6 */ (0x2E),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_MP0_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR,  7 */ (0x03),
};

//============================================================================
// QUSB High-Speed MultiPort 1 PHY Configuration Array
//============================================================================

ALIGN(4)
static const uint32 qusb_dci_hs_phy_MP1_cfg_address[QUSB_DCI_HS_PHY_MP1_CFG_ARRAY_ENTRY_CNT] = 
{
  //-------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB2PHY_MP1_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR,  //  4 : (0x61),
  HWIO_AHB2PHY_SOUTH_USB2PHY_MP1_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR,  //  5 : (0x43),
  HWIO_AHB2PHY_SOUTH_USB2PHY_MP1_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR,  //  6 : (0x2E),
  HWIO_AHB2PHY_SOUTH_USB2PHY_MP1_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR,  //  7 : (0x03),
  //-------------------------------------------------------------------------------------------
};

ALIGN(4)
static const uint8 qusb_dci_hs_phy_MP1_cfg_value[QUSB_DCI_HS_PHY_MP1_CFG_ARRAY_ENTRY_CNT] = 
{
  //-------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_MP1_USB_PHY_PARAMETER_OVERRIDE_X0_ADDR,  4 */ (0x61),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_MP1_USB_PHY_PARAMETER_OVERRIDE_X1_ADDR,  5 */ (0x43),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_MP1_USB_PHY_PARAMETER_OVERRIDE_X2_ADDR,  6 */ (0x2E),
  /* HWIO_AHB2PHY_SOUTH_USB2PHY_MP1_USB_PHY_PARAMETER_OVERRIDE_X3_ADDR,  7 */ (0x03),
  //-------------------------------------------------------------------------------------------
};

//============================================================================
// QUSB Super-Speed Primary PHY Configuration Array
//============================================================================


// Values according to SS QMP PHY HPG version - cm_usb3_dp_con_USB3_Config_for_Hana_Poipu_Kona_1.06
ALIGN(4)
static const uint32 qusb_dci_ss_phy_cfg_address_common[QUSB_DCI_SS_PHY_COMMON_CFG_ARRAY_ENTRY_CNT] =
{
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_POWER_DOWN_CTRL_ADDR,                    //   0    :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_POWER_DOWN_CONTROL_ADDR,                    //   1    :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_EN_CENTER_ADDR,                 //   2    :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_PER1_ADDR,                      //   3    :    (0x31)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_PER2_ADDR,                      //   4    :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_STEP_SIZE1_MODE0_ADDR,          //   5    :    (0xDE)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_STEP_SIZE2_MODE0_ADDR,          //   6    :    (0x07)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_STEP_SIZE1_MODE1_ADDR,          //   7    :    (0xDE)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_STEP_SIZE2_MODE1_ADDR,          //   8    :    (0x07)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,             //   9    :    (0x0A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_CMN_IPTRIM_ADDR,                    //   10   :    (0x20)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_CP_CTRL_MODE0_ADDR,                 //   11   :    (0x06)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_CP_CTRL_MODE1_ADDR,                 //   12   :    (0x06)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_PLL_RCTRL_MODE0_ADDR,               //   13   :    (0x16)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_PLL_RCTRL_MODE1_ADDR,               //   14   :    (0x16)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_PLL_CCTRL_MODE0_ADDR,               //   15   :    (0x36)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_PLL_CCTRL_MODE1_ADDR,               //   16   :    (0x36)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SYSCLK_EN_SEL_ADDR,                 //   17   :    (0x1A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP_EN_ADDR,                   //   18   :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP1_MODE0_ADDR,               //   19   :    (0x14)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP2_MODE0_ADDR,               //   20   :    (0x34)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP1_MODE1_ADDR,               //   21   :    (0x34)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP2_MODE1_ADDR,               //   22   :    (0x82)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DEC_START_MODE0_ADDR,               //   23   :    (0x82)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DEC_START_MODE1_ADDR,               //   24   :    (0x82)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,         //   25   :    (0xAB)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,         //   26   :    (0xEA)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,         //   27   :    (0x02)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START1_MODE1_ADDR,         //   28   :    (0xAB)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START2_MODE1_ADDR,         //   29   :    (0xEA)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START3_MODE1_ADDR,         //   30   :    (0x02)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_VCO_TUNE_MAP_ADDR,                  //   31   :    (0x02)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_VCO_TUNE1_MODE0_ADDR,               //   32   :    (0x24)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_VCO_TUNE1_MODE1_ADDR,               //   33   :    (0x24)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_VCO_TUNE2_MODE1_ADDR,               //   34   :    (0x02)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_HSCLK_SEL_ADDR,                     //   35   :    (0x01)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_CORECLK_DIV_MODE1_ADDR,             //   36   :    (0x08)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0_ADDR,    //   37   :    (0xCA)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0_ADDR,    //   38   :    (0x1E)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1_ADDR,    //   39   :    (0xCA)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1_ADDR,    //   40   :    (0x1E)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_HSCLK_SEL_ADDR,          //   41   :    (0x11)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RES_CODE_LANE_TX_ADDR,              //   42   :    (0x60)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RES_CODE_LANE_RX_ADDR,              //   43   :    (0x60)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RES_CODE_LANE_OFFSET_TX_ADDR,       //   44   :    (0x15)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RES_CODE_LANE_OFFSET_RX_ADDR,       //   45   :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_LANE_MODE_1_ADDR,                   //   46   :    (0xD5)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_LANE_MODE_2_ADDR,                   //   47   :    (0xC2)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RCV_DETECT_LVL_2_ADDR,              //   48   :    (0x12)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_PI_QEC_CTRL_ADDR,                   //   49   :    (0x20)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SO_GAIN_ADDR,                  //   50   :    (0x06)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_FASTLOCK_FO_GAIN_ADDR,         //   51   :    (0x2F)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SO_SATURATION_AND_ENABLE_ADDR,  //   52   :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_FASTLOCK_COUNT_LOW_ADDR,       //   53   :    (0xFF)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_FASTLOCK_COUNT_HIGH_ADDR,      //   54   :    (0x0F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_PI_CONTROLS_ADDR,              //   55   :    (0x99)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SB2_THRESH1_ADDR,              //   56   :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SB2_THRESH2_ADDR,              //   57   :    (0x08)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SB2_GAIN1_ADDR,                //   58   :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SB2_GAIN2_ADDR,                //   59   :    (0x05)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_VGA_CAL_CNTRL1_ADDR,                //   60   :    (0x54)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_VGA_CAL_CNTRL2_ADDR,                //   61   :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL2_ADDR,         //   62   :    (0x0F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL3_ADDR,         //   63   :    (0x4A)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL4_ADDR,         //   64   :    (0x0A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_IDAC_TSETTLE_LOW_ADDR,           //   65   :    (0xC0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_IDAC_TSETTLE_HIGH_ADDR,          //   66   :    (0x00)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR,   //   67   :    (0x77)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_SIGDET_CNTRL_ADDR,                  //   68   :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_SIGDET_DEGLITCH_CNTRL_ADDR,         //   69   :    (0x0E)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_LOW_ADDR,                //   70   :    (0xBF)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_HIGH_ADDR,               //   71   :    (0xBF)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_HIGH2_ADDR,              //   72   :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_HIGH3_ADDR,              //   73   :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_HIGH4_ADDR,              //   74   :    (0xA8)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_LOW_ADDR,                //   75   :    (0xC9)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_HIGH_ADDR,               //   76   :    (0xC9)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_HIGH2_ADDR,              //   77   :    (0x39)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_HIGH3_ADDR,              //   78   :    (0x7B)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_HIGH4_ADDR,              //   79   :    (0xB3)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_DFE_EN_TIMER_ADDR,                  //   80   :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_DFE_CTLE_POST_CAL_OFFSET_ADDR,      //   81   :    (0x38)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_AUX_DATA_TCOARSE_TFINE_ADDR,        //   82   :    (0xA0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_DCC_CTRL1_ADDR,                     //   83   :    (0x0C)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_GM_CAL_ADDR,                        //   84   :    (0x1F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_VTH_CODE_ADDR,                      //   85   :    (0x10)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RES_CODE_LANE_TX_ADDR,              //   86   :    (0x60)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RES_CODE_LANE_RX_ADDR,              //   87   :    (0x60)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RES_CODE_LANE_OFFSET_TX_ADDR,       //   88   :    (0x1F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RES_CODE_LANE_OFFSET_RX_ADDR,       //   89   :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_LANE_MODE_1_ADDR,                   //   90   :    (0xD5)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_LANE_MODE_2_ADDR,                   //   91   :    (0xC2)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RCV_DETECT_LVL_2_ADDR,              //   92   :    (0x12)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_PI_QEC_CTRL_ADDR,                   //   93   :    (0x20)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SO_GAIN_ADDR,                  //   94   :    (0x06)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_FASTLOCK_FO_GAIN_ADDR,         //   95   :    (0x2F)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SO_SATURATION_AND_ENABLE_ADDR,  //   96   :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_FASTLOCK_COUNT_LOW_ADDR,       //   97   :    (0xFF)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_FASTLOCK_COUNT_HIGH_ADDR,      //   98   :    (0x0F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_PI_CONTROLS_ADDR,              //   99   :    (0x99)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SB2_THRESH1_ADDR,              //   100  :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SB2_THRESH2_ADDR,              //   101  :    (0x08)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SB2_GAIN1_ADDR,                //   102  :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SB2_GAIN2_ADDR,                //   103  :    (0x05)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_VGA_CAL_CNTRL1_ADDR,                //   104  :    (0x54)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_VGA_CAL_CNTRL2_ADDR,                //   105  :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL2_ADDR,         //   106  :    (0x0F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL3_ADDR,         //   107  :    (0x4A)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL4_ADDR,         //   108  :    (0x0A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_IDAC_TSETTLE_LOW_ADDR,           //   109  :    (0xC0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_IDAC_TSETTLE_HIGH_ADDR,          //   110  :    (0x00)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR,   //   111  :    (0x77)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_SIGDET_CNTRL_ADDR,                  //   112  :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_SIGDET_DEGLITCH_CNTRL_ADDR,         //   113  :    (0x0E)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_LOW_ADDR,                //   114  :    (0xBF)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_HIGH_ADDR,               //   115  :    (0xBF)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_HIGH2_ADDR,              //   116  :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_HIGH3_ADDR,              //   117  :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_HIGH4_ADDR,              //   118  :    (0xA8)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_LOW_ADDR,                //   119  :    (0xC9)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_HIGH_ADDR,               //   120  :    (0xC9)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_HIGH2_ADDR,              //   121  :    (0x39)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_HIGH3_ADDR,              //   122  :    (0x7B)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_HIGH4_ADDR,              //   123  :    (0xB3)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_DFE_EN_TIMER_ADDR,                  //   124  :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_DFE_CTLE_POST_CAL_OFFSET_ADDR,      //   125  :    (0x38)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_AUX_DATA_TCOARSE_TFINE_ADDR,        //   126  :    (0xA0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_DCC_CTRL1_ADDR,                     //   127  :    (0x0C)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_GM_CAL_ADDR,                        //   128  :    (0x1F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_VTH_CODE_ADDR,                      //   129  :    (0x10)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_LOCK_DETECT_CONFIG1_ADDR,                   //   130  :    (0xD0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_LOCK_DETECT_CONFIG2_ADDR,                   //   131  :    (0x07)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_LOCK_DETECT_CONFIG3_ADDR,                   //   132  :    (0x20)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_LOCK_DETECT_CONFIG6_ADDR,                   //   133  :    (0x13)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_REFGEN_REQ_CONFIG1_ADDR,                    //   134  :    (0x21)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_RX_SIGDET_LVL_ADDR,                         //   135  :    (0xAA)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_CDR_RESET_TIME_ADDR,                        //   136  :    (0x0A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_ALIGN_DETECT_CONFIG1_ADDR,                  //   137  :    (0x88)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_ALIGN_DETECT_CONFIG2_ADDR,                  //   138  :    (0x13)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_PCS_TX_RX_CONFIG_ADDR,                      //   139  :    (0x0C)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_EQ_CONFIG1_ADDR,                            //   140  :    (0x4B)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_EQ_CONFIG5_ADDR,                            //   141  :    (0x10)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL_ADDR,          //   142  :    (0xF8)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_USB3_RXEQTRAINING_DFE_TIME_S2_ADDR,         //   143  :    (0x07)
};

ALIGN(4)
static const uint8 qusb_dci_ss_phy_cfg_value_common[QUSB_DCI_SS_PHY_COMMON_CFG_ARRAY_ENTRY_CNT] =
{
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_DP_COM_POWER_DOWN_CTRL,                          0    */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_POWER_DOWN_CONTROL,                          1    */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_EN_CENTER,                       2    */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_PER1,                            3    */   (0x31),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_PER2,                            4    */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_STEP_SIZE1_MODE0,                5    */   (0xDE),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_STEP_SIZE2_MODE0,                6    */   (0x07),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_STEP_SIZE1_MODE1,                7    */   (0xDE),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SSC_STEP_SIZE2_MODE1,                8    */   (0x07),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SYSCLK_BUF_ENABLE,                   9    */   (0x0A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_CMN_IPTRIM,                          10   */   (0x20),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_CP_CTRL_MODE0,                       11   */   (0x06),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_CP_CTRL_MODE1,                       12   */   (0x06),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_PLL_RCTRL_MODE0,                     13   */   (0x16),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_PLL_RCTRL_MODE1,                     14   */   (0x16),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_PLL_CCTRL_MODE0,                     15   */   (0x36),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_PLL_CCTRL_MODE1,                     16   */   (0x36),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_SYSCLK_EN_SEL,                       17   */   (0x1A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP_EN,                         18   */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP1_MODE0,                     19   */   (0x14),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP2_MODE0,                     20   */   (0x34),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP1_MODE1,                     21   */   (0x34),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_LOCK_CMP2_MODE1,                     22   */   (0x82),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DEC_START_MODE0,                     23   */   (0x82),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DEC_START_MODE1,                     24   */   (0x82),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START1_MODE0,               25   */   (0xAB),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START2_MODE0,               26   */   (0xEA),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START3_MODE0,               27   */   (0x02),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START1_MODE1,               28   */   (0xAB),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START2_MODE1,               29   */   (0xEA),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_DIV_FRAC_START3_MODE1,               30   */   (0x02),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_VCO_TUNE_MAP,                        31   */   (0x02),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_VCO_TUNE1_MODE0,                     32   */   (0x24),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_VCO_TUNE1_MODE1,                     33   */   (0x24),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_VCO_TUNE2_MODE1,                     34   */   (0x02),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_HSCLK_SEL,                           35   */   (0x01),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_CORECLK_DIV_MODE1,                   36   */   (0x08),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0,          37   */   (0xCA),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0,          38   */   (0x1E),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1,          39   */   (0xCA),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1,          40   */   (0x1E),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_COM_BIN_VCOCAL_HSCLK_SEL,                41   */   (0x11),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RES_CODE_LANE_TX,                    42   */   (0x60),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RES_CODE_LANE_RX,                    43   */   (0x60),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RES_CODE_LANE_OFFSET_TX,             44   */   (0x15),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RES_CODE_LANE_OFFSET_RX,             45   */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_LANE_MODE_1,                         46   */   (0xD5),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_LANE_MODE_2,                         47   */   (0xC2),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_RCV_DETECT_LVL_2,                    48   */   (0x12),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXA_PI_QEC_CTRL,                         49   */   (0x20),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SO_GAIN,                        50   */   (0x06),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_FASTLOCK_FO_GAIN,               51   */   (0x2F),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SO_SATURATION_AND_ENABLE,       52   */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_FASTLOCK_COUNT_LOW,             53   */   (0xFF),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_FASTLOCK_COUNT_HIGH,            54   */   (0x0F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_PI_CONTROLS,                    55   */   (0x99),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SB2_THRESH1,                    56   */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SB2_THRESH2,                    57   */   (0x08),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SB2_GAIN1,                      58   */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_UCDR_SB2_GAIN2,                      59   */   (0x05),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_VGA_CAL_CNTRL1,                      60   */   (0x54),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_VGA_CAL_CNTRL2,                      61   */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL2,               62   */   (0x0F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL3,               63   */   (0x4A),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL4,               64   */   (0x0A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_IDAC_TSETTLE_LOW,                 65   */   (0xC0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_IDAC_TSETTLE_HIGH,                66   */   (0x00),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_EQ_OFFSET_ADAPTOR_CNTRL1,         67   */   (0x77),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_SIGDET_CNTRL,                        68   */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_SIGDET_DEGLITCH_CNTRL,               69   */   (0x0E),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_LOW,                      70   */   (0xBF),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_HIGH,                     71   */   (0xBF),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_HIGH2,                    72   */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_HIGH3,                    73   */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_00_HIGH4,                    74   */   (0xA8),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_LOW,                      75   */   (0xC9),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_HIGH,                     76   */   (0xC9),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_HIGH2,                    77   */   (0x39),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_HIGH3,                    78   */   (0x7B),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_RX_MODE_01_HIGH4,                    79   */   (0xB3),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_DFE_EN_TIMER,                        80   */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_DFE_CTLE_POST_CAL_OFFSET,            81   */   (0x38),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_AUX_DATA_TCOARSE_TFINE,              82   */   (0xA0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_DCC_CTRL1,                           83   */   (0x0C),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_GM_CAL,                              84   */   (0x1F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXA_VTH_CODE,                            85   */   (0x10),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RES_CODE_LANE_TX,                    86   */   (0x60),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RES_CODE_LANE_RX,                    87   */   (0x60),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RES_CODE_LANE_OFFSET_TX,             88   */   (0x1F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RES_CODE_LANE_OFFSET_RX,             89   */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_LANE_MODE_1,                         90   */   (0xD5),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_LANE_MODE_2,                         91   */   (0xC2),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_RCV_DETECT_LVL_2,                    92   */   (0x12),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_TXB_PI_QEC_CTRL,                         93   */   (0x20),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SO_GAIN,                        94   */   (0x06),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_FASTLOCK_FO_GAIN,               95   */   (0x2F),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SO_SATURATION_AND_ENABLE,       96   */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_FASTLOCK_COUNT_LOW,             97   */   (0xFF),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_FASTLOCK_COUNT_HIGH,            98   */   (0x0F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_PI_CONTROLS,                    99   */   (0x99),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SB2_THRESH1,                    100  */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SB2_THRESH2,                    101  */   (0x08),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SB2_GAIN1,                      102  */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_UCDR_SB2_GAIN2,                      103  */   (0x05),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_VGA_CAL_CNTRL1,                      104  */   (0x54),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_VGA_CAL_CNTRL2,                      105  */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL2,               106  */   (0x0F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL3,               107  */   (0x4A),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL4,               108  */   (0x0A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_IDAC_TSETTLE_LOW,                 109  */   (0xC0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_IDAC_TSETTLE_HIGH,                110  */   (0x00),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_EQ_OFFSET_ADAPTOR_CNTRL1,         111  */   (0x77),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_SIGDET_CNTRL,                        112  */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_SIGDET_DEGLITCH_CNTRL,               113  */   (0x0E),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_LOW,                      114  */   (0xBF),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_HIGH,                     115  */   (0xBF),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_HIGH2,                    116  */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_HIGH3,                    117  */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_00_HIGH4,                    118  */   (0xA8),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_LOW,                      119  */   (0xC9),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_HIGH,                     120  */   (0xC9),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_HIGH2,                    121  */   (0x39),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_HIGH3,                    122  */   (0x7B),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_RX_MODE_01_HIGH4,                    123  */   (0xB3),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_DFE_EN_TIMER,                        124  */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_DFE_CTLE_POST_CAL_OFFSET,            125  */   (0x38),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_AUX_DATA_TCOARSE_TFINE,              126  */   (0xA0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_DCC_CTRL1,                           127  */   (0x0C),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_GM_CAL,                              128  */   (0x1F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_QSERDES_RXB_VTH_CODE,                            129  */   (0x10),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_LOCK_DETECT_CONFIG1,                         130  */   (0xD0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_LOCK_DETECT_CONFIG2,                         131  */   (0x07),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_LOCK_DETECT_CONFIG3,                         132  */   (0x20),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_LOCK_DETECT_CONFIG6,                         133  */   (0x13),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_REFGEN_REQ_CONFIG1,                          134  */   (0x21),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_RX_SIGDET_LVL,                               135  */   (0xAA),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_CDR_RESET_TIME,                              136  */   (0x0A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_ALIGN_DETECT_CONFIG1,                        137  */   (0x88),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_ALIGN_DETECT_CONFIG2,                        138  */   (0x13),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_PCS_TX_RX_CONFIG,                            139  */   (0x0C),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_EQ_CONFIG1,                                  140  */   (0x4B),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_EQ_CONFIG5,                                  141  */   (0x10),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL,                142  */   (0xF8),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_USB3_PCS_USB3_RXEQTRAINING_DFE_TIME_S2,               143  */   (0x07),
};

//============================================================================
// QUSB Super-Speed Secondary PHY Configuration Array
//============================================================================

ALIGN(4)
static const uint32 qusb_dci_ss_phy_sec_cfg_address_common[QUSB_DCI_SS_PHY_COMMON_CFG_ARRAY_ENTRY_CNT] =
{
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_DP_COM_POWER_DOWN_CTRL_ADDR,                    //   0    :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_POWER_DOWN_CONTROL_ADDR,                    //   1    :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_EN_CENTER_ADDR,                 //   2    :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_PER1_ADDR,                      //   3    :    (0x31)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_PER2_ADDR,                      //   4    :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_STEP_SIZE1_MODE0_ADDR,          //   5    :    (0xDE)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_STEP_SIZE2_MODE0_ADDR,          //   6    :    (0x07)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_STEP_SIZE1_MODE1_ADDR,          //   7    :    (0xDE)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_STEP_SIZE2_MODE1_ADDR,          //   8    :    (0x07)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,             //   9    :    (0x0A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_CMN_IPTRIM_ADDR,                    //   10   :    (0x20)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_CP_CTRL_MODE0_ADDR,                 //   11   :    (0x06)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_CP_CTRL_MODE1_ADDR,                 //   12   :    (0x06)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_PLL_RCTRL_MODE0_ADDR,               //   13   :    (0x16)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_PLL_RCTRL_MODE1_ADDR,               //   14   :    (0x16)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_PLL_CCTRL_MODE0_ADDR,               //   15   :    (0x36)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_PLL_CCTRL_MODE1_ADDR,               //   16   :    (0x36)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SYSCLK_EN_SEL_ADDR,                 //   17   :    (0x1A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP_EN_ADDR,                   //   18   :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP1_MODE0_ADDR,               //   19   :    (0x14)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP2_MODE0_ADDR,               //   20   :    (0x34)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP1_MODE1_ADDR,               //   21   :    (0x34)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP2_MODE1_ADDR,               //   22   :    (0x82)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DEC_START_MODE0_ADDR,               //   23   :    (0x82)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DEC_START_MODE1_ADDR,               //   24   :    (0x82)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,         //   25   :    (0xAB)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,         //   26   :    (0xEA)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,         //   27   :    (0x02)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START1_MODE1_ADDR,         //   28   :    (0xAB)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START2_MODE1_ADDR,         //   29   :    (0xEA)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START3_MODE1_ADDR,         //   30   :    (0x02)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_VCO_TUNE_MAP_ADDR,                  //   31   :    (0x02)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_VCO_TUNE1_MODE0_ADDR,               //   32   :    (0x24)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_VCO_TUNE1_MODE1_ADDR,               //   33   :    (0x24)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_VCO_TUNE2_MODE1_ADDR,               //   34   :    (0x02)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_HSCLK_SEL_ADDR,                     //   35   :    (0x01)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_CORECLK_DIV_MODE1_ADDR,             //   36   :    (0x08)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0_ADDR,    //   37   :    (0xCA)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0_ADDR,    //   38   :    (0x1E)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1_ADDR,    //   39   :    (0xCA)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1_ADDR,    //   40   :    (0x1E)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_HSCLK_SEL_ADDR,          //   41   :    (0x11)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RES_CODE_LANE_TX_ADDR,              //   42   :    (0x60)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RES_CODE_LANE_RX_ADDR,              //   43   :    (0x60)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RES_CODE_LANE_OFFSET_TX_ADDR,       //   44   :    (0x15)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RES_CODE_LANE_OFFSET_RX_ADDR,       //   45   :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_LANE_MODE_1_ADDR,                   //   46   :    (0xD5)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_LANE_MODE_2_ADDR,                   //   47   :    (0xC2)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RCV_DETECT_LVL_2_ADDR,              //   48   :    (0x12)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_PI_QEC_CTRL_ADDR,                   //   49   :    (0x20)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SO_GAIN_ADDR,                  //   50   :    (0x06)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_FASTLOCK_FO_GAIN_ADDR,         //   51   :    (0x2F)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SO_SATURATION_AND_ENABLE_ADDR,  //   52   :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_FASTLOCK_COUNT_LOW_ADDR,       //   53   :    (0xFF)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_FASTLOCK_COUNT_HIGH_ADDR,      //   54   :    (0x0F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_PI_CONTROLS_ADDR,              //   55   :    (0x99)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SB2_THRESH1_ADDR,              //   56   :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SB2_THRESH2_ADDR,              //   57   :    (0x08)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SB2_GAIN1_ADDR,                //   58   :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SB2_GAIN2_ADDR,                //   59   :    (0x05)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_VGA_CAL_CNTRL1_ADDR,                //   60   :    (0x54)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_VGA_CAL_CNTRL2_ADDR,                //   61   :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL2_ADDR,         //   62   :    (0x0F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL3_ADDR,         //   63   :    (0x4A)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL4_ADDR,         //   64   :    (0x0A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_IDAC_TSETTLE_LOW_ADDR,           //   65   :    (0xC0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_IDAC_TSETTLE_HIGH_ADDR,          //   66   :    (0x00)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR,   //   67   :    (0x77)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_SIGDET_CNTRL_ADDR,                  //   68   :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_SIGDET_DEGLITCH_CNTRL_ADDR,         //   69   :    (0x0E)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_LOW_ADDR,                //   70   :    (0xBF)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_HIGH_ADDR,               //   71   :    (0xBF)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_HIGH2_ADDR,              //   72   :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_HIGH3_ADDR,              //   73   :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_HIGH4_ADDR,              //   74   :    (0xA8)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_LOW_ADDR,                //   75   :    (0xC9)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_HIGH_ADDR,               //   76   :    (0xC9)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_HIGH2_ADDR,              //   77   :    (0x39)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_HIGH3_ADDR,              //   78   :    (0x7B)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_HIGH4_ADDR,              //   79   :    (0xB3)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_DFE_EN_TIMER_ADDR,                  //   80   :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_DFE_CTLE_POST_CAL_OFFSET_ADDR,      //   81   :    (0x38)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_AUX_DATA_TCOARSE_TFINE_ADDR,        //   82   :    (0xA0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_DCC_CTRL1_ADDR,                     //   83   :    (0x0C)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_GM_CAL_ADDR,                        //   84   :    (0x1F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_VTH_CODE_ADDR,                      //   85   :    (0x10)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RES_CODE_LANE_TX_ADDR,              //   86   :    (0x60)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RES_CODE_LANE_RX_ADDR,              //   87   :    (0x60)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RES_CODE_LANE_OFFSET_TX_ADDR,       //   88   :    (0x1F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RES_CODE_LANE_OFFSET_RX_ADDR,       //   89   :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_LANE_MODE_1_ADDR,                   //   90   :    (0xD5)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_LANE_MODE_2_ADDR,                   //   91   :    (0xC2)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RCV_DETECT_LVL_2_ADDR,              //   92   :    (0x12)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_PI_QEC_CTRL_ADDR,                   //   93   :    (0x01)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SO_GAIN_ADDR,                  //   94   :    (0x06)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_FASTLOCK_FO_GAIN_ADDR,         //   95   :    (0x2F)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SO_SATURATION_AND_ENABLE_ADDR,  //   96   :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_FASTLOCK_COUNT_LOW_ADDR,       //   97   :    (0xFF)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_FASTLOCK_COUNT_HIGH_ADDR,      //   98   :    (0x0F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_PI_CONTROLS_ADDR,              //   99   :    (0x99)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SB2_THRESH1_ADDR,              //   100  :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SB2_THRESH2_ADDR,              //   101  :    (0x08)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SB2_GAIN1_ADDR,                //   102  :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SB2_GAIN2_ADDR,                //   103  :    (0x05)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_VGA_CAL_CNTRL1_ADDR,                //   104  :    (0x54)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_VGA_CAL_CNTRL2_ADDR,                //   105  :    (0x05)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL2_ADDR,         //   106  :    (0x0F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL3_ADDR,         //   107  :    (0x4A)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL4_ADDR,         //   108  :    (0x0A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_IDAC_TSETTLE_LOW_ADDR,           //   109  :    (0xC0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_IDAC_TSETTLE_HIGH_ADDR,          //   110  :    (0x00)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR,   //   111  :    (0x77)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_SIGDET_CNTRL_ADDR,                  //   112  :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_SIGDET_DEGLITCH_CNTRL_ADDR,         //   113  :    (0x0E)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_LOW_ADDR,                //   114  :    (0xBF)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_HIGH_ADDR,               //   115  :    (0xBF)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_HIGH2_ADDR,              //   116  :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_HIGH3_ADDR,              //   117  :    (0x7F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_HIGH4_ADDR,              //   118  :    (0xA8)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_LOW_ADDR,                //   119  :    (0xC9)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_HIGH_ADDR,               //   120  :    (0xC9)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_HIGH2_ADDR,              //   121  :    (0x39)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_HIGH3_ADDR,              //   122  :    (0x7B)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_HIGH4_ADDR,              //   123  :    (0xB3)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_DFE_EN_TIMER_ADDR,                  //   124  :    (0x04)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_DFE_CTLE_POST_CAL_OFFSET_ADDR,      //   125  :    (0x38)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_AUX_DATA_TCOARSE_TFINE_ADDR,        //   126  :    (0xA0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_DCC_CTRL1_ADDR,                     //   127  :    (0x0C)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_GM_CAL_ADDR,                        //   128  :    (0x1F)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_VTH_CODE_ADDR,                      //   129  :    (0x10)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_LOCK_DETECT_CONFIG1_ADDR,                   //   130  :    (0xD0)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_LOCK_DETECT_CONFIG2_ADDR,                   //   131  :    (0x07)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_LOCK_DETECT_CONFIG3_ADDR,                   //   132  :    (0x20)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_LOCK_DETECT_CONFIG6_ADDR,                   //   133  :    (0x13)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_REFGEN_REQ_CONFIG1_ADDR,                    //   134  :    (0x21)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_RX_SIGDET_LVL_ADDR,                         //   135  :    (0xAA)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_CDR_RESET_TIME_ADDR,                        //   136  :    (0x0A)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_ALIGN_DETECT_CONFIG1_ADDR,                  //   137  :    (0x88)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_ALIGN_DETECT_CONFIG2_ADDR,                  //   138  :    (0x13)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_PCS_TX_RX_CONFIG_ADDR,                      //   139  :    (0x0C)
  //------------------------------------------------------------------------------------------------
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_EQ_CONFIG1_ADDR,                            //   140  :    (0x4B)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_EQ_CONFIG5_ADDR,                            //   141  :    (0x10)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL_ADDR,          //   142  :    (0xF8)
  HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_USB3_RXEQTRAINING_DFE_TIME_S2_ADDR,         //   143  :    (0x07)
};

ALIGN(4)
static const uint8 qusb_dci_ss_phy_sec_cfg_value_common[QUSB_DCI_SS_PHY_COMMON_CFG_ARRAY_ENTRY_CNT] =
{
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_DP_COM_POWER_DOWN_CTRL,                          0    */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_POWER_DOWN_CONTROL,                          1    */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_EN_CENTER,                       2    */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_PER1,                            3    */   (0x31),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_PER2,                            4    */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_STEP_SIZE1_MODE0,                5    */   (0xDE),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_STEP_SIZE2_MODE0,                6    */   (0x07),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_STEP_SIZE1_MODE1,                7    */   (0xDE),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SSC_STEP_SIZE2_MODE1,                8    */   (0x07),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SYSCLK_BUF_ENABLE,                   9    */   (0x0A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_CMN_IPTRIM,                          10   */   (0x20),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_CP_CTRL_MODE0,                       11   */   (0x06),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_CP_CTRL_MODE1,                       12   */   (0x06),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_PLL_RCTRL_MODE0,                     13   */   (0x16),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_PLL_RCTRL_MODE1,                     14   */   (0x16),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_PLL_CCTRL_MODE0,                     15   */   (0x36),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_PLL_CCTRL_MODE1,                     16   */   (0x36),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_SYSCLK_EN_SEL,                       17   */   (0x1A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP_EN,                         18   */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP1_MODE0,                     19   */   (0x14),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP2_MODE0,                     20   */   (0x34),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP1_MODE1,                     21   */   (0x34),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_LOCK_CMP2_MODE1,                     22   */   (0x82),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DEC_START_MODE0,                     23   */   (0x82),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DEC_START_MODE1,                     24   */   (0x82),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START1_MODE0,               25   */   (0xAB),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START2_MODE0,               26   */   (0xEA),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START3_MODE0,               27   */   (0x02),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START1_MODE1,               28   */   (0xAB),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START2_MODE1,               29   */   (0xEA),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_DIV_FRAC_START3_MODE1,               30   */   (0x02),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_VCO_TUNE_MAP,                        31   */   (0x02),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_VCO_TUNE1_MODE0,                     32   */   (0x24),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_VCO_TUNE1_MODE1,                     33   */   (0x24),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_VCO_TUNE2_MODE1,                     34   */   (0x02),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_HSCLK_SEL,                           35   */   (0x01),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_CORECLK_DIV_MODE1,                   36   */   (0x08),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0,          37   */   (0xCA),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0,          38   */   (0x1E),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1,          39   */   (0xCA),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1,          40   */   (0x1E),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_COM_BIN_VCOCAL_HSCLK_SEL,                41   */   (0x11),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RES_CODE_LANE_TX,                    42   */   (0x60),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RES_CODE_LANE_RX,                    43   */   (0x60),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RES_CODE_LANE_OFFSET_TX,             44   */   (0x15),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RES_CODE_LANE_OFFSET_RX,             45   */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_LANE_MODE_1,                         46   */   (0xD5),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_LANE_MODE_2,                         47   */   (0xC2),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_RCV_DETECT_LVL_2,                    48   */   (0x12),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXA_PI_QEC_CTRL,                         49   */   (0x20),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SO_GAIN,                        50   */   (0x06),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_FASTLOCK_FO_GAIN,               51   */   (0x2F),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SO_SATURATION_AND_ENABLE,       52   */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_FASTLOCK_COUNT_LOW,             53   */   (0xFF),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_FASTLOCK_COUNT_HIGH,            54   */   (0x0F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_PI_CONTROLS,                    55   */   (0x99),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SB2_THRESH1,                    56   */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SB2_THRESH2,                    57   */   (0x08),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SB2_GAIN1,                      58   */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_UCDR_SB2_GAIN2,                      59   */   (0x05),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_VGA_CAL_CNTRL1,                      60   */   (0x54),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_VGA_CAL_CNTRL2,                      61   */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL2,               62   */   (0x0F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL3,               63   */   (0x4A),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_EQU_ADAPTOR_CNTRL4,               64   */   (0x0A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_IDAC_TSETTLE_LOW,                 65   */   (0xC0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_IDAC_TSETTLE_HIGH,                66   */   (0x00),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_EQ_OFFSET_ADAPTOR_CNTRL1,         67   */   (0x77),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_SIGDET_CNTRL,                        68   */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_SIGDET_DEGLITCH_CNTRL,               69   */   (0x0E),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_LOW,                      70   */   (0xBF),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_HIGH,                     71   */   (0xBF),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_HIGH2,                    72   */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_HIGH3,                    73   */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_00_HIGH4,                    74   */   (0xA8),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_LOW,                      75   */   (0xC9),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_HIGH,                     76   */   (0xC9),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_HIGH2,                    77   */   (0x39),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_HIGH3,                    78   */   (0x7B),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_RX_MODE_01_HIGH4,                    79   */   (0xB3),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_DFE_EN_TIMER,                        80   */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_DFE_CTLE_POST_CAL_OFFSET,            81   */   (0x38),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_AUX_DATA_TCOARSE_TFINE,              82   */   (0xA0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_DCC_CTRL1,                           83   */   (0x0C),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_GM_CAL,                              84   */   (0x1F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXA_VTH_CODE,                            85   */   (0x10),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RES_CODE_LANE_TX,                    86   */   (0x60),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RES_CODE_LANE_RX,                    87   */   (0x60),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RES_CODE_LANE_OFFSET_TX,             88   */   (0x1F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RES_CODE_LANE_OFFSET_RX,             89   */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_LANE_MODE_1,                         90   */   (0xD5),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_LANE_MODE_2,                         91   */   (0xC2),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_RCV_DETECT_LVL_2,                    92   */   (0x12),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_TXB_PI_QEC_CTRL,                         93   */   (0x01),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SO_GAIN,                        94   */   (0x06),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_FASTLOCK_FO_GAIN,               95   */   (0x2F),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SO_SATURATION_AND_ENABLE,       96   */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_FASTLOCK_COUNT_LOW,             97   */   (0xFF),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_FASTLOCK_COUNT_HIGH,            98   */   (0x0F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_PI_CONTROLS,                    99   */   (0x99),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SB2_THRESH1,                    100  */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SB2_THRESH2,                    101  */   (0x08),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SB2_GAIN1,                      102  */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_UCDR_SB2_GAIN2,                      103  */   (0x05),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_VGA_CAL_CNTRL1,                      104  */   (0x54),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_VGA_CAL_CNTRL2,                      105  */   (0x05),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL2,               106  */   (0x0F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL3,               107  */   (0x4A),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_EQU_ADAPTOR_CNTRL4,               108  */   (0x0A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_IDAC_TSETTLE_LOW,                 109  */   (0xC0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_IDAC_TSETTLE_HIGH,                110  */   (0x00),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_EQ_OFFSET_ADAPTOR_CNTRL1,         111  */   (0x77),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_SIGDET_CNTRL,                        112  */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_SIGDET_DEGLITCH_CNTRL,               113  */   (0x0E),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_LOW,                      114  */   (0xBF),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_HIGH,                     115  */   (0xBF),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_HIGH2,                    116  */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_HIGH3,                    117  */   (0x7F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_00_HIGH4,                    118  */   (0xA8),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_LOW,                      119  */   (0xC9),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_HIGH,                     120  */   (0xC9),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_HIGH2,                    121  */   (0x39),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_HIGH3,                    122  */   (0x7B),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_RX_MODE_01_HIGH4,                    123  */   (0xB3),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_DFE_EN_TIMER,                        124  */   (0x04),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_DFE_CTLE_POST_CAL_OFFSET,            125  */   (0x38),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_AUX_DATA_TCOARSE_TFINE,              126  */   (0xA0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_DCC_CTRL1,                           127  */   (0x0C),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_GM_CAL,                              128  */   (0x1F),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_QSERDES_RXB_VTH_CODE,                            129  */   (0x10),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_LOCK_DETECT_CONFIG1,                         130  */   (0xD0),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_LOCK_DETECT_CONFIG2,                         131  */   (0x07),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_LOCK_DETECT_CONFIG3,                         132  */   (0x20),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_LOCK_DETECT_CONFIG6,                         133  */   (0x13),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_REFGEN_REQ_CONFIG1,                          134  */   (0x21),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_RX_SIGDET_LVL,                               135  */   (0xAA),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_CDR_RESET_TIME,                              136  */   (0x0A),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_ALIGN_DETECT_CONFIG1,                        137  */   (0x88),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_ALIGN_DETECT_CONFIG2,                        138  */   (0x13),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_PCS_TX_RX_CONFIG,                            139  */   (0x0C),
  //------------------------------------------------------------------------------------------------
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_EQ_CONFIG1,                                  140  */   (0x4B),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_EQ_CONFIG5,                                  141  */   (0x10),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL,                142  */   (0xF8),
  /* HWIO_AHB2PHY_SOUTH_USB3_DP_PHY_SECUSB3_PCS_USB3_RXEQTRAINING_DFE_TIME_S2,               143  */   (0x07),
};

//============================================================================
// QUSB Super-Speed MultiPort 0 PHY Configuration Array
//============================================================================

ALIGN(4)
static const uint32 qusb_dci_ss_phy_MP0_cfg_address_common[QUSB_DCI_SS_PHY_MP0_COMMON_CFG_ARRAY_ENTRY_CNT] =
{
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL_ADDR,             // 000 : (0x01)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SYSCLK_EN_SEL_ADDR,                        // 001 : (0x1A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_HSCLK_SEL_ADDR,                 // 002 : (0x11)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_HSCLK_SEL_ADDR,                            // 003 : (0x01)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DEC_START_MODE0_ADDR,                      // 004 : (0x82)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,                // 005 : (0xAB)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,                // 006 : (0xEA)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,                // 007 : (0x02)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0_ADDR,           // 008 : (0xCA)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0_ADDR,           // 009 : (0x1E)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_CP_CTRL_MODE0_ADDR,                        // 010 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_PLL_RCTRL_MODE0_ADDR,                      // 011 : (0x16)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_PLL_CCTRL_MODE0_ADDR,                      // 012 : (0x36)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_VCO_TUNE1_MODE0_ADDR,                      // 013 : (0x24)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP2_MODE0_ADDR,                      // 014 : (0x34)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP1_MODE0_ADDR,                      // 015 : (0x14)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP_EN_ADDR,                          // 016 : (0x04)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,                    // 017 : (0x0A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_VCO_TUNE2_MODE1_ADDR,                      // 018 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_VCO_TUNE1_MODE1_ADDR,                      // 019 : (0x24)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_CORECLK_DIV_MODE1_ADDR,                    // 020 : (0x08)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DEC_START_MODE1_ADDR,                      // 021 : (0x82)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START1_MODE1_ADDR,                // 022 : (0xAB)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START2_MODE1_ADDR,                // 023 : (0xEA)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START3_MODE1_ADDR,                // 024 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP2_MODE1_ADDR,                      // 025 : (0x82)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP1_MODE1_ADDR,                      // 026 : (0x34)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_CP_CTRL_MODE1_ADDR,                        // 027 : (0x06)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_PLL_RCTRL_MODE1_ADDR,                      // 028 : (0x16)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_PLL_CCTRL_MODE1_ADDR,                      // 029 : (0x36)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1_ADDR,           // 030 : (0xCA)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1_ADDR,           // 031 : (0x1E)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_CMN_IPTRIM_ADDR,                           // 032 : (0x20)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_EN_CENTER_ADDR,                        // 033 : (0x01)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_PER1_ADDR,                             // 034 : (0x31)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_PER2_ADDR,                             // 035 : (0x01)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_STEP_SIZE1_MODE1_ADDR,                 // 036 : (0xDE)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_STEP_SIZE2_MODE1_ADDR,                 // 037 : (0x07)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_STEP_SIZE1_MODE0_ADDR,                 // 038 : (0xDE)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_STEP_SIZE2_MODE0_ADDR,                 // 039 : (0x07)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_VCO_TUNE_MAP_ADDR,                         // 040 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_HIGH4_ADDR,                      // 041 : (0x86)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_HIGH3_ADDR,                      // 042 : (0x7F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_HIGH2_ADDR,                      // 043 : (0xFF)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_HIGH_ADDR,                       // 044 : (0x3F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_LOW_ADDR,                        // 045 : (0xFF)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_HIGH4_ADDR,                      // 046 : (0xB3)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_HIGH3_ADDR,                      // 047 : (0x0B)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_HIGH2_ADDR,                      // 048 : (0x5C)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_HIGH_ADDR,                       // 049 : (0xDC)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_LOW_ADDR,                        // 050 : (0xDC)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_PI_CONTROLS_ADDR,                      // 051 : (0x99)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SB2_THRESH1_ADDR,                      // 052 : (0x04)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SB2_THRESH2_ADDR,                      // 053 : (0x08)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SB2_GAIN1_ADDR,                        // 054 : (0x05)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SB2_GAIN2_ADDR,                        // 055 : (0x05)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_FASTLOCK_FO_GAIN_ADDR,                 // 056 : (0x2F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_FASTLOCK_COUNT_LOW_ADDR,               // 057 : (0xFF)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_FASTLOCK_COUNT_HIGH_ADDR,              // 058 : (0x0F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SO_SATURATION_AND_ENABLE_ADDR,         // 059 : (0x7F)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_FO_GAIN_ADDR,                          // 060 : (0x0A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_VGA_CAL_CNTRL1_ADDR,                        // 061 : (0x54)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_VGA_CAL_CNTRL2_ADDR,                        // 062 : (0x0F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL2_ADDR,                 // 063 : (0x0F)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL3_ADDR,                 // 064 : (0x4A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL4_ADDR,                 // 065 : (0x0A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_DFE_EN_TIMER_ADDR,                          // 066 : (0x04)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR,           // 067 : (0x47)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_OFFSET_ADAPTOR_CNTRL2_ADDR,              // 068 : (0x80)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_SIGDET_CNTRL_ADDR,                          // 069 : (0x04)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_SIGDET_DEGLITCH_CNTRL_ADDR,                 // 070 : (0x0E)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_IDAC_TSETTLE_HIGH_ADDR,                  // 071 : (0x00)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_IDAC_TSETTLE_LOW_ADDR,                   // 072 : (0xC0)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_DFE_CTLE_POST_CAL_OFFSET_ADDR,              // 073 : (0x38)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SO_GAIN_ADDR,                          // 074 : (0x06)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_DCC_CTRL1_ADDR,                             // 075 : (0x0C)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_GM_CAL_ADDR,                                // 076 : (0x1F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_RCV_DETECT_LVL_2_ADDR,                      // 077 : (0x12)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_LANE_MODE_1_ADDR,                           // 078 : (0xD5)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_LANE_MODE_2_ADDR,                           // 079 : (0xC2)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_PI_QEC_CTRL_ADDR,                           // 080 : (0x40)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_RES_CODE_LANE_OFFSET_TX_ADDR,               // 081 : (0x11)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_RES_CODE_LANE_OFFSET_RX_ADDR,               // 082 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG1_ADDR,            // 083 : (0xD0)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG2_ADDR,            // 084 : (0x07)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG3_ADDR,            // 085 : (0x20)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG6_ADDR,            // 086 : (0x13)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_RCVR_DTCT_DLY_P1U2_L_ADDR,           // 087 : (0xE7)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_RCVR_DTCT_DLY_P1U2_H_ADDR,           // 088 : (0x03)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_RX_SIGDET_LVL_ADDR,                  // 089 : (0xAA)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_USB3_RXEQTRAINING_DFE_TIME_S2_ADDR,  // 090 : (0x07)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL_ADDR,   // 091 : (0xF8)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_CDR_RESET_TIME_ADDR,                 // 092 : (0x0A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_ALIGN_DETECT_CONFIG1_ADDR,           // 093 : (0x88)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_ALIGN_DETECT_CONFIG2_ADDR,           // 094 : (0x13)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_EQ_CONFIG1_ADDR,                     // 095 : (0x4B)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_EQ_CONFIG5_ADDR,                     // 096 : (0x10)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_REFGEN_REQ_CONFIG1_ADDR,             // 097 : (0x21)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_SW_RESET_ADDR,                       // 098 : (0x00)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_START_CONTROL_ADDR,                  // 099 : (0x03)
    //-------------------------------------------------------------------------------------------------------
};

ALIGN(4)
static const uint8 qusb_dci_ss_phy_MP0_cfg_value_common[QUSB_DCI_SS_PHY_MP0_COMMON_CFG_ARRAY_ENTRY_CNT] =
{
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL_ADDR,             000 */ (0x01),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SYSCLK_EN_SEL_ADDR,                        001 */ (0x1A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_HSCLK_SEL_ADDR,                 002 */ (0x11),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_HSCLK_SEL_ADDR,                            003 */ (0x01),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DEC_START_MODE0_ADDR,                      004 */ (0x82),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,                005 */ (0xAB),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,                006 */ (0xEA),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,                007 */ (0x02),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0_ADDR,           008 */ (0xCA),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0_ADDR,           009 */ (0x1E),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_CP_CTRL_MODE0_ADDR,                        010 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_PLL_RCTRL_MODE0_ADDR,                      011 */ (0x16),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_PLL_CCTRL_MODE0_ADDR,                      012 */ (0x36),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_VCO_TUNE1_MODE0_ADDR,                      013 */ (0x24),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP2_MODE0_ADDR,                      014 */ (0x34),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP1_MODE0_ADDR,                      015 */ (0x14),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP_EN_ADDR,                          016 */ (0x04),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,                    017 */ (0x0A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_VCO_TUNE2_MODE1_ADDR,                      018 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_VCO_TUNE1_MODE1_ADDR,                      019 */ (0x24),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_CORECLK_DIV_MODE1_ADDR,                    020 */ (0x08),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DEC_START_MODE1_ADDR,                      021 */ (0x82),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START1_MODE1_ADDR,                022 */ (0xAB),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START2_MODE1_ADDR,                023 */ (0xEA),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_DIV_FRAC_START3_MODE1_ADDR,                024 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP2_MODE1_ADDR,                      025 */ (0x82),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_LOCK_CMP1_MODE1_ADDR,                      026 */ (0x34),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_CP_CTRL_MODE1_ADDR,                        027 */ (0x06),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_PLL_RCTRL_MODE1_ADDR,                      028 */ (0x16),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_PLL_CCTRL_MODE1_ADDR,                      029 */ (0x36),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1_ADDR,           030 */ (0xCA),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1_ADDR,           031 */ (0x1E),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_CMN_IPTRIM_ADDR,                           032 */ (0x20),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_EN_CENTER_ADDR,                        033 */ (0x01),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_PER1_ADDR,                             034 */ (0x31),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_PER2_ADDR,                             035 */ (0x01),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_STEP_SIZE1_MODE1_ADDR,                 036 */ (0xDE),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_STEP_SIZE2_MODE1_ADDR,                 037 */ (0x07),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_STEP_SIZE1_MODE0_ADDR,                 038 */ (0xDE),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_SSC_STEP_SIZE2_MODE0_ADDR,                 039 */ (0x07),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_COM_VCO_TUNE_MAP_ADDR,                         040 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_HIGH4_ADDR,                      041 */ (0x86),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_HIGH3_ADDR,                      042 */ (0x7F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_HIGH2_ADDR,                      043 */ (0xFF),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_HIGH_ADDR,                       044 */ (0x3F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_00_LOW_ADDR,                        045 */ (0xFF),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_HIGH4_ADDR,                      046 */ (0xB3),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_HIGH3_ADDR,                      047 */ (0x0B),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_HIGH2_ADDR,                      048 */ (0x5C),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_HIGH_ADDR,                       049 */ (0xDC),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_MODE_01_LOW_ADDR,                        050 */ (0xDC),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_PI_CONTROLS_ADDR,                      051 */ (0x99),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SB2_THRESH1_ADDR,                      052 */ (0x04),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SB2_THRESH2_ADDR,                      053 */ (0x08),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SB2_GAIN1_ADDR,                        054 */ (0x05),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SB2_GAIN2_ADDR,                        055 */ (0x05),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_FASTLOCK_FO_GAIN_ADDR,                 056 */ (0x2F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_FASTLOCK_COUNT_LOW_ADDR,               057 */ (0xFF),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_FASTLOCK_COUNT_HIGH_ADDR,              058 */ (0x0F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SO_SATURATION_AND_ENABLE_ADDR,         059 */ (0x7F),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_FO_GAIN_ADDR,                          060 */ (0x0A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_VGA_CAL_CNTRL1_ADDR,                        061 */ (0x54),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_VGA_CAL_CNTRL2_ADDR,                        062 */ (0x0F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL2_ADDR,                 063 */ (0x0F),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL3_ADDR,                 064 */ (0x4A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_EQU_ADAPTOR_CNTRL4_ADDR,                 065 */ (0x0A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_DFE_EN_TIMER_ADDR,                          066 */ (0x04),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR,           067 */ (0x47),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_OFFSET_ADAPTOR_CNTRL2_ADDR,              068 */ (0x80),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_SIGDET_CNTRL_ADDR,                          069 */ (0x04),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_SIGDET_DEGLITCH_CNTRL_ADDR,                 070 */ (0x0E),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_IDAC_TSETTLE_HIGH_ADDR,                  071 */ (0x00),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_RX_IDAC_TSETTLE_LOW_ADDR,                   072 */ (0xC0),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_DFE_CTLE_POST_CAL_OFFSET_ADDR,              073 */ (0x38),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_UCDR_SO_GAIN_ADDR,                          074 */ (0x06),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_DCC_CTRL1_ADDR,                             075 */ (0x0C),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_RX_GM_CAL_ADDR,                                076 */ (0x1F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_RCV_DETECT_LVL_2_ADDR,                      077 */ (0x12),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_LANE_MODE_1_ADDR,                           078 */ (0xD5),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_LANE_MODE_2_ADDR,                           079 */ (0xC2),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_PI_QEC_CTRL_ADDR,                           080 */ (0x40),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_RES_CODE_LANE_OFFSET_TX_ADDR,               081 */ (0x11),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_QSERDES_TX_RES_CODE_LANE_OFFSET_RX_ADDR,               082 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG1_ADDR,            083 */ (0xD0),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG2_ADDR,            084 */ (0x07),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG3_ADDR,            085 */ (0x20),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG6_ADDR,            086 */ (0x13),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_RCVR_DTCT_DLY_P1U2_L_ADDR,           087 */ (0xE7),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_RCVR_DTCT_DLY_P1U2_H_ADDR,           088 */ (0x03),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_RX_SIGDET_LVL_ADDR,                  089 */ (0xAA),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_USB3_RXEQTRAINING_DFE_TIME_S2_ADDR,  090 */ (0x07),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL_ADDR,   091 */ (0xF8),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_CDR_RESET_TIME_ADDR,                 092 */ (0x0A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_ALIGN_DETECT_CONFIG1_ADDR,           093 */ (0x88),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_ALIGN_DETECT_CONFIG2_ADDR,           094 */ (0x13),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_EQ_CONFIG1_ADDR,                     095 */ (0x4B),
    //-------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_EQ_CONFIG5_ADDR,                     096 */ (0x10),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_REFGEN_REQ_CONFIG1_ADDR,             097 */ (0x21),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_SW_RESET_ADDR,                       098 */ (0x00),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_PCIE_USB3_UNI_PCS_START_CONTROL_ADDR,                  099 */ (0x03),
    //-------------------------------------------------------------------------------------------------------
};

//============================================================================
// QUSB Super-Speed MultiPort 1 PHY Configuration Array
//============================================================================
ALIGN(4)
static const uint32 qusb_dci_ss_phy_MP1_cfg_address_common[QUSB_DCI_SS_PHY_MP1_COMMON_CFG_ARRAY_ENTRY_CNT] =
{
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL_ADDR,             // 000 : (0x01)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SYSCLK_EN_SEL_ADDR,                        // 001 : (0x1A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_HSCLK_SEL_ADDR,                 // 002 : (0x11)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_HSCLK_SEL_ADDR,                            // 003 : (0x01)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DEC_START_MODE0_ADDR,                      // 004 : (0x82)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,                // 005 : (0xAB)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,                // 006 : (0xEA)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,                // 007 : (0x02)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0_ADDR,           // 008 : (0xCA)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0_ADDR,           // 009 : (0x1E)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_CP_CTRL_MODE0_ADDR,                        // 010 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_PLL_RCTRL_MODE0_ADDR,                      // 011 : (0x16)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_PLL_CCTRL_MODE0_ADDR,                      // 012 : (0x36)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_VCO_TUNE1_MODE0_ADDR,                      // 013 : (0x24)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP2_MODE0_ADDR,                      // 014 : (0x34)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP1_MODE0_ADDR,                      // 015 : (0x14)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP_EN_ADDR,                          // 016 : (0x04)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,                    // 017 : (0x0A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_VCO_TUNE2_MODE1_ADDR,                      // 018 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_VCO_TUNE1_MODE1_ADDR,                      // 019 : (0x24)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_CORECLK_DIV_MODE1_ADDR,                    // 020 : (0x08)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DEC_START_MODE1_ADDR,                      // 021 : (0x82)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START1_MODE1_ADDR,                // 022 : (0xAB)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START2_MODE1_ADDR,                // 023 : (0xEA)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START3_MODE1_ADDR,                // 024 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP2_MODE1_ADDR,                      // 025 : (0x82)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP1_MODE1_ADDR,                      // 026 : (0x34)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_CP_CTRL_MODE1_ADDR,                        // 027 : (0x06)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_PLL_RCTRL_MODE1_ADDR,                      // 028 : (0x16)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_PLL_CCTRL_MODE1_ADDR,                      // 029 : (0x36)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1_ADDR,           // 030 : (0xCA)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1_ADDR,           // 031 : (0x1E)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_CMN_IPTRIM_ADDR,                           // 032 : (0x20)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_EN_CENTER_ADDR,                        // 033 : (0x01)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_PER1_ADDR,                             // 034 : (0x31)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_PER2_ADDR,                             // 035 : (0x01)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_STEP_SIZE1_MODE1_ADDR,                 // 036 : (0xDE)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_STEP_SIZE2_MODE1_ADDR,                 // 037 : (0x07)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_STEP_SIZE1_MODE0_ADDR,                 // 038 : (0xDE)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_STEP_SIZE2_MODE0_ADDR,                 // 039 : (0x07)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_VCO_TUNE_MAP_ADDR,                         // 040 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_HIGH4_ADDR,                      // 041 : (0x86)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_HIGH3_ADDR,                      // 042 : (0x7F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_HIGH2_ADDR,                      // 043 : (0xFF)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_HIGH_ADDR,                       // 044 : (0x3F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_LOW_ADDR,                        // 045 : (0xFF)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_HIGH4_ADDR,                      // 046 : (0xB3)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_HIGH3_ADDR,                      // 047 : (0x0B)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_HIGH2_ADDR,                      // 048 : (0x5C)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_HIGH_ADDR,                       // 049 : (0xDC)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_LOW_ADDR,                        // 050 : (0xDC)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_PI_CONTROLS_ADDR,                      // 051 : (0x99)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SB2_THRESH1_ADDR,                      // 052 : (0x04)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SB2_THRESH2_ADDR,                      // 053 : (0x08)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SB2_GAIN1_ADDR,                        // 054 : (0x05)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SB2_GAIN2_ADDR,                        // 055 : (0x05)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_FASTLOCK_FO_GAIN_ADDR,                 // 056 : (0x2F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_FASTLOCK_COUNT_LOW_ADDR,               // 057 : (0xFF)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_FASTLOCK_COUNT_HIGH_ADDR,              // 058 : (0x0F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SO_SATURATION_AND_ENABLE_ADDR,         // 059 : (0x7F)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_FO_GAIN_ADDR,                          // 060 : (0x0A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_VGA_CAL_CNTRL1_ADDR,                        // 061 : (0x54)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_VGA_CAL_CNTRL2_ADDR,                        // 062 : (0x0F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_EQU_ADAPTOR_CNTRL2_ADDR,                 // 063 : (0x0F)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_EQU_ADAPTOR_CNTRL3_ADDR,                 // 064 : (0x4A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_EQU_ADAPTOR_CNTRL4_ADDR,                 // 065 : (0x0A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_DFE_EN_TIMER_ADDR,                          // 066 : (0x04)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR,           // 067 : (0x47)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_OFFSET_ADAPTOR_CNTRL2_ADDR,              // 068 : (0x80)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_SIGDET_CNTRL_ADDR,                          // 069 : (0x04)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_SIGDET_DEGLITCH_CNTRL_ADDR,                 // 070 : (0x0E)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_IDAC_TSETTLE_HIGH_ADDR,                  // 071 : (0x00)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_IDAC_TSETTLE_LOW_ADDR,                   // 072 : (0xC0)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_DFE_CTLE_POST_CAL_OFFSET_ADDR,              // 073 : (0x38)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SO_GAIN_ADDR,                          // 074 : (0x06)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_DCC_CTRL1_ADDR,                             // 075 : (0x0C)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_GM_CAL_ADDR,                                // 076 : (0x1F)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_RCV_DETECT_LVL_2_ADDR,                      // 077 : (0x12)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_LANE_MODE_1_ADDR,                           // 078 : (0xD5)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_LANE_MODE_2_ADDR,                           // 079 : (0xC2)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_PI_QEC_CTRL_ADDR,                           // 080 : (0x40)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_RES_CODE_LANE_OFFSET_TX_ADDR,               // 081 : (0x11)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_RES_CODE_LANE_OFFSET_RX_ADDR,               // 082 : (0x02)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG1_ADDR,            // 083 : (0xD0)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG2_ADDR,            // 084 : (0x07)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG3_ADDR,            // 085 : (0x20)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG6_ADDR,            // 086 : (0x13)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_RCVR_DTCT_DLY_P1U2_L_ADDR,           // 087 : (0xE7)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_RCVR_DTCT_DLY_P1U2_H_ADDR,           // 088 : (0x03)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_RX_SIGDET_LVL_ADDR,                  // 089 : (0xAA)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_USB3_RXEQTRAINING_DFE_TIME_S2_ADDR,  // 090 : (0x07)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL_ADDR,   // 091 : (0xF8)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_CDR_RESET_TIME_ADDR,                 // 092 : (0x0A)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_ALIGN_DETECT_CONFIG1_ADDR,           // 093 : (0x88)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_ALIGN_DETECT_CONFIG2_ADDR,           // 094 : (0x13)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_EQ_CONFIG1_ADDR,                     // 095 : (0x4B)
    //-------------------------------------------------------------------------------------------------------
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_EQ_CONFIG5_ADDR,                     // 096 : (0x10)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_REFGEN_REQ_CONFIG1_ADDR,             // 097 : (0x21)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_SW_RESET_ADDR,                       // 098 : (0x00)
    HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_START_CONTROL_ADDR,                  // 099 : (0x03)
    //-------------------------------------------------------------------------------------------------------
};

ALIGN(4)
static const uint8 qusb_dci_ss_phy_MP1_cfg_value_common[QUSB_DCI_SS_PHY_MP1_COMMON_CFG_ARRAY_ENTRY_CNT] =
{
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_POWER_DOWN_CONTROL_ADDR,             000 */ (0x01),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SYSCLK_EN_SEL_ADDR,                        001 */ (0x1A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_HSCLK_SEL_ADDR,                 002 */ (0x11),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_HSCLK_SEL_ADDR,                            003 */ (0x01),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DEC_START_MODE0_ADDR,                      004 */ (0x82),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START1_MODE0_ADDR,                005 */ (0xAB),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START2_MODE0_ADDR,                006 */ (0xEA),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START3_MODE0_ADDR,                007 */ (0x02),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE0_ADDR,           008 */ (0xCA),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE0_ADDR,           009 */ (0x1E),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_CP_CTRL_MODE0_ADDR,                        010 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_PLL_RCTRL_MODE0_ADDR,                      011 */ (0x16),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_PLL_CCTRL_MODE0_ADDR,                      012 */ (0x36),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_VCO_TUNE1_MODE0_ADDR,                      013 */ (0x24),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP2_MODE0_ADDR,                      014 */ (0x34),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP1_MODE0_ADDR,                      015 */ (0x14),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP_EN_ADDR,                          016 */ (0x04),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SYSCLK_BUF_ENABLE_ADDR,                    017 */ (0x0A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_VCO_TUNE2_MODE1_ADDR,                      018 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_VCO_TUNE1_MODE1_ADDR,                      019 */ (0x24),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_CORECLK_DIV_MODE1_ADDR,                    020 */ (0x08),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DEC_START_MODE1_ADDR,                      021 */ (0x82),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START1_MODE1_ADDR,                022 */ (0xAB),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START2_MODE1_ADDR,                023 */ (0xEA),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_DIV_FRAC_START3_MODE1_ADDR,                024 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP2_MODE1_ADDR,                      025 */ (0x82),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_LOCK_CMP1_MODE1_ADDR,                      026 */ (0x34),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_CP_CTRL_MODE1_ADDR,                        027 */ (0x06),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_PLL_RCTRL_MODE1_ADDR,                      028 */ (0x16),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_PLL_CCTRL_MODE1_ADDR,                      029 */ (0x36),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_CMP_CODE1_MODE1_ADDR,           030 */ (0xCA),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_BIN_VCOCAL_CMP_CODE2_MODE1_ADDR,           031 */ (0x1E),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_CMN_IPTRIM_ADDR,                           032 */ (0x20),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_EN_CENTER_ADDR,                        033 */ (0x01),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_PER1_ADDR,                             034 */ (0x31),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_PER2_ADDR,                             035 */ (0x01),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_STEP_SIZE1_MODE1_ADDR,                 036 */ (0xDE),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_STEP_SIZE2_MODE1_ADDR,                 037 */ (0x07),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_STEP_SIZE1_MODE0_ADDR,                 038 */ (0xDE),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_SSC_STEP_SIZE2_MODE0_ADDR,                 039 */ (0x07),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_COM_VCO_TUNE_MAP_ADDR,                         040 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_HIGH4_ADDR,                      041 */ (0x86),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_HIGH3_ADDR,                      042 */ (0x7F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_HIGH2_ADDR,                      043 */ (0xFF),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_HIGH_ADDR,                       044 */ (0x3F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_00_LOW_ADDR,                        045 */ (0xFF),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_HIGH4_ADDR,                      046 */ (0xB3),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_HIGH3_ADDR,                      047 */ (0x0B),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_HIGH2_ADDR,                      048 */ (0x5C),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_HIGH_ADDR,                       049 */ (0xDC),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_MODE_01_LOW_ADDR,                        050 */ (0xDC),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_PI_CONTROLS_ADDR,                      051 */ (0x99),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SB2_THRESH1_ADDR,                      052 */ (0x04),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SB2_THRESH2_ADDR,                      053 */ (0x08),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SB2_GAIN1_ADDR,                        054 */ (0x05),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SB2_GAIN2_ADDR,                        055 */ (0x05),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_FASTLOCK_FO_GAIN_ADDR,                 056 */ (0x2F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_FASTLOCK_COUNT_LOW_ADDR,               057 */ (0xFF),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_FASTLOCK_COUNT_HIGH_ADDR,              058 */ (0x0F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SO_SATURATION_AND_ENABLE_ADDR,         059 */ (0x7F),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_FO_GAIN_ADDR,                          060 */ (0x0A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_VGA_CAL_CNTRL1_ADDR,                        061 */ (0x54),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_VGA_CAL_CNTRL2_ADDR,                        062 */ (0x0F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_EQU_ADAPTOR_CNTRL2_ADDR,                 063 */ (0x0F),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_EQU_ADAPTOR_CNTRL3_ADDR,                 064 */ (0x4A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_EQU_ADAPTOR_CNTRL4_ADDR,                 065 */ (0x0A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_DFE_EN_TIMER_ADDR,                          066 */ (0x04),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_EQ_OFFSET_ADAPTOR_CNTRL1_ADDR,           067 */ (0x47),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_OFFSET_ADAPTOR_CNTRL2_ADDR,              068 */ (0x80),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_SIGDET_CNTRL_ADDR,                          069 */ (0x04),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_SIGDET_DEGLITCH_CNTRL_ADDR,                 070 */ (0x0E),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_IDAC_TSETTLE_HIGH_ADDR,                  071 */ (0x00),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_RX_IDAC_TSETTLE_LOW_ADDR,                   072 */ (0xC0),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_DFE_CTLE_POST_CAL_OFFSET_ADDR,              073 */ (0x38),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_UCDR_SO_GAIN_ADDR,                          074 */ (0x06),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_DCC_CTRL1_ADDR,                             075 */ (0x0C),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_RX_GM_CAL_ADDR,                                076 */ (0x1F),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_RCV_DETECT_LVL_2_ADDR,                      077 */ (0x12),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_LANE_MODE_1_ADDR,                           078 */ (0xD5),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_LANE_MODE_2_ADDR,                           079 */ (0xC2),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_PI_QEC_CTRL_ADDR,                           080 */ (0x40),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_RES_CODE_LANE_OFFSET_TX_ADDR,               081 */ (0x11),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECQSERDES_TX_RES_CODE_LANE_OFFSET_RX_ADDR,               082 */ (0x02),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG1_ADDR,            083 */ (0xD0),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG2_ADDR,            084 */ (0x07),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG3_ADDR,            085 */ (0x20),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_LOCK_DETECT_CONFIG6_ADDR,            086 */ (0x13),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_RCVR_DTCT_DLY_P1U2_L_ADDR,           087 */ (0xE7),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_RCVR_DTCT_DLY_P1U2_H_ADDR,           088 */ (0x03),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_RX_SIGDET_LVL_ADDR,                  089 */ (0xAA),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_USB3_RXEQTRAINING_DFE_TIME_S2_ADDR,  090 */ (0x07),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_USB3_LFPS_DET_HIGH_COUNT_VAL_ADDR,   091 */ (0xF8),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_CDR_RESET_TIME_ADDR,                 092 */ (0x0A),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_ALIGN_DETECT_CONFIG1_ADDR,           093 */ (0x88),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_ALIGN_DETECT_CONFIG2_ADDR,           094 */ (0x13),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_EQ_CONFIG1_ADDR,                     095 */ (0x4B),
    //---------------------------------------------------------------------------------------------------------
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_EQ_CONFIG5_ADDR,                     096 */ (0x10),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_REFGEN_REQ_CONFIG1_ADDR,             097 */ (0x21),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_SW_RESET_ADDR,                       098 */ (0x00),
    /* HWIO_AHB2PHY_SOUTH_USB3_UNI_PHY_SECPCIE_USB3_UNI_PCS_START_CONTROL_ADDR,                  099 */ (0x03),
    //---------------------------------------------------------------------------------------------------------
};

