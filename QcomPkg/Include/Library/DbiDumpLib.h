/**
  @file DbiDumpLib.h
 
  SDI ram dumps 

  Copyright (c) 2016,2019 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Confidential and Proprietary.
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/19/19   vb      fix CR2396170 : apreg info missing in hyper-v enabled builds
 10/24/16   ai      Initial Version

 =============================================================================*/

#ifndef _DBIDUMPLIB_H_
#define _DBIDUMPLIB_H_

#include <Library/DBIDump.h>
#include <Library/UefiBootServicesTableLib.h>

/* Setup DBI Dump Table only */
VOID SetupDBIDumpTable();

/* Init DBI Dump Table and set crashdump cookie */
EFI_STATUS InitDbiDump(); 

/* Save Cpu Context to UefiInfoBlock */
EFI_STATUS SaveUefiContext(EFI_SYSTEM_CONTEXT SystemContext);

#endif /* _DBIDUMPLIB_H_ */

