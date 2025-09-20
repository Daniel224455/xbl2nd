/**
@file SmbiosLib.h
@brief SMBIOS chipset specific definitions/routines

This file contains the target specific information for SMBIOS

Copyright 2018, 2019 by Qualcomm Technologies, Inc.  All Rights Reserved

*/

/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 08/08/22        nauduri    Revert back reading PTE fuse changes 
 09/24/21        rc         Add support for reading PTE fuse
 12/02/19        rc         Add support for processor version string
 04/10/19        vb         Add support for reading APQ feature_id fuse information
 01/20/19        vb         Add support for reading feature_id fuse information
 2018/08/02     ltg         Initial version
=============================================================================*/

#ifndef SMBIOS_LIB_TARGET_H
#define SMBIOS_LIB_TARGET_H

UINT32 SmbiosGetFeatureID();
EFI_STATUS smbiosGetMaxSpeed(UINT16 *maxSpeed);
EFI_STATUS smbiosGetDDRSpeed(UINT16 *DDRSpeed);
EFI_STATUS smbiosGetProcVersionStr(CHAR8 *ProcessorVersionString);

#endif
