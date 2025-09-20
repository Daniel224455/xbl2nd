/*=======================================================================*//**
 * @file        qusb_ldr_log.c
 * @author:     jaychoi
 * @date        19-Sep-2017
 *
 * @brief       QUSB (Qualcomm High-Speed USB) Logging implementation.
 *
 * @details     This file contains the debug log APIs which can be used for
 *              getting log information
 * @note        
 *
 *              Copyright 2012-2018 Qualcomm Technologies, Inc.
 *              All Rights Reserved.
 *              Qualcomm Confidential and Proprietary
 * 
*//*========================================================================*/


// ===========================================================================
// 
//                            EDIT HISTORY FOR FILE
//   This section contains comments describing changes made to the module.
//   Notice that changes are listed in reverse chronological order.
// 
// 
// when         who        what, where, why
// --------     ---        ---------------------------------------------------
// 09/19/12   jaychoi      Initial revision
//
// ===========================================================================

//----------------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------------

#include "qusb_log.h"

#ifdef QUSB_ENABLE_LOGGING

#define QUSB_LOG_GEN_CNT  (4)
#define QUSB_LOG_BUS_CNT  (4)
#define QUSB_LOG_ERR_CNT  (8)

typedef struct
{
  uint16      seq_num;
  uint16      gen_idx;
  uint16      bus_idx;
  uint16      err_idx;
  qusb_log_t  gen[QUSB_LOG_GEN_CNT];
  qusb_log_t  bus[QUSB_LOG_BUS_CNT];
  qusb_log_t  err[QUSB_LOG_ERR_CNT];
} qusb_ldr_log_ctx_t;

qusb_ldr_log_ctx_t qusb_ldr_log_ctx;
uint32 qusb_ldr_log_gen_count = QUSB_LOG_GEN_CNT;
uint32 qusb_ldr_log_bus_count = QUSB_LOG_BUS_CNT;
uint32 qusb_ldr_log_err_count = QUSB_LOG_ERR_CNT;


void qusb_log(qusb_log_enum id)
{
  qusb_log_t *log       = qusb_ldr_log_ctx.gen;
  uint32      idx       = qusb_ldr_log_ctx.gen_idx;
  qusb_ldr_log_ctx.gen_idx  = (qusb_ldr_log_ctx.gen_idx + 1) % qusb_ldr_log_gen_count;

  log[idx].tt           = qusb_timetick32();
  log[idx].id           = id;
  log[idx].idx          = qusb_ldr_log_ctx.seq_num;
  qusb_ldr_log_ctx.seq_num++;

}

void qusb_log__param(qusb_log_enum id, uint32 param)
{
  qusb_log_t *log       = qusb_ldr_log_ctx.gen;
  uint32      idx       = qusb_ldr_log_ctx.gen_idx;
  qusb_ldr_log_ctx.gen_idx  = (qusb_ldr_log_ctx.gen_idx + 1) % qusb_ldr_log_gen_count;

  log[idx].tt           = qusb_timetick32();
  log[idx].id           = id;
  log[idx].p0           = param;  
  log[idx].idx          = qusb_ldr_log_ctx.seq_num;
  qusb_ldr_log_ctx.seq_num++; 
}

void qusb_log_bus(qusb_log_enum id)
{
  qusb_log_t *log       = qusb_ldr_log_ctx.bus;
  uint32      idx       = qusb_ldr_log_ctx.bus_idx;
  qusb_ldr_log_ctx.bus_idx  = (qusb_ldr_log_ctx.bus_idx + 1) % qusb_ldr_log_bus_count;

  log[idx].tt           = qusb_timetick32();
  log[idx].id           = id;
  log[idx].idx          = qusb_ldr_log_ctx.seq_num;
  qusb_ldr_log_ctx.seq_num++;

}

void qusb_log_bus__param(qusb_log_enum id, uint32 param)
{
  qusb_log_t *log       = qusb_ldr_log_ctx.bus;
  uint32      idx       = qusb_ldr_log_ctx.bus_idx;
  qusb_ldr_log_ctx.bus_idx  = (qusb_ldr_log_ctx.bus_idx + 1) % qusb_ldr_log_bus_count;

  log[idx].tt           = qusb_timetick32();
  log[idx].id           = id;
  log[idx].p0           = param;  
  log[idx].idx          = qusb_ldr_log_ctx.seq_num;
  qusb_ldr_log_ctx.seq_num++;
}

void qusb_log_err(qusb_log_enum id)
{
  qusb_log_t *log       = qusb_ldr_log_ctx.err;
  uint32      idx       = qusb_ldr_log_ctx.err_idx;
  qusb_ldr_log_ctx.err_idx  = (qusb_ldr_log_ctx.err_idx + 1) % qusb_ldr_log_err_count;

  log[idx].tt           = qusb_timetick32();
  log[idx].id           = id;
  log[idx].idx          = qusb_ldr_log_ctx.seq_num;
  qusb_ldr_log_ctx.seq_num++;
}

void qusb_log_err__param(qusb_log_enum id, uint32 param)
{
  qusb_log_t *log       = qusb_ldr_log_ctx.err;
  uint32      idx       = qusb_ldr_log_ctx.err_idx;
  qusb_ldr_log_ctx.err_idx  = (qusb_ldr_log_ctx.err_idx + 1) % qusb_ldr_log_err_count;

  log[idx].tt           = qusb_timetick32();
  log[idx].id           = id;
  log[idx].p0           = param;  
  log[idx].idx          = qusb_ldr_log_ctx.seq_num;
  qusb_ldr_log_ctx.seq_num++;
}

#endif


