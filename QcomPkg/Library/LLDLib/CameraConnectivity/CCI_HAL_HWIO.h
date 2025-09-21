/* ===========================================================================

Camera Control Interface Device Abstraction Layer Header

*//** @file CCI_HAL_HWIO.h
@brief Header file for CCI driver.

This header file defines the constant objects and enumerations constants for CCI.

Copyright (c) 2012-2017 Qualcomm Technologies, Incorporated. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.


*//*======================================================================= */
#ifndef H_CCI_HAL_HWIO_H
#define  H_CCI_HAL_HWIO_H

#include "CameraPlatform.h"
#include "CameraCCITypes.h"

#define MAX_NUM_OF_QUEUES 7
#define MAX_NUM_OF_MASTER 2

#define MAX_NUM_OF_PROTOCOL_TYPE 2
#define MAX_NUM_OF_SYNC_TIMER 4

#define MAX_I2C_WRTE_TRANSFER_CNTS 12
#define MAX_I2C_PARAM_SET 3

typedef enum 
{ 
    CCI_HAL_CMD_SET_PARAM       = 0x1,
    CCI_HAL_CMD_I2C_WAIT        = 0x2,
    CCI_HAL_CMD_WAIT_SYNC       = 0x3,
    CCI_HAL_CMD_I2C_WAIT_GPIO_EVENT = 0x4,
    CCI_HAL_CMD_I2C_TRIG_I2C_EVENT  = 0x5,
    CCI_HAL_CMD_I2C_LOCK        = 0x6,
    CCI_HAL_CMD_I2C_UNLOCK      = 0x7,
    CCI_HAL_CMD_I2C_REPORT      = 0x8,
    CCI_HAL_CMD_I2C_WRITE       = 0x9,
    CCI_HAL_CMD_I2C_READ        = 0xA,
    CCI_HAL_CMD_I2C_WRITE_DP    = 0xB,
    CCI_HAL_CMD_I2C_READ_DP     = 0xC,

    CCI_HAL_CMD_GPIO_WAIT       = 0x2,
    CCI_HAL_CMD_GPIO_IN         = 0x4,
    CCI_HAL_CMD_GPIO_WAIT_I2C_EVENT = 0x5,
    CCI_HAL_CMD_GPIO_OUT        = 0x6,
    CCI_HAL_CMD_TRIG_GPIO_EVENT = 0x7,
    CCI_HAL_CMD_GPIO_REPORT     = 0x8,
} CCI_HAL_CMDType;

typedef union 
{
    struct 
    {
        UINT32 id     : 4;
        UINT32 nSlave : 7;
        UINT32        : 5;
        UINT32 nretry : 2;
        UINT32 ncid   : 2;
    }bits;
    UINT32 data;
}CCISetparamCmdType;

typedef union 
{
    struct 
    {
        UINT32 id        : 4;
        UINT32 report_id : 4;
        UINT32 irq_enable: 1;
        UINT32 capture   : 1;
    }bits;
    UINT32 data;
}CCIReportCmdType;

enum CCI_I2C_FRQCY
{
    frqcy_0,
    frqcy_100khz = frqcy_0,
    frqcy_400khz,
    frqcy_1000khz,
    frqcy_max
};

enum I2C_IRQ
{
    irq_i2c_begins      = 0x00000001,
    irq_m0_rd_done      = irq_i2c_begins,
    irq_m0_rd_underflow = 0x00000002,
    irq_m0_rd_overflow  = 0x00000004,

    irq_m0_q0_report    = 0x00000010,
    irq_m0_q0_underflow = 0x00000020,
    irq_m0_q0_overflow  = 0x00000040,
    irq_m0_q0_cmd_err   = 0x00000080,

    irq_m0_q1_report    = 0x00000100, 
    irq_m0_q1_underflow = 0x00000200, 
    irq_m0_q1_overflow  = 0x00000400, 
    irq_m0_q1_cmd_err   = 0x00000800, 

    irq_m1_rd_done      = 0x00001000,
    irq_m1_rd_underflow = 0x00002000,
    irq_m1_rd_overflow  = 0x00004000,

    irq_m1_q0_report    = 0x00010000,
    irq_m1_q0_underflow = 0x00020000,
    irq_m1_q0_overflow  = 0x00040000,
    irq_m1_q0_cmd_err   = 0x00080000,

    irq_m1_q1_report    = 0x00100000,
    irq_m1_q1_underflow = 0x00200000,
    irq_m1_q1_overflow  = 0x00400000,
    irq_m1_q1_cmd_err   = 0x00800000,

    irq_rst_done_ack    = 0x01000000,

    irq_m0_q0q1_halt_ack= 0x02000000,
    irq_m1_q0q1_halt_ack= 0x04000000,

    irq_m0_q0_nack_err  = 0x08000000,
    irq_m0_q1_nack_err  = 0x10000000,
    irq_m1_q0_nack_err  = 0x20000000,
    irq_m1_q1_nack_err  = 0x40000000,

    irq_i2c_ends,
};

enum I2C_IRQCombo
{
    irq_m0_rd_err = irq_m0_rd_underflow |
                    irq_m0_rd_overflow,

    irq_m1_rd_err = irq_m0_rd_underflow |
                    irq_m0_rd_overflow,

    irq_m0_q0_err = irq_m0_q0_underflow |
                    irq_m0_q0_overflow  |
                    irq_m0_q0_cmd_err   |
                    irq_m0_q0_nack_err,

    irq_m1_q0_err = irq_m1_q0_underflow |
                    irq_m1_q0_overflow  |
                    irq_m1_q0_cmd_err   |
                    irq_m1_q0_nack_err,

    irq_m0_q1_err = irq_m0_q1_underflow |
                    irq_m0_q1_overflow  |
                    irq_m0_q1_cmd_err   |
                    irq_m0_q1_nack_err,

    irq_m1_q1_err = irq_m1_q1_underflow |
                    irq_m1_q1_overflow  |
                    irq_m1_q1_cmd_err   |
                    irq_m1_q1_nack_err,

    irq_m0_err =    irq_m0_rd_err |
                    irq_m0_q0_err |
                    irq_m0_q1_err,

    irq_m1_err =    irq_m1_rd_err |
                    irq_m1_q0_err |
                    irq_m1_q1_err,

    irq_i2c_err =   irq_m0_err |
                    irq_m1_err,

    irq_m0_q0   =   irq_m0_q0_err   |
                    irq_m0_rd_err   |
                    irq_m0_q0_report|
                    irq_m0_rd_done  |
                    irq_rst_done_ack|
                    irq_m0_q0q1_halt_ack,

    irq_m0_q1   =   irq_m0_q1_err   |
                    irq_m0_rd_err   |
                    irq_m0_q1_report|
                    irq_m0_rd_done  |
                    irq_rst_done_ack|
                    irq_m0_q0q1_halt_ack,

    irq_m1_q0   =   irq_m1_q0_err   |
                    irq_m1_rd_err   |
                    irq_m1_q0_report|
                    irq_m1_rd_done  |
                    irq_rst_done_ack|
                    irq_m1_q0q1_halt_ack,

    irq_m1_q1   =   irq_m1_q1_err   |
                    irq_m1_rd_err   |
                    irq_m1_q1_report|
                    irq_m1_rd_done  |
                    irq_rst_done_ack|
                    irq_m1_q0q1_halt_ack,

    irq_m0_report_ack =
                    irq_m0_q0_report |
                    irq_m0_q1_report,

    irq_m1_report_ack =
                    irq_m1_q0_report |
                    irq_m1_q1_report,

    irq_i2c_async_ack = 
                    irq_m0_q0_report |
                    irq_m1_q0_report, 

    irq_i2c_sync_ack = 
                    irq_m0_q1_report |
                    irq_m0_rd_done   |
                    irq_m1_q1_report |
                    irq_m1_rd_done   |
                    irq_rst_done_ack,

    irq_all_i2c_report_ack =
                    irq_m0_report_ack |
                    irq_m1_report_ack,

    irq_m0_ack =    irq_m0_rd_done   |
                    irq_m0_q0_report |
                    irq_m0_q1_report |
                    irq_m0_q0q1_halt_ack,

    irq_m1_ack =    irq_m1_rd_done   |
                    irq_m1_q0_report |
                    irq_m1_q1_report |
                    irq_m1_q0q1_halt_ack,

    irq_i2c_ack =   irq_m0_ack |
                    irq_m1_ack |
                    irq_rst_done_ack,
};

typedef enum 
{
    MasterMin = 0,
    m0        = MasterMin,
    m1,
    MasterMax,
    gpio      = MasterMax

} CCIMasterType;

typedef enum
{
    ProtocolMin = 0,
    i2c_protocol = ProtocolMin,
    gpio_protocol, 
    ProtocolMax, 

} CCIProtocolType;

typedef enum 
{
    queue_min = 0,
    m0q0      = queue_min,
    m0q1,
    m1q0,
    m1q1,
    gpioq0,
    gpioq1,
    gpioq2,
    queue_max

} CCIQueueType;

typedef enum
{
    queue_start_begins = 0x1,
    i2c_m0_q0_start    = queue_start_begins,
    i2c_m0_q1_start    = 0x2,
    i2c_m1_q0_start    = 0x4,
    i2c_m1_q1_start    = 0x8,
    gpio_q0_start      = 0x10,
    gpio_q1_start      = 0x20,
    gpio_q2_start      = 0x40,
    queue_start_ends,

} QueueStartType;

typedef volatile UINT32 CCIRegisterType;

struct CCIHWControlType       
{                             
    CCIRegisterType HW_VERSION;           // (MMSS_A_CCI_REG_BASE + 0x00000000)
    CCIRegisterType RESET_CMD;            // (MMSS_A_CCI_REG_BASE + 0x00000004)
    CCIRegisterType QUEUE_START;          // (MMSS_A_CCI_REG_BASE + 0x00000008)
    CCIRegisterType TESTBUS_SEL;          // (MMSS_A_CCI_REG_BASE + 0x0000000c)
};

struct CCII2CMasterType
{
    CCIRegisterType SCL_CTL;              // (MMSS_A_CCI_REG_BASE + 0x00000100)
    CCIRegisterType SDA_CTL_0;            // (MMSS_A_CCI_REG_BASE + 0x00000104)
    CCIRegisterType SDA_CTL_1;            // (MMSS_A_CCI_REG_BASE + 0x00000108)
    CCIRegisterType SDA_CTL_2;            // (MMSS_A_CCI_REG_BASE + 0x0000010c)
    CCIRegisterType MISC_CTL;             // (MMSS_A_CCI_REG_BASE + 0x00000110)
    CCIRegisterType STATUS;               // (MMSS_A_CCI_REG_BASE + 0x00000114)
    CCIRegisterType READ_DATA;            // (MMSS_A_CCI_REG_BASE + 0x00000118)
    CCIRegisterType READ_BUF_LEVEL;       // (MMSS_A_CCI_REG_BASE + 0x0000011c)
};

struct CCIHWQueueType
{
    CCIRegisterType EXEC_WORD_CNT;        // (MMSS_A_CCI_REG_BASE + 0x00000300)
    CCIRegisterType CUR_WORD_CNT;         // (MMSS_A_CCI_REG_BASE + 0x00000304)
    CCIRegisterType CUR_CMD;              // (MMSS_A_CCI_REG_BASE + 0x00000308)
    CCIRegisterType REPORT_STATUS;        // (MMSS_A_CCI_REG_BASE + 0x0000030c)
    CCIRegisterType LOAD_DATA;            // (MMSS_A_CCI_REG_BASE + 0x00000310)
};

struct CCIIRQControlType
{
    CCIRegisterType MASK;               // (MMSS_A_CCI_REG_BASE      + 0x00000c04)
    CCIRegisterType CLEAR;              // (MMSS_A_CCI_REG_BASE      + 0x00000c08)
    CCIRegisterType STATUS;             // (MMSS_A_CCI_REG_BASE      + 0x00000c0c)
};

struct CCIIRQGlblControlType
{
    CCIRegisterType GLOBAL_CLEAR_CMD;     
};

struct CCISyncTimerType
{
    CCIRegisterType SET_CID_SYNC_TIMER;
};
typedef struct CCIHWControlType*  CCIHWCTRL;
typedef struct CCII2CMasterType*  CCIMASTER;
typedef struct CCIHWQueueType*      CCIQUEUE;
typedef struct CCIIRQControlType* CCIIRQCTRL;
typedef struct CCIIRQGlblControlType* CCIIRQGlblControlType;
typedef struct CCISyncTimerType* CCISyncTimerType;

struct _I2CFrequencyParams
{
    const UINT16 THIGH;
    const UINT16 TLOW;
    const UINT16 TSUSTO;
    const UINT16 TSUSTA;
    const UINT16 THDDAT;
    const UINT16 THDSTA;
    const UINT16 TBUF;
    
    const UINT8 HW_SCL_STRETCH_EN;   // enable or disable SCL clock stretching
    const UINT8 HW_RDHLD;            // internal hold time 1-6 cycles of SDA to bridge undefined falling SCL region
    const UINT8 HW_TSP;              // glitch filter 1-3 cycles
}; 

typedef struct _I2CFrequencyParams* I2CFrequencyParams;
/// change the name to QueueInfo
struct _QueueInfo
{
    const CCIMasterType masterId;
    const CCIQueueType  queueId;
    const QueueStartType queue_start_id;
    const UINT8 maxQueueDepth;
};

typedef struct _QueueInfo* QueueInfo;


typedef struct _CCIHWDetaisType
{ 
    // The max number of i2c masters on our product line
    INT32 max_num_of_master;
    // The max number of i2c queues on our product line
    INT32 max_num_of_i2c_queue;
    UINT32 max_i2cq0_depth;
    UINT32 max_i2cq1_depth;
    UINT32 max_gpioq0_depth;
    UINT32 max_gpioq1_depth;
    UINT32 gpio_pin_nums;
    // The max number of gpio queues on our product line
    INT32 max_num_of_gpio_queue;
    INT32 max_num_of_queues;
    INT32 max_num_of_sync_timers;

    UINT32 cci_bit_wdth;
    UINT32 default_bitvector_sz;
    // The default heap size
    // UINT32 default_heapsz; 
    UINT32 master_offset;
    UINT32 master_base;
    UINT32 queue_offset;
    UINT32 queue_base;
    UINT32 irq_base;
    UINT32 irq_set_base;
    UINT32 irq_set_offset;
    UINT32 sync_timer_base;
    UINT32 sync_timer_offset;
    UINT32 mx_pltfm_write_trnsf_unt;
    UINT32 mx_write_trnsf_unt;
    UINT32 mx_read_fifo_len;
    UINT32 mx_cid;
    UINT32 default_retry_count;
    UINT32 cciword_sz;
    UINT32 default_i2c_speed;
    UINT32 default_async_queue;
    UINT32 default_async_gpio_queue;
    UINT32 async_exec_cnt;
    BOOLEAN bContinousModeSupported;
    UINT32 ni2c_paramsz;
    struct _I2CFrequencyParams cci_i2c_frequency_params[MAX_I2C_PARAM_SET]; 
    struct _QueueInfo queue_info_list[MAX_NUM_OF_QUEUES];                   

} CCIHWDetaisType;

struct _CCI
{ 
    UINT8 *CCIBase;
    CCIIRQGlblControlType GLBL_IRQ;
    CCIIRQCTRL IRQ[MAX_NUM_OF_PROTOCOL_TYPE];
    CCIHWCTRL CCICMNCTRL;
    CCISyncTimerType SYNC_TIMER[MAX_NUM_OF_SYNC_TIMER];
    CCIQUEUE  QUEUES [MAX_NUM_OF_QUEUES];
    CCIMASTER MASTERS[MAX_NUM_OF_MASTER];
    UINT32 interrupts[MAX_NUM_OF_QUEUES];
    BOOLEAN bExit;
};

typedef struct _CCI* CCICTX;

/* ---------------------------------------------------------------------------
 *    FUNCTION        cci_ctrl_init
 *    DESCRIPTION     Inits CCI Control Registers Struct
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrlInit(CCICTX CCI, CCIHWDetaisType* CCIInfo);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_Reset
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_Reset(CCICTX CCI, UINT32 cmd);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_QueueStart
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCIHWCtrl_QueueStart(CCIHWCTRL CCI, UINT32 cmd);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetSCL
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_SetSCL(CCICTX CCI, CCIMasterType masterId, UINT32 val);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetSDA0
 *    DESCRIPTION     Set I2C Control register0 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_SetSDA0(CCICTX CCI, CCIMasterType masterId, UINT32 val);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetSDA1
 *    DESCRIPTION     Set I2C Control register1 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_SetSDA1(CCICTX CCI, CCIMasterType masterId, UINT32 val);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetSDA2
 *    DESCRIPTION     Set I2C Control register2 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_SetSDA2(CCICTX CCI, CCIMasterType masterId, UINT32 val);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetMiscCtl
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_SetMiscCtl(CCICTX CCI, CCIMasterType masterId, UINT32 val);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_GetI2CReadData
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
UINT32 CCICtrl_GetI2CReadData(CCICTX CCI, CCIMasterType masterId);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_GetI2CReadBufLevel
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
UINT32 CCICtrl_GetI2CReadBufLevel(CCICTX CCI, CCIMasterType masterId);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCIQueueCtrl_SetExecWordCnt
 *    DESCRIPTION    
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCIQueueCtrl_SetExecWordCnt(CCIQUEUE CCIQueue, UINT32 cnts);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_LoadDATA
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCIQueueCtrl_LoadDATA(CCIQUEUE CCIQueue, UINT32 data);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_DoInterruptGlobalClear
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_DoInterruptGlobalClear(CCICTX CCI);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_SetMaskForInterrupt
 *    DESCRIPTION      
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_SetMaskForInterrupt(CCICTX CCI, CCIProtocolType protocol, UINT32 mask);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_ClearInterruptStatus
 *    DESCRIPTION     Clears interrupt status 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
void CCICtrl_ClearInterruptStatus(CCICTX CCI, CCIProtocolType protocol, UINT32 val);

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICtrl_GetInterruptStatus
 *    DESCRIPTION     Read Interrupt Status 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    void
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
UINT32 CCICtrl_GetInterruptStatus(CCICTX CCI, CCIProtocolType protocol);

#endif
