/**
 * @file    SpiNorDxe.c
 * @brief   SPI NOR Dxe Driver
 *
 *  Copyright (c) 2017-2019 Qualcomm Technologies, Inc. All rights
 *  reserved.
 *  Portions Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
 *  This program and the accompanying materials
 *  are licensed and made available under the terms and conditions of the BSD License
 *  which accompanies this distribution.  The full text of the license may be found at
 *  http://opensource.org/licenses/bsd-license.php

 *  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
**/


/*=============================================================================
EDIT HISTORY


when            who   what, where, why
----------      ---   ----------------------------------------------------------- 
2019-04-01      sa    Added Write Protecion support in SPINOR.
2019-01-04      jt    Remove assert if syscall fails
2018-07-32      wek   Fix error codes returned by the SCM call.
2018-04-24      jt    Add GPT Listener
2018-03-28      wek   Fix bugs, not installing protocol, memory leak.
2018-03-23      wek   Fix indentation
2018-03-20      md    Updates/Fixes to handle SCM calls
2017-11-14      jt    Initial version
=============================================================================*/

#include <Include/scm_sip_interface.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BootConfig.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/GPTListener.h>
#include <Library/RpmbLib.h>
#include <Library/DebugLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/EFIEraseBlock.h>
#include <Protocol/EFIScm.h>
#include <Protocol/EFIStorageWriteProtect.h>

#include "SpiNorDxe.h"
#include <string.h>

static EFI_EVENT ScmInitialized = NULL;

typedef enum  {
  STOR_INIT = 0,
  STOR_READ,
  STOR_WRITE,
  STOR_ERASE,
  STOR_GET_INFO,
  STOR_DEINIT,
  STOR_WP_EN,
  STOR_WP_GET_INFO
} StorScmCommand;

typedef enum  {
  STOR_SPINOR = 0
} StorDevType;

/* UFS Device Path */
typedef struct {
  VENDOR_DEVICE_PATH  SpiNor;
  EFI_DEVICE_PATH     End;
} QCSPINOR_DEVICE_PATH;

static QCSPINOR_DEVICE_PATH gSpiNorDevicePath;

static VENDOR_DEVICE_PATH SpiNorPath =
{
  HARDWARE_DEVICE_PATH,
  HW_VENDOR_DP,
  (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
  (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8),
  0
};

static EFI_DEVICE_PATH SpiNorEndPath =
{
  END_DEVICE_PATH_TYPE,
  END_ENTIRE_DEVICE_PATH_SUBTYPE,
  sizeof (EFI_DEVICE_PATH_PROTOCOL),
  0
};

static EFI_GUID SpiNorDevicePathGuid = EFI_SPINOR_GUID;
static SPINOR_DEV gSpiNorDevice = { SPINOR_DEV_SIGNATURE, NULL, 0, 0, 0};

/* SPINOR Block IO's Media */
static EFI_BLOCK_IO_MEDIA SpiNorMedia = {
  SIGNATURE_32('s','n','o','r'),            //MediaId
  FALSE,                                    //RemovableMedia
  FALSE,                                    //MediaPresent
  FALSE,                                    //LogicalPartition
  FALSE,                                    //ReadOnly
  FALSE,                                    //WriteCaching
  4096,                                     //BlockSize
  8,                                        //IoAlign
  0,                                        //LastBlock
  0,                                        //LowestAlignedLba
  0,                                        //LogicalBlocksPerPhysicalBlock
  0                                         //OptimalTransferLengthGranularity
};

/* Block IO Protocol */
EFI_BLOCK_IO_PROTOCOL gBlockIoSpiNor = {
  EFI_BLOCK_IO_PROTOCOL_REVISION3,          /* Revision */
  0,                                        /* Media */
  SpiNorReset,                              /* Reset */
  SpiNorReadBlocks,                         /* ReadBlocks */
  SpiNorWriteBlocks,                        /* WriteBlocks */
  SpiNorFlushBlocks                         /* FlushBlocks */
};

/* SPI-NOR Write Protect Protocol */
static EFI_STORAGE_WP_PROTOCOL gSpiNorWriteProtectProtocol = {
   EFI_STORAGE_WP_PROTOCOL_REVISION,      /* Revision */
   SpiNorGetLbaWPTypes,                   /* GetLbaWriteProtectTypes */                      
   SpiNorSetLUWriteProtect,               /* SetLUWriteProtect */
   SpiNorSetLbaWriteProtect,              /* SetLbaWriteProtect */  
   SpiNorClearWriteProtect,               /* ClearWriteProtect */
   SpiNorGetLbaWriteProtectInfo           /* GetLbaWriteProtectInfo */
   
};

/* Erase Blocks Protocol */
static const EFI_ERASE_BLOCK_PROTOCOL gSpiNorEraseBlkProtocol = {
  EFI_ERASE_BLOCK_PROTOCOL_REVISION,        //Revision
  4096,                                      //Erase Granularity
  SpiNorEraseBlocks                         //Erase Blocks
};

/* SCM variables */
extern EFI_GUID gQcomScmProtocolGuid;
static QCOM_SCM_PROTOCOL *ScmProtocol = NULL;

/* Variable to store the WP debug struct entry */
UINT32 Lba_WP_count = 0;

/**
   Reset the Block Device.
**/
EFI_STATUS EFIAPI SpiNorReset (
  IN EFI_BLOCK_IO_PROTOCOL   *This,
  IN BOOLEAN                 ExtendedVerification)
{
  return EFI_SUCCESS;
}

/**
   Send syscall to TZ spinor driver
**/
EFI_STATUS SpiNorScmSyscall (
  IN StorScmCommand      Op,
  IN EFI_LBA             LBA,
  IN UINTN               BufferSize,
  OUT VOID               *Buffer
  )
{
  EFI_STATUS Status = EFI_DEVICE_ERROR;
  EFI_STATUS retval = EFI_DEVICE_ERROR;
  UINT64 Parameters[SCM_MAX_NUM_PARAMETERS] = {0};
  UINT64 Results[SCM_MAX_NUM_RESULTS] = {0};
  VOID *cmd_data;
  tz_stor_cmd *command;
  tz_stor_cmd_details *cmd_details;
  int command_size = sizeof(tz_stor_cmd) + sizeof(tz_stor_cmd_details);             /* size of command and command details structure */

  tz_send_storage_cmd_req_t *SysCallReq = (tz_send_storage_cmd_req_t*)Parameters;
  tz_send_storage_cmd_rsp_t *SysCallRsp = (tz_send_storage_cmd_rsp_t*)Results;

  cmd_data = UncachedAllocatePool (command_size);
  if (cmd_data == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }

  /*clear the comamnd structure */
  memset(cmd_data, 0, command_size);
  command = (tz_stor_cmd *)cmd_data;
  cmd_details = (tz_stor_cmd_details *)(cmd_data + sizeof(tz_stor_cmd));

  /*Populate the command */
  command->storage_type = TZ_STORAGE_TYPE_SPINOR;
  command->slot_num =0;
  command->LUN = 0;
  command->GUID_ptr = 0;
  command->storage_cmd = Op;

  /*Populate the coammnd_details */
  cmd_details->LBA = LBA;
  cmd_details->length= BufferSize;
  cmd_details->data_buffer_ptr =(UINT64) Buffer;

  /* Fill in the Request Structure */
  SysCallReq->cmd_metadata = command;
  SysCallReq->cmd_metadata_size = sizeof(tz_stor_cmd);
  SysCallReq->cmd_details = cmd_details;
  SysCallReq->cmd_details_size = sizeof (tz_stor_cmd_details);

  if (NULL == ScmProtocol)
  {
    DEBUG ((EFI_D_ERROR, "SpiNorDxe: ScmProtocol NULL"));
    retval = EFI_INVALID_PARAMETER;
    goto cleanup;
  }

  // Make ScmSipSysCall
  Status = ScmProtocol->ScmSipSysCall(ScmProtocol, TZ_SEND_STORAGE_CMD_ID,TZ_SEND_STORAGE_CMD_ID_PARAM_ID,
                                      Parameters, Results);
  if (EFI_ERROR (Status))
  {
    retval = EFI_DEVICE_ERROR;
    DEBUG(( EFI_D_ERROR, "ScmSipSysCall() failed, OP=%d Status = (0x%x)\r\n", Op, Status));
  }
  else if (SysCallRsp->common_rsp.status != 1)
  {
    retval = EFI_DEVICE_ERROR;
    DEBUG((EFI_D_ERROR," ScmSipSysCall() failed, OP=%d common_rsp Status = (0x%x)\r\n", Op, SysCallRsp->common_rsp.status));
  }
  else if (SysCallRsp->result != 0)
  {
    retval = EFI_DEVICE_ERROR;
    DEBUG(( EFI_D_ERROR, "TZ_SEND_STORAGE_CMD_ID failed, OP=%d start_addr=%d Status = (0x%x)\r\n", Op,  LBA ,SysCallRsp->result ));
    switch (SysCallRsp->result)
    {
    case STORAGE_CMD_RESULT_SUCCESS:
      retval = EFI_SUCCESS;
      break;
    case STORAGE_CMD_RESULT_NO_MEMORY:
      retval = EFI_OUT_OF_RESOURCES;
      break;
    case STORAGE_CMD_RESULT_INVALID_PARAMETER:
      retval = EFI_INVALID_PARAMETER;
      break;
    case STORAGE_CMD_RESULT_STORAGE_ERROR:
      retval = EFI_DEVICE_ERROR;
      break;
    case STORAGE_CMD_RESULT_ACCESS_DENIED:
      retval = EFI_ACCESS_DENIED;
      break;
    default:
      retval = EFI_NO_RESPONSE;
      break;
    }
  }
  else
  {
    retval = EFI_SUCCESS;
  }

cleanup:

  if (cmd_data != NULL)
    UncachedFreePool(cmd_data);

  return retval;
}

/* get device Info */
EFI_STATUS SpiNorGetInfo (VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  VOID *unbuf;
  int BufferSize = sizeof(tz_storage_info);
  tz_storage_info *dev_info;

  unbuf = UncachedAllocatePool(BufferSize);
  if (unbuf == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }
  /* Send SCM syscall to do the actual read */
  Status = SpiNorScmSyscall(STOR_GET_INFO, 0, BufferSize, unbuf);
  if (EFI_SUCCESS != Status)
  {
    DEBUG ((EFI_D_ERROR, "SPINOR Get Info with return value of %x\n", Status));
  }
  else
  {
    dev_info = (tz_storage_info*)unbuf;
    SpiNorMedia.MediaPresent  = TRUE;
    SpiNorMedia.BlockSize     = dev_info->block_size;
    SpiNorMedia.LastBlock     = dev_info->total_blocks -1;
    gSpiNorDevicePath.SpiNor  = SpiNorPath;
    gSpiNorDevicePath.End     = SpiNorEndPath;
    gSpiNorDevice.Signature   = SPINOR_DEV_SIGNATURE;
    gSpiNorDevice.BlkIo       = gBlockIoSpiNor;
    gSpiNorDevice.EraseBlk    = gSpiNorEraseBlkProtocol;
    gSpiNorDevice.WriteProtect = gSpiNorWriteProtectProtocol;
    gSpiNorDevice.BlkIo.Media = &SpiNorMedia;
    gSpiNorDevicePath.SpiNor.Guid = SpiNorDevicePathGuid;
  }

  UncachedFreePool(unbuf);

  return Status;
}

/**
   Read BufferSize bytes from Lba into Buffer.
**/
EFI_STATUS EFIAPI SpiNorReadBlocks (
  IN EFI_BLOCK_IO_PROTOCOL   *This,
  IN UINT32                  MediaId,
  IN EFI_LBA                 Lba,
  IN UINTN                   BufferSize,
  OUT VOID                   *Buffer)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 BlockCount = 0;
  VOID *unbuf;

  /* Validate passed-in pointers are not NULL */
  if ((NULL == Buffer) || (NULL == This) || (NULL == This->Media))
  {
    return EFI_INVALID_PARAMETER;
  }

  /* Validate block size and access range */
  if ((Lba > This->Media->LastBlock) || (This->Media->BlockSize == 0))
  {
    return EFI_INVALID_PARAMETER;
  }

  /* Validate if buffer is multiple of block size */
  if ((BufferSize % This->Media->BlockSize) != 0)
  {
    return EFI_BAD_BUFFER_SIZE;
  }

  if (This->Media->MediaId != MediaId)
  {
    return EFI_MEDIA_CHANGED;
  }

  /* If BufferSize is zero, the host is pinging the card for its presence */
  if (0 == BufferSize)
  {
    if (gSpiNorDevice.BlkIo.Media->MediaPresent == TRUE)
    {
      return EFI_SUCCESS;
    }
    else
    {
      return EFI_NO_MEDIA;
    }
  }

  BlockCount = (BufferSize/This->Media->BlockSize);

  /* Check if LBA plus the total sectors trying to access would exceed the */
  /* total size of the partition */
  if ((Lba + BlockCount) > (This->Media->LastBlock + 1))
  {
    return EFI_INVALID_PARAMETER;
  }

  unbuf = UncachedAllocatePool(BufferSize);
  if (unbuf == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }

  /* Send SCM syscall to do the actual read */
  Status = SpiNorScmSyscall(STOR_READ, Lba , BufferSize, unbuf);
  if (EFI_SUCCESS != Status)
  {
    DEBUG ((EFI_D_ERROR, "SPINOR READ with return value of %x\n", Status));
  }
  else
  {
    CopyMem (Buffer, unbuf, BufferSize);
  }

  UncachedFreePool(unbuf);

  return Status;
}

/**
   Write BufferSize bytes from Lba into Buffer.
**/
EFI_STATUS EFIAPI SpiNorWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL   *This,
  IN UINT32                  MediaId,
  IN EFI_LBA                 Lba,
  IN UINTN                   BufferSize,
  IN VOID                    *Buffer)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 BlockCount = 0;
  VOID *unbuf;

  /* Validate passed-in pointers are not NULL */
  if ((NULL == Buffer) || (NULL == This) || (NULL == This->Media))
  {
    return EFI_INVALID_PARAMETER;
  }

  /* Validate block size and access range */
  if ((Lba > This->Media->LastBlock) || (This->Media->BlockSize == 0))
  {
    return EFI_INVALID_PARAMETER;
  }

  /* Validate if buffer is multiple of block size */
  if ((BufferSize % This->Media->BlockSize) != 0)
  {
    return EFI_BAD_BUFFER_SIZE;
  }

  BlockCount = (BufferSize/This->Media->BlockSize);

  /* Validate if LBA plus the total sectors trying to access would exceed the */
  /* total size of the partition */
  if ((Lba + BlockCount) > (This->Media->LastBlock + 1))
  {
    return EFI_INVALID_PARAMETER;
  }

  if (This->Media->MediaId != MediaId)
  {
    return EFI_MEDIA_CHANGED;
  }

  unbuf = UncachedAllocatePool(BufferSize);
  if (unbuf == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (unbuf, Buffer, BufferSize);

  /* Send SCM syscall to do the actual write */
  Status = SpiNorScmSyscall(STOR_WRITE, Lba , BufferSize, unbuf);
  if (EFI_SUCCESS != Status)
  {
    DEBUG ((EFI_D_ERROR, "SPINOR WRITE with return value of %x\n", Status));
  }

  UncachedFreePool(unbuf);

  return Status;
}

/**
   Flush the Block Device.
**/
EFI_STATUS EFIAPI SpiNorFlushBlocks (
  IN EFI_BLOCK_IO_PROTOCOL  *This)
{
  return EFI_SUCCESS;
}

/**
   Erase blocks in the Block Device

   If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED
   is returned and non-blocking I/O is being used, the Event associated with
   this request will not be signaled.

   @param[in]      This     Indicates a pointer to the calling context.
   @param[in,out]  Token    A pointer to the token associated with the transaction

   @retval EFI_SUCCESS          The flush request was queued if Event is not NULL.
   All outstanding data was written correctly to the
   device if the Event is NULL.
   @retval EFI_DEVICE_ERROR     The device reported an error while writting back
   the data.
   @retval EFI_WRITE_PROTECTED  The device cannot be written to.
   @retval EFI_NO_MEDIA         There is no media in the device.
   @retval EFI_MEDIA_CHANGED    The MediaId is not for the current media.
   @retval EFI_OUT_OF_RESOURCES The request could not be completed due to a lack
   of resources.
**/
EFI_STATUS EFIAPI SpiNorEraseBlocks (
  IN     EFI_BLOCK_IO_PROTOCOL *This,
  IN     UINT32 MediaId,
  IN     EFI_LBA LBA,
  IN OUT EFI_ERASE_BLOCK_TOKEN *Token,
  IN     UINTN Size
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 BlockCount = 0;

  /* Validate passed-in parameters are valid */
  if ((NULL == This) || (NULL == This->Media) || (0 == Size))
  {
    Status = EFI_INVALID_PARAMETER;
    goto EraseExit;
  }

  /* Validate block size and access range */
  if ((LBA > This->Media->LastBlock) || (This->Media->BlockSize == 0))
  {
    Status = EFI_INVALID_PARAMETER;
    goto EraseExit;
  }

  /* Validate if size is multiple of block size */
  if ((Size % This->Media->BlockSize) != 0)
  {
    Status = EFI_INVALID_PARAMETER;
    goto EraseExit;
  }

  if (This->Media->MediaId != MediaId)
  {
    Status = EFI_MEDIA_CHANGED;
    goto EraseExit;
  }

  BlockCount = Size /(This->Media->BlockSize);

  /* Check if LBA plus the total sectors trying to erase would exceed the */
  /* total size of the partition */
  if ((LBA + BlockCount) > (This->Media->LastBlock + 1))
  {
    Status = EFI_INVALID_PARAMETER;
    goto EraseExit;
  }

  /* Send SCM syscall to do the actual erase */
  Status = SpiNorScmSyscall(STOR_ERASE,LBA, Size, NULL);
  if (EFI_SUCCESS != Status)
  {
    DEBUG ((EFI_D_ERROR, "SPINOR ERASE with return value of %x", Status));
  }

EraseExit:
  if (Token != NULL) {
    Token->Event = NULL;
    Token->TransactionStatus = Status;
  }

  return Status;
}

/**
   Return supported write protect types
**/
EFI_STATUS EFIAPI SpiNorGetLbaWPTypes(
   IN EFI_STORAGE_WP_PROTOCOL *This, 
   OUT UINT32 *LbaWpTypes
)
{
   if ((NULL == This) || (NULL == LbaWpTypes)) 
   {
      return EFI_INVALID_PARAMETER; 
   }

   /* We are only supporting POWERON Write Protect */
   *LbaWpTypes = WP_SECURE_P; 

   return EFI_SUCCESS; 
}

/**
   SPINOR doesnot support LU WP. LBA WP in only supported.
**/
EFI_STATUS EFIAPI SpiNorSetLUWriteProtect(
   IN EFI_STORAGE_WP_PROTOCOL *This 
)
{
  return EFI_UNSUPPORTED; 
}

/**
   Stores information about request for WP Lba's for debugging.
   Info is stored about all the WP requests made by clients.
   To check the actual regions which are WP, call 'SpiNorGetLbaWriteProtectInfo'.
**/
static void StoreLbaWriteProtected( EFI_WP_ENTRY *WpEntry, EFI_STATUS status)
{
  /* For now we will only store 15 entries in the table. */
  if(Lba_WP_count >= LBA_WP_COUNT_MAX)
	Lba_WP_count = 0;
  
  /* Store the LBA WP  info in the array.*/
  Lba_WP_Entry_dbg_info[Lba_WP_count].status = status & 0xF;
  Lba_WP_Entry_dbg_info[Lba_WP_count].WpEntry.Lba = WpEntry->Lba;
  Lba_WP_Entry_dbg_info[Lba_WP_count].WpEntry.NumBlocks = WpEntry->NumBlocks;
  /* Since we only support 1 type of write protect it will be always Power On */
  Lba_WP_Entry_dbg_info[Lba_WP_count].WpEntry.WpType = WpEntry->WpType;
	
  Lba_WP_count++;
	
}

/**
   Write Protect blocks in the Block Device

   @param[in]  This           Indicates a pointer to the calling context.
   @param[in]  WpConfig   Pointer to LBA range.

   @retval EFI_SUCCESS          The flush request was queued if Event is not NULL.
   All outstanding data was written correctly to the
   device if the Event is NULL.
   @retval EFI_DEVICE_ERROR     The device reported an error while writting back
   the data.
   @retval EFI_WRITE_PROTECTED  The device cannot be written to.
   @retval EFI_NO_MEDIA         There is no media in the device.
   @retval EFI_MEDIA_CHANGED    The MediaId is not for the current media.
   @retval EFI_OUT_OF_RESOURCES The request could not be completed due to a lack
   of resources.
**/
EFI_STATUS EFIAPI SpiNorSetLbaWriteProtect (
  IN  EFI_STORAGE_WP_PROTOCOL *This,
  IN  EFI_WP_CONFIG *WpConfig
 )
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER; 
  UINT32 Wpcount = 0;
  EFI_WP_ENTRY *WpEntry;
  
  if ((NULL == This) || (NULL == WpConfig)) 
  {
    return EFI_DEVICE_ERROR; 
  }
  
  /* Send SCM syscall to WriteProtect the block */
  while(Wpcount < WpConfig->NumEntries)
  {
  	WpEntry = &WpConfig->WpEntries[Wpcount];
	
	Status = SpiNorScmSyscall(STOR_WP_EN , WpEntry->Lba, WpEntry->NumBlocks * SpiNorMedia.BlockSize, NULL);

	/*Store LBA info with status if WP was successful or not */
    StoreLbaWriteProtected(WpEntry, Status);

	if (EFI_SUCCESS != Status)
    {
      DEBUG ((EFI_D_ERROR, "SPINOR WriteProtect with return value of 0x%x\n", Status));
	  break;
    } 
	
	Wpcount++;
  }
  return Status;  
}

/**
   SPINOR doesnot support Clear WP. 
**/
EFI_STATUS EFIAPI SpiNorClearWriteProtect(
   IN EFI_STORAGE_WP_PROTOCOL *This
)
{
  return EFI_UNSUPPORTED; 
}

/**
   Return info about the Lba's which are WriteProtected.
**/
EFI_STATUS EFIAPI SpiNorGetLbaWriteProtectInfo(
  IN  EFI_STORAGE_WP_PROTOCOL *This,
  IN  EFI_WP_CONFIG *WpConfig
 )
{
  EFI_STATUS Status = EFI_SUCCESS;
  VOID *unbuf;
  int BufferSize = sizeof(spinor_stor_scm_reg_info);
  int i;
  spinor_stor_scm_reg_info *WpInfo;

  if ((NULL == This) || (NULL == WpConfig)) 
  {
    return EFI_DEVICE_ERROR; 
  }
	
  unbuf = UncachedAllocatePool(BufferSize);
  if (unbuf == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }
  
  /* Send SCM syscall to do the actual read */
  Status = SpiNorScmSyscall(STOR_WP_GET_INFO, 0, BufferSize, unbuf);
  if (EFI_SUCCESS != Status)
  {
    DEBUG ((EFI_D_ERROR, "SPINOR Get Info with return value of 0x%x\n", Status));
  }
  else
  {
    /* Fill the WpConfig struct to return to client */
	WpInfo = (spinor_stor_scm_reg_info *) unbuf;
    WpConfig->NumEntries = 0;
	for(i=0; i<WpInfo->num_regions; i++)
	{
      WpConfig->NumEntries++;
	  WpConfig->WpEntries[i].WpType = WpInfo->entries[i].WpType;
	  WpConfig->WpEntries[i].Lba = WpInfo->entries[i].start_lba;
	  WpConfig->WpEntries[i].NumBlocks = WpInfo->entries[i].num_blocks;
	}
  }

  UncachedFreePool(unbuf);

  return Status;

}

VOID EFIAPI RegisterListener (
            IN EFI_EVENT        Event,
            IN VOID             *Context)
{
   EFI_STATUS EfiStatus = EFI_UNSUPPORTED; 

   EfiStatus = GPTListenerInit((EFI_BLOCK_IO_PROTOCOL *)Context); 
   if (EFI_SUCCESS != EfiStatus)
   {
      DEBUG ((EFI_D_ERROR, "Failed to initialize GPT Listener, Status 0x%08x\n", EfiStatus));
   }

   EfiStatus = InitPartitionConfig();
   if (EFI_SUCCESS != EfiStatus)
   {
      DEBUG ((EFI_D_ERROR, "Failed to initialize Partition config, Status 0x%08x\n", EfiStatus));
   }  
}

/**
   Driver initialization entry point.
**/
EFI_STATUS EFIAPI SpiNorDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_STATUS Status;

  /* Don't do anything if we are booting from UFS */
  if (boot_from_ufs ())
    return EFI_SUCCESS;

  /*Locate QCOM_SCM_PROTOCOL.*/
  Status = gBS->LocateProtocol(&gQcomScmProtocolGuid, NULL,
                               (VOID **)&ScmProtocol);
  if (EFI_SUCCESS != Status)
  {
    DEBUG ((EFI_D_ERROR, "No spinor support: Failed to locate SCM protocol for SPINOR %d", Status));
    return Status;
  }

  if (NULL == ScmProtocol)
  {
    DEBUG ((EFI_D_ERROR, "SpiNorDxe: ScmProtocol NULL"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }
  // get device info here and fill in blkio media
  Status = SpiNorGetInfo();
  if (EFI_SUCCESS != Status)
  {
    DEBUG ((EFI_D_ERROR, "SPINOR Get Info with return value of %x\n", Status));
    return Status;
  }

  /* Install the following protocols: BlockIo, DevicePath, EraseBlock */
  Status = gBS->InstallMultipleProtocolInterfaces (
    &gSpiNorDevice.ClientHandle,
    &gEfiBlockIoProtocolGuid, &gSpiNorDevice.BlkIo,
    &gEfiDevicePathProtocolGuid, &gSpiNorDevicePath,
    &gEfiEraseBlockProtocolGuid, &gSpiNorDevice.EraseBlk,
    &gEfiStorageWpProtocolGuid, &gSpiNorDevice.WriteProtect,
    NULL
    );
  ASSERT_EFI_ERROR(Status);

  /* Allocate memory for GPT listener */
  Status = GPTListenerAllocMem();
  if (Status != EFI_SUCCESS)
  {
     DEBUG ((EFI_D_ERROR, "Failed to allocate memory for GPT listener\n"));
     return Status; 
  }
  else
  {
     /* Register SPI-NOR GPT listener */
     RegisterListener(ScmInitialized, &gSpiNorDevice.BlkIo);
  }

  return Status;
}
