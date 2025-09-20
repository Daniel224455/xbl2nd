/**
  @file pcie_core.h
  @brief Internal interface include file for the PCIe Core layer.

  This file contains definitions of constants, data structures, and
  interfaces that provide operational control over a QC specific PCIe 
  root port controller 

*/
/*
===============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------
10/18/18   MK      Created

===============================================================================
                   Copyright (c) 2018 QUALCOMM Technologies Incorporated
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/

#ifndef PCIE_HOST_CORE_H
#define PCIE_HOST_CORE_H

#include "com_dtypes.h"
#include "pcie_host_tgt_cfg.h"
/***************************************************************************//**
 * @enum iATUIndex_t
 * @brief Address space identified for iATU
 *
 * viewport region index number for SNPS iATU configuration
 ******************************************************************************/
typedef enum
{
  OutMem = 0,
  OutCfg = 1,
  OutIo = 2,
  InMem = 3,
  MaxAsRegions=4
} pcierc_atus_t;

/** Bus address space (1MB) */
#define PCIE_SINGLE_BUS_ADDR_SPACE_SIZE                  (0x100000)
/** Config address space (4kB) */
#define PCIE_CONFIG_SPACE_LENGTH                         (0x1000)

/** Condenses bus,dev,function values to a single uint32 integer */
#define PCIE_BDF(b,d,f) (((b&0xFF)<<8)|((d&0x1F)<<3)|((f&0x7)))
/** Condenses rootport, bus,dev,function values to a single uint32 integer */
#define PCIE_RBDF(r,b,d,f) (((r&0x7)<<16)|((b&0xFF)<<8)|((d&0x1F)<<3)|((f&0x7)))
/** retrieves the bdf value from a rbdf value */
#define PCIE_GET_BDF(rbdf) (rbdf &0xFFFF)
/** retrieves the rootport value from a rbdf value */
#define PCIE_GET_RC(rbdf) ((rbdf&0x70000)>>16)
/** splits the rbdf value into bus, dev, func (for printing purposes) */
#define PCIE_SPLIT_BDF(rbdf) (rbdf>>8)&0xFF, (rbdf>>3)&0x1F, (rbdf&0x7)

/** Macro to pack primary, secondary and subordinate buses to their respective fields*/
#define PCIE_BUS_CFG(pri,sec,sub) ((sub & 0xFF)<<16|(sec & 0xFF)<<8|(pri&0xFF))
/** Macro to pack memory base and limit values to their respective fields*/
#define PCIE_MEM_CFG(base, limit) ( (((limit>>20)&0xFFF)<<20)|(((base>>20)&0xFFF)<<4) )

/** Macro to align a number to the next higher boundary*/
#define UPALIGN(val,align) (((val)+(align)-1) & (-(align)))

/** Macro to represent 1MB */
#define PCIE_MEM_1MB (1024 * 1024)

/** Max number of root ports */
#define PCIERC_MAX_DEVICES 1

/** PCIe root complex descriptor */
typedef struct {
    pcie_host_rc_config_t *rc_cfg; /**< Root complex config descriptor*/
    uint8 link_trained;            /**< Flag to indicate if link was trained */
}pcierc_dev_t;

/**< Resource descriptor */
typedef struct {
    pcie_host_mem_region_t mmio_space; /**< MMIO space available for a given rootport*/
    //MSI resources    
}pcierc_res_t;

/**< Offset, Mask and Shift macro to be used with HWIO macros */
#define PCIE_OMS(reg, field)  HWIO_OFFS(reg), HWIO_FMSK(reg,field), HWIO_SHFT(reg,field)

/* ============================================================================
**  HWIO Read and Write helper functions
** ============================================================================
*/
void pcierc_core_write_reg(uintnt base,uint32 offset,uint32 val);
void pcierc_core_write_reg_field(uintnt base,uint32 offset,uint32 mask, uint32 shift, uint32 field_val);
uint32 pcierc_core_read_reg(uintnt base,uint32 offset);
uint32 pcierc_core_read_reg_field(uintnt base,uint32 offset,uint32 mask, uint32 shift);

/* ============================================================================
**  Function : pcierc_core_init
** ============================================================================
*/
/**
    @brief
    Initializes the core layer and attempts to initialize the link.
    
    @details
    Initializes the core layer and attempts to initialize the link. The current
    design doesn't allow the environment to control the root port to initialize.
    
    @param  None
   
    @return
    None
    
    @sa
    Other_Relevant_APIS_or_None
*/
void pcierc_core_init (void);

/* ============================================================================
**  Function : pcierc_core_getrcs
** ============================================================================
*/
/**
    @brief
    Retrieves the number of rootports present in the system
    
    @details
    Retrieves the number of rootports present in the system. This is used by
    the host driver to scan and enumerate devices behind each of the root ports
    
    @param[in]  *rc_devs    A pointer to an array containing the rootports in the system
    @param[in]  *num_rcs    Number of rootports

    @dependencies
    pcierc_core_init should have been called prior to this call.
 
    @return
    None
*/
void pcierc_core_getrcs(pcierc_dev_t ** rc_devs, uint32 *num_rcs);

/* ============================================================================
**  Function : pcierc_core_get_resources
** ============================================================================
*/
/**
    @brief
    Gets the resources available to the device
    
    @details
    Gets the resources available to the root device. Currently only
    non-prefetchable memory is assigned (excluding the ECAM region  consumed
    by the bus topology under the root port)
    
    @param[in]  *rc_dev      handle to the rootport device
    @param[in]  num_buses    Number of buses managed by the rootport device
    @param[out] *res         Resource descriptor to be filled with
 
    @return
    None
*/
void pcierc_core_get_resources(pcierc_dev_t *rc_dev, uint8 num_buses, pcierc_res_t *res);

#endif
