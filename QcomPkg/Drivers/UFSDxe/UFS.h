/**
@file     UFS.h
@brief   Universal Flash Storage (UFS) DXE Header File

This file contains the definitions of the constants, data structures,
and interfaces for the UFS driver in UEFI.

 Copyright (c) 2013 - 2020, Qualcomm Technologies, Inc. All rights reserved.
 Portions Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>
 This program and the accompanying materials
 are licensed and made available under the terms and conditions of the BSD License
 which accompanies this distribution.  The full text of the license may be found at
 http://opensource.org/licenses/bsd-license.php

 THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

/*=============================================================================
                              EDIT HISTORY


when            who   what, where, why
----------      ---   -----------------------------------------------------------
2020-12-22      gk    Add Secondary UFS LUN Guids
2019-07-02      jt    Add voltage macros 
2018-12-14      jt    Add UFS provision protocol  
2018-06-04      jt    Add clock macros 
2017-02-01      jt    Add read/write wrapper definitions
2016-09-21      jt    Secure write protect changes  
2016-06-28      jt    Add get/set boot LUN APIs  
2016-01-21      rh    Adding BlockIO2 
2015-03-26      rm    Add RPMB protocol and GUID
09-23-2014      rh    Use unique GUID for LUs, different from eMMC
05-05-2013      ai    Initial version

=============================================================================*/

#ifndef _UFS_H_
#define _UFS_H_

/** @name UFS Device Signature
      Signature as per UEFI requirement.
  */
/*@{*/
#define UFS_DEV_SIGNATURE SIGNATURE_32 ('u', 'f', 's', ' ')   /**< -- UFS  */
/*@}*/

#define UFS_UEFI_MAX_LUN_ACCESS              8

#define UFS_CLK_OFF   0
#define UFS_CLK_ON    1

#define UFS_VOLT_OFF  0
#define UFS_VOLT_ON   1

/**
  UFS Device Structure
  */
typedef struct {
  UINT32                     Signature;
  EFI_HANDLE                 ClientHandle;  /**< -- Client Handle */
  EFI_HANDLE                 DeviceHandle;  /**< -- Device Handle */
  EFI_BLOCK_IO_PROTOCOL      BlkIo;         /**< -- Block I/O */
  EFI_BLOCK_IO2_PROTOCOL     BlkIo2;        /**< -- Block I/O 2 */
  EFI_SDCC_RPMB_PROTOCOL     RPMB;          /**< -- RPMB protocol */
  EFI_MEM_CARDINFO_PROTOCOL  CardInfo;      /**< -- Memory card into */
  EFI_ERASE_BLOCK_PROTOCOL   EraseBlk;      /**< -- Erase block */
  EFI_STORAGE_WP_PROTOCOL    WriteProtect;  /**< -- Write Protect */
  EFI_UFS_PROV_PROTOCOL      UfsProvision;  /**< -- UFS provision */
} UFS_DEV;


/**
   Reset the Block Device.
   
   @param  This                 Indicates a pointer to the calling context.
   @param  ExtendedVerification Driver may perform diagnostics on reset.
   
   @retval EFI_SUCCESS          The device was reset.
   @retval EFI_DEVICE_ERROR     The device is not functioning properly
                                and could not be reset.

**/
EFI_STATUS EFIAPI UFSReset (
   IN EFI_BLOCK_IO_PROTOCOL   *This,
   IN BOOLEAN                 ExtendedVerification
   );

/**
   Provision the UFS device.
   
   @param  This                 Indicates a pointer to the calling context.
   @param  ConfigFileName       Cfg file name with provisioning details.
   
   @retval EFI_SUCCESS          The device was provisioned successfully.
   @retval EFI_DEVICE_ERROR     The device failed to provisioned. 
**/
EFI_STATUS EFIAPI UFSProvisionHandler (
   IN EFI_UFS_PROV_PROTOCOL *This, 
   IN CONST CHAR8           *ConfigFileName
);

/**
   Read BufferSize bytes from Lba into Buffer.
   
   @param  This       Indicates a pointer to the calling context.
   @param  MediaId    Id of the media, changes every time the media is replaced.
   @param  Lba        The starting Logical Block Address to read from
   @param  BufferSize Size of Buffer, must be a multiple of device block size.
   @param  Buffer     A pointer to the destination buffer for the data.
                      The caller is responsible for either having implicit or
                      explicit ownership of the buffer.
   
   @retval EFI_SUCCESS           The data was read correctly from the device.
   @retval EFI_DEVICE_ERROR      The device reported an error while
                                 performing the read.
   @retval EFI_NO_MEDIA          There is no media in the device.
   @retval EFI_MEDIA_CHANGED     The MediaId does not matched the
                                 current device.
   @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block
                                 size of the device.
   @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not
                                 valid,or the buffer is not on proper alignment.
EFI_STATUS
**/
EFI_STATUS EFIAPI UFSReadBlocks (
   IN EFI_BLOCK_IO_PROTOCOL   *This,
   IN UINT32                  MediaId,
   IN EFI_LBA                 Lba,
   IN UINTN                   BufferSize,
   OUT VOID                   *Buffer
   );

/**
   Write BufferSize bytes from Lba into Buffer.
   
   @param  This       Indicates a pointer to the calling context.
   @param  MediaId    The media ID that the write request is for.
   @param  Lba        The starting logical block address to be 
                      written. The   caller is responsible for
                      writing to only legitimate locations.
   @param  BufferSize Size of Buffer, must be a multiple of device block size.
   @param  Buffer     A pointer to the source buffer for the data.
   
   @retval EFI_SUCCESS           The data was written correctly to the device.
   @retval EFI_WRITE_PROTECTED   The device can not be written to.
   @retval EFI_DEVICE_ERROR      The device reported an error 
                                 while performing the write.
U   @retval EFI_NO_MEDIA          There is no media in the device.
   @retval EFI_MEDIA_CHNAGED     The MediaId does not matched 
                                 the current device.
   @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple 
                                 of the block size of the device.
   @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not
                                 valid or the buffer is not on proper
                                 alignment.
**/ 
EFI_STATUS EFIAPI UFSWriteBlocks (
   IN EFI_BLOCK_IO_PROTOCOL   *This,
   IN UINT32                  MediaId,
   IN EFI_LBA                 Lba,
   IN UINTN                   BufferSize,
   IN VOID                    *Buffer
   );

/**
   Flush the Block Device.
   
   @param  This              Indicates a pointer to the calling context.
   
   @retval EFI_SUCCESS       All outstanding data was written to the device
   @retval EFI_DEVICE_ERROR  The device reported an error while 
                             writting back the data
   @retval EFI_NO_MEDIA      There is no media in the device.

**/
EFI_STATUS EFIAPI UFSFlushBlocks (
   IN EFI_BLOCK_IO_PROTOCOL  *This
   );

/**
   Function: RPMBReadCounterPkt
 
   This function retrieves Read Counter Response packet from flash device.
   The Response packet includes the Write Counter as well as the MAC which
   is used later to validate the Response packet
  
   @param  RespPktBuffer [OUT]    Pointer to the response from the Read
                                  Counter command
    
   @retval  RPMB_NO_ERROR           Successfully read the Counter packet.
   @retval  RPMB_ERR_INVALID_PARAM  NULL pointer is passed in to the function.
   @retval  RPMB_ERR_READ           Failed to read the Counter packet.
**/
INT32 
EFIAPI
RPMBReadCounterPkt (
   EFI_SDCC_RPMB_PROTOCOL       *This,
   UINT32                       *RespPktBuffer
   );

/**
 Function: RPMBProgProvisionKey
 
 This function programs the eMMC's key using the provided packet
 formatted as ProvisionKey command. 

 @param CmdPktBuffer  [IN] : Pointer to a ProvisionKey command packet
                             to be sent to eMMC to program its key
 @param RespPktBuffer [OUT]: Pointer to the response packet for the
                             ProvisionKey command
 
 @retval  RPMB_NO_ERROR            Successfully provision the eMMC's Key.
 @retval  RPMB_ERR_INVALID_PARAM   NULL pointer is passed in to the function.
 @retval  RPMB_ERR_GENERAL         Failed to provision the key (Key has been
                                   provisioned).
  
**/
INT32 
EFIAPI
RPMBProgProvisionKey (
   EFI_SDCC_RPMB_PROTOCOL       *This,
   UINT32                       *CmdPktBuffer,
   UINT32                       *RespPktBuffer 
   );

/**
  Function: RPMBReadBlocks
 
  This function reads the sectors from RPMB partition using the
  cmd packet buffers provided.
 
  @param   CmdPktBuffer  [IN] : Pointer to a formatted packet for Read Request 
  @param   PktCount      [IN] : How many half sectors to read   
  @param   DataBuffer    [OUT]: Pointer to data read from eMMC
 
  @retval  RPMB_NO_ERROR            Successfully read the Counter packet
  @retval  RPMB_ERR_INVALID_PARAM   NULL pointer is passed in to the function
  @retval  RPMB_ERR_READ            Failed to read the sectors
     
**/ 
INT32 
EFIAPI
RPMBReadBlocks (
   EFI_SDCC_RPMB_PROTOCOL       *This,
   UINT32                       *CmdPktBuffer,
   UINT32                       PktCount,
   UINT32                       *DataBuffer
   );  

/**
  Function: RPMBWriteBlocks
 
  This function writes the sectors to the RPMB partition using
  the provided cmd packet buffers
                       
  @param  CmdPktBuffer  [IN] :  Pointer to a formatted packets to be sent
                                to eMMC to write to RPMB partition
  @param  PktCount      [IN] :  Number of half sectors to write
  @param  RespPktBuffer [OUT]:  Pointer to the response packet from eMMC
 
  @retval  RPMB_NO_ERROR            Successfully Write the RPMB sectors.
                                    Caller should authenticate the Response
                                    packet and validate the Write Counter
  @retval  RPMB_ERR_INVALID_PARAM   NULL pointer is passed in to the function
  @retval  RPMB_ERR_WRITE           Failed to write the sectors
  
**/
INT32 
EFIAPI
RPMBWriteBlocks (
   EFI_SDCC_RPMB_PROTOCOL       *This,
   UINT32                       *CmdPktBuffer,
   UINT32                       PktCount,
   UINT32                       *RespPktBuffer 
   );

/**
  Function: UFSGetCardInfo
 
   This function retrieves the card information. 
  
   @param  card_info              Pointer to a UFS_CARD_INFO variable 
                                  that is used to store the retrieved
                                  card information
    
   @retval EFI_SUCCESS            The function completed successfully. 
   @retval EFI_INVALID_PARAMETER  The pass-in parameter is invalid.
**/
EFI_STATUS EFIAPI UFSGetCardInfo (
   EFI_MEM_CARDINFO_PROTOCOL   *This,
   MEM_CARD_INFO*              card_info
   );

/**
   Read BufferSize bytes from Lba into Buffer.
   
   @param  This       Indicates a pointer to the calling context.
   @param  MediaId    Id of the media, changes every time the media is replaced.
   @param  Lba        The starting Logical Block Address to read from
   @param  BufferSize Size of Buffer, must be a multiple of device block size.
   @param  Buffer     A pointer to the destination buffer for the data.
                      The caller is responsible for either having implicit or
                      explicit ownership of the buffer.
   
   @retval EFI_SUCCESS           The data was read correctly from the device.
   @retval EFI_DEVICE_ERROR      The device reported an error while
                                 performing the read.
   @retval EFI_NO_MEDIA          There is no media in the device.
   @retval EFI_MEDIA_CHANGED     The MediaId does not matched the
                                 current device.
   @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block
                                 size of the device.
   @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not
                                 valid,or the buffer is not on proper alignment.
**/
EFI_STATUS UFSReadBlocksWrapper (
            IN EFI_BLOCK_IO_PROTOCOL  *This,
            IN UINT32                  MediaId,
            IN EFI_LBA                 Lba,
            IN UINTN                   BufferSize,
            OUT VOID                  *Buffer);

/**
   Write BufferSize bytes from Buffer to Lba.
   
   @param  This       Indicates a pointer to the calling context.
   @param  MediaId    The media ID that the write request is for.
   @param  Lba        The starting logical block address to be 
                      written. The   caller is responsible for
                      writing to only legitimate locations.
   @param  BufferSize Size of Buffer, must be a multiple of device block size.
   @param  Buffer     A pointer to the source buffer for the data.
   
   @retval EFI_SUCCESS           The data was written correctly to the device.
   @retval EFI_WRITE_PROTECTED   The device can not be written to.
   @retval EFI_DEVICE_ERROR      The device reported an error 
                                 while performing the write.
   @retval EFI_NO_MEDIA          There is no media in the device.
   @retval EFI_MEDIA_CHNAGED     The MediaId does not matched 
                                 the current device.
   @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple 
                                 of the block size of the device.
   @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not
                                 valid or the buffer is not on proper
                                 alignment.
**/
EFI_STATUS UFSWriteBlocksWrapper (
            IN EFI_BLOCK_IO_PROTOCOL   *This,
            IN UINT32                  MediaId,
            IN EFI_LBA                 Lba,
            IN UINTN                   BufferSize,
            IN VOID                    *Buffer);

/**
  Read BufferSize bytes from Lba into Buffer.
  
  This function reads the requested number of blocks from the device. All the
  blocks are read, or an error is returned.
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_or EFI_MEDIA_CHANGED is returned and
  non-blocking I/O is being used, the Event associated with this request will
  not be signaled.

  @param[in]       This       Indicates a pointer to the calling context.
  @param[in]       MediaId    Id of the media, changes every time the media is 
                              replaced.
  @param[in]       Lba        The starting Logical Block Address to read from.
  @param[in, out]  Token       A pointer to the token associated with the transaction.
  @param[in]       BufferSize Size of Buffer, must be a multiple of device block size.  
  @param[out]      Buffer     A pointer to the destination buffer for the data. The 
                              caller is responsible for either having implicit or 
                              explicit ownership of the buffer.

  @retval EFI_SUCCESS           The read request was queued if Token->Event is
                                not NULL.The data was read correctly from the
                                device if the Token->Event is NULL.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing
                                the read.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId is not for the current media.
  @retval EFI_BAD_BUFFER_SIZE   The BufferSize parameter is not a multiple of the
                                intrinsic block size of the device.
  @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack
                                of resources.
**/ 
EFI_STATUS UFSReadBlocksEx (
   IN     EFI_BLOCK_IO2_PROTOCOL *This,
   IN     UINT32                 MediaId,
   IN     EFI_LBA                Lba,
   IN OUT EFI_BLOCK_IO2_TOKEN    *Token,
   IN     UINTN                  BufferSize,
   OUT VOID                      *Buffer
   );

/**
  Write BufferSize bytes from Buffer to Lba.

  This function writes the requested number of blocks to the device. All blocks
  are written, or an error is returned.If EFI_DEVICE_ERROR, EFI_NO_MEDIA,
  EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED is returned and non-blocking I/O is
  being used, the Event associated with this request will not be signaled.

  @param[in]       This       Indicates a pointer to the calling context.
  @param[in]       MediaId    The media ID that the write request is for.
  @param[in]       Lba        The starting logical block address to be written. The
                              caller is responsible for writing to only legitimate
                              locations.
  @param[in, out]  Token      A pointer to the token associated with the transaction.
  @param[in]       BufferSize Size of Buffer, must be a multiple of device block size.
  @param[in]       Buffer     A pointer to the source buffer for the data.

  @retval EFI_SUCCESS           The write request was queued if Event is not NULL.
                                The data was written correctly to the device if
                                the Event is NULL.
  @retval EFI_WRITE_PROTECTED   The device can not be written to.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHNAGED     The MediaId does not matched the current device.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing the write.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.
  @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack
                                of resources.

**/
EFI_STATUS UFSWriteBlocksEx (
   IN     EFI_BLOCK_IO2_PROTOCOL *This,
   IN     UINT32                 MediaId,
   IN     EFI_LBA                Lba,
   IN OUT EFI_BLOCK_IO2_TOKEN    *Token,
   IN     UINTN                  BufferSize,
   OUT VOID                      *Buffer
   );

/**
  Reset the block device hardware.

  @param[in]  This                 Indicates a pointer to the calling context.
  @param[in]  ExtendedVerification Indicates that the driver may perform a more
                                   exhausive verfication operation of the device
                                   during reset.

  @retval EFI_SUCCESS          The device was reset.
  @retval EFI_DEVICE_ERROR     The device is not functioning properly and could
                               not be reset.
**/
EFI_STATUS EFIAPI UFSResetEx (
   IN EFI_BLOCK_IO2_PROTOCOL  *This,
   IN BOOLEAN                 ExtendedVerification
   );

/**
  Flush the Block Device.
 
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED
  is returned and non-blocking I/O is being used, the Event associated with
  this request will not be signaled.  

  @param[in]      This     Indicates a pointer to the calling context.
  @param[in,out]  Token    A pointer to the token associated with the transaction

  @retval EFI_SUCCESS          The flush request was queued if Event is not NULL.
                               All outstanding data was written correctly to the
                               device if the Event is NULL.
  @retval EFI_DEVICE_ERROR     The device reported an error while writting back
                               the data.
  @retval EFI_WRITE_PROTECTED  The device cannot be written to.
  @retval EFI_NO_MEDIA         There is no media in the device.
  @retval EFI_MEDIA_CHANGED    The MediaId is not for the current media.
  @retval EFI_OUT_OF_RESOURCES The request could not be completed due to a lack
                               of resources.
**/
EFI_STATUS EFIAPI UFSFlushBlocksEx (
   IN     EFI_BLOCK_IO2_PROTOCOL   *This,
   IN OUT EFI_BLOCK_IO2_TOKEN      *Token
   );

/**
  Erase blocks in the Block Device
 
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED
  is returned and non-blocking I/O is being used, the Event associated with
  this request will not be signaled.  

  @param[in]      This     Indicates a pointer to the calling context.
  @param[in,out]  Token    A pointer to the token associated with the transaction

  @retval EFI_SUCCESS          The flush request was queued if Event is not NULL.
                               All outstanding data was written correctly to the
                               device if the Event is NULL.
  @retval EFI_DEVICE_ERROR     The device reported an error while writting back
                               the data.
  @retval EFI_WRITE_PROTECTED  The device cannot be written to.
  @retval EFI_NO_MEDIA         There is no media in the device.
  @retval EFI_MEDIA_CHANGED    The MediaId is not for the current media.
  @retval EFI_OUT_OF_RESOURCES The request could not be completed due to a lack
                               of resources.
**/
EFI_STATUS EFIAPI UFSEraseBlocks (
   IN     EFI_BLOCK_IO_PROTOCOL *This,
   IN     UINT32 MediaId,
   IN     EFI_LBA LBA,
   IN OUT EFI_ERASE_BLOCK_TOKEN *Token,
   IN     UINTN Size
   );

/**
  Get Active BOOT LU 

  @param[in]      This       Indicates a pointer to the calling context.
  @param[out]     BootLU     Pointer that contains the active 
                             BOOT LU (0 = disable BOOT 
                             LU, 1 = BOOT for A, 2 = BOOT for B)

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_INVALID_PARAMETER  Parameter is invalid 
**/
EFI_STATUS EFIAPI UFSGetActiveBootLU (
   IN      EFI_MEM_CARDINFO_PROTOCOL   *This,
   OUT     UINT32                      *BootLU);

/**
  Set Active BOOT LU 

  @param[in]  This      Indicates a pointer to the calling context.
  @param[in]  BootLU    LU to set as active (0 = disable BOOT 
                        LU, 1 = BOOT for A, 2 = BOOT for B)

  @retval EFI_SUCCESS             Operation completed successfully.
  @retval EFI_INVALID_PARAMETER   Parameter is invalid 
**/
EFI_STATUS EFIAPI UFSSetActiveBootLU (
   IN      EFI_MEM_CARDINFO_PROTOCOL   *This,
   IN      UINT32                      BootLU);

/**
  Gets supported LBA write protect types for the LU 

  @param[in]  This       Indicates a pointer to the calling context. 
  @param[out] LbaWpTypes LBA write protect types for the LU

  @retval EFI_SUCCESS             Operation completed successfully.
  @retval EFI_INVALID_PARAMETER   Parameter is invalid 
**/
EFI_STATUS EFIAPI UFSGetSupportedLbaWriteProtectTypes(
   IN EFI_STORAGE_WP_PROTOCOL *This, 
   OUT UINT32 *LbaWpTypes
);

/**
  Set LU write protect 

  @param[in]  This       Indicates a pointer to the calling context. 

  @retval EFI_SUCCESS             Operation completed successfully.
  @retval EFI_INVALID_PARAMETER   Parameter is invalid 
**/
EFI_STATUS EFIAPI UFSSetLUWriteProtect(
   IN EFI_STORAGE_WP_PROTOCOL *This
);

/**
  Sets LBA write protect for the given ranges

  @param[in]  This       Indicates a pointer to the calling context. 
  @param[in]  WpConfig   Pointer to the LBA ranges 

  @retval EFI_SUCCESS             Operation completed successfully.
  @retval EFI_INVALID_PARAMETER   Parameter is invalid 
**/
EFI_STATUS EFIAPI UFSSetLbaWriteProtect(
   IN EFI_STORAGE_WP_PROTOCOL *This,     
   IN EFI_WP_CONFIG *WpConfig
);

/**
  Clears secure write protect entries 

  @param[in]  This       Indicates a pointer to the calling context. 

  @retval EFI_SUCCESS             Operation completed successfully.
  @retval EFI_INVALID_PARAMETER   Parameter is invalid 
**/
EFI_STATUS EFIAPI UFSClearWriteProtect(
   IN EFI_STORAGE_WP_PROTOCOL *This
);

/**
  Gets write protection information of the LU

  @param[in]  This        Indicates a pointer to the calling context. 
  @param[out] ProtectInfo LUN Write protection information

  @retval EFI_SUCCESS             Operation completed successfully.
  @retval EFI_INVALID_PARAMETER   Parameter is invalid 
**/
EFI_STATUS EFIAPI UFSGetLUWriteProtectInfo(
   IN EFI_STORAGE_WP_PROTOCOL *This, 
   OUT EFI_WP_CONFIG *ProtectInfo
);
   
#define UFS_DEV_FROM_BLOCKIO(a)   CR (a, UFS_DEV, BlkIo, UFS_DEV_SIGNATURE)
#define UFS_DEV_FROM_BLOCKIO2(a)  CR (a, UFS_DEV, BlkIo2, UFS_DEV_SIGNATURE)
#define UFS_DEV_FROM_RPMB(a)        CR (a, UFS_DEV, RPMB, UFS_DEV_SIGNATURE)
#define UFS_DEV_FROM_CARD_INFO(a)   CR (a, UFS_DEV, CardInfo, UFS_DEV_SIGNATURE)
#define UFS_DEV_FROM_ERASE_BLOCK(a) CR (a, UFS_DEV, EraseBlk, UFS_DEV_SIGNATURE)
#define UFS_DEV_FROM_WRITE_PROTECT(a) CR (a, UFS_DEV, WriteProtect, UFS_DEV_SIGNATURE)
#define UFS_DEV_FROM_UFS_PROV(a) CR(a, UFS_DEV, UfsProvision, UFS_DEV_SIGNATURE)


#define EFI_UFS_LUN_0_GUID \
    /* gEfiUFSLun0Guid */ \
    { 0x860845c1, 0xbe09, 0x4355, { 0x8b, 0xc1, 0x30, 0xd6, 0x4f, 0xf8, 0xe6, 0x3a } }

#define EFI_UFS_LUN_1_GUID \
    /* gEfiUFSLun1Guid */ \
    { 0x8d90d477, 0x39a3, 0x4a38, { 0xab, 0x9e, 0x58, 0x6f, 0xf6, 0x9e, 0xd0, 0x51 } }

#define EFI_UFS_LUN_2_GUID \
    /* gEfiUFSLun2Guid */ \
    { 0xedf85868, 0x87ec, 0x4f77, { 0x9c, 0xda, 0x5f, 0x10, 0xdf, 0x2f, 0xe6, 0x01 } }

#define EFI_UFS_LUN_3_GUID \
   /* gEfiUFSLun3Guid */ \
   { 0x1ae69024, 0x8aeb, 0x4df8, { 0xbc, 0x98, 0x00, 0x32, 0xdb, 0xdf, 0x50, 0x24 } }

#define EFI_UFS_LUN_4_GUID \
   /* gEfiUFSLun4Guid */ \
   { 0xd33f1985, 0xf107, 0x4a85, { 0xbe, 0x38, 0x68, 0xdc, 0x7a, 0xd3, 0x2c, 0xea } }

#define EFI_UFS_LUN_5_GUID \
   /* gEfiUFSLun5Guid */ \
   { 0x4ba1d05f, 0x088e, 0x483f, { 0xa9, 0x7e, 0xb1, 0x9b, 0x9c, 0xcf, 0x59, 0xb0 } }

#define EFI_UFS_LUN_6_GUID \
   /* gEfiUFSLun6Guid */ \
   { 0x4acf98f6, 0x26fa, 0x44d2, { 0x81, 0x32, 0x28, 0x2f, 0x2d, 0x19, 0xa4, 0xc5 } }

#define EFI_UFS_LUN_7_GUID \
   /* gEfiUFSLun7Guid */ \
   { 0x8598155f, 0x34de, 0x415c, { 0x8b, 0x55, 0x84, 0x3e, 0x33, 0x22, 0xd3, 0x6f } }

#define EFI_UFS_RPMB_LUN_GUID \
   /* gEfiUfsRPMBGuid */ \
   { 0x5397474e, 0xf75d, 0x44b3, { 0x8e, 0x57, 0xd9, 0x32, 0x4f, 0xcf, 0x6f, 0xe1 } }

#ifdef ENABLE_AUTO_PLAT
#define EFI_UFS1_LUN_0_GUID \
    /* gEfiUFS1Lun0Guid */ \
    { 0x3d329913, 0x7d10, 0x4239, { 0x83, 0xb3, 0xc3, 0xc5, 0x5c, 0xbd, 0x1a, 0x80 } }

#define EFI_UFS1_LUN_1_GUID \
    /* gEfiUFS1Lun1Guid */ \
    { 0xfc0fec15, 0x9cab, 0x448e, { 0xac, 0x5d, 0x3e, 0x5f, 0x62, 0x6c, 0xa0, 0x1f } }

#define EFI_UFS1_LUN_2_GUID \
    /* gEfiUFS1Lun2Guid */ \
    { 0x12804fb5, 0x4a70, 0x4e16, { 0xb7, 0x08, 0xd9, 0xd3, 0x8d, 0x3b, 0xc9, 0x01 } }

#define EFI_UFS1_LUN_3_GUID \
   /* gEfiUFS1Lun3Guid */ \
   { 0xcbecb456, 0xb3b9, 0x4ef1, { 0xa1, 0x64, 0x4e, 0x0e, 0x22, 0x23, 0x99, 0x5c } }

#define EFI_UFS1_LUN_4_GUID \
   /* gEfiUFS1Lun4Guid */ \
   { 0x45503342, 0x3c29, 0x4fb3, { 0xb0, 0xaf, 0x21, 0x19, 0xd2, 0xea, 0x49, 0xb3 } }

#define EFI_UFS1_LUN_5_GUID \
   /* gEfiUFS1Lun5Guid */ \
   { 0x87f54361, 0xb82f, 0x4b14, { 0xb8, 0x11, 0x86, 0x31, 0x79, 0x80, 0x34, 0xeb } }

#define EFI_UFS1_LUN_6_GUID \
   /* gEfiUFS1Lun6Guid */ \
   { 0xc820feba, 0x7054, 0x417d, { 0xa5, 0x9b, 0x51, 0x11, 0x07, 0x73, 0xb9, 0x1e } }

#define EFI_UFS1_LUN_7_GUID \
   /* gEfiUFS1Lun7Guid */ \
   { 0xbef7637f, 0xb118, 0x4197, { 0xab, 0xe0, 0xdb, 0x53, 0x10, 0x15, 0x22, 0x9a } }
#endif

#endif /* _UFS_H_ */

