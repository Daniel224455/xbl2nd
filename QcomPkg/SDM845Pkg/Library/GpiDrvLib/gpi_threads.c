/**
  @file  gpi_threads.c
  @brief GPI Thread Handling.

*/
/*
===============================================================================

                               Edit History

$Header:

when       who     what, where, why
--------   ---     ------------------------------------------------------------ 
02/19/16   ah      File created.

===============================================================================
              Copyright (c) 2016 QUALCOMM Technologies, Inc.  
                         All Rights Reserved.
            Qualcomm Technologies Proprietary and Confidential.
===============================================================================
*/

#include "gpi_utils.h"

#ifdef GPI_MT
char *thread_name = "GPI_WORKER";
#endif

/**
 * @brief Handler for MHI command events.
 *
 * @param[in ]        gpi            gpi context
 * @param[in ]        evt            Event to handle
 */
void handle_helper_evt(gpi_ctxt *gpi, gpi_ring_elem *evt)
{
   GPI_IRQ_TYPE helper_action = (GPI_IRQ_TYPE)evt->ctrl;
   gpii_info *gpii;
   GPI_CHAN_TYPE chan = UNKNOWN_CHAN;
   uint8 state;
   uint32 err;
   uint32 val;
   ring_info_type *ring_info;

   gpi_log(GPI_DEBUG, "handle_helper_evts() <-\n");

   gpii = &gpi->gpii[get_gpii_idx(gpi, 
                                  GPI_READ_FIELD(evt->dword_2, LOCAL_EVT_GPII_ID),
                                  (QUP_BLOCK_TYPE)GPI_READ_FIELD(evt->dword_2, LOCAL_EVT_GSI_INST))];

   if (gpii->qup_type >= GPI_MAX_BLOCKS)
   {
      gpi_log(GPI_ERROR, "ERROR - bad qup_type parameters!\n");
      return;
   }

   gpi_syncenter(&gpii->gpii_mutex);
   gpi_memset(&gpii->result, 0, sizeof(gpi_result_type));

   switch (helper_action)
   {
      case GPI_IRQ_INTER_EE_CH_CTRL:
      case GPI_IRQ_CH_CTRL:
      case GPI_IRQ_INTER_EE_EV_CTRL:
      case GPI_IRQ_EV_CTRL:
         if (helper_action == GPI_IRQ_INTER_EE_CH_CTRL || helper_action == GPI_IRQ_CH_CTRL)
         {
            chan = (GPI_CHAN_TYPE)GPI_READ_FIELD(evt->dword_2, LOCAL_EVT_CHID);
            if (chan >= NUM_RINGS_PER_GPII)
            {
               gpi_log(GPI_ERROR, "ERROR - Transfer evt for chan %d!\n", chan);
               return;
            }
	   
            gpii->result.chan = chan;
            ring_info = &gpii->chan_info[chan];
            /* Read the new channel state */
            state = (GPI_CHAN_STATE)GPI_READ_FIELD(evt->dword_2, LOCAL_EVT_CHAN_STATE);

            gpi_log(GPI_DEBUG, "gpii ID: %d, chan: %d, new channel action: "
                               "GPI_IRQ_CH_CTRL, state: %d\n", gpii->id, chan, state);

            if (state == GPI_CHAN_ALLOCATED)
            {
               init_chan_regs(gpii, chan);
            }
         }
         else
         {
            gpii->result.chan = GPI_EVT_RING;
            ring_info = &gpii->evt_ring_info;

            /* Only 1 event ring per GPII, no need to read this register to figure out which event state changed */
            /* bmsk = HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_INMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_EV_CH_BIT_MAP_BMSK); */

            state = (GPI_EVT_RING_STATE)GPI_READ_FIELD(evt->dword_2, LOCAL_EVT_CHAN_STATE);
            gpi_log(GPI_DEBUG, "gpii ID: %d evt ring, new action: GPI_IRQ_EV_CTRL, state: %d\n",
                    gpii->id, state);

            if (state == GPI_CHAN_ALLOCATED)
            {
               init_evt_ring_regs(gpii);
            }
         }

         ring_info->state = (GPI_CHAN_STATE)state;

         if (gpii->chan_info[GPI_INBOUND_CHAN].state == GPI_CHAN_RUNNING &&
             gpii->chan_info[GPI_OUTBOUND_CHAN].state == GPI_CHAN_RUNNING &&
             gpii->evt_ring_info.state == GPI_CHAN_ALLOCATED)
         {
            /* Ring evt ring DB with the WP */
            if (sizeof(ptr_type) > 4)
            {
               HWIO_EE_n_EV_CH_k_DOORBELL_1_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 
                                                  (get_system_address(gpii->evt_ring_info.base_pa) +
                                                  (NUM_GPI_EVT_RING_ELEM - 1) * sizeof(gpi_ring_elem)) >> 32); 
            }
            else
            {
               HWIO_EE_n_EV_CH_k_DOORBELL_1_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 0);
            }
            HWIO_EE_n_EV_CH_k_DOORBELL_0_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 
                                               (get_system_address(gpii->evt_ring_info.base_pa) +
                                                (NUM_GPI_EVT_RING_ELEM - 1) * sizeof(gpi_ring_elem)) & 0xFFFFFFFF);
         }

         if (gpii->pending_cmd != GPI_CHAN_CMD_NONE)
         {
            gpii->result.type = EVT_CMD_COMPLETION;
            gpii->result.user_data = gpii->cmd_user_data;
         }
         else
         {
            /* unexpected state change */
            gpii->result.type = GPI_EVT_OTHER;
         }

         gpii->pending_cmd = GPI_CHAN_CMD_NONE;
         gpii->result.dword_0 = ring_info->state;

         gpii->result.code = EVT_COMPLETION_OTHER;

         /* pass it to the client */
         gpii->cb(&gpii->result);
         gpi_log(GPI_DEBUG, "gpii ID: %d, CTRL, Invoked client callback\n",
                 gpii->id);
         break;

      case GPI_IRQ_GLOB_EE:
      case GPI_IRQ_GENERAL:
         if (helper_action == GPI_IRQ_GLOB_EE)
         {
            err = GPI_READ_FIELD(evt->dword_2, LOCAL_EVT_ERROR);
            gpi_log(GPI_DEBUG, "gpii ID: %d, new action: GPI_IRQ_GLOB_EE, error: %d\n",
                    gpii->id, err);

            gpii->result.dword_0 = GPI_ERROR_GLOBAL;
            gpii->result.dword_1 = err;
         }
         else
         {
            val = GPI_READ_FIELD(evt->dword_2, LOCAL_EVT_INT_TYPE);
            gpi_log(GPI_DEBUG, "gpii ID: %d, new action: GPI_IRQ_GENERAL, LOCAL_EVT_INT_TYPE: %d\n",
                    gpii->id, val);

            /* Find out the interrupt type */
            /* bit-0 "GSI_BREAK_POINT"       - MCS code break-point */
            if (val & HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_BREAK_POINT_BMSK)
            {
               gpi_log(GPI_DEBUG, "gpii ID: %d, GPI_IRQ_GENERAL, GSI_BREAK_POINT - MCS code break-point\n",
                       gpii->id);
               gpii->result.dword_0 = BREAK_POINT;
            }

            /* bit-1 "GSI_BUS_ERROR"         - Bus Error interrupt */
            if (val & HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_BUS_ERROR_BMSK)
            {
               gpi_log(GPI_DEBUG, "gpii ID: %d, GPI_IRQ_GENERAL, GSI_BUS_ERROR - Bus Error interrupt\n",
                       gpii->id);
               gpii->result.dword_0 = BUS_ERROR;
            }

            /* bit-2 "GSI_CMD_FIFO_OVRFLOW"  - Command Queue Overflow Error */
            if (val & HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_CMD_FIFO_OVRFLOW_BMSK)
            {
               gpi_log(GPI_DEBUG, "gpii ID: %d, GPI_IRQ_GENERAL, GSI_CMD_FIFO_OVRFLOW - Command Queue Overflow Error\n",
                       gpii->id);
               gpii->result.dword_0 = CMD_FIFO_OVRFLOW;
            }

            /* bit-3 "GSI_MCS_STACK_OVRFLOW" - Stack Overflow Error */
            if (val & HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_MCS_STACK_OVRFLOW_BMSK)
            {
               gpi_log(GPI_DEBUG, "gpii ID: %d, GPI_IRQ_GENERAL, GSI_MCS_STACK_OVRFLOW - Stack Overflow Error\n",
                       gpii->id);
               gpii->result.dword_0 = MCS_STACK_OVRFLOW;
            }
         }

         gpii->result.type = GPI_EVT_OTHER;

         /* pass it to the client */
         gpii->cb(&gpii->result);
         gpi_log(GPI_DEBUG, "gpii ID: %d GLOB_EE/GENERAL, Invoked client callback\n", gpii->id);
         break;

      default:
         break;
   }

   gpi_syncleave(&gpii->gpii_mutex);
   gpi_log(GPI_DEBUG, "handle_helper_evts() ->\n");
}

/**
 * @brief Handler for GPI transfer completion events.
 *
 * @param[in ]        gpi            gpi context
 * @param[in ]        gpii           gpii context
 * @param[in ]        evt            Event to handle
 */
void handle_comp_evt(gpi_ctxt *gpi, gpii_info *gpii, gpi_ring_elem *evt)
{
   GPI_EVT_TYPE evt_type;
   GPI_EVT_CODE evt_code;
   uint8 chan;
   ptr_type ptr;

   gpi_log(GPI_DEBUG, "handle_comp_evt() <-\n");

   chan = GPI_READ_FIELD(evt->ctrl, EVT_CHID);
   if (chan >= NUM_RINGS_PER_GPII)
   {
      gpi_log(GPI_ERROR, "ERROR - Transfer evt for chan %d!\n", chan);
      return;
   }

   evt_type = (GPI_EVT_TYPE)GPI_READ_FIELD(evt->ctrl, EVT_TYPE);
   gpi_log(GPI_DEBUG, "evt_type: %d\n", evt_type);

   evt_code = (GPI_EVT_CODE)GPI_READ_FIELD(evt->dword_2, EVT_CODE);
   gpi_log(GPI_DEBUG, "evt_code: %d\n", evt_code);

   gpi_memset(&gpii->result, 0, sizeof(gpi_result_type));
   gpii->result.chan = (GPI_CHAN_TYPE)chan;
   gpii->result.type = evt_type;
   gpii->result.code = evt_code;

   switch (evt_type)
   {
      case EVT_XFER_COMPLETION:
         gpii->result.status = GPI_READ_FIELD(evt->ctrl, EVT_STATUS);
         ptr = evt->dword_0;
         if (sizeof(ptr_type) > 4)
         {
            ptr |= (uint64)evt->dword_1 << 32;
         }
         ptr = get_local_address(ptr);

         gpii->result.user_data = gpii->transf_ctxt[chan][GET_RING_ELEMENT_INDEX(gpii->chan_info[chan].base_pa, ptr)];
         gpii->result.length = GPI_READ_FIELD(evt->dword_2, EVT_XFER_LEN);
         gpi_log(GPI_DEBUG, "Transf_length: %d\n", gpii->result.length);

         /* Callback should be very fast, client should just make a copy of the result and return */
         gpii->cb(&gpii->result);
         gpi_log(GPI_DEBUG, "chan: %d, Invoked client callback\n", chan);
         break;

      case EVT_IMM_DATA:
      case EVT_QUP_NOTIF:
         if (evt_type == EVT_IMM_DATA)
         {
            gpii->result.status = GPI_READ_FIELD(evt->ctrl, EVT_STATUS);
            gpii->result.length = GPI_READ_FIELD(evt->dword_2, EVT_IMM_DATA_LEN);
            gpii->result.tre_idx = GPI_READ_FIELD(evt->dword_2, EVT_IMM_DATA_TRE_IDX);
         }
         else
         {
            gpii->result.length = GPI_READ_FIELD(evt->dword_2, EVT_QUP_COUNT);
         }

         gpii->result.dword_0 = evt->dword_0;
         gpii->result.dword_1 = evt->dword_1;
         gpii->result.tre_idx = gpii->id; 

         gpii->cb(&gpii->result);
         gpi_log(GPI_DEBUG, "chan: %d, Invoked client callback\n", chan);
         break;

      default:
         /* EVT_COMPLETION_STATUS_INVALID / EVT_COMPLETION_STATUS_UNDEFINED / EVT_COMPLETION_STATUS_MALFORMED */
         gpi_log(GPI_ERROR, "ERROR - default completion type: %d, code: %d, for chan: %d\n",
                 (uint32)evt_type, (uint32)evt_code, chan);
         /* TBD: error handling */
         break;
   }

   gpi_log(GPI_DEBUG, "handle_comp_evt() ->\n");
}


/**
 * @brief Checks event rings for new events
 *
 * @param[in ]        gpi           GPI context
 * @param[in ]        gpii          GPI instance
 * 
 * @return            0 if no events are found, 1 otherwise.
 */
uint32 handle_ring_evts(gpi_ctxt *gpi, gpii_info *gpii)
{
   gpi_ring_elem *evt;
   ptr_type rp;

   gpi_log(GPI_DEBUG, "handle_ring_evts() <-\n");

   gpi_syncenter(&gpii->gpii_mutex);
   rp = get_ring_ptr(gpii, 0, EVT_RING_RP);

   gpi_log(GPI_DEBUG, "gpii ID: %d, local evt ring rp: 0x%llx, evt ring rp: 0x%llx\n",
           gpii->id, gpii->evt_ring_info.local_rp, rp);

   if (gpii->evt_ring_info.local_rp == rp)
   {
      gpi_log(GPI_INFO, "Event Ring %d is empty\n", gpii->id);
      gpi_syncleave(&gpii->gpii_mutex);
      return 0;
   }

   while (gpii->evt_ring_info.local_rp != rp)
   {
      evt = (gpi_ring_elem *)GET_RING_VIRT_ADDR(&gpii->evt_ring_info, gpii->evt_ring_info.local_rp);

      gpi_log(GPI_DEBUG, "evt->ptr: 0x%llx, evt->dword_2: 0x%x, evt->ctrl: 0x%x\n",
              (uint64)(((uint64)evt->dword_1 << 32) | (uint64)evt->dword_0), evt->dword_2, evt->ctrl);

      handle_comp_evt(gpi, gpii, evt);

      gpi_cachecmd(GPI_CACHE_INVALIDATE, (ptr_type)evt, sizeof(gpi_ring_elem));

      /* Consumed one evt, recycle one */
      if (sizeof(ptr_type) > 4)
      {
         HWIO_EE_n_EV_CH_k_DOORBELL_1_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 
                                            (uint32)(get_system_address(gpii->evt_ring_info.local_rp) >> 32));
      }
      else
      {
         HWIO_EE_n_EV_CH_k_DOORBELL_1_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 0);
      }
      HWIO_EE_n_EV_CH_k_DOORBELL_0_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0,
                                         (uint32)(get_system_address(gpii->evt_ring_info.local_rp) & 0xFFFFFFFF));

      ADVANCE_RING_PTR(gpii->evt_ring_info.base_pa,
                       &gpii->evt_ring_info.local_rp,
                       gpii->evt_ring_info.num_elem);
   }

   gpi_syncleave(&gpii->gpii_mutex);

   gpi_log(GPI_DEBUG, "handle_ring_evts() ->\n");
   return 1;
}

/**
 * @brief Handles all GPI events
 *
 * @param[in ]        gpi           GPI context
 */
void handle_gpi_evts(gpi_ctxt *gpi)
{
   uint32 i;
   gpi_ring_elem evt;

   gpi_log(GPI_DEBUG, "handle_gpi_evts() <-\n");

   /* Handle transfer completion events */
   while (gpi->gsi_evt_ring_bmsk)
   {
      for (i = 0; i < gpi->num_gpii; i++)
      {
         if (gpi->gsi_evt_ring_bmsk & (1 << i))
         {
            gpi_syncenter(&gpi->gpi_mutex);
            gpi->gsi_evt_ring_bmsk &= ~(1 << i);
            gpi_syncleave(&gpi->gpi_mutex);

            if (0 == handle_ring_evts(gpi, &gpi->gpii[i]))
            {
               gpi_log(GPI_WARNING, "WARNING -  MSI with no events!!\n");
               /* ERROR handling, check for ERRORs... */
            }
         }
      }
   }

   /* Handle non-transfer events if any are queued */
   while (FALSE != de_q(&gpi->gpi_thread.q, &evt))
   {
      gpi_log(GPI_DEBUG, "evt.dword_0: 0x%x, evt.dword_1: 0x%x, evt.dword_2: 0x%x, evt.ctrl: 0x%x\n",
              evt.dword_0, evt.dword_1, evt.dword_2, evt.ctrl);
      handle_helper_evt(gpi, &evt);
   }

   gpi_log(GPI_DEBUG, "handle_gpi_evts() ->\n");
}


#ifdef GPI_MT
/**
 * @brief Terminates a given thread.
 *
 * @param[in ]        Thread            Executing Thread
 */
void terminate_thread(gpi_thread_type *gpi_thread)
{
   gpi_log(GPI_DEBUG, "terminate_thread() <-\n");
   gpi_log(GPI_DEBUG, "GPI Thread Terminating\n");
   gpi_thread->state = GPI_THREAD_INITIAL;
   gpi_log(GPI_DEBUG, "terminate_thread() ->\n");
}


/**
 * @brief Re-Entrant Thread handler.
 *
 * @param[in,out ]    gpi_ctxt           GPI context 
 */
void gpi_worker_thread(DALSYSEventHandle unused_handle, void *ctxt)
{
   gpi_ctxt *gpi;
   gpi_thread_type *gpi_thread;

   gpi_log(GPI_DEBUG, "gpi_worker_thread() <-\n");

   if (ctxt == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - bad parameters, quitting\n");
      return;
   }

   gpi = (gpi_ctxt *)ctxt;
   gpi_thread = &gpi->gpi_thread;

   gpi_eventreset(gpi_thread->thread.hEvent);

   /* Wait for event */
   for (;;)
   {
      gpi_log(GPI_DEBUG, "GPI Thread Waiting\n");
      gpi_eventwait(gpi_thread->thread.hEvent);
      gpi_log(GPI_DEBUG, "GPI Thread got event, its state: %d\n", gpi_thread->state);

      /* Reset the event to allow it to be retriggered */
      gpi_eventreset(gpi_thread->thread.hEvent);

      if (gpi_thread->state == GPI_THREAD_TERMINATING)
      {
         gpi_log(GPI_DEBUG, "GPI Thread getting terminated\n");
         terminate_thread(gpi_thread);
      }
      else
      {
         if (gpi_thread->state != GPI_THREAD_STOPPED)
         {
            gpi_thread->state = GPI_THREAD_RUNNING;
            gpi_log(GPI_DEBUG, "GPI THREAD RUNNING\n");
            handle_gpi_evts(gpi);
         }

         gpi_log(GPI_DEBUG, "GPI THREAD State: %d!\n", gpi_thread->state);

         if (gpi_thread->state == GPI_THREAD_TERMINATING)
         {
            gpi_log(GPI_DEBUG, "Terminating GPI thread!\n");
            terminate_thread(gpi_thread);
            break;
         }
         else if (gpi_thread->state != GPI_THREAD_STOPPED)
         {
            gpi_thread->state = GPI_THREAD_WAITING;
         }
      }
   }

   gpi_log(GPI_DEBUG, "gpi_worker_thread() ->\n");
}


/**
 * @brief Spawn GPI Threads.
 *
 * @param[in,out ]     gpi_ctxt           GPI context 
 */
GPI_RETURN_STATUS spawn_threads(gpi_ctxt *gpi)
{
   gpi_log(GPI_DEBUG, "spawn_threads() <-\n");

   if (GPI_STATUS_SUCCESS != gpi_spawn_thread(&gpi->gpi_thread.thread,
                                              thread_name,
                                              &gpi_worker_thread,
                                              (void *)gpi,
                                              gpi->gpi_thread.priority))
   {
      gpi_log(GPI_ERROR, "ERROR - Could not spawn GPI thread, quitting!");
      return GPI_STATUS_ERROR;
   }

   gpi_log(GPI_DEBUG, "spawn_threads() ->\n");
   return GPI_STATUS_SUCCESS;
}


/**
 * @brief Stop GPI threads
 *
 * @param[in,out ]     gpi_ctxt           GPI context 
 * @param[in ]         terminate          Terminate threads if TRUE, otherwise stops them 
 * @param[in ]         wait               Wait time in msec 
 *
 * @return       GPI_RETURN_STATUS     GPI_STATUS_SUCCESS for success, GPI_STATUS_ERROR otherwise.
 *
 */
GPI_RETURN_STATUS stop_thread(gpi_ctxt *gpi, boolean terminate, uint32 wait)
{
   uint32 timeout;

   gpi_log(GPI_DEBUG, "stop_thread() <-\n");

   if (gpi->gpi_thread.state != GPI_THREAD_INITIAL)
   {
      if (terminate)
      {
         gpi->gpi_thread.state = GPI_THREAD_TERMINATING;
         gpi_eventsignal(gpi->gpi_thread.thread.hEvent); 
      }
      else
      {
         gpi->gpi_thread.state = GPI_THREAD_STOPPED;
      }
   }

   if (terminate)
   {
      /* Bail if not done within "wait" msec */
      timeout = 0;
      while (timeout < wait)
      {
         gpi_sleep(5000);
         if (gpi->gpi_thread.state == GPI_THREAD_INITIAL)
         {
            gpi_log(GPI_DEBUG, "Thread terminated\n");
            break;
         }
         timeout += 5;
      }

      if (timeout >= wait)
      {
         gpi_log(GPI_WARNING, "WARNING - Failed to terminate thread\n");
      }
   }

   gpi_log(GPI_DEBUG, "stop_thread() ->\n");
   return GPI_STATUS_SUCCESS;
}

#endif
