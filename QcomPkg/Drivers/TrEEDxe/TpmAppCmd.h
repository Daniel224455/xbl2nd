#ifndef _TPM_APP_CMD_H
#define _TPM_APP_CMD_H

/*===========================================================================
  Copyright (c) 2018-2019 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  ========================================================================= */

/*===========================================================================
                      EDIT HISTORY FOR FILE



# when       who     what, where, why
# --------   ---     ---------------------------------------------------------
11/10/19     mj      Changes to read Boot Config values
02/10/18     shl     Initial Version
===========================================================================*/

#define dTPM   0x6454504D
#define fTPM   0x6654504D

#define TZEXEC_TPM_TYPE_ID   0x0080000

/* Status codes */
#define SUCCESS                 0
#define FAIL                    1

/*------------------------------------------------------------
-------------------------------------------------------------*/
typedef struct TPM_TYPE_REQ_S
{
    /* Command ID */
    UINT32  commandId;
} __attribute__ ((packed)) TPM_TYPE_REQ_T , *PTPM_TYPE_REQ_T;

typedef struct TPM_TYPE_RSP_S
{
    /* Command ID */
    UINT32  commandId;
    /* error return */
    UINT32  status;
    /* TPM type */
    UINT32  type;
} __attribute__ ((packed)) TPM_TYPE_RSP_T, *PTPM_TYPE_RSP_T;

typedef enum _BOOT_CONFIG_TYPE {
    BOOT_CONFIG_DEFAULT = 0,
    BOOT_CONFIG_SD_UFS_EDL = 1,
    BOOT_CONFIG_SD_EDL = 2,
    BOOT_CONFIG_USB_EDL = 3,
    BOOT_CONFIG_QSPI_EDL = 4,
    BOOT_CONFIG_SPI_EDL = 5,
    BOOT_CONFIG_UFS_SD_USB_EDL = 8
} BOOT_CONFIG_TYPE;

typedef enum _STORAGE_TYPE {
    STORAGE_TYPE_UFS = 0xfa,
    STORAGE_TYPE_NVME = 0xfb,
    STORAGE_TYPE_INVALID = 0xff
}STORAGE_TYPE;

#endif


