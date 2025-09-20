/*==================================================================================================

FILE: HSUartXBLCoreOs.c

DESCRIPTION: This module provides the coreos based functionalities for the HS-UART.

Copyright (c) 2018 Qualcomm Technologies, Inc.
        All Rights Reserved.
Qualcomm Technologies, Inc. Confidential and Proprietary.

==================================================================================================*/
/*==================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
   hsuart_busy_wait
   hsuart_clock_open
   hsuart_clock_close
   hsuart_get_driver_properties
   hsuart_get_properties
   hsuart_interrupt_close
   hsuart_interrupt_done
   hsuart_interrupt_open
   hsuart_tlmm_open
   hsuart_tlmm_close
   hsuart_busy_wait

==================================================================================================*/
/*==================================================================================================
Edit History


when       who     what, where, why
--------   ---     --------------------------------------------------------
11/16/18   NM       Initial revision

==================================================================================================*/

/*-------------------------------------------------------------------------
* Include Files
* ----------------------------------------------------------------------*/
#include "HSUartOs.h"
#include "HSUartApi.h"
#include "HSUartDefs.h"
#include "HSUartLog.h"
#include "HSUartSettings.h"

// For Wait implementation
#include "DALSys.h"
#include "DDIClock.h"

// For GPIO enable/disable 
#include "DalDevice.h"
#include "DDITlmm.h"


#define HSUART_LOGGING_FILE_ID 40

/*==================================================================================================
                                              TYPEDEFS
==================================================================================================*/

#define INTLOCK()

/*==================================================================================================
                                          LOCAL VARIABLES
==================================================================================================*/

static DalDeviceHandle *clock_handle = NULL;
static DalDeviceHandle *hTlmm = NULL;

/*==================================================================================================
                                          GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================

FUNCTION: hsuart_busy_wait

DESCRIPTION:

==================================================================================================*/
void hsuart_busy_wait(uint32 usecs)
{
   DALSYS_BusyWait(usecs);
}

/*==================================================================================================

FUNCTION: hsuart_clock_close

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_clock_close(hsuart_context* h)
{
   DALResult   res;
   hsuart_result result = HSUART_SUCCESS;
   ClockIdType se_id;
   ClockIdType common_clock_id;
   uint8       **clocks;

   if (clock_handle == NULL) { return HSUART_ERROR; }

   res = DalClock_GetClockId(clock_handle, (const char *) (*(h->properties->core_clock_id)), &se_id);
   if (DAL_SUCCESS != res) { 
      result = HSUART_ERROR;
	  goto exit;
   }

   res = DalClock_DisableClock(clock_handle, se_id);
   if (DAL_SUCCESS != res) { 
      result = HSUART_ERROR;
	  goto exit; 
   }

   clocks = h->properties->common_clock_id;
   while (*clocks != NULL)
   {
      res = DalClock_GetClockId(clock_handle, (const char *) (*clocks), &common_clock_id);
      if (DAL_SUCCESS != res) { 
         result = HSUART_ERROR;
         goto exit; 
      }

      res = DalClock_DisableClock(clock_handle, common_clock_id);
      if (DAL_SUCCESS != res) { 
         result = HSUART_ERROR;
         goto exit; 
      }

      clocks++;
   };
    
   res = DAL_DeviceDetach(clock_handle);
   if ((DAL_SUCCESS != res) || (NULL == clock_handle)) {
      result = HSUART_ERROR;
      goto exit; 
   }
    
exit:	
   return result; 
}

/*==================================================================================================

FUNCTION: hsuart_clock_open

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_clock_open(hsuart_context* h, uint32 input_freq)
{
   DALResult res = TRUE ;
   hsuart_result result = HSUART_SUCCESS;
   ClockIdType common_clock_id;
   ClockIdType se_id;
   uint8       **clocks;
	
   if (!h) { 
      res = HSUART_ERROR;
      goto exit; 
   }
	
   /* Fails to attach to clocks, they are not available */
   res = DAL_ClockDeviceAttach(DALDEVICEID_CLOCK, &clock_handle);
   if ((DAL_SUCCESS != res) || (NULL == clock_handle)) {
      result = HSUART_ERROR;
      goto exit; 
   }

   clocks = h->properties->common_clock_id;
   while (*clocks != NULL)
   {
      res = DalClock_GetClockId(clock_handle, (const char *) (*clocks), &common_clock_id);
      if (DAL_SUCCESS != res) { 
         result = HSUART_ERROR;
         goto exit; 
      }
      res = DalClock_EnableClock(clock_handle, common_clock_id);
      if (DAL_SUCCESS != res) { 
         result = HSUART_ERROR;
         goto exit; 
      }

      clocks++;
   };
    		
   res = DalClock_GetClockId(clock_handle, (const char *) (*(h->properties->core_clock_id)), &se_id);
   if (DAL_SUCCESS != res) { 
      result = HSUART_ERROR;
      goto exit; 
   }

   res = DalClock_SetClockFrequency(clock_handle, se_id, input_freq, CLOCK_FREQUENCY_HZ_EXACT, NULL);
   if (DAL_SUCCESS != res) { 
      result = HSUART_ERROR;
      goto exit; 
   }
   
   res = DalClock_EnableClock(clock_handle, se_id);
   if (DAL_SUCCESS != res) { 
      result = HSUART_ERROR;
      goto exit; 
   }
	
exit:
   if (HSUART_SUCCESS != result) {
      hsuart_clock_close(h);
   }
   
   return result;
}

/*==================================================================================================

FUNCTION: hsuart_get_driver_propertiess

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_get_driver_properties( void )
{
   return HSUART_SUCCESS;
}

/*==================================================================================================

FUNCTION: hsuart_get_properties

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_get_properties(hsuart_context* h)
{
   h->properties = hsuart_settings_get_properties(h->port_id);
   return HSUART_SUCCESS;

}
/*==================================================================================================

FUNCTION: hsuart_interrupt_close

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_interrupt_close(hsuart_context* h)
{
    return HSUART_SUCCESS;
}

/*==================================================================================================

FUNCTION: hsuart_interrupt_done

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_interrupt_done(hsuart_context* h)
{
    return HSUART_SUCCESS;
}

/*==================================================================================================

FUNCTION: hsuart_interrupt_open

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_interrupt_open(hsuart_context* h, void* isr)
{
   return HSUART_SUCCESS;
}

/*==================================================================================================

FUNCTION: hsuart_tlmm_close

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_tlmm_close(hsuart_context* h)
{
   hsuart_result result = HSUART_SUCCESS;
   DALResult dalRes = DAL_SUCCESS;
   
   if (!h) {
      result = HSUART_ERROR;
      goto exit;
   }
   
   dalRes = DalDevice_Close(hTlmm);
   if (DAL_SUCCESS != dalRes) {
      result = HSUART_SUCCESS; // Incase OPEN had failed #ag.tbd
   }
   
   dalRes = DAL_DeviceDetach(hTlmm);
   if (DAL_SUCCESS != dalRes) {
      result = HSUART_ERROR;
      goto exit;
   }
   
exit:
   return result;
}

/*==================================================================================================

FUNCTION: hsuart_tlmm_open

DESCRIPTION:

==================================================================================================*/
hsuart_result hsuart_tlmm_open(hsuart_context* h)
{
   hsuart_result result = HSUART_SUCCESS;
   DALResult dalRes = DAL_SUCCESS;
   
   if (!h) {
      result = HSUART_ERROR;
      goto exit;
   }
   
   dalRes = DAL_DeviceAttachEx(NULL, DALDEVICEID_TLMM, DALTLMM_INTERFACE_VERSION, &hTlmm);
   if ((DAL_SUCCESS != dalRes) || (NULL == hTlmm)) {
      result = HSUART_ERROR;
      goto exit;
   }
   
   dalRes = DalDevice_Open(hTlmm, DAL_OPEN_SHARED);
   if (DAL_SUCCESS != dalRes) {
      result = HSUART_ERROR;
      goto exit;
   }
	
   dalRes = DalTlmm_ConfigGpio(hTlmm, h->properties->gpio_tx_config, DAL_TLMM_GPIO_ENABLE);
   if (DAL_SUCCESS != dalRes) {
      result = HSUART_ERROR;
      goto exit;
   }
	
   dalRes = DalTlmm_ConfigGpio(hTlmm, h->properties->gpio_rx_config, DAL_TLMM_GPIO_ENABLE);
   if (DAL_SUCCESS != dalRes) {
      result = HSUART_ERROR;
      goto exit;
   }
   
   if (h->properties->gpio_cts_config) {
      dalRes = DalTlmm_ConfigGpio(hTlmm, h->properties->gpio_cts_config, DAL_TLMM_GPIO_ENABLE);
      if (DAL_SUCCESS != dalRes) {
         result = HSUART_ERROR;
         goto exit;
      }
   }
	
   if (h->properties->gpio_rfr_config) {
      dalRes = DalTlmm_ConfigGpio(hTlmm, h->properties->gpio_rfr_config, DAL_TLMM_GPIO_ENABLE);
      if (DAL_SUCCESS != dalRes) {
         result = HSUART_ERROR;
         goto exit;
      }
   }
   
exit:
   if (HSUART_SUCCESS != result) {
      hsuart_tlmm_close(h);
   }
   
   return result;
}
