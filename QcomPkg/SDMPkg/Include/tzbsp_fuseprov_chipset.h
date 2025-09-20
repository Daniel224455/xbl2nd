/**
@file tzbsp_fuseprov_chipset.h
@brief Trustzone Fuse Provisioning chipset specific definitions/routines

This file contains the target specific information for Trustzone Fuse Provisioning

Copyright 2010,2015,2018 by Qualcomm Technologies, Inc.  All Rights Reserved

*/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2018/23/04     vk          Use SECDATA base
 2015/29/01     kpa         Update sec dat buffer offset
 2013/12/10     sm          Initial version
=============================================================================*/

#ifndef TZBSP_FUSEPROV_CHIPSET_H
#define TZBSP_FUSEPROV_CHIPSET_H

/* This are has been carved out from TZ DDR shared region */
#define TZ_RAM_DUMP_SIZE                          (0xC0000)  

#define TZBSP_SECDAT_BUFFER_BASE                  (SCL_SECDATA_BASE)
#define TZBSP_SECDAT_BUFFER_SIZE                  (SCL_SECDATA_TOTAL_SIZE)

#endif
