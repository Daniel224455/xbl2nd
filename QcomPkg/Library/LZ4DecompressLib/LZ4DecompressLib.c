/** @file
  LZ4 Decompress GUIDed Section Extraction Library.

  Copyright (c) 2017, 2018, Qualcomm Technologies Inc. All rights reserved.
  Copyright (c) 2009 - 2011, Intel Corporation. All rights reserved.
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
 
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/ExtractGuidedSectionLib.h>
#include <Library/LZ4DecompressLib.h>
#include <Library/LZ4Lib.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>


/* Scratch buffer used by zlib for dictionaries, etc. */
#define SCRATCH_BUFFER_REQUEST_SIZE   SIZE_128KB
#define MAX_DECOMPRESSED_SIZE 7000000


EFI_STATUS
EFIAPI
LZ4GetInfo (
  IN  CONST VOID  *Source,
  IN  UINT32      SourceSize,
  OUT UINT32      *DestinationSize,
  OUT UINT32      *ScratchSize
  )
{
  *DestinationSize = MAX_DECOMPRESSED_SIZE;
  *ScratchSize = SCRATCH_BUFFER_REQUEST_SIZE;
  return EFI_SUCCESS;
}


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
  If SectionAttribute is NULL, then ASSERT().


  @param[in]  InputSection       A pointer to a GUIDed section of an FFS formatted file.
  @param[out] OutputBufferSize   A pointer to the size, in bytes, of an output buffer required
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
  )
{
  ASSERT (InputSection != NULL);
  ASSERT (OutputBufferSize != NULL);
  ASSERT (ScratchBufferSize != NULL);
  ASSERT (SectionAttribute != NULL);

  if (IS_SECTION2 (InputSection)) {
    if (!CompareGuid (
        &gLZ4DecompressGuid,
        &(((EFI_GUID_DEFINED_SECTION2 *) InputSection)->SectionDefinitionGuid))) {
      return RETURN_INVALID_PARAMETER;
    }

    *SectionAttribute = ((EFI_GUID_DEFINED_SECTION2 *) InputSection)->Attributes;

    return LZ4GetInfo (
             (UINT8 *) InputSection + ((EFI_GUID_DEFINED_SECTION2 *) InputSection)->DataOffset,
             SECTION2_SIZE (InputSection) - ((EFI_GUID_DEFINED_SECTION2 *) InputSection)->DataOffset,
             OutputBufferSize,
             ScratchBufferSize
             );
  } else {
    if (!CompareGuid (
        &gLZ4DecompressGuid,
        &(((EFI_GUID_DEFINED_SECTION *) InputSection)->SectionDefinitionGuid))) {
      return RETURN_INVALID_PARAMETER;
    }

    *SectionAttribute = ((EFI_GUID_DEFINED_SECTION *) InputSection)->Attributes;

    return LZ4GetInfo (
             (UINT8 *) InputSection + ((EFI_GUID_DEFINED_SECTION *) InputSection)->DataOffset,
             SECTION_SIZE (InputSection) - ((EFI_GUID_DEFINED_SECTION *) InputSection)->DataOffset,
             OutputBufferSize,
             ScratchBufferSize
             );
  }
}

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

  @param[in]  InputSection  A pointer to a GUIDed section of an FFS formatted file.
  @param[out] OutputBuffer  A pointer to a buffer that contains the result of a decode operation. 
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
  )
{
  ASSERT (OutputBuffer != NULL);
  ASSERT (InputSection != NULL);
  UINT64 DestinationSize = MAX_DECOMPRESSED_SIZE; 
    if (IS_SECTION2 (InputSection)) {
    if (!CompareGuid (
        &gLZ4DecompressGuid,
        &(((EFI_GUID_DEFINED_SECTION2 *) InputSection)->SectionDefinitionGuid))) {
      return RETURN_INVALID_PARAMETER;
    }

    //
    // Authentication is set to Zero, which may be ignored.
    //
    *AuthenticationStatus = 0;
    return DecompressDefault (
             (UINT8 *) InputSection + ((EFI_GUID_DEFINED_SECTION2 *) InputSection)->DataOffset,
             SECTION2_SIZE (InputSection) - ((EFI_GUID_DEFINED_SECTION2 *) InputSection)->DataOffset,
             *OutputBuffer,&DestinationSize
             );
    // return DecompressFast (
    //          (UINT8 *) InputSection + ((EFI_GUID_DEFINED_SECTION2 *) InputSection)->DataOffset,
    //          DestinationSize, *OutputBuffer
    //          );
  } else {
    if (!CompareGuid (
        &gLZ4DecompressGuid,
        &(((EFI_GUID_DEFINED_SECTION *) InputSection)->SectionDefinitionGuid))) {
        return RETURN_INVALID_PARAMETER;
    }

    //
    // Authentication is set to Zero, which may be ignored.
    //
    *AuthenticationStatus = 0;

    return DecompressDefault (
             (UINT8 *) InputSection + ((EFI_GUID_DEFINED_SECTION *) InputSection)->DataOffset,
             SECTION_SIZE (InputSection) - ((EFI_GUID_DEFINED_SECTION *) InputSection)->DataOffset,
             *OutputBuffer,&DestinationSize
    );
 // return DecompressFast (
 //             (UINT8 *) InputSection + ((EFI_GUID_DEFINED_SECTION2 *) InputSection)->DataOffset,
 //             DestinationSize, *OutputBuffer
 //             );
  }
}


/**
  Register LZ4Decompress and LZ4GetInfo handlers with LZ4DecompressGuid.

  @retval  RETURN_SUCCESS            Register successfully.
  @retval  RETURN_OUT_OF_RESOURCES   No enough memory to store this handler.
**/
EFI_STATUS
EFIAPI
LZ4DecompressLibConstructor (
  )
{
  return ExtractGuidedSectionRegisterHandlers (
          &gLZ4DecompressGuid,
          LZ4GuidedSectionGetInfo,
          LZ4GuidedSectionDecompress
          );      
}

