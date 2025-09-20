/**
 * @file:  cpr_image_target.c
 *
 * Copyright (c) 2015-2017 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * QUALCOMM Proprietary and Confidential.
 *
 * $DateTime: 2017/06/26 10:47:50 $
 * $Header: //components/rel/boot.xf/2.0/QcomPkg/SDM845Pkg/Library/CPRTargetLib/image/xbl/target/845/cpr_image_target.c#5 $
 * $Change: 13672265 $
 */
#include "cpr_image_target.h"
#include "cpr_logs.h"


static uint32 cpr_pte_qc_spare_fuse;

const cpr_settings* cpr_image_target_get_settings(void)
{
    extern const cpr_settings cpr_settings_845;

    return &cpr_settings_845;
}

boolean cpr_image_target_is_turbo_mode_only(void)
{
    /*
     * CR 1050500:
     * TURBO_MODE_ONLY fuse: QC Spare 16[45] - bit 13 of 0x780324/0x784324
     */
    cpr_pte_qc_spare_fuse = CPR_HWIO_IN(SECURITY_CONTROL_CORE_REG_BASE + 0x004324);

    return ((cpr_pte_qc_spare_fuse & (1 << 13)) != 0);
}

cpr_domain_id cpr_image_target_railway_rail_to_cpr_domain(int railway_rail)
{
    switch(railway_rail)
    {
        case 0: return CPR_RAIL_MX;
        case 1: return CPR_RAIL_CX;
        case 2: return CPR_RAIL_VDDA;
        case 3: return CPR_RAIL_LPI_MX;
        case 4: return CPR_RAIL_LPI_CX;
        case 5: return CPR_RAIL_WLAN;
        default:
            CPR_LOG_FATAL("Unsupported rail %u", railway_rail);
            break;
    }

    CPR_ASSERT(0);

    return (cpr_domain_id)0;
}

railway_corner cpr_image_target_cpr_mode_to_railway_corner(cpr_voltage_mode mode)
{
    railway_corner map[CPR_VOLTAGE_MODE_COUNT] = {
        [CPR_VOLTAGE_MODE_OFF                ] = RAILWAY_NO_REQUEST,
        [CPR_VOLTAGE_MODE_RETENTION          ] = RAILWAY_RETENTION,
        [CPR_VOLTAGE_MODE_MIN_SVS            ] = RAILWAY_SVS_MIN,
        [CPR_VOLTAGE_MODE_LOW_SVS            ] = RAILWAY_SVS_LOW,
        [CPR_VOLTAGE_MODE_SVS                ] = RAILWAY_SVS,
        [CPR_VOLTAGE_MODE_SVS_L1             ] = RAILWAY_SVS_HIGH,
        [CPR_VOLTAGE_MODE_NOMINAL            ] = RAILWAY_NOMINAL,
        [CPR_VOLTAGE_MODE_NOMINAL_L1         ] = RAILWAY_TURBO,
        [CPR_VOLTAGE_MODE_TURBO              ] = RAILWAY_TURBO,
        [CPR_VOLTAGE_MODE_TURBO_L1           ] = RAILWAY_TURBO_L1,
        [CPR_VOLTAGE_MODE_SUPER_TURBO        ] = RAILWAY_SUPER_TURBO,
        [CPR_VOLTAGE_MODE_SUPER_TURBO_NO_CPR ] = RAILWAY_SUPER_TURBO_NO_CPR,
    };

    return (mode < CPR_VOLTAGE_MODE_COUNT) ? map[mode] : map[CPR_VOLTAGE_MODE_COUNT - 1];
}

cpr_voltage_mode cpr_image_target_railway_corner_to_cpr_mode(railway_corner corner)
{
    cpr_voltage_mode map[RAILWAY_CORNERS_COUNT] = {
        [RAILWAY_NO_REQUEST        ] = CPR_VOLTAGE_MODE_OFF,
        [RAILWAY_RETENTION         ] = CPR_VOLTAGE_MODE_RETENTION,
        [RAILWAY_SVS_MIN           ] = CPR_VOLTAGE_MODE_MIN_SVS,
        [RAILWAY_SVS_LOW           ] = CPR_VOLTAGE_MODE_LOW_SVS,
        [RAILWAY_SVS               ] = CPR_VOLTAGE_MODE_SVS,
        [RAILWAY_SVS_HIGH          ] = CPR_VOLTAGE_MODE_SVS_L1,
        [RAILWAY_NOMINAL           ] = CPR_VOLTAGE_MODE_NOMINAL,
        [RAILWAY_TURBO             ] = CPR_VOLTAGE_MODE_TURBO,
        [RAILWAY_TURBO_L1          ] = CPR_VOLTAGE_MODE_TURBO_L1,
        [RAILWAY_SUPER_TURBO       ] = CPR_VOLTAGE_MODE_SUPER_TURBO,
        [RAILWAY_SUPER_TURBO_NO_CPR] = CPR_VOLTAGE_MODE_SUPER_TURBO_NO_CPR,
    };

    return (corner < RAILWAY_CORNERS_COUNT) ? map[corner] : map[RAILWAY_CORNERS_COUNT - 1];
}

void cpr_image_target_enable_measurements(cpr_rail* rail, cpr_measurement_type type)
{
    /* TODO: set rail to PWM */
}

void cpr_image_target_disable_measurements(cpr_rail* rail, cpr_measurement_type type)
{
    /* TODO: reset rail */
}
