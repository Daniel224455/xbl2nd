#ifndef HSUART_LOG_H
#define HSUART_LOG_H
/*==================================================================================================

FILE: HSUartLog.h

DESCRIPTION: Defines the logging feature APIs for the HS-UART driver.

Copyright (c) 2018 Qualcomm Technologies, Inc.
        All Rights Reserved.
Qualcomm Technologies, Inc. Confidential and Proprietary.

==================================================================================================*/

/*==================================================================================================
Edit History


when       who     what, where, why
--------   ---     --------------------------------------------------------
11/16/18   NM       Initial revision

==================================================================================================*/

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/

#include "HSUartDefs.h"

void   hsuart_log( uint8 file_id, uint16 line, uint8 num_args, uint32 a1, uint32 a2, uint32 a3 );

#ifdef HSUART_LOG_ALL
   #define HSUART_MSG_INFO( format, num_args, a1, a2, a3 ) \
                 hsuart_log((uint8) UART_LOGGING_FILE_ID, (uint16) __LINE__,num_args, a1, a2, a3)
#else
   #define HSUART_MSG_INFO( format, num_args, a1, a2, a3 )
#endif

#if ( defined(UART_LOG_ALL) || defined(UART_LOG_ERROR))
   #define HSUART_MSG_ERROR( format, num_args, a1, a2, a3 ) \
                 hsuart_log((uint8) UART_LOGGING_FILE_ID, (uint16) __LINE__,num_args, a1, a2, a3)
#else
   #define HSUART_MSG_ERROR( format, num_args, a1, a2, a3 )
#endif


#define HSUART_LOG( level, format )                  HSUART_LOG_0( level, format )
#define HSUART_LOG_0( level, format )                HSUART_MSG_##level( format, 0,  0,  0,  0 )
#define HSUART_LOG_1( level, format, a1 )            HSUART_MSG_##level( format, 1, a1,  0,  0 )
#define HSUART_LOG_2( level, format, a1, a2 )        HSUART_MSG_##level( format, 2, a1, a2,  0 )
#define HSUART_LOG_3( level, format, a1, a2, a3 )    HSUART_MSG_##level( format, 3, a1, a2, a3 )

#ifdef HSUART_QDSS_ENABLE
#include "UARTLiteSWEventId.h"
#include "tracer_micro.h"

#define UART_QDSS_LOG_0( log )     UTRACER_EVENT( log )
#define UART_QDSS_LOG_1( log, a1 ) UTRACER_EVENT( log, a1 )

#else
#define UART_QDSS_LOG_0( log )
#define UART_QDSS_LOG_1( log, a1 )

#endif

#endif


