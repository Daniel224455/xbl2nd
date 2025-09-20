/*******************************************************************************
 *
 * Copyright (c) 2024 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * QUALCOMM Proprietary and Confidential.
 *
 * THIS IS AUTO-GENERATED CPR CONFIG FILE FOR 1000.
 *
 * DateTime: 2024/06/24 00:20:33
 *
 *******************************************************************************/

#include "cpr_data.h"
#include "cpr_image_target_init.h"
#include "cpr_target_hwio.h"


//hash value of Voltage Plan file (extracted all cell values)
static char voltage_plan_hash_value[] = "54BEC6358FA2F5A0933FB4BF2005B94C";


/*******************************************************************************
********************************************************************************
**
** Voltage Plan Data
**
********************************************************************************
********************************************************************************/

static cpr_margin_cfg margins_300510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  100000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


static cpr_fuse_data  fuses_3952340992 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR3_TARG_VOLT_SVS3) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_0 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_655279266 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR3_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_2626994354 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR3_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_1606482579 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR3_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


/*
 * VDDA Voltage Plan
 */
static cpr_voltage_plan voltage_plan_13764417942 =
{
    .modesCount      = 6,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_3952340992, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  500000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 50} } },

        // SVS
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_655279266, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 57} } },

        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_2626994354, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  688000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_1606482579, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_version  ver_ANY_1_0_1_255 = { .foundry = CPR_FOUNDRY_ANY, .min = CPR_CHIPINFO_VERSION(1,0), .max = CPR_CHIPINFO_VERSION(1,255) };


static cpr_margin_cfg margins_165510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   22000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_5432455552 =
{
    .count = 2,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  342 },
                                      { .ro =  8, .quotient =  511 } },
};


static cpr_fuse_data  fuses_2311445188 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR10_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW11_MSB, CPR10_TARG_VOLT_OFFSET_SVS2) } } },
};


static cpr_margin_cfg margins_205510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   30000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_4651845514 =
{
    .count = 2,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  472 },
                                      { .ro =  8, .quotient =  674 } },
};


static cpr_fuse_data  fuses_3429767487 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR10_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW9_LSB, CPR10_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_190510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  -15000, .closedLoop =   39000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_5355928531 =
{
    .count = 2,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  590 },
                                      { .ro =  8, .quotient =  817 } },
};


static cpr_fuse_data  fuses_2263058797 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW9_LSB, CPR10_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_135510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  -15000, .closedLoop =   28000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_4366364116 =
{
    .count = 2,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  785 },
                                      { .ro =  8, .quotient = 1033 } },
};


static cpr_fuse_data  fuses_3889535621 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR10_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW9_LSB, CPR10_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_182510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  -15000, .closedLoop =   35000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_13849825356 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient = 1300 },
                                      { .ro =  3, .quotient = 1120 },
                                      { .ro =  8, .quotient = 1221 },
                                      { .ro =  9, .quotient = 1058 },
                                      { .ro = 14, .quotient = 1005 },
                                      { .ro = 15, .quotient = 1017 } },
};


static cpr_fuse_data  fuses_6678369296 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR10_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW8_MSB, CPR10_TARG_VOLT_OFFSET_TUR) } } },
};


static cpr_kv_cfg  kvs_35568964360 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =    0 },
                                { .ro =  1, .kv =  363 },
                                { .ro =  2, .kv =    0 },
                                { .ro =  3, .kv =  333 },
                                { .ro =  4, .kv =    0 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =  300 },
                                { .ro =  7, .kv =    0 },
                                { .ro =  8, .kv =  324 },
                                { .ro =  9, .kv =  300 },
                                { .ro = 10, .kv =    0 },
                                { .ro = 11, .kv =    0 },
                                { .ro = 12, .kv =    0 },
                                { .ro = 13, .kv =    0 },
                                { .ro = 14, .kv =  259 },
                                { .ro = 15, .kv =  258 } },
};


/*
 * MM Voltage Plan
 */
static cpr_voltage_plan voltage_plan_11526383673 =
{
    .modesCount      = 5,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_35568964360,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_2311445188, .quotients = &quotients_5432455552, .margins = &margins_165510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  500000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_3429767487, .quotients = &quotients_4651845514, .margins = &margins_205510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_2263058797, .quotients = &quotients_5355928531, .margins = &margins_190510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_3889535621, .quotients = &quotients_4366364116, .margins = &margins_135510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_6678369296, .quotients = &quotients_13849825356, .margins = &margins_182510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_version  ver_ANY_2_0_2_255 = { .foundry = CPR_FOUNDRY_ANY, .min = CPR_CHIPINFO_VERSION(2,0), .max = CPR_CHIPINFO_VERSION(2,255) };


static cpr_margin_cfg margins_840510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  100000, .closedLoop =  100000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_7565264404 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  407 },
                                      { .ro =  7, .quotient =  204 },
                                      { .ro =  9, .quotient =  422 },
                                      { .ro = 10, .quotient =  279 } },
};


static cpr_fuse_data  fuses_1236501989 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_SVS3) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_quotient_cfg  quotients_11389963752 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  558 },
                                      { .ro =  7, .quotient =  327 },
                                      { .ro =  9, .quotient =  563 },
                                      { .ro = 10, .quotient =  423 } },
};


static cpr_fuse_data  fuses_1584637373 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_quotient_cfg  quotients_19206360606 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient =  817 },
                                      { .ro =  1, .quotient =  805 },
                                      { .ro =  2, .quotient =  701 },
                                      { .ro =  9, .quotient =  699 },
                                      { .ro = 10, .quotient =  552 },
                                      { .ro = 15, .quotient =  394 } },
};


static cpr_quotient_cfg  quotients_15229568806 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1061 },
                                      { .ro =  1, .quotient = 1059 },
                                      { .ro =  2, .quotient =  920 },
                                      { .ro =  9, .quotient =  903 },
                                      { .ro = 10, .quotient =  779 },
                                      { .ro = 15, .quotient =  544 } },
};


static cpr_fuse_data  fuses_2673371045 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_852510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  100000, .closedLoop =  100000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_10660182061 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1278 },
                                      { .ro =  1, .quotient = 1299 },
                                      { .ro =  2, .quotient = 1106 },
                                      { .ro =  9, .quotient = 1063 },
                                      { .ro = 10, .quotient =  986 },
                                      { .ro = 15, .quotient =  696 } },
};


static cpr_quotient_cfg  quotients_14558608459 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1417 },
                                      { .ro =  1, .quotient = 1434 },
                                      { .ro =  2, .quotient = 1234 },
                                      { .ro =  9, .quotient = 1177 },
                                      { .ro = 10, .quotient = 1103 },
                                      { .ro = 15, .quotient =  782 } },
};


static cpr_fuse_data  fuses_599821542 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR0_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_kv_cfg  kvs_32421585693 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =  332 },
                                { .ro =  1, .kv =  338 },
                                { .ro =  2, .kv =  312 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =    0 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =    0 },
                                { .ro =  7, .kv =  288 },
                                { .ro =  8, .kv =    0 },
                                { .ro =  9, .kv =  276 },
                                { .ro = 10, .kv =  296 },
                                { .ro = 11, .kv =    0 },
                                { .ro = 12, .kv =    0 },
                                { .ro = 13, .kv =    0 },
                                { .ro = 14, .kv =    0 },
                                { .ro = 15, .kv =  201 } },
};


/*
 * CX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_13729291978 =
{
    .modesCount      = 6,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_32421585693,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1 },
    .modes           = (struct cpr_voltage_data[]) {
        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_1236501989, .quotients = &quotients_7565264404, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  516000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_1584637373, .quotients = &quotients_11389963752, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_19206360606, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_2673371045, .quotients = &quotients_15229568806, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_10660182061, .margins = &margins_852510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  712000, .freq =       0, .interplEnabled = 60} } },

        // TURBO_L1
        {.fref = 888000, .freqDelta =       0, .fuses = &fuses_599821542, .quotients = &quotients_14558608459, .margins = &margins_852510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  952000, .floor =  756000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =       0, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


static cpr_fuse_data  fuses_2084319257 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR5_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_29245628 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR5_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


/*
 * LPI_MX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_5492981749 =
{
    .modesCount      = 3,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // SVS
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  752000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_2084319257, .quotients = NULL, .margins = &margins_510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_29245628, .quotients = NULL, .margins = &margins_510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  848000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


/*
 * WMSS_CX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_5035517949 =
{
    .modesCount      = 3,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_NOMINAL },
    .modes           = (struct cpr_voltage_data[]) {
        // LOW_SVS
        {.fref = 600000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  656000, .floor =  530000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 668000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  728000, .floor =  596000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 744000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  652000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_200510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   29000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_10360978392 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  223 },
                                      { .ro =  8, .quotient =  365 },
                                      { .ro =  9, .quotient =  285 },
                                      { .ro = 11, .quotient =  747 } },
};


static cpr_fuse_data  fuses_5696447649 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_SVS3) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW11_MSB, CPR1_TARG_VOLT_OFFSET_SVS3_3_1), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW11_LSB, CPR1_TARG_VOLT_OFFSET_SVS3_0) } } },
};


static cpr_margin_cfg margins_255510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   40000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_8343000424 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  342 },
                                      { .ro =  8, .quotient =  511 },
                                      { .ro =  9, .quotient =  420 },
                                      { .ro = 11, .quotient =  999 } },
};


static cpr_fuse_data  fuses_2319898897 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_SVS2) } } },
};


static cpr_margin_cfg margins_210510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   31000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_11287828000 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  472 },
                                      { .ro =  8, .quotient =  674 },
                                      { .ro =  9, .quotient =  562 },
                                      { .ro = 11, .quotient = 1267 } },
};


static cpr_fuse_data  fuses_382500705 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_240510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   37000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_9704711232 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  590 },
                                      { .ro =  8, .quotient =  817 },
                                      { .ro =  9, .quotient =  697 },
                                      { .ro = 11, .quotient = 1485 } },
};


static cpr_fuse_data  fuses_313224298 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_quotient_cfg  quotients_13146401973 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient = 1059 },
                                      { .ro =  6, .quotient =  785 },
                                      { .ro =  8, .quotient = 1033 },
                                      { .ro =  9, .quotient =  900 },
                                      { .ro = 11, .quotient = 1885 } },
};


static cpr_fuse_data  fuses_5682902217 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_225510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   34000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_10175423919 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient = 1221 },
                                      { .ro =  6, .quotient =  940 },
                                      { .ro =  8, .quotient = 1155 },
                                      { .ro =  9, .quotient = 1003 },
                                      { .ro = 11, .quotient = 2071 } },
};


static cpr_fuse_data  fuses_3249579422 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_247510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   36000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_9126134217 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient = 1300 },
                                      { .ro =  6, .quotient = 1009 },
                                      { .ro =  8, .quotient = 1221 },
                                      { .ro =  9, .quotient = 1058 },
                                      { .ro = 11, .quotient = 2168 } },
};


static cpr_fuse_data  fuses_6138978676 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR1_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_TUR) } } },
};


static cpr_margin_cfg margins_492510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    5000, .closedLoop =   85000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_9618572727 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient = 1435 },
                                      { .ro =  6, .quotient = 1127 },
                                      { .ro =  8, .quotient = 1337 },
                                      { .ro =  9, .quotient = 1172 },
                                      { .ro = 11, .quotient = 2344 } },
};


static cpr_fuse_data  fuses_5975672073 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR1_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR1_TARG_VOLT_OFFSET_SUT) } } },
};


static cpr_kv_cfg  kvs_35528992078 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =    0 },
                                { .ro =  1, .kv =  363 },
                                { .ro =  2, .kv =    0 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =    0 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =  300 },
                                { .ro =  7, .kv =    0 },
                                { .ro =  8, .kv =  324 },
                                { .ro =  9, .kv =  300 },
                                { .ro = 10, .kv =    0 },
                                { .ro = 11, .kv =  518 },
                                { .ro = 12, .kv =    0 },
                                { .ro = 13, .kv =    0 },
                                { .ro = 14, .kv =    0 },
                                { .ro = 15, .kv =    0 } },
};


/*
 * MSS Voltage Plan
 */
static cpr_voltage_plan voltage_plan_17380090077 =
{
    .modesCount      = 8,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_35528992078,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_NOMINAL_L1,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1 },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 516000, .freqDelta =       0, .fuses = &fuses_5696447649, .quotients = &quotients_10360978392, .margins = &margins_200510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  612000, .floor =  484000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_2319898897, .quotients = &quotients_8343000424, .margins = &margins_255510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  500000, .freq =       0, .interplEnabled = 50} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_382500705, .quotients = &quotients_11287828000, .margins = &margins_210510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_313224298, .quotients = &quotients_9704711232, .margins = &margins_240510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_5682902217, .quotients = &quotients_13146401973, .margins = &margins_190510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL_L1
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_3249579422, .quotients = &quotients_10175423919, .margins = &margins_225510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  688000, .freq =       0, .interplEnabled = 60} } },

        // TURBO
        {.fref = 888000, .freqDelta =       0, .fuses = &fuses_6138978676, .quotients = &quotients_9126134217, .margins = &margins_247510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  952000, .floor =  712000, .freq =       0, .interplEnabled = 0} } },

        // TURBO_L1
        {.fref = 888000, .freqDelta =       0, .fuses = &fuses_5975672073, .quotients = &quotients_9618572727, .margins = &margins_492510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  952000, .floor =  756000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_fuse_data  fuses_383138748 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW5_LSB, CPR6_TARG_VOLT_SVS3) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_1129114804 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW5_LSB, CPR6_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_57588456 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW5_LSB, CPR6_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_3326791033 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW5_LSB, CPR6_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


/*
 * LPI_CX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_11921134198 =
{
    .modesCount      = 5,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 520000, .freqDelta =       0, .fuses = &fuses_383138748, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  608000, .floor =  480000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 576000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  504000, .freq =       0, .interplEnabled = 50} } },

        // SVS
        {.fref = 624000, .freqDelta =       0, .fuses = &fuses_1129114804, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  736000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_57588456, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  648000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_3326791033, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  880000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_390510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =       0, .closedLoop =   70000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_8370701868 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  4, .quotient =  795 },
                                      { .ro =  6, .quotient =  785 },
                                      { .ro =  7, .quotient =  634 },
                                      { .ro =  9, .quotient =  900 } },
};


static cpr_fuse_data  fuses_3447771725 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_MSB, CPR4_TARG_VOLT_SVSP) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_MSB, CPR4_TARG_VOLT_OFFSET_SVSP) } } },
};


static cpr_margin_cfg margins_335510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =       0, .closedLoop =   59000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_8007194413 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  4, .quotient =  795 },
                                      { .ro =  6, .quotient =  785 },
                                      { .ro =  9, .quotient =  900 },
                                      { .ro = 10, .quotient =  777 } },
};


static cpr_fuse_data  fuses_5906379025 =
{
    .volt       = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_NOM_4_3), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR4_TARG_VOLT_NOM_2_0) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_380510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =       0, .closedLoop =   68000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_7956523274 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  4, .quotient =  931 },
                                      { .ro =  6, .quotient =  940 },
                                      { .ro =  9, .quotient = 1003 },
                                      { .ro = 10, .quotient =  910 } },
};


static cpr_fuse_data  fuses_4409588226 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR4_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_OFFSET_TUR) } } },
};


static cpr_margin_cfg margins_332510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =       0, .closedLoop =   56000, .maxFloorToCeil =  52000 } }
};


static cpr_margin_cfg margins_337510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =       0, .closedLoop =   57000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_7189827295 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  4, .quotient = 1012 },
                                      { .ro =  6, .quotient = 1009 },
                                      { .ro =  9, .quotient = 1058 },
                                      { .ro = 10, .quotient =  983 } },
};


static cpr_fuse_data  fuses_4678981118 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR4_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_MSB, CPR4_TARG_VOLT_OFFSET_SUT) } } },
};


static cpr_kv_cfg  kvs_27604961577 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =    0 },
                                { .ro =  1, .kv =    0 },
                                { .ro =  2, .kv =    0 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =  334 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =  300 },
                                { .ro =  7, .kv =  284 },
                                { .ro =  8, .kv =    0 },
                                { .ro =  9, .kv =  300 },
                                { .ro = 10, .kv =  311 },
                                { .ro = 11, .kv =    0 },
                                { .ro = 12, .kv =    0 },
                                { .ro = 13, .kv =    0 },
                                { .ro = 14, .kv =    0 },
                                { .ro = 15, .kv =    0 } },
};


/*
 * MX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_15597056725 =
{
    .modesCount      = 6,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_27604961577,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_NOMINAL_L1,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1 },
    .modes           = (struct cpr_voltage_data[]) {
        // SVS
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  752000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_3447771725, .quotients = &quotients_8370701868, .margins = &margins_390510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_5906379025, .quotients = &quotients_8007194413, .margins = &margins_335510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL_L1
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_4409588226, .quotients = &quotients_7956523274, .margins = &margins_380510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_4409588226, .quotients = &quotients_7956523274, .margins = &margins_332510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // TURBO_L1
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_4678981118, .quotients = &quotients_7189827295, .margins = &margins_337510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_fuse_data  fuses_437998931 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR10_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_1166708690 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR10_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_2392756860 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR10_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_fuse_data  fuses_3950674564 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR10_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


/*
 * MM Voltage Plan
 */
static cpr_voltage_plan voltage_plan_12800424681 =
{
    .modesCount      = 5,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_32421585693,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_437998931, .quotients = &quotients_7565264404, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  500000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_1166708690, .quotients = &quotients_11389963752, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_19206360606, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_2392756860, .quotients = &quotients_15229568806, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_3950674564, .quotients = &quotients_10660182061, .margins = &margins_852510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_304510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   33000, .closedLoop =   33000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_10754045283 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient =  394 },
                                      { .ro =  6, .quotient =  256 },
                                      { .ro =  8, .quotient =  408 },
                                      { .ro = 10, .quotient =  194 } },
};


static cpr_fuse_data  fuses_4279573963 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_SVS3) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW11_MSB, CPR2_TARG_VOLT_OFFSET_SVS3) } } },
};


static cpr_margin_cfg margins_194510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   13000, .closedLoop =   23000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_9652845256 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient =  501 },
                                      { .ro =  6, .quotient =  342 },
                                      { .ro =  8, .quotient =  511 },
                                      { .ro = 10, .quotient =  278 } },
};


static cpr_fuse_data  fuses_4094405953 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_SVS2) } } },
};


static cpr_margin_cfg margins_344510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   13000, .closedLoop =   53000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_10796062666 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient =  669 },
                                      { .ro =  6, .quotient =  472 },
                                      { .ro =  8, .quotient =  674 },
                                      { .ro =  9, .quotient =  562 } },
};


static cpr_fuse_data  fuses_4140801727 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_704510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   33000, .closedLoop =  113000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_9736280979 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  1, .quotient =  805 },
                                      { .ro =  6, .quotient =  590 },
                                      { .ro =  8, .quotient =  817 },
                                      { .ro =  9, .quotient =  697 } },
};


static cpr_fuse_data  fuses_77020093 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_788510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   61000, .closedLoop =  113000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_9729398260 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  785 },
                                      { .ro =  8, .quotient = 1033 },
                                      { .ro =  9, .quotient =  900 },
                                      { .ro = 11, .quotient = 1885 } },
};


static cpr_fuse_data  fuses_9796669542 =
{
    .volt       = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_NOM_4_3), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR2_TARG_VOLT_NOM_2_0) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_776510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   57000, .closedLoop =  113000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_7865043285 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  940 },
                                      { .ro =  8, .quotient = 1155 },
                                      { .ro =  9, .quotient = 1003 },
                                      { .ro = 11, .quotient = 2071 } },
};


static cpr_fuse_data  fuses_3518836740 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_OFFSET_TUR) } } },
};


static cpr_margin_cfg margins_764510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   49000, .closedLoop =  113000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_8169569444 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1280 },
                                      { .ro =  2, .quotient = 1106 },
                                      { .ro =  8, .quotient = 1221 },
                                      { .ro =  9, .quotient = 1058 },
                                      { .ro = 11, .quotient = 2168 } },
};


static cpr_fuse_data  fuses_2715853845 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_TUR) } } },
};


static cpr_margin_cfg margins_444510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    9000, .closedLoop =   73000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_9653353308 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1419 },
                                      { .ro =  2, .quotient = 1235 },
                                      { .ro =  8, .quotient = 1337 },
                                      { .ro =  9, .quotient = 1172 },
                                      { .ro = 11, .quotient = 2344 } },
};


static cpr_fuse_data  fuses_5009458971 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR2_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR2_TARG_VOLT_OFFSET_SUT) } } },
};


static cpr_margin_cfg margins_455510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   21000, .closedLoop =   68000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_15273326730 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1670 },
                                      { .ro =  2, .quotient = 1470 },
                                      { .ro =  8, .quotient = 1565 },
                                      { .ro =  9, .quotient = 1384 },
                                      { .ro = 11, .quotient = 2707 } },
};


static cpr_fuse_data  fuses_6542394585 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR2_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_MSB, CPR4_TARG_VOLT_OFFSET_SVSP) } } },
};


static cpr_kv_cfg  kvs_34876136770 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =  356 },
                                { .ro =  1, .kv =  363 },
                                { .ro =  2, .kv =  334 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =    0 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =  300 },
                                { .ro =  7, .kv =    0 },
                                { .ro =  8, .kv =  324 },
                                { .ro =  9, .kv =  300 },
                                { .ro = 10, .kv =  311 },
                                { .ro = 11, .kv =  518 },
                                { .ro = 12, .kv =    0 },
                                { .ro = 13, .kv =    0 },
                                { .ro = 14, .kv =    0 },
                                { .ro = 15, .kv =    0 } },
};


/*
 * GFX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_20940593294 =
{
    .modesCount      = 9,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_34876136770,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_NOMINAL_L1,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1,
                                                    CPR_VOLTAGE_MODE_TURBO_L2 },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 532000, .freqDelta =       0, .fuses = &fuses_4279573963, .quotients = &quotients_10754045283, .margins = &margins_304510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  612000, .floor =  500000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_4094405953, .quotients = &quotients_9652845256, .margins = &margins_194510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  500000, .freq =       0, .interplEnabled = 41} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_4140801727, .quotients = &quotients_10796062666, .margins = &margins_344510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_77020093, .quotients = &quotients_9736280979, .margins = &margins_704510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_9796669542, .quotients = &quotients_9729398260, .margins = &margins_788510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  652000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL_L1
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_3518836740, .quotients = &quotients_7865043285, .margins = &margins_776510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  912000, .floor =  688000, .freq =       0, .interplEnabled = 36} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_2715853845, .quotients = &quotients_8169569444, .margins = &margins_764510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  932000, .floor =  712000, .freq =       0, .interplEnabled = 60} } },

        // TURBO_L1
        {.fref = 888000, .freqDelta =       0, .fuses = &fuses_5009458971, .quotients = &quotients_9653353308, .margins = &margins_444510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  960000, .floor =  756000, .freq =       0, .interplEnabled = 0} } },

        // TURBO_L2
        {.fref = 960000, .freqDelta =       0, .fuses = &fuses_6542394585, .quotients = &quotients_15273326730, .margins = &margins_455510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling = 1024000, .floor =  932000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_48510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   16000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


/*
 * LPI_CX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_9023662394 =
{
    .modesCount      = 5,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 520000, .freqDelta =       0, .fuses = &fuses_383138748, .quotients = NULL, .margins = &margins_48510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  608000, .floor =  480000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 576000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  504000, .freq =       0, .interplEnabled = 50} } },

        // SVS
        {.fref = 624000, .freqDelta =       0, .fuses = &fuses_1129114804, .quotients = NULL, .margins = &margins_510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  736000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_57588456, .quotients = NULL, .margins = &margins_510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  648000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_3326791033, .quotients = NULL, .margins = &margins_510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  880000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


/*
 * LPI_MX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_9754561764 =
{
    .modesCount      = 3,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // SVS
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  752000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_2084319257, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_29245628, .quotients = NULL, .margins = &margins_300510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  848000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_quotient_cfg  quotients_15261536402 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient =  348 },
                                      { .ro =  1, .quotient =  350 },
                                      { .ro =  2, .quotient =  271 },
                                      { .ro =  9, .quotient =  286 },
                                      { .ro = 12, .quotient =  608 },
                                      { .ro = 14, .quotient =  211 } },
};


static cpr_fuse_data  fuses_2757147037 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_SVS3) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_quotient_cfg  quotients_11521099858 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient =  499 },
                                      { .ro =  1, .quotient =  502 },
                                      { .ro =  2, .quotient =  407 },
                                      { .ro =  9, .quotient =  422 },
                                      { .ro = 12, .quotient =  846 },
                                      { .ro = 14, .quotient =  307 } },
};


static cpr_quotient_cfg  quotients_17444907194 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient =  666 },
                                      { .ro =  1, .quotient =  670 },
                                      { .ro =  2, .quotient =  558 },
                                      { .ro =  9, .quotient =  563 },
                                      { .ro = 12, .quotient = 1101 },
                                      { .ro = 14, .quotient =  409 } },
};


static cpr_fuse_data  fuses_69276407 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_quotient_cfg  quotients_14787992845 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient =  817 },
                                      { .ro =  2, .quotient =  701 },
                                      { .ro =  8, .quotient =  715 },
                                      { .ro =  9, .quotient =  699 },
                                      { .ro = 15, .quotient =  394 } },
};


static cpr_quotient_cfg  quotients_13602230424 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1061 },
                                      { .ro =  2, .quotient =  920 },
                                      { .ro =  8, .quotient =  927 },
                                      { .ro =  9, .quotient =  903 },
                                      { .ro = 10, .quotient =  779 },
                                      { .ro = 15, .quotient =  544 } },
};


static cpr_fuse_data  fuses_2433322795 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_quotient_cfg  quotients_15144058687 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1201 },
                                      { .ro =  2, .quotient = 1038 },
                                      { .ro =  8, .quotient = 1032 },
                                      { .ro =  9, .quotient = 1007 },
                                      { .ro = 10, .quotient =  913 },
                                      { .ro = 15, .quotient =  637 } },
};


static cpr_quotient_cfg  quotients_11179200814 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1278 },
                                      { .ro =  2, .quotient = 1106 },
                                      { .ro =  8, .quotient = 1078 },
                                      { .ro =  9, .quotient = 1063 },
                                      { .ro = 10, .quotient =  986 },
                                      { .ro = 15, .quotient =  696 } },
};


static cpr_quotient_cfg  quotients_14075229612 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1417 },
                                      { .ro =  2, .quotient = 1234 },
                                      { .ro =  8, .quotient = 1159 },
                                      { .ro =  9, .quotient = 1177 },
                                      { .ro = 10, .quotient = 1103 },
                                      { .ro = 15, .quotient =  782 } },
};


static cpr_fuse_data  fuses_3792391293 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR1_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_kv_cfg  kvs_40585202637 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =  332 },
                                { .ro =  1, .kv =  338 },
                                { .ro =  2, .kv =  312 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =    0 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =    0 },
                                { .ro =  7, .kv =    0 },
                                { .ro =  8, .kv =  224 },
                                { .ro =  9, .kv =  276 },
                                { .ro = 10, .kv =  296 },
                                { .ro = 11, .kv =    0 },
                                { .ro = 12, .kv =  417 },
                                { .ro = 13, .kv =    0 },
                                { .ro = 14, .kv =  187 },
                                { .ro = 15, .kv =  201 } },
};


/*
 * MSS Voltage Plan
 */
static cpr_voltage_plan voltage_plan_14200141672 =
{
    .modesCount      = 8,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_40585202637,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_NOMINAL_L1,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1 },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 516000, .freqDelta =       0, .fuses = &fuses_2757147037, .quotients = &quotients_15261536402, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  612000, .floor =  484000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_11521099858, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  500000, .freq =       0, .interplEnabled = 50} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_69276407, .quotients = &quotients_17444907194, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_14787992845, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_2433322795, .quotients = &quotients_13602230424, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL_L1
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_15144058687, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  688000, .freq =       0, .interplEnabled = 36} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_11179200814, .margins = &margins_852510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  712000, .freq =       0, .interplEnabled = 60} } },

        // TURBO_L1
        {.fref = 888000, .freqDelta =       0, .fuses = &fuses_3792391293, .quotients = &quotients_14075229612, .margins = &margins_852510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  952000, .floor =  756000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_quotient_cfg  quotients_7618787316 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient =  865 },
                                      { .ro =  1, .quotient =  853 },
                                      { .ro =  6, .quotient =  640 },
                                      { .ro = 10, .quotient =  597 },
                                      { .ro = 14, .quotient =  529 },
                                      { .ro = 15, .quotient =  425 } },
};


static cpr_fuse_data  fuses_142860236 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_MSB, CPR4_TARG_VOLT_SVSP) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_quotient_cfg  quotients_16711091110 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1061 },
                                      { .ro =  1, .quotient = 1065 },
                                      { .ro =  6, .quotient =  806 },
                                      { .ro = 10, .quotient =  784 },
                                      { .ro = 14, .quotient =  642 },
                                      { .ro = 15, .quotient =  544 } },
};


static cpr_fuse_data  fuses_5829358932 =
{
    .volt       = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_NOM_4_3), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR4_TARG_VOLT_NOM_2_0) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_quotient_cfg  quotients_17705528488 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1201 },
                                      { .ro =  1, .quotient = 1219 },
                                      { .ro =  6, .quotient =  943 },
                                      { .ro = 10, .quotient =  913 },
                                      { .ro = 14, .quotient =  726 },
                                      { .ro = 15, .quotient =  637 } },
};


static cpr_fuse_data  fuses_890751486 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR4_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_quotient_cfg  quotients_13034916337 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1278 },
                                      { .ro =  1, .quotient = 1299 },
                                      { .ro =  6, .quotient = 1018 },
                                      { .ro = 10, .quotient =  986 },
                                      { .ro = 14, .quotient =  770 },
                                      { .ro = 15, .quotient =  696 } },
};


static cpr_fuse_data  fuses_3591189554 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR4_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_kv_cfg  kvs_34492242304 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =  332 },
                                { .ro =  1, .kv =  338 },
                                { .ro =  2, .kv =    0 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =    0 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =  285 },
                                { .ro =  7, .kv =    0 },
                                { .ro =  8, .kv =    0 },
                                { .ro =  9, .kv =    0 },
                                { .ro = 10, .kv =  296 },
                                { .ro = 11, .kv =    0 },
                                { .ro = 12, .kv =    0 },
                                { .ro = 13, .kv =    0 },
                                { .ro = 14, .kv =  187 },
                                { .ro = 15, .kv =  201 } },
};


/*
 * MX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_9634031529 =
{
    .modesCount      = 6,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_34492242304,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_NOMINAL_L1,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1 },
    .modes           = (struct cpr_voltage_data[]) {
        // SVS
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  752000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_142860236, .quotients = &quotients_7618787316, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_5829358932, .quotients = &quotients_16711091110, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL_L1
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_890751486, .quotients = &quotients_17705528488, .margins = &margins_840510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_890751486, .quotients = &quotients_17705528488, .margins = &margins_852510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // TURBO_L1
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_3591189554, .quotients = &quotients_13034916337, .margins = &margins_852510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_fuse_data  fuses_2181766068 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW11_MSB, CPR9_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


/*
 * WLAN Voltage Plan
 */
static cpr_voltage_plan voltage_plan_3367037990 =
{
    .modesCount      = 1,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_NOMINAL },
    .modes           = (struct cpr_voltage_data[]) {
        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_2181766068, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  752000, .floor =  752000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_345510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   30000, .closedLoop =   43000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_10117817724 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  335 },
                                      { .ro =  4, .quotient =  212 },
                                      { .ro =  6, .quotient =  279 },
                                      { .ro =  9, .quotient =  351 },
                                      { .ro = 13, .quotient =  469 } },
};


static cpr_fuse_data  fuses_1572900299 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_SVS3) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_315510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   30000, .closedLoop =   37000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_7564449290 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  463 },
                                      { .ro =  4, .quotient =  326 },
                                      { .ro =  6, .quotient =  393 },
                                      { .ro =  9, .quotient =  478 },
                                      { .ro = 13, .quotient =  675 } },
};


static cpr_margin_cfg margins_470510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   30000, .closedLoop =   68000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_15174580794 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  558 },
                                      { .ro =  4, .quotient =  420 },
                                      { .ro =  6, .quotient =  473 },
                                      { .ro =  9, .quotient =  563 },
                                      { .ro = 15, .quotient =  308 } },
};


static cpr_fuse_data  fuses_2709896491 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_505510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   40000, .closedLoop =   69000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_14336561345 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  701 },
                                      { .ro =  4, .quotient =  554 },
                                      { .ro =  6, .quotient =  592 },
                                      { .ro =  9, .quotient =  699 },
                                      { .ro = 15, .quotient =  394 } },
};


static cpr_margin_cfg margins_540510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   40000, .closedLoop =   76000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_17955365815 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1061 },
                                      { .ro =  1, .quotient = 1059 },
                                      { .ro =  2, .quotient =  920 },
                                      { .ro =  4, .quotient =  796 },
                                      { .ro =  9, .quotient =  903 },
                                      { .ro = 14, .quotient =  633 } },
};


static cpr_fuse_data  fuses_5540345690 =
{
    .volt       = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_NOM_4_3), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR2_TARG_VOLT_NOM_2_0) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_590510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   50000, .closedLoop =   80000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_11565937595 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1201 },
                                      { .ro =  1, .quotient = 1219 },
                                      { .ro =  2, .quotient = 1038 },
                                      { .ro =  4, .quotient =  932 },
                                      { .ro =  9, .quotient = 1007 },
                                      { .ro = 14, .quotient =  726 } },
};


static cpr_margin_cfg margins_642510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   50000, .closedLoop =   88000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_12244792112 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1278 },
                                      { .ro =  1, .quotient = 1299 },
                                      { .ro =  2, .quotient = 1106 },
                                      { .ro =  4, .quotient = 1013 },
                                      { .ro =  9, .quotient = 1063 },
                                      { .ro = 14, .quotient =  764 } },
};


static cpr_margin_cfg margins_552510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   25000, .closedLoop =   85000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_10788015488 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  0, .quotient = 1417 },
                                      { .ro =  1, .quotient = 1434 },
                                      { .ro =  2, .quotient = 1234 },
                                      { .ro =  4, .quotient = 1145 },
                                      { .ro =  9, .quotient = 1177 },
                                      { .ro = 14, .quotient =  837 } },
};


static cpr_fuse_data  fuses_3237483096 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR2_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_kv_cfg  kvs_34542833919 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =  332 },
                                { .ro =  1, .kv =  338 },
                                { .ro =  2, .kv =  312 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =  321 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =  285 },
                                { .ro =  7, .kv =    0 },
                                { .ro =  8, .kv =    0 },
                                { .ro =  9, .kv =  276 },
                                { .ro = 10, .kv =    0 },
                                { .ro = 11, .kv =    0 },
                                { .ro = 12, .kv =    0 },
                                { .ro = 13, .kv =  454 },
                                { .ro = 14, .kv =  187 },
                                { .ro = 15, .kv =  201 } },
};


/*
 * GFX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_12902284799 =
{
    .modesCount      = 8,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_34542833919,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_NOMINAL_L1,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1 },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 516000, .freqDelta =       0, .fuses = &fuses_1572900299, .quotients = &quotients_10117817724, .margins = &margins_345510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  612000, .floor =  484000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_7564449290, .margins = &margins_315510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  500000, .freq =       0, .interplEnabled = 50} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_2709896491, .quotients = &quotients_15174580794, .margins = &margins_470510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_14336561345, .margins = &margins_505510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_5540345690, .quotients = &quotients_17955365815, .margins = &margins_540510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL_L1
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_11565937595, .margins = &margins_590510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  688000, .freq =       0, .interplEnabled = 36} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_0, .quotients = &quotients_12244792112, .margins = &margins_642510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  712000, .freq =       0, .interplEnabled = 60} } },

        // TURBO_L1
        {.fref = 888000, .freqDelta =       0, .fuses = &fuses_3237483096, .quotients = &quotients_10788015488, .margins = &margins_552510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  952000, .floor =  756000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_36510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   12000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


static cpr_margin_cfg margins_144510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   48000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


static cpr_margin_cfg margins_12510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =    4000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


static cpr_margin_cfg margins_108510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   36000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


/*
 * VDDA Voltage Plan
 */
static cpr_voltage_plan voltage_plan_17238768373 =
{
    .modesCount      = 6,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_3952340992, .quotients = NULL, .margins = &margins_48510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  500000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_36510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 50} } },

        // SVS
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_655279266, .quotients = NULL, .margins = &margins_144510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_144510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 57} } },

        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_2626994354, .quotients = NULL, .margins = &margins_12510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  852000, .floor =  688000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_1606482579, .quotients = NULL, .margins = &margins_108510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_276510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   12000, .closedLoop =   40000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_7891102628 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  4, .quotient =  276 },
                                      { .ro =  5, .quotient =  254 },
                                      { .ro =  6, .quotient =  342 },
                                      { .ro =  7, .quotient =  204 },
                                      { .ro = 10, .quotient =  278 } },
};


static cpr_fuse_data  fuses_2187884975 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_SVS3) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR0_TARG_VOLT_OFFSET_SVS2) } } },
};


static cpr_margin_cfg margins_216510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =   -3000, .closedLoop =   37000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_12208158500 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  4, .quotient =  420 },
                                      { .ro =  5, .quotient =  403 },
                                      { .ro =  6, .quotient =  472 },
                                      { .ro =  7, .quotient =  327 },
                                      { .ro = 10, .quotient =  422 } },
};


static cpr_fuse_data  fuses_3586501535 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_245510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  -15000, .closedLoop =   50000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_10597728036 =
{
    .count = 4,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  590 },
                                      { .ro =  7, .quotient =  430 },
                                      { .ro =  8, .quotient =  817 },
                                      { .ro = 10, .quotient =  551 } },
};


static cpr_fuse_data  fuses_2001864162 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_185510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  -15000, .closedLoop =   38000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_6484663883 =
{
    .count = 3,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  785 },
                                      { .ro =  8, .quotient = 1033 },
                                      { .ro =  9, .quotient =  900 } },
};


static cpr_fuse_data  fuses_5047327515 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_222510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  -15000, .closedLoop =   43000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_4672367189 =
{
    .count = 3,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient = 1009 },
                                      { .ro =  8, .quotient = 1221 },
                                      { .ro =  9, .quotient = 1058 } },
};


static cpr_fuse_data  fuses_3504360139 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_OFFSET_TUR) } } },
};


static cpr_margin_cfg margins_257510 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 255, .openLoop =  -15000, .closedLoop =   50000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_7076653003 =
{
    .count = 3,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient = 1127 },
                                      { .ro =  8, .quotient = 1337 },
                                      { .ro =  9, .quotient = 1172 } },
};


static cpr_fuse_data  fuses_4353647651 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR0_TARG_VOLT_SUT) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW17_LSB, CPR0_TARG_VOLT_OFFSET_SUT) } } },
};


static cpr_kv_cfg  kvs_28320459600 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =    0 },
                                { .ro =  1, .kv =    0 },
                                { .ro =  2, .kv =    0 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =  334 },
                                { .ro =  5, .kv =  335 },
                                { .ro =  6, .kv =  300 },
                                { .ro =  7, .kv =  284 },
                                { .ro =  8, .kv =  324 },
                                { .ro =  9, .kv =  300 },
                                { .ro = 10, .kv =  311 },
                                { .ro = 11, .kv =    0 },
                                { .ro = 12, .kv =    0 },
                                { .ro = 13, .kv =    0 },
                                { .ro = 14, .kv =    0 },
                                { .ro = 15, .kv =    0 } },
};


/*
 * CX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_17710492843 =
{
    .modesCount      = 6,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_28320459600,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1 },
    .modes           = (struct cpr_voltage_data[]) {
        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_2187884975, .quotients = &quotients_7891102628, .margins = &margins_276510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  672000, .floor =  516000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_3586501535, .quotients = &quotients_12208158500, .margins = &margins_216510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  732000, .floor =  552000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_2001864162, .quotients = &quotients_10597728036, .margins = &margins_245510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  796000, .floor =  600000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_5047327515, .quotients = &quotients_6484663883, .margins = &margins_185510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  816000, .floor =  652000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 832000, .freqDelta =       0, .fuses = &fuses_3504360139, .quotients = &quotients_4672367189, .margins = &margins_222510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  884000, .floor =  712000, .freq =       0, .interplEnabled = 60} } },

        // TURBO_L1
        {.fref = 888000, .freqDelta =       0, .fuses = &fuses_4353647651, .quotients = &quotients_7076653003, .margins = &margins_257510,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  952000, .floor =  756000, .freq =       0, .interplEnabled = 0} } }},
};


/*
 * Aging configs
 */

static cpr_aging_cfg  aging_cfg_mss =
{
    .cprRevMinToRun       = 1,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO_L1,
    .sensorID             = 9,
    .kv                   = 86,
    .scalingFactor        = 10,
    .marginLimit          = 15000,
    .bypassSensorIDs      = NULL,
    .bypassSensorIDsCount = 0,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 1, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x784210 & (SECURITY_CONTROL_BASE_SIZE - 1)), 16, (1<<23) | (1<<22) | (1<<21) | (1<<20) | (1<<19) | (1<<18) | (1<<17) | (1<<16)} } }
};


static cpr_aging_cfg  aging_cfg_cx =
{
    .cprRevMinToRun       = 1,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO_L1,
    .sensorID             = 14,
    .kv                   = 86,
    .scalingFactor        = 10,
    .marginLimit          = 15000,
    .bypassSensorIDs      = NULL,
    .bypassSensorIDsCount = 0,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 1, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x78420C & (SECURITY_CONTROL_BASE_SIZE - 1)), 4, (1<<11) | (1<<10) | (1<<9) | (1<<8) | (1<<7) | (1<<6) | (1<<5) | (1<<4)} } }
};


static cpr_aging_cfg  aging_cfg_mm =
{
    .cprRevMinToRun       = 1,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO,
    .sensorID             = 5,
    .kv                   = 86,
    .scalingFactor        = 10,
    .marginLimit          = 15000,
    .bypassSensorIDs      = NULL,
    .bypassSensorIDsCount = 0,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 1, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x784250 & (SECURITY_CONTROL_BASE_SIZE - 1)), 19, (1<<26) | (1<<25) | (1<<24) | (1<<23) | (1<<22) | (1<<21) | (1<<20) | (1<<19)} } }
};


static cpr_aging_cfg  aging_cfg_gfx =
{
    .cprRevMinToRun       = 1,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO_L1,
    .sensorID             = 39,
    .kv                   = 86,
    .scalingFactor        = 25,
    .marginLimit          = 15000,
    .bypassSensorIDs      = NULL,
    .bypassSensorIDsCount = 0,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 2, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x784214 & (SECURITY_CONTROL_BASE_SIZE - 1)), 28, (1<<31) | (1<<30) | (1<<29) | (1<<28)},
                                 {SECURITY_CONTROL_BASE | (0x784218 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<3) | (1<<2) | (1<<1) | (1<<0)} } }
};


static cpr_aging_cfg  aging_cfg_mx =
{
    .cprRevMinToRun       = 1,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO,
    .sensorID             = 5,
    .kv                   = 86,
    .scalingFactor        = 10,
    .marginLimit          = 15000,
    .bypassSensorIDs      = (uint8[]) {1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37},
    .bypassSensorIDsCount = 36,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 1, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x784254 & (SECURITY_CONTROL_BASE_SIZE - 1)), 15, (1<<22) | (1<<21) | (1<<20) | (1<<19) | (1<<18) | (1<<17) | (1<<16) | (1<<15)} } }
};


/*
 * Versioned Voltage Plans
 */

static cpr_versioned_voltage_plan vdda_vvp = {
    .rail     = CPR_RAIL_VDDA,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_13764417942 },

        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_17238768373 },
     },
    .count    = 2,
    .agingCfg = NULL
};


static cpr_versioned_voltage_plan mss_vvp = {
    .rail     = CPR_RAIL_MSS,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_17380090077 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_14200141672 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_mss
};


static cpr_versioned_voltage_plan lpi_cx_vvp = {
    .rail     = CPR_RAIL_LPI_CX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_11921134198 },

        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_9023662394 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_cx
};


static cpr_versioned_voltage_plan mm_vvp = {
    .rail     = CPR_RAIL_MM,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_11526383673 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_12800424681 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_mm
};


static cpr_versioned_voltage_plan cx_vvp = {
    .rail     = CPR_RAIL_CX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_13729291978 },

        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_17710492843 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_cx
};


static cpr_versioned_voltage_plan gfx_vvp = {
    .rail     = CPR_RAIL_GFX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_20940593294 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_12902284799 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_gfx
};


static cpr_versioned_voltage_plan lpi_mx_vvp = {
    .rail     = CPR_RAIL_LPI_MX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_5492981749 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_9754561764 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_mx
};


static cpr_versioned_voltage_plan wmss_cx_vvp = {
    .rail     = CPR_RAIL_WMSS_CX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_5035517949 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_5035517949 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_cx
};


static cpr_versioned_voltage_plan wlan_vvp = {
    .rail     = CPR_RAIL_WLAN,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_3367037990 },

        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_3367037990 },
     },
    .count    = 2,
    .agingCfg = NULL
};


static cpr_versioned_voltage_plan mx_vvp = {
    .rail     = CPR_RAIL_MX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_15597056725 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_9634031529 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_mx
};

/*******************************************************************************
********************************************************************************
**
** Enablement Config Data
**
********************************************************************************
********************************************************************************/

/*
 * CPR Controller Thread Config
 */

static cpr_hal_thread_cfg mss_thread_cfg = {
    .upThresh     = 2,
    .dnThresh     = 2,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 },
    .sensorsCount = 17,
};


static cpr_hal_thread  mss_thread = {
    .id   = 0,
    .cfg  = &mss_thread_cfg,
};


static cpr_hal_thread_cfg cx_thread_cfg = {
    .upThresh     = 2,
    .dnThresh     = 2,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46 },
    .sensorsCount = 47,
};


static cpr_hal_thread  cx_thread = {
    .id   = 0,
    .cfg  = &cx_thread_cfg,
};


static cpr_hal_thread_cfg gfx_thread_cfg = {
    .upThresh     = 2,
    .dnThresh     = 2,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 },
    .sensorsCount = 19,
};


static cpr_hal_thread  gfx_thread = {
    .id   = 0,
    .cfg  = &gfx_thread_cfg,
};


static cpr_hal_thread_cfg mm_thread_cfg = {
    .upThresh     = 2,
    .dnThresh     = 2,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21 },
    .sensorsCount = 22,
};


static cpr_hal_thread  mm_thread = {
    .id   = 0,
    .cfg  = &mm_thread_cfg,
};


static cpr_hal_thread_cfg mx_thread_cfg = {
    .upThresh     = 2,
    .dnThresh     = 2,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 0,5,30,31,36,37 },
    .sensorsCount = 6,
};


static cpr_hal_thread  mx_thread = {
    .id   = 0,
    .cfg  = &mx_thread_cfg,
};


static cpr_hal_thread_cfg vdda_thread_cfg = {
    .upThresh     = 0,
    .dnThresh     = 0,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29 },
    .sensorsCount = 24,
};


static cpr_hal_thread  vdda_thread = {
    .id   = 1,
    .cfg  = &vdda_thread_cfg,
};


static cpr_hal_thread_cfg lpi_cx_thread_cfg = {
    .upThresh     = 0,
    .dnThresh     = 0,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 1,2 },
    .sensorsCount = 2,
};


static cpr_hal_thread  lpi_cx_thread = {
    .id   = 2,
    .cfg  = &lpi_cx_thread_cfg,
};


static cpr_hal_thread_cfg lpi_mx_thread_cfg = {
    .upThresh     = 0,
    .dnThresh     = 0,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 3,4 },
    .sensorsCount = 2,
};


static cpr_hal_thread  lpi_mx_thread = {
    .id   = 3,
    .cfg  = &lpi_mx_thread_cfg,
};


static cpr_hal_thread_cfg wlan_thread_cfg = {
    .upThresh     = 0,
    .dnThresh     = 0,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 32,33,34,35 },
    .sensorsCount = 4,
};


static cpr_hal_thread  wlan_thread = {
    .id   = 4,
    .cfg  = &wlan_thread_cfg,
};


/*
 * CPR Controller Config
 */

static cpr_hal_controller_cfg cpr3_controller_cfg = {
    .stepQuotMin         = 8,
    .stepQuotMax         = 20,
    .sensorsTotal        = 17,
    .bypassSensors       = NULL,
    .bypassSensorsCount  = 0,
    .disableSensors      = NULL,
    .disableSensorsCount = 0,
    .refClk              = NULL,
    .ahbClk              = NULL,
};


static cpr_hal_controller  cpr3_controller = {
    .base                = 0xedb80000,
    .type                = CPR_CONTROLLER_TYPE_HW_CL_ONLY,
    .cfg                 = &cpr3_controller_cfg,
    .threads             = (cpr_hal_thread*[]){ &mss_thread },
    .threadsCount        = 1,
};


static cpr_hal_controller_cfg cx_cpr3_controller_cfg = {
    .stepQuotMin         = 8,
    .stepQuotMax         = 12,
    .sensorsTotal        = 47,
    .bypassSensors       = NULL,
    .bypassSensorsCount  = 0,
    .disableSensors      = (uint8[]){ 1,2,3,4,8,9,15,22,23,25,26,41,42 },
    .disableSensorsCount = 13,
    .refClk              = NULL,
    .ahbClk              = NULL,
};


static cpr_hal_controller  cx_cpr3_controller = {
    .base                = 0x628000,
    .type                = CPR_CONTROLLER_TYPE_HW_CL_ONLY,
    .cfg                 = &cx_cpr3_controller_cfg,
    .threads             = (cpr_hal_thread*[]){ &cx_thread },
    .threadsCount        = 1,
};


static cpr_hal_controller_cfg gpu_cpr_controller_cfg = {
    .stepQuotMin         = 8,
    .stepQuotMax         = 19,
    .sensorsTotal        = 19,
    .bypassSensors       = NULL,
    .bypassSensorsCount  = 0,
    .disableSensors      = NULL,
    .disableSensorsCount = 0,
    .refClk              = NULL,
    .ahbClk              = NULL,
};


static cpr_hal_controller  gpu_cpr_controller = {
    .base                = 0x2c9a000,
    .type                = CPR_CONTROLLER_TYPE_HW_CL_ONLY,
    .cfg                 = &gpu_cpr_controller_cfg,
    .threads             = (cpr_hal_thread*[]){ &gfx_thread },
    .threadsCount        = 1,
};


static cpr_hal_controller_cfg mmcx_cpr3_controller_cfg = {
    .stepQuotMin         = 8,
    .stepQuotMax         = 13,
    .sensorsTotal        = 22,
    .bypassSensors       = NULL,
    .bypassSensorsCount  = 0,
    .disableSensors      = NULL,
    .disableSensorsCount = 0,
    .refClk              = NULL,
    .ahbClk              = NULL,
};


static cpr_hal_controller  mmcx_cpr3_controller = {
    .base                = 0x630000,
    .type                = CPR_CONTROLLER_TYPE_HW_CL_ONLY,
    .cfg                 = &mmcx_cpr3_controller_cfg,
    .threads             = (cpr_hal_thread*[]){ &mm_thread },
    .threadsCount        = 1,
};


static cpr_hal_controller_cfg mx_cpr3_controller_cfg = {
    .stepQuotMin         = 7,
    .stepQuotMax         = 13,
    .sensorsTotal        = 38,
    .bypassSensors       = (uint8[]){ 1,2,3,4,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,32,33,34,35 },
    .bypassSensorsCount  = 32,
    .disableSensors      = (uint8[]){ 1,2,3,4,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,32,33,34,35 },
    .disableSensorsCount = 32,
    .refClk              = NULL,
    .ahbClk              = NULL,
};


static cpr_hal_controller  mx_cpr3_controller = {
    .base                = 0x62c000,
    .type                = CPR_CONTROLLER_TYPE_HW_CL_ONLY,
    .cfg                 = &mx_cpr3_controller_cfg,
    .threads             = (cpr_hal_thread*[]){ &mx_thread, &vdda_thread, &lpi_cx_thread, &lpi_mx_thread, &wlan_thread },
    .threadsCount        = 5,
};


/*
 * HAL Rail Config
 */

static cpr_rail cpr_rail_cx_cfg = {
    .id               = CPR_RAIL_CX,
    .name             = "CX",
    .hal              = { .controller = &cx_cpr3_controller, .thread = &cx_thread },
    .interruptId      = 0,
    .settleModes      = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_NOMINAL, CPR_VOLTAGE_MODE_TURBO },
    .settleModesCount = 2,
};


static cpr_rail cpr_rail_gfx_cfg = {
    .id               = CPR_RAIL_GFX,
    .name             = "GFX",
    .hal              = { .controller = &gpu_cpr_controller, .thread = &gfx_thread },
    .interruptId      = 0,
    .settleModes      = NULL,
    .settleModesCount = 0,
};


static cpr_rail cpr_rail_lpi_cx_cfg = {
    .id               = CPR_RAIL_LPI_CX,
    .name             = "LPI_CX",
    .hal              = { .controller = &mx_cpr3_controller, .thread = &lpi_cx_thread },
    .interruptId      = 0,
    .settleModes      = NULL,
    .settleModesCount = 0,
};


static cpr_rail cpr_rail_lpi_mx_cfg = {
    .id               = CPR_RAIL_LPI_MX,
    .name             = "LPI_MX",
    .hal              = { .controller = &mx_cpr3_controller, .thread = &lpi_mx_thread },
    .interruptId      = 0,
    .settleModes      = NULL,
    .settleModesCount = 0,
};


static cpr_rail cpr_rail_mm_cfg = {
    .id               = CPR_RAIL_MM,
    .name             = "MM",
    .hal              = { .controller = &mmcx_cpr3_controller, .thread = &mm_thread },
    .interruptId      = 0,
    .settleModes      = NULL,
    .settleModesCount = 0,
};


static cpr_rail cpr_rail_mss_cfg = {
    .id               = CPR_RAIL_MSS,
    .name             = "MSS",
    .hal              = { .controller = &cpr3_controller, .thread = &mss_thread },
    .interruptId      = 0,
    .settleModes      = NULL,
    .settleModesCount = 0,
};


static cpr_rail cpr_rail_mx_cfg = {
    .id               = CPR_RAIL_MX,
    .name             = "MX",
    .hal              = { .controller = &mx_cpr3_controller, .thread = &mx_thread },
    .interruptId      = 0,
    .settleModes      = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_NOMINAL, CPR_VOLTAGE_MODE_TURBO },
    .settleModesCount = 2,
};


static cpr_rail cpr_rail_vdda_cfg = {
    .id               = CPR_RAIL_VDDA,
    .name             = "VDDA",
    .hal              = { .controller = &mx_cpr3_controller, .thread = &vdda_thread },
    .interruptId      = 0,
    .settleModes      = NULL,
    .settleModesCount = 0,
};


static cpr_rail cpr_rail_wlan_cfg = {
    .id               = CPR_RAIL_WLAN,
    .name             = "WLAN",
    .hal              = { .controller = &mx_cpr3_controller, .thread = &wlan_thread },
    .interruptId      = 0,
    .settleModes      = NULL,
    .settleModesCount = 0,
};


static cpr_rail cpr_rail_wmss_cx_cfg = {
    .id               = CPR_RAIL_WMSS_CX,
    .name             = "WMSS_CX",
    .hal              = { .controller = NULL, .thread = NULL },
    .interruptId      = 0,
    .settleModes      = NULL,
    .settleModesCount = 0,
};


/*
 * Rail Enablement Config
 */

static cpr_enablement cpr_rail_cx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_CX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_SW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_cx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_CX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_SW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_gfx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_GFX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 0,
};


static cpr_enablement cpr_rail_gfx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_GFX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 0,
};


static cpr_enablement cpr_rail_lpi_cx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_LPI_CX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_lpi_cx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_LPI_CX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_lpi_mx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_LPI_MX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_lpi_mx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_LPI_MX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_mm_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_MM,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 0,
};


static cpr_enablement cpr_rail_mm_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_MM,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 0,
};


static cpr_enablement cpr_rail_mss_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_MSS,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 0,
};


static cpr_enablement cpr_rail_mss_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_MSS,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 0,
};


static cpr_enablement cpr_rail_mx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_MX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_SW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_mx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_MX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_SW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_vdda_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_VDDA,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_vdda_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_VDDA,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_wlan_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_WLAN,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_wlan_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_WLAN,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 1,
};


static cpr_enablement cpr_rail_wmss_cx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_WMSS_CX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 0,
};


static cpr_enablement cpr_rail_wmss_cx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_WMSS_CX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
    .enable            = 0,
};


static cpr_misc_cfg misc_cfg = {
    .cprRev = {.count = 1, .data = (struct raw_fuse_data[]) { {SECURITY_CONTROL_BASE | (0x00784258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } }
};


cpr_settings cpr_settings_1000 =
{
    .hashValue        = voltage_plan_hash_value,

    .railVVPs         = (cpr_versioned_voltage_plan*[]) { &cx_vvp, &gfx_vvp, &lpi_cx_vvp, &lpi_mx_vvp, &mm_vvp, &mss_vvp, &mx_vvp, &vdda_vvp, &wlan_vvp, &wmss_cx_vvp },
    .rails            = (cpr_rail*[]) { &cpr_rail_cx_cfg, &cpr_rail_gfx_cfg, &cpr_rail_lpi_cx_cfg, &cpr_rail_lpi_mx_cfg, &cpr_rail_mm_cfg, &cpr_rail_mss_cfg, &cpr_rail_mx_cfg, &cpr_rail_vdda_cfg, &cpr_rail_wlan_cfg, &cpr_rail_wmss_cx_cfg },
    .railsCount       = 10,

    .controllers      = (cpr_hal_controller*[]) { &cpr3_controller, &cx_cpr3_controller, &gpu_cpr_controller, &mmcx_cpr3_controller, &mx_cpr3_controller },
    .controllersCount = 5,

    .enablements      = (cpr_enablement*[]) { &cpr_rail_cx_enablement_1_0_1_255, &cpr_rail_cx_enablement_2_0_2_255, &cpr_rail_gfx_enablement_1_0_1_255, &cpr_rail_gfx_enablement_2_0_2_255, &cpr_rail_lpi_cx_enablement_1_0_1_255, &cpr_rail_lpi_cx_enablement_2_0_2_255, &cpr_rail_lpi_mx_enablement_1_0_1_255, &cpr_rail_lpi_mx_enablement_2_0_2_255, &cpr_rail_mm_enablement_1_0_1_255, &cpr_rail_mm_enablement_2_0_2_255, &cpr_rail_mss_enablement_1_0_1_255, &cpr_rail_mss_enablement_2_0_2_255, &cpr_rail_mx_enablement_1_0_1_255, &cpr_rail_mx_enablement_2_0_2_255, &cpr_rail_vdda_enablement_1_0_1_255, &cpr_rail_vdda_enablement_2_0_2_255, &cpr_rail_wlan_enablement_1_0_1_255, &cpr_rail_wlan_enablement_2_0_2_255, &cpr_rail_wmss_cx_enablement_1_0_1_255, &cpr_rail_wmss_cx_enablement_2_0_2_255 },
    .enablementsCount = 20,

    .miscCfg          = &misc_cfg,
};

