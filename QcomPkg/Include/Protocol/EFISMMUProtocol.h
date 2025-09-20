/** @file EFISMMUProtocol.h

  SMMU Protocol for UEFI

  Copyright (c) 2010-2011,2014 Qualcomm Technologies, Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/17/15   amo     Created

=============================================================================*/

#ifndef __SMMU_PROTOCOL_H__
#define __SMMU_PROTOCOL_H__


#define SMMU_REVISION 0x0000000000000001

typedef 
void
(EFIAPI *EFI_SMMU_PROTOCOL_INIT)(
   OUT   SMMU_fptable_t            **p_fpTable
   );

typedef 
void
(EFIAPI *EFI_SMMU_PROTOCOL_DEINIT)(
   VOID
   );

typedef struct _EFI_SMMU_PROTOCOL {
   UINT64                       revision;
   EFI_SMMU_PROTOCOL_INIT       SMMU_protocol_init;
   EFI_SMMU_PROTOCOL_DEINIT     SMMU_protocol_deinit;
} 
EFI_SMMU_PROTOCOL; 

extern EFI_GUID gEfiSmmuProtocolGuid;

#endif /*__SMMU_PROTOCOL_H__*/
