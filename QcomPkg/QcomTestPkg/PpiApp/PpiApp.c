/** @file
                    PpiApp.c

  Copyright (c) 2019 Qualcomm Technologies, Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 09/13/19   dp       Initial version

=============================================================================*/
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TestInterface.h>
#include <Protocol/EFIMorPpi.h>
#include <Protocol/EFITrEE.h>
#include <Protocol/EFITpm.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/EFIScm.h>
#include "../../Drivers/TrEEDxe/TzfTPMCmd.h"


//control variables 
#define TPM_COMMAND_SIZE            0x1000
#define TPM_RESPONSE_SIZE           0x1000
#define TPM_BUFFER_SIZE             ((sizeof(TPM2_ACPI_CONTROL_AREA)) + (TPM_COMMAND_SIZE) + (TPM_RESPONSE_SIZE))

STATIC  TPM2_ACPI_CONTROL_AREA     *pControlArea = NULL;
STATIC  UINT32                      pControlAreaSize = 0;

extern EFI_GUID gQcomTokenSpaceGuid;
extern EFI_GUID gQcomMorPpiProtocolGuid;

typedef struct _PPI_EFI_VAR {
    UINT8 ManagementFlags;
    UINT8 PendingOperation;
    UINT8 LastOperation;
    UINT8 OperationResult;
} PPI_EFI_VAR_T;


EFI_STATUS ControlAreaSetup(void)
{
    EFI_STATUS         Status = EFI_SUCCESS;
    EFI_TPM_PROTOCOL  *pTpmProtocol = NULL;

    // locate Tpm protocol
    Status = gBS->LocateProtocol( &gEfiTpmProtocolGuid, NULL, (VOID **)&pTpmProtocol );
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, " Locate TPM Protocol failed, Status =  (0x%x)\r\n", Status));
      goto ErrorExit;
    }

    Status = pTpmProtocol->GetControlArea(pTpmProtocol, (PHYSICAL_ADDRESS  *)&pControlArea, &pControlAreaSize);
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, " SetControlArea failed, Status =  (0x%p)\r\n", Status));
      goto ErrorExit;
    }

    if (pControlArea != NULL)
    {
        // Zero out the entire control area.
        SetMem(pControlArea, sizeof(TPM2_ACPI_CONTROL_AREA), 0);

        // Initialize the Command and Response members of the control area to point to the appropriate places 
        // in the memory region (immediately after the control area).
        pControlArea->CommandSize  = TPM_COMMAND_SIZE;
        pControlArea->Command      = (UINT64)((UINT8*)pControlArea) + sizeof(TPM2_ACPI_CONTROL_AREA);
        pControlArea->ResponseSize = TPM_RESPONSE_SIZE;
        pControlArea->Response     = (UINT64)((UINT8*)pControlArea) + sizeof(TPM2_ACPI_CONTROL_AREA) + TPM_COMMAND_SIZE;
    }
    else
    {
        DEBUG((EFI_D_ERROR, " ControlArea Invalid."));
        Status = EFI_OUT_OF_RESOURCES;
        goto ErrorExit;
    }
       
ErrorExit:
    return Status;
}


/**
  The PPI application tests Physical Presence Interface
  feature.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS EFIAPI PpiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
    )
{

    EFI_STATUS Status = EFI_SUCCESS;
    UINTN PpiSize = sizeof(PPI_EFI_VAR_T);
    PPI_EFI_VAR_T Ppi = { 0 };
    EFI_QCOM_MORPPI_PROTOCOL *MorPpiProtocol = NULL;

    TEST_START("PpiApp");

    AsciiPrint("####NOTE: Reboot the device before running this test. ####\r\n");

    Status = gBS->LocateProtocol(&gQcomMorPpiProtocolGuid, NULL, (VOID**)&MorPpiProtocol );
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, " LocateProtocol failed, Status =  (0x%x)\r\n", Status));
        goto ErrorExit;
    }

    //set control area:
    Status = ControlAreaSetup();
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, " ControlAreaSetup failed, Status =  (0x%x)\r\n", Status));
        goto ErrorExit;
    }

    //set ppi variable
    Ppi.PendingOperation = 22;

    Status = gRT->SetVariable(L"UIPPI",
                    &gQcomTokenSpaceGuid,
                    (EFI_VARIABLE_NON_VOLATILE |
                    EFI_VARIABLE_BOOTSERVICE_ACCESS |
                    EFI_VARIABLE_RUNTIME_ACCESS),
                    PpiSize,
                    &Ppi);
    if (EFI_ERROR(Status))
    {
        AsciiPrint("ERROR: Set Ppi failed.\r\n");
        goto ErrorExit;
    }

    //Run process:
    AsciiPrint("Start PPI test.\r\n");
    Status = MorPpiProtocol->ProcessMorPpi(MorPpiProtocol);
    if (EFI_ERROR(Status)) 
    {
        AsciiPrint("ERROR: ProcessMorPpi failed status = %x\r\n", Status);
        goto ErrorExit;
    }

    // read Ppi variable.
    Status = gRT->GetVariable(L"UIPPI",
                    &gQcomTokenSpaceGuid,
                    NULL,
                    &PpiSize,
                    &Ppi);
    if (EFI_ERROR(Status)) 
    {
        AsciiPrint("ERROR: GetVariable Ppi failed status = %x\r\n", Status);
        goto ErrorExit;
    }

    if (22 == Ppi.LastOperation)
        AsciiPrint("ppi=%x PPI Test PASSED\n", Ppi);
    else 
    {
        AsciiPrint("***PPI FAILED***  Ppi = (0x%08x), result=%x, pend op= %x, last op= %x\r\n", Ppi, Ppi.OperationResult, Ppi.PendingOperation, Ppi.LastOperation);
        goto ErrorExit;
    }    

ErrorExit:

    TestStatus("PpiApp", Status);
    TEST_STOP("PpiApp");

    AsciiPrint("\n PpiApp Test completed. Press any key to exit and reset the device\n");
    ReadAnyKey(NULL, 0);
    gRT->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);

    return Status;
}
