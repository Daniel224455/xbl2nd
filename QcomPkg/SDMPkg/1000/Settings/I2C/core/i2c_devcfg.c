/*=============================================================================   
    @file  i2c_devcfg.c
    @brief I2C Device Config data
   
    Copyright (c) 2017 - 2021 Qualcomm Technologies, Incorporated.
                        All rights reserved.
    Qualcomm Technologies, Confidential and Proprietary.
===============================================================================*/

/*=============================================================================
                              EDIT HISTORY
 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/10/21   PCR     Add SSC QUP instances support
 05/21/19   kkish   Enable I2C15 For Poipu AU Uefi Splash Display
 04/26/18   ag      Enable I2C8 instance for CLS platform
 08/08/17   vg      Added common base addr 
 07/06/17   vg      Copyright updated 
=============================================================================*/

#include <i2c_config.h>
#include "gpi.h"

// NOTE: QUPs are numbered starting from 0 as per the IO sheet
#define ENABLE_QUP_04 // eeprom
#define ENABLE_QUP_07 // keyboard
#define ENABLE_QUP_19 // charger
#define ENABLE_QUP_15

// west  [se0 - se7] [1:8]
#define QUPV3_0_CORE_BASE_ADDRESS        0x00800000
#define QUPV3_0_CORE_COMMON_BASE_ADDRESS 0x008C0000

// east  [se0 - se5] [9:14]
#define QUPV3_1_CORE_BASE_ADDRESS        0x00A00000
#define QUPV3_1_CORE_COMMON_BASE_ADDRESS 0x00AC0000

// east [se0 - se6] [15:21]
#define QUPV3_2_CORE_BASE_ADDRESS        0x00C00000
#define QUPV3_2_CORE_COMMON_BASE_ADDRESS 0x00CC0000

// SSC QUP [se0 - se5] [22:27]
#define SSC_QUPV3_CORE_BASE_ADDRESS        0x02600000
#define SSC_QUPV3_CORE_COMMON_BASE_ADDRESS 0x026C0000

#define TLMM_GPIO_CFG(gpio, func, dir, pull, drive) \
                          (((gpio) & 0x3FF) << 4  | \
                           ((func) & 0xF  ) << 0  | \
                           ((dir)  & 0x1  ) << 14 | \
                           ((pull) & 0x3  ) << 15 | \
                           ((drive)& 0xF  ) << 17)

#define TLMM_GPIO_INPUT     0x0
#define TLMM_GPIO_PULL_UP   0x3
#define TLMM_GPIO_2MA       0x0

#define QUP_0   0
#define QUP_1   1
#define QUP_2   2
#define QUP_SSC 99 // resuing macros from GPI driver


//
// CONFIGURATION START ============================================
//
#define TOP_QUP_00_SDA TLMM_GPIO_CFG(0,   1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_00_SCL TLMM_GPIO_CFG(1,   1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_01_SDA TLMM_GPIO_CFG(114, 1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_01_SCL TLMM_GPIO_CFG(115, 1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_02_SDA TLMM_GPIO_CFG(126, 2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_02_SCL TLMM_GPIO_CFG(127, 2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_03_SDA TLMM_GPIO_CFG(144, 2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_03_SCL TLMM_GPIO_CFG(145, 2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_04_SDA TLMM_GPIO_CFG(51,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_04_SCL TLMM_GPIO_CFG(52,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_05_SDA TLMM_GPIO_CFG(121, 1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_05_SCL TLMM_GPIO_CFG(122, 1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_06_SDA TLMM_GPIO_CFG(6,   1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_06_SCL TLMM_GPIO_CFG(7,   1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_07_SDA TLMM_GPIO_CFG(98,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_07_SCL TLMM_GPIO_CFG(99,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_08_SDA TLMM_GPIO_CFG(88,  2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_08_SCL TLMM_GPIO_CFG(89,  2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_09_SDA TLMM_GPIO_CFG(39,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_09_SCL TLMM_GPIO_CFG(40,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_10_SDA TLMM_GPIO_CFG(9,   2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_10_SCL TLMM_GPIO_CFG(10,  4, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_11_SDA TLMM_GPIO_CFG(94,  2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_11_SCL TLMM_GPIO_CFG(95,  2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_12_SDA TLMM_GPIO_CFG(83,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_12_SCL TLMM_GPIO_CFG(84,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_13_SDA TLMM_GPIO_CFG(43,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_13_SCL TLMM_GPIO_CFG(44,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_14_SDA TLMM_GPIO_CFG(47,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_14_SCL TLMM_GPIO_CFG(48,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_15_SDA TLMM_GPIO_CFG(27,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_15_SCL TLMM_GPIO_CFG(28,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_16_SDA TLMM_GPIO_CFG(85,  2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_16_SCL TLMM_GPIO_CFG(86,  2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_17_SDA TLMM_GPIO_CFG(55,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_17_SCL TLMM_GPIO_CFG(56,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_18_SDA TLMM_GPIO_CFG(23,  2, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_18_SCL TLMM_GPIO_CFG(24,  3, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_19_SDA TLMM_GPIO_CFG(181, 1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_19_SCL TLMM_GPIO_CFG(182, 1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_20_SDA TLMM_GPIO_CFG(0,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_20_SCL TLMM_GPIO_CFG(1,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_21_SDA TLMM_GPIO_CFG(2,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_21_SCL TLMM_GPIO_CFG(3,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_22_SDA TLMM_GPIO_CFG(6,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_22_SCL TLMM_GPIO_CFG(7,  1, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_23_SDA TLMM_GPIO_CFG(8,  3, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_23_SCL TLMM_GPIO_CFG(9,  3, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

//GPIOs are not bolled out
#define TOP_QUP_24_SDA TLMM_GPIO_CFG(0,  0, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_24_SCL TLMM_GPIO_CFG(0,  0, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

#define TOP_QUP_25_SDA TLMM_GPIO_CFG(4, 3, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)
#define TOP_QUP_25_SCL TLMM_GPIO_CFG(5, 3, TLMM_GPIO_INPUT, TLMM_GPIO_PULL_UP, TLMM_GPIO_2MA)

const char *common_clocks_str_0 [] =
{
    "gcc_qupv3_wrap0_core_clk",
    "gcc_qupv3_wrap0_core_2x_clk",
    "gcc_qupv3_wrap_0_m_ahb_clk",
    "gcc_qupv3_wrap_0_s_ahb_clk",
    NULL,
};

const char *common_clocks_str_1 [] =
{
    "gcc_qupv3_wrap1_core_clk",
    "gcc_qupv3_wrap1_core_2x_clk",
    "gcc_qupv3_wrap_1_m_ahb_clk",
    "gcc_qupv3_wrap_1_s_ahb_clk",
    NULL,
};

const char *common_clocks_str_2 [] =
{
    "gcc_qupv3_wrap2_core_clk",
    "gcc_qupv3_wrap2_core_2x_clk",
    "gcc_qupv3_wrap_2_m_ahb_clk",
    "gcc_qupv3_wrap_2_s_ahb_clk",
    NULL,
};


const char *common_clocks_str_ssc [] =
{
    "scc_qupv3_core_clk",
    "scc_qupv3_2xcore_clk",
    "scc_qupv3_m_hclk_clk",
    "scc_qupv3_s_hclk_clk",
    NULL,
};

const char *se_clocks_str_0 [] =
{
    "gcc_qupv3_wrap0_s0_clk",
    "gcc_qupv3_wrap0_s1_clk",
    "gcc_qupv3_wrap0_s2_clk",
    "gcc_qupv3_wrap0_s3_clk",
    "gcc_qupv3_wrap0_s4_clk",
    "gcc_qupv3_wrap0_s5_clk",
    "gcc_qupv3_wrap0_s6_clk",
    "gcc_qupv3_wrap0_s7_clk",
};

const char *se_clocks_str_1 [] =
{
    "gcc_qupv3_wrap1_s0_clk",
    "gcc_qupv3_wrap1_s1_clk",
    "gcc_qupv3_wrap1_s2_clk",
    "gcc_qupv3_wrap1_s3_clk",
    "gcc_qupv3_wrap1_s4_clk",
    "gcc_qupv3_wrap1_s5_clk",
};

const char *se_clocks_str_2 [] =
{
    "gcc_qupv3_wrap2_s0_clk",
    "gcc_qupv3_wrap2_s1_clk",
    "gcc_qupv3_wrap2_s2_clk",
    "gcc_qupv3_wrap2_s3_clk",
    "gcc_qupv3_wrap2_s4_clk",
    "gcc_qupv3_wrap2_s5_clk",
};

const char *se_clocks_str_ssc [] =
{
    "scc_qupv3_se0_clk",
    "scc_qupv3_se1_clk",
    "scc_qupv3_se2_clk",
    "scc_qupv3_se3_clk",
    "scc_qupv3_se4_clk",
    "scc_qupv3_se5_clk",
};

plat_clock_config clk_cfg[] =
{
    // src-freq, speed, div, cycle, high, low
    {     19200,   100,   7,    26,   10,  11 },
    {     19200,   400,   2,    24,    5,  12 },
    {     19200,  1000,   1,    18,    3,   9 },
    {    100000,  8000,   2,     0,    3,   5 },
    {    120000, 12500,   1,     0,    8,   6 },
    {         0,     0,   0,     0,    0,   0 },
};

#ifdef ENABLE_QUP_00
plat_device_config i2c_device_config_01 = 
{
    .core_base_addr              = (uint8 *) QUPV3_0_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_0_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00080000,
    .qupv3_instance              = QUP_0,
    .core_index                  = 1,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 0,

    .scl_encoding                = TOP_QUP_00_SCL,
    .sda_encoding                = TOP_QUP_00_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_0,
    .se_clock                    = (uint8 **) (se_clocks_str_0 + 0),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_01
plat_device_config i2c_device_config_02 = 
{
    .core_base_addr              = (uint8 *) QUPV3_0_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_0_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00084000,
    .qupv3_instance              = QUP_0,
    .core_index                  = 2,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 1,

    .scl_encoding                = TOP_QUP_01_SCL,
    .sda_encoding                = TOP_QUP_01_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_0,
    .se_clock                    = (uint8 **) (se_clocks_str_0 + 1),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_02
plat_device_config i2c_device_config_03 = 
{
    .core_base_addr              = (uint8 *) QUPV3_0_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_0_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00088000,
    .qupv3_instance              = QUP_0,
    .core_index                  = 3,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 2,

    .scl_encoding                = TOP_QUP_02_SCL,
    .sda_encoding                = TOP_QUP_02_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_0,
    .se_clock                    = (uint8 **) (se_clocks_str_0 + 2),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_03
plat_device_config i2c_device_config_04 = 
{
    .core_base_addr              = (uint8 *) QUPV3_0_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_0_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x0008C000,
    .qupv3_instance              = QUP_0,
    .core_index                  = 4,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 3,

    .scl_encoding                = TOP_QUP_03_SCL,
    .sda_encoding                = TOP_QUP_03_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_0,
    .se_clock                    = (uint8 **) (se_clocks_str_0 + 3),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_04
plat_device_config i2c_device_config_05 = 
{
    .core_base_addr              = (uint8 *) QUPV3_0_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_0_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00090000,
    .qupv3_instance              = QUP_0,
    .core_index                  = 5,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 4,

    .scl_encoding                = TOP_QUP_04_SCL,
    .sda_encoding                = TOP_QUP_04_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_0,
    .se_clock                    = (uint8 **) (se_clocks_str_0 + 4),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_05
plat_device_config i2c_device_config_06 = 
{
    .core_base_addr              = (uint8 *) QUPV3_0_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_0_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00094000,
    .qupv3_instance              = QUP_0,
    .core_index                  = 6,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 5,

    .scl_encoding                = TOP_QUP_05_SCL,
    .sda_encoding                = TOP_QUP_05_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_0,
    .se_clock                    = (uint8 **) (se_clocks_str_0 + 5),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_06
plat_device_config i2c_device_config_07 = 
{
    .core_base_addr              = (uint8 *) QUPV3_0_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_0_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00098000,
    .qupv3_instance              = QUP_0,
    .core_index                  = 7,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 6,

    .scl_encoding                = TOP_QUP_06_SCL,
    .sda_encoding                = TOP_QUP_06_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_0,
    .se_clock                    = (uint8 **) (se_clocks_str_0 + 6),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_07
plat_device_config i2c_device_config_08 = 
{
    .core_base_addr              = (uint8 *) QUPV3_0_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_0_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x0009C000,
    .qupv3_instance              = QUP_0,
    .core_index                  = 8,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 7,

    .scl_encoding                = TOP_QUP_07_SCL,
    .sda_encoding                = TOP_QUP_07_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_0,
    .se_clock                    = (uint8 **) (se_clocks_str_0 + 7),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_08
plat_device_config i2c_device_config_09 = 
{
    .core_base_addr              = (uint8 *) QUPV3_1_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_1_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00080000,
    .qupv3_instance              = QUP_1,
    .core_index                  = 9,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 8,

    .scl_encoding                = TOP_QUP_08_SCL,
    .sda_encoding                = TOP_QUP_08_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_1,
    .se_clock                    = (uint8 **) (se_clocks_str_1 + 0),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_09
plat_device_config i2c_device_config_10 = 
{
    .core_base_addr              = (uint8 *) QUPV3_1_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_1_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00084000,
    .qupv3_instance              = QUP_1,
    .core_index                  = 10,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 9,

    .scl_encoding                = TOP_QUP_09_SCL,
    .sda_encoding                = TOP_QUP_09_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_1,
    .se_clock                    = (uint8 **) (se_clocks_str_1 + 1),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_10
plat_device_config i2c_device_config_11 = 
{
    .core_base_addr              = (uint8 *) QUPV3_1_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_1_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00088000,
    .qupv3_instance              = QUP_1,
    .core_index                  = 11,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 10,

    .scl_encoding                = TOP_QUP_10_SCL,
    .sda_encoding                = TOP_QUP_10_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_1,
    .se_clock                    = (uint8 **) (se_clocks_str_1 + 2),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_11
plat_device_config i2c_device_config_12 = 
{
    .core_base_addr              = (uint8 *) QUPV3_1_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_1_CORE_COMMON_BASE_ADDRESS,  
    .core_offset                 = 0x0008C000,
    .qupv3_instance              = QUP_1,
    .core_index                  = 12,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 11,

    .scl_encoding                = TOP_QUP_11_SCL,
    .sda_encoding                = TOP_QUP_11_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_1,
    .se_clock                    = (uint8 **) (se_clocks_str_1 + 3),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_12
plat_device_config i2c_device_config_13 = 
{
    .core_base_addr              = (uint8 *) QUPV3_1_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_1_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00090000,
    .qupv3_instance              = QUP_1,
    .core_index                  = 13,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 12,

    .scl_encoding                = TOP_QUP_12_SCL,
    .sda_encoding                = TOP_QUP_12_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_1,
    .se_clock                    = (uint8 **) (se_clocks_str_1 + 4),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_13
plat_device_config i2c_device_config_14 = 
{
    .core_base_addr              = (uint8 *) QUPV3_2_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_2_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x0008c000,
    .qupv3_instance              = QUP_2,
    .core_index                  = 14,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 13,

    .scl_encoding                = TOP_QUP_13_SCL,
    .sda_encoding                = TOP_QUP_13_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_2,
    .se_clock                    = (uint8 **) (se_clocks_str_2 + 3),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_14
plat_device_config i2c_device_config_15 = 
{
    .core_base_addr              = (uint8 *) QUPV3_2_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_2_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00090000,
    .qupv3_instance              = QUP_2,
    .core_index                  = 15,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 14,

    .scl_encoding                = TOP_QUP_14_SCL,
    .sda_encoding                = TOP_QUP_14_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_2,
    .se_clock                    = (uint8 **) (se_clocks_str_2 + 4),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_15
plat_device_config i2c_device_config_16 = 
{
    .core_base_addr              = (uint8 *) QUPV3_2_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_2_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00094000,
    .qupv3_instance              = QUP_2,
    .core_index                  = 16,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 15,

    .scl_encoding                = TOP_QUP_15_SCL,
    .sda_encoding                = TOP_QUP_15_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_2,
    .se_clock                    = (uint8 **) (se_clocks_str_2 + 5),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_16
plat_device_config i2c_device_config_17 = 
{
    .core_base_addr              = (uint8 *) QUPV3_1_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_1_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00094000,
    .qupv3_instance              = QUP_1,
    .core_index                  = 17,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 16,

    .scl_encoding                = TOP_QUP_16_SCL,
    .sda_encoding                = TOP_QUP_16_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_1,
    .se_clock                    = (uint8 **) (se_clocks_str_1 + 5),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_17
plat_device_config i2c_device_config_18 = 
{
    .core_base_addr              = (uint8 *) QUPV3_2_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_2_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00080000,
    .qupv3_instance              = QUP_2,
    .core_index                  = 18,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 17,

    .scl_encoding                = TOP_QUP_17_SCL,
    .sda_encoding                = TOP_QUP_17_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_2,
    .se_clock                    = (uint8 **) (se_clocks_str_2 + 0),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_18
plat_device_config i2c_device_config_19 = 
{
    .core_base_addr              = (uint8 *) QUPV3_2_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_2_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00084000,
    .qupv3_instance              = QUP_2,
    .core_index                  = 19,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 18,

    .scl_encoding                = TOP_QUP_18_SCL,
    .sda_encoding                = TOP_QUP_18_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_2,
    .se_clock                    = (uint8 **) (se_clocks_str_2 + 1),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_19
plat_device_config i2c_device_config_20 = 
{
    .core_base_addr              = (uint8 *) QUPV3_2_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) QUPV3_2_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00088000,
    .qupv3_instance              = QUP_2,
    .core_index                  = 20,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 19,

    .scl_encoding                = TOP_QUP_19_SCL,
    .sda_encoding                = TOP_QUP_19_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_2,
    .se_clock                    = (uint8 **) (se_clocks_str_2 + 2),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_20
plat_device_config i2c_device_config_21 = 
{
    .core_base_addr              = (uint8 *) SSC_QUPV3_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) SSC_QUPV3_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00080000,
    .qupv3_instance              = QUP_SSC,
    .core_index                  = 21,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 20,

    .scl_encoding                = TOP_QUP_20_SCL,
    .sda_encoding                = TOP_QUP_20_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_ssc,
    .se_clock                    = (uint8 **) (se_clocks_str_ssc + 0),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_21
plat_device_config i2c_device_config_22 = 
{
    .core_base_addr              = (uint8 *) SSC_QUPV3_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) SSC_QUPV3_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00084000,
    .qupv3_instance              = QUP_SSC,
    .core_index                  = 22,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 21,

    .scl_encoding                = TOP_QUP_21_SCL,
    .sda_encoding                = TOP_QUP_21_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_ssc,
    .se_clock                    = (uint8 **) (se_clocks_str_ssc + 1),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_22
plat_device_config i2c_device_config_23 = 
{
    .core_base_addr              = (uint8 *) SSC_QUPV3_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) SSC_QUPV3_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00088000,
    .qupv3_instance              = QUP_SSC,
    .core_index                  = 23,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 22,

    .scl_encoding                = TOP_QUP_22_SCL,
    .sda_encoding                = TOP_QUP_22_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_ssc,
    .se_clock                    = (uint8 **) (se_clocks_str_ssc + 2),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_23
plat_device_config i2c_device_config_24 = 
{
    .core_base_addr              = (uint8 *) SSC_QUPV3_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) SSC_QUPV3_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x0008c000,
    .qupv3_instance              = QUP_SSC,
    .core_index                  = 24,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 23,

    .scl_encoding                = TOP_QUP_23_SCL,
    .sda_encoding                = TOP_QUP_23_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_ssc,
    .se_clock                    = (uint8 **) (se_clocks_str_ssc + 3),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_24
plat_device_config i2c_device_config_25 = 
{
    .core_base_addr              = (uint8 *) SSC_QUPV3_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) SSC_QUPV3_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00090000,
    .qupv3_instance              = QUP_SSC,
    .core_index                  = 25,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 24,

    .scl_encoding                = TOP_QUP_24_SCL,
    .sda_encoding                = TOP_QUP_24_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_ssc,
    .se_clock                    = (uint8 **) (se_clocks_str_ssc + 4),

    .clock_config                = clk_cfg,
};
#endif

#ifdef ENABLE_QUP_25
plat_device_config i2c_device_config_26 = 
{
    .core_base_addr              = (uint8 *) SSC_QUPV3_CORE_BASE_ADDRESS,
    .common_base_addr            = (uint8 *) SSC_QUPV3_CORE_COMMON_BASE_ADDRESS,
    .core_offset                 = 0x00094000,
    .qupv3_instance              = QUP_SSC,
    .core_index                  = 26,
    .core_irq                    = 0,
    .polled_mode                 = TRUE,
    .min_data_length_for_dma     = 0,

    .gpi_index                   = 25,

    .scl_encoding                = TOP_QUP_25_SCL,
    .sda_encoding                = TOP_QUP_25_SDA,

    .tcsr_base_addr              = (uint8 *) 0x01FC0000,
    .tcsr_reg_offset             = 0x00000000,
    .tcsr_reg_value              = 0x00000000,

    .common_clocks               = (uint8 **) common_clocks_str_ssc,
    .se_clock                    = (uint8 **) (se_clocks_str_ssc + 5),

    .clock_config                = clk_cfg,
};
#endif
