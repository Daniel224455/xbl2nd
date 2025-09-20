#ifndef __HSUART_H__
#define __HSUART_H__
/*==================================================================================================

FILE: HSUart.h

DESCRIPTION: UEFI driver for HS-UARTDM/UARTBAM

                              Copyright (c) 2018-2019, 2021 Qualcomm Technologies, Incorporated
                                        All Rights Reserved
                                     QUALCOMM Proprietary/GTDR

==================================================================================================*/
/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/

RETURN_STATUS EFIAPI hsuart_deinitialize(void);
RETURN_STATUS EFIAPI hsuart_initialize(void);
BOOLEAN       EFIAPI hsuart_poll(void);
UINTN         EFIAPI hsuart_read(OUT UINT8 *user_buffer, IN UINTN bytes_requested);
UINTN         EFIAPI hsuart_write(IN UINT8 *user_buffer, IN UINTN bytes_to_send);
BOOLEAN       EFIAPI hsuart_is_cable_connected(void);
BOOLEAN       EFIAPI hsuart_power_off(void);
BOOLEAN       EFIAPI hsuart_power_on(void);
RETURN_STATUS EFIAPI hsuart_set_baudrate(IN UINTN baud_rate);

#endif  // __HSUART_H__

