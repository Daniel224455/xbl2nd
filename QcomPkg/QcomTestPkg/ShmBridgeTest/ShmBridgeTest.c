
/** @file ShmBridgeTest.c
 *  
 * UEFI application to test ShmBridge Create/Delete*  
  
 Copyright (c) 2017-2018 Copyright Qualcomm Technologies, Inc. All
 Rights Reserved. Qualcomm Technologies Proprietary and Confidential.
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/TestInterface.h>
#include <Library/ShmBridgeLib.h>
#include <Protocol/EFITzeLoader.h>
#include <Protocol/EFIScm.h>
#include <Library/EfiFileLib.h>
#include <Protocol/EFIRpmb.h>
#include <Protocol/EFIShmBridge.h>
#include <string.h>
#include <Include/scm_sip_interface.h>
#include <Include/scm_qsee_interface.h>
#include "ShmBridgeTest.h"
#include "LoaderUtils.h"



#define VM_PERM_X 					0x1
#define VM_PERM_W 					0x2
#define VM_PERM_R 					0x4
#define CALLBACK_APP_MBN_FILE       "retstapp0.mbn"
#define MSG_SIZE 					5
#define MSG 						"Good"
#define TEST_LISTENER_ID 			99
#define CLIENT_REQUEST_LISTENER_N  	4
#define TEST_LISTENER_BUFFER_SIZE  	(1024*4)
#define TOTAL_FS_LIMIT    			15

//retstapp0 will load from SD card so no need to TZAPPSPartition
//STATIC EFI_GUID TzAppsPartitionType = 
 // { 0x14D11C40, 0x2A3D, 0x4F97, { 0x88, 0x2D, 0x10, 0x3A, 0x1E, 0xC0, 0x93, 0x33 } };

//Listener 
extern EFI_GUID gEfiSdccRpmbProtocolGuid;
EFI_SHMBRIDGE_PROTOCOL *ShmBridgeProtocol = NULL;

typedef struct gen_list_cmd_s
{
  UINT32 cmd_id;
  UINT64 lid; /* listener id */
  UINT64 lrl; /* listener request/response length */
  uint64_t misc1;
  char   buf[MSG_SIZE];
} gen_list_cmd_t;

typedef struct send_cmd_rsp
{
  UINT32 data;
  INT32 status;
} send_cmd_rsp_t;

gen_list_cmd_t test_req;
send_cmd_rsp_t test_rsp={0};
static EFI_STATUS test_callback_status = 0;

/* This is the callback for test Listener  */
EFI_STATUS 
TestListenerCallback(void *Handle, void *BufferPtr, UINT32 BufferLen)
{
  int i;
  AsciiPrint("Callback message is ");
  //AsciiPrint("Callback message is %a ", (char*)BufferPtr); 
  for (i=0;i<MSG_SIZE;i++)
    AsciiPrint("%c", ((char*)BufferPtr)[i]);

  if ( memcmp(BufferPtr, MSG, MSG_SIZE) ) {
    AsciiPrint(". Callback test ***FAILED***\r\n ");
	test_callback_status=0x40;
	return EFI_INVALID_PARAMETER;
  }
  AsciiPrint(" Callback test PASSED\r\n");
  return EFI_SUCCESS; 
}

EFI_STATUS
TestListener(
    void * name)
{
    EFI_STATUS  Status = 0;
    static EFI_TZE_LOADER_PROTOCOL *TzeLoader;
    static QCOM_SCM_PROTOCOL *ScmProtocol;
    EFI_SDCC_RPMB_PROTOCOL *TestProtocol;
    UINT32 AppId = 0;
    UINT8* TestListenerBuffer;
    UINTN uSize = 0;
	CHAR8			FilePath[50] = {};

    AsciiPrint("Start Listener. \r\n");
    Status = gBS->LocateProtocol (&gEfiTzeLoaderProtocolGuid, NULL, (VOID**)&TzeLoader);
    if (Status != EFI_SUCCESS)
    {
        AsciiPrint("Failed to get EFI_TZE_LOADER_PROTOCOL\n");
    }
    Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID**)&ScmProtocol);
    if (Status != EFI_SUCCESS)
    {
        AsciiPrint("Failed to get QCOM_SCM_PROTOCOL\n");
    }

    
     for( int i= 0; i<TOTAL_FS_LIMIT; i++)
		 {
			snprintf(FilePath, sizeof(FilePath),"fs%d:\\%s",i,(CHAR8*)name);
			
			/*load sample app using LoadImageFromFile  */
			Status = TzeLoader->LoadImageFromFile (TzeLoader,
											   (CHAR8*)FilePath,
											   &AppId);
			if (Status != EFI_SUCCESS)
			{
			   //AsciiPrint("Failed to load %a,  Status = %x\n", name, Status);
			   continue;
			}
			break;
		 }
	if(Status!=EFI_SUCCESS)
		{
		AsciiPrint("Failed to  loaded %a \n",(CHAR8*)name);
		return Status;
		}
    AsciiPrint("Successfully loaded %a,  AppId = %x\n",(CHAR8*)name, AppId);

    //now add callback
    uSize = TEST_LISTENER_BUFFER_SIZE;
    TestListenerBuffer = ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &uSize, ShmBridgeBootSvcData);
    AsciiPrint("Listener #1: pAddr %p uSize %x\n", TestListenerBuffer, uSize);

    Status = gBS->LocateProtocol(&gEfiSdccRpmbProtocolGuid, NULL, (VOID**)&TestProtocol);
    Status = ScmProtocol->ScmRegisterCallback(ScmProtocol,
                                              TEST_LISTENER_ID, 
                                              TestListenerCallback,
                                              TestProtocol,
                                              TestListenerBuffer,
                                              TEST_LISTENER_BUFFER_SIZE );
    if (Status == EFI_SUCCESS)
        AsciiPrint("callback registered. \r\n");
    else
        AsciiPrint("Failed to register  Status = %x\r\n", (UINT32)Status);
	  
    //send message to app:
    test_req.cmd_id=CLIENT_REQUEST_LISTENER_N;
    test_req.lid=TEST_LISTENER_ID;
    test_req.lrl=MSG_SIZE;
	test_req.misc1=0;
    memcpy(test_req.buf, MSG, MSG_SIZE);
    Status = ScmProtocol->ScmSendCommand(ScmProtocol,
                                         APP_SEND_DATA_CMD,
                                         &AppId,
                                         &test_req,
                                         sizeof(test_req),
                                         &test_rsp,
                                         sizeof(test_rsp) );
    if (Status != EFI_SUCCESS || 0 != test_rsp.status )
        AsciiPrint("Failed send message to app Status=%x rsp status=%d, rsp data=%d\r\n",
                  (UINT32)Status, (UINT32)test_rsp.status , (UINT32)test_rsp.data);
    //Now unload:
    Status = ScmProtocol->ScmSendCommand(ScmProtocol, APP_SHUTDOWN_CMD, &AppId, NULL,0, NULL, 0);
    if (Status != EFI_SUCCESS)
        AsciiPrint("Failed to shutdown \n");

    Status = ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, TestListenerBuffer);

    return Status;
}

/**
  Entry point for the application

  @param[in] ImageHandle    Image handle 
  @param[in] SystemTable    Pointer to the System Table
  
  @retval EFI_SUCCESS       Execution successful
  @retval other             Error occurred

**/
EFI_STATUS
EFIAPI
ShmBridgeTestMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  VOID *pAddr = NULL;
  VOID *pSave = NULL;
  UINTN uSize;
  UINTN uHandle;

  TEST_START("ShmBridgeTest");
  AsciiPrint("\nCopy retstapp0.mbn to SD card\n");

  do {
    Status = gBS->LocateProtocol(&gEfiShmBridgeProtocolGuid, NULL, (VOID **)&ShmBridgeProtocol);
    if (Status != EFI_SUCCESS)
    {
      AsciiPrint("Failed to get EFI_SHMBRIDGE_PROTOCOL\n");
      break;
    }

    Status = EnableShmBridge();
    AsciiPrint("ShmBridge enabling status %d\n", Status);

    // create shared memory bridge with ShmBridgeLib
    pAddr = UncachedAllocateZeroPool(0x4000);
    Status = CreateShmBridge(pAddr, 0x4000, &uHandle);
    if (Status == EFI_SUCCESS)
    {
        AsciiPrint("\n16K ShmBridge creation succeeded, addr %p handle %x.\n", pAddr, uHandle);
    }
    else
    {
        AsciiPrint("\n16K ShmBridge creation failed, status %d. Please Check!!\n", Status);
    }

    // delete shared memory bridge with ShmBridgeLib
    Status = DeleteShmBridge(uHandle);
    if (Status == EFI_SUCCESS)
    {
        AsciiPrint("16K ShmBridge deletion succeeded, handle %x.\n", uHandle);
    }
    else
    {
        AsciiPrint("ShmBridge deletion failed, handle %x. Please Check!!\n", uHandle);
    }
    UncachedFreePool(pAddr);

    // create shared memory bridge with ShmBridgeLib
    pAddr = UncachedAllocateZeroPool(0x1100);
    Status = CreateShmBridge(pAddr, 0x1100, &uHandle);
    if (Status != EFI_SUCCESS)
    {
        AsciiPrint("ShmBridge creation failed as expected, size 0x1100 is not page aligned.\n");
    }
    else
    {
        AsciiPrint("ShmBridge creation should fail. Please check!!\n");
    }
    UncachedFreePool(pAddr);

    // create shared memory bridge with ShmBridgeLib
    pAddr = UncachedAllocateZeroPool(0x3000);
    Status = CreateShmBridge(pAddr, 0x3000, &uHandle);
    if (Status == EFI_SUCCESS)
    {
        AsciiPrint("12K ShmBridge creation succeeded, addr %p handle %x.\n", pAddr, uHandle);
    }
    else
    {
        AsciiPrint("12K ShmBridge creation failed, status %d. Please check!!\n", Status);
    }

    // delete shared memory bridge with ShmBridgeLib
    Status = DeleteShmBridge(uHandle);
    if (Status ==EFI_SUCCESS)
    {
        AsciiPrint("12K ShmBridge deletion succeeded, handle %x.\n", uHandle);
    }
    else
    {
        AsciiPrint("ShmBridge deletion failed, handle %x. Please Check!!\n", uHandle);
    }
    UncachedFreePool(pAddr);

    // Create shared memory bridge with ShmBridgeDxe
    pAddr = UncachedAllocateZeroPool(0x4000);

    MapAddrInfo MapInfo;
    DstVMPermInfo VMInfo[2];
    MapInfo.uInputAddr = (UINTN)(pAddr);
    MapInfo.uOutputAddr = (UINTN)(pAddr);
    MapInfo.uSize = 0x4000;
    VMInfo[0].uDstVM = AC_VM_HLOS;
    VMInfo[0].uDstVMperm = VM_PERM_R|VM_PERM_W;

    Status = ShmBridgeProtocol->ShmBridgeCreate(ShmBridgeProtocol, &MapInfo, &VMInfo[0], 1, 0x2|0x4, &uHandle);
    if (Status == EFI_SUCCESS)
    {
        AsciiPrint("16K ShmBridge creation succeeded, addr %p handle %x.\n", pAddr, uHandle);
    }
    else
    {
        AsciiPrint("16K ShmBridge creation failed, status %d. Please check!!\n", Status);
    }

    // delete shared memory bridge with ShmBridgeDxe
    Status = ShmBridgeProtocol->ShmBridgeDelete(ShmBridgeProtocol, uHandle);
    if (Status ==EFI_SUCCESS)
    {
        AsciiPrint("16K ShmBridge deletion succeeded, handle %x.\n", uHandle);
    }
    else
    {
        AsciiPrint("16K ShmBridge deletion failed, handle %x. Please Check!!\n", uHandle);
    }
    UncachedFreePool(pAddr);

    // Allocate 150K with ShmBridgeDxe
    uSize = 0x25800; // 150K
    pAddr = ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &uSize, ShmBridgeBootSvcData);
    if (pAddr != 0)
    {
        AsciiPrint("Allocate 150K succeeded, addr %x.\n", pAddr);
    }
    else
    {
        AsciiPrint("Allocate 150K failed. Please check!!\n");
    }
    pSave = pAddr;

    // Allocate 160K with ShmBridgeDxe
    uSize = 0x28000; // 160K
    pAddr = ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &uSize, ShmBridgeBootSvcData);
    if (pAddr != 0)
    {
        AsciiPrint("Allocate 160K succeeded, addr %x.\n", pAddr);
    }
    else
    {
        AsciiPrint("Allocate 160K failed. Please check!!\n");
    }

    // Allocate 160K with ShmBridgeDxe
    uSize = 0x28000;
    pAddr = ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &uSize, ShmBridgeBootSvcData);
    if (pAddr != 0)
    {
        AsciiPrint("Allocate 160K succeeded, addr %x.\n", pAddr);
    }
    else
    {
        AsciiPrint("Allocate 160K failed. Please check!!\n");
    }


    // Free 160K with ShmBridgeDxe
    Status = ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, pAddr);
    if (Status == EFI_SUCCESS)
    {
        AsciiPrint("Free 160K succeeded, addr %x.\n", pAddr);
    }
    else
    {
        AsciiPrint("Free 160K failed. Please check!!\n");
    }


    // Free 150K with ShmBridgeDxe
    Status = ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, pSave);
    if (Status == EFI_SUCCESS)
    {
        AsciiPrint("Free 150K succeeded, addr %x.\n", pSave);
    }
    else
    {
        AsciiPrint("Free 150K failed. Please check!!\n");
    }

    // Allocate 156K with ShmBridgeDxe
    uSize = 0x27000;
    pAddr = ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &uSize, ShmBridgeBootSvcData);
    if (pAddr != 0)
    {
        AsciiPrint("Allocate 156K succeeded, addr %x.\n", pAddr);
        AsciiPrint("Address should match previous freed memory.\n", pAddr);
    }
    else
    {
        AsciiPrint("Allocate 156K failed. Please check!!\n");
    }

    // Allocate 520K with ShmBridgeDxe
    uSize = 0x82000;
    pAddr = ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &uSize, ShmBridgeBootSvcData);
    if (pAddr != 0)
    {
        AsciiPrint("Allocate 520K succeeded, addr %x.\n", pAddr);
    }
    else
    {
        AsciiPrint("Allocate 520K failed. Please check!!\n");
    }

    // Free 520K with ShmBridgeDxe
    Status = ShmBridgeProtocol->ShmBridgeFree(ShmBridgeProtocol, pAddr);
    if (Status == EFI_SUCCESS)
    {
        AsciiPrint("Free 520K succeeded, addr %x.\n", pAddr);
    }
    else
    {
        AsciiPrint("Free 520K failed. Please check!!\n");
    }

    // Allocate 11MB with ShmBridgeDxe
    uSize = 0xB00000;
    pAddr = ShmBridgeProtocol->ShmBridgeAllocate(ShmBridgeProtocol, &uSize, ShmBridgeBootSvcData);
    if (pAddr == 0)
    {
        AsciiPrint("Allocate 11MB failed as expected, addr %x.\n", pAddr);
    }
    else
    {
        AsciiPrint("Allocate 11MB succeeded. Please check!!\n");
    }

    Status = TestListener(CALLBACK_APP_MBN_FILE);
    AsciiPrint("Listener test status %d\n", Status);

  } while (0);

  TestStatus("ShmBridgeTest", Status);
  TEST_STOP("ShmBridgeTest");

  return Status;  
}

