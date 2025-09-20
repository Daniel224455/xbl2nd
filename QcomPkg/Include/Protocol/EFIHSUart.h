/** 
  @file  EFIHSUart.h
  @brief HSUart UEFI Protocol definitions.
*/
/*=============================================================================
  Copyright (c) 2018-2019, 2021 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 01/21/21   PCR     Added changes to support set_baudrate API
 04/22/19   nm      Support de-initialize API
 11/16/18   nm      Initial revision for UEFI

=============================================================================*/

#ifndef __EFIHSUART_H__
#define __EFIHSUART_H__

/** @addtogroup efi_hsuart_constants 
@{ */
/** 
  Protocol version. 
*/
#define HSUART_DXE_REVISION 0x00010006
/** @} */ /* end_addtogroup efi_hsuart_constants */

/**
  HS-UART UEFI typedefs
*/
typedef struct _EFI_QCOM_HSUART_PROTOCOL   EFI_QCOM_HSUART_PROTOCOL;
/** @endcond */

/*  HS-UART UEFI Procotol GUID */
/** @ingroup efi_hsuart_protocol */
extern EFI_GUID gQcomHSUartProtocolGuid;

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
/* OPEN */ 
/** @ingroup OPEN
  @par Summary
  This function is called by the client code to initialize the respective 
  HS-UART core instance used by the client.
  
*/
typedef
RETURN_STATUS
(EFIAPI *OPEN) (
    void
);

/* READ */
/** @ingroup READ
  @par Summary
  Performs an hsuart read.

  @param[out] buffer                Buffer into which data is read.
  @param[in]  buffer_len            Length of data that needs to be read.
                                    the slave.
  @return 
  Actual bytes read
*/
typedef
UINTN
(EFIAPI *READ) (
    OUT UINT8 *user_buffer,
    IN UINTN bytes_requested
);

/* WRITE */
/** @ingroup WRITE
  @par Summary
  Performs an hsuart write.

  @param[out] buffer                Buffer from which data is written.
  @param[in]  buffer_len            Length of data that needs to be written.
                                    the slave.

  @return 
  Actual bytes written
*/
typedef
UINTN
(EFIAPI *WRITE) (
    IN UINT8 *user_buffer,
    IN UINTN bytes_to_send
);

/* POLL */
/** @ingroup POLL
  @par Summary
  Polls for RX Watermark

  @return 
  Boolean TRUE if watermark set and vice versa.
*/
typedef
BOOLEAN
(EFIAPI *POLL) (
    void
);

/* CLOSE */ 
/** @ingroup CLOSE
  @par Summary
  This function is called by the client code to de-initialize the respective 
  HS-UART core instance used by the client.
  
*/
typedef
RETURN_STATUS
(EFIAPI *CLOSE) (
    void
);

/* SETBAUD */
/** @ingroup CLOSE
  @par Summary
  This function is called by the client code to de-initialize the respective
  HS-UART core instance used by the client.

*/
typedef
RETURN_STATUS
(EFIAPI *SETBAUD) (
    IN UINTN baud_rate
);
/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_hsuart_protocol
  @par Summary
  Qualcomm HS-UART Protocol interface.

  @par Parameters
  @inputprotoparams{hsuart_proto_params.tex} 
*/
struct _EFI_QCOM_HSUART_PROTOCOL {
  UINT64        Revision;
  OPEN          open;
  READ          read;
  WRITE         write;
  POLL          poll;
  CLOSE         close;
  SETBAUD       set_baudrate;
};

#endif	/* __EFIHSUART_H__ */
