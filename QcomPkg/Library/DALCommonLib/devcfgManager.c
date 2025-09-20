#include "DALSysInt.h"
#include "stdlib.h"

#ifdef DEVCFG_XBLCONFIG
#include "XBLConfig.h"
#include "DALSys.h"
#include "CoreVerify.h"
extern size_t memscpy( void *dst, size_t dst_size, const void *src, size_t src_size);
#else
#include "assert.h"
#ifndef ASSERT
#define ASSERT assert
#endif
#endif

extern void devcfg_blob_append(DALProps * newPropInfo, int bid);
extern devcfgBlobTableType devcfgBlobTable[];
extern int blobTableSize;

#ifndef DEVCFG_NO_DAL
extern void DAL_setModInfo(DALProps * mod_info_addr, DALREG_DriverInfoList * global_drivers);
extern DALProps DAL_Mod_Info;
extern DALREG_DriverInfoList gDALModDriverInfoList;
#endif

DALProps devcfgPropsInfo;

static int loadStage = 0;

void devcfgManagerInit()
{
   int i = 0;
   for (i = 0; i < blobTableSize; i++ )
   {
      if ((devcfgBlobTable[i].loadtype == DEVCFG_LOADTYPE_RAM) && (devcfgBlobTable[i].stage == loadStage))
      {
         DALProps * dalpropsptr = devcfgBlobTable[i].name;
         devcfg_blob_append(dalpropsptr, 0);
#ifndef DEVCFG_NO_DAL
         DALProps * mod_info_ptr = &DAL_Mod_Info;
         DALREG_DriverInfoList * global_drivers = &gDALModDriverInfoList;
         DAL_setModInfo(mod_info_ptr, global_drivers);
#endif
      }
      else if ((devcfgBlobTable[i].loadtype == DEVCFG_LOADTYPE_XBL)  && (devcfgBlobTable[i].stage == loadStage))
      {
#ifdef DEVCFG_XBLCONFIG
         xblconfig_status_type status;
         unsigned int xblconfig_size;
#ifndef DEVCFG_STANDALONE
         unsigned int xblconfig_handle;
         status = xblconfig_open((char *)devcfgBlobTable[i].name, &xblconfig_handle, &xblconfig_size);
#else
         FILE * xblconfig_handle = malloc(sizeof(FILE));
         status = xblconfig_open((char *)devcfgBlobTable[i].name, xblconfig_handle, &xblconfig_size);
#endif
         ASSERT(status == XBLCONFIG_SUCCESS);

         int currentOffset = 0x0;
         devcfgMetaDataType blobMetaData;
         status = xblconfig_read(xblconfig_handle, currentOffset, sizeof(devcfgMetaDataType), &blobMetaData);
         ASSERT(status == XBLCONFIG_SUCCESS);

         currentOffset += sizeof(devcfgMetaDataType);
         DALSYSPropStructTblType * devcfgStructPtrs;
         DALSYS_Malloc(sizeof(blobMetaData.structPtrTable), (void**)&devcfgStructPtrs);
         status = xblconfig_read(xblconfig_handle, currentOffset, blobMetaData.structPtrTable, (void *)devcfgStructPtrs);
         ASSERT(status == XBLCONFIG_SUCCESS);

         currentOffset += blobMetaData.structPtrTable;
         byte * devcfgPropsBin;
         DALSYS_Malloc(sizeof(blobMetaData.propBin), (void**)&devcfgPropsBin);
         status = xblconfig_read(xblconfig_handle, currentOffset, blobMetaData.propBin, (void *)devcfgPropsBin);
         ASSERT(status == XBLCONFIG_SUCCESS);

         currentOffset += blobMetaData.propBin;
         StringDevice * devcfgDriverlist;
         DALSYS_Malloc(sizeof(blobMetaData.deviceList), (void**)&devcfgDriverlist);
         status = xblconfig_read(xblconfig_handle, currentOffset, blobMetaData.deviceList, (void *)devcfgDriverlist);
         ASSERT(status == XBLCONFIG_SUCCESS);

         int numDrivers = blobMetaData.deviceList/sizeof(StringDevice);

         //patch struct ptr table
         void * thisStruct;
         if(devcfgStructPtrs)
         {
            int j = 0;
            do
            {
               DALSYS_Malloc(sizeof(devcfgStructPtrs[j].dwSize), (void**)&thisStruct);
               status = xblconfig_read(xblconfig_handle, (unsigned long)devcfgStructPtrs[j].pStruct, devcfgStructPtrs[j].dwSize, thisStruct);
               devcfgStructPtrs[j].pStruct = thisStruct;
               ASSERT(status == XBLCONFIG_SUCCESS);
               j++;
            }while((devcfgStructPtrs[j].dwSize != 0) && (devcfgStructPtrs[j].pStruct != 0));
         }
         
         devcfgPropsInfo.pDALPROP_PropBin = devcfgPropsBin;
         devcfgPropsInfo.pDALPROP_StructPtrs = devcfgStructPtrs;
         devcfgPropsInfo.dwDeviceSize = numDrivers;
         devcfgPropsInfo.pDevices = devcfgDriverlist;

         devcfg_blob_append(&devcfgPropsInfo, 0);
         status = xblconfig_close(xblconfig_handle);
#endif
      }
      else if ((devcfgBlobTable[i].loadtype == DEVCFG_LOADTYPE_XBL_FIXED) && (devcfgBlobTable[i].stage == loadStage))
      {
#ifdef DEVCFG_XBLCONFIG
         xblconfig_status_type status;
         unsigned int xblconfig_size = 0;
#ifndef DEVCFG_STANDALONE
         unsigned int xblconfig_handle = 0;
         status = xblconfig_open("/devcfg.bin", &xblconfig_handle, &xblconfig_size);
#else
         FILE * xblconfig_handle = malloc(sizeof(FILE));
         status = xblconfig_open((char *)devcfgBlobTable[i].name, xblconfig_handle, &xblconfig_size);
#endif
         CORE_VERIFY(status == XBLCONFIG_SUCCESS);

         void * devcfgXblconfigMetaBuffer = (void *)DEVCFG_META_SECTION_BASE;
         devcfgMetaDataType * blobMetaData = NULL;

         status = xblconfig_read(xblconfig_handle, 0x0, sizeof(devcfgMetaDataType), (void *)devcfgXblconfigMetaBuffer);
         CORE_VERIFY(status == XBLCONFIG_SUCCESS);
         blobMetaData = (devcfgMetaDataType *)devcfgXblconfigMetaBuffer;
         int remainingBuf = xblconfig_size - sizeof(devcfgMetaDataType);

         void * devcfgXblconfigBuffer = (void *)DEVCFG_DATA_BUFFER_BASE;
         
         status = xblconfig_read(xblconfig_handle, sizeof(devcfgMetaDataType), remainingBuf, (void *)devcfgXblconfigBuffer);
         CORE_VERIFY(status == XBLCONFIG_SUCCESS);

         volatile DALSYSPropStructTblType * const devcfgStructPtrs = (DALSYSPropStructTblType *)blobMetaData->structPtrTable;
         volatile byte * const devcfgPropsBin = (byte *)blobMetaData->propBin;
         volatile StringDevice * const devcfgDriverlist = (StringDevice *)blobMetaData->deviceList;

         memscpy((void *)devcfgStructPtrs,remainingBuf,devcfgXblconfigBuffer,remainingBuf);

         int numDrivers = blobMetaData->deviceListSize;
         
         devcfgPropsInfo.pDALPROP_PropBin = (byte *)devcfgPropsBin;
         devcfgPropsInfo.pDALPROP_StructPtrs = (DALSYSPropStructTblType *)devcfgStructPtrs;
         devcfgPropsInfo.dwDeviceSize = numDrivers;
         devcfgPropsInfo.pDevices = (StringDevice *)devcfgDriverlist;
          
         devcfg_blob_append(&devcfgPropsInfo, 1);
         status = xblconfig_close(xblconfig_handle);
#endif
      }
   }
   loadStage = loadStage + 1;
}
