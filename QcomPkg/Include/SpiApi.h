/** 
  @file  SpiApi.h
  @brief SPI APIs
*/
/*=============================================================================
  Copyright (c) 2014 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 09/23/14   sg     Created

=============================================================================*/

#ifndef __SPIAPI_H__
#define __SPIAPI_H__

#include "SpiDeviceTypes.h"
#include "SpiDevice.h"

/** @addtogroup spi_data_types 
@{ */

/**
  SPI core passed to the spi_open() function. 
*/
typedef enum {
  SPI_INSTANCE_001,          /**< Instance 01. */
  SPI_INSTANCE_002,          /**< Instance 02. */
  SPI_INSTANCE_003,          /**< Instance 03. */
  SPI_INSTANCE_004,          /**< Instance 04. */
  SPI_INSTANCE_005,          /**< Instance 05. */
  SPI_INSTANCE_006,          /**< Instance 06. */
  SPI_INSTANCE_007,          /**< Instance 07. */
  SPI_INSTANCE_008,          /**< Instance 08. */
  SPI_INSTANCE_009,          /**< Instance 09. */
  SPI_INSTANCE_010,          /**< Instance 10. */
  SPI_INSTANCE_011,          /**< Instance 11. */
  SPI_INSTANCE_012,          /**< Instance 12. */

  SPI_INSTANCE_MAX,          /**< Instance Check. */

} spi_instance;

/**
  Function status codes.
*/
typedef enum {
   SPI_SUCCESS,
   /**< Operation succeeded. */
   SPI_ERROR_INVALID_PARAMETER,
   /**< Invalid parameter. */
   SPI_ERROR_INIT,
   /**< Initialization failed. */
   SPI_ERROR_DEINIT,
   /**< De-initialization failed. */
   SPI_ERROR_POWER_ON,
   /**< Power ON sequence failed. */
   SPI_ERROR_POWER_OFF,
   /**< Power OFF sequence failed. */
   SPI_ERROR_BUS_ACQUISITION_TIMEOUT,
   /**< Timeout occurred waiting for the lock. */
   SPI_ERROR_BUSY,
   /**< Unable to obtain the bus lock. */
   SPI_ERROR_OPERATION_DENIED,
   /**< Client has not locked the bus. */
   SPI_ERROR_OPERATION_FAILED,         
   /**< Operation failed. */
   SPI_ERROR_TRANSFER,
   /**< Transfer failed. */
   SPI_ERROR_DRV_FWK_INIT,
   /**< Driver framework specific initiliazation failed. */
   SPI_ERROR_DRV_FWK_NULL_HANDLE
   /**< Driver framework specific handle is invalid. */

} spi_status;


/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
/* spi_open */ 
/** @ingroup spi_open
  @par Summary
  This function is called by the client code to initialize the respective 
  SPI core instance used by the client. On success, spi_handle points to the handle 
  for the SPI core. 
  
  @param[in]  instance              SPI core instance that the client intends to
                                    initialize; see #spi_instance for 
                                    details.
  @param[out] spi_handle            Pointer location to be filled by the
                                    driver with a handle to the core.

  @return
  See #spi_status for details.
*/
spi_status 
spi_open( 
	spi_instance instance, 
	void **spi_handle 
);

/**
  spi_transfer ()

  @brief
  Transfer implementation of EFI_QCOM_SPI_PROTOCOL
 */
spi_status 
spi_transfer( 
		void	*spi_handle,
		SpiDeviceInfoType	*devInfo,
		CONST UINT8	*write_buffer,
		UINT32	write_len,
		UINT8	*read_buffer,
		UINT32	read_len
);


/* spi_close */ 
/** @ingroup spi_close
  @par Summary
  De-initializes the SPI core.

  @param[in] spi_handle             Handle to the SPI core.

  @return
  See #spi_status for details.
*/
spi_status
spi_close(
    void *spi_handle
);

#endif	/* __SPIAPI_H__ */
