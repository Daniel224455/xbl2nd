/*===============================================================================
  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         pil_loader.h
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  06/14/17   yw         initial version

================================================================================*/

#ifndef __PIL_LOADER_H__
#define __PIL_LOADER_H__

#include<elf.h>
#include<pil_tz.h>
#include<assert.h>

#define FW_IMAGES_PATH    "\\image\\"

#define   SZ_4K                        0x00001000
#define   SZ_1M                        0x00100000
#define   SZ_4M                        0x00400000

PIL_STATUS PilSetupMemoryRange(PIL_SUBSYS_CTX_TYPE*);
PIL_STATUS PilLoadSegments(PIL_SUBSYS_CTX_TYPE*);
PIL_STATUS PilDeleteMemoryRange(PIL_SUBSYS_CTX_TYPE*);
PIL_STATUS PilLoadMdtFile(PIL_SUBSYS_CTX_TYPE*);
PIL_STATUS PilValidateMetadata(PIL_SUBSYS_CTX_TYPE*);
PIL_STATUS PilLoadElfFile(PIL_SUBSYS_CTX_TYPE*);
EFI_STATUS PilScmStdCall(PIL_SUBSYS_CTX_TYPE*, UINT32, UINT32,...);
PIL_STATUS CopyMetaDataFromLoadedElf(PIL_SUBSYS_CTX_TYPE*);
PIL_STATUS PilCleanUp(PIL_SUBSYS_CTX_TYPE*);
PIL_STATUS PilAuthAndReset(PIL_SUBSYS_CTX_TYPE*);
PIL_STATUS PilPostLoad(PIL_SUBSYS_CTX_TYPE*);

#endif
