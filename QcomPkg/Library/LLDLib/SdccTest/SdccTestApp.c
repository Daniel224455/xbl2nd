/** @file SdccTestApp.c
   
  SDCC Test Application. 
  
  Copyright (c) 2011-2016 Qualcomm Technologies, Inc. 
  All Rights Reserved. 
  Qualcomm Technologies Proprietary and Confidential.
   
**/

/*=============================================================================
                              EDIT HISTORY


when         who     what, where, why
----------   ---     -----------------------------------------------------------
2016-03-11   jt      Remove RpmbHalfSectorGranularity 
2015-05-13   jb      Change device path to not conflict with standard one
2015-04-20   rm/jt   Add UFS support
2014-04-16   bn      Fixed Klockwork warnings
2013-07-01   jt      Use UefiCfg to get RpmbHalfSectorGranularity 
2013-04-01   jt      Remove assert if RpmbProtocol is not found  
2013-02-13   bn      Fix klockwork warnings
2013-01-17   vk      Fix warning
2013-01-17   jt      Updated RPMB test from listener changes 
2013-01-16   pa      Fixes for klocwork errors
2012-09-18   jt      Change tick values to 64-bit 
2012-08-13   bn      Added Reliable Write initialization  
2012-07-03   bn      Enable RPMB Test
2012-06-27   pbi     Updated for timetick changes.
2012-04-26   bn      Removed RPMB provisioning. Fixed RPMB write packet count
                     Updated print performance function
2012-04-03   bn      Added MediaID. Changed the test sector size to 512  
2012-03-06   bn      Increased the test transfer size 
2011-11-16   bn      Removed RPMB Test
2011-10-26   bn      Added tests for RPMB partition
2011-10-13   jt      Added tests for all eMMC physical partitions
2011-09-01   bn      Added print out for performance data (read/write speeds)   
2011-07-21   bn      Fixed issue when card not found. Added test 
                     support for both eMMC and ext SD   
2011-06-30   bn      Set a pattern to the test buffer for easy debug  
2011-03-01   bn      Initial revision.
 
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/BlockIo.h>
#include <Library/TimerLib.h>
#include "DALSys.h"
#include <Library/PcdLib.h>
#include <Library/DevicePathLib.h>
#ifdef RPMB_SUPPORTED
#include <Protocol/EFIRpmb.h>
#include <Library/RpmbCg.h>
#endif // RPMB_SUPPORTED

#define MMC_WRITE_OP    0
#define MMC_READ_OP     1

typedef UINT32 MEDIA_TYPE;

#define EMMC     0
#define SD       1
#define UFS      2

typedef struct
{
  char   *media_name;
  /* Size of 1 sector in byte */
  UINT32  block_size;
  /* Use 2 sectors for multi sectors test */
  UINT32  multi_sector_buff_len;
} MEDIA_INFO;

static const MEDIA_INFO media_info[] =
{
  {"EMMC", 512, 1024}, 
  {"SD",   512, 1024},
  {"UFS", 4096, 8192}
};

#ifdef RPMB_SUPPORTED
/** 
  RPMB handle and variables. 
 */
EFI_SDCC_RPMB_PROTOCOL *RpmbProtocol = NULL;
static UINT8 ResponsePktBuffer[BLOCK_SIZE];

#define RPMB_TEST_SECTOR_START  0
#define RPMB_TEST_SECTOR_END    256
#define RPMB_TEST_SECTORS_TOTAL (RPMB_TEST_SECTOR_END - RPMB_TEST_SECTOR_START)
#define RPMB_TEST_BUFFER_SIZE   BLOCK_SIZE*RPMB_TEST_SECTORS_TOTAL
#endif // RPMB_SUPPORTED

/* Select the consecutive sectors used for the test. 
   For simplicity, choose the total test sectors to be an even number.
   If an odd number is chosen, the last sector will not be tested in
   multi block mode test */
#define SDCC_TEST_SECTOR_START  0
#define SDCC_TEST_SECTOR_END    10
#define SDCC_TEST_SECTORS_TOTAL (SDCC_TEST_SECTOR_END - SDCC_TEST_SECTOR_START)

/* Total Sectors used for the performance data collection */
#define SDCC_PERF_SECTOR_START  0
#define SDCC_PERF_SECTOR_END    512
#define SDCC_PERF_TEST_SECTORS  (SDCC_PERF_SECTOR_END - SDCC_PERF_SECTOR_START)

/* Total number of read/write iterations used for collecting performance data */
#define SDCC_PERF_TEST_COUNT  6

static UINTN p0_tested = 0, p1_tested = 0, p2_tested = 0, p3_tested = 0; 
static UINTN p4_tested = 0, p5_tested = 0, p6_tested = 0, p7_tested = 0;  

extern EFI_GUID gEfiEmmcUserPartitionGuid; 
extern EFI_GUID gEfiEmmcBootPartition1Guid;
extern EFI_GUID gEfiEmmcBootPartition2Guid; 
extern EFI_GUID gEfiEmmcGppPartition1Guid; 
extern EFI_GUID gEfiEmmcGppPartition2Guid; 
extern EFI_GUID gEfiEmmcGppPartition3Guid; 
extern EFI_GUID gEfiEmmcGppPartition4Guid; 

extern EFI_GUID gEfiUfsLU0Guid;
extern EFI_GUID gEfiUfsLU1Guid;
extern EFI_GUID gEfiUfsLU2Guid;
extern EFI_GUID gEfiUfsLU3Guid;
extern EFI_GUID gEfiUfsLU4Guid;
extern EFI_GUID gEfiUfsLU5Guid;
extern EFI_GUID gEfiUfsLU6Guid;
extern EFI_GUID gEfiUfsLU7Guid;

#ifdef RPMB_SUPPORTED
extern EFI_GUID gEfiUfsRPMBGuid;
extern EFI_GUID gEfiEmmcRpmbPartitionGuid;
#endif // RPMB_SUPPORTED

typedef struct {
   VENDOR_DEVICE_PATH  Mmc;
   EFI_DEVICE_PATH     End;
} MMCHS_DEVICE_PATH;

/* UFS Device Path */
typedef struct {
   VENDOR_DEVICE_PATH  Ufs;
   EFI_DEVICE_PATH     End;
} QCUFS_DEVICE_PATH;

/**
  This function performs a loop back test. 
  It saves the content of the sectors used for the test, writes 
  a predefined data to the sectors and reads back. It then 
  compares the buffers for any mismatch. Once the test 
  completes, it restores the content of the test sectors. 
  
  @retval EFI_SUCCESS       The test passed.
  @retval EFI_NO_MEDIA      No memory card found
  @retval EFI_ABORTED       Some errors during the read/write.  
**/
EFI_STATUS
EFIAPI
SdccLoopBackTest (
  EFI_BLOCK_IO_PROTOCOL *SdccBlockIOProtocol,
  UINT32       length,
  UINT32       logical_sector_addr,  
  CONST UINT8  *write_buffer,
  UINT8        *read_buffer,
  UINT8        *backup_buffer
  )
{
  EFI_STATUS status = EFI_SUCCESS;
  UINT32 i;
  UINT32 MediaId;

  if (NULL == SdccBlockIOProtocol)
  {
    AsciiPrint ("ERROR: SdccTestApp - SdccBlockIOProtocol is NULL \n");
    status = EFI_ABORTED;
    return status;
  }

  MediaId = SdccBlockIOProtocol->Media->MediaId;

  /* Save the content of the sectors used for the loopback test */
  status = SdccBlockIOProtocol->ReadBlocks(SdccBlockIOProtocol, MediaId, 
             logical_sector_addr, length, backup_buffer);

  if (EFI_NO_MEDIA == status)
  {
    AsciiPrint ("SdccTestApp - No Card Presence !! \n\n");
    return EFI_NO_MEDIA;
  }
 
  if (EFI_SUCCESS != status)  
  {
    AsciiPrint ("ERROR: SdccTestApp - Failed to back up the test sectors."
                        "Abort \n");
    return EFI_ABORTED;
  }

  /* write buffer contains a predefined data. Write to the test sectors */
  status = SdccBlockIOProtocol->WriteBlocks(SdccBlockIOProtocol, MediaId, 
             logical_sector_addr, length, (VOID *)write_buffer);

  if (EFI_SUCCESS != status)  
  {
    AsciiPrint ("ERROR: SdccTestApp - Failed to write the test sectors."
                        "Abort \n");
    /* Restore the content of the test sectors */   
    SdccBlockIOProtocol->WriteBlocks(SdccBlockIOProtocol, MediaId, 
      logical_sector_addr, length, backup_buffer);

    return EFI_ABORTED;
  }
  
  /* Set the read buffer to 0xAA before the read operation so we can the
     read actually changes the buffer content */
  SetMem(read_buffer, length, 0xAA);  

  /* Read back the test sectors */
  status = SdccBlockIOProtocol->ReadBlocks(SdccBlockIOProtocol, MediaId, 
             logical_sector_addr, length, read_buffer);

  if (EFI_SUCCESS == status)
  {
    /* Read back succeeded. Compare the write vs read contents */
    for (i = 0;  i < length; i++)
    {
      if (*(read_buffer + i) != *(write_buffer + i))
      {
        AsciiPrint ("ERROR: SdccTestApp - Content mismatch at byte 0x%x \n");
        status = EFI_ABORTED;
        break;
      }
    }
  }
  else  
  {
    AsciiPrint ("ERROR: SdccTestApp - Failed to read back the "
                        "test sectors \n");
    status = EFI_ABORTED;
  }

  /* Restore the content of the test sectors */
  if (EFI_SUCCESS != SdccBlockIOProtocol->WriteBlocks(SdccBlockIOProtocol, MediaId, 
                       logical_sector_addr, length, backup_buffer))
  {
    AsciiPrint ("ERROR: SdccTestApp - Failed to restore the test sectors");
    status = EFI_ABORTED;
  }
  
  return status;
}

/**
  This function performs BlockIO tests for the eMMC and external SD card.
  It calls the SdccLoopBackTest API to perform the write/read loop back tests

  @param[in] SdccBlockIoProtocol    Handle to either the eMMC's 
                                    or External SD's Block IO Protocol.

  @retval EFI_SUCCESS       Test completed successfully .
  @retval EFI_NO_MEDIA      No memory card found
  @retval EFI_ABORTED       Some errors during the read/write. 
**/

EFI_STATUS
EFIAPI
SdccBlockIoTest (
  EFI_BLOCK_IO_PROTOCOL *SdccBlockIoProtocol,
  MEDIA_TYPE media
  )
{
  EFI_STATUS status = EFI_SUCCESS;  
  UINT32 index = 0, test_sectors_len = 0, last_sector = 0;
  UINT8 *write_buff, *read_buff, *backup_buff;
  UINT32 pages = 0;
  UINT32 sdcc_multi_sector_buff_len = 0;
  UINT32 sdcc_single_sector_buff_len = 0;
  
  if (NULL == SdccBlockIoProtocol)
  {
    AsciiPrint ("SdccTestApp - SdccBlockIoProtocol is NULL \n");
    status = EFI_ABORTED;
    return status;
  }

  sdcc_single_sector_buff_len = media_info[media].block_size;
  sdcc_multi_sector_buff_len = media_info[media].multi_sector_buff_len;
  pages = sdcc_multi_sector_buff_len/4096;
  
  if (sdcc_multi_sector_buff_len % 4096 != 0) pages++;
  
  /* Allocate the memory for test buffers */

  write_buff = AllocatePages(pages);
  if (NULL == write_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. write_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  read_buff = AllocatePages(pages);
  if (NULL == read_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. read_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  backup_buff = AllocatePages(pages);
  if (NULL == backup_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. backup_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  /* Fill in the write buffer with predefined data */
  for (index = 0; index < sdcc_multi_sector_buff_len; index++)
  {
    *(write_buff + index) = (index & 0xff);
  }

  /* Test a small set of sectors only, not the whole memory size */
  last_sector = SdccBlockIoProtocol->Media->LastBlock;

  if (0 == last_sector)
  {
    AsciiPrint ("SdccTestApp - No Card Found. \n");
    FreePages(write_buff, pages);
    FreePages(read_buff, pages);
    FreePages(backup_buff, pages);    

    return EFI_NO_MEDIA;
  }

  test_sectors_len = ((SDCC_TEST_SECTORS_TOTAL < last_sector) ? 
                      SDCC_TEST_SECTORS_TOTAL : last_sector);

  /* Multi sector loopback test */
  AsciiPrint ("SdccTestApp - Testing Multiple Sectors Loopback \n");
  index = 0;
  
  while (index < test_sectors_len)
  {
    status = SdccLoopBackTest (SdccBlockIoProtocol, 
                sdcc_multi_sector_buff_len, (SDCC_TEST_SECTOR_START + index), 
                write_buff, read_buff, backup_buff);

    if (EFI_NO_MEDIA == status)
    {
      AsciiPrint ("SdccTestApp - No Card Found. Test Skipped \n");
      FreePages(write_buff, pages);
      FreePages(read_buff, pages);
      FreePages(backup_buff, pages);
      
      return status;
    }
    else if (EFI_SUCCESS != status)
    {
      AsciiPrint ("ERROR: SdccTestApp - Multiple Sectors Loopback Failed \n");
      break;
    }
    
    index += 2;
  }

  /* Single sector loopback test */
  AsciiPrint ("SdccTestApp - Testing Single Sector Loopback \n\n");

  index = 0;

  while (index < test_sectors_len)
  { 
    status = SdccLoopBackTest (SdccBlockIoProtocol, 
               sdcc_single_sector_buff_len, (SDCC_TEST_SECTOR_START + index), 
               write_buff, read_buff, backup_buff);

    if (EFI_NO_MEDIA == status)
    {
      AsciiPrint ("SdccTestApp - No Card Found. Test Skipped \n");
      break;
    }
    else if (EFI_SUCCESS != status)
    {
      AsciiPrint ("ERROR: SdccTestApp - Single Sector Loopback Failed \n");
      break;
    }

    index++;
  }

  FreePages(write_buff, pages);
  FreePages(read_buff, pages);
  FreePages(backup_buff, pages);      

  return status;
}


/**
  This function prints the read/write size and time
  
  @param[in]  Total number of ticks between the events
  @param[in]  Total number of blocks
  @param[in]  read/write operations
  @param[in]  media type
 
**/
VOID
EFIAPI
PrintPerfData (
  UINT64 total_ticks,
  UINT32 blocks,
  UINT32 op,
  MEDIA_TYPE media
  )
{
  UINT64 TimetickFreq = 0;
  UINT64 uSeconds = 0;
  UINT32 size = media_info[media].block_size * blocks;
  char* name = media_info[media].media_name;
  UINT64 tput = 0, remainder = 0;
   
  /* Convert ticks to micro seconds */
  TimetickFreq = GetPerformanceCounterProperties(NULL,NULL);
  uSeconds = MultU64x32(total_ticks, 1000000);
  uSeconds = DivU64x32(uSeconds, TimetickFreq);
  tput = DivU64x64Remainder(size,uSeconds,&remainder);
  remainder = DivU64x64Remainder(MultU64x32(remainder, 100),uSeconds,NULL);
  
  if (op == MMC_READ_OP)
  {
     AsciiPrint ("%a Read: Size = %d Bytes. uSeconds = %ld. Speed = %ld.%2ld MB/s \n",
                  name, size, uSeconds, tput, remainder);
  }
  else if (op == MMC_WRITE_OP)
  {
     AsciiPrint ("%a Write: Size = %d Bytes. uSeconds = %ld. Speed = %ld.%2ld MB/s \n",
                  name, size, uSeconds, tput, remainder);
  }
}
/**
  This function performs write/read tests for SDCC_PERF_TEST_SECTORS sectors.
  It then prints out the time it takes to perform the read/write
  
  @param[in] SdccBlockIoProtocol    Handle to either the eMMC's 
                                    or External SD's Block IO Protocol.
                                    
  @retval EFI_SUCCESS       The test passed.
  @retval EFI_NO_MEDIA      No memory card found
  @retval EFI_ABORTED       Some errors during the read/write.  
**/
EFI_STATUS
EFIAPI
SdccPerformanceTest (
  EFI_BLOCK_IO_PROTOCOL *SdccBlockIOProtocol,
  MEDIA_TYPE media
  )
{
  UINT8 *perf_write_buff, *perf_read_buff, *perf_backup_buff;
  UINT64 starting_tick = 0, ending_tick = 0, total_ticks = 0; 
  UINT32 i = 0;
  UINT32 block_size = media_info[media].block_size;
  UINTN test_pages = (SDCC_PERF_TEST_SECTORS*block_size/4096) + 1; 
  EFI_STATUS status = EFI_SUCCESS;  
  UINT32 MediaId;
  
  if (NULL == SdccBlockIOProtocol)
  {
    AsciiPrint ("ERROR: SdccTestApp - SdccBlockIOProtocol is NULL \n");
    status = EFI_ABORTED;
    return status;
  }

  MediaId = SdccBlockIOProtocol->Media->MediaId;

  /* Allocate the memory for test buffers */

  perf_write_buff = AllocatePages(test_pages);
  if (NULL == perf_write_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. perf_write_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  perf_read_buff = AllocatePages(test_pages);
  if (NULL == perf_read_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. perf_read_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  perf_backup_buff = AllocatePages(test_pages);
  if (NULL == perf_backup_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. perf_backup_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  /* Save the content of test sectors */
  status = SdccBlockIOProtocol->ReadBlocks(SdccBlockIOProtocol, MediaId, 
             SDCC_PERF_SECTOR_START, (block_size*SDCC_PERF_TEST_SECTORS), 
             perf_backup_buff);
  
  if (EFI_SUCCESS != status)  
  {
    AsciiPrint ("ERROR: SdccTestApp: Failed back-up Performance sectors."
                        "Abort \n");

    FreePages(perf_write_buff, test_pages);
    FreePages(perf_read_buff, test_pages);
    FreePages(perf_backup_buff, test_pages);    

    return EFI_ABORTED;
  }

  /* Set the read buffer content to a pattern of 0xAA */
  SetMem(perf_write_buff, (block_size*SDCC_PERF_TEST_SECTORS), 0xAA);  

  for (i=0; i<SDCC_PERF_TEST_COUNT; i++)
  {
    /* Get the tick count at the start of the event */
    starting_tick = GetPerformanceCounter();
  
    /* Perform a Write a total of SDCC_PERF_TEST_SECTORS sectors */
    status = SdccBlockIOProtocol->WriteBlocks(SdccBlockIOProtocol, MediaId, 
      SDCC_PERF_SECTOR_START, (block_size*SDCC_PERF_TEST_SECTORS), perf_write_buff);

    /* Get the tick count at the end of the event */
    ending_tick = GetPerformanceCounter();

    /* Calculate the number of ticks spent in the event */
    total_ticks = ending_tick - starting_tick;

    /* Print out the time spent in the event */
    PrintPerfData(total_ticks, SDCC_PERF_TEST_SECTORS, MMC_WRITE_OP, media);  

    if (EFI_SUCCESS != status)  
    {
      AsciiPrint ("ERROR: SdccTestApp: Failed Performance Write. Abort \n");

      /* Restore the content of the performance test sectors */   
      SdccBlockIOProtocol->WriteBlocks(SdccBlockIOProtocol, MediaId, 
        SDCC_PERF_SECTOR_START, (block_size*SDCC_PERF_TEST_SECTORS), perf_backup_buff);

      FreePages(perf_write_buff, test_pages);
      FreePages(perf_read_buff, test_pages);
      FreePages(perf_backup_buff, test_pages); 

      return EFI_ABORTED;
    }
  }
  
  /* Set the buffer to a pattern, so we can verify the buffer after the read */
  SetMem(perf_read_buff, (block_size*SDCC_PERF_TEST_SECTORS), 0xBB);  

  for (i=0; i<SDCC_PERF_TEST_COUNT; i++)
  {
    /* Get the tick count at the start of the event */
    starting_tick = GetPerformanceCounter();

    /* Read back the test sectors */
    status = SdccBlockIOProtocol->ReadBlocks(SdccBlockIOProtocol, MediaId, 
               SDCC_PERF_SECTOR_START, (block_size*SDCC_PERF_TEST_SECTORS), 
               perf_read_buff);

    /* Get the tick count at the end of the event */
    ending_tick = GetPerformanceCounter();

    /* Calculate the number of ticks spent in the event */
    total_ticks = ending_tick - starting_tick;

    /* Print out the time spent in the event */
    PrintPerfData(total_ticks, SDCC_PERF_TEST_SECTORS, MMC_READ_OP, media);  
  }  

  /* Restore the content of the test sectors */
  status = SdccBlockIOProtocol->WriteBlocks(SdccBlockIOProtocol, MediaId, 
             SDCC_PERF_SECTOR_START, (block_size*SDCC_PERF_TEST_SECTORS), 
             perf_backup_buff);

  if (EFI_SUCCESS == status)
  {
     AsciiPrint ("SdccTestApp: Performance Test Done!!! \n\n");
  }
  else
  {
    AsciiPrint ("ERROR: SdccTestApp - Failed to restore Performance "
                        "test sectors \n");
  }

  FreePages(perf_write_buff, test_pages);
  FreePages(perf_read_buff, test_pages);
  FreePages(perf_backup_buff, test_pages);
    
  return status;
}


EFI_STATUS
EFIAPI
SdccBlkIoTest (
  EFI_BLOCK_IO_PROTOCOL *SdccBlockIoProtocol, 
  UINT32                PartitionNum,
  MEDIA_TYPE            media
  )
{
  EFI_STATUS status = EFI_SUCCESS; 
  char *name = media_info[media].media_name;   

  AsciiPrint ("SdccTestApp - Testing %a Card Partition(LUN) = %d\n", name, PartitionNum);
  status = SdccBlockIoTest(SdccBlockIoProtocol, media);

  if (EFI_NO_MEDIA == status)
  {
    AsciiPrint ("SdccTestApp - No %a Card Found \n\n\n", name);
  }
  else if (EFI_SUCCESS == status)
  {
    AsciiPrint ("SdccTestApp - %a Performance Test "
                        "Partition %d Start \n", name, PartitionNum);    

    SdccPerformanceTest(SdccBlockIoProtocol, media);

    AsciiPrint ("SdccTestApp - %a Block IO TEST "
                        "Partition %d PASSED!!! \n\n", name, PartitionNum);
  }
  else
  {
    AsciiPrint ("SdccTestApp - %a Block IO TEST "
                        "Partition %d FAILED!!! \n\n", name, PartitionNum);
    return EFI_ABORTED;
  }

  switch (PartitionNum)
  {
    case 0: 
      p0_tested++; 
      break; 
    case 1: 
      p1_tested++; 
      break; 
    case 2: 
      p2_tested++; 
      break;    
    case 3: 
      p3_tested++; 
      break; 
    case 4: 
      p4_tested++; 
      break; 
    case 5: 
      p5_tested++; 
      break; 
    case 6: 
      p6_tested++; 
      break;
    case 7: 
      p7_tested++; 
      break;      
    default: 
      return EFI_INVALID_PARAMETER; 
  }

  return EFI_SUCCESS;
}

#ifdef RPMB_SUPPORTED
/**
Name: PRMBLoopBackTest

  This function does a loop back test for the RPMB partition.
  It saves the contents of the sectors to be used for the test.
  It writes a pattern to the test sectors and read back.
  Once done, it restores the test sectors content.
  To access the RPMB, the data packet has to be formatted per the
  eMMC specification. Therefore, it calls into the Content Generator lib
  to correctly format the test data.


  @param  length [IN]        : Size of test buffer in byte
  @param  Lba [IN]           : Start of test sector
  @param  write_buffer [IN]  : Allocated buffer used for the Write
  @param  read_buffer [IN]   : Allocated buffer used for the Read
  @param  backup_buffer [IN] : Allocated buffer used for the content backup

  @retval  EFI_SUCCESS : Test passed
  @retval  EFI_ABORT:    Test failed.

**/
EFI_STATUS
EFIAPI
RPMBLoopBackTest (

  EFI_SDCC_RPMB_PROTOCOL *RpmbProtocol,
  UINT32       length,
  UINT32       Lba,  
  CONST UINT8  *write_buffer,
  UINT8        *read_buffer,
  UINT8        *backup_buffer,
  MEDIA_TYPE   media
  )
{
  UINT32  *TempPktBuffer = NULL;
  UINT8  *BackupData = NULL; 
  UINT32  PktCount = 0;
  EFI_STATUS status = EFI_SUCCESS;
  UINT32 i, PktNum = 0, PktIndex = 0; 
  UINT64 starting_tick = 0, ending_tick = 0, total_ticks = 0;

  TempPktBuffer = AllocatePages(64);
  if (NULL == TempPktBuffer)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. TempPktBuffer\n"));
    return EFI_OUT_OF_RESOURCES;
  } 

  BackupData = AllocatePages(32); 
  if (NULL == BackupData)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. BackupData\n"));
    return EFI_OUT_OF_RESOURCES;
  } 

  PktCount = (length/BLOCK_SIZE); 

/*-----------------------------------------------------------------------*/ 
/* BACKUP. Read and save the test sectors */

  SetMem(backup_buffer, length, 0xBB);

  GenerateRPMBReadPkts(Lba, PktCount, (UINT8 *)TempPktBuffer);

  RpmbProtocol->RPMBReadBlocks(RpmbProtocol, TempPktBuffer, 
              PktCount * 2, (UINT32*)backup_buffer);
  
  /* Save the data from the backup_buffer */ 
  for (i = 0; i < (PktCount * 2); i++)
  {
    CopyMem(BackupData + (DATA_FIELD_LEN * i), 
            backup_buffer + DATA_INDEX + (sizeof(RPMB_DATA_FRAME) * i), 
            DATA_FIELD_LEN); 
  }	

/*-----------------------------------------------------------------------*/ 

/*-----------------------------------------------------------------------*/ 
/* WRITE test sectors */

/* Get the tick count at the start of the event */
  starting_tick = GetPerformanceCounter();
 
  /* Generate the Write packet */
  GenerateRPMBWritePkts(Lba, PktCount, (UINT8 *)write_buffer, 
              (UINT8 *)&ResponsePktBuffer[0], (UINT8 *)TempPktBuffer);

  /* Write to the test sectors */
  RpmbProtocol->RPMBWriteBlocks(RpmbProtocol, TempPktBuffer, 
              PktCount*2, (UINT32 *)&ResponsePktBuffer[0]);

/* Get the tick count at the end of the event */
  ending_tick = GetPerformanceCounter();
  
/* Calculate the number of ticks spent in the event */
  total_ticks = ending_tick - starting_tick;

/* Print out the time spent in the event */
  PrintPerfData(total_ticks, PktCount, MMC_WRITE_OP, media);  

/*-----------------------------------------------------------------------*/ 
/* READ test sectors */

  SetMem(read_buffer, length, 0xBB);

/* Get the tick count at the start of the event */
  starting_tick = GetPerformanceCounter();

  GenerateRPMBReadPkts(Lba, PktCount, (UINT8 *)TempPktBuffer);

  RpmbProtocol->RPMBReadBlocks(RpmbProtocol, TempPktBuffer, 
              PktCount * 2, (UINT32*)read_buffer);

/* Get the tick count at the end of the event */
  ending_tick = GetPerformanceCounter();

/* Calculate the number of ticks spent in the event */
  total_ticks = ending_tick - starting_tick;

/* Print out the time spent in the event */
  PrintPerfData(total_ticks, PktCount, MMC_READ_OP, media);  

/*-----------------------------------------------------------------------*/ 
/* RESTORE test sectors */
  
  /* Generate the Write packet */
  GenerateRPMBWritePkts(Lba, PktCount, (UINT8 *)BackupData, 
              (UINT8 *)ResponsePktBuffer, (UINT8 *)TempPktBuffer);  

  /* Restore the test sectors */
  RpmbProtocol->RPMBWriteBlocks(RpmbProtocol, TempPktBuffer, 
              PktCount*2, (UINT32 *)ResponsePktBuffer);

/*-----------------------------------------------------------------------*/ 
/* COMPARE test data */

  if (EFI_SUCCESS == status)
  {
    /* Read back succeeded. Compare the write vs read contents */
    for (i = 0;  i < length; i++)
    {
      PktNum = i / DATA_FIELD_LEN; 
      PktIndex = i % DATA_FIELD_LEN; 	  
      if (*(read_buffer + DATA_INDEX + (sizeof(RPMB_DATA_FRAME) * PktNum) + PktIndex) != *(write_buffer + i))
      {
        AsciiPrint ("RPMB - Content mismatch at byte 0x%x \n", i);
        status = EFI_ABORTED;
        break;
      }
    }

    if (i == length)
    {
       AsciiPrint ("RPMB - Test PASSED!!! \n\n");
    }
  }
  
  FreePages(TempPktBuffer, 64);

  return status;
}

/**

Name: SdccRPMBTest

  This function allocates the buffers used for the test.
  It goes through a sequence of events that need to happen in order to
  access the RPMB partition. It then calls RPMBLoopBackTest to do a loopback
  write/read tests


  @param   RpmbProtocol [IN]:  Pointer to the RPMB protocol

  @retval  EFI_SUCCESS : Test passed
  @retval  EFI_ABORT:    Test failed.

**/
EFI_STATUS
EFIAPI
SdccRPMBTest (
    EFI_SDCC_RPMB_PROTOCOL *RpmbProtocol,
    MEDIA_TYPE media    
  )
{
  EFI_STATUS status = EFI_SUCCESS;
  UINT32 index = 0;  
  UINT8 *write_buff, *read_buff, *backup_buff;
  
  AsciiPrint ("SdccTestApp - Testing RPMB Partition \n");

  /* Read the eMMC's Write Counter */
  status = RpmbProtocol->RPMBReadCounterPkt(RpmbProtocol, 
                         (UINT32 *)&ResponsePktBuffer[0]);
  
  if (RPMB_NO_ERROR != status)
  {
    AsciiPrint ("SdccTestApp - Skip RPMB Test. ");
    AsciiPrint ("Device's RPMB partition is not provisioned.\n");
    return EFI_SUCCESS;
  }

  /* Communicate the Reliable Write (RW) size to the Content Generator */
  /* If RW is > 1 and the transfer size is multiple of RW size, the CG will */
  /* format the frames using transfer size = RW. Otherwise, CG will format */
  /* the frames using RW = 1 */
  SetRPMBConfigInfo(RpmbProtocol->ReliableWriteCount);  
  if (media == UFS)
  {
      SetDeviceType (SIGNATURE_32('u','f','s',' ')); 
  }

  /* Allocate the memory for test buffers */
  write_buff = AllocatePages(32);
  if (NULL == write_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. write_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  read_buff = AllocatePages(64);
  if (NULL == read_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. read_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  backup_buff = AllocatePages(64);
  if (NULL == backup_buff)
  {
    DEBUG ((EFI_D_ERROR, "Failed mem allocation. backup_buff\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  /* Fill in the write buffer with predefined data */
  for (index = 0; index < RPMB_TEST_BUFFER_SIZE; index++)
  {
    *(write_buff + index) = (UINT8)(index & 0xff);
  }

  status = RPMBLoopBackTest (RpmbProtocol, RPMB_TEST_BUFFER_SIZE, 
              RPMB_TEST_SECTOR_START, write_buff, read_buff, backup_buff, media);

  FreePages(write_buff, 32);
  FreePages(read_buff, 64);
  FreePages(backup_buff, 64); 
    
  if (EFI_SUCCESS != status)
  {
    AsciiPrint ("ERROR: SdccTestApp - RPMB Test Failed \n");
    /* return EFI_ABORTED for now */    
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}
#endif //RPMB_SUPPORTED

/**
  The user Entry Point for SDCC Test Application. The user code 
  starts with this function as the real entry point for the 
  application. 

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval EFI_NO_MEDIA      No memory card found
  @retval EFI_ABORTED        Some other errors occured.
**/
EFI_STATUS
EFIAPI
SdccTestAppMain (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE *BlockIoHandles;
  UINTN NumberBlockIoHandles, count;
  EFI_STATUS status = EFI_SUCCESS;
  EFI_BLOCK_IO_PROTOCOL *BlkIo;
  UINTN removable_tested = 0;
  MEDIA_TYPE media = EMMC;

  UINT32 EmmcMediaSignature = SIGNATURE_32('e','m','m','c');
  UINT32 SdMediaSignature = SIGNATURE_32('s','d','i','o');
  UINT32 UfsMediaSignature = SIGNATURE_32('u','f','s',' '); 

  AsciiPrint ("SdccTestApp - Start Testing SDCC Block IO Protocol \n");

  status = gBS->LocateHandleBuffer (
      ByProtocol,
      &gEfiBlockIoProtocolGuid,
      NULL,
      &NumberBlockIoHandles,
      &BlockIoHandles
      );

  ASSERT (!EFI_ERROR (status));

  for (count = 0; count < NumberBlockIoHandles; count++) 
  {
     status = gBS->HandleProtocol (
                   BlockIoHandles[count],
                   &gEfiBlockIoProtocolGuid,
                   (VOID **) &BlkIo
                   );
    if (EFI_ERROR (status)) 
    {
      status = EFI_ABORTED;
      return status;
    }

    /* Test the eMMC physical partitions */
    if ((FALSE == BlkIo->Media->RemovableMedia) && 
        (EmmcMediaSignature == BlkIo->Media->MediaId))
    {
      media = EMMC;
      MMCHS_DEVICE_PATH *DevicePath;       
      DevicePath = 
        (MMCHS_DEVICE_PATH *) DevicePathFromHandle(BlockIoHandles[count]);

      if (CompareGuid(&DevicePath->Mmc.Guid, &gEfiEmmcUserPartitionGuid) && 
          (0 == p0_tested))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 0, media))
        {
          return EFI_ABORTED; 
        }
      }

      else if (CompareGuid(&DevicePath->Mmc.Guid, &gEfiEmmcBootPartition1Guid) && 
               (0 == p1_tested))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 1, media))
        {
          return EFI_ABORTED; 
        }
      }

      else if (CompareGuid(&DevicePath->Mmc.Guid, &gEfiEmmcBootPartition2Guid) &&
               (p2_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 2, media))
        {
          return EFI_ABORTED; 
        }
      }

      else if (CompareGuid(&DevicePath->Mmc.Guid, &gEfiEmmcGppPartition1Guid) &&
               (p3_tested == 0))
      { 
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 4, media))
        {
          return EFI_ABORTED; 
        }
      }

      else if (CompareGuid(&DevicePath->Mmc.Guid, &gEfiEmmcGppPartition2Guid) &&
               (p4_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 5, media))
        {
          return EFI_ABORTED; 
        } 
      }

      else if (CompareGuid(&DevicePath->Mmc.Guid, &gEfiEmmcGppPartition3Guid) &&
               (p5_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 6, media))
        {
          return EFI_ABORTED; 
        } 
      }

      else if (CompareGuid(&DevicePath->Mmc.Guid, &gEfiEmmcGppPartition4Guid) &&
               (p6_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 7, media))
        {
          return EFI_ABORTED; 
        }
      }
    }

    /* Test the UFS Logic Units */
    else if ((FALSE == BlkIo->Media->RemovableMedia) && 
        (UfsMediaSignature == BlkIo->Media->MediaId))
    {
      media = UFS;
      QCUFS_DEVICE_PATH *DevicePath;       
      DevicePath = 
        (QCUFS_DEVICE_PATH *) DevicePathFromHandle(BlockIoHandles[count]);

      if (CompareGuid(&DevicePath->Ufs.Guid, &gEfiUfsLU0Guid) && 
          (0 == p0_tested))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 0, media))
        {
          return EFI_ABORTED; 
        }
      }

      else if (CompareGuid(&DevicePath->Ufs.Guid, &gEfiUfsLU1Guid) && 
               (0 == p1_tested))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 1, media))
        {
          return EFI_ABORTED; 
        }
      }

      else if (CompareGuid(&DevicePath->Ufs.Guid, &gEfiUfsLU2Guid) &&
               (p2_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 2, media))
        {
          return EFI_ABORTED; 
        }
      }

      else if (CompareGuid(&DevicePath->Ufs.Guid, &gEfiUfsLU3Guid) &&
               (p3_tested == 0))
      { 
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 3, media))
        {
          return EFI_ABORTED; 
        }
      }

      else if (CompareGuid(&DevicePath->Ufs.Guid, &gEfiUfsLU4Guid) &&
               (p4_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 4, media))
        {
          return EFI_ABORTED; 
        } 
      }

      else if (CompareGuid(&DevicePath->Ufs.Guid, &gEfiUfsLU5Guid) &&
               (p5_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 5, media))
        {
          return EFI_ABORTED; 
        } 
      }

      else if (CompareGuid(&DevicePath->Ufs.Guid, &gEfiUfsLU6Guid) &&
               (p6_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 6, media))
        {
          return EFI_ABORTED; 
        }
      }
      
      else if (CompareGuid(&DevicePath->Ufs.Guid, &gEfiUfsLU7Guid) &&
               (p6_tested == 0))
      {
        if (EFI_ABORTED == SdccBlkIoTest(BlkIo, 7, media))
        {
          return EFI_ABORTED; 
        }
      }      
    }

    /* Test the external SD card */
    if ((0 == removable_tested) && (TRUE == BlkIo->Media->RemovableMedia) &&
        (SdMediaSignature == BlkIo->Media->MediaId))
    {
      media = SD;   
      AsciiPrint ("SdccTestApp - Testing External SD Card \n\n");
      status = SdccBlockIoTest(BlkIo, media);

      if (EFI_NO_MEDIA == status)
      {
        AsciiPrint ("SdccTestApp - No External SD Card Found. \n\n");
      }
      else if (EFI_SUCCESS == status)
      {
        AsciiPrint ("SdccTestApp - External SD Performance Test Start \n");

        SdccPerformanceTest(BlkIo, media);

        AsciiPrint ("SdccTestApp - External SD Card Block IO "
                            "TEST PASSED!!! \n\n");
      }
      else
      {
        AsciiPrint (" ERROR: SdccTestApp - External SD Card Block IO "
                            "TEST FAILED!!! \n\n");
        return EFI_ABORTED;
      }     

      removable_tested++;
    }
     
    if (removable_tested && p0_tested && p1_tested && p2_tested && 
        p3_tested && p4_tested && p5_tested && p6_tested)
    {
      break;
    }
  }

  #ifdef RPMB_SUPPORTED
  /* RPMB Testing*/
  EFI_HANDLE *RPMBHandles;
  UINTN NumberRPMBHandles;
  VOID *RPMBDevicePath = NULL;
  
  status = gBS->LocateHandleBuffer (
                ByProtocol,
                &gEfiSdccRpmbProtocolGuid,
                NULL,
                &NumberRPMBHandles,
                &RPMBHandles
                );

  ASSERT (!EFI_ERROR (status));

  for (count = 0; count < NumberRPMBHandles; count++) 
  {
     status = gBS->HandleProtocol (
                   RPMBHandles[count],
                   &gEfiSdccRpmbProtocolGuid,
                   (VOID **) &RpmbProtocol
                   );
    if (EFI_ERROR (status)) 
    {
      status = EFI_ABORTED;
      return status;
    }  
    
    RPMBDevicePath = NULL;
    RPMBDevicePath = (VOID *)DevicePathFromHandle(RPMBHandles[count]);
    
    if (RPMBDevicePath == NULL) 
    {
      AsciiPrint ("SdccTestApp - Rpmb device path not found, skipping RPMB Test. \n\n");      
      return EFI_SUCCESS;
    }
    
    if (CompareGuid(&((QCUFS_DEVICE_PATH *)RPMBDevicePath)->Ufs.Guid, 
                    &gEfiUfsRPMBGuid))
    {
      media = UFS;
    }
    else if (CompareGuid(&((MMCHS_DEVICE_PATH *)RPMBDevicePath)->Mmc.Guid, 
                         &gEfiEmmcRpmbPartitionGuid))
    {
      media = EMMC;
    }
    else
    {
      continue;
    }    
    
    if (NULL == RpmbProtocol)
    {
      AsciiPrint ("SdccTestApp - Rpmb Protocol not found, skipping RPMB Test. \n\n");    
      return EFI_DEVICE_ERROR;
    }
    
    status = SdccRPMBTest(RpmbProtocol, media);
    
    if (EFI_SUCCESS != status)
    {
      return EFI_ABORTED;
    }
  }

  #endif // RPMB_SUPPORTED

  return EFI_SUCCESS;
  
} /* SdccTestAppMain */


