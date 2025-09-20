/** @file EFIGpiProtocol.h

  EFI GPI protocols declerations

  Copyright (c) 2017-2018, Qualcomm Technologies Inc. All rights reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/21/18   ts      Added new protocol interfaces
 04/11/17   ts      included de register function
 01/31/17   ts      Initial checkin for sdm845

=============================================================================*/
#ifndef __EFIGPIPROTOCOL_H__
#define __EFIGPIPROTOCOL_H__

#include "gpi.h"

extern EFI_GUID gQcomGpiInitProtocolGuid;

/*
 * Protocol version.
 */
#define EFI_GPI_PROTOCOL_REVISION 0x0000000000010002

typedef struct _QCOM_GPI_PROTOCOL QCOM_GPI_PROTOCOL;

/* ****************************************************************************/
/*                            GPI Protocol APIs                               */
/* ****************************************************************************/

/**
 * Registers a client GPI interface.
 *
 * @param[in]    params                GPI interface registration parameters
 *
 * @return       EFI_STATUS            Return status.
 */
typedef
EFI_STATUS (EFIAPI * GPI_IFACE_REG)
  (gpi_iface_params *params);

/**
 * De-Registers a client GPI interface.
 *
 * @param[in]    params                GPI interface registration parameters
 *
 * @return       EFI_STATUS            Return status.
 */
typedef
EFI_STATUS (EFIAPI * GPI_IFACE_DEREG)
  (GPI_CLIENT_HANDLE handle);

/**
 * Queries the channel current status.
 *
 * @param[in]         handle           Handle to the GPI Interface
 * @param[in]         chan             Virtual Channel ID
 * @param[in, out]    chan_status      Information about channel current status
 *
 * @return            EFI_STATUS       Return status.
 */
typedef
EFI_STATUS (EFIAPI * GPI_QUERY_CHAN_STATUS)
  (GPI_CLIENT_HANDLE handle,
   GPI_CHAN_TYPE chan,
   chan_status_info * chan_status);

/**
 * Polls a GPI interface.
 * The corresponding callback will be invoked with any available results.
 *
 * @param[in]    handle               GPI client handle
 *
 * @return       EFI_STATUS           Return status.
 */
typedef
EFI_STATUS (EFIAPI * GPI_IFACE_POLL)
  (GPI_CLIENT_HANDLE handle);

/**
 * Issues a channel command.
 *
 * @param[in]      handle              Handle to GPI Interface
 * @param[in]      chan                Virtual Channel ID
 * @param[in]      cmd_type            Command type
 * @param[in]      user_cmd            GENERIC_CMD_IBI content
 * @param[in]      user_data           Client context data for this command
 *
 * @return         EFI_STATUS          Return status.
 */
typedef
EFI_STATUS (EFIAPI * GPI_ISSUE_CMD)
  (GPI_CLIENT_HANDLE handle,
   GPI_CHAN_TYPE chan,
   GPI_CHAN_CMD cmd_type,
   UINT32 user_cmd,
   void *user_data);

/**
 * Process TREs request.
 *
 * @param[in]    tre_request           Request details
 *
 * @return       EFI_STATUS            Return status.
 */
typedef
EFI_STATUS (EFIAPI * GPI_PROCESS_TRE)
  (gpi_tre_req *tre_request);

/**
 * Informs GPI of the interface active status
 *
 * @param[in]    params                GPI interface registration parameters
 * @param[in]    active                TRUE if interface is active, FALSE otherwise
 *
 * @return       EFI_STATUS            Return status.
 */
typedef
EFI_STATUS (EFIAPI * GPI_IFACE_ACTIVE)
  (GPI_CLIENT_HANDLE handle,
   boolean active);

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
typedef
EFI_STATUS (EFIAPI * GPI_REG_SAVE)
  (GPI_CLIENT_HANDLE handle,
   gpi_debug_regs *regs);

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/

struct _QCOM_GPI_PROTOCOL {
  UINT64                    Revision;
  GPI_IFACE_REG             iface_reg;
  GPI_REG_SAVE              reg_save;
  GPI_QUERY_CHAN_STATUS     query_chan_status;
  GPI_IFACE_POLL            iface_poll;
  GPI_ISSUE_CMD             issue_cmd;
  GPI_PROCESS_TRE           process_tre;
  GPI_IFACE_DEREG           iface_dereg;
  GPI_IFACE_ACTIVE          iface_active;
};

#endif  /* __EFIGPIPROTOCOL_H__ */
