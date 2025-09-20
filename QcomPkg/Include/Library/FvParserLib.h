/** @file FvParserLib.h

  FvUtils lib to parse FV in memory and mount
   
  Copyright (c) 2017-2020, Qualcomm Technologies, Inc. All rights reserved. 
  Qualcomm Technologies Proprietary and Confidential.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 07/02/19   kpa     Initial Revision

=============================================================================*/

#include <Uefi.h>

#include <Library/BaseLib.h>

#ifndef __PARSEFV_LIB_H__
#define __PARSEFV_LIB_H__

/*=============================================================================
  
             In memory FV parsing and mounting routines
  
 =============================================================================*/

/******************************************************************************
 *
 *    Auxilliary functions to get fine control over traversing with
 *    in memory FV. If the need is not complex, use the simpler routine
 *    ProcessAndMountFvImageFile instead of these.
 *
 ******************************************************************************/

#define FFS_FILE_ATTRIBUTE_COMPRESSED    (1 << 30)

#define MAX_FFS_FILE_SIZE_CHARS           32

/* If not multiple contexts are opened at the same time, this defines
 * how many levels deep does the lib recurse into */
#define MAX_FV_CONTEXTS                   4

typedef struct __FfsFileInfo
{
  EFI_GUID     FileNameGuid;
  UINT32       FileSize;
  UINT32       FileType;
  CHAR8        UIFileName[MAX_FFS_FILE_SIZE_CHARS];
  CONST VOID*  ROFileDataPtr;
  CONST VOID*  SectionPtr;
  UINT32       Attributes;
}FfsFileInfo;

struct __FVContext;
typedef struct __FVContext FVContext;

/*
 *  Open the FV Buffer context. If the FV is valid, FvSizePtr can return
 *  the actual size of the FV contained
 * */
FVContext* 
OpenFVFile (IN  VOID       *FVBuffer, 
            IN  UINTN       BufferSize, 
            OUT UINTN      *FvSizePtr OPTIONAL);

/*
 * Close the FV context
 * */
VOID 
CloseFVFile (IN FVContext* Ctxt);

/*
 * Get Next file in the FV (First file if calling for the first time).
 * */
FfsFileInfo*
GetNextFileFromFV (IN     FVContext          *Ctxt, 
                   IN OUT FfsFileInfo        *FileInfoPtr, 
                   IN     UINT8               FilterFileType OPTIONAL);



#endif /* __PARSEFV_LIB_H__ */

