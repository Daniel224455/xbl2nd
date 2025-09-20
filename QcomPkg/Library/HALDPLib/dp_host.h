#ifndef _DP_HOST_H_
#define _DP_HOST_H_
/*! \file */

/*=============================================================================
 
  File: dp_host.h
 
  DP Host layer header file
  
 
 Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 =============================================================================*/

#include "hal_dp.h"
#include "dp_types.h"


/* --------------------------------------------------------------------------------------------------------------------
** Macros and Struct
---------------------------------------------------------------------------------------------------------------------*/

/* Define bit shift */
#define DP_BIT(x)            ((uint32)(1<<(x)))

/* DP flags */
typedef uint32               DP_FlagsType;

/* DP handle */
typedef void                *DP_HandleType;


/* --------------------------------------------------------------------------------------------------------------------
Enumerated types
---------------------------------------------------------------------------------------------------------------------*/
/*! 
 * \b DP_Status
 *
 * Enumeration giving return values from DP function calls.
 *
 */
typedef enum
{
   DP_STATUS_SUCCESS                              = 0x0000,
   DP_STATUS_FAIL,
   DP_STATUS_FAILED_INVALID_INPUT_PARAMETER,
   DP_STATUS_FAILED_TIMEOUT,
   DP_STATUS_FAILED_RESOURCE_FATAL_ERROR,
   DP_STATUS_FAILED_RESOURCE_FAILED,   
   DP_STATUS_FAILED_RESOURCE_NOT_READY,
   DP_STATUS_FAILED_DRIVER_ALREADY_INITIALIZED,
   DP_STATUS_FAILED_DRIVER_NOT_INITIALIZED,
   DP_STATUS_FAILED_NOT_SUPPORTED,
   DP_STATUS_FAILED_PKT_LENGTH_EXCEED,
   DP_STATUS_FAILED_LINK_TRAINING,
   DP_STATUS_MAX,
   DP_STATUS_FORCE_32BIT                          = 0x7FFFFFFF
} DP_Status;

/*! 
 * \b DP_LogLevel
 *
 * Enumeration giving DP debug log levels.
 *
 */
typedef enum 
{
  DP_LOGLEVEL_ERROR,
  DP_LOGLEVEL_WARNING,
  DP_LOGLEVEL_INFO,
  DP_LOGLEVEL_TRACE,
} DP_LogLevel;


/*!
* \b DP_ConnectionStatusType
*
* Enumeration DP cable connection status, 
*
*/
typedef enum
{
  DP_CONNECTION_STATUS_CONNECTED_POLARITY_A = 0,   /* DP cable is connected in normal polarity.    */
  DP_CONNECTION_STATUS_CONNECTED_POLARITY_B = 1,   /* DP cable is connected in reverted polarity.  */
  DP_CONNECTION_STATUS_DISCONNECTED         = 2,   /* DP cable is disconnected.                    */
  DP_CONNECTION_STATUS_MAX,
  DP_CONNECTION_STATUS_32BIT                = 0x7FFFFFFF
} DP_ConnectionStatusType;

/*!
* \b DP_FeatureMaskType
*
* Bit mask definition for DP features.
*
*/
typedef enum
{
  DP_FEATURE_MASK_DSC                       = DP_BIT(1),
  DP_FEATURE_MASK_FET                       = DP_BIT(2),
  DP_FEATURE_MASK_AUDIO                     = DP_BIT(3),
  DP_FEATURE_MASK_MST                       = DP_BIT(4),
  DP_FEATURE_MASK_HDCP_1_3                  = DP_BIT(5),
  DP_FEATURE_MASK_HDCP_2_2                  = DP_BIT(6),
  DP_FEATURE_MASK_YUV                       = DP_BIT(7),
  DP_FEATURE_MASK_PSR                       = DP_BIT(8),
  DP_FEATURE_MASK_PSR2                      = DP_BIT(9),
  DP_FEATURE_MASK_HDR                       = DP_BIT(10),
} DP_FeatureMaskType;

/* --------------------------------------------------------------------------------------------------------------------
  Functions
---------------------------------------------------------------------------------------------------------------------*/


/*********************************************************************************************
*
** FUNCTION: DP_Host_Open()
*/
/*!
* \DESCRIPTION
*        This init function must be called before any other DP function is called 
*
* \param [out]  phHandle        - Pointer to handle of DP Device
* \param [in]   psOpenConfig    - Pointer to host open configuration  
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_Open(DP_HandleType               *phHandle,
                        DP_HostOpenConfigType       *psOpenConfig);

/*********************************************************************************************
*
** FUNCTION: DP_Host_QueryPanelInfo()
*/
/*!
* \DESCRIPTION
*     Query EDID/DPCD info from panel
*
* \param [in]   hHandle           - Handle to DP device
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status DP_Host_QueryPanelInfo(DP_HandleType    hHandle);

/*********************************************************************************************
*
** FUNCTION: DP_Host_Close()
*/
/*!
* \DESCRIPTION
*  This API is called to de-initialize the corresponding resources that is initialized when 
*  DP_Open() is called. It will return an error if the device handle is invalid. All resources
*  will be de-initialized . User should ensure that no other DP function will be called before
*  another DP_Open() were called. otherwise corresponding error will be returned to the user.
*  Upon an invalid device handle, this call will also return an error.
*
* \param [in]   hHandle           - Handle to DP device
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_Close(DP_HandleType    hHandle);

/*********************************************************************************************
*
** FUNCTION: DP_Host_Commit()
*/
/*!
* \DESCRIPTION
*  This API allows users to commit all the settings and apply to H/W. 
*
* \param [in]   hHandle           - Handle to DP device
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_Commit(DP_HandleType    hHandle);

/*********************************************************************************************
*
** FUNCTION: DP_Host_SetProperty()
*/
/*!
* \DESCRIPTION
*      Allows the user to set the attributes and properties of the DP device
*
* \param [in]   hHandle        - Handle to DP Device
* \param [in]   ePropertyType  - Property ID that requires update
* \param [in]   pPropertyData  - Parameters of the property to be applied
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_Host_SetProperty(DP_HandleType                   hHandle,
                               DP_SetPropertyType              ePropertyType,
                               DP_SetPropertyParamsType       *pPropertyData);

 /*********************************************************************************************
 *
 ** FUNCTION: DP_Host_GetProperty()
 */
 /*!
 * \DESCRIPTION
 *      Allows the user to query the attributes and properties of the DP device
 *
 * \param [in]      hHandle        - Handle to DP device
 * \param [in]      ePropertyType  - Property ID query
 * \param [in/out]  pPropertyData  - Parameters of the property that will be returned
 *
 * \retval DP_Status
 *
 **********************************************************************************************/
DP_Status DP_Host_GetProperty(DP_HandleType                   hHandle,
                              DP_GetPropertyType              ePropertyType,
                              DP_GetPropertyParamsType       *pPropertyData);


/*********************************************************************************************
*
** FUNCTION: DP_Host_PSR()
*/
/*!
* \DESCRIPTION
*      Allows the user to query the attributes and properties of the DP device
*
* \param [in]   hHandle        - Logical handle of DP Device
* \param [in]   bEnterPSR      - Enable/Disable
*
* \retval DP_Status
*
**********************************************************************************************/
//DP_Status  DP_Host_PSR(DP_DeviceCtxType  *pDeviceCtx, bool32 bEnterPSR);

#endif //_DP_HOST_H_
