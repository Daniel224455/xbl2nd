/** @file FwCapsule.h
   
  Header for capsule related functions
  Portions of this file Copyright (c) 2011, Intel Corporation.
  All rights reserved. This program and the accompanying
  materials are licensed and made available under the terms and 
  conditions of the BSD License which accompanies this 
  distribution. The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php 

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS"
  BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND,
  EITHER EXPRESS OR IMPLIED. 
   
  Copyright (c) 2011-2013, 2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential. 

**/
/*=============================================================================
                              EDIT HISTORY
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



 when           who         what, where, why
 --------       ---         --------------------------------------------------
 2017/04/18     gparam      Changed the update status from internal type to
                            standard ones defined in EDK headers + Removed
                            unused/redundant definitions/parameters + Modified existing
                            function prototype to match the FMP capsule implementation
 2013/07/02     mic         Updated CopyRight for UXCapsule
 2012/10/04     mic         Added Data-Driven update support
 2012/07/02     mic         Added debug message support for file node
 2012/06/12     rs          Added support for BreakChangeNumber
 2012/06/05     rs          Added support for UXCapsule 
 2011/10/12     jthompso    Initial version

=============================================================================*/

#ifndef __QCOM_FW_CAPSULE_H_
#define __QCOM_FW_CAPSULE_H_

#include <Include/Library/FwCommonLib.h>

#define TZ_ENCRYPT_SIGN_LEN 8

#define QPAYLOAD_METADATA_SIGNATURE1  0x2E1946FB
#define QPAYLOAD_METADATA_SIGNATURE2  0x7F744D57

/**
 * Check if the firmware version in the payload is supported 
*
 * @param bPass -  TRUE: supported, FALSE: not supported
 * @return EFI_STATUS
 */


EFI_STATUS
FwUpdateCheckLowestSupportedVersion(
  OUT BOOLEAN  *bPass,
  OUT UINT32   *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag

  );

/**
* Main function for update capsule 
*
* @param MsftAttemptStatus  -Microsoft last attempt status 
* @param QcResultFlag       -QCFirmwareUpdate result flag
*
* @retval EFI_SUCCESS       The entry point is executed successfully.
* @retval other             Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
FwUpdateCapsulePhaseMain (
  IN  VOID                     *Image,
  IN  UINTN                     ImageSize,
  OUT UINT32                   *MsftAttemptStatus,
  OUT QCFW_UPDATE_FLAG_VALUE   *QcResultFlag
  );

#endif
