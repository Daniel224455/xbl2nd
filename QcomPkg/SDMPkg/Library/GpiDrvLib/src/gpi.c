/**
 * @file  gpi.c
 * @brief Implements the GSI interface 
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
   gpi_init_all(&gpi);
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
   gpii_info *gpii;
   QUPV3_BLOCK_TYPE qup;

   if (params == NULL)
   {
      gpi_log(GPI_ERROR, 0, "iface_reg - ERR - NULL parameters!\n");
      return GPI_STATUS_ERROR;
   }

   /* GPI must have been initialized */
   if (gpi.initialized == FALSE)
   {
      gpi_log(GPI_ERROR, 0, "iface_reg - ERR - GSI not ready!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_log(GPI_DEBUG, 5, "iface_reg - gpii-%d protocol: %d, SE-%d, OUT size: %d, OUT pa: 0x%llx\n",
           params->gpii_id, params->protocol, params->se, params->chan[GPI_OUTBOUND_CHAN].tre_ring_size,
           (uint64)params->chan[GPI_OUTBOUND_CHAN].ring_base_pa);
   gpi_log(GPI_DEBUG, 5, "iface_reg - gpii-%d, OUT va: 0x%llx, IN size: %d, IN pa: 0x%llx, IN va: 0x%llx\n",
           (uint64)params->chan[GPI_OUTBOUND_CHAN].ring_base_va,
           params->chan[GPI_INBOUND_CHAN].tre_ring_size, (uint64)params->chan[GPI_INBOUND_CHAN].ring_base_pa,
           (uint64)params->chan[GPI_INBOUND_CHAN].ring_base_va);

   qup = get_qup_type(params->qup_type);

   /* Validate parameters */
   if (!VALID_QUP(qup) ||
       !VALID_PROTOCOL(params->protocol) ||
       gpi.qup[qup].active == FALSE          ||
       params->gpii_id >= NUM_GPII_PER_QUP                ||
       params->chan[GPI_OUTBOUND_CHAN].tre_ring_size <= 0 ||
       params->chan[GPI_OUTBOUND_CHAN].tre_ring_size > MAX_NUM_GPI_RING_ELEM ||
       params->chan[GPI_OUTBOUND_CHAN].ring_base_pa <= 0 ||
       params->chan[GPI_OUTBOUND_CHAN].ring_base_va <= 0 ||
       params->chan[GPI_INBOUND_CHAN].tre_ring_size <= 0 ||
       params->chan[GPI_INBOUND_CHAN].tre_ring_size > MAX_NUM_GPI_RING_ELEM ||
       params->chan[GPI_INBOUND_CHAN].ring_base_pa <= 0 ||
       params->chan[GPI_INBOUND_CHAN].ring_base_va <= 0 ||
       params->cb == NULL)
   {
      gpi_log(GPI_ERROR, 0, "iface_reg - ERR - wrong params!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.qup[qup].gpii[params->gpii_id])
   {
      if (gpi.qup[qup].gpii[params->gpii_id]->registered == TRUE)
      {
         gpi_log(GPI_ERROR, 1, "iface_reg - ERR - GPII-%d registered!\n", params->gpii_id);
         return GPI_STATUS_ERROR;
      }
   }
   else
   {
      gpi_log(GPI_ERROR, 1, "iface_reg - Allocating GPII-%d\n", params->gpii_id);
      /* Allocate GPII */
      gpi.qup[qup].gpii[params->gpii_id] = (gpii_info *)gpii_malloc();
      if (gpi.qup[qup].gpii[params->gpii_id] == NULL)
      {
         gpi_log(GPI_ERROR, 1, "iface_reg - ERR - GPII-%d alloc\n", params->gpii_id);
         return GPI_STATUS_ERROR;
      }
   }

   gpii = gpi.qup[qup].gpii[params->gpii_id];

   /* Set GPII parameters */
   if (GPI_STATUS_SUCCESS != set_gpii_params(&gpi, gpii, params))
   {
      gpi_log(GPI_ERROR, 0, "iface_reg - ERR - set params!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpii->initialized == FALSE)
   {
      if (GPI_STATUS_SUCCESS != gpii_init(gpii))
      {
         gpi_log(GPI_ERROR, 1, "iface_reg - ERR - gpii-%d init failed!\n", params->gpii_id);
         return GPI_STATUS_ERROR;
      }

      gpii->initialized = TRUE;

      gpi_syncenter(&gpi.qup[qup].qup_mutex);
      if (gpi.irq_muxing)
      {
         if (GPI_STATUS_SUCCESS != gpi_isr_enable(gpii))
         {
            gpi_log(GPI_ERROR, 1, "iface_reg - ERR - gpii ID: %d isr_enable failed\n", gpii->id);
            gpi_syncleave(&gpi.qup[qup].qup_mutex);
            return GPI_STATUS_ERROR;
         }
      }
      gpi.qup[qup].active_gpii_bmsk |= (1 << gpii->id);
      gpi_syncleave(&gpi.qup[qup].qup_mutex);
   }
   else
   {
      gpi_syncenter(&gpi.qup[qup].qup_mutex);
      if ((gpi.qup[gpii->qup_type].active_gpii_bmsk & (1 << gpii->id)) == 0)
      {
         gpi.qup[qup].active_gpii_bmsk |= (1 << gpii->id);
         if (GPI_STATUS_SUCCESS != gpi_isr_enable(gpii))
         {
            gpi_log(GPI_ERROR, 1, "iface_reg - ERR - gpii ID: %d failed isr_enable\n", gpii->id);
            gpi_syncleave(&gpi.qup[qup].qup_mutex);
            return GPI_STATUS_ERROR;
         }
      }
      gpi_syncleave(&gpi.qup[qup].qup_mutex);
   }

   gpii->registered = TRUE;
   gpi_log(GPI_ERROR, 1, "iface_reg - GPII-%d register DONE!\n", params->gpii_id);

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

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, 0, "iface_dereg - ERR - bad params!\n");
      return GPI_STATUS_ERROR;
   }

   gpii = (gpii_info *)handle;

   gpi_syncenter(&gpii->gpii_mutex);
   gpii->cb = NULL;
   gpii->registered = FALSE;
   gpi_syncleave(&gpii->gpii_mutex);

   return GPI_STATUS_SUCCESS;
}

/**
 * Informs GPI of the interface active status.
 *
 * @param[in]    handle            Handle to GPI Interface
 * @param[in]    active            TRUE if interface is active, FALSE otherwise
 *
 * @return       None.
 */
GPI_RETURN_STATUS gpi_iface_active(GPI_CLIENT_HANDLE handle, boolean active)
{
   gpii_info *gpii;
   qup_instance *qup;

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, 0, "iface_active - ERR - bad params!\n");
      return GPI_STATUS_ERROR;
   }

   gpii = (gpii_info *)handle;

   if (!gpii->registered)
   {
      gpi_log(GPI_ERROR, 1, "iface_active - ERR - gpii-%d not reg!\n", gpii->id);
      return GPI_STATUS_ERROR;
   }

   qup = &gpi.qup[gpii->qup_type];

   gpi_syncenter(&gpii->gpi->gpi_mutex);
   gpi_log(GPI_ERROR, 3, "gpi_iface_active - gpii ID: %d, bmsk: %d, active: %d\n", gpii->id, qup->active_gpii_bmsk, active);

   if (active && !(qup->active_gpii_bmsk & (1 << gpii->id)))
   {
      qup->active_gpii_bmsk |= (1 << gpii->id);

      if (GPI_STATUS_SUCCESS != gpi_isr_enable(gpii))
      {
         gpi_syncleave(&gpii->gpi->gpi_mutex);
         gpi_log(GPI_ERROR, 1, "iface_active - ERR - gpii-%d isr enable\n", gpii->id);
         return GPI_STATUS_ERROR;
      }
   }
   else if (!active && (qup->active_gpii_bmsk & (1 << gpii->id)))
   {
      qup->active_gpii_bmsk &= ~(1 << gpii->id);

      if (GPI_STATUS_SUCCESS != gpi_isr_disable(gpii))
      {
         gpi_syncleave(&gpii->gpi->gpi_mutex);
         gpi_log(GPI_ERROR, 1, "iface_active - ERR - gpii-%d isr disable\n", gpii->id);
         return GPI_STATUS_ERROR;
      }
   }

   gpi_syncleave(&gpii->gpi->gpi_mutex);
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

   if (handle == NULL || irq_mode == NULL)
   {
      gpi_log(GPI_ERROR, 0, "iface_in_irq_mode - ERR - bad params!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.qup[gpii->qup_type].gpii_hw_ready_bmsk & (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, 0, "iface_in_irq_mode - ERR - GSI not ready!\n");
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_syncenter(&gpii->gpii_mutex);
   *irq_mode = gpii->irq_mode ? 1 : 0;
   gpi_syncleave(&gpii->gpii_mutex);

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

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, 0, "iface_set_irq_mode - ERR - bad params!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.qup[gpii->qup_type].gpii_hw_ready_bmsk & (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, 1, "iface_set_irq_mode - ERR - GPII-%d not ready!\n", gpii->id);
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_syncenter(&gpii->gpii_mutex);
   gpii->irq_mode = irq_en;
   gpi_syncleave(&gpii->gpii_mutex);

   gpi_log(GPI_DEBUG, 2, "iface_set_irq_mode - gpii-%d irq_mode: %d\n", gpii->id, irq_en);

   return GPI_STATUS_SUCCESS;
}

/**
 * @brief A GPI client may call this function to save the QUP registers of a GPII that it owns.
 *        Clients must make sure to turn the specific QUP clocks on similar to what's done during transfer requests. 
 *
 * @param[in]        handle              Handle to GPI Interface
 * @param[in, out]   regs                Register values saved
 *
 * @return         GPI_RETURN_STATUS     Return status.
 *
 */
GPI_RETURN_STATUS gpi_reg_save(GPI_CLIENT_HANDLE handle, gpi_debug_regs *regs)
{
   gpii_info *gpii = (gpii_info *)handle;
   U_LONG gsi_pa = gpii->gpi->qup[gpii->qup_type].gsi_pa;

   if (handle == NULL || regs == NULL)
   {
      return GPI_STATUS_ERROR;
   }

   regs->irq_type = HWIO_EE_n_CNTXT_TYPE_IRQ_INI(gsi_pa, gpii->id);
   regs->GSI_CH_IRQ = (HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_INMI(gsi_pa, gpii->id,
                                                           HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_BMSK)) >>
                      HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_SHFT;
   regs->EV_CH_IRQ = HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_INMI(gsi_pa, gpii->id, HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_EV_CH_BIT_MAP_BMSK);
   regs->GLOB_IRQ_STTS = HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_INMI(gsi_pa, gpii->id,
                                                            HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ERROR_INT_BMSK) >>
                         HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ERROR_INT_SHFT;
   regs->IEOB_IRQ = HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_INMI(gsi_pa, gpii->id,
                                                      HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_EV_CH_BIT_MAP_BMSK) >>
                    HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_EV_CH_BIT_MAP_SHFT;
   regs->GSI_IRQ_STTS = HWIO_EE_n_CNTXT_GSI_IRQ_STTS_INI(gsi_pa, gpii->id);

   regs->R_0_LEN = HWIO_EE_n_GSI_CH_k_CNTXT_1_INI2(gsi_pa, gpii->id, 0);
   regs->R_1_LEN = HWIO_EE_n_GSI_CH_k_CNTXT_1_INI2(gsi_pa, gpii->id, 1);
   regs->EVT_RING_RP = get_ring_ptr(gpii, 0, EVT_RING_RP);
   regs->local_rp = gpii->evt_ring_info.local_rp;
   regs->CHAN_0_RP = get_ring_ptr(gpii, 0, EVT_RING_RP);
   regs->CHAN_0_WP = get_ring_ptr(gpii, 0, RING_WP);
   regs->CHAN_1_RP = get_ring_ptr(gpii, 1, EVT_RING_RP);
   regs->CHAN_1_WP = get_ring_ptr(gpii, 1, RING_WP);

   regs->CH_0_CNTXT = HWIO_EE_n_GSI_CH_k_CNTXT_0_INI2(gsi_pa, gpii->id, 0);
   regs->CH_1_CNTXT = HWIO_EE_n_GSI_CH_k_CNTXT_0_INI2(gsi_pa, gpii->id, 1);
   regs->CH_0_SCRATCH_0 = HWIO_EE_n_GSI_CH_k_SCRATCH_0_INI2(gsi_pa, gpii->id, 0);
   regs->CH_1_SCRATCH_0 = HWIO_EE_n_GSI_CH_k_SCRATCH_0_INI2(gsi_pa, gpii->id, 1);

   regs->ERROR_LOG = HWIO_EE_n_ERROR_LOG_INI(gsi_pa, gpii->id);
   regs->DEBUG_PC = HWIO_GSI_DEBUG_PC_FOR_DEBUG_IN(gsi_pa);
   regs->DEBUG_SW_RF_10_READ = HWIO_GSI_DEBUG_SW_RF_n_READ_ADDR(gsi_pa, 10);
   regs->DEBUG_SW_RF_11_READ = HWIO_GSI_DEBUG_SW_RF_n_READ_ADDR(gsi_pa, 11);
   regs->DEBUG_SW_RF_12_READ = HWIO_GSI_DEBUG_SW_RF_n_READ_ADDR(gsi_pa, 12);
   regs->DEBUG_SW_RF_21_READ = HWIO_GSI_DEBUG_SW_RF_n_READ_ADDR(gsi_pa, 21);

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

   if (handle == NULL || chan_status == NULL || gpii->qup_type >= MAX_NUM_QUP_BLOCKS)
   {
      gpi_log(GPI_ERROR, 0, "query_chan_status - ERR - bad params!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.qup[gpii->qup_type].gpii_hw_ready_bmsk & (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, 1, "query_chan_status - ERR - GPII-%d not ready!\n", gpii->id);
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_syncenter(&gpii->gpii_mutex);
   gpii->chan_info[chan].state = get_chan_state(gpii, (uint8)chan);
   if (!VALID_CHAN_STATE(gpii->chan_info[chan].state))
   {
      gpi_log(GPI_ERROR, 1, "query_chan_status - ERR - Inv chan state: %d!\n",
              gpii->chan_info[chan].state);
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

   gpi_log(GPI_DEBUG, 4, "query_chan_status - chan state: %d, avail tre: %d, wp index: %d, rp index: %d\n",
           chan_status->chan_state, chan_status->num_avail_tre, chan_status->wp_index, chan_status->rp_index);

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

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, 0, "iface_poll - ERR - bad params!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.qup[gpii->qup_type].gpii_hw_ready_bmsk & (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, 1, "iface_poll - ERR - GPII-%d not ready!\n", gpii->id);
      return GPI_STATUS_GSI_NOT_READY;
   }

   if (gpii->irq_mode)
   {
      gpi_log(GPI_ERROR, 1, "iface_poll - ERR - GPII-%d IRQ mode!\n", gpii->id);
      return GPI_STATUS_ERROR;
   }

   gpi_isr(gpii);

   return GPI_STATUS_SUCCESS;
}

/**
 * Issues a channel command.
 *
 * @param[in]      handle              Handle to GPI Interface
 * @param[in]      chan                Virtual Channel ID
 * @param[in]      cmd_type            Command type
 * @param[in]      user_cmd            GENERIC_CMD_IBI content
 * @param[in]      user_data           Client context data for this command
 *
 * @return       GPI_RETURN_STATUS   Return status.
 */
GPI_RETURN_STATUS gpi_issue_cmd(GPI_CLIENT_HANDLE handle,
                                GPI_CHAN_TYPE     chan,
                                GPI_CHAN_CMD      cmd_type,
                                uint32            user_cmd,
                                void            *user_data)
{
   gpii_info *gpii = (gpii_info *)handle;

   if (handle == NULL)
   {
      gpi_log(GPI_ERROR, 0, "issue_cmd - ERR - bad params!\n");
      return GPI_STATUS_ERROR;
   }

   if (gpi.initialized == FALSE || !(gpi.qup[gpii->qup_type].gpii_hw_ready_bmsk & (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, 1, "issue_cmd - ERR - GSI not ready for GPII-%d!\n",
              gpii->id);
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_log(GPI_DEBUG, 2, "issue_cmd - gpii-%d, cmd_type: %d\n",
           gpii->id, cmd_type);

   if (user_data == NULL)
   {
      gpi_log(GPI_WARNING, 0, "issue_cmd - WARN - Bad input params!\n");
   }

   gpii->cmd_user_data = user_data;

   /* Issue the channel command */
   return issue_cmd(gpii, chan, cmd_type, user_cmd);
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
   uint8 i, chan;
   gpii_info *gpii;
   uint8 wp_idx;
   U_LONG wp;

   /* Validate parameters */
   if (tre_request == NULL ||
       tre_request->handle == NULL ||
       tre_request->tre_list == NULL ||
       tre_request->num_tre == 0)
   {
      gpi_log(GPI_ERROR, 0, "process_tre - ERR - bad params!\n");
      return GPI_STATUS_ERROR;
   }

   gpii = (gpii_info *)tre_request->handle;

   chan = tre_request->chan;
   if (chan >= NUM_RINGS_PER_GPII)
   {
      gpi_log(GPI_ERROR, 2, "process_tre - ERR - GPII-%d bad chan: %d!\n", gpii->id, chan);
      return GPI_STATUS_ERROR;
   }

   wp = get_ring_ptr(gpii, chan, RING_WP);
   if (wp != GET_RING_PHYS_ADDR(&gpii->chan_info[chan], (U_LONG)tre_request->tre_list))
   {
      gpi_log(GPI_ERROR, 1, "process_tre - ERR - GPII-%d wp out of sync!\n", gpii->id);
      return GPI_STATUS_WP_OUT_OF_SYNC;
   }

   if (gpi.initialized == FALSE || !(gpi.qup[gpii->qup_type].gpii_hw_ready_bmsk & (1 << gpii->id)))
   {
      gpi_log(GPI_ERROR, 1, "process_tre - ERR - GPII-%d not ready!\n", gpii->id);
      return GPI_STATUS_GSI_NOT_READY;
   }

   gpi_syncenter(&gpii->gpii_mutex);

   /* make sure the chan is in a good state */
   gpii->chan_info[chan].state = get_chan_state(gpii, chan);
   if (gpii->chan_info[chan].state != GPI_CHAN_RUNNING)
   {
      gpi_log(GPI_ERROR, 3, "process_tre - ERR - GPII-%d chan-%d state: %d!\n",
              gpii->id, tre_request->chan, gpii->chan_info[chan].state);
      gpi_syncleave(&gpii->gpii_mutex);
      return GPI_STATUS_ERROR;
   }

   if (tre_request->user_data == NULL)
   {
      gpi_log(GPI_WARNING, 1, "process_tre - WARN - gpii-%d user data!\n", gpii->id);
   }

   /* make sure ring has enough space */
   if (tre_request->num_tre > nbr_avail_ring_elem(gpii, (uint8)chan,
                                                  gpii->chan_info[chan].base_pa,
                                                  wp,
                                                  get_ring_ptr(gpii, chan, RING_RP),
                                                  gpii->chan_info[chan].num_elem))
   {
      gpi_log(GPI_ERROR, 3, "process_tre - ERR - gpii-%d chan-%d, no space for %d TRE's!\n",
              gpii->id, chan, tre_request->num_tre);
      gpi_syncleave(&gpii->gpii_mutex);
      return GPI_STATUS_ERROR;
   }

   wp_idx = GET_RING_PTR_INDEX(gpii, chan, RING_WP);

   for (i = 0; i < tre_request->num_tre; i++)
   {
      gpii->transf_ctxt[chan][wp_idx] = tre_request->user_data;
      wp_idx = INC_RING_IDX(wp_idx, gpii->chan_info[chan].num_elem);
   }

   /* Ring channel DB */
   if (gpi.env_64bit)
   {
      HWIO_EE_n_GSI_CH_k_DOORBELL_1_OUTI2(gpi.qup[gpii->qup_type].gsi_pa,
                                          gpii->id,
                                          chan,
                                          (uint32)(((gpii->chan_info[chan].base_pa + (wp_idx * sizeof(gpi_ring_elem))) & 0xFFFFFFFF00000000) >> 32));
   }
   else
   {
      HWIO_EE_n_GSI_CH_k_DOORBELL_1_OUTI2(gpi.qup[gpii->qup_type].gsi_pa, gpii->id, chan, 0);
   }

   HWIO_EE_n_GSI_CH_k_DOORBELL_0_OUTI2(gpi.qup[gpii->qup_type].gsi_pa,
                                       gpii->id,
                                       chan,
                                       (uint32)((get_system_address(gpii->chan_info[chan].base_pa) + (wp_idx * sizeof(gpi_ring_elem))) & 0xFFFFFFFF));
   gpi_syncleave(&gpii->gpii_mutex);

   return GPI_STATUS_SUCCESS;
}
