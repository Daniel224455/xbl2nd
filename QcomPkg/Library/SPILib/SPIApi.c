/** @file SPIApi.c
   
  SPI API code

  Copyright (c) 2014, , 2018 Qualcomm Technologies, Inc. All Rights Reserved
   
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 09/23/14   sg     Created

=============================================================================*/

#include "DALDeviceId.h"
#include <DALSys.h>
#include <DALDeviceId.h>
#include <SpiDevice.h>
#include <SpiDalProps.h>

/* SPI Interfaces */
#include "SpiApi.h"
#include "SpiDeviceOsSvc.h"

/**
  List of SPI DAL Device IDs
 */
static UINT32 SPIDalDeviceList [] =
{
  DALDEVICEID_SPI_DEVICE_1,
  DALDEVICEID_SPI_DEVICE_2,
  DALDEVICEID_SPI_DEVICE_3,
  DALDEVICEID_SPI_DEVICE_4,
  DALDEVICEID_SPI_DEVICE_5,
  DALDEVICEID_SPI_DEVICE_6,
  DALDEVICEID_SPI_DEVICE_7,
  DALDEVICEID_SPI_DEVICE_8,
  DALDEVICEID_SPI_DEVICE_9,
  DALDEVICEID_SPI_DEVICE_10,
  DALDEVICEID_SPI_DEVICE_11,
  DALDEVICEID_SPI_DEVICE_12
};

/* spi_open */ 
spi_status spi_open( spi_instance instance, void **spi_handle )
{
	spi_status status = SPI_SUCCESS;
	INT32 res = 0;
	SpiDevice_Config config;
	
	if ((instance >= SPI_INSTANCE_MAX) ||
		(NULL == spi_handle))
	{
		status = SPI_ERROR_INVALID_PARAMETER;
	}

	if (SPI_SUCCESS == status)
	{
		IBUSOS_Memset(&config, 0x0, sizeof(config));
		config.QupHandle = (void *) (UINTN) SPIDalDeviceList[instance];

		res = SpiDevProp_GetConfig(config.QupHandle, &config);

		if(res == SPI_SUCCESS)
		{	
			res = SpiDevice_Init (&config, (SPIDEVICE_HANDLE *) spi_handle);
    
			if (res != SPIDEVICE_RES_SUCCESS)
			{
				status = SPI_ERROR_INIT;
			}
			else
			{
				res = SpiDevice_SetPowerState ((SPIDEVICE_HANDLE) *spi_handle, SPIDEVICE_POWER_STATE_2);
			
				if (res != SPIDEVICE_RES_SUCCESS)
				{
					status = SPI_ERROR_POWER_ON;
				}
			}
		}
	}
	
	return status;
}



/**
  QUP_SPI_Transfer ()

  @brief
  Transfer implementation of EFI_QCOM_SPI_PROTOCOL
 */
spi_status spi_transfer(void	*spi_handle,
						SpiDeviceInfoType	*devInfo,
						CONST UINT8	*write_buffer,
						UINT32	write_len,
						UINT8	*read_buffer,
						UINT32	read_len)
{
  SpiDataAddrType SpiReadAddrBuff, SpiWriteAddrBuff;
  SpiDeviceInfoType SpiDeviceInfo;
  spi_status status = SPI_SUCCESS;
  INT32 res = 0;

  if ((NULL == spi_handle) ||
     ((NULL == write_buffer) &&
      (NULL == read_buffer)) ||
      (NULL == devInfo))
  {
    return SPI_ERROR_INVALID_PARAMETER;
  }

  SpiDeviceInfo.deviceParameters.eClockMode =
    (SpiClockModeType) devInfo->deviceParameters.eClockMode;

  SpiDeviceInfo.deviceParameters.u32DeassertionTime =
    devInfo->deviceParameters.u32DeassertionTime;

  SpiDeviceInfo.deviceParameters.u32MinSlaveFrequencyHz =
    devInfo->deviceParameters.u32MinSlaveFrequencyHz;

  SpiDeviceInfo.deviceParameters.u32MaxSlaveFrequencyHz =
    devInfo->deviceParameters.u32MaxSlaveFrequencyHz;

  SpiDeviceInfo.deviceParameters.eCSPolarity =
    (SpiCSPolarityType) devInfo->deviceParameters.eCSPolarity;

  SpiDeviceInfo.deviceParameters.eCSMode =
    (SpiCSModeType) devInfo->deviceParameters.eCSMode;
 
  SpiDeviceInfo.deviceParameters.bHSMode =
    devInfo->deviceParameters.bHSMode;
	
  SpiDeviceInfo.deviceBoardInfo.nSlaveNumber =
    devInfo->deviceBoardInfo.nSlaveNumber;

  SpiDeviceInfo.deviceBoardInfo.eCoreMode =
    (SpiCoreModeType) devInfo->deviceBoardInfo.eCoreMode;

  SpiDeviceInfo.transferParameters.nNumBits =
    devInfo->transferParameters.nNumBits;

  SpiDeviceInfo.transferParameters.eLoopbackMode =
    (SpiLoopbackModeType) devInfo->transferParameters.eLoopbackMode;

  SpiDeviceInfo.deviceParameters.eClockPolarity = devInfo->deviceParameters.eClockPolarity;
  SpiDeviceInfo.deviceParameters.eShiftMode = devInfo->deviceParameters.eShiftMode;
  

  SpiWriteAddrBuff.virtualAddr = (void *) write_buffer;
  SpiWriteAddrBuff.physicalAddr = (void *) write_buffer;

  SpiReadAddrBuff.virtualAddr = (void *) read_buffer;
  SpiReadAddrBuff.physicalAddr = (void *) read_buffer;

  res = SpiDevice_WriteRead (spi_handle,
          &SpiDeviceInfo,
          &SpiReadAddrBuff,
          read_len,
          &SpiWriteAddrBuff,
          write_len);

  if (res != SPIDEVICE_RES_SUCCESS)
  {
    status = SPI_ERROR_TRANSFER;
  }

  return status;
}



/* spi_close */ 
spi_status spi_close( void *spi_handle )
{
	spi_status status = SPI_SUCCESS;
	INT32 res = 0;

	res = SpiDevice_SetPowerState ((SPIDEVICE_HANDLE) spi_handle, SPIDEVICE_POWER_STATE_0);
	
	if (res != SPIDEVICE_RES_SUCCESS)
	{
      status = SPI_ERROR_POWER_OFF;
	}

	res = SpiDevice_DeInit (spi_handle);
	if (res != SPIDEVICE_RES_SUCCESS)
	{
		status = SPI_ERROR_DEINIT;
	}

	return status;
}