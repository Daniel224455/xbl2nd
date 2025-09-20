#ifndef __AXICFGOS_H__
#define __AXICFGOS_H__
/**
 * @file axicfgos.h
 * @note Internal header file. Platform independent supporting API
 * 
 *                REVISION  HISTORY
 *  
 * This section contains comments describing changes made to this file. Notice
 * that changes are listed in reverse chronological order.
 * 
 * 
 * when         who     what, where, why
 * ----------   ---     ---------------------------------------------------
 * 09/15/2017   sds     Add fetch for sleep ff mask
 * 06/26/2017   sds     Add Deinit
 * 05/03/2016   sds     Add support for power domains during configuration
 * 03/12/2014   sds     Change the hardware base address to a uint8_t*.
 * 12/05/2011   dj      Created file
 *  
 *  
 * Copyright (c) 2011-2017 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
 */ 

#include <stdint.h>
#include <stdbool.h>
#include "com_dtypes.h" /* for ClockDefs.h */
#include "ClockDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @brief  Used to initialize platform dependent resources
  
  @return bool
      @retval true  if successfully initialized
      @retval false if initialization failed
 */
bool AxiCfgOs_Init( void );

/**
  @brief  Used to de-initialize platform dependent resources
  
  @return bool
      @retval true  if successfully initialized
      @retval false if initialization failed
 */
void AxiCfgOs_Deinit( void );

/**
  @brief  Used to acquire the mutex. 
          Can only be called after AxiCfgOs_Init
 */
void AxiCfgOs_MutexLock( void ); 

/**
  @brief  Used to release the mutex acquired with AxiCfgOs_MutexLock
 */
void AxiCfgOs_MutexUnlock( void ); 

/**
  @brief  Used to map a hardware register base
          Can only be called after AxiCfgOs_Init

  @param  pBaseName   A char string that gives the register base name
  @param  pBaseAddr   The physical base address
  @param  uRegionSize The size of the register base area

  @return uint8_t *
          @retval NULL      if mapping failed
          @retval virtaddr  if mapping succeeded
 */
uint8_t *AxiCfgOs_Mmap( const char * pBaseName, uint8_t *pBaseAddr, uint32_t uRegionSize );

/**
  @brief  Used to fetch a value from xml property file in target config lib
          Can only be called after AxiCfgOs_Init

  @param  pString   Name of the property string

  @return void*   A pointer to the data
  */
void* AxiCfgOs_GetProperty( const char * pString );

/*------------------------------ Clock Control ------------------------------*/
/**
  @brief  Used to fetch the clock id for a non-npa clock.

  @param  pName   The name of the clock
  @param  uId     A pointer to where to store the clock id

  @return bool
      @retval true  if successful
      @retval false if failed
  */
bool AxiCfgOs_GetClockId( const char * pName, ClockIdType *puId );

/**
  @brief  Enable the given non-npa clock

  @param  uId   The clock id for the clock to enable
  */
void AxiCfgOs_EnableClock( ClockIdType uId );

/**
  @brief  Disable the given non-npa clock

  @param  uId   The clock id for the clock to disable
  */
void AxiCfgOs_DisableClock( ClockIdType uId );

/*--------------------------- Power Domain Control ---------------------------*/
/**
  @brief  Used to fetch the power domain id.

  @param  pName   The name of the power domain
  @param  uId     A pointer to where to store the power domain id

  @return bool
      @retval true  if successful
      @retval false if failed
  */
bool AxiCfgOs_GetPowerDomainId( const char * pName, ClockPowerDomainIdType * puId );

/**
  @brief  Enable the given power domain

  @param  uId   The power domain id for the power domain to enable
  */
void AxiCfgOs_EnablePowerDomain( ClockPowerDomainIdType uId );

/**
  @brief  Disable the given power domain

  @param  uId   The power domain id for the power domain to disable
  */
void AxiCfgOs_DisablePowerDomain( ClockPowerDomainIdType uId );

/*------------------------------ Activity Notification ------------------------------*/
/**
 @brief Indicate active sleep requests
 
 @remarks
    This API is refcounted, so individual clients (ul_arb, standalone nodes)
    should only call this API on inactive->active transitions.
 */
void AxiCfgOs_ApplyActive( void );

/**
 @brief Remove active sleep indication
 
 @remarks
    This API is refcounted, so individual clients (ul_arb, standalone nodes)
    should only call this API on active->inactive transitions.
 */
void AxiCfgOs_RemoveActive( void );

/*------------------------------ Miscellaneous ------------------------------*/
/**
 @brief Fetch wake set fire-and-forget mask

 @returns Mask of VCDs to issue as fire-and-forget in WAKE set
*/
uint32_t AxiCfgOs_GetWakeSetFFMask( void );
#ifdef __cplusplus
}
#endif

#endif /* __AXICFGOS_H__ */
