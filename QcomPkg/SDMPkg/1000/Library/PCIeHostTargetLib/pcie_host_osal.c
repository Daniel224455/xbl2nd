/**
  @file pcie_osal.c
  @brief Internal interface include file for the PCIe OS abstraction layer.

  This file contains definitions of constants, data structures, and
  interfaces that provide an abstraction layer to OS primitives.

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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "pcie_host_osal.h"
#include "pcie_host_tgt_cfg.h"
#include "pcie_host.h"
#include "DALDeviceId.h"
#include "DDIChipInfo.h"
#include "DDITlmm.h"
#include <api/boot/boot_logger.h>
#include <api/pmic/pm/pm_lib.h>
#include <api/systemdrivers/ClockBoot.h>
#include <api/systemdrivers/busywait.h>
#include <XBLLoader/boot_cache_mmu.h>

/** Default log level */
#define PCIE_HOST_OSAL_MSG_LEVEL PCIE_HOST_LOG_DEBUG

/** Max size of the log message */
#define PCIE_HOST_OSAL_LOG_BUFSIZE 100

/** Temporary buffer for the log message */
static char pcierc_log_buffer[PCIE_HOST_OSAL_LOG_BUFSIZE];

/* ============================================================================
**  Function : pcie_host_osal_log_msg
** ============================================================================
*/
/**
    @brief
    Debug message logging interface.

    @details
    Debug message logging interface. Depending upon the OSAL implementation, it
    could be printed to a term window or sent as a diag msg or saved to a file

    @param[in]  lvl    Level/severity of the log
    @param[in]  fmt    Format similar to printf
    @param[in]  ...    Variable list of arguments to be processed

    @return
    None
*/
void pcie_host_osal_log_msg(pcie_host_loglevel_t lvl, const char *fmt, ...)
{
    va_list arg_list;

    if(PCIE_HOST_OSAL_MSG_LEVEL > lvl)
        return;

   va_start( arg_list, fmt );

    memset(pcierc_log_buffer,0,PCIE_HOST_OSAL_LOG_BUFSIZE);
    va_start(arg_list, fmt);
    vsnprintf(pcierc_log_buffer, PCIE_HOST_OSAL_LOG_BUFSIZE, fmt, arg_list);
    va_end(arg_list);
    boot_log_message(pcierc_log_buffer);
}


/* ============================================================================
**  Function : pcie_osal_init_ldos
** ============================================================================
*/
/**
    @brief
    Initializes the regulators needed by the rootport

    @details
    Initializes the regulators needed by the rootport

    @param[in]  *pmic_config    Target specific pmic config

    @return
    Status of the operation
*/
uint32 pcie_osal_init_ldos(const pcie_host_pmic_config_t *pmic_config)
{
    int i;
    pcie_host_osal_log(PCIE_HOST_LOG_INFO, "++");

    for (i=0;i<pmic_config->num_ldos;i++)
    {
        if (PM_ERR_FLAG_SUCCESS != pm_ldo_sw_enable(pmic_config->ldo_cfg[i].pmic_chip, pmic_config->ldo_cfg[i].ldo_num, PM_ON, TRUE))
        {
            pcie_host_osal_log(PCIE_HOST_LOG_ERROR, "PMIC %d LDO %d is NOT enabled",pmic_config->ldo_cfg[i].pmic_chip, pmic_config->ldo_cfg[i].ldo_num);
        }
        else
        {
            pcie_host_osal_log(PCIE_HOST_LOG_INFO, "PMIC %d LDO %d is enabled",pmic_config->ldo_cfg[i].pmic_chip, pmic_config->ldo_cfg[i].ldo_num);
        }
    }
    pcie_host_osal_log(PCIE_HOST_LOG_INFO,"--");
    return 0;
}

/* ============================================================================
**  Function : pcie_osal_init_clocks
** ============================================================================
*/
/**
    @brief
    Initializes the clocks needed by the rootport

    @details
    Initializes the clocks needed by the rootport

    @param[in]  *clock_config    Target specific clock config

    @return
    Return_description

*/
uint32 pcie_osal_init_clocks(const pcie_host_clk_config_t *clock_config)
{
    switch (*clock_config)
    {
    case 1: //Root Port 1 for 2 lane NVMe support
        Clock_PCIeInit(CLK_PCIE1, CLOCK_BOOT_PERF_NOMINAL);
        break;
    case 2: //Root Port 2 for 4 lane NVMe support
        Clock_PCIeInit(CLK_PCIE2, CLOCK_BOOT_PERF_NOMINAL);
        break;
    // Un-supported Root Ports
    case 0:
    case 3:
    default:
        break;
    }
    return 0;
}

/* ============================================================================
**  Function : pcie_osal_get_chipversion
** ============================================================================
*/
/**
    @brief
    Returns the chip version

    @details
    Returns the chip version

    @return
    Version of the chip
*/
uint32 pcie_osal_get_chipversion()
{
    uint32 chip_version = 0xFFFFFFFF;
    DalDeviceHandle *phChipInfo = NULL;

    if (DAL_SUCCESS == DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &phChipInfo))
    {
        DalChipInfo_GetChipVersion(phChipInfo, &chip_version);
    }

    return chip_version;
}

/* ============================================================================
**  Function : pcie_osal_init_gpios
** ============================================================================
*/
/**
    @brief
    Initializes the GPIOs required by the rootport

    @details
    Initializes the GPIOs required by the rootport

    @param[in]  *gpio_cfg    Target specific GPIO config

    @return
    Status of the operation
*/
pcie_host_status_t pcie_osal_init_gpios(const pcie_host_gpio_config_t *gpio_cfg)
{
    const pcie_host_gpio_config_t *gpio;
    DALGpioSignalType   dal_gpio_config     = 0;
    const pcie_host_tlmm_gpio_config_t *tlmm_cfg;
    DALResult eStatus;
    DalDeviceHandle           *TLMMHandle = NULL;

    eStatus = DAL_DeviceAttach(DALDEVICEID_TLMM, &TLMMHandle);
    if(eStatus != DAL_SUCCESS)
    {
        return(PCIE_HOST_ERROR);
    }

    if (gpio_cfg == NULL)
    {
        return PCIE_HOST_ERROR;
    }
    gpio=gpio_cfg;
    while (gpio->type != NONE)
    {
        switch (gpio->type)
        {
            case MSM_GPIO:
            {
                tlmm_cfg = &gpio->pin_cfg.tlmm_gpio_config;
                dal_gpio_config = (DALGpioSignalType)DAL_GPIO_CFG(tlmm_cfg->gpio_num, tlmm_cfg->func, tlmm_cfg->dir, tlmm_cfg->pull, tlmm_cfg->drv_strength);

                eStatus = DalTlmm_ConfigGpio(TLMMHandle, dal_gpio_config, DAL_TLMM_GPIO_ENABLE);

                if (eStatus != DAL_SUCCESS)
                {
                    pcie_host_osal_log(PCIE_HOST_LOG_ERROR,"Gpio Config failed for GPIO %d", tlmm_cfg->gpio_num);
                    return PCIE_HOST_ERROR;
                }
                if (tlmm_cfg->value != 0)
                {
                    eStatus = DalTlmm_GpioOut(TLMMHandle,dal_gpio_config, tlmm_cfg->value);
                }
                break;
            }
            case DELAY:
            {
                busywait(gpio->pin_cfg.delay_us);
                break;
            }
           default:
               break;
        }
        gpio++;
    }
    return PCIE_HOST_SUCCESS;
}

/* ============================================================================
**  Function : pcie_host_osal_delay
** ============================================================================
*/
/**
    @brief
    provides a busy wait delay

    @details
    Provides a busy wait delay

    @param[in]  usecs    Delay in usecs

    @return
    None
*/
void pcie_host_osal_delay(uint32 usecs)
{
    busywait(usecs);
}

/* ============================================================================
**  Function : pcie_host_osal_barrier
** ============================================================================
*/
/**
    @brief
    Provides a data sync barrier

    @details
    Provides a data sync barrier

    @return
    None

*/
void pcie_host_osal_barrier()
{
    data_sync_barrier();
}
