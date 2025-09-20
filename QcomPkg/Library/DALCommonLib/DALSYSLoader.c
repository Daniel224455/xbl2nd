/** @file DALSYSLoader.c

  DALSYS library for XBL loader

  Copyright (c) 2010-2011,2014, 2017-2018 Qualcomm Technologies, Inc. All rights reserved.

**/

#include "busywait.h"
#include "DALStdDef.h"
#include "DALStdErr.h"
#include "DALSysCmn.h"
#include "DALSys.h"
#include "DALReg.h"
#include "DALDeviceId.h"
#include "DALGlbCtxt.h"
#include "smem.h"
#include <Uefi.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/TimerLib.h>
#include "string.h"
#include "boot_cache.h"
#include "devcfg.h"

#ifdef DEVCFG_STRICT_XML
DALProps * module_info;
DALREG_DriverInfoList * gDALModDriverInfo;
void DAL_setModInfo(DALProps * mod_info_addr, DALREG_DriverInfoList * global_drivers)
{
   module_info = mod_info_addr;
   gDALModDriverInfo = global_drivers;
}
#else
extern DALProps gDALModStringDeviceInfoList;
extern DALREG_DriverInfoList gDALModDriverInfoList;
#endif

/*-------------------------------------------------------------------
 Constant Definitions 
 ------------------------------------------------------------------*/
#define ALIGN_MASK      0x00000001f
#define ALIGN_UP( s )   (((DALSYSMemAddr)(s) + ALIGN_MASK) & ~(ALIGN_MASK))
#define ADD_SIZE_FOR_PADDING(s)  ((s) + ALIGN_MASK) 
#define PID             0xB000000D

/*-------------------------------------------------------------------
 Structure Definitions 
 ------------------------------------------------------------------*/
typedef struct 
{
   DALSYSMemObjBase BaseObj;
   void *pMemRegion;
}
_DALSYSMemObj;

/*=============================================================================
Forward Declarations
=============================================================================*/
DALResult System_DalSystem_Attach(const char *pszArg, DALDEVICEID DeviceId,
                                  DalDeviceHandle **phDalDevice);
                                  
/*=============================================================================
 Variable Definitions
=============================================================================*/
static DALDEVICEID DalSystem_DeviceId[1] = {DALDEVICEID_SYSTEM};
static int hasInit = 0;

const DALREG_DriverInfo
DALSystem_DriverInfo = { System_DalSystem_Attach,
                         0,
                         DalSystem_DeviceId
                       };

static uint32 updateUncachedMemProps( uint32 memProps )
{
   if((memProps & DALSYS_MEM_PROPS_CACHED_MASK) == DALSYS_MEM_PROPS_UNCACHED)
      memProps = (memProps & ~DALSYS_MEM_PROPS_CACHED_MASK) | DALSYS_MEM_PROPS_CACHED_WB;
   return memProps;
}

static DALProps * DALSYS_GetDAL_Mod(void)
{
#ifdef DEVCFG_STRICT_XML
   return module_info;
#else
   return (DALProps *)&gDALModStringDeviceInfoList;
#endif
}

void
DALSYS_InitMod(DALSYSConfig * pCfg)
{
   // below is used to protect against multiple invocations
   if (!hasInit)
   {
      hasInit = 1;
#ifdef DEVCFG_STRICT_XML
      extern void DevCfg_Init();
      DevCfg_Init();
      extern void devcfgManagerInit();
      devcfgManagerInit();
      DALSYSCMN_RegisterModDriverList(gDALModDriverInfo);
#else
      DALSYSCMN_RegisterModDriverList(&gDALModDriverInfoList);
#endif
   }
}

void DALSYS_DeInitMod(void)
{
  if(hasInit)
  {
#ifdef DEVCFG_STRICT_XML
   DALSYSCMN_DeRegisterModDriverList(gDALModDriverInfo);
#else
   DALSYSCMN_DeRegisterModDriverList(&gDALModDriverInfoList);
#endif
  }
}

DALResult _DALSYSCMN_GetStringDeviceObject(const char *pszDevName, const char *pszArg, DalDeviceHandle **phDevice)
{ 
   const StringDevice *pStringDeviceObj = NULL;
   DALREG_DriverInfo *pDriverInfo = NULL;
#ifdef DEVCFG_STRICT_XML
   if(DAL_SUCCESS == _DAL_StringDeviceObjectLookup(pszDevName, &pStringDeviceObj, module_info)){
#else    
   if(DAL_SUCCESS == _DAL_StringDeviceObjectLookup(pszDevName, &pStringDeviceObj, DALSYS_GetDAL_Mod())){
#endif
      //Get the Property Dir Info Ptr
      pDriverInfo = pStringDeviceObj->pFunctionName;
      return pDriverInfo->pfnDALNewFunc(pszDevName,0,phDevice);
   } 
   return DAL_ERROR;
}

static uint32
DJB2_hash(const char *pszDevName)
{
   uint32 dwHash = 5381;
   while (*pszDevName)
      dwHash = ((dwHash << 5) + dwHash) + (int)*pszDevName++; /* dwHash * 33 + c */
   return dwHash;
}

DALResult
_DAL_StringDeviceObjectLookup(const char *pszDevName, const StringDevice **ppStringDeviceObj, DALProps *pDALPtr)
{
   uint32 dwDriverHash;
   int dwIndex = 0;
   int dwSize;
   const uint32 *pCollisionsIndexes = NULL;

   if(pDALPtr!= NULL && pDALPtr->pDevices!=NULL && pszDevName!= NULL 
     && dwIndex< pDALPtr->dwDeviceSize && ppStringDeviceObj!=NULL)
   {
      dwDriverHash = DJB2_hash(pszDevName); 
      for(dwIndex=0;dwIndex< pDALPtr->dwDeviceSize ; dwIndex++)
      {
         if(dwDriverHash ==  pDALPtr->pDevices[dwIndex].dwHash)
         {
            break;
         } 
      }
      // Finish the search with right device index. 
      if(dwIndex != pDALPtr->dwDeviceSize)
      {
         // if there is no Collisions.
         if(pDALPtr->pDevices[dwIndex].dwNumCollision == 0)
         {
            *ppStringDeviceObj = &(pDALPtr->pDevices[dwIndex]);
            return DAL_SUCCESS;
         }
         // If there is a Collision, scan the list for right device object.
         pCollisionsIndexes = pDALPtr->pDevices[dwIndex].pdwCollisions;
         dwSize = pDALPtr->pDevices[dwIndex].dwNumCollision;
         for(dwIndex=0;dwIndex<dwSize;dwIndex++)
         {
            // Get the dwIndex, where the str might be
            if(AsciiStrLen(pszDevName) == AsciiStrLen(pDALPtr->pDevices[pCollisionsIndexes[dwIndex]].pszName))
            {
               if(AsciiStrnCmp(pszDevName, pDALPtr->pDevices[pCollisionsIndexes[dwIndex]].pszName, AsciiStrLen(pszDevName))==0)
               {
                  *ppStringDeviceObj = &(pDALPtr->pDevices[pCollisionsIndexes[dwIndex]]);
                  return DAL_SUCCESS;
               }
            }
         }
      }   
   }
   return DAL_ERROR_NOT_FOUND;
}

/*=============================================================================
 Stubbed Function Definitions
=============================================================================*/
DALResult
System_DalSystem_Attach(const char *pszArg, DALDEVICEID DeviceId,
                            DalDeviceHandle **phDalDevice)
{
   *phDalDevice = NULL;
   return DAL_ERROR;
}

DALResult
DALSYS_DestroyObject(DALSYSObjHandle hObj)
{
   return DAL_SUCCESS;
}

DALResult
DALSYS_CopyObject(DALSYSObjHandle hObjOrig, DALSYSObjHandle *phObjCopy )
{
   return DAL_ERROR;
}

DALResult
DALSYS_RegisterWorkLoop(uint32 dwPriority,
                        uint32 dwMaxNumEvents,
                        DALSYSWorkLoopHandle *phWorkLoop,
                        DALSYSWorkLoopObj *pWorkLoopObj)
{
   return DAL_ERROR;
}

DALResult
DALSYS_AddEventToWorkLoop(DALSYSWorkLoopHandle hWorkLoop,
                          DALSYSWorkLoopExecute pfnWorkLoopExecute,
                          void * pArg,
                          DALSYSEventHandle hEvent,
                          DALSYSSyncHandle hSync)
{
   return DAL_ERROR;
}

DALResult
DALSYS_DeleteEventFromWorkLoop(DALSYSWorkLoopHandle hWorkLoop,
                               DALSYSEventHandle hEvent)
{
   return DAL_ERROR;
}

DALResult
DALSYS_EventWait(DALSYSEventHandle hEvent)
{
   return DAL_ERROR;
}

DALResult
DALSYS_EventMultipleWait(DALSYSEventHandle* phEvent, int nEvents,
                         uint32 dwTimeoutUs, uint32 *pdwEventIdx)
{
   return DAL_ERROR;
}

void
DALSYS_SyncEnter(DALSYSSyncHandle hSync)
{
}

void
DALSYS_SyncLeave(DALSYSSyncHandle hSync)
{
}

DALResult
DALSYS_MemDescAddBuf(DALSysMemDescList * pMemDesc, uint32 dwMemDescBufIdx,
                          uint32 dwOffset, uint32 dwSize)
{
   return DAL_ERROR;
}

DALResult
DALSYS_MemDescPrepare(DALSysMemDescList * pMemDesc, uint32 dwBufIdx)
{
   return DAL_ERROR;
}

DALResult
DALSYS_MemDescValidate(DALSysMemDescList * pMemDesc,uint32 dwBufIdx)
{
   return DAL_ERROR;
}

void
DALSYS_InitSystemHandle(DalDeviceHandle *hDalDevice)
{
}

DALResult
DAL_DeviceAttachRemote(const char *pszArg,
                   DALDEVICEID DevId,
                   DALInterfaceVersion ClientVersion,
                   DalDeviceHandle **phDALDevice)
{
   return DAL_ERROR;
}

DALResult
DALSYS_MemRegionMapPhys(DALSYSMemHandle hMem, uint32 dwVirtualBaseOffset,
                        DALSYSMemAddr PhysicalAddr, uint32 dwLen)
{
   return DAL_SUCCESS;
}

// Allocate global context from shared memory driver
EFI_STATUS
_AllocGlbCtxtMem(DALSYSMemAddr *p_ctxt_addr, UINT32 size)
{
   *p_ctxt_addr = (DALSYSMemAddr)smem_alloc(SMEM_DAL_AREA, size);
   if (*p_ctxt_addr)
   {
      return EFI_SUCCESS;
   }
   else
   {
      return EFI_PROTOCOL_ERROR;
   }
}
static void *
_CreateMemSection(uint32 dwLen, uint32 dwAttribs,
                  DALSYSMemAddr *VirtualAddr,DALSYSMemAddr *PhysicalAddr)
{
    void *pMemRegion = NULL;

    //Only support no addr specified in this environemt
    if ( DALSYS_MEM_ADDR_NOT_SPECIFIED  == *PhysicalAddr && 
         DALSYS_MEM_ADDR_NOT_SPECIFIED == *VirtualAddr )
    {
       if(DAL_SUCCESS != DALSYS_Malloc(ADD_SIZE_FOR_PADDING(dwLen),&pMemRegion))
       {             
          pMemRegion = NULL;
       }
       pMemRegion = (void *)ALIGN_UP(pMemRegion);
       *VirtualAddr = (DALSYSMemAddr)pMemRegion;
       *PhysicalAddr = (DALSYSMemAddr)pMemRegion;
    }
    else if(DALSYS_MEM_ADDR_NOT_SPECIFIED  != *PhysicalAddr)
    {
      DEBUG(( EFI_D_ERROR, "Unsupported functionality in XBLLoader\n"));
    }
    return pMemRegion;
}


static
_DALSYSMemObj *_AllocAndInitMemObj(_DALSYSMemObj *pMemObj,
                    uint32 dwLen,
                    uint32 dwAttribs,
                    void * pMemRegion,
                    DALSYSMemAddr VirtualAddr,
                    DALSYSMemAddr PhysicalAddr)
{
    // VirtualAddr and PhysicalAddr should not be NULL
    if(0 == VirtualAddr || 0 == PhysicalAddr)
    {
       DEBUG(( EFI_D_ERROR, "MMU SW Config Error; VirtualAddr:0x%x PhysicalAddr:0x%x\n", 
              VirtualAddr, PhysicalAddr ));
       return NULL;
    }

    if(NULL == pMemObj)
    {
        if(DAL_SUCCESS == DALSYS_Malloc(DALSYS_MEM_OBJ_SIZE_BYTES,
                                         (void **)&pMemObj))
        {
            DALSYSCMN_RESET_ALL_BITS(pMemObj->BaseObj.dwObjInfo);
            DALSYSCMN_SET_DYNAMIC_ALLOC_BIT(pMemObj->BaseObj.dwObjInfo);
        }
        else
        {
           DEBUG(( EFI_D_ERROR, "Insufficient memory for memory object\n" ));
           return NULL;
        }
    }
    else
    {
        DALSYSCMN_RESET_ALL_BITS(pMemObj->BaseObj.dwObjInfo);
    }

    DALSYSCMN_SET_OBJECT_ID(pMemObj->BaseObj.dwObjInfo,DALSYSCMN_MEM_OBJECT_ID);

    // Update mem properties if we are asking for uncached
    // since we only support cached-WB
    dwAttribs = updateUncachedMemProps(dwAttribs);

    DALSYSCMN_SET_ATTRIB(pMemObj->BaseObj.dwObjInfo,dwAttribs);
    DALSYSCMN_RESET_MEM_COPY_BIT(pMemObj->BaseObj.dwObjInfo);

    pMemObj->BaseObj.dwLen             = dwLen;
    pMemObj->BaseObj.hOwnerProc        = PID;
    pMemObj->BaseObj.thisVirtualAddr   = (DALSYSMemAddr)pMemObj;
    pMemObj->BaseObj.OwnerVirtualAddr  = VirtualAddr;
    pMemObj->BaseObj.VirtualAddr       = VirtualAddr;
    pMemObj->BaseObj.PhysicalAddr      = PhysicalAddr;
    pMemObj->pMemRegion                = pMemRegion;
    return pMemObj;
}


DALResult
DALSYS_MemRegionAlloc(uint32 dwAttribs, DALSYSMemAddr VirtualAddr,
                      DALSYSMemAddr PhysicalAddr, uint32 dwLen, DALSYSMemHandle *phMem,
                      DALSYSMemObj *pObj)
{
   void * pMemRegion = NULL;
   _DALSYSMemObj *pMemObj = (_DALSYSMemObj *)pObj;
   *phMem = NULL;

   if (dwAttribs & DALSYS_MEM_PROPS_PHYS_PREALLOC)
   {
      // assign memory region as the virtual address value
      pMemRegion = (void *)VirtualAddr;
   }
   else
   {
      /*----------------------------------------------------------------------
      Create the underlying memsection
      ----------------------------------------------------------------------*/
      pMemRegion = _CreateMemSection(dwLen,dwAttribs,&VirtualAddr,&PhysicalAddr);
   }

   if(NULL == pMemRegion)
      return DAL_ERROR;

   /*----------------------------------------------------------------------
   Alloc Memory for the DALSYS Mem Obj
   ----------------------------------------------------------------------*/
   pMemObj = _AllocAndInitMemObj(pMemObj,dwLen,dwAttribs,pMemRegion,
                                        VirtualAddr,PhysicalAddr);
   if(NULL == pMemObj)
   {
      // do some clean up here in real-world case
      return DAL_ERROR;
   }

   // Return handle
   *phMem = (DALSYSMemHandle)pMemObj;
   return DAL_SUCCESS;
}

void
DALSYS_BusyWait(uint32 pause_time_us)
{
  busywait(pause_time_us);
}


VOID *
EFIAPI
InvalidateDataCacheRange (
  IN      VOID                      *Address,
  IN      UINTN                     Length
  )
{
  dcache_flush_region(Address, Length);
  return Address;
}

VOID *
EFIAPI
WriteBackDataCacheRange (
  IN      VOID                      *Address,
  IN      UINTN                     Length
  )
{
  dcache_inval_region(Address, Length);
  return Address;
}

VOID *
EFIAPI
WriteBackInvalidateDataCacheRange (
 IN      VOID                      *Address,
 IN      UINTN                     Length
  )
{
  dcache_flush_region(Address, Length);
  return Address;
}


DALResult
DALSYS_SyncCreate(uint32 dwAttribs,
                  DALSYSSyncHandle *phSync,
                  DALSYSSyncObj *pSyncObj)
{
   static DALEventObject dummyEventObject;
   
   if(pSyncObj == NULL)
      *phSync = (DALSYSSyncHandle)&dummyEventObject;
   else
      *phSync = (DALSYSSyncHandle)pSyncObj;
   return DAL_SUCCESS;
}

