/*===========================================================================

              SMEM native transport for Glink Driver Source File


   Copyright (c) 2014-2015, 2019 Qualcomm Technologies, Inc.
   All Rights Reserved.
   Qualcomm Technologies, Inc. Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/30/15   ap      Handle cases when the channel is not found by rcid or lcid
08/31/15   an      Fixed tracing flags
08/17/15   bc      Add secure processor support
07/01/15   bc      Remove unnecessary header
06/19/15   pp      Update to use Ulogs and support for stats and diag cmds
05/29/15   an      QoS support
05/18/15   bc      Removed unused function call
03/24/15   an      Fixes for Tx resume
02/23/15   bc      Implemented tracer packet support
12/22/14   bc      Feature negotiation initial implement
12/09/14   db      Added channel migration specific code
11/03/14   bc      Implemented ssr
06/17/14   an      Initial version.
===========================================================================*/


/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "smem_list.h"
#include "glink.h"
#include "glink_core_if.h"
#include "glink_transport_if.h"
#include "glink_os_utils.h"
#include "glink_internal.h"
#include "glink_vector.h"
#include "xport_smem_config.h"
#include "xport_smem.h"
#include "smem.h"
#include "Protocol/EFISmem.h"

/*===========================================================================
                           MACRO DEFINITIONS
===========================================================================*/
/** Flags for use with smem_alloc_ex()/
 ** For use by Mproc code only. */
#define SMEM_ALLOC_FLAG_CACHED          0x80000000 /**< Allocate from cached 
                                                    **  heap */
#define SMEM_ALLOC_FLAG_PARTITION_ONLY  0x40000000 /**< Allocate from edge 
                                                    **  partition only. */
#define XPORT_SMEM_NAME "SMEM"

/*Check for xport logging filter mask bit and edge before logging*/
#define XPORT_SMEM_LOG_MSG(host_id, msg, remote_host_name, param)            \
  if (((log_filter_cfg.xport_filter_mask &                                   \
        (1<<GLINK_XPORT_SMEM_MASK_BIT)) != 0) &&                             \
       ((log_filter_cfg.ch_filter_status == FALSE) ||                        \
        ((uint32)host_id == log_filter_cfg.remote_host)))                    \
  {                                                                          \
    GLINK_LOG_EVENT_NO_FILTER( GLINK_EVENT_XPORT_INTERNAL, msg,              \
                               XPORT_SMEM_NAME, remote_host_name,            \
                               (uint32)param);                               \
  }

#define XPORT_SMEM_LOG_ERROR(msg, remote_host_name, param)                \
  GLINK_LOG_ERROR_EVENT(GLINK_EVENT_XPORT_INTERNAL, msg, XPORT_SMEM_NAME, \
			            remote_host_name, (uint32)param);

#define XPORT_BASE_VERSION          ( 1 )

#define XPORT_BASE_FEATURE          ( 0x1 )
#define XPORT_CH_MIGRATION_FEATURE  ( 0x2 )
#define XPORT_TRACER_PACKET_FEATURE ( 0x4 )

#define XPORT_SMEM_ENABLE_TRACER_PACKET   ( TRUE )
#define XPORT_SMEM_DISABLE_TRACER_PACKET  ( FALSE )

/*===========================================================================
                              GLOBAL DATA DECLARATIONS
===========================================================================*/

extern const uint32 xport_smem_config_num;
extern const smem_host_type xport_smem_this_host;

xport_smem_config_type* xport_smem_get_config(uint32 ind);

xport_smem_ctx_type *xport_smem_ctx = NULL;
glink_core_version_type *xport_smem_version = NULL;

static EFI_SMEM_PROTOCOL *xport_smem_smem_protocol_iface;

/*===========================================================================
                    LOCAL FUNCTION DECLARATIONS
===========================================================================*/
uint32 xport_smem_negotiate_features
(
  glink_transport_if_type *if_ptr,
  const glink_core_version_type *version_ptr,
  uint32 features
);

/*===========================================================================
                    LOCAL FUNCTION DEFINITIONS
===========================================================================*/
/*===========================================================================
FUNCTION      xport_smem_smem_alloc_ex
===========================================================================*/
/**
  Wrapper function for smem_alloc_ex().
*/
/*=========================================================================*/
static int32 xport_smem_smem_alloc_ex
(
  smem_alloc_params_type *params
)
{
  EFI_STATUS ret;
  
  if (xport_smem_smem_protocol_iface)
  {
    ret = xport_smem_smem_protocol_iface->SmemAllocEx(params);
    /* TODO: Translate the FEI error to SMEM error */
  }
  else
  {
    ret = SMEM_STATUS_FAILURE;
  }
  
  return ret;
}

/*===========================================================================
FUNCTION      xport_smem_smem_get_addr_ex
===========================================================================*/
/**
  Wrapper function for smem_get_addr_ex().
*/
/*=========================================================================*/
static int32 xport_smem_smem_get_addr_ex
(
  smem_alloc_params_type *params
)
{
  EFI_STATUS ret;

  if (xport_smem_smem_protocol_iface)
  {
    ret = xport_smem_smem_protocol_iface->SmemGetAddrEx(params);
    /* TODO: Translate the FEI error to SMEM error */
  }
  else
  {
    ret = SMEM_STATUS_FAILURE;
  }
  
  return ret;
}

/*===========================================================================
FUNCTION      xport_smem_read64
===========================================================================*/
/**

  Reads 64 bit value from Rx FIFO and updates Rx indices.

  @param[in]      ctx_ptr      Pointer to transport context.
  @param[in/out]  rx_ind_ptr   Pinter to indices.

  @return         Read 64-bit value.

  @sideeffects    None.

  @dependencies   None.
*/
/*=========================================================================*/
static uint64 xport_smem_read64
(
  xport_smem_ctx_type *ctx_ptr, 
  xport_smem_ind_type *rx_ind_ptr
)
{
  uint64 ret = *(uint64*)&ctx_ptr->rx_fifo[rx_ind_ptr->read_ind];

  rx_ind_ptr->read_ind += sizeof(uint64);

  if (rx_ind_ptr->read_ind >= ctx_ptr->rx_fifo_size)
  {
    rx_ind_ptr->read_ind = 0;
  }

  return ret;
}

/*===========================================================================
FUNCTION      xport_smem_get_rx_size
===========================================================================*/
/**

  Calculates size of available Rx data.

  @param[in]      ctx_ptr      Pointer to transport context.
  @param[in/out]  rx_ind_ptr   Pinter to indices.

  @return         Size of available Rx data.

  @sideeffects    None.

  @dependencies   None.
*/
/*=========================================================================*/
static uint32 xport_smem_get_rx_size
(
  xport_smem_ctx_type *ctx_ptr, 
  xport_smem_ind_type *rx_ind_ptr
)
{
  uint32 rx_size = rx_ind_ptr->write_ind - rx_ind_ptr->read_ind;

  if (rx_ind_ptr->write_ind < rx_ind_ptr->read_ind)
  {
    rx_size += ctx_ptr->rx_fifo_size;
  }

  return rx_size;
}
/*===========================================================================
FUNCTION      xport_smem_read_algn64
===========================================================================*/
/**

  Reads data from Rx FIFO and updates Rx indices to 64-bit alignment.

  @param[in]      ctx_ptr      Pointer to transport context.
  @param[in/out]  rx_ind_ptr   Pinter to indices.
  @param[out]     buffer_ptr   Pinter to buffer to read into.
  @param[in]      data_size    Size of data to read.

  @return         None.

  @sideeffects    None.

  @dependencies   None.
*/
/*=========================================================================*/
static void xport_smem_read_algn64
(
  xport_smem_ctx_type *ctx_ptr, 
  xport_smem_ind_type *rx_ind_ptr, 
  char                *buffer_ptr,
  uint32              data_size
)
{
  uint32 data_size64, rx_last, rx_size;

  rx_size = xport_smem_get_rx_size(ctx_ptr, rx_ind_ptr);

  if (data_size > rx_size)
  {
    data_size = rx_size;
    ASSERT(0); //should never happen
  }

  /* Round up to 64 bit */
  data_size64 = glink_os_roundup64(data_size);

  rx_last = ctx_ptr->rx_fifo_size - rx_ind_ptr->read_ind;

  if (rx_last <= data_size)
  {
    glink_os_copy_mem(buffer_ptr, &ctx_ptr->rx_fifo[rx_ind_ptr->read_ind], rx_last);
    
    rx_ind_ptr->read_ind = 0;
    data_size -= rx_last;
    data_size64 -= rx_last;
    buffer_ptr += rx_last;
  }

  if (data_size != 0)
  {
    glink_os_copy_mem(buffer_ptr, &ctx_ptr->rx_fifo[rx_ind_ptr->read_ind], data_size);
  }

  rx_ind_ptr->read_ind += data_size64;

  if (rx_ind_ptr->read_ind >= ctx_ptr->rx_fifo_size)
  {
    rx_ind_ptr->read_ind = 0;
  }
}

/*===========================================================================
FUNCTION      xport_smem_write64
===========================================================================*/
/**

  Writes 64 bit value into Tx FIFO.

  @param[in]  ctx_ptr       Pointer to transport context.
  @param[in]  reserve_size  Extra space to reserve.
  @param[in]  value         Value to write.

  @return     TRUE if the write succeeded, FALSE otherwise.

  @sideeffects  Internal write pointer is updated but shared write pointer 
                is not touched.

  @dependencies It should be invoked within tx_link_lock protection.
*/
/*=========================================================================*/
static boolean xport_smem_write64
(
  xport_smem_ctx_type *ctx_ptr,
  uint32              reserve_size, 
  uint64              value
)
{
  uint32 read_ind = ctx_ptr->tx_desc->read_ind;
  uint32 write_ind = ctx_ptr->local_ind.write_ind;

  if (!glink_os_fifo_index_check(ctx_ptr->tx_fifo_size, read_ind, 0))
  {
    OS_LOG_ERROR(3, "Invalid TX read_ind=0x%X index, ctx_ptr=0x%p.",
                     read_ind, ctx_ptr);
    return FALSE;
  }
  uint32 avail_size = read_ind > write_ind ? read_ind - write_ind :
                      ctx_ptr->tx_fifo_size - (write_ind - read_ind);

  /* There should be at least 64 bit gap after writing */
  if (avail_size < glink_os_add(2*sizeof(uint64), reserve_size))
  {
    return FALSE;
  }

  *(uint64*)&ctx_ptr->tx_fifo[write_ind] = value;

  write_ind += sizeof(uint64);

  if (write_ind >= ctx_ptr->tx_fifo_size)
  {
    write_ind = 0;
  }

  ctx_ptr->local_ind.write_ind = write_ind;

  return TRUE;
}

/*===========================================================================
FUNCTION      xport_smem_write_algn64
===========================================================================*/
/**

  Writes data into Tx FIFO and aligns write pointer to 64-bit.

  @param[in]  ctx_ptr      Pointer to transport context.
  @param[in]  buffer_ptr   Data to write.
  @param[in]  data_size    Size of data.

  @return     TRUE if the write succeeded, FALSE otherwise.

  @sideeffects  Internal write pointer is updated but shared write pointer 
                is not touched.

  @dependencies It should be invoked within tx_link_lock protection.
*/
/*=========================================================================*/
static boolean xport_smem_write_algn64
(
  xport_smem_ctx_type *ctx_ptr, 
  char                *buffer_ptr,
  uint32              data_size
)
{
  uint32 read_ind = ctx_ptr->tx_desc->read_ind;
  uint32 write_ind = ctx_ptr->local_ind.write_ind;
  uint32 tx_last = ctx_ptr->tx_fifo_size - write_ind;
  uint32 data_size64 = glink_os_roundup64(data_size);
  uint32 avail_size;

  if (!glink_os_fifo_index_check(ctx_ptr->tx_fifo_size, read_ind, 0))
  {
    OS_LOG_ERROR(3, "Invalid TX read_ind=0x%X index, ctx_ptr=0x%p.",
                     read_ind, ctx_ptr);
    return FALSE;
  }

  avail_size = read_ind > write_ind ? read_ind - write_ind :
                      tx_last + read_ind;

  /* There should be at least 64 bit gap after writing */
  if (avail_size < glink_os_add(sizeof(uint64), data_size64))
  {
    return FALSE;
  }

  if (tx_last <= data_size)
  {
    glink_os_copy_mem(&ctx_ptr->tx_fifo[write_ind], buffer_ptr, tx_last);
    
    write_ind = 0;
    data_size -= tx_last;
    data_size64 -= tx_last;
    buffer_ptr += tx_last;
  }

  if (data_size != 0)
  {
    glink_os_copy_mem(&ctx_ptr->tx_fifo[write_ind], buffer_ptr, data_size);
  }

  write_ind += data_size64;

  if (write_ind >= ctx_ptr->tx_fifo_size)
  {
    write_ind = 0;
  }

  ctx_ptr->local_ind.write_ind = write_ind;

  return TRUE;
}

/*===========================================================================
FUNCTION      xport_smem_send_event
===========================================================================*/
/**

  Updates shared write index and sends interrupt.

  @param[in]  ctx_ptr   Pointer to transport context.

  @return     None.

  @sideeffects  None.

  @dependencies It should be invoked within tx_link_lock protection.
*/
/*=========================================================================*/
static void xport_smem_send_event
(
  xport_smem_ctx_type *ctx_ptr
)
{
  ctx_ptr->tx_desc->write_ind = ctx_ptr->local_ind.write_ind;

  XPORT_SMEM_LOG_MSG(ctx_ptr->pcfg->remote_host, "Send event write ind", 
                     ctx_ptr->pcfg->remote_ss, ctx_ptr->local_ind.write_ind);

  glink_os_send_interrupt((os_ipc_intr_type *)&ctx_ptr->pcfg->irq_out);
}

/*===========================================================================
FUNCTION      xport_smem_add_cmd
===========================================================================*/
/**

  Helper to add to Tx pending list a single command.

  @param[in]  ctx_ptr           Pointer to transport context.
  @param[in]  cmd_raw64         64-bit value of the command.
  @param[in]  attachment_size   Size of command attachment.
  @param[in]  pattachment       Pointer to the attachment data.

  @return     TRUE if command was added successfully, FALSE otherwise.

  @sideeffects  None.

  @dependencies Attachment data should not be freed while it is in Tx list.
*/
/*=========================================================================*/
static boolean xport_smem_add_cmd
(
  xport_smem_ctx_type *ctx_ptr,
  uint64               cmd_raw64,
  uint32               attachment_size,
  void*                pattachment
)
{
  xport_smem_cmd_item_type* pdesc;
  
  glink_os_cs_acquire(ctx_ptr->tx_free_list_lock);
  pdesc = smem_list_first(&ctx_ptr->tx_free_list);
  if (pdesc != NULL)
  {
    smem_list_delete(&ctx_ptr->tx_free_list, pdesc);
  }
  glink_os_cs_release(ctx_ptr->tx_free_list_lock);
  
  if (pdesc == NULL)
  {
    pdesc = glink_os_malloc(sizeof(xport_smem_cmd_item_type));
    
    if (!pdesc)
    {
      XPORT_SMEM_LOG_ERROR("add cmd failed", ctx_ptr->pcfg->remote_ss, (uint32)cmd_raw64);
      return FALSE;
    }
  }

  pdesc->cmd.raw64 = cmd_raw64;
  pdesc->attachment_size = attachment_size;
  pdesc->pattachment = pattachment;

  glink_os_cs_acquire(ctx_ptr->tx_list_lock);

  smem_list_append(&ctx_ptr->tx_list, pdesc);
  glink_os_cs_release(ctx_ptr->tx_list_lock);
 
  return TRUE;
}

/*===========================================================================
FUNCTION      xport_smem_send_cmd
===========================================================================*/
/**

  Helper to send a single command.

  @param[in]  ctx_ptr           Pointer to transport context.
  @param[in]  cmd_raw64         64-bit value of the command
  @param[in]  attachment_size   Size of command attachment.
  @param[in]  pattachment       Pointer to the attachment data

  @return     Returns error code.

  @sideeffects  Attachment data will be copied into an allocated buffer
                if the command could not be sent.

  @dependencies None.
*/
/*=========================================================================*/
static glink_err_type xport_smem_send_cmd
(
  xport_smem_ctx_type *ctx_ptr,
  uint64               cmd_raw64,
  uint32               attachment_size,
  void*                pattachment
)
{
  glink_err_type ret = GLINK_STATUS_SUCCESS;

  glink_os_cs_acquire(ctx_ptr->tx_link_lock);

  XPORT_SMEM_LOG_MSG(ctx_ptr->pcfg->remote_host, "send cmd", 
                     ctx_ptr->pcfg->remote_ss, (uint32)cmd_raw64);
  
  do
  {
    void* pdata;
    xport_smem_cmd_type cmd;

    if (smem_list_count(&ctx_ptr->tx_list) == 0)
    {
      uint32 reserve_size = glink_os_roundup64(attachment_size) + sizeof(uint64);
      uint32 tx_write_ind_temp = ctx_ptr->local_ind.write_ind;

      if (xport_smem_write64(ctx_ptr, reserve_size, cmd_raw64))
      {
        if (pattachment != NULL)
        {
          if (xport_smem_write_algn64(ctx_ptr, pattachment, attachment_size))
          {
            break;
        }
          else
          {
            /* Restore the write index in case of fail to write attachment 
             * Queue command into TX list */
            ctx_ptr->local_ind.write_ind = tx_write_ind_temp;
          }
        }
        else
        {
        break;
      }
    }
    }

    if (pattachment != NULL)
    {
      pdata = glink_os_malloc(attachment_size);

      if (!pdata)
      { 
        ret = GLINK_STATUS_OUT_OF_RESOURCES;
        break;
      }

      glink_os_copy_mem(pdata, pattachment, attachment_size);
    }
    else
    {
      pdata = NULL;
    }

    if (!xport_smem_add_cmd(ctx_ptr, cmd_raw64, attachment_size, pdata))
    {
      glink_os_free(pdata);
      ret = GLINK_STATUS_OUT_OF_RESOURCES;
      break;
    }

    cmd.gen_cmd.id = XPORT_SMEM_CMD_READ_REQ;
    xport_smem_write64(ctx_ptr, 0, cmd.raw64);

  } while (0);

  if (ret == GLINK_STATUS_SUCCESS)
  {
    xport_smem_send_event(ctx_ptr);
  }

  glink_os_cs_release(ctx_ptr->tx_link_lock);

  return ret;
}

/*===========================================================================
FUNCTION      xport_smem_process_rx_packet_cmd
===========================================================================*/
/**

  read a data packet or a fragment of it and notifies core

  @param[in]  ctx_ptr            Pointer to smem xport context
  @param[in]  cmd                command received
  @param[in]  rx_ind             rx index of fifo
  @param[in]  log_tracer_packet  tells you whether to log tracer packek event

  @return     Returns error code.

  @sideeffects  None.

  @dependencies None.
*/
/*=========================================================================*/
static void xport_smem_process_rx_packet_cmd
(
  xport_smem_ctx_type     *ctx_ptr,
  xport_smem_tx_cmd_type  *cmd,
  xport_smem_ind_type     *rx_ind,
  boolean                  log_tracer_packet
)
{
  glink_rx_intent_type* pdesc;
  xport_smem_cmd_type cmd_arg;

#ifdef GLINK_MEMCPY_PROFILE_ENABLED
  if( log_tracer_packet )
  {
    tracer_packet_cache_rx_timetick( GLINK_RX_MEMCPY_EVENT );
  }
#endif
  
  cmd_arg.raw64 = xport_smem_read64(ctx_ptr, rx_ind);

  pdesc = ctx_ptr->xport_if.glink_core_if_ptr->rx_get_pkt_ctx(
    (glink_transport_if_type *)ctx_ptr,
    cmd->cid, cmd->iid);

  if (pdesc != NULL)
  {
    uint32 pkt_size = glink_os_add(cmd_arg.data_desc.fragment_size, 
                                   cmd_arg.data_desc.remaining_size);
    uint32 rx_size = xport_smem_get_rx_size(ctx_ptr, rx_ind);

    if (pdesc->size < pkt_size)
    {
      pkt_size = (uint32)pdesc->size;
      ASSERT(0);
    }

    if (cmd_arg.data_desc.fragment_size > pdesc->size)
    {
      cmd_arg.data_desc.fragment_size = (uint32)pdesc->size;
      ASSERT(0);
    }

    if (cmd_arg.data_desc.fragment_size > rx_size)
    {
      cmd_arg.data_desc.fragment_size = rx_size;
      ASSERT(0);
    }

    xport_smem_read_algn64( ctx_ptr, rx_ind, 
                            ((glink_iovector_type*)pdesc->iovec)->vlist->data, 
                            cmd_arg.data_desc.fragment_size );

    pdesc->pkt_sz     = pkt_size;
    pdesc->used       = cmd_arg.data_desc.fragment_size;
    pdesc->tracer_pkt = XPORT_SMEM_DISABLE_TRACER_PACKET;
    
    ctx_ptr->xport_if.glink_core_if_ptr->rx_put_pkt_ctx(
      (glink_transport_if_type *)ctx_ptr, cmd->cid, pdesc,
      cmd_arg.data_desc.remaining_size == 0 ? TRUE : FALSE);
  }
}

/*===========================================================================
FUNCTION      xport_smem_process_rx_packet_cont_cmd
===========================================================================*/
/**

  read continued data packet or a fragment of it and notifies core

  @param[in]  ctx_ptr            Pointer to smem xport context
  @param[in]  cmd                command received
  @param[in]  rx_ind             rx index of fifo
  @param[in]  log_tracer_packet  tells you whether to log tracer packek event

  @return     Returns error code.

  @sideeffects  None.

  @dependencies None.
*/
/*=========================================================================*/
static void xport_smem_process_rx_packet_cont_cmd
(
  xport_smem_ctx_type     *ctx_ptr,
  xport_smem_tx_cmd_type  *cmd,
  xport_smem_ind_type     *rx_ind,
  boolean                  log_tracer_packet
)
{
  xport_smem_cmd_type cmd_arg;
  glink_rx_intent_type* pdesc;

  cmd_arg.raw64 = xport_smem_read64(ctx_ptr, rx_ind);

  pdesc = ctx_ptr->xport_if.glink_core_if_ptr->rx_get_pkt_ctx(
    (glink_transport_if_type *)ctx_ptr,
    cmd->cid, cmd->iid);

  if (pdesc != NULL)
  {
    uint32 rx_size = xport_smem_get_rx_size(ctx_ptr, rx_ind);

    if (cmd_arg.data_desc.fragment_size > rx_size)
    {
      cmd_arg.data_desc.fragment_size = rx_size;
      ASSERT(0);
    }

    if (cmd_arg.data_desc.fragment_size > (pdesc->pkt_sz - pdesc->used))
    {
      cmd_arg.data_desc.fragment_size = (uint32)(pdesc->pkt_sz - pdesc->used);
      ASSERT(0);
    }

    xport_smem_read_algn64(ctx_ptr, rx_ind, 
      (char*)(((glink_iovector_type*)pdesc->iovec)->vlist->data) + pdesc->used,
      cmd_arg.data_desc.fragment_size);

    pdesc->used      += cmd_arg.data_desc.fragment_size;
    pdesc->tracer_pkt = XPORT_SMEM_DISABLE_TRACER_PACKET;
    
    ctx_ptr->xport_if.glink_core_if_ptr->rx_put_pkt_ctx(
      (glink_transport_if_type *)ctx_ptr, cmd->cid, pdesc,
      cmd_arg.data_desc.remaining_size == 0 ? TRUE : FALSE);
  }
}

/*===========================================================================
FUNCTION      xport_smem_alloc_common_proc_fifo
===========================================================================*/
/**
  allocate smem fifo/shared indices for all q6 and apss
  
  @param[in]  ctx_ptr   Pointer to processor edge context

  @return     TRUE if allocation was successful.
              FALSE otherwise

  @sideeffects  None.
*/
/*=========================================================================*/
static boolean xport_smem_alloc_common_proc_fifo
(
  xport_smem_ctx_type *ctx_ptr
)
{
  smem_alloc_params_type smem_params;
  uint32 local_host_ind;
  
  /* Allocate SMEM channel descriptor item */
  smem_params.remote_host = ctx_ptr->pcfg->remote_host;
  smem_params.smem_type   = SMEM_GLINK_NATIVE_XPORT_DESCRIPTOR;
  smem_params.size        = sizeof(xport_smem_ch_desc_type);
  smem_params.buffer      = NULL;
  smem_params.flags       = SMEM_ALLOC_FLAG_PARTITION_ONLY;

  if (xport_smem_smem_alloc_ex(&smem_params) != SMEM_STATUS_SUCCESS)
  {
    /* SMEM allocation, set index to invalid. */
    ctx_ptr->pcfg = NULL;
    return FALSE;
  }

  ASSERT(smem_params.buffer != NULL);
 
  ctx_ptr->ch_desc = smem_params.buffer;

  /* Initialize indices */
  local_host_ind = xport_smem_this_host < ctx_ptr->pcfg->remote_host ? 0 : 1;

  ctx_ptr->tx_desc = &ctx_ptr->ch_desc->shared_ind[local_host_ind];
  ctx_ptr->rx_desc = &ctx_ptr->ch_desc->shared_ind[local_host_ind^1];

  /* Allocate SMEM Tx FIFO item */
  smem_params.remote_host = ctx_ptr->pcfg->remote_host;
  smem_params.smem_type   = SMEM_GLINK_NATIVE_XPORT_FIFO_0 + local_host_ind;
  smem_params.size        = ctx_ptr->pcfg->fifo_size;
  smem_params.buffer      = NULL;
  smem_params.flags       = SMEM_ALLOC_FLAG_PARTITION_ONLY;

  if (xport_smem_smem_alloc_ex(&smem_params) != SMEM_STATUS_SUCCESS)
  {
    /* SMEM allocation, set index to invalid. */
    ctx_ptr->pcfg = NULL;
    return FALSE;
  }

  ASSERT(smem_params.buffer != NULL);
 
  ctx_ptr->tx_fifo      = smem_params.buffer;
  ctx_ptr->tx_fifo_size = smem_params.size;

  return TRUE;
}

/*===========================================================================
                    EXTERNAL FUNCTION DEFINITIONS
===========================================================================*/

/******************** CHANNEL MIGRATION FEATURE Functions ********************/
/*===========================================================================
FUNCTION      xport_smem_tx_cmd_ch_open_ch_migration_feature
===========================================================================*/
/**

  Sends the open command - expected response is to be delivered 
  via glink_rx_cmd_ch_open_ack().
  This is channel migration feature (2)

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  lcid     Local channel ID.
  @param[in]  name     Channel name.

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_tx_cmd_ch_open_ch_migration_feature
(
  glink_transport_if_type *if_ptr,
  uint32 lcid,
  const char *name,
  glink_xport_priority prio
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.ch_cmd.id = XPORT_SMEM_CMD_OPEN_CHANNEL;
  cmd.ch_cmd.cid = (uint16)lcid;
  cmd.ch_cmd.name_len = (uint16)strlen(name) + 1;
  cmd.ch_cmd.prio = (uint16)prio;
  return xport_smem_send_cmd(ctx_ptr, cmd.raw64, cmd.ch_cmd.name_len, (void*)name);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_ch_remote_open_ack_ch_migration_feature
===========================================================================*/
/**

  Sends the remote open ACK command.
  This is channel migration feature

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  rcid     Remote channel ID.

  @return     None.

  @sideeffects  None.
*/
/*=========================================================================*/
void xport_smem_tx_cmd_ch_remote_open_ack_ch_migration_feature
(
  glink_transport_if_type *if_ptr,
  uint32 rcid,
  glink_xport_priority prio  
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.ch_cmd.id = XPORT_SMEM_CMD_OPEN_CHANNEL_ACK;
  cmd.ch_cmd.cid = (uint16)rcid;
  cmd.ch_cmd.name_len = 0;
  cmd.ch_cmd.prio = (uint16)prio;
  xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/************************ BASE FEATURE Functions ************************
 * These functions are base feature functions ( 0 )  
 */
/*===========================================================================
FUNCTION      xport_smem_tx_cmd_version
===========================================================================*/
/**

  Transmit a version command for local negotiation - 
  expected response is to be delivered via glink_rx_cmd_version_ack().

  @param[in]  if_ptr    Pointer to transport interface instance.
  @param[in]  version   Version to be sent.
  @param[in]  feature   Features to be sent.

  @return     None.

  @sideeffects  None.
*/
/*=========================================================================*/
void xport_smem_tx_cmd_version
(
  glink_transport_if_type *if_ptr,
  uint32 version,
  uint32 features
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.ver_cmd.id = XPORT_SMEM_CMD_VERSION_REQ;
  cmd.ver_cmd.version = (uint16)version;
  cmd.ver_cmd.features = features;

  xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_version_ack
===========================================================================*/
/**

  Transmit a version ack for remote negotiation.

  @param[in]  if_ptr    Pointer to transport interface instance.
  @param[in]  version   Version to be sent.
  @param[in]  feature   Features to be sent.

  @return     None.

  @sideeffects  None.
*/
/*=========================================================================*/
void xport_smem_tx_cmd_version_ack
(
  glink_transport_if_type *if_ptr,
  uint32 version,
  uint32 features
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.ver_cmd.id = XPORT_SMEM_CMD_VERSION_ACK;
  cmd.ver_cmd.version = (uint16)version;
  cmd.ver_cmd.features = features;

  xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_ch_open
===========================================================================*/
/**

  Sends the open command - expected response is to be delivered 
  via glink_rx_cmd_ch_open_ack().

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  lcid     Local channel ID.
  @param[in]  name     Channel name.
  @param[in]  prio     xport priority requested by channel

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_tx_cmd_ch_open
(
  glink_transport_if_type *if_ptr,
  uint32 lcid,
  const char *name,
  glink_xport_priority prio
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.ch_cmd.id = XPORT_SMEM_CMD_OPEN_CHANNEL;
  cmd.ch_cmd.cid = (uint16)lcid;
  cmd.ch_cmd.name_len = (uint16)strlen(name) + 1;

  return xport_smem_send_cmd(ctx_ptr, cmd.raw64, cmd.ch_cmd.name_len, (void*)name);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_ch_close
===========================================================================*/
/**

  Sends the close command - expected response is to be delivered 
  via glink_rx_cmd_ch_close_ack().

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  lcid     Local channel ID.

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_tx_cmd_ch_close
(
  glink_transport_if_type *if_ptr,
  uint32 lcid
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.ch_cmd.id = XPORT_SMEM_CMD_CLOSE_CHANNEL;
  cmd.ch_cmd.cid = (uint16)lcid;
  cmd.ch_cmd.name_len = 0;

  return xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_ch_remote_open_ack
===========================================================================*/
/**

  Sends the remote open ACK command.

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  rcid     Remote channel ID.
  @param[in]  prio     Negotiated xport priority to be sent to remote side

  @return     None.

  @sideeffects  None.
*/
/*=========================================================================*/
void xport_smem_tx_cmd_ch_remote_open_ack
(
  glink_transport_if_type *if_ptr,
  uint32 rcid,
  glink_xport_priority prio  
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.ch_cmd.id = XPORT_SMEM_CMD_OPEN_CHANNEL_ACK;
  cmd.ch_cmd.cid = (uint16)rcid;
  cmd.ch_cmd.name_len = 0;

  xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_ch_remote_close_ack
===========================================================================*/
/**

  Sends the remote close ACK command.

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  rcid     Remote channel ID.

  @return     None.

  @sideeffects  None.
*/
/*=========================================================================*/
void xport_smem_tx_cmd_ch_remote_close_ack
(
  glink_transport_if_type *if_ptr,
  uint32 rcid
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.ch_cmd.id = XPORT_SMEM_CMD_CLOSE_CHANNEL_ACK;
  cmd.ch_cmd.cid = (uint16)rcid;
  cmd.ch_cmd.name_len = 0;

  xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_rx_intent_req
===========================================================================*/
/**

  Sends the intent request command.

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  lcid     Local channel ID.
  @param[in]  size     size of requested intent.

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_tx_cmd_rx_intent_req
(
  glink_transport_if_type *if_ptr,
  uint32 lcid,
  size_t size
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.int_req_cmd.id = XPORT_SMEM_CMD_INTENT_REQ;
  cmd.int_req_cmd.cid = (uint16)lcid;
  cmd.int_req_cmd.size = (uint32)size;

  return xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_remote_rx_intent_req_ack
===========================================================================*/
/**

  Sends the ack for remote intent request.

  @param[in]  if_ptr    Pointer to transport interface instance.
  @param[in]  lcid      Local channel ID.
  @param[in]  granted   Whether the intent request iss granted. 

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_tx_cmd_remote_rx_intent_req_ack
(
  glink_transport_if_type *if_ptr,
  uint32 lcid,
  boolean granted
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.int_ack_cmd.id = XPORT_SMEM_CMD_INTENT_ACK;
  cmd.int_ack_cmd.cid = (uint16)lcid;
  cmd.int_ack_cmd.ack = granted ? 1 : 0;

  return xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_local_rx_intent
===========================================================================*/
/**

  Send receive intent command.

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  lcid     Local channel ID.
  @param[in]  size     Size of receive intent.
  @param[in]  liid     Local intent ID.

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_tx_cmd_local_rx_intent
(
  glink_transport_if_type *if_ptr,
  uint32 lcid,
  size_t size,
  uint32 liid
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd[2];

  /* Send the command and intent descriptor */  
  cmd[0].rx_int_cmd.id = XPORT_SMEM_CMD_RX_INTENT;
  cmd[0].rx_int_cmd.cid = (uint16)lcid;
  cmd[0].rx_int_cmd.intent_num = 1;
  cmd[1].int_desc.size = (uint32)size;
  cmd[1].int_desc.id = liid;

  return xport_smem_send_cmd(ctx_ptr, cmd[0].raw64, sizeof(cmd[1]), &cmd[1]);
}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_local_rx_done
===========================================================================*/
/**

  Send receive-done command.

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  lcid     Local channel ID.
  @param[in]  liid     Local intent ID.
  @param[in]  reuse    Reuse intent.

  @return     None.

  @sideeffects  None.
*/
/*=========================================================================*/
void xport_smem_tx_cmd_local_rx_done
(
  glink_transport_if_type *if_ptr,
  uint32 lcid,
  uint32 liid,
  boolean reuse
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.rx_done_cmd.id = reuse ? XPORT_SMEM_CMD_RX_DONE_REUSE : 
                               XPORT_SMEM_CMD_RX_DONE;
  cmd.rx_done_cmd.cid = (uint16)lcid;
  cmd.rx_done_cmd.iid = liid;

  xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_tx
===========================================================================*/
/**

  Send a data packet or a fragment of it.

  @param[in]  if_ptr   Pointer to transport interface instance.
  @param[in]  lcid     Local channel ID.
  @param[in]  pctx     Packet TX context.

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_tx
(
  glink_transport_if_type *if_ptr,
  uint32 lcid,
  glink_core_tx_pkt_type *pctx
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd[2];
  size_t frag_size;
  void* frag_data;
  
  if( pctx->tracer_pkt && 
      !( ctx_ptr->negotiated_version.features & XPORT_TRACER_PACKET_FEATURE ) )
  {
    /* TRACER PACKET FEATURE IS NOT SUPPORTED for this smem context */
    return GLINK_STATUS_FAILURE;
  }
  
  if (pctx->vprovider != NULL)
  {
    frag_data = pctx->vprovider(pctx->iovec, 
                  pctx->size - pctx->size_remaining, &frag_size);
  }
  else
  {
    // map from prhysical provider
    frag_data = NULL;
    frag_size = 0;
    ASSERT(0); //not implemented
  }

  ASSERT(frag_data != NULL && frag_size <= pctx->size_remaining);

  if (frag_size > ctx_ptr->mtu_size)
  {
    frag_size = ctx_ptr->mtu_size;
  }

  glink_os_cs_acquire(ctx_ptr->tx_link_lock);

  do
  {
    uint32 read_ind, write_ind, avail_size;

    if (ctx_ptr->resume_pending)
    {
      /* Waiting for resume already, there is no space to transmit */
      /* Read notifiction request should be in the FIFO already */
      break;
    }

    if (smem_list_count(&ctx_ptr->tx_list) != 0)
    {
      /* There are commands waiting transmission */
      /* Read notifiction request should be in the FIFO already */
      ctx_ptr->resume_pending = TRUE;
      break;
    }

    read_ind = ctx_ptr->tx_desc->read_ind;
    write_ind = ctx_ptr->local_ind.write_ind;
    avail_size = read_ind > write_ind ? read_ind - write_ind :
                 ctx_ptr->tx_fifo_size - write_ind + read_ind;

    /* Chack if the command and at least some data will fit in there */
    if (avail_size > sizeof(cmd) + 2*sizeof(uint64))
    {
      avail_size -= sizeof(cmd) + 2*sizeof(uint64);

      if (avail_size < frag_size)
      {
        frag_size = avail_size;
      }

      XPORT_SMEM_LOG_MSG(ctx_ptr->pcfg->remote_host, "Tx write ind", 
                         ctx_ptr->pcfg->remote_ss, write_ind);
      XPORT_SMEM_LOG_MSG(ctx_ptr->pcfg->remote_host, "Tx frag size", 
                         ctx_ptr->pcfg->remote_ss, frag_size);

      if( !pctx->tracer_pkt )
      {
        cmd[0].tx_cmd.id = pctx->size == pctx->size_remaining ? 
                           XPORT_SMEM_CMD_TX_DATA : XPORT_SMEM_CMD_TX_DATA_CONT;
      }
      else
      {
        cmd[0].tx_cmd.id = pctx->size == pctx->size_remaining ? 
                           XPORT_SMEM_CMD_TRACER_PACKET : 
                           XPORT_SMEM_CMD_TRACER_PACKET_CONT;
      }
      
      cmd[0].tx_cmd.cid = (uint16)lcid;
      cmd[0].tx_cmd.iid = pctx->iid;
      cmd[1].data_desc.fragment_size = (uint32)frag_size;
      cmd[1].data_desc.remaining_size = (uint32)(pctx->size_remaining - frag_size);

      if (!xport_smem_write_algn64(ctx_ptr, (char*)&cmd[0], sizeof(cmd)))
      {
        break;
      }
      if (!xport_smem_write_algn64(ctx_ptr, frag_data, (uint32)frag_size))
      {
        /* Restore the write index in case of fail to write frag_data */
        ctx_ptr->local_ind.write_ind = write_ind;
        break;
      }

      xport_smem_send_event(ctx_ptr);
      pctx->size_remaining -= frag_size;

      break;
    }

    ctx_ptr->resume_pending = TRUE;

    cmd[0].gen_cmd.id = XPORT_SMEM_CMD_READ_REQ;

    if (xport_smem_write64(ctx_ptr, 0, cmd[0].raw64))
    {
      xport_smem_send_event(ctx_ptr);
    }

  } while (0);

  glink_os_cs_release(ctx_ptr->tx_link_lock);

  return GLINK_STATUS_SUCCESS;

}

/*===========================================================================
FUNCTION      xport_smem_tx_cmd_set_sigs
===========================================================================*/
/**

  Sends the local channel signals as per the specified 32-bit mask.

  @param[in]  if_ptr    Pointer to transport interface instance.
  @param[in]  lcid      Local channel ID.
  @param[in]  sigs      Signal mask. 

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_tx_cmd_set_sigs
(
  glink_transport_if_type *if_ptr,
  uint32 lcid,
  uint32 sigs
)
{
  xport_smem_ctx_type *ctx_ptr = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_type cmd;

  cmd.sigs_cmd.id = XPORT_SMEM_CMD_TX_SIGNALS;
  cmd.sigs_cmd.cid = (uint16)lcid;
  cmd.sigs_cmd.sigs = sigs;

  return xport_smem_send_cmd(ctx_ptr, cmd.raw64, 0, NULL);
}

/*===========================================================================
FUNCTION      xport_smem_allocate_rx_intent
===========================================================================*/
/**

  Allocates a receive buffer for the local rx intent.

  @param[in]  if_ptr        Pointer to transport interface.
  @param[in]  size          Size of Rx intent.
  @param[in]  intent_ptr    intent pointer to be filled with allocated vector.

  @return     Returns error code.

  @sideeffects  None.

  @dependencies None.
*/
/*=========================================================================*/
glink_err_type xport_smem_allocate_rx_intent(
  glink_transport_if_type *if_ptr,    
  size_t                  size,       
  glink_rx_intent_type    *intent_ptr
)
{
  glink_iovector_type* iovec;

  iovec = (glink_iovector_type*)glink_os_malloc(sizeof(glink_iovector_type) + 
                                  sizeof(glink_iovector_element_type) + size);

  if (iovec == NULL)
  {
    return GLINK_STATUS_OUT_OF_RESOURCES;
  }

  iovec->vlist = (glink_iovector_element_type*)(iovec + 1);
  iovec->vlist->next = NULL;
  iovec->vlast = NULL;
  iovec->vlist->data = (void*)(iovec->vlist + 1);
  iovec->vlist->start_offset = 0;
  iovec->vlist->size = size;
  iovec->plist = NULL;
  iovec->plast = NULL;

  intent_ptr->iovec = iovec;
  intent_ptr->vprovider = &glink_iovec_vprovider;
  intent_ptr->pprovider = &glink_iovec_pprovider;

  return GLINK_STATUS_SUCCESS;
}

/*===========================================================================
FUNCTION      xport_smem_deallocate_rx_intent
===========================================================================*/
/**

  Deallocates a receive buffer for the local rx intent.

  @param[in]  if_ptr        Pointer to transport interface.
  @param[in]  intent_ptr    intent pointer with allocated vector to be freed.

  @return     Returns error code.

  @sideeffects  None.

  @dependencies None.
*/
/*=========================================================================*/
glink_err_type xport_smem_deallocate_rx_intent(
  glink_transport_if_type *if_ptr,    
  glink_rx_intent_type    *intent_ptr
)
{
  glink_os_free(intent_ptr->iovec);

  return GLINK_STATUS_SUCCESS;
}

/*===========================================================================
FUNCTION      xport_smem_ssr
===========================================================================*/
/**

  Deallocates a receive buffer for the local rx intent.

  @param[in]  if_ptr        Pointer to transport interface.

  @return     Returns error code.

  @sideeffects  None.

  @dependencies None.
*/
/*=========================================================================*/
glink_err_type xport_smem_ssr
(
  glink_transport_if_type *if_ptr
)
{
  xport_smem_ctx_type      *ctx = (xport_smem_ctx_type *)if_ptr;
  xport_smem_cmd_item_type *item;
  
  glink_os_cs_acquire( ctx->tx_list_lock );
  glink_os_cs_acquire( ctx->tx_free_list_lock );
  
  item = smem_list_first( &ctx->tx_list );
  while( item )
  {
    smem_list_delete( &ctx->tx_list, item );
    if( item->pattachment )
    {
      glink_os_free( item->pattachment );
    }
    
    smem_list_append( &ctx->tx_free_list, item );
    item = smem_list_first( &ctx->tx_list );
  }

  glink_os_cs_release( ctx->tx_free_list_lock );
  glink_os_cs_release( ctx->tx_list_lock );
  
  glink_os_cs_acquire(ctx->rx_link_lock);

  ctx->resume_pending = FALSE;

  ctx->rx_desc->read_ind = 0;
  ctx->tx_desc->write_ind = 0;
  ctx->local_ind.read_ind = 0;
  ctx->local_ind.write_ind = 0;
  ctx->rx_fifo_size = 0; // to indicate link is down

  glink_os_cs_release(ctx->rx_link_lock);

  return GLINK_STATUS_SUCCESS;
}
/*********************** End of Base Feature Functions ***********************/

/*===========================================================================
FUNCTION      xport_smem_set_version
===========================================================================*/
/**

  Signals that negotiation is complete and the transport can now 
  do version-specific initialization.

  @param[in]  if_ptr    Pointer to transport interface instance.
  @param[in]  version   Version to be used.
  @param[in]  feature   Features to be used.

  @return     None.

  @sideeffects  None.
*/
/*=========================================================================*/
uint32 xport_smem_set_version
(
  glink_transport_if_type *if_ptr,
  uint32 version,
  uint32 features
)
{
  xport_smem_ctx_type *ctx = (xport_smem_ctx_type *)if_ptr;
  uint32 capabilities      = GLINK_CAPABILITY_SIG_SUPPORT;
  
  ctx->negotiated_version.version  = version;
  ctx->negotiated_version.features = features;

  /* Adjust functions properly per feature */
  if( features & XPORT_CH_MIGRATION_FEATURE )
  {
    if_ptr->tx_cmd_ch_open = &xport_smem_tx_cmd_ch_open_ch_migration_feature;
    
    if_ptr->tx_cmd_ch_remote_open_ack 
      = &xport_smem_tx_cmd_ch_remote_open_ack_ch_migration_feature;
  }
  
  return capabilities;
}

/*===========================================================================
FUNCTION      xport_smem_negotiate_features
===========================================================================*/
/**

  Callback to verify feature set.

  @param[in]  if_ptr        Pointer to transport interface.
  @param[in]  version_ptr   Pointer to version descriptor.
  @param[in]  features      Proposed feature set.

  @return     negotiated feature.

  @sideeffects  None.

  @dependencies None.
*/
/*=========================================================================*/
uint32 xport_smem_negotiate_features
(
  glink_transport_if_type *if_ptr,
  const glink_core_version_type *version_ptr,
  uint32 features
)
{
  return version_ptr->features & features;
}

/*===========================================================================
FUNCTION      xport_smem_check_link
===========================================================================*/
/**

  Checks if SMEM transport on remote host is ready.

  @param[in]  ctx_ptr   Pointer to transport context.

  @return     None.

  @dependencies None.
*/
/*=========================================================================*/
void xport_smem_check_link(xport_smem_ctx_type *ctx_ptr)
{
  glink_os_cs_acquire(ctx_ptr->rx_link_lock);

  if (ctx_ptr->rx_fifo_size == 0)
  {
    smem_alloc_params_type smem_params;

    smem_params.remote_host = ctx_ptr->pcfg->remote_host;
    smem_params.smem_type = 
      xport_smem_this_host < ctx_ptr->pcfg->remote_host ?
        SMEM_GLINK_NATIVE_XPORT_FIFO_1 : SMEM_GLINK_NATIVE_XPORT_FIFO_0;
    smem_params.size = 0;
    smem_params.buffer = NULL;
    smem_params.flags       = SMEM_ALLOC_FLAG_PARTITION_ONLY;

    xport_smem_smem_get_addr_ex(&smem_params);

    if (smem_params.buffer != 0 && smem_params.size != 0 && ctx_ptr->tx_desc->read_ind == 0)
    {
      ctx_ptr->rx_fifo = smem_params.buffer;
      ctx_ptr->rx_fifo_size = smem_params.size;

      ctx_ptr->xport_if.glink_core_if_ptr->link_up((glink_transport_if_type *)ctx_ptr);
    }
  }

  glink_os_cs_release(ctx_ptr->rx_link_lock);
}

/*===========================================================================
FUNCTION      xport_smem_isr
===========================================================================*/
/**

  ISR of SMEM transport.

  @param[in]  ctx_ptr   Pointer to transport context.

  @return     None.

  @sideeffects  None.

  @dependencies None.
*/
/*=========================================================================*/
glink_err_type xport_smem_isr(xport_smem_ctx_type *ctx_ptr)
{
  xport_smem_ind_type rx_ind;
  uint32 tmpu32;
  char* tmpptr;
  boolean resume_pending = FALSE;
  boolean invalid_cmd = FALSE;

  /* If edge specifies an explicit clear interrupt (is level triggered), 
   * invoke it */
  if ( ctx_ptr->pcfg->clear_interrupt)
  {
    ctx_ptr->pcfg->clear_interrupt();
  }
  
  glink_os_cs_acquire(ctx_ptr->rx_link_lock);

  if (ctx_ptr->rx_fifo_size == 0)
  {
    /* Link is not ready yet, check the state of the remote host */
    xport_smem_check_link(ctx_ptr);

    if (ctx_ptr->rx_fifo_size == 0)
    {
      glink_os_cs_release(ctx_ptr->rx_link_lock);
      return GLINK_STATUS_FAILURE;
    }
  }

  /* Process pending commands and data */
  rx_ind.write_ind = ctx_ptr->rx_desc->write_ind;
  rx_ind.read_ind = ctx_ptr->local_ind.read_ind;

  if ((!glink_os_fifo_index_check(ctx_ptr->rx_fifo_size, rx_ind.write_ind, 8)) ||
      (!glink_os_fifo_index_check(ctx_ptr->rx_fifo_size, rx_ind.read_ind, 0)))
  {
    glink_os_cs_release(ctx_ptr->rx_link_lock);
    OS_LOG_ERROR(4, "Invalid RX indices write_ind=0x%X, read_ind=0x%X, ctx_ptr=0x%p",
                     rx_ind.write_ind, rx_ind.read_ind, ctx_ptr);
    return GLINK_STATUS_FAILURE;
  }

  XPORT_SMEM_LOG_MSG(ctx_ptr->pcfg->remote_host, "SMEM ISR write ind", 
                     ctx_ptr->pcfg->remote_ss, rx_ind.write_ind);
  XPORT_SMEM_LOG_MSG(ctx_ptr->pcfg->remote_host, "SMEM ISR read ind", 
                     ctx_ptr->pcfg->remote_ss, rx_ind.read_ind);

  /* Do cache operations */
  if (rx_ind.write_ind != rx_ind.read_ind)
  {
    if (rx_ind.write_ind > rx_ind.read_ind)
    {
      glink_os_cache_invalidate( &ctx_ptr->rx_fifo[ rx_ind.read_ind ], rx_ind.write_ind - rx_ind.read_ind );
      glink_os_cache_prefetch( &ctx_ptr->rx_fifo[ rx_ind.read_ind ], rx_ind.write_ind - rx_ind.read_ind );
    }
    else
    {
      glink_os_cache_invalidate( &ctx_ptr->rx_fifo[ rx_ind.read_ind ], ctx_ptr->rx_fifo_size - rx_ind.read_ind );
      glink_os_cache_prefetch( &ctx_ptr->rx_fifo[ rx_ind.read_ind ], ctx_ptr->rx_fifo_size - rx_ind.read_ind );
      
      glink_os_cache_invalidate( &ctx_ptr->rx_fifo[ 0 ], rx_ind.write_ind );
      glink_os_cache_prefetch( &ctx_ptr->rx_fifo[ 0 ], rx_ind.write_ind );
    }
  }

  while ((rx_ind.write_ind != rx_ind.read_ind) && (invalid_cmd == FALSE))
  {
    xport_smem_cmd_type cmd;
    uint32 read_ind_temp = rx_ind.read_ind;

    cmd.raw64 = xport_smem_read64(ctx_ptr, &rx_ind);

    XPORT_SMEM_LOG_MSG(ctx_ptr->pcfg->remote_host, "Cmd Rx", 
                       ctx_ptr->pcfg->remote_ss, (uint32)cmd.raw64);

    switch (cmd.gen_cmd.id)
    {
      case XPORT_SMEM_CMD_TX_DATA:
        xport_smem_process_rx_packet_cmd( ctx_ptr, 
                                          &cmd.tx_cmd, 
                                          &rx_ind,
                                          XPORT_SMEM_DISABLE_TRACER_PACKET );
        break;

      case XPORT_SMEM_CMD_TX_DATA_CONT:
        xport_smem_process_rx_packet_cont_cmd( ctx_ptr, 
                                               &cmd.tx_cmd, 
                                               &rx_ind,
                                               XPORT_SMEM_DISABLE_TRACER_PACKET );
        break;

      case XPORT_SMEM_CMD_TRACER_PACKET:
#ifdef GLINK_MEMCPY_PROFILE_ENABLED
        tracer_packet_cache_rx_timetick( GLINK_RX_INTERRUPT );
#endif
        xport_smem_process_rx_packet_cmd( ctx_ptr, 
                                          &cmd.tx_cmd, 
                                          &rx_ind,
                                          XPORT_SMEM_ENABLE_TRACER_PACKET );
        break;
        
      case XPORT_SMEM_CMD_TRACER_PACKET_CONT:
#ifdef GLINK_MEMCPY_PROFILE_ENABLED
        tracer_packet_cache_rx_timetick( GLINK_RX_INTERRUPT );
#endif
        xport_smem_process_rx_packet_cont_cmd( ctx_ptr, 
                                               &cmd.tx_cmd, 
                                               &rx_ind,
                                               XPORT_SMEM_ENABLE_TRACER_PACKET );
        break;

      case XPORT_SMEM_CMD_VERSION_REQ:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_version(
          (glink_transport_if_type *)ctx_ptr,
          cmd.ver_cmd.version, cmd.ver_cmd.features);
        break;

      case XPORT_SMEM_CMD_VERSION_ACK:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_version_ack(
          (glink_transport_if_type *)ctx_ptr,
          cmd.ver_cmd.version, cmd.ver_cmd.features);
        break;

      case XPORT_SMEM_CMD_OPEN_CHANNEL:
      {
        uint32 rx_size = xport_smem_get_rx_size(ctx_ptr, &rx_ind);
        if ((cmd.ch_cmd.name_len > rx_size) || 
            (cmd.ch_cmd.name_len > GLINK_CH_NAME_LEN))
        {
          rx_ind.read_ind = read_ind_temp;
          XPORT_SMEM_LOG_ERROR("Invalid channel name length.",
                               ctx_ptr->pcfg->remote_ss, cmd.ch_cmd.name_len);
          invalid_cmd = TRUE;
          break;
        }
        tmpptr = glink_os_malloc(cmd.ch_cmd.name_len);
        ASSERT(tmpptr != NULL);

        xport_smem_read_algn64(ctx_ptr, &rx_ind, tmpptr, cmd.ch_cmd.name_len);
        tmpptr[cmd.ch_cmd.name_len - 1] = 0;

        if( !( ctx_ptr->negotiated_version.features & XPORT_CH_MIGRATION_FEATURE ) )
        {
          cmd.ch_cmd.prio = GLINK_XPORT_SMEM;
        }
        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_ch_remote_open(
          (glink_transport_if_type *)ctx_ptr, cmd.ch_cmd.cid, tmpptr, (glink_xport_priority)cmd.ch_cmd.prio);

        glink_os_free(tmpptr);
        break;
      }

      case XPORT_SMEM_CMD_CLOSE_CHANNEL:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_ch_remote_close(
          (glink_transport_if_type *)ctx_ptr, cmd.ch_cmd.cid);
        break;

      case XPORT_SMEM_CMD_OPEN_CHANNEL_ACK:

        if( !( ctx_ptr->negotiated_version.features & XPORT_CH_MIGRATION_FEATURE ) )
        {
          cmd.ch_cmd.prio = GLINK_XPORT_SMEM;
        }
        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_ch_open_ack(
          (glink_transport_if_type *)ctx_ptr, cmd.ch_cmd.cid, (glink_xport_priority)cmd.ch_cmd.prio);
        break;

      case XPORT_SMEM_CMD_CLOSE_CHANNEL_ACK:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_ch_close_ack(
          (glink_transport_if_type *)ctx_ptr, cmd.ch_cmd.cid);
        break;

      case XPORT_SMEM_CMD_RX_INTENT:

        tmpu32 = cmd.rx_int_cmd.intent_num;

        for (; tmpu32 != 0; tmpu32--)
        {
          xport_smem_cmd_type cmd_arg;

          cmd_arg.raw64 = xport_smem_read64(ctx_ptr, &rx_ind);

          ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_remote_rx_intent_put(
            (glink_transport_if_type *)ctx_ptr, cmd.rx_int_cmd.cid,
            cmd_arg.int_desc.id, cmd_arg.int_desc.size);
        }
        break;

      case XPORT_SMEM_CMD_RX_DONE:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_tx_done(
          (glink_transport_if_type *)ctx_ptr,
          cmd.rx_done_cmd.cid, cmd.rx_done_cmd.iid, FALSE);
        break;

      case XPORT_SMEM_CMD_INTENT_REQ:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_remote_rx_intent_req(
          (glink_transport_if_type *)ctx_ptr, 
          cmd.int_req_cmd.cid, cmd.int_req_cmd.size);
        break;

      case XPORT_SMEM_CMD_INTENT_ACK:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_rx_intent_req_ack(
          (glink_transport_if_type *)ctx_ptr, 
          cmd.int_ack_cmd.cid, (boolean)cmd.int_ack_cmd.ack);
        break;

      case XPORT_SMEM_CMD_READ_REQ:

        /* Update the local and shared read indices */ 
        ctx_ptr->local_ind.read_ind = rx_ind.read_ind;
        ctx_ptr->rx_desc->read_ind = rx_ind.read_ind;
       
        glink_os_send_interrupt((os_ipc_intr_type *)&ctx_ptr->pcfg->irq_out);
        break;

      case XPORT_SMEM_CMD_RX_DONE_REUSE:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_tx_done(
          (glink_transport_if_type *)ctx_ptr,
          cmd.rx_done_cmd.cid, cmd.rx_done_cmd.iid, TRUE);
        break;

      case XPORT_SMEM_CMD_TX_SIGNALS:

        ctx_ptr->xport_if.glink_core_if_ptr->rx_cmd_remote_sigs(
          (glink_transport_if_type *)ctx_ptr,
          cmd.sigs_cmd.cid, cmd.sigs_cmd.sigs);
        break;

      default:
        break;
    }
  }

  /* Update the local and shared read indices */ 
  ctx_ptr->local_ind.read_ind = rx_ind.write_ind;
  ctx_ptr->rx_desc->read_ind = rx_ind.write_ind;

  glink_os_cs_release(ctx_ptr->rx_link_lock);

  glink_os_cs_acquire(ctx_ptr->tx_link_lock);

  if (smem_list_count(&ctx_ptr->tx_list) != 0)
  {
    xport_smem_cmd_item_type* pdesc;

    for (pdesc = smem_list_first(&ctx_ptr->tx_list);
         pdesc != NULL;
         pdesc = smem_list_first(&ctx_ptr->tx_list))
    {
      uint32 reserve_size = glink_os_roundup64(pdesc->attachment_size) + sizeof(uint64);
      uint32 tx_write_ind_temp = ctx_ptr->local_ind.write_ind;

      if (!xport_smem_write64(ctx_ptr, reserve_size, pdesc->cmd.raw64))
      {
        break;
      }

      if (pdesc->pattachment != NULL)
      {
        if (!xport_smem_write_algn64(ctx_ptr, pdesc->pattachment, 
                                     pdesc->attachment_size))
        {
          /* Restore the write index in case of fail to write attachment */
          ctx_ptr->local_ind.write_ind = tx_write_ind_temp;
          break;
        }
        glink_os_free(pdesc->pattachment);     
      }

      smem_list_delete(&ctx_ptr->tx_list, pdesc);

      glink_os_cs_acquire(ctx_ptr->tx_free_list_lock);
      smem_list_append(&ctx_ptr->tx_free_list, pdesc);
      glink_os_cs_release(ctx_ptr->tx_free_list_lock);
    }

    xport_smem_send_event(ctx_ptr);
  }

  if (ctx_ptr->resume_pending)
  {
    ctx_ptr->resume_pending = FALSE;
    resume_pending = TRUE;
  }

  glink_os_cs_release(ctx_ptr->tx_link_lock);

  if (resume_pending)
  {
    ctx_ptr->xport_if.glink_core_if_ptr->tx_resume(
      (glink_transport_if_type *)ctx_ptr);
  }

  return GLINK_STATUS_SUCCESS;
}
	
#if defined(FEATURE_XPORT_SMEM_POLL_TIMER)
/*===========================================================================
FUNCTION      xport_smem_isr_timer_cb
===========================================================================*/
/**

  poll FIFO for all supported remote proc

  @param[in]  none.

  @return     none

  @sideeffects  None.
*/
/*=========================================================================*/

static
void
EFIAPI
xport_smem_isr_timer_cb(EFI_EVENT event, void* context)
{
	int ind;
	for (ind = 0; ind < xport_smem_config_num; ind++)
		xport_smem_isr(&xport_smem_ctx[ind]);
	return;
}

/*===========================================================================
FUNCTION      xport_smem_set_handler
===========================================================================*/
/**

  set timer callback to trigger xport_smem_isr() for all remote proc that 
  we want to create glink

  @param[in]  none.

  @return     TURE for success, FALSE for fail.

  @sideeffects  None.
*/
/*=========================================================================*/

boolean xport_smem_set_handler()
{
	EFI_STATUS status;
	EFI_EVENT GlinkTimerEvent;
	
	status = gBS->CreateEvent (
                    EVT_TIMER | EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    xport_smem_isr_timer_cb,
                    NULL,
                    &GlinkTimerEvent
                    );
    if(EFI_ERROR (status)) {
      return FALSE;
    }
	
	gBS->SignalEvent(GlinkTimerEvent);
	status = gBS->SetTimer(GlinkTimerEvent, TimerPeriodic, 200*10000U);  // peroid = 200ms
	if (EFI_ERROR(status)) {
		return FALSE;
	}
	
	return TRUE;
}
#endif /* FEATURE_XPORT_SMEM_POLL_TIMER */

/*===========================================================================
FUNCTION      xport_smem_init
===========================================================================*/
/**

  Initializes SMEM transport.
  Must be called before any other operations are done.

  @param[in]  arg   The argument.

  @return     Returns error code.

  @sideeffects  None.
*/
/*=========================================================================*/
glink_err_type xport_smem_init(void *arg)
{
  uint32 ind;
  glink_core_transport_cfg_type xport_smem_cfg = { 0 };
  EFI_STATUS status;
  EFI_GUID smem_guid = EFI_SMEM_PROTOCOL_GUID;

  if (xport_smem_config_num == 0)
  {
    return GLINK_STATUS_SUCCESS;
  }
  /* Get the SMEM protocol interface */
  status = gBS->LocateProtocol(&smem_guid, NULL,
                               (void**)&xport_smem_smem_protocol_iface);
  if(status != EFI_SUCCESS)
  {
    return GLINK_STATUS_NOT_INIT;
  }

  xport_smem_ctx = glink_os_calloc(sizeof(*xport_smem_ctx)*xport_smem_config_num);

  ASSERT(xport_smem_ctx != NULL);

  /* Initialize supported version and features */
  xport_smem_version = glink_os_calloc( sizeof( glink_core_version_type ) );
  
  ASSERT( xport_smem_version );

  xport_smem_version->version  = XPORT_BASE_VERSION;
  xport_smem_version->features = XPORT_BASE_FEATURE | 
                                 XPORT_CH_MIGRATION_FEATURE;
  xport_smem_version->negotiate_features = &xport_smem_negotiate_features;
  
  /* Initialize edges */
  for (ind = 0; ind < xport_smem_config_num; ind++)
  {
    xport_smem_ctx[ind].pcfg = xport_smem_get_config(ind);

    /* If edge specifies a unique FIFO allocation routine, use it
     * instead of the default */
    if(xport_smem_ctx[ind].pcfg->alloc_fifo)
    {
      if ( !xport_smem_ctx[ind].pcfg->alloc_fifo( &xport_smem_ctx[ind] ) )
      {
        continue;
      }
    }
    else
    {
      if ( !xport_smem_alloc_common_proc_fifo( &xport_smem_ctx[ind] ) )
      {
        continue;
      }
    }
    
    xport_smem_ctx[ind].mtu_size = xport_smem_ctx[ind].pcfg->mtu_size;
    
    xport_smem_ctx[ind].rx_desc->read_ind = 0;
    xport_smem_ctx[ind].tx_desc->write_ind = 0;

    /* Initialize context */
    xport_smem_ctx[ind].tx_link_lock = glink_os_cs_create();
    xport_smem_ctx[ind].rx_link_lock = glink_os_cs_create();
    xport_smem_ctx[ind].tx_list_lock = glink_os_cs_create();
    xport_smem_ctx[ind].tx_free_list_lock = glink_os_cs_create();
    smem_list_init(&xport_smem_ctx[ind].tx_list);
    smem_list_init(&xport_smem_ctx[ind].tx_free_list);

    /* Initialize GLink transport interface */
    xport_smem_ctx[ind].xport_if.tx_cmd_version = &xport_smem_tx_cmd_version;
    xport_smem_ctx[ind].xport_if.tx_cmd_version_ack = &xport_smem_tx_cmd_version_ack;
    xport_smem_ctx[ind].xport_if.set_version = &xport_smem_set_version;
    xport_smem_ctx[ind].xport_if.tx_cmd_ch_open = &xport_smem_tx_cmd_ch_open;
    xport_smem_ctx[ind].xport_if.tx_cmd_ch_close = &xport_smem_tx_cmd_ch_close;
    xport_smem_ctx[ind].xport_if.tx_cmd_ch_remote_open_ack = &xport_smem_tx_cmd_ch_remote_open_ack;
    xport_smem_ctx[ind].xport_if.tx_cmd_ch_remote_close_ack = &xport_smem_tx_cmd_ch_remote_close_ack;
    xport_smem_ctx[ind].xport_if.tx_cmd_local_rx_intent = &xport_smem_tx_cmd_local_rx_intent;
    xport_smem_ctx[ind].xport_if.tx_cmd_local_rx_done = &xport_smem_tx_cmd_local_rx_done;
    xport_smem_ctx[ind].xport_if.tx_cmd_rx_intent_req = &xport_smem_tx_cmd_rx_intent_req;
    xport_smem_ctx[ind].xport_if.tx_cmd_remote_rx_intent_req_ack = &xport_smem_tx_cmd_remote_rx_intent_req_ack;
    xport_smem_ctx[ind].xport_if.tx = &xport_smem_tx;
    xport_smem_ctx[ind].xport_if.allocate_rx_intent = &xport_smem_allocate_rx_intent;
    xport_smem_ctx[ind].xport_if.deallocate_rx_intent = &xport_smem_deallocate_rx_intent;
    xport_smem_ctx[ind].xport_if.tx_cmd_set_sigs = &xport_smem_tx_cmd_set_sigs;
    xport_smem_ctx[ind].xport_if.ssr = &xport_smem_ssr;
    xport_smem_ctx[ind].xport_if.poll = (poll_fn)&xport_smem_isr;
    xport_smem_ctx[ind].xport_if.qos_functbl = xport_smem_ctx[ind].pcfg->qos_api;

    /* TODO: glink transport priority */
    xport_smem_ctx[ind].xport_if.glink_priority = GLINK_XPORT_SMEM;
	
    /* Setup GLink configuration */
    xport_smem_cfg.name = XPORT_SMEM_NAME;
    xport_smem_cfg.remote_ss = xport_smem_ctx[ind].pcfg->remote_ss;
    xport_smem_cfg.version   = xport_smem_version;
    xport_smem_cfg.version_count = 1;
    xport_smem_cfg.max_cid = 0xFFFF;
    xport_smem_cfg.max_iid = 0xFFFFFFFF;

    if (glink_core_register_transport(&xport_smem_ctx[ind].xport_if, &xport_smem_cfg) !=
        GLINK_STATUS_SUCCESS)
    {
      /* Registration failed, set index to invalid. */
      xport_smem_ctx[ind].pcfg = NULL;
      continue;
    }

    //if (!glink_os_register_isr(xport_smem_ctx[ind].pcfg->irq_in, (os_isr_cb_fn)xport_smem_isr, 
    //                       &xport_smem_ctx[ind]))
    //{
      /* ISR registration failed, set index to invalid. */
    //  xport_smem_ctx[ind].pcfg = NULL;
    //  continue;
    //}

    glink_os_send_interrupt((os_ipc_intr_type *)&xport_smem_ctx[ind].pcfg->irq_out);

    xport_smem_check_link(&xport_smem_ctx[ind]);
  }

#if defined(FEATURE_XPORT_SMEM_POLL_TIMER)
  if (!xport_smem_set_handler()) {
	  return GLINK_STATUS_FAILURE;
  }
#endif /* FEATURE_XPORT_SMEM_POLL_TIMER */

  return GLINK_STATUS_SUCCESS;
}
