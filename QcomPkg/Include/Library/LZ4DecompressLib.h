
/** @file  LZ4GuidedSectionExtraction.h                                                                                    
 LZ4 GUIDed section Decompression 

Copyright (c) 2018, Qualcomm Technologies, Inc. All rights reserved. 
Portions Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under 
the terms and conditions of the BSD License that accompanies this distribution.  
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.                                          
    
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#ifndef __LZ4_DECOMPRESS1_H__
#define __LZ4_DECOMPRESS1_H__

#include <Uefi.h>

///
/// The Global ID used to identify a section of an FFS file of type 
/// EFI_SECTION_GUID_DEFINED, whose contents have been compressed using 
/// LZ4 compression.

//361DBF8B-C83B-4D79-AF06-50057A04DDF3
#define LZ4_DECOMPRESS_GUID  \
  { 0x361DBF8B, 0xC83B, 0x4D79, { 0xAF, 0x06, 0x50, 0x05, 0x7A, 0x04, 0xDD, 0xF3 } }

extern GUID gLZ4DecompressGuid;

/**                                                                                                 
  Examines a GUIDed section and returns the size of the decoded buffer and the
  size of an scratch buffer required to actually decode the data in a GUIDed section.

  Examines a GUIDed section specified by InputSection.  
  If GUID for InputSection does not match the GUID that this handler supports,
  then RETURN_UNSUPPORTED is returned.  
  If the required information can not be retrieved from InputSection,
  then RETURN_INVALID_PARAMETER is returned.
  If the GUID of InputSection does match the GUID that this handler supports,
  then the size required to hold the decoded buffer is returned in OututBufferSize,
  the size of an optional scratch buffer is returned in ScratchSize, and the Attributes field
  from EFI_GUID_DEFINED_SECTION header of InputSection is returned in SectionAttribute.
  
  If InputSection is NULL, then ASSERT().
  If OutputBufferSize is NULL, then ASSERT().
  If ScratchBufferSize is NULL, then ASSERT().
  If SectionAttribute is NULL, then ASSERT().


  @param[in]  InputSection       A pointer to a GUIDed section of an FFS formatted file.
  @param[out] OutputBufferSize   A pointer to the size, in bytes, of an output buffer required
                                 if the buffer specified by InputSection were decoded.
  @param[out] ScratchBufferSize  A pointer to the size, in bytes, required as scratch space
                                 if the buffer specified by InputSection were decoded.
  @param[out] SectionAttribute   A pointer to the attributes of the GUIDed section. See the Attributes
                                 field of EFI_GUID_DEFINED_SECTION in the PI Specification.

  @retval  RETURN_SUCCESS            The information about InputSection was returned.
  @retval  RETURN_UNSUPPORTED        The section specified by InputSection does not match the GUID this handler supports.
  @retval  RETURN_INVALID_PARAMETER  The information can not be retrieved from the section specified by InputSection.

**/
RETURN_STATUS
EFIAPI
LZ4GuidedSectionGetInfo (
  IN  CONST VOID  *InputSection,
  OUT UINT32      *OutputBufferSize,
  OUT UINT32      *ScratchBufferSize,
  OUT UINT16      *SectionAttribute
  );


/**                                                                                                 
  Decompress a LZ4 compressed GUIDed section into a caller allocated output buffer.
  
  Decodes the GUIDed section specified by InputSection.  
  If GUID for InputSection does not match the GUID that this handler supports, then RETURN_UNSUPPORTED is returned.  
  If the data in InputSection can not be decoded, then RETURN_INVALID_PARAMETER is returned.
  If the GUID of InputSection does match the GUID that this handler supports, then InputSection
  is decoded into the buffer specified by OutputBuffer and the authentication status of this
  decode operation is returned in AuthenticationStatus.  If the decoded buffer is identical to the
  data in InputSection, then OutputBuffer is set to point at the data in InputSection.  Otherwise,
  the decoded data will be placed in caller allocated buffer specified by OutputBuffer.
  
  If InputSection is NULL, then ASSERT().
  If OutputBuffer is NULL, then ASSERT().
  If ScratchBuffer is NULL and this decode operation requires a scratch buffer, then ASSERT().
  If AuthenticationStatus is NULL, then ASSERT().


  @param[in]  InputSection  A pointer to a GUIDed section of an FFS formatted file.
  @param[out] OutputBuffer  A pointer to a buffer that contains the result of a decode operation. 
  @param[out] ScratchBuffer A caller allocated buffer that may be required by this function
                            as a scratch buffer to perform the decode operation. 
  @param[out] AuthenticationStatus 
                            A pointer to the authentication status of the decoded output buffer.
                            See the definition of authentication status in the EFI_PEI_GUIDED_SECTION_EXTRACTION_PPI
                            section of the PI Specification. EFI_AUTH_STATUS_PLATFORM_OVERRIDE must
                            never be set by this handler.
                                                                                                    
  @retval  RETURN_SUCCESS            The buffer specified by InputSection was decoded.
  @retval  RETURN_UNSUPPORTED        The section specified by InputSection does not match the GUID this handler supports.
  @retval  RETURN_INVALID_PARAMETER  The section specified by InputSection can not be decoded.

**/
RETURN_STATUS
EFIAPI
LZ4GuidedSectionDecompress (
  IN CONST  VOID    *InputSection,
  OUT       VOID    **OutputBuffer,
  OUT       VOID    *ScratchBuffer1,        OPTIONAL
  OUT       UINT32  *AuthenticationStatus
  );


/**                                                                                                 
  Given a LZ4 compressed source buffer, this function retrieves the size of 
  the uncompressed buffer and the size of the scratch buffer required 
  to decompress the compressed source buffer.

  Retrieves the size of the uncompressed buffer and the temporary scratch buffer 
  required to decompress the buffer specified by Source and SourceSize.
  The size of the uncompressed buffer is returned in DecompressedSize, 
  the size of the scratch buffer is returned in ScratchSize, and EFI_SUCCESS is returned.

  @param  Source          The source buffer containing the compressed data.
  @param  SourceSize      The size, in bytes, of the source buffer.
  @param  DecompressedSize A pointer to the size, in bytes, of the uncompressed buffer
                          that will be generated when the compressed buffer specified
                          by Source and SourceSize is decompressed.
  @param  ScratchSize     A pointer to the size, in bytes, of the scratch buffer that
                          is required to decompress the compressed buffer specified 
                          by Source and SourceSize.

  @retval  EFI_SUCCESS    The size of the uncompressed data was returned 
                          in DecompressedSize and the size of the scratch 
                          buffer was returned in ScratchSize.

**/
EFI_STATUS
EFIAPI
LZ4GetInfo (
  IN  CONST VOID  *Source,
  IN  UINT32      SourceSize,
  OUT UINT32      *DecompressedSize,
  OUT UINT32      *ScratchSize
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
  @param  SourceSize  The size of source buffer.
  @param  Destination The destination buffer to store the decompressed data
  @param  DecompressedSize   size of decompressed data
                     
  @retval  EFI_SUCCESS Decompression completed successfully, and 
                          the uncompressed buffer is returned in Destination.
  @retval  EFI_INVALID_PARAMETER 
                          The source buffer specified by Source is corrupted 
                          (not in a valid compressed format).
**/

#endif