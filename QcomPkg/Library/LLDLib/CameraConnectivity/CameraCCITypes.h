#ifndef _CAMERACCITYPES_H
#define _CAMERACCITYPES_H
/* ===========================================================================

             Camera Control Interface Device Abstraction Layer Header

*//** @file CameraCCITypes.h
@brief Header file for CCI driver types.

This header file defines the interface for CCI.

Copyright (c) 2012-2017 QUALCOMM Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*//*======================================================================= */

#include "CameraPlatform.h"

/* ---------------------------------------------------------------------------
** Type Declarations
** ------------------------------------------------------------------------ */
/**
 * This enumerates the status codes.
 */
typedef enum CCIStatusType
{
    CCI_Status_OK = 0,               ///< Successful
    CCI_Status_GENERAL_ERROR        = 0x00001,        ///< General or unknown error
    CCI_Status_ERROR_IRQ            = 0x00002,        ///< I2C  ERROR
    CCI_Status_IRQ_EXPIRED          = 0x00004,       ///< IRQ expired
    CCI_Status_NOT_SUPPORTED        = 0x00008,       ///< Request is not supported
    CCI_Status_MASTER_NOT_READY     = 0x00010,     ///< I2C is not ready
    CCI_Status_BAD_PARAM            = 0x00020,  ///< Bad Parameter
    CCI_Status_NULL_PTR             = 0x00040, ///< Invalid pointer or address
    CCI_Status_BAD_STATE            = 0x00080,    ///< Bad state to call
    CCI_Status_NO_RESOURCE          = 0x00100,    ///< No more resource
    CCI_Status_HARDWARE_ERROR       = 0x00200,  ///< Hardware Error
    CCI_Status_INSERTION_FAILURE    = 0x00400,  ///< Insertion to command heap failed
    CCI_Status_SET_PARAM_NOT_SET    = 0x00800,   ///< Set param is not set
    CCI_Status_INVALID_FRAME_NUMBER = 0x01000, ///< Invalid frame number given
    CCI_Status_ABORT                = 0x02000,
    CCI_Status_HEAP_EMPTY           = 0x04000, ///< Command heap is empty
    CCI_Status_BUSY                 = 0x08000,///< CCI Busy
    CCI_Status_NUM                  = 0x10000, ///<
    CCI_Status_HW_QUEUE_FULL        = 0x20000,
    CCI_Status_ALLOCATION_FAILURE   = 0x40000, ///< Memory Allocation failed
    CCI_Status_MAX                  
} CCIStatus;

/**
 * This enumerates the command queue type.
 */
typedef enum
{
    CCICommandQueueType_begins = 0x00,
    CCI_I2C_M0_Q0 = CCICommandQueueType_begins, ///< Master #0 and its queue #0
    CCI_I2C_M0_Q1 = 1, ///< Master #0 and its queue #1
    CCI_I2C_M1_Q0 = 2, ///< 
    CCI_I2C_M1_Q1 = 3, ///< 
    CCI_GPIO_Q0   = 4, ///< GPIO queue #0
    CCI_GPIO_Q1   = 5, ///< 
    CCI_GPIO_Q2   = 6, ///< 
    CCICommandQueueType_Size,
} CCICommandQueueType;

/**
 * This is a structure stores CCI parameters and methods.
 */
typedef struct CCIStruct* CCIType;
#endif //_CAMERACCITYPES_H


