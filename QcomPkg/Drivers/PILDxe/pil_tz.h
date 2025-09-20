/*===============================================================================
  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         pil_tz.h
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  06/14/17   yw         initial version

================================================================================*/

#ifndef __PIL_TZ_H__
#define __PIL_TZ_H__

#include<pil_ctx.h>
#include<pil_log.h>
#include<elf.h>

#define FWNAME_SHORT_LEN            8


/* struct to fill out image load info in IMEM */
#pragma pack(push)
#pragma pack(4)
typedef struct _PIL_IMAGE_INFO_TYPE_
{
    CHAR8 Name[FWNAME_SHORT_LEN]; /* image name, i.e. adsp, cdsp, etc */
    UINT64 Start;          /* start  address of the image loading */
    UINT32 Size;           /* loading size of the image */
} PIL_IMAGE_INFO_TYPE;
#pragma pack(pop)


typedef PIL_STATUS (*PIL_FUNCTION) (IN PIL_SUBSYS_CTX_TYPE*);
	
typedef struct _PIL_METHODS_TYPE_
{
	PIL_FUNCTION  PreLoad;
	PIL_FUNCTION  LoadEntireImage;
	PIL_FUNCTION  LoadMetaData;
	PIL_FUNCTION  ValidateMetaData;
	PIL_FUNCTION  SetupMemoryRange;
	PIL_FUNCTION  LoadSegments;
	PIL_FUNCTION  AuthAndReset;
	PIL_FUNCTION  PostLoad;
} PIL_METHODS_TYPE;

PIL_STATUS PilProcessImageExt(PIL_SUBSYS_CFG_TYPE*);
PIL_STATUS PilProcessImage(CHAR16*);
PIL_SUBSYS_CTX_TYPE* FindSubsysCtx(PIL_SUBSYS_CFG_TYPE*);

#endif