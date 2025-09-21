/**
 * @file  gpi_utils.c
 * @brief Implements GPI utility APIs
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


/**
 * @brief Checks whether the transfer ring is empty/full or has queued xfers.
 *
 * @param[in ]        gpii         GPII instance
 * @param[in ]        chan         Channel number
 * @param[in ]        Base              transfer ring base address
 * @param[in ]        WP           transfer ring WP address
 * @param[in ]        RP           transfer ring RP address
 * @param[in ]        Size              Ring size
 */
uint32 nbr_avail_ring_elem(gpii_info *gpii,
                           uint8 chan,
                           ptr_type base,
                           ptr_type wp,
                           ptr_type rp,
                           ptr_type size)
{
   uint32 wp_idx, rp_idx;
   uint32 avail_elem;

   gpi_log(GPI_DEBUG, "nbr_avail_ring_elem() <-\n");

   wp_idx = GET_RING_ELEMENT_INDEX(base, wp);
   rp_idx = GET_RING_ELEMENT_INDEX(base, rp);

   if ((wp_idx + 1) % size == rp_idx)
      avail_elem = 0;
   else
      avail_elem = (wp_idx < rp_idx) ? (rp_idx - wp_idx) : (size - wp_idx + rp_idx);

   if (gpii != NULL)
      gpi_log(GPI_INFO, "GPII ID: %d Chan %d Ring with size of %d has %d available elements!!\n",
              gpii->id, chan, size, avail_elem);
   else
      gpi_log(GPI_INFO, "Queue %d with size of %d has %d available elements!!\n",
              chan, size, avail_elem);

   gpi_log(GPI_DEBUG, "nbr_avail_ring_elem() ->\n");
   return avail_elem;
}


/**
 * @brief Queue an element to a Work queue.
 *
 * @param[in ]        Queue             Work queue
 */
boolean en_q(gpi_work_q *q, gpi_ring_elem *elem)
{
   uint32 wp_idx;
   uint32 nbr_avail;

   gpi_syncenter(&q->lock);
   wp_idx = GET_RING_ELEMENT_INDEX((ptr_type)q->base, (ptr_type)q->wp);

   nbr_avail = nbr_avail_ring_elem(NULL, 0,
                                   (ptr_type)q->base, (ptr_type)q->wp,
                                   (ptr_type)q->rp, GPI_EVT_Q_SIZE);

   if (0 == nbr_avail)
   {
      gpi_log(GPI_WARNING, "WARNING - Queue full!\n");
      gpi_syncleave(&q->lock);
      return FALSE;
   }

   gpi_memcpy(q->wp, elem, sizeof(gpi_ring_elem));
   q->wp = ((wp_idx + 1) == GPI_EVT_Q_SIZE) ? q->base : q->wp + 1;
   gpi_syncleave(&q->lock);

   return TRUE;
}

/**
 * @brief Retrieves a work queue element.
 *
 * @param[in ]        Queue             Work queue
 * @param[in,out ]    Element           Work queue element
 */
boolean de_q(gpi_work_q *q, gpi_ring_elem *elem)
{
   uint32 rp_idx;
   uint32 nbr_avail;

   gpi_syncenter(&q->lock);
   rp_idx = GET_RING_ELEMENT_INDEX((ptr_type)q->base, (ptr_type)q->rp);

   nbr_avail = nbr_avail_ring_elem(NULL, 0, (ptr_type)q->base,
                                   (ptr_type)q->wp, (ptr_type)q->rp, GPI_EVT_Q_SIZE);

   if (nbr_avail == GPI_EVT_Q_SIZE)
   {
      gpi_log(GPI_WARNING, "WARNING - Queue empty!\n");
      gpi_syncleave(&q->lock);
      return FALSE;
   }

   gpi_memcpy(elem, (void *)q->rp, sizeof(gpi_ring_elem));
   q->rp = ((rp_idx + 1) == GPI_EVT_Q_SIZE) ? q->base : q->rp + 1;

   gpi_syncleave(&q->lock);

   return TRUE;
}

/**
 * @brief Resets a GPI event queue.
 *
 * @param[in ]        q             GPI work queues
 */
void reset_evt_q(gpi_work_q *q)
{
   q->base = &q->elem[0];
   q->rp = q->base;
   q->wp = q->base;
}

/** Reads a channel ctxt state
 */
GPI_CHAN_STATE get_chan_state(gpii_info *gpii, uint8 chan)
{
   return ((GPI_CHAN_STATE)((HWIO_EE_n_GSI_CH_k_CNTXT_0_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan) &
                             HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_BMSK) >> HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_SHFT));
}

/** Reads a ring ctxt pointer
 */
ptr_type get_ring_ptr(gpii_info *gpii, uint8 chan, GPI_RING_PTR_TYPE ring_ptr_type)
{
   ptr_type ptr;

   switch (ring_ptr_type)
   {
      case RING_RP:
         ptr = HWIO_EE_n_GSI_CH_k_CNTXT_4_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan);
         if (sizeof(ptr) > 4)
         {
            ptr |= (uint64)HWIO_EE_n_GSI_CH_k_CNTXT_5_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan) << 32;
         }
         break;
      case RING_WP:
         ptr = HWIO_EE_n_GSI_CH_k_CNTXT_6_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan);
         if (sizeof(ptr) > 4)
         {
            ptr |= (uint64)HWIO_EE_n_GSI_CH_k_CNTXT_7_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan) << 32;
         }
         break;
      case EVT_RING_RP:
         ptr = HWIO_EE_n_EV_CH_k_CNTXT_4_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0);
         if (sizeof(ptr) > 4)
         {
            ptr |= (uint64)HWIO_EE_n_EV_CH_k_CNTXT_5_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0) << 32;
         }
         break;
      case EVT_RING_UPDATED_RP:
         ptr = HWIO_EE_n_EV_CH_k_CNTXT_12_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0);
         if (sizeof(ptr) > 4)
         {
            ptr |= (uint64)HWIO_EE_n_EV_CH_k_CNTXT_13_INI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0) << 32;
         }
         break;
      /* case EVT_RING_WP: Not needed */
      default:
         ptr = 0;
         gpi_log(GPI_DEBUG, "get_ring_ptr(), unsupported ptr type\n");
   }

   return (ptr_type)get_local_address(ptr);
}

/**
 * @brief Initializes a specific channel registers.
 *
 * @param[in ]        gpii           gpii context
 * @param[in ]        chan_type      inbound/outbound channel type
 */
void init_chan_regs(gpii_info *gpii, GPI_CHAN_TYPE chan)
{
   GPI_CHAN_TYPE comp_chan = (chan == GPI_INBOUND_CHAN) ? GPI_OUTBOUND_CHAN : GPI_INBOUND_CHAN;

   HWIO_EE_n_GSI_CH_k_CNTXT_0_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan,                        
                                     HWIO_EE_n_GSI_CH_k_CNTXT_0_ELEMENT_SIZE_BMSK, 
                                     16 << HWIO_EE_n_GSI_CH_k_CNTXT_0_ELEMENT_SIZE_SHFT);

   /* Store the ring length of this channel */
   HWIO_EE_n_GSI_CH_k_CNTXT_1_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan,
                                     HWIO_EE_n_GSI_CH_k_CNTXT_1_R_LENGTH_BMSK,
                                     (gpii->chan_info[chan].num_elem * sizeof(gpi_ring_elem))<<
                                     HWIO_EE_n_GSI_CH_k_CNTXT_1_R_LENGTH_SHFT);

   /*  Store the ring base address (LSB and MSB) of this channel */
   HWIO_EE_n_GSI_CH_k_CNTXT_2_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan,
                                     HWIO_EE_n_GSI_CH_k_CNTXT_2_R_BASE_ADDR_LSBS_BMSK, 
                                     ((get_system_address(gpii->chan_info[chan].base_pa) & 0xFFFFFFFF) <<
                                      HWIO_EE_n_GSI_CH_k_CNTXT_2_R_BASE_ADDR_LSBS_SHFT));

   if (sizeof(ptr_type) > 4)
   {
      HWIO_EE_n_GSI_CH_k_CNTXT_3_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan, 
                                        (uint32)((get_system_address(gpii->chan_info[chan].base_pa) >> 32)));
   }
   else
   {
      HWIO_EE_n_GSI_CH_k_CNTXT_3_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan, 0);
   }

   HWIO_EE_n_GSI_CH_k_CNTXT_0_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan,
                                     HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_BMSK, 
                                     HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_GPI_FVAL <<
                                     HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_SHFT);
   HWIO_EE_n_GSI_CH_k_CNTXT_0_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan,
                                     HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_DIR_BMSK, 
                                     comp_chan << HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_DIR_SHFT);
   HWIO_EE_n_GSI_CH_k_CNTXT_0_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan,
                                     HWIO_EE_n_GSI_CH_k_CNTXT_0_ERINDEX_BMSK, 
                                     0 << HWIO_EE_n_GSI_CH_k_CNTXT_0_ERINDEX_SHFT);

   HWIO_EE_n_GSI_CH_k_QOS_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan,
                                0 << HWIO_EE_n_GSI_CH_k_QOS_USE_DB_ENG_SHFT   |
                                0 << HWIO_EE_n_GSI_CH_k_QOS_MAX_PREFETCH_SHFT |
                                1 << HWIO_EE_n_GSI_CH_k_QOS_WRR_WEIGHT_SHFT);

   HWIO_EE_n_GSI_CH_k_SCRATCH_0_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan, 
                                      (comp_chan << 16 | gpii->protocol << 4 | gpii->se));

   HWIO_EE_n_GSI_CH_k_SCRATCH_1_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan, 0);
   HWIO_EE_n_GSI_CH_k_SCRATCH_2_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan, 0);
   HWIO_EE_n_GSI_CH_k_SCRATCH_3_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, chan, 0);
}

/**
 * @brief Initializes a specific event ring registers.
 *
 * @param[in ]        gpii           gpii context
 */
void init_evt_ring_regs(gpii_info *gpii)
{
   HWIO_EE_n_EV_CH_k_CNTXT_0_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 
                                    HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_BMSK, 
                                    HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_GPI_EV_FVAL << HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_SHFT);
   HWIO_EE_n_EV_CH_k_CNTXT_0_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 
                                    HWIO_EE_n_EV_CH_k_CNTXT_0_INTYPE_BMSK, 
                                    HWIO_EE_n_EV_CH_k_CNTXT_0_INTYPE_IRQ_FVAL << HWIO_EE_n_EV_CH_k_CNTXT_0_INTYPE_SHFT);
   HWIO_EE_n_EV_CH_k_CNTXT_0_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 
                                    HWIO_EE_n_EV_CH_k_CNTXT_0_ELEMENT_SIZE_BMSK, 
                                    16 << HWIO_EE_n_EV_CH_k_CNTXT_0_ELEMENT_SIZE_SHFT);

   /* configure the ring length of this event ring */
   HWIO_EE_n_EV_CH_k_CNTXT_1_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0,
                                    HWIO_EE_n_EV_CH_k_CNTXT_1_R_LENGTH_BMSK,
                                    (NUM_GPI_EVT_RING_ELEM * sizeof(gpi_ring_elem)) <<
                                    HWIO_EE_n_EV_CH_k_CNTXT_1_R_LENGTH_SHFT);

   /* configure the read base address (LSB and MSB) of this event ring */
   HWIO_EE_n_EV_CH_k_CNTXT_2_OUTMI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0,
                                    HWIO_EE_n_EV_CH_k_CNTXT_2_R_BASE_ADDR_LSBS_BMSK, 
                                    ((get_system_address(gpii->evt_ring_info.base_pa) & 0xFFFFFFFF) <<
                                     HWIO_EE_n_EV_CH_k_CNTXT_2_R_BASE_ADDR_LSBS_SHFT));

   if (sizeof(ptr_type) > 4)
   {
      HWIO_EE_n_EV_CH_k_CNTXT_3_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 
                                        (uint32)((get_system_address(gpii->evt_ring_info.base_pa) >> 32)));
   }
   else
   {
      HWIO_EE_n_EV_CH_k_CNTXT_3_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 0);
   }

   HWIO_EE_n_EV_CH_k_CNTXT_8_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0,
                                   (gpii->int_modt_val & 0xFFFF) << HWIO_EE_n_EV_CH_k_CNTXT_8_INT_MODT_SHFT |
                                   (gpii->int_modc_val & 0xFF) << HWIO_EE_n_EV_CH_k_CNTXT_8_INT_MODC_SHFT);

   HWIO_EE_n_EV_CH_k_CNTXT_13_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 0);
   HWIO_EE_n_EV_CH_k_CNTXT_12_OUTI2(gpii->gpi->qup[gpii->qup_type].gsi_pa, gpii->id, 0, 0);
}

/**
 * @brief Given a GPII ID return the GPII index if it exists
 *
 * @param[in ]     gpi_ctxt           GPI context 
 * @param[in ]     gpii_id            GPII ID
 *
 * @return       index of a GPII if it exists, 0xFF otherwise.
 *
 */
uint8 get_gpii_idx(gpi_ctxt *gpi, uint8 gpii_id, QUP_BLOCK_TYPE qup_type)
{
   uint8 i;

   for (i = 0; i < gpi->num_gpii; i++)
   {
      if (gpi->gpii[i].qup_type == qup_type &&
          gpi->gpii[i].id == gpii_id)
      {
         return i;
      }
   }

   return 0xFF;
}

