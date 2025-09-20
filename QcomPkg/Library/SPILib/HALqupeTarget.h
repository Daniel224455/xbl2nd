#ifndef _HALQUPETARGET_H
#define _HALQUPETARGET_H

/*
===========================================================================

FILE:   HALqupTarget.h

DESCRIPTION:
    This file contains enumerations for QUP specific to 8960

===========================================================================

        Edit History


When     Who    What, where, why
-------- ---    -----------------------------------------------------------
06/12/12 LK     Removed dependency on spi types.
04/19/12 ddk    Added changes for avoiding symbol match with I2C library.
                Added updated HAL implementation used for I2C.
03/28/12 ddk    Added QUP physical address info for BAM cmd desc creation.
02/22/12 ddk    Added SPI BAM mode requirements.
11/06/11 LK     Created

===========================================================================
        Copyright (c) 2011-2013 Qualcomm Technologies Incorporated
                            All Rights Reserved
                         Qualcomm Proprietary/GTDR
===========================================================================
*/

#include "QupTypes.h"
#include "QupHwio.h"

#define HAL_QUPE_TARGET_LOG
#include "QupLog.h"

#ifdef _WIN32  // Microsoft compiler is not C99 compliant and uses __inline instead of inline
#define inline __inline
#endif

/** @enum HAL_qupe_SingleTransferSizeLimitType .        */
typedef enum HAL_qupe_SingleTransferSizeLimitType
{
   HAL_QUPE_SINGLE_TRANSFER_SIZE_MIN   = 4  , /**<Min allowed transfer size.*/
   HAL_QUPE_SINGLE_TRANSFER_SIZE_MAX   = 32 , /**<Max allowed transfer size.*/

   HAL_QUPE_SINGLE_TRANSFER_SIZE_EXTEND  = 0x7FFFFFFF
} HAL_qupe_SingleTransferSizeLimitType;

/** @enum HAL_qupe_FifoModeOutTransferCountLimitType.        */
typedef enum HAL_qupe_FifoModeOutTransferCountLimitType
{
   /**
      Max allowed transfer size.
    */
   HAL_QUPE_FIFO_MODE_OUT_TRANSFER_CNT_MAX   = 0xFFFF ,

   HAL_QUPE_FIFO_MODE_OUT_TRANSFER_CNT_EXTEND  = 0x7FFFFFFF
} HAL_qupe_FifoModeOutTransferCountLimitType;


/** @enum HAL_qupe_BlockModeOutTransferCountLimitType.        */
typedef enum HAL_qupe_BlockModeOutTransferCountLimitType
{
   /**
      Max allowed transfer size.
    */
   HAL_QUPE_BLOCK_MODE_OUT_TRANSFER_CNT_MAX   = 0xFFFF ,

   HAL_QUPE_BLOCK_MODE_OUT_TRANSFER_CNT_EXTEND  = 0x7FFFFFFF
} HAL_qupe_BlockModeOutTransferCountLimitType;

/** @enum HAL_qupe_BlockModeInTransferCountLimitType.        */
typedef enum HAL_qupe_BlockModeInTransferCountLimitType
{
   /**
      Max allowed transfer size.
    */
   HAL_QUPE_BLOCK_MODE_IN_TRANSFER_CNT_MAX   = 0xFFFF ,

   HAL_QUPE_BLOCK_MODE_IN_TRANSFER_CNT_EXTEND  = 0x7FFFFFFF
} HAL_qupe_BlockModeInTransferCountLimitType;

/** @enum HAL_qupe_FifoModeInTransferCountLimitType.        */
typedef enum HAL_qupe_FifoModeInTransferCountLimitType
{
   /**
      Max allowed transfer size.
    */
   HAL_QUPE_FIFO_MODE_IN_TRANSFER_CNT_MAX   = 0xFFFF ,

   HAL_QUPE_FIFO_MODE_IN_TRANSFER_CNT_EXTEND  = 0x7FFFFFFF
} HAL_qupe_FifoModeInTransferCountLimitType;

/** @enum HAL_qupe_TimeOutLimitType Time out limit.        */
typedef enum HAL_qupe_TimeOutLimitType
{
   HAL_QUPE_TIME_OUT_MAX   = 0xFFFF, /**<Max allowed timeout.*/

   HAL_QUPE_TIME_OUT_EXTEND  = 0x7FFFFFFF
} HAL_qupe_TimeOutLimitType;

/** @enum HAL_qupe_I2cFreqLimitType I2C frequency limit.        */
typedef enum HAL_qupe_I2cFreqLimitType
{
   HAL_QUPE_FS_FREQ_KHZ_MAX   = 1000, /**<Max allowed timeout.*/

   HAL_QUPE_HS_FREQ_KHZ_MAX  = 3400
} HAL_qupe_I2cFreqLimitType;

/** @brief This function reads a QUP register and returns the entire register.
    @param[in]  base    base address of QUP instance
    @param[in]  reg     register name
    @param[in]  offset  register offset
    @param[in]  rmsk    register mask
    @return             contents of QUP register
  */
static inline uint32 qupe_in(uint8 *base, char *reg, uint32 offset, uint32 rmsk)
{
   uint32 data;
   uint32 result;

   data = *(volatile uint32 *)(base + offset);
   result = data & rmsk;
   QUPLOG_LOG_READ((uint32)base, 2, "HWIO_IN:  %s = 0x%08X", reg, data);
   return(result);
}

/** @brief This function reads a QUP register and returns a specified field.
    @param[in]  base    base address of QUP instance
    @param[in]  reg     register name
    @param[in]  field   field name
    @param[in]  offset  register offset
    @param[in]  fmsk    field mask
    @param[in]  shift   field shift
    @return             contents of QUP register field
  */
static inline uint32 qupe_inf(uint8 *base, char *reg, char *field,
                              uint32 offset, uint32 fmsk, uint32 shift)
{
   uint32 data;
   uint32 result;

   data = *(volatile uint32 *)(base + offset);
   result = (data & fmsk) >> shift;
   QUPLOG_LOG_READ((uint32)base, 4, "HWIO_IN:  %s = 0x%08X [%s = %X]",
                                    reg, data, field, result);
   return(result);
}

/** @brief This function writes an entire QUP register.
    @param[in]  base    base address of QUP instance
    @param[in]  reg     register name
    @param[in]  data    register value to write
    @param[in]  offset  register offset
    @return             void
  */
static inline void qupe_out(uint8 *base, char *reg, uint32 data, uint32 offset)
{
   QUPLOG_LOG_WRITE((uint32)base, 2, "HWIO_OUT: %s = 0x%08X", reg, data);
   *(volatile uint32 *)(base + offset) = data;
}

/** @brief This function writes a specified field in a QUP register.
    @param[in]  base    base address of QUP instance
    @param[in]  reg     register name
    @param[in]  field   field name
    @param[in]  data    register field value to write
    @param[in]  offset  register offset
    @param[in]  rmsk    register mask
    @param[in]  fmsk    field mask
    @param[in]  shift   field shift
    @return             void
  */
static inline void qupe_outf(uint8 *base, char *reg, char *field, uint32 data,
                             uint32 offset, uint32 rmsk, uint32 fmsk, uint32 shift)
{
   uint32 old_value;
   uint32 new_value;

   old_value = *(volatile uint32 *)(base + offset);
   QUPLOG_LOG_READ((uint32)base, 2, "HWIO_IN:  %s = 0x%08X", reg, old_value);

   new_value = ((old_value & rmsk) & ~fmsk) | ((data << shift) & fmsk);
   QUPLOG_LOG_WRITE((uint32)base, 4, "HWIO_OUT: %s = 0x%08X [%s = %X]",
                                     reg, new_value, field, data);
   *(volatile uint32 *)(base + offset) = new_value;
}

#define QUPE_IN(base, reg)                 qupe_in(base, #reg, \
                                                   HWIO_##reg##_OFFS, \
                                                   HWIO_##reg##_RMSK)

#define QUPE_OUT(base, reg, data)          qupe_out(base, #reg, data, \
                                                    HWIO_##reg##_OFFS)

#define QUPE_INF(base, reg, field)         qupe_inf(base, #reg, #field, \
                                                    HWIO_##reg##_OFFS, \
                                                    HWIO_##reg##_##field##_BMSK, \
                                                    HWIO_##reg##_##field##_SHFT)

#define QUPE_OUTF(base, reg, field, data)  qupe_outf(base, #reg, #field, data, \
                                                     HWIO_##reg##_OFFS, \
                                                     HWIO_##reg##_RMSK, \
                                                     HWIO_##reg##_##field##_BMSK, \
                                                     HWIO_##reg##_##field##_SHFT)

#define QUPE_INI(base, reg, index)         qupe_in(base, #reg, \
                                                   HWIO_##reg##_OFFS(base, index), \
                                                   HWIO_##reg##_RMSK)

#define QUPE_OUTI(base, reg, index, data)  qupe_out(base, #reg, data, \
                                                    HWIO_##reg##_OFFS(base, index))

#define QUPE_PHYS_ADDR(addr, reg)  (addr + HWIO_##reg##_OFFS)
#define QUPE_FMSK(reg, field)      (HWIO_##reg##_##field##_BMSK)
#define QUPE_SHFT(reg, field)      (HWIO_##reg##_##field##_SHFT)

#endif  // _HALQUPETARGET_H
