/** @file
  Updates the SMBios target specific lib.
   
  Copyright (c) 2019-2020 Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 08/08/22   nauduri Revert support for AG&AH using PTE fuse
 09/24/21   rc      Add support for AG&AH using PTE fuse
 1/16/20    pd      Added CPU max speed for APQ feature_id and MSM feature_id
 12/02/19   rc      Add support for processor version string
 08/07/19   vb      fix DDR max freq for MSM v2.1 AA
 05/16/19   rc      Added CPU & DDR max freq for AD SKU CR 2453566
 04/10/19   vb      Add support for reading APQ feature_id fuse information
 01/20/19   vb      Add support for reading feature_id fuse information

=============================================================================*/

#include <Uefi.h>
#include "HALhwio.h"
#include "HALbootHWIO.h"
#include "SmbiosLibTarget.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIChipInfo.h>
#include <Library/DebugLib.h>
#include <Library/QcomLib.h>

/**
Get feature id

@retval fuse feature id value

**/
UINT32 SmbiosGetFeatureID()
{
    return (UINT32) HWIO_INF(QFPROM_CORR_PTE_ROW0_LSB, FEATURE_ID);
}

/**
Get processor max speed

@param *maxSpeed        pointer to max speed
node

@retval EFI_SUCCESS    success

**/
EFI_STATUS smbiosGetMaxSpeed(UINT16 *maxSpeed)
{
  EFI_STATUS                Status = EFI_UNSUPPORTED;
  EFI_CHIPINFO_PROTOCOL     *pEfiChipInfoProtocol = NULL;
  EFIChipInfoIdType         chipID = EFICHIPINFO_NUM_IDS;
  EFIChipInfoVersionType    ChipVersion;
  UINT32                    in_buf = 0;

  if (maxSpeed == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }

  // locate chip info protocol
  Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN,"gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
    return Status;
  }
  
  Status = pEfiChipInfoProtocol->GetChipId(pEfiChipInfoProtocol,&chipID);    
  if((Status != EFI_SUCCESS) || (chipID == EFICHIPINFO_NUM_IDS))
  {
    DEBUG((EFI_D_WARN,"GetChipId failed\r\n" ));  
    return Status;
  }
  
  Status = pEfiChipInfoProtocol->GetChipVersion(pEfiChipInfoProtocol,&ChipVersion);    
  if(Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN,"GetChipVersion failed\r\n" ));  
    return Status;
  }

  in_buf = SmbiosGetFeatureID();

  if(chipID == EFICHIPINFO_ID_SCX8180)
  {
    if((ChipVersion == 0x20000) && (in_buf == 0x5))
    {
      *maxSpeed = 2841;
    }
    else if((ChipVersion == 0x20001) && (in_buf == 0x2))
    {
      *maxSpeed = 2841;
    }
    else if((ChipVersion == 0x20001) && (in_buf == 0x5))
    {
      *maxSpeed = 3148;
    }
    else if((ChipVersion == 0x20001) && (in_buf == 0x3))
    {
      *maxSpeed = 2457;
    }
    else 
    {
      *maxSpeed = 2995;
    }
  }
  else if (chipID == EFICHIPINFO_ID_SC8180XP)
  {
    if((ChipVersion == 0x20001) && (in_buf == 0x1))
    {
      *maxSpeed = 2841;
    }
    else if((ChipVersion == 0x20001) && (in_buf == 0x2))
    {
      *maxSpeed = 2457;
    }
    else if((ChipVersion == 0x20001) && (in_buf == 0x3))
    {
      *maxSpeed = 3148;
    }
    else
    {
      *maxSpeed = 2995;
    }
  }

  return Status;
}



/**
Get DDR speed

@param *DDRSpeed        pointer to ddr speed
node

@retval EFI_SUCCESS    success

**/
EFI_STATUS smbiosGetDDRSpeed(UINT16 *DDRSpeed)
{
  EFI_STATUS                Status = EFI_UNSUPPORTED;
  EFI_CHIPINFO_PROTOCOL     *pEfiChipInfoProtocol = NULL;
  EFIChipInfoIdType         chipID = EFICHIPINFO_NUM_IDS;
  EFIChipInfoVersionType    ChipVersion;
  UINT32                    in_buf = 0;

  if (DDRSpeed == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }

  // locate chip info protocol
  Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN,"gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
    return Status;
  }
  
  Status = pEfiChipInfoProtocol->GetChipId(pEfiChipInfoProtocol,&chipID);    
  if((Status!=EFI_SUCCESS) || (chipID == EFICHIPINFO_NUM_IDS))
  {
    DEBUG((EFI_D_WARN,"GetChipId failed\r\n" ));  
    return Status;
  }
  
  Status = pEfiChipInfoProtocol->GetChipVersion(pEfiChipInfoProtocol,&ChipVersion);    
  if(Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN,"GetChipVersion failed\r\n" ));  
    return Status;
  }

  in_buf = SmbiosGetFeatureID();

  if(chipID == EFICHIPINFO_ID_SCX8180)
  {
    if((ChipVersion == 0x20000) && (in_buf == 0x5))
    {
      *DDRSpeed = 1805;
      Status = EFI_SUCCESS;
    }
    else if((ChipVersion == 0x20001) && (in_buf == 0x2))
    {
      *DDRSpeed = 1805;
      Status = EFI_SUCCESS;
    }
    else
    {
      *DDRSpeed = 2096;
      Status = EFI_SUCCESS;
    }
  }
  else if (chipID == EFICHIPINFO_ID_SC8180XP)
  {
    if((ChipVersion == 0x20001) && (in_buf == 0x1))
    {
      *DDRSpeed = 1805;
      Status = EFI_SUCCESS;
    }
    else
    {
      *DDRSpeed = 2096;
      Status = EFI_SUCCESS;
    }
  }

  return Status;
}

/**
Get processor version string

@param *ProcessorVersionString        pointer to version
node

@retval EFI_SUCCESS    success

**/
EFI_STATUS smbiosGetProcVersionStr(CHAR8 *ProcessorVersionString)
{
  EFI_STATUS                Status = EFI_UNSUPPORTED;
  EFI_CHIPINFO_PROTOCOL     *pEfiChipInfoProtocol = NULL;
  EFIChipInfoIdType         chipID = EFICHIPINFO_NUM_IDS;
  EFIChipInfoVersionType    ChipVersion;
  UINT32                    in_buf = 0;
  UINTN                     procVersionStringLen = 0;

  if (ProcessorVersionString == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }

  // locate chip info protocol
  Status = gBS->LocateProtocol(&gEfiChipInfoProtocolGuid,NULL, (VOID**) &pEfiChipInfoProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN,"gEfiChipInfoProtocolGuid protocol failed\r\n" ));  
    return Status;
  }
  
  Status = pEfiChipInfoProtocol->GetChipId(pEfiChipInfoProtocol,&chipID);    
  if((Status != EFI_SUCCESS) || (chipID == EFICHIPINFO_NUM_IDS))
  {
    DEBUG((EFI_D_WARN,"GetChipId failed\r\n" ));  
    return Status;
  }
  
  Status = pEfiChipInfoProtocol->GetChipVersion(pEfiChipInfoProtocol,&ChipVersion);    
  if(Status != EFI_SUCCESS)
  {
    DEBUG((EFI_D_WARN,"GetChipVersion failed\r\n" ));  
    return Status;
  }

  in_buf = SmbiosGetFeatureID();

  // allocate processor version string
  procVersionStringLen = AsciiStrSize((CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr));

  if(chipID == EFICHIPINFO_ID_SCX8180)
  {
    if((ChipVersion == 0x20001) && (in_buf == 0x3))
    { 
      //strip off the 'x' from '8cx' and 'Gen 2' for AD part
      Status = AsciiStrnCpyS(ProcessorVersionString, procVersionStringLen, (CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr), procVersionStringLen - 8);
    }
    else if ((ChipVersion == 0x20001) && (in_buf == 0x2))
    {
      //strip off the 'Gen 2' for the AA part
      Status = AsciiStrnCpyS(ProcessorVersionString, procVersionStringLen, (CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr), procVersionStringLen - 7); 
    }
    else
    {
      //AC, AE, AF, AB SKUs
      Status = AsciiStrCpyS(ProcessorVersionString, procVersionStringLen, (CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr));
        
    }
  }
  else if (chipID == EFICHIPINFO_ID_SC8180XP)
  {
    if((ChipVersion == 0x20001) && (in_buf == 0x2))
    {
      //strip off the 'x' from '8cx' and 'Gen 2' for AD part
      Status = AsciiStrnCpyS(ProcessorVersionString, procVersionStringLen, (CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr), procVersionStringLen - 8);
    }
    else if ((ChipVersion == 0x20001) && (in_buf == 0x1))
    {
      //strip off the 'Gen 2' for AA part
      Status = AsciiStrnCpyS(ProcessorVersionString, procVersionStringLen, (CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr), procVersionStringLen - 7);
    }
    else
    {
      //AC, AE, AF, AB SKUs
      Status = AsciiStrCpyS(ProcessorVersionString, procVersionStringLen, (CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr));
      
    }
  }
  return Status;
}