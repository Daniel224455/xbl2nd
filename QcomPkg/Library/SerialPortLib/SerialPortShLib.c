/** @file SerialPortShLib.c
   
  Contains Serial IO Shared Library loader and access functions.

  Copyright (c) 2012,2017 Qualcomm Technologies, Inc.  All Rights Reserved.
   
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 11/09/17   bd      Add option to enable HS UART
 01/17/13   vk      Fix warning
 09/17/12   yg      Initial Version

=============================================================================*/

#include "ShLib.h"
#include "Library/SerialPortShLib.h"
#include <Library/SerialPortLib.h>

/* Lib interface pointer instance local to the app,
 * for using in the pass through layer */
STATIC SioPortLibType *SioPortLibPtr = NULL;

EFI_STATUS SerialPortInitialize (VOID);

UINTN
SerialPortRead (UINT8* Buffer, UINTN Bytes)
{
  if (SioPortLibPtr == NULL)
  {
    SerialPortInitialize();
    if (SioPortLibPtr == NULL)
      return 0;
  }
  return SioPortLibPtr->Read (Buffer, Bytes);
}

UINTN
SerialPortWrite (UINT8* Buffer, UINTN Bytes)
{
  if (SioPortLibPtr == NULL)
  {
    SerialPortInitialize();
    if (SioPortLibPtr == NULL)
      return 0;
  }
  return SioPortLibPtr->Write (Buffer, Bytes);
}

BOOLEAN
SerialPortPoll (VOID)
{
  if (SioPortLibPtr == NULL)
  {
    SerialPortInitialize();
    if (SioPortLibPtr == NULL)
      return 0;
  }
  return SioPortLibPtr->Poll ();
}

UINTN SerialPortControl (IN UINTN Arg,
                     IN UINTN Param)
{
  if (SioPortLibPtr == NULL)
  {
    SerialPortInitialize();
    if (SioPortLibPtr == NULL)
      return 0;
  }
  return SioPortLibPtr->Control (Arg, Param);
}

UINTN SerialPortFlush (VOID)
{
  if (SioPortLibPtr == NULL)
  {
    SerialPortInitialize();
    if (SioPortLibPtr == NULL)
      return 0;
  }
  return SioPortLibPtr->Flush ();
}

UINTN SerialPortDrain (VOID)
{
  if (SioPortLibPtr == NULL)
  {
    SerialPortInitialize();
    if (SioPortLibPtr == NULL)
      return 0;
  }
  return SioPortLibPtr->Drain ();
}

VOID
EnableSynchronousSerialPortIO (VOID)
{
  SerialPortControl (SIO_CONTROL_SYNCHRONOUS_IO, TRUE);
}

VOID
DisableSerialOut (VOID)
{
  SerialPortControl (SIO_CONTROL_PORTOUT, FALSE);
}

EFI_STATUS SerialPortInitialize (VOID)
{
  ShLibLoaderType *ShLibLoader;

  ShLibLoader = GetShLibLoader();
  if (ShLibLoader == NULL)
    return EFI_DEVICE_ERROR;

  return ShLibLoader->LoadLib (SIO_PORT_LIB_NAME, 0, (VOID*)&SioPortLibPtr);
}

RETURN_STATUS
EFIAPI
SerialPortSetAttributes (
  IN OUT UINT64             *BaudRate,
  IN OUT UINT32             *ReceiveFifoDepth,
  IN OUT UINT32             *Timeout,
  IN OUT EFI_PARITY_TYPE    *Parity,
  IN OUT UINT8              *DataBits,
  IN OUT EFI_STOP_BITS_TYPE *StopBits
  )
{
  if (SioPortLibPtr == NULL)
  {
    SerialPortInitialize();
    if (SioPortLibPtr == NULL)
      return 0;
  }
  return SioPortLibPtr->SetAttributes(BaudRate, ReceiveFifoDepth, Timeout, Parity, DataBits, StopBits);
}