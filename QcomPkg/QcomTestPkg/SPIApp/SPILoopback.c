/** @file SPILoopback.c
   
  SPI Loopback App
**/

/*=============================================================================
  Copyright (c) 2010-2014,2019 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 04/19/19   nm      Test Multiple Iterations
 09/23/14   sg      XBL changes.
 05/01/14   sg      Added new device parameter, HS mode .
 09/13/11   unr     APT test support
 06/10/11   unr     Initial revision of SPI loopback test

=============================================================================*/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TestInterface.h>
#include <Library/QcomLib.h>
#include <Library/DebugLib.h>

#include "DALSys.h"

#include <Protocol/EFISPI.h>
#include "SpiDevice.h"


#define DATA_SIZE 512

#define REMINDER(a, b) ((a)-((b)*(UINT32)((a)/(b))))

/** Number of bits. */
#define SPI_NUM_BITS            8
/** Slave number. */
#define SPI_SLAVE_NUMBER        0

/** Minimum slave frequency in Hz. */
#define SPI_MIN_SLAVE_FREQ_HZ   0
/** Maximum slave frequency in Hz. */
#define SPI_MAX_SLAVE_FREQ_HZ   (10 * 1000 * 1000)
/** Deassert time. */
#define SPI_DEASSERT_TIME_IN_NS (50)

UINT8 WriteBuf[DATA_SIZE];
UINT8  ReadBuf[DATA_SIZE];

UINT32 TransferSize = 512;

/* Command Line Types */
typedef enum
{
  SPI_CORE_NUMBER = 0,
  SPI_CS_ASSERT,
  SPI_SPLIT_SIZE,
  SPI_CMD_INVALID,

} CmdOption;

/* Command Parameters */
typedef struct slave_context
{
  UINT32 spi_core_number;
  UINT8  spi_cs_assert;
  UINT32 spi_split_size;

} SlaveContext;

static SlaveContext Sctxt;

/**
  Prints the error strings on console

  @param[in] error          Error code defined in spi_status

  @retval none

 */
VOID SpiErrorPrint(spi_status error)
{
  switch (error)
  {
    case SPI_ERROR_INVALID_PARAM:
      DEBUG((EFI_D_ERROR, "INVALID_PARAMETER\n"));
      break;
    case SPI_ERROR_BUS_ACQUISITION_TIMEOUT:
      DEBUG((EFI_D_ERROR, "BUS_ACQUISITION_TIMEOUT\n"));
      break;
    case SPI_ERROR_BUSY:
      DEBUG((EFI_D_ERROR, "BUSY\n"));
      break;
    case SPI_ERROR_OPERATION_DENIED:
      DEBUG((EFI_D_ERROR, "OPERATION_DENIED\n"));
      break;
    case SPI_ERROR_OPERATION_FAILED:
      DEBUG((EFI_D_ERROR, "OPERATION_FAILED\n"));
      break;
    default:
      break;
  }
}

/**
  SPIConfigure ()

  @brief
  Fills the SPI Device Info Type

  @param[out] SPI_DeviceInit    Pointer to the SPI configuration
 */
static VOID
SPIConfigure (SpiDeviceInfoType *SPI_DeviceInfo, UINT8 spi_cs_assert)
{ 
  SPI_DeviceInfo->deviceParameters.eClockPolarity = SPI_CLK_IDLE_HIGH;
  SPI_DeviceInfo->deviceParameters.eShiftMode = SPI_OUTPUT_FIRST_MODE;
  SPI_DeviceInfo->deviceParameters.eClockMode = SPI_CLK_NORMAL;
  SPI_DeviceInfo->deviceParameters.u32DeassertionTime = SPI_DEASSERT_TIME_IN_NS;
  SPI_DeviceInfo->deviceParameters.u32MinSlaveFrequencyHz = SPI_MIN_SLAVE_FREQ_HZ;
  SPI_DeviceInfo->deviceParameters.u32MaxSlaveFrequencyHz = SPI_MAX_SLAVE_FREQ_HZ;
  SPI_DeviceInfo->deviceParameters.eCSPolarity = SPI_CS_ACTIVE_LOW;
  
  if (spi_cs_assert) SPI_DeviceInfo->deviceParameters.eCSMode = SPI_CS_KEEP_ASSERTED;
  else SPI_DeviceInfo->deviceParameters.eCSMode = SPI_CS_DEASSERT;
  
  /*Loopback is only valid in non-HS mode*/
  SPI_DeviceInfo->deviceParameters.bHSMode = FALSE;  

  SPI_DeviceInfo->deviceBoardInfo.nSlaveNumber = SPI_SLAVE_NUMBER;
  SPI_DeviceInfo->deviceBoardInfo.eCoreMode = SPI_CORE_MODE_MASTER;

  SPI_DeviceInfo->transferParameters.nNumBits = SPI_NUM_BITS;
  /* Enable SPI loopback mode for testing only */
  SPI_DeviceInfo->transferParameters.eLoopbackMode = SPI_LOOPBACK_ENABLED;
}

void SaveSlaveContextFromCommandLine (UINTN argc, CHAR8 **argv)
{
  UINT32 index = 0;

  while (index < argc)
  {
    CmdOption CmdOp = SPI_CMD_INVALID;

    if ((AsciiStriCmp (*(argv + index), "--core") == 0) ||
        (AsciiStriCmp (*(argv + index), "-c") == 0))
    {
      CmdOp = SPI_CORE_NUMBER;
    }
    else if ((AsciiStriCmp (*(argv + index), "--assert") == 0) ||
             (AsciiStriCmp (*(argv + index), "-a") == 0))
    {
      CmdOp = SPI_CS_ASSERT;
    }
	else if ((AsciiStriCmp (*(argv + index), "--split") == 0) ||
             (AsciiStriCmp (*(argv + index), "-s") == 0))
    {
      CmdOp = SPI_SPLIT_SIZE;
    }

    switch (CmdOp)
    {
      case SPI_CORE_NUMBER:
        index++;
        Sctxt.spi_core_number = (UINT32) AsciiStrDecimalToUintn (*(argv + index));
        break;
        
      case SPI_CS_ASSERT:
        index++;
        Sctxt.spi_cs_assert = (UINT8) AsciiStrDecimalToUintn (*(argv + index));
        break;

	  case SPI_SPLIT_SIZE:
        index++;
        Sctxt.spi_split_size = (UINT32) AsciiStrDecimalToUintn (*(argv + index));
        break;

      case SPI_CMD_INVALID:
      default:
        break;
    }
    index++;
  }
}


/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

 **/

EFI_STATUS
EFIAPI
SPITestLoopback (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
    )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_QCOM_SPI_PROTOCOL *SPIIo = NULL;
  VOID  *SPIHandle = NULL;
  SpiDeviceInfoType SPIDevInfo;
  spi_status SPIStatus = SPI_SUCCESS;
  UINT32 i = 0;
  UINT32 split_len = DATA_SIZE;
  UINT32 Error = 0;
  UINTN argc;
  CHAR8 **argv = NULL;

  TEST_START("SPITestLoopback");

  Status = GetCmdLineArgs (ImageHandle, &argc, &argv);
  if (Status == EFI_SUCCESS)
  {
    SaveSlaveContextFromCommandLine (argc, argv);
  }

  Status = gBS->LocateProtocol(&gQcomSPIProtocolGuid, NULL, (VOID **) &SPIIo);
  if (EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_ERROR, "SPI Protocol not found\n"));
    TestStatus("SPITestLoopback",Status);
    TEST_STOP("SPITestLoopback");
    return Status;
  }

  if (NULL == SPIIo)
  {
    DEBUG((EFI_D_ERROR, "SPI Io NULL\n"));
    TestStatus("SPITestLoopback",EFI_DEVICE_ERROR);
    TEST_STOP("SPITestLoopback");
    return EFI_DEVICE_ERROR;
  }

  if (Sctxt.spi_core_number == 0)
  {
      Sctxt.spi_core_number = PcdGet32(SPILoopbackCore);
  }

  Status = SPIIo->open((spi_instance) (Sctxt.spi_core_number - 1),
                       &SPIHandle);
  if (EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_ERROR, "SPI Init failed\n"));
    TestStatus("SPITestLoopback",Status);
    TEST_STOP("SPITestLoopback");
    return Status;
  }

  /* Get the default configuration and set loopback mode */
  SPIConfigure (&SPIDevInfo, Sctxt.spi_cs_assert);

  for (i = 0; i < DATA_SIZE; i++)
  {
      WriteBuf[i] = (i + 1) & 0xFF;
      ReadBuf[i] = 0;
  }
  
  if(Sctxt.spi_split_size) 
  {
  	DALSYS_BusyWait(10000000); // .1 sec
  
  	do {
		if(REMINDER(DATA_SIZE, Sctxt.spi_split_size)==0) break; 
	} while(Sctxt.spi_split_size--);
	
  	split_len = DATA_SIZE/Sctxt.spi_split_size;
  }
  
  for (i = 0; i < (Sctxt.spi_split_size?Sctxt.spi_split_size:1); i++)
  {
      SPIStatus = SPIIo->transfer(SPIHandle,
          &SPIDevInfo,
          WriteBuf+i*split_len,
          split_len,
          ReadBuf+i*split_len,
          split_len);
      if(Sctxt.spi_split_size) DALSYS_BusyWait(1000000); // .1 sec
  }

  if (SPI_SUCCESS != SPIStatus)
  {
    DEBUG((EFI_D_ERROR, "SPI Transfer failed\n"));
    SpiErrorPrint(SPIStatus);
  }

  for (i = 0; i < TransferSize; i++)
  {
    if (ReadBuf[i] != WriteBuf[i])
    {
      DEBUG((EFI_D_ERROR, "SPI ERROR Unexpected data\n"));
      Error = 1;
      break;
    }
  }

  Status = SPIIo->close(SPIHandle);
  if (EFI_SUCCESS != Status)
  {
    DEBUG((EFI_D_ERROR, "SPI Deinit failed\n"));
    TestStatus("SPITestLoopback",Status);
    TEST_STOP("SPITestLoopback");
    return Status;
  }

  if (Error)
  {
    DEBUG((EFI_D_ERROR, "SPI Loopback Test FAILED\n"));
    TestStatus("SPITestLoopback",EFI_DEVICE_ERROR);
    TEST_STOP("SPITestLoopback");
  }
  else
  {
    DEBUG((EFI_D_ERROR, "SPI Loopback Test PASSED\n"));
    TestStatus("SPITestLoopback",EFI_SUCCESS);
    TEST_STOP("SPITestLoopback");
  }
  return EFI_SUCCESS;
}
