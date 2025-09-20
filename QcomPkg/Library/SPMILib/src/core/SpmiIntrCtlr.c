/*=========================================================================*/
/**
@file SpmiIntrCtlr.c

This module provides the driver to the SPMI Interrupt Controller
hardware.
*/ 
/*=========================================================================

Edit History


when       who     what, where, why
--------   ---     -------------------------------------------------------- 
08/28/12   PS      Added support to specify intrId as PPID. And support to
                   register for individual Extended Intr bits within a
                   peripheral
07/05/12   PS      Initial revision.

===========================================================================
             Copyright (c) 2012-2017 Qualcomm Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
===========================================================================
*/

/*-------------------------------------------------------------------------
* Include Files
* ----------------------------------------------------------------------*/
#include "SpmiIntrCtlr.h"
#include "SpmiUtils.h"
#include "PmicArb.h"
#include "SpmiPic.h"
#include "SpmiBusInternal.h"

/*-------------------------------------------------------------------------
* Externalized Function Definitions
* ----------------------------------------------------------------------*/
/**
@brief Enables an Interrupt  

This function enables an Interrupt on SPMI Interrupt 
Controller

@param[in] periphId Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@return  SPMI_SUCCESS on success, error code on 
error

@see SpmiIntrCtlr_InterruptDisable()
*/
Spmi_Result SpmiIntrCtlr_InterruptEnable(SpmiPhyPeriphIdType periphId)
{
    Spmi_Result spmiRes;
    uint8 slaveId, busId;
    uint16 channel;
    uint8 vSlaveId = PPID_TO_VSLAVE_ID(periphId);
    SpmiDevInfo *pDev = SpmiBus_GetDevInfo(vSlaveId);
    SpmiInfo *pSpmiInfo = SpmiBus_GetInfo();

    if ((pSpmiInfo == NULL) || (pDev == NULL)) {
        return SPMI_FAILURE_INVALID_PARAMETER;     
    }

    spmiDecodeVSid(vSlaveId, &busId, &slaveId);
    
    spmiRes = PmicArb_FindChannel(pDev, slaveId, PPID_TO_PERIPH_ID(periphId), pSpmiInfo->owner, &channel );
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }

    spmiRes = SpmiPic_SetInterruptEnabled(pDev, pSpmiInfo->owner, channel, TRUE);
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }
    return SPMI_SUCCESS;
}

/**
@brief Disables an Interrupt  

This function disables an Interrupt on SPMI Interrupt 
Controller

@param[in] periphId Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@return  SPMI_SUCCESS on success, error code on 
error

@see SpmiIntrCtlr_InterruptEnable()
*/
Spmi_Result SpmiIntrCtlr_InterruptDisable(SpmiPhyPeriphIdType periphId)
{
    Spmi_Result spmiRes;
    uint8 slaveId, busId;
    uint16 channel;
    uint8 vSlaveId = PPID_TO_VSLAVE_ID(periphId);
    SpmiDevInfo *pDev = SpmiBus_GetDevInfo(vSlaveId);
    SpmiInfo *pSpmiInfo = SpmiBus_GetInfo();

    if ((pSpmiInfo == NULL) || (pDev == NULL)) {
        return SPMI_FAILURE_INVALID_PARAMETER;     
    }

    spmiDecodeVSid(vSlaveId, &busId, &slaveId);

    spmiRes = PmicArb_FindChannel(pDev, slaveId, PPID_TO_PERIPH_ID(periphId), pSpmiInfo->owner, &channel );
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }

    spmiRes = SpmiPic_SetInterruptEnabled(pDev, pSpmiInfo->owner, channel, FALSE);
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }
    return SPMI_SUCCESS;
}

/**
@brief Is Interrupt Enabled  

This function returns whether or not an Interrupt is enabled 
on SPMI Interrupt Controller 

@param[in] periphId Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@param[out] bState  Pointer to a variable which will hold the 
return value of TRUE or FALSE

@return  SPMI_SUCCESS on success, error code on 
error

@see SpmiIntrCtlr_InterruptEnable(), 
SpmiIntrCtlr_InterruptDisable()
*/
Spmi_Result SpmiIntrCtlr_IsInterruptEnabled(SpmiPhyPeriphIdType periphId, uint32 *bState)
{
    Spmi_Result spmiRes;
    uint8 slaveId, busId;
    uint16 channel;
    uint8 vSlaveId = PPID_TO_VSLAVE_ID(periphId);
    SpmiDevInfo *pDev = SpmiBus_GetDevInfo(vSlaveId);
    SpmiInfo *pSpmiInfo = SpmiBus_GetInfo();
    
    if (bState == NULL) {
        return SPMI_FAILURE_INVALID_PARAMETER;     
    }

    if ((pSpmiInfo == NULL) || (pDev == NULL)) {
        return SPMI_FAILURE_INVALID_PARAMETER;     
    }

    spmiDecodeVSid(vSlaveId, &busId, &slaveId);

    spmiRes = PmicArb_FindChannel(pDev, slaveId, PPID_TO_PERIPH_ID(periphId), pSpmiInfo->owner, &channel );
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }

    spmiRes = SpmiPic_IsInterruptEnabled(pDev, channel, (boolean*)bState);
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }
    return SPMI_SUCCESS;
}


/* Extension APIs to read peripheral extended interrupts */


/**
@brief Get the Extended Interrupt Status  

This function returns the extended Interrupt Status associated with
the periphId 

@param[in] periphId Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@param[out] puIntrStatusMask Pointer to the uint32 which will be
populated by this function reflecting the status of the extended
interrupts. Each set bit in this parameter, represents that
extended Interrupts is set.

@return  SPMI_SUCCESS on success, error code on 
error

@see None
*/
Spmi_Result SpmiIntrCtlr_ExtendedInterruptStatus(SpmiPhyPeriphIdType periphId, uint32 *puIntrStatusMask)
{
    Spmi_Result spmiRes;
    uint8 slaveId, busId;
    uint16 channel;
    uint8 vSlaveId = PPID_TO_VSLAVE_ID(periphId);
    SpmiDevInfo *pDev = SpmiBus_GetDevInfo(vSlaveId);
    SpmiInfo *pSpmiInfo = SpmiBus_GetInfo();

    if ((pSpmiInfo == NULL) || (pDev == NULL)) {
        return SPMI_FAILURE_INVALID_PARAMETER;     
    }

    spmiDecodeVSid(vSlaveId, &busId, &slaveId);

    spmiRes = PmicArb_FindChannel(pDev, slaveId, PPID_TO_PERIPH_ID(periphId), pSpmiInfo->owner, &channel );
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }

    spmiRes = SpmiPic_ExtendedInterruptStatus(pDev, channel, puIntrStatusMask);
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }
    return SPMI_SUCCESS;
}


/**
@brief Clear the Extended Interrupt Status  

This function clears the extended Interrupt Status associated 
with the periphId 

@param[in] periphId Physical Peripheral address. 12 bits (4 bit
SID + 8 bit upper peripheral address)

@param[in] uIntrClearMask This is a mask of bits which 
reprsents Interrupts to be cleared. 

@return  SPMI_SUCCESS on success, error code on 
error

@see None
*/
Spmi_Result  SpmiIntrCtlr_ExtendedInterruptClear(SpmiPhyPeriphIdType periphId, uint32 uIntrClearMask)
{
    Spmi_Result spmiRes;
    uint8 slaveId, busId;
    uint16 channel;
    uint8 vSlaveId = PPID_TO_VSLAVE_ID(periphId);
    SpmiDevInfo *pDev = SpmiBus_GetDevInfo(vSlaveId);
    SpmiInfo *pSpmiInfo = SpmiBus_GetInfo();

    if ((pSpmiInfo == NULL) || (pDev == NULL)) {
        return SPMI_FAILURE_INVALID_PARAMETER;
    }

    spmiDecodeVSid(vSlaveId, &busId, &slaveId);

    spmiRes = PmicArb_FindChannel(pDev, slaveId, PPID_TO_PERIPH_ID(periphId), pSpmiInfo->owner, &channel );
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }

    spmiRes = SpmiPic_InterruptClear(pDev, channel, uIntrClearMask);
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }
    return SPMI_SUCCESS;
}

/**
@brief Clear all spmi interrupts

This function clears the all spmi interrupts

@return  SPMI_SUCCESS on success, error code on error 

@see None
*/
Spmi_Result SpmiIntrCtlr_InterruptClearAll(void)
{
    Spmi_Result spmiRes;
    SpmiDevInfo *pDev = SpmiBus_GetDevInfo(0);
    SpmiInfo *pSpmiInfo = SpmiBus_GetInfo();
    
    if (pDev == NULL || pSpmiInfo == NULL)
    {
      return SPMI_FAILURE_INVALID_PARAMETER;
    }

    spmiRes = SpmiPic_InterruptClearAll(pDev, pSpmiInfo->owner);
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;
    }

    return SPMI_SUCCESS;
}



/**
@brief Get the Logical(channel id) from a physical peripheral Id
       (PPID)

This function returns the logical id or channel id of a 
peripheral from the given PPID 

@param[in] periphId Physical Peripheral address. 12 bits (4 bit 
                    SID + 8 bit upper peripheral address)

@param[out] pLogicalId Pointer to the uint32 which will be 
                       populated by this function. The value
                       will be the corresponding channel id.

@return SPMI_SUCCESS on success, error code on error

@see None
*/
Spmi_Result SpmiIntrCtlr_GetLogicalIdFromPpid(SpmiPhyPeriphIdType periphId, uint32 *pLogicalId)
{
    Spmi_Result spmiRes;
    uint8 slaveId, busId;
    uint16 channel;
    uint8 vSlaveId = PPID_TO_VSLAVE_ID(periphId);
    SpmiDevInfo *pDev = SpmiBus_GetDevInfo(vSlaveId);
    SpmiInfo *pSpmiInfo = SpmiBus_GetInfo();

    if ((pSpmiInfo == NULL) || (pDev == NULL)) {
        return SPMI_FAILURE_INVALID_PARAMETER;     
    }
    
    if (pLogicalId == NULL) {
        return SPMI_FAILURE_INVALID_PARAMETER;     
    }

    spmiDecodeVSid(vSlaveId, &busId, &slaveId);

    spmiRes = PmicArb_FindChannel(pDev, slaveId, PPID_TO_PERIPH_ID(periphId), pSpmiInfo->owner, &channel );
    if ( SPMI_SUCCESS != spmiRes ) {
        return spmiRes;     
    }

    *pLogicalId = (uint32)channel;

    return SPMI_SUCCESS;
}

