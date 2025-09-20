/** 
 @file PcieClockConfig.c

  Handles all the Pcie related clocks and power domains.

  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

**/


/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   -------  ----------------------------------------------------------
 05/16/18   ts       Updated SMMU and clock settings
 11/15/17   ts       Removed SMMU settings from this file
 05/22/17   ts       Removed SMMU workaround from PCIe library code
 02/10/17   ts       Enabled IO coherency based on a flag from devcfg
 01/12/17   ts       Turning Off AGNOC clocks to unblock XO shutdown in HLOS
 12/07/16   ts       Programmed SMMU SMR and S2CR register using MASK instead
                     of programming all the registers with SID
 10/07/16   ts       Handled Reset, Enable of external and gcc controlled 
                     clocks seperately
 08/10/16   ts       Disabled clock power domain along with port clocks
                     during de initialization
 07/28/16   ts       Added SMMU configuration
 07/15/16   ts       Formatted strings to %a in debug prints
 06/27/16   ts       SpunOff from PciLink.c to create PcieClockConfig.c
=============================================================================*/

#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/BaseLib.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFINpa.h>
#include <Library/PcieConfigLib.h>
#include <HALbootHWIO.h>
#include <Library/PcdLib.h>

/*NPA headers*/
#include "npa.h"
#include "pmapp_npa.h"
/* PCIe lib haeders */
#include "PcieLink.h"
#include "PcieBsp.h"

/* handle for the npa client. */
STATIC npa_client_handle PcieReqClient = NULL;

/***************************************************************************//**
 * @fn PcieConfigLibDisableClocks
 * @brief Disable Clocks
 *
 * This function will disable clocks from the clocks list
 * 
 * @param[in] Pointer to EFI_CLOCK_PROTOCOL
 * @param[in] A const char * string of clock power domain name
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS PcieConfigLibDisableClocks (
        EFI_CLOCK_PROTOCOL    *pClockProtocol,
        CONST char ** ClockList)
{
  UINTN                ClockId;
  UINT32               ClockIndex = 0;
  BOOLEAN              ClockEnabled = FALSE;
  EFI_STATUS           Status = EFI_SUCCESS;
  
  if (pClockProtocol == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Clock Protocol is NULL %a\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  if (ClockList == NULL)
  {
    /*
     * DEBUG(( EFI_D_INFO, "%a: No Action Required!!!\r\n", __func__));
     */
    return EFI_SUCCESS;
  }
  
  while (ClockList[ClockIndex] != NULL)
  {
    Status = pClockProtocol->GetClockID(pClockProtocol, ClockList[ClockIndex], &ClockId);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: GetClockID failed %a\r\n",
              __func__, ClockList[ClockIndex]));
      return Status;
    }
    
    Status = pClockProtocol->IsClockEnabled(pClockProtocol, ClockId, &ClockEnabled);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: IsClockEnabled failed %a\r\n",
              __func__, ClockList[ClockIndex]));
      return Status;
    }
    
    if (ClockEnabled == TRUE)
    {
      Status = pClockProtocol->DisableClock(pClockProtocol, ClockId);
      if EFI_ERROR (Status)
      {
        DEBUG(( EFI_D_WARN, "%a: DisableClock failed %a\r\n",
                __func__, ClockList[ClockIndex]));
        return Status;
      }    	
    }

    ClockIndex++;  
  }
  
  return EFI_SUCCESS;
}

/***************************************************************************//**
 * @fn PcieConfigLibVoteIcbClkBW
 * @brief vore for ICB bandwidth
 *
 * This is a self aware function, based on the parameter pcieIcbClkBW_t type's,
 * npa_hdl member, will decide whether to create / destroy an npa client handle.
 *
 * @param[in] pointer to pcieIcbClkBW_t
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS PcieConfigLibVoteIcbClkBW (
        pcieIcbClkBW_t * icb_bw)
{
  if (icb_bw == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: icb_bw is NULL\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  /* If client doesn't exist, create one */
  if (icb_bw->npa_hdl == NULL)
  {

    icb_bw->npa_hdl = npa_create_sync_client_ex( "/icb/arbiter",
                                                 icb_bw->npa_client_name,
                                                 NPA_CLIENT_VECTOR,
                                                 icb_bw->ms_size,
                                                 (void *)icb_bw->ms_list );

    if (icb_bw->npa_hdl == NULL)
    {
      DEBUG(( EFI_D_WARN, "%a: NULL Client for%a\r\n", __func__,
	    icb_bw->npa_client_name));
      return EFI_LOAD_ERROR;
    }

    npa_issue_vector_request( icb_bw->npa_hdl, 
                              icb_bw->req_size/sizeof(npa_resource_state), 
                              (npa_resource_state *)icb_bw->req_list );
  }
  /* There is a client, destroy it if someone called us */
  else
  {
    npa_complete_request(icb_bw->npa_hdl);
    npa_destroy_client(icb_bw->npa_hdl);
    icb_bw->npa_hdl = NULL;
  }

  return EFI_SUCCESS;
}

/***************************************************************************//**
 * @fn PcieConfigLibDisableClockPower
 * @brief Disble the clock power domain
 *
 * Disable clock power domain
 *
 * @param[in] Pointer to EFI_CLOCK_PROTOCOL
 * @param[in] A const char * string of clock power domain name
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS PcieConfigLibDisableClockPower (
        EFI_CLOCK_PROTOCOL    *pClockProtocol,
        CONST char * PowerDomain)
{
  UINTN      uClockPowerDomainId;
  EFI_STATUS Status;

  if (pClockProtocol == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Clock Protocol is NULL %a\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  Status = pClockProtocol->GetClockPowerDomainID(pClockProtocol,
                                                 PowerDomain,
                                                 &uClockPowerDomainId);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: Get Failed %a\r\n", __func__, PowerDomain));
    return Status;
  }
  Status =  pClockProtocol->DisableClockPowerDomain(pClockProtocol,
                                                   uClockPowerDomainId);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: Disable Failed for %a\r\n", __func__, PowerDomain));
    return Status;
  }

  return EFI_SUCCESS;
}

/***************************************************************************//**
 * @fn PcieConfigLibTurnOffPortClocks
 * @brief Turn Off PCIe port Clocks
 *
 * This function will turn off all the port clocks
 *
 * @param[in] Root port index in portInfo_t struct to disable the clocks 
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibTurnOffPortClocks (UINT32 RootPortIndex)
{
  EFI_CLOCK_PROTOCOL * pClockProtocol;
  rootPort_t         * PcieRootPortInfo = PcieConfigLibGetPciRootPortInfo();
  portInfo_t         * PortInfo;
  CONST char        ** ClockList;
  EFI_STATUS           Status;
  
  if (PcieRootPortInfo->port_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Library not initialized\r\n", __func__));
    return RETURN_LOAD_ERROR;
  }

  if (RootPortIndex >= PcieRootPortInfo->num_rp)
  {
      DEBUG(( EFI_D_WARN, "%a: RootPortIndex is out of range\r\n", __func__));
      return EFI_INVALID_PARAMETER;
  }

  PortInfo = &PcieRootPortInfo->port_info[RootPortIndex];
  
  if(PortInfo->clk == NULL)
  {
    /*
     * If there are no clocks to disable
     * return EFI_SUCCESS
     */
    return EFI_SUCCESS;
  }
  
  Status = gBS->LocateProtocol(&gEfiClockProtocolGuid,
                               NULL,
                               (VOID **)&pClockProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Clock LocateProtocol failed\r\n", __func__));
    return Status;
  }
  
  /* Disable gcc controlled port clocks */
  ClockList = PortInfo->clk->gcc;
  Status = PcieConfigLibDisableClocks (pClockProtocol, ClockList);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: gcc port clocks failed\r\n", __func__));
    return Status;
  }

  /* Disable external port clocks */
  ClockList = PortInfo->clk->external;
  Status = PcieConfigLibDisableClocks (pClockProtocol, ClockList);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: external port clocks failed\r\n", __func__));
    return Status;
  }

  /* Destroy Client handle for ICB bandwidth vote */
  Status = PcieConfigLibVoteIcbClkBW(PortInfo->icb_bw);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: ICB vote failed\r\n", __func__));
    return Status;
  }

  /* Disable Clock power domain */
  Status = PcieConfigLibDisableClockPower (pClockProtocol, PortInfo->pwr);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: port power domain failed\r\n", __func__));
    return Status;
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibTurnOffCommonClocks
 * @brief Turn Off PCIe Common Clocks
 *
 * This function will turn off all the common clocks if any
 *
 * @param[in] void 
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibTurnOffCommonClocks (VOID)
{
  EFI_CLOCK_PROTOCOL * pClockProtocol;
  CONST pcieBsp_t    * PcieBspInfo = PcieConfigLibGetPcieBspInfo();
  CONST char        ** ClockList;
  EFI_STATUS           Status = EFI_SUCCESS;

  if(PcieBspInfo == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Invalid BSP Info pointer\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  if (PcieBspInfo->phy_clk == NULL)
  {
    /*
     * If there are no clocks to disable
     * return EFI_SUCCESS
     */
    return EFI_SUCCESS;
  }
  
  Status = gBS->LocateProtocol(&gEfiClockProtocolGuid,
                               NULL,
                               (VOID **)&pClockProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Clock LocateProtocol failed\r\n", __func__));
    return Status;
  }
 
  /* Disable common gcc controlled clocks */
  ClockList = PcieBspInfo->phy_clk->gcc;
  Status = PcieConfigLibDisableClocks (pClockProtocol, ClockList);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Disable gcc clocks failed\r\n", __func__));
    return Status;
  }

  /* Disable external common clocks */
  ClockList = PcieBspInfo->phy_clk->external;
  Status = PcieConfigLibDisableClocks (pClockProtocol, ClockList);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Disable external clocks failed\r\n", __func__));
    return Status;
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibSetClockFreq
 * @brief Set Clock Freq for the listed clocks
 *
 * Try and set the correct clock frequency as requested
 *
 * @param[in] Pointer to EFI_CLOCK_PROTOCOL
 * @param[in] Pointer to clkFreqSettings_t type
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS PcieConfigLibSetClockFreq (
        EFI_CLOCK_PROTOCOL    *pClockProtocol,
        CONST clkFreqSettings_t * clk)
{
  UINTN      ClockId;
  UINT32     nFreqHz;
  EFI_STATUS Status = EFI_SUCCESS;

  if (pClockProtocol == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Clock Protocol is NULL \r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  if (clk == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: clk settings is NULL \r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  /* until there is a valid clock frequency to set */
  while(clk->freq)
  {
    Status = pClockProtocol->GetClockID(pClockProtocol,
                                        clk->name,
                                        &ClockId);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: Get Clock ID Failed\r\n", __func__));
      return Status;
    }

    /* Try exact frequency match */
    Status = pClockProtocol->SetClockFreqHz(pClockProtocol, ClockId,
                                           clk->freq,
                                           EFI_CLOCK_FREQUENCY_HZ_CLOSEST,
                                           &nFreqHz);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: SetClockFreqHz failed\r\n", __func__));
      return Status;
    }
    
    /* move to the next entry in the list */
    clk++;
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibEnableClockPower
 * @brief Enable the clock power domain
 *
 * Enables clock power domain
 *
 * @param[in] Pointer to EFI_CLOCK_PROTOCOL
 * @param[in] A const char * string of clock power domain name
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS PcieConfigLibEnableClockPower (
        EFI_CLOCK_PROTOCOL    *pClockProtocol,
        CONST char * PowerDomain)
{
  UINTN      uClockPowerDomainId;
  EFI_STATUS Status = EFI_SUCCESS;

  if (pClockProtocol == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Clock Protocol is NULL\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  Status = pClockProtocol->GetClockPowerDomainID(pClockProtocol,
                                                 PowerDomain,
                                                 &uClockPowerDomainId);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: Get Failed %a\r\n", __func__, PowerDomain));
    return Status;
  }

  Status =  pClockProtocol->EnableClockPowerDomain(pClockProtocol,
                                                   uClockPowerDomainId);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: Enable Failed for %a\r\n", __func__, PowerDomain));
    return Status;
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibResetClocks
 * @brief Reset the clocks
 *
 * Resets the list of clocks provided
 *
 * @param[in] Pointer to EFI_CLOCK_PROTOCOL
 * @param[in] An array of const char * string list of clock names to reset
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS PcieConfigLibResetClocks (
        EFI_CLOCK_PROTOCOL    *pClockProtocol,
        CONST char ** ResetClockList)
{
  UINT32     ClockIndex = 0;
  UINTN      ClockId;
  EFI_STATUS Status = EFI_SUCCESS;

  if (pClockProtocol == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Clock Protocol is NULL \r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  /*
   * check if ResetClockList is NULL
   * if so, there is no clock needed to be enabled
   */
  if (ResetClockList == NULL)
  {
    /*
     * DEBUG(( EFI_D_INFO, "%a: No Action Required!!!\r\n", __func__));
     */
    return EFI_SUCCESS;
  }

  while (ResetClockList[ClockIndex] != NULL)
  {
    Status = pClockProtocol->GetClockID(pClockProtocol, ResetClockList[ClockIndex], &ClockId);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: GetClockID failed %a\r\n",
	    __func__, ResetClockList[ClockIndex]));
      return Status;
    }

    Status = pClockProtocol->ResetClock(pClockProtocol, ClockId, EFI_CLOCK_RESET_ASSERT);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: ResetClock assert failed %a\r\n",
	    __func__, ResetClockList[ClockIndex]));
      return Status;
    }

    gBS->Stall(500);

    Status = pClockProtocol->ResetClock(pClockProtocol, ClockId, EFI_CLOCK_RESET_DEASSERT);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: ResetClock deassert failed %a\r\n",
	    __func__, ResetClockList[ClockIndex]));
      return Status;
    }

    ClockIndex++;
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibEnableClocks
 * @brief Enable the clocks
 *
 * Resets and Enabled clocks provided in clock descriptor
 *
 * @param[in] Pointer to EFI_CLOCK_PROTOCOL
 * @param[in] An array of const char * string list of clock names to Enable
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibEnableClocks (
        EFI_CLOCK_PROTOCOL  * pClockProtocol,
        CONST char ** EnableClockList)
{
  UINTN      ClockId;
  UINT32     ClockIndex = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  if (pClockProtocol == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Clock Protocol is NULL %a\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  /*
   * check if EnableClockList is NULL
   * if so, there is no clock needed to be enabled
   */
  if (EnableClockList == NULL)
  {
    /*
     * DEBUG(( EFI_D_INFO, "%a: No Action Required!!!\r\n", __func__));
     */
    return EFI_SUCCESS;
  }

  while (EnableClockList[ClockIndex] != NULL)
  {
    Status = pClockProtocol->GetClockID(pClockProtocol, EnableClockList[ClockIndex], &ClockId);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: GetClockID failed %a\r\n",
              __func__, EnableClockList[ClockIndex]));
      return Status;
    }

    Status = pClockProtocol->EnableClock(pClockProtocol, ClockId);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: EnableClock failed %a\r\n",
              __func__, EnableClockList[ClockIndex]));
      return Status;
    }

    ClockIndex++;
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibConfigureClocks
 * @brief Configure the PCIe clocks
 *
 * This function performs the following operations:
 *  o-> Enabled clock power domain
 *  o-> Resets and Enables all the clocks
 *  o-> sets the Aux clock frequency
 *
 * @param[in] void
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibConfigureClocks (UINT32 RootPortIndex)
{
  EFI_CLOCK_PROTOCOL  * pClockProtocol = NULL;
  CONST pcieBsp_t     * pcie_bsp_info = PcieConfigLibGetPcieBspInfo();
  rootPort_t          * pcie_root_port_info = PcieConfigLibGetPciRootPortInfo();
  portInfo_t          * port = &pcie_root_port_info->port_info[RootPortIndex];
  CONST pcieClkDesc_t * clk;
  EFI_STATUS            Status = EFI_SUCCESS;

  Status = gBS->LocateProtocol(&gEfiClockProtocolGuid,
                               NULL,
                               (VOID **)&pClockProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Clock LocateProtocol failed\r\n", __func__));
    return Status;
  }

  /*
   * Turn ON clock power domain
   */
  Status = PcieConfigLibEnableClockPower(pClockProtocol, port->pwr);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: Enable power Domain failed\r\n", __func__));
    return Status;
  }

  /*
   * Vote for ICB bandwidth
   */
  Status = PcieConfigLibVoteIcbClkBW(port->icb_bw);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: ICB vote failed\r\n", __func__));
    return Status;
  }

  clk = pcie_bsp_info->phy_clk;
  if (clk != NULL)
  {
    /*
     * Reset common PHY clock if any
     */
    Status = PcieConfigLibResetClocks(pClockProtocol, clk->reset);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: Reset common clock failed \r\n", __func__));
      return Status;
    }

    /*
     * Turn ON common PHY clock if any
     */
    Status = PcieConfigLibEnableClocks(pClockProtocol, clk->gcc);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: Enable PHY clocks failed\r\n", __func__));
      return Status;
    }
  }

  clk = port->clk;
  if (clk != NULL)
  {
    /*
     * Reset port PHY clock if any
     */
    Status = PcieConfigLibResetClocks(pClockProtocol, clk->reset);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: Reset port clock failed \r\n", __func__));
      return Status;
    }

    /*
     * Turn ON port PHY clocks 
     */
    Status = PcieConfigLibEnableClocks(pClockProtocol, clk->gcc);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: Enable port clocks failed\r\n", __func__));
      return Status;
    }
  }

  /*
   * Configure the AUX clock to requested frequency 
   */
  Status = PcieConfigLibSetClockFreq(pClockProtocol, port->clk_set);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: Aux Clock Set failed\r\n", __func__));
    return Status;
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibInitializeLDO
 * @brief Configure the PCIe LDOs
 *
 * Vote for the LDOs necessary for PCIe to be operational
 *
 * @param[in] void
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibInitializeLDO(VOID)
{
  if (PcieReqClient == NULL)
  {
    PcieReqClient = npa_create_sync_client(PMIC_NPA_GROUP_ID_PCIE, 
                                           "pcie",
                                           NPA_CLIENT_REQUIRED);
    if (PcieReqClient == NULL)
    {
      DEBUG(( EFI_D_WARN, "%a: pcie npa_create_sync_client returned NULL\r\n",
              __func__));
      return EFI_DEVICE_ERROR;
    }
  
    /* Sends Activate to the driver */
    npa_issue_required_request(PcieReqClient, 
                               PMIC_NPA_MODE_ID_GENERIC_ACTIVE);
  }

  return EFI_SUCCESS;
}

/***************************************************************************//**
 * @fn PcieConfigLibDeInitializeLDO
 * @brief Disable the PCIe LDOs
 *
 * Vote for the LDOs to be turned OFF
 *
 * @param[in] void
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibDeInitializeLDO(VOID)
{
  if (PcieReqClient != NULL)
  {
    npa_issue_required_request(PcieReqClient, PMIC_NPA_MODE_ID_GENERIC_OFF);
    npa_complete_request(PcieReqClient);
    npa_destroy_client(PcieReqClient);
    PcieReqClient = NULL;
  }

  return EFI_SUCCESS;
}
