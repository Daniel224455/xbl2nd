/** @file \UsbConfigUsbCores.c

  Function and structure definitions for each USB core supported by this
  platform.

**/
//============================================================================
/**
  Copyright (c) 2015-2019 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2007 - 2013 Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php
  
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 */
//============================================================================

/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 04/18/19   gk       Fix the UsbConfigDxe Taking More Boot time
 03/26/19   ak       Support for advancing TX SuperSpeed compliance test pattern
 02/20/19   kameya   Fix GCC reset secondary USB controller and disable USB20 internal retry feature.
 01/16/19   gk       Added Support to Turnoff NPA for all cores.
 12/26/18   gk       Added Register for Non-Driving Mode
 10/11/18   kameya   Support USB SLT loopback feature.
 04/19/18   amitg    Poipu Multi-Port Support
 04/10/18   amitg    Poipu BU Fixes
 04/03/18   psapin   Remove extra ICBArb bandwidth requests for Poipu BU, set Port 2 naming as 3_2
 02/13/18   gk	     Port changes from boot.xf.2.0 and boot.xf.2.2
 01/30/18   pm       Fixed issue to StartUsbController, if no USB cable connected during bootup
 01/22/18   gk       Added USB Variable to Enable High Speed Mode For Display port
 11/27/17   amitg    MP Core Changes for Poipu
 10/20/17   gk       Updated new Pmic API for UsbPwrCtrlDxe for UEFI
 08/10/17   amitg    Init Changes for Poipu
 06/14/17   amitg    Bus Bandwidth Vote for primary and secondary core and cleanup
 04/26/17   jc       Added missing PMI8998 check to switch primary to SS3 LDO
 03/22/17   pm       Added configuration for sec Clock and core
 03/21/17   amitg    SDM845 UEFI USB Updates
 03/16/17   pm       Enabled the initialization of clk
 03/06/17   amitg    EUD Support
 02/03/17   pm       Vbus Enablement in USB Config Protocol
 02/03/17   pm       Port from Boot.xf.1.4
 01/24/17   pm       Fix the USB clock reference count
 01/19/17   pm       Do not set PIPE3 freq to XO
 12/20/16   sm       Edited input parameter list for EnableOtg PmicUsbProtocol API
 09/22/16   amitg    Napali Pre-Si Updates
 05/06/16   ck       Temp workaround by enable VBUS asap
 04/29/16   ck       Update HS PHY init for host mode
 03/31/16   ck       Disable SS PHY init for BU and correct HS PHY init
 02/23/16   ck       Isolate UsbCore data within the module
 01/22/16   ck       Fix the USB debug message usages
 01/14/16   ck       Enable host mode support in Virtio
 11/24/15   ck       Virtio FBC support
 10/26/15   ck       Init Sdm845 Pre-Si support
=============================================================================*/

#include "UsbConfigUsbCoresPrivate.h"
#include "UsbSharedLib.h"
#include "api/systemdrivers/icbarb.h"


#define FREQ_MHZ(f)     (f * 1000 * 1000)
#define FREQ_KHZ(f)     (f * 1000)

/*
 * USB 3.0 Clock definitions
 */
#define USB3_MASTER_CLK_MIN_FREQ_HZ           FREQ_MHZ(200)
#define USB3_AXI_CLK_MIN_FREQ_HZ              FREQ_MHZ(200)
#define USB3_PHY_AUX_CLK_MIN_FREQ_HZ          FREQ_KHZ(19200)
// Mock CLK needs to be set at 19.2 Mhz
#define USB3_MOCK_CLK_MIN_FREQ_HZ             FREQ_KHZ(19200)

#define MICRO_STALL_UNITS(x)  ((x) * 10)
#define MILLI_STALL_UNITS(x)  ((x) * 10000)

#define QUSB_PHY_MAX_STATUS_VERIFY_ATTEMPTS   50

#define USB3_INC_CMPL_PATTERN(x)  x=((x+1)%16)


/* Power Domain list */
const CHAR8 SSUsb1PowerDomain[] = "usb30_prim_gdsc";
const CHAR8 SSUsb2PowerDomain[] = "usb30_sec_gdsc";
const CHAR8 SSUsb3PowerDomain[] = "usb30_mp_gdsc";


/*******************************************************************************
 * USB core structures
 ******************************************************************************/

/*
 * USB core enable clock configuration arrays
 */
// gcc_ahb2phy_south_clk would be turned ON by BOOT (PBL/XBL), so can be removed

QCOM_USB_CLOCK SSUsb1EnCoreClocks[] =
{
  //{ "clock name",                     frequency,                    divider }
  {"gcc_cfg_noc_usb3_prim_axi_clk",     USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_aggre_usb3_prim_axi_clk",       USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_usb30_prim_master_clk",         USB3_MASTER_CLK_MIN_FREQ_HZ,   1 },
  {"gcc_usb30_prim_sleep_clk",          0,                             1 },
  {"gcc_usb30_prim_mock_utmi_clk",      USB3_MOCK_CLK_MIN_FREQ_HZ,     1 },
  {"gcc_usb3_prim_phy_aux_clk",         USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },
  {"gcc_usb3_prim_phy_com_aux_clk",     USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },  
  {"gcc_usb3_prim_phy_pipe_clk",        0,                             1 }
};

QCOM_USB_CLOCK SSUsb2EnCoreClocks[] =
{
  //{ "clock name",                    frequency,                    divider }
  {"gcc_cfg_noc_usb3_sec_axi_clk",     USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_aggre_usb3_sec_axi_clk",       USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_usb30_sec_master_clk",         USB3_MASTER_CLK_MIN_FREQ_HZ,   1 },
  {"gcc_usb30_sec_sleep_clk",          0,                             1 },
  {"gcc_usb30_sec_mock_utmi_clk",      USB3_MOCK_CLK_MIN_FREQ_HZ,     1 },
  {"gcc_usb3_sec_phy_aux_clk",         USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },
  {"gcc_usb3_sec_phy_com_aux_clk",     USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },    
  {"gcc_usb3_sec_phy_pipe_clk",        0,                             1 }
};


QCOM_USB_CLOCK SSUsb3EnCoreClocks[] =
{
  //{ "clock name",                    frequency,                    divider }
  {"gcc_cfg_noc_usb3_mp_axi_clk",     USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_aggre_usb3_mp_axi_clk",       USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_usb30_mp_master_clk",         USB3_MASTER_CLK_MIN_FREQ_HZ,   1 },
  {"gcc_usb30_mp_sleep_clk",          0,                             1 },
  {"gcc_usb30_mp_mock_utmi_clk",      USB3_MOCK_CLK_MIN_FREQ_HZ,     1 },
  {"gcc_usb3_mp_phy_aux_clk",         USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },
  {"gcc_usb3_mp_phy_com_aux_clk",     USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },
  {"gcc_usb3_mp_phy_pipe_0_clk",      0,                             1 },
  {"gcc_usb3_mp_phy_pipe_1_clk",      0,                             1 }
};

/*
 * USB core disable clock configuration arrays
 */

QCOM_USB_CLOCK SSUsb1DisCoreClocks[] =
{
  {"gcc_cfg_noc_usb3_prim_axi_clk",     USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_aggre_usb3_prim_axi_clk",       USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_usb30_prim_master_clk",         USB3_MASTER_CLK_MIN_FREQ_HZ,   1 },
  {"gcc_usb30_prim_sleep_clk",          0,                             1 },
  {"gcc_usb30_prim_mock_utmi_clk",      USB3_MOCK_CLK_MIN_FREQ_HZ,     1 },
  {"gcc_usb3_prim_phy_aux_clk",         USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 }, 
  {"gcc_usb3_prim_phy_com_aux_clk",     USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },    
  {"gcc_usb3_prim_phy_pipe_clk",        0,                             1 }
};

QCOM_USB_CLOCK SSUsb2DisCoreClocks[] =
{
  {"gcc_cfg_noc_usb3_sec_axi_clk",     USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_aggre_usb3_sec_axi_clk",       USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_usb30_sec_master_clk",         USB3_MASTER_CLK_MIN_FREQ_HZ,   1 },
  {"gcc_usb30_sec_sleep_clk",          0,                             1 },
  {"gcc_usb30_sec_mock_utmi_clk",      USB3_MOCK_CLK_MIN_FREQ_HZ,     1 },
  {"gcc_usb3_sec_phy_aux_clk",         USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 }, 
  {"gcc_usb3_sec_phy_com_aux_clk",     USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },
  {"gcc_usb3_sec_phy_pipe_clk",        0,                             1 }
};

QCOM_USB_CLOCK SSUsb3DisCoreClocks[] =
{
  {"gcc_cfg_noc_usb3_mp_axi_clk",     USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_aggre_usb3_mp_axi_clk",       USB3_AXI_CLK_MIN_FREQ_HZ,      1 },
  {"gcc_usb30_mp_master_clk",         USB3_MASTER_CLK_MIN_FREQ_HZ,   1 },
  {"gcc_usb30_mp_sleep_clk",          0,                             1 },
  {"gcc_usb30_mp_mock_utmi_clk",      USB3_MOCK_CLK_MIN_FREQ_HZ,     1 },
  {"gcc_usb3_mp_phy_aux_clk",         USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },
  {"gcc_usb3_mp_phy_com_aux_clk",     USB3_PHY_AUX_CLK_MIN_FREQ_HZ,  1 },
  {"gcc_usb3_mp_phy_pipe_0_clk",      0,                             1 },
  {"gcc_usb3_mp_phy_pipe_1_clk",      0,                             1 }
};


/*
 * Static Variables
 */
// TLMMProtocol to enable USB HUB
EFI_TLMM_PROTOCOL                             *TLMMProtocol = NULL;

// PMIC GPIO protocol
EFI_QCOM_PMIC_GPIO_PROTOCOL                   *PmicGpioProtocol = NULL;

// NPA Client Handle for SS1 and SS2 Cores
STATIC npa_client_handle                      gPmicNpaClientSS1 = NULL;
STATIC npa_client_handle                      gPmicNpaClientSS2 = NULL;
STATIC npa_client_handle                      gPmicNpaClientSS3 = NULL;

//NPA client handles for ICB - required for all cores
STATIC npa_client_handle                      gNpaClientSS1Bus = NULL;
STATIC npa_client_handle                      gNpaClientSS2Bus = NULL;
STATIC npa_client_handle                      gNpaClientSS3Bus = NULL;

EFI_CLOCK_PROTOCOL                            *ClockProtocol = NULL;
EFI_USB_PWR_CTRL_PROTOCOL                     *gPwrCtrlProtocol = NULL;

extern EFI_GUID                               gQcomTokenSpaceGuid;
extern EFI_PLATFORMINFO_PLATFORM_INFO_TYPE    PlatformInfo;
extern BOOLEAN                                gHighSpeedOnlyPrimary;
extern BOOLEAN                                gHighSpeedOnlySecondary;
extern BOOLEAN                                gUSBPriDPBonded;
extern BOOLEAN                                gUSBSecDPBonded;

static UINT8              Usb1HstCmplPattNum = 0;
static UINT8              Usb2HstCmplPattNum = 0;
static UINT8              Usb3HstCmplPattNum = 0;

/*******************************************************************************
 * ICB bandwidth request for USB Port 0, 1 and 2
 ******************************************************************************/
STATIC CONST ICBArb_RequestType usb_icb_request[] = 
{
  {
    .arbType = ICBARB_REQUEST_TYPE_3,
    .arbData.type3 = 
    {
      .uIb = 400000000,        /**< Instantaneous bandwidth in Bytes/second */
      .uAb = 0,                /**< Arbitrated bandwidth in Bytes/second */
      .uLatencyNs = 0          /**< Latency requirement in nanoseconds */
    }
  },
  {
    .arbType = ICBARB_REQUEST_TYPE_3,
    .arbData.type3 = 
    {
      .uIb = 670000000,        /**< Instantaneous bandwidth in Bytes/second */
      .uAb = 670000000,        /**< Arbitrated bandwidth in Bytes/second */
      .uLatencyNs = 0          /**< Latency requirement in nanoseconds */
    }
  }
};

STATIC CONST ICBArb_RequestType usb_icb_clrrequest[] = 
{
  {
    .arbType = ICBARB_REQUEST_TYPE_3,
    .arbData.type3 = 
    {
      .uIb = 0,                /**< Instantaneous bandwidth in Bytes/second */
      .uAb = 0,                /**< Arbitrated bandwidth in Bytes/second */
      .uLatencyNs = 0          /**< Latency requirement in nanoseconds */
    }
  },
  {
    .arbType = ICBARB_REQUEST_TYPE_3,
    .arbData.type3 = 
    {
      .uIb = 0,                /**< Instantaneous bandwidth in Bytes/second */
      .uAb = 0,                /**< Arbitrated bandwidth in Bytes/second */
      .uLatencyNs = 0          /**< Latency requirement in nanoseconds */
    }
  }
};


/*******************************************************************************
 * ICB Master and Slave list for USB Port 0, USB Port 1, and USB Port 2
 ******************************************************************************/
STATIC CONST ICBArb_MasterSlaveType usbport0_str_slv_lst[] = 
{
  {
    .eMaster = ICBID_MASTER_APPSS_PROC,
    .eSlave = ICBID_SLAVE_USB3_0
  },
  {
    .eMaster = ICBID_MASTER_USB3_0,
    .eSlave = ICBID_SLAVE_EBI1
  }
};

STATIC CONST ICBArb_MasterSlaveType usbport1_str_slv_lst[] = 
{
  {
    .eMaster = ICBID_MASTER_APPSS_PROC,
    .eSlave = ICBID_SLAVE_USB3_1
  },
  {
    .eMaster = ICBID_MASTER_USB3_1,
    .eSlave = ICBID_SLAVE_EBI1
  }
};

STATIC CONST ICBArb_MasterSlaveType usbport2_str_slv_lst[] = 
{
  {
    .eMaster = ICBID_MASTER_APPSS_PROC,
    .eSlave = ICBID_SLAVE_USB3_2
  },
  {
    .eMaster = ICBID_MASTER_USB3_2,
    .eSlave = ICBID_SLAVE_EBI1
  }
};



/*******************************************************************************
 * Core initialization function implementations
 ******************************************************************************/

/*
 * See UsbConfigLibPrivate.h
 */
STATIC
EFI_STATUS
UsbConfigLibOpenProtocols (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG();

  //TODO:CK:Temp. use WRN_CHK to avoid issues
  if (!ClockProtocol) {
    Status = gBS->LocateProtocol(&gEfiClockProtocolGuid, NULL, (VOID**)&ClockProtocol);
    if (EFI_ERROR(Status)) {
      ClockProtocol = NULL;
      WRN_CHK("failed to locate CLOCK protocol");
    }
  }

   if (NULL == gPwrCtrlProtocol) {
    // Find the Power control USB charger protocol
    Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (void**)&gPwrCtrlProtocol);
    if (EFI_ERROR(Status))
    {
      gPwrCtrlProtocol = NULL;
      WRN_CHK("Failed to open Power control USB protocol Status =  (0x%x)", Status);
    }
  }
 
  //Creating NPA sync client for USB Super speed Primary core
  if (!gPmicNpaClientSS1) {
    
      gPmicNpaClientSS1 = npa_create_sync_client(
        PMIC_NPA_GROUP_ID_USB_SS1, //PMIC_NPA_GROUP_ID_USB_SS1 - Connect to the usb ss1 resource
        "usb_ss1",                 //client name "usb_ss1"
        NPA_CLIENT_REQUIRED);      //NPA_CLIENT_REQUIRED: Request must honor request
    
    if (!gPmicNpaClientSS1)
    {
      DBG(EFI_D_ERROR, "gPmicNpaClientSS1 cannot be created");
    }
  }

  //Create NPA nodes for USB Primary Controller required buses requests
  if (!gNpaClientSS1Bus) {
    gNpaClientSS1Bus = npa_create_sync_client_ex( "/icb/arbiter",
                           "usb_ss1_bus",
                           NPA_CLIENT_VECTOR,
                           sizeof(usbport0_str_slv_lst),
                           (void *)&usbport0_str_slv_lst);

    if (!gNpaClientSS1Bus){
      WRN_CHK("gNpaClientSS1Bus cannot be created");
    }
  }


  if (!gPmicNpaClientSS2) {
    gPmicNpaClientSS2 = npa_create_sync_client(
                            PMIC_NPA_GROUP_ID_USB_SS1, //PMIC_NPA_GROUP_ID_USB_SS2 - Connect to the usb ss2 resource
                            "usb_ss1",                 //client name "usb_ss2"
                            NPA_CLIENT_REQUIRED);      //NPA_CLIENT_REQUIRED: Request must honor request
  }

  if (!gPmicNpaClientSS2) {
    DBG(EFI_D_ERROR, "gPmicNpaClientSS2 cannot be created");
  }

  //Create NPA nodes for USB secondary controller required buses requests
  if (!gNpaClientSS2Bus) {
    gNpaClientSS2Bus = npa_create_sync_client_ex( "/icb/arbiter",
                           "usb_ss1_bus",
                           NPA_CLIENT_VECTOR,
                           sizeof(usbport1_str_slv_lst),
                           (void *)&usbport1_str_slv_lst);

    if (!gNpaClientSS2Bus)
    {
      WRN_CHK("gNpaClientSS2Bus cannot be created");
    }
  }

  if (!gPmicNpaClientSS3) {
    gPmicNpaClientSS3 = npa_create_sync_client(
                            PMIC_NPA_GROUP_ID_USB_SS1, //PMIC_NPA_GROUP_ID_USB_SS3 - Connect to the usb ss3 resource
                            "usb_ss1",                 //client name "usb_ss3"
                            NPA_CLIENT_REQUIRED);      //NPA_CLIENT_REQUIRED: Request must honor request
  }

  if (!gPmicNpaClientSS3) {
    DBG(EFI_D_ERROR, "gPmicNpaClientSS3 cannot be created");
  }

  //Create NPA nodes for USB MP controller required buses requests
  if (!gNpaClientSS3Bus) {
    gNpaClientSS3Bus = npa_create_sync_client_ex( "/icb/arbiter",
                           "usb_ss1_bus",
                           NPA_CLIENT_VECTOR,
                           sizeof(usbport2_str_slv_lst),
                           (void *)&usbport2_str_slv_lst);

    if (!gNpaClientSS3Bus)
    {
      WRN_CHK("gNpaClientSS3Bus cannot be created");
    }
  }


  if (!PmicGpioProtocol) {
    Status = gBS->LocateProtocol(&gQcomPmicGpioProtocolGuid, NULL, (void **)&PmicGpioProtocol);
    if (EFI_ERROR(Status))
    {
      PmicGpioProtocol = NULL;
      WRN_CHK("Failed to open PMIC GPIO protocol Status =  (0x%x)", Status);
    }
  }

//TODO:CK:Temp. use ON_EXIT, remove Status = EFI_SUCCESS later
//ON_EXIT:
  FNC_LEAVE_MSG();
  Status = EFI_SUCCESS;
  return Status;
}

/**
  Initializes power domains for USB.

  @retval EFI_SUCCESS            USB clocks initialized successfully.
  @retval EFI_NOT_FOUND          Unable to locate clock protocol.

**/
STATIC
EFI_STATUS
EFIAPI
ConfigPowerDomain(
  IN BOOLEAN       bEnable,
  IN CONST CHAR8   *szDomain
  )
{
  EFI_STATUS   Status = EFI_SUCCESS;
  UINTN        uClockPowerDomainId;

  FNC_ENTER_MSG ();

  if (ClockProtocol)
  {
      Status = ClockProtocol->GetClockPowerDomainID(ClockProtocol, szDomain, &uClockPowerDomainId);
      WRN_CHK ("USBConfigLib:GetClockPowerDomainID failed!");

      if (TRUE == bEnable)
      {
        Status =  ClockProtocol->EnableClockPowerDomain(ClockProtocol, uClockPowerDomainId);
        WRN_CHK ("USBConfigLib:EnableClockPowerDomain failed!");
      }
      else if (FALSE == bEnable)
      {
        Status =  ClockProtocol->DisableClockPowerDomain(ClockProtocol, uClockPowerDomainId);
        WRN_CHK ("USBConfigLib:DisableClockPowerDomain failed!");
      }
  }
  else
  {
      WRN_CHK ("failed to locate Clock Protocol ");
      WRN_CHK ("failed to initialize PowerDomain");
  }

  FNC_LEAVE_MSG ();
  return Status;
}



/**
  Initializes all clocks for a USB core.

  @param [in]  Clocks       Clock array
  @param [in]  ClockCount   Number of clocks in array

  @retval EFI_SUCCESS       USB clocks enabled successfully
  @retval Others            Error enabling at least 1 clock
**/
STATIC
EFI_STATUS
InitUsbClocks (
  IN  UINTN                 ClockCount,
  IN  QCOM_USB_CLOCK        *Clocks
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       ClkIdx;

  FNC_ENTER_MSG ();

  // validate parameters
  if ((NULL == Clocks) || (0 == ClockCount)) {
    Status = EFI_INVALID_PARAMETER;
    DBG(EFI_D_ERROR, "invalid parameter");
    goto ON_EXIT;
  }

  if (NULL == ClockProtocol) {
    DBG(EFI_D_ERROR, "ClockProtocol unavailable");
    Status = EFI_NOT_FOUND;
    goto ON_EXIT;
  }

  // enable each clock
  for (ClkIdx = 0; ClkIdx < ClockCount; ClkIdx++) {
    Status = EnableClock (
                Clocks[ClkIdx].Name,
                Clocks[ClkIdx].Frequency,
                Clocks[ClkIdx].Divider
                );
    if (EFI_ERROR (Status)) {
      DBG(EFI_D_ERROR, "failed to enable clock '%a'", Clocks[ClkIdx].Name);
    }
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
} // end InitUsbClocks



/**
  Disables all clocks for a USB core.

  @param [in]  Clocks       Clock array
  @param [in]  ClockCount   Number of clocks in array

  @retval EFI_SUCCESS       USB clocks disabled successfully
  @retval Others            Error disabling at least 1 clock
**/
STATIC
EFI_STATUS
DisableUsbClocks (
  IN  UINTN                 ClockCount,
  IN  QCOM_USB_CLOCK        *Clocks
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       ClkIdx;

  FNC_ENTER_MSG ();

  // validate parameters
  if ((NULL == Clocks) || (0 == ClockCount)) {
    Status = EFI_INVALID_PARAMETER;
    DBG(EFI_D_ERROR, "invalid parameter");
    goto ON_EXIT;
  }

  if (NULL == ClockProtocol) {
    DBG(EFI_D_ERROR, "ClockProtocol unavailable");
    Status = EFI_NOT_FOUND;
    goto ON_EXIT;
  }
  // disable each clock
  for (ClkIdx = 0; ClkIdx < ClockCount; ClkIdx++) {
    Status = DisableClock (Clocks[ClkIdx].Name);
    if (EFI_ERROR (Status)) {
      DBG(EFI_D_ERROR, "failed to disable clock '%a'", Clocks[ClkIdx].Name);
    }
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}

#if 0
/*
 * See UsbConfigUsbCoresPublic.h
 */
EFI_STATUS
UsbConfigLibCloseNPA (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG();

  //Disable the clocks for primary port, since it ll be only enabled by default.
  
  UINTN           PrimaryEnClockCount   = sizeof(SSUsb1EnCoreClocks) / sizeof(QCOM_USB_CLOCK);
  QCOM_USB_CLOCK  *PrimaryEnClocks      = SSUsb1EnCoreClocks;
  UINTN           PrimaryDisClockCount  = sizeof(SSUsb1DisCoreClocks) / sizeof(QCOM_USB_CLOCK);
  QCOM_USB_CLOCK  *PrimaryDisClocks     = SSUsb1DisCoreClocks;
  
  // Enable clocks first, without that disable clocks does not work
  Status = InitUsbClocks (PrimaryEnClockCount, PrimaryEnClocks);
  WRN_CHK ("failed to initialize USB clocks");

  // Disable clocks for Primary Core controller
  Status = DisableUsbClocks (PrimaryDisClockCount, PrimaryDisClocks);
  WRN_CHK ("failed to disable USB clocks");
  
  if(gPmicNpaClientSS1){
    npa_issue_required_request(gPmicNpaClientSS1, PMIC_NPA_MODE_ID_GENERIC_OFF);
  }
  
  if(gPmicNpaClientSS2){
    npa_issue_required_request( gPmicNpaClientSS2, PMIC_NPA_MODE_ID_GENERIC_OFF);
  }
  
  if(gPmicNpaClientSS3){
    npa_issue_required_request( gPmicNpaClientSS3, PMIC_NPA_MODE_ID_GENERIC_OFF);
  }
  
  // Remove Bus NPA votes for all Cores. Since we have turned On the votes using vector, so we need to disbale it with the same.
  if(gNpaClientSS1Bus){
    npa_issue_vector_request( gNpaClientSS1Bus,
                               (sizeof(usb_icb_clrrequest)/sizeof(npa_resource_state)), 
                               ((npa_resource_state *)usb_icb_clrrequest) );
  }
  if(gNpaClientSS2Bus){
    npa_issue_vector_request( gNpaClientSS2Bus,
                               (sizeof(usb_icb_clrrequest)/sizeof(npa_resource_state)), 
                               ((npa_resource_state *)usb_icb_clrrequest) );
  }
  if(gNpaClientSS3Bus){
    npa_issue_vector_request( gNpaClientSS3Bus,
                               (sizeof(usb_icb_clrrequest)/sizeof(npa_resource_state)), 
                               ((npa_resource_state *)usb_icb_clrrequest) );
  }
  
  FNC_LEAVE_MSG ();
  return Status;
}
#endif

/**
  Enable vbus for primary port (SuperSpeed Port)
  @param  [in]  CoreNum          The CoreNum associated with the core number/port index.
  @retval EFI_SUCCESS            Success

**/
EFI_STATUS
EFIAPI
EnableVbusSS (
  UINT8 CoreNum
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;

  FNC_ENTER_MSG ();

  // For Clamshell Platform Host Mode Enablement is automatically taken care by EC
  if (IS_CLS_PLAT(PlatformInfo.platform)) {
     Status = EFI_SUCCESS;
     goto ON_EXIT;
  }
  else {
   if (NULL == gPwrCtrlProtocol) {
        // Find the Power Control USB charger protocol
        Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (void**)&gPwrCtrlProtocol);
      if (EFI_ERROR(Status)) {
          gPwrCtrlProtocol = NULL;
          WRN_CHK("Failed to open Power Control USB protocol Status =  (0x%x)", Status);
        goto ON_EXIT;
      }
    }

    Status = gPwrCtrlProtocol->SetVbusSourceEn(CoreNum, TRUE);
    WRN_CHK ("failed to enable Vbus, Status = 0x%x", Status);
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  This API is used for resetting the Link and PHYs using clock control

  @param None

**/
VOID
SynopsysGccReset(
  IN  QCOM_USB_CORE         *UsbCore
)
{
  FNC_ENTER_MSG ();

  BOOLEAN is_eud_active = usb_shared_is_eud_active();

  if (UsbCore->CoreType == USB_CONFIG_SSUSB1)
  {
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_PRIM_BCR_OFFS, HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_BMSK);
    if (FALSE == is_eud_active) {
      HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_PRIM_BCR_OFFS, HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK);
    }
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OFFS, HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OFFS, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);

    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OFFS, ~HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OFFS, ~HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    if (FALSE == is_eud_active) {
      HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_PRIM_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK);
    }
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_PRIM_BCR_OFFS, ~HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
  }
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB2) 
{
    // TBD - Sort out reset registers
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_SEC_BCR_OFFS, HWIO_GCC_USB30_SEC_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_SEC_BCR_OFFS, HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_OFFS, HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OFFS, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OFFS, ~HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_OFFS, ~HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_SEC_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_BMSK);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_SEC_BCR_OFFS, ~HWIO_GCC_USB30_SEC_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
  }
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB3) 
  {
    //USB 3.0 MP Core Reset
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_MP_BCR_OFFS, HWIO_GCC_USB30_MP_BCR_BLK_ARES_BMSK);
    // HS Phy 1 Reset
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_MP0_BCR_OFFS, HWIO_GCC_QUSB2PHY_MP0_BCR_BLK_ARES_BMSK);
    // HS Phy 2 Reset
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_MP1_BCR_OFFS, HWIO_GCC_QUSB2PHY_MP1_BCR_BLK_ARES_BMSK);
    // SS Phy1 Reset
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_UNIPHY_MP0_BCR_OFFS, HWIO_GCC_USB3_UNIPHY_MP0_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_OFFS, HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_BLK_ARES_BMSK);
    // SSPhy 2 Reset
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_UNIPHY_MP1_BCR_OFFS, HWIO_GCC_USB3_UNIPHY_MP1_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_OFFS, HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_BLK_ARES_BMSK);

    gBS->Stall (100);
  
    // SSPhy 2 Out of Reset
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_OFFS, ~HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_BLK_ARES_BMSK);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_UNIPHY_MP1_BCR_OFFS, ~HWIO_GCC_USB3_UNIPHY_MP1_BCR_BLK_ARES_BMSK);
    // SSPhy 1 Out of Reset 
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_OFFS, ~HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_BLK_ARES_BMSK);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_UNIPHY_MP0_BCR_OFFS, ~HWIO_GCC_USB3_UNIPHY_MP0_BCR_BLK_ARES_BMSK);
    // HS Phy 2 Out of Reset
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_MP1_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_MP1_BCR_BLK_ARES_BMSK);
    // HS Phy 1 Out of Reset
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_MP0_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_MP0_BCR_BLK_ARES_BMSK);
    //USB 3.0 MP Core out of Reset
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_MP_BCR_OFFS, ~HWIO_GCC_USB30_MP_BCR_BLK_ARES_BMSK);

    gBS->Stall (100);
  }
  else
  {
    DBG(EFI_D_WARN, "Invalid Core Type");
  }

  FNC_LEAVE_MSG ();
  return;
}

/**
  Initializes High Speed phy for primary core when connected to SNPS Core for RUMI

  @retval EFI_SUCCESS            High Speed USB phy  initialized
                                 successfully.

**/

EFI_STATUS
InitRUMIHSUSBPhy (
  IN QCOM_USB_CORE              *UsbCore,
  IN UINT8                       PhyNum
  )
{
  UINTN       TI_Phy_Base =  0x0;
  EFI_STATUS  Status      =  EFI_SUCCESS;

  FNC_ENTER_MSG ();      

  if (UsbCore->CoreType == USB_CONFIG_SSUSB1) {
    TI_Phy_Base = TI_Base_PRIM;
  }
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB2) {
    TI_Phy_Base = TI_Base_SEC;
  }
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB3){
    TI_Phy_Base = TI_Base_MP(PhyNum);    
  }
  else {
    Status = EFI_INVALID_PARAMETER;  
    ERR_CHK("invalid parameter");
  }

  DBG(EFI_D_WARN, "EMULATION PHY INIT\n");
   //outp(HWIO_HS2_LOCAL_RESET_REG_ADDR, 0xFFF0);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_LOCAL_RESET_REG_OFFS, 0xFFF0);
  //sleep(100);
  gBS->Stall(500);
  //outp(HWIO_HS2_LOCAL_RESET_REG_ADDR, 0xFFF3);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_LOCAL_RESET_REG_OFFS, 0xFFF3);

  //sleep(100);
  gBS->Stall(100);

  //outp(HWIO_HS2_LOCAL_RESET_REG_ADDR, 0xFFF0);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_LOCAL_RESET_REG_OFFS, 0xFFF0);

  /* Shifting ULPI/UTMI clock MMCM */
  //outp(HWIO_HS2_USB_CLK_CTRL_ADDR,0x00100000);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_USB_CLK_CTRL_OFFS, 0x00100000);

  //sleep(100);
  gBS->Stall(100);

  //outp(HWIO_HS2_USB_CLK_CTRL_ADDR,0x00000000);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_USB_CLK_CTRL_OFFS, 0x00000000);

  //sleep(100);
  gBS->Stall(100);

  //outp(HWIO_HS2_USB_CLK_CTRL_ADDR,MMCM_Shift); /* MMCM shift value */
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_USB_CLK_CTRL_OFFS, 0x000001E0);

   //sleep(100);
   gBS->Stall(100);

  /* Shifting PIPE3 clock MMCM - Not used on RUMI */
  //outp(HWIO_HS2_USB_PIPE3_CLK_CTRL_ADDR,0x00100000);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_USB_PIPE3_CLK_CTRL_OFFS, 0x00100000);

  //sleep(100);
  gBS->Stall(100);

  //outp(HWIO_HS2_USB_PIPE3_CLK_CTRL_ADDR,0x00000000);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_USB_PIPE3_CLK_CTRL_OFFS, 0x00000000);

  //sleep(100);
  gBS->Stall(100);

  //outp(HWIO_HS2_USB_PIPE3_CLK_CTRL_ADDR,TI_PHY_PIPE_MMCM_SHIFT);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_USB_PIPE3_CLK_CTRL_OFFS, 0x0000004);

  //sleep(100);
  gBS->Stall(100);

  //DBG_E(DUTIL, ( "PIPE3 MMCM clock shift value set \n"));
  DBG(EFI_D_WARN, "UTMI MMCM clock shift value set\n");

  /* Clear the other Resets */
  //outp(HWIO_HS2_LOCAL_RESET_REG_ADDR, 0x0);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_LOCAL_RESET_REG_OFFS, 0x0);

  /*Put clock selector to read UTMI clock */
  //outp(HWIO_HS2_CLK_STATUS_SEL_ADDR,0x00000009);
  HAL_USB_WRITE(TI_Phy_Base, HWIO_HS2_CLK_STATUS_SEL_OFFS, 0x00000009);

  //sleep(100);
  gBS->Stall(100);

  //clkfreq_sec=inp(HWIO_HS2_CLK_STATUS_ADDR);
  //DBG_E(DUTIL, ( "uw_ssusb_init MP TI PHY : ULPI/UTMI frequency is: %d\n",clkfreq_sec));
  DBG(EFI_D_WARN, "UTMI CLK @ PAD: %d KHz\n", HAL_USB_READ(TI_Phy_Base, HWIO_HS2_CLK_STATUS_OFFS));

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}



/**
  Initializes High Speed QUSB2 phy for primary core when connected to SNPS Core

  @retval EFI_SUCCESS            High Speed USB phy  initialized
                                 successfully.

**/
EFI_STATUS
InitHSUSBPhy (
  IN QCOM_USB_CORE              *UsbCore
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;

  FNC_ENTER_MSG ();

  if (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) {
    // RUMI HS Phy Initialization
    Status = InitRUMIHSUSBPhy(UsbCore, CORE_DEFAULT_PHY);
    ERR_CHK("InitRUMIHSUSBPhy failed");

  }
  else if ((PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_RUMI)   &&
           (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_VIRTIO) &&
           (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_UNKNOWN) && 
           (FALSE == usb_shared_is_eud_active())) {
    //Hard reset PHY
    HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_PRIM_BCR_OFFS, HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);
    HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_PRIM_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);

    DBG(EFI_D_INFO, "HS Phy Init");
    usb_shared_hs_phy_init();
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Initializes High Speed USB QUSB2 phy for sceondary core when connected to SNPS Core

  @retval EFI_SUCCESS            High Speed USB phy  initialized
                                 successfully.

**/
EFI_STATUS
InitHSUSBPhySec (
  IN QCOM_USB_CORE              *UsbCore
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;
  FNC_ENTER_MSG ();

  // Sdm845 RUMI PHY init sequence for secondary port. 
  if (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) {
    // RUMI HS Phy Initialization
    Status = InitRUMIHSUSBPhy(UsbCore, CORE_DEFAULT_PHY);
    ERR_CHK("InitRUMIHSUSBPhy failed");

  }

  if ((PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_RUMI)   &&
      (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_VIRTIO) &&
      (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    //Hard reset PHY
    HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_SEC_BCR_OFFS, HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);
    HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_SEC_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);

    DBG(EFI_D_INFO, "HS Phy Init Sec");
    usb_shared_hs_phy_init_sec();
  }
ON_EXIT:
  FNC_LEAVE_MSG ();
  return EFI_SUCCESS;
}


/**
  Initializes High Speed QUSB2 phy for Multi Port core when connected to SNPS Core

  @retval EFI_SUCCESS            High Speed USB phy  initialized
                                 successfully.

**/
EFI_STATUS
InitHSUSBPhyMP (
  IN QCOM_USB_CORE              *UsbCore
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;

  FNC_ENTER_MSG ();

  if (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) {
    // RUMI HS Phy Initialization
    Status = InitRUMIHSUSBPhy(UsbCore, CORE_DEFAULT_PHY);
    ERR_CHK("InitRUMIHSUSBPhy primary port failed");

    gBS->Stall(100);

    Status = InitRUMIHSUSBPhy(UsbCore, CORE_SEC_PHY);
    ERR_CHK("InitRUMIHSUSBPhy failed");
  }
  else if ((PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_RUMI)   &&
           (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_VIRTIO) &&
           (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    //Hard reset PHY
    HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_MP0_BCR_OFFS, HWIO_GCC_QUSB2PHY_MP0_BCR_BLK_ARES_BMSK);
    HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_MP1_BCR_OFFS, HWIO_GCC_QUSB2PHY_MP1_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);
    HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_MP1_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_MP1_BCR_BLK_ARES_BMSK);
    HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_MP0_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_MP0_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);

    DBG(EFI_D_INFO, "HS Phy Init");

    // Add MP Core Initialization Seq in shared library
    usb_shared_hs_phy_init_MP0();
    usb_shared_hs_phy_init_MP1();
  }

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Initializes super speed Primary USB phy

  @retval EFI_SUCCESS            Super Speed USB phy  initialized
                                 successfully.

**/
STATIC
EFI_STATUS
InitSSUSBPhy (
  IN QCOM_USB_CORE              *UsbCore
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG ();

  // not needed for Sdm845 PRE-SI platform
  if ((PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI)   ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_VIRTIO) ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    DBG(EFI_D_WARN, "Pre-SI: Bypass InitSSUSBPhy");
    goto ON_EXIT;
  }

  if (UsbCore->BaseAddr != USB3_PRI_USB30_PRIM_BASE) {
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  //Disable QMP PIPE3 clock branch: turn on the QMP PIPE3 clock output to the controller while the PHY is in 
  //the reset state (prior to releasing PHY_START and PHY_SW_RESET) since this clock also comes back to the 
  //PHY and needs to be sourced from the PHY pipe clock output when the PHY starts up.
  HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_OFFS, ~(UINT32)HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ENABLE_BMSK);

  // Reset the PHY block (no GCC reset if DP Bonded Mode is enabled)
  if (gUSBPriDPBonded) {
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OFFS, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OFFS, ~HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
  } else {
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OFFS, HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OFFS, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_OFFS, ~HWIO_GCC_USB3PHY_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_OFFS, ~HWIO_GCC_USB3_PHY_PRIM_SP0_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
  }
  

  //Enable QMP PIPE3 clock branch
  HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_OFFS, HWIO_GCC_USB3_PRIM_PHY_PIPE_CBCR_CLK_ENABLE_BMSK);

  if(!usb_shared_ss_phy_init(UsbCore->LaneNum)) {
    DBG(EFI_D_ERROR, "Init SSUSBPhy Enable Error");
    Status = EFI_DEVICE_ERROR; 
    goto ON_EXIT;
  }

ON_EXIT:

  FNC_LEAVE_MSG ();
  return Status;
}


 /**
  Initializes Super Speed Secondary USB phy

  @retval EFI_SUCCESS            Super Speed USB phy  initialized
                                 successfully.

**/
STATIC
EFI_STATUS
InitSSUSBPhySec (
  IN QCOM_USB_CORE              *UsbCore
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG ();

  // not needed for Sdm845 PRE-SI platform
  if ((PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI)   ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_VIRTIO) ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    DBG(EFI_D_WARN, "Pre-SI: Bypass InitSSUSBPhySec");
    goto ON_EXIT;
  }

  if (UsbCore->BaseAddr != USB3_2ND_USB30_PRIM_BASE) {
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  //Disable QMP PIPE3 clock branch: turn on the QMP PIPE3 clock output to the controller while the PHY is in 
  //the reset state (prior to releasing PHY_START and PHY_SW_RESET) since this clock also comes back to the 
  //PHY and needs to be sourced from the PHY pipe clock output when the PHY starts up.
  HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_OFFS, ~(UINT32)HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ENABLE_BMSK);

  // Reset the PHY block (no GCC reset if DP Bonded Mode is enabled)
  if (gUSBSecDPBonded) {
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OFFS, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OFFS, ~HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
  } else {
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_OFFS, HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OFFS, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_OFFS, ~HWIO_GCC_USB3PHY_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_OFFS, ~HWIO_GCC_USB3_PHY_PRIM_SP1_BCR_BLK_ARES_BMSK);
    gBS->Stall (100);
  }
  


  //Enable QMP PIPE3 clock branch
  HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_OFFS, HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ENABLE_BMSK);

  if(!usb_shared_ss_phy_init_secDP(UsbCore->LaneNum)) {
    DBG(EFI_D_ERROR, "Init SSUSBPhySec Enable Error");
    Status = EFI_DEVICE_ERROR; 
    goto ON_EXIT;
  }

ON_EXIT:

  FNC_LEAVE_MSG ();
  return Status;
}


/**
  Polls SS PHY USB Core for training

  @retval EFI_SUCCESS            Poll did not encounter any errors

**/
STATIC
EFI_STATUS
SSUsb1PollSSPhyTraining (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  usb_shared_poll_ss_phy_training();
  return EFI_SUCCESS;
}

/**
  Polls SS PHY USB Core for training

  @retval EFI_SUCCESS            Poll did not encounter any errors

**/
STATIC
EFI_STATUS
SSUsb2PollSSPhyTraining (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  usb_shared_poll_ss_phy_training_sec();
  return EFI_SUCCESS;
}

/**
  Prints SS compliance pattern on a core

  @retval none.

**/
VOID UsbPrintSSCmplPattern(
  IN QCOM_USB_CORE_NUM_SDM_1000 CoreNum,
  IN UINT8 CpNum
  )
{
    switch(CpNum) {
    case  0: DBG (EFI_D_WARN, "CoreNum = %d, CP0 - 5G Pseudo random data pattern", CoreNum); break;
    case  1: DBG (EFI_D_WARN, "CoreNum = %d, CP1 - Nyquist frequency 5Gbps", CoreNum); break;
    case  2: DBG (EFI_D_WARN, "CoreNum = %d, CP2 - Nyquist/2 i.e. 2.5Gbps", CoreNum); break;
    case  3: DBG (EFI_D_WARN, "CoreNum = %d, CP3 - COM pattern", CoreNum); break;
    case  4: DBG (EFI_D_WARN, "CoreNum = %d, CP4 - Low frequency periodic signaling pattern", CoreNum); break;
    case  5: DBG (EFI_D_WARN, "CoreNum = %d, CP5 - K28.7 with de-emphasis", CoreNum); break;
    case  6: DBG (EFI_D_WARN, "CoreNum = %d, CP6 - K28.7 without de-emphasis", CoreNum); break;
    case  7: DBG (EFI_D_WARN, "CoreNum = %d, CP7 - 50-250 repeating 1s and 0s with de-emphasis ", CoreNum); break;
    case  8: DBG (EFI_D_WARN, "CoreNum = %d, CP8 - 50-250 repeating 1s and 0s without de-emphasis ", CoreNum); break;
    case  9: DBG (EFI_D_WARN, "CoreNum = %d, CP9 - 10G Pseudo random data pattern", CoreNum); break;
    case 10: DBG (EFI_D_WARN, "CoreNum = %d, CP10 - Nyquist frequency 10Gbps", CoreNum); break;
    case 11: DBG (EFI_D_WARN, "CoreNum = %d, CP11 - Nyquist/2 i.e. 5Gbps", CoreNum); break;
    case 12: DBG (EFI_D_WARN, "CoreNum = %d, CP9 - 10G Pseudo random data pattern", CoreNum); break;
    case 13: DBG (EFI_D_WARN, "CoreNum = %d, CP12 - Uncoded LFSR15 for phy level testing", CoreNum); break;
    case 14: DBG (EFI_D_WARN, "CoreNum = %d, CP12 - Uncoded LFSR15 for phy level testing", CoreNum); break;
    case 15: DBG (EFI_D_WARN, "CoreNum = %d, CP12 - Uncoded LFSR15 for phy level testing", CoreNum); break;
    case 16: DBG (EFI_D_WARN, "CoreNum = %d, CP12 - Uncoded LFSR15 for phy level testing", CoreNum); break;
  }
}

/**
  Advances SS compliance pattern

  @retval EFI_SUCCESS           If write to hardware register was successful.

**/
STATIC
EFI_STATUS
Usb1AdvanceSSCmplPattern (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  USB3_INC_CMPL_PATTERN(Usb1HstCmplPattNum);

  HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OUTMI(0, HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_BMSK, 
    0x1 << HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_SHFT);

  // Set this bit to 0, when you run compliance tests.
  HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OUTMI(0, HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_BMSK, 
    0x0 << HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_SHFT);

  
  UsbPrintSSCmplPattern(USB_CORE_0_SDM_1000, Usb1HstCmplPattNum);

  return EFI_SUCCESS;
}

/**
  Advances SS compliance pattern

  @retval EFI_SUCCESS           If write to hardware register was successful.

**/
STATIC
EFI_STATUS
Usb2AdvanceSSCmplPattern (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  USB3_INC_CMPL_PATTERN(Usb2HstCmplPattNum);

  HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OUTMI(0, HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_BMSK, 
    0x1 << HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_SHFT);

  // Set this bit to 0, when you run compliance tests.
  HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OUTMI(0, HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_BMSK, 
    0x0 << HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_SHFT);

  UsbPrintSSCmplPattern(USB_CORE_1_SDM_1000, Usb2HstCmplPattNum);
  return EFI_SUCCESS;
}

/**
  Advances SS compliance pattern

  @retval EFI_SUCCESS           If write to hardware register was successful.

**/
STATIC
EFI_STATUS
Usb3AdvanceSSCmplPattern (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  USB3_INC_CMPL_PATTERN(Usb3HstCmplPattNum);

  // For multi-port core, toggle compliance bit on port #0 and port #1.
  HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OUTMI(0, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_BMSK, 
    0x1 << HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_SHFT);

  HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OUTMI(1, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_BMSK, 
    0x1 << HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_SHFT);

  // Set this bit to 0, when you run compliance tests.
  HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OUTMI(0, HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_BMSK, 
    0x0 << HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_SHFT);

  HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OUTMI(1, HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_BMSK, 
    0x0 << HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_HSTPRTCMPL_SHFT);

  UsbPrintSSCmplPattern(USB_CORE_2_SDM_1000, Usb3HstCmplPattNum);
  
  return EFI_SUCCESS;
}


/**
  Initializes Super Speed MP Core USB phy

  @retval EFI_SUCCESS            Super Speed USB phy  initialized
                                 successfully.

**/
STATIC
EFI_STATUS
InitSSUSBPhyMP (
  IN QCOM_USB_CORE              *UsbCore
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  FNC_ENTER_MSG ();

  // not needed for Sdm845 PRE-SI platform
  if ((PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI)   ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_VIRTIO) ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    DBG(EFI_D_WARN, "Pre-SI: Bypass InitSSUSBPhySec");
    goto ON_EXIT;
  }

  if (UsbCore->BaseAddr != USB3_MP_USB31_SCND_BASE) {
    Status = EFI_UNSUPPORTED;
    goto ON_EXIT;
  }

  //Disable QMP PIPE3 clock branch: turn on the QMP PIPE3 clock output to the controller while the PHY is in 
  //the reset state (prior to releasing PHY_START and PHY_SW_RESET) since this clock also comes back to the 
  //PHY and needs to be sourced from the PHY pipe clock output when the PHY starts up.
  //HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_OFFS, ~(UINT32)HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ENABLE_BMSK);

  // Reset the PHY block
  //HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_SEC_BCR_OFFS, HWIO_GCC_USB3_PHY_SEC_BCR_BLK_ARES_BMSK);
  //HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_SEC_BCR_OFFS, HWIO_GCC_USB3PHY_PHY_SEC_BCR_BLK_ARES_BMSK);
  //gBS->Stall (100);
  //HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_SEC_BCR_OFFS, ~HWIO_GCC_USB3PHY_PHY_SEC_BCR_BLK_ARES_BMSK);
  //HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_SEC_BCR_OFFS, ~HWIO_GCC_USB3_PHY_SEC_BCR_BLK_ARES_BMSK);
  //gBS->Stall (100);

  // TBD - Validate these are correct for Poipu
  // Dont see Phy reset for QMP phy, just see PHY_PHY reset

  //Disable QMP PIPE3 clock branch: turn on the QMP PIPE3 clock output to the controller while the PHY is in 
  //the reset state (prior to releasing PHY_START and PHY_SW_RESET) since this clock also comes back to the 
  HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_OFFS, ~(UINT32)HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ENABLE_BMSK);
  HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_OFFS, ~(UINT32)HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ENABLE_BMSK);

  // SS Phy1 Reset
  HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_UNIPHY_MP0_BCR_OFFS, HWIO_GCC_USB3_UNIPHY_MP0_BCR_BLK_ARES_BMSK);
  HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_OFFS, HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_BLK_ARES_BMSK);
  // SSPhy 2 Reset
  HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_UNIPHY_MP1_BCR_OFFS, HWIO_GCC_USB3_UNIPHY_MP1_BCR_BLK_ARES_BMSK);
  HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_OFFS, HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_BLK_ARES_BMSK);
  gBS->Stall (100);
  // SSPhy 2 Out of Reset
  HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_OFFS, ~HWIO_GCC_USB3UNIPHY_PHY_MP1_BCR_BLK_ARES_BMSK);
  HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_UNIPHY_MP1_BCR_OFFS, ~HWIO_GCC_USB3_UNIPHY_MP1_BCR_BLK_ARES_BMSK);

  // SSPhy 1 Out of Reset 
  HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_OFFS, ~HWIO_GCC_USB3UNIPHY_PHY_MP0_BCR_BLK_ARES_BMSK);
  HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_UNIPHY_MP0_BCR_OFFS, ~HWIO_GCC_USB3_UNIPHY_MP0_BCR_BLK_ARES_BMSK);

  gBS->Stall (100);

  //Enable QMP PIPE3 clock branch
  HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_MP_PHY_PIPE_0_CBCR_OFFS, HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ENABLE_BMSK);
  HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_MP_PHY_PIPE_1_CBCR_OFFS, HWIO_GCC_USB3_MP_PHY_COM_AUX_CBCR_CLK_ENABLE_BMSK);


  //Enable QMP PIPE3 clock branch
  //HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_OFFS, HWIO_GCC_USB3_SEC_PHY_PIPE_CBCR_CLK_ENABLE_BMSK);

  // Add MP Core Initialization Seq in shared library
  if(!usb_shared_ss_phy_init_MP0()||!usb_shared_ss_phy_init_MP1()) {
    DBG(EFI_D_ERROR, "Init SSUSBPhyMP Enable Error");
    Status = EFI_DEVICE_ERROR; 
    goto ON_EXIT;
  }

ON_EXIT:

  FNC_LEAVE_MSG ();
  return Status;
}





/**
  Refer UsbConfigUsbCoresPrivate.h for the header description
 **/
STATIC
EFI_STATUS
SSUsb1InitCommon (
  IN  QCOM_USB_CORE_INTERFACE         *This,
  IN  BOOLEAN                          ResetUsbOnly
  )
{
  EFI_STATUS     Status   =  EFI_SUCCESS;
  BOOLEAN        bSsPhyFailed = FALSE;
  UINTN          BaseAddr;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG ();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  if (!ClockProtocol) goto ON_EXIT;

  UsbCore = USB_CORE_FROM_IFC(This);
  
  BaseAddr = UsbCore->BaseAddr;

  if (!ResetUsbOnly) {
    // Init Dal Lib
    DALSYS_InitMod(NULL);

    //Turn on power rails
    if (UsbCore->CoreType == USB_CONFIG_SSUSB1)
    {
      if (NULL != gPmicNpaClientSS1)
      {
        npa_issue_required_request(gPmicNpaClientSS1, PMIC_NPA_MODE_ID_GENERIC_ACTIVE);   //Sends the request to RPM, no return
        DBG(EFI_D_INFO, "NPA Request to set SS1 PMIC_NPA_MODE_ID_GENERIC_ACTIVE");
      }
      else {
      Status = EFI_INVALID_PARAMETER;
      WRN_CHK("USB NPA SS1 Client unavailable.Aborting");
      }
    }

    //Enable USB power domain - TBD (Check) - Clock team to debug further
    ConfigPowerDomain(TRUE, SSUsb1PowerDomain);

    if (UsbCore->CoreType == USB_CONFIG_SSUSB1)
    {
      //Vote for Bus
      if (gNpaClientSS1Bus) {
        npa_issue_vector_request( gNpaClientSS1Bus,
                                 (sizeof(usb_icb_request)/sizeof(npa_resource_state)), 
                                 ((npa_resource_state *)usb_icb_request) );
      }
      else {
        DBG (EFI_D_ERROR, "gNpaClientSS1Bus is NULL)");
      }
    }


 
    // Enable clocks for super speed controller
    Status = InitUsbClocks (UsbCore->EnClockCount, UsbCore->EnClocks);
    ERR_CHK ("failed to initialize USB clocks");
    
    //Increment reference count
    UsbCore->ClkRefCnt++;
  }

  /*3. 
    To avoid ambiguity in the initial state, put the core in power collapse, then bring the core out of power collapse
    Applies to primary core only
  */
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_PRIM_GDSCR_OFFS, HWIO_GCC_USB30_PRIM_GDSCR_SW_COLLAPSE_BMSK);
    gBS->Stall (100);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_PRIM_GDSCR_OFFS, ~HWIO_GCC_USB30_PRIM_GDSCR_SW_COLLAPSE_BMSK);
    gBS->Stall (100);
  
  // Hard Reset the USB Link and PHYs using GCC control
  SynopsysGccReset (UsbCore);

  //GCC_USB_SS_PHY_LDO_EN is enabled by default

  //Write 0x11 to the AHB2PHY bridge CSR PERIPH_SS_AHB2PHY_TOP_CFG 
  //to enable one-wait-state writes and reads prior to writing or reading the QMP USB3 PHY CSRs.
  // TBD - confirm this one
  // HAL_USB_WRITE (AHB2PHY_SOUTH_AHB2PHY_SOUTH_BASE, HWIO_AHB2PHY_SOUTH_AHB2PHY_SOUTH_AHB2PHY_TOP_CFG_OFFS, 0x11);

  Status = InitHSUSBPhy(UsbCore);
  WRN_CHK("InitHSUSBPhy failed: 0x%08x", Status);

  // Dont enable the SS USB Phy for the high speed only configuration
  if(!gHighSpeedOnlyPrimary)
  {
    Status = InitSSUSBPhy(UsbCore);
    if (Status != EFI_SUCCESS)
    {
      bSsPhyFailed = TRUE;
      DBG( EFI_D_WARN, "InitSSUSBPhy failed: 0x%08x. Falling back to HS PHY only", Status);
      Status = EFI_SUCCESS;
    }
  }

  /*
     11. HS only: if operating without the SS PHY, follow this sequence to disable the pipe clock requirement.
    a. usb30_reg_general_cfg[PIPE_UTMI_CLK_DIS] = 0x1
    b. Wait 1 s.
    c. usb30_reg_general_cfg[PIPE_UTMI_CLK_SEL] = 0x1
    d. usb30_reg_general_cfg [PIPE3_PHYSTATUS_SW] = 0x1
    e. Wait 1 s.
    f. usb30_reg_general_cfg[PIPE_UTMI_CLK_DIS] = 0x0
    In this case, program the software to skip all SuperSpeed configurations in 
    the link controller and PHY, otherwise the core could get stuck.
  */
  if ((((UsbCore->CoreType == USB_CONFIG_SSUSB1) && bSsPhyFailed)) || gHighSpeedOnlyPrimary)
  {
    DBG( EFI_D_INFO, "Disabling SS PHY");
    HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GENERAL_CFG_OFFS, HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_DIS_BMSK);
    gBS->Stall (MICRO_STALL_UNITS(10));
    HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GENERAL_CFG_OFFS, HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_SEL_BMSK);
    HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GENERAL_CFG_OFFS, HWIO_USB3_PRI_GENERAL_CFG_PIPE3_PHYSTATUS_SW_BMSK);
    //When PIPE3_PHYSTATUS is high, Setting the PIPE3_SET_PHYSTATUS_SW bit will assert the PIPE_PHYSTATUS controller input 
    //So clear PHY status here
    HAL_USB_AND (BaseAddr, HWIO_USB3_PRI_GENERAL_CFG_OFFS, ~HWIO_USB3_PRI_GENERAL_CFG_PIPE3_SET_PHYSTATUS_SW_BMSK);
    gBS->Stall (MICRO_STALL_UNITS(10));
    HAL_USB_AND (BaseAddr, HWIO_USB3_PRI_GENERAL_CFG_OFFS, ~HWIO_USB3_PRI_GENERAL_CFG_PIPE_UTMI_CLK_DIS_BMSK);
  }
  else
  {
    /* Set DELAYP1TRANS to 0 */
    HAL_USB_AND (BaseAddr, HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), ~HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_DELAYP1TRANS_BMSK);

    /* Set GUSB3PIPECTL[UX_EXIT_IN_PX] to 1 */
    HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_UX_EXIT_IN_PX_BMSK);
  }

  /*
    15. To save power, enable the hardware-based clock gating (not relevant for PBL):
    a. usb30_reg_cgctl[DBM_FSM_EN] = 0x1
  */
  HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_CGCTL_REG_OFFS, HWIO_USB3_PRI_CGCTL_REG_DBM_FSM_EN_BMSK);

  // Disable clock gating: DWC_USB3_GCTL.DSBLCLKGTNG = 1
  HAL_USB_OR(BaseAddr, HWIO_USB3_PRI_GCTL_OFFS, HWIO_USB3_PRI_GCTL_DSBLCLKGTNG_BMSK);

  // Allow PHY to transition to P2 from suspend (P3) state.
  HAL_USB_OR(BaseAddr, HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_P3EXSIGP2_BMSK);
  HAL_USB_OR(BaseAddr, HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_PRI_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_UX_EXIT_IN_PX_BMSK);

  // Reduce U3 exit handshake timer to 300ns.
  HAL_USB_OUTM(BaseAddr, HWIO_USB3_PRI_LINK_REGS_p_LU3LFPSRXTIM_OFFS(0),
    HWIO_USB3_PRI_LINK_REGS_p_LU3LFPSRXTIM_GEN1_U3_EXIT_RSP_RX_CLK_BMSK,
    (0x5) << HWIO_USB3_PRI_LINK_REGS_p_LU3LFPSRXTIM_GEN1_U3_EXIT_RSP_RX_CLK_SHFT);

  HAL_USB_OUTM(BaseAddr, HWIO_USB3_PRI_LINK_REGS_p_LU3LFPSRXTIM_OFFS(0),
    HWIO_USB3_PRI_LINK_REGS_p_LU3LFPSRXTIM_GEN2_U3_EXIT_RSP_RX_CLK_BMSK,
    (0x6) << HWIO_USB3_PRI_LINK_REGS_p_LU3LFPSRXTIM_GEN2_U3_EXIT_RSP_RX_CLK_SHFT);

  /* HW automatic L1 exit feature in device mode to 1 */
  HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GUCTL1_OFFS, HWIO_USB3_PRI_GUCTL1_DEV_L1_EXIT_BY_HW_BMSK);

  /* Decouple L1 entry and exit wakeup event from L2 event */
  HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GUCTL1_OFFS, HWIO_USB3_PRI_GUCTL1_DEV_DECOUPLE_L1L2_EVT_BMSK);

  /* Disable turning off PLL during L1 Sleep to prevent PLL not bring enabled during EP Command  */
  HAL_USB_AND (BaseAddr, HWIO_USB3_PRI_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_OFFS(0), 
    ~HWIO_USB3_PRI_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_ENBLSLPM_BMSK);

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
} // end SynopsysInitCommon


/**
  Refer UsbConfigUsbCoresPrivate.h for the header description
 **/
STATIC
EFI_STATUS
SSUsb2InitCommon (
  IN  QCOM_USB_CORE_INTERFACE         *This,
  IN  BOOLEAN                          ResetUsbOnly
  )
{
  EFI_STATUS     Status   =  EFI_SUCCESS;
  BOOLEAN        bSsPhyFailed = FALSE;
  UINTN          BaseAddr;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG ();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  if (!ClockProtocol) goto ON_EXIT;

  UsbCore = USB_CORE_FROM_IFC(This);
  
  BaseAddr = UsbCore->BaseAddr;

  if (!ResetUsbOnly) {
    // Init Dal Lib
    DALSYS_InitMod(NULL);

    //Turn on power rails
    if (UsbCore->CoreType == USB_CONFIG_SSUSB2)
    {
      if (NULL != gPmicNpaClientSS2)
      {                                               
        npa_issue_required_request(gPmicNpaClientSS2, PMIC_NPA_MODE_ID_GENERIC_ACTIVE);   //Sends the request to RPM, no return
        DBG(EFI_D_INFO, "NPA Request to set SS2 PMIC_NPA_MODE_ID_GENERIC_ACTIVE");
      }
      else {
      Status = EFI_INVALID_PARAMETER;
      WRN_CHK("USB NPA SS2 Client unavailable.Aborting");
      }
    }
    
    //Enable USB power domain
    ConfigPowerDomain(TRUE, SSUsb2PowerDomain);

    if (UsbCore->CoreType == USB_CONFIG_SSUSB2)
    { 
      //Vote for Bus
      if (gNpaClientSS2Bus) {
        npa_issue_vector_request( gNpaClientSS2Bus,
                                 (sizeof(usb_icb_request)/sizeof(npa_resource_state)), 
                                 ((npa_resource_state *)usb_icb_request) );
      }
      else {
        DBG (EFI_D_ERROR, "gNpaClientSS2Bus is NULL)");
      }
    }


    // Enable clocks for super speed controller
    Status = InitUsbClocks (UsbCore->EnClockCount, UsbCore->EnClocks);
    ERR_CHK ("failed to initialize USB clocks");
    
    //Increment reference count
    UsbCore->ClkRefCnt++;
  }

  /*3. 
    To avoid ambiguity in the initial state, put the core in power collapse, then bring the core out of power collapse
    Applies to primary core only
  */
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_SEC_GDSCR_OFFS, HWIO_GCC_USB30_SEC_GDSCR_SW_COLLAPSE_BMSK);
    gBS->Stall (100);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_SEC_GDSCR_OFFS, ~HWIO_GCC_USB30_SEC_GDSCR_SW_COLLAPSE_BMSK);
    gBS->Stall (100);
  
  // Hard Reset the USB Link and PHYs using GCC control
  SynopsysGccReset (UsbCore);

  //GCC_USB_SS_PHY_LDO_EN is enabled by default

  //Write 0x11 to the AHB2PHY bridge CSR PERIPH_SS_AHB2PHY_TOP_CFG 
  //to enable one-wait-state writes and reads prior to writing or reading the QMP USB3 PHY CSRs.
  // HAL_USB_WRITE (AHB2PHY_SOUTH_AHB2PHY_SOUTH_BASE, HWIO_AHB2PHY_SOUTH_AHB2PHY_SOUTH_AHB2PHY_TOP_CFG_OFFS, 0x11);

  Status = InitHSUSBPhySec(UsbCore);
  WRN_CHK("InitHSUSBPhySec failed: 0x%08x", Status);

  // Dont enable the SS USB Phy for the high speed only configuration
  if(!gHighSpeedOnlySecondary)
  {
    Status = InitSSUSBPhySec(UsbCore);
    if (Status != EFI_SUCCESS)
    {
      bSsPhyFailed = TRUE;
      DBG( EFI_D_WARN, "InitSSUSBPhySec failed: 0x%08x. Falling back to HS PHY only", Status);
      Status = EFI_SUCCESS;
    }
  }

  /*
   11. HS only: if operating without the SS PHY, follow this sequence to disable the pipe clock requirement.
    a. usb30_reg_general_cfg[PIPE_UTMI_CLK_DIS] = 0x1
    b. Wait 1 s.
    c. usb30_reg_general_cfg[PIPE_UTMI_CLK_SEL] = 0x1
    d. usb30_reg_general_cfg [PIPE3_PHYSTATUS_SW] = 0x1
    e. Wait 1 s.
    f. usb30_reg_general_cfg[PIPE_UTMI_CLK_DIS] = 0x0
    In this case, program the software to skip all SuperSpeed configurations in 
    the link controller and PHY, otherwise the core could get stuck.
  */
  if ((((UsbCore->CoreType == USB_CONFIG_SSUSB2) && bSsPhyFailed)) || gHighSpeedOnlySecondary)
  {
    DBG( EFI_D_INFO, "Disabling SS PHY");
    HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GENERAL_CFG_OFFS, HWIO_USB3_2ND_GENERAL_CFG_PIPE_UTMI_CLK_DIS_BMSK);
    gBS->Stall (MICRO_STALL_UNITS(10));
    HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GENERAL_CFG_OFFS, HWIO_USB3_2ND_GENERAL_CFG_PIPE_UTMI_CLK_SEL_BMSK);
    HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GENERAL_CFG_OFFS, HWIO_USB3_2ND_GENERAL_CFG_PIPE3_PHYSTATUS_SW_BMSK);
    //When PIPE3_PHYSTATUS is high, Setting the PIPE3_SET_PHYSTATUS_SW bit will assert the PIPE_PHYSTATUS controller input 
    //So clear PHY status here
    HAL_USB_AND (BaseAddr, HWIO_USB3_2ND_GENERAL_CFG_OFFS, ~HWIO_USB3_2ND_GENERAL_CFG_PIPE3_SET_PHYSTATUS_SW_BMSK);
    gBS->Stall (MICRO_STALL_UNITS(10));
    HAL_USB_AND (BaseAddr, HWIO_USB3_2ND_GENERAL_CFG_OFFS, ~HWIO_USB3_2ND_GENERAL_CFG_PIPE_UTMI_CLK_DIS_BMSK);
  }
  else
  {
    /* Set DELAYP1TRANS to 0 */
    HAL_USB_AND (BaseAddr, HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), ~HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_DELAYP1TRANS_BMSK);

    /* Set GUSB3PIPECTL[UX_EXIT_IN_PX] to 1 */
    HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_UX_EXIT_IN_PX_BMSK);
  }
  
  /*
    15. To save power, enable the hardware-based clock gating (not relevant for PBL):
    a. usb30_reg_cgctl[DBM_FSM_EN] = 0x1
  */
  HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_CGCTL_REG_OFFS, HWIO_USB3_2ND_CGCTL_REG_DBM_FSM_EN_BMSK);
  
  // Disable clock gating: DWC_USB3_GCTL.DSBLCLKGTNG = 1
  HAL_USB_OR(BaseAddr, HWIO_USB3_2ND_GCTL_OFFS, HWIO_USB3_2ND_GCTL_DSBLCLKGTNG_BMSK);
  
  // Allow PHY to transition to P2 from suspend (P3) state.
  HAL_USB_OR(BaseAddr, HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_P3EXSIGP2_BMSK);
  HAL_USB_OR(BaseAddr, HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_2ND_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_UX_EXIT_IN_PX_BMSK);
  
  // Reduce U3 exit handshake timer to 300ns.
  HAL_USB_OUTM(BaseAddr, HWIO_USB3_2ND_LINK_REGS_p_LU3LFPSRXTIM_OFFS(0),
    HWIO_USB3_2ND_LINK_REGS_p_LU3LFPSRXTIM_GEN1_U3_EXIT_RSP_RX_CLK_BMSK,
    (0x5) << HWIO_USB3_2ND_LINK_REGS_p_LU3LFPSRXTIM_GEN1_U3_EXIT_RSP_RX_CLK_SHFT);
  
  HAL_USB_OUTM(BaseAddr, HWIO_USB3_2ND_LINK_REGS_p_LU3LFPSRXTIM_OFFS(0),
    HWIO_USB3_2ND_LINK_REGS_p_LU3LFPSRXTIM_GEN2_U3_EXIT_RSP_RX_CLK_BMSK,
    (0x6) << HWIO_USB3_2ND_LINK_REGS_p_LU3LFPSRXTIM_GEN2_U3_EXIT_RSP_RX_CLK_SHFT);
  
  /* HW automatic L1 exit feature in device mode to 1 */
  HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GUCTL1_OFFS, HWIO_USB3_2ND_GUCTL1_DEV_L1_EXIT_BY_HW_BMSK);
  
  /* Decouple L1 entry and exit wakeup event from L2 event */
  HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GUCTL1_OFFS, HWIO_USB3_2ND_GUCTL1_DEV_DECOUPLE_L1L2_EVT_BMSK);
  
  /* Disable turning off PLL during L1 Sleep to prevent PLL not bring enabled during EP Command  */
  HAL_USB_AND (BaseAddr, HWIO_USB3_2ND_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_OFFS(0), 
    ~HWIO_USB3_2ND_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_ENBLSLPM_BMSK);
  
ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
} // end SynopsysInitCommon


/**
  Refer UsbConfigUsbCoresPrivate.h for the header description
 **/
STATIC
EFI_STATUS
SSUsb3InitCommon (
  IN  QCOM_USB_CORE_INTERFACE         *This,
  IN  BOOLEAN                          ResetUsbOnly
  )
{
  EFI_STATUS     Status   =  EFI_SUCCESS;
  BOOLEAN        bSsPhyFailed = FALSE;
  UINTN          BaseAddr;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG ();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  if (!ClockProtocol) goto ON_EXIT;

  UsbCore = USB_CORE_FROM_IFC(This);
  
  BaseAddr = UsbCore->BaseAddr;

   //TBD - Update for MP Core
   
  if (!ResetUsbOnly) {
    // Init Dal Lib
    DALSYS_InitMod(NULL);

    //Turn on power rails
    if (UsbCore->CoreType == USB_CONFIG_SSUSB3)
    {
      if (NULL != gPmicNpaClientSS3)
      {                                               
        npa_issue_required_request(gPmicNpaClientSS3, PMIC_NPA_MODE_ID_GENERIC_ACTIVE);   //Sends the request to RPM, no return
        DBG(EFI_D_INFO, "NPA Request to set SS3 PMIC_NPA_MODE_ID_GENERIC_ACTIVE");
      }
      else {
      Status = EFI_INVALID_PARAMETER;
      WRN_CHK("USB NPA SS3 Client unavailable.Aborting");
      }
    }
    
    //Enable USB power domain
    ConfigPowerDomain(TRUE, SSUsb3PowerDomain);

    if (UsbCore->CoreType == USB_CONFIG_SSUSB3)
    { 
      // TBD - modify them
      //Vote for Bus
      if (gNpaClientSS3Bus) {
        npa_issue_vector_request( gNpaClientSS3Bus,
                                 (sizeof(usb_icb_request)/sizeof(npa_resource_state)), 
                                 ((npa_resource_state *)usb_icb_request) );
      }
      else {
        DBG (EFI_D_ERROR, "gNpaClientSS3Bus is NULL)");
      }
    }


    // Enable clocks for super speed controller
    Status = InitUsbClocks (UsbCore->EnClockCount, UsbCore->EnClocks);
    ERR_CHK ("failed to initialize USB clocks");
    
    //Increment reference count
    UsbCore->ClkRefCnt++;
  }

  /*3. 
    To avoid ambiguity in the initial state, put the core in power collapse, then bring the core out of power collapse
  */
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_MP_GDSCR_OFFS, HWIO_GCC_USB30_MP_GDSCR_SW_COLLAPSE_BMSK);
    gBS->Stall (100);
    HAL_USB_AND (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_MP_GDSCR_OFFS, ~HWIO_GCC_USB30_MP_GDSCR_SW_COLLAPSE_BMSK);
    gBS->Stall (100);
  
  // Hard Reset the USB Link and PHYs using GCC control
  SynopsysGccReset (UsbCore);

  //GCC_USB_SS_PHY_LDO_EN is enabled by default

  //Write 0x11 to the AHB2PHY bridge CSR PERIPH_SS_AHB2PHY_TOP_CFG 
  //to enable one-wait-state writes and reads prior to writing or reading the QMP USB3 PHY CSRs.
  //HAL_USB_WRITE (AHB2PHY_SOUTH_AHB2PHY_SOUTH_BASE, HWIO_AHB2PHY_SOUTH_AHB2PHY_SOUTH_AHB2PHY_TOP_CFG_OFFS, 0x11);

  Status = InitHSUSBPhyMP(UsbCore);
  WRN_CHK("InitHSUSBPhy MP Core Primary/Sec phy failed: 0x%08x", Status);


  // Dont enable the SS USB Phy for the high speed only configuration
  if (!FeaturePcdGet(HighSpeedOnlySupported))
  {
    // TBD - implement later
    Status = InitSSUSBPhyMP(UsbCore);
    if (Status != EFI_SUCCESS)
    {
      bSsPhyFailed = TRUE;
      DBG( EFI_D_WARN, "InitSSUSBPhySec failed: 0x%08x. Falling back to HS PHY only", Status);
      Status = EFI_SUCCESS;
    }
  }

  /*
   11. HS only: if operating without the SS PHY, follow this sequence to disable the pipe clock requirement.
    a. usb30_reg_general_cfg[PIPE_UTMI_CLK_DIS] = 0x1
    b. Wait 1 s.
    c. usb30_reg_general_cfg[PIPE_UTMI_CLK_SEL] = 0x1
    d. usb30_reg_general_cfg [PIPE3_PHYSTATUS_SW] = 0x1
    e. Wait 1 s.
    f. usb30_reg_general_cfg[PIPE_UTMI_CLK_DIS] = 0x0
    In this case, program the software to skip all SuperSpeed configurations in 
    the link controller and PHY, otherwise the core could get stuck.
  */
  if ((((UsbCore->CoreType == USB_CONFIG_SSUSB3) && bSsPhyFailed)) || (FeaturePcdGet(HighSpeedOnlySupported)))
  {
    DBG( EFI_D_INFO, "Disabling SS PHY 0");
    HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, HWIO_USB3_MP_GENERAL_CFG_PIPE_UTMI_CLK_DIS_0_BMSK);
    gBS->Stall (MICRO_STALL_UNITS(10));
    HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, HWIO_USB3_MP_GENERAL_CFG_PIPE_UTMI_CLK_SEL_0_BMSK);
    HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, HWIO_USB3_MP_GENERAL_CFG_PIPE3_PHYSTATUS_SW_0_BMSK);
    //When PIPE3_PHYSTATUS is high, Setting the PIPE3_SET_PHYSTATUS_SW bit will assert the PIPE_PHYSTATUS controller input 
    //So clear PHY status here
    HAL_USB_AND (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, ~HWIO_USB3_MP_GENERAL_CFG_PIPE3_SET_PHYSTATUS_SW_0_BMSK);
    gBS->Stall (MICRO_STALL_UNITS(10));
    HAL_USB_AND (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, ~HWIO_USB3_MP_GENERAL_CFG_PIPE_UTMI_CLK_DIS_0_BMSK);

    DBG( EFI_D_INFO, "Disabling SS PHY 1");
    HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, HWIO_USB3_MP_GENERAL_CFG_PIPE_UTMI_CLK_DIS_1_BMSK);
    gBS->Stall (MICRO_STALL_UNITS(10));
    HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, HWIO_USB3_MP_GENERAL_CFG_PIPE_UTMI_CLK_SEL_1_BMSK);
    HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, HWIO_USB3_MP_GENERAL_CFG_PIPE3_PHYSTATUS_SW_1_BMSK);
    //When PIPE3_PHYSTATUS is high, Setting the PIPE3_SET_PHYSTATUS_SW bit will assert the PIPE_PHYSTATUS controller input 
    //So clear PHY status here
    HAL_USB_AND (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, ~HWIO_USB3_MP_GENERAL_CFG_PIPE3_SET_PHYSTATUS_SW_1_BMSK);
    gBS->Stall (MICRO_STALL_UNITS(10));
    HAL_USB_AND (BaseAddr, HWIO_USB3_MP_GENERAL_CFG_OFFS, ~HWIO_USB3_MP_GENERAL_CFG_PIPE_UTMI_CLK_DIS_1_BMSK);
  }
  else
  {
    /* Set DELAYP1TRANS to 0 */
    HAL_USB_AND (BaseAddr, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), ~HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_DELAYP1TRANS_BMSK);
    HAL_USB_AND (BaseAddr, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(1), ~HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_DELAYP1TRANS_BMSK);

    /* Set GUSB3PIPECTL[UX_EXIT_IN_PX] to 1 */
    HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_UX_EXIT_IN_PX_BMSK);
    HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(1), HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_UX_EXIT_IN_PX_BMSK);
  }
  
  /*
    15. To save power, enable the hardware-based clock gating (not relevant for PBL):
    a. usb30_reg_cgctl[DBM_FSM_EN] = 0x1
  */
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_CGCTL_REG_OFFS, HWIO_USB3_MP_CGCTL_REG_DBM_FSM_EN_BMSK);
  
  // Disable clock gating: DWC_USB3_GCTL.DSBLCLKGTNG = 1
  HAL_USB_OR(BaseAddr, HWIO_USB3_MP_GCTL_OFFS, HWIO_USB3_MP_GCTL_DSBLCLKGTNG_BMSK);
  
  // Allow PHY to transition to P2 from suspend (P3) state.
  HAL_USB_OR(BaseAddr, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_P3EXSIGP2_BMSK);
  HAL_USB_OR(BaseAddr, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(0), HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_UX_EXIT_IN_PX_BMSK);
  HAL_USB_OR(BaseAddr, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(1), HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_P3EXSIGP2_BMSK);
  HAL_USB_OR(BaseAddr, HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_OFFS(1), HWIO_USB3_MP_GUSB3PIPECTL_REGS_p_GUSB3PIPECTL_UX_EXIT_IN_PX_BMSK);
  
  // Reduce U3 exit handshake timer to 300ns.
  HAL_USB_OUTM(BaseAddr, HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_OFFS(0),
    HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_GEN1_U3_EXIT_RSP_RX_CLK_BMSK,
    (0x5) << HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_GEN1_U3_EXIT_RSP_RX_CLK_SHFT);
  HAL_USB_OUTM(BaseAddr, HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_OFFS(1),
    HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_GEN1_U3_EXIT_RSP_RX_CLK_BMSK,
    (0x5) << HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_GEN1_U3_EXIT_RSP_RX_CLK_SHFT);
  
  HAL_USB_OUTM(BaseAddr, HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_OFFS(0),
    HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_GEN2_U3_EXIT_RSP_RX_CLK_BMSK,
    (0x6) << HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_GEN2_U3_EXIT_RSP_RX_CLK_SHFT);
  HAL_USB_OUTM(BaseAddr, HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_OFFS(1),
    HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_GEN2_U3_EXIT_RSP_RX_CLK_BMSK,
    (0x6) << HWIO_USB3_MP_LINK_REGS_p_LU3LFPSRXTIM_GEN2_U3_EXIT_RSP_RX_CLK_SHFT);
  
  /* HW automatic L1 exit feature in device mode to 1 */
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GUCTL1_OFFS, HWIO_USB3_MP_GUCTL1_DEV_L1_EXIT_BY_HW_BMSK);
  
  /* Decouple L1 entry and exit wakeup event from L2 event */
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GUCTL1_OFFS, HWIO_USB3_MP_GUCTL1_DEV_DECOUPLE_L1L2_EVT_BMSK);
  
  /* Disable turning off PLL during L1 Sleep to prevent PLL not bring enabled during EP Command  */
  HAL_USB_AND (BaseAddr, HWIO_USB3_MP_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_OFFS(0), 
    ~HWIO_USB3_MP_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_ENBLSLPM_BMSK);
  /* Disable turning off PLL during L1 Sleep to prevent PLL not bring enabled during EP Command  */
  HAL_USB_AND (BaseAddr, HWIO_USB3_MP_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_OFFS(1), 
    ~HWIO_USB3_MP_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_ENBLSLPM_BMSK);

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
} // end SynopsysInitCommon

/**
  Performs Host mode specific initialization on a Synopsys super speed USB Primary core.

  @param [in]  UsbCore      The USB core instance

  @retval EFI_SUCCESS       USB core successfully initialized
  @retval EFI_UNSUPPORTED   USB core not supported
  @retval Others            Error encountered initializing USB core
**/
STATIC
EFI_STATUS
SynopsysInitHostConfig (
  IN  QCOM_USB_CORE         *UsbCore
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;
  UINTN       BaseAddr;

  FNC_ENTER_MSG ();

  if (NULL == UsbCore) {
    DBG(EFI_D_ERROR, "invalid parameter");
    Status = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
  }
  
  BaseAddr = UsbCore->BaseAddr;
  
  //2. Set usb30_reg_gctl:[PRTCAPDIR] = 0x1 (host), [U2EXIT_LFPS] = 0x1
  HAL_USB_AND(BaseAddr,HWIO_USB3_PRI_GCTL_OFFS,~HWIO_USB3_PRI_GCTL_PRTCAPDIR_BMSK);
  HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GCTL_OFFS, HWIO_USB3_PRI_GCTL_U2EXIT_LFPS_BMSK | ( (1 << HWIO_USB3_PRI_GCTL_PRTCAPDIR_SHFT)) );

  //3. Enable the wake on connect event for the port:
  HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_PORTSC_20_REGS_p_PORTSC_20_OFFS(0), HWIO_USB3_PRI_PORTSC_20_REGS_p_PORTSC_20_WCE_BMSK );
  HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_PORTSC_30_REGS_p_PORTSC_30_OFFS(0), HWIO_USB3_PRI_PORTSC_30_REGS_p_PORTSC_30_WCE_BMSK );

  //4. Set GRXTHRCFG based on the case 8000615753 values
  HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GRXTHRCFG_OFFS,  (3 << HWIO_USB3_PRI_GRXTHRCFG_USBMAXRXBURSTSIZE_SHFT) |
  (3 << HWIO_USB3_PRI_GRXTHRCFG_USBRXPKTCNT_SHFT) | HWIO_USB3_PRI_GRXTHRCFG_USBRXPKTCNTSEL_BMSK);

  //5. Set the bus configuration 1K page pipe limit:
  //TODO:CK:HWIO_GSBUSCFG1_DESADRSPC_BMSK & HWIO_GSBUSCFG1_DATADRSPC_BMSK is not defined.
  //HAL_USB_AND(BaseAddr, HWIO_GSBUSCFG1_ADDR(0), ~HWIO_GSBUSCFG1_DESADRSPC_BMSK | ~HWIO_GSBUSCFG1_DATADRSPC_BMSK );
  HAL_USB_OR(BaseAddr, HWIO_USB3_PRI_GSBUSCFG1_OFFS, (0xE << HWIO_USB3_PRI_GSBUSCFG1_PIPETRANSLIMIT_SHFT) | HWIO_USB3_PRI_GSBUSCFG1_EN1KPAGE_BMSK );

  /* Disable USB2.0 internal retry feature */
  HAL_USB_OR (BaseAddr, HWIO_USB3_PRI_GUCTL3_OFFS, HWIO_USB3_PRI_GUCTL3_USB20_RETRY_DISABLE_BMSK);

  //set USB GUCTL1[21] = 3 (IP_GAP_ADD_ON)
  //to fix 1.	EL_22 Inter-Packet gap between host and device packet test
  //       2. EL_23 Inter-Packet gap between first two packets test
  HAL_USB_OR(BaseAddr, HWIO_USB3_PRI_GUCTL1_OFFS, (0x3 << HWIO_USB3_PRI_GUCTL1_IP_GAP_ADD_ON_SHFT)& HWIO_USB3_PRI_GUCTL1_IP_GAP_ADD_ON_BMSK);
  

  Usb1HstCmplPattNum = 0;
ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
} // end SynopsysInitHost


/**
  Performs Host mode specific initialization on a Synopsys super speed USB Secondary core.

  @param [in]  UsbCore      The USB core instance

  @retval EFI_SUCCESS       USB core successfully initialized
  @retval EFI_UNSUPPORTED   USB core not supported
  @retval Others            Error encountered initializing USB core
**/
STATIC
EFI_STATUS
SynopsysInitHostConfigSec (
  IN  QCOM_USB_CORE         *UsbCore
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;
  UINTN       BaseAddr;

  FNC_ENTER_MSG ();

  if (NULL == UsbCore) {
    DBG(EFI_D_ERROR, "invalid parameter");
    Status = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
  }
  
  BaseAddr = UsbCore->BaseAddr;
  
  //2. Set usb30_reg_gctl:[PRTCAPDIR] = 0x1 (host), [U2EXIT_LFPS] = 0x1
  HAL_USB_AND(BaseAddr, HWIO_USB3_2ND_GCTL_OFFS, ~HWIO_USB3_2ND_GCTL_PRTCAPDIR_BMSK);
  HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GCTL_OFFS, HWIO_USB3_2ND_GCTL_U2EXIT_LFPS_BMSK | ( (1 << HWIO_USB3_2ND_GCTL_PRTCAPDIR_SHFT)) );

  //3. Enable the wake on connect event for the port:
  HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_PORTSC_20_REGS_p_PORTSC_20_OFFS(0), HWIO_USB3_2ND_PORTSC_20_REGS_p_PORTSC_20_WCE_BMSK );
  HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_PORTSC_30_REGS_p_PORTSC_30_OFFS(0), HWIO_USB3_2ND_PORTSC_30_REGS_p_PORTSC_30_WCE_BMSK );

  //4. Set GRXTHRCFG based on the case 8000615753 values
  HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GRXTHRCFG_OFFS,  (3 << HWIO_USB3_2ND_GRXTHRCFG_USBMAXRXBURSTSIZE_SHFT) |
  (3 << HWIO_USB3_2ND_GRXTHRCFG_USBRXPKTCNT_SHFT) | HWIO_USB3_2ND_GRXTHRCFG_USBRXPKTCNTSEL_BMSK);

  //5. Set the bus configuration 1K page pipe limit:
  //TODO:CK:HWIO_GSBUSCFG1_DESADRSPC_BMSK & HWIO_GSBUSCFG1_DATADRSPC_BMSK is not defined.
  //HAL_USB_AND(BaseAddr, HWIO_GSBUSCFG1_ADDR(0), ~HWIO_GSBUSCFG1_DESADRSPC_BMSK | ~HWIO_GSBUSCFG1_DATADRSPC_BMSK );
  HAL_USB_OR(BaseAddr, HWIO_USB3_2ND_GSBUSCFG1_OFFS, (0xE << HWIO_USB3_2ND_GSBUSCFG1_PIPETRANSLIMIT_SHFT) | HWIO_USB3_2ND_GSBUSCFG1_EN1KPAGE_BMSK );

  /* Disable USB2.0 internal retry feature */
  HAL_USB_OR (BaseAddr, HWIO_USB3_2ND_GUCTL3_OFFS, HWIO_USB3_2ND_GUCTL3_USB20_RETRY_DISABLE_BMSK);

  //set USB GUCTL1[21] = 3 (IP_GAP_ADD_ON)
  //to fix 1.	EL_22 Inter-Packet gap between host and device packet test
  //       2. EL_23 Inter-Packet gap between first two packets test
  HAL_USB_OR(BaseAddr, HWIO_USB3_2ND_GUCTL1_OFFS, (0x3 << HWIO_USB3_2ND_GUCTL1_IP_GAP_ADD_ON_SHFT)& HWIO_USB3_2ND_GUCTL1_IP_GAP_ADD_ON_BMSK);

  Usb2HstCmplPattNum = 0;

ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
} // end SynopsysInitHostSec

/**
  Performs Host mode specific initialization on a Synopsys super speed USB MultiPort core.

  @param [in]  UsbCore      The USB core instance

  @retval EFI_SUCCESS       USB core successfully initialized
  @retval EFI_UNSUPPORTED   USB core not supported
  @retval Others            Error encountered initializing USB core
**/
STATIC
EFI_STATUS
SynopsysInitHostConfigMPCore (
  IN  QCOM_USB_CORE         *UsbCore
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;
  UINTN       BaseAddr;

  FNC_ENTER_MSG ();

  if (NULL == UsbCore) {
    DBG(EFI_D_ERROR, "invalid parameter");
    Status = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
  }
  
  BaseAddr = UsbCore->BaseAddr;
  
  //TBD - Add Stuff for MP Core
  
  //2. Set usb30_reg_gctl:[PRTCAPDIR] = 0x1 (host), [U2EXIT_LFPS] = 0x1
  HAL_USB_AND(BaseAddr, HWIO_USB3_MP_GCTL_OFFS, ~HWIO_USB3_MP_GCTL_PRTCAPDIR_BMSK);
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GCTL_OFFS, HWIO_USB3_MP_GCTL_U2EXIT_LFPS_BMSK | ( (1 << HWIO_USB3_MP_GCTL_PRTCAPDIR_SHFT)) );

  //3. Enable the wake on connect event for the port:
  //Port 0
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_PORTSC_20_REGS_p_PORTSC_20_OFFS(0), HWIO_USB3_MP_PORTSC_20_REGS_p_PORTSC_20_WCE_BMSK );
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_PORTSC_30_REGS_p_PORTSC_30_OFFS(0), HWIO_USB3_MP_PORTSC_30_REGS_p_PORTSC_30_WCE_BMSK );

  //Port -1
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_PORTSC_20_REGS_p_PORTSC_20_OFFS(1), HWIO_USB3_MP_PORTSC_20_REGS_p_PORTSC_20_WCE_BMSK );
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_PORTSC_30_REGS_p_PORTSC_30_OFFS(1), HWIO_USB3_MP_PORTSC_30_REGS_p_PORTSC_30_WCE_BMSK );


  //4. Set GRXTHRCFG based on the case 8000615753 values
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GRXTHRCFG_OFFS,  (3 << HWIO_USB3_MP_GRXTHRCFG_USBMAXRXBURSTSIZE_SHFT) |
  (3 << HWIO_USB3_MP_GRXTHRCFG_USBRXPKTCNT_SHFT) | HWIO_USB3_MP_GRXTHRCFG_USBRXPKTCNTSEL_BMSK);

  //5. Set the bus configuration 1K page pipe limit:
  //TODO:CK:HWIO_GSBUSCFG1_DESADRSPC_BMSK & HWIO_GSBUSCFG1_DATADRSPC_BMSK is not defined.
  //HAL_USB_AND(BaseAddr, HWIO_GSBUSCFG1_ADDR(0), ~HWIO_GSBUSCFG1_DESADRSPC_BMSK | ~HWIO_GSBUSCFG1_DATADRSPC_BMSK );
  HAL_USB_OR(BaseAddr, HWIO_USB3_MP_GSBUSCFG1_OFFS, (0xE << HWIO_USB3_MP_GSBUSCFG1_PIPETRANSLIMIT_SHFT) | HWIO_USB3_MP_GSBUSCFG1_EN1KPAGE_BMSK );

  /* Disable USB2.0 internal retry feature */
  HAL_USB_OR (BaseAddr, HWIO_USB3_MP_GUCTL3_OFFS, HWIO_USB3_MP_GUCTL3_USB20_RETRY_DISABLE_BMSK);

  //set USB GUCTL1[21] = 3 (IP_GAP_ADD_ON)
  //to fix 1.	EL_22 Inter-Packet gap between host and device packet test
  //       2. EL_23 Inter-Packet gap between first two packets test
  HAL_USB_OR(BaseAddr, HWIO_USB3_MP_GUCTL1_OFFS, (0x3 << HWIO_USB3_MP_GUCTL1_IP_GAP_ADD_ON_SHFT)& HWIO_USB3_MP_GUCTL1_IP_GAP_ADD_ON_BMSK);

  Usb3HstCmplPattNum = 0;
ON_EXIT:
  FNC_LEAVE_MSG ();
  return Status;
} // end SynopsysInitHostConfigMPCore



/**
  Refer UsbConfigUsbCoresPrivate.h for the header description
**/
STATIC
EFI_STATUS
Usb30InitHost (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameters");
  }

  UsbCore = USB_CORE_FROM_IFC(This);
  
  /*if (UsbCore->CoreType == USB_CONFIG_SSUSB1) {
    Status = EnableVbusSS1();
    ERR_CHK("Failed to initialize VbusSS1");
  }*/

  if(UsbCore->CoreType == USB_CONFIG_SSUSB1)
  Status = SynopsysInitHostConfig(UsbCore);
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB2)
    Status = SynopsysInitHostConfigSec(UsbCore);
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB3)
    Status = SynopsysInitHostConfigMPCore(UsbCore);
  else
  	DBG(EFI_D_WARN, "Invalid Core Type");

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/**
  Refer UsbConfigUsbCoresPrivate.h for the header description
**/
STATIC
EFI_STATUS
Usb30GetVbusSt (
  IN  QCOM_USB_CORE_INTERFACE         *This,
  OUT UINT32                          *VbusSt
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This || !VbusSt) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameters");
  }

  UsbCore = USB_CORE_FROM_IFC(This);

  // For Clamshell Platform Host Mode Enablement is automatically taken care by EC
  if (IS_CLS_PLAT(PlatformInfo.platform)) {
      Status = EFI_SUCCESS;
      goto ON_EXIT;
  }
  else {
     if (NULL == gPwrCtrlProtocol) {
      // Find the Power Control USB charger protocol
      Status = gBS->LocateProtocol(&gQcomUsbPwrCtrlProtocolGuid, NULL, (void**)&gPwrCtrlProtocol);
      if (EFI_ERROR(Status)) {
        gPwrCtrlProtocol = NULL;
        WRN_CHK("Failed to open Power control USB protocol Status =  (0x%x)", Status);
        goto ON_EXIT;
        }
      }
    
    if(UsbCore->CoreType == USB_CONFIG_SSUSB1)
      Status = gPwrCtrlProtocol->GetVbusSrcOkStatus(USB_CORE_0_SDM_1000, (BOOLEAN *)VbusSt);
    else if(UsbCore->CoreType == USB_CONFIG_SSUSB2)
      Status = gPwrCtrlProtocol->GetVbusSrcOkStatus(USB_CORE_1_SDM_1000, (BOOLEAN *)VbusSt);
    else if(UsbCore->CoreType == USB_CONFIG_SSUSB3)
      Status = gPwrCtrlProtocol->GetVbusSrcOkStatus(USB_CORE_2_SDM_1000, (BOOLEAN *)VbusSt);
    else
    {
      Status = EFI_INVALID_PARAMETER;
      DBG(EFI_D_WARN, "Invalid Core Type");
    }
    
    WRN_CHK ("failed to get Vbus Status, Status = 0x%x", Status);
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/**
  Refer UsbConfigUsbCoresPrivate.h for the header description
**/
STATIC
EFI_STATUS
Usb30EnableVbus (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameters");
  }

  UsbCore = USB_CORE_FROM_IFC(This);
  
  // Enable Vbus for the Primary Core (host mode)
  if(UsbCore->CoreType == USB_CONFIG_SSUSB1) {
    Status = EnableVbusSS(USB_CORE_0_SDM_1000);
    ERR_CHK("Failed to initialize VbusSS for core 1");
  }
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB2) { 
    Status = EnableVbusSS(USB_CORE_1_SDM_1000);
    ERR_CHK("Failed to initialize VbusSS for core 2");
  }
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB3) { 
    //Wont ever come here since PMIC always enables the Vbus for Multi-Port
    Status = EnableVbusSS(USB_CORE_2_SDM_1000);
   Status = EFI_SUCCESS;
  }
  else
    DBG(EFI_D_WARN, "Invalid Core Type");


ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/**
  Performs device mode specific initialization on a Synopsys super speed USB Primary core.

  @param [in]  UsbCore      The USB core instance

  @retval EFI_SUCCESS       USB core successfully initialized
  @retval EFI_UNSUPPORTED   USB core not supported
  @retval Others            Error encountered initializing USB core
**/
STATIC
EFI_STATUS
SynopsysInitDevice (
  IN  QCOM_USB_CORE         *UsbCore
  )
{

  EFI_STATUS  Status  =  EFI_SUCCESS;
  UINTN       BaseAddr;

  FNC_ENTER_MSG ();

  if (NULL == UsbCore) {
    DBG(EFI_D_ERROR, "invalid parameter");
    Status = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
  }

  BaseAddr = UsbCore->BaseAddr;
  USB_ASSERT_GOTO ((BaseAddr != 0), ON_EXIT);

  if (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) {
    // Reset Device Core.
    HAL_USB_OR(BaseAddr, HWIO_USB3_PRI_DCTL_OFFS, HWIO_USB3_PRI_DCTL_CSFTRST_BMSK);
    while (HAL_USB_READ(BaseAddr, HWIO_USB3_PRI_DCTL_OFFS) & HWIO_USB3_PRI_DCTL_CSFTRST_BMSK) { ; }
    //InitHSUSBPhy(UsbCore);
    goto ON_EXIT;
  }

  /*
    16. Enable the hardware LPM:
    usb30_reg_gusb2phycfg_regs_p_gusb2phycfg[ENBLSLPM] = 0x1
  */
  HAL_USB_OR( BaseAddr, HWIO_USB3_PRI_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_OFFS (0), HWIO_USB3_PRI_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_ENBLSLPM_BMSK);
  
ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/**
  Performs device mode specific initialization on a Synopsys super speed USB Secondary core.

  @param [in]  UsbCore      The USB core instance

  @retval EFI_SUCCESS       USB core successfully initialized
  @retval EFI_UNSUPPORTED   USB core not supported
  @retval Others            Error encountered initializing USB core
**/
STATIC
EFI_STATUS
SynopsysInitDeviceSec (
  IN  QCOM_USB_CORE         *UsbCore
  )
{

  EFI_STATUS  Status  =  EFI_SUCCESS;
  UINTN       BaseAddr;

  FNC_ENTER_MSG ();

  if (NULL == UsbCore) {
    DBG(EFI_D_ERROR, "invalid parameter");
    Status = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
  }

  BaseAddr = UsbCore->BaseAddr;
  USB_ASSERT_GOTO ((BaseAddr != 0), ON_EXIT);

  if (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) {
    // Reset Device Core.
    HAL_USB_OR(BaseAddr, HWIO_USB3_2ND_DCTL_OFFS, HWIO_USB3_2ND_DCTL_CSFTRST_BMSK);
    while (HAL_USB_READ(BaseAddr, HWIO_USB3_2ND_DCTL_OFFS) & HWIO_USB3_2ND_DCTL_CSFTRST_BMSK) { ; }
    InitHSUSBPhySec(UsbCore);
    goto ON_EXIT;
  }

  /*
    16. Enable the hardware LPM:
    usb30_reg_gusb2phycfg_regs_p_gusb2phycfg[ENBLSLPM] = 0x1
  */
  HAL_USB_OR( BaseAddr, HWIO_USB3_2ND_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_OFFS (0), HWIO_USB3_2ND_GUSB2PHYCFG_REGS_p_GUSB2PHYCFG_ENBLSLPM_BMSK);
  
ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/**
  Refer UsbConfigUsbCoresPrivate.h for the header description
**/
STATIC
EFI_STATUS
Usb30InitDevice (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  EFI_STATUS  Status  =  EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);

  if(UsbCore->CoreType == USB_CONFIG_SSUSB1)
    Status = SynopsysInitDevice(UsbCore);
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB2)
    Status = SynopsysInitDeviceSec(UsbCore);
  else
    DBG(EFI_D_WARN, "Invalid Core Type");

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description*/
STATIC
EFI_STATUS
SSUsb1EnterLpm (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);

  // not needed for Sdm845 PRE-SI platform
  if ((PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI)   ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_VIRTIO) ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    DBG(EFI_D_WARN, "Pre-SI: Bypass SynopsysPrimEnterLpm");
    goto ON_EXIT;
  }

  if (!UsbCore->IsCoreInLPM)
  {
    // Reference Count 0 implies that we have not enabled clocks
    if (UsbCore->ClkRefCnt == 0)
    {
      // Enable clocks first, without that disable clocks doesn't work
      Status = InitUsbClocks (UsbCore->EnClockCount, UsbCore->EnClocks);
      WRN_CHK ("failed to enable USB clocks");
      // Increment reference count
      UsbCore->ClkRefCnt++;
    }
    
    //Assert controller & PHYs resets - TBD - Get correct registers
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_PRIM_BCR_OFFS, HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_PRIM_BCR_OFFS, HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK);
    //HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_PRIM_BCR_OFFS, HWIO_GCC_USB3_PHY_PRIM_BCR_BLK_ARES_BMSK);
    //HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_PRIM_BCR_OFFS, HWIO_GCC_USB30_PRIM_BCR_BLK_ARES_BMSK);

    Status = DisableUsbClocks (UsbCore->DisClockCount, UsbCore->DisClocks);
    WRN_CHK ("failed to disable USB clocks");

    // Disable USB Foot Switch (Power domain)
    ConfigPowerDomain(FALSE, SSUsb1PowerDomain);

    //  Enable SW PowerCollapse for USB30 Controller
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_PRIM_GDSCR_OFFS, HWIO_GCC_USB30_PRIM_GDSCR_SW_COLLAPSE_BMSK);

    if (NULL != gPmicNpaClientSS1) 
    {
      npa_issue_required_request(gPmicNpaClientSS1, PMIC_NPA_MODE_ID_GENERIC_STANDBY);   //Sends the request to RPM, no return
      DBG(EFI_D_INFO, "SSUsb1EnterLpm : NPA Request to set PMIC_NPA_MODE_ID_GENERIC_STANDBY");
    }

    //Remove bus votes
    if (gNpaClientSS1Bus) {
        npa_issue_vector_request( gNpaClientSS1Bus,
                                 (sizeof(usb_icb_clrrequest)/sizeof(npa_resource_state)), 
                                 ((npa_resource_state *)usb_icb_clrrequest) );
    }

    // Decrement reference count
    UsbCore->ClkRefCnt--;
    UsbCore->IsCoreInLPM = TRUE;

    // Unknown Reference Count, Faulty scenario
    if (UsbCore->ClkRefCnt > 1)
    {
      WRN_CHK ("SSUsb1EnterLpm: Wrong clock Reference count= 0x%x", UsbCore->ClkRefCnt);
    }
  }
  else
  {
    WRN_CHK ("SSUsb1EnterLpm: Synopsys Core already in LPM");
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description*/
STATIC
EFI_STATUS
SSUsb2EnterLpm (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);

  // not needed for Sdm845 PRE-SI platform
  if ((PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI)   ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_VIRTIO) ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    DBG(EFI_D_WARN, "Pre-SI: Bypass SynopsysPrimEnterLpm");
    goto ON_EXIT;
  }

  if (!UsbCore->IsCoreInLPM)
  {
    // Reference Count 0 implies that we have not enabled clocks
    if (UsbCore->ClkRefCnt == 0)
    {
      // Enable clocks first, without that disable clocks doesn't work
      Status = InitUsbClocks (UsbCore->EnClockCount, UsbCore->EnClocks);
      WRN_CHK ("failed to enable USB clocks");
      // Increment reference count
      UsbCore->ClkRefCnt++;
    }
    
    //Assert controller & PHYs resets
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_SEC_BCR_OFFS, HWIO_GCC_USB30_SEC_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_SEC_BCR_OFFS, HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3_PHY_SEC_BCR_OFFS, HWIO_GCC_USB3_PHY_SEC_BCR_BLK_ARES_BMSK);
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB3PHY_PHY_SEC_BCR_OFFS, HWIO_GCC_USB30_SEC_BCR_BLK_ARES_BMSK);

    Status = DisableUsbClocks (UsbCore->DisClockCount, UsbCore->DisClocks);
    WRN_CHK ("failed to disable USB clocks");

    // Disable USB Foot Switch (Power domain)
    ConfigPowerDomain(FALSE, SSUsb2PowerDomain);

    //  Enable SW PowerCollapse for USB30 Controller
    HAL_USB_OR (GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_USB30_SEC_GDSCR_OFFS, HWIO_GCC_USB30_SEC_GDSCR_SW_COLLAPSE_BMSK);

    if (NULL != gPmicNpaClientSS2)
    {
      npa_issue_required_request(gPmicNpaClientSS2, PMIC_NPA_MODE_ID_GENERIC_STANDBY);   //Sends the request to RPM, no return
      DBG(EFI_D_INFO, "SSUsb2EnterLpm : NPA Request to set PMIC_NPA_MODE_ID_GENERIC_STANDBY");
    }

    //Remove bus votes
    if (gNpaClientSS2Bus) {
        npa_issue_vector_request( gNpaClientSS2Bus,
                                 (sizeof(usb_icb_clrrequest)/sizeof(npa_resource_state)), 
                                 ((npa_resource_state *)usb_icb_clrrequest) );
    }

    // Decrement reference count
    UsbCore->ClkRefCnt--;
    UsbCore->IsCoreInLPM = TRUE;

    // Unknown Reference Count, Faulty scenario
    if (UsbCore->ClkRefCnt > 1)
    {
      WRN_CHK ("SSUsb2EnterLpm: Wrong clock Reference count= 0x%x", UsbCore->ClkRefCnt);
    }
  }
  else
  {
    WRN_CHK ("SSUsb2EnterLpm: Synopsys Core already in LPM");
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description*/
STATIC
EFI_STATUS
Usb30ExitLpm (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);

  // not needed for Sdm845 PRE-SI platform
  if ((PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI)   ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_VIRTIO) ||
      (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    DBG(EFI_D_WARN, "Pre-SI: Bypass SynopsysExitLpm");
    goto ON_EXIT;
  }

  // Perform GCC reset for controller and PHYs
  SynopsysGccReset (UsbCore);

ON_EXIT:
  FNC_LEAVE_MSG();
  return EFI_SUCCESS;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description*/
EFI_STATUS
Usb30Reset (
  IN  QCOM_USB_CORE_INTERFACE         *This
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description */
STATIC
EFI_STATUS
Usb30SetPhyLane(
  IN QCOM_USB_CORE_INTERFACE           *This,
  IN UINT8                              LaneNum
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);

  UsbCore->LaneNum = LaneNum;

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description */
STATIC
EFI_STATUS
Usb30GetBaseAddr(
  IN   QCOM_USB_CORE_INTERFACE           *This,
  OUT  UINTN                             *BaseAddr
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This || !BaseAddr) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);

  *BaseAddr = UsbCore->BaseAddr;

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description */
STATIC
EFI_STATUS
Usb30ResetSSPhy(
  IN   QCOM_USB_CORE_INTERFACE           *This
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);

  if(UsbCore->CoreType == USB_CONFIG_SSUSB1)
  InitSSUSBPhy(UsbCore);
  else if(UsbCore->CoreType == USB_CONFIG_SSUSB2)
    InitSSUSBPhySec(UsbCore);
  else
    DBG(EFI_D_WARN, "Invalid Core Type");

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description */ 
STATIC
EFI_STATUS
SSUsb1SetPhyNonDrivingMode(
  IN   QCOM_USB_CORE_INTERFACE           *This,
  IN   BOOLEAN                            NonDrivingMode
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;
  UINTN BaseAddr;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);
  
  BaseAddr = UsbCore->BaseAddr;

  // power off the PHY for high-z
  if (NonDrivingMode)
  {
    HAL_USB_WRITE(AHB2PHY_SOUTH_AHB2PHY_SOUTH_BASE, HWIO_AHB2PHY_SOUTH_USB2PHY_PRIM_USB_PHY_UTMI_CTRL0_OFFS, 0x08);
  }
  else
  {
    // in order to put the phy back to the NORMAL mode, we have to re-init the HS phy.
    InitHSUSBPhy(UsbCore);
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description */ 
STATIC
EFI_STATUS
SSUsb2SetPhyNonDrivingMode(
  IN   QCOM_USB_CORE_INTERFACE           *This,
  IN   BOOLEAN                            NonDrivingMode
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;
  UINTN BaseAddr;

  FNC_ENTER_MSG();

  if (!This) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("invalid parameter");
  }

  UsbCore = USB_CORE_FROM_IFC(This);
  
  BaseAddr = UsbCore->BaseAddr;

  // power off the PHY for high-z
  if (NonDrivingMode)
  {
    HAL_USB_WRITE(AHB2PHY_SOUTH_AHB2PHY_SOUTH_BASE, HWIO_AHB2PHY_SOUTH_USB2PHY_SEC_USB_PHY_UTMI_CTRL0_OFFS, 0x08);
  }
  else
  {
    // in order to put the phy back to the NORMAL mode, we have to re-init the HS phy.
    InitHSUSBPhySec(UsbCore);
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description */
STATIC
EFI_STATUS
SSUsb1InitHSUSBPhyKeepPwrDwn (
  IN QCOM_USB_CORE_INTERFACE           *This
  )
{
  FNC_ENTER_MSG ();

  // Sdm845 RUMI PHY init sequence.
  if (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) {
      DBG(EFI_D_INFO, "Return without any configuration for RUMI");
      goto ON_EXIT;
  }

// TBD - check the sequence later for snps femto phy
#if 0
  if ((PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_RUMI)   &&
      (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_VIRTIO) &&
      (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    //Hard reset PHY
    HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_PRIM_BCR_OFFS, HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);
    HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_PRIM_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_PRIM_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);

    DBG(EFI_D_INFO, "HS Phy Init");

    // Keep Port Power Down
    HAL_USB_WRITE(PERIPH_SS_AHB2PHY_NORTH_BASE, HWIO_PERIPH_SS_QUSB2PHY_QUSB2PHY_PWR_CTRL1_OFFS, 0x23);
    gBS->Stall(100);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_QUSB2PHY_PLL_ANALOG_CONTROLS_TWO_OFFS,
      0x03);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_QUSB2PHY_PLL_CLOCK_INVERTERS_OFFS,
      0x7C);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_QUSB2PHY_PLL_CMODE_OFFS,
      0x80);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_QUSB2PHY_PLL_PLL_LOCK_DELAY_OFFS,
      0x0A);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_QUSB2PHY_TUNE2_OFFS,
      0x09);
  }
#endif

  ON_EXIT:
  FNC_LEAVE_MSG ();
  return EFI_SUCCESS;
}


/* Refer UsbConfigUsbCoresPrivate.h for the header description */
STATIC
EFI_STATUS
SSUsb2InitHSUSBPhyKeepPwrDwn (
  IN QCOM_USB_CORE_INTERFACE           *This
  )
{
  FNC_ENTER_MSG ();

  // Sdm845 RUMI PHY init sequence.
  if (PlatformInfo.platform == EFI_PLATFORMINFO_TYPE_RUMI) {
      DBG(EFI_D_INFO, "Return without any configuration for RUMI");
      goto ON_EXIT;
  }

  // TBD - check the sequence later for snps femto phy
  #if 0
  if ((PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_RUMI)   &&
      (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_VIRTIO) &&
      (PlatformInfo.platform != EFI_PLATFORMINFO_TYPE_UNKNOWN)) {
    //Hard reset PHY
    HAL_USB_OR(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_SEC_BCR_OFFS, HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);
    HAL_USB_AND(GCC_CLK_CTL_REG_REG_BASE, HWIO_GCC_QUSB2PHY_SEC_BCR_OFFS, ~HWIO_GCC_QUSB2PHY_SEC_BCR_BLK_ARES_BMSK);
    gBS->Stall(100);

    DBG(EFI_D_INFO, "HS Phy Init");

    // Keep Port Power Down
    HAL_USB_WRITE(PERIPH_SS_AHB2PHY_NORTH_BASE, HWIO_PERIPH_SS_QUSB2PHY_SEC_QUSB2PHY_PWR_CTRL1_OFFS, 0x23);
    gBS->Stall(100);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_SEC_QUSB2PHY_PLL_ANALOG_CONTROLS_TWO_OFFS,
      0x03);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_SEC_QUSB2PHY_PLL_CLOCK_INVERTERS_OFFS,
      0x7C);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_SEC_QUSB2PHY_PLL_CMODE_OFFS,
      0x80);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_SEC_QUSB2PHY_PLL_PLL_LOCK_DELAY_OFFS,
      0x0A);

    HAL_USB_WRITE(
      PERIPH_SS_AHB2PHY_NORTH_BASE,
      HWIO_PERIPH_SS_QUSB2PHY_SEC_QUSB2PHY_TUNE2_OFFS,
      0x09);
  }
#endif

  ON_EXIT:
  FNC_LEAVE_MSG ();
  return EFI_SUCCESS;
}


/* Refer to UsbConfigUsbCoresPublic.h for header description*/
EFI_STATUS
UsbConfigCoresInit(
  IN  UINTN                      CoreNum,
  OUT QCOM_USB_CORE_INTERFACE  **UsbCoreInterface
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;
  STATIC UINTN FlagToCallAPIOnlyOnce = 0; 

  FNC_ENTER_MSG();

  if (!UsbCoreInterface) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: CoreNum %d, UsbCore %p", CoreNum, UsbCoreInterface);
  }

  UsbCore = AllocateZeroPool(sizeof(QCOM_USB_CORE));

  if (!UsbCore) {
    Status = EFI_OUT_OF_RESOURCES;
    ERR_CHK("Out of Resources");
  }

  if (CoreNum == USB_CORE_0_SDM_1000) {
    UsbCore->CoreInterface.InitCommon = SSUsb1InitCommon;
    UsbCore->CoreInterface.InitDevice = Usb30InitDevice;
    UsbCore->CoreInterface.InitHost = Usb30InitHost;
    UsbCore->CoreInterface.GetVbusStatus = Usb30GetVbusSt;
    UsbCore->CoreInterface.EnableVbus = Usb30EnableVbus;
    UsbCore->CoreInterface.Reset = Usb30Reset;
    UsbCore->CoreInterface.CoreEnterLPM = SSUsb1EnterLpm;
    UsbCore->CoreInterface.CoreExitLPM = Usb30ExitLpm;
    UsbCore->CoreInterface.SetPhyLane = Usb30SetPhyLane;
    UsbCore->CoreInterface.GetBaseAddress = Usb30GetBaseAddr;
    UsbCore->CoreInterface.ResetSSPhy = Usb30ResetSSPhy;
    UsbCore->CoreInterface.SetNonDrivingMode = SSUsb1SetPhyNonDrivingMode;
    UsbCore->CoreInterface.InitHsPhyPwrDwn = SSUsb1InitHSUSBPhyKeepPwrDwn;
    UsbCore->CoreInterface.PollSSPhyTraining = SSUsb1PollSSPhyTraining;
    UsbCore->CoreInterface.AdvanceSSCmplPattern = Usb1AdvanceSSCmplPattern;

    *UsbCoreInterface = &UsbCore->CoreInterface;

    UsbCore->CoreType = USB_CONFIG_SSUSB1;
    UsbCore->BaseAddr = USB3_PRI_USB30_PRIM_BASE;
    UsbCore->EnClockCount = sizeof(SSUsb1EnCoreClocks) / sizeof(QCOM_USB_CLOCK);
    UsbCore->EnClocks = SSUsb1EnCoreClocks;
    UsbCore->DisClockCount = sizeof(SSUsb1DisCoreClocks) / sizeof(QCOM_USB_CLOCK);
    UsbCore->DisClocks = SSUsb1DisCoreClocks;
    UsbCore->ClkRefCnt = 0;
    UsbCore->IsCoreInLPM = FALSE;
    UsbCore->LaneNum = 0;
  }
  else if(CoreNum == USB_CORE_1_SDM_1000) {
    UsbCore->CoreInterface.InitCommon = SSUsb2InitCommon;
    UsbCore->CoreInterface.InitDevice = Usb30InitDevice;
    UsbCore->CoreInterface.InitHost = Usb30InitHost;
    UsbCore->CoreInterface.GetVbusStatus = Usb30GetVbusSt;
    UsbCore->CoreInterface.EnableVbus = Usb30EnableVbus;
    UsbCore->CoreInterface.Reset = Usb30Reset;
    UsbCore->CoreInterface.CoreEnterLPM = SSUsb2EnterLpm;
    UsbCore->CoreInterface.CoreExitLPM = Usb30ExitLpm;
    UsbCore->CoreInterface.SetPhyLane = Usb30SetPhyLane;
    UsbCore->CoreInterface.GetBaseAddress = Usb30GetBaseAddr;
    UsbCore->CoreInterface.ResetSSPhy = Usb30ResetSSPhy;
    UsbCore->CoreInterface.SetNonDrivingMode = SSUsb2SetPhyNonDrivingMode;
    UsbCore->CoreInterface.InitHsPhyPwrDwn = SSUsb2InitHSUSBPhyKeepPwrDwn;
    UsbCore->CoreInterface.PollSSPhyTraining = SSUsb2PollSSPhyTraining;
    UsbCore->CoreInterface.AdvanceSSCmplPattern = Usb2AdvanceSSCmplPattern;

    *UsbCoreInterface = &UsbCore->CoreInterface;

    UsbCore->CoreType = USB_CONFIG_SSUSB2;
    UsbCore->BaseAddr = USB3_2ND_USB30_PRIM_BASE;
    UsbCore->EnClockCount = sizeof(SSUsb2EnCoreClocks) / sizeof(QCOM_USB_CLOCK);
    UsbCore->EnClocks = SSUsb2EnCoreClocks;
    UsbCore->DisClockCount = sizeof(SSUsb2DisCoreClocks) / sizeof(QCOM_USB_CLOCK);
    UsbCore->DisClocks = SSUsb2DisCoreClocks;
    UsbCore->ClkRefCnt = 0;
    UsbCore->IsCoreInLPM = FALSE;
    UsbCore->LaneNum = 0;
  }
  else if(CoreNum == USB_CORE_2_SDM_1000) {
    UsbCore->CoreInterface.InitCommon = SSUsb3InitCommon;
    UsbCore->CoreInterface.InitDevice = NULL;
    UsbCore->CoreInterface.InitHost = Usb30InitHost;
    UsbCore->CoreInterface.GetVbusStatus = Usb30GetVbusSt;
    UsbCore->CoreInterface.EnableVbus = Usb30EnableVbus;
    UsbCore->CoreInterface.Reset = Usb30Reset;
    UsbCore->CoreInterface.CoreEnterLPM = SSUsb2EnterLpm;
    UsbCore->CoreInterface.CoreExitLPM = Usb30ExitLpm;
    UsbCore->CoreInterface.SetPhyLane = Usb30SetPhyLane;
    UsbCore->CoreInterface.GetBaseAddress = Usb30GetBaseAddr;
    UsbCore->CoreInterface.ResetSSPhy = Usb30ResetSSPhy;
    UsbCore->CoreInterface.SetNonDrivingMode = SSUsb2SetPhyNonDrivingMode;
    UsbCore->CoreInterface.InitHsPhyPwrDwn = SSUsb2InitHSUSBPhyKeepPwrDwn;
    UsbCore->CoreInterface.AdvanceSSCmplPattern = Usb3AdvanceSSCmplPattern;

    *UsbCoreInterface = &UsbCore->CoreInterface;

    UsbCore->CoreType = USB_CONFIG_SSUSB3;
    UsbCore->BaseAddr = USB3_MP_USB31_SCND_BASE;
    UsbCore->EnClockCount = sizeof(SSUsb3EnCoreClocks) / sizeof(QCOM_USB_CLOCK);
    UsbCore->EnClocks = SSUsb3EnCoreClocks;
    UsbCore->DisClockCount = sizeof(SSUsb3DisCoreClocks) / sizeof(QCOM_USB_CLOCK);
    UsbCore->DisClocks = SSUsb3DisCoreClocks;
    UsbCore->ClkRefCnt = 0;
    UsbCore->IsCoreInLPM = FALSE;
    UsbCore->LaneNum = 0;
  }
  else {
    DBG(EFI_D_WARN, "Invalid Core Type");
  }

  //Open EFI Protocols only once
  if(FlagToCallAPIOnlyOnce == 0 && CoreNum < USB_CORE_MAX_NUM_SDM_1000) {
    Status = UsbConfigLibOpenProtocols();
    ERR_CHK("UsbConfigLibOpenProtocols failed %r",Status);
	FlagToCallAPIOnlyOnce = 1;
  }

ON_EXIT:
  FNC_LEAVE_MSG();
  return Status;
}


/* Refer to UsbConfigUsbCoresPublic.h for header description*/
EFI_STATUS
UsbConfigCoresDeinit(
  IN QCOM_USB_CORE_INTERFACE  *UsbCoreInterface
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  QCOM_USB_CORE *UsbCore;

  FNC_ENTER_MSG();

  if (!UsbCoreInterface) {
    Status = EFI_INVALID_PARAMETER;
    ERR_CHK("Invalid Parameters: CoreNum %d, UsbCore %p", UsbCoreInterface);
  }

  UsbCore = USB_CORE_FROM_IFC(UsbCoreInterface);

  if (UsbCore)
    FreePool(UsbCore);

ON_EXIT:
  return Status;
}
