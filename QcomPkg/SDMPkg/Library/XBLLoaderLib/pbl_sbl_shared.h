/****************************************************************************** 
**  
**   Qualcomm Technologies Incorporated
** 
**   Copyright (c) 2014,2016-2018 Qualcomm Technologies, Inc.
**   All rights reserved.
**   Qualcomm Technologies, Inc. Confidential and Proprietary.
** 
******************************************************************************/

#ifndef BOOT_PBL_V2_H
#define BOOT_PBL_V2_H

/*=============================================================================
                            
                       Boot PBL Version 2 Header File

GENERAL DESCRIPTION
 **     This header file contains PBL-BL shared data structure
 **     specific details.
 ** 
 **     Any new shared data that has to be passed to BL should be
 **     added in this file. Any dependencies, like enums, defines etc ...
 **     that are supposed to be passed in this structure should be present
 **     in this file.
 ** 
 **     BL code relies heavily on this structure and hence
 **     everytime there is a change in the shared data-structure
 **     (additions/deletions of fields/new values etc) ... structure
 **     version should be incremented appropriately.
 ** 
 **     Any new shared data field that has to be added has to be
 **     at the end of the structure.
 **
 **     NOTE:
 **     -----
 **
 **     ANY CHANGES INT THIS FILE OR ANY FIELDS RELATED TO THE 
 **     SHARED DATA STRUCTURE MUST BE REVIEWED WITH BL IMAGE
 **     OWNERS.
 **
 **==========================================================================*/


/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module..
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/20/18   yps     Add clock data struct
04/02/18   kpa     relocate boot_flash_type to boot_flash_type.h header
08/22/17   vk      version 1005 updates
06/18/17   dp      ROM shared data structure updates.
05/28/14   ck      Added boot_pbl_log_milestones
05/28/14   ck      Added PBL_TIMESTAMP_TIMER_FREQ_HZ
05/21/14   ck      Updated boot_flash_type to match PBL
05/05/14   ck      Added PBL timestamp type and entry in boot_pbl_shared_data_type
04/01/14   ck      Removed pbl_secboot_verified_info_type, pbl_secboot_ftbl_type
                   and pbl_secboot_hw_ftbl_type as Bear PBL now uses common secboot lib.
02/24/13   jz      Updated for Bear support on MDM
08/26/13   jz      Added UFS flash type
08/15/13   kedara  Update bootrom shared data struct defines.
08/06/13   aus     For secboot verified info, define a local definition of
                   pbl_secboot_verified_info
05/07/13   dh      For secboot function tables, define a local definition 
                   of pbl_secboot_ftbl_type pbl_secboot_hw_ftbl_type 
02/03/12   kedara      Initial revision.

============================================================================*/


/******************************************************************************
                                MODULE INCLUDES
                         ADD NEW ONES UNDER THIS LINE
******************************************************************************/
#include "sec_img_auth.h"
#include "boot_comdef.h"
#include "secboot_hw.h"
#include "secboot.h"
#include "pbl_sbl_external.h"
#include "boot_flash_type.h"

#if 0
#include "ecc_shared_hw.h"
#endif

/******************************************************************************
                             PUBLIC MACROS/DEFINES
                         ADD NEW ONES UNDER THIS LINE  
******************************************************************************/
/* Shared data structure Version */
/* Version Format: 0xMMMMNNNN
          MMMM : Major Version 
          NNNN : Minor Version */
#define PBL_SHARED_DATA_VERSION               0x00010001
#define PBL_BL_SHARED_DATA_VERSION            0x00010000
#define PBL_APPS_BL_SEC_SHARED_DATA_VERSION   0x00010003
#define PBL_APPS_CACHE_MMU_SHARE_DATA_VERSION 0x00010001
#define PBL_NAND_FLASH_SHARED_DATA_VERSION    0x00010000
#define PBL_SDCC_FLASH_SHARED_DATA_VERSION    0x00010000
#define PBL_SAHARA_SHARED_DATA_VERSION        0x00010001
#define PBL_ELF_IMAGE_SHARED_DATA_VERSION     0x00010000
#define PBL_PRNG_SHARED_DATA_VERSION          0x00010000
#define PBL_TIMESTAMP_SHARED_DATA_VERSION     0x00010001
#define BL_SEC_ELF_SEG_SHARED_DATA_VERSION    0x00010001



#define BL_RESERVED_LENGTH           32
#define BL_SEC_RESERVED_LENGTH       32


/******************************************************************************
                         PUBLIC TYPES and TYPE-DEFINES
                         ADD NEW ONES UNDER THIS LINE  
******************************************************************************/
#ifndef _ARM_ASM_

/* Define the types of flash controllers that may be in use */
typedef enum
{
  NO_CTRL,                          /* Invalid controller     */
  NOR_CTRL,                         /* NOR controller         */
  NAND_CTRL,                        /* NAND controller        */
  SFLASH_CTRL,                      /* Sync flash controller          */
  SDC_CTRL,                         /* Secure Digital Card controller */
  SPI_CTRL,                         /* SPI controller         */
  UFS_CTRL,                         /* UFS controller         */
  USB_CTRL,                         /* USB controller         */
  SIZE_FLASH_CTRL_TYPE = 0x7FFFFFFF /* Force 4 byte alignment */
} boot_flash_ctrl_type;

/* Define the types of ECC Mode being used by NAND Controller */
typedef enum
{
  ECC_4_BIT_MODE         = 0,
  ECC_6_BIT_MODE,
  ECC_8_BIT_MODE,
  ECC_RESERVED_BIT_MODE_0,
  ECC_16_BIT_MODE,
  ECC_RESERVED_BIT_MODE_1,
  ECC_NOT_ENABLED,
  ECC_BIT_MODE_ENUM_COUNT,
  EBM_SIZE_ENUM          = 0x7FFFFFFF          /* Size enum to 32 bits. */
} boot_ecc_bit_mode_enum_type;

/* Defines the types of Boot Option is configured on the
   device. */
typedef enum
{
  BOOT_DEFAULT_OPTION = 0,              /* UFS HS G1-> SDC -> USB */
  BOOT_SDC_THEN_UFS_OPTION,             /* 1, SDC -> UFS */
  BOOT_SDC_OPTION,                      /* 2, SDC */
  BOOT_USB_OPTION,                      /* 3, USB */
  BOOT_QSPI_OPTION,                     /* 4, QSPI */
  BOOT_SPI_OPTION,                      /* 5, SPI */
  BOOT_RSVD_1_OPTION,                   /* 6, Reserved-1 */
  BOOT_RSVD_2_OPTION,                   /* 7, Reserved-2 */
  UFS_PWM_MODE_SELECT,                  /* 8, BOOT_CONFIG[3] defines UFS running in PWM Mode */
  BOOT_SELECT_OPTION_COUNT,
  BOOT_SDC_PORT2_THEN_USB_OPTION = 0x80,/* This is a Force dload option */
  BOOT_SDC_PORT2_OPTION          = 0x81,/* This is not a boot option, it is for error handler*/
  BOOT_QSPI_EDL_OPTION           = 0x82,
  BOOT_OPTION_SIZE_ENUM          = 0x7FFFFFFF          /* Size enum to 32 bits. */
} boot_select_option_type;

/* SDC Flash shared data */
/* This defines the exact SDC device type that is detected */
typedef enum
{
  SDC_CARD_UNKNOWN = 0,
  SDC_CARD_SD      = 1,
  SDC_CARD_MMC     = 2,
  SDC_CARD_SDHC    = 5,
  SDC_CARD_MMCHC   = 6,  /* MMC card with higher than a density of 2GB */
  SDC_CARD_TYPE_SIZE = 0x7FFFFFFF
} boot_flash_sdc_card_type;

/* This defines SDC card states */
typedef enum
{ 
  SDC_CARD_IDLE = 0,
  SDC_CARD_READY,
  SDC_CARD_IDENT,
  SDC_CARD_STBY,
  SDC_CARD_TRAN,
  SDC_CARD_DATA,
  SDC_CARD_RCV,
  SDC_CARD_PRG,
  SDC_CARD_DIS,
  SDC_CARD_BTST,
  SDC_CARD_IGNORE = 0xFF,         /*dummy entry*/
  SDC_CARD_STATE_SIZE = 0x7FFFFFFF
} boot_flash_sdc_card_state_type;

/* This defines the SDC device memory access modes */
typedef enum
{
  SDC_MEM_BYTE_MODE,
  SDC_MEM_BLK_MODE,
  SDC_MEM_MODE_SIZE = 0x7FFFFFFF
} boot_flash_sdc_mem_mode_type;

/* This defines the MMC bus width */
typedef enum
{
  SDC_MMC_BUSWIDTH_1BIT = 0,
  SDC_MMC_BUSWIDTH_4BIT = 1,
  SDC_MMC_BUSWIDTH_8BIT = 2,
  SDC_MMC_BUSWIDTH_COUNT,
  SDC_MMC_BUSWIDTH_SIZE = 0x7FFFFFFF
} pbl_sdc_mmc_buswidth_type;

/* This defines the SDC Port used to boot */
/* !!!Warning!!! order of this port numbers should match with
   sdcc_reg[] array defined in sdcc driver files. sdcc_reg array
   defines base address for particular PORT and if below order
   doesn't match, you could see issues !!! */
typedef enum
{
  SDC_PORT1,
  SDC_PORT2,
  SDC_PORT_INVALID,
  SDC_PORT_SIZE = 0x7FFFFFFF
} boot_flash_sdc_port_type;

/* Defines SDC memory information */
typedef struct
{
   uint32                        block_len;       /* Block size (aka the
                                                     sector size, in bytes) */
   uint32                        block_len_shft;  /* Block length shift bits*/
   boot_flash_sdc_mem_mode_type  block_mode;      /* transfer blocks or bytes */
   uint32                        spec_vers;       /* For MMC only: MMC System
                                                     Spec version */
   uint32                        phy_parti_size;  /* number of blocks */
} boot_flash_sdc_mem_type;


typedef struct
{
   uint32             phy_parti_size;  
} boot_flash_spi_mem_type;

typedef enum
{
  SPI_PORT,
  SPI_PORT_INVALID,
  SPI_PORT_SIZE = 0x7FFFFFFF
} boot_flash_spi_port_type;

/* This defines the MMC bus width */ 
typedef enum
{
  SPI_BUSWIDTH_1BIT = 0,
  SPI_BUSWIDTH_4BIT = 1,
  SPI_BUSWIDTH_8BIT = 2,
  SPI_BUSWIDTH_COUNT,
  SPI_BUSWIDTH_SIZE = 0x7FFFFFFF
} pbl_spi_buswidth_type;

/* This is the main structure that contains
   NAND Flash specific shared data */
typedef struct
{
  uint32                      page_size_bytes;
  uint32                      page_size_shift;
  uint32                      block_size_pages;
  uint32                      block_size_shift;
  boot_ecc_bit_mode_enum_type ecc_mode;
} boot_flash_shared_nand_dev_type;

/* This is the main structure that contains
   SDC Flash specific shared data */
typedef struct
{
   boot_flash_sdc_mem_type         mem;         /* info pertaining to memory device */
   boot_flash_sdc_card_type        card_type;   /* card type  */
   boot_flash_sdc_card_state_type  card_state;  /* card state */
   uint32                          rca;         /* the card's relative address */
   uint32                          mclk;        /* current host clock speed */
   boot_flash_sdc_port_type        port;        /* Port/Slot the device was detected */
   uint32                          boot_cfg;    /* Specify boot parition */
   pbl_sdc_mmc_buswidth_type       data_width;  /* data_width ( 1/4/8 bit) */ 
   boolean                         is_gpt_parti;
   boolean                         is_primary_table;
   uint64                          starting_lba;/* starting LBA of BL partition */
   uint64                          ending_lba;  /* Last LBA of SBL partition */
} boot_flash_shared_sdc_dev_type;

typedef struct
{
  uint32                          chip_select;
  uint32                          addr_len;
  boot_flash_spi_port_type        port;        /* BLSP-QUP SPI Port where device was detected */
  pbl_spi_buswidth_type           data_width;
  boot_flash_spi_mem_type         mem;
  boolean                         is_gpt_parti;
  boolean                         is_primary_table;
  uint64                          starting_lba;
  uint64                          ending_lba;
  pbl_clock_qspi_freq_type        spi_core_clock;
} boot_flash_shared_spi_dev_type;


/* This defines Flash shared data */
typedef struct
{
  uint32                shared_data_version;
  boot_flash_type       type;        /* Type of flash                      */
  uint32                CS_base;     /* Base address of chip select device
                                        is connected to.                   */
  uint32                data_width;  /* Bit width of device ie: 8bit/16bit */
  boot_flash_ctrl_type  ctrl_type;   /* Controller used to interface with
                                        device                             */
  union
  {
    boot_flash_shared_nand_dev_type nand;  /* Shared info for NAND device  */
    boot_flash_shared_sdc_dev_type  sdc;   /* Shared info for SDC device   */
    boot_flash_shared_spi_dev_type  spi;   /* Shared info for SPI device   */
    struct boot_sahara_shared_data* sahara;/* Shared info pointer for Sahara flashless boot/eDL */
  }dev_info;
}boot_flash_shared_dev_info_type;


typedef struct
{   
uint32       shared_data_version;
uint64      *page_tbl_base;/* Page table base address */
uint32       page_tbl_size;  /* Page table size */
}pbl_cache_mmu_share_info_type;


typedef struct
{   
uint32        shared_data_version;
void*         elf_img_meta_info;           /* sec_img_auth_elf_info_type */
uint32        num_of_bytes_read_from_elf;  /* Total number of bytes ready by PBL from ELF (BL + BL-sec)*/ 
}pbl_elf_img_share_info_type;

typedef struct
{
  uint32 shared_data_version;
  uint32 pbl_apps_init_timestamp;
  uint32 bootable_media_detect_timestamp;
  uint32 bl_elf_metadata_loading_timestamp;
  uint32 bl_hash_seg_auth_timestamp;
  uint32 bl_elf_loadable_segment_loading_timestamp;
  uint32 bl_elf_segs_hash_verify_timestamp;
  uint32 bl_sec_hash_seg_auth_timestamp;
  uint32 bl_sec_segs_hash_verify_timestamp;
  uint32 pbl_populate_shared_data_and_exit_timestamp;
} pbl_apps_timestamp_type;

typedef struct pbl_secboot_shared_info_type
{
  secboot_verified_info_type pbl_verified_info;    /**<  */
  secboot_ftbl_type          pbl_secboot_ftbl;     /**< Contains pointers to PBL secboot routines */
  secboot_hw_ftbl_type       pbl_secboot_hw_ftbl;  /**< Contains pointers to PBL secboot hw routines */
} pbl_secboot_shared_info_type;

/** PCIe structure to hold the shared data for PBL profile markers timestamps */
typedef struct _pcie_bhi_shared_data
{
   uint32 version;                              /* Holds the version for the structure */
   uint32 cookie;                               /* Holds the cookie */
   uint32 pcie_pbl_bhi_init_entry_ts;           /* Start of PCIe BHI Init entry */
   uint32 pcie_pbl_wait_for_perst_ts;           /* Start of loop waiting for PERST# to be deasserted */
   uint32 pcie_pbl_perst_deasserted_ts;         /* PERST# is deasserted by the host */
   uint32 pcie_pbl_wait_for_phy_ready_ts;       /* Start of loop waiting for PHY to be ready */
   uint32 pcie_pbl_phy_ready_ts;                /* PHY is ready */
   uint32 pcie_pbl_enable_ltssm_ts;             /* Set the LTSSM bit and start link training */
   uint32 pcie_pbl_link_in_l0_ts;               /* Link Training complete and link is in L0 state */
   uint32 pcie_pbl_bme_set_ts;                  /* BME is set by the host */
   uint32 pcie_pbl_wait_for_imgtxdb_ts;         /* Start of wait for the image transfer doorbell to be set */
   uint32 pcie_pbl_imgtxdb_set_ts;              /* Image transfer doorbell is set */
   uint32 pcie_pbl_bhi_close_ts;                /* PCIe BHI close is complete */
}pcie_bhi_shared_data_type;

#if 0
typedef struct pbl_crypto_shared_info_type
{
   uint32 shared_data_version;
   int (* ecc_init)(void);
   int (* ecc_deinit)(void);
   boolean_t (* ecc_verify)(
               bigval_t const *msgdgst,  
               affine_point_t const *pubkey,
               ECDSA_sig_t const *sig);
   void (* ecc_hash_to_bigval)(
               bigval_t *tgt,
               void const *hashp,
               unsigned int hashlen);
}pbl_crypto_shared_info_type;
#endif
/*---------------------------------------------------------------------------
  Defines the clock Data structure that is filled with data that is shared by
  the PBL image
---------------------------------------------------------------------------*/
typedef struct {
  uint32                       nVersionInfo;
  uint32                       nProcSpeedMHz;
} pbl_clock_shared_data_type;
/*---------------------------------------------------------------------------
  Defines the data structure that is filled with data that is shared with
  the BL image. It may be necessary for the BL to make a deep copy of
  this structure for its own use if it wipes out PBL memory.
---------------------------------------------------------------------------*/

typedef struct boot_pbl_shared_data_type
{
 
  /* Shared data structure version */
  uint32                          shared_data_version;

  /* PBL version */
  uint32                          pbl_version;

    /* PBL Patch version */
  uint32                          pbl_patch_version;

  /* PBL Boot option configured in the fuse */
  boot_select_option_type         boot_option;

  /* BL loader authentication state */
  uint32                          bl_loader_auth_enabled;

  /* BL loader hash integrity check state */
  uint32                          hash_integrity_check_disabled; 

  
  /* BL-Sec boot flow support state */
  uint32                          bl_sec_boot_flow_supported;

  /* BL-Sec authentication state */
  uint32                          bl_sec_auth_disabled;  

  /* PBL stack base */
  uint8                           *boot_stack_base;

  /* PBL stack size */
  uint32                          boot_stack_size;
  
  /*Reserved fields*/
  uint32                          reserved_fields_bl[BL_RESERVED_LENGTH];    

   /*Timestamps at main milestones in PBL*/
  pbl_apps_timestamp_type *        timestamps;
     
  /* PBL shared data structure, maps to: pbl_sdata_type */
  void*                           bl_shared_data;

  /* Pointer to clock shared data structure, maps to: pbl_clock_shared_data_type */
  void*                           pbl_clock_shared_data;

  /* Pointer to elf image shared data structure, maps to: pbl_elf_img_share_info_type */
  void*                           elf_img_shared_info;

  /* Pointer to Secboot shared data structure, maps to: pbl_secboot_shared_info_type */
  void*                           secboot_shared_data;

  /* Pointer to PCIE shared shared data structure, maps to: pcie_bhi_shared_data_type*/
  void*                           pcie_bhi_shared_info;
    
  /* Pointer to flash shared data structure, maps to: boot_flash_shared_dev_info_type */
  void*                           flash_shared_data;
  
  /* Pointer to cache mmu shared data structure,maps to: pbl_cache_mmu_share_info_type */
  void*                           cache_mmu_share_data;

  /* Pointer to ECC shared data structure,maps to: crypto_ecc_shared_info_type */
  void*                           pbl_ecc_shared_data;

  /* Pointer to cipher Hash shared data structure,maps to: crypto_hash_cipher_shared_info_type */
  void*                           crypto_shared_data;

    /*Reserved fields*/
  uint32                          reserved_fields[BL_RESERVED_LENGTH];
} boot_pbl_shared_data_type;

#endif /* _ARM_ASM_ */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*=========================================================================

**  Function :  boot_pbl_get_pbl_version

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns the PBL version from PBL shared data.
* 
* @par Dependencies
*   None
*   
* @retval
*   uint32
* 
* @par Side Effects
*   None
* 
*/
uint32 boot_pbl_get_pbl_version
(
  void
);

/*=========================================================================

**  Function :  boot_pbl_get_clock_speed

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns the clock speed set by PBL.
* 
* @par Dependencies
*   None
*   
* @retval
*   uint32
* 
* @par Side Effects
*   None
* 
*/
uint32 boot_pbl_get_clock_speed
(
  void
);


/*===========================================================================

**  Function :  boot_pbl_get_flash_shared_data

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns the pointer to the flash related information found
*   by PBL.
* 
* @par Dependencies
*   None
* 
* @retval
*   Pointer of boot_flash_shared_dev_info_type type
* 
* @par Side Effects
*   None.
* 
*/
boot_flash_shared_dev_info_type* boot_pbl_get_flash_shared_data
(
  void
);


/*===========================================================================

**  Function :  boot_pbl_is_auth_enabled

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns whether PBL detected that auth was enabled.
* 
* @par Dependencies
*   None
* 
* @retval
*   boot_boolean
* 
* @par Side Effects
*   None.
* 
*/
boot_boolean boot_pbl_is_auth_enabled
(
  void
);


/*===========================================================================

**  Function :  boot_pbl_get_flash_type

** ==========================================================================
*/
/*!
* 
* @brief
*   This function returns the flash type detected by PBL
* 
* @par Dependencies
*   None
* 
* @retval
*   boot_flash_type
* 
* @par Side Effects
*   None.
* 
*/
boot_flash_type boot_pbl_get_flash_type
(
  void
);


/*===========================================================================

**  Function :  boot_pbl_interface_init

** ==========================================================================
*/
/*!
* 
* @brief
*   Function that will initialize the target interface so that generic SBL
*   code can use it.
* 
* @par Dependencies
*   None
* 
* @retval
*   boot_boolean
* 
* @par Side Effects
*   None
* 
*/
boot_boolean boot_pbl_interface_init
(
  boot_pbl_shared_data_type   *pbl_shared_data  /* Pointer to data coming from PBL */
);


/*===========================================================================

**  Function :  boot_pbl_log_milestones

** ==========================================================================
*/
/*!
* 
* @brief
*   This funcion will parse the PBL timestamp milestones passed to SBL
*   and insert them into the boot log.  Currently PBL's unit of measure is
*   clock ticks.  PBL does not pass the clock frequency yet so it is hard
*   wired to 19.2 Mhz.  Also PBL does not pass the names of each field so for
*   now reference a structure of our own to get the names which will have
*   logic ready for the day PBL starts passing them.
*
* @param[in]
*   boot_pbl_shared_data_type
*
* @par Dependencies
*   None
*   
* @retval
*   None
* 
* @par Side Effects
*   None
* 
*/
void boot_pbl_log_milestones(boot_pbl_shared_data_type * pbl_shared_data);


#endif /* BOOT_PBL_V2_H */

