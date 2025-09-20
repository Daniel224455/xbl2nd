
/** @file
  Qualcomm ACPI Platform Driver call back function for chipinfo

  Copyright (c) 2014-2015,2019 Qualcomm Technologies Inc. All rights
  reserved. 
**/ 

/*=============================================================================
                              EDIT HISTORY
 when           who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/08/19   vb      Added AmlUpdatePlst variable
 01/08/19   vb      Added AmlUpdateSidt variable
07-09-15    wayne   Added for Chip Serial number callback
03-27-15    zzl       fix modem feature checking issue
03-20-15    zzl       fix modem feature checking issue
10-08-14    zzl       Created for Acpi chip info callback
=============================================================================*/
#include <AcpiPlatform.h>
#include "AcpiPlatformChipInfo.h"
#include "PlatformInfoDefs.h"
#include "smem_type.h"
#include <Library/BootConfig.h>
#include "HALhwio.h"

//booting from
typedef enum  {
  STOR_NVME = 0,
  STOR_UFS,
  STOR_EMMC
} StorType;

typedef boolean (*FunctionPtr)();

typedef struct {
   const char *bootOption; //string describing the boot option
   FunctionPtr storage_func;  //function pointer to the boot_from_<storage option> function       
   StorType storage;  //storage option
} StorDescription;

static const StorDescription storage_list[] = {
  {"NVME", boot_from_nvme, STOR_NVME},
  {"UFS", boot_from_ufs, STOR_UFS},
  {NULL}
};


EFI_STATUS EFIAPI AmlUpdateChipId (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
    EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
    UINTN i;
    EFIChipInfoIdType chipID = EFICHIPINFO_NUM_IDS;
    EFI_STATUS Status;

    if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
    {
      DEBUG((EFI_D_WARN,"AML: ChipID Buffer is empty\r\n"));
      return EFI_INVALID_PARAMETER;
    }
    //check aml variablebuffersize should be equal or bigger than the type of varaible we  suppose it to be . 
    if(AmlVariableBufferSize<sizeof(EFIChipInfoIdType))
    {
        DEBUG((EFI_D_WARN,"AML: ChipID input Buffer is too small\r\n"));
        return EFI_BUFFER_TOO_SMALL;
    }
    
    for(i=0; i<AmlVariableBufferSize; i++)
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));

    // locate chip info protocol
    Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN," gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
        return Status;
    }
    
    Status = pEfiChipInfoProtocol->GetChipId(pEfiChipInfoProtocol,&chipID);    
    if((chipID < EFICHIPINFO_NUM_IDS)&& (Status==EFI_SUCCESS))
    {
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," chipID: %x\r\n", chipID ));      
      *((UINT32 *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINT32)chipID;
    }
    else
    {
      DEBUG((EFI_D_WARN,"GetChipId failed\r\n" ));  
    }     
    return Status;
    
}

EFI_STATUS EFIAPI AmlUpdateChipChipIdString (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
  UINTN i;
  CHAR8 ChipIdString[EFICHIPINFO_MAX_ID_LENGTH];
  EFI_STATUS Status;
  DEBUG((DEBUG_LOAD,"AML: Update Chip and Speed Bin values\r\n"));
  if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
  {
    DEBUG((EFI_D_WARN,"AML: Chip Info Buffer is empty\r\n"));
    return EFI_INVALID_PARAMETER;
  }
  else
  {
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"  Chip info buffer:\r\n"));
  }
  
  //check aml variablebuffersize should be equal or bigger than the type of varaible we  suppose it to be . 
  if(AmlVariableBufferSize<EFICHIPINFO_MAX_ID_LENGTH)
  {
      DEBUG((EFI_D_WARN,"AML: ChipIdString input Buffer is too small\r\n"));
      return EFI_BUFFER_TOO_SMALL;
  }
  
  for(i=0; i<AmlVariableBufferSize; i++)
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
  DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));

  // locate chip info protocol
  Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
  if (EFI_ERROR(Status))
  {
      DEBUG((EFI_D_WARN," gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
      return Status;
  }
  
  Status = pEfiChipInfoProtocol->GetChipIdString( pEfiChipInfoProtocol, ChipIdString, EFICHIPINFO_MAX_ID_LENGTH);
  
  if((ChipIdString!=NULL)&&(Status == EFI_SUCCESS))
  {
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT," ChipIdString: %a\r\n", ChipIdString ));
    AsciiStrnCpy( ((CHAR8 *) *AmlVariableBuffer)+AML_NAMESPACE_HEADER_SIZE, ChipIdString, EFICHIPINFO_MAX_ID_LENGTH);
  }
  else
  {
    DEBUG((EFI_D_WARN,"GetChipIdString failed\r\n" ));  
  } 
  
  DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"  New chip info buffer:\r\n"));
  for(i=0; i<AmlVariableBufferSize; i++)
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
  DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));
  return EFI_SUCCESS;
}



EFI_STATUS EFIAPI AmlUpdateChipVersion (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
    EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
    UINTN i;
    EFIChipInfoVersionType ChipVersion;
    EFI_STATUS Status;
    if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
    {
      DEBUG((EFI_D_WARN,"AML: ChipID Buffer is empty\r\n"));
      return EFI_INVALID_PARAMETER;
    }

    for(i=0; i<AmlVariableBufferSize; i++)
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));
    
    //check aml variablebuffersize should be equal or bigger than the type of varaible we  suppose it to be . 
    if(AmlVariableBufferSize<sizeof(EFIChipInfoVersionType))
    {
        DEBUG((EFI_D_WARN,"AML: ChipVersion input Buffer is too small\r\n"));
        return EFI_BUFFER_TOO_SMALL;
    }
    

    // locate chip info protocol
    Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN," gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
        return Status;
    }

    Status = pEfiChipInfoProtocol->GetChipVersion(pEfiChipInfoProtocol,&ChipVersion);    
    if(Status ==  EFI_SUCCESS)
    {
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," chipID: %x\r\n", ChipVersion ));      
      *((UINT32 *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) =(UINT32) ChipVersion;
    }
    else{
      DEBUG((EFI_D_WARN," chipID: protocol failed\r\n" ));  
    }    
    
    return Status;
}


EFI_STATUS EFIAPI AmlUpdateMajorChipVersion (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
    EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
    UINTN i;
    EFIChipInfoVersionType ChipVersion;
    EFI_STATUS Status;
    if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
    {
      DEBUG((EFI_D_WARN,"AML: Major sChipID Buffer is empty\r\n"));
      return EFI_INVALID_PARAMETER;
    }

    for(i=0; i<AmlVariableBufferSize; i++)
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));
    
    //check aml variablebuffersize should be equal or bigger than the type of varaible we  suppose it to be . 
    if(AmlVariableBufferSize<sizeof(UINT16))
    {
        DEBUG((EFI_D_WARN,"AML: MajorChip input Buffer is too small\r\n"));
        return EFI_BUFFER_TOO_SMALL;
    }

    
    // locate chip info protocol
    Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN," gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
        return Status;
    }

    Status = pEfiChipInfoProtocol->GetChipVersion(pEfiChipInfoProtocol,&ChipVersion);    
    if(Status ==  EFI_SUCCESS)
    {
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," Major ChipVersion: %lx\r\n", ChipVersion ));      
      *((UINT16 *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINT16)(ChipVersion>>16);    
    }
    else{
      DEBUG((EFI_D_WARN," Major ChipVersion: protocol failed\r\n" ));  
    }
    return Status;
}

EFI_STATUS EFIAPI AmlUpdateMinorChipVersion (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
    EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
    UINTN i;
    EFIChipInfoVersionType ChipVersion;
    EFI_STATUS Status;
    if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
    {
      DEBUG((EFI_D_WARN,"AML:Minor ChipID Buffer is empty\r\n"));
      return EFI_INVALID_PARAMETER;
    }

    for(i=0; i<AmlVariableBufferSize; i++)
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));
    
    //check aml variablebuffersize should be equal or bigger than the type of varaible we  suppose it to be . 
    if(AmlVariableBufferSize<sizeof(UINT16))
    {
        DEBUG((EFI_D_WARN,"AML: MinorChip input Buffer is too small\r\n"));
        return EFI_BUFFER_TOO_SMALL;
    }

    // locate chip info protocol
    Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN," gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
        return Status;
    }

    Status = pEfiChipInfoProtocol->GetChipVersion(pEfiChipInfoProtocol,&ChipVersion);    
    if(Status ==  EFI_SUCCESS)
    {
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," Minor ChipVersion: %x\r\n", ChipVersion ));      
      *((UINT16 *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINT16)(ChipVersion&0xffff);
    }
    else{
      DEBUG((EFI_D_WARN," Minor ChipVersion: protocol failed\r\n" ));  
    }
    return Status;
}

EFI_STATUS EFIAPI AmlUpdateChipFamily(
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
    EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
    UINTN i;
    EFIChipInfoFamilyType ChipFamily;
    EFI_STATUS Status;

    if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
    {
      DEBUG((EFI_D_WARN,"AML: ChipFamily Buffer is empty\r\n"));
      return EFI_INVALID_PARAMETER;
    }

    for(i=0; i<AmlVariableBufferSize; i++)
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));
    
    //check aml variablebuffersize should be equal or bigger than the type of varaible we  suppose it to be . 
    if(AmlVariableBufferSize<sizeof(UINT16))
    {
        DEBUG((EFI_D_WARN,"AML: ChipFamily input Buffer is too small\r\n"));
        return EFI_BUFFER_TOO_SMALL;
    }
    
    // locate chip info protocol
    Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN," gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
        return Status;
    }

    Status = pEfiChipInfoProtocol->GetChipFamily(pEfiChipInfoProtocol,&ChipFamily);    
    if((ChipFamily < EFICHIPINFO_NUM_FAMILIES)&&(Status == EFI_SUCCESS))
    {
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," ChipFamily: %x\r\n", ChipFamily ));      
      *((UINT16 *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINT16)ChipFamily;
    }
    else{
        DEBUG((EFI_D_WARN," ChipFamily: protocol failed\r\n" ));  
    }
    return Status;
}


EFI_STATUS EFIAPI AmlUpdateModemInfo(
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
    EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
    UINTN i;
    EFIChipInfoModemType ModemInfo;
    EFI_STATUS Status;

    if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
    {
      DEBUG((EFI_D_WARN,"AML: ModemInfo Buffer is empty\r\n"));
      return EFI_INVALID_PARAMETER;
    }

    for(i=0; i<AmlVariableBufferSize; i++)
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));
    
    //check aml variablebuffersize should be equal or bigger than the type of varaible we  suppose it to be . 
    if(AmlVariableBufferSize<sizeof(EFIChipInfoModemType))
    {
        DEBUG((EFI_D_WARN,"AML: ModemInfo input Buffer is too small\r\n"));
        return EFI_BUFFER_TOO_SMALL;
    }

    // locate chip info protocol
    Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN," gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
        return Status;
    }
    Status = pEfiChipInfoProtocol->GetModemSupport(pEfiChipInfoProtocol,&ModemInfo);    
    if(Status == EFI_SUCCESS)
    {
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," ModemInfo: %x\r\n", ModemInfo ));      
      *((UINT16 *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINT16 ) ModemInfo;
    }
    else
    {
        DEBUG((EFI_D_WARN,"GetModemSupport failed\r\n" ));  
        return Status;
    }    
    return Status;
}


EFI_STATUS EFIAPI AmlUpdateChipSerialNum(
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
    EFI_CHIPINFO_PROTOCOL *pEfiChipInfoProtocol = NULL;
    UINTN i;
    EFIChipInfoSerialNumType chipSerialNum = 0;
    EFIChipInfoQFPROMChipIdType chipIDinfo = 0;
    EFI_STATUS Status;

    if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
    {
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: Chip Serial num Buffer is empty\r\n"));
      return EFI_SUCCESS;
    }
    //check aml variablebuffersize should be equal or bigger than the type of varaible we  suppose it to be . 
    if(AmlVariableBufferSize<sizeof(EFIChipInfoSerialNumType))
    {
        DEBUG((EFI_D_WARN,"AML:  Chip Serial num  input Buffer is too small\r\n"));
        return EFI_BUFFER_TOO_SMALL;
    }
    
    for(i=0; i<AmlVariableBufferSize; i++)
      DEBUG((ACPI_PLATFORM_DEBUG_PRINT," %02x", *( (UINT8*)(*AmlVariableBuffer)+i) ));
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"\r\n"));

    // locate chip info protocol
    Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_WARN," gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
        return Status;
    }
    
    Status = pEfiChipInfoProtocol->GetSerialNumber(pEfiChipInfoProtocol,&chipSerialNum);    
    if(Status==EFI_SUCCESS)
    {   
      *((UINT32*) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINT32)chipSerialNum;
    }
    else
    {
      DEBUG((EFI_D_WARN,"Get  Chip Serial num  failed\r\n" ));  
      return Status;
    }     

    Status = pEfiChipInfoProtocol->GetQFPROMChipId(pEfiChipInfoProtocol,&chipIDinfo);    
    if(Status==EFI_SUCCESS)
    {   
        *((UINT32*) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE+sizeof(UINT32))) = (UINT32)chipIDinfo;
    }
    else
    {
      DEBUG((EFI_D_WARN,"Get  Chip Serial num : id info failed\r\n" ));  
    }     

    
    return Status;
    
}


/*
 * Call SMEM APIs to retrieve the base address of the socinfo shared memory region
 * which contains chip and platform information populated by XBL loader.
 */
EFI_STATUS EFIAPI AmlUpdateSocInfoAddr(
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_SMEM_PROTOCOL *pEfiSmemProtocol = NULL;
  EFI_STATUS Status;
  UINTN nSocInfoAddr;
  DalPlatformInfoSMemType *pSocInfo = (DalPlatformInfoSMemType *)&nSocInfoAddr;
  UINT32 nSocInfoSize = 0;

  if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
  {
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: Chip SocInfo buffer is empty\r\n"));
    return EFI_SUCCESS;
  }

  // The AML variablebuffer size should be large enough to hold an address
  if(AmlVariableBufferSize < sizeof(UINTN))
  {
    DEBUG((EFI_D_WARN, "AML: Chip SocInfo buffer is too small\r\n"));
    return EFI_BUFFER_TOO_SMALL;
  }

  // Locate the EFI SMEM protocol
  Status = gBS->LocateProtocol(&gEfiSMEMProtocolGuid,NULL, (VOID **)&pEfiSmemProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN,"gEfiSmemProtocolGuid protocol failed\r\n"));
    return Status;
  }

  // Get the socinfo address
  Status = pEfiSmemProtocol->SmemGetAddr(SMEM_HW_SW_BUILD_ID, &nSocInfoSize, (VOID **)&pSocInfo);
  if(Status == EFI_SUCCESS)
  {
    // Store the pointer to SMEM in the ACPI variable
    *((UINTN*) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINTN)pSocInfo;//nSocInfoAddr;
  }
  else
  {
    DEBUG((EFI_D_WARN, "Get socinfo address failed\r\n"));
    return Status;
  }
  return Status;
}


EFI_STATUS EFIAPI AmlGetBootOption(
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
  {
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: GetBootOptions buffer is empty\r\n"));
    return EFI_SUCCESS;
  }

  int CurrentEntry = 0;

  //iterate through storage list and figure out where we're booting from
  //if not found, initialize with default value from acpi
  while(NULL != storage_list[CurrentEntry].bootOption) {
    if(1 == storage_list[CurrentEntry].storage_func()) {
      *((UINT32*) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINT32)(storage_list[CurrentEntry].storage);
    }
    CurrentEntry++;
  }

  return Status;
}


EFI_STATUS EFIAPI AmlUpdateSidt(
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 in_buf = 0;

  if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
  {
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: UpdateSidtVars buffer is empty\r\n"));
    return EFI_SUCCESS;
  }
  //check aml variablebuffersize 
  if(AmlVariableBufferSize<sizeof(UINT32))
  {
    DEBUG((EFI_D_WARN,"AML: UpdateSidtVars input Buffer is too small\r\n"));
    return EFI_BUFFER_TOO_SMALL;
  }

  in_buf = HWIO_INF(QFPROM_CORR_PTE_ROW0_LSB, FEATURE_ID);
  DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: UpdateSidtVars FEATURE_ID buffer is %d\r\n", in_buf));
  *((UINT32 *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) =(UINT32) in_buf;

  return Status;
}


EFI_STATUS EFIAPI AmlUpdatePlst(
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_PLATFORMINFO_PROTOCOL           * hPlatformInfoProtocol;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE   PlatformInfo;
  EFI_STATUS                            Status = EFI_SUCCESS;

  if((AmlVariableBuffer == NULL)||(*AmlVariableBuffer == NULL))
  {
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: AmlUpdatePlst buffer is empty\r\n"));
    return EFI_SUCCESS;
  }

  // Read the platform info to get CDT substype
  Status = gBS->LocateProtocol (&gEfiPlatformInfoProtocolGuid,
                                NULL,
                                (VOID **)&hPlatformInfoProtocol);
  if EFI_ERROR (Status)
  {
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: AmlUpdatePlst LocateProtocol Failed\r\n"));
    return EFI_SUCCESS;
  }
  //check aml variablebuffersize 
  if(AmlVariableBufferSize<sizeof(UINT32))
  {
    DEBUG((EFI_D_WARN,"AML: AmlUpdatePlst input Buffer is too small\r\n"));
    return EFI_BUFFER_TOO_SMALL;
  }

  Status = hPlatformInfoProtocol->GetPlatformInfo (hPlatformInfoProtocol,
                                                   &PlatformInfo);
  if EFI_ERROR (Status)
  {
    DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: AmlUpdatePlst GetPlatformInfo Failed\r\n"));
    return EFI_SUCCESS;
  }

  DEBUG((ACPI_PLATFORM_DEBUG_PRINT,"AML: AmlUpdatePlst CDT subtype buffer is %d\r\n", PlatformInfo.subtype));
  *((UINT32 *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) =(UINT32) PlatformInfo.subtype;

  return Status;
}
