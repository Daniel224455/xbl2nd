//============================================================================
/**
  Copyright (c) 2015-2018 Qualcomm Technologies, Inc. All rights reserved.
  Portions Copyright (c) 2007 - 2013 Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php
  
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 */
//============================================================================

#include <Protocol/EFIHALIOMMUProtocol.h>
#include <Library/DebugLib.h>
#include "HALIOMMUReg.h"
#include "HALIOMMULib.h"

/* Forward function declarations */
void _HAL_IOMMU_protocol_init(HAL_IOMMU_fptable_t **p_fp_table);
void _HAL_IOMMU_protocol_deinit(void);
static EFI_STATUS HAL_IOMMU_Dummy_Client(void);

/* Global Variables */
void *p_sdc_domain = NULL/*, *p_ufs_domain = NULL, *p_usb_domain = NULL, *p_ufs_domain = NULL, *p_pcie_domain = NULL*/;


EFI_HAL_IOMMU_PROTOCOL HAL_IOMMUProtocol = 
{
    HAL_IOMMU_REVISION,
    _HAL_IOMMU_protocol_init,
    _HAL_IOMMU_protocol_deinit,
};

void _HAL_IOMMU_protocol_init(HAL_IOMMU_fptable_t **p_fp_table)
{
    static HAL_IOMMU_fptable_t hal_iommu_fp_table =
    {
        hal_iommu_domain_create,
        hal_iommu_domain_delete,
        hal_iommu_domain_attach_device,
        hal_iommu_domain_detach_device,
        hal_iommu_domain_configure,
        hal_iommu_flush_domain,
        hal_iommu_flush_domain_va_list,
        hal_iommu_register_fault_handler,
        hal_iommu_configure_bypass_domain,
		IDIOM_Detach,
    };
    
    // install function pointer table
    *p_fp_table = &hal_iommu_fp_table;
}

void _HAL_IOMMU_protocol_deinit(void)
{
    // TODO
    return;
}

EFI_STATUS
EFIAPI
HAL_IOMMUDxeEntryPoint(
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
   EFI_HANDLE  handle = NULL;
   EFI_STATUS  status;

   // Publish the HAL_IOMMU Protocol
   status = gBS->InstallMultipleProtocolInterfaces( &handle, 
         &gEfiHalIommuProtocolGuid, 
         (void **)&HAL_IOMMUProtocol, NULL );
   if (EFI_SUCCESS != status)
   {
      return status;
   }
   
   // Register BootExitServices Callback function
   //TODO
   
   // Invoke dummy client and return
   return HAL_IOMMU_Dummy_Client();
}

static EFI_STATUS HAL_IOMMU_Dummy_Client(void)
{
    struct hal_iommu_domain_config non_cca_domain_cfg/*, cca_domain_cfg*/;
    HAL_IOMMUResult_t ret;
    
    // Create Non-CCA Domains
    // SDC Domain Creation
    if(HAL_IOMMU_ERR_OK != hal_iommu_domain_create(&p_sdc_domain)) {
        return EFI_NOT_READY;
    }
/* 
    // UFS Domain Creation
    if(HAL_IOMMU_ERR_OK != hal_iommu_domain_create(&p_ufs_domain)) {
        return EFI_NOT_READY;
    }
*/ 
    /*** Configure Domains ***/
    // setup non cca domain configuration and then configure the domains
    non_cca_domain_cfg.vmsa = HAL_IOMMU_ARM_ARCH_AARCH64;
    non_cca_domain_cfg.config.domain_cfg64.TTBR0 = 0x0;
    non_cca_domain_cfg.config.domain_cfg64.TTBR1 = 0x0;
    non_cca_domain_cfg.config.domain_cfg64.MAIR0 = 0x0;
    non_cca_domain_cfg.config.domain_cfg64.MAIR1 = 0x0;
    non_cca_domain_cfg.config.domain_cfg64.SCTLR = HAL_IOMMU_BYPASS_NON_CCA_SCTLR_VAL;
    non_cca_domain_cfg.config.domain_cfg64.TCR = HAL_IOMMU_BYPASS_NON_CCA_TCR_VAL;
    
    // SDC Domain configuration
    if(HAL_IOMMU_ERR_OK != hal_iommu_domain_configure(p_sdc_domain, &non_cca_domain_cfg)) {
        return EFI_NOT_READY;
    }
/* 
    // UFS Domain configuartion
    if(HAL_IOMMU_ERR_OK != hal_iommu_domain_configure(p_ufs_domain, &non_cca_domain_cfg)) {
        return EFI_NOT_READY;
    }
*/
/*    
    // Create CCA Domains
    // PCIe
    if(HAL_IOMMU_ERR_OK != hal_iommu_domain_create(&p_pcie_domain)) {
        return EFI_NOT_READY;
    }
    
    // setup cca domain configuration and then conifgure the domain
    cca_domain_cfg.vmsa = HAL_IOMMU_ARM_ARCH_AARCH64;
    cca_domain_cfg.vmsa = HAL_IOMMU_ARM_ARCH_AARCH64;
    cca_domain_cfg.config.domain_cfg64.TTBR0 = 0x0;
    cca_domain_cfg.config.domain_cfg64.TTBR1 = 0x0;
    cca_domain_cfg.config.domain_cfg64.MAIR0 = 0x0;
    cca_domain_cfg.config.domain_cfg64.MAIR1 = 0x0;
    cca_domain_cfg.config.domain_cfg64.SCTLR = HAL_IOMMU_BYPASS_CCA_SCTLR_VAL;
    cca_domain_cfg.config.domain_cfg64.TCR = HAL_IOMMU_BYPASS_CCA_TCR_VAL;
    
    // PCIe Domain configuration
    if(HAL_IOMMU_ERR_OK != hal_iommu_domain_configure(p_pcie_domain, &cca_domain_cfg)) {
        return EFI_NOT_READY;
    }
*/    
    /***  Attach Devices (for all ARIDs) to their domains ***/
    // attach SDC devices
    ret = hal_iommu_domain_attach_device(p_sdc_domain, (void *)"\\_SB_.SDC2", 0x0, 0x0);
    if(ret != HAL_IOMMU_ERR_OK) {
        DEBUG ((EFI_D_ERROR,"SDC2 node not found in the IORT\n"));
    }
    ret = hal_iommu_domain_attach_device(p_sdc_domain, (void *)"\\_SB_.SDC4", 0x0, 0x0);
    if(ret != HAL_IOMMU_ERR_OK) {
        DEBUG ((EFI_D_ERROR,"SDC4 node not found in the IORT\n"));
    }
/*
    // attach UFS device (ARID 0x0, 0x10)
    ret = hal_iommu_domain_attach_device(p_ufs_domain, (void *)"\\_SB_.UFS0", 0x0, 0x0);
    if(ret != HAL_IOMMU_ERR_OK) {
        return EFI_NOT_READY;
    }
    ret = hal_iommu_domain_attach_device(p_ufs_domain, (void *)"\\_SB_.UFS0", 0x10, 0x0);
    if(ret != HAL_IOMMU_ERR_OK) {
        return EFI_NOT_READY;
    }
    ret = hal_iommu_domain_attach_device(p_ufs_domain, (void *)"\\_SB_.UFS0", 0x20, 0x0);
    if(ret != HAL_IOMMU_ERR_OK) {
        return EFI_NOT_READY;
    }
*/ 
/*    
    // attach PCIe devices
    ret = hal_iommu_domain_attach_device(p_pcie_domain, (void *)"\\_SB_.PCI0", 0x0, 0x0);
    if(ret != HAL_IOMMU_ERR_OK) {
        return EFI_NOT_READY;
    }
    ret = hal_iommu_domain_attach_device(p_pcie_domain, (void *)"\\_SB_.PCI1", 0x0, 0x0);
    if(ret != HAL_IOMMU_ERR_OK) {
        return EFI_NOT_READY;
    }
*/    
	
	return EFI_SUCCESS;
}

HAL_IOMMUResult_t IDIOM_domain_lookup(void**p_domain_local, void *client_handle)
{
	HAL_IOMMUResult_t ret = HAL_IOMMU_ERR_OK;
	/* Input Validation */
    if( !client_handle || !p_domain_local ) {
        return HAL_IOMMU_ERR_NULL_POINTER;
    }
	
	/*ToDo Have a table of Client_name and Device_Handle 
  	 *either a hash or a ENUM of devices to scale the IDIOM driver init/lookup/deinit */
 	//Get the domain for the said device
	if( strcmp((void *)"\\_SB_.SDC2",client_handle ) == 0 ||  strcmp((void *)"\\_SB_.SDC4",client_handle ) == 0)
	{
		*p_domain_local = p_sdc_domain;
	}
/* 
    else if(strcmp((void *)"\\_SB_.UFS0",client_handle ) == 0)
	{
		*p_domain_local = p_ufs_domain;
	}
*/ 
    else{
		return HAL_IOMMU_ERR_INVALID_PARAM;
	}
	
	return ret;	
}
