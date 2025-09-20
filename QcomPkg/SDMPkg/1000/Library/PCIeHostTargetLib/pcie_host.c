/**
  @file pcie_host.c
  @brief PCIe host driver implementation.

  This file contains the implementation of the PCIe host protocol specification.

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

#include "pcie_host.h"
#include "pcie_host_core.h"
#include "pcie_host_osal.h"

#define PCIE_INVALID_VAL 0xFFFFFFFF
#define IS_VALID_VAL(x) ((x!=PCIE_INVALID_VAL)&&(x!=0))

/** PCIe device descriptor */
typedef struct _pcie_devi_t
{
    pcierc_dev_t *rc_dev; /** Pointer to the root port's platform config */
    uint32 rbdf;/**< Unique identifier with  (rootport:bus:device:function) format */
    uint8 isrc; /**< Is a root complex? */
    uint16 vendor_id; /**< Vendor Id */
    uint16 device_id; /**< Device Id*/
    uint32 class_code; /**< Class code */
    uint32 header; /**< Header type */
    uint8 primary_bus; /**< Primary bus number*/
    uint8 secondary_bus; /**< Secondary bus number */
    uint8 subordinate_bus;/**< Subordinate bus number*/
    uint64 mem_base; /** Base of the memory region owned by this device */
    uint64 mem_limit; /** Limit of the memory region owned by this device */
    pcie_host_mem_region_t bars[PCIE_HOST_MAX_BARS];
    struct _pcie_devi_t *parent; /** Parent node of this device */
    struct _pcie_devi_t *children; /** Downstream nodes of this device */
    struct _pcie_devi_t *peer; /** peer device on the same bus as this device */
}pcie_devi_t;

/** Maximum number of PCIe device supported */
#define MAX_PCIE_DEVICES 2 

/** PCIe devices table to gather devices discovered under the root ports */
pcie_devi_t pcie_devs[MAX_PCIE_DEVICES];

/** Driver descriptor */
typedef struct{
    pcie_host_device_id_t device_id; /**< ID of the device the driver owns*/
    pcie_host_probe_info_t probe_info; /**< Probe to be called when the device is detected*/
}pcie_host_drvinfo_t;

#define MAX_PCIE_DRIVERS 2 /**< Maximum number for drivers the host driver can support*/

/** Driver descriptor table */
pcie_host_drvinfo_t pcie_host_drvinfo_table[MAX_PCIE_DRIVERS];  

/** Number of devices detected during the bus scan */
static uint32 pcie_devices = 0;

/** Number of drivers registered with the PCI host driver */
uint32 pcie_num_drivers = 0;

/** Init status of the PCIe host driver */
uint32 pcie_host_initialized = FALSE;

/* List of all discovered root ports */
pcie_devi_t *pcie_rootports = NULL;

/* ============================================================================
**  Function : pcie_host_cfg_read
** ============================================================================
*/
/**
    @brief
    Reads a value from the config space.
    
    @details
    Reads a value from the config space. It uses ECAM mechanism to detect the
    location of the offset for a given bus:dev:func assignment of the device.
    
    @param[in]  *pcierc_dev    Handle to the root port this device belongs to     
    @param[in]  rbdf           Rootport::Bus::Device::Function assignment         
    @param[in]  offset         Offset of the register in the device's config space

    @return
    Returns the value read from the offset
*/
uint32 pcie_host_cfg_read(pcierc_dev_t *pcierc_dev, uint32 rbdf ,uint32 offset)
{
    /* TODO: Sanity check the BDF and the offset */
    uint32 bdf = PCIE_GET_BDF(rbdf);
    uintnt cfgbase = pcierc_dev->rc_cfg->mem[axi_reg].pa + (bdf<<12);
    return pcierc_core_read_reg(cfgbase,offset);
}

/* ============================================================================
**  Function : pcie_host_cfg_write
** ============================================================================
*/
/**
    @brief
    Writes a value to the config space.
    
    @details
    Writes a value to the config space. It uses ECAM mechanism to detect the
    location of the offset for a given bus:dev:func assignment of the device.
    
    @param[in]  *pcierc_dev    Handle to the root port this device belongs to
    @param[in]  rbdf           Rootport::Bus::Device::Function assignment
    @param[in]  offset         Offset of the register in the device's config space
    @param[in]  val            Value to be written with

    @return
    None
*/
void pcie_host_cfg_write(pcierc_dev_t *pcierc_dev, uint32 rbdf ,uint32 offset, uint32 val)
{
    /* TODO: Sanity check the BDF and the offset */
    uint32 bdf = PCIE_GET_BDF(rbdf);
    uintnt cfgbase = pcierc_dev->rc_cfg->mem[axi_reg].pa + (bdf<<12);
    return pcierc_core_write_reg(cfgbase,offset,val);
}

/* ============================================================================
**  Function : pcie_host_get_deviceinfo
** ============================================================================
*/
/**
    @brief
    Gets basic information about the device.
    
    @details
    Gets some basic information like device id, vendor id, class code etc that
    will be used for bookkeeping the device. 
    
    @param[in]  *pcie_dev    Handle to the PCIe device

    @return
    None
*/
void pcie_host_get_deviceinfo(pcie_devi_t *pcie_dev)
{
    uint32 val;
    /* First vendor id and device id */
    val = pcie_host_cfg_read(pcie_dev->rc_dev,pcie_dev->rbdf, PCIE_CFG_DEVVENDID);
    pcie_dev->vendor_id = val & 0xFFFF;
    pcie_dev->device_id = (val>>16)&0xFFFF;
    /* Class Code */
    val = pcie_host_cfg_read(pcie_dev->rc_dev,pcie_dev->rbdf, PCIE_CFG_REVCLASS);
    pcie_dev->class_code = (val & 0xFFFFFF00)>>8;
    
    /* Header */
    val = pcie_host_cfg_read(pcie_dev->rc_dev, pcie_dev->rbdf, PCIE_CFG_HDRTYPE );
    pcie_dev->header = (val & 0x7F0000)>>16;

    /* Todo: Rest of the capabilities will be parsed in the future*/
}

void pcie_host_scan_bus(pcie_devi_t *parent, pcierc_dev_t *pcierc_dev, uint8 rc, uint8 bus);

/* ============================================================================
**  Function : pcie_host_scan_device
** ============================================================================
*/
/**
    @brief
    Scans for a presence of a device/function. This function although called
    "scan_device" is actually trying to scan for each function in each of the
    devices under the bus. Since there is no distinction between devices and
    functions from a scan point of view, this function is called for every
    function detected in a device.
    
    @details
    Detailed_Description
    
    @param[in]  *parent        Pointer to the parent pcie device                  
    @param[in]  *pcierc_dev    Pointer to the root port this bus is attached to   
    @param[in]  rc             Unique identifier to the root port                 
    @param[in]  bus            Primary bus number this bus would be assigned with 
    @param[in]  dev            Device number this device will be assigned with
    @param[in]  func           Function number this device will be assigned with

    @dependencies
    None
    
    @return
    Return_description
    
    @sa
    Other_Relevant_APIS_or_None
*/
pcie_devi_t * pcie_host_scan_device(pcie_devi_t *parent, pcierc_dev_t *pcierc_dev, uint8 rc, uint8 bus,  uint8 dev, uint8 func)
{
    uint32 vend_dev_id = PCIE_INVALID_VAL;
    uint32 bdf;
    pcie_devi_t *pdev;

    bdf = PCIE_BDF(bus,dev,func);
    
    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"++ (bdf = %d)",bdf);
    vend_dev_id = pcie_host_cfg_read(pcierc_dev, 
                                     PCIE_RBDF(rc, bus, dev, func), PCIE_CFG_DEVVENDID);
    if(IS_VALID_VAL(vend_dev_id)) {
        pdev = &pcie_devs[pcie_devices];
        pdev->rc_dev = pcierc_dev;
        /* First one is always a root port */
        if(bus == 0 && dev == 0 && func == 0) {
            pdev->isrc = TRUE;
            pdev->parent = NULL;
            pdev->subordinate_bus = 0;
            if(pcie_rootports == NULL) {
                pcie_rootports = pdev;
            }
            else
            {
                pcie_rootports->peer = pdev;
            }
        }
        else
        {
            /* It is a child device */
            pdev->isrc = FALSE;
            pdev->parent = parent;
        }
        pdev->rbdf = PCIE_RBDF(rc, bus, dev, func);
        /* Populate with the right information */
        pcie_host_get_deviceinfo(&pcie_devs[pcie_devices]);
        pcie_host_osal_log(PCIE_HOST_LOG_INFO,
                           "Discovered Device %02d [%d:%d:%d:%d] :",
                           pcie_devices, rc, bus, dev, func);
        pcie_host_osal_log(PCIE_HOST_LOG_INFO,
                           "     Vendor id = 0x%X, Device Id = 0x%X", 
                           pdev->vendor_id, pdev->device_id);
        pcie_host_osal_log(PCIE_HOST_LOG_INFO,
                           "     Class = 0x%x , Header = 0x%X",
                           pdev->class_code,pdev->header);
        
        /* If header was a Type 1...initiate another scan below... */
        if(pdev->header == 1) {

            pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"Detected a type 1 device...Scanning further...");
            pcie_devices++;
            pdev->primary_bus=bus;
            pdev->secondary_bus=bus+1;
            pdev->subordinate_bus=0xFF; //The max possible depth a bus can have
            /* Update the bus config of the type 1 device before scanning below.
             * We will correct it to the actual depth once the scan returns
             */
            pcie_host_cfg_write(pdev->rc_dev,pdev->rbdf, 
                                PCIE_CFG_TYPE1_BUS, 
                                PCIE_BUS_CFG(pdev->primary_bus,pdev->secondary_bus,pdev->subordinate_bus));
            pcie_host_scan_bus(pdev, pcierc_dev, rc, pdev->secondary_bus);
            if(NULL != parent)
            {
                /* Update the parent's subordinate with the deepest bus we have discovered so far..*/
                pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,
                                   "Updating parent (0x%X) sub to 0x%X from pdev (0x%X) sub",
                                   parent->vendor_id, 
                                   pdev->subordinate_bus, 
                                   pdev->vendor_id);
                parent->subordinate_bus = pdev->subordinate_bus;
            }
            /* Update primary, secondary, & subordinate buses with the discovered topology */
            pcie_host_osal_log(PCIE_HOST_LOG_DEBUG, 
                               " Updating Pri = %d Sec = %d Sub = %d",
                               pdev->primary_bus, 
                               pdev->secondary_bus, 
                               pdev->subordinate_bus);
            pcie_host_cfg_write(pdev->rc_dev,pdev->rbdf, 
                                PCIE_CFG_TYPE1_BUS, 
                                PCIE_BUS_CFG(pdev->primary_bus,pdev->secondary_bus,pdev->subordinate_bus));
        }
        else
        {
            pcie_devices++;
        }
    }
    else
    {
        pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"-- (bdf = %d is not present)",bdf);
        return NULL;
    }

    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"-- (bdf = %d)",bdf);
    return pdev;
}

/* ============================================================================
**  Function : pcie_host_scan_bus
** ============================================================================
*/
/**
    @brief
    Scans a bus for downstream devices.
    
    @details
    Scans the bus for devices on the bus and downstream below that bus. This
    function recursively scans the bus until the last node is discovered under
    the root bus.
    
    @param[in]  *parent        Pointer to the parent pcie device
    @param[in]  *pcierc_dev    Pointer to the root port this bus is attached to
    @param[in]  rc             Unique identifier to the root port
    @param[in]  bus            Primary bus number this bus would be assigned with

    @dependencies
    None
        
    @return
    None
*/
void pcie_host_scan_bus(pcie_devi_t *parent, pcierc_dev_t *pcierc_dev, uint8 rc, uint8 bus)
{
    uint32 func = 0;
    uint8 dev = 0;
    uint32 vendor_device_id; 
    pcie_devi_t *child;
    pcie_devi_t *children = NULL;
    
    if(NULL != parent)
    {
        parent->subordinate_bus = bus;
    }
    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"++ (rc = %d, bus = %d)",rc,bus);

    if(parent!= NULL)
    {
      children = parent->children;
    }
    for(dev=0;dev<MAX_DEVICES_PER_BUS;dev++) {
       
        /* Check if we have a valid device */
        vendor_device_id = pcie_host_cfg_read(pcierc_dev, 
                                              PCIE_BDF(bus, dev, 0),
                                              PCIE_CFG_DEVVENDID);

        if(!IS_VALID_VAL(vendor_device_id)) {
            /* No device found..bail out*/
            pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,
                               "No device detected at %d:%d:%d:%d, bailing out",
                               rc,bus,dev);
            pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"-- (rc = %d, bus = %d)",
                               rc,bus);
            return;
        }
        for(func = 0 ; func < MAX_FUNCS_PER_DEVICE; func++) {
             
            child = pcie_host_scan_device(parent, pcierc_dev, rc, bus, dev, func);
            /*Scan devices */
            if(NULL == child)
            {
                /* No more devices found in the bus...
                 * For now we will stop at the first failure 
                 * and not scan the entire range...
                 */
                break;
            }
            if(children == NULL) {
                children = child;
                if(NULL != parent)
                {
                    parent->children = child;
                }
            }
            else
            {
                children->peer = child;
                children = child;
            }            
        }
    }    
    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"-- (rc = %d, bus = %d)",rc,bus);
}

/* ============================================================================
**  Function : pcie_host_assign_bars
** ============================================================================
*/
/**
    @brief
    Allocate memory space for BARs reported by the device
    
    @details
    Allocates memory space for BARs reported by the device. The memory is
    aligned to the BAR space boundary as per the PCIe specification. This
    routine also handles 64-bit BARs. Currently both pre-fetchable and
    non-prefetchable BARs are allocated from the non-prefetchable space.
    
    @param[in]  *pdev    Handle to the PCIe device
    @param[in]  *res     Resource map to allocate from

    @dependencies
    Bus scan is complete with  the device alive and accessible
        
    @return
    None
    
*/
void pcie_host_assign_bars(pcie_devi_t *pdev, pcierc_res_t *res)
{
    uint8 bar, num_bars;
    uint32 val,type, memtype;

    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"++{%d:%d;%d} Device (0x%X : 0x%X)", PCIE_SPLIT_BDF(pdev->rbdf),pdev->vendor_id,pdev->device_id);
    num_bars = (pdev->header)?2:6;

    for(bar = 0; bar < num_bars; bar++) 
    {
        /* Get the BAR Mask */
        pcie_host_cfg_write(pdev->rc_dev,pdev->rbdf, PCIE_CFG_BAR(bar), 0xFFFFFFFF);
        val = pcie_host_cfg_read(pdev->rc_dev,pdev->rbdf, PCIE_CFG_BAR(bar));
        type = (val &0x6)>>1;
        memtype = (val & 0x1);

        if(   val == 0x00000000 ||
              val == 0xFFFFFFFF  || 
              type == 0x1 || 
              type == 0x3 ||
              memtype == 0x1 ) 
        {
            /* Invalid bar */
            pdev->bars[bar].pa=0x0;
            pdev->bars[bar].sz = 0x0;
        }
        else
        {
            pdev->bars[bar].sz = (~(val&(~0xF)))+1;

            if(pdev->bars[bar].sz != 0x0) {
                res->mmio_space.pa = UPALIGN(res->mmio_space.pa, pdev->bars[bar].sz);
                   
                if(res->mmio_space.pa + pdev->bars[bar].sz >= res->mmio_space.limit)
                {
                    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,
                                       "++ {%d:%d:%d} Resource :{0x%llX : 0x%llX} -- Error! Out of resources..Aborting",
                                       PCIE_SPLIT_BDF(pdev->rbdf),
                                       (res->mmio_space.pa + pdev->bars[bar].sz), 
                                       res->mmio_space.limit);
                    /* There is no recovery from this...We will have an imbalanced PCI tree..*/
                    return;
                }       
                pdev->bars[bar].pa = res->mmio_space.pa;
                res->mmio_space.pa += pdev->bars[bar].sz;
            }
            else
            {
                pdev->bars[bar].pa=0;
            }
            
            /* Assign the BAR resource */
            pcie_host_cfg_write(pdev->rc_dev,pdev->rbdf,PCIE_CFG_BAR(bar),pdev->bars[bar].pa);
            pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,
                               "Allocating bar %d for {%d:%d:%d} {Addr :0x%llX Size : 0x%llX}", 
                               bar, 
                               PCIE_SPLIT_BDF(pdev->rbdf),
                               pdev->bars[bar].pa, 
                               pdev->bars[bar].sz);
            
            if(type == 0x2) {
                /* 64-bit bar */
                //We only support 32-bit BARs for now...so mask the upper 32-bits anyways.
                bar++;
                pcie_host_cfg_write(pdev->rc_dev,pdev->rbdf,
                                    PCIE_CFG_BAR(bar),
                                    0x0);
            }
        }
    }
    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"-- {%d:%d;%d} Device (0x%X : 0x%X)", 
                       PCIE_SPLIT_BDF(pdev->rbdf),
                       pdev->vendor_id,
                       pdev->device_id);

}

/* ============================================================================
**  Function : pcie_host_assign_resources_per_device
** ============================================================================
*/
/**
    @brief
    Assign resources for the device and its downstream devices
    
    @details
    Assigns resources for teh device and its downstream devices. After assigning
    resources to the device, it first does a breadth wise walk of its peers and
    assign resources to them. As it unwinds from that walk, if it discovers
    peers with downstream ports, it recurses to allocate resources for them as well.
 
    By the time this routine is done, all devices that are at the same level or
    below this device will have their resources assigned.
    
    @param[in]  *pdev    Handle to the PCIe device
    @param[in]  *res     Resource map to be used to allocate resources

    @dependencies
    PCIe bus scan must have been completed and the topology is already built
    for a rootport.
    
    @sideeffects
    None
    
    @return
    None
    
*/
void pcie_host_assign_resources_per_device(pcie_devi_t *pdev, pcierc_res_t *res )
{
    pcie_devi_t *peer;
        
   if(res->mmio_space.pa >= res->mmio_space.limit)
   {
        pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,
                           "++ {%d:%d:%d} Resource :{0x%llX : 0x%llX} -- Error! Out of resources..Aborting", 
                           PCIE_SPLIT_BDF(pdev->rbdf),
                           res->mmio_space.pa, 
                           res->mmio_space.limit);
        /* We should probably fatal here, since we are now in a weird state of partially configured bus */
        return;
   }       
    /* We start with the entire range */
    pdev->mem_base = res->mmio_space.pa;
    pdev->mem_limit = res->mmio_space.limit;

    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,
                       "++ {%d:%d:%d} Resource :{0x%llX : 0x%llX}", 
                       PCIE_SPLIT_BDF(pdev->rbdf),
                       res->mmio_space.pa, 
                       res->mmio_space.limit);
    
    peer = pdev->peer;
    /* First we assign resources to the device itself */
    pcie_host_assign_bars(pdev, res);

    /* Check if the device has peers on the same bus...if so assign their resources first */
    if(peer)
    {        
        pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"Calling peer node of 0x%X", 
                           pdev->vendor_id);
        pcie_host_assign_resources_per_device(peer,res);
    }
    /* Now assign resources to the children if there are any */
    if(pdev->header == 1) 
    {
        if(pdev->children)
        {
            //Make sure the base is aligned at a 1MB boundary...
            res->mmio_space.pa = UPALIGN(res->mmio_space.pa, PCIE_MEM_1MB);
            pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"Calling child node of 0x%X", 
                               pdev->vendor_id);
            pcie_host_assign_resources_per_device(pdev->children,res);            
        }
        /* Now update the mem  base and limit */
        pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,
                           "Updating %d:%d:%d 's memory limit from 0x%llX to 0x%llX ",
                           PCIE_SPLIT_BDF(pdev->rbdf),
                           pdev->mem_limit, 
                           res->mmio_space.pa);
        pdev->mem_limit = res->mmio_space.pa;
        pcie_host_cfg_write(pdev->rc_dev,pdev->rbdf,
                            PCIE_CFG_TYPE1_MEMBASELIMIT, 
                            PCIE_MEM_CFG(pdev->mem_base,pdev->mem_limit));
        /* Disable Prefetch and IO spaces */
        pcie_host_cfg_write(pdev->rc_dev,pdev->rbdf,
                            PCIE_CFG_TYPE1_PREFETCH_MEMBASELIMIT, 
                            PCIE_MEM_CFG(0xFFFFFFFF,0x0));
        pcie_host_cfg_write(pdev->rc_dev,pdev->rbdf,
                            PCIE_CFG_TYPE1_IOBASELIMIT, 
                            PCIE_MEM_CFG(0xFFFFFFFF,0x0));

    }
    
    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"-- {%d:%d:%d} Resource :{0x%llX : 0x%llX}", 
                       PCIE_SPLIT_BDF(pdev->rbdf),
                       res->mmio_space.pa, 
                       res->mmio_space.limit);

}

/* ============================================================================
**  Function : pcie_host_assign_resources
** ============================================================================
*/
/**
    @brief
    Assign resources (memory, interrupt, caps) to the devices discovered in the
    system (including the rootports).
    
    @details
    Assign resources (memory, interrupt, caps) to the devices discovered in the
    system (including the rootports). Current version only allocates
    non-prefetchable memory to the devices. Future versions will exapand to support
    prefetchable memory, interrupts, capabilities etc.
    

    @dependencies
    PCIe ports must have been initialized
    
    @sideeffects
    None
    
    @return
    None
*/
void pcie_host_assign_resources()
{
    pcie_devi_t *rootport = pcie_rootports;
    pcierc_res_t res;
    
    while(rootport) {
        pcierc_core_get_resources(rootport->rc_dev, 
                                  (rootport->subordinate_bus + 1), 
                                  &res); //TODO: Need to add prefetch support later
        
        res.mmio_space.limit = res.mmio_space.pa + res.mmio_space.sz - 1;

        //Make sure the base is aligned at a 1MB boundary...
        res.mmio_space.pa = UPALIGN(res.mmio_space.pa, PCIE_MEM_1MB);
        pcie_host_osal_log(PCIE_HOST_LOG_DEBUG, "Resource available : {0x%llX : 0x%llX}", 
                           res.mmio_space.pa, 
                           res.mmio_space.limit );
        pcie_host_assign_resources_per_device(rootport, &res );
        rootport = rootport->peer;
    }

}

void pcie_host_probe(pcie_devi_t *pdev)
{
    int probe;
    int bar;
    pcie_host_drvinfo_t *drvinfo = &pcie_host_drvinfo_table[0];
    for(probe = 0; probe < pcie_num_drivers; probe++)
    {
        
        if ( (drvinfo->device_id.class_id==PCIE_HOST_ID_ANY || 
              drvinfo->device_id.class_id == pdev->class_code) &&
             (drvinfo->device_id.vendor_id==PCIE_HOST_ID_ANY || 
              drvinfo->device_id.vendor_id == pdev->vendor_id) &&
             (drvinfo->device_id.device_id==PCIE_HOST_ID_ANY || 
              drvinfo->device_id.device_id == pdev->device_id) )
             {
                 /* This is now owned by this driver..
                  */
                  pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"Calling %s for [%d:%d] found at {%d:%d:%d}",
                                     drvinfo->probe_info.drvname,pdev->vendor_id, 
                                     pdev->device_id, 
                                     PCIE_SPLIT_BDF(pdev->rbdf) );
                  for(bar=0;bar<PCIE_HOST_MAX_BARS;bar++) {
                      drvinfo->probe_info.cb_data->res->bars[bar] = pdev->bars[bar];
                  }
                  drvinfo->probe_info.probe_fn((pcie_host_dev_t *)pdev, 
                                             drvinfo->probe_info.cb_data);
             }
    }    
}

void pcie_host_enumerate()
{
    uint8 rc, bus, dev, func;
    uint32 device;
    pcierc_dev_t *pcierc_dev;
    uint32 num_rcs;

    pcie_host_osal_log(PCIE_HOST_LOG_INFO, "++");

    pcierc_core_getrcs(&pcierc_dev, &num_rcs);
    device = 0;
    rc = 0; bus = 0; dev = 0; func = 0;
    while(rc<num_rcs) {
        pcie_host_osal_log(PCIE_HOST_LOG_INFO,"Scanning Root complex %d", rc);

        /* Now scan for all devices in this bus */
        pcie_host_scan_bus(NULL, pcierc_dev, rc, bus);
        /* Now assign resources to the devices */
        pcie_host_assign_resources();
        rc++;
        pcierc_dev++;
    }

   /* Enumeration is complete at the bus level. Now probe each device to complete
    * device level enumeration. For this, we don't have to go through the 
    * rootports. We will simply cycle through the pcie_devs to probe the devices 
    * one by one.
    */
   for(dev=0; dev<pcie_devices;dev++)
   {
       pcie_host_probe(&pcie_devs[dev]);
   }

    pcie_host_osal_log(PCIE_HOST_LOG_INFO, "--");
}

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
void pcie_host_drv_init( )
{
    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"++");
    pcierc_core_init();

    pcie_host_enumerate();
    pcie_host_initialized = TRUE;
    pcie_host_osal_log(PCIE_HOST_LOG_DEBUG,"--");

}


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
    @param[in]  probe_cb     Probe call back to be called when the device is found

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
pcie_host_status_t pcie_host_register_device(pcie_host_device_id_t device_id, pcie_host_probe_info_t probe_info)
{
    if(pcie_num_drivers<MAX_PCIE_DRIVERS) {
        /*Eventually...this will be malloc'ed as part of drv init 
          and the ordering of this sequence will change a bit
          For now, it is a static array and we will leave it as is
          */
        pcie_host_drvinfo_table[pcie_num_drivers].device_id = device_id;
        pcie_host_drvinfo_table[pcie_num_drivers].probe_info = probe_info;
        pcie_num_drivers++;
    }
    else
    {
        return PCIE_HOST_ERROR;
    }
    if(pcie_host_initialized == FALSE)
    {
        pcie_host_drv_init();
    }
    return PCIE_HOST_SUCCESS;
}


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
 
    @param[in]  *pdev             Handle to the pcie device
    @param[in]  enable_options    Options to enable different operational modes 

    @dependencies
    Device must have been probed, and pdev should be valid
    
    @sideeffects
    None
    
    @return
    None
    
*/
void pcie_host_enable_device( pcie_host_dev_t *pdev, pcie_enable_opts_t enable_options)
{
    uint32 val = enable_options;
    if(NULL == pdev) 
    {
        return;
    }
    pcie_host_config_rw(pdev, PCIE_HOST_OPER_WRITE, PCIE_CFG_CMDSTTS, &val);
}

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
void pcie_host_disable_device(pcie_host_dev_t *pdev)
{
    uint32 val = 0x0;
    if(NULL == pdev) 
    {
        return;
    }
    pcie_host_config_rw(pdev, PCIE_HOST_OPER_WRITE, PCIE_CFG_CMDSTTS, &val);
}


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
void pcie_host_config_rw ( pcie_host_dev_t *pdev, pcie_host_rw_mode_t mode, uint32 offset, uint32 *val)
{
    pcie_devi_t *_pdev;
    if(NULL == pdev) 
    {
        return;
    }

    _pdev = (pcie_devi_t *)pdev;

    if(PCIE_HOST_OPER_WRITE == mode) {
        pcie_host_cfg_write(_pdev->rc_dev,_pdev->rbdf,offset, *val);
    }
    else
    {
        *val = pcie_host_cfg_read(_pdev->rc_dev,_pdev->rbdf,offset);
    }
}
