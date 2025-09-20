/*
===========================================================================

FILE:   HALIntSel.c

DESCRIPTION:
    This file implements a Interrupt select hal.

===========================================================================

        Edit History


When     Who    What, where, why
-------- ---    -----------------------------------------------------------
06/21/13 LK     Removed test gards on put/get word from/to fifo to speed up
                 the code. The tests are done somewhere else.
06/13/12 ddk    Removed unused include.
06/06/12 ddk    Removed non-common functions related to I2c and SPI.
06/05/12 ddk    Removed unwanted writes in bam mode.
04/19/12 ddk    Added changes for avoiding symbol match with I2C library.
                Added updated HAL implementation used for I2C.
02/22/12 ddk    Added support for SPI BAM interface requirements.
02/14/11 LK     Changed QUP address block initialization from static to
                dynamic. Virtual addresses are intialized at runtime.
10/15/10 LK     Created

===========================================================================
        Copyright (c) 2008-2012 Qualcomm Technologies Incorporated
                            All Rights Reserved
                         Qualcomm Proprietary/GTDR
===========================================================================
*/

#include "QupLog.h"
#include "QupTypes.h"
#include "QupTcsrHwio.h"

/**
 * This function configures QUP interrupts in MSS by
 * enabling an TCSR register
 * 
 * @param[in]  pTcsrAddr TCR address.
 * @param[in]  bitMask - Bit to be set for a QUP.
 */
int32 
HAL_qupe_EnableTcsrInterrupt
(
 uint8 *pTcsrAddr,
 uint32 uBitMask,
 uint32 uBitShift
 )
{
	HWIO_PHSS_QUP_MSS_INT_SEL_n_OUTMI(
			pTcsrAddr, 0, uBitMask, (1 <<uBitShift));
	return 0;
}
