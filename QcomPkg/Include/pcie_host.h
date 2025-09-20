#ifndef __PCIE_HOST_H__
#define __PCIE_HOST_H__

/**
  @file pcie_host.h
  @brief Public interface include file for the PCIe host driver.

  This file contains definitions of constants, data structures, and
  interfaces that provide operational control over a PCIe device.

*/
/*
===============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------
05/08/18   MK      Created

===============================================================================
                   Copyright (c) 2018 QUALCOMM Technologies Incorporated
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/

//----------------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------------
#include "com_dtypes.h"

/** @addtogroup pcie_host_public_constants
@{ */
#define PCIE_HOST_ID_ANY 0xFF /**< Used as a wild card to capture all IDs in a category */

/** PCI Config Header */
#define PCIE_CFG_DEVVENDID                                 (0x0000)           /**< Device Vendor ID offset */
#define PCIE_CFG_CMDSTTS                                   (0x0004)           /**< Command Status offset */
#define PCIE_CFG_REVCLASS                                  (0x0008)           /**< Revision Class Code offset */
#define PCIE_CFG_HDRTYPE                                   (0x000C)           /**< Header type offset */
#define PCIE_CFG_BAR(bar)                                 (0x0010+4*(bar))   /**< BAR offset */
#define PCIE_CFG_TYPE1_BUS                                 (0x0018)           /**< Bus config offset */
#define PCIE_CFG_TYPE1_MEMBASELIMIT                        (0x0020)           /**< Memory Base and Limit offset */
#define PCIE_CFG_TYPE1_PREFETCH_MEMBASELIMIT               (0x0024)           /**< Prefetchable Base and Limit offset */
#define PCIE_CFG_TYPE1_PREFETCH_UPPER_MEMBASE              (0x0028)           /**< Prefetchable Memory Base Upper offset */
#define PCIE_CFG_TYPE1_PREFETCH_UPPER_MEMLIMIT             (0x002C)           /**< Prefetchable Memory Limit Upper offset */
#define PCIE_CFG_TYPE1_IOBASELIMIT                         (0x0030)           /**< IO Base and Limit offset */

#define MAX_DEVICES_PER_BUS 32 /**< Maximum number of devices on a single bus */
#define MAX_FUNCS_PER_DEVICE 8 /**< Maximum number of functions in a device */
#define PCIE_HOST_MAX_BARS 6 /**< Maximum number of BARs PCIe config space has */

#define PCIE_HOST_MAX_DRVNAME_LENGTH 12 /**< Maximum length of a driver name */

/** @}  - pcie_host_constants */

/** @addtogroup pcie_host_public_enums
@{ */


/** Status codes returned by PCIe Host APIs */
typedef enum 
{
    PCIE_HOST_SUCCESS=0, /**< Operation was a success */
    PCIE_HOST_ERROR = 1  /**< Operation met with an error */
}pcie_host_status_t;

/** Different types of TLPs*/
typedef enum {
  PCIE_HOST_TLP_MEMWR=0x00,      /**< Memory Write TLP */
  PCIE_HOST_TLP_MEMRD=0x01,      /**< Memory Read TLP */
  PCIE_HOST_TLP_IORDWR=0x02,     /**< IO Read Write TLP */
  PCIE_HOST_TLP_CFG0RDWR=0x04,   /**< Config0 Read Write TLP */
  PCIE_HOST_TLP_CFG1RDWR=0x05,   /**< Config1 Read Write TLP */
} pcie_host_tlptype_t;

/** Config access mode */
typedef enum { 
	PCIE_HOST_OPER_READ  = 0x0, /**< Config Read operation */ 
	PCIE_HOST_OPER_WRITE = 0x1  /**< Config Write operation */
}pcie_host_rw_mode_t; 

/** Options to enable the device with */
typedef enum { 
	PCIE_HOST_ENABLE_BME = 0x1, /**< Enable the Device to do DMA */
	PCIE_HOST_ENABLE_MSE = 0x2, /**< Enable access to the Device’s MMIO space */
	PCIE_HOST_ENABLE_IO  = 0x4, /**< Enable access to the Device’s IO space */
}pcie_enable_opts_t; 
/** @}  - pcie_host_enum_api */

/** @addtogroup pcie_host_public_structs
@{ */
/** Device ID structure the device can be identified with */
typedef struct {
    uint32 class_id;  /**< Class code (BCC+SCC+PI). Use PCIE_HOST_ANY_ID if not req to match */ 
    uint32 vendor_id; /**< Vendor ID. Use PCIE_HOST_ANY_ID if not required to match */
    uint32 device_id; /**< Device ID. Use PCIE_HOST_ANY_ID if not required to match */
}pcie_host_device_id_t;

/** Memory region descriptor*/
typedef struct { 
     uint64 pa;    /**< physical address of memory region */ 
     uint64 sz;    /**< size of memory region */ 
     uint64 limit; /**< Limit of the region (pa + sz -1) */ 
}pcie_host_mem_region_t; 

/** Opaque handle to the PCIe device */
typedef void * pcie_host_dev_t;

/** Resource descriptor capturing resources allocated
 *  to a given device 
 */
typedef struct{ 
        pcie_host_mem_region_t bars[PCIE_HOST_MAX_BARS]; /**< BARs assigned to this device’s MMIO Space */
       //Interrupts and other resources are not supported for now 
}pcie_host_res_t;

/** Probe payload descriptor. This will be passed back
 *  when the device registered for, is getting probed */
typedef struct
{
    void *user_data;                /**< User provided data to be supplied with the callback */
    pcie_host_res_t *res;           /**< Pointer to the resource structure to be filled by the host driver  */
}pcie_host_probe_cb_payload_t;

/**
 * User callback function to be called for probing the PCIe device
 */
typedef void (*pcie_host_probefn_t)(pcie_host_dev_t *pdev, pcie_host_probe_cb_payload_t *cb_data );

/**
 * User callback type with payload info to be provided to the
 * user callback
 */
typedef struct
{
    char drvname[PCIE_HOST_MAX_DRVNAME_LENGTH];                  /**< Name of the driver that claims the device */
    pcie_host_probefn_t probe_fn;    /**< Function to be called when the device is discovered */
    pcie_host_probe_cb_payload_t *cb_data; /**< Callback data to be passed with the probe_fn */
}pcie_host_probe_info_t;
/** @}  - pcie_host_struct */

/** @addtogroup pcie_host_public_funcs
@{ */

/* ============================================================================
**  Function : pcie_host_drv_init
** ============================================================================
*/
/**
    @brief
    Initializes the PCIe host driver
    
    @details
    Initializes the PCIe host driver and enumerates the devices seen on the
    PCIe bus.
    
    @return
    None. If any error occurs, this function may not return (or enter error handler
    path as appropriate)
    
    @sa
    None
*/
void pcie_host_drv_init( );


/* ============================================================================
**  Function : pcie_host_register_device
** ============================================================================
*/
/**
    @brief
    Registers the client as  the owner of a PCIe device. 
    
    @details
    This function will perform the following:
    1.	Register the client as the owner of the device that has the attributes as defined in the devid.
    2.	If the device is already present in the bus, invoke the corresponding probe

   
    @param[in]  device_id    Device ID of the device to be registered with.
    @param[in]  probe_info     Probe call back to be called when the device is found

    @dependencies
    None
    
    @sideeffects
    If the PCIe host driver stack was not initialized, it will get initialized,
    and controllers will be brought up.
    It is possible that the probe function can get called in the context of this
    function, and so the client drivers must ensure that register_device call
    doesn’t conflict with their probe function. Also, probe function will be
    called independently for each device found on the bus that match the devid
    attribute

    @return
    Status of the operation    
    
*/
pcie_host_status_t pcie_host_register_device(pcie_host_device_id_t device_id, pcie_host_probe_info_t probe_info);

/* ============================================================================
**  Function : pcie_host_enable_device
** ============================================================================
*/
/**
    @brief
    Enables the PCIe device for operation
 
    @details
    Enables the PCIe device for
     a) BME - Bus Master Enable -- Allows the device to DMA to/from the host memory
     b) MSE - Memory Space Enable -- Allows the host to access device's MMIO space
     c) IE  - IO Space Enable -- Allows  the host to access device's IO space
    when (a/b/c) are provided via enable_options.
    In general, the clients are encouraged to set all these bits as part of the
    enable command.
 
    @param[in]  *pdev             Handle to the pcie device
    @param[in]  enable_options    Options to enable different operational modes 

    @dependencies
    Device must have been probed, and pdev should be valid
    
    @sideeffects
    None
    
    @return
    None
    
*/
void pcie_host_enable_device( pcie_host_dev_t *pdev, pcie_enable_opts_t enable_options);

/* ============================================================================
**  Function : pcie_host_disable_device
** ============================================================================
*/
/**
    @brief
    Disables the device from all IO operations
    
    @details
    Disables the device from bus mastering as well as from MMIO accesses
 
    @param[in]  *pdev    Handle to the pcie device

    @dependencies
    Device must have been probed, and pdev should be valid
    
    @sideeffects
    None
    
    @return
    None
  
*/
void pcie_host_disable_device(pcie_host_dev_t *pdev);

/* ============================================================================
**  Function : pcie_host_config_rw
** ============================================================================
*/
/**
    @brief
    Performs an access to the config space of the PCIe device
    
    @details
    Performs an access to the config space of the PCIe device
    
    @param[in]      *pdev     Handle to the device
    @param[in]      mode      Config access mode - Read or Write operation
    @param[in]      offset    Offset from the config base to perform the access
    @param[in,out]  *val      Value to be written with or read from      

    @return
    For read accesses, the *val will have the content read from the offset
*/
void pcie_host_config_rw ( pcie_host_dev_t *pdev, 	pcie_host_rw_mode_t mode, uint32 offset, uint32 *val);
/** @}  - pcie_host_funcs */


#endif //__PCIE_HOST_H__
