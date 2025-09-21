/** @file
  Updates the SMBios table with the processor info structure.

  Copyright (c) 2010-2013,2017-2018 Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 06/08/18   ltg     Add max speed to processor version string
 01/16/18	  ltg		  Port changes from 1.2.2 to comply with v3.0
 11/01/13   vk      Fix 64 bit compiler warnings
 04/10/13   niting  Updated NumCpus to read from cfg file
 01/30/13   vk      Fix warning
 04/13/12   jz      Fixed ProcessorInfoVersionString
 03/23/12   kpa     Add support for config file based smbios table init
 07/20/11   yg      Added dynamic Processor info
 06/22/11   cgross  Added MpPark support
 05/25/11   niting  Initial revision

=============================================================================*/

#include <Uefi.h>
#include <Protocol/Smbios.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIChipInfo.h>
#include "SmbiosCfgInfo.h"
#include <Library/QcomLib.h>
#include <Library/UefiCfgLib.h>
#include "HALhwio.h"
#include "secboot_hwio.h"
#include "SmbiosLibTarget.h"

#pragma pack(1)

#define MAX_SPEED_TEMPLATE " @ x.xx GHz"

typedef struct _SMBIOS_PROCESSOR_INFO
{
  EFI_SMBIOS_TABLE_HEADER  Header;
  UINT8                    SocketDesignationStringNum;
  UINT8                    ProcessorType;
  UINT8                    ProcessorFamily;
  UINT8                    ProcessorManufacturerStringNum;
  UINT64                   ProcessorID;
  UINT8                    ProcessorVersionStringNum;
  UINT8                    Voltage;
  UINT16                   ExternalClock;
  UINT16                   MaxSpeed;
  UINT16                   CurrentSpeed;
  UINT8                    Status;
  UINT8                    ProcessorUpgrade;
  UINT16                   L1CacheHandle;
  UINT16                   L2CacheHandle;
  UINT16                   L3CacheHandle;
  UINT8                    SerialNumberStringNum;
  UINT8                    AssetTagStringNum;
  UINT8                    PartNumberStringNum;
  UINT8                    CoreCount;
  UINT8                    CoreEnabled;
  UINT8                    ThreadCount;
  UINT16                   ProcessorCharacteristics;
  UINT16                   ProcessorFamily2;
  UINT16                   CoreCount2;
  UINT16                   CoreEnabled2;
  UINT16                   ThreadCount2;

  UINT8                    String1[2];
  UINT8                    String2[2];
  UINT8                    String3[2];
  UINT8                    String4[2];
  UINT8                    EndNull;
}SMBIOS_PROCESSOR_INFO;

typedef struct _PROCESSOR_STRING_STRUCTURE{
  UINT8                    String1[2];
  UINT8                    String2[2];
  UINT8                    String3[2];
  UINT8                    String4[2];
  UINT8                    EndNull;
}PROCESSOR_STRING_STRUCTURE;

SMBIOS_PROCESSOR_INFO smbiosProcessorInfo =
{
  {
    EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION,
    sizeof(SMBIOS_PROCESSOR_INFO) - sizeof(PROCESSOR_STRING_STRUCTURE),
    0 //handle
  },
  1,                       // *** String 1 - SocketDesignationStringNum - only for socketed CPU;
  3,                       // ProcessorType - 1 is none, 2 is unknown, 3 is central processor
  0xFE,                    // ProcessorFamily - 0xFE means the real value is in ProcessorFamily2,
  2,                       // ***String 2:  ProcessorManfacturerStringNum
  0,                       // ProcessorID x86 specific
  3,                       // *** String3: ProcessorVersionString
  0x8B,                    // 1.1 V : 0x80 + (1.1*10) | 0x8B
  0,                       // ExternalClock - Sourcing clock in MHz
  2400,                    // MaxSpeed in MHz - initialized to bin 0 value
  1996,                    // CurrentSpeed - Current speed in MHz
  0x41,                    // Status - 1 - Processor Enabled.  Also OR in bit 6
  0x06,                    // ProcessorUpgrade - No upgrade
  0xFFFF,                  // L1CacheHandle - Type7 cache struct is provided
  0xFFFF,                  // L2CacheHandle - Type7 cache struct is provided
  0xFFFF,                  // L3CacheHandle - Type7 cache struct is provided
  0,                       // SerialNumberStringNum - Do we want one?
  0,                       //  AssetTagStringNum - Do we want one?
  4,                       // ***String4 - Add a string PartNumberStringNum;
  1,                       // CoreCount;
  1,                       // CoreEnabled;
  1,                       // ThreadCount;
  0x2C,                    // ProcessorCharacteristics - 64-bit Capable|Multi-Core|Execute Protection
  0x118,                   // ProcessorFamily2 - 0x118 is ARM
  0,                       // CoreCount2;
  0,                       // CoreEnabled2;
  0,                       // ThreadCount2;


  "1",
  "2",
  "3",
  "4",
  0
};


#pragma pack()

UINTN  ProcessorInfoSocketInfoStringNum = 1;
CHAR8*  ProcessorInfoSocketInfoString = (CHAR8*)PcdGetPtr(PcdProcessorInfoSockInfoStr);

UINTN  ProcessorManufacturerStringNum = 2;
CHAR8*  ProcessorManufacturerString = "Qualcomm Technologies Inc";

UINTN  ProcessorVersionStringNum = 3;
CHAR8* ProcessorVersionString = NULL;

UINTN  ProcessorInfoPartNumberStringNum = 4;
CHAR8  ProcessorInfoPartNumberString[EFICHIPINFO_MAX_ID_LENGTH];


extern EFI_SMBIOS_HANDLE       L1InstCacheHandle;
extern EFI_SMBIOS_HANDLE       L1DataCacheHandle;
extern EFI_SMBIOS_HANDLE       L2CacheHandle;
extern EFI_SMBIOS_HANDLE       L3CacheHandle;

/**
  Add Processor Info /Type 4 table to Smbios table List

  @param  SmbiosTableNode   Pointer to the Smbios Table list
                            node

  @retval EFI_SUCCESS    Initialization success


**/
EFI_STATUS ProcessorInfoTableInit( SMBIOS_TABLE_NODE     *SmbiosTableNode)
{
  EFI_STATUS       Status = EFI_INVALID_PARAMETER ;

  if( SmbiosTableNode == NULL)
    return Status;

  // Add type 4 /Processor info table.
  SmbiosTableNode->TablePtr= (VOID *) &smbiosProcessorInfo ;
  SmbiosTableNode->TableType = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
  Status = EFI_SUCCESS;

  return Status;
}

EFI_STATUS UpdateProcessorInfoTable (EFI_SMBIOS_PROTOCOL *Smbios)
{
  EFI_STATUS                Status;
  EFI_SMBIOS_HANDLE         ProcessorHandle = 0;
  EFI_CHIPINFO_PROTOCOL     *mChipInfoProtocol;
  UINT32 NumCpus = 0;
  UINT16                    maxSpeedGHZHigh = 0;
  UINT16                    maxSpeedGHZLow = 0;
  UINTN                     maxSpeedCharCount = 0;
  CHAR8                     maxSpeedString[AsciiStrSize(MAX_SPEED_TEMPLATE)];
  UINTN                     procVersionStringLen = 0;
  UINT16                    maxSpeedGHZLowRem = 0;

  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid,
                                NULL,
                                (VOID **) &mChipInfoProtocol
                               );
  if(Status != EFI_SUCCESS)
  {
    return Status;
  }

  // Update the number of processors
  Status = GetConfigValue ("NumCpus", &NumCpus);
  if ((Status != EFI_SUCCESS) || (NumCpus == 0))
  {
    DEBUG ((EFI_D_ERROR, "NumCpus not found in uefiplat.cfg\r\n"));
  }
  else
  {
    smbiosProcessorInfo.CoreCount = NumCpus;
    smbiosProcessorInfo.CoreEnabled = NumCpus;
    smbiosProcessorInfo.ThreadCount = NumCpus;
  }
  if(L1InstCacheHandle != 0)
  {
    smbiosProcessorInfo.L1CacheHandle = L1InstCacheHandle;
  }
  if(L2CacheHandle != 0)
  {
    smbiosProcessorInfo.L2CacheHandle = L2CacheHandle;
  }
  if(L3CacheHandle != 0)
  {
    smbiosProcessorInfo.L3CacheHandle = L3CacheHandle;
  }

  // get the speed bin and the freq in MHz for that speed bin
  UINT16 speedBin = HWIO_QFPROM_RAW_PTE_ROW0_LSB_INM(HWIO_QFPROM_RAW_PTE_ROW0_LSB_PTE_DATA0_BMSK) >> HWIO_QFPROM_RAW_PTE_ROW0_LSB_PTE_DATA0_SHFT;
  if (speedBin < NUM_SPEED_BINS)
  {
    smbiosProcessorInfo.MaxSpeed = SPEED_BIN_FREQ_MHZ[speedBin];
  }

  // construct max speed string in GHz
  maxSpeedGHZHigh = smbiosProcessorInfo.MaxSpeed / 1000;
  maxSpeedGHZLow = (smbiosProcessorInfo.MaxSpeed % 1000);
  maxSpeedGHZLowRem = maxSpeedGHZLow % 10;
  if (maxSpeedGHZLowRem < 5)
  {
    maxSpeedGHZLow = maxSpeedGHZLow / 10;
  }
  else
  {
    maxSpeedGHZLow = (maxSpeedGHZLow / 10) + 1;
  }
  maxSpeedCharCount = AsciiSPrint(maxSpeedString, sizeof(maxSpeedString), " @ %d.%d GHz", maxSpeedGHZHigh, maxSpeedGHZLow);

  // allocate processor version string
  procVersionStringLen = AsciiStrSize((CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr));
  ProcessorVersionString = (CHAR8*)AllocatePool(procVersionStringLen + maxSpeedCharCount);
  if (ProcessorVersionString == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }

  // copy processor string
  Status = AsciiStrCpyS(ProcessorVersionString, procVersionStringLen, (CHAR8*)PcdGetPtr(PcdProcessorInfoVersionStr));
  if (Status != EFI_SUCCESS)
  {
    return Status;
  }

  // concat processor string with max speed string
  Status = AsciiStrCatS(ProcessorVersionString, procVersionStringLen + maxSpeedCharCount, maxSpeedString);
  if (Status != EFI_SUCCESS)
  {
    return Status;
  }

  //Add the processor Info
  Status = Smbios->Add(Smbios, NULL, &ProcessorHandle, (EFI_SMBIOS_TABLE_HEADER*)&smbiosProcessorInfo);
  if(Status != EFI_SUCCESS)
  {
    return Status;
  }

  // Store table handle and Update strings if any.
  UpdateTableData(Smbios, ProcessorHandle, EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION );

  Status = Smbios->UpdateString(Smbios,
                                &ProcessorHandle,
                                &ProcessorInfoSocketInfoStringNum,
                                ProcessorInfoSocketInfoString);
  if(Status != EFI_SUCCESS)
  {
    return Status;
  }


  Status = Smbios->UpdateString(Smbios,
                                &ProcessorHandle,
                                &ProcessorManufacturerStringNum,
                                ProcessorManufacturerString);
  if(Status != EFI_SUCCESS)
  {
    return Status;
  }

  Status =	Smbios->UpdateString(Smbios,
                                 &ProcessorHandle,
                                 &ProcessorVersionStringNum,
                                 ProcessorVersionString);
  if(Status != EFI_SUCCESS)
  {
    return Status;
  }

  Status = mChipInfoProtocol->GetChipIdString(mChipInfoProtocol,
                                              (CHAR8*) &ProcessorInfoPartNumberString,
                                              sizeof(ProcessorInfoPartNumberString));
  if(Status != EFI_SUCCESS)
  {
    return Status;
  }

  return Smbios->UpdateString(Smbios,
                              &ProcessorHandle,
                              &ProcessorInfoPartNumberStringNum,
                              ProcessorInfoPartNumberString);
}
