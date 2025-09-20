/*
===========================================================================

FILE:   SpiDeviceOsSvc.c

DESCRIPTION:
    This file contains the implementation for the QUP OS Services 
    Interface: IQupSpiDevice 

===========================================================================

        Edit History


When     Who    What, where, why
-------- ---    -----------------------------------------------------------
09/23/14 sg     XBL changes.
04/20/12 ddk    Added updates to enable logging.
04/19/12 ddk    Added changes for avoiding symbol match with I2C library.
03/27/12 DK     Added Physical memory request functions.
09/26/11 LK     Created

===========================================================================
           Copyright c 2011-2014 Qualcomm Technologies Incorporated.
                              All Rights Reserved.
                           Qualcomm Proprietary/GTDR

===========================================================================
*/

#include "SpiDeviceOsSvc.h"
#include "DALStdDef.h"
#include "DALDeviceId.h"
#include "DALSys.h"
#include "SpiLog.h"

typedef enum SpiDeviceOs_HandleType
{
   SpiDeviceOs_EventHandleType,
   SpiDeviceOs_CritSecHandleType,
} SpiDeviceOs_HandleType;

typedef struct SpiDeviceOs_EventHandle
{
   SpiDeviceOs_HandleType hType;
   DALSYSEventHandle hQupEvents[2];
   DALSYSEventObj aQupEventObj[2];
} SpiDeviceOs_EventHandle;

typedef struct SpiDeviceOs_SyncHandle
{
   SpiDeviceOs_HandleType hType;
   DALSYSSyncHandle hDeviceSynchronization;
   DALSYS_SYNC_OBJECT(DeviceSyncObject);
} SpiDeviceOs_SyncHandle;

typedef struct SpiDeviceOs_Handle
{
   SpiDeviceOs_HandleType hType;
} SpiDeviceOs_Handle;

typedef struct SpiDeviceOs_IrqHandle
{
   ISR_HOOK IsrHook;
   IST_HOOK IstHook;
   void *pIsrData;
   void *pIstData;
   uint32 uInterruptId;
   uint32 uIrqTriggerCfg;
} SpiDeviceOs_IrqHandle;

uint32 IBUSOS_CreateEvent(IBUSOS_EVENT_HANDLE *phEvt)
{
   uint32 res;
   SpiDeviceOs_EventHandle *pEvt;
   DALResult dalRes;
   enum IBUSOS_CreatEventState
   {
      IBUSOS_CreatEventState_Reset,
      IBUSOS_CreatEventState_Malloced,
      IBUSOS_CreatEventState_1stEvt,
      IBUSOS_CreatEventState_2ndEvt,
   }crEvtState;

   crEvtState = IBUSOS_CreatEventState_Reset;
   do
   {
      res = IBUSOS_Malloc((void **)&pEvt, sizeof(SpiDeviceOs_EventHandle));
      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "IBUSOS_CreateEvent: Malloc failure\n");
         break;
      }
      IBUSOS_Memset((void *)pEvt, 0, sizeof(SpiDeviceOs_EventHandle));

      crEvtState = IBUSOS_CreatEventState_Malloced;
      /* Create a timeout object used to timeout operations.               */
      dalRes = DALSYS_EventCreate(DALSYS_EVENT_ATTR_TIMEOUT_EVENT,
                                  &pEvt->hQupEvents[0],
                                  &pEvt->aQupEventObj[0]);
      if (dalRes != DAL_SUCCESS)
      {
         res = IBUSOS_ERROR_EVT_CREATE_FAILED;
         SPIDEVICE_LOG(ERROR, "IBUSOS_CreateEvent: Dal failure err 0x%x\n",
                       (unsigned int)dalRes);
         break;
      }
      crEvtState = IBUSOS_CreatEventState_1stEvt;

      /* Create a wait object on which the client will wait.              */
      dalRes = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT,
                                  &pEvt->hQupEvents[1],
                                  &pEvt->aQupEventObj[1]);
      if (dalRes != DAL_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "IBUSOS_CreateEvent: Dal failure err 0x%x\n",
                       (unsigned int)dalRes);
         res = IBUSOS_ERROR_EVT_CREATE_FAILED;
         break;
      }
      pEvt->hType = SpiDeviceOs_EventHandleType;
      crEvtState = IBUSOS_CreatEventState_2ndEvt;
   }
   while (0);

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      switch (crEvtState)
      {
      case IBUSOS_CreatEventState_1stEvt:
         DALSYS_DestroyObject(&pEvt->aQupEventObj[0]);
      case IBUSOS_CreatEventState_Malloced:
         IBUSOS_Free(pEvt);
         break;
         // unreachable added to avoid compiler warnings
      case IBUSOS_CreatEventState_Reset:
      case IBUSOS_CreatEventState_2ndEvt:
         break;
      };
   } else
   {
      *phEvt = (IBUSOS_EVENT_HANDLE)pEvt;
   }

   return res;
}

uint32 IBUSOS_DestroyEvent(SpiDeviceOs_EventHandle *pEvt)
{
   return IBUSOS_Free(pEvt);
}

uint32 IBUSOS_SetEvent(IBUSOS_EVENT_HANDLE hEvent)
{
   DALResult dalRes;
   SpiDeviceOs_EventHandle *pEvt = (SpiDeviceOs_EventHandle *)hEvent;

   dalRes = DALSYS_EventCtrl(pEvt->hQupEvents[1], DALSYS_EVENT_CTRL_TRIGGER);
   if (dalRes != DAL_SUCCESS)
   {
      return IBUSOS_ERROR_EVT_CTRL_FAILED;
   }
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_ClearEvent(IBUSOS_EVENT_HANDLE hEvt)
{
   DALResult dalRes;
   uint32 res = SPIDEVICE_RES_SUCCESS;
   SpiDeviceOs_EventHandle *pEvt = (SpiDeviceOs_EventHandle *)hEvt;

   dalRes = DALSYS_EventCtrl(pEvt->hQupEvents[0], DALSYS_EVENT_CTRL_RESET);
   if (dalRes != DAL_SUCCESS)
   {
      res = IBUSOS_ERROR_EVT_CLEAR_FAILED;
   }
   dalRes = DALSYS_EventCtrl(pEvt->hQupEvents[1], DALSYS_EVENT_CTRL_RESET);
   if (dalRes != DAL_SUCCESS)
   {
      res = IBUSOS_ERROR_EVT_CLEAR_FAILED;
   }

   return res;
}

uint32 IBUSOS_WaitEvent(IBUSOS_EVENT_HANDLE hEvent, uint32 dwMilliseconds)
{
   DALResult dalRes;
   SpiDeviceOs_EventHandle *pEvt = (SpiDeviceOs_EventHandle *)hEvent;

   dalRes = DALSYS_EventWait(pEvt->hQupEvents[1]);

   if ((DAL_SUCCESS != dalRes))
   {
      return IBUSOS_ERROR_EVT_WAIT_FAILED;
   }

   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_DestroyCriticalSection(SpiDeviceOs_SyncHandle *pCritSec)
{
   uint32 res = SPIDEVICE_RES_SUCCESS;
   DALResult dalRes;

   dalRes = DALSYS_DestroyObject(pCritSec->hDeviceSynchronization);
   if (dalRes != DAL_SUCCESS)
   {
      res = IBUSOS_ERROR_SYNC_DESTROY_FAILED;
   }
   if (SPIDEVICE_RES_SUCCESS == res)
   {
      res = IBUSOS_Free(pCritSec);
   } else
   {
      IBUSOS_Free(pCritSec);
   }

   return res;
}

uint32 IBUSOS_CloseHandle(IBUSOS_EVENT_HANDLE hEvt)
{
   SpiDeviceOs_Handle *hOsEvt = (SpiDeviceOs_Handle *)hEvt;

   if (SpiDeviceOs_EventHandleType == hOsEvt->hType)
   {
      return IBUSOS_DestroyEvent((SpiDeviceOs_EventHandle *)hEvt);
   } else if (SpiDeviceOs_CritSecHandleType == hOsEvt->hType)
   {
      return IBUSOS_DestroyCriticalSection((SpiDeviceOs_SyncHandle *)hEvt);
   } else
   {
      return IBUSOS_ERROR_INVALID_HANDLE_TYPE;
   }
}

uint32 IBUSOS_Malloc(void **pBuffer, uint32 size)
{
   DALResult dalRes;
   dalRes = DALSYS_Malloc(size, pBuffer);
   if (DAL_SUCCESS != dalRes)
   {
      return IBUSOS_ERROR_MEM_MALLOC_FAILED;
   }
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_Memset(void *buffer, int32 val, uint32 size)
{
   DALSYS_memset(buffer, val, size);
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_Free(void *pBuffer)
{
   if (DAL_SUCCESS != DALSYS_Free(pBuffer))
   {
      return IBUSOS_ERROR_MEM_FREE_FAILED;
   }
   return SPIDEVICE_RES_SUCCESS;
}

uint32
IBUSOS_RegisterISTHooks(void *QupHandle, ISR_HOOK IsrHook, void *pIsrData, IST_HOOK IstHook, void *pIstData)
{
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_UnRegisterISTHooks(void *QupHandle)
{
   return SPIDEVICE_RES_SUCCESS;
}

void IBUSOS_BusyWait(uint32 uTimeMicrosec)
{
   DALSYS_BusyWait(uTimeMicrosec);
}

uint32 IBUSOS_PhysMemAlloc (uint32 uLen, void **phMem, void **puVirtAddr, void **puPhysAddr)
{
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_PhysMemFree(void *phMem)
{
   return SPIDEVICE_RES_SUCCESS;
}

void IBUSOS_Memcpy(void *dst, void *src, uint32 size)
{
  //DALSYS_memcpy is deprecated and superseded by DALSYS_memscpy
   DALSYS_memscpy(dst, size, src, size);
}
