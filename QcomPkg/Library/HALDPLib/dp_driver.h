#ifndef __DPDRIVER__
#define __DPDRIVER__
/*=============================================================================

  File: edpdriver.h

  Copyright (c) 2012-2020 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/


/* -----------------------------------------------------------------------
** Defines
** ----------------------------------------------------------------------- */
#define DP_DRIVER_MAX_MODES                  32

#ifdef DPDRIVER_EDP_MODE
#define DP_DEVICE_NUM                        1
#else
#define DP_DEVICE_NUM                        6
#endif

#endif // __DPDRIVER__
