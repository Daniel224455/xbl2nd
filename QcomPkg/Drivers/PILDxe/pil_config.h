/*===============================================================================
  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         pil_config.h
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  06/14/17   yw         initial version

================================================================================*/

#ifndef __PIL_CONFIG_H__
#define __PIL_CONFIG_H__

#include <Protocol/EFIPIL.h>
#include <scm_sip_interface.h>

#define PIL_CONFIG_FILE L"uefipil.cfg"

#define BUILD_VAR_NAME(b,s,n)  \
       AsciiStrToUnicodeStr(s, b); \
       StrnCatS(b, NAME_MAX_LENGTH, n, NAME_MAX_LENGTH)

PIL_SUBSYS_CFG_TYPE* PilInitCfgByUnicodeName(CHAR16*);
EFI_STATUS InitImageList(CHAR8*);
PIL_SUBSYS_CFG_TYPE* PilDuplicateCfg(PIL_SUBSYS_CFG_TYPE*);
#endif