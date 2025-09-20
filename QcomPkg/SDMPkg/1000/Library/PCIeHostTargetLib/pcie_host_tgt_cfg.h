/**
  @file pcie_tgt_cfg.h
  @brief Internal interface include file for the PCIe target specific 
  configuration layer.

  This file contains definitions of constants, data structures, and
  interfaces that provide target specific configuration data. 

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

#ifndef __PCIE_HOST_TGT_CFG_H__
#define __PCIE_HOST_TGT_CFG_H__
#include "com_dtypes.h"
#include "ChipInfoDefs.h"
#include "pcie_host.h"

/*
 * Register setting special meaning macros
 */
#define PCIE_INVALID_REG_BASE                            (0xFFFFFFFFUL)
#define PCIE_INVALID_REG_SIZE                            (0x0UL)
#define PHY_REG_DELAY                                    (0x44454C41UL)
#define PHY_REG_NOP                                      (0x4E4F4F50UL)
#define PHY_REG_WRI                                      (0x57524954UL)
#define PHY_REG_REA                                      (0x52454144UL)
#define PHY_REG_RMW                                      (0x524D4F44UL)
#define PHY_REG_DSB                                      (0x44495342UL)
#define PHY_REG_POLL                                     (0x504F4C4CUL) 

/***************************************************************************//**
 * @enum pcie_soc_version_number
 * @brief SoC version numbering ENUM
 *
 * Enum to describe major version number of the SoC
 *
 ******************************************************************************/
typedef enum
{
  PCIE_SOC_VER_ALL   = 0x00000000, /**< Applicable for all versions */
  PCIE_SOC_VER_1     = 0x00010000, /**< Qualcomm SoC version 1 */
  PCIE_SOC_VER_2     = 0x00020000, /**< Qualcomm SoC version 2 */
  PCIE_SOC_VER_3     = 0x00030000, /**< Qualcomm SoC version 3 */
  PCIE_SOC_VER_4     = 0x00040000, /**< Qualcomm SoC version 4 */
  PCIE_SOC_VER_5     = 0x00050000, /**< Qualcomm SoC version 5 */
  PCIE_SOC_VER_6     = 0x00060000, /**< Qualcomm SoC version 6 */
  PCIE_SOC_VER_7     = 0x00070000, /**< Qualcomm SoC version 7 */
  PCIE_SOC_VER_8     = 0x00080000, /**< Qualcomm SoC version 8 */
  PCIE_SOC_VER_9     = 0x00090000, /**< Qualcomm SoC version 9 */
  PCIE_SOC_VER_STOP  = 0x53544F50, /**< STOP sequence */
}pcie_soc_version_number;


typedef struct
{
    uint8 pmic_chip;
    uint8 ldo_num;
}pcie_host_ldo_config_t;

typedef struct{
    uint8 num_ldos;
    pcie_host_ldo_config_t *ldo_cfg;
}pcie_host_pmic_config_t;

typedef uint32 pcie_host_clk_config_t; //Dummy clock struct until an HLOS comes along
typedef uint32 pcie_host_rst_config_t; //Dummy clock struct until an HLOS comes along

/***************************************************************************//**
 * @enum pin_config_type
 * @brief GPIO pin config type
 *
 * Enum to choose between different types of GPIOs to be configured
 * type of this enum will be used to determine:
 *
 * @note
 * - Whether to call TLMM/PMIC lib functions to configure the GPIO
 * - Determine end of GPIO init sequence using NONE value
 * - Introduce delay in micro seconds in b/w the sequence
 ******************************************************************************/
typedef enum
{
  NONE = 0,    /**< invalid GPIO type */
  MSM_GPIO=1,  /**< TLMM GPIO type */
  PMIC_GPIO=2, /**< PMIC Gpio Type */
  PMIC_MPP=3,  /**< PMIC MPP type */
  DELAY=4      /**< Delay in micro seconds b/w GPIO init sequence */
}pcie_host_pin_type;


/***************************************************************************//**
 * @struct tlmmGpioParam_t
 * @brief TLMM GPIO settings
 *
 * This structure is created to capture all the configuration
 * parameter pertainig to configure a TLMM GPIO
 ******************************************************************************/
typedef struct
{
  uint32 gpio_num;       /**< GPIO number */
  uint32 func;          /**< GPIO sub function to be selected from */
  uint32 dir;     /**< Input/Output direction to be configured (TLMM_DirectionType) */
  uint32 pull;          /**< Pull down/up settings (TLMM_PullType) */
  uint32 drv_strength; /**< drive strength TLMM_DriveStrengthType*/
  uint32 value; /**< HIGH/LOW state if configured as Output (TLMM_ValueType)*/
}pcie_host_tlmm_gpio_config_t;

/***************************************************************************//**
 * @struct gpioPinConfig_t
 * @brief GPIO config information
 *
 * An array of this type will be created for each root port
 * to hold the sequence of TLMM/PMIC/Delay to be configured
 * before performing LTSSM
 ******************************************************************************/
typedef struct
{
  pcie_host_pin_type type;             /**< this enum will hold info how to access
                                     *   the union type. This type can be NONE
                                     *   to denote end of sequence
                                     */
  union
  {
      pcie_host_tlmm_gpio_config_t tlmm_gpio_config; /**< TLMM GPIO description */
      uint32 delay_us;              /**< Delay to be observed in us
                                  *   between the sequence
                                           */
  }pin_cfg;
}pcie_host_gpio_config_t;


/***************************************************************************//**
 * @enum mem_reg_index
 * @brief Memory region indexer
 *
 * Enum to index array of memRegions_t type, to get access to
 * desired memory regions
 *
 * max_mem_reg should be used declare array to hold all the mem regions
 * Individial elements of the array can be accessed using
 * one of the valid reg index defined in this enum
 ******************************************************************************/
typedef enum
{
  axi_reg            = 0x00, /**< AXI register space */
  ahb_reg            = 0x01, /**< AHB register space */
  tcsr_reg           = 0x02, /**< TCSR register space */
  refgen_reg         = 0x03, /**< REFGEN center AHB2PHY register space */
  invalid_reg        = 0x04, /**< Invalid reg base (EOS) */
  max_mem_reg        = 0x05  /**< End of total memRegions_t */
}pcie_host_memreg_index;



/***************************************************************************//**
 * @struct pciePhyRegSeq_t
 * @brief PHY initialization register sequence
 *
 * PHY Initialization value to be written to get the PHY
 * out of rest
 *
 * An array of this type will be defined and filled in with
 * PHY init register write sequence for common PHY & port PHY
 * (if available).
 *
 * REG_WRITE_CONDITIONAL macro defined within the project will be used
 * to walk through this base/offset/value sets and perform:
 *
 * @note
 * - Get the pa corresponding to the mem_reg_index value found in base
 * - add reg_offset to the pa obtained from step one
 * - write the reg_val (usually 1 byte) to the calculated register
 *
 * @note Example:
 *  { phy_tx_reg, HWIO_OFFS(PCIE_COM_POWER_DOWN_CONTROL), 0x01 }
 *
 * @par
 *  parsing above example element of this type array will get the
 *  base pointed by phy_tx_reg and offset it to
 *  PCIE_COM_POWER_DOWN_CONTROL regsier then write the value 0x01
 ******************************************************************************/
typedef struct
{
  pcie_host_memreg_index base;  /**< mem_reg_index to select base address from
                        *   memRegions_t
                        */
  uint32 opflag;   /**< flag used to identify the operation */
  uint32 offset;   /**< register offset to be added to the base address */
  uint32 mask;     /**< register mask to be cleared before writing value */
  uint32 val;      /**< register init value to be written after POR */
} pcie_host_phy_seq_t;

/***************************************************************************//**
 * @struct pciePhyAllVerSeq_t
 * @brief Version dependent PHY init sequences
 *
 * This structure is created to hold all possible PHY init sequences for
 * different versions of SoC
 ******************************************************************************/
typedef struct
{
  const ChipInfoVersionType version; /**< Version for following PHY init seq */
  const pcie_host_phy_seq_t * phy_seq;      /**< PHY init sequence value */
} pcie_host_phy_cfg_t;


/** RC configuration structure */
typedef struct{ 
    const pcie_host_pmic_config_t *pmic_config; /**< Analog rails to be powered */
    const pcie_host_clk_config_t *clk_config; /**< Clock Configuration */
    const pcie_host_rst_config_t *rst_config; /**< Reset Configuration */
    const pcie_host_gpio_config_t *gpio_config; /**< GPIO Configuration */
    const pcie_host_phy_cfg_t    *phy_config; /**< PHY Configuration */
    const pcie_host_mem_region_t *mem;        /**< Memory Configuration */
}pcie_host_rc_config_t; 

/** Target specific configuration*/
typedef struct {
    uint32 num_rcs; /**< Number of RCs to support */
    pcie_host_rc_config_t **rc_cfg; /** Array of RC configurations */
    uint32 link_training_timeout_us; /** Link Training timeout */
}pcie_host_tgt_config_t;

extern uint32 pcie_host_tgt_num_ports;
extern pcie_host_tgt_config_t *pcie_host_tgt_config;

/* ============================================================================
**  Function : pcie_host_tgt_cfg_init
** ============================================================================
*/
/**
    @brief
    Initializes target specific config data
    
    @details
    Initializes target specific configuration data to be used by the PCIe host
    driver
    
    @return
    None
    
*/
void pcie_host_tgt_cfg_init();

#endif //__PCIE_HOST_TGT_CFG_H__
