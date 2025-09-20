
/*
===========================================================================

FILE:   SpiLog.c

DESCRIPTION:
    This file contains functions for SPI logging driver
 
===========================================================================

                                  Edit History


When     Who    What, where, why
-------- ---    -----------------------------------------------------------
12/17/13 sg     Updated common logging mechanism
11/02/11 unr    Created

===========================================================================
             Copyright c 2011 - 2013 Qualcomm Technologies Incorporated.
                              All Rights Reserved.
                           Qualcomm Proprietary/GTDR

===========================================================================
*/

#include "SpiLog.h"
       
/* There are there log levels.
 * Refer SpiLog.h for details.
 * Default it level SPI_LOG_LEVEL_INFO, 
 * but during debugging the levels can be 
 * increased through JTAG.
 */
static uint32 uSpiLogLevel = SPI_LOG_LEVEL_INFO; 

// Initialization 
int32 SpiLog_Init(void)
{

   return 0;
}

uint32 SpiLog_GetLogLevel(void)
{
   return(uSpiLogLevel);
}

void SpiLog_SetLogLevel(uint32 uLogLevel)
{
	uSpiLogLevel = (uLogLevel > SPI_LOG_LEVEL_MAX) ? 
									SPI_LOG_LEVEL_MAX : uLogLevel;
}

