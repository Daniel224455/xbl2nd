/*
 * @file LZ4LibWrapper.c
 *
 * LZ4LIB compression-decompression wrapper lib 
 *  
 * Copyright (c) 2017, Qualcomm Technologies, Inc. All rights reserved.
*/
/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 10/24/17   ps      initial version

=============================================================================*/

#include <Uefi.h>
#include <Library/LZ4Lib.h>
#include "lz4.h"
#include "lz4hc.h"

/**
  Decompress default 

  @param[in]  Source  A pointer to a GUIDed section of an FFS formatted file.
  @param[in]  SourceSize is size of formatted file.
  @param[out] OutputBuffer  A pointer to a buffer that contains the result of a decode operation. 
  @param[out] compressedSize  size of decoded buffer. 

  @retval  RETURN_SUCCESS            The buffer specified by InputSection was decoded.
  @retval  RETURN_INVALID_PARAMETER  The section specified by InputSection can not be decoded.

  **/
EFI_STATUS
EFIAPI
DecompressDefault(
  IN CONST UINT8  *Source,
  IN UINTN        SourceSize,
  IN OUT UINT8    *Destination,
  IN OUT UINT64   *DecompressedSize

  )
{

  if (Source == NULL || Destination == NULL || DecompressedSize == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  *DecompressedSize = LZ4_decompress_safe((char*)Source, (char*)Destination, SourceSize, *DecompressedSize); 
  
  if(DecompressedSize == NULL)
    return EFI_INVALID_PARAMETER;
  else if( *DecompressedSize < 0)
    return EFI_INVALID_PARAMETER;
  return EFI_SUCCESS; 
}


/**
  Decompress fast 

  @param[in]  Source  A pointer to a GUIDed section of an FFS formatted file.
  @param[in]  OriginalSize is size of original FV file.
  @param[out] Destination  A pointer to a buffer that contains the result of a encode operation. 

  @retval  RETURN_SUCCESS            The buffer specified by InputSection was decoded.
  @retval  RETURN_INVALID_PARAMETER  The section specified by InputSection can not be decoded.

  **/
EFI_STATUS
EFIAPI
DecompressFast(
  IN CONST UINT8  *Source,
  IN UINTN        OriginalSize,
  IN OUT UINT8    *Destination
  )
{
  UINT8 DecompressedSize = 0;
  if (Source == NULL || Destination == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  DecompressedSize = LZ4_decompress_fast((char*)Source, (char*)Destination, OriginalSize) ;
  if(DecompressedSize < 0)
  {
    return EFI_INVALID_PARAMETER;
  }     
  return EFI_SUCCESS; 
}


/**
  Compress default

  @param[in]  Source  A pointer to a GUIDed section of an FFS formatted file.
  @param[in]  SourceSize is size of formatted file.
  @param[out] Destination  A pointer to a buffer that contains the result of a encode operation. 
  @param[out] compressedSize  size of encoded buffer. 

  @retval  RETURN_SUCCESS            The buffer specified by InputSection was decoded.
  @retval  RETURN_INVALID_PARAMETER  The section specified by InputSection can not be decoded.

  **/

EFI_STATUS
EFIAPI
CompressDefault(
  IN CONST UINT8  *Source,
  IN UINTN        SourceSize,
  IN OUT UINT8    *Destination,
  IN OUT UINT64   *CompressedSize
  )
{
   if (Source == NULL || Destination == NULL || CompressedSize == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  *CompressedSize = LZ4_compress_default((char*)Source, (char*)Destination, SourceSize, *CompressedSize); 
  if(CompressedSize == NULL)
    return EFI_INVALID_PARAMETER;
  else if(*CompressedSize < 0)
    return EFI_INVALID_PARAMETER;
  return EFI_SUCCESS; 
}

/**
  Compress fast 

  @param[in]  Source  A pointer to a GUIDed section of an FFS formatted file.
  @param[in]  SourceSize is size of formatted file.
  @param[in]  level 4-9 -acceleration.
  @param[out] Destination  A pointer to a buffer that contains the result of a encode operation. 
  @param[out] CompressedSize  size of encoded buffer. 

  @retval  RETURN_SUCCESS            The buffer specified by InputSection was decoded.
  @retval  RETURN_INVALID_PARAMETER  The section specified by InputSection can not be decoded.

  **/
EFI_STATUS
EFIAPI
CompressFast(
  IN CONST UINT8  *Source,
  IN UINTN       SourceSize,
  IN OUT UINT8    *Destination,
  IN OUT UINT64    *CompressedSize,
  IN UINT8 level
  )
{
  if (Source == NULL || Destination == NULL || CompressedSize == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  *CompressedSize = LZ4_compress_fast((char*)Source, (char*)Destination, SourceSize, *CompressedSize, level); 
   if(CompressedSize == NULL)
    return EFI_INVALID_PARAMETER;
   else if(*CompressedSize < 0)
    return EFI_INVALID_PARAMETER; 
  return EFI_SUCCESS; 
}