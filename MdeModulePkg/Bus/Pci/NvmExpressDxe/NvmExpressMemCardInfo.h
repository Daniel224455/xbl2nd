/** @file
  Header file for EFI_MEM_CARDINFO_PROTOCOL interface.

  Copyright (c) 2017 Qualcomm Technologies, Incorporated. 
  All rights reserved. 
  Qualcomm Technologies, Confidential and Proprietary.

**/

#ifndef _NVME_MEMCARDINFO_H_
#define _NVME_MEMCARDINFO_H_

/**
  Initialize the installation of MemCardInfo protocol. 

  @param[in]  Device  The pointer of NVME_DEVICE_PRIVATE_DATA.

**/
VOID
InitializeMemCardInfo (
  IN  NVME_DEVICE_PRIVATE_DATA    *Device
  );

/**
  This function is used to get the card information. 

  @param[in]   This              Pointer to the
                                 EFI_MEM_CARDINFO_PROTOCOL
                                 instance.
  @param[out]  CardInfo          Pointer to a variable that the
                                 driver returns with the
                                 retrieved card information; see
                                 #_MEM_CARD_INFO for details.
   
  @retval EFI_SUCCESS           Function completed successfully.
  @retval EFI_INVALID_PARAMETER Parameter is invalid.

**/
EFI_STATUS
EFIAPI
NvmExpressGetCardInfo (
   IN  EFI_MEM_CARDINFO_PROTOCOL *This,
   OUT MEM_CARD_INFO*   CardInfo
  );

/**
  This function is used to get the BOOT LU.

  @param[in]   This              Pointer to the
                                 EFI_MEM_CARDINFO_PROTOCOL
                                 instance.
  @param[out]  BootLU            Pointer to a variable
                                 that indicates the BOOT LU (0 =
                                 disable BOOT LU, 1 = BOOT for
                                 A, 2 = BOOT for B)

  @retval EFI_SUCCESS           Function completed successfully.
  @retval EFI_INVALID_PARAMETER Parameter is invalid.

**/
EFI_STATUS
EFIAPI
NvmExpressGetBootLU (
   IN   EFI_MEM_CARDINFO_PROTOCOL *This,
   OUT  UINT32                    *BootLU
  );

/**
  This function is used to set the BOOT LU

  @param[in]   This              Pointer to the
                                 EFI_MEM_CARDINFO_PROTOCOL
                                 instance.
  @param[in]   BootLU            Boot LU to switch to
                                 (0 = disable BOOT LU, 1 = BOOT
                                 for A, 2 = BOOT for B)

  @retval EFI_SUCCESS           Function completed successfully.
  @retval EFI_INVALID_PARAMETER Parameter is invalid.

**/
EFI_STATUS
EFIAPI
NvmExpressSetBootLU (
   IN  EFI_MEM_CARDINFO_PROTOCOL *This,
   IN  UINT32                    BootLU
  );

#endif
