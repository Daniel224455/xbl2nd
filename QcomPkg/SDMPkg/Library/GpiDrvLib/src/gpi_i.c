/**
  @file  gpi_i.c
  @brief GPI internal APIs.

*/
/*
===============================================================================

                               Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------ 
03/09/18   ah      Hana changes.
02/19/16   ah      File created.

===============================================================================
              Copyright (c) 2016-2018 QUALCOMM Technologies, Inc.  
                         All Rights Reserved.
            Qualcomm Technologies Proprietary and Confidential.
===============================================================================
*/

#include "gpi_utils.h"


/**
 * Issues a GPI command.
 *
 * @param[in]    gpii_id         GPI Interface ID
 * @param[in]    chan            Virtual Channel ID
 * @param[in]    cmd_type        Command type
 * @param[in]    user_cmd        GENERIC_CMD_IBI content
 *
 * @return       GPI_RETURN_STATUS   Return status.
 */
GPI_RETURN_STATUS issue_cmd(gpii_info    *gpii,
                            GPI_CHAN_TYPE chan,
                            GPI_CHAN_CMD  cmd_type,
                            uint32        user_cmd)
{
   uint32 cmd = 0;
   GPI_RETURN_STATUS status = GPI_STATUS_SUCCESS;
   GPI_CHAN_STATE curr_state = (chan == GPI_EVT_RING) ?
                               gpii->evt_ring_info.state :
                               gpii->chan_info[chan].state;
   uint32 cmd_opcode_shft = (chan == GPI_EVT_RING) ?
                            HWIO_EE_n_EV_CH_CMD_OPCODE_SHFT :
                            HWIO_EE_n_GSI_CH_CMD_OPCODE_SHFT;

   gpi_log(GPI_DEBUG, 3, "issue_cmd - gpii-%d, chan: %d, cmd: %d\n",
           gpii->id, chan, cmd_type);

   if (chan == GPI_EVT_RING              &&
       cmd_type != GPI_CHAN_CMD_ALLOCATE &&
       cmd_type != GPI_CHAN_CMD_DE_ALLOC)
   {
      gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - cmd: %d not allowed for event rings\n",
              cmd_type);
      return GPI_STATUS_ERROR;
   }

   gpi_syncenter(&gpii->gpii_mutex);

   /* EEs are limited to a single outstanding CMD for both
      event channel/transfer channel */
   if (gpii->pending_cmd != GPI_CHAN_CMD_NONE)
   {
      gpi_log(GPI_ERROR, 2, "issue_cmd - ERR - pending command: %d for GPII-%d\n",
              gpii->pending_cmd, gpii->id);
      gpi_syncleave(&gpii->gpii_mutex);
      return GPI_STATUS_ERROR;
   }

   /* Validate the requested state according to the chan state machine */
   switch (cmd_type)
   {
      case GENERIC_CMD_IBI:
         /* GENERIC_CMD, not channel-specific, no checks needed */
         break;
      case GPI_CHAN_CMD_ALLOCATE:
         if (curr_state != GPI_CHAN_DISABLED)
         {
            gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - CMD_ALLOCATE, curr state: %d\n",
                    curr_state);
            status = GPI_STATUS_ERROR;
         }
         break;
      case GPI_CHAN_CMD_START:
         if (curr_state != GPI_CHAN_ALLOCATED &&
             curr_state != GPI_CHAN_STOPPED &&
             curr_state != GPI_CHAN_STOP_IN_PROG)
         {
            gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - CMD_START, curr state: %d\n",
                    curr_state);
            status = GPI_STATUS_ERROR;
         }
         break;
      case GPI_CHAN_CMD_STOP:
         if (curr_state == GPI_CHAN_DISABLED ||
             curr_state == GPI_CHAN_ALLOCATED)
         {
            gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - CMD_STOP, curr state: %d\n",
                    curr_state);
            status = GPI_STATUS_ERROR;
         }
         break;
      case GPI_CHAN_CMD_RESET:
         /* Reset should not be called from Disabled or Allocated state,
          * this causes IPA endpoint to be reset before it was enabled.
          * After reset, channel should be reconfigured before using it again
          */
         if (curr_state != GPI_CHAN_STOPPED)
         {
            gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - CMD_RESET, curr state: %d\n",
                    curr_state);
            status = GPI_STATUS_ERROR;
         }
         break;
      case GPI_CHAN_CMD_DE_ALLOC:
         if (curr_state != GPI_CHAN_ALLOCATED)
         {
            gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - CMD_DE_ALLOC, curr state: %d\n",
                    curr_state);
            status = GPI_STATUS_ERROR;
         }
         break;
      case GPI_CHAN_CMD_DB_STOP:
         if (curr_state == GPI_CHAN_DISABLED  ||
             curr_state == GPI_CHAN_ALLOCATED ||
             chan == GPI_EVT_RING)
         {
            gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - CMD_DB_STOP, curr state: %d\n",
                    curr_state);
            status = GPI_STATUS_ERROR;
         }
         break;

      case QUP_CMD_GSI_STOP_ON_EOT:
      case QUP_CMD_UART_SW_STALE:
      case QUP_CMD_UART_RFR_READY:
      case QUP_CMD_UART_RFR_NOT_READY:
      case QUP_CMD_UART_BREAK_START:
      case QUP_CMD_UART_BREAK_STOP:
         /* For UART commands we don't do any protocol specific checking... */
         break;

      default:
         gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - default cmd_type: %d\n", cmd_type);
         status = GPI_STATUS_ERROR;
   }

   if (status != GPI_STATUS_ERROR)
   {
      if (cmd_type != GENERIC_CMD_IBI)
      {
         cmd = cmd_type << cmd_opcode_shft;
         gpii->pending_cmd = cmd_type;

         if (chan != GPI_EVT_RING)
         {
            cmd |= chan;
            HWIO_EE_n_GSI_CH_CMD_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, cmd);
         }
         else
         {
            HWIO_EE_n_EV_CH_CMD_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, cmd);
         }
      }
      else
      {
         HWIO_EE_n_GSI_EE_GENERIC_CMD_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, user_cmd);
      }
      gpi_log(GPI_ERROR, 0, "issue_cmd - ISSUED\n");
   }

   gpi_syncleave(&gpii->gpii_mutex);
   return status;
}

#ifdef GPI_MT
/**
 * @brief Initializes GPI synchronization objects
 *
 * @param[in]    gpi                   Pointer to GPI context.
 *
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS init_gpi_sync(gpi_ctxt *gpi)
{
   uint32 i;

   if (GPI_STATUS_SUCCESS != gpi_syncinit(&gpi->gpi_mutex))
      return GPI_STATUS_ERROR;

   for (i = 0; i < MAX_NUM_QUP_BLOCKS; i++)
   {
      if (gpi->qup[i].active)
      {
         if (GPI_STATUS_SUCCESS != gpi_syncinit(&gpi->qup[i].qup_mutex))
         {
            gpi_log(GPI_ERROR, 1, "init_gpi_sync - ERR - QUP-%d!\n", i);
            return GPI_STATUS_ERROR;
         }
      }
   }

   return GPI_STATUS_SUCCESS;
}

/**
 * @brief Initializes GPI threads
 *
 * @param[in]    gpi                   Pointer to GPI context.
 *
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS init_threads(gpi_ctxt *gpi)
{
   gpi->gpi_thread.priority = GPI_TASK_PRI;
   gpi->gpi_thread.state = GPI_THREAD_INITIAL;
   gpi_eventInit(&gpi->gpi_thread.thread.hEvent);
   reset_evt_q(&gpi->gpi_thread.q);
   if (GPI_STATUS_SUCCESS != gpi_syncinit(&gpi->gpi_thread.q.lock))
   {
      gpi_log(GPI_ERROR, 0, "init_threads - ERR - fail!\n");
      return GPI_STATUS_ERROR;
   }

   return GPI_STATUS_SUCCESS;
}
#endif

/**
 * @brief Set GPII specific parameters all GPI rings
 *
 * @param[in]       gpii           Pointer to GPII context.
 * @param[in]       params         Client submitted parameters
 * @param[in]       chan_type      Channel type
 *  
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS set_gpii_chan(ring_info_type *chan_info,
                                gpi_tre_ring *params)
{
   chan_info->state = GPI_CHAN_DISABLED;
   chan_info->base_pa = params->ring_base_pa;
   chan_info->base_va = params->ring_base_va;
   chan_info->num_elem = params->tre_ring_size;
   chan_info->local_rp = chan_info->base_pa;

   return GPI_STATUS_SUCCESS;
}

/**
 * @brief Set GPII specific parameters all GPI rings
 *
 * @param[in]       gpii               Pointer to GPII context.
 *  
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS set_gpii_evt_chan(gpii_info *gpii)
{
   ring_info_type *evt_ring_info = &gpii->evt_ring_info;

#ifdef GPI_ISLAND_MODE_ENABLE
   /* Allocate evt ring if needed */
   gpi_ctxt *gpi = gpii->gpi;
#endif

   if (gpii->evt_ring_elem == NULL)
   {
#ifdef GPI_ISLAND_MODE_ENABLE
      gpi_syncenter(&gpi->gpi_mutex);
      if (gpi->pram_idx < gpi->num_pram_blocks)
      {
         gpii->evt_ring_elem = gpi->pram + (gpi->pram_idx * NUM_GPI_EVT_RING_ELEM);
         gpii->gpi->pram_idx++;
      }
      gpi_syncleave(&gpi->gpi_mutex);
#else
      gpii->evt_ring_elem = (gpi_ring_elem  *)gpi_er_malloc();
#endif
   }

      if (!gpii->evt_ring_elem)
      {
      gpi_log(GPI_ERROR, 1, "set_gpii_evt_chan - ERR - GPII-%d no mem\n", gpii->id);
         return GPI_STATUS_ERROR;
      }

   evt_ring_info->base_va = (U_LONG)&gpii->evt_ring_elem[0];
   evt_ring_info->base_pa = (U_LONG)gpi_va_to_pa((void *)evt_ring_info->base_va);
   evt_ring_info->local_rp = evt_ring_info->base_pa;
   evt_ring_info->state = GPI_CHAN_DISABLED;
   evt_ring_info->num_elem = NUM_GPI_EVT_RING_ELEM;

   return GPI_STATUS_SUCCESS;
}

/**
 * @brief Set GPII specific parameters all GPI rings
 *
 * @param[in,out ]     gpi_ctxt           GPI context
 * @param[in]          gpii              Pointer to GPII context.
 * @param[in,out]      params            Client submitted parameters
 */
GPI_RETURN_STATUS set_gpii_params(gpi_ctxt *gpi, gpii_info *gpii, gpi_iface_params *params)
{
   params->gpi_handle = (void *)gpii;
   gpii->qup_type = get_qup_type(params->qup_type);
   gpii->id = params->gpii_id;
   gpii->protocol = params->protocol;
   gpii->se = params->se;
   gpii->int_modt_val = params->int_modt_val;
   gpii->int_modc_val = params->int_modc_val;
   gpii->irq_mode = params->irq_mode;
   gpii->cb = params->cb;
   gpii->user_data = params->user_data;
   gpii->gpi = gpi;
   gpii->pending_cmd = GPI_CHAN_CMD_NONE;

   /* Channel ctxt */
   if (GPI_STATUS_SUCCESS != set_gpii_chan(&gpii->chan_info[GPI_INBOUND_CHAN],
                                           &params->chan[GPI_INBOUND_CHAN]))
   {
      gpi_log(GPI_ERROR, 2, "set_gpii_params - ERR - gpii-%d chan %d!\n",
              gpii->id, GPI_INBOUND_CHAN);
      return GPI_STATUS_ERROR;
   }
   if (GPI_STATUS_SUCCESS != set_gpii_chan(&gpii->chan_info[GPI_OUTBOUND_CHAN],
                                           &params->chan[GPI_OUTBOUND_CHAN]))
   {
      gpi_log(GPI_ERROR, 2, "set_gpii_params - ERR - gpii-%d chan %d!\n",
              gpii->id, GPI_OUTBOUND_CHAN);
      return GPI_STATUS_ERROR;
   }

   /* Evt ring ctxt */
   if (GPI_STATUS_SUCCESS != set_gpii_evt_chan(gpii))
   {
      gpi_log(GPI_ERROR, 1, "set_gpii_params - ERR - gpii-%d evt ring!\n", gpii->id);
      return GPI_STATUS_ERROR;
   }

   return GPI_STATUS_SUCCESS;
}

/**
 * Initializes GPII HW registers and interrupt.
 *
 * @param[in]    gpii                  Pointer to GPII context.
 *
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS gpii_init(gpii_info *gpii)
{
   gpi_ctxt *gpi = gpii->gpi;
   U_LONG gsi_pa = gpi->qup[gpii->qup_type].gsi_pa;

   if (gpii->initialized == TRUE)
   {
      gpi_log(GPI_WARNING, 1, "gpii_init - WARN - gpii-%d initialized!\n", gpii->id);
      return GPI_STATUS_SUCCESS;
   }

#ifdef GPI_MT
   if (GPI_STATUS_SUCCESS != gpi_syncinit(&gpii->gpii_mutex))
   {
      gpi_log(GPI_ERROR, 0, "gpii_init - ERR - gpi_syncinit failed!\n");
      return GPI_STATUS_ERROR;
   }
#endif

   if (((HWIO_EE_n_GSI_STATUS_INMI(gsi_pa, gpii->id,
                                   HWIO_EE_n_GSI_STATUS_ENABLED_BMSK)) >> HWIO_EE_n_GSI_STATUS_ENABLED_SHFT) != 0)
   {
      HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTI(gsi_pa, gpii->id, ~0x0u);
      HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_OUTI(gsi_pa, gpii->id, ~0x0u);
      HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_OUTI(gsi_pa, gpii->id, ~0x0u);
      HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_OUTI(gsi_pa, gpii->id, ~0x0u);
      HWIO_EE_n_CNTXT_GLOB_IRQ_EN_OUTI(gsi_pa, gpii->id, ~0x0u);
      HWIO_EE_n_CNTXT_GSI_IRQ_EN_OUTI(gsi_pa, gpii->id, ~0x0u);

      /* Set interrupt type */
      HWIO_EE_n_CNTXT_INTSET_OUTMI(gsi_pa, gpii->id,
                                   HWIO_EE_n_CNTXT_INTSET_INTYPE_BMSK, GPI_INT_IRQ << HWIO_EE_n_CNTXT_INTSET_INTYPE_SHFT);

      if (gpii->irq_mode && !gpi->irq_muxing)
      {
         gpii->irq = gpii_irq_val(gpii);
         if (GPI_STATUS_SUCCESS != register_irq(gpii, gpii->irq))
         {
            gpi_log(GPI_ERROR, 0, "gpii_init - ERR - register irq failed!\n");
            return GPI_STATUS_ERROR;
         }
      }

      HWIO_EE_n_CNTXT_MSI_BASE_LSB_OUTI(gsi_pa, gpii->id, 0);
      HWIO_EE_n_CNTXT_MSI_BASE_MSB_OUTI(gsi_pa, gpii->id, 0);
      HWIO_EE_n_CNTXT_INT_VEC_OUTI(gsi_pa, gpii->id, 0);
      HWIO_EE_n_CNTXT_SCRATCH_0_OUTI(gsi_pa, gpii->id, 0);
      HWIO_EE_n_CNTXT_SCRATCH_1_OUTI(gsi_pa, gpii->id, 0);
      HWIO_EE_n_ERROR_LOG_OUTI(gsi_pa, gpii->id, 0x0);
      HWIO_EE_n_ERROR_LOG_CLR_OUTI(gsi_pa, gpii->id, ~0x0u);

      HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_OUTI(gsi_pa, gpii->id, ~0x0u);
      HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_OUTI(gsi_pa, gpii->id, ~0x0u);

      gpi->qup[gpii->qup_type].gpii_hw_ready_bmsk |= 1 << gpii->id;
   }
   else
   {
      gpi_log(GPI_ERROR, 1, "gpii_init - ERR - gpii-%d not ready\n", gpii->id);
      return GPI_STATUS_ERROR;
   }

   return GPI_STATUS_SUCCESS;
}


/**
 * Main function to initialize GPI common data structure, interrupts,
 * threads, and synchronization objects.
 *
 * @param[in]    gpi                   Pointer to GPI context.
 *
 * @return       GPI_RETURN_STATUS     Return status.
 */
GPI_RETURN_STATUS gpi_common_init(gpi_ctxt *gpi)
{
#ifdef GPI_ISLAND_MODE_ENABLE
   uint32 size = 0;
#endif

   if (!gpi->initialized)
   {
      /* Start GPI basic initialization */
      if (gpi->exec_mode == GPI_THREAD)
      {
         if (GPI_STATUS_SUCCESS != init_gpi_sync(gpi))
         {
            gpi_log(GPI_ERROR, 0, "common_init - ERR - init sync fail!g\n");
            return GPI_STATUS_ERROR;
         }

         if (GPI_STATUS_SUCCESS != init_threads(gpi))
         {
            gpi_log(GPI_ERROR, 0, "common_init - ERR - init thread fail!\n");
            return GPI_STATUS_ERROR;
         }

         if (GPI_STATUS_SUCCESS != spawn_threads(gpi))
         {
            gpi_log(GPI_ERROR, 0, "common_init - ERR - spawn thread fail!\n");
            return GPI_STATUS_ERROR;
         }
      }
      else
      {
         /* Initialize thread queue only, to be used by task/polling */
         reset_evt_q(&gpi->gpi_thread.q);
      }

      if (gpi->irq_muxing)
      {
         if (GPI_STATUS_SUCCESS != register_irq_all(gpi))
         {
            gpi_log(GPI_ERROR, 0, "common_init - ERR - irq all fail!\n");
            return GPI_STATUS_ERROR;
         }
      }

#ifdef GPI_ISLAND_MODE_ENABLE
      /* Get memory from PRAM */
      gpi->pram = (gpi_ring_elem  *)gpi_acquire_partition(&size);
      if (gpi->pram == NULL)
      {
         gpi_log(GPI_ERROR, 0, "ERROR - partition alloc fail!\n");
         return GPI_STATUS_ERROR;
      }
      gpi->num_pram_blocks = size / (NUM_GPI_EVT_RING_ELEM * sizeof(gpi_ring_elem));
#endif

      gpi->initialized = TRUE;
      return GPI_STATUS_SUCCESS;
   }
   else
   {
      gpi_log(GPI_WARNING, 0, "common_init - WARN - gpi initialized!!\n");
      return GPI_STATUS_SUCCESS;
   }
}
