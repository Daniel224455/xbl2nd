/** @file DppUtil.c
   
  Qualcomm Dpp tool application

  Copyright (c) 2010-2013, Qualcomm Technologies, Inc. All rights reserved.
  Qualcomm Technologies Proprietary and Confidential.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/16/13   mic     Initial version
=============================================================================*/
#include "DppUtil.h"
#include "CommonUtil.h"

#define OUTPUT_DIR_NAME   L"ProvDump"

static DPP_PARTITION_TYPE               gDppPartType                 = DPP_PARTITION_TYPE_UNKNOWN;
static UINT32                           gDppBlockSize                = 0;
static UINT64                           gDppSize                     = 0;
static EFI_BLOCK_IO_PROTOCOL           *gDppBlockIo                  = NULL;

static EFI_GUID gEfiDppBlockPartitionGuid = 
{ 0xF97B8793, 0x3ABF, 0x4719,{ 0x89, 0x6B, 0x7C, 0x3E, 0x9B, 0x85, 0xE1, 0x04 } };

static EFI_GUID gEfiDppFatPartitionGuid = 
  {0xebd0a0a2, 0xb9e5, 0x4433, {0x87, 0xc0, 0x68, 0xb6, 0xb7, 0x26, 0x99, 0xc7}};

EFI_STATUS
BlockDppProtocolInit(
  IN EFI_HANDLE         DppHandle
  )
{
    EFI_STATUS  Status = EFI_SUCCESS;
  
    if(DppHandle == NULL)
    {
      Status = EFI_UNSUPPORTED;
      goto BlockDppProtocolInit;
    }
  
    // Get Block IO protocol interface to DPP handle
    Status = gBS->HandleProtocol(DppHandle,
                                 &gEfiBlockIoProtocolGuid,
                                 (VOID**)&gDppBlockIo);
    HANDLE_ERROR_LABEL(Status,gBS->HandleProtocol,BlockDppProtocolInit);

    
    gDppBlockSize = gDppBlockIo->Media->BlockSize;
    if(gDppBlockSize == 0)
    {
      gDppBlockSize = 512;
    }
  
    gDppSize = gDppBlockIo->Media->BlockSize * ((gDppBlockIo->Media->LastBlock) + 1);
    if(gDppSize == 0)
    {
      gDppSize = 16 * 1024 * 1024;
    }
    
BlockDppProtocolInit:

    return Status;
}

/**
  Initialize global variables which will be used to access DPP
**/
EFI_STATUS
DppGvariableInit(VOID)
{
  EFI_STATUS     Status       = EFI_SUCCESS;
  EFI_HANDLE     PartHandle   = NULL;
  
  /* Search if Block-based DPP exist? if not, check if Fat-based DPP exist */
  Status = GetPartitionHandle(&PartHandle,
                              &gEfiDppBlockPartitionGuid,
                              DPP_NAME,
                              &gEfiBlockIoProtocolGuid);
  if(!EFI_ERROR(Status) && (PartHandle != NULL))
  {
    Status = BlockDppProtocolInit(PartHandle);
    HANDLE_ERROR_LABEL(Status, BlockDppProtocolInit, GetDppTypeExit);
    gDppPartType = DPP_PARTITION_TYPE_BLOCK;

  }
  else if (Status == EFI_NOT_FOUND)
  {
    Status = GetPartitionHandle(&PartHandle,
                                &gEfiDppFatPartitionGuid,
                                DPP_NAME,
                                &gEfiSimpleFileSystemProtocolGuid);
    if(!EFI_ERROR(Status) && (PartHandle != NULL))
    {
      gDppPartType = DPP_PARTITION_TYPE_FAT;
    }
    else
    {
      Status = EFI_UNSUPPORTED;
      AsciiPrint("Warning:  DPP not found \n");
      goto GetDppTypeExit;
    }
  }
  else
  {
    Status = EFI_UNSUPPORTED;
    AsciiPrint("Warning: Block DPP unsupported platform type \n");
    goto GetDppTypeExit;
  }

  /* initialized required global variable */
  if (gDppPartType == DPP_PARTITION_TYPE_BLOCK)
  {
    HANDLE_ERROR_LABEL(Status, GetDppInfo, GetDppTypeExit);
    AsciiPrint( "Block DPP detected\n");
  }
  else if (gDppPartType == DPP_PARTITION_TYPE_FAT)
  {
    AsciiPrint( "Fat DPP detected\n");
  }
  else
  { /* shouldn't happen */
    AsciiPrint( "unknown DPP detected\n");
  }

GetDppTypeExit:

return Status;
}

EFI_STATUS
DppItemRead (
  IN  CHAR16       *Type,
  IN  CHAR16       *Name,
  OUT UINT8       **Buff,
  OUT UINT32       *Size
)
{
  EFI_STATUS        Status      = EFI_SUCCESS;
  EFI_DPP_PROTOCOL *DppProtocol = NULL;
	UINTN            BuffSize = 0;


  if( (Buff == NULL) || (Type == NULL) ||
      (Name == NULL) || (Size == NULL))
  {
    Status = EFI_INVALID_PARAMETER;
    goto DppItemReadExit;
  }
  /* Get DPP protocol interface */
  Status = gBS->LocateProtocol(&gEfiDppProtocolGuid,
                               NULL,
                               (VOID**)&DppProtocol);
  HANDLE_ERROR_LABEL(Status, gBS->LocateProtocol, DppItemReadExit);

  /* use Dpp protocol to get required buffer size first */
  Status = DppProtocol->GetDPP( DppProtocol, Type, Name, NULL, &BuffSize);
  HANDLE_ERROR_LABEL(Status, DppProtocol->GetDPP, DppItemReadExit);

  /* allocate required buffer */
  *Buff = (UINT8 *) AllocatePool(BuffSize);
  if(*Buff == NULL)
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto DppItemReadExit;
  }

  *Size = BuffSize;

  /* read dpp item */
  Status = DppProtocol->GetDPP( DppProtocol, Type, Name, *Buff, &BuffSize);
  HANDLE_ERROR_LABEL(Status, DppProtocol->GetDPP, DppItemReadExit);

DppItemReadExit:

  return Status;
}

EFI_STATUS
DppItemWrite (
  IN  CHAR16       *Type,
  IN  CHAR16       *Name,
  IN UINT8         *Buff,
  IN UINT32         Size
  )
{
  EFI_STATUS        Status      = EFI_SUCCESS;
  EFI_DPP_PROTOCOL *DppProtocol = NULL;
  
  if( (Buff == NULL) ||
      (Type == NULL) ||
      (Name == NULL))
  {
    Status = EFI_INVALID_PARAMETER;
    goto DppItemWriteExit;
  }

  /* Get DPP protocol interface */
  Status = gBS->LocateProtocol(&gEfiDppProtocolGuid,
                               NULL,
                               (VOID**)&DppProtocol);
  HANDLE_ERROR_LABEL(Status, gBS->LocateProtocol, DppItemWriteExit);

  /* write dpp item*/
  Status = DppProtocol->SetDPP( DppProtocol, Type, Name, Buff, Size);
  HANDLE_ERROR_LABEL(Status, DppProtocol->SetDPP, DppItemWriteExit);

DppItemWriteExit:

  return Status;
}

EFI_STATUS
DppItemDelete (
  IN  CHAR16       *Type,
  IN  CHAR16       *Name
  )
{

  EFI_STATUS        Status      = EFI_SUCCESS;
  EFI_DPP_PROTOCOL *DppProtocol = NULL;
  
  if( (Type == NULL) ||
      (Name == NULL))
  {
    Status = EFI_INVALID_PARAMETER;
    goto DppItemDeleteExit;
  }

  /* Get DPP protocol interface */
  Status = gBS->LocateProtocol(&gEfiDppProtocolGuid,
                               NULL,
                               (VOID**)&DppProtocol);
  HANDLE_ERROR_LABEL(Status, gBS->LocateProtocol, DppItemDeleteExit);

  /* Dpp delete*/
  Status = DppProtocol->DeleteItemDPP(DppProtocol, Type, Name);
  HANDLE_ERROR_LABEL(Status, DppProtocol->DeleteItemDPP, DppItemDeleteExit);

DppItemDeleteExit:

  return Status;

}

EFI_STATUS
DppErase (VOID)
{
  EFI_STATUS        Status      = EFI_SUCCESS;
  EFI_DPP_PROTOCOL *DppProtocol = NULL;
  
  /* Get DPP protocol interface */
  Status = gBS->LocateProtocol(&gEfiDppProtocolGuid,
                               NULL,
                               (VOID**)&DppProtocol);
  HANDLE_ERROR_LABEL(Status, gBS->LocateProtocol, DppEraseExit);

  /* erase Dpp*/
  Status = DppProtocol->EraseDPP(DppProtocol);
  HANDLE_ERROR_LABEL(Status, DppProtocol->EraseDPP, DppEraseExit);

DppEraseExit:

  return Status;
}


EFI_STATUS
ListAllDppItems(VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;

   switch (gDppPartType)
   {
      case DPP_PARTITION_TYPE_FAT:
       AsciiPrint("  List FAT not supported yet\n\n");
        break;

      case DPP_PARTITION_TYPE_BLOCK: 
		AsciiPrint("  List BLOCK not supported yet\n\n");
        break;
      default:
       Status = EFI_UNSUPPORTED;
       AsciiPrint("  Not supported yet\n\n");

         break;
   }   

  return Status;
}
