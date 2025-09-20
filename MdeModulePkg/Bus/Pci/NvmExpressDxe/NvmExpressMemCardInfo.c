/** @file
  This file is used to implement the EFI_MEM_CARDINFO_PROTOCOL 
  interface. 

  Copyright (c) 2017 Qualcomm Technologies, Incorporated. 
  All rights reserved. 
  Qualcomm Technologies, Confidential and Proprietary.

**/

#include "NvmExpress.h"

EFI_MEM_CARDINFO_PROTOCOL gNvmExpressMemCardInfoProtocolTemplate = {
  EFI_MEM_CARD_INFO_PROTOCOL_REVISION,
  NvmExpressGetCardInfo, 
  NvmExpressGetBootLU,
  NvmExpressSetBootLU
};

/**
  Initialize the installation of MemCardInfo protocol. 

  @param[in]  Device  The pointer of NVME_DEVICE_PRIVATE_DATA.

**/
VOID
InitializeMemCardInfo (
  IN  NVME_DEVICE_PRIVATE_DATA    *Device
  )
{
  CopyMem (&Device->MemCardInfo, &gNvmExpressMemCardInfoProtocolTemplate, sizeof (EFI_MEM_CARDINFO_PROTOCOL));
}

/**
  This function is used to get the card information. 

  @param[in]   This              Pointer to the
                                 EFI_MEM_CARDINFO_PROTOCOL
                                 instance.
  @param[out]  card_info         Pointer to a variable that the driver returns
                                 with the retrieved card information; see
                                 #_MEM_CARD_INFO for details.
   
  @retval EFI_SUCCESS           Function completed successfully.
  @retval EFI_INVALID_PARAMETER Parameter is invalid.

**/
EFI_STATUS
EFIAPI
NvmExpressGetCardInfo (
   IN  EFI_MEM_CARDINFO_PROTOCOL *This,
   OUT MEM_CARD_INFO*   card_info
  )
{
   if ((This == NULL) || (card_info == NULL)) {
      return EFI_INVALID_PARAMETER; 
   }

   card_info->mfr_id = 0; 
   card_info->oem_id = 0; 

   card_info->product_serial_num[0] = 0x1; 
   card_info->product_serial_num[1] = 0x2;
   card_info->product_serial_num[2] = 0x3;
   card_info->serial_num_len = 0x3; 
   
   card_info->rpmb_size_in_byte = 0; 
   card_info->reliable_write_count = 0; 

   card_info->card_type[0] = 'N';
   card_info->card_type[1] = 'V';
   card_info->card_type[2] = 'M';
   card_info->card_type[3] = ' ';

   return EFI_SUCCESS; 
}

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
  )
{
  return EFI_UNSUPPORTED;
}


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
  )
{
  return EFI_UNSUPPORTED;
}

