/**
 * @file  gpi.c
 * @brief Implements the GSI interface 
 */
/*
===============================================================================

                               Edit History

$Header:

when       who     what, where, why
--------   ---     ------------------------------------------------------------ 
04/11/17   ts      Included support to deregister and also to load fw from ELF.
01/31/17   ts      file ported to UEFI.
02/19/16   ah      Created

===============================================================================
              Copyright (c) 2016-2017 QUALCOMM Technologies, Inc.
                         All Rights Reserved.
            Qualcomm Technologies Proprietary and Confidential.
===============================================================================
*/

#include "gpi_utils.h"
#include <string.h>

static gpi_ctxt gpi;


/**
 * Main function to initialize GPI EE, target, and gloabl data structure.
 * To be called from the main task before any GPI request is made.
 * 
 * @param[in]    None.
 *
 * @return       None.
 */
void gpi_init(void)
{
   gpi_log(GPI_DEBUG, "<< GPI ENTRY >>\n");

   if (GPI_STATUS_SUCCESS !=  gpi_load_firmware())
   {
      gpi_log(GPI_ERROR, "GPI firmware load failed!\n");
      return;
   }

   /* Default execution mode */
#ifdef GPI_MT
   gpi.exec_mode = GPI_THREAD;   /* Multi-threaded with interrupts */
#elif GPI_ST
   gpi.exec_mode = GPI_TASK;     /* Single-task, with interrupts, no task preemption */
#else
   gpi.exec_mode = GPI_POLLING;  /* Polling, no tasks, threads, or interrupts (Boot) */
   uint8 i;
   for (i = 0; i < NUM_GPII; i++)
      gpi.gpii[i].irq_mode = FALSE;
#endif

   if (GPI_STATUS_SUCCESS != gpi_ee_init())
   {
      gpi_log(GPI_ERROR, "GPI ee initialization failed!\n");
      return;
   }

   /* Initialize target data */
   if (GPI_STATUS_SUCCESS != gpi_init_tgt_info(&gpi))
   {
      gpi_log(GPI_ERROR, "GPI tgt info initialization failed!\n");
      return;
   }

   if (gpi.exec_mode == GPI_TASK)
   {
      gpi_reg_task(&gpi);
   }

   /* Initialize GPI common data, GSI FW should have absolutely been loaded at this point */
   if (GPI_STATUS_SUCCESS != gpi_common_init(&gpi))
   {
      gpi_log(GPI_ERROR, "GPI common initialization failed!\n");
      return;
   }

   gpi_log(GPI_DEBUG, "GPI initialization done!\n");
   gpi_log(GPI_DEBUG, "gpi_init() ->\n");
}

/**
 * Prints client GPI interface parameters for debugging purposes.
 *
 * @param[in]    params       GPI interface registration parameters
 *
 * @return       None.
 */
void print_gpii_params(gpi_iface_params *params)
{
   gpi_log(GPI_DEBUG, "gpii[%d] protocol: %d\n",
           params->gpii_id, params->protocol);
   gpi_log(GPI_DEBUG, "gpii[%d] OUTBOUND_CHAN tre_ring_size: %d\n",
           params->gpii_id, params->chan[GPI_OUTBOUND_CHAN].tre_ring_size);
   gpi_log(GPI_DEBUG, "gpii[%d] OUTBOUND_CHAN ring_base_pa: 0x%llx\n",
           params->gpii_id, params->chan[GPI_OUTBOUND_CHAN].ring_base_pa);
   gpi_log(GPI_DEBUG, "gpii[%d] OUTBOUND_CHAN ring_base_va: 0x%llx\n",
           params->gpii_id, params->chan[GPI_OUTBOUND_CHAN].ring_base_va);
   gpi_log(GPI_DEBUG, "gpii[%d] INBOUND_CHAN tre_ring_size: %d\n",
           params->gpii_id, params->chan[GPI_INBOUND_CHAN].tre_ring_size);
   gpi_log(GPI_DEBUG, "gpii[%d] INBOUND_CHAN ring_base_pa: 0x%llx\n",
           params->gpii_id, params->chan[GPI_INBOUND_CHAN].ring_base_pa);
   gpi_log(GPI_DEBUG, "gpii[%d] INBOUND_CHAN ring_base_va: 0x%llx\n",
           params->gpii_id, params->chan[GPI_INBOUND_CHAN].ring_base_va);
}

/**
 * Registers a client GPI interface.
 * Hardware resources for this GPII are initialized as well.
 *
 * @param[in]    params       GPI interface registration parameters
 *
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS gpi_iface_reg(gpi_iface_params *params)
{
   uint32 gpii_idx;

   gpi_log(GPI_DEBUG, "gpi_iface_reg() <-\n");

   if (params == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - NULL parameters!\n");
      return GPI_STATUS_ERROR;
   }

   /* GPI must have been initialized */
   if (gpi.initialized == FALSE)
   {
      gpi_log(GPI_ERROR, "ERROR - GSI not ready!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpii_idx = get_gpii_idx(&gpi, params->gpii_id, params->qup_type);
   if (gpii_idx >= NUM_GPII)
   {
      gpi_log(GPI_ERROR, "ERROR - GPII ID: %d does not exist, returning!\n",
              params->gpii_id);
      return GPI_STATUS_ERROR;
   }

   if (gpi.gpii[gpii_idx].registered == TRUE)
   {
      gpi_log(GPI_ERROR, "ERROR - GPII ID: %d already registered, returning!\n",
              params->gpii_id);
      return GPI_STATUS_ERROR;
   }
   
   print_gpii_params(params);

   /* Validate parameters */
   if (gpi.gpii[gpii_idx].protocol != params->protocol ||
       params->chan[GPI_OUTBOUND_CHAN].tre_ring_size <= 0 ||
       params->chan[GPI_OUTBOUND_CHAN].ring_base_pa <= 0 ||
       params->chan[GPI_OUTBOUND_CHAN].ring_base_va <= 0 ||
       params->chan[GPI_INBOUND_CHAN].tre_ring_size <= 0 ||
       params->chan[GPI_INBOUND_CHAN].ring_base_pa <= 0 ||
       params->chan[GPI_INBOUND_CHAN].ring_base_va <= 0 ||
       params->cb == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - wrong GPII params!\n");
      return GPI_STATUS_ERROR;
   }

   if (GPI_STATUS_SUCCESS != gpii_init(&gpi.gpii[gpii_idx]))
   {
      gpi_log(GPI_ERROR, "ERROR - gpii_init() failed\n");
      return GPI_STATUS_ERROR;
   }
   
   gpi.gpii[gpii_idx].initialized = TRUE;
   gpi_log(GPI_DEBUG, "gpii_hw_ready_bmsk: 0x%x\n", gpi.gpii_hw_ready_bmsk);

   /* Set GPII parameters */
   if (GPI_STATUS_SUCCESS != set_gpii_params(&gpi, &gpi.gpii[gpii_idx], params))
   {
      gpi_log(GPI_ERROR, "ERROR - set_gpii_params failed!\n");
      return GPI_STATUS_ERROR;
   }

   gpi.gpii[gpii_idx].registered = TRUE;

   gpi_log(GPI_DEBUG, "gpi_iface_reg() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * De-Registers a client GPI interface.
 *
 * @param[in]    handle            Handle to GPI Interface
 *
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS gpi_iface_dereg(GPI_CLIENT_HANDLE handle)
{
   gpii_info *gpii;

   gpi_log(GPI_DEBUG, "gpi_iface_dereg() <-\n");

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - bad parameters!\n");
      return GPI_STATUS_ERROR;
   }

   gpii = (gpii_info *)handle;

   gpi_syncenter(&gpii->gpii_mutex);
   gpii->cb = NULL;
   gpii->registered = FALSE;
   gpi_syncleave(&gpii->gpii_mutex);

   gpi_log(GPI_DEBUG, "gpi_iface_dereg() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Queries wheteher a GPI interface is running in interrupt or polling mode.
 *
 * @param[in]        handle            Handle to GPI Interface
 * @param[in, out]   irq_mode          1 if Interface in IRQ mode, 0 otherwise
 *
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS gpi_iface_in_irq_mode(GPI_CLIENT_HANDLE handle, uint8 *irq_mode)
{
   gpii_info *gpii = (gpii_info *)handle;

   gpi_log(GPI_DEBUG, "gpi_iface_in_irq_mode() <-\n");

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - bad parameters!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.gpii_hw_ready_bmsk | (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, "ERROR - GSI not ready!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_syncenter(&gpii->gpii_mutex);
   *irq_mode = gpii->irq_mode ? 1 : 0;
   gpi_syncleave(&gpii->gpii_mutex);

   gpi_log(GPI_DEBUG, "irq_mode: %d\n", *irq_mode);

   gpi_log(GPI_DEBUG, "gpi_iface_in_irq_mode() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Enables/Disables a GPI interface IRQ mode.
 *
 * @param[in]    handle               Handle to GPI Interface
 * @param[in]    irq_en               TRUE to enable IRQ mode, FALSE for polling
 * 
 * @return       GPI_RETURN_STATUS    Return status.
 */
GPI_RETURN_STATUS gpi_iface_set_irq_mode(GPI_CLIENT_HANDLE handle, boolean irq_en)
{
   gpii_info *gpii = (gpii_info *)handle;

   gpi_log(GPI_DEBUG, "gpi_iface_set_irq_mode() - irq_en: %d <-\n", irq_en);

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - bad parameters!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.gpii_hw_ready_bmsk | (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, "ERROR - GSI not ready for this EE!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_syncenter(&gpii->gpii_mutex);
   gpii->irq_mode = irq_en;
   gpi_syncleave(&gpii->gpii_mutex);

   gpi_log(GPI_DEBUG, "gpi_iface_set_irq_mode() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Queries the channel current status.
 *
 * @param[in]        handle              Handle to GPI Interface
 * @param[in]        chan                Virtual Channel ID
 * @param[in, out]   chan_status         Information about channel current status
 *
 * @return         GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS gpi_query_chan_status(GPI_CLIENT_HANDLE handle,
                                        GPI_CHAN_TYPE chan,
                                        chan_status_info *chan_status)
{
   gpii_info *gpii = (gpii_info *)handle;

   gpi_log(GPI_DEBUG, "gpi_query_chan_status() <-\n");

   if (handle == NULL || chan_status == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - bad parameters!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpii->qup_type >= GPI_MAX_BLOCKS)
   {
      gpi_log(GPI_ERROR, "ERROR - bad qup_type parameters!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.gpii_hw_ready_bmsk | (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, "ERROR - GSI not ready for this EE!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_syncenter(&gpii->gpii_mutex);
   gpii->chan_info[chan].state = get_chan_state(gpii, (uint8)chan);
   if (!VALID_CHAN_STATE(gpii->chan_info[chan].state))
   {
      gpi_log(GPI_ERROR, "ERROR - Invalid channel state!\n");
   }
   chan_status->chan_state = gpii->chan_info[chan].state;
   chan_status->rp_index = GET_RING_PTR_INDEX(gpii, chan, RING_RP);
   chan_status->wp_index = GET_RING_PTR_INDEX(gpii, chan, RING_WP);
   chan_status->num_avail_tre = nbr_avail_ring_elem(gpii, (uint8)chan,
                                                    gpii->chan_info[chan].base_pa,
                                                    get_ring_ptr(gpii, chan, RING_WP),
                                                    get_ring_ptr(gpii, chan, RING_RP),
                                                    gpii->chan_info[chan].num_elem);
   gpi_syncleave(&gpii->gpii_mutex);

   gpi_log(GPI_DEBUG, "chan_state: %d\n", chan_status->chan_state);
   gpi_log(GPI_DEBUG, "num_avail_tre: %d\n", chan_status->num_avail_tre);
   gpi_log(GPI_DEBUG, "wp_index: %d\n", chan_status->wp_index);
   gpi_log(GPI_DEBUG, "rp_index: %d\n", chan_status->rp_index);

   gpi_log(GPI_DEBUG, "gpi_query_chan_status() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Polls a GPI interface.
 * The corresponding callback will be invoked with any available results.
 *
 * @param[in]    handle               Client handle.
 *
 * @return       GPI_RETURN_STATUS    Return status.
 */
GPI_RETURN_STATUS gpi_iface_poll(GPI_CLIENT_HANDLE handle)
{
   gpii_info *gpii = (gpii_info *)handle;

   gpi_log(GPI_DEBUG, "gpi_iface_poll() <-\n");

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - bad parameters!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.gpii_hw_ready_bmsk | (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, "ERROR - GSI not ready for this EE!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_isr(gpii);

   gpi_log(GPI_DEBUG, "gpi_iface_poll() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Issues a channel command.
 *
 * @param[in]      handle              Handle to GPI Interface
 * @param[in]      chan              Virtual Channel ID
 * @param[in]      cmd_type          Command type
 * @param[in]      user_data           Client context data for this command
 *
 * @return       GPI_RETURN_STATUS   Return status.
 */
GPI_RETURN_STATUS gpi_issue_cmd(GPI_CLIENT_HANDLE handle,
                                GPI_CHAN_TYPE chan,
                                GPI_CHAN_CMD cmd_type,
                                void *user_data)
{
   gpii_info *gpii = (gpii_info *)handle;

   gpi_log(GPI_DEBUG, "gpi_issue_cmd() <-\n");

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - bad parameters!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpii->qup_type >= GPI_MAX_BLOCKS)
   {
      gpi_log(GPI_ERROR, "ERROR - qup_type bad parameters!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.gpii_hw_ready_bmsk | (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, "ERROR - GSI not ready for this EE!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_log(GPI_DEBUG, "gpii ID: %d, cmd_type: %d\n", gpii->id, cmd_type);

   if (user_data == NULL)
   {
      gpi_log(GPI_WARNING, "WARNING - Bad input parameters, cmd_user_data NULL!\n");
   }

   gpii->cmd_user_data = user_data;

   /* Issue the channel command */
   return issue_cmd(gpii, chan, cmd_type);
}

/**
 * Process TREs request.
 *
 * @param[in]    tre_request           Request details
 * 
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS gpi_process_tre(gpi_tre_req *tre_request)
{
   uint8 i, wp_idx, chan;
   gpii_info *gpii;
   ptr_type wp;

   gpi_log(GPI_DEBUG, "gpi_process_tre() <-\n");

   /* Validate parameters */
   if (tre_request == NULL ||
       tre_request->handle == NULL ||
       tre_request->tre_list == NULL ||
       tre_request->num_tre == 0)
   {
      gpi_log(GPI_ERROR, "ERROR - bad parameters!\n");
      return GPI_STATUS_ERROR;
   }

   gpii = (gpii_info *)tre_request->handle;
   chan = tre_request->chan;

   if(gpii == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - bad gpii parameters!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpii->qup_type >= GPI_MAX_BLOCKS)
   {
      gpi_log(GPI_ERROR, "ERROR - bad qup_type parameters!\n");
      return GPI_STATUS_ERROR;
   }

   wp = get_ring_ptr(gpii, chan, RING_WP);
   if (wp != GET_RING_PHYS_ADDR(&gpii->chan_info[chan], (ptr_type)tre_request->tre_list))
   {
      gpi_log(GPI_ERROR, "ERROR - WP out of sync!\n");
      return GPI_STATUS_WP_OUT_OF_SYNC;
   }

   if (gpi.initialized == FALSE || !(gpi.gpii_hw_ready_bmsk | (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, "ERROR - GSI not ready!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_syncenter(&gpii->gpii_mutex);

   /* make sure the chan is in a good state */
   gpii->chan_info[chan].state = get_chan_state(gpii, chan);
   if (gpii->chan_info[chan].state != GPI_CHAN_RUNNING)
   {
      gpi_log(GPI_ERROR, "ERROR - channel %d not in a running state, state: %d!\n",
              tre_request->chan, gpii->chan_info[chan].state);
      return GPI_STATUS_ERROR;
   }

   if (tre_request->user_data == NULL)
   {
      gpi_log(GPI_WARNING, "WARNING - user_data is NULL!\n");
   }

   gpi_log(GPI_DEBUG, "gpii ID: %d, chan: %d\n", gpii->id, chan);

   /* make sure ring has enough space */
   if (tre_request->num_tre > nbr_avail_ring_elem(gpii, (uint8)chan,
                                                  gpii->chan_info[chan].base_pa,
                                                  wp,
                                                  get_ring_ptr(gpii, chan, RING_RP),
                                                  gpii->chan_info[chan].num_elem))
   {
      gpi_log(GPI_ERROR, "ERROR - channel %d does not have enough space for %d new TRE's!\n",
              chan, tre_request->num_tre);
      return GPI_STATUS_ERROR;
   }

   wp_idx = GET_RING_PTR_INDEX(gpii, chan, RING_WP);

   for (i = 0; i < tre_request->num_tre; i++)
   {
      gpii->transf_ctxt[chan][wp_idx] = tre_request->user_data;
      wp_idx = INC_RING_IDX(wp_idx, gpii->chan_info[chan].num_elem);
   }

   /* Ring channel DB */
   if (sizeof(ptr_type) > 4)
   {
      HWIO_EE_n_GSI_CH_k_DOORBELL_1_OUTI2(gpi.qup[gpii->qup_type].gsi_pa, gpii->id, chan, 
                                          (get_system_address(gpii->chan_info[chan].base_pa) +
                                           ((wp_idx * sizeof(gpi_ring_elem)) >> 32)));
   }
   else
   {
      HWIO_EE_n_GSI_CH_k_DOORBELL_1_OUTI2(gpi.qup[gpii->qup_type].gsi_pa, gpii->id, chan, 0);
   }
   HWIO_EE_n_GSI_CH_k_DOORBELL_0_OUTI2(gpi.qup[gpii->qup_type].gsi_pa, gpii->id, chan,
                                       (get_system_address(gpii->chan_info[chan].base_pa) +
                                        ((wp_idx * sizeof(gpi_ring_elem)) & 0xFFFFFFFF)));
   gpi_syncleave(&gpii->gpii_mutex);

   gpi_log(GPI_DEBUG, "gpi_process_tre() ->\n");
   return GPI_STATUS_SUCCESS;
}
