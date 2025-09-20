#ifndef __PM_RESOURCES_AND_TYPES_H__
#define __PM_RESOURCES_AND_TYPES_H__

/*! \file pm_resources_and_types.h
 *  \n
 *  \brief This file contains enumerations with resource names for different
 *  \n module types that should be used by external clients when accessing
 *  \n     the resources that are required.
 *  \n
 *  \n &copy; Copyright 2010-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
 */
/* ======================================================================= */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/19/17   aab     pm_sw_mode_type, removed Auto Retention since it is NOT supported
12/08/17   aab     Updated pm_sw_mode_type
11/03/15   al      Updated rails API
10/17/14   aab     Added pm_smbchg_param_config_enable_type
03/31/14   akm     Added/updated enums/typedefs
03/13/13   hw      Rearchitecting clk module driver to peripheral driver
02/27/13   kt      Added common IRQ enums  
01/22/13   aab     Added support for PM8026 and PM8110
09/25/12   hw      moving some npa type to this file so we don't need to include pmapp_npa.h
09/29/11   hs      Added CLK resource IDs.
09/14/11   dy      Move UICC resource IDs to this file
04/25/11   wra     Adding Battery Temperature Management BTM, LPG, more LVS, ADC_ARB,
                    OVP & INTerrupt channel enumeration needed for PM8921
02/08/11   hw      Merging changes from the PMIC Distributed Driver Arch branch
07/05/10   wra     Modified Header includes to accomodate MSM8660
=============================================================================*/
/*===========================================================================

                        HEADER FILES

===========================================================================*/
#include "DALStdDef.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
#define  PM_IMG_NONE    0
#define  PM_IMG_LOADER  1
#define  PM_IMG_CORE    2
#define  PM_IMG_INVALID 3

#define LOCKED_NONE   0
#define LOCKED_OPEN   2
#define LOCKED_CLOSED 3

/*! \enum chargers
 *  \brief This enum contains definitions of all chargers in the target
 */
enum
{
  PM_CHG_1,
  PM_CHG_INVALID
};

/*!
 * PMIC power module voltage level
 */
typedef uint32 pm_volt_level_type;

typedef enum
{
   PM_OFF,
   PM_ON,
   PM_INVALID
}pm_on_off_type;


typedef enum
{
   PM_DISABLE_CONFIG = 0,
   PM_ENABLE_CONFIG  = 1,
   PM_CONFIG_INVALID
} pm_param_config_enable_type;

/** Software mode setting.*/
typedef enum 
{
   PM_SW_MODE_LPM,             /**< 0 : Low Power mode                  */
   PM_SW_MODE_BYPASS,          /**< 1 : Bypass mode         (LDO only ) */
   PM_SW_MODE_AUTO,            /**< 2 : Auto Mode           (SMPS Only) */
   PM_SW_MODE_NPM,             /**< 3 : Normal Power mode               */
   PM_SW_MODE_RETENTION,       /**< 4 : Retention mode.                 */
   PM_SW_MODE_INVALID
}pm_sw_mode_type;

/*!
 * The number of phases that the autonomous phase controller(APC) can use.
 */
typedef enum 
{
    PM_PHASE_CNT__1,		
    PM_PHASE_CNT__2,	
    PM_PHASE_CNT__3, 
    PM_PHASE_CNT__4,
    PM_PHASE_CNT__INVALID
}pm_phase_cnt_type;

/** 
  @struct peripheral_info_type
  @brief Contains Peripheral Information such as Type, Subtype, 
         Analog and Digital Major Versions. 
 */
typedef struct 
{
    uint16 base_address;
    uint8  peripheral_type;
    uint8  peripheral_subtype;
    uint8  analog_major_version;
    uint8  analog_minor_version;
    uint8  digital_major_version;
    uint8  digital_minor_version;
}peripheral_info_type;


typedef enum
{
    PM_CLK_BUFF_OUT_DRV__1X,
    PM_CLK_BUFF_OUT_DRV__2X,
    PM_CLK_BUFF_OUT_DRV__3X,
    PM_CLK_BUFF_OUT_DRV__4X,
} pm_clk_buff_output_drive_strength_type;

/** 
  Types of IRQ bit fields of power rails. 
 */
typedef enum 
{
  PM_PWR_IRQ_VREG_FAULT,  /**< IRQ VREG Fault. */
  PM_PWR_IRQ_VREG_READY,  /**< IRQ VREG Ready. */
  PM_PWR_IRQ_INVALID
}pm_pwr_irq_type;


/*! \enum pm_irq_trigger_type
 *  \brief Type definition for different IRQ triggers
 */
typedef enum 
{
  PM_IRQ_TRIGGER_ACTIVE_LOW,
  PM_IRQ_TRIGGER_ACTIVE_HIGH,
  PM_IRQ_TRIGGER_RISING_EDGE,
  PM_IRQ_TRIGGER_FALLING_EDGE,
  PM_IRQ_TRIGGER_DUAL_EDGE,
  PM_IRQ_TRIGGER_INVALID
} pm_irq_trigger_type;

/*! \enum pm_irq_status_type
 *  \brief Type definition for different IRQ STATUS
 */
typedef enum
{
  PM_IRQ_STATUS_RT,
  PM_IRQ_STATUS_LATCHED,
  PM_IRQ_STATUS_PENDING,
  PM_IRQ_STATUS_ENABLE,
  PM_IRQ_STATUS_INVALID
} pm_irq_status_type;



/**
 * Regulator voltage information.
 */
typedef struct
{
    uint32    RangeMin; /**< Minimum voltage range supported. */
    uint32    RangeMax; /**< Maximum voltage range supported. */
    uint32    VStep;	/**< Step size in uV. */	
}pm_pwr_volt_info_type;

/**
 * LockByBit information.
 */
typedef struct
{
    uint32  ppid       : 8;  /**< peripheral id */
    uint32  slave_id   : 4;  /**< peripheral slave id */
    uint32  lock_owner : 2;  /**< Peripheral Lock owner. */
    uint32  lock_value : 2;  /**< Peripheral Lock value.  */	
}pm_lock_by_bit_info;





#endif /* __PM_RESOURCES_AND_TYPES_H__ */

