#ifndef GLINK_RESTRICTED_H
#define GLINK_RESTRICTED_H

/**
 * @file glink_restricted.h
 *
 * Restricted Public API for the GLink
 */

/** \defgroup glink GLink
 * \ingroup SMD
 *
 * GLink reliable, in-order, datagram-based interprocessor communication
 * over a set of supported transport (Shared Memory, UART, BAM, HSIC)
 *
 * All ports preserve message boundaries across the interprocessor channel; one
 * write into the port exactly matches one read from the port.
 */
/*@{*/

/*==============================================================================
     Copyright (c) 2016-2018 QUALCOMM Technologies Incorporated.
     All rights reserved.
     Qualcomm Confidential and Proprietary
==============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/23/16   dks      Expose glink_rx_poll as restricted api
===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "com_dtypes.h"
#include "glink.h"

/*================= RESTRICTED API ==========================*/

/**
 * This API allows clients to poll the transport for any new data that might
 * have come in. It would *usually* be used when incoming interrupts are
 * disabled.
 *
 * @param[in]   handle      GLink handle associated with the logical channel
 *
 * @return      Standard GLink error codes
 *
 * @sideeffects  None
 */
glink_err_type glink_rx_poll
(
  glink_handle_type handle
);


/**
 * This API allows clients to poll the link state. It would *usually* be used
 * when incoming interrupts are disabled.
 *
 * @param[in]   handle      GLink Link handle returned from
 *                          glink_register_link_state_cb
 * @param[in]   state       Buffer to hold GLink Link state info
 *
 * @return      Standard GLink error codes
 *
 * @sideeffects  None
 */
glink_err_type glink_link_state_poll
(
  glink_link_handle_type handle,
  glink_link_state_type *state
);


#endif //GLINK_RESTRICTED_H
