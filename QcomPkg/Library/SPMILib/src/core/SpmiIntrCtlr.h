#ifndef SPMI_INTR_CTLR_H
#define SPMI_INTR_CTLR_H
/*=========================================================================*/
/**
@file SpmiIntrCtlr.h

This module provides the interface to the SPMI Interrupt Controller
software. 
*/ 
/*=========================================================================

Edit History


when       who     what, where, why
--------   ---     -------------------------------------------------------- 
02/11/16   MJS     Binary compatibility changes.
07/10/15   LK      Removed unused API SpmiIntrCtlr_GetAccumulatedIntrStatus.
08/28/12   PS      Added support to specify intrId as PPID. And support to
                   register for individual Extended Intr bits within a
                   peripheral 
07/05/12   PS      Initial revision.

===========================================================================
             Copyright (c) 2012, 2015, 2016 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================
*/

/*-------------------------------------------------------------------------
* Include Files
* ----------------------------------------------------------------------*/
#include "SpmiTypes.h"

/*-------------------------------------------------------------------------
* Function Declarations and Documentation
* ----------------------------------------------------------------------*/
/**
@brief Enables an Interrupt  

This function enables an Interrupt on SPMI Interrupt 
Controller

@param[in] periphID Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@return  SPMI_SUCCESS on success, error code on error

@see SpmiIntrCtlr_InterruptDisable()
*/
Spmi_Result SpmiIntrCtlr_InterruptEnable(SpmiPhyPeriphIdType periphID);

/**
@brief Disables an Interrupt  

This function disables an Interrupt on SPMI Interrupt 
Controller

@param[in] periphID Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@return  SPMI_SUCCESS on success, error code on error 

@see SpmiIntrCtlr_InterruptEnable()
*/
Spmi_Result  SpmiIntrCtlr_InterruptDisable(SpmiPhyPeriphIdType periphId);


/**
@brief Is Interrupt Enabled  

This function returns whether or not an Interrupt is enabled 
on SPMI Interrupt Controller 

@param[in] periphID Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@param[out] bState  Pointer to a variable which will hold the 
return value of TRUE or FALSE

@return  SPMI_SUCCESS on success, error code on error 

@see SpmiIntrCtlr_InterruptEnable(), 
SpmiIntrCtlr_InterruptDisable()
*/
Spmi_Result  SpmiIntrCtlr_IsInterruptEnabled(SpmiPhyPeriphIdType periphId, uint32 *bState);

/* Extension APIs to read peripheral extended interrupts */


/**
@brief Get the Extended Interrupt Status  

This function returns the extended Interrupt Status associated with
the periphId 

@param[in] periphID Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@param[out] puIntrStatusMask Pointer to the uint32 which will be
populated by this function reflecting the status of the extended
interrupts. Each set bit in this parameter, represents that
extended Interrupts is set.

@return  SPMI_SUCCESS on success, error code on error

@see None
*/
Spmi_Result  SpmiIntrCtlr_ExtendedInterruptStatus(SpmiPhyPeriphIdType periphId, uint32 *puIntrStatusMask);


/**
@brief Get the Extended Interrupt Status  

This function clears the extended Interrupt Status associated 
with the periphId 

@param[in] periphID Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@param[in] uIntrClearMask This is a mask of bits which 
reprsents Interrupts to be cleared. 

@return  SPMI_SUCCESS on success, error code on error 

@see None
*/
Spmi_Result  SpmiIntrCtlr_ExtendedInterruptClear(SpmiPhyPeriphIdType periphId, uint32 uIntrClearMask);


/**
@brief Get the Logical(channel id) from a physical peripheral Id
       (PPID)

This function returns the logical id or channel id of a 
peripheral from the given PPID 

@param[in] periphID Physical Peripheral address. 12 bits (4 bit 
                    SID + 8 bit upper peripheral address)

@param[out] pLogicalId Pointer to the uint32 which will be 
                       populated by this function. The value
                       will be the corresponding channel id.

@return SPMI_SUCCESS on success, error code on error

@see None
*/
Spmi_Result SpmiIntrCtlr_GetLogicalIdFromPpid(SpmiPhyPeriphIdType periphID, uint32 *pLogicalId);

#endif /* __SPMI_INTR_CTLR_H__ */
