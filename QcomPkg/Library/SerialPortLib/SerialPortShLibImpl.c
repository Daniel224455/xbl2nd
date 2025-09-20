/** @file SerialPortShLibImpl.c
   
  Contains Serial IO Shared Library loader and access functions.

  Copyright (c) 2012-2019 Qualcomm Technologies, Inc.  All Rights Reserved.
   
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/31/19   nm      Add IOCTL to support setting UART Baud Rate
 12/18/18   md      Added API to reinitialize Serial buffer to configurable size.
                    Added API to lock Serial buffer reinitialization.  
 10/15/18   bd      SerialLock  changes to handle MT/MC disabled case
 03/23/18   yg      No port out fixes
 02/08/18   bd      Add option to enable HS UART
 11/01/17   yg      Refactor to support add locks for multithread/multicore
 11/01/16   bh      Update log buffer initialization
 07/15/16   vk      Print buffering status
 10/20/15   bh      Return if SBL Logs are not being passed correctly
 08/19/15   vk      Enable buffered logs
 08/13/15   vk      Add PRE_SIL for supporting RAM loading
 06/04/15   wg      Added function to prepend Sbl Logs to Uefi Logs
 02/11/15   bh      Put EnQ/DeQ in critical section, fix bug with missing logs
 08/12/14   vk      Enable UART write
 07/09/14   vk      Skip uart_write till driver is available
 06/05/14   na      Set SyncIOEnable to TRUE for 8994 bring up
 03/28/14   vk      Set SyncIOEnable to TRUE for PRE_SIL
 10/22/13   vk      Update return types for 64 bit
 04/15/13   yg      Use buffer layer as top level serial port Lib
 01/24/13   yg      Add non blocking buffer layer
 12/10/12   yg      Buffer dynamically allocated by client
 09/17/12   yg      Initial Version

=============================================================================*/

#include "ShLib.h"
#include "Library/SerialPortShLib.h"
#include <Library/BaseMemoryLib.h>
#include "Uart.h"
#include <Library/ArmLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <PiDxe.h>
#include <boot_shared_imem_cookie.h>
#include <Library/UefiCfgLib.h>

/* Version 1.1 */
#define CURRENT_SIO_LIB_VERSION  0x00010001

// FB
#include <Library/TimerLib.h>
#include <Library/SerialPortLib.h>
#include <Library/font5x12.h>
#include <Library/CacheMaintenanceLib.h>
#include "FrameBufferSerialPortLib.h"

#define FB_BGRA8888_BLACK 0xff000000
#define FB_BGRA8888_WHITE 0xffffffff
#define FB_BGRA8888_CYAN 0xff00ffff
#define FB_BGRA8888_BLUE 0xff0000ff
#define FB_BGRA8888_SILVER 0xffc0c0c0
#define FB_BGRA8888_YELLOW 0xffffff00
#define FB_BGRA8888_ORANGE 0xffffa500
#define FB_BGRA8888_RED 0xffff0000
#define FB_BGRA8888_GREEN 0xff00ff00

FBCON_POSITION m_Position;
FBCON_POSITION m_MaxPosition;
FBCON_COLOR    m_Color;
BOOLEAN        m_Initialized = FALSE;

UINTN gWidth = 1080;
// Reserve half screen for output
UINTN gHeight = 2400;
UINTN gBpp    = 32;
UINTN delay = 10000;

// Module-used internal routine
void FbConPutCharWithFactor(char c, int type, unsigned scale_factor);

void FbConDrawglyph(
    char *pixels, unsigned stride, unsigned bpp, UINTN *glyph,
    unsigned scale_factor);

void FbConReset(void);
void FbConScrollUp(void);
void FbConFlush(void);

// FB end

STATIC UINT8 *SerialPortBuffer;
STATIC UINT8 *EnQPtr, *DeQPtr;
STATIC UINT32 PortBufferSize = 0, ActiveDataSize = 0;
STATIC BOOLEAN NoPortOut = FALSE;
STATIC BOOLEAN SyncIOEnable = FALSE;
STATIC BOOLEAN UartPowered = TRUE;
STATIC BOOLEAN ReInitDone = FALSE;

/*Define a global pointer which points to the boot shared imem cookie structure */
struct boot_shared_imem_cookie_type *boot_shared_imem_cookie_ptr = NULL;                  

extern SioPortLibType SioLib;

/*******************************************************************************
 *            Private functions
 *******************************************************************************/

/* Start + Stop + Gap, reduce to 2 if required */
#define UART_OVER_HEAD_BITS         3
#define TX_FIFO_SIZE                64
#define BAUD_RATE                   115200

#define BITS_PER_UART_BYTE          (8 + UART_OVER_HEAD_BITS)
#define BITS_FOR_FIFO_DRAIN         (TX_FIFO_SIZE * BITS_PER_UART_BYTE)
#define UART_FIFO_DRAIN_TIME_MS     ((1000 * BITS_FOR_FIFO_DRAIN)/BAUD_RATE)

UINTN DequeueSendBufferedData (VOID);
UINTN EnqueueData (UINT8*, UINTN);
UINTN SerialPortLock (VOID);
VOID SerialPortUnlock (UINTN State);
void Sleep (uint32_t MilliSec);

#define CHECK_SIZE(x)   do{ if (x > PortBufferSize) while (1);   }while(0)

/*
 *   UART Buffering design
 *
 *   SerialPortBuffer
 *   |
 *   |
 *   |<-------------------- PortBufferSize ------------------------------>|                                                                   
 *   |                                                                    |
 *   |             |<---ActiveDataSize--->|                               |
 *   V             |                      |                               |
 *   +-------------+----------------------+-------------------------------+
 *   |             |                      |                               |
 *   | Free spc    | Buffered Data        |   Free Space                  |
 *   |             |                      |                               |
 *   +-------------+----------------------+-------------------------------+
 *                 | Send to port =>      |
 *                 V                      +--> Add data here
 *                 DeQPtr (DQ)            EnQPtr (NQ)
 *
 *   NQ will be pointing to empty location where data can be written
 *   DQ will be pointing to filled location where valid data is prsent
 *   ActiveDataSize variable holds the number of active bytes to be sent
 *   over to UART
 *
 * */

/* Returns how many bytes remaining to send
 * Expectation is that the lock is already obtained
 * Do NOT call this function if NOPORT out is set */
UINTN
DequeueSendBufferedData (VOID)
{
  UINT8* BufferEndPtr;
  UINTN BytesToSend, BytesSent, ContiguousSize;

  if (SerialPortBuffer == NULL)
    return 0;

  BufferEndPtr = SerialPortBuffer + PortBufferSize;
  /*
   *  Scenario 1:       Start        DeQ       EnQ        End
   *
   *  Scenario 2:       Start           EnQ        DeQ    End
   * */
  BytesToSend = ActiveDataSize;

  if (BytesToSend)
  {
    ContiguousSize = BufferEndPtr - DeQPtr;
    if (BytesToSend > ContiguousSize)
      BytesToSend = ContiguousSize;

    CHECK_SIZE (BytesToSend);
    BytesSent = uart_write (DeQPtr, BytesToSend);

    ActiveDataSize -= BytesSent;
    CHECK_SIZE (ActiveDataSize);

    DeQPtr += BytesSent;

    /* Wrap around */
    if (DeQPtr >= BufferEndPtr)
      DeQPtr = SerialPortBuffer;

    BytesToSend = ActiveDataSize;
  }

  return BytesToSend;
}

/* Returns the number of bytes Queued (that were consumed) 
 * Expectation is that the lock is already obtained */
UINTN
EnqueueData (UINT8* SrcDataBuffer, UINTN ReqBytes)
{
  UINT8* BufferEndPtr;
  UINTN  FreeSpace, CopySize, BytesRemaining;

  if (SerialPortBuffer == NULL)
    return 0;

  BytesRemaining = ReqBytes;
  BufferEndPtr = SerialPortBuffer + PortBufferSize;

  while (BytesRemaining)
  {
    /* See how much we can fill/copy */
    CopySize = BytesRemaining;

    /* See if we have enough space to Queue this */
    FreeSpace = PortBufferSize - ActiveDataSize;
    if (CopySize > FreeSpace)
      CopySize = FreeSpace;

    /* See if we can copy this contiguously before wrapping around */
    FreeSpace = BufferEndPtr - EnQPtr;
    if (CopySize > FreeSpace)
      CopySize = FreeSpace;

    if (CopySize == 0)
      return ReqBytes - BytesRemaining;

    CopyMem (EnQPtr, SrcDataBuffer, CopySize);

    BytesRemaining -= CopySize;
    SrcDataBuffer += CopySize;
    ActiveDataSize += CopySize;
    EnQPtr += CopySize;

    /* Check for wrap around */
    if (EnQPtr >= BufferEndPtr)
      EnQPtr = SerialPortBuffer;
  }

  return ReqBytes - BytesRemaining;
}

#if 0
__WEAK UINTN SerialPortLock (VOID)
{  
  UINTN  State = 0;
  
  State = ArmGetInterruptState();
  ArmDisableInterrupts();  
  return State;
}

__WEAK VOID SerialPortUnlock (UINTN State)
{
  if (State)
    ArmEnableInterrupts(); 
}

__WEAK VOID Sleep (UINT32 DelayMs)
{
}
#endif

/*******************************************************************************
 *            Public functions
 *******************************************************************************/

/*EFI_STATUS EFIAPI
SerialPortInitialize(void)
{
#if defined ENABLE_AUTO_PLAT
  EFI_STATUS Status;
#endif

  static UINTN SerialInitialized = 0;
  if (SerialInitialized)
    return EFI_SUCCESS;
  ++SerialInitialized;

#if defined ENABLE_AUTO_PLAT
  Status = uart_initialize ();
  if (!uart_is_cable_connected())
  {
    NoPortOut = TRUE;
  }
  return Status;
#else
  return uart_initialize ();
#endif
}*/

/*
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
 // TBD : Enable when API is available 
 // return uart_set_baud_rate ((UINT32)*BaudRate); 
  return RETURN_SUCCESS;
}

/
BOOLEAN EFIAPI
SerialPortPoll(void)
{
  return uart_poll ();
}

UINTN EFIAPI
SerialPortRead(OUT UINT8 *Buffer, IN UINTN ReqBytes)
{
  return uart_read (Buffer, ReqBytes);
}*/

/*
 *  This is going through additional layer to control the output options
 *  Here the buffering and port output control options are available
 * */
UINTN
SerialBufferedWrite (UINT8* Buffer, UINTN NumberOfBytes)
{
  return SerialPortWrite (Buffer, NumberOfBytes);
}

/*UINTN
SerialPortWrite (UINT8* Buffer, UINTN Bytes)
{
  return SerialBufferedWrite (Buffer, Bytes);
}*/

/* Control Port output and buffering options
 *
 * Refer to header file for Arg and Param values documentation
 */
UINTN
SerialPortControl (IN UINTN Arg, IN UINTN Param)
{
  UINTN  State = 0;
 
  State = SerialPortLock ();
 
  switch (Arg)
  {
  case SIO_CONTROL_BUFFERING:
   /* Ignore, now always buffered */
    break;

  case SIO_CONTROL_PORTOUT:
    /* 
     * Param : TRUE => Enable data output to UART
     *         FALSE => Disable data output to UART
     * */
    NoPortOut = (Param == 0);
    break;

  case SIO_CONTROL_SYNCHRONOUS_IO:
    SyncIOEnable = (Param != 0);
    break;

  case SIO_CONTROL_POWER:
      UartPowered = (Param != 0);
      if (Param == 0)
      {
         NoPortOut = TRUE;
         uart_power_off ();
      }
      else
         uart_power_on ();
    break;
    
  case SIO_CONTROL_SET_BAUD_RATE:
    uart_set_baud_rate(((UartBaudRateCb*)Param)->BaudRate, ((UartBaudRateCb*)Param)->callback_fn);
    break;

  default:
    break;
  }
  
  SerialPortUnlock (State);

  return 0;
}
/*
VOID
EnableSynchronousSerialPortIO (VOID)
{
  SerialPortControl (SIO_CONTROL_SYNCHRONOUS_IO, TRUE);
}*/

SioPortLibType SioLib =
{
  .LibVersion = CURRENT_SIO_LIB_VERSION,
  .Read = SerialPortRead,
  .Write = NULL,
  .Drain = SerialPortDrain,
  .Flush = SerialPortFlush,
  .Poll = SerialPortPoll,
  .Control = SerialPortControl,
  .SetAttributes = SerialPortSetAttributes,
};

EFI_STATUS
SerialBufferInit( IN  UINT8*   SerialBuffer  OPTIONAL,
                  IN  UINT32   BufferSize    OPTIONAL)
{
  if ((SerialBuffer == NULL) || (BufferSize == 0) || 
      (((UINTN)SerialBuffer + BufferSize) <= (UINTN)SerialBuffer))
  {
    return EFI_INVALID_PARAMETER;
  }

  SerialPortBuffer = SerialBuffer;
  PortBufferSize = BufferSize;
  EnQPtr = DeQPtr = SerialPortBuffer;
  ActiveDataSize = 0;
  ZeroMem (SerialPortBuffer, PortBufferSize);

  boot_shared_imem_cookie_ptr = (struct boot_shared_imem_cookie_type *)((UINT64)PcdGet32(PcdIMemCookiesBase));
  if(boot_shared_imem_cookie_ptr == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
    
  /* Prepend Boot Logs To Uefi Uart(Serial) Buffer if they exist*/
  EnqueueData ((UINT8*)boot_shared_imem_cookie_ptr->boot_log_addr, boot_shared_imem_cookie_ptr->boot_log_size);

  /* No need to send over Previous boot loader data to UART now */
  ActiveDataSize = 0;
  DeQPtr = EnQPtr;

  return EFI_SUCCESS;
}

EFI_STATUS
SerialPortShLibInit (VOID)
{
  ShLibLoaderType *ShLibLoader;
  EFI_STATUS  LoadStatus;

  ShLibLoader = GetShLibLoader();
  if (ShLibLoader == NULL)
    return EFI_NOT_FOUND;

  if ((SerialPortBuffer == NULL) || (PortBufferSize == 0))
    return EFI_NOT_READY;

  LoadStatus = ShLibLoader->InstallLib (SIO_PORT_LIB_NAME, SioLib.LibVersion, (VOID*)&SioLib);

  SioLib.Write = SerialBufferedWrite;
 
  if(SyncIOEnable == TRUE) {
    DEBUG((EFI_D_ERROR, "Sync IO     : Enabled [WARNING]\n")); 
  }

  if (LoadStatus == LIB_SUCCESS)
    return EFI_SUCCESS;
  else
    return EFI_NOT_FOUND;
}

/* Just good faith attempt to send as much data can be dequeued and sent over UART
 * Doesn't block, just queues and returns */
UINTN
SerialPortDrain (VOID)
{
  UINTN Bytes = 0;
  UINTN  State = 0;

  State = SerialPortLock ();
  
  if (NoPortOut == FALSE)
    Bytes = DequeueSendBufferedData ();
  
  SerialPortUnlock (State);
  
  return Bytes;
}

/* Flushes all the remaining data in Queue over to UART
 * This is blocking call, doesn't return until queue is empty
 * If NO port out flag is set it returns as if this API was
 * not called */
/*UINTN
SerialPortFlush (VOID)
{
  UINTN BytesSent = 0, TotalBytes = 0;
  UINTN  State = 0;

  State = SerialPortLock ();

  if (NoPortOut == FALSE)
  {
    do {
      BytesSent = DequeueSendBufferedData ();
      TotalBytes += BytesSent;
    }while (BytesSent);
  }

  SerialPortUnlock (State);

  return TotalBytes;
}*/

/* Locks Serial buffer reinitialization */
VOID SerialBufferReInitLock()
{  
   ReInitDone = TRUE; 
}


/* Reinitializes serial buffer to dynamically allocated pool */
EFI_STATUS
SerialBufferReInit( IN  UINT8*   SerialBuffer  OPTIONAL,
                  IN  UINT32   BufferSize    OPTIONAL)
{
  UINT8 *OldSerialBufferPtr, *OldEnQPtr, *OldDeQPtr, *OldBufferEndPtr;
  UINT32 OldActiveDataSize; 

  if(ReInitDone)
  {
    return EFI_UNSUPPORTED;
  }
  
    /* Lock SerialBuffer reinitialization */
  ReInitDone = TRUE; 

  if ((SerialBuffer == NULL) || (BufferSize == 0) || 
  (((UINTN)SerialBuffer + BufferSize) <= (UINTN)SerialBuffer))
  {
    return EFI_INVALID_PARAMETER;
  }
      
  /* Save previous buffer information */
  OldSerialBufferPtr = SerialPortBuffer; 
  OldEnQPtr = EnQPtr;
  OldDeQPtr = DeQPtr;
  OldActiveDataSize = ActiveDataSize;
  OldBufferEndPtr = SerialPortBuffer + PortBufferSize;

  /* Assign new Allocated buffer as Serial Buffer */
  SerialPortBuffer = SerialBuffer;
  EnQPtr = DeQPtr = SerialPortBuffer;
  ActiveDataSize = 0;
  PortBufferSize = BufferSize;
      
   /*
  *  Scenario 1:       Start        DeQ       EnQ        End
  *
  *  Scenario 2:       Start           EnQ        DeQ    End
  * */

  /* if wraparound : Scenario 2 */
  if (OldEnQPtr < OldDeQPtr)
  {        
    EnqueueData(OldDeQPtr, OldBufferEndPtr - OldDeQPtr);
    EnqueueData(OldSerialBufferPtr, OldEnQPtr - OldSerialBufferPtr);
    /* Adjust EnQPtr and DeqPtr to point to new Buffer location */
    DeQPtr = SerialPortBuffer ;
    EnQPtr = SerialPortBuffer + OldActiveDataSize;
  }
  else /* Scenario 1 */
  {
    /* Copy over Boot Logs to new allocated Uefi Uart(Serial) Buffer */
    EnqueueData (OldSerialBufferPtr, OldEnQPtr - OldSerialBufferPtr);
    /* Adjust EnQPtr and DeqPtr to point to new Buffer location */
    DeQPtr = ((OldDeQPtr - OldSerialBufferPtr) + SerialPortBuffer) ;
    EnQPtr = ((OldEnQPtr - OldSerialBufferPtr) + SerialPortBuffer);
  }
  /* Retain number of active bytes to be sent over to UART. Now from new allocated buffer */ 
  ActiveDataSize = OldActiveDataSize;         
     
  return EFI_SUCCESS;
}

// I know it's bad
RETURN_STATUS
EFIAPI
SerialPortInitialize(VOID)
{
  UINTN InterruptState = 0;

  // Prevent dup initialization
  if (m_Initialized)
    return RETURN_SUCCESS;

  // Interrupt Disable
  InterruptState = ArmGetInterruptState();
  ArmDisableInterrupts();

  // Reset console
  FbConReset();

  // Set flag
  m_Initialized = TRUE;

  if (InterruptState)
    ArmEnableInterrupts();
  return RETURN_SUCCESS;
}

void ResetFb(void)
{
  // Clear current screen.
  char *Pixels  = (void *)0x9C000000;
  UINTN BgColor = FB_BGRA8888_BLACK;

  // Set to black color.
  for (UINTN i = 0; i < gWidth; i++) {
    for (UINTN j = 0; j < gHeight; j++) {
      BgColor = FB_BGRA8888_BLACK;
      // Set pixel bit
      for (UINTN p = 0; p < (gBpp / 8); p++) {
        *Pixels = (unsigned char)BgColor;
        BgColor = BgColor >> 8;
        Pixels++;
      }
    }
  }
}

void FbConReset(void)
{
  // Reset position.
  m_Position.x = 0;
  m_Position.y = 0;

  // Calc max position.
  m_MaxPosition.x = gWidth / (FONT_WIDTH + 1);
  m_MaxPosition.y = (gHeight - 1) / FONT_HEIGHT;

  // Reset color.
  m_Color.Foreground = FB_BGRA8888_WHITE;
  m_Color.Background = FB_BGRA8888_BLACK;
}

void FbConPutCharWithFactor(char c, int type, unsigned scale_factor)
{
  char *Pixels;

  if (!m_Initialized)
    return;

paint:

  if ((unsigned char)c > 127)
    return;

  if ((unsigned char)c < 32) {
    if (c == '\n') {
      goto newline;
    }
    else if (c == '\r') {
      m_Position.x = 0;
      return;
    }
    else {
      return;
    }
  }

  // Save some space
  if (m_Position.x == 0 && (unsigned char)c == ' ' &&
      type != FBCON_SUBTITLE_MSG && type != FBCON_TITLE_MSG)
    return;

  BOOLEAN intstate = ArmGetInterruptState();
  ArmDisableInterrupts();

  Pixels = (void *)0x9C000000;
  Pixels += m_Position.y * ((gBpp / 8) * FONT_HEIGHT * gWidth);
  Pixels += m_Position.x * scale_factor * ((gBpp / 8) * (FONT_WIDTH + 1));

  FbConDrawglyph(
      Pixels, (UINT32)gWidth, (gBpp / 8), (UINTN *)(font5x12 + (c - 32) * 2), scale_factor);

  m_Position.x++;

  if (m_Position.x >= (int)(m_MaxPosition.x / scale_factor))
    goto newline;

  if (intstate)
    ArmEnableInterrupts();
  return;

newline:
  MicroSecondDelay( delay ); 
  m_Position.y += scale_factor;
  m_Position.x = 0;
  if (m_Position.y >= m_MaxPosition.y - scale_factor) {
    FbConFlush();
    m_Position.y = 0;

    if (intstate)
      ArmEnableInterrupts();
    goto paint;
  }
  else {
    Pixels = (void *)0x9C000000;
    Pixels += m_Position.y * ((gBpp / 8) * FONT_HEIGHT * gWidth);
    ZeroMem(Pixels, ((gBpp / 8) * FONT_HEIGHT * gWidth) * scale_factor);
    FbConFlush();
    if (intstate)
      ArmEnableInterrupts();
  }
}

void FbConDrawglyph(
    char *pixels, unsigned stride, unsigned bpp, UINTN *glyph,
    unsigned scale_factor)
{
  char *       bg_pixels = pixels;
  unsigned     x, y, i, j, k;
  UINTN       data, temp;
  unsigned int fg_color = m_Color.Foreground;
  unsigned int bg_color = m_Color.Background;
  stride -= FONT_WIDTH * scale_factor;

  for (y = 0; y < FONT_HEIGHT / 2; ++y) {
    for (i = 0; i < scale_factor; i++) {
      for (x = 0; x < FONT_WIDTH; ++x) {
        for (j = 0; j < scale_factor; j++) {
          bg_color = m_Color.Background;
          for (k = 0; k < bpp; k++) {
            *bg_pixels = (unsigned char)bg_color;
            bg_color   = bg_color >> 8;
            bg_pixels++;
          }
        }
      }
      bg_pixels += (stride * bpp);
    }
  }

  for (y = 0; y < FONT_HEIGHT / 2; ++y) {
    for (i = 0; i < scale_factor; i++) {
      for (x = 0; x < FONT_WIDTH; ++x) {
        for (j = 0; j < scale_factor; j++) {
          bg_color = m_Color.Background;
          for (k = 0; k < bpp; k++) {
            *bg_pixels = (unsigned char)bg_color;
            bg_color   = bg_color >> 8;
            bg_pixels++;
          }
        }
      }
      bg_pixels += (stride * bpp);
    }
  }

  data = glyph[0];
  for (y = 0; y < FONT_HEIGHT / 2; ++y) {
    temp = data;
    for (i = 0; i < scale_factor; i++) {
      data = temp;
      for (x = 0; x < FONT_WIDTH; ++x) {
        if (data & 1) {
          for (j = 0; j < scale_factor; j++) {
            fg_color = m_Color.Foreground;
            for (k = 0; k < bpp; k++) {
              *pixels  = (unsigned char)fg_color;
              fg_color = fg_color >> 8;
              pixels++;
            }
          }
        }
        else {
          for (j = 0; j < scale_factor; j++) {
            pixels = pixels + bpp;
          }
        }
        data >>= 1;
      }
      pixels += (stride * bpp);
    }
  }

  data = glyph[1];
  for (y = 0; y < FONT_HEIGHT / 2; ++y) {
    temp = data;
    for (i = 0; i < scale_factor; i++) {
      data = temp;
      for (x = 0; x < FONT_WIDTH; ++x) {
        if (data & 1) {
          for (j = 0; j < scale_factor; j++) {
            fg_color = m_Color.Foreground;
            for (k = 0; k < bpp; k++) {
              *pixels  = (unsigned char)fg_color;
              fg_color = fg_color >> 8;
              pixels++;
            }
          }
        }
        else {
          for (j = 0; j < scale_factor; j++) {
            pixels = pixels + bpp;
          }
        }
        data >>= 1;
      }
      pixels += (stride * bpp);
    }
  }
}

/* TODO: Take stride into account */
void FbConScrollUp(void)
{
  unsigned short *dst   = (void *)0x9C000000;
  unsigned short *src   = dst + (gWidth * FONT_HEIGHT);
  unsigned        count = gWidth * (gHeight - FONT_HEIGHT);

  while (count--) {
    *dst++ = *src++;
  }

  count = gWidth * FONT_HEIGHT;
  while (count--) {
    *dst++ = m_Color.Background;
  }

  FbConFlush();
}

void FbConFlush(void)
{
  unsigned total_x, total_y;
  unsigned bytes_per_bpp;

  total_x       = gWidth;
  total_y       = gHeight;
  bytes_per_bpp = (gBpp / 8);

  WriteBackInvalidateDataCacheRange(
      (void *)0x9C000000,
      (total_x * total_y * bytes_per_bpp));
}

UINTN
EFIAPI
SerialPortWrite(IN UINT8 *Buffer, IN UINTN NumberOfBytes)
{
  UINT8 *CONST Final          = &Buffer[NumberOfBytes];
  UINTN        InterruptState = ArmGetInterruptState();
  ArmDisableInterrupts();

  while (Buffer < Final) {
    FbConPutCharWithFactor(*Buffer++, FBCON_COMMON_MSG, SCALE_FACTOR);
  }

  if (InterruptState)
    ArmEnableInterrupts();
  return NumberOfBytes;
}

UINTN
EFIAPI
SerialPortWriteCritical(IN UINT8 *Buffer, IN UINTN NumberOfBytes)
{
  UINT8 *CONST Final             = &Buffer[NumberOfBytes];
  UINTN        CurrentForeground = m_Color.Foreground;
  UINTN        InterruptState    = ArmGetInterruptState();

  ArmDisableInterrupts();
  m_Color.Foreground = FB_BGRA8888_YELLOW;

  while (Buffer < Final) {
    FbConPutCharWithFactor(*Buffer++, FBCON_COMMON_MSG, SCALE_FACTOR);
  }

  m_Color.Foreground = CurrentForeground;

  if (InterruptState)
    ArmEnableInterrupts();
  return NumberOfBytes;
}

UINTN
EFIAPI
SerialPortRead(OUT UINT8 *Buffer, IN UINTN NumberOfBytes) { return 0; }

BOOLEAN
EFIAPI
SerialPortPoll(VOID) { return FALSE; }

RETURN_STATUS
EFIAPI
SerialPortSetControl(IN UINT32 Control) { return RETURN_UNSUPPORTED; }

RETURN_STATUS
EFIAPI
SerialPortGetControl(OUT UINT32 *Control) { return RETURN_UNSUPPORTED; }

RETURN_STATUS
EFIAPI
SerialPortSetAttributes(
    IN OUT UINT64 *BaudRate, IN OUT UINT32 *ReceiveFifoDepth,
    IN OUT UINT32 *Timeout, IN OUT EFI_PARITY_TYPE *Parity,
    IN OUT UINT8 *DataBits, IN OUT EFI_STOP_BITS_TYPE *StopBits)
{
  return RETURN_UNSUPPORTED;
}

UINTN SerialPortFlush(VOID) { return 0; }

VOID EnableSynchronousSerialPortIO(VOID)
{
  // Already synchronous
}