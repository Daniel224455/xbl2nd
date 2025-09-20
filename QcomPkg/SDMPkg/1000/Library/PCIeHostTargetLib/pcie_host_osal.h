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

#ifndef __PCIE_HOST_OSAL_H__
#define __PCIE_HOST_OSAL_H__

#include "pcie_host.h"
#include "pcie_host_tgt_cfg.h"

/** Log Message Levels */
typedef enum 
{
  PCIE_HOST_LOG_DEBUG, /**< Debug Level - Very chatty */
  PCIE_HOST_LOG_INFO,  /**< Info Level - Useful for tracing */
  PCIE_HOST_LOG_WARN, /**< Warning Level - Potential unexpected outcomes */
  PCIE_HOST_LOG_ERROR, /**< Error Level - Errors are being reported; most likely to fail elsewhere */
  PCIE_HOST_LOG_CRITICAL, /**< Critical Level - Unrecoverable error, should not proceed further */
}pcie_host_loglevel_t;

/** Default log level */
#define PCIE_HOST_OSAL_MSG_LEVEL PCIE_HOST_LOG_DEBUG
#define PCIE_HOST_OSAL_MSG_ENABLE

#ifdef PCIE_HOST_OSAL_MSG_ENABLE
#define pcie_host_osal_log(lvl, f, ...)  pcie_host_osal_log_msg(lvl, "[%s:%d]"f, __func__,__LINE__,##__VA_ARGS__)
#else
#define pcie_host_osal_log(lvl, f, ...)  
#endif

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
void pcie_host_osal_log_msg(pcie_host_loglevel_t lvl, const char *fmt, ...);

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
uint32 pcie_osal_init_ldos(const pcie_host_pmic_config_t *pmic_config);

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
uint32 pcie_osal_init_clocks(const pcie_host_clk_config_t *clock_config);

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
uint32 pcie_osal_get_chipversion();

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
pcie_host_status_t pcie_osal_init_gpios(const pcie_host_gpio_config_t *gpio_cfg);

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
void pcie_host_osal_delay(uint32 usecs);

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
void pcie_host_osal_barrier();

#endif //__PCIE_HOST_OSAL_H__
