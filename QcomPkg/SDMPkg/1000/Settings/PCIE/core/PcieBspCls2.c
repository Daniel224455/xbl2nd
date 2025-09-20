/*============================================================================
  FILE:         PCieBsp.c

  OVERVIEW:     BSP for Pcie.

  DEPENDENCIES: None

Copyright (c) 2019 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  09/12/19    ts   ClamShell 2 settings separated

============================================================================*/

/*############################################################################*/
/*############################################################################*/
/*########################## CLAM SHELL 2 SETTINGS ###########################*/
/*############################################################################*/
/*############################################################################*/

/*******************************************************************************
 * GPIO configuration for RP3 in CLS 1 platform
 *
     <props name="PCIE_CLS_1_BSP" type="DALPROP_ATTR_TYPE_STRUCT_PTR">
      PcieCls1Bsp
    </props>* This array will be access from index 0 upto the end of
 * array to to configure all the GPIOs before performing
 * link training
 ******************************************************************************/
/*
 * ENDP GPIO configuration for RP3 in CLS 2 platform
 */
STATIC CONST gpioPinConfig_t cls_2_gpio_config_rp3[] =
{
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 178, // PCIE_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 185, // supply 3.3v for device
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_HIGH_VALUE
   }
  }
  ,
  {
   .type = PMIC_GPIO,
   {
    .PmicGpioParam.PmicIndex = 2, // PMIC_C GPIO#9 Reset# to M.2
    .PmicGpioParam.GpioNum = EFI_PM_GPIO_9,
    .PmicGpioParam.OutBuffConfig = EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
    .PmicGpioParam.VSrc = EFI_PM_GPIO_VIN1,
    .PmicGpioParam.Source = EFI_PM_GPIO_SOURCE_GND,
    .PmicGpioParam.BufferStrength = EFI_PM_GPIO_OUT_BUFFER_HIGH,
    .PmicGpioParam.inversion = 1
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 75000 // 75ms delay for power to become stable
   }
  }
  ,
  {
   .type = PMIC_GPIO,
   {
    .PmicGpioParam.PmicIndex = 4, // PMIC_E GPIO 6 To turn ON device
    .PmicGpioParam.GpioNum = EFI_PM_GPIO_6,
    .PmicGpioParam.OutBuffConfig = EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
    .PmicGpioParam.VSrc = EFI_PM_GPIO_VIN1,
    .PmicGpioParam.Source = EFI_PM_GPIO_SOURCE_GND,
    .PmicGpioParam.BufferStrength = EFI_PM_GPIO_OUT_BUFFER_HIGH,
    .PmicGpioParam.inversion = 1
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 179, // CLK_REQ
    .TlmmGpioParam.Func = 1,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_PULL_UP,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 180, // PCIE_WAKE
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_INPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_LOW_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 150 // 150us delay for REFCLK to become stable
   }
  }
  ,
  {
   .type = MSM_GPIO,
   {
    .TlmmGpioParam.GpioNum = 178, // PCIE_RST
    .TlmmGpioParam.Func = 0,
    .TlmmGpioParam.Direction = GPIO_OUTPUT,
    .TlmmGpioParam.Pull = GPIO_NO_PULL,
    .TlmmGpioParam.DriveStrength = GPIO_2MA,
    .TlmmGpioParam.value = GPIO_HIGH_VALUE
   }
  }
  ,
  {
   .type = DELAY,
   {
    .DelayValue = 500000 // 500ms delay for EP device preparation for Link Training
   }
  }
  ,
  /* Default and end of sequence entry */
  {
   .type = NONE,
   {
    .DelayValue = NONE
   }
  }
};

/*******************************************************************************
 * array of structures will hold the information
 * of available root ports for CLS platform
 ******************************************************************************/
static portInfo_t cls2_multi_port_info[] =
{
/* Include port_info for RP0 */
#if (RPMASK & PCIE_RP(0))
  {
    .clk                 = pcie_clk_rp0,
    .pwr                 = "pcie_0_gdsc",
    .icb_bw              = pcie_0_icb_bw,
    .clk_set             = pcie_0_clk_settings,
    /* CLS Specific settings */
    .gpio_cfg            = cls_gpio_config_rp0,
    .mem                 = port_0_reg,
    .port_phy_init_seq   = sdm1000_port_0_phy_init_seq,
    .port_phy_deinit_seq = port_0_phy_de_init_seq,
    .link                = port0_link,
    .hw_rp_index         = 0,
    .io_coherency_en     = FALSE,
    .iommu_hdl           = "\\_SB_.PCI0",
    .num_ports           = NUM_PCIE_0_PORTS
  },
#endif
/* Include port_info for RP1 if it exist */
#if (RPMASK & PCIE_RP(1))
  {
    .clk                 = pcie_clk_rp1,
    .pwr                 = "pcie_1_gdsc",
    .icb_bw              = pcie_1_icb_bw,
    .clk_set             = pcie_1_clk_settings,
    .gpio_cfg            = gpio_config_rp1,
    .mem                 = port_1_reg,
    .port_phy_init_seq   = sdm1000_port_1_phy_init_seq,
    .port_phy_deinit_seq = port_1_phy_de_init_seq,
    .link                = port1_link,
    .hw_rp_index         = 1,
    .io_coherency_en     = FALSE,
    .iommu_hdl           = "\\_SB_.PCI2",
    .num_ports           = NUM_PCIE_1_PORTS
  },
#endif
/* Include port_info for RP2 if it exist */
#if (RPMASK & PCIE_RP(2))
  {
    .clk                 = pcie_clk_rp2,
    .pwr                 = "pcie_2_gdsc",
    .icb_bw              = pcie_2_icb_bw,
    .clk_set             = pcie_2_clk_settings,
    .gpio_cfg            = gpio_config_rp2,
    .mem                 = port_2_reg,
    .port_phy_init_seq   = sdm1000_port_2_phy_init_seq,
    .port_phy_deinit_seq = port_2_phy_de_init_seq,
    .link                = port2_link,
    .hw_rp_index         = 2,
    .io_coherency_en     = FALSE,
    .iommu_hdl           = "\\_SB_.PCI1",
    .num_ports           = NUM_PCIE_2_PORTS
  },
#endif
/* Include port_info for RP3 if it exist */
#if (RPMASK & PCIE_RP(3))
  {
    .clk                 = pcie_clk_rp3,
    .pwr                 = "pcie_3_gdsc",
    .icb_bw              = pcie_3_icb_bw,
    .clk_set             = pcie_3_clk_settings,
    /* CLS 1 Specific settings */
    .gpio_cfg            = cls_2_gpio_config_rp3,
    .mem                 = port_3_reg,
    .port_phy_init_seq   = sdm1000_port_3_phy_init_seq,
    .port_phy_deinit_seq = port_3_phy_de_init_seq,
    .link                = port3_link,
    .hw_rp_index         = 3,
    .io_coherency_en     = FALSE,
    .iommu_hdl           = "\\_SB_.PCI3",
    .num_ports           = NUM_PCIE_3_PORTS
  },
#endif
};

/*******************************************************************************
 * This struct will hold the port info for all the available root ports and also
 * number of root ports enabled during compile time for CLS platform
 ******************************************************************************/
STATIC CONST rootPort_t cls2_root_port[] = {
  {
    .port_info = cls2_multi_port_info,
    .num_rp    = ARRAY_LENGTH(cls2_multi_port_info)
  }
};

/*******************************************************************************
 * The single structure will hold all the information needed to bring the
 * root ports out of reset for CLS platform
 ******************************************************************************/
STATIC CONST pcieBsp_t PcieCls2Bsp[] =
{
  {
    /* null-terminated string common PHY clock name
     * Since, there is no common PHY in MSM8998,
     * this is assigned with NULL
     */
    .phy_clk       = pcie_clk_cmn,
    /* common PHY init sequence */
    .phy_init_seq  = sdm1000_common_phy_init_seq,
    /* root port info for all enabled root ports */
    .rp_info       = cls2_root_port
  }
};
