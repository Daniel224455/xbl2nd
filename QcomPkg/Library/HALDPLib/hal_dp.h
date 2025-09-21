#ifndef _HAL_DP_H_
#define _HAL_DP_H_
/*! \file */

/*=============================================================================
 
  File: hal_dp.h
 
  Header file for DP controller hardware functionality 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/



/*=====================================================================================================================
*                         INCLUDE FILES
=====================================================================================================================*/
#include "hal_mdss.h"


/* --------------------------------------------------------------------------------------------------------------------
Enumerated types
---------------------------------------------------------------------------------------------------------------------*/

typedef enum 
{
  DP_DEVICE_ID0 = 0,
  DP_DEVICE_ID1 = 1,
  DP_DEVICE_MAX,
  DP_DEVICE_FORCE_32BIT = 0x7FFFFFFF
}DP_DeviceIDType;


/*!
 * \enum HAL_DP_InterruptType
 *
 * The \b HAL_DP_InterruptType provides the possible configuration/status read interrupts for DP controller and DP 
 * PHY.
 *
 */
typedef enum
{
   HAL_DP_INTR_TYPE_NONE                           = 0x00000000,
   HAL_DP_INTR_TYPE_HPD                            = HAL_MDSS_BIT(1),  /**< Indicates Sink has request via HDP pin  */
   HAL_DP_INTR_TYPE_AUX_I2C_DONE                   = HAL_MDSS_BIT(2),  /**< Indicates AUX/I2C transaction is done   */
   HAL_DP_INTR_TYPE_WRONG_ADDRESS                  = HAL_MDSS_BIT(3),  /**< Indicates wrong AUX address was given   */
   HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT            = HAL_MDSS_BIT(4),  /**< AUX/I2C has timed out consecutively     */
   HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER         = HAL_MDSS_BIT(5),  /**< AUX/I2C has NACK/DEFER consecutively    */
   HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT           = HAL_MDSS_BIT(6),  /**< # of read data bytes is not expected    */
   HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C            = HAL_MDSS_BIT(7),  /**< AUX NACK`ed during an I2C transaction   */
   HAL_DP_INTR_TYPE_AUX_DEFER_DURING_I2C           = HAL_MDSS_BIT(8),  /**< AUX DEFER`ed during an I2C transaction  */
   HAL_DP_INTR_TYPE_PLL_UNLOCK_DET                 = HAL_MDSS_BIT(9),  /**< Indicates PLL in DP PHY has unlocked   */
   HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR                = HAL_MDSS_BIT(10), /**< Indicates DPPHY AUX detected an error  */
   HAL_DP_INTR_TYPE_READY_FOR_VIDEO                = HAL_MDSS_BIT(11), /**< DP HW is ready to receive video        */
   HAL_DP_INTR_TYPE_IDLE_PATTERNS_SENT             = HAL_MDSS_BIT(12), /**< Idle patterns are sent after push idle  */
   HAL_DP_INTR_TYPE_FRAME_END                      = HAL_MDSS_BIT(13), /**< Indicate end of a frame                 */
   HAL_DP_INTR_TYPE_CRC_UPDATED                    = HAL_MDSS_BIT(14), /**< Indicate MISR registers have new values */
   HAL_DP_INTR_TYPE_AUX_RX_STOP_ERR                = HAL_MDSS_BIT(15), /**< Indicate STOP pattern error on AUX RX   */
   HAL_DP_INTR_TYPE_AUX_RX_DEC_ERR                 = HAL_MDSS_BIT(16), /**< Indicate Data decode error on AUX RX.   */
   HAL_DP_INTR_TYPE_AUX_RX_SYNC_ERR                = HAL_MDSS_BIT(17), /**< Indicate SYNC pattern error on AUX RX   */
   HAL_DP_INTR_TYPE_AUX_RX_ALIGN_ERR               = HAL_MDSS_BIT(18), /**< RX received less than 8 bits data       */
   HAL_DP_INTR_TYPE_AUX_TX_REQ_ERR                 = HAL_MDSS_BIT(19), /**< REQ. signal is late on TX.              */
   HAL_DP_INTR_TYPE_MAX,
   HAL_DP_INTR_TYPE_FORCE_32BIT                    = 0x7FFFFFFF
} HAL_DP_InterruptType;

/*!
 * \enum HAL_DP_LinkCommandType
 *
 * The \b HAL_DP_LinkCommandType defines the command to the main link Finite State Machine states, 
 *
 */
typedef enum
{
   HAL_DP_LINKCOMMAND_TYPE_NONE                    = 0x00000000,
   HAL_DP_LINKCOMMAND_TYPE_ENABLE,                                     /**< Enable DP Main link Controller          */
   HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN1,                              /**< Trigger main link to send train pattern1*/
   HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN2,                              /**< Trigger main link to send train pattern2*/
   HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN3,                              /**< Trigger main link to send train pattern3*/
   HAL_DP_LINKCOMMAND_TYPE_SEND_SYMBOL_ERM,                            /**< Send symbol error rate measure  pattern */
   HAL_DP_LINKCOMMAND_TYPE_SEND_PRBS7,                                 /**< Trigger main link to send PRBS7 pattern */
   HAL_DP_LINKCOMMAND_TYPE_SEND_CUSTOM_PATTERN,                        /**< Trigger to send 80bit custom pattern    */
   HAL_DP_LINKCOMMAND_TYPE_SEND_VIDEO,                                 /**< Trigger main link to send video data    */
   HAL_DP_LINKCOMMAND_TYPE_PUSH_IDLE,                                  /**< Trigger to send push for idle state     */
   HAL_DP_LINKCOMMAND_TYPE_DISABLE,                                    /**< Disable DP Main link Controller         */
   HAL_DP_LINKCOMMAND_TYPE_MAX,
   HAL_DP_LINKCOMMAND_TYPE_FORCE_32BIT             = 0x7FFFFFFF
} HAL_DP_LinkCommandType;

/*!
 * \enum HAL_DP_LinkStatusType
 *
 * The \b HAL_DP_LinkStatusType defines status of the main link for SW to query.
 *
 */
typedef enum
{
   HAL_DP_LINKSTATUS_TYPE_NONE                     = 0x00000000,
   HAL_DP_LINKSTATUS_TYPE_ENABLED                  = HAL_MDSS_BIT(0),  /**< DP Main link Controller is enabled      */
   HAL_DP_LINKSTATUS_TYPE_READY_FOR_VIDEO          = HAL_MDSS_BIT(1),  /**< HW is ready to accept video data        */
   HAL_DP_LINKSTATUS_TYPE_IDLE_PATTERNS_SENT       = HAL_MDSS_BIT(2),  /**< Push for Idle Pattern has been sent     */
   HAL_DP_LINKSTATUS_TYPE_READY_STATE              = HAL_MDSS_BIT(3),  /**< HW is up and ready for command          */
   HAL_DP_LINKSTATUS_TYPE_SENDING_PATTERN1         = HAL_MDSS_BIT(4),  /**< HW is sending link training pattern 1   */
   HAL_DP_LINKSTATUS_TYPE_SENDING_PATTERN2         = HAL_MDSS_BIT(5),  /**< HW is sending link training pattern 2   */
   HAL_DP_LINKSTATUS_TYPE_SENDING_PATTERN3         = HAL_MDSS_BIT(6),  /**< HW is sending link training pattern 3   */
   HAL_DP_LINKSTATUS_TYPE_SENDING_SYMBOL_ERM       = HAL_MDSS_BIT(7),  /**<sending symbol error rate measure pattern*/
   HAL_DP_LINKSTATUS_TYPE_SENDING_PRBS7            = HAL_MDSS_BIT(8),  /**< HW is sending PRBS7 pattern 3           */
   HAL_DP_LINKSTATUS_TYPE_SENDING_CUSTOM_PATTERN   = HAL_MDSS_BIT(9),  /**< HW is sending 80bit custom pattern      */
   HAL_DP_LINKSTATUS_TYPE_SENDING_IDLE_PATTERN     = HAL_MDSS_BIT(10), /**< Sending idle pattern after video enabled*/
   HAL_DP_LINKSTATUS_TYPE_SENDING_BS               = HAL_MDSS_BIT(11), /**< HW is sending blank sequence            */
   HAL_DP_LINKSTATUS_TYPE_SENDING_MSA              = HAL_MDSS_BIT(12), /**< HW is sending Main Stream Attribute     */
   HAL_DP_LINKSTATUS_TYPE_SENDING_VSC              = HAL_MDSS_BIT(13), /**< HW is sending Video Stream Config       */
   HAL_DP_LINKSTATUS_TYPE_SENDING_ACTIVE18BPP      = HAL_MDSS_BIT(14), /**< HW is sending 18bpp Video data          */
   HAL_DP_LINKSTATUS_TYPE_SENDING_ACTIVE24BPP      = HAL_MDSS_BIT(15), /**< HW is sending 24bpp Video data          */ 
   HAL_DP_LINKSTATUS_TYPE_SENDING_IDLE_PATTERN2    = HAL_MDSS_BIT(16), /**< HW is sending push for idle pattern     */
   HAL_DP_LINKSTATUS_TYPE_DISABLED                 = HAL_MDSS_BIT(17), /**< DP Main link Controller is disabled     */
   HAL_DP_LINKSTATUS_TYPE_MAX,
   HAL_DP_LINKSTATUS_TYPE_FORCE_32BIT              = 0x7FFFFFFF
} HAL_DP_LinkStatusType;

/*!
 * \enum HAL_DP_AUXStatusType
 *
 * The \b HAL_DP_AUXStatusType defines status of the AUX for SW to query.   
 *
 */
typedef enum
{
   HAL_DP_AUXSTATUS_TYPE_NONE                      = 0x00000000,
   HAL_DP_AUXSTATUS_TYPE_AUX_I2C_DONE              = HAL_MDSS_BIT(0),  /**< Indicates AUX/I2C transaction is done   */
   HAL_DP_AUXSTATUS_TYPE_WRONG_ADDRESS             = HAL_MDSS_BIT(1),  /**< Indicates wrong AUX address was given   */
   HAL_DP_AUXSTATUS_TYPE_CONSECUTIVE_TIMEOUT       = HAL_MDSS_BIT(2),  /**< AUX/I2C has timed out consecutively     */
   HAL_DP_AUXSTATUS_TYPE_CONSECUTIVE_NACK_DEFER    = HAL_MDSS_BIT(3),  /**< AUX/I2C has NACK/DEFER consecutively    */
   HAL_DP_AUXSTATUS_TYPE_WRONG_READDATA_COUNT      = HAL_MDSS_BIT(4),  /**< # of read data bytes is not expected    */
   HAL_DP_AUXSTATUS_TYPE_AUX_NACK_DURING_I2C       = HAL_MDSS_BIT(5),  /**< AUX NACK`ed during an I2C transaction   */
   HAL_DP_AUXSTATUS_TYPE_AUX_DEFER_DURING_I2C      = HAL_MDSS_BIT(6),  /**< AUX DEFER`ed during an I2C transaction  */
   HAL_DP_AUXSTATUS_TYPE_MAX,
   HAL_DP_AUXSTATUS_TYPE_FORCE_32BIT               = 0x7FFFFFFF
}HAL_DP_AUXStatusType;

/*!
 * \enum HAL_DP_AuxTransactionModeType
 *
 * The \b HAL_DP_AuxTransactionModeType defines types of AUX Transaction.
 *
 */
typedef enum
{
   HAL_DP_AUX_TRANSACTION_MODE_NONE               = 0x00000000,
   HAL_DP_AUX_TRANSACTION_MODE_NATIVE,                                /**< Native AUX Transaction       */
   HAL_DP_AUX_TRANSACTION_MODE_I2C,                                   /**< I2C over AUX Transaction     */
   HAL_DP_AUX_TRANSACTION_MODE_MAX,
   HAL_DP_AUX_TRANSACTION_MODE_FORCE_32BIT        = 0x7FFFFFFF
} HAL_DP_AuxTransactionModeType;

/*!
 * \enum HAL_DP_LaneMapType
 *
 * The \b HAL_DP_LaneMapType defines physical lane and logic lane mapping.   
 *
 */
typedef enum
{
   HAL_DP_LANEMAP_TYPE_NONE                        = 0x00000000,
   HAL_DP_LANEMAP_TYPE_0123,                                           /**< Physical lane map to logic lane as 0123 */
   HAL_DP_LANEMAP_TYPE_0132,                                           /**< Physical lane map to logic lane as 0132 */
   HAL_DP_LANEMAP_TYPE_0213,                                           /**< Physical lane map to logic lane as 0213 */
   HAL_DP_LANEMAP_TYPE_0231,                                           /**< Physical lane map to logic lane as 0231 */
   HAL_DP_LANEMAP_TYPE_0312,                                           /**< Physical lane map to logic lane as 0312 */
   HAL_DP_LANEMAP_TYPE_0321,                                           /**< Physical lane map to logic lane as 0321 */
   HAL_DP_LANEMAP_TYPE_1023,                                           /**< Physical lane map to logic lane as 1023 */
   HAL_DP_LANEMAP_TYPE_1032,                                           /**< Physical lane map to logic lane as 1032 */
   HAL_DP_LANEMAP_TYPE_1203,                                           /**< Physical lane map to logic lane as 1203 */
   HAL_DP_LANEMAP_TYPE_1230,                                           /**< Physical lane map to logic lane as 1230 */
   HAL_DP_LANEMAP_TYPE_1302,                                           /**< Physical lane map to logic lane as 1302 */
   HAL_DP_LANEMAP_TYPE_1320,                                           /**< Physical lane map to logic lane as 1320 */
   HAL_DP_LANEMAP_TYPE_2013,                                           /**< Physical lane map to logic lane as 2013 */
   HAL_DP_LANEMAP_TYPE_2031,                                           /**< Physical lane map to logic lane as 2031 */
   HAL_DP_LANEMAP_TYPE_2103,                                           /**< Physical lane map to logic lane as 2103 */
   HAL_DP_LANEMAP_TYPE_2130,                                           /**< Physical lane map to logic lane as 2130 */
   HAL_DP_LANEMAP_TYPE_2301,                                           /**< Physical lane map to logic lane as 2301 */
   HAL_DP_LANEMAP_TYPE_2310,                                           /**< Physical lane map to logic lane as 2310 */
   HAL_DP_LANEMAP_TYPE_3012,                                           /**< Physical lane map to logic lane as 3012 */
   HAL_DP_LANEMAP_TYPE_3021,                                           /**< Physical lane map to logic lane as 3021 */
   HAL_DP_LANEMAP_TYPE_3102,                                           /**< Physical lane map to logic lane as 3102 */
   HAL_DP_LANEMAP_TYPE_3120,                                           /**< Physical lane map to logic lane as 3120 */
   HAL_DP_LANEMAP_TYPE_3201,                                           /**< Physical lane map to logic lane as 3201 */
   HAL_DP_LANEMAP_TYPE_3210,                                           /**< Physical lane map to logic lane as 3210 */
   HAL_DP_LANEMAP_TYPE_MAX,
   HAL_DP_LANEMAP_TYPE_FORCE_32BIT                 = 0x7FFFFFFF
} HAL_DP_LaneMapType;

/*!
 * \enum HAL_DP_RGBMapType
 *
 * The \b HAL_DP_RGBMapType defines RGB component mapping.   
 *
 */
typedef enum
{
   HAL_DP_PIXELFORMAT_TYPE_RGB                         = 0x0,
   HAL_DP_PIXELFORMAT_TYPE_YUV420                      = 0x1,         
   HAL_DP_PIXELFORMAT_TYPE_YUV422                      = 0x2,
   HAL_DP_PIXELFORMAT_TYPE_MAX,
   HAL_DP_PIXELFORMAT_TYPE_FORCE_32BIT                  = 0x7FFFFFFF
} HAL_DP_PixelFormatType;

/*!
 * \enum HAL_DP_RGBMapType
 *
 * The \b HAL_DP_RGBMapType defines RGB component mapping.   
 *
 */
typedef enum
{
   HAL_DP_RGBMAP_TYPE_NONE                         = 0x00000000,
   HAL_DP_RGBMAP_TYPE_RGB,                                             /**< Color component order is RGB            */
   HAL_DP_RGBMAP_TYPE_RBG,                                             /**< Color component order is RBG            */
   HAL_DP_RGBMAP_TYPE_GBR,                                             /**< Color component order is GBR            */
   HAL_DP_RGBMAP_TYPE_GRB,                                             /**< Color component order is GRB            */
   HAL_DP_RGBMAP_TYPE_BGR,                                             /**< Color component order is BGR            */
   HAL_DP_RGBMAP_TYPE_BRG,                                             /**< Color component order is BRG            */
   HAL_DP_RGBMAP_TYPE_MAX,
   HAL_DP_RGBMAP_TYPE_FORCE_32BIT                  = 0x7FFFFFFF
} HAL_DP_RGBMapType;


/*!
 * \enum HAL_DP_ModuleType
 *
 * The \b HAL_DP_ModuleType defines HW module inside DP controller.
 *
 */
typedef enum
{
   HAL_DP_MODULE_TYPE_NONE                         = 0x00000000,
   HAL_DP_MODULE_TYPE_MAINLINK                     = HAL_MDSS_BIT(0),  /**< HW module: mainlink                     */
   HAL_DP_MODULE_TYPE_AUX                          = HAL_MDSS_BIT(1),  /**< HW module: AUX                          */
   HAL_DP_MODULE_TYPE_PHY                          = HAL_MDSS_BIT(2),  /**< HW module: DP PHY                       */
   HAL_DP_MODULE_TYPE_PLL                          = HAL_MDSS_BIT(3),  /**< HW module: DP PLL                       */
   HAL_DP_MODULE_TYPE_MAX,
   HAL_DP_MODULE_TYPE_FORCE_32BIT                  = 0x7FFFFFFF
} HAL_DP_ModuleType;


/*!
 * \enum HAL_DP_PropertyType
 *
 * The \b HAL_DP_PropertyType defines controller property ID to be set.
 *
 */
typedef enum
{
   HAL_DP_PROPERTY_TYPE_NONE                               = 0x00000000,
   HAL_DP_PROPERTY_TYPE_MAX,
   HAL_DP_PROPERTY_TYPE_FORCE_32BIT                        = 0x7FFFFFFF
} HAL_DP_PropertyType;

/*!
 * \enum HAL_DP_PllConfigFlagType
 *
 *
 * \brief
 *     Configuration flags for DP PLL.
 *
 */
typedef enum _HAL_DP_PllConfigFlagType
{
   HAL_DP_PLLCONFIG_FLAG_NONE                     = 0x00000000,
   HAL_DP_PLLCONFIG_FLAG_SVS                      = HAL_MDSS_BIT(1),   /**< enable PLL SVS mode */
   HAL_DP_PLLCONFIG_FLAG_SSC                      = HAL_MDSS_BIT(2),   /**< enable PLL SSC mode */   
   HAL_DP_PLLCONFIG_FLAG_ASYNC                    = HAL_MDSS_BIT(3),   /**< asynchronous mode   */
   HAL_DP_PLLCONFIG_FLAG_MAX                      = HAL_MDSS_BIT(30),
   HAL_DP_PLLCONFIG_FLAG_FORCE_32_BIT = 0x7FFFFFFF
} HAL_DP_PllConfigFlagType;



/* --------------------------------------------------------------------------------------------------------------------
** Macros and Struct
---------------------------------------------------------------------------------------------------------------------*/
#define HAL_DP_AUX_INTR_ALL        (HAL_DP_INTR_TYPE_AUX_I2C_DONE            |\
                                    HAL_DP_INTR_TYPE_WRONG_ADDRESS           |\
                                    HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT     |\
                                    HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER  |\
                                    HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT    |\
                                    HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C     |\
                                    HAL_DP_INTR_TYPE_AUX_DEFER_DURING_I2C    |\
                                    HAL_DP_INTR_TYPE_PLL_UNLOCK_DET          |\
                                    HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR        |\
                                    HAL_DP_INTR_TYPE_AUX_RX_STOP_ERR         |\
                                    HAL_DP_INTR_TYPE_AUX_RX_DEC_ERR          |\
                                    HAL_DP_INTR_TYPE_AUX_RX_SYNC_ERR         |\
                                    HAL_DP_INTR_TYPE_AUX_RX_ALIGN_ERR        |\
                                    HAL_DP_INTR_TYPE_AUX_TX_REQ_ERR)

#define HAL_DP_INTR_ALL            (HAL_DP_INTR_TYPE_HPD                     |\
                                    HAL_DP_INTR_TYPE_AUX_I2C_DONE            |\
                                    HAL_DP_INTR_TYPE_WRONG_ADDRESS           |\
                                    HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT     |\
                                    HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER  |\
                                    HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT    |\
                                    HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C     |\
                                    HAL_DP_INTR_TYPE_AUX_DEFER_DURING_I2C    |\
                                    HAL_DP_INTR_TYPE_PLL_UNLOCK_DET          |\
                                    HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR        |\
                                    HAL_DP_INTR_TYPE_READY_FOR_VIDEO         |\
                                    HAL_DP_INTR_TYPE_IDLE_PATTERNS_SENT      |\
                                    HAL_DP_INTR_TYPE_FRAME_END               |\
                                    HAL_DP_INTR_TYPE_CRC_UPDATED             |\
                                    HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR        |\
                                    HAL_DP_INTR_TYPE_AUX_RX_STOP_ERR         |\
                                    HAL_DP_INTR_TYPE_AUX_RX_DEC_ERR          |\
                                    HAL_DP_INTR_TYPE_AUX_RX_SYNC_ERR         |\
                                    HAL_DP_INTR_TYPE_AUX_RX_ALIGN_ERR        |\
                                    HAL_DP_INTR_TYPE_AUX_TX_REQ_ERR)


/*!
 * \struct HAL_DP_WritePacketType
 *
 * The \b HAL_DP_WritePacketType defines write parameters to the CMD_FIFO.
 *
 */
typedef struct
{
   HAL_DP_AuxTransactionModeType    eTransactionType;                  /**< AUX transaction type                    */
   uint32                           uNumofTransactions;                /**< # of Transaction in the buffer          */
   uint8                           *puWriteData;                       /**< Buffer of data to be written            */
   uint32                           uWriteDataSize;                    /**< Size of data to be written in byte      */
   uint32                           uBytesWritten;                     /**< How many bytes written                  */
} HAL_DP_WritePacketType;


/*!
 * \struct HAL_DP_ReadPacketType
 *
 * The \b HAL_DP_ReadPacketType defines read packet for AUX/I2C read from DATA_FIFO.
 *
 */
typedef struct
{
   uint8                           *puReadDataBuffer;                  /**< Data buffer for return values           */
   uint32                           uReadSize;                         /**< Data size to be read in byte            */
   uint32                           uBytesRead;                        /**< How many bytes Read from FIFO           */
} HAL_DP_ReadPacketType;

/*!
 * \struct HAL_DP_AUXAttribType
 *
 * The \b HAL_DP_AUXAttribType defines AUX attribute, i.e. Endian settings, the clock cycles before a Timeout, how
 * many time out will trigger interrupt. Default value should work.
 *
 */
typedef struct
{
   uint32                           *puTimeoutCount;                    /**< How many clk before timeout             */
   uint32                           *puTimeoutLimit;                    /**< TO # before an interrupt                */
   uint32                           *puReadZeroLimit;                   /**< Read # with no data before irq          */
   uint32                           *puNackDeferLimit;                  /**< nack/defer # before an irq              */
   uint32                           *puI2CDeferLimit;                   /**< I2C defer # before an irq               */
   bool32                           *pbStopTXOnPhyErr;                  /**< Stop TX upon PHY error?                 */
   bool32                           *pbTXLittleEndian;                  /**< is TX little Endian?                    */
   bool32                           *pbRXLittleEndian;                  /**< is RX little Endian?                    */
} HAL_DP_AUXAttribType;

/*!
 * \struct HAL_DP_AUXConfigType
 *
 * The \b HAL_DP_AUXConfigType defines AUX controller parameters
 *
 */
typedef struct
{
   HAL_DP_AUXAttribType            *psAUXAttrib;                       /**< Default value should work               */
} HAL_DP_AUXConfigType;

/*!
 * \struct HAL_DP_CustomPatternType
 *
 * The \b HAL_DP_CustomPatternType defines 80 bit custom pattern values
 *
 */
typedef struct
{
   uint32                           uCustomPattern_R0;                  /**< 80 Bit Pattern R0                       */
   uint32                           uCustomPattern_R1;                  /**< 80 Bit Pattern R1                       */
   uint32                           uCustomPattern_R2;                  /**< 80 Bit Pattern R2                       */
} HAL_DP_CustomPatternType;

/*!
 * \struct HAL_DP_MSAPacketType
 *
 * The \b HAL_DP_MSAPacketType defines Main stream attribute according to DP V1.2 spec
 *
 */
typedef struct                                                          /**< Main Stream Attribute                   */
{
   uint32                           uSWMVid;                            /**< SW M value for video                    */
   uint32                           uSWNVid;                            /**< SW N value for video                    */
   uint32                           uVisibleWidthInPixels;              /**< Visible active area width in pixels     */
   uint32                           uVisibleHeightInPixels;             /**< Visible active area height in pixels    */
   uint32                           uHsyncFrontPorchInPixels;           /**< HSYNC Front Porch width in pixels       */
   uint32                           uHsyncBackPorchInPixels;            /**< HSYNC Back Porch width in pixels        */
   uint32                           uHsyncPulseInPixels;                /**< HSYNC pulse width in pixels             */
   uint32                           uHsyncSkewInPixels;                 /**< HSYNC active edge is delayed from VSYNC */
   uint32                           uVsyncFrontPorchInLines;            /**< VSYNC Front Porch width in lines        */
   uint32                           uVsyncBackPorchInLines;             /**< VSYNC Back Porch width in lines         */
   uint32                           uVsyncPulseInLines;                 /**< VSYNC pulse width in lines              */
   uint32                           uHLeftBorderInPixels;               /**< Left border in pixels                   */
   uint32                           uHRightBorderInPixels;              /**< Right border in pixels                  */
   uint32                           uVTopBorderInLines;                 /**< Top border in lines                     */
   uint32                           uVBottomBorderInLines;              /**< Bottom border in lines                  */
   bool32                           bHSyncActiveLow;                    /**< Hsync polarity                          */
   bool32                           bVSyncActiveLow;                    /**< Vsync polarity                          */
   uint32                           uMisc0;                             /**< Misc data 0 defined by DP1.2            */
   uint32                           uMisc1;                             /**< Misc data 1 defined by DP1.2            */
} HAL_DP_MSAPacketType;

/*!
 * \struct HAL_DP_VSCPacketType
 *
 * The \b HAL_DP_VSCPacketType defines Video Stream Configuration according to DP V1.2 spec. Only used for 3D.
 *
 */
typedef struct                                                          /**< Video Stream Configuration              */
{
   bool32                           bSendVSC;                           /**< Sending VSC data?                       */
   uint8                            uVSC_DB0;                           /**< VSC DB0 Value                           */
   uint8                            uVSC_PB4;                           /**< VSC PB4 Value                           */
   uint8                            uVSC_HB0;                           /**< VSC HB0 Value                           */
   uint8                            uVSC_PB0;                           /**< VSC PB0 Value                           */
   uint8                            uVSC_HB1;                           /**< VSC HB1 Value                           */
   uint8                            uVSC_PB1;                           /**< VSC PB1 Value                           */
   uint8                            uVSC_HB2;                           /**< VSC HB2 Value                           */
   uint8                            uVSC_PB2;                           /**< VSC PB2 Value                           */
   uint8                            uVSC_HB3;                           /**< VSC HB3 Value                           */
   uint8                            uVSC_PB3;                           /**< VSC PB3 Value                           */
} HAL_DP_VSCPacketType;

/*!
 * \struct HAL_DP_StreamAttribType
 *
 * The \b HAL_DP_StreamAttribType defines the configuration that must provided by
 * host driver/panel driver
 *
 */
typedef struct
{
   bool32                           bSynchronousClk;                    /**< is link clk/pixel clk in Sync?          */
   bool32                           bEnhancedFraming;                   /**< Enable Enhanced eDP Framing?            */
   bool32                           bASSR;                              /**< Enable ASSR authentication?             */
   bool32                           bInterlaced;                        /**< interlaced mode                         */
} HAL_DP_StreamAttribType;

/*!
 * \struct HAL_DP_SinkAttribType
 *
 * The \b HAL_DP_SinkAttribType defines the DP panel HW attributes, which should not
 * change for a given panel.
 *
 */
typedef struct
{
   HAL_DP_LaneMapType              eLaneMapping;                       /**< Logical/physical lane map               */
   HAL_DP_PixelFormatType          ePixelFormat;                       /**< Pixel format                            */
   HAL_DP_RGBMapType               eRGBMapping;                        /**< RGB Mapping                             */
   uint32                          uLaneNumber;                        /**< # of lanes (1, 2, 4)                    */
   uint32                          uBitPerComponent;                   /**< Bit per Component                       */
} HAL_DP_SinkAttribType;

/*!
 * \struct HAL_DP_TUConfigType
 *
 * The \b HAL_DP_TUConfigType defines the Transfer Unit size related configuration.
 *
 */
typedef struct
{
   uint32                           uTuSize;                            /**< Desired TU Size                         */
   uint32                           uValidBoundaryLink;                 /**< Upper valid boundary                    */
   uint32                           uDelayStartLink;                    /**< # of clock cycles to delay              */
   bool32                           bBoundaryModEnable;                 /**< Enable boundary Moderation?             */
   uint32                           uValidLowerBoundary;                /**< Valid lower boundary link               */
   uint32                           uUpperBoundaryCount;                /**< Upper boundary Count                    */
   uint32                           uLowerBoundaryCount;                /**< Lower boundary Count                    */
} HAL_DP_TUConfigType;

/*!
 * \struct HAL_DP_OptionalType
 *
 * The \b HAL_DP_OptionalType defines the configuration that can work with the HW default
 * value.
 *
 */
typedef struct                                                          /**< Default values should work              */
{
   bool32                           *pbBypassLaneSkew;                  /**< Bypass Inter-lane Skew insert?          */
   bool32                           *pbBypassScrambler;                 /**< Bypass Scrambler?                       */
   bool32                           *pbBypass8B10B;                     /**< Bypass 8B10B? pads 0 to msb             */
   bool32                           *pbForcePixeClkOn;                  /**< Force Pixel Clk on                      */
   bool32                           *pbLCDSelfTest;                     /**< Drive LCD_Self_Test Pad?                */
   bool32                           *pbLinkBigEndian;                   /**< is main link little Endian?             */
   bool32                           *pbPixelBigEndian;                  /**< is Pixel data little Endian?            */
   uint32                           *puSafeToExitLevel;                 /**< Level that is safe to exit              */
   uint32                           *puIdlePatternRepNum;               /**< Idle pattern repetition #               */
   uint32                           *puMSA2VSCDelay;                    /**< delay between MSA and VSC               */
   uint32                           *puVSCRepNum;                       /**< # of VSC repetition                     */
   HAL_DP_CustomPatternType         *psCustomPattern;                   /**< 80 Bit Custom Pattern                   */
} HAL_DP_OptionalType;


/*!
 * \struct HAL_DP_HPDConfigType
 *
 * The \b HAL_DP_HPDConfigType defines the HPD configuration 
 * value.
 *
 */
typedef struct                                                          
{
  bool32        bEnable;               // Enable HPD hardware       
  bool32        bActiveLow;            // HPD pin is active low     
} HAL_DP_HPDConfigType;


/**<!
 * \struct HAL_DP_AuxHwCapsType
 *
 * The \b HAL_DP_AuxHwCapsType defines AUX HW capabiliites for caller to query
 *
 */
typedef struct
{
   uint32                           uAUXCmdFifoLen;                     /**< Length of AUX CMD_FIFO                  */
   uint32                           uMaxAUXCmdLen;                      /**< Max length of AUX CMD packet            */
   uint32                           uMaxI2CCmdLen;                      /**< Length of I2C CMD_FIFO                  */
} HAL_DP_AuxHwCapsType;




/*---------------------------------------------------------------------------------------------------------------------
Public Function declarations
---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************
*
** FUNCTION: HAL_DP_SetupBIST()
*/
/*!
* \brief
*     Set BIST test pattern in DP controller
*
* \param [in]   psMainStream    - Main stream info for the timing info
* \param [in]   dpOptions       - Test pattern ID. 0->disable test pattern
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_SetupBIST(HAL_DP_MSAPacketType *psMainStream, uint32 dpOptions);

/**********************************************************************************************************************
*
** FUNCTION: HAL_DP_Reset()
*/
/*!
* \brief
*     Resets DP HW Block
*
* \param [in]   eModuleTobeReset    - Bit array of HW module to be reset
* \param [in]   uResetFlag          - Reserved
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Reset(uint32 uModule, uint32 uResetFlag);


/**********************************************************************************************************************
*
** FUNCTION: HAL_DP_Interrupt_Clear()
*/
/*!
* \brief
*     Clears or Acknowledges the interrupts
*
* \param [in]   eInterruptSrc       - Interrupt sources to be clear.
* \param [in]   uClearFlags         - Reserved
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Interrupt_Clear(HAL_DP_InterruptType       eInterruptSrc,
                                          uint32                     uClearFlags );

/**********************************************************************************************************************
*
** FUNCTION: HAL_DP_Interrupt_GetStatus()
*/
/*!
* \brief
*     Returns the interrupts status
*
* \param [in/out] peIntrStatus      - Status for each module.
* \param [in]     uGetStatusFlags   - Reserved
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Interrupt_GetStatus(HAL_DP_InterruptType      *peIntrStatus,
                                              uint32                     uGetStatusFlags);

/**********************************************************************************************************************
*
** FUNCTION:HAL_DP_Write()
*/
/*!
* \brief
*     This API write AUX packet to CMD FIFO and trigger the transaction upon request.
*
* \param [in] psWritePacket         - pointer to data struct to be written out.
* \param [in] uWriteFlags           - Reserved
*
* \retval HAL_MDSS_ErrorType
*
**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Write(HAL_DP_WritePacketType    *psWritePacket,
                                uint32                     uWriteFlags);

/****************************************************************************

FUNCTION: HAL_DP_Read()
   
  This API allows users to read data from DATA FIFO, the read command has 
  already sent to panel by HAL_DP_AUXWriteFifo, and triggered.

Parameters:
  psReadPacket               -[in] pointer to data struct to be read.
  uAUXReadFlags              -[in] Reserved

Return: 
  HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Read(HAL_DP_ReadPacketType        *psReadPacket,
                               uint32                        uAUXReadFlags);


/****************************************************************************

FUNCTION: HAL_DP_AUXChannel_Enable()
   
  This API allows users to enable/disable AUX channel. 

Parameters:
  bEnable               -[in] TRUE = enable, FALSE = disable

Return: 
  void

****************************************************************************/
void HAL_DP_AUXChannel_Enable(bool32 bEnable);


/****************************************************************************

FUNCTION: HAL_DP_AUXChannel_Enable()
   
  This API allows users to get AUX channel status. 
  It also clears previous transaction details when done

Parameters:

Return: 
  AUX status

****************************************************************************/
HAL_DP_AUXStatusType HAL_DP_AUX_Status(void);

/****************************************************************************

FUNCTION: HAL_DP_AUX_ClearTransaction()
   
  This API allows users to clear previous AUX transaction. 

Parameters:

Return: 
  void

****************************************************************************/
void HAL_DP_AUX_ClearTransaction(void);

/****************************************************************************

FUNCTION: HAL_DP_AUX_GetHwCaps()
   
  This API allows users to get AUX HW capabilities. 

Parameters:
  psHwCap               -[out] pointer to hw caps

Return: 
  void

****************************************************************************/
void HAL_DP_AUX_GetHwCaps(HAL_DP_AuxHwCapsType  *psHwCap);

/**********************************************************************************************************************
FUNCTION: :HAL_DP_AUXConfig()
   
  This API Config AUX for AUX/I2C transaction.

Parameters: 
  psWritePacket              -[in]  pointer to data struct to be write out.
  uAUXConfigFlags            -[in] Reserved

Return:  
  HAL_MDSS_ErrorType

**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_AUXConfig(HAL_DP_AUXConfigType      *psAUXCtrlConfig,
                                    uint32                     uAUXConfigFlags);

/****************************************************************************
*
** FUNCTION: HAL_DP_Mainlink_Enable()
*/
/*!
* \brief
*   The \b Enable main link  
*
* \param [in] - bEnable TRUE - Enable, FALSE - Disable
*
* \retval void
*
****************************************************************************/
void HAL_DP_Mainlink_Enable(bool32 bEnable);

/**********************************************************************************************************************
FUNCTION: HAL_DP_SetLinkState()
   
   This API set the Main Link State

Parameters: 
   peLinkState                -[in]  Link state to set.

Return:  
   None

**********************************************************************************************************************/
void HAL_DP_SetLinkState(HAL_DP_LinkCommandType    peLinkState);

/**********************************************************************************************************************
FUNCTION: HAL_DP_SetTUConfig()
   
   This API set the TU configuration 

Parameters: 
   psTUConfig                 -[in]  pointer to TU configurations structure.

Return:  
   None

**********************************************************************************************************************/
void HAL_DP_SetTUConfig (HAL_DP_TUConfigType       *psTUConfig);

/**********************************************************************************************************************
FUNCTION: HAL_DP_SetSinkAttrib()
   
   This API set the sink attributes

Parameters: 
   psSinkAttrib               -[in]  pointer to sink attributes structure.

Return:  
   None

**********************************************************************************************************************/
void HAL_DP_SetSinkAttrib(HAL_DP_SinkAttribType     *psSinkAttrib);

/**********************************************************************************************************************
FUNCTION: HAL_DP_SetStreamAttrib()
   
   This API set the stream attributes

Parameters: 
   psStreamAttrib             -[in]  pointer to stream attributes structure.

Return:  
   None

**********************************************************************************************************************/
void HAL_DP_SetStreamAttrib (HAL_DP_StreamAttribType  *psStreamAttrib);

/**********************************************************************************************************************
FUNCTION: HAL_DP_SetMSA()
   
   This API set the MSA structure for main video stream.

Parameters: 
   psMainStream               -[in]  pointer to MSA structure.

Return:  
   None

**********************************************************************************************************************/
void HAL_DP_SetMSA(HAL_DP_MSAPacketType     *psMainStream);

/**********************************************************************************************************************
FUNCTION: HAL_DP_SetVSC()
   
   This API set the VSC structure for 3D video stream.

Parameters: 
   psVSCPacket                -[in]  pointer to VSC structure.

Return:  
   None

**********************************************************************************************************************/
void HAL_DP_SetVSC(HAL_DP_VSCPacketType      *psVSCPacket);

/**********************************************************************************************************************
FUNCTION: HAL_DP_SetOptionalConfig()
   
   This API Config optional settings for DP controller.

Parameters: 
   psCtrlOptionalCfg          -[in]  pointer to optional  configuration structure.

Return:  
   None

**********************************************************************************************************************/
void HAL_DP_SetOptionalConfig(HAL_DP_OptionalType      *psCtrlOptionalCfg);

/****************************************************************************
*
** FUNCTION: HAL_DP_Mainlink_Status()
*/
/*!
* \brief
*   The \b Get Main link status 
*
* \param [in] -   
*
* \retval bit-field of HAL_DP_LinkStatusType
*
****************************************************************************/
uint32 HAL_DP_Mainlink_Status(void);

/****************************************************************************
*
** FUNCTION: HAL_DP_HPD_Engine_Enable()
*/
/*!
* \brief
*   The \b Enable Hot Plug Detect functionality 
*
* \param [in] - bEnable TRUE/FALSE
*
* \retval void
*
****************************************************************************/
void HAL_DP_HPD_Engine_Enable(HAL_DP_HPDConfigType *psConfig);

/****************************************************************************
*
** FUNCTION: HAL_DP_HPD_PanelPlugged(void)
*/
/*!
* \brief
*   The \b Checks whether panel is plugged in
*
* \param [in] - 
*
* \retval True/False for connection
*
****************************************************************************/
bool32 HAL_DP_HPD_PanelPlugged();


/****************************************************************************
*
** FUNCTION: HAL_DP_ReadVersionInfo()
*/
/*!
* \brief
*     Returns the version information
*
* \param [in/out] psDPVersionInfo            - Version information
*
* \retval None
*
****************************************************************************/
void HAL_DP_ReadVersionInfo(HAL_HW_VersionType *psDPVersionInfo);


#endif // _HAL_DP_H_
