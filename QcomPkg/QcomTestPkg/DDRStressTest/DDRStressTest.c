/** @file DDRStressTest.c
   
  Application to test DDR in UEFI env.
  
#  Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
   
**/
/*
 * Portions Copyright (c) 2008-2014 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*=============================================================================
                              EDIT HISTORY


  when         who     what, where, why
  ----------   ---     -----------------------------------------------------------
  11/01/2018   dais    Initial version
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/EFIClock.h>
#include <Library/KernelLib.h>
#include <Include/scm_sip_interface.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/LockLib.h>
#include <Protocol/EFIDisplayPwrCtrl.h>
#include <Protocol/EFIDDRGetConfig.h>
#include <Protocol/EFIQcomEnv.h>
#include <DDRStressTest.h>

EFI_DDRGETINFO_PROTOCOL   *pDDRGetInfoProtocol = NULL;

/* ============================================================================
**  Function : EnableAuxCores
** ============================================================================
*/
/*!
*   @brief
*   This function is used to enable the cores.
*
*   @param
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

VOID EnableAuxCores (UINT32 CoreMask)
{
  if (MpcoreGetAvailCpuCount() < MpcoreGetMaxCpuCount())
    MpcoreInitDeferredCores (CoreMask);
  ThreadSleep (10);
}

/* ============================================================================
**  Function : GetMaxFreeMem
** ============================================================================
*/
/*!
*   @brief
*   This function is called to get maximum available free contiguous memory chunk
*
*   @param
*   FreeSize - number of contiguous free 4K pages.
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  Start address of maximum contiguous memory block.
*
*/

UINTN * GetMaxFreeMem (UINT64 *FreeSize)
{
  EFI_STATUS            Status;
  EFI_MEMORY_DESCRIPTOR *MemMap = NULL;
  EFI_MEMORY_DESCRIPTOR *OrigMemMap = 0;
  UINTN                 MemMapSize = 0;
  UINTN                 MapKey, MaxFree = 0;
  UINTN                 DescriptorSize;
  UINT32                DescriptorVersion;
  UINTN                 i;
  UINT64                TotalPages = 0;
  UINTN * MaxFreeBase = 0;

  // First call is to figure out how big the buffer needs to be
  Status = gBS->GetMemoryMap (&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (Status == EFI_BUFFER_TOO_SMALL)
  {
    // In case the AllocatPool changes the memory map we added in some extra descriptors
    MemMapSize += (DescriptorSize * 0x100);
    OrigMemMap = MemMap = AllocatePool (MemMapSize);
  }

  if (OrigMemMap != NULL) {
    Status = gBS->GetMemoryMap (&MemMapSize, MemMap, &MapKey, &DescriptorSize, &DescriptorVersion);

    if (!EFI_ERROR (Status)) {
      for (i = 0; i < MemMapSize/DescriptorSize; i++) {
        if (MemMap->Type == EfiConventionalMemory)
        {
          if (MaxFree < MemMap->NumberOfPages)
          {
            MaxFree = MemMap->NumberOfPages;
            MaxFreeBase = (UINTN*)MemMap->PhysicalStart;
          }
        }
        TotalPages += MemMap->NumberOfPages;
        MemMap = NEXT_MEMORY_DESCRIPTOR (MemMap, DescriptorSize);
      }
   }

    FreePool ( OrigMemMap );

  }

  *FreeSize = MaxFree;
  return MaxFreeBase;
}


/* ============================================================================
**  Function : free_memory
** ============================================================================
*/
/*!
*   @brief
*   This function is called to free allocated maximum available contiguous free memory block
*
*   @param
*   AllocBase - Address of allocated memory block to be freed
*   FreeSize  - Number of allocated pages to be freed.
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

void free_memory (UINT64* AllocBase,UINT64 FreeSize)
{
#ifdef USE_CACHED_BUFFER
  gBS->FreePages ((EFI_PHYSICAL_ADDRESS)AllocBase, FreeSize);
#else
  UncachedFreePages (AllocBase, FreeSize);
#endif
}

/* ============================================================================
**  Function : get_available_memory
** ============================================================================
*/
/*!
*   @brief
*   This function is called to allocate maximum available contiguous free memory block
*
*   @param
*   FreeSize - number of allocated contiguous free 4K pages.
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  Return address of allocated contiguous memory 
*
*/

UINT64* get_available_memory (UINT64 *FreeSize)
{
  UINTN * FreeMemBase = NULL;
  UINT64 *AllocBase = 0;

  FreeMemBase = GetMaxFreeMem (FreeSize);

#ifdef USE_CACHED_BUFFER
  AllocBase = FreeMemBase;
  gBS->AllocatePages (AllocateAddress, EfiBootServicesData, FreeSize, (EFI_PHYSICAL_ADDRESS *)&AllocBase);
#else
  AllocBase = UncachedAllocatePages (*FreeSize);
#endif

  AsciiPrint("AllocBase = %lX,FreeSize = %lX\n",AllocBase,(*FreeSize)*4096);

  if (AllocBase == 0)
  {
    AsciiPrint ("Failed memory alloc \n");
    return NULL;
  }

  return AllocBase;
}

void print_usage()
{
    AsciiPrint("\nUsage : st DDRStressTest \"aux 0xff fidx <freq_index> t <time_in sec> c <core_numbers separated by space>\"\n\n");
    AsciiPrint("Example : st DDRStressTest \"aux 0xff fidx 2 t 9 c 4 5 6 7\"  This executes tests on core4,core5,core6,core7 \nfor duration of 9 seconds without clock scaling\n\n");
    AsciiPrint("aux : power up aux cores, mask is 1 char hex of bitmask of CPU's to enable\n\n");
    AsciiPrint("fidx 2 to fix DDR frequency at 200 MHz \n\n");
    AsciiPrint("t 30 to run tests for 30 seconds \n\n");
    AsciiPrint("c 4 5 6 7 to run tests on core 4 , core 5, core 6, core 7 \n\n");
    AsciiPrint("if fidx argument is not given then clock scaling is enabled by default \n\n");
    AsciiPrint("if t argument is not given then the tests are run until completion \n\n");
    AsciiPrint("if c argument is not given then the tests are run on all cores \n\n");
}


/**
  Entry point for DDRStressTest App

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS           Erase successfully.
  @retval EFI_DEVICE_ERROR      Failed to program
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate memory for 
                                the buffers
**/


EFI_STATUS
EFIAPI
DDRStressTestMain ( IN EFI_HANDLE         ImageHandle,
             IN EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINTN                       Argc, i=0;
  CHAR8                     **Argv;
  UINT64 time_sec = 0;
  UINT8 all_cores=1, clock_index=0, j=0;
  UINT8 core_numbers[8];

  Status = GetCmdLineArgs (ImageHandle, &Argc, &Argv);

  if (EFI_ERROR (Status))
    return Status;

  /* Check if the app was launched by shell and the Cmd str is actually the
   * Cmd app name itself, ie it would be "cmd" or "cmd.efi", then again process
   * to split, this is because Ebl doesn't pass the name of app, but shell does
   * other methods could yeild mixed results (like BDS launching apps via Menu, etc) */
  if (Argc &&
      ((AsciiStriCmp (Argv[0], "DDRStressTest") == 0) ||
       (AsciiStriCmp (Argv[0], "DDRStressTest.efi") == 0)))
  {
    --Argc;
    ++Argv;
  }
  
  /*  Usage : start fvx:ddrstresstest "[aux msk ] [ddrtest x... ]"
   *  commands :
   *       aux : power up aux cores, mask is 1 char hex of bitmask of CPU's to enable
   *       ddrtest : ddr read write test
   *             params : cpu num
   *                      eg: ddrtest 0 1 2 3 etc
   *       
   *  Commands are executed in the given order                              
   *
   *  eg:
   *    Ebl>start fv3:\ddrstresstest "aux 0xff ddrtest 0 1 2 3"
   */
   if (Argc <= 0)
   {
      print_usage();
      return Status;
   }

  for (; i < Argc; )
  {
    if (AsciiStriCmp (Argv[i], "-h") == 0)
    {
      ++i;
      print_usage();
      return Status;
    }

    if (AsciiStriCmp (Argv[i], "aux") == 0)
    {
      UINTN Mask = 0xFF;
      ++i;

      if (i < Argc)
      {
        Mask = AsciiStrHexToUintn (Argv[i]);
        ++i;
      }

      EnableAuxCores (Mask);
      continue;
    }


    if (AsciiStriCmp (Argv[i], "t") == 0)
    {
      ++i;
      time_sec = (UINT64)AsciiStrDecimalToUintn (Argv[i]);
      ++i;
      continue;
    }
	
	if (AsciiStriCmp (Argv[i], "fidx") == 0)
    {
      ++i;
      clock_index = (UINT8)AsciiStrDecimalToUintn (Argv[i]);
      ++i;
      continue;
    }
	
    if (AsciiStriCmp (Argv[i], "c") == 0)
    {
      ++i;
      all_cores = 0;
      for(j=0; j<8; j++)
      {
        core_numbers[j] = (UINT8)AsciiStrDecimalToUintn (Argv[i]);
        ++i;
        if( i == Argc)
          break;
        if (i < Argc)
        {
          if ((AsciiStriCmp (Argv[i], "fidx") == 0) || (AsciiStriCmp (Argv[i], "t") == 0))
          break;
        }
      }
      continue;
    }  
    continue;
  }

  if (all_cores == 1)
  RunDDRStressTest (0, &core_numbers[0], time_sec, clock_index);
  else
  RunDDRStressTest (j+1, &core_numbers[0], time_sec, clock_index);

  AsciiPrint ("\r\nPress any key to exit...\r\n");
  ReadAnyKey (NULL, 0);

  AsciiPrint ("\r\nExiting...\r\n");
  
  return Status;
}

/*==============================================================================

  FUNCTION      ddr_setlowbandwidth

  DESCRIPTION   Function to set low bandwidth for DDR

==============================================================================*/

static EFI_STATUS ddr_setlowbandwidth(void)
{
  EFI_STATUS Status;
  EFI_QCOMENV_PROTOCOL  *EnvProtocol = NULL;    
    
  Status = gBS->LocateProtocol(&gEfiQcomEnvProtocolGuid, NULL, (void**)&EnvProtocol);
  if(Status == EFI_SUCCESS)
  {
    if (EFI_SUCCESS != (Status = EnvProtocol->SetDdrThroughputLow(EnvProtocol)))
    {
        AsciiPrint("SetDdrThroughLow Failed\n");
    }
    else
    {
        AsciiPrint("SetDdrThroughLow Success\n");
    }
  } 
  return Status; 
}

/* ============================================================================
**  Function : TurnOnDisplayTest
** ============================================================================
*/
/*!
*   @brief
*   This function is used to switch on Display
*
*   @param
*   None

*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

VOID
TurnOnDisplayTest (VOID)
{
  EFI_STATUS  Status;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL *gDisplayPwrCtrlProtocol = NULL;

  Status = gBS->LocateProtocol(&gQcomDisplayPwrCtrlProtocolGuid,
                               NULL,
                               (VOID**) &gDisplayPwrCtrlProtocol);

  if ((Status != EFI_SUCCESS) || (gDisplayPwrCtrlProtocol == NULL))
  {
    DEBUG ((EFI_D_WARN, "Display Power Control protocol not available\r\n"));
    return;
  }

  Status = gDisplayPwrCtrlProtocol->DisplayPanelPowerControl(EFI_DISPLAY_TYPE_PRIMARY, EFI_DISPLAY_POWER_STATE_ON);

  if (Status != EFI_SUCCESS)
    AsciiPrint("Display turn ON Failed\r\n");
  else
    AsciiPrint("Successfully turned ON display\r\n");
}

/* ============================================================================
**  Function : TurnOffDisplayTest
** ============================================================================
*/
/*!
*   @brief
*   This function is used to switch off Display
*
*   @param
*   None

*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

VOID
TurnOffDisplayTest (VOID)
{
  EFI_STATUS Status;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL *gDisplayPwrCtrlProtocol = NULL;

  Status = gBS->LocateProtocol(&gQcomDisplayPwrCtrlProtocolGuid,
                               NULL,
                               (VOID**) &gDisplayPwrCtrlProtocol);

  if ((Status != EFI_SUCCESS) || (gDisplayPwrCtrlProtocol == NULL))
  {
    AsciiPrint("Display Power Control protocol not available\r\n");
    return;
  }

  Status = gDisplayPwrCtrlProtocol->DisplayPanelPowerControl(EFI_DISPLAY_TYPE_PRIMARY, EFI_DISPLAY_POWER_STATE_OFF);

  if (Status != EFI_SUCCESS)
    AsciiPrint("Display turn OFF Failed\r\n");
  else
    AsciiPrint("Successfully turned OFF display\r\n");
}


/* ============================================================================
**  Function : DDRStressTestThr
** ============================================================================
*/
/*!
*   @brief
*   This function is used as a thread argument to launch DDR tests
*
*   @param
*   CpuCoreNum      - Core number to launch the thread
*   mem_descriptor  - Descriptor containing start address and size
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/
int
DDRStressTestThr (IN VOID  *Arg)
{
  UINT32 RetVal=0;
  UINTN cp;
  UINT64 start, end;
  cp = MpcoreGetCurrCpu ();
  
  start =GetTimerCountms ();
  memory_test(Arg);
  end = GetTimerCountms ();
  AsciiPrint ("Core%d - Time taken to complete test %d seconds\n", cp, (end - start)/1000);
 
  return RetVal;
}

/* ============================================================================
**  Function : LaunchDDRStressTestThread
** ============================================================================
*/
/*!
*   @brief
*   This function is used create a thread and launch the test
*
*   @param
*   CpuCoreNum      - Core number to launch the thread
*   mem_descriptor  - Descriptor containing start address and size
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

Thread* 
LaunchDDRStressTestThread (UINT32 CpuCoreNum, struct ddr_mem_descriptor *mem_descriptor)
{
  CHAR8 TName[16];
  EFI_STATUS    Status = EFI_NOT_FOUND;
  UINT32 ddr_freq = 0;
  Thread *Pct;

  AsciiSPrint (TName, sizeof(TName), "DDRTest%d", CpuCoreNum);
  Status = pDDRGetInfoProtocol->GetDDRFreq(pDDRGetInfoProtocol, &ddr_freq);
  if (Status != EFI_SUCCESS)
  {
    if (EFI_ERROR (Status))
    {
      DEBUG((EFI_D_WARN, "Failed to get DDR Current Freq"));
    }
  }
  
  Pct = ThreadCreate (TName, &DDRStressTestThr, (void*)mem_descriptor, HIGHEST_PRIORITY - 1, DEFAULT_STACK_SIZE);

  if (Pct == NULL)
  {
    DEBUG (( EFI_D_ERROR, "Unable to create thread on %d, exiting ...\n", CpuCoreNum));
    return NULL;
  }

  ThreadSetPinnedCpu (Pct, CpuCoreNum);
  ThreadResume (Pct);
  
  return Pct;
}

/* ============================================================================
**  Function : RunDDRStressTest
** ============================================================================
*/
/*!
*   @brief
*   This function is used to run DDR write/read tests and launch it in multiple cores.
*
*   @param
*
*   @par Dependencies
*
*   @par Side Effects
*   None
*
*   @retval  
*   None
*/

EFI_STATUS RunDDRStressTest (UINTN Argc, UINT8* core_numbers, UINT64 time_sec, UINT8 clock_index)
{
  int i=0;
  Thread* Thr[8];
  UINT32 ActiveMask;
  UINTN core_idx;
  UINT64 FreeSize, WriteSize, WriteAddress;
  UINT64 *AllocBase, *FreeBase;
  INT64 Num_Cpu;
  struct ddr_mem_descriptor *mem_descriptor=NULL,*temp_descriptor=NULL;
  
  if(Argc == 0)
   Num_Cpu = 8;
  else
   Num_Cpu = (INT64)Argc ;
  UINT8 Core0_enabled=0;
  EFI_STATUS                  Status = EFI_NOT_FOUND;
  ddr_freq_plan_entry         *pDDRFreqTbl = NULL;
  UINT32 set_ddr_freq = 0, freq_index=0, ddr_freq=0;
  
  Status = gBS->LocateProtocol(&gEfiDDRGetInfoProtocolGuid, NULL, (VOID**)&pDDRGetInfoProtocol);
  if (EFI_ERROR(Status) || (&pDDRGetInfoProtocol == NULL))
  {
    DEBUG((EFI_D_ERROR, "Locate EFI_DDRGETINFO_PROTOCOL failed, Status =  (0x%x)\r\n", Status));
    return EFI_NOT_FOUND;
  }
  pDDRFreqTbl = AllocatePool (sizeof (ddr_freq_plan_entry));
  if (pDDRFreqTbl == NULL)
      return EFI_OUT_OF_RESOURCES;
  
  Status = pDDRGetInfoProtocol->GetDDRFreqTable(pDDRGetInfoProtocol, pDDRFreqTbl);
  if (Status != EFI_SUCCESS)
  {
    if (EFI_ERROR (Status))
    {
      DEBUG((EFI_D_WARN, "Failed to get DDR Freq Table"));
    }
    return Status;
  }
  
  SetMem (Thr, sizeof(Thr), 0);
  
  if(Num_Cpu <= 0)
  {
    Num_Cpu = 8;
  }
  mem_descriptor = AllocatePool(sizeof(struct ddr_mem_descriptor) * Num_Cpu);
  if(mem_descriptor == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }
  temp_descriptor = mem_descriptor;
  
  AllocBase = get_available_memory(&FreeSize);
  if(AllocBase == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }
  FreeBase = AllocBase;
  WriteAddress = (UINT64)AllocBase;
  WriteSize = (FreeSize * 4096) / Num_Cpu;

  for(i=0; i<Num_Cpu; i++)
  {
      temp_descriptor->AllocBase = AllocBase;
      temp_descriptor->WriteSize = WriteSize;
      temp_descriptor->time_ms = time_sec*1000;
      WriteAddress += WriteSize;
      AllocBase = (UINT64*)WriteAddress;
      temp_descriptor++ ;
  }  
  
  ActiveMask = MpcoreGetActiveMask ();
  ddr_setlowbandwidth();
  TurnOffDisplayTest ();
  if(clock_index != 0)
  {
      freq_index = clock_index;
      if(freq_index < 2 || freq_index >= pDDRFreqTbl->num_ddr_freqs)
      {
         AsciiPrint ("Please give frequency index between 2 and %d\n", pDDRFreqTbl->num_ddr_freqs-1);
         free_memory(FreeBase,FreeSize);
         return EFI_INVALID_PARAMETER;
      }
  }
  if(Argc == 0)
  {
    for (i=7; i>=0; i--)
    {
      if (ActiveMask & (1 << i))
      {
        if(clock_index == 0)  
           freq_index = pDDRFreqTbl->num_ddr_freqs - i - 1;
        set_ddr_freq = pDDRFreqTbl->ddr_freq[freq_index].freq_khz;
        Status = pDDRGetInfoProtocol->SetDDRFreq(pDDRGetInfoProtocol, set_ddr_freq);
        if (Status != EFI_SUCCESS)
        {
            if (EFI_ERROR (Status))
            {
               DEBUG((EFI_D_WARN, "Failed to set DDR Freq"));
            }
        }  
        Status = pDDRGetInfoProtocol->GetDDRFreq(pDDRGetInfoProtocol, &ddr_freq);
        if (Status != EFI_SUCCESS)
        {
            if (EFI_ERROR (Status))
            {
               DEBUG((EFI_D_WARN, "Failed to get DDR Current Freq"));
            }
            free_memory(FreeBase,FreeSize);
            return Status;
        }
        while(1)
        {
            /*DDR frequency read is in the range of set DDR frequency +5 or -5 */
            if( (ddr_freq <= ((set_ddr_freq/1000)+5)) && (ddr_freq >= ((set_ddr_freq/1000)-5)) )
              break;
            ddr_freq = 0;
            Status = pDDRGetInfoProtocol->GetDDRFreq(pDDRGetInfoProtocol, &ddr_freq);
            if (Status != EFI_SUCCESS)
            {
               if (EFI_ERROR (Status))
               {
                   DEBUG((EFI_D_WARN, "Failed to get DDR Current Freq"));
               }
               free_memory(FreeBase,FreeSize);
               return Status;
            }
        }
        if(clock_index != 0 && i==7) 
           AsciiPrint ("Core%d - DDR frequency switched to %d MHz\n", MpcoreGetCurrCpu(), ddr_freq);  
        if(clock_index == 0) 
        {
            if(mem_descriptor->time_ms !=0 )
                AsciiPrint ("Core%d - DDR frequency switched to %d MHz, Core%d - DDR tests running from DDR Start Address %lx to DDR End Address %lx for %d seconds\n", MpcoreGetCurrCpu(), ddr_freq, i, mem_descriptor->AllocBase, ((UINT64)mem_descriptor->AllocBase) + mem_descriptor->WriteSize - 4, mem_descriptor->time_ms/1000);
            else
                AsciiPrint ("Core%d - DDR frequency switched to %d MHz, Core%d - DDR tests running from DDR Start Address %lx to DDR End Address %lx \n", MpcoreGetCurrCpu(), ddr_freq, i, mem_descriptor->AllocBase, ((UINT64)mem_descriptor->AllocBase) + mem_descriptor->WriteSize - 4);
        }
        else
        {
            if(mem_descriptor->time_ms !=0 )
                AsciiPrint ("Core%d - DDR tests running from DDR Start Address %lx to DDR End Address %lx for %d seconds\n", i, mem_descriptor->AllocBase, ((UINT64)mem_descriptor->AllocBase) + mem_descriptor->WriteSize - 4, mem_descriptor->time_ms/1000);
            else
                AsciiPrint ("Core%d - DDR tests running from DDR Start Address %lx to DDR End Address %lx \n", i, mem_descriptor->AllocBase, ((UINT64)mem_descriptor->AllocBase) + mem_descriptor->WriteSize - 4);
        }
        Thr[i] = LaunchDDRStressTestThread (i, mem_descriptor);
        mem_descriptor++;
      }
    } 
  }
  else
  {
    for (i = 0; i < Argc;)
    {
      if (i < Argc)
      {
        core_idx = (core_numbers[i]);
        ++i;
      }
      if (core_idx == 0)
      {
        Core0_enabled = 1;
        continue;
      }
      if (core_idx >= 8)
        AsciiPrint ("Wrong CPU number %d\n", core_idx);

      if (ActiveMask & (1 << core_idx))
      {
        if(clock_index == 0)  
           freq_index = pDDRFreqTbl->num_ddr_freqs - i - 1;
        set_ddr_freq = pDDRFreqTbl->ddr_freq[freq_index].freq_khz;
        Status = pDDRGetInfoProtocol->SetDDRFreq(pDDRGetInfoProtocol, set_ddr_freq);
        if (Status != EFI_SUCCESS)
        {
            if (EFI_ERROR (Status))
            {
               DEBUG((EFI_D_WARN, "Failed to set DDR Freq"));
            }
        }
        Status = pDDRGetInfoProtocol->GetDDRFreq(pDDRGetInfoProtocol, &ddr_freq);
        if (Status != EFI_SUCCESS)
        {
            if (EFI_ERROR (Status))
            {
               DEBUG((EFI_D_WARN, "Failed to get DDR Current Freq"));
            }
            free_memory(FreeBase,FreeSize);
            return Status;
        } 
        while(1)
        {
            /*DDR frequency read is in the range of set DDR frequency +5 or -5 */
            if( (ddr_freq <= ((set_ddr_freq/1000)+5)) && (ddr_freq >= ((set_ddr_freq/1000)-5)) )
              break;
            ddr_freq = 0;
            Status = pDDRGetInfoProtocol->GetDDRFreq(pDDRGetInfoProtocol, &ddr_freq);
            if (Status != EFI_SUCCESS)
            {
               if (EFI_ERROR (Status))
               {
                   DEBUG((EFI_D_WARN, "Failed to get DDR Current Freq"));
               }
               free_memory(FreeBase,FreeSize);
               return Status;
            }
        }
        if(clock_index != 0 && i==1) 
           AsciiPrint ("Core%d - DDR frequency switched to %d MHz\n", MpcoreGetCurrCpu(), ddr_freq); 
        if(clock_index == 0) 
        {
            if(mem_descriptor->time_ms !=0 )
                AsciiPrint ("Core%d - DDR frequency switched to %d MHz, Core%d - DDR tests running from DDR Start Address %lx to DDR End Address %lx for %d seconds\n", MpcoreGetCurrCpu(), ddr_freq, core_idx, mem_descriptor->AllocBase, ((UINT64)mem_descriptor->AllocBase) + mem_descriptor->WriteSize - 4, mem_descriptor->time_ms/1000);
            else
                AsciiPrint ("Core%d - DDR frequency switched to %d MHz, Core%d - DDR tests running from DDR Start Address %lx to DDR End Address %lx \n", MpcoreGetCurrCpu(), ddr_freq, core_idx, mem_descriptor->AllocBase, ((UINT64)mem_descriptor->AllocBase) + mem_descriptor->WriteSize - 4);
        }
        else
        {
            if(mem_descriptor->time_ms !=0 )
                AsciiPrint ("Core%d - DDR tests running from DDR Start Address %lx to DDR End Address %lx for %d seconds\n", core_idx, mem_descriptor->AllocBase, ((UINT64)mem_descriptor->AllocBase) + mem_descriptor->WriteSize - 4, mem_descriptor->time_ms/1000);
            else
                AsciiPrint ("Core%d - DDR tests running from DDR Start Address %lx to DDR End Address %lx \n", core_idx, mem_descriptor->AllocBase, ((UINT64)mem_descriptor->AllocBase) + mem_descriptor->WriteSize - 4);
        }
        Thr[core_idx] = LaunchDDRStressTestThread (core_idx, mem_descriptor);
        mem_descriptor++;
      }
    }
    if(Core0_enabled == 1)
    {
      if (ActiveMask & (1 << 0))
      {
        Thr[0] = LaunchDDRStressTestThread (0, mem_descriptor);
      }
    }
  } 
  
  for (i=0; i<8; ++i)
  {
    INT32  RetCode;

    if (Thr[i])
      ThreadJoin (Thr[i], &RetCode, INFINITE_TIME);
  }

  free_memory(FreeBase,FreeSize);

  TurnOnDisplayTest ();
  return EFI_SUCCESS;
}




