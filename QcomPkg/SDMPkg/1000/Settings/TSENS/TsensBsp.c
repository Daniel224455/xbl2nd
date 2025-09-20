/*============================================================================
  FILE:         TsensBsp.c

  OVERVIEW:     Auto-generated TSENS BSP
                * Chip: SDM1000, version: all
                * Config file: TSENS_Poipu.xlsm
                * Config file format version: 4
                * Generator version: 1.2.0

                Copyright (c) 2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "TsensBsp.h"
#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
static const uint32 gauTbcbReqs0[] =
{
   /* Client 0: APC0_llm */
   0x7e,

   /* Client 1: APC1_llm */
   0x7fe0,

   /* Client 2: gpu */
   0x8000,

   /* Client 3: glm */
   0xfffe,
};

static const TsensControllerCfgType gaTsensControllerCfgs[] =
{
   /* Controller 0 */
   {
      .bPSHoldResetEn     = TRUE,
      .uPeriodActive      = 2,
      .uPeriodSleep       = 64,
      .bAutoAdjustPeriod  = TRUE,
      .uTSControl         = 0x0,
      .uTSConfig          = 0x0,
      .bStandAlone        = FALSE,
      .uSidebandEnMask    = 0xfffe,
      .bPWMEn             = FALSE,
      .pucTsensSROTPhys   = (uint8 *)TSENS0_REG_BASE_PHYS,
      .uTsensSROTSize     = TSENS0_REG_BASE_SIZE,
      .pucTsensTMPhys     = (uint8 *)TSENS0_TM_REG_BASE_PHYS,
      .uTsensTMSize       = TSENS0_TM_REG_BASE_SIZE,
      .pauTbcbReqs        = gauTbcbReqs0,
      .uNumTbcbClients    = ARRAY_LENGTH(gauTbcbReqs0),
      .bWatchdogEn        = TRUE,
      .bWatchdogResetEn   = TRUE,
      .uWatchdogLoadVal   = 9850,
   },

   /* Controller 1 */
   {
      .bPSHoldResetEn     = TRUE,
      .uPeriodActive      = 32,
      .uPeriodSleep       = 64,
      .bAutoAdjustPeriod  = TRUE,
      .uTSControl         = 0x0,
      .uTSConfig          = 0x0,
      .bStandAlone        = FALSE,
      .uSidebandEnMask    = 0xfffe,
      .bPWMEn             = FALSE,
      .pucTsensSROTPhys   = (uint8 *)TSENS1_REG_BASE_PHYS,
      .uTsensSROTSize     = TSENS1_REG_BASE_SIZE,
      .pucTsensTMPhys     = (uint8 *)TSENS1_TM_REG_BASE_PHYS,
      .uTsensTMSize       = TSENS1_TM_REG_BASE_SIZE,
      .pauTbcbReqs        = NULL,
      .uNumTbcbClients    = 0,
      .bWatchdogEn        = TRUE,
      .bWatchdogResetEn   = TRUE,
      .uWatchdogLoadVal   = 9850,
   },
};

static const TsensSensorCfgType gaTsensSensorCfgs[] =
{
   /* Sensor 0 */
   {
      .ucController                = 0,
      .ucChannel                   = 0,
      .ucSensorID                  = 0,
      .ucFuseIdx                   = 0,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 1 */
   {
      .ucController                = 0,
      .ucChannel                   = 1,
      .ucSensorID                  = 1,
      .ucFuseIdx                   = 1,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 2 */
   {
      .ucController                = 0,
      .ucChannel                   = 2,
      .ucSensorID                  = 2,
      .ucFuseIdx                   = 2,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 3 */
   {
      .ucController                = 0,
      .ucChannel                   = 3,
      .ucSensorID                  = 3,
      .ucFuseIdx                   = 3,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 4 */
   {
      .ucController                = 0,
      .ucChannel                   = 4,
      .ucSensorID                  = 4,
      .ucFuseIdx                   = 4,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 5 */
   {
      .ucController                = 0,
      .ucChannel                   = 5,
      .ucSensorID                  = 5,
      .ucFuseIdx                   = 5,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 6 */
   {
      .ucController                = 0,
      .ucChannel                   = 6,
      .ucSensorID                  = 6,
      .ucFuseIdx                   = 6,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 7 */
   {
      .ucController                = 0,
      .ucChannel                   = 7,
      .ucSensorID                  = 7,
      .ucFuseIdx                   = 7,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 8 */
   {
      .ucController                = 0,
      .ucChannel                   = 8,
      .ucSensorID                  = 8,
      .ucFuseIdx                   = 8,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 9 */
   {
      .ucController                = 0,
      .ucChannel                   = 9,
      .ucSensorID                  = 9,
      .ucFuseIdx                   = 9,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 10 */
   {
      .ucController                = 0,
      .ucChannel                   = 10,
      .ucSensorID                  = 10,
      .ucFuseIdx                   = 10,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 11 */
   {
      .ucController                = 0,
      .ucChannel                   = 11,
      .ucSensorID                  = 11,
      .ucFuseIdx                   = 11,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 12 */
   {
      .ucController                = 0,
      .ucChannel                   = 12,
      .ucSensorID                  = 12,
      .ucFuseIdx                   = 12,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 13 */
   {
      .ucController                = 0,
      .ucChannel                   = 13,
      .ucSensorID                  = 13,
      .ucFuseIdx                   = 13,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 14 */
   {
      .ucController                = 0,
      .ucChannel                   = 14,
      .ucSensorID                  = 14,
      .ucFuseIdx                   = 14,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 15 */
   {
      .ucController                = 0,
      .ucChannel                   = 15,
      .ucSensorID                  = 15,
      .ucFuseIdx                   = 15,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 16 */
   {
      .ucController                = 1,
      .ucChannel                   = 0,
      .ucSensorID                  = 0,
      .ucFuseIdx                   = 16,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 17 */
   {
      .ucController                = 1,
      .ucChannel                   = 1,
      .ucSensorID                  = 1,
      .ucFuseIdx                   = 17,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 18 */
   {
      .ucController                = 1,
      .ucChannel                   = 2,
      .ucSensorID                  = 2,
      .ucFuseIdx                   = 18,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 19 */
   {
      .ucController                = 1,
      .ucChannel                   = 3,
      .ucSensorID                  = 3,
      .ucFuseIdx                   = 19,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 20 */
   {
      .ucController                = 1,
      .ucChannel                   = 4,
      .ucSensorID                  = 4,
      .ucFuseIdx                   = 20,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 21 */
   {
      .ucController                = 1,
      .ucChannel                   = 5,
      .ucSensorID                  = 5,
      .ucFuseIdx                   = 21,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 22 */
   {
      .ucController                = 1,
      .ucChannel                   = 6,
      .ucSensorID                  = 6,
      .ucFuseIdx                   = 22,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 23 */
   {
      .ucController                = 1,
      .ucChannel                   = 7,
      .ucSensorID                  = 7,
      .ucFuseIdx                   = 23,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 24 */
   {
      .ucController                = 1,
      .ucChannel                   = 8,
      .ucSensorID                  = 8,
      .ucFuseIdx                   = 24,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 25 */
   {
      .ucController                = 1,
      .ucChannel                   = 9,
      .ucSensorID                  = 9,
      .ucFuseIdx                   = 25,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 26 */
   {
      .ucController                = 1,
      .ucChannel                   = 10,
      .ucSensorID                  = 10,
      .ucFuseIdx                   = 26,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 27 */
   {
      .ucController                = 1,
      .ucChannel                   = 11,
      .ucSensorID                  = 11,
      .ucFuseIdx                   = 27,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 28 */
   {
      .ucController                = 1,
      .ucChannel                   = 12,
      .ucSensorID                  = 12,
      .ucFuseIdx                   = 28,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 29 */
   {
      .ucController                = 1,
      .ucChannel                   = 13,
      .ucSensorID                  = 13,
      .ucFuseIdx                   = 29,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 30 */
   {
      .ucController                = 1,
      .ucChannel                   = 14,
      .ucSensorID                  = 14,
      .ucFuseIdx                   = 30,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },

   /* Sensor 31 */
   {
      .ucController                = 1,
      .ucChannel                   = 15,
      .ucSensorID                  = 15,
      .ucFuseIdx                   = 31,
      .eCal                        = TSENS_BSP_SENSOR_CAL_NORMAL,
      .nsCalPoint1CodeDefault      = 480,
      .nsCalPoint2CodeDefault      = 714,
      .nsThresholdMinDeciDegC      = TSENS_THRESHOLD_DISABLED,
      .nsThresholdLowerDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdUpperDeciDegC    = TSENS_THRESHOLD_DISABLED,
      .nsThresholdCriticalDeciDegC = TSENS_THRESHOLD_DISABLED,
      .nsThresholdMaxDeciDegC      = 1200,
   },
};

const TsensBspType TsensBsp[] =
{
   {
      .paControllerCfgs    = gaTsensControllerCfgs,
      .uNumControllers     = ARRAY_LENGTH(gaTsensControllerCfgs),
      .paSensorCfgs        = gaTsensSensorCfgs,
      .uNumSensors         = ARRAY_LENGTH(gaTsensSensorCfgs),
      .uSensorConvTime_us  = 30,
      .nCalPoint1DeciDegC  = 300,
      .nCalPoint2DeciDegC  = 1200,
      .uShift              = 10,
   }
};

