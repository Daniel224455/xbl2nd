/** @file  LZ4Lib.h                                                                                    
 LZ4 decompression 

Copyright (c) 2017, Qualcomm Technologies, Inc. All rights reserved.                                         
    

**/

#ifndef __LZ4_DECOMPRESS_H__
#define __LZ4_DECOMPRESS_H__

#include <Uefi.h>

/**
  Decompresses a LZ4 compressed source buffer.                                                     

  PRECONDITION: Destination buffer and Scratch buffer must be at least the size
                returned by LZ4GetInfo()
  
  Extracts decompressed data to its original form.
  If the compressed source data specified by Source is successfully decompressed 
  into Destination, then EFI_SUCCESS is returned.  If the compressed source data 
  specified by Source is not in a valid compressed data format,
  then EFI_INVALID_PARAMETER is returned.

  @param  Source      The source buffer containing the compressed data.
  @param  SourceSize  The size of source buffer.
  @param  Destination The destination buffer to store the decompressed data
  @param  DecompressedSize   size of decompressed data
                     
  @retval  EFI_SUCCESS Decompression completed successfully, and 
                          the uncompressed buffer is returned in Destination.
  @retval  EFI_INVALID_PARAMETER 
                          The source buffer specified by Source is corrupted 
                          (not in a valid compressed format).
**/




EFI_STATUS
EFIAPI
DecompressDefault(
  IN CONST UINT8  *Source,
  IN UINTN       SourceSize,
  IN OUT UINT8    *Destination,
  IN OUT UINT64    *DecompressedSize
  );

/**
  Decompresses a LZ4 compressed source buffer.                                                     

  PRECONDITION: Destination buffer and Scratch buffer must be at least the size
                returned by LZ4GetInfo()
  
  Extracts decompressed data to its original form.
  If the compressed source data specified by Source is successfully decompressed 
  into Destination, then EFI_SUCCESS is returned.  If the compressed source data 
  specified by Source is not in a valid compressed data format,
  then EFI_INVALID_PARAMETER is returned.

  @param  Source      The source buffer containing the compressed data.
  @param  OriginalSize   size of decompressed data
  @param  Destination The destination buffer to store the decompressed data
  
                     
  @retval  EFI_SUCCESS Decompression completed successfully, and 
                          the uncompressed buffer is returned in Destination.
  @retval  EFI_INVALID_PARAMETER 
                          The source buffer specified by Source is corrupted 
                          (not in a valid compressed format).
**/

EFI_STATUS
EFIAPI
DecompressFast(
  IN CONST UINT8  *Source,
  IN UINTN       OriginalSize,
  IN OUT UINT8    *Destination
  );

/**
  Compresses a LZ4 compressed source buffer.                                                     

  PRECONDITION: Destination buffer and Scratch buffer must be at least the size
                returned by LZ4GetInfo()
  
  Extracts decompressed data to its original form.
  If the compressed source data specified by Source is successfully decompressed 
  into Destination, then EFI_SUCCESS is returned.  If the compressed source data 
  specified by Source is not in a valid compressed data format,
  then EFI_INVALID_PARAMETER is returned.

  @param  Source      The source buffer containing the compressed data.
  @param  SourceSize  The size of source buffer.
  @param  Destination The destination buffer to store the decompressed data
  @param  CompressedSize   size of compressed data
                     
  @retval  EFI_SUCCESS Decompression completed successfully, and 
                          the uncompressed buffer is returned in Destination.
  @retval  EFI_INVALID_PARAMETER 
                          The source buffer specified by Source is corrupted 
                          (not in a valid compressed format).
**/

EFI_STATUS
EFIAPI
CompressDefault(
  IN CONST UINT8  *Source,
  IN UINTN        SourceSize,
  IN OUT UINT8    *Destination,
  IN OUT UINT64   *CompressedSize
  );
/**
  Compresses a LZ4 compressed source buffer.                                                     

  PRECONDITION: Destination buffer and Scratch buffer must be at least the size
                returned by LZ4GetInfo()
  
  compresses  decompressed data to its compressed form.


  @param  Source      The source buffer containing the decompressed data.
  @param  SourceSize  The size of source buffer.
  @param  Destination The destination buffer to store the compressed data
  @param  CompressedSize   size of compressed data
  @param  level       The level of compression 4-9
                     
  @retval  EFI_SUCCESS Decompression completed successfully, and 
                          the uncompressed buffer is returned in Destination.
  @retval  EFI_INVALID_PARAMETER 
                          The source buffer specified by Source is corrupted 
                          (not in a valid compressed format).
**/
EFI_STATUS
EFIAPI
CompressFast(
  IN CONST UINT8  *Source,
  IN UINTN        SourceSize,
  IN OUT UINT8    *Destination,
  IN OUT UINT64   *CompressedSize,
  IN UINT8 level
  );

#endif /* __LZ4_DECOMPRESS_H__ */
