/** @file SerialPortShLib.h
   
  Contains Serial IO Shared Library Interface.

  Copyright (c) 2012,2018-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
   
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/31/19   nm      Add IOCTL to support setting UART Baud Rate
 03/23/18   yg      Enable serial out
 02/08/18   bd      Add option to enable HS UART
 09/17/12   yg      Initial Version

=============================================================================*/
#ifndef __SIO_LIB__H__
#define __SIO_LIB__H__

#include "Uefi.h"
#include <Library/SerialPortLib.h>

/* Name for the SerialPort Shared Library Interface */
#define SIO_PORT_LIB_NAME "SerialPort Lib"

/* Corresponds to version 1.2
 * MSB 16 bits Major version
 * LSB 16 bits Minor version
 * See note about the versioning and the way Loader works in LOAD_LIB comments
 * In short, if major version is same the lib should be "strictly compatible"
 * with all the previous versions */
#define SIO_PORT_LIB_VERSION  0x00010002

/* Port Control Option Arguments and Param values (Refer to SIO_CONTROL)
 *   Arg : 
 *     SIO_CONTROL_BUFFERING
 *       Control Buffering of the output log data,
 *          Param : FALSE - Disables buffering
 *          Param : TRUE  - Enables buffering
 *
 *     SIO_CONTROL_PORTOUT
 *       Control Data output through Serial Port, the part of logs during disabled
 *       state would just stay in the buffer will not be sent over PORT until re-
 *       enabled
 *          Param : FALSE - Disables Data output via serial port
 *          Param : TRUE  - Enables Data output via serial port
 *
 *     SIO_CONTROL_SYNCHRONOUS_IO
 *       Configure if Synchronous IO will be performed.
 *          Param : TRUE  - Enables Blocked IO (doesn't return until the data is sent).
 *          Param : FALSE - Disables Blocked IO
 *
 *     SIO_CONTROL_POWER
 *       Power up or down UART port.
 *          Param : TRUE  - Powers ON UART port
 *          Param : FALSE - Powers OFF UART port
 *
 *     SIO_CONTROL_SET_BAUD_RATE
 *       Set the Baud Rate for the UART port with option to pass callback function to change frequency
 *          Param : UartBaudRateCb - Baud Rate and Callback to set frequency
 * */
#define SIO_CONTROL_BUFFERING        0x01
#define SIO_CONTROL_PORTOUT          0x02
#define SIO_CONTROL_SYNCHRONOUS_IO   0x03
#define SIO_CONTROL_POWER            0x04
#define SIO_CONTROL_SET_BAUD_RATE    0x05

/*
 * Supported UART baud rates
*/ 
typedef enum _UART_BAUD_RATE
{
  UART_BAUD_RATE_115200 = 115200,
  UART_BAUD_RATE_1MBPS = 1000000,
  UART_BAUD_RATE_3MBPS = 3000000,
  UART_BAUD_RATE_INVALID = 0
} UART_BAUD_RATE;

typedef struct {
  UART_BAUD_RATE   BaudRate; 
  VOID*            callback_fn;
} UartBaudRateCb;

/* Typedefs for SerialPort Shared Library Interface */
typedef UINTN   (*SIO_READ) (OUT  UINT8  *Buffer, IN UINTN NumberOfBytes);
typedef UINTN   (*SIO_WRITE) (IN  UINT8  *Buffer, IN UINTN NumberOfBytes);
typedef BOOLEAN (*SIO_POLL) (VOID);
typedef UINTN   (*SIO_DRAIN) (VOID);
typedef UINTN   (*SIO_FLUSH) (VOID);
typedef UINTN   (*SIO_CONTROL) (IN UINTN Arg, IN UINTN Param);
typedef EFI_STATUS   (*SIO_SETATTRIBUTES) (IN OUT UINT64    *BaudRate,  IN OUT UINT32   *ReceiveFifoDepth,
                                           IN OUT UINT32    *Timeout,   IN OUT EFI_PARITY_TYPE    *Parity,
                                           IN OUT UINT8     *DataBits,  IN OUT EFI_STOP_BITS_TYPE *StopBits );

/* SerialPort Shared Library Interface */
typedef struct {
  UINT32       LibVersion; 
  SIO_READ     Read;
  SIO_WRITE    Write;
  SIO_POLL     Poll;
  SIO_DRAIN    Drain;
  SIO_FLUSH    Flush;
  SIO_CONTROL  Control;
  SIO_SETATTRIBUTES  SetAttributes;
}SioPortLibType;

/**

  Loader function that helps getting the Serial Port Lib interface

  @return EFI_SUCCESS               Successfully notified. 
  @return EFI_INVALID_PARAMETER     Invalid parameter passed.

**/
INTN LoadSioLib (IN  SioPortLibType* SioLibIntfPtr OPTIONAL,
                 IN  UINT32          LibVersion    OPTIONAL);

/******************************************************************
 *
 *  Below are specific to the shared lib implementation 
 *
 ******************************************************************/

 /**

  De queue and write buffer content to UART, until end of buffer is reached

  @return Number of bytes remaining to be sent. 
**/
UINTN SerialPortDrain (VOID);

/**

  De queue and write buffer content to UART, until buffer is empty

  @return Number of bytes remaining to be sent. 
**/
UINTN SerialPortFlush (VOID);

/**

  Control Port output and buffering options

  @param  Arg                       Argument indicating different control options
  @param  Param                     Parameter for previous control argument
  
  @return EFI_SUCCESS               Successfully updated the options. 

**/
UINTN SerialPortControl (IN UINTN Arg,
                         IN UINTN Param);

/**

  Enable Synchronous IO.

**/
VOID EnableSynchronousSerialPortIO (VOID);

/**

  Disable Serial IO.

**/
VOID DisableSerialOut (VOID);

/**

  Enable Serial IO.

**/
VOID EnableSerialOut (VOID);

/**

  Set different attributes of serial port
  
  @param  BaudRate                  Value indicating the BaudRate to be set
  @param  ReceiveFifoDepth          
  @param  Timeout          
  @param  Parity          
  @param  DataBits            
  @param  StopBits              
  
  @return EFI_SUCCESS               Successfully updated the options.   

**/
RETURN_STATUS
EFIAPI
SerialPortSetAttributes (
  IN OUT UINT64             *BaudRate,
  IN OUT UINT32             *ReceiveFifoDepth,
  IN OUT UINT32             *Timeout,
  IN OUT EFI_PARITY_TYPE    *Parity,
  IN OUT UINT8              *DataBits,
  IN OUT EFI_STOP_BITS_TYPE *StopBits
  );

/**

  Set Baud Rate of serial port
  
  @param  BaudRate                  Value indicating the BaudRate to be set
  
  @return EFI_SUCCESS               Successfully updated the options.   

**/  
RETURN_STATUS
EFIAPI
SerialPortSetBaudRate (
  IN OUT UINT64             *BaudRate
  );

#endif /* __SIO_LIB__H__ */

