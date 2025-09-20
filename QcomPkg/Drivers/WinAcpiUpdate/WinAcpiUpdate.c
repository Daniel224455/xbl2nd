/*
  @file WinAcpiUpdate.c
    Copyright (c) 2016-2018,2023 by Qualcomm Technologies, Inc. All Rights Reserved.
  ============================================================================*/
  
  /*=============================================================================
                                EDIT HISTORY
  
  
   when 	  who	  what, where, why
   --------   ---	  -----------------------------------------------------------
   03/21/23   vbu         Porting partial binning changes of MK to address security issues. Driver is currently disabled for Rennell.
   05/04/18   ltg         Added qwpp and qdss option related changes
   02/17/17   vinitb      Added qccdi option related changes
   01/30/17   vinitb      Added sensors/gpu option related changes
   09/30/16   ltg         Added full implementation for patching ACPI, updating UEFI variable, and protocol API's
   08/4/16    vahidd      Intial Draft for Patching DSDT tables.
  =============================================================================*/


#include <PiDxe.h>
#include <Protocol/AcpiTable.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/UefiCfgLib.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/Acpi.h>

/**
WinAcpiUpdate PROTOCOL interfaces
*/
#include <Protocol/EfiWinAcpiUpdate.h>

/*Include QC ACPIPlatform Protocol */
#include <Protocol/EFIAcpiPlatform.h>
#include <Library/QcomBaseLib.h>

/*Include Limits Protocol */
#include <Protocol/EFILimits.h>

/* Include ChipInfo Protocol */
#include <Protocol/EFIChipInfo.h>
#include <Protocol/EFIChipInfoTypes.h>

#include "WinAcpiUpdate.h"

EFI_STATUS
SetDefectivePartsVariable(
IN  EFI_WIN_ACPI_UPDATE_PROTOCOL    *This,
IN  EFI_WIN_ACPI_PART_TYPE          Part_Type,
IN  BOOLEAN                         Disable
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    // update global variable
    if (Disable == TRUE)
    {
        defectivePartsBitMask |= (UINT32)(0x1 << Part_Type);
    }
    else
    {
        defectivePartsBitMask &= (UINT32)(~(0x1 << Part_Type));
    }

    PcdSet32(PcdDefectivePartsBitmask,defectivePartsBitMask);

    return Status;
}

/**
EFI_WIN_ACPI_UPDATE Protocol implementation
*/
EFI_WIN_ACPI_UPDATE_PROTOCOL gEfiWinAcpiUpdateProtocol =
{
    EFI_WIN_ACPI_UPDATE_PROTOCOL_REVISION,
    SetDefectivePartsVariable
};

static
EFI_STATUS
UpdateSTA(
IN EFI_ACPI_HANDLE STAAcpiHandle,
IN BOOLEAN Enabled
)
{
    EFI_ACPI_HANDLE ReturnAcpiHandle = NULL;
    EFI_ACPI_HANDLE ValueAcpiHandle = NULL;
    EFI_ACPI_DATA_TYPE DataType;
    CONST UINT8 *Data;
    UINTN DataSize;
    UINT8 STAValue;
    EFI_STATUS Status = EFI_SUCCESS;

    if (NULL == STAAcpiHandle) 
    {
        DEBUG((EFI_D_ERROR, "[WinAcpiUpdate] UpdateSTA: Handle is NULL. Exiting.\n"));
        Status = EFI_INVALID_PARAMETER;
        goto Error;
    }

    Status = gAcpiSdt->GetChild(STAAcpiHandle, &ReturnAcpiHandle);
    if (EFI_ERROR(Status) || (NULL == ReturnAcpiHandle))
    {
        goto Error;
    }

    Status = gAcpiSdt->GetOption(ReturnAcpiHandle, 0, &DataType, (CONST VOID **) &Data, &DataSize);
    if ((EFI_ERROR(Status)))
    {
        goto Error;
    }
    if ((DataType != EFI_ACPI_DATA_TYPE_OPCODE) || (Data[0] != AML_RETURN_OP))
    {
        Status = EFI_NOT_FOUND;
        goto Error;
    }

    Status = gAcpiSdt->GetChild(ReturnAcpiHandle, &ValueAcpiHandle);
    if ((EFI_ERROR(Status)) || (NULL == ValueAcpiHandle))
    {
        goto Error;
    }

    Status = gAcpiSdt->GetOption(ValueAcpiHandle, 0, &DataType, (CONST VOID **) &Data, &DataSize);
    if ((EFI_ERROR(Status)))
    {
        goto Error;
    }
    if ((DataType != EFI_ACPI_DATA_TYPE_OPCODE) || (Data[0] != AML_BYTE_PREFIX))
    {
        Status = EFI_NOT_FOUND;
        goto Error;
    }

    // get the value of the return statement
    Status = gAcpiSdt->GetOption(ValueAcpiHandle, 1, &DataType, (CONST VOID **) &Data, &DataSize);
    if ((EFI_ERROR(Status)))
    {
        goto Error;
    }

    if ((DataType == EFI_ACPI_DATA_TYPE_UINT) && (DataSize == 1)) 
    {
        if (Enabled) 
        {
            STAValue = Data[0];
        }
        else 
        {
            STAValue = 0x0;
        }
        if (Data[0] != STAValue) 
        {
            Status = gAcpiSdt->SetOption(ValueAcpiHandle, 1, &STAValue, sizeof(STAValue));
            if ((EFI_ERROR(Status))){
                DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateSTA: Error setting STA return value\n"));
                goto Error;
            }
        }
    }
    else 
    {
        Status = EFI_NOT_FOUND;
        goto Error;
    }

Error:
    if (ValueAcpiHandle)
    {
        gAcpiSdt->Close(ValueAcpiHandle);
    }
    if (ReturnAcpiHandle)
    {
        gAcpiSdt->Close(ReturnAcpiHandle);
    }
  return Status;
}


static 
EFI_STATUS
UpdateDsdtTable(
  IN EFI_ACPI_SDT_HEADER *Table,  
  IN EFI_ACPI_TABLE_VERSION Version)  
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_ACPI_HANDLE AcpiHandle = NULL;
    EFI_ACPI_HANDLE OutHandle = NULL;

    // open handle with top node of DSDT table
    Status = gAcpiSdt->Open((VOID*)(UINTN)Table+sizeof(EFI_ACPI_SDT_HEADER),&AcpiHandle);
    if (EFI_ERROR (Status))
    {
        DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateDsdtTable: Error opening the handle for DSDT tables \r\n"));
        goto Error;
    }

    defectivePartsBitMask = PcdGet32(PcdDefectivePartsBitmask);

    // check if there are any defective parts
    for (int i = 0; i < EFI_WIN_ACPI_PART_MAX; i++)
    {
        if((defectivePartsBitMask >> i) & 0x1)
        {
            DEBUG((EFI_D_INFO, "[WinAcpiUpdate]UpdateDsdtTable: Disabling defective part (0x%x) \r\n", i));
            Status = gAcpiSdt->FindPath(AcpiHandle,(UINT8*)DEVICE_LIST[i],&OutHandle);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateDsdtTable: Error gAcpiSdt->FindPath, Status = (0x%x) \r\n", Status));
                goto Error;
            }
            // Apply mask
            Status = UpdateSTA(OutHandle, FALSE);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateDsdtTable: UpdateACPI failed, Status = (0x%x) \r\n", Status));
            }
            DEBUG((EFI_D_WARN, "[WinAcpiUpdate]UpdateDsdtTable: Disabled features with defectivePartsBitMask (0x%x) \r\n", i));
        }
    }
    goto Error;

Error:
    if (AcpiHandle){
        gAcpiSdt->Close(AcpiHandle);
    }
    if (OutHandle){
        gAcpiSdt->Close(OutHandle);
    }
    return Status;
}

static
EFI_STATUS
EFIAPI
AcpiNotification (
    IN EFI_ACPI_SDT_HEADER *Table,
    IN EFI_ACPI_TABLE_VERSION Version,
    IN UINTN TableKey)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (Table->Signature == EFI_ACPI_6_1_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE)
    {
        Status = UpdateDsdtTable(Table,Version);
    }

    return Status;
}

static VOID updateDefectivePartsMask (IN  EFIChipInfoPartType defectivePart)
{
    switch(defectivePart)
    {
            case EFICHIPINFO_PART_GPU:
            case EFICHIPINFO_PART_VIDEO:
            case EFICHIPINFO_PART_CAMERA:
            case EFICHIPINFO_PART_DISPLAY:
                //GPU partial binning disables GPU, MMU1, and IMM1 in ACPI
                defectivePartsBitMask |= ((1 << EFI_WIN_ACPI_PART_GPU) | (1 << EFI_WIN_ACPI_PART_MMU1) | (1 << EFI_WIN_ACPI_PART_IMM1));
                defectivePartsFuseMask |= GPU_FUSE_MASK;
                DEBUG((EFI_D_WARN, "[WinAcpiUpdate]updateDefectivePartsMask: Marking GPU for disabling. Updated bitmask to (0x%x) \r\n", defectivePartsBitMask));
                break;
            case EFICHIPINFO_PART_COMPUTE_1:
                defectivePartsFuseMask |= NSP1_FUSE_MASK;
                DEBUG((EFI_D_WARN, "[WinAcpiUpdate]updateDefectivePartsMask: Marking NSP1 for disabling (Unsupported)\r\n"));
                break;
            case EFICHIPINFO_PART_COMP:
                defectivePartsBitMask |= (UINT32)(0x1 << EFI_WIN_ACPI_PART_NSP0);
                defectivePartsFuseMask |= NSP_BOTH_FUSE_MASK;
                DEBUG((EFI_D_WARN, "[WinAcpiUpdate]updateDefectivePartsMask: Marking NSP0 for disabling. Updated bitmask to (0x%x) \r\n", defectivePartsBitMask));
                break;
            case EFICHIPINFO_PART_SENSORS:
                defectivePartsBitMask |= (UINT32)(0x1 << EFI_WIN_ACPI_PART_SENSORS);
                defectivePartsFuseMask |= SCSS_FUSE_MASK;
                DEBUG((EFI_D_WARN, "[WinAcpiUpdate]updateDefectivePartsMask: Marking SCSS(Sensors) for disabling. Updated bitmask to (0x%x) \r\n", defectivePartsBitMask));
            break;
            case EFICHIPINFO_PART_SPSS:
                defectivePartsBitMask |= (UINT32)(0x1 << EFI_WIN_ACPI_PART_SPSS);
                defectivePartsFuseMask |= SPSS_FUSE_MASK;
                DEBUG((EFI_D_WARN, "[WinAcpiUpdate]updateDefectivePartsMask: Marking SPSS for disabling. Updated bitmask to (0x%x) \r\n", defectivePartsBitMask));
            break;
            default:
                break;
    }    
    
    //Fuse value & bins not aligned, need to correct if both NSP1 & NSP0 were binned here
    if((defectivePartsFuseMask & NSP_BOTH_FUSE_MASK) && (defectivePartsFuseMask & NSP1_FUSE_MASK))
    {
        defectivePartsFuseMask &= (~NSP1_FUSE_MASK);
        defectivePartsFuseMask |= NSP_BOTH_FUSE_MASK;
    }
            
    return;
}


static
EFI_STATUS
GetDefectiveParts(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT32 nMask = 0x0;

    if(RUMI_NO_GPU_CHECK())
    {
        //If we're running on a RUMI with no GPU disable GPU devices (GPU & MMU1)
        DEBUG((EFI_D_WARN, "[WinAcpiUpdate]UpdateACPIEntry: RUMI_NO_GPU_CHECK succeeded. Disabling GPU/MMU1/IMM1\r\n"));
        defectivePartsBitMask = ((1 << EFI_WIN_ACPI_PART_GPU) | (1 << EFI_WIN_ACPI_PART_MMU1) | (1 << EFI_WIN_ACPI_PART_IMM1));
    }
    else
    {
        // call chipinfo protocol
        // loop through all features, check in ACPI if it is enabled/disabled by default, and construct global variable
        for (int i = 0; i < EFICHIPINFO_NUM_PARTS; i++)
        {
            DEBUG((EFI_D_INFO, "[WinAcpiUpdate]GetDefectiveParts: Checking part (0x%x) \r\n", i));
            Status = pChipInfoProtocol->GetDefectivePart(pChipInfoProtocol, i, &nMask);
            if (EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]GetDefectiveParts: GetDefectivePart failed, Status = (0x%x), default to Non-Defective Part (0x%x) \r\n", Status, i));
            }
            else
            {
                // nMask is non-zero if the part is defective. "non-zero" currently means 1, but that might change in the future
                // if more granularity is needed to determine defectiveness
                if (nMask != 0)
                {
                    updateDefectivePartsMask((EFIChipInfoPartType)i);  //updates defectivePartsBitMask for defective part
                }
                else
                {
                    DEBUG((EFI_D_INFO, "[WinAcpiUpdate]GetDefectiveParts: GetDefectivePart got Non-Defective Part (0x%x) \r\n", i));
                }
            }
        }
        
        //Query defective CPU for fuse reporting
        Status = pChipInfoProtocol->GetDefectiveCPUs(pChipInfoProtocol, 0, &nMask);
        if (EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]GetDefectiveParts: GetDefectiveCPUs failed, Status = (0x%x). \r\n", Status));
        }
        else
        {
            // MK supports Gold+ defective binning only (GOLD_PLUS_BIN_MASK - 0xF0)
            if (nMask == GOLD_PLUS_BIN_MASK)
            {
                defectivePartsFuseMask |= CPU_FUSE_MASK;
            }
        }
    }
    DEBUG((EFI_D_WARN, "[WinAcpiUpdate]GetDefectiveParts: completed with fuse mask (0x%x) \r\n", defectivePartsFuseMask));
    PcdSet32(PcdDefectivePartsBitmask,defectivePartsBitMask);
    return Status;
}

EFI_STATUS
EFIAPI
UpdateACPIEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle,
                                                    &gEfiWinAcpiUpdateProtocolGuid,
                                                    (void *)&gEfiWinAcpiUpdateProtocol,
                                                    NULL);

    if (EFI_ERROR(Status)) 
    {
        DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateACPIEntry: Install WinAcpiUpdate Protocol failed, Status = (0x%x)\r\n", Status));
        goto Error;
    }

    // Locate ACPI SDT Protocol and register for callback for loading of ACPI tables
    Status = gBS->LocateProtocol(&gEfiAcpiSdtProtocolGuid, NULL, (VOID**)&gAcpiSdt);
    if (EFI_ERROR(Status)) 
    {
        DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateACPIEntry: Locate SDT Protocol failed, Status = (0x%x)\r\n", Status));
        goto Error;
    }

    // Register callback for when any ACPI table is loaded
    gAcpiSdt->RegisterNotify(TRUE,AcpiNotification);

    // Locate ChipInfo Protocol to get Defective Parts and CPUs
    Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid, NULL, (VOID **) &pChipInfoProtocol);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateACPIEntry: Locate ChipInfo Protocol failed, Status = (0x%x)\r\n", Status));
        goto Error; 
    }

    // Get defective parts
    Status = GetDefectiveParts();

    if (EFI_ERROR(Status))
    {
        defectivePartsBitMask = 0x0;
        DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateACPIEntry: GetDefectiveParts failed, Status = (0x%x)\r\n", Status));
        DEBUG((EFI_D_ERROR, "[WinAcpiUpdate]UpdateACPIEntry: Using Default defectivePartsBitMask = (0x%x)\r\n", defectivePartsBitMask));
    }
    DEBUG((EFI_D_INFO, "[WinAcpiUpdate]UpdateACPIEntry: GetDefectiveParts succeeded returning defectivePartsBitMask = (0x%x)\r\n", defectivePartsBitMask));

Error:
    return Status;
  
}
