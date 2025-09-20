/*============================================================================
  FILE:         NOCBWDxe.c

  OVERVIEW:     Implementation of the NOCBW EFI protocol interface

  DEPENDENCIES: None

                Copyright (c) 2020 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Confidential and Proprietary,
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who      what, where, why
  ----------  ---      -----------------------------------------------------------


============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFINpa.h>
#include <api/systemdrivers/icbarb.h>
#include <Protocol/EFINOCBWConfig.h>


/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

npa_client_handle bw_req_handle = NULL;
#define MAX_AB_IB 0xFFFFFFFF

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/

static void NotifyNpaCallback (IN EFI_EVENT  Event, IN VOID *Context);

static VOID IcbArbAvailableCallback (void *context, unsigned int event_type, void *data, unsigned int data_size);

/*==============================================================================

  FUNCTION      EFI_RemoveNOCBW

  DESCRIPTION   Removes NOC BW vote done by this DXE

==============================================================================*/
EFI_STATUS EFIAPI EFI_RemoveNOCBW (IN EFI_NOCBW_PROTOCOL *This)
{
 if(bw_req_handle == NULL)
  {
    // No bus bandwidth vote placed earlier.
    // Handle uninitialized
    return EFI_NOT_READY;
  }
  npa_complete_request(bw_req_handle);
  return EFI_SUCCESS;
}


STATIC EFI_NOCBW_PROTOCOL NOCBWInfoProtocol = 
{
  EFI_NOCBW_PROTOCOL_REVISION,
  EFI_RemoveNOCBW,
};

static VOID IcbArbAvailableCallback (void *context, unsigned int event_type, void *data, unsigned int data_size)
{
  ICBArb_MasterSlaveType BW_aMasterSlave[] =
  {
    { ICBID_MASTER_QUP_0, ICBID_SLAVE_EBI1 },
    { ICBID_MASTER_APPSS_PROC, ICBID_SLAVE_CLK_CTL },
  };

  ICBArb_RequestType BW_aRequest[] =
  {
	{ ICBARB_REQUEST_TYPE_3, { MAX_AB_IB, MAX_AB_IB, 0 } },
	{ ICBARB_REQUEST_TYPE_3, { MAX_AB_IB, MAX_AB_IB, 0 } },
  };

  bw_req_handle = npa_create_sync_client_ex( "/icb/arbiter",
									        "Uefi NOC BW req",
									        NPA_CLIENT_SUPPRESSIBLE_VECTOR,
									        sizeof(BW_aMasterSlave),
									        &BW_aMasterSlave);
  if (bw_req_handle == NULL)
  {
	DEBUG ((EFI_D_ERROR, "Unable to create /icb/arbiter client for uefi NOC BW req\n"));
	goto cleanup;
  }
  else
  {
	npa_issue_vector_request(bw_req_handle, sizeof(BW_aRequest)/sizeof(npa_resource_state),
							 (npa_resource_state *)BW_aRequest );
  }
cleanup:
  return;
}

static void NotifyNpaCallback (IN EFI_EVENT  Event, IN VOID *Context)
{
  EFI_NPA_PROTOCOL  *NpaProtocol = NULL;
  EFI_STATUS         Status      = EFI_DEVICE_ERROR;
  Status = gBS->LocateProtocol(&gEfiNpaProtocolGuid, NULL, (void**)&NpaProtocol);
  if(Status == EFI_SUCCESS)
  {
    npa_resource_available_cb("/icb/arbiter", IcbArbAvailableCallback, NULL);
  }
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
EFI_STATUS EFIAPI NOCBWDxeEntryPoint(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  EFI_EVENT NpaNotifyEvent = (EFI_EVENT)NULL;
  VOID *NpaNotifyEventReg = NULL;
  NpaNotifyEvent = EfiCreateProtocolNotifyEvent (
                          &gEfiNpaProtocolGuid,
                          TPL_CALLBACK,
                          NotifyNpaCallback,
                          NULL,
                          &NpaNotifyEventReg
                          );  
  if (NpaNotifyEvent == NULL) {
	DEBUG((EFI_D_WARN, "Error: Unable to register for NPA Notify client\n"));
	return EFI_DEVICE_ERROR;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, 
													&gEfiNOCBWProtocolGuid, 
													&NOCBWInfoProtocol, 
													NULL, NULL);
  return Status;
}

