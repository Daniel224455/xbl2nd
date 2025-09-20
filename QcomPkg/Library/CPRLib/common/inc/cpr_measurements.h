/**
 * @file:  cpr_measurements.h
 * @brief:
 *
 * Copyright (c) 2015-2016 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * QUALCOMM Proprietary and Confidential.
 *
 * $DateTime: 2017/06/26 10:47:50 $
 * $Header: //components/rel/boot.xf/2.0/QcomPkg/Library/CPRLib/common/inc/cpr_measurements.h#3 $
 * $Change: 13672265 $
 */
#ifndef CPR_MEASUREMENTS_H
#define	CPR_MEASUREMENTS_H

#include "cpr_types.h"
#include "cpr_cfg.h"

void cpr_measurements_settle(cpr_rail* rail, cpr_voltage_mode mode);
void cpr_measurements_thermal_update(cpr_thermal_region region);
void cpr_measurements_aging(cpr_rail* rail);

#endif
