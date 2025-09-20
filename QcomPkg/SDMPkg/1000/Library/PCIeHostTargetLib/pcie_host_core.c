/**
  @file pcie_core.c
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

#include "com_dtypes.h"
#include "pcie_host.h"
#include "pcie_host_core.h"
#include "pcie_host_tgt_cfg.h"
#include "pcie_host_osal.h"
#include "pcie_hwio.h"
#include "HALhwio.h"

/** PCIe root port devices */
pcierc_dev_t pcierc_devs[PCIERC_MAX_DEVICES];

/** Number of RCs */
uint32 pcierc_num_rcs;


/* ============================================================================
**  HWIO Read and Write helper functions
** ============================================================================
*/

void pcierc_core_write_reg(uintnt base,uint32 offset,uint32 val)
{
    uintnt addr = base + offset;
    (*((volatile uint32 *)(addr))) = val;
}

void pcierc_core_write_reg_field(uintnt base,uint32 offset,uint32 mask, uint32 shift, uint32 field_val)
{
    uintnt addr = base + offset;
    uint32 regval = (*((volatile uint32 *)(addr)));

    regval &= ~mask;
    regval = regval | (field_val << shift);
    (*((volatile uint32 *)(addr))) = regval;
}

uint32 pcierc_core_read_reg(uintnt base,uint32 offset)
{
    uintnt addr = base + offset;

    return (*((volatile uint32 *)(addr)));
}

uint32 pcierc_core_read_reg_field(uintnt base,uint32 offset,uint32 mask, uint32 shift)
{
    uintnt addr = base + offset;
    uint32 regval = (*((volatile uint32 *)(addr)));

    regval &= mask;
    regval >>= shift;
    return regval;
}



/* ============================================================================
**  Function : pcierc_core_init_phy_cfg
** ============================================================================
*/
/**
    @brief
    Initializes the PHY's configuration

    @details
    Initializes the PHY's configuration

    @param[in]  *mem        handle to the rootport's memory descriptor
    @param[in]  *phy_seq    Sequence to be applied to the PHY

    @return
    Status of the operation
*/
static pcie_host_status_t pcierc_core_init_phy_cfg(const pcie_host_mem_region_t *mem, const pcie_host_phy_seq_t *phy_seq)
{
    uint32 index = 0, ret = PCIE_HOST_SUCCESS;
    uint32 reg_addr;
    uint32 Temp;

    if (mem == NULL || phy_seq == NULL)
      return PCIE_HOST_ERROR;

    /*
     * This while loop will work on the Operation flag from
     * the PHY init sequence array and perform one of the following:
     *
     * o Delay of specified micro seconds
     * o Read Modify write of a specified register with mask and value
     * o Write into the specified register
     */
    while(phy_seq[index].opflag != PHY_REG_NOP)
    {
        //MK : XBL uses pa so we are ok for now...
      reg_addr = mem[phy_seq[index].base].pa + phy_seq[index].offset;

      switch (phy_seq[index].opflag)
      {
      case PHY_REG_DELAY:
        pcie_host_osal_delay(phy_seq[index].val);
      break;
      case PHY_REG_RMW:

        Temp = pcierc_core_read_reg(mem[phy_seq[index].base].pa , phy_seq[index].offset);
        Temp = Temp & ~(phy_seq[index].mask);
        Temp = Temp | phy_seq[index].val;
        pcierc_core_write_reg(mem[phy_seq[index].base].pa , phy_seq[index].offset, Temp);
      break;
      case PHY_REG_WRI:
        pcierc_core_write_reg(mem[phy_seq[index].base].pa , phy_seq[index].offset, phy_seq[index].val);
      break;
      case PHY_REG_DSB:
          pcie_host_osal_barrier();
      break;
      case PHY_REG_POLL:
          {
              uint32 shift = __builtin_ctz((phy_seq[index].mask));
              uint32 cnt = 100; //MKTODO: Constant...

              /*
               * Read once before the condition check, as there is a posibilty
               * that the first read of the status register will have the
               * condition met and we dont have to stall for even one cycle
               */
              Temp = pcierc_core_read_reg(mem[phy_seq[index].base].pa , phy_seq[index].offset);
              Temp = (Temp & (phy_seq[index].mask)) >> shift;

              while ( (Temp != (phy_seq[index].val)) && (cnt >= 0))
              {
                pcie_host_osal_delay(100); //MKTODO: Constant...
                Temp = pcierc_core_read_reg(mem[phy_seq[index].base].pa , phy_seq[index].offset);
                Temp = (Temp & (phy_seq[index].mask)) >> shift;
                cnt--;
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
              ret = ((cnt>=0)?PCIE_HOST_SUCCESS:PCIE_HOST_ERROR);
              break;
          }
      case PHY_REG_NOP:
      default:
      break;
      }

      index++;
    }

    return ret;
}

/* ============================================================================
**  Function : pcierc_core_init_phy
** ============================================================================
*/
/**
    @brief
    Initialize the phy

    @details
    Initialize the phy

    @param[in]  *rc    handle to the root port

    @return
    Status of the operation
*/
static pcie_host_status_t pcierc_core_init_phy(pcie_host_rc_config_t *rc)
{
    uint32 chip_version;
    pcie_host_status_t retval = PCIE_HOST_SUCCESS;
    const pcie_host_phy_cfg_t *phy_cfg;

    /* First get the chip version */
    chip_version = pcie_osal_get_chipversion();
    if (chip_version == 0xFFFFFFFF)
    {
        return PCIE_HOST_ERROR;
    }
    phy_cfg = rc->phy_config;
    while (phy_cfg->version !=  PCIE_SOC_VER_STOP)
    {
        if (phy_cfg->version <=chip_version)
        {
            retval = pcierc_core_init_phy_cfg(rc->mem, phy_cfg->phy_seq);
            if (retval != PCIE_HOST_SUCCESS)
            {
                return retval;
            }
        }
        phy_cfg++;
    }
    return retval;
}

/* ============================================================================
**  Function : pcierc_core_setup_ob_iatu
** ============================================================================
*/
/**
    @brief
    Sets up the outbound ATU to generate appropriate TLP traffic

    @details
    Sets up the outbound ATU to generate appropriate TLP traffic

    @param[in]  rc_base          Base address of the rootport's DBI space
    @param[in]  atu_index        Index of the ATU region to be configured
    @param[in]  type             Type of the TLPs the translation should be for
    @param[in]  cpu_mem_base     Base of the CPU visible memory region
    @param[in]  cpu_mem_limit    Limit of the CPU visible memory region
    @param[in]  pcie_mem_base    Base of the PCIe address space

    @return
    Status of the operation
*/
pcie_host_status_t pcierc_core_setup_ob_iatu(
   uint64 rc_base,
   pcierc_atus_t atu_index,
   pcie_host_tlptype_t type,
   uint64 cpu_mem_base,
   uint64 cpu_mem_limit,
   uint64 pcie_mem_base
   )
{
      /* Step 1. Setup the Region Base and Limit Address Registers. */
  pcierc_core_write_reg(rc_base, HWIO_PCIE_GEN3X1_IATU_LWR_BASE_ADDR_OFF_OUTBOUND_i_OFFS(atu_index),
   (uint32)cpu_mem_base);
  pcierc_core_write_reg(rc_base,HWIO_PCIE_GEN3X1_IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_i_OFFS(atu_index),
   (uint32)(cpu_mem_base >> 32));
  pcierc_core_write_reg(rc_base, HWIO_PCIE_GEN3X1_IATU_LIMIT_ADDR_OFF_OUTBOUND_i_OFFS(atu_index),
              (uint32)cpu_mem_limit);
  /* Step 2. Setup the Target Address Registers. */
  pcierc_core_write_reg(rc_base, HWIO_PCIE_GEN3X1_IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_i_OFFS(atu_index),
              (uint32)pcie_mem_base);
  pcierc_core_write_reg(rc_base, HWIO_PCIE_GEN3X1_IATU_UPPER_TARGET_ADDR_OFF_OUTBOUND_i_OFFS(atu_index),
              (uint32)(pcie_mem_base >> 32));
  /* Step 3. Configure the region through the Region Control 1 Register. */
  pcierc_core_write_reg(rc_base, HWIO_PCIE_GEN3X1_IATU_REGION_CTRL_1_OFF_OUTBOUND_i_OFFS(atu_index),
              type);
  /* Step 4. Enable the region. */
  pcierc_core_write_reg(rc_base, HWIO_PCIE_GEN3X1_IATU_REGION_CTRL_2_OFF_OUTBOUND_i_OFFS(atu_index),
              HWIO_PCIE_GEN3X1_IATU_REGION_CTRL_2_OFF_OUTBOUND_i_REGION_EN_BMSK);

  return PCIE_HOST_SUCCESS;
}

/* ============================================================================
**  Function : pcierc_core_setup_iatu
** ============================================================================
*/
/**
    @brief
    Sets up iATU

    @details
    Sets up Configuration space iATU for specified root port

    @param[in]  rc_base    Base Address of the rootport's DBI space

    @return
    Status of the operation
*/
pcie_host_status_t pcierc_core_setup_iatu(uint64 rc_base)
{
  uint64 ep_cfg_mem_base, ep_cfg_mem_limit;
  pcie_host_status_t status;

  ep_cfg_mem_base = rc_base + PCIE_SINGLE_BUS_ADDR_SPACE_SIZE;
  ep_cfg_mem_limit = ep_cfg_mem_base +
                               PCIE_SINGLE_BUS_ADDR_SPACE_SIZE -1;

  status = pcierc_core_setup_ob_iatu (  rc_base,
                                           OutCfg,
                                           PCIE_HOST_TLP_CFG0RDWR,
                                           ep_cfg_mem_base,
                                           ep_cfg_mem_limit,
                                           (1 << 24) /* Read the config space of Bus 1 */ );
  if (status != PCIE_HOST_SUCCESS)
  {
    pcie_host_osal_log(PCIE_HOST_LOG_ERROR, "Outbound ATU mapping failed\r\n");
  }

  return status;
}

/* ============================================================================
**  Function : pcierc_core_config_ecam
** ============================================================================
*/
/**
    @brief
    Sets up ECAM Blocker regions

    @details
    Sets up ECAM blocker regions for the specified root port

    @param[in]  *rc_mem              handle to the root port's memory descriptor
    @param[in]  unblock_axi_range    Flag to unblock AXI range

    @return
    Status of the operation

    @sa
    Other_Relevant_APIS_or_None
*/
pcie_host_status_t pcierc_core_config_ecam(const pcie_host_mem_region_t * rc_mem, uint32 unblock_axi_range)
{
  uint64              val = 0;
  /* ECAM base */
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_ECAM_BASE), (uint32)rc_mem[axi_reg].pa);

  /* ECAM blocker wr & rd base for RC */
  val = rc_mem[axi_reg].pa + PCIE_CONFIG_SPACE_LENGTH;
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_WR_BASE),
              (uint32)val);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_WR_BASE_HI),
              (uint32)(val >> 32));

  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_RD_BASE),
              (uint32)val);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_RD_BASE_HI),
              (uint32)(val >> 32));

  /* ECAM blocker wr & rd limit for RC */
  if (unblock_axi_range == TRUE)
  {
    val = rc_mem[axi_reg].pa + (PCIE_SINGLE_BUS_ADDR_SPACE_SIZE -1);
  }
  else
  {
    /*
     * Hide the entire PCIe AXI range starting from Root complex
     * ECAM end, since there will be no endpoint to access
     */
    val = rc_mem[axi_reg].pa + (rc_mem[axi_reg].sz - 1);
  }

  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_WR_LIMIT),
              (uint32)val);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_WR_LIMIT_HI),
              (uint32)(val >> 32));

  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_RD_LIMIT),
                (uint32)val);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_RD_LIMIT_HI),
              (uint32)(val >> 32));

  /* ECAM blocker wr & rd base for EP */
  val = rc_mem[axi_reg].pa + PCIE_SINGLE_BUS_ADDR_SPACE_SIZE + PCIE_CONFIG_SPACE_LENGTH;
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_WR_BASE_2)   ,
              (uint32)val);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_WR_BASE_2_HI),
              (uint32)(val >> 32));

  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_RD_BASE_2)   ,
              (uint32)val);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_RD_BASE_2_HI),
              (uint32)(val >> 32));

  /* ECAM blocker wr & rd limit for EP */
  val = rc_mem[axi_reg].pa + PCIE_SINGLE_BUS_ADDR_SPACE_SIZE + (PCIE_SINGLE_BUS_ADDR_SPACE_SIZE -1);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_WR_LIMIT_2),
              (uint32)val);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_WR_LIMIT_2_HI),
              (uint32)(val >> 32));

  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_RD_LIMIT_2),
              (uint32)val);
  pcierc_core_write_reg(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_BLOCK_SLV_AXI_RD_LIMIT_2_HI),
              (uint32)(val >> 32));

  /* Enable ECAM blocker regions in the end after the configurations */
  if (unblock_axi_range == TRUE)
  {
      pcierc_core_write_reg_field(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_SYS_CTRL),HWIO_PCIE_GEN3X1_PCIE_PARF_SYS_CTRL_ECAM_BLOCKER_EN_BMSK, HWIO_PCIE_GEN3X1_PCIE_PARF_SYS_CTRL_ECAM_BLOCKER_EN_SHFT,0x1);
      pcierc_core_write_reg_field(rc_mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_SYS_CTRL),HWIO_PCIE_GEN3X1_PCIE_PARF_SYS_CTRL_ECAM_BLOCKER_EN_RANGE2_BMSK, HWIO_PCIE_GEN3X1_PCIE_PARF_SYS_CTRL_ECAM_BLOCKER_EN_RANGE2_SHFT,0x1);
  }
  else
  {
    /*
     * Hide the entire PCIe AXI range starting from Root complex
     * ECAM end, since there will be no endpoint to access
     */
      pcierc_core_write_reg_field(rc_mem[ahb_reg].pa, PCIE_OMS(PCIE_GEN3X1_PCIE_PARF_SYS_CTRL, ECAM_BLOCKER_EN), 0x1);
  }

  return PCIE_HOST_SUCCESS;
}

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
void pcierc_core_get_resources(pcierc_dev_t *rc_dev, uint8 num_buses, pcierc_res_t *res)
{
    //TODO : Sanity check the range
    uint64 ecam_space = num_buses * PCIE_SINGLE_BUS_ADDR_SPACE_SIZE;
    res->mmio_space.pa = rc_dev->rc_cfg->mem[axi_reg].pa + ecam_space;
    res->mmio_space.sz = rc_dev->rc_cfg->mem[axi_reg].sz - ecam_space;

    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG, "ECAM Space: 0x%X--0x%X MMIO Space: 0x%X--0x%X",
                       rc_dev->rc_cfg->mem[axi_reg].pa, (rc_dev->rc_cfg->mem[axi_reg].pa+ecam_space-1),
                       res->mmio_space.pa, (res->mmio_space.pa+res->mmio_space.sz -1));


    /* TODO: No IOSpace support at the moment..but no current EPs need them anyways */

}

/* ============================================================================
**  Function : pcierc_core_post_linkup
** ============================================================================
*/
/**
    @brief
    Performs post link up configuration

    @details
    Performs post link up configuration to the controller

    @param[in]  *rc_dev    handle to the root port device

    @return
    Status of the operation
*/
pcie_host_status_t pcierc_core_post_linkup( pcierc_dev_t *rc_dev)
{
    pcie_host_status_t status;

    /* Configure Outbound ATUs */
    status = pcierc_core_setup_iatu(rc_dev->rc_cfg->mem[axi_reg].pa);
    if (status != PCIE_HOST_SUCCESS)
    {
        pcie_host_osal_log(PCIE_HOST_LOG_ERROR,"Setting up ATUs failed");
        return status;
    }

    /* Configure ECAM access*/
    status = pcierc_core_config_ecam(rc_dev->rc_cfg->mem, rc_dev->link_trained);
    if (status != PCIE_HOST_SUCCESS)
    {
        pcie_host_osal_log(PCIE_HOST_LOG_ERROR,"Setting up ECAM failed");
        return status;
    }
    return status;
}

/* ============================================================================
**  Function : pcierc_core_islinkup
** ============================================================================
*/
/**
    @brief
    Provides the stauts of the link for the rootport

    @details
    provides the status of the link for the rootport

    @param[in]  rc    Root port descriptor

    @return
    Returns the link state
*/
uint32 pcierc_core_islinkup(pcie_host_rc_config_t *rc)
{
    return pcierc_core_read_reg_field(rc->mem[ahb_reg].pa,PCIE_OMS(PCIE_GEN3X1_PCIE_PARF_PM_STTS, XMLH_LINK_UP));
}

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
void pcierc_core_init (void)
{
  uint32 temp;
  uint32 current_rc=0;
  pcie_host_rc_config_t *rc;
  uint32 count, step=5000; //TODO: Should be in the target config

  /* Procure the target files */
  pcie_host_tgt_cfg_init();

  /* Link training timeout in usecs */
  count = 0;

  pcierc_num_rcs = 0;

  for (current_rc = 0; current_rc < pcie_host_tgt_config->num_rcs;current_rc++)
  {
      rc = pcie_host_tgt_config->rc_cfg[current_rc];

    /* Initialize the LDOs */
    pcie_osal_init_ldos(rc->pmic_config);

    /* Initialize the Clocks */
    pcie_osal_init_clocks(rc->clk_config);

    //XBL has one to one in memory...so not much to do

    /* Initialize the PHY */
    pcierc_core_init_phy(rc);

    /* Initialize the GPIOs */
    pcie_osal_init_gpios(rc->gpio_config);

    /* Enable Link Training */
    do
    {
      pcierc_core_write_reg_field(rc->mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_LTSSM),
               HWIO_PCIE_GEN3X1_PCIE_PARF_LTSSM_LTSSM_EN_BMSK, HWIO_PCIE_GEN3X1_PCIE_PARF_LTSSM_LTSSM_EN_SHFT, 1);
      temp = pcierc_core_read_reg_field(rc->mem[ahb_reg].pa, HWIO_OFFS(PCIE_GEN3X1_PCIE_PARF_LTSSM),
               HWIO_PCIE_GEN3X1_PCIE_PARF_LTSSM_LTSSM_EN_BMSK, HWIO_PCIE_GEN3X1_PCIE_PARF_LTSSM_LTSSM_EN_SHFT);
    } while (!(temp & 1));


    /* Check for link up */
    while (count < pcie_host_tgt_config->link_training_timeout_us)
    {
        if (pcierc_core_islinkup(rc))
        {
            pcie_host_osal_log(PCIE_HOST_LOG_INFO,"Pcie rootport %d successfully trained its link",current_rc);
            break;
        }
        pcie_host_osal_delay(step);
        count = count +step;
    }
    pcierc_devs[pcierc_num_rcs].rc_cfg = rc;

    if (pcierc_core_islinkup(rc))
    {
        /* Add the pcie root port to the list of discovered devices for the host driver to scan */
        pcierc_devs[pcierc_num_rcs].link_trained = TRUE;
    }
    else
    {
        pcierc_devs[pcierc_num_rcs].link_trained = FALSE;
        /* Optionally we might actually turn off the port to save power .. Future use case */
    }

    /* Perform post link up operations */
    pcierc_core_post_linkup(&pcierc_devs[pcierc_num_rcs]);
    pcierc_num_rcs++;
  }

  return;
}

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
void pcierc_core_getrcs(pcierc_dev_t ** rc_devs, uint32 *num_rcs)
{
    *rc_devs =  pcierc_devs;
    *num_rcs = pcierc_num_rcs;
}
