#ifndef __DPDRIVER__
#define __DPDRIVER__
/*=============================================================================

  File: edpdriver.h

  Copyright (c) 2012-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/


/* -----------------------------------------------------------------------
** Defines
** ----------------------------------------------------------------------- */
#define DP_DRIVER_MAX_MODES                  32
#define DP_DEVICE_NUM                        1
/* -----------------------------------------------------------------------
** Typedefs
** ----------------------------------------------------------------------- */
//-------------------------------------------------------------------------------------------------
//  DPModeType
//
//  @brief
//      Mode information for DP
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  uint32                           uIdxEDIDModeList;       // Index for this mode in sEDIDModeList which has all the info
} DPModeType;

/*
 * Internal structure for holding DP command data
 */
typedef struct {
    MDP_Display_IDType              eDisplayId;                                            /**< Physical display ID  */
    DP_HandleType                   hDPHandle;                                             /**< DP device handle for DP Host driver */
    uint32                          uLastHostErrorCode;                                    /**< DP last host error code for packet to peripheral */
    uint32                          uLastPanelErrorCode;                                   /**< DP last panel error code for packet from peripheral */
    uint32                          uCmdBufSize;                                           /**< DP command buffer size */
    DPModeType                      sMode[DP_DRIVER_MAX_MODES];                            /**< Mode info */
    uint32                          uTotalModes;                                           /**< Total number of supported modes */

} DPDriverConfigType;

/*
 * Internal structure for DP Command packets
 */
typedef struct {
    uint8       uDPCmd;         /**< DP Command type */
    uint8       aPayload[1];    /**< DP Payload Data type */
} DPPanelCommandType;

/* -----------------------------------------------------------------------
** Public Structures
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Public Functions
** ----------------------------------------------------------------------- */


#endif // __DPDRIVER__
