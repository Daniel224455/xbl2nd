#ifndef _SPIDEVICEOSSVC_H_
#define _SPIDEVICEOSSVC_H_
/*==================================================================================================

FILE: SpiDeviceOsSvc.h

DESCRIPTION:
   This file defines an API for providing OS services to the SPI common device layer.

====================================================================================================

   Edit History

When      Who  What, where, why
--------  ---  -----------------------------------------------------------
07/14/12  ag   ISR and IST prototype changes.
04/07/12  ddk  Added name change to avoid conflicts with I2c library.
04/07/12  ddk  Added mem free API.
03/27/12  ddk  Added SPI BAM mode updates.
09/26/11  LK   Created

====================================================================================================
                     Copyright (c) 2011-2014 Qualcomm Technologies Incorporated
                                        All Rights Reserved
                                     Qualcomm Proprietary/GTDR
==================================================================================================*/

#include "SpiDeviceTypes.h"
#include "SpiDeviceError.h"

/*==================================================================================================
                                              TYPEDEFS
==================================================================================================*/

typedef enum
{
   IBUSOS_ERROR_BASE = SPIDEVICE_RES_ERROR_CLS_OS,
   IBUSOS_ERROR_TIMEOUT,
   IBUSOS_ERROR_EVT_CREATE_FAILED,
   IBUSOS_ERROR_EVT_DESTROY_FAILED,
   IBUSOS_ERROR_MEM_FREE_FAILED,
   IBUSOS_ERROR_MEM_MALLOC_FAILED,
   IBUSOS_ERROR_EVT_CLEAR_FAILED,
   IBUSOS_ERROR_EVT_CTRL_FAILED,
   IBUSOS_ERROR_EVT_WAIT_FAILED,
   IBUSOS_ERROR_SYNC_CREATE_FAILED,
   IBUSOS_ERROR_SYNC_DESTROY_FAILED,
   IBUSOS_ERROR_INVALID_HANDLE_TYPE,
   IBUSOS_ERROR_NULL_PTR,
   IBUSOS_ERROR_REGISTER_IST_HOOKS,
} IBUSOS_ERROR;

typedef void* IBUSOS_EVENT_HANDLE;

typedef uint32 (*ISR_HOOK)(void *);
typedef uint32 (*IST_HOOK)(void *);

/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/

/*  @brief Creates an event object and sets handle pointer.
 *
 *  @param[out] phEvent  pointer to event handle
 *  @return              error status
 */
uint32 IBUSOS_CreateEvent(IBUSOS_EVENT_HANDLE *phEvent);

/*  @brief Signals the event object pointed to by the handle.
 *
 *  @param[in] hEvent  event handle
 *  @return            error status
 */
uint32 IBUSOS_SetEvent(IBUSOS_EVENT_HANDLE hEvent);

/*  @brief Clears outstanding signals on the event object.
 *
 *  @param[in] hEvent  event handle
 *  @return            error status
 */
uint32 IBUSOS_ClearEvent(IBUSOS_EVENT_HANDLE hEvent);

/*  @brief Wait with a timeout on the event object.
 *
 *  @param[in] hEvent          event handle
 *  @param[in] dwMilliseconds  event wait timeout
 *  @return                    error status
 */
uint32 IBUSOS_WaitEvent(IBUSOS_EVENT_HANDLE hEvent,
                        uint32              dwMilliseconds);

/*  @brief Closes a handle destroying the object associated with it.
 *
 *  @param[in] hEvent  event handle
 *  @return            error status
 */
uint32 IBUSOS_CloseHandle(IBUSOS_EVENT_HANDLE hEvent);

/*  @brief Allocates a memory buffer of the given size and sets the pointer to it.
 *
 *  @param[out] ppBuffer  pointer to pointer to allocated memory
 *  @param[in]  size      size of the memory to be allocated
 *  @return               error status
 */
uint32 IBUSOS_Malloc(void **ppBuffer,
                     uint32 size);

/*  @brief Frees memory allocated by IBUSOS_Malloc.
 *
 *  @param[in] pBuffer  pointer to allocated memory
 *  @return             error status
 */
uint32 IBUSOS_Free(void *pBuffer);

/*  @brief Memcpy operation.
 *
 *  @param[in] dst   destination buffer
 *  @param[in] src   source buffer
 *  @param[in] size  number of bytes to copy
 *  @return          void
 */
void IBUSOS_Memcpy(void   *dst,
                   void   *src,
                   uint32 size);

/*  @brief Memset operation.
 *
 *  @param[in] buffer  destination buffer
 *  @param[in] val     value to write
 *  @param[in] size    number of bytes to write
 *  @return            error status
 */
uint32 IBUSOS_Memset(void   *buffer,
                     int32   val,
                     uint32  size);

/*  @brief Registers IST/ISR hooks for the indicated device.
 *
 *  @param[in] QupHandle  QUP handle
 *  @param[in] IsrHook    ISR hook
 *  @param[in] pIsrData   Pointer to data handed back when ISR is called
 *  @param[in] IstHook    IST hook, thread handling the request
 *  @param[in] pIstData   Pointer to data handed back when IST is called
 *  @return               error status
 */
uint32 IBUSOS_RegisterISTHooks(void     *QupHandle,
                               ISR_HOOK  IsrHook,
                               void     *pIsrData,
                               IST_HOOK  IstHook,
                               void     *pIstData);

/*  @brief Unregisters IST/ISR hooks for the indicated device.
 *
 *  @param[in] QupHandle  QUP handle
 *  @return               error status
 */
uint32 IBUSOS_UnRegisterISTHooks(void *QupHandle);

/*  @brief Busy waits before returning.
 *
 *  @param[in] uTimeMicrosec  time to wait (in microseconds)
 *  @return                   void
 */
void IBUSOS_BusyWait(uint32 uTimeMicrosec);

/*  @brief Allocates physical memory.
 *
 *  @param[in]  uLen        size of memory to allocate
 *  @param[out] phMem       memory handle (used to free memory)
 *  @param[out] ppVirtAddr  virtual address of allocated memory
 *  @param[out] ppPhysAddr  physical address of allocated memory
 *  @return                 error status
 */
uint32 IBUSOS_PhysMemAlloc(uint32   uLen,
                           void   **phMem,
                           void   **ppVirtAddr,
                           void   **ppPhysAddr);

/*  @brief Free up previously allocated physical memory.
 *
 *  @param[in] hMem  memory handle (obtained from IBUSOS_PhysMemAlloc)
 *  @return          error status
 */
uint32 IBUSOS_PhysMemFree(void *hMem);

#endif  // _SPIDEVICEOSSVC_H_
