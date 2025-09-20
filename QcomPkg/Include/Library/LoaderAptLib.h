/*==============================================================================
  @file LoaderAptLib.h

  APT LoaderTest Lib api's .

         Copyright (c) 2015-2018 QUALCOMM Technologies, Inc.
               All Rights Reserved.
     Qualcomm Technologies Inc. Proprietary and Confidential.


when       who          what, where, why
--------   ---          -------------------------------------------------- 
05/30/18   vg           Added apt_sti_init params
06/18/15   kedara       Initial version  

============================================================================*/ 


#ifndef LOADER_APT_LIB_H
#define LOADER_APT_LIB_H

#include "com_dtypes.h"

/* ============================================================================
**  Function : apt_sti_init
** ============================================================================
*/
/*!
 * @brief Api to initialize STI image shared function table and jump to it.
 *  Note: Function does not return
 *
 * @param[in] sti_entry_point  Entry point for STI image 
 *
 * @return None
 *
 */
void apt_sti_init(uintnt entry_point, bl_shared_data_type *bl_shared_data);

#endif /* LOADER_APT_LIB_H */
