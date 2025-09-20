/**
 @file PcieLink.c

  Implements link initialization related routines.

  Copyright (c) 2014-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

**/


/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   ---    ------------------------------------------------------------
 06/25/19 ts       Optimized time consumed in LTSSM cycles for all ports
 05/30/18 ts       L1ss disablement fixed
 05/16/18 ts       Deferred L1ss config to a 100 ms timer callback
 09/11/17 ts       Added support for SDM1000
 05/22/17 ts       TurnOff port clocks when link training failed
 05/05/17 ts       Included support to perform RMW on PHY init sequence
 04/21/17 ts       Optimized by removing macro implementation for register
                   writes and polling functionality
 02/10/17 ts       Implementing version dependent PHY init sequence writes
 10/07/16 ts       Change in the Phy init sequence by enabling pipe clock after
                   PLL locked in
 08/10/16 ts       Added PHY power down sequence and returned EFI_SUCCESS from
                   L1ss init to prevent PciHostBridgeDxe image from unloading,
                   so that the ACPI variable can be updated during callback
 07/15/16 ts       Formatted strings to %a in debug prints & Updated the global
                   with updated info if Link training fails for a root port
 07/08/16 ts       code refactoring
 06/08/16 ts       Validated PCIe library on MSM8998
 06/08/16 ts       Performed PCIe PHY init, moved all the GPIO inits right
                   before link training and enabled L1 for EP only if it is
                   supported for the End point
 02/08/16 ts       Added pcie xml config support
 11/12/15 ts       KW reivew #6754 & #6755
 09/03/15 ts       MSM8998 Support Included
 08/07/15 ts       Updated with new PHY seq for V3.0 h/w
 07/01/15 ts       Fixed the 7e bug check in HLOS by reverting the Limit region
                   value to just cover until the first 4KB of EP config space.
 06/22/15 ts       Fixed the compile error while 32-bit uefi.mbn build.
 06/15/15 ts       Used clock reset API and also updated unified code.
                   Implemented PCIe capability walk for L1ss enabling.
 05/13/15 ts       Unified code for multi platforms
 03/18/15 ts       Fixed Copyrights header
 03/09/15 ts       reduced some delays to improve boot time and include warning
                   messages and fixed running pcieTest more than once by adding
                   gcc bcr reset sequences.
 03/05/15 ts       written class code register value in PCIe_Setup_L1ss fn.
 01/26/15 ts       Updated as per 8994 changes
 12/15/14 ts       Changes for 8996 for all three root ports
 10/07/14 ah       Changes for 8996
 08/04/14 nk       Changed MemLabel
 06/16/14 hk       Add PcieInit protocol
 05/20/14 hk       Edit to bring in changes for 8994 PHY
 05/07/14 hk       Turn on PCie during exitbootservices
 04/18/14 hk       First checkin for 8994
=============================================================================*/

#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/BaseLib.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFINpa.h>
#include <Library/PcieConfigLib.h>

#include "PcieLink.h"
#include "PcieBsp.h"

/***************************************************************************//**
 * @fn PcieConfigLibTurnOnExternalClock
 * @brief Turn ON external clock before polling for PHY status
 *
 * This function will enable the external clock
 *
 * @param[in] external clock string name embedded in pcieClkDesc_t struct
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS PcieConfigLibTurnOnExternalClock(
    CONST pcieClkDesc_t * ClockDesc)
{
  EFI_CLOCK_PROTOCOL    * pClockProtocol;
  EFI_STATUS              Status = EFI_SUCCESS;

  /* Locate the clock protocol */
  Status = gBS->LocateProtocol(&gEfiClockProtocolGuid, NULL,
      (VOID **)&pClockProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Clock LocateProtocol failed\r\n", __func__));
    return Status;
  }

  /*
   * Make sure external clock is enabled in GCC ctrl register
   * before we even poll for PHYSTATUS bit in the next step
   */
  Status = PcieConfigLibEnableClocks (pClockProtocol, ClockDesc->external);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Clock Enable failed\r\n", __func__));
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibRegWriteConditional
 * @brief Write PHY init sequences
 *
 * This function will into PHY init sequences provided
 *
 * @param[in] phy_seq - array of phy init sequence
 * @param[in] mem - memory base addresses of corresponding regisers in phy_seq
 *
 * @return None
 ******************************************************************************/
STATIC inline VOID PcieConfigLibRegWriteConditional(
    CONST pciePhyRegSeq_t * phy_seq,
    CONST memRegions_t * mem)
{
  UINT32 Index = 0;
  UINT64 Addr;
  UINT32 Operation;;
  UINT32 Offset;
  UINT32 Mask;
  UINT32 Value;
  UINT32 Temp;

  if (mem == NULL || phy_seq == NULL)
    return;

  /* Assign the value of operation once before the while loop */
  Operation = phy_seq[Index].opflag;

  /*
   * This while loop will work on the Operation flag from
   * the PHY init sequence array and perform one of the following:
   *
   * o Delay of specified micro seconds
   * o Read Modify write of a specified register with mask and value
   * o Write into the specified register
   */
  while(Operation != PHY_REG_NOP)
  {
    Addr = mem[phy_seq[Index].base].pa;
    Offset = phy_seq[Index].offset;
    Value = phy_seq[Index].val;

    switch (Operation)
    {
    case PHY_REG_DELAY:
      gBS->Stall(Value);
    break;
    case PHY_REG_RMW:
      Mask = phy_seq[Index].mask;
      Temp = MmioRead32(Addr + Offset);
      Temp = Temp & ~Mask;
      Temp = Temp | Value;
      MmioWrite32(Addr + Offset, Temp);
    break;
    case PHY_REG_WRI:
      MmioWrite32(Addr + Offset, Value);
    break;
    case PHY_REG_DSB:
      MemoryFence ();
    break;
    case PHY_REG_NOP:
    default:
    break;
    }

    Index++;
    Operation = phy_seq[Index].opflag;
  }
}

/***************************************************************************//**
 * @fn PcieConfigLibWriteRegisterSequence
 * @brief Write into sequence of registers
 *
 * This function will write into sequence of registers from different
 * register regions to bring the PHY out of reset
 *
 * @param[in] Root port index in portInfo_t struct
 * @param[in] Complete phy init sequence in pciePhyAllVerSeq_t pointer
 * @param[in] Clock names in pcieClkDesc_t pointer
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS PcieConfigLibWriteRegisterSequence(
    UINT32 RootPortIndex,
    CONST pciePhyAllVerSeq_t * Phy,
    CONST pcieClkDesc_t * ClockDesc)
{
  UINT32                  version_index = 0;
  UINT32                  reg_index = 0;
  rootPort_t            * pcie_root_port_info;
  CONST memRegions_t    * mem;
  EFI_CHIPINFO_PROTOCOL * pChipInfoProtocol;
  EFIChipInfoVersionType  chipInfoVersion;
  CONST pciePhyRegSeq_t * phy_seq;
  EFI_STATUS              Status = EFI_SUCCESS;

  /*
   * Check if the PHY sequence is NULL
   */
  if(Phy == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: Complete PHY init sequence is NULL\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  /*
   * Return Success if there is no entry to write into
   */
  if (Phy[version_index].version == Pcie_Qcom_SoC_Ver_STOP)
  {
    return EFI_SUCCESS;
  }

  /*
   * Get the Root port info and Check if it is NULL
   */
  pcie_root_port_info = PcieConfigLibGetPciRootPortInfo();
  if(pcie_root_port_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: root port info is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  /*
   * Check if the provided RootPortIndex is valid and available
   */
  if (RootPortIndex >= pcie_root_port_info->num_rp)
  {
    DEBUG(( EFI_D_WARN, "%a: RootPortIndex out of range\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  /*
   * Get the memory regions base address
   */
  mem = pcie_root_port_info->port_info[RootPortIndex].mem;
  if (mem == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: mem regiosn is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid,NULL,(VOID **) &pChipInfoProtocol);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: ChipInfo Locate Failed\r\n", __func__));
    return Status ;
  }

  Status = pChipInfoProtocol->GetChipVersion(pChipInfoProtocol, &chipInfoVersion);
  if (EFI_ERROR (Status))
  {
    DEBUG(( EFI_D_WARN, "%a: Failed to get version info\r\n", __func__));
    return Status ;
  }

  while (Phy[version_index].version != Pcie_Qcom_SoC_Ver_STOP)
  {
    phy_seq = Phy[version_index].phy_seq;
    switch(Phy[version_index].version)
    {
      default:
        /*
         * If the version numbering in the PHY init sequence is
         * equal to the actual chipset major version number
         * read from the h/w, program the sequence.
         */
        if (Phy[version_index].version == (chipInfoVersion & 0xFFFF0000))
        {
          PcieConfigLibRegWriteConditional(phy_seq, mem);
        }
        break;
      case Pcie_Qcom_SoC_Ver_ALL:
        /*
         * Version independent sequences are tied to a version
         * number "0x00000000" aka. Pcie_Qcom_SoC_Ver_ALL in
         * the PHY init sequences to denote it is applicable
         * for all versions of the SoC
         */
        PcieConfigLibRegWriteConditional(phy_seq, mem);
        break;
      case Pcie_Qcom_SoC_Ver_POLL:
        /*
         * Polling sequence is empty, better dont call PollBitMask
         */
        if (phy_seq[reg_index].base == invalid_reg)
        {
          break;
        }

        /*
         * Once the PHY PLL is enabled make sure that the
         * external pipe clock is being enabled via gcc clock
         * device driver
         */
        if (ClockDesc != NULL && ClockDesc->external != NULL)
        {
          PcieConfigLibTurnOnExternalClock(ClockDesc);
        }

        /*
         * Check for the bit condition to be TRUE or FALSE
         */
        if( !PcieConfigLibPollBitMsk(mem[phy_seq[reg_index].base].pa + phy_seq[reg_index].offset,
              phy_seq[reg_index].mask,
              (BOOLEAN)phy_seq[reg_index].val,
              PCIE_PHY_UP_TIMEOUT_COUNT_MAX_15_MILLI_SEC))
        {
          DEBUG(( EFI_D_WARN, "%a: PHY not ready\r\n", __func__));
          return EFI_NOT_READY;
        }

        break;
    }

    version_index++;
  }

  return Status;
}

/***************************************************************************//**
 * @fn PcieConfigLibPollBitMsk
 * @brief Poll for the status until the Condition is satisifed
 *
 * Polls the "Addr" for bit specified in "Mask" with an interval of "Delay" for
 * each iteration until the "Condition" specified for that bit is met
 *
 * @param[in] Root port index in portInfo_t struct
 * @param[in] UINT64 Addr - Address to be read register from
 * @param[in] UINT32 Mask - Bit Mask to be used for checking the condition
 * @param[in] BOOLEAN Condition - TRUE/FALSE condition to be met for bit mask
 * @param[in] UINT32 Delay - Delay in micro seconds between each iteration
 *
 * @return TRUE if the condition is met else FALSE
 ******************************************************************************/
BOOLEAN PcieConfigLibPollBitMsk (
    UINT64 Addr,
    UINT32 Mask,
    BOOLEAN Condition,
    UINT32 Delay)
{
  INT32 CountVariable = INIT_TIMEOUT;
  volatile UINT32 StatusBit;
  UINT32 Shift = __builtin_ctz((Mask));

  /*
   * Read once before the condition check, as there is a posibilty
   * that the first read of the status register will have the
   * condition met and we dont have to stall for even one cycle
   */
  StatusBit = MmioRead32(Addr);
  StatusBit = (StatusBit & (Mask)) >> Shift;

  while ( (StatusBit != (Condition)) && (CountVariable >= 0))
  {
    gBS->Stall(Delay);
    StatusBit = MmioRead32(Addr);
    StatusBit = (StatusBit & (Mask)) >> Shift;
    CountVariable--;
  }

  /*
   * Return "TRUE" if CountVariable is "Non-Zero":
   *  This denotes CountVariable did not exhausted and we could able to poll
   *  for the satisfaction condition before the time out
   *
   * Return "FALSE" if CountVariable is "Zero":
   *  This denotes CountVariable did exhausted and we could not able to poll
   *  for the satisfaction condition before the time out
   */
  return ((CountVariable>=0)?TRUE:FALSE);
}

/***************************************************************************//**
 * @fn PcieConfigLibInitializePHY
 * @brief Bring PHY out of reset
 *
 * This function will bring the PCIe PHY out of reset
 *
 * @param[in] Root port index in portInfo_t struct
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibInitializePHY(UINT32 RootPortIndex)
{
  CONST pcieBsp_t * pcie_bsp_info = PcieConfigLibGetPcieBspInfo();
  rootPort_t      * pcie_root_port_info = PcieConfigLibGetPciRootPortInfo();
  portInfo_t      * port_info;
  CONST pciePhyAllVerSeq_t * phy_seq;
  EFI_STATUS Status = EFI_SUCCESS;

  if (pcie_bsp_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: pcie_bsp_info is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  if(pcie_root_port_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: root port info is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  if (RootPortIndex >= pcie_root_port_info->num_rp)
  {
    DEBUG(( EFI_D_WARN, "%a: RootPortIndex out of range\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  /* Call Common PHY init only for Root port 0 */
  if (RootPortIndex == 0)
  {
    /*
     * Get init sequence for common PHY
     */
    phy_seq = pcie_bsp_info->phy_init_seq;
    if(phy_seq != NULL)
    {
      Status = PcieConfigLibWriteRegisterSequence(RootPortIndex,
                                                  phy_seq,
                                                  pcie_bsp_info->phy_clk);
      if EFI_ERROR (Status)
      {
        DEBUG(( EFI_D_WARN, "%a: Common PHY init Failed \r\n", __func__));
        return Status ;
      }
    }
  }

  /*
   * Get the port info struct base for corresponding root port index
   */
  port_info = &pcie_root_port_info->port_info[RootPortIndex];

  /*
   * Get init sequence for port PHY
   */
  phy_seq = port_info->port_phy_init_seq;
  Status = PcieConfigLibWriteRegisterSequence(RootPortIndex,
                                              phy_seq,
                                              port_info->clk);
  if EFI_ERROR (Status)
  {
    DEBUG(( EFI_D_WARN, "%a: Port PHY init Failed \r\n", __func__));
    return Status ;
  }

  return EFI_SUCCESS;
}

/***************************************************************************//**
 * @fn PcieConfigLibDeInitializePHY
 * @brief Power down PHY
 *
 * This function will power down port PHY
 *
 * @param[in] Root port index in portInfo_t struct
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibDeInitializePHY(UINT32 RootPortIndex)
{
  rootPort_t            * pcie_root_port_info = PcieConfigLibGetPciRootPortInfo();
  portInfo_t            * port;

  if(pcie_root_port_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: root port info is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  if (RootPortIndex >= pcie_root_port_info->num_rp)
  {
    DEBUG(( EFI_D_WARN, "%a: RootPortIndex out of range\r\n", __func__));
    return EFI_INVALID_PARAMETER;
  }

  port = &pcie_root_port_info->port_info[RootPortIndex];

  PcieConfigLibRegWriteConditional(port->port_phy_deinit_seq, port->mem);

  return EFI_SUCCESS;
}

/***************************************************************************//**
 * @fn PcieConfigLibTrainLink
 * @brief Perform Link Training
 *
 * This function will perform LTSSM_EN and do link training with connected
 * endpoint for all supported root port.
 *
 * @param[in] void
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibTrainLink (VOID)
{
  UINT32                  index;
  UINT32                  temp;
  rootPort_t            * pcie_root_port_info;
  UINT32                  root_port_index = 0;
  UINT32                  total_root_port;
  UINT32                  rp_mask = 0;
  UINT32                  count_variable;
  UINT32                  linkup_status;
  CONST memRegions_t    * mem;
  CONST gpioPinConfig_t * gpio;
  portInfo_t            * port;
  EFI_STATUS              Status = EFI_SUCCESS;

  pcie_root_port_info = PcieConfigLibGetPciRootPortInfo();
  if(pcie_root_port_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: root port info is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  total_root_port = pcie_root_port_info->num_rp;

  for (root_port_index = 0; root_port_index < total_root_port; root_port_index++)
  {
    /*
     * Prepare the Root Port Mask to use while checking
     * collective LTSSM state
     */
    rp_mask |= 1 << root_port_index;

    port = &pcie_root_port_info->port_info[root_port_index];
    if (port == NULL)
    {
      DEBUG(( EFI_D_WARN, "%a: port info is NULL\r\n", __func__));
      return EFI_LOAD_ERROR;
    }

    /*
     * Get the memory regions base address
     */
    mem = port->mem;
    if (mem == NULL)
    {
      DEBUG(( EFI_D_WARN, "%a: mem regiosn is NULL\r\n", __func__));
      return EFI_LOAD_ERROR;
    }

    /*
     * Bring the End-points out of reset
     * and configure the clk_req gpio
     */
    gpio = port->gpio_cfg;
    index = 0;
    while(gpio[index].type != NONE)
    {
      Status = PcieConfigLibConfigureGPIOs(&gpio[index]);
      if EFI_ERROR (Status)
      {
        DEBUG(( EFI_D_WARN, "%a: Configure GPIO failed@%d\r\n",
                __func__, index));
        return Status;
      }
      index++;
    }

    /*
     * Link Training:
     * Write and spin into the LTSSM_EN bit to make
     * sure it is written into the register.
     */
    do
    {
      MmioOr32((HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_LTSSM) + mem[ahb_reg].pa),
               HWIO_PCIE_GEN3X1_PCIE_PARF_LTSSM_LTSSM_EN_BMSK);
      temp = MmioRead32((HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_LTSSM) + mem[ahb_reg].pa));
    } while (!(temp & HWIO_PCIE_GEN3X1_PCIE_PARF_LTSSM_LTSSM_EN_BMSK));
  }

  count_variable = INIT_TIMEOUT;
  while (rp_mask && (count_variable > 0))
  {
    /* Wait for a millisecond before checking the status of link */
    gBS->Stall(PCIE_LINK_UP_TIMEOUT_COUNT_MAX_150_MILLI_SEC);
    for (root_port_index = 0; root_port_index < total_root_port; root_port_index++)
    {
      port = &pcie_root_port_info->port_info[root_port_index];
      /* if the link was not trained successful for this root port then try */
      if (port->link->trained == FALSE)
      {
        mem = port->mem;
        /* Read and extract the link up status bit*/
        linkup_status = MmioRead32((HWIO_OFFS(PCIE_GEN3X1_PCIE_ELBI_SYS_STTS) + mem[axi_reg].pa));
        linkup_status &= HWIO_PCIE_GEN3X1_PCIE_ELBI_SYS_STTS_XMLH_LINK_UP_BMSK;

        /* if the link up status bit it set, mark link as trained */
        port->link->trained = linkup_status?TRUE:FALSE;

        /*
         * rp_mask has all the pcie root port's available in this platform as
         * bit mask. For example, if the root port 0 is present, then rp_mask
         * has b'1 set in bit position '0' and so on for root port 'n' at bit
         * position 'n'
         *
         * Once the LINK_UP status bit is set, then clear it from
         * rp_mask, to avoid any mistakes and unnecessarily check
         * again for linkup.
         */
        if (linkup_status)
        {
          rp_mask ^= (1 << root_port_index);
        }
      }
    }
    count_variable--;
  }

  return EFI_SUCCESS;
}

/***************************************************************************//**
 * @fn LocateCapabilityRegBlock
 * @brief Locate capcbility register block
 *
 * Locate capability register block per capability ID.
 *
 * @param[in]  CapID     , 16-bit PCIe extended capability ID
 * @param[in]  RegBase   , configuration register base of the device
 * @param[out] CapOffset , offset found based on CapID
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS
LocateCapabilityRegBlock (IN UINT16 CapID,
                          IN UINTN RegBase,
                          OUT UINT8 * CapOffset)
{
  UINT8   CapabilityPtr;
  UINT32  CapabilityEntry;
  UINT8   CapabilityID;

  if(CapOffset == NULL)
  {
    DEBUG(( EFI_D_WARN, "LocateCapabilityRegBlock: CapOffset is NULL\r\n"));
    return EFI_INVALID_PARAMETER;
  }

  CapabilityPtr = (UINT8)MmioRead32 (RegBase + PCI_CAPBILITY_POINTER_OFFSET);

  while ((CapabilityPtr >= 0x40) && ((CapabilityPtr & 0x03) == 0x00)){
    CapabilityEntry = MmioRead32 (RegBase + CapabilityPtr);

    CapabilityID = (UINT8) CapabilityEntry;

    if (CapabilityID == CapID) {
      *CapOffset = CapabilityPtr;
      return EFI_SUCCESS;
    }

    CapabilityPtr = (UINT8) (CapabilityEntry >> 8);
  }

  return EFI_NOT_FOUND;
}

/***************************************************************************//**
 * @fn LocatePciExpressCapabilityRegBlock
 * @brief Locate Express capcbility register block
 *
 * Find the capability pointer offset for the specified
 * capability by walking through the capability list.
 *
 * @param[in]  CapID     , 16-bit PCIe extended capability ID
 * @param[in]  RegBase   , configuration register base of the device
 * @param[out] CapOffset , offset found based on CapID
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
STATIC EFI_STATUS LocatePciExpressCapabilityRegBlock (IN UINT16 CapID,
                                                      IN UINTN RegBase,
                                                      OUT UINT16 * CapOffset)
{
  UINT16 CapabilityPtr;
  UINT32 CapabilityEntry;
  UINT16 CapabilityID;

  CapabilityPtr = EFI_PCIE_CAPABILITY_BASE_OFFSET;

  if(CapOffset == NULL)
  {
    DEBUG(( EFI_D_WARN, "LocatePciExpressCapabilityRegBlock: CapOffset is NULL\r\n"));
    return EFI_INVALID_PARAMETER;
  }

  do
  {
    /*
     * Mask it to DWORD alignment as per PCI spec
     */
    CapabilityPtr &= 0xFFC;
    CapabilityEntry = MmioRead32 (RegBase + CapabilityPtr);
    if (CapabilityEntry == 0xFFFFFFFF)
    {
      DEBUG(( EFI_D_WARN, "%a: Cap Reg not accessible\r\n", __func__));
      return EFI_NOT_FOUND;
    }
    CapabilityID = (UINT16) CapabilityEntry;

    if (CapabilityID == CapID)
    {
      *CapOffset = CapabilityPtr;
      return EFI_SUCCESS;
    }

    CapabilityPtr = (CapabilityEntry & HWIO_PCIE_GEN3X1_L1SUB_CAP_HEADER_REG_NEXT_OFFSET_BMSK)
                                     >> HWIO_PCIE_GEN3X1_L1SUB_CAP_HEADER_REG_NEXT_OFFSET_SHFT;
  }while(CapabilityPtr != 0);

  return EFI_NOT_FOUND;
}

/***************************************************************************//**
 * @fn PcieConfigLibSetupL1ss
 * @brief Configure L1ss
 *
 * This function will configure link parameters for L1ss
 *
 * @param[in] Root port index in portInfo_t struct
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibSetupL1ss(UINT32 RootPortIndex)
{
  rootPort_t         * pcie_root_port_info = PcieConfigLibGetPciRootPortInfo();
  CONST memRegions_t * mem;
  portInfo_t         * port;
  UINT32              TempReg;
  UINT8               PcieCapBaseRp = 0;
  UINT8               PcieCapBaseEp = 0;
  UINTN               EndPointConfigAddr;
  EFI_STATUS          Status = EFI_SUCCESS;

  if(pcie_root_port_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: root port info is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  if (RootPortIndex >= pcie_root_port_info->num_rp)
  {
    DEBUG(( EFI_D_WARN, "%a: No RootPorts Left to configure\r\n", __func__));
    return EFI_SUCCESS;
  }

  if (pcie_root_port_info->port_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: port info is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  port = &pcie_root_port_info->port_info[RootPortIndex];

  /*
   * Get the memory regions base address
   */
  mem = port->mem;
  if (mem == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: mem regiosn is NULL\r\n", __func__));
    return EFI_LOAD_ERROR;
  }

  if (port->link->trained)
  {
    Status = LocateCapabilityRegBlock(EFI_PCI_PCIE_CAP_ID,
                                      mem[axi_reg].pa,
                                      &PcieCapBaseRp);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: PCIe CapOffset not found for %d root complex\r\n",
              __func__, RootPortIndex));
      goto L1ssSetupFunctionEnd;
    }

    /* Get the endpoint base address from the iATU viewport registers */
    EndPointConfigAddr = MmioRead32((
      HWIO_PCIE_GEN3X1_IATU_LWR_BASE_ADDR_OFF_OUTBOUND_i_OFFS(OutCfg) + mem[axi_reg].pa));

    Status = LocateCapabilityRegBlock(EFI_PCI_PCIE_CAP_ID,
                                                  EndPointConfigAddr,
                                                  &PcieCapBaseEp);
    if EFI_ERROR (Status)
    {
      DEBUG(( EFI_D_WARN, "%a: PCIe CapOffset not found for EP of %d root complex\r\n",
              __func__, RootPortIndex));
      goto L1ssSetupFunctionEnd;
    }

    /*
     * Perform Common Clock Configuration and Link Retrain
     * before enabling ASPM
     */

    /* Enable Common Clock Configuration for Root port */
    MmioOr16(mem[axi_reg].pa + PcieCapBaseRp + link_control_offset,
      HWIO_PCIE_GEN3X1_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_COMMON_CLK_CONFIG_BMSK);
    /* Enable Common Clock Configuration for End Point */
    MmioOr16(EndPointConfigAddr + PcieCapBaseEp + link_control_offset,
      HWIO_PCIE_GEN3X1_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_COMMON_CLK_CONFIG_BMSK);
    /* Re-Train the link */
    MmioOr16(mem[axi_reg].pa + PcieCapBaseRp + link_control_offset,
      HWIO_PCIE_GEN3X1_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_RETRAIN_LINK_BMSK);
    /*
     * Wait for link to retrain
     */
    PcieConfigLibPollBitMsk(
      mem[axi_reg].pa + PcieCapBaseRp + link_control_offset,
      HWIO_PCIE_GEN3X1_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_TRAINING_BMSK,
      FALSE,
      100);

    /* PCIE_1_TYPE0_DEVICE_CONTROL2_DEVICE_STATUS2_REG_1.PCIE_CAP_LTR_EN Enable */
    TempReg =  HWIO_PCIE_GEN3X1_TYPE0_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_LTR_EN_BMSK;
    MmioOr32(EndPointConfigAddr + PcieCapBaseEp + device_control2_offset, TempReg);

    /* PCIE_1_TYPE0_DEVICE_CONTROL2_DEVICE_STATUS2_REG_1.PCIE_CAP_LTR_EN Enable */
    TempReg =  HWIO_PCIE_GEN3X1_TYPE0_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_LTR_EN_BMSK;
    MmioOr32(mem[axi_reg].pa + PcieCapBaseRp + device_control2_offset, TempReg);

    /* Enable DBI_RO_WR_EN to access CS1 region */
    MmioWrite32 ((HWIO_OFFS(PCIE_GEN3X1_MISC_CONTROL_1_OFF) + mem[axi_reg].pa),
      HWIO_PCIE_GEN3X1_MISC_CONTROL_1_OFF_DBI_RO_WR_EN_BMSK);
    MemoryFence();

    /* Optionality Compliance */
    TempReg = MmioRead32 (mem[axi_reg].pa + PcieCapBaseRp + link_capabilities_offset);
    TempReg |= HWIO_PCIE_GEN3X1_LINK_CAPABILITIES_REG_PCIE_CAP_ASPM_OPT_COMPLIANCE_BMSK;
    /* clear AS_LINK_PM_SUPT */
    TempReg = TempReg & (~(UINT32)(HWIO_PCIE_GEN3X1_LINK_CAPABILITIES_REG_PCIE_CAP_ACTIVE_STATE_LINK_PM_SUPPORT_BMSK));
    /* Disable L0s & Enable L1 */
    TempReg = TempReg | (port->link->aspm << HWIO_PCIE_GEN3X1_LINK_CAPABILITIES_REG_PCIE_CAP_ACTIVE_STATE_LINK_PM_SUPPORT_SHFT);
    MmioWrite32 (mem[axi_reg].pa + PcieCapBaseRp + link_capabilities_offset, TempReg);

    /* Disable DBI_RO_WR_EN bit to access CS1 region */
    MmioWrite32 ((HWIO_OFFS(PCIE_GEN3X1_MISC_CONTROL_1_OFF) + mem[axi_reg].pa),
                 !(HWIO_PCIE_GEN3X1_MISC_CONTROL_1_OFF_DBI_RO_WR_EN_BMSK));
    MemoryFence();
  }

L1ssSetupFunctionEnd:
  return EFI_SUCCESS;
}

/***************************************************************************//**
 * @fn PcieConfigLibSetupEndPointL1ssCallBack
 * @brief Callback triggered by UEFI framework when associated timer
 *        expires. Trigger PcieConfigLibSetupEndPointL1ssCallBack on timer expiry.
 *        to configure L1 PM registers pertaining to Endpoint
 *
 * @param[in] Event   Event associated with the timer
 * @param[in] Context Callback data
 *
 * @return none
 ******************************************************************************/
VOID PcieConfigLibSetupEndPointL1ssCallBack (
    EFI_EVENT Event,
    VOID *Context)
{
  rootPort_t         * pcie_root_port_info = PcieConfigLibGetPciRootPortInfo();
  UINT32               RpL1Cap, EpL1Cap, L1Cap;
  UINT32               i, NumRp;
  UINT16               RpL1SubCapBase = 0;
  UINT16               EpL1SubCapBase = 0;
  UINTN                EpEcamBase;
  CONST memRegions_t * mem;
  portInfo_t         * port;
  EFI_STATUS  Status = EFI_SUCCESS;

  if(pcie_root_port_info == NULL)
  {
    DEBUG(( EFI_D_WARN, "%a: root port info is NULL\r\n", __func__));
    return;
  }

  NumRp = pcie_root_port_info->num_rp;

  for (i = 0; i < NumRp; i++)
  {
    port = &pcie_root_port_info->port_info[i];

    /* if the EP not present look for the next one */
    if (port->link->trained == FALSE)
      continue;

    mem = port->mem;

    Status = LocatePciExpressCapabilityRegBlock(EFI_PCIE_L1SS_PM_CAP_ID,
                                                mem[axi_reg].pa,
                                                &RpL1SubCapBase);
    if EFI_ERROR (Status)
    {
      /*
       * DEBUG(( EFI_D_INFO, "%a: CapOffset not found for %d root complex\r\n",
       *        __func__, RootPortIndex));
       */
      continue;
    }

    RpL1Cap = MmioRead32(mem[axi_reg].pa + RpL1SubCapBase + cap_reg_offset);

    /* Get the endpoint base address from the iATU viewport registers */
    EpEcamBase = MmioRead32((
      HWIO_PCIE_GEN3X1_IATU_LWR_BASE_ADDR_OFF_OUTBOUND_i_OFFS(OutCfg) + mem[axi_reg].pa));

    Status = LocatePciExpressCapabilityRegBlock(EFI_PCIE_L1SS_PM_CAP_ID,
                                                EpEcamBase,
                                                &EpL1SubCapBase);
    if EFI_ERROR (Status)
    {
      /*
       * DEBUG(( EFI_D_INFO, "%a: L1ss CapOffset not found for EP of %d root complex\r\n",
       *      __func__, RootPortIndex));
       */
      continue;
    }

    EpL1Cap = MmioRead32(EpEcamBase + EpL1SubCapBase + cap_reg_offset);

    /*
     * Enable the common states bwtween EP & RP from the following:
     * L1_2_PCIPM_EN | L1_1_PCIPM_EN | L1_2_ASPM_EN | L1_1_ASPM_EN
     */
    L1Cap = ((EpL1Cap & RpL1Cap) & 0xF);

    /* Clear the L1ss Enable Bits */
    MmioAnd32(mem[axi_reg].pa + RpL1SubCapBase + control1_reg_offset, ~0xF);


    /* Write LTR Threshold value and scale */
    MmioBitFieldWrite32(mem[axi_reg].pa + RpL1SubCapBase + control1_reg_offset,
                        HWIO_PCIE_GEN3X1_L1SUB_CONTROL1_REG_L1_2_TH_VAL_SHFT,
                        HWIO_PCIE_GEN3X1_L1SUB_CONTROL1_REG_L1_2_TH_VAL_SHFT+9,
                        port->link->ltr_threshold_val);
    MmioBitFieldWrite32(mem[axi_reg].pa + RpL1SubCapBase + control1_reg_offset,
                        HWIO_PCIE_GEN3X1_L1SUB_CONTROL1_REG_L1_2_TH_SCA_SHFT,
                        HWIO_PCIE_GEN3X1_L1SUB_CONTROL1_REG_L1_2_TH_SCA_SHFT+2,
                        port->link->ltr_threshold_scale);


    /* Enable L1ss */
    MmioOr32(mem[axi_reg].pa + RpL1SubCapBase + control1_reg_offset, L1Cap);
    /* Endpoint L1ss should be enable only after the Root port L1ss is enabled */
    MemoryFence();
    /* Clear the L1ss Enable Bits */
    MmioAnd32(EpEcamBase + EpL1SubCapBase + control1_reg_offset, ~0xF);

    /* Write LTR Threshold value and scale */
    MmioBitFieldWrite32(EpEcamBase + EpL1SubCapBase + control1_reg_offset,
                        HWIO_PCIE_GEN3X1_L1SUB_CONTROL1_REG_L1_2_TH_VAL_SHFT,
                        HWIO_PCIE_GEN3X1_L1SUB_CONTROL1_REG_L1_2_TH_VAL_SHFT+9,
                        port->link->ltr_threshold_val);
    MmioBitFieldWrite32(EpEcamBase + EpL1SubCapBase + control1_reg_offset,
                        HWIO_PCIE_GEN3X1_L1SUB_CONTROL1_REG_L1_2_TH_SCA_SHFT,
                        HWIO_PCIE_GEN3X1_L1SUB_CONTROL1_REG_L1_2_TH_SCA_SHFT+2,
                        port->link->ltr_threshold_scale);

    /* Enable L1ss */
    MmioOr32(EpEcamBase + EpL1SubCapBase + control1_reg_offset, L1Cap);
  }
}
