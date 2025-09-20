/*===============================================================================
  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         EFIPIL.h
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  06/14/17   yw         initial version

================================================================================*/

#ifndef __EFIPIL_H__
#define __EFIPIL_H__

#define EFI_PIL_PROTOCOL_VERSION_V1 0x0000000000010001

#define EFI_PIL_PROTOCOL_GUID \
  { 0x3bec87db, 0x4e76, 0x491c, { 0x96, 0x6c, 0x7c, 0xa5, 0x81, 0x2a, 0x64, 0xc9 } }
  
#define NAME_MAX_LENGTH        32
  
typedef enum _PIL_ELF_TYPE_
{
    ELF_FV           = 1,            /*FV image pushed to raw parition*/
    ELF_SPLIT        = 2,            /*ELF image are splited to xxx.mdt xxx.b00 xxx,b01... */
    ELF_SINGLE       = 3,            /*original ELF image */
    ELF_MERGED       = 4,            /*merged splited image */
    MAX_PIL_ELF_TYPE = 0x7FFFFFFF
} PIL_ELF_TYPE;


/**
  struct to describe a subsystem which will be loaded by PIL
**/
typedef struct _PIL_SUBSYS_CFG_TYPE_
{
    //
    // version of the struct, caller must set the appropriate version
    //
    UINT64  Version;
    
    //
    // Elf type of the image to be loaded
    //
    PIL_ELF_TYPE  Type;
    
    //
    // firmware name of the subsys
    // optional for FAT partition
    // required for RAW partition
    //
    UINT16   FwName[NAME_MAX_LENGTH] OPTIONAL;
    
    // 
    // Partition label where to load the image, for RAW/FAT partition
    //
    UINT16   PartiLabel[NAME_MAX_LENGTH] OPTIONAL;
    
    //
    // RootGuid of the partition where to load the iamge, used for RAW partition
    // optional for FAT partition
    //
    EFI_GUID    PartiRootGuid OPTIONAL;
    
    //
    // GUID of the partition where to load the image, used for RAW partition
    // optional if PartiLabel is given
    // 
    EFI_GUID    PartiGuid OPTIONAL;
    
    //
    // Image path where to load the image, used for FAT patition
    // example: \\image\\adsp
    // optional if it's RAW partition
    //
    UINT16   ImagePath[NAME_MAX_LENGTH] OPTIONAL;
    
    //
    // subsystem ID of the proc
    //
    UINT32  SubsysID;
    
    //
    // start address of the reserved memory region where to load the image
    // if the ResvRegionStart is not specified memory will be allocated dynamically
    //
    UINT64    ResvRegionStart OPTIONAL;
    
    //
    // size of the reserved loading memory region
    //
    UINT64    ResvRegionSize OPTIONAL;
    
    //
    // if image load info is required to fill in IMEM
    // default is FLASE unless it's specified
    //
    BOOLEAN ImageLoadInfo;
    
    // 
    // if unlock is required immdiatly  after bring subsys out of reset
    //
    BOOLEAN Unlock;
    
    // 
    // if yes ingore checking if loading address read from ELF matches ipcat
    //
    BOOLEAN OverrideElfAddr;
} PIL_SUBSYS_CFG_TYPE;

#define    PIL_SUBSYS_CFG_VERSION_V1        1
#define PIL_SUBSYS_CFG_LATEST_VERSION    PIL_SUBSYS_CFG_VERSION_V1

extern EFI_GUID gEfiPilProtocolGuid;

/**
  Boot PIL images, taking the firmware name input, find out the PIL image 
  configuration from SecParti.cfg and load the image

  @param  Subsys               firmware name of the image

  @retval EFI_SUCCESS          The image is loaded successfully.
  
**/
typedef 
EFI_STATUS 
(EFIAPI *EFI_PROCESS_PIL_IMAGE) (
    IN CHAR16*  Subsys
    );
    
/**
  Boot PIL images, taking the PIL_SUBSYS_CFG_TYPE struct as input,
  load the image, verify authentication and boot the image

  @param  SubsysCfg            Configure structure of the image

  @retval EFI_SUCCESS          The image is loaded successfully.
  
**/
typedef 
EFI_STATUS 
(EFIAPI *EFI_PROCESS_PIL_IMAGE_EXT) (
    IN PIL_SUBSYS_CFG_TYPE*  SubsysCfg
    );


typedef struct _EFI_PIL_PROTOCOL {
   UINT64                           Revision;
   EFI_PROCESS_PIL_IMAGE            ProcessPilImage;
   EFI_PROCESS_PIL_IMAGE_EXT        ProcessPilImageExt;

}EFI_PIL_PROTOCOL;

#endif