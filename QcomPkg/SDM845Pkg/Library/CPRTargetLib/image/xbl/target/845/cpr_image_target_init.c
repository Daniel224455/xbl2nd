/**
 * @file:  cpr_init.c
 *
 * Copyright (c) 2015-2016 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * QUALCOMM Proprietary and Confidential.
 *
 * $DateTime: 2017/09/08 08:52:50 $
 * $Header: //components/rel/boot.xf/2.0/QcomPkg/SDM845Pkg/Library/CPRTargetLib/image/xbl/target/845/cpr_image_target_init.c#5 $
 * $Change: 14313909 $
 */
#include <string.h>
#include "cpr_logs.h"
#include "cpr_data.h"
#include "cpr_rail.h"
#include "cpr_smem.h"
#include "cpr_image.h"
#include "cpr_measurements.h"
#include "cpr_cfg.h"
#include "cpr_hal.h"
#include "cpr_utils.h"
#include "cpr_image_target_init.h"

//******************************************************************************
// Local Helper Functions
//******************************************************************************


static cpr_cfg_funcs* init_open_loop(cpr_rail* rail)
{
    /*
     * Napali V1:
     * GLOBAL_RC = 0 – All global rails blown at ceiling voltages
     * GLOBAL_RC = 1 – All global rails blown at CPR recommended voltages, includes all aging fuses blown with correct values for all rails except AON
     * GLOBAL_RC = 2 – All global rails blown at CPR recommended voltages, includes all aging fuses blown correctly
     *
     * Napali V2:
     * GLOBAL_RC = 0 – All global rails blown at CPR recommended voltages, includes all aging fuses blown correctly 
     */

    uint8 rev = 0;
    cpr_rail_state *railState = NULL;

    CPR_LOG_TRACE( "Initializing open loop on %s", rail->name );

    railState = cpr_utils_get_rail_state(rail->id);
    rev = railState ? railState->railCPRRev: 0;

    if (cpr_info.chipVersion < CHIPINFO_VERSION(2,0))
        cpr_rail_set_initial_voltages( rail, (rev == 0), false );
    else
        cpr_rail_set_initial_voltages( rail, false, false );

    return NULL;
}

static cpr_cfg_funcs* init_settle(cpr_rail* rail)
{
    CPR_LOG_TRACE( "Settling on %s", rail->name );

    for(int i = 0; i < rail->settleModesCount; i++) {
        cpr_measurements_settle( rail, rail->settleModes[i] );
    }

    return NULL;
}

static cpr_cfg_funcs* init_closed_loop(cpr_rail* rail)
{
    cpr_rail_init_rail_hw( rail, true );

    cpr_measurements_aging( rail );

    init_open_loop(rail);

    CPR_LOG_TRACE( "Configuring closed loop on %s", rail->name );
    cpr_rail_calculate_target_quotients( rail,
            &cpr_info.railStates[rail->railIdx] );

    init_settle( rail );

    return NULL;
}

static cpr_cfg_funcs* init_hw_closed_loop(cpr_rail* rail) {
    init_open_loop( rail );
    cpr_rail_calculate_target_quotients( rail,
            &cpr_info.railStates[rail->railIdx] );
    return NULL;
}

static cpr_cfg_funcs* init_hw_open_loop(cpr_rail* rail) {
    init_open_loop( rail );
    return NULL;
}

/*
 * CPR config functions
 */
static cpr_cfg_funcs init_funcs[CPR_INIT_FUNCS_COUNT] =
{
    [CPR_INIT_NONE          ] = {.cMode = CPR_CONTROL_NONE,           .init = NULL,                .enable = NULL},
    [CPR_INIT_OPEN_LOOP     ] = {.cMode = CPR_CONTROL_OPEN_LOOP,      .init = init_open_loop,      .enable = NULL},
    [CPR_INIT_SW_CLOSED_LOOP] = {.cMode = CPR_CONTROL_SW_CLOSED_LOOP, .init = init_closed_loop,    .enable = NULL},
    [CPR_INIT_HW_CLOSED_LOOP] = {.cMode = CPR_CONTROL_HW_CLOSED_LOOP, .init = init_hw_closed_loop, .enable = NULL},
    [CPR_INIT_HW_OPEN_LOOP  ] = {.cMode = CPR_CONTROL_HW_OPEN_LOOP,   .init = init_hw_open_loop,   .enable = NULL},
};

//******************************************************************************
// Default Enablement Structures
//******************************************************************************

cpr_cfg_funcs* cpr_image_target_get_init_func(cpr_init_funcs_id func_id)
{
    CPR_ASSERT(func_id < CPR_INIT_FUNCS_COUNT);

    return &init_funcs[func_id];
}

