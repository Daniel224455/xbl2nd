/**@file DbiDumpLib.c

  Copyright (c) 2016-2019,2021 Qualcomm Technologies Inc. All rights reserved.<BR>

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 
 --------   ---     -----------------------------------------------------------
 06/06/21   nk      Clear the upper 32-bit of DbiDumpTable address
 02/19/19   vb      fix CR2396170 : apreg info missing in hyper-v enabled builds
 06/13/16   vk      Initial version
=============================================================================*/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ArmLib.h>
#include <Library/IoLib.h>
#include <Protocol/DebugSupport.h>

#include <Include/UefiInfoBlk.h>
#include <Library/SerialPortShLib.h>
#include <Library/UefiCfgLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/DbiDumpLib.h>
#include <Library/CookieLib.h>
#include <Library/WatchdogLib.h>
#include <Library/UefiCfgLib.h>
#include <Include/scm_sip_interface.h>
#include <Include/scm_qsee_interface.h>
#include <Protocol/EFIShmBridge.h>

VOID
SetupDBIDumpTable ()
{
   EFI_STATUS Status = EFI_SUCCESS;
   UINTN WriteAddr;
   UINT32 WriteValue;
   UINT32 i;
   UINT32 running_index = 0;
   UINT32 NumCores = 0;
   UINT32 SkipDBISetup = 0;
   UINT32 SharedIMEMBaseAddr = 0;
   UINT64 MemoryBase;
   MemRegionInfo DBIDumpRegionInfo;
   EFI_SHMBRIDGE_PROTOCOL *ShmBridgeProtocol;
   UINTN uHandle;
   MapAddrInfo MapInfo;
   DstVMPermInfo VMInfo[1];
   
   Status = GetMemRegionInfoByName("DBI Dump", &DBIDumpRegionInfo);
   if (Status != EFI_SUCCESS)
   {
      DEBUG((EFI_D_ERROR, " Could not find the DBI Dump region .\r\n"));
      return;
   }
   if (DBIDumpRegionInfo.MemBase == 0 || DBIDumpRegionInfo.MemSize == 0) 
   {
      DEBUG((EFI_D_ERROR, " DBI Dump region is invalid .\r\n"));
      return;
   }
   MemoryBase = DBIDumpRegionInfo.MemBase;
   
   /* Zero-out DBI Dump Region. Commented out due to the extra time it takes. */
   //memset((void *)DBIDumpRegionInfo.MemBase, 0x0, DBIDumpRegionInfo.MemSize); 
   
   DUMP_TABLE_TYPE *DbiDumpTable = (DUMP_TABLE_TYPE*)(MemoryBase + DBI_DUMP_TABLE_OFFSET);
   DUMP_TABLE_TYPE *AppDumpTable = (DUMP_TABLE_TYPE*)(MemoryBase + DBI_DUMP_TABLE_OFFSET + sizeof(DUMP_TABLE_TYPE));
   DUMP_DATA_TYPE_TABLE *DumpDataTypeTable = (DUMP_DATA_TYPE_TABLE*)(MemoryBase + DBI_DUMP_TABLE_OFFSET + 2*sizeof(DUMP_TABLE_TYPE));
   
   if (DbiDumpTable == NULL)
   {
      DEBUG ((EFI_D_ERROR, "ERROR: DBI Dump table not setup\r\n"));
      return;
   }
   
   Status = GetConfigValue("SkipDBISetup", (UINT32*)&SkipDBISetup);
   if ((Status == EFI_SUCCESS) && (SkipDBISetup == 1)) {
      return;
   }
   
   Status = GetConfigValue("NumCpus", (UINT32*)&NumCores);
   if (Status != EFI_SUCCESS)
   {
      DEBUG(( EFI_D_ERROR, "Could not NumCpus uefiplat.cfg Defaulting to 8 cores , Status = (0x%x)\r\n", Status));
      NumCores = 0x08;
   }
   /* Fill out the top level DbiDumpTable version and entries*/
   DbiDumpTable->Version    =    DBI_DUMP_TABLE_VERSION;
   DbiDumpTable->NumEntries =    0x1;
   
   /* Fill out the second level APP dump table */
   AppDumpTable->Version     =   DBI_DUMP_TABLE_VERSION;
   AppDumpTable->NumEntries  =   NUM_OF_APPS_DUMP_ENTRY;
   
   DbiDumpTable->Entries[0].id         = MSM_DUMP_CLIENT_APPS;
   DbiDumpTable->Entries[0].type       = MSM_DUMP_TYPE_TABLE;
   DbiDumpTable->Entries[0].start_addr = (UINT64)AppDumpTable; 
   
   DEBUG ((EFI_D_INFO, "NumCores : %d \n", NumCores));
   
   /* Populate CPU REG Dump entries */
   for (i=0; i < NumCores ; i++)
   {
      DumpDataTypeTable->DumpDataType[i].header.version = 0x0;
      DumpDataTypeTable->DumpDataType[i].header.magic   = 0x0;
      DumpDataTypeTable->DumpDataType[i].start_addr     = MemoryBase + CPU_REG_DUMP_START_OFFSET + i * SDI_DUMP_CORE_AP_REG_SIZE;
      DumpDataTypeTable->DumpDataType[i].len            = SDI_DUMP_CORE_AP_REG_SIZE;
      AppDumpTable->Entries[i].id                       = MSM_CPU_REGS_DUMP + i;
      AppDumpTable->Entries[i].type                     = MSM_DUMP_TYPE_DATA;
      AppDumpTable->Entries[i].start_addr               = (UINT64)(&DumpDataTypeTable->DumpDataType[i]); 
   }
   running_index += NumCores;

   /* Fill in ETB entry */
   DumpDataTypeTable->DumpDataType[running_index].header.version = 0x0;
   DumpDataTypeTable->DumpDataType[running_index].header.magic   = 0x0;
   DumpDataTypeTable->DumpDataType[running_index].start_addr     = MemoryBase + CPU_REG_DUMP_END_OFFSET;
   DumpDataTypeTable->DumpDataType[running_index].len            = SDI_DUMP_ETB_DUMP_SIZE;
   AppDumpTable->Entries[running_index].id                       = MSM_ETB_DUMP;
   AppDumpTable->Entries[running_index].type                     = MSM_DUMP_TYPE_DATA;
   AppDumpTable->Entries[running_index].start_addr               = (UINT64)(&DumpDataTypeTable->DumpDataType[running_index]);
   running_index += 1;

   /* Fill in TMC entries */
   for (i=0; i < NUMBER_OF_TMC_DUMP_ENTRIES ; i++)
   {
      DumpDataTypeTable->DumpDataType[running_index + i].header.version = 0x0;
      DumpDataTypeTable->DumpDataType[running_index + i].header.magic   = 0x0;
      DumpDataTypeTable->DumpDataType[running_index + i].start_addr     = MemoryBase + CPU_TMC_DUMP_START_OFFSET + i * SDI_DUMP_TMC_DUMP_SIZE;
      DumpDataTypeTable->DumpDataType[running_index + i].len            = SDI_DUMP_TMC_DUMP_SIZE;
      AppDumpTable->Entries[running_index + i].id                       = MSM_TMC_DUMP + i;
      AppDumpTable->Entries[running_index + i].type                     = MSM_DUMP_TYPE_DATA;
      AppDumpTable->Entries[running_index + i].start_addr               = (UINT64)(&DumpDataTypeTable->DumpDataType[running_index + i]); 
   }
   running_index += NUMBER_OF_TMC_DUMP_ENTRIES;
  
   /* Fill in RPMh CODE RAM DUMP entry */
   DumpDataTypeTable->DumpDataType[running_index].header.version = 0x0;
   DumpDataTypeTable->DumpDataType[running_index].header.magic   = 0x0;
   DumpDataTypeTable->DumpDataType[running_index].start_addr     = MemoryBase + RPMH_CODE_RAM_DUMP_START_OFFSET;
   DumpDataTypeTable->DumpDataType[running_index].len            = SDI_RPMH_CODE_RAM_DUMP_SIZE;
   AppDumpTable->Entries[running_index].id                       = MSM_DATA_RPMhDUMP;
   AppDumpTable->Entries[running_index].type                     = MSM_DUMP_TYPE_DATA;
   AppDumpTable->Entries[running_index].start_addr               = (UINT64)(&DumpDataTypeTable->DumpDataType[running_index]);
   running_index += 1;

   /* Fill in PMIC dump entry */
   DumpDataTypeTable->DumpDataType[running_index].header.version = 0x0;
   DumpDataTypeTable->DumpDataType[running_index].header.magic   = 0x0;
   DumpDataTypeTable->DumpDataType[running_index].start_addr     = MemoryBase + PMIC_DUMP_START_OFFSET;
   DumpDataTypeTable->DumpDataType[running_index].len            = SDI_PMIC_DUMP_SIZE;
   AppDumpTable->Entries[running_index].id                       = MSM_PMIC_DUMP;
   AppDumpTable->Entries[running_index].type                     = MSM_DUMP_TYPE_DATA;
   AppDumpTable->Entries[running_index].start_addr               = (UINT64)(&DumpDataTypeTable->DumpDataType[running_index]);
   running_index += 1;

   /* Scan Dump is per core, each is 64KB */
   for (i=0; i < NumCores ; i++)
   {
      DumpDataTypeTable->DumpDataType[running_index + i].header.version = 0x0;
      DumpDataTypeTable->DumpDataType[running_index + i].header.magic   = 0x0;
      DumpDataTypeTable->DumpDataType[running_index + i].start_addr     = MemoryBase + SCAN_DUMP_START_OFFSET + i * SDI_SCAN_DUMP_PER_CORE_SIZE;
      DumpDataTypeTable->DumpDataType[running_index + i].len            = SDI_SCAN_DUMP_PER_CORE_SIZE;
      AppDumpTable->Entries[running_index + i].id                       = MSM_DATA_APPS_SCANDUMP + i;
      AppDumpTable->Entries[running_index + i].type                     = MSM_DUMP_TYPE_DATA;
      AppDumpTable->Entries[running_index + i].start_addr               = (UINT64)(&DumpDataTypeTable->DumpDataType[running_index + i]);
   }
   running_index += NumCores;
   // LA calls into this function in SEC where NumCores is 0x4
   ASSERT(running_index == NUM_OF_APPS_DUMP_ENTRY || NumCores < NUMBER_OF_CORES);

   /* Set the IMEM Cookie with the address to the DbiDumpTable */
   Status = GetConfigValue ("SharedIMEMBaseAddr", (UINT32 *)&SharedIMEMBaseAddr); 
   if ((Status != EFI_SUCCESS) || (SharedIMEMBaseAddr == 0))
   {
      DEBUG ((EFI_D_ERROR, "Unable to get SharedIMEMBaseAddr from uefiplat!\n"));
      return;
   }

   /* TODO: Fix this when Shared IMEM table is fixed to support 64-bit addressing */
   WriteAddr = (UINTN) SharedIMEMBaseAddr + DBI_SHARED_IMEM_COOKIE_OFFSET;
   //ASSERT((UINTN) DbiDumpTable < 0xFFFFFFFF);
   WriteValue = ((UINTN)DbiDumpTable & 0xFFFFFFFF);
   MmioWrite32(WriteAddr, WriteValue);
   WriteAddr += sizeof(UINT32);
   MmioWrite32(WriteAddr, (UINT32)0);

   // Create a ShmBridge on DBI region. this is required in hyper-v enabled cases
   // to collect APREG region. TZ checks to see if the APREG region is already
   // registered for it to write to it.
   Status = gBS->LocateProtocol(&gEfiShmBridgeProtocolGuid, NULL, (VOID **)&ShmBridgeProtocol);
   if (Status != EFI_SUCCESS)
   {
      DEBUG((EFI_D_ERROR, "SetupDBIDumpTable: Unable to locate EfiShmBridgeProtocol.\n"));
   }
   else
   {
      MapInfo.uInputAddr = DBIDumpRegionInfo.MemBase;
      MapInfo.uOutputAddr = DBIDumpRegionInfo.MemBase;
      MapInfo.uSize = DBIDumpRegionInfo.MemSize;
      VMInfo[0].uDstVM = AC_VM_HLOS;
      VMInfo[0].uDstVMperm = VM_PERM_R | VM_PERM_W;
      Status = ShmBridgeProtocol->ShmBridgeCreate(ShmBridgeProtocol, &MapInfo, &VMInfo[0], 1, VM_PERM_R | VM_PERM_W, &uHandle);
      if (Status != EFI_SUCCESS)
      {
         DEBUG((EFI_D_ERROR, "SetupDBIDumpTable: ShmBridge creation failed.\n"));
      }
      else
      {
         DEBUG((EFI_D_INFO, "SetupDBIDumpTable: ShmBridge addr %llx handle %llx\n", (DBIDumpRegionInfo.MemBase), uHandle));
      }
   }
   
}


EFI_STATUS
InitDbiDump(VOID)
{
   EFI_STATUS Status;

   SetupDBIDumpTable();

   Status = SetDLOADCookie();
   if (Status != EFI_SUCCESS ) 
     DEBUG((EFI_D_ERROR, "ERROR: Failed to set DLOAD cookie\n"));

   return Status;
}

EFI_STATUS
SaveUefiContext (EFI_SYSTEM_CONTEXT SystemContext)
{
  UefiInfoBlkType* UefiInfoBlkPtr;
  
  UefiInfoBlkPtr = GetInfoBlkPtr();
  if (UefiInfoBlkPtr == NULL) {
    DEBUG((EFI_D_ERROR, "ERROR: Failed to get UefiInfoBlk!\n"));
    return EFI_DEVICE_ERROR;
  }

  /* TODO: add handling for ARM */
  UefiInfoBlkPtr->CrashContext = (VOID*)SystemContext.SystemContextAArch64;
  return EFI_SUCCESS;
}

