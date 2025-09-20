#ifndef MIHEADER_H
#define MIHEADER_H

/*===========================================================================

                          MULTI-IMAGE HEADER FILE

DESCRIPTION
  This header file contains declarations and type definitions for the
  Image Header Information.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

* Copyright (c) 2009-2018
* Qualcomm Technologies Incorporated.
* All Rights Reserved.
* Qualcomm Confidential and Proprietary
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who                      what, where, why
--------   -------       ----------------------------------------------------------
01/18/18   vk             Add FlexSku ID
05/04/17   kpa            Add XBL config image ID
01/11/17   digants        Add AOP sec boot ID
08/25/16   digants        change STI sec boot ID
10/15/15   wg             Added SECBOOT_ABL_SW_TYPE for Android LK Image
03/02/14   ka             Added SECBOOT_QSEE_DEVCFG_SW_TYPE
10/15/14   ka             Added SECBOOT_PMIC_SW_TYPE
04/15/14   wg             Update QSEE and QHEE sw_id
12/09/13   ck             Added SECBOOT_QSEE_SW_TYPE and SECBOOT_QHEE_SW_TYPE
12/03/13   ck             Added QSEE and QHEE to secboot_sw_type
04/17/12   kedara         Update sw type for MBA and Lpass image.
06/07/12   kedara         Added sw type for MBA, APPS, LPASS images 
04/05/11   dh     	      do not include customer.h
03/27/12   dh             Added secboot_sw_type define and SECBOOT_WDT_SW_TYPE
09/21/10   plc            Added in NAND Definitions for SB3.0
08/02/10   aus            Moved image_type to mibib.h
06/01/10   plc            Refactoring for SB3.0
03/12/10   plc            Reorder IMG enum's to keep consistent with previously 
                          built images
03/09/10   plc            Keep RPM_IMG at same location in array, so RPM image 
                          won't need to change it'd ID, since it builds remotely
03/03/10   plc            Added address for SBL1
01/05/10   aus            Changes for unified boot cookie and image type
                          APPS_KERNEL_IMG
03/11/09   msm            Added Q5 decoupling and removed offset for SCl_DBL_CODE_BASE
09/28/09   vtw            Added boot cookie header and RAMFS image types.
09/09/09   msm            Port for ehostdl.
06/12/09   mrunalm        Initial porting for msm7x30.

============================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#ifndef _ARM_ASM_
#include "comdef.h"

#include "MIbib.h"
#endif  /* _ASM_ARM_ */


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* ----------------------------------------------------- */
/*   Image Header Defintion                              */
/* ----------------------------------------------------- */

#ifndef _ARM_ASM_

/*
*****************************************************************************
* @brief This structure is used as the header for the hash data segment in 
 *        signed ELF images.  The sec_img_auth component of secure boot 
 *        makes use of the image_size, code_size, signature_size and 
 *        cert_chain_size elements only.  The reserved structure elements are
*        from an earlier implementation that does not support ELF images.
*        The reserved elements are preserved to minimize the impact on code
*        signing and image creation tools.
*****************************************************************************
*/ 
typedef struct
{
  uint32 res1;            /* Reserved for compatibility: was image_id */
  uint32 res2;            /* Reserved for compatibility: was header_vsn_num */
  uint32 res3;            /* Reserved for compatibility: was image_src */
  uint32 res4;            /* Reserved for compatibility: was image_dest_ptr */
  uint32 image_size;      /* Size of complete hash segment in bytes */
  uint32 code_size;       /* Size of hash table in bytes */
  uint32 res5;            /* Reserved for compatibility: was signature_ptr */
  uint32 signature_size;  /* Size of the attestation signature in bytes */
  uint32 res6;            /* Reserved for compatibility: was cert_chain_ptr */
  uint32 cert_chain_size; /* Size of the attestation chain in bytes */

} mi_boot_image_header_type;


/*---------------------------------------------------------------------------
  Software Type identifiying image being authenticated. These values
  correspond to the code signing tools (CSMS) Software ID field which has
  lower 32 bits for Software type and upper 32 bits for Software version.
---------------------------------------------------------------------------*/
/**
@brief List of Software IDs
**/
/* type definition */
typedef uint32 secboot_sw_type;

/* SW Types for each of the subsystems; used during image authentication */

#define SECBOOT_SBL_SW_TYPE         0x0
#define SECBOOT_SBL1_SW_TYPE        (SECBOOT_SBL_SW_TYPE)
#define SECBOOT_MBA_SW_TYPE         0x1
#define SECBOOT_AMSS_SW_TYPE        (SECBOOT_MBA_SW_TYPE)
#define SECBOOT_DMSS_SW_TYPE        (SECBOOT_MBA_SW_TYPE)
#define SECBOOT_MPSS_SW_TYPE        0x2
#define SECBOOT_AMSS_HASH_TABLE_SW_TYPE (SECBOOT_MPSS_SW_TYPE)
#define SECBOOT_EHOSTDL_SW_TYPE     0x3
#define SECBOOT_FLASH_PRG_SW_TYPE   (SECBOOT_EHOSTDL_SW_TYPE)
#define SECBOOT_EHOSTD_SW_TYPE      (SECBOOT_EHOSTDL_SW_TYPE)
#define SECBOOT_ADSP_SW_TYPE        0x4
#define SECBOOT_DSP_HASH_TABLE_SW_TYPE  (SECBOOT_ADSP_SW_TYPE)
#define SECBOOT_LPASS_HASH_TABLE_TYPE   (SECBOOT_ADSP_SW_TYPE) /* Lpass hash table */
#define SECBOOT_QSEE_DEVCFG_SW_TYPE 0x5
#define SECBOOT_SBL3_SW_TYPE        0x6
#define SECBOOT_TZ_KERNEL_SW_TYPE   0x7
#define SECBOOT_QSEE_SW_TYPE        0x7
#define SECBOOT_HOSTDL_SW_TYPE      0x8
#define SECBOOT_APPSBL_SW_TYPE      0x9
#define SECBOOT_RPM_FW_SW_TYPE      0xA
#define SECBOOT_APPS_SEC_SW_TYPE    0xB
#define SECBOOT_SPS_HASH_TABLE_TYPE (SECBOOT_APPS_SEC_SW_TYPE)
#define SECBOOT_TZ_APP_SW_TYPE      0xC
#define SECBOOT_TZ_EXEC_HASH_TABLE_TYPE (SECBOOT_TZ_APP_SW_TYPE)
#define SECBOOT_WCNSS_SW_TYPE       0xD
#define SECBOOT_RIVA_HASH_TABLE_TYPE (SECBOOT_WCNSS_SW_TYPE)
#define SECBOOT_VIDEO_SW_TYPE       0xE
#define SECBOOT_APPS_HASH_TABLE_TYPE  (SECBOOT_VIDEO_SW_TYPE)
#define SECBOOT_VPU_SW_TYPE         0xF
#define SECBOOT_BCSS_SW_TYPE        0x10
#define SECBOOT_STI_SW_TYPE         0x11
#define SECBOOT_WDT_SW_TYPE         0x12
#define SECBOOT_GPU_UCODE_SW_TYPE   0x14
#define SECBOOT_QHEE_SW_TYPE        0x15
#define SECBOOT_PMIC_SW_TYPE        0x16
#define SECBOOT_CDSP_SW_TYPE        0x17
#define SECBOOT_SLPI_SW_TYPE        0x18
#define SECBOOT_EOSFW_SW_TYPE       0x19
#define SECBOOT_EFSTAR_SW_TYPE      0x1B
#define SECBOOT_ABLFV_SW_TYPE       0x1C
#define SECBOOT_ABL_SW_TYPE         (SECBOOT_ABLFV_SW_TYPE)
#define SECBOOT_IPA_GSI_SW_TYPE     0x1D
#define SECBOOT_TZ_TEE_SW_TYPE      0x1E
#define SECBOOT_TZ_LIB_SW_TYPE      0x1F
#define SECBOOT_SHRM_SW_TYPE        0x20
#define SECBOOT_AOP_SW_TYPE         0x21
#define SECBOOT_OEM_MISC_SW_TYPE    0x22
#define SECBOOT_QTI_MISC_SW_TYPE    0x23
#define SECBOOT_QUP_SW_TYPE         0x24
#define SECBOOT_XBL_CONFIG_SW_TYPE  0x25
#define SECBOOT_XBLCONFIG_SW_TYPE   (SECBOOT_XBL_CONFIG_SW_TYPE)
#define SECBOOT_FMAC_SW_TYPE        0x29
#define SECBOOT_SEC_SW_TYPE         0x2B

#define SECBOOT_APDP_SW_TYPE        0x200

#define SECBOOT_INVALID_SW_TYPE     (-1)
#define SECBOOT_MAX_SW_TYPE         0x7FFFFFFF
/*===========================================================================
                      PUBLIC FUNCTION DECLARATIONS
===========================================================================*/


#endif  /* _ASM_ARM_ */
#endif  /* MIHEADER_H */

