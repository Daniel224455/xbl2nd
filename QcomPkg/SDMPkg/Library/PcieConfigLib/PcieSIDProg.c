/** 
 @file PcieSIDProg.c

  This file implements routines to program BDF to SID table in PARF region.

  Copyright (c) 2017 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

**/


/*=============================================================================
                              EDIT HISTORY


 when       who    what, where, why
 --------   ---    ------------------------------------------------------------
 06/28/17   ts     Initial version for SDM1000
=============================================================================*/

/* Include Files */
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/BaseLib.h>
#include <Protocol/EFIHALIOMMUProtocol.h>
#include <Library/PcieConfigLib.h>

#include "PcieLink.h"
#include "PcieBsp.h"

#define MAX_PCIE_SIDS          128
#define MAX_BDF_TO_SID_TBL_CNT 256

/* 
 * Lookup table for bus:0->127, dev:0, fn:0
 * 
 * Table index can be retrived by indexing this array
 * with bus number as index. The table is computed
 * using the c function in the below comment by applying
 * crc-8 with polynomial X8+X2+X+1
 */

/***************************************************************************//**
 * @fn PcieConfigLibBdfToTableIndex
 * @brief calculates SID table index  
 *
 * This function converts 16-bit BDF entry into a 8-bit table index for
 * programming SID using crc-8 coeffient X^8+X^2+X+1
 *
 * @param[in] bdf, a 16-bit value representing PCI bus, device & function
 *
 * @return An 8-bit table index for programming SID

STATIC UINT8 PcieConfigLibBdfToTableIndex(UINT16 bdf)
{
  UINT8 byte, i, crc = 0;
  INT32 len;

  for(len=sizeof(bdf)-1; len >= 0 ; len--)
  {
    byte = (UINT8)((bdf>>(8*len)) & 0xFF);
    for(i = 0x80; i != 0; i >>= 1)
    {
      if(crc & 0x80)
      {
        crc <<= 1;
        crc ^= 0x07;
      }
      else
      {
        crc <<= 1;
      }
      if (byte & i)
        crc ^= 0x07;
    }
  }

  return crc;
}
 ******************************************************************************/
UINT8 Bdf2TblIndex[MAX_PCIE_SIDS] = {
                        0x00, 0x15, 0x2a, 0x3f, 0x54, 0x41, 0x7e, 0x6b, 
                        0xa8, 0xbd, 0x82, 0x97, 0xfc, 0xe9, 0xd6, 0xc3, 
                        0x57, 0x42, 0x7d, 0x68, 0x03, 0x16, 0x29, 0x3c, 
                        0xff, 0xea, 0xd5, 0xc0, 0xab, 0xbe, 0x81, 0x94, 
                        0xae, 0xbb, 0x84, 0x91, 0xfa, 0xef, 0xd0, 0xc5, 
                        0x06, 0x13, 0x2c, 0x39, 0x52, 0x47, 0x78, 0x6d, 
                        0xf9, 0xec, 0xd3, 0xc6, 0xad, 0xb8, 0x87, 0x92, 
                        0x51, 0x44, 0x7b, 0x6e, 0x05, 0x10, 0x2f, 0x3a, 
                        0x5b, 0x4e, 0x71, 0x64, 0x0f, 0x1a, 0x25, 0x30, 
                        0xf3, 0xe6, 0xd9, 0xcc, 0xa7, 0xb2, 0x8d, 0x98, 
                        0x0c, 0x19, 0x26, 0x33, 0x58, 0x4d, 0x72, 0x67, 
                        0xa4, 0xb1, 0x8e, 0x9b, 0xf0, 0xe5, 0xda, 0xcf, 
                        0xf5, 0xe0, 0xdf, 0xca, 0xa1, 0xb4, 0x8b, 0x9e, 
                        0x5d, 0x48, 0x77, 0x62, 0x09, 0x1c, 0x23, 0x36, 
                        0xa2, 0xb7, 0x88, 0x9d, 0xf6, 0xe3, 0xdc, 0xc9, 
                        0x0a, 0x1f, 0x20, 0x35, 0x5e, 0x4b, 0x74, 0x61
                       };

/* Local Macros */
#define BUS_DEV_FN_TO_BDF(bus, dev, fn) \
                                        ((UINT16)(bus & 0xFF) << 0x8) | \
                                        ((UINT16)(dev & 0x1F) << 0x3) | \
                                        ((UINT16)(fn & 0x7))

/***************************************************************************//**
 * @fn PcieConfigLibInitBdfToSidTable
 * @brief Sets up initial BDF to SID table as '0x00'
 *
 * Initializes BDF to SID table for specified root port as all zeroes
 *
 * @param[in] mem, memory region base address for specific root port
 *
 * @return VOID
 ******************************************************************************/
STATIC VOID PcieConfigLibInitBdfToSidTable(CONST memRegions_t * mem)
{
  INT32                i;

  for (i = 0; i < MAX_BDF_TO_SID_TBL_CNT; i++)
  {
    MmioWrite32(
      (HWIO_PCIE_GEN3X1_PCIE_PARF_BDF_TO_SID_TABLE_n_OFFS(i) + mem[ahb_reg].pa),
      0x0);
  }

  return;
}

/***************************************************************************//**
 * @fn PcieConfigLibSetupBDF2SIDTable
 * @brief Sets up BDF to SID table
 *
 * Sets up BDF to SID table for specified root port
 *
 * @param[in] mem, memory region base address for specific root port
 *
 * @return VOID
 ******************************************************************************/
STATIC VOID PcieConfigLibSetupBDF2SIDTable (CONST memRegions_t * mem)
{
  UINT8                bus = 0, dev = 0, fn = 0;
  UINT8                idx;
  UINT32               bdf_2_sid_val;

  /* Lock access to HW */
  MmioWrite32(
    (HWIO_PCIE_GEN3X1_PCIE_PARF_BDF_TO_SID_LOCK_OFFS + mem[ahb_reg].pa),
    0x1);

  /* Initialize BDF to SID table with 0 values */
  PcieConfigLibInitBdfToSidTable(mem);

  for (bus = 0; bus < MAX_PCIE_SIDS; bus++)
  {
    /* Get the HW table index for programming SID and BDF */
    idx = Bdf2TblIndex[bus];

    /*
     * All the value of index @Bdf2TblIndex is not overlapping
     * Since there is no overlapping, no need to program next
     *
     * Change this way of programming if dev & fn is non zero
     */
    bdf_2_sid_val = 0;
	/* 
	 * bdf_2_sid_val = BDF(31:16) | SID (15:8) | Next (7:0)
	 *
	 * where:
	 *        BDF = bus (16:8) | dev (7:3) | fn (2:0)
	 *        SID = bus number from 0 to 127
	 *        Next = zero (as there is no overlapping for now)
	 */
    bdf_2_sid_val = ((BUS_DEV_FN_TO_BDF(bus, dev, fn)) << 16) | bus << 8;

    MmioWrite32(HWIO_PCIE_GEN3X1_PCIE_PARF_BDF_TO_SID_TABLE_n_OFFS(idx) +
                mem[ahb_reg].pa,
                bdf_2_sid_val);
  }

  /* UnLock access to HW */
  MmioWrite32(
      (HWIO_PCIE_GEN3X1_PCIE_PARF_BDF_TO_SID_LOCK_OFFS + mem[ahb_reg].pa),
      0x0);

  return;
}

/***************************************************************************//**
 * @fn PcieConfigLibConfigureSMMU
 * @brief Configure the SMMU
 *
 * This function performs the following operations:
 *  o-> Enabled clock power domain for SMMU
 *  o-> Configure SMRn, S2CRn, CBAR0, and PARF BDF to SID registers
 *
 * @param[in] UINT32 RootPortIndex, root port index to be configured
 *
 * @return EFI_SUCCESS or appropriate error value
 ******************************************************************************/
EFI_STATUS PcieConfigLibConfigureSMMU(UINT32 RootPortIndex)
{
  rootPort_t         * pcie_root_port_info = PcieConfigLibGetPciRootPortInfo();
  portInfo_t         * port = &pcie_root_port_info->port_info[RootPortIndex];
  EFI_HAL_IOMMU_PROTOCOL * pHALIOMMUProtocol;
  HAL_IOMMU_fptable_t * fp_table = NULL;
  VOID                * p_pcie_domain_hdl;
  EFI_STATUS           Status = EFI_SUCCESS;

  /*
   * Write PARF BDF to SID registers
   */
  PcieConfigLibSetupBDF2SIDTable(port->mem);
  
  /* Call into HALIOMMU driver to program SIDs to SMMU context banks */
  Status = gBS->LocateProtocol(&gEfiHalIommuProtocolGuid,
                               NULL,
                               (VOID **)&pHALIOMMUProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG(( EFI_D_WARN, "%a: IOMMU LocateProtocol failed\r\n", __func__));
    return Status;
  }
  pHALIOMMUProtocol->_HAL_IOMMU_protocol_init(&fp_table);
  if(HAL_IOMMU_ERR_OK != fp_table->domain_create_func(&p_pcie_domain_hdl)) {
    DEBUG(( EFI_D_WARN, "%a: IOMMU domain create failed\r\n", __func__));
    return EFI_NOT_STARTED;
  }
  if(HAL_IOMMU_ERR_OK != fp_table->config_bypass_domain_func(p_pcie_domain_hdl, HAL_IOMMU_AARCH64_CCA_DOMAIN_TYPE)) {
    DEBUG(( EFI_D_WARN, "%a: IOMMU domain configure failed\r\n", __func__));
    return EFI_UNSUPPORTED;
  }
  if (HAL_IOMMU_ERR_OK != fp_table->domain_attach_func(p_pcie_domain_hdl,
                                                       (void *)port->iommu_hdl, 0x0, 0x0))
  {
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}
