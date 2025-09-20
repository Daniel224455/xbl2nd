/*===============================================================================
  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE:         PILDxe.c
  DESCRIPTION:    
  
  REVISION HISTORY
  when       who         what, where, why
  --------   ---         --------------------------------------------------------
  06/14/17   yw         initial version

================================================================================*/

#include <Uefi.h>
#include <pil_tz.h>
#include <pil_loader.h>
#include <Protocol/EFIPIL.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/QcomLib.h>
#include <Library/DebugLib.h>
#include <Library/ParserLib.h>
#include <Library/QcomTargetLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TargetResetLib.h>
#include <Protocol/EFIDALSYSProtocol.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/LockLib.h>
#include "KernelLib.h"
#include "Library/UefiCfgLib.h"

/*
    the last error code of EFI and PIL status
*/
EFI_STATUS  gLastEfiError;
PIL_STATUS  gLastPilError;
/*
    AutoStartList[] contains the list of SubsysID which is processed/loaded
    at PIL loading
*/
extern UINT32     AutoStartImageNum;
extern PIL_SUBSYS_CFG_TYPE*  AutoStartList[];

extern UINT32 RetailImageNum;
extern UINT32 AutoStartImageNum;

/**
  process/load images in AutoStartList.

  @param                          none

  @return EFI_SUCCESS           The function completed successfully.
  @return other                    Some error occurred.
**/
STATIC EFI_STATUS ProcessAutoImages()
{
    EFI_STATUS Status = EFI_SUCCESS; 
    PIL_STATUS Ret;
    UINT32 Idx;
    
    for(Idx=0; Idx<AutoStartImageNum; Idx++)
    {
        Ret = PilProcessImageExt(AutoStartList[Idx]);
        if (Ret != PIL_SUCCESS)
            Status = EFI_LOAD_ERROR;
    }
    
    return Status;
}

/**
  read cfg (i.e. ImageLoadInfoBase & ImageLoadInfoSize) of image load info 
  from cfg file.

  @param                          none

  @return EFI_SUCCESS           The function completed successfully.
  @return other                    Some error occurred.
**/

STATIC EFI_STATUS InitImageLoadInfoRegion()
{
    return InitImageList("IMAGE_LOAD_INFO_REGION");
}

/**
  process/load subsys image of given configure

  @param  cfg     [in]          configure infomation of the subsys to be processed

  @return EFI_SUCCESS           The function completed successfully.
  @return other                    Some error occurred.
**/

STATIC EFI_STATUS EFI_PIL_ProcessPilImageExt(
    IN PIL_SUBSYS_CFG_TYPE* cfg
    )
{
    PIL_STATUS PilRet = PilProcessImageExt(cfg);
    if (PilRet == PIL_SUCCESS) {
        PIL_WARN_CFG(cfg, "Boot subsys, Done\n");
    }
    else {
        PIL_WARN_CFG(cfg, "Failed to boot subsys\n");
    }
    
    return (PIL_SUCCESS == PilRet)? EFI_SUCCESS: EFI_LOAD_ERROR;
}

/**
  process/load subsys image of given name

  @param  subsys_name [in]      name of the subsys to be processed

  @return EFI_SUCCESS           The function completed successfully.
  @return other                    Some error occurred.
**/

STATIC EFI_STATUS EFI_PIL_ProcessPilImage(
    IN CHAR16 *subsys_name
    )
{    
    return (PIL_SUCCESS == PilProcessImage(subsys_name))? EFI_SUCCESS: EFI_LOAD_ERROR;
}


/**
  PIL protocol defination
  
**/
static EFI_PIL_PROTOCOL PILProtocol =
{
   EFI_PIL_PROTOCOL_VERSION_V1,
   EFI_PIL_ProcessPilImage,
   EFI_PIL_ProcessPilImageExt,
};

static int ProcessPilImages(void *arg)
{
	EFI_STATUS Status;
	VOID* PIL_Process_Lock = InitLock ("PIL_Lock");

	UINT32 current_core = 0;
	current_core = ThreadGetCurrCpu(GetCurrentThread());
	DEBUG ((EFI_D_WARN, "Start time %d ms\n", current_core));

	Lock(PIL_Process_Lock);
    Status = InitImageLoadInfoRegion();
    if (EFI_ERROR(Status)) {
        PIL_ERROR("Failed to get info of image load info region in IMEM\n");
        goto ErrorExit;
    }
                    
    /* initiate retail list which contains subsys allowed to 
       be processed by PIL in RETAIL product  */
    Status = InitImageList("RETAIL");
    if (EFI_ERROR(Status)) {
        PIL_ERROR("Failed to initiate retail list\n");
        goto ErrorExit;
    }
    else
        PIL_INFO("Retail list added %d images\n", RetailImageNum);
    
    
    /* initiate auto_boot list which contains subsys processed 
       by PIL by default */ 
    Status = InitImageList("AUTO");
    if (EFI_ERROR(Status)) {
        PIL_ERROR("Failed to initiate auto list\n");
        goto ErrorExit;
    }
    else
        PIL_INFO("Auto list added %d images\n", AutoStartImageNum);

    Status = ProcessAutoImages();
    // assert if auto start image is failed to load in retail
    if (EFI_ERROR(Status)) {
        if (RETAIL) {
            PIL_ERROR("Auto processing of uefipil.cfg failed. Calling target panic\n");
            TargetPanic();
        }
        else {
            ASSERT_EFI_ERROR(Status);
//            ASSERT_FAIL_EFI_RETURN(0, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR);
        }
    }
ErrorExit:
	UnLock(PIL_Process_Lock);
    return (Status & (current_core & 0x08));
}

/**
  The Entry Point for PIL module. 

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurred when executing this entry point.

**/
 
EFI_STATUS EFIAPI 
PILDxeEntryPoint
(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
    
    EFI_HANDLE handle = NULL;
    EFI_STATUS Status;
	uint32 Pil_Thread_Parallelization_Enable,Pil_Dest_Core;
	Thread *hPilThread = NULL;      /* PIL thread handle */

	DEBUG ((EFI_D_INFO, "In PILDxeEntryPoint\n"));

	Status = GetConfigValue ("PilThreadParallelizationEnable", &Pil_Thread_Parallelization_Enable);
	if (Status == EFI_SUCCESS && Pil_Thread_Parallelization_Enable == 1)
	{
		hPilThread = ThreadCreate ("PIL", &ProcessPilImages, 0, DEFAULT_PRIORITY, DEFAULT_STACK_SIZE );
		if (NULL != hPilThread)
		{
			if(GetConfigValue ("PilDestCore", &Pil_Dest_Core) == EFI_SUCCESS)
			{
					if(MpcoreIsCpuActive(Pil_Dest_Core))
						ThreadSetPinnedCpu(hPilThread, Pil_Dest_Core);
			}
			ThreadResume(hPilThread);
		}
	}
	else
	{
		ProcessPilImages(NULL);
	}
    Status = gBS->InstallMultipleProtocolInterfaces(
                    &handle,
                    &gEfiPilProtocolGuid,
                    (VOID **)&PILProtocol,
                    NULL );
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        ASSERT_FAIL_EFI_RETURN(0, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR);
    }
    
    return Status;
}
