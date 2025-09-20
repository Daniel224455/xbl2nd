/**
 * @file:  cpr_image.c
 *
 * Copyright (c) 2015-2018 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * QUALCOMM Proprietary and Confidential.
 *
 * $Change: 30389851 $
 */
#include <stdlib.h>
#include "cpr_image.h"
#include "cpr_image_defs.h"
#include "cpr_image_target.h"
#include "cpr_cfg.h"
#include "cpr_data.h"
#include "cpr.h"
#include "cpr_smem.h"
#include "cpr_utils.h"

#include "railway.h"
#include "CoreHeap.h"
#include "CoreVerify.h"
#include "CoreMsgRam.h"
#include "DALSys.h"
#include "smem.h"
#include "LoaderUtils.h"
#include "rpmh_client.h"
#include "cmd_db.h"
#include "voltage_level.h"

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/EFIClock.h>
#include "gpu_clk_hwio.h"

//******************************************************************************
// Global Data
//******************************************************************************

typedef struct
{
    cpr_domain_id   cpr_rail_id;
    const char*     railway_name;
} rail_name_map;

const static rail_name_map railway_name_map[] =
{
    { CPR_RAIL_MX,       "vddmx"      },
    { CPR_RAIL_CX,       "vddcx"      },
    { CPR_RAIL_VDDA,     "vdda_ebi"   },
    { CPR_RAIL_SSC_MX,   "vdd_ssc_mx" },
    { CPR_RAIL_SSC_CX,   "vdd_ssc_cx" },
    { CPR_RAIL_LPI_MX,   "vdd_lpi_mx" },
    { CPR_RAIL_LPI_CX,   "vdd_lpi_cx" },
    { CPR_RAIL_WLAN,     "vdd_wlan" },
};

typedef struct
{
    boolean         init;
    int32           railway_rail_id;
    railway_voter_t voter;
} railway_handle;

static railway_handle *handles;


//******************************************************************************
// Local Helper Functions
//******************************************************************************

static const char* cpr_rpm_railway_rail_name(cpr_domain_id rail_id)
{
    for(int i = 0; i < (sizeof(railway_name_map) / sizeof(rail_name_map)); i++) {
        if(rail_id == railway_name_map[i].cpr_rail_id) {
            return railway_name_map[i].railway_name;
        }
    }

    CPR_ASSERT( 0 );
    return NULL;
}

static railway_handle* get_railway_handle(cpr_domain_id railId)
{
    uint32 railIdx = cpr_utils_get_rail_idx( railId );

    if(!handles)
    {
        handles = cpr_image_malloc(sizeof(railway_handle) * cpr_info.settings->railsCount);
    }

    if(!handles[railIdx].init)
    {
        handles[railIdx].railway_rail_id = rail_id( cpr_rpm_railway_rail_name( railId ) );
        CPR_ASSERT( handles[railIdx].railway_rail_id != RAIL_NOT_SUPPORTED_BY_RAILWAY );

        handles[railIdx].init = true;
    }

    return &handles[railIdx];
}

/*static void cpr_image_pre_switch(const railway_settings *settings, void* ctx)
{
    cpr_domain_id railId = (cpr_domain_id)(uintptr_t)ctx;
    cpr_pre_state_switch( railId );
}

static void cpr_image_post_switch(const railway_settings *settings, void* ctx)
{
    cpr_domain_id railId = (cpr_domain_id)(uintptr_t)ctx;
    cpr_post_state_switch( railId, (cpr_voltage_mode)settings->mode, settings->microvolts, CPR_UNKNOWN_FREQ );
}*/

//******************************************************************************
// Public API Functions
//******************************************************************************

void* cpr_image_malloc(uint32 size)
{
    void* buf = NULL;
    buf = Core_Malloc(size);
    if (buf == NULL)
        CPR_ASSERT(0);

    memset( buf, 0, size );
    return buf;
}

void cpr_image_free(void* buf)
{
    Core_Free( buf );
}

void cpr_image_memscpy(void* dst, const void* src, uint32 size)
{
    memscpy(dst, size, src, size);
}

void cpr_image_register_thermal_cb(cpr_therm_cb cb)
{

}

void cpr_image_register_isr(cpr_domain_id railId, uint32 interrupt, cpr_image_isr isr, void* ctx)
{

}

void cpr_image_enable_clock(const char* clkId)
{

}

uint32 cpr_image_get_chip_version()
{
    return DalChipInfo_ChipVersion();
}

cpr_foundry_id cpr_image_get_foundry()
{
    //TODO fix this when we have support
    /*DalChipInfoFoundryIdType foundryID = DalChipInfo_FoundryId();

    switch(foundryID)
    {
        case(DALCHIPINFO_FOUNDRYID_TSMC):
            return CPR_FOUNDRY_TSMC;
        case(DALCHIPINFO_FOUNDRYID_GF):
            return CPR_FOUNDRY_GF;
        case(DALCHIPINFO_FOUNDRYID_SS):
            return CPR_FOUNDRY_SS;
        case(DALCHIPINFO_FOUNDRYID_IBM):
            return CPR_FOUNDRY_IBM;
        case(DALCHIPINFO_FOUNDRYID_UMC):
            return CPR_FOUNDRY_UMC;
        default:
            break;
    }*/
    return CPR_FOUNDRY_TSMC;
    //return 0;       // Should never get here.
}

boolean cpr_image_set_rail_mode(cpr_domain_id railId, cpr_domain_info* info)
{
    return cpr_image_target_set_rail_mode(railId, info);
}

boolean cpr_image_rail_transition_voltage(cpr_domain_id railId)
{
    //Not needed for Core.
    return true;
}

void cpr_image_open_remote_cfg(void** cfg, uint32* size)
{
    *cfg = smem_get_addr( SMEM_CPR_CONFIG, size );

    if(!(*cfg))
    {
        /*
         * KW does not like CPR_LOG_FATAL() 
         */
        CPR_LOG_ERROR("Failed to get smem addr");
        CPR_ASSERT(0);
    }
}

void cpr_image_close_remote_cfg()
{
    //Not needed for Core.
}

void cpr_image_wait(uint32 us)
{
    DALSYS_BusyWait( us );
}

//Function to be called after SMEM is initialized to push out the CPR settings
//to SMEM. 
void cpr_externalize_state()
{
    //Not needed for Core.
}

uint32 cpr_image_get_eldo_voltage(cpr_domain_id railId, cpr_domain_info* info)
{
    //Not needed for Core.
    return 0;
}

void cpr_image_enter_sleep(void)
{
    //Not needed for Core.
}

void cpr_image_exit_sleep(void)
{
    //Not needed for Core.
}

boolean cpr_image_can_resume_control(cpr_domain_id railId)
{
    //Not needed for Core.
    return true;
}

void cpr_image_enable_measurements(cpr_rail* rail, cpr_measurement_type type)
{
    cpr_image_target_enable_measurements(rail, type);
}

void cpr_image_disable_measurements(cpr_rail* rail, cpr_measurement_type type)
{
    cpr_image_target_disable_measurements(rail, type);
}

const cpr_settings* cpr_image_get_settings(void)
{
    return cpr_image_target_get_settings();
}

uint8 cpr_image_get_cprf_id(cpr_rail *rail)
{
    switch (rail->id)
    {
        case CPR_RAIL_MX: return 0;
        case CPR_RAIL_CX: return 1;
        case CPR_RAIL_VDDA: return 2;
        case CPR_RAIL_LPI_CX: return 3;
        case CPR_RAIL_LPI_MX: return 4;
        case CPR_RAIL_GFX: return 5;
        case CPR_RAIL_MSS: return 6;
        case CPR_RAIL_MM: return 7;
        default: return -1;
    }
}

const char * cpr_image_get_vrm_map(const char *name)
{
    if (strncmp(name, "GFX", strlen("GFX")) == 0)
    {
        return "vrm.gfx";
    }
    else if (strncmp(name, "MM", strlen("MM")) == 0)
    {
        return "vrm.mmcx";
    }
    else if (strncmp(name, "MSS", strlen("MSS")) == 0)
    {
        return "vrm.mss";
    }

    CPR_ASSERT(0);
    return NULL;
}

/*
 * Wrapper functions for Railway declared in cpr_image_api.h
 */
uint32 cpr_railway_get_voltage(int railwayRailId, railway_corner railwayCorner)
{
    //Not yet supported for core.
    return 0;
}

void cpr_railway_get_supported_corners(int railwayRailId, railway_corner *corners, uint32 *cornerCount )
{
    //Not yet supported for core.
}

