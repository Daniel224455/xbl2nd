#ifndef H_CCIResourceType_H
#define H_CCIResourceType_H

/* ===========================================================================

                              C C I D r i v e r

                              (CCIResourceType)

                             D e f i n i t i o n s
@file CCIResourceType.h
This file contains CCIResourceType implementation

@par EXTERNALIZED FUNCTIONS
None

@par INITIALIZATION AND SEQUENCING REQUIREMENTS
None

Copyright (c) 2014-2017 Qualcomm Technologies, Incorporated. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.									 */
/* ===========================================================================
**                         INCLUDE FILES FOR MODULE
**=========================================================================== */   

#include "CCI_HAL_HWIO.h"
#include "CCI_HAL_Util.h"

typedef enum
{
    i2c_read,
    i2c_write,
    i2c_async_write,
    i2c_queued_write,
    i2c_async_irq,
    on_gpio_in,
    gpio_write,
    operationtype_max
} operationtype;

typedef enum
{
    CCILD_MIN = 0,
    CCILD_DONT_CARE = CCILD_MIN,
    CCILD_AUTO,
    CCILD_MANUAL,
    CCILD_MAX,
} CCILdRunType;

typedef enum
{
    CCILD_State_MIN   = 0,
    CCILD_State_Complete = CCILD_State_MIN,
    CCILD_State_Unset, 
    CCILD_State_MAX, 
} CCILdState;

typedef enum
{
    CCIRs_Cap_Min = 0,
    CCIRs_Cap_Ok  = CCILD_MIN,
    CCIRs_Cap_After_Unload,
    CCIRs_Cap_Impossible,
    CCIRs_Cap_Max
} CCIRsCapabilityType;

typedef enum
{ 
    CCIClkFrqIdx_Min  = 0,
    CCIClkFrqIdx_Normal = CCIClkFrqIdx_Min, 
    CCIClkFrqIdx_Turbo, 
    CCIClkFrqIdx_Max
} CCICLKFrqIdx;

typedef enum
{ 
    CCIClkFrqVal_Min  = 0,
    CCIClkFrqVal_Normal = 192,
    CCIClkFrqVal_Turbo  = 372, 
    CCIClkFrq_Max
} CCICLKFrqVal;

typedef struct CCII2CConfig CCII2CConfig;
struct CCII2CConfig
{ 
    UINT8*  pdata;
    UINT8*  pdata_orig;
    UINT32 ndatasz;
    UINT32 nregaddr; 
    UINTN max_write_cnts;
    UINT8  nregsz;
    BOOLEAN bcontinuous; 
    BOOLEAN brptstart;
};

typedef struct CCIResourceConfig CCIResourceConfig;
struct CCIResourceConfig
{
    operationtype op;
    CCIMasterType master;
    CCI_SIGS signal;
    UINT32  irq;
    CCILdRunType eautorun;
    BOOLEAN bcontinuous;
};

typedef struct CCICapability CCICapability;
struct CCICapability
{
    UINT32 mx_cid;
    UINT32 cciclkfrq;
    UINT16 mx_burt_write_transf_capability;
     INT16 mx_write_trnsf_unt;
     INT16 mx_pltfm_write_trnsf_unt;
     INT16 mx_read_trnsf_unt;
     CCIQueueType gpio_in_gpio_queue;
     CCIQueueType gpio_in_m0_i2c_queue;
     CCIQueueType gpio_in_m1_i2c_queue;
    BOOLEAN bContinousSupported;
};
typedef struct CCILoader CCILoader;

struct CCILoader
{ 
    /// CCI h/w mapped reigister for queue
    CCIQUEUE CCI;
    /// CCI h/w mapped reigster
    CCIHWCTRL CCI_CTRL_REG;
    QueueInfo queueInfo;
    UINTN nSz;
    /// size of cci quee
    UINTN nCapacity;
    /// trigger point
    UINTN nFutureCapacity;
    /// currently allocated space for load
    UINTN nLogicalCapacity;
    UINTN nPresetLogicalCapacity;
    /// type of signal
    CCI_SIGS signal;
    /// irq to load
    UINT32 irq;
    /// CCI Loader state
    CCILdState state;
    /// 
    INT8  reportcnts;
    /// if true, queue is cleared
    /// when it's appropriate
    BOOLEAN bAutoRunEnabled;
};

typedef struct CCIResourceType CCIResourceType; 
struct CCIResourceType 
{
    QueueInfo queueInfo;
    CCIQUEUE  Queue;
    /// loads cci command to queue defined by current resource
    CCILoader loader;
    /// define capability of resource
    CCICapability capability;

    CCIResourceConfig config;
};

typedef struct CCIServiceRequestType CCIServiceRequestType;
struct CCIServiceRequestType
{
    /// specifies master
    CCIMasterType master;
    /// determines loader properties
    CCILdRunType loader_run_type;
    /// This is current I2C bus frequency
    CameraSensorI2C_SpeedType eCCI_I2CBusSpeed;
    /// specifies user options
    CameraSensorI2COptionsType user_options;
};

typedef struct CCIUserRequestType CCIUserRequestType;

struct CCIUserRequestType
{
    CCIServiceRequestType serviceRqst;

    /// Specifies queue info
    QueueInfo queueInfo;

    /// This is current I2C options
    CameraSensorI2COptionsType noptions;

    /// This indicates the outcome of user request
    CCIStatus eStatus;

    /// number of i2c transactions
    UINT32 ntransactions;

    /// defines i2c transaction
    CCII2CConfig config;

    /// This indicates timout value 
    UINT32 nwaittime;

    /// This indicates delay after transaction 
    UINT32 ndelay;

    /// This is current CID
    UINT8 ncid;

    /// This is current slave address
    UINT8 nslaveaddr;

    /// This is current data size
    UINT16 norig_datasz;

    /// This is current data size
    UINT8 norig_regsz;

    /// This is current read data size
    UINT16 nrddatasz;

    /// read buff
    UINT8* rdbuff;
    
    /// points to sequential buff array
    CameraSensorI2CRegConfigType* seq_buff;

    UINT8 data[MAX_I2C_WRTE_TRANSFER_CNTS + 1];
};

#endif
