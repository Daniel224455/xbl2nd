#ifndef __UART_H__
#define __UART_H__
/*==================================================================================================

FILE: Uart.h

DESCRIPTION: UEFI driver for UARTDM/UARTBAM

                              Copyright (c) 2013,2018-2019 Qualcomm Technologies, Incorporated
                                        All Rights Reserved
                             Qualcomm Technologies, Inc. Confidential and Proprietary.

==================================================================================================*/
/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/

RETURN_STATUS EFIAPI uart_initialize(void);
BOOLEAN       EFIAPI uart_poll(void);
UINTN         EFIAPI uart_read(OUT UINT8 *user_buffer, IN UINTN bytes_requested);
UINTN         EFIAPI uart_write(IN UINT8 *user_buffer, IN UINTN bytes_to_send);
BOOLEAN       EFIAPI uart_is_cable_connected(void);
BOOLEAN       EFIAPI uart_power_off(void);
BOOLEAN       EFIAPI uart_power_on(void);
RETURN_STATUS EFIAPI uart_set_baud_rate(IN UINT32 baud_rate, IN VOID* callback_fn);

#endif  // __UART_H__

