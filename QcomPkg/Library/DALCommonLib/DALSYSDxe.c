/** @file DALSYSDxe.c

  DALSYS library for DXE driver

  Copyright (c) 2010-2011,2014,2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 03/13/18   ak      Move DALSYS_MemRegionAlloc extended for dynamic mapping
 01/24/18   vk      Add UEFI function for BusyWait
 08/14/14   aa      Updating DALSYS_GetDAL_Mod to return string device info list
 08/13/14   aa      Adding DALSYS_GetDAL_Mod for XBL Core
 08/11/14   sho     Creation 

=============================================================================*/

#include "DALStdDef.h"
#include "DALStdErr.h"
#include "DALSysCmn.h"
#include "DALSys.h"
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFISmem.h>
#include <PiDxe.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/LockLib.h>
#include "string.h"


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

typedef struct _DALSYSSyncObj _DALSYSSyncObj;
struct _DALSYSSyncObj
{    
   INHERIT_DALSYS_BASE_OBJ();
   void* hMutex;
};


#ifdef DEVCFG_STRICT_XML
/*DALProps * module_info;
DALREG_DriverInfoList * gDALModDriverInfo;
void DAL_setModInfo(DALProps * mod_info_addr, DALREG_DriverInfoList * global_drivers)
{
   module_info = mod_info_addr;
   gDALModDriverInfo = global_drivers;
}*/
#endif

//StringDevice Info List for DALFwk
DALProps *gpDALModStringDevDrvInfoList[DALSYSCMN_MAX_NUM_MODS_PER_PROCESS];

static uint32 updateUncachedMemProps( uint32 memProps )
{
   if((memProps & DALSYS_MEM_PROPS_CACHED_MASK) == DALSYS_MEM_PROPS_UNCACHED)
      memProps = (memProps & ~DALSYS_MEM_PROPS_CACHED_MASK) | DALSYS_MEM_PROPS_CACHED_WB;
   return memProps;
}

void
DALSYS_BusyWait(uint32 pause_time_us)
{
  MicroSecondDelay(pause_time_us);
}

DALResult
DALSYS_Malloc(UINT32 dwSize, VOID **ppMem)
{
   EFI_STATUS efiStatus;
   efiStatus = gBS->AllocatePool( EfiBootServicesData, dwSize, ppMem );
   return (EFI_SUCCESS == efiStatus)? DAL_SUCCESS: DAL_ERROR;
}

DALResult
DALSYS_Free(VOID *pmem)
{
   EFI_STATUS efiStatus;
   efiStatus = gBS->FreePool( pmem );
   return (EFI_SUCCESS == efiStatus)? DAL_SUCCESS: DAL_ERROR;
}

DALResult
DALSYSCMN_RegisterStrModDriverList(DALProps *pDALModStringDevDrvInfoList)
{
   uint32 dwModIdx = 0;
   for(dwModIdx=0;dwModIdx<DALSYSCMN_MAX_NUM_MODS_PER_PROCESS;dwModIdx++)
   {
      if(NULL == gpDALModStringDevDrvInfoList[dwModIdx])
      {
         gpDALModStringDevDrvInfoList[dwModIdx] = pDALModStringDevDrvInfoList;
         return DAL_SUCCESS;
      }
   }
   DEBUG(( EFI_D_ERROR, "Insufficient space for a new module driver list\n" ));
   return DAL_ERROR;
}

DALResult
DALSYSCMN_DeRegisterStrModDriverList(DALProps *pDALModStringDevDrvInfoList)
{
   uint32 dwModIdx = 0;
   for(dwModIdx=0;dwModIdx<DALSYSCMN_MAX_NUM_MODS_PER_PROCESS;dwModIdx++)
   {
      if(pDALModStringDevDrvInfoList == gpDALModStringDevDrvInfoList[dwModIdx])
      {
         gpDALModStringDevDrvInfoList[dwModIdx] = NULL;
         return DAL_SUCCESS;
      }
   }
   DEBUG(( EFI_D_ERROR, "Cannot find the specified module driver list 0x%x\n", 
         pDALModStringDevDrvInfoList ));
   return DAL_ERROR;
}

static DALProps *
DALSYS_GetDAL_Mod(const char *pszDevName)
{
   uint32 dwModIdx = 0;
   /*--------------------------------------------------------------------------
   For all modules in this list, find the drivers infos
   --------------------------------------------------------------------------*/
   for(dwModIdx=0;dwModIdx<DALSYSCMN_MAX_NUM_MODS_PER_PROCESS;dwModIdx++)
   {
      DALProps *pDALModStringDevDrvInfoList = gpDALModStringDevDrvInfoList[dwModIdx];
      if(NULL != pDALModStringDevDrvInfoList)
      {
         uint32 dwDriverIdx = 0;
         /*------------------------------------------------------------------
           For all drivers of this mod search for requested device string
          -----------------------------------------------------------------*/
         for(dwDriverIdx=0; dwDriverIdx < pDALModStringDevDrvInfoList->dwDeviceSize;dwDriverIdx++)
         {
            // Get the dwIndex, where the str might be
            if(AsciiStrLen(pszDevName) == AsciiStrLen(pDALModStringDevDrvInfoList->pDevices[dwDriverIdx].pszName))
            {
               if(AsciiStrnCmp(pszDevName, pDALModStringDevDrvInfoList->pDevices[dwDriverIdx].pszName, AsciiStrLen(pszDevName))==0)
               {
                  return pDALModStringDevDrvInfoList;
               }
            }
         }
      }
   }
   /*--------------------------------------------------------------------------
   Did not find the requested string device Id in this process
   --------------------------------------------------------------------------*/
   DEBUG(( EFI_D_ERROR, "DAL device (0x%s) not found\n", pszDevName ));
   return NULL;
}

DALResult
_DALSYSCMN_GetStringDeviceObject(const char *pszDevName, const char *pszArg, DalDeviceHandle **phDevice)
{ 
   const StringDevice *pStringDeviceObj = NULL;
   DALREG_DriverInfo *pDriverInfo = NULL;
   
   if(DAL_SUCCESS == _DAL_StringDeviceObjectLookup(pszDevName, &pStringDeviceObj, DALSYS_GetDAL_Mod(pszDevName))){
      //Get the Property Dir Info Ptr
      pDriverInfo = pStringDeviceObj->pFunctionName;
      return pDriverInfo->pfnDALNewFunc(pszDevName,0,phDevice);
   } 
   return DAL_ERROR;
}

DALResult
_DAL_StringDeviceObjectLookup(const char *pszDevName, const StringDevice **ppStringDeviceObj, DALProps *pDALPtr)
{
   int dwIndex = 0;
   
   if(pDALPtr!= NULL && pDALPtr->pDevices!=NULL && pszDevName!= NULL 
     && dwIndex< pDALPtr->dwDeviceSize && ppStringDeviceObj!=NULL)
   {
      // Finish the search with right device index. 
      for(dwIndex=0; dwIndex < pDALPtr->dwDeviceSize; dwIndex++)
      {
         // Get the dwIndex, where the str might be
         if(AsciiStrLen(pszDevName) == AsciiStrLen(pDALPtr->pDevices[dwIndex].pszName))
         {
            if(AsciiStrnCmp(pszDevName, pDALPtr->pDevices[dwIndex].pszName, AsciiStrLen(pszDevName))==0)
            {
               *ppStringDeviceObj =  &(pDALPtr->pDevices[dwIndex]);
               return DAL_SUCCESS;
            }
         }
      }   
   }
   return DAL_ERROR_NOT_FOUND;
}

// Allocate global context from shared memory driver
EFI_STATUS
_AllocGlbCtxtMem(DALSYSMemAddr *p_ctxt_addr, UINT32 size)
{
   EFI_SMEM_PROTOCOL * smem_protocol;
   EFI_STATUS status;

   status = gBS->LocateProtocol(&gEfiSMEMProtocolGuid, NULL,
      (void**)&smem_protocol);
   if(status != EFI_SUCCESS)
   {
      DEBUG ((EFI_D_ERROR, 
         "Unable to locate smem protocol; status=0x%x\n",
         status));
      return status;
   }

   status = smem_protocol->SmemAlloc(SMEM_DAL_AREA, size,
         (void **)p_ctxt_addr);
   return status;
}
 
static void *
_CreateMemSection(uint32 dwLen, uint32 dwAttribs,
                  DALSYSMemAddr *VirtualAddr,DALSYSMemAddr *PhysicalAddr)
{
    void *pMemRegion = NULL;

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
       return pMemRegion;
    }
    else if(DALSYS_MEM_ADDR_NOT_SPECIFIED  != *PhysicalAddr)
    {

      int eStatus=0;
      EFI_GCD_MEMORY_SPACE_DESCRIPTOR MemDesc;

      /*Check if this mapping exists*/
      eStatus = gDS->GetMemorySpaceDescriptor((EFI_PHYSICAL_ADDRESS)*PhysicalAddr,
                                               &MemDesc 
                                              );

      if(EFI_SUCCESS != eStatus)
      {
          goto FAIL;
      }
      else
      {
          if(EfiGcdMemoryTypeMemoryMappedIo == MemDesc.GcdMemoryType)
          {
            /*Mapping exists, nothing to do here*/
            *VirtualAddr = *PhysicalAddr;
            return (void*)(*PhysicalAddr);
          }

          if(EfiGcdMemoryTypeNonExistent== MemDesc.GcdMemoryType)
          {
            /*Mapping does not exist, Add memory region to cpu's coherency pool*/
            eStatus = gDS->AddMemorySpace(
                                      EfiGcdMemoryTypeMemoryMappedIo,
                                      (EFI_PHYSICAL_ADDRESS)*PhysicalAddr, 
                                      dwLen,
                                      EFI_MEMORY_UC
                                      );

            if(EFI_SUCCESS != eStatus)
            {
              goto FAIL;
            }
            else
            {
              /*Change atrributes on mapped memory*/
              eStatus = gDS->SetMemorySpaceAttributes(
                                              (EFI_PHYSICAL_ADDRESS)*PhysicalAddr, 
                                               dwLen, 
                                               EFI_MEMORY_UC
                                              );

              if(EFI_SUCCESS != eStatus)
              {
                goto FAIL;
              }
              else
              {
                  /*Mapping exists, nothing to do here*/
                  *VirtualAddr = *PhysicalAddr;
                  return (void*)(*PhysicalAddr);
              }
            }
          }
      }
    }

    FAIL:
      pMemRegion = NULL;
      DEBUG(( EFI_D_ERROR, "Failed to map PhysicalAddr:0x%x, size:0x%x, SetMemorySpaceAttributes\n", *PhysicalAddr, dwLen));

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

UINTN NumberOfSyncObjs = 0;

DALResult
DALSYS_SyncCreate(uint32 dwAttribs,
                  DALSYSSyncHandle *phSync,
                  DALSYSSyncObj *pObj)
{
   int nRet = DAL_ERROR;
   _DALSYSSyncObj *pSyncObj = (_DALSYSSyncObj *)pObj;

   *phSync = NULL;

   if(!pSyncObj)
   {
      if(DAL_SUCCESS == DALSYS_Malloc(sizeof(_DALSYSSyncObj),
                                      (void**)&pSyncObj))
      {
         memset(pSyncObj, 0, sizeof(_DALSYSSyncObj));
         DALSYSCMN_SET_DYNAMIC_ALLOC_BIT(pSyncObj->dwObjInfo);
         nRet = DAL_SUCCESS;
      }
      else
      {
         return DAL_ERROR;
      }
   }
   else
   {
      DALSYSCMN_RESET_ALL_BITS(pSyncObj->dwObjInfo);
      nRet = DAL_SUCCESS;
   }

   /* Its a sync object */
   DALSYSCMN_SET_OBJECT_ID(pSyncObj->dwObjInfo,DALSYSCMN_SYNC_OBJECT_ID);   
   pSyncObj->thisVirtualAddr = (DALSYSMemAddr)pSyncObj;

   /* Init mutex */
   pSyncObj->hMutex = InitRecursiveLock(0);

   ++ NumberOfSyncObjs;

   /* Set up handle */
   *phSync = pSyncObj;

   return nRet;
}


