/**
  @file  gpi_isr.c
  @brief GPI interrupt handler.
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


static gpi_ctxt *glob_ctxt = NULL;


/**
 * @brief Complete the execution of remaining work if in polling mode,
 *        or defer to a task or worker thread if in ISR context.
 *
 * @param[in ]     gpii        GPII context
 * @param[in ]     handler     handling thread
 * @param[in ]     evt         Event
 */
void dispatch_worker(gpii_info *gpii, gpi_ring_elem *evt)
{
   if (evt != NULL)
   {
      if (TRUE != en_q(&gpii->gpi->gpi_thread.q, evt))
         return;
   }

   if (gpii->gpi->exec_mode == GPI_POLLING)
   {
      /* Complete the handling of this event */
      handle_gpi_evts(gpii->gpi);
   }
   else
   {
      /* Defer further execution to a worker thread/task */
      gpi_signal_worker(gpii->gpi);
   }
}

/**
 * @brief Handle channle control
 *
 * @param[in ]     gpii        GPII context
 */
void handle_chan_ctrl(gpii_info *gpii)
{
   uint32 i, bmsk;
   gpi_ring_elem evt;
   uint32 chan_state;

   evt.dword_2 = 0;
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GPII_ID, gpii->id);
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GSI_INST, gpii->qup_type);

   HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                      HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_CH_CTRL_BMSK,
                                      (1 << HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_CH_CTRL_SHFT));

   HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_OUTMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                            HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_GSI_CH_BIT_MAP_MSK_BMSK,
                                            HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_GSI_CH_BIT_MAP_MSK_BMSK <<
                                            HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_GSI_CH_BIT_MAP_MSK_SHFT);

   evt.ctrl = GPI_IRQ_CH_CTRL;

   /* Get a Bit map of channels whose state has changed */
   bmsk = (HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_INMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                               HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_BMSK)) >>
          HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_SHFT;

   for (i = 0; i < NUM_RINGS_PER_GPII; i++)
   {
      if (bmsk & (1 << i))
      {
         GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_CHID, i);

         chan_state = HWIO_EE_n_GSI_CH_k_CNTXT_0_INMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, i,
                                                       HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_BMSK) >>
                      HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_SHFT;

         /* Save the new channel state for later handling by the helper */
         GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_CHAN_STATE, chan_state);

         dispatch_worker(gpii, &evt);

         /* Clear corresponding bit */
         HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, (1 << i));
      }
   }
}

/**
 * @brief Handle event ring control
 *
 * @param[in ]     gpii        GPII context
 */
void handle_evt_ctrl(gpii_info *gpii)
{
   gpi_ring_elem evt;
   uint32 chan_state;

   evt.dword_2 = 0;
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GPII_ID, gpii->id);
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GSI_INST, gpii->qup_type);

   HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                      HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_EV_CTRL_BMSK,
                                      (1 << HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_EV_CTRL_SHFT));

   evt.ctrl = GPI_IRQ_EV_CTRL;

   chan_state = HWIO_EE_n_EV_CH_k_CNTXT_0_INMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0,
                                                HWIO_EE_n_EV_CH_k_CNTXT_0_CHSTATE_BMSK) >>
                HWIO_EE_n_EV_CH_k_CNTXT_0_CHSTATE_SHFT;

   /* Save new state for later handling by the helper */
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_CHAN_STATE, chan_state);

   dispatch_worker(gpii, &evt);

   /* Clear corresponding bit */
   HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 1);
}

/**
 * @brief Handle global events
 *
 * @param[in ]     gpii        GPII context
 */
void handle_global(gpii_info *gpii)
{
   uint32 bmsk;
   gpi_ring_elem evt;

   evt.dword_2 = 0;
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GPII_ID, gpii->id);
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GSI_INST, gpii->qup_type);

   HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                      HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_GLOB_EE_BMSK,
                                      (1 << HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_GLOB_EE_SHFT));

   evt.ctrl = GPI_IRQ_GLOB_EE;

   /* Figure out what kind of global interrupt occurred */
   bmsk = HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_INMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                             HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ERROR_INT_BMSK) >>
          HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ERROR_INT_SHFT;

   /* Currently only error interrupt is supported */
   if (bmsk & HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ERROR_INT_BMSK)
   {
      /* Track the first error that occurred in this GPII, helper will handle it */
      GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_ERROR,
                      HWIO_EE_n_ERROR_LOG_INMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                               HWIO_EE_n_ERROR_LOG_ERROR_LOG_BMSK) >>
                      HWIO_EE_n_ERROR_LOG_ERROR_LOG_SHFT);

      dispatch_worker(gpii, &evt);

      /* Clear it */
      HWIO_EE_n_ERROR_LOG_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0);
      HWIO_EE_n_ERROR_LOG_CLR_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, ~0u);
   }

   /* Clear corresponding bit */
   HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 1);
}

/**
 * @brief Handle IEOB completions
 *
 * @param[in ]     gpii        GPII context
 */
void handle_ieob(gpii_info *gpii)
{
   HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                      HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_IEOB_BMSK,
                                      (1 << HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_IEOB_SHFT));

   gpi_syncenter(&gpii->gpi->gpi_mutex);
   //gpii->gpi->gsi_evt_ring_bmsk |= (1 << gpii->id);
   gpii->gpi->gsi_evt_ring_bmsk |= (1 << (get_gpii_idx(gpii->gpi, gpii->id, gpii->qup_type)));
   gpi_syncleave(&gpii->gpi->gpi_mutex);

   dispatch_worker(gpii, NULL);

   HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                         HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_EV_CH_BIT_MAP_BMSK);
}

/**
 * @brief Handle inter-EE channel control
 *
 * @param[in ]     gpii        GPII context
 */
void handle_inter_ee_chan_ctrl(gpii_info *gpii)
{
   uint32 i, bmsk;
   gpi_ring_elem evt;

   evt.dword_2 = 0;
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GPII_ID, gpii->id);
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GSI_INST, gpii->qup_type);

   HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                      HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INTER_EE_CH_CTRL_BMSK,
                                      (1 << HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INTER_EE_CH_CTRL_SHFT));

   evt.ctrl = GPI_IRQ_INTER_EE_CH_CTRL;

   /* Figure which channel state changed in which EE. */
   bmsk = HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_INMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                              HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_BMSK) >>
          HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_SHFT;

   if (bmsk)
   {
      /* In this case there is no additional channel state information */
      for (i = 0; i < NUM_RINGS_PER_GPII; i++)
      {
         if (bmsk & (1 << i))
         {
            GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_CHID, i);

            /* SW expected to figure the new channel state based on
               the control operation it performed on the channel */
            GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_CHAN_STATE,
                            HWIO_EE_n_GSI_CH_k_CNTXT_0_INMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, i,
                                                             HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_BMSK) >>
                            HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_SHFT);

            dispatch_worker(gpii, &evt);

            /* Clear */
            HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, (1 << i));
         }
      }
   }
}

/**
 * @brief Handle inter-EE event ring control
 *
 * @param[in ]     gpii        GPII context
 */
void handle_inter_ee_evt_ctrl(gpii_info *gpii)
{
   uint32 bmsk;
   gpi_ring_elem evt;

   evt.dword_2 = 0;
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GPII_ID, gpii->id);
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GSI_INST, gpii->qup_type);

   HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                      HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INTER_EE_EV_CTRL_BMSK,
                                      (1 << HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INTER_EE_EV_CTRL_SHFT));

   evt.ctrl = GPI_IRQ_INTER_EE_EV_CTRL;

   /* Figure which event channel state changed in which EE */
   bmsk = HWIO_INTER_EE_n_SRC_EV_CH_IRQ_INMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                             HWIO_INTER_EE_n_SRC_EV_CH_IRQ_EV_CH_BIT_MAP_BMSK) >>
          HWIO_INTER_EE_n_SRC_EV_CH_IRQ_EV_CH_BIT_MAP_SHFT;

   if (bmsk)
   {
      /* Figure the new channel state based on the control operation it performed,
         save new state for later handling by the helper */
      GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_CHAN_STATE,
                      HWIO_EE_n_EV_CH_k_CNTXT_0_INMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0,
                                                      HWIO_EE_n_EV_CH_k_CNTXT_0_CHSTATE_BMSK) >>
                      HWIO_EE_n_EV_CH_k_CNTXT_0_CHSTATE_SHFT);
      dispatch_worker(gpii, &evt);

      /* Clear */
      HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 1);
   }
}

/**
 * @brief Handle general events
 *
 * @param[in ]     gpii        GPII context
 */
void handle_general(gpii_info *gpii)
{
   gpi_ring_elem evt;

   evt.dword_2 = 0;
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GPII_ID, gpii->id);
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_GSI_INST, gpii->qup_type);

   HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTMI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                      HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_GENERAL_BMSK,
                                      (1 << HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_GENERAL_SHFT));

   evt.ctrl = GPI_IRQ_GENERAL;

   /* Figure the interrupt type, save it for later handling */
   GPI_WRITE_FIELD(evt.dword_2, LOCAL_EVT_INT_TYPE,
                   HWIO_EE_n_CNTXT_GSI_IRQ_STTS_INI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id));

   dispatch_worker(gpii, &evt);

   /* Clear */
   HWIO_EE_n_CNTXT_GSI_IRQ_CLR_OUTI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id,
                                    HWIO_EE_n_CNTXT_GSI_IRQ_CLR_RMSK);
}

/**
 * @brief Handle IRQ, gets the IRQ type and handles it
 *
 * @param[in ]     gpii        GPII context
 */
void handle_irq(gpii_info *gpii)
{
   volatile GPI_IRQ_TYPE irq_type;

   /* Get the IRQ type */
   irq_type = (GPI_IRQ_TYPE)HWIO_EE_n_CNTXT_TYPE_IRQ_INI(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id);

   if (irq_type & GPI_IRQ_CH_CTRL)           /* Channel control interrupt */
      handle_chan_ctrl(gpii);

   if (irq_type & GPI_IRQ_EV_CTRL)           /* Event control interrupt */
      handle_evt_ctrl(gpii);

   if (irq_type & GPI_IRQ_GLOB_EE)           /* Global interrupt */
      handle_global(gpii);

   if (irq_type & GPI_IRQ_IEOB)              /* Event interrupts, associated with specific event element */
      handle_ieob(gpii);

   if (irq_type & GPI_IRQ_INTER_EE_CH_CTRL)  /* External channel control interrupt */
      handle_inter_ee_chan_ctrl(gpii);

   if (irq_type & GPI_IRQ_INTER_EE_EV_CTRL)  /* External evnet control interrupt */
      handle_inter_ee_evt_ctrl(gpii);

   if (irq_type & GPI_IRQ_GENERAL)           /* General interrupt */
      handle_general(gpii);
}

/**
 * @brief GPI interrupt handler
 *
 * @param[in ]
 */
void gpi_isr(void *ctxt)
{
   uint8           i;
   QUP_BLOCK_TYPE  qup_type = GPI_MAX_BLOCKS;

   if (glob_ctxt != NULL)
   {
      qup_type = (QUP_BLOCK_TYPE)ctxt;
      
      if (qup_type >= QUP_0 && qup_type < GPI_MAX_BLOCKS)
      {
         /* Investigate all GPII's belonging to this QUP */
         for (i = 0; i < glob_ctxt->num_gpii; i++)
         {
            if (glob_ctxt->gpii[i].qup_type == qup_type &&
                glob_ctxt->gpii[i].registered == TRUE)
            {
               handle_irq(&glob_ctxt->gpii[i]);
            }
         }
      }
   }
   else
   {
      handle_irq((gpii_info *)ctxt);
   }
}

/**
 * @brief Registers GPI interrupt handler
 *
 * @param[in ]        gpii           GPII instance
 */
GPI_RETURN_STATUS register_irq(gpii_info *gpii)
{
   /* Register IRQ handler */
   return gpi_isrinstall(gpii->irq, gpi_isr, (void *)gpii);
}

/**
 * Installs ISRs for all muxed IRQ's, one IRQ per QUPv3.
 * Called in case of muxed IRQ's only.
 *
 * @param[in ]        gpi           GPI context
 * 
 * @return   None.
 */
GPI_RETURN_STATUS register_irq_all(gpi_ctxt *gpi)
{
   uint8 i;

   glob_ctxt = gpi;

   for (i = 0; i < MAX_NUM_QUP_BLOCKS; i++)
   {
      if (gpi->qup[i].active)
      {
         if (GPI_STATUS_SUCCESS != gpi_isrinstall(gpi->qup[i].qup_irq, gpi_isr, (void *)gpi->qup[i].type))
         {
            return GPI_STATUS_ERROR;
         }
      }
   }

   return GPI_STATUS_SUCCESS;
}

