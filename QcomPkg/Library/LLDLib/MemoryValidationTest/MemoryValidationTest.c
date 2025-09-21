/** @file MemoryValidationTest.c

Tests for DDR memory validation.

Copyright (c) 2011-2017, Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                           EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/05/17   akeshan  Initial Creation
01/09/17   akeshan  Added code for error handling
=============================================================================*/

#include <DDITF.h>
#include <Library/TestInterface.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIDDRGetConfig.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/EFIMemoryMap.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>  // for ZeroMem
#include <Library/QcomLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/OfflineCrashDump.h>
//#include "../LLD.h"
 
/*=========================================================================
      Type Definitions
==========================================================================*/

#define  WALKING_PAT_INIT   0x00000001  // Start testing at data line 0

/*======================================================================

  FUNCTION            getMemoryMap

  DESCRIPTION 
     Used to get memory map of DDR where UEFI is running
     so that we can extrapolate the base address of DDR 
     and the end of where UEFI runs
 
  RETURN VALUE
     Returns EFI_STATUS
======================================================================*/
STATIC
EFI_STATUS getMemoryMap(uint64 *base_CS0_0, uint64 *end_CS0_0) {
    EFI_STATUS              Status;
    UINTN                   MemMapSize;
    EFI_MEMORY_DESCRIPTOR   *MemMap = NULL;  
    EFI_MEMORY_DESCRIPTOR   *pMemMap;
    UINTN                   MapKey;
    UINTN                   DescriptorSize;
    UINT32                  DescriptorVersion;
    UINTN                   Index;
    EFI_PHYSICAL_ADDRESS    MemStart, MemEnd = 0;
    
    // First call is to figure out how big the buffer needs to be
    MemMapSize = 0;
    Status = gBS->GetMemoryMap (&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (Status == EFI_BUFFER_TOO_SMALL)
    {
      MemMapSize += EFI_PAGE_SIZE;
      MemMap = AllocatePool(MemMapSize);
      if (MemMap == NULL)
      {
        Status = EFI_OUT_OF_RESOURCES;
        return Status;
      }
      // Now get the memory map
      Status = gBS->GetMemoryMap(&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    }
  
    if ((!EFI_ERROR (Status)) && (MemMap != NULL)) 
    {
        pMemMap = MemMap;
        *base_CS0_0 = pMemMap->PhysicalStart;
        for (Index = 0; Index < MemMapSize/DescriptorSize; Index++)
        {
          MemStart = pMemMap->PhysicalStart;
          MemEnd = pMemMap->PhysicalStart + EFI_PAGES_TO_SIZE(pMemMap->NumberOfPages);
          pMemMap = NEXT_MEMORY_DESCRIPTOR (pMemMap, DescriptorSize);
        }
        *end_CS0_0 = MemEnd;
    }
    if (MemMap != NULL)
        FreePool(MemMap);
    return Status;
}


/*======================================================================

  FUNCTION            getWidthSize()

  DESCRIPTION 
     Used to get the size of the DDR from the worksheet enum 
 
  RETURN VALUE
     Returns size (of DDR) and negative value on failure

  Table:
  0000B: 4Gb dual channel die / 2Gb single channel die
  0001B: 6Gb dual channel die / 3Gb single channel die
  0010B: 8Gb dual channel die / 4Gb single channel die
  0011B: 12Gb dual channel die / 6Gb single channel die
  0100B: 16Gb dual channel die / 8Gb single channel die
  0101B: 24Gb dual channel die / 12Gb single channel die
  0110B: 32Gb dual channel die / 16Gb single channel die

  We return single channel die size for each corresponding value

  Table taken from Samsung_K3UH6H60AM-AGCJ.pdf
======================================================================*/
int getwidthSize(uint32 density) {
    if (density == 0)
        return 2;
    else if (density == 1)
        return 3;
    else if (density == 2)
        return 4;
    else if (density == 3)
        return 6;
    else if (density == 4)
        return 8;
    else if (density == 5)
        return 12;
    else if (density == 6)
        return 16;
    else 
        return -1;
}


/*======================================================================

  FUNCTION            getDDRSize()

  DESCRIPTION 
     Used to get the size of the DDR (4GB or 6GB)
 
  RETURN VALUE
     Returns size (of DDR in GB) and FAILURE status on failure
======================================================================*/

STATIC
EFI_STATUS getDDRSize(uint32 *size) {
    EFI_STATUS                  Status = EFI_NOT_FOUND;
    EFI_DDRGETINFO_PROTOCOL   *pDDRGetInfoProtocol = NULL;
    ddr_details_entry           *pDDREntry = NULL;
    UINT32 ddr_channel = 0, rank = 0;
    
    Status = gBS->LocateProtocol(&gEfiDDRGetInfoProtocolGuid, NULL, (VOID**)&pDDRGetInfoProtocol);
    if (EFI_ERROR(Status) || (&pDDRGetInfoProtocol == NULL))
    {
      AsciiPrint("LocateProtocol function from getDDRSize failed\n");
      return Status;
    }
    
    pDDREntry = AllocatePool (sizeof (ddr_details_entry));
    if (pDDREntry == NULL) {
        AsciiPrint("AllocatePool function from getDDRSize failed\n");
        return Status;
    }
    
    Status = pDDRGetInfoProtocol->GetDDRDetails(pDDRGetInfoProtocol, pDDREntry);
    if (Status != EFI_SUCCESS)
    {
      if (EFI_ERROR (Status))
      {
        AsciiPrint("getDDRDetails function from getDDRSize failed\n");
        return Status;
      }
    }
    
    uint32 numOfChannels = 0;
    for(ddr_channel=0; ddr_channel<DDR_MAX_NUM_CH; ddr_channel++)
    {
      for(rank=0; rank < 2; rank++)
      {
          if (pDDREntry->ddr_params[ddr_channel].density[rank] != 0)
                numOfChannels = numOfChannels + 1;
      }
    } 

    int widthSize = getwidthSize(pDDREntry->ddr_params[0].density[0]);
    if (widthSize < 0) {
        AsciiPrint("getWidthSize function from getDDRSize failed\n");
        Status = TF_FAILURE;
        return Status;
    }
    (*size) = (widthSize * numOfChannels)/8; 
    return Status;
}

/*======================================================================

  FUNCTION            MEMORYTEST

  DESCRIPTION 
     Test address lines to the memory device by performing destructive
     walking-1's-and-0's test. Data pattern is left-shifted for each
     subsequent memory location tested.
 
     Up to 64-bit address is supported. Data width is assumed to be 32-bits.
     Base address is assumed to be a power of 2.
 
     Address rollover is not supported.

  PARAMETERS
     base           [in]: 64-bit memory block starting address
     end            [in]: 64-bit memory block end address
 
  RETURN VALUE
     Returns PASS on success, FAIL and bad address on failure

  DEPENDENCIES
     The EBI1 memory controller must be configured correctly to 
     match the installed RAM device prior to calling this routine.

  SIDE EFFECTS
     Changes the value in a variety of RAM memory locations.

     
   PLEASE NOTE:
     Most of the test from this function has been C/P'd from QBAT RAM_EBI1 test
        File: RAM.c
        Function: MemTestWalk1s0sU64Addr

======================================================================*/

boolean MemoryTest()
{
    uint64 base_CS0_0;
    uint64 end_CS0_0;
    EFI_STATUS Status = getMemoryMap(&base_CS0_0, &end_CS0_0);
    if (Status != EFI_SUCCESS) {
        AsciiPrint("getMemoryMap function from MemoryTest failed\n");
        return FALSE;
    }
    AsciiPrint("got memory map successfully");


   uint32 size;
   Status = getDDRSize(&size);
   if (Status != EFI_SUCCESS) {
        AsciiPrint("getDDRSize function from MemoryTest failed\n");
        return FALSE;
   }
   AsciiPrint("size of DDR is %d\n", size);


   size = size<<30; //converting into GB
   uint64 base = end_CS0_0 + 0x1;
   uint64 end = base_CS0_0 + (size - (uint64)1);
   Print(L"the size is %lx \n", size);
   Print(L"the base is %lx \n", base);
   Print(L"the end is %lx \n", end);
   uint64  addrPattern = 211; // Ignore A[1:0]
   uint32  dataPattern = WALKING_PAT_INIT; 
   uint64  testAddr = base;
   uint32  readVal;
   boolean dataPatternRolled = FALSE;   

   /* 1st sweep across address range, storing test data */
   do
   {
       if (dataPatternRolled) {
            *(volatile uint64 *) testAddr = ~dataPattern; //write into memory
       } else {
            *(volatile uint64 *) testAddr = dataPattern; //write into memory
       }

      /* Advance to next test address */
      addrPattern = addrPattern << 1;
      testAddr = base + addrPattern;

      /* Derive next test data value */
      dataPattern = dataPattern << 1;
      if (!dataPattern)
      {
         /* Data pattern rolled over */
         dataPattern = WALKING_PAT_INIT;
         dataPatternRolled = TRUE;
      }
   } while (testAddr < end);
   
   /* 2nd sweep back across address range, checking for corrupted or missing data */
   addrPattern = 211; // Ignore A[1:0]
   dataPattern = WALKING_PAT_INIT; 
   testAddr = base;
   dataPatternRolled = FALSE;
   
   do
   {
      /* Reading data pattern from memory */
      uint64* address = (uint64*) testAddr;
      readVal = *address; 
      
      /* Validate read value */
      if (readVal != (dataPatternRolled ? ~dataPattern : dataPattern))
      {
         /* Stored value doesn't match what was written */
         AsciiPrint("value doesn't match; Memory Validation FAILURE!!!\n");
         return FALSE;
      }

      /* Advance to next test address */
      addrPattern = addrPattern << 1;
      testAddr = base + addrPattern;

      /* Derive next test data value */
      dataPattern = dataPattern << 1;
      if (!dataPattern)
      {
         /* Data pattern rolled over */
         dataPattern = WALKING_PAT_INIT;
         dataPatternRolled = TRUE;
      }
   } while (testAddr < end);

   /* All stored values match what was written */
   AsciiPrint("values match; Memory Validation SUCCESS!!!\n");
   return TRUE;

} // end MemoryTest()

/*======================================================================

  FUNCTION            MemoryValidationTestMain

  DESCRIPTION 
     Used to validate the DDR by writing walking 1's to addresses 
     and then reading from them to make sure that memory isn't corrupted
 
  RETURN VALUE
     Returns EFI_STATUS
======================================================================*/
UINT32 MemoryValidationTestMain(uint32 dwParam, char *apszParam[]) {
    EFI_STATUS Status = TF_FAILURE;
    boolean memoryValidated;

    memoryValidated = MemoryTest();

    if (memoryValidated) {
        Status = EFI_SUCCESS;
    }
    return Status;
}

