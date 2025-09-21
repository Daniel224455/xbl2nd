/** @file IMX318Test.c
   
  Camera Sensor IMX318 Test App

Copyright (c) 2016-2017 QUALCOMM Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

**/

/*=============================================================================
                              EDIT HISTORY

 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
2017/05/05  VB         Updated  app to test Sony IMX318 camera sensor
2016/07/08  MFC/JW     Created  app to test Sony IMX230 camera sensor
=============================================================================*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFITlmm.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFIPmicGpio.h>
#include "pmapp_npa.h"
#include "npa.h"
#include <Library/IoLib.h>
#include "CCIService.h"
#include "CCIResourceManager.h"
#include "CCI_HAL.h"
//#include "../LLD.h"


// TLMM GPIO configuration
// EFI_GPIO_CFG(gpio, func,dir, pull, drive) 


#define TLMM_GPIO_nCAM_CCI_SDA0 EFI_GPIO_CFG(17, 1, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_2MA)
#define TLMM_GPIO_nCAM_CCI_SCL0 EFI_GPIO_CFG(18, 1, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_2MA)
#define TLMM_GPIO_nCAM_MCLK0    EFI_GPIO_CFG(13, 1, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA)
#define TLMM_GPIO_nCAM_RST      EFI_GPIO_CFG(80, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA)
#define TLMM_GPIO_nCAM_ELDO9_EN EFI_GPIO_CFG(79, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA)
#define TLMM_GPIO_nCAM_ELDO3_EN EFI_GPIO_CFG(27, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA)

#define      MAX_CLOCK_NAME                         28
#define      MAX_POWER_DOMAIN_NAME                  28

// Power Domain and clock configuration
typedef struct
{
  CHAR8                     szName[MAX_POWER_DOMAIN_NAME];    /* Power domain name */
} PowerDomainEntry;

typedef struct
{
  CHAR8                     szName[MAX_CLOCK_NAME];    /* Clock name */
  UINT32                    nFreqHz;                   /* Requested Frequency */
  UINTN                     nMatchType;
} ClockEntry;

//power domain as mentioned in the HPG. 
static const PowerDomainEntry PDList[] = {{"titan_top_gdsc"}};


//camera specific clock list
static const ClockEntry ClockList[] = {{"cam_cc_soc_ahb_clk"    ,37500000  ,EFI_CLOCK_FREQUENCY_HZ_CLOSEST},	    //SVS-freq based on cust_camera_resources.asl \\_SB.CAMP
                                {"cam_cc_cpas_ahb_clk"   ,66670000  ,EFI_CLOCK_FREQUENCY_HZ_CLOSEST},	    //SVS-freq based on cust_camera_resources.asl \\_SB.CAMP
                                {"cam_cc_cci_clk"        ,0         ,EFI_CLOCK_FREQUENCY_HZ_AT_LEAST},		//minSVS-freq
                                {"cam_cc_mclk0_clk"      ,24000000  ,EFI_CLOCK_FREQUENCY_HZ_CLOSEST}		//based on imx318_lib.h
                                };


//camera specific gcc clock list
static const ClockEntry GccClockList[] = {{"gcc_camera_xo_clk"    ,0         ,EFI_CLOCK_FREQUENCY_HZ_AT_LEAST},	    //0hz freq based on cust_camera_resources.asl \\_SB.CAMP
                                   {"gcc_camera_ahb_clk"   ,0         ,EFI_CLOCK_FREQUENCY_HZ_AT_LEAST} 	    //SVS-freq based on cust_camera_resources.asl \\_SB.CAMP
                                   };

npa_client_handle back_camera = NULL;
EFI_CLOCK_PROTOCOL *mClockProtocol = NULL;


EFI_STATUS 
Init_GccClocks(void)
{
  EFI_STATUS         status = EFI_SUCCESS;
  UINTN              nCount, nClockID, i = 0;
  BOOLEAN            nFlag, bIsEnabled;

  if (mClockProtocol == NULL){
     status = EFI_NOT_READY;
     goto exit;
  }

  nCount = sizeof (GccClockList) / sizeof (ClockEntry);

  for( i = 0; i < nCount; i++){
 
     AsciiPrint("  Checking clock \"%a\" ...\n",GccClockList[i].szName );
    
    /* For each clock get the ID */
    status = mClockProtocol->GetClockID(mClockProtocol, GccClockList[i].szName, &nClockID);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", mClockProtocol->GetClockID, status);
      goto exit;
    }

    /* Check IsClockOn */
    status = mClockProtocol->IsClockOn(mClockProtocol, nClockID, &nFlag);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", mClockProtocol->IsClockOn, status);
      goto exit;
    }else{
      AsciiPrint("  IsClockOn: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
    }

    status = mClockProtocol->IsClockEnabled(mClockProtocol, nClockID, &nFlag);
    bIsEnabled = nFlag;
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", mClockProtocol->IsClockEnabled, status);
      goto exit;
    }else{
      AsciiPrint("  IsClockEnabled: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
    }

    /* If clock is not enabled, then enable it */
    if(!bIsEnabled)
    {    
      status = mClockProtocol->EnableClock(mClockProtocol, nClockID);
      if(EFI_ERROR (status))
      {
        AsciiPrint("  : %a returned %r \n", mClockProtocol->EnableClock, status);
        goto exit;
      }else{
        AsciiPrint("  EnableClock: SUCCESS\n" );
      }

      // Recheck IsClockEnabled 
      status = mClockProtocol->IsClockEnabled(mClockProtocol, nClockID, &nFlag);
      if(EFI_ERROR (status))
      {
        AsciiPrint("  : %a returned %r \n",mClockProtocol->IsClockEnabled, status);
        goto exit;
      }else{
        AsciiPrint("  Rechecking IsClockEnabled: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
      }

      // Recheck IsClockOn 
      status = mClockProtocol->IsClockOn(mClockProtocol, nClockID, &nFlag);
      if(EFI_ERROR (status))
      {
        AsciiPrint("  : %a returned %r \n",mClockProtocol->IsClockOn, status);
        goto exit;
      }else{
        AsciiPrint("  Rechecking IsClockOn: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
      }
    }

    AsciiPrint("  ========================================\n" );
  };

exit:
    
  return status;
}


void
DeInit_GccClocks(void)
{
    EFI_STATUS status = EFI_SUCCESS;
    UINTN nClockID;
    BOOLEAN nFlag;
    UINTN nCount = 0, i = 0;
     
    if (mClockProtocol == NULL) return;

    nCount = sizeof (GccClockList) / sizeof (ClockEntry);

    for (i = 0; i < nCount; i++){

        AsciiPrint("  Checking clock \"%a\" ...\n",GccClockList[i].szName );
        status = mClockProtocol->GetClockID(mClockProtocol, GccClockList[i].szName, &nClockID);
        if(EFI_ERROR (status)){
          AsciiPrint("  : %a returned %r \n", mClockProtocol->GetClockID, status);
        }

        status = mClockProtocol->DisableClock(mClockProtocol, nClockID);
        if(EFI_ERROR (status)) {
          AsciiPrint("  : %a returned %r \n", mClockProtocol->DisableClock, status);
        }
        else{
          AsciiPrint("  DisableClock: SUCCESS\n" );
        }
      
        //Recheck IsClockEnabled
        status = mClockProtocol->IsClockEnabled(mClockProtocol, nClockID, &nFlag);
        if(EFI_ERROR (status)) {
          AsciiPrint("  : %a returned %r \n", mClockProtocol->IsClockEnabled, status);
        }
        else{
          AsciiPrint("  Rechecking IsClockEnabled: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
        }
      
        // Recheck IsClockOn
        status = mClockProtocol->IsClockOn(mClockProtocol, nClockID, &nFlag);
        if(EFI_ERROR (status)) {
          AsciiPrint("  : %a returned %r \n", mClockProtocol->IsClockOn, status);
        }
        else{
          AsciiPrint("  Rechecking IsClockOn: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
        }       

    }
   
}

EFI_STATUS 
Init_Clocks(void)
{
  EFI_STATUS         status = EFI_SUCCESS;
  UINTN              nCount, nClockID, i = 0;
  BOOLEAN            nFlag, bIsEnabled;

  if (mClockProtocol == NULL){
     status = EFI_NOT_READY;
     goto exit;
  }

  nCount = sizeof (ClockList) / sizeof (ClockEntry);

  for( i = 0; i < nCount; i++){
 
     AsciiPrint("  Checking clock \"%a\" ...\n",ClockList[i].szName );
    
    /* For each clock get the ID */
    status = mClockProtocol->GetClockID(mClockProtocol, ClockList[i].szName, &nClockID);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", mClockProtocol->GetClockID, status);
      goto exit;
    }

    /* Check IsClockOn */
    status = mClockProtocol->IsClockOn(mClockProtocol, nClockID, &nFlag);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", mClockProtocol->IsClockOn, status);
      goto exit;
    }else{
      AsciiPrint("  IsClockOn: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
    }

    status = mClockProtocol->IsClockEnabled(mClockProtocol, nClockID, &nFlag);
    bIsEnabled = nFlag;
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", mClockProtocol->IsClockEnabled, status);
      goto exit;
    }else{
      AsciiPrint("  IsClockEnabled: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
    }

    /* If clock is not enabled, then enable it */
    if(!bIsEnabled)
    {    
      status = mClockProtocol->EnableClock(mClockProtocol, nClockID);
      if(EFI_ERROR (status))
      {
        AsciiPrint("  : %a returned %r \n", mClockProtocol->EnableClock, status);
        goto exit;
      }else{
        AsciiPrint("  EnableClock: SUCCESS\n" );
      }

      // Recheck IsClockEnabled 
      status = mClockProtocol->IsClockEnabled(mClockProtocol, nClockID, &nFlag);
      if(EFI_ERROR (status))
      {
        AsciiPrint("  : %a returned %r \n",mClockProtocol->IsClockEnabled, status);
        goto exit;
      }else{
        AsciiPrint("  Rechecking IsClockEnabled: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
      }

      // Recheck IsClockOn 
      status = mClockProtocol->IsClockOn(mClockProtocol, nClockID, &nFlag);
      if(EFI_ERROR (status))
      {
        AsciiPrint("  : %a returned %r \n",mClockProtocol->IsClockOn, status);
        goto exit;
      }else{
        AsciiPrint("  Rechecking IsClockOn: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
      }
    }

    AsciiPrint("  ========================================\n" );
  };

exit:
    
  return status;
}


void
DeInit_Clocks(void)
{
    EFI_STATUS status = EFI_SUCCESS;
    UINTN nClockID;
    BOOLEAN nFlag;
    UINTN nCount = 0, i = 0;
     
    if (mClockProtocol == NULL) return;

    nCount = sizeof (ClockList) / sizeof (ClockEntry);

    for (i = 0; i < nCount; i++){

        AsciiPrint("  Checking clock \"%a\" ...\n",ClockList[i].szName );
        status = mClockProtocol->GetClockID(mClockProtocol, ClockList[i].szName, &nClockID);
        if(EFI_ERROR (status)){
          AsciiPrint("  : %a returned %r \n", mClockProtocol->GetClockID, status);
        }

        status = mClockProtocol->DisableClock(mClockProtocol, nClockID);
        if(EFI_ERROR (status)) {
          AsciiPrint("  : %a returned %r \n", mClockProtocol->DisableClock, status);
        }
        else{
          AsciiPrint("  DisableClock: SUCCESS\n" );
        }
      
        //Recheck IsClockEnabled
        status = mClockProtocol->IsClockEnabled(mClockProtocol, nClockID, &nFlag);
        if(EFI_ERROR (status)) {
          AsciiPrint("  : %a returned %r \n", mClockProtocol->IsClockEnabled, status);
        }
        else{
          AsciiPrint("  Rechecking IsClockEnabled: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
        }
      
        // Recheck IsClockOn
        status = mClockProtocol->IsClockOn(mClockProtocol, nClockID, &nFlag);
        if(EFI_ERROR (status)) {
          AsciiPrint("  : %a returned %r \n", mClockProtocol->IsClockOn, status);
        }
        else{
          AsciiPrint("  Rechecking IsClockOn: %a\n", (nFlag == TRUE) ? "TRUE":"FALSE" );
        }       

    }
   
}


/* Turn on rear camera sensor power rail, LVS1 */  
EFI_STATUS
PowerOnCameraSensorRail (VOID){

    EFI_STATUS status = EFI_SUCCESS;
    npa_query_type  query_result;        
    npa_query_status  query_status_back = NPA_QUERY_NO_VALUE;


    AsciiPrint("  Query the current state of rear camera\n" ); 
    query_status_back = npa_query_by_name(PMIC_NPA_GROUP_ID_CAMERA_BACK, NPA_QUERY_CURRENT_STATE, &query_result);
    if (NPA_QUERY_SUCCESS == query_status_back)
    {
      AsciiPrint("    Turning on back camera rails\n" );  
      back_camera = npa_create_sync_client(PMIC_NPA_GROUP_ID_CAMERA_BACK, "BACK_CAMERA", NPA_CLIENT_REQUIRED);
      npa_issue_required_request(back_camera, PMIC_NPA_MODE_ID_GENERIC_ACTIVE); 
    }
    else
    {
       AsciiPrint("  : %a returned %d\n", npa_query_by_name, query_status_back);
       status = EFI_NOT_READY;
    }

    return status;
}

void
PowerOffCameraSensorRail (VOID){

    AsciiPrint("  Turning off back camera rails\n" ); 
    if(back_camera){
      npa_issue_required_request(back_camera, PMIC_NPA_MODE_ID_GENERIC_STANDBY);

      npa_complete_request(back_camera);

      npa_destroy_client(back_camera);
    } 
    return ;
}

void
DeInit_PowerDomains(VOID){

  EFI_STATUS              status = EFI_SUCCESS;
  UINTN                   uClockPowerDomainId;
  UINTN                   nCount = 0 , i = 0;

  if (mClockProtocol == NULL) return;

  nCount = sizeof (PDList) / sizeof (PowerDomainEntry);

  for (i = 0; i < nCount; i++){

    AsciiPrint("  Checking Domain ID \"%a\" ...\n",PDList[i].szName );

    status = mClockProtocol->GetClockPowerDomainID(mClockProtocol, PDList[i].szName, &uClockPowerDomainId);
    if (EFI_ERROR (status)){
      AsciiPrint( "  GetClockPowerDomainID failed\r\n");
      continue;
    }

    AsciiPrint("  Disabling power Domain\n");
    status = mClockProtocol->DisableClockPowerDomain(mClockProtocol, uClockPowerDomainId);
    if (EFI_ERROR (status)){
      AsciiPrint( "  DisableClockPowerDomain failed\r\n");
    }
  } 

  return ;
}

EFI_STATUS 
Init_PowerDomains(VOID){

  EFI_STATUS              status = EFI_SUCCESS;
  UINTN                   uClockPowerDomainId;
  UINTN                   nCount = 0, i = 0;

  nCount = sizeof (PDList) / sizeof (PowerDomainEntry);

  for( i = 0; i < nCount; i++){
    /* Configure power domain*/
    AsciiPrint("  Checking Domain ID \"%a\" ...\n",PDList[i].szName );

    status = mClockProtocol->GetClockPowerDomainID(mClockProtocol, PDList[i].szName, &uClockPowerDomainId);
    if (EFI_ERROR (status)){
      AsciiPrint("  : %a returned %r \n", mClockProtocol->GetClockPowerDomainID, status);
      continue;
    }

    AsciiPrint("  Enabling power Domain\n");
    status = mClockProtocol->EnableClockPowerDomain(mClockProtocol, uClockPowerDomainId);
    if (EFI_ERROR (status)){
      AsciiPrint("  : %a returned %r \n", mClockProtocol->EnableClockPowerDomain, status);
      goto exit;
    }
  }
exit:
 
  return status;
}

/* Sanity check CCI by reading CCI version register */
EFI_STATUS 
SanityCheck_CCI(void)
{
    EFI_STATUS status = EFI_SUCCESS;

    UINT32 nCCI_HW_VER = MmioRead32(0x0AC4A000);

    AsciiPrint(" CCI_HW_VERSION : %r \n", nCCI_HW_VER);

    return status;
}

void CCIReadRequestInit(CCIUserRequestType* rqst, UINT8* read_buffer){

    /// number of i2c transactions
    rqst->ntransactions = 1;

    /// defines i2c transaction
    rqst->serviceRqst.master = m0;
    rqst->config.nregaddr  = 0x0016;
    rqst->config.nregsz    = 2;
    rqst->config.pdata = read_buffer;
    rqst->config.ndatasz   = 2;
    rqst->config.brptstart = FALSE;
    rqst->config.bcontinuous = FALSE;
    rqst->config.max_write_cnts = 0;
    CCI_HAL_get_queueInfo(&rqst->queueInfo, CCI_I2C_M0_Q1); 


    /// This indicates timout value 
    rqst->nwaittime = 1000;

    /// This indicates delay after transaction 
    rqst->ndelay = 0;

    /// This is current CID
    rqst->ncid = 0;

    /// This is current slave address
    /// sony imx 318 slave addr
    rqst->nslaveaddr = 0x34;

    /// This is current data size
    rqst->norig_datasz = 2;

    /// This is current data size
    rqst->norig_regsz = 2;

    /// This is current read data size
    rqst->nrddatasz = 2;

    /// read buff
    rqst->rdbuff = read_buffer;

}


/* Read to camera sensor registers and poll for completion need to  */
EFI_STATUS 
ReadCameraSensorRegister(void)
{
    EFI_STATUS status = EFI_SUCCESS;
    CCIService I2CReadBuilder;
    CCIRsm  rsm;
    CCIUserRequestType rqst;
    UINT8 read_buffer[2] = {0};
    UINT16 sensorID = 0;

    AsciiPrint( "  Initialiazing  read request\n");
    CCIReadRequestInit(&rqst, read_buffer);

    AsciiPrint( "  Initialiazing  CCI control variables\n");
    CCI_HAL_UEFI_Init();

    AsciiPrint( "  Initialiazing  CCI Resource Manager\n");
    CCIRsm_Init(&rsm);

    AsciiPrint( "  Initialiazing  CCI Service\n");
    CCIService_Init(&I2CReadBuilder, &rsm, i2c_read, &rqst.serviceRqst);

    AsciiPrint( "  Reading camera sensor ID\n");
    CCIService_Read(&I2CReadBuilder, &rqst);

    if(rqst.eStatus != CCI_Status_OK){
      status = EFI_DEVICE_ERROR;
    }else{
      sensorID = (read_buffer[0] << 8) + read_buffer[1];
      AsciiPrint("  The camera sensor ID is  0x%x \n",  sensorID );
    }

    return status;
}

EFI_STATUS
Init_CamRearSensor (VOID)
{
    EFI_STATUS status = EFI_SUCCESS;
    EFI_TLMM_PROTOCOL       *TLMMProtocol = NULL;
    EFI_QCOM_PMIC_GPIO_PROTOCOL           *PmicGPIOProtocol = NULL;
	
    status = gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (VOID **)&TLMMProtocol);
    if (EFI_ERROR (status)){
      AsciiPrint( "ConfigTLMMGPIO: TLMM Locate Protocol failed\r\n");
      goto exit;
     }
	
    status = gBS->LocateProtocol( &gQcomPmicGpioProtocolGuid, NULL, (VOID **)&PmicGPIOProtocol);
    if (EFI_ERROR (status)){
      AsciiPrint( "ConfigPMICGPIO: PMIC GPIO Locate Protocol failed\r\n");
      goto exit;
     }
	
    status = gBS->LocateProtocol(&gEfiClockProtocolGuid, NULL, (VOID **)&mClockProtocol);
    if (EFI_ERROR (status)){
      AsciiPrint("  ConfigClocks : %a returned %r \n", gBS->LocateProtocol, status);
      goto exit;
     }

    // Turn on camera subsystem gcc clocks
    AsciiPrint("1/20__Turing on Gcc clocks\n" );
    status = Init_GccClocks();
    if (EFI_SUCCESS != status){
      AsciiPrint("  : %a returned %r \n", Init_Clocks, status);
      goto exit;
    
    }else{
      AsciiPrint("  ...PASS\n" );
     }  


	  // enable CAM0_RST_N pin
    AsciiPrint("2/20__enable CAM0_RST_N pin \n" );
    status = TLMMProtocol->ConfigGpio(TLMM_GPIO_nCAM_RST, TLMM_GPIO_ENABLE);
    if (EFI_ERROR (status)){
      AsciiPrint("    : %a returned %r \n", TLMMProtocol->ConfigGpio, status);
      goto exit;
    }else{
       AsciiPrint("    ...PASS\n" );
     }
	 
	  // enable CAM_ELDO9_EN pin
    AsciiPrint("3/20__enable CAM_ELDO9_EN pin \n" );
    status = TLMMProtocol->ConfigGpio(TLMM_GPIO_nCAM_ELDO9_EN, TLMM_GPIO_ENABLE);
    if (EFI_ERROR (status)){
      AsciiPrint("    : %a returned %r \n", TLMMProtocol->ConfigGpio, status);
      goto exit;
    }else{
       AsciiPrint("    ...PASS\n" );
     }
	 
	  // enable CAM_ELDO3_EN pin
    AsciiPrint("4/20__ enable CAM_ELDO3_EN pin\n" );
    status = TLMMProtocol->ConfigGpio(TLMM_GPIO_nCAM_ELDO3_EN, TLMM_GPIO_ENABLE);
    if (EFI_ERROR (status)){
      AsciiPrint("    : %a returned %r \n", TLMMProtocol->ConfigGpio, status);
      goto exit;
    }else{
       AsciiPrint("    ...PASS\n" );
     }

    // Set CAM0_RST_N line low                       
    AsciiPrint("5/20__Set CAM0_RST_N pin low\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_RST, GPIO_LOW_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

    // wait for 1 milliseconds
    gBS->Stall(1000);

    // Set CAM_ELDO9_EN line high                       
    AsciiPrint("6/20__Set CAM_ELDO9_EN pin high\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_ELDO9_EN, GPIO_HIGH_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

    // wait for 1 milliseconds
    gBS->Stall(1000);	
	
    //  Turn on rear camera sensor power rail, ELDO1
    AsciiPrint("7/20__Turning on back camera ELDO1 rails\n" );
    status = PmicGPIOProtocol->ConfigDigitalOutput (0,					//PmicDeviceIndex
													EFI_PM_GPIO_12,						            //Gpio
													EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,   //OutBufferConfig
													EFI_PM_GPIO_VIN1,					            //VoltageSource
													EFI_PM_GPIO_SOURCE_GND,				        //Source
													EFI_PM_GPIO_OUT_BUFFER_HIGH,		      //OutBufferStrength
													1									                    //OutInversion
													);
	  if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }
	
    //  Turn on rear camera sensor power rail, LVS1
    AsciiPrint("8/20__Turning on back camera LVS1 rails\n" );
    status = PowerOnCameraSensorRail();
    if (EFI_ERROR (status)){
      AsciiPrint("  : %a returned %r \n", PowerOnCameraSensorRail, status);
      goto exit;
    }else{
      AsciiPrint("  ...PASS\n" );
     }

    // wait for 5 milliseconds
    gBS->Stall(5000);
	
    // configure CAM_CCI_SDA0
    AsciiPrint("9/20__Configuring CAM_CCI_SDA0\n" );
    status = TLMMProtocol->ConfigGpio(TLMM_GPIO_nCAM_CCI_SDA0, TLMM_GPIO_ENABLE);
    if (EFI_ERROR (status)){
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->ConfigGpio, status);
      goto exit;
    }else{
      AsciiPrint("  ...PASS\n" );
     }
	
    // wait for 1 milliseconds
    gBS->Stall(1000);

    // Set CAM_CCI_SDA0 line high                       
    AsciiPrint("10/20__Set CAM_CCI_SDA0 pin high\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_CCI_SDA0, GPIO_HIGH_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

    // configure CAM_CCI_SCL0
    AsciiPrint("11/20__Configuring CAM_CCI_SCL0\n" );
    status = TLMMProtocol->ConfigGpio(TLMM_GPIO_nCAM_CCI_SCL0,TLMM_GPIO_ENABLE);
    if (EFI_ERROR (status)){
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->ConfigGpio, status);
      goto exit;
    }else{
      AsciiPrint("  ...PASS\n" );
     }
	
    // wait for 1 milliseconds
    gBS->Stall(1000);

    // Set CAM_CCI_SCL0 line high                       
    AsciiPrint("12/20__Set CAM_CCI_SCL0 pin high\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_CCI_SCL0, GPIO_HIGH_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

    // configure CAM_MCLK0
    AsciiPrint("13/20__Configuring CAM_MCLK\n" );
    status = TLMMProtocol->ConfigGpio(TLMM_GPIO_nCAM_MCLK0,TLMM_GPIO_ENABLE);
    if (EFI_ERROR (status)){
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->ConfigGpio, status);
      goto exit;
    }else{
      AsciiPrint("  ...PASS\n" );
     }
	
    // wait for 1 milliseconds
    gBS->Stall(1000);

    // Set CAM_MCLK0 line high                       
    AsciiPrint("14/20__Set CAM_MCLK0 pin high\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_MCLK0, GPIO_HIGH_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

     // Turn on camera subsystem FOOTSWITCH
     AsciiPrint("15/20__Turing on power domain\n" );
     status = Init_PowerDomains();
     if (EFI_ERROR (status)){
       AsciiPrint("  : %a returned %r \n", Init_PowerDomains, status);
       goto exit;
     }else{
       AsciiPrint("  ...PASS\n" );
     }

    // wait for 1 milliseconds
    gBS->Stall(1000);

    // Set CAM_ELDO3_EN line high                       
    AsciiPrint("16/20__Set CAM_ELDO3_EN pin high\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_ELDO3_EN, GPIO_HIGH_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

    // wait for 1 milliseconds
    gBS->Stall(1000);

    // Turn on camera subsystem clocks
    AsciiPrint("17/20__Turing on clocks\n" );
    status = Init_Clocks();
    if (EFI_SUCCESS != status){
      AsciiPrint("  : %a returned %r \n", Init_Clocks, status);
      goto exit;
    }else{
      AsciiPrint("  ...PASS\n" );
     }    

    // wait for 1 milliseconds
    gBS->Stall(1000);

    // Set CAM0_RST_N line high
    AsciiPrint("18/20__Set Reset_N pin high\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_RST, GPIO_HIGH_VALUE);
    if (EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n",  TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint("  ...PASS\n" );
     }

    // wait for 18 milliseconds
    gBS->Stall(18000);

    // Sanity check CCI by reading CCI version register 
    AsciiPrint("19/20__SanityCheck_CCI\n" );
    //status = SanityCheck_CCI();
    if (EFI_ERROR (status)){
      AsciiPrint("  : %a returned %r \n", SanityCheck_CCI, status);
      goto exit;
    }else{
      AsciiPrint("  ...PASS\n" );
     }

 exit:
    return status;

}

void
DeInit_CamRearSensor (VOID)
{
    EFI_STATUS status = EFI_SUCCESS;
    EFI_TLMM_PROTOCOL       *TLMMProtocol = NULL;
    EFI_QCOM_PMIC_GPIO_PROTOCOL           *PmicGPIOProtocol = NULL;

    status = gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (VOID **)&TLMMProtocol);
    if (EFI_ERROR (status)){
      AsciiPrint( "ConfigTLMMGPIO: TLMM Locate Protocol failed\r\n");
      goto exit;
    }
	
    status = gBS->LocateProtocol( &gQcomPmicGpioProtocolGuid, NULL, (VOID **)&PmicGPIOProtocol);
    if (EFI_ERROR (status)){
      AsciiPrint( "ConfigPMICGPIO: PMIC GPIO Locate Protocol failed\r\n");
      goto exit;
    }
	
    AsciiPrint("Disable clocks\n" );
    DeInit_Clocks();

    // wait for 1 milliseconds
    gBS->Stall(1000);
	
    // Set CAM0_RST_N line low                       
    AsciiPrint("Set CAM0_RST_N pin low\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_RST, GPIO_LOW_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
    }

    // wait for 1 milliseconds
    gBS->Stall(1000);
		
    AsciiPrint("Turning off power rails\n" );
    PowerOffCameraSensorRail();

    // wait for 1 milliseconds
    gBS->Stall(1000);
  
    //  Turn on rear camera sensor power rail, ELDO1
    AsciiPrint("Turning off back camera ELDO1 rails\n" );
    status = PmicGPIOProtocol->ConfigDigitalOutput (0,          //PmicDeviceIndex
                          EFI_PM_GPIO_12,                       //Gpio
                          EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,   //OutBufferConfig
                          EFI_PM_GPIO_VIN1,                     //VoltageSource
                          EFI_PM_GPIO_SOURCE_GND,               //Source
                          EFI_PM_GPIO_OUT_BUFFER_HIGH,          //OutBufferStrength
                          0                                     //OutInversion
                          );
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

    // wait for 1 milliseconds
    gBS->Stall(1000);
	
    // Set CAM_ELDO3_EN line low                       
    AsciiPrint("Set CAM_ELDO3_EN pin low\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_ELDO3_EN, GPIO_LOW_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

    // wait for 1 milliseconds
    gBS->Stall(1000);
  
    // Set CAM_ELDO9_EN line low                       
    AsciiPrint("Set CAM_ELDO9_EN pin low\n" );
    status = TLMMProtocol->GpioOut((UINT32)TLMM_GPIO_nCAM_ELDO9_EN, GPIO_LOW_VALUE);
    if(EFI_ERROR (status))
    {
      AsciiPrint("  : %a returned %r \n", TLMMProtocol->GpioOut, status);
      goto exit;
    }else{
      AsciiPrint ( "  ...PASS!\n");
     }

    // wait for 1 milliseconds
    gBS->Stall(1000);
	
	
    AsciiPrint("Switch off power domains\n" );
    DeInit_PowerDomains();

    AsciiPrint("Switch off gcc clocks\n" );
    DeInit_GccClocks();

 exit:
    return;
}

EFI_STATUS
EFIAPI
Imx318TestMain (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
    )
{	  
  EFI_STATUS status = EFI_SUCCESS;

  AsciiPrint("Start testing rear camera sensor, IMX318\n" ); 
  
  status = Init_CamRearSensor();

  if (EFI_ERROR (status)){
     AsciiPrint(": %a returned %r...Continue\n", Init_CamRearSensor, status); 
  }
  else{
    
    AsciiPrint("20/20__Reading camera sensor register\n" );
    status = ReadCameraSensorRegister();
    if (EFI_ERROR (status)){
      AsciiPrint(": %a returned %r \n", ReadCameraSensorRegister, status);
      goto exit;
    }else{
      AsciiPrint("  ...PASS\n" );
    }
  }

exit:

  AsciiPrint("Camera sensor test  %a\n", (EFI_ERROR (status)) ? "FAILED":"PASSED" );
  AsciiPrint("========================================\n" );

  AsciiPrint("Ending test\n" ); 
  DeInit_CamRearSensor();
  AsciiPrint("Exit...\n" ); 
  
  return status;
}


UINT32 CameraConnectivityTestMain (uint32 dwParam, char *apszParam[])
{	  
  return Imx318TestMain(NULL, NULL);
}
