/****************************************************************************** 
**  
**   Qualcomm Technologies Incorporated
** 
**   Copyright (c) 2017 Qualcomm Technologies, Inc.
**   All rights reserved.
**   Qualcomm Technologies, Inc. Confidential and Proprietary.
** 
******************************************************************************/

/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module..
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/06/17   vk      Initial revision

============================================================================*/

#ifndef _PBL_SBL_EXTERNAL_H_
#define _PBL_SBL_EXTERNAL_H_

/******************************************************************************
                     Location for adding entries not in  main PBL header
 ******************************************************************************/

/*---------------------------------------------------------------------------
  Magic number definition for identifying PBL shared data.
---------------------------------------------------------------------------*/
#define BOOT_PBL_SHARED_DATA_MAGIC  0x78577020

/*---------------------------------------------------------------------------
  Magic words to indicate that the data structures are valid.
---------------------------------------------------------------------------*/
#define BOOT_APPS_STATUS_MAGIC          0xA4CD7EA2

/* Definitons for PBL to enter HS-USB or SD download mode. */
#define FORCE_DLOAD_BOOT_NONE       0x0
#define FORCE_DLOAD_BOOT_USB        0x1
#define FORCE_DLOAD_BOOT_SD_USB     0x2
#define FORCE_DLOAD_BOOT_SD         0x3

/*---------------------------------------------------------------------------
  Magic numbers for notifying PBL to enter download mode.
  These numbers MUST be same across bootrom otherwise customer would have to 
  change code to use force download on different bootrom. 
  !!!!!!!!!! Never change below values !!!!!!!!!!!
---------------------------------------------------------------------------*/
#define FORCE_DLOAD_MASK                 0x0000000F

#define FORCE_DLOAD_HS_USB_MAGIC_NUM     0x00000001
#define FORCE_DLOAD_SD_MAGIC_NUM         0x00000002
#define FORCE_DLOAD_MMC_MAGIC_NUM        0x00000003


/* Timestamps from PBL are given to SBL as ticks of a timer.  On some
   processors floating point might not be available so calculate
   PS_PER_PBL_TIMESTAMP_TICK (Pico seconds).  The value is not currently passed
   to SBL so it must be hardcoded here. */
#define PS_PER_PBL_TIMESTAMP_TICK   52083  /* 19.2Mhz */

/*
 * pbl_clock_sdcc_freq_type
 *
 */

typedef enum
{
  PBL_CLOCK_SDCC_FREQ_400KHZ,
  PBL_CLOCK_SDCC_FREQ_19P2MHZ,
  PBL_CLOCK_SDCC_FREQ_25MHZ,
  
  PBL_CLOCK_SDCC_FREQ_TOTAL,
  PBL_CLOCK_SDCC_FREQ_FORCE32BITS = 0x7FFFFFFF
} pbl_clock_sdcc_freq_type;

/*
 * pbl_clock_qspi_freq_type
 *
 */
typedef enum
{
  PBL_CLOCK_QSPI_FREQ_19MHZ = 0,
  PBL_CLOCK_QSPI_FREQ_75MHZ,
  PBL_CLOCK_QSPI_FREQ_120MHZ, 
  PBL_CLOCK_QSPI_FREQ_TOTAL,
  PBL_CLOCK_QSPI_FREQ_FORCE32BITS = 0x7FFFFFFF
} pbl_clock_qspi_freq_type;

/* Define Force Download Device type.
 
   If this field is anything other than FORCE_DLOAD_NONE
   then it indicates that boot option was ignored and
   corresponding force download option was taken to boot
   into SBL. */
typedef enum
{
  FORCE_DLOAD_NONE  = 0x0,
  FORCE_DLOAD_HS_USB_GPIO,
  FORCE_DLOAD_HS_USB_MAGIC_NUMBER,
  FORCE_DLOAD_SD_USB,
  FORCE_DLOAD_SD,
  FORCE_DLOAD_FORCE32BITS = 0x7FFFFFFF
} force_dload_type;

#endif /* _PBL_SBL_EXTERNAL_H_ */

