/*******************************************************************************
 *
 * Copyright (c) 2020 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * QUALCOMM Proprietary and Confidential.
 *
 * THIS IS AUTO-GENERATED CPR CONFIG FILE FOR 845.
 *
 * DateTime: 2020/04/13 03:58:05
 *
 *******************************************************************************/

#include "cpr_data.h"
#include "cpr_image_target_init.h"
#include "cpr_target_hwio.h"


//hash value of Voltage Plan file (extracted all cell values)
static char voltage_plan_hash_value[] = "E61A8D68E46B8A75AEC761A54B02A84E";


/*******************************************************************************
********************************************************************************
**
** Voltage Plan Data
**
********************************************************************************
********************************************************************************/

static cpr_margin_cfg margins_14 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =       0, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


static cpr_fuse_data  fuses_3951258467 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR3_TARG_VOLT_SVS2) } } },
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
static cpr_voltage_plan voltage_plan_8405015862 =
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
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_3951258467, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_3951258467, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  740000, .floor =  532000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_655279266, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  752000, .floor =  672000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_655279266, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  672000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_2626994354, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  928000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_1606482579, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_version  ver_ANY_1_0_1_255 = { .foundry = CPR_FOUNDRY_ANY, .min = CPR_CHIPINFO_VERSION(1,0), .max = CPR_CHIPINFO_VERSION(1,255) };


static cpr_fuse_data  fuses_0 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_40014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =       0, .closedLoop =       0, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_8749835594 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  724 },
                                      { .ro =  7, .quotient =  804 },
                                      { .ro =  9, .quotient = 1347 },
                                      { .ro = 10, .quotient = 1266 },
                                      { .ro = 12, .quotient = 1251 },
                                      { .ro = 13, .quotient = 1134 } },
};


static cpr_fuse_data  fuses_5829358932 =
{
    .volt       = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_NOM_4_3), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR4_TARG_VOLT_NOM_2_0) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_272014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =       0, .closedLoop =   44000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_8445637474 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  6, .quotient =  867 },
                                      { .ro =  7, .quotient =  937 },
                                      { .ro =  9, .quotient = 1477 },
                                      { .ro = 10, .quotient = 1419 },
                                      { .ro = 12, .quotient = 1358 },
                                      { .ro = 13, .quotient = 1197 } },
};


static cpr_fuse_data  fuses_890751486 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR4_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_kv_cfg  kvs_31700313473 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =    0 },
                                { .ro =  1, .kv =    0 },
                                { .ro =  2, .kv =    0 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =    0 },
                                { .ro =  5, .kv =    0 },
                                { .ro =  6, .kv =  221 },
                                { .ro =  7, .kv =  252 },
                                { .ro =  8, .kv =    0 },
                                { .ro =  9, .kv =  243 },
                                { .ro = 10, .kv =  239 },
                                { .ro = 11, .kv =    0 },
                                { .ro = 12, .kv =  240 },
                                { .ro = 13, .kv =  205 },
                                { .ro = 14, .kv =    0 },
                                { .ro = 15, .kv =    0 } },
};


/*
 * MX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_7284521173 =
{
    .modesCount      = 3,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_31700313473,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // SVS_L1
        {.fref =      0, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_5829358932, .quotients = &quotients_8749835594, .margins = &margins_40014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  928000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_890751486, .quotients = &quotients_8445637474, .margins = &margins_272014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_95014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   20000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_10145960225 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  512 },
                                      { .ro =  3, .quotient =  572 },
                                      { .ro =  4, .quotient =  467 },
                                      { .ro =  5, .quotient =  500 },
                                      { .ro =  6, .quotient =  219 },
                                      { .ro =  7, .quotient =  253 } },
};


static cpr_fuse_data  fuses_1237584514 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_200014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   41000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_7883330866 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  681 },
                                      { .ro =  3, .quotient =  749 },
                                      { .ro =  4, .quotient =  628 },
                                      { .ro =  5, .quotient =  665 },
                                      { .ro =  6, .quotient =  320 },
                                      { .ro =  7, .quotient =  364 } },
};


static cpr_fuse_data  fuses_1584637373 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_135014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   28000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_12007322685 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  814 },
                                      { .ro =  3, .quotient =  888 },
                                      { .ro =  4, .quotient =  762 },
                                      { .ro =  5, .quotient =  805 },
                                      { .ro =  6, .quotient =  431 },
                                      { .ro =  7, .quotient =  490 } },
};


static cpr_margin_cfg margins_270014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   55000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_7902280641 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  9, .quotient = 1217 },
                                      { .ro = 10, .quotient = 1143 },
                                      { .ro = 11, .quotient =  735 },
                                      { .ro = 12, .quotient = 1138 },
                                      { .ro = 13, .quotient = 1055 } },
};


static cpr_fuse_data  fuses_2673371045 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_222014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   43000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_5444426829 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  9, .quotient = 1477 },
                                      { .ro = 10, .quotient = 1419 },
                                      { .ro = 11, .quotient =  999 },
                                      { .ro = 12, .quotient = 1358 },
                                      { .ro = 13, .quotient = 1197 } },
};


static cpr_fuse_data  fuses_1905602668 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_12014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =    1000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_15287849837 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  9, .quotient = 1665 },
                                      { .ro = 10, .quotient = 1563 },
                                      { .ro = 11, .quotient = 1203 },
                                      { .ro = 12, .quotient = 1512 },
                                      { .ro = 13, .quotient = 1305 } },
};


static cpr_temp_adj_cfg  temp_adj_cfg_cx_5_6_30_75_150 =
{
    .tempSensorStartId = 5,
    .tempSensorEndId   = 6,
    .tempPoints        = { 30, 75, 150 }, // degree C
};


static cpr_kv_cfg  kvs_31234238427 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =    0 },
                                { .ro =  1, .kv =    0 },
                                { .ro =  2, .kv =  248 },
                                { .ro =  3, .kv =  265 },
                                { .ro =  4, .kv =  237 },
                                { .ro =  5, .kv =  257 },
                                { .ro =  6, .kv =  221 },
                                { .ro =  7, .kv =  252 },
                                { .ro =  8, .kv =    0 },
                                { .ro =  9, .kv =  243 },
                                { .ro = 10, .kv =  239 },
                                { .ro = 11, .kv =  255 },
                                { .ro = 12, .kv =  240 },
                                { .ro = 13, .kv =  205 },
                                { .ro = 14, .kv =    0 },
                                { .ro = 15, .kv =    0 } },
};


/*
 * CX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_16908331946 =
{
    .modesCount      = 7,
    .tempAdjCfg      = &temp_adj_cfg_cx_5_6_30_75_150,
    .kvCfg           = &kvs_31234238427,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO,
                                                    CPR_VOLTAGE_MODE_TURBO_L1 },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 580000, .freqDelta =       0, .fuses = &fuses_1237584514, .quotients = &quotients_10145960225, .margins = &margins_95014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  532000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 580000, .freqDelta =       0, .fuses = &fuses_1237584514, .quotients = &quotients_10145960225, .margins = &margins_95014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  532000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_1584637373, .quotients = &quotients_7883330866, .margins = &margins_200014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  740000, .floor =  532000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_1584637373, .quotients = &quotients_12007322685, .margins = &margins_135014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_2673371045, .quotients = &quotients_7902280641, .margins = &margins_270014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  632000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_1905602668, .quotients = &quotients_5444426829, .margins = &margins_222014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  712000, .freq =       0, .interplEnabled = 0} } },

        // TURBO_L1
        {.fref = 924000, .freqDelta =       0, .fuses = &fuses_1905602668, .quotients = &quotients_15287849837, .margins = &margins_12014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling = 1068000, .floor =  756000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_130014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   27000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_2188967500 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR0_TARG_VOLT_OFFSET_SVS2) } } },
};


static cpr_margin_cfg margins_210014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   43000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_3586501535 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_125014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   26000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_2001864162 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_240014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   49000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_5047327515 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_287014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   56000, .maxFloorToCeil =  52000 } }
};


static cpr_fuse_data  fuses_5409962807 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_LSB, CPR0_TARG_VOLT_OFFSET_TUR) } } },
};


/*
 * CX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_11549500602 =
{
    .modesCount      = 6,
    .tempAdjCfg      = &temp_adj_cfg_cx_5_6_30_75_150,
    .kvCfg           = &kvs_31234238427,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_2188967500, .quotients = &quotients_10145960225, .margins = &margins_130014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_2188967500, .quotients = &quotients_10145960225, .margins = &margins_130014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_3586501535, .quotients = &quotients_7883330866, .margins = &margins_210014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  740000, .floor =  532000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_2001864162, .quotients = &quotients_12007322685, .margins = &margins_125014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_5047327515, .quotients = &quotients_7902280641, .margins = &margins_240014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  632000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_5409962807, .quotients = &quotients_5444426829, .margins = &margins_287014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_version  ver_ANY_2_0_2_255 = { .foundry = CPR_FOUNDRY_ANY, .min = CPR_CHIPINFO_VERSION(2,0), .max = CPR_CHIPINFO_VERSION(2,255) };


static cpr_margin_cfg margins_215014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   44000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_9011349209 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  4, .quotient =  467 },
                                      { .ro =  5, .quotient =  500 },
                                      { .ro =  6, .quotient =  219 },
                                      { .ro =  7, .quotient =  253 },
                                      { .ro = 10, .quotient =  580 },
                                      { .ro = 12, .quotient =  580 } },
};


static cpr_fuse_data  fuses_5668388777 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_SVS2) } } },
};


static cpr_quotient_cfg  quotients_9053476932 =
{
    .count = 3,
    .quots = (cpr_quotient[]) { { .ro = 10, .quotient =  763 },
                                      { .ro = 11, .quotient =  424 },
                                      { .ro = 12, .quotient =  762 } },
};


static cpr_fuse_data  fuses_4140801727 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_265014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   54000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_7463901740 =
{
    .count = 3,
    .quots = (cpr_quotient[]) { { .ro = 10, .quotient =  907 },
                                      { .ro = 11, .quotient =  550 },
                                      { .ro = 12, .quotient =  916 } },
};


static cpr_fuse_data  fuses_1430905236 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_margin_cfg margins_225014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   46000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_9136368353 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  8, .quotient = 1400 },
                                      { .ro = 10, .quotient = 1143 },
                                      { .ro = 11, .quotient =  735 },
                                      { .ro = 12, .quotient = 1138 },
                                      { .ro = 13, .quotient = 1055 } },
};


static cpr_fuse_data  fuses_9796669542 =
{
    .volt       = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_NOM_4_3), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR2_TARG_VOLT_NOM_2_0) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_quotient_cfg  quotients_8672399701 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  8, .quotient = 1532 },
                                      { .ro = 10, .quotient = 1266 },
                                      { .ro = 11, .quotient =  858 },
                                      { .ro = 12, .quotient = 1251 },
                                      { .ro = 13, .quotient = 1134 } },
};


static cpr_fuse_data  fuses_2306073573 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW11_MSB, CPR2_TARG_VOLT_OFFSET_NOMP) } } },
};


static cpr_margin_cfg margins_522014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =   15000, .closedLoop =   85000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_8082646743 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  8, .quotient = 1600 },
                                      { .ro = 10, .quotient = 1419 },
                                      { .ro = 11, .quotient =  999 },
                                      { .ro = 12, .quotient = 1358 },
                                      { .ro = 13, .quotient = 1197 } },
};


static cpr_fuse_data  fuses_2715853845 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_OFFSET_TUR) } } },
};


static cpr_margin_cfg margins_282014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   55000, .maxFloorToCeil =  52000 } }
};


static cpr_quotient_cfg  quotients_12026380941 =
{
    .count = 5,
    .quots = (cpr_quotient[]) { { .ro =  8, .quotient = 1722 },
                                      { .ro = 10, .quotient = 1563 },
                                      { .ro = 11, .quotient = 1203 },
                                      { .ro = 12, .quotient = 1512 },
                                      { .ro = 13, .quotient = 1305 } },
};


static cpr_fuse_data  fuses_4698302767 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR2_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW11_MSB, CPR2_TARG_VOLT_OFFSET_SUT) } } },
};


static cpr_temp_adj_cfg  temp_adj_cfg_gfx_12_13_30_75_150 =
{
    .tempSensorStartId = 12,
    .tempSensorEndId   = 13,
    .tempPoints        = { 30, 75, 150 }, // degree C
};


static cpr_kv_cfg  kvs_30148362976 =
{
    .count = 16,
    .kvs   = (cpr_kv[]) { { .ro =  0, .kv =    0 },
                                { .ro =  1, .kv =    0 },
                                { .ro =  2, .kv =    0 },
                                { .ro =  3, .kv =    0 },
                                { .ro =  4, .kv =  243 },
                                { .ro =  5, .kv =  264 },
                                { .ro =  6, .kv =  217 },
                                { .ro =  7, .kv =  252 },
                                { .ro =  8, .kv =  202 },
                                { .ro =  9, .kv =    0 },
                                { .ro = 10, .kv =  270 },
                                { .ro = 11, .kv =  269 },
                                { .ro = 12, .kv =  264 },
                                { .ro = 13, .kv =  205 },
                                { .ro = 14, .kv =    0 },
                                { .ro = 15, .kv =    0 } },
};


/*
 * GFX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_15026087805 =
{
    .modesCount      = 8,
    .tempAdjCfg      = &temp_adj_cfg_gfx_12_13_30_75_150,
    .kvCfg           = &kvs_30148362976,
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
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_5668388777, .quotients = &quotients_9011349209, .margins = &margins_215014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =  180000, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =   77000, .fuses = &fuses_5668388777, .quotients = &quotients_9011349209, .margins = &margins_215014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =  257000, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =   85000, .fuses = &fuses_4140801727, .quotients = &quotients_9053476932, .margins = &margins_210014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  740000, .floor =  532000, .freq =  342000, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =   72000, .fuses = &fuses_1430905236, .quotients = &quotients_7463901740, .margins = &margins_265014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =  414000, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =  106000, .fuses = &fuses_9796669542, .quotients = &quotients_9136368353, .margins = &margins_225014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  632000, .freq =  520000, .interplEnabled = 0} } },

        // NOMINAL_L1
        {.fref = 800000, .freqDelta =   76000, .fuses = &fuses_2306073573, .quotients = &quotients_8672399701, .margins = &margins_270014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  928000, .floor =  672000, .freq =  596000, .interplEnabled = 29} } },

        // TURBO
        {.fref = 856000, .freqDelta =   79000, .fuses = &fuses_2715853845, .quotients = &quotients_8082646743, .margins = &margins_522014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  712000, .freq =  675000, .interplEnabled = 61} } },

        // TURBO_L1
        {.fref = 924000, .freqDelta =   35000, .fuses = &fuses_4698302767, .quotients = &quotients_12026380941, .margins = &margins_282014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling = 1068000, .floor =  756000, .freq =  710000, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_24014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =    8000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


static cpr_fuse_data  fuses_2084319257 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR5_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_36014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =   12000, .closedLoop =       0, .maxFloorToCeil =      0 } }
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
static cpr_voltage_plan voltage_plan_4531556564 =
{
    .modesCount      = 3,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // SVS_L1
        {.fref =      0, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_2084319257, .quotients = NULL, .margins = &margins_24014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  928000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_29245628, .quotients = NULL, .margins = &margins_36014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_48014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =   16000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


static cpr_fuse_data  fuses_382056223 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW5_LSB, CPR6_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_132014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =   44000, .closedLoop =       0, .maxFloorToCeil =      0 } }
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
static cpr_voltage_plan voltage_plan_11221824952 =
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
        {.fref = 576000, .freqDelta =       0, .fuses = &fuses_382056223, .quotients = NULL, .margins = &margins_48014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  680000, .floor =  520000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 576000, .freqDelta =       0, .fuses = &fuses_382056223, .quotients = NULL, .margins = &margins_48014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  680000, .floor =  520000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 624000, .freqDelta =       0, .fuses = &fuses_1129114804, .quotients = NULL, .margins = &margins_132014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  736000, .floor =  536000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 680000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_36014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_57588456, .quotients = NULL, .margins = &margins_36014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  632000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_3326791033, .quotients = NULL, .margins = &margins_36014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  992000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_420014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   85000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_17972410550 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  4, .quotient =  331 },
                                      { .ro =  5, .quotient =  357 },
                                      { .ro =  6, .quotient =  127 },
                                      { .ro =  7, .quotient =  148 },
                                      { .ro = 10, .quotient =  419 },
                                      { .ro = 12, .quotient =  420 } },
};


static cpr_fuse_data  fuses_1573982824 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_60014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   13000, .maxFloorToCeil =  40000 } }
};


static cpr_margin_cfg margins_110014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   23000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_2709896491 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_170014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   35000, .maxFloorToCeil =  40000 } }
};


static cpr_margin_cfg margins_160014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   33000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_5540345690 =
{
    .volt       = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_MSB, CPR2_TARG_VOLT_NOM_4_3), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR2_TARG_VOLT_NOM_2_0) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_230014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   47000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_2822618051 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR2_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_257014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   50000, .maxFloorToCeil =  52000 } }
};


/*
 * GFX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_12600180336 =
{
    .modesCount      = 8,
    .tempAdjCfg      = &temp_adj_cfg_gfx_12_13_30_75_150,
    .kvCfg           = &kvs_30148362976,
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
        {.fref = 516000, .freqDelta =       0, .fuses = &fuses_1573982824, .quotients = &quotients_17972410550, .margins = &margins_420014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  616000, .floor =  516000, .freq =  147000, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =   63000, .fuses = &fuses_1573982824, .quotients = &quotients_9011349209, .margins = &margins_60014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =  210000, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =   70000, .fuses = &fuses_2709896491, .quotients = &quotients_9053476932, .margins = &margins_110014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  740000, .floor =  532000, .freq =  280000, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =   58000, .fuses = &fuses_2709896491, .quotients = &quotients_7463901740, .margins = &margins_170014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =  338000, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =   87000, .fuses = &fuses_5540345690, .quotients = &quotients_9136368353, .margins = &margins_160014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  632000, .freq =  425000, .interplEnabled = 0} } },

        // NOMINAL_L1
        {.fref = 800000, .freqDelta =   62000, .fuses = &fuses_2822618051, .quotients = &quotients_8672399701, .margins = &margins_230014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  928000, .floor =  672000, .freq =  487000, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =   61000, .fuses = &fuses_2822618051, .quotients = &quotients_8082646743, .margins = &margins_257014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  712000, .freq =  548000, .interplEnabled = 0} } },

        // TURBO_L1
        {.fref = 924000, .freqDelta =   52000, .fuses = &fuses_2822618051, .quotients = &quotients_12026380941, .margins = &margins_272014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling = 1068000, .floor =  756000, .freq =  600000, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_300014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  100000, .closedLoop =       0, .maxFloorToCeil =      0 } }
};


/*
 * LPI_MX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_1239686952 =
{
    .modesCount      = 3,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // SVS_L1
        {.fref =      0, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_2084319257, .quotients = NULL, .margins = &margins_300014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  928000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_29245628, .quotients = NULL, .margins = &margins_300014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  992000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


/*
 * LPI_CX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_9645298921 =
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
        {.fref = 680000, .freqDelta =       0, .fuses = &fuses_1129114804, .quotients = NULL, .margins = &margins_300014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 680000, .freqDelta =       0, .fuses = &fuses_1129114804, .quotients = NULL, .margins = &margins_300014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 680000, .freqDelta =       0, .fuses = &fuses_1129114804, .quotients = NULL, .margins = &margins_300014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 680000, .freqDelta =       0, .fuses = &fuses_1129114804, .quotients = NULL, .margins = &margins_300014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_57588456, .quotients = NULL, .margins = &margins_300014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  632000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_3326791033, .quotients = NULL, .margins = &margins_300014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  992000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_34986 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   -6000, .maxFloorToCeil =  40000 } }
};


static cpr_quotient_cfg  quotients_18751134592 =
{
    .count = 6,
    .quots = (cpr_quotient[]) { { .ro =  2, .quotient =  368 },
                                      { .ro =  3, .quotient =  420 },
                                      { .ro =  4, .quotient =  331 },
                                      { .ro =  5, .quotient =  357 },
                                      { .ro =  6, .quotient =  127 },
                                      { .ro =  7, .quotient =  148 } },
};


static cpr_fuse_data  fuses_2758229562 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_85014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   18000, .maxFloorToCeil =  40000 } }
};


static cpr_margin_cfg margins_115014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   24000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_69276407 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_185014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   38000, .maxFloorToCeil =  40000 } }
};


static cpr_margin_cfg margins_195014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   40000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_2433322795 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_margin_cfg margins_277014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   54000, .maxFloorToCeil =  52000 } }
};


static cpr_fuse_data  fuses_2460378218 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


static cpr_temp_adj_cfg  temp_adj_cfg_mss_0_0_30_75_150 =
{
    .tempSensorStartId = 0,
    .tempSensorEndId   = 0,
    .tempPoints        = { 30, 75, 150 }, // degree C
};


/*
 * MSS Voltage Plan
 */
static cpr_voltage_plan voltage_plan_12958866232 =
{
    .modesCount      = 6,
    .tempAdjCfg      = &temp_adj_cfg_mss_0_0_30_75_150,
    .kvCfg           = &kvs_31234238427,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 516000, .freqDelta =       0, .fuses = &fuses_2758229562, .quotients = &quotients_18751134592, .margins = &margins_34986,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  616000, .floor =  516000, .freq =  300100, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =  169920, .fuses = &fuses_2758229562, .quotients = &quotients_10145960225, .margins = &margins_85014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =  470020, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =  149980, .fuses = &fuses_69276407, .quotients = &quotients_7883330866, .margins = &margins_115014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  740000, .floor =  532000, .freq =  620000, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =  180000, .fuses = &fuses_69276407, .quotients = &quotients_12007322685, .margins = &margins_185014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =  800000, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =  150000, .fuses = &fuses_2433322795, .quotients = &quotients_7902280641, .margins = &margins_195014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  632000, .freq =  950000, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =  250000, .fuses = &fuses_2460378218, .quotients = &quotients_5444426829, .margins = &margins_277014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  712000, .freq = 1200000, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_730014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =   40000, .closedLoop =  114000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_5906379025 =
{
    .volt       = (cpr_fuse[]) { {.count = 2, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_NOM_4_3), CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR4_TARG_VOLT_NOM_2_0) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_567014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =   20000, .closedLoop =   91000, .maxFloorToCeil =  52000 } }
};


static cpr_fuse_data  fuses_4409588226 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_LSB, CPR4_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW4_MSB, CPR4_TARG_VOLT_OFFSET_TUR) } } },
};


/*
 * MX Voltage Plan
 */
static cpr_voltage_plan voltage_plan_7267126352 =
{
    .modesCount      = 3,
    .tempAdjCfg      = NULL,
    .kvCfg           = &kvs_31700313473,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // SVS_L1
        {.fref =      0, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_5906379025, .quotients = &quotients_8749835594, .margins = &margins_730014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  928000, .floor =  800000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_4409588226, .quotients = &quotients_8445637474, .margins = &margins_567014,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_fuse_data  fuses_1741551070 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW12_LSB, CPR9_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = NULL,
};


/*
 * WLAN Voltage Plan
 */
static cpr_voltage_plan voltage_plan_2545652066 =
{
    .modesCount      = 1,
    .tempAdjCfg      = NULL,
    .kvCfg           = NULL,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_NOMINAL },
    .modes           = (struct cpr_voltage_data[]) {
        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_1741551070, .quotients = NULL, .margins = NULL,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  800000, .freq =       0, .interplEnabled = 0} } }},
};


static cpr_margin_cfg margins_175014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   36000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_5078128459 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_SVS2) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_SVS2) } } },
};


static cpr_margin_cfg margins_285014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   58000, .maxFloorToCeil =  40000 } }
};


static cpr_fuse_data  fuses_382500705 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_SVS) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_fuse_data  fuses_313224298 =
{
    .volt       = NULL,
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_SVS) } } },
};


static cpr_fuse_data  fuses_5682902217 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_NOM) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_NOM) } } },
};


static cpr_margin_cfg margins_432014 = {
    .count = 1,
    .data = (cpr_margin_data[]) { { .cprRevMin = 0, .cprRevMax = 7, .openLoop =  -15000, .closedLoop =   85000, .maxFloorToCeil =  52000 } }
};


static cpr_fuse_data  fuses_4806965601 =
{
    .volt       = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW2_MSB, CPR1_TARG_VOLT_TUR) } } },
    .quot       = NULL,
    .rosel      = NULL,
    .quotOffset = NULL,
    .voltOffset = (cpr_fuse[]) { {.count = 1, .data = (struct raw_fuse_data[]) { CPR_FUSE_MAPPING(QFPROM_CORR_CALIB_ROW3_LSB, CPR1_TARG_VOLT_OFFSET_TUR) } } },
};


/*
 * MSS Voltage Plan
 */
static cpr_voltage_plan voltage_plan_13258351293 =
{
    .modesCount      = 6,
    .tempAdjCfg      = &temp_adj_cfg_mss_0_0_30_75_150,
    .kvCfg           = &kvs_31234238427,
    .supportedModes  = (cpr_voltage_mode[]) { CPR_VOLTAGE_MODE_MIN_SVS,
                                                    CPR_VOLTAGE_MODE_LOW_SVS,
                                                    CPR_VOLTAGE_MODE_SVS,
                                                    CPR_VOLTAGE_MODE_SVS_L1,
                                                    CPR_VOLTAGE_MODE_NOMINAL,
                                                    CPR_VOLTAGE_MODE_TURBO },
    .modes           = (struct cpr_voltage_data[]) {
        // MIN_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_5078128459, .quotients = &quotients_10145960225, .margins = &margins_175014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_5078128459, .quotients = &quotients_10145960225, .margins = &margins_175014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =       0, .interplEnabled = 0} } },

        // SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_382500705, .quotients = &quotients_7883330866, .margins = &margins_285014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  740000, .floor =  532000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_313224298, .quotients = &quotients_12007322685, .margins = &margins_420014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  800000, .floor =  584000, .freq =       0, .interplEnabled = 45} } },

        // NOMINAL
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_5682902217, .quotients = &quotients_7902280641, .margins = &margins_420014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  632000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_4806965601, .quotients = &quotients_5444426829, .margins = &margins_432014,
         .marginTempBands = { 0, 0, 0, 0 }, // PMIC step size
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  712000, .freq =       0, .interplEnabled = 0} } }},
};


/*
 * VDDA Voltage Plan
 */
static cpr_voltage_plan voltage_plan_15673425822 =
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
        {.fref = 572000, .freqDelta =       0, .fuses = &fuses_3951258467, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  676000, .floor =  516000, .freq =       0, .interplEnabled = 0} } },

        // LOW_SVS
        {.fref = 628000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  740000, .floor =  532000, .freq =       0, .interplEnabled = 50} } },

        // SVS
        {.fref = 684000, .freqDelta =       0, .fuses = &fuses_655279266, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  752000, .floor =  672000, .freq =       0, .interplEnabled = 0} } },

        // SVS_L1
        {.fref = 752000, .freqDelta =       0, .fuses = &fuses_0, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  872000, .floor =  672000, .freq =       0, .interplEnabled = 57} } },

        // NOMINAL
        {.fref = 800000, .freqDelta =       0, .fuses = &fuses_2626994354, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  928000, .floor =  752000, .freq =       0, .interplEnabled = 0} } },

        // TURBO
        {.fref = 856000, .freqDelta =       0, .fuses = &fuses_1606482579, .quotients = NULL, .margins = &margins_14,
         .marginTempBands = {0},
         .subModesCount   = 1,
         .subModes        = (cpr_freq_data[]){ {.ceiling =  996000, .floor =  784000, .freq =       0, .interplEnabled = 0} } }},
};


/*
 * Aging configs
 */

static cpr_aging_cfg  aging_cfg_mss =
{
    .cprRevMinToRun       = 0,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO,
    .sensorID             = 9,
    .kv                   = 162,
    .scalingFactor        = 10,
    .marginLimit          = 15000,
    .bypassSensorIDs      = NULL,
    .bypassSensorIDsCount = 0,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 1, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x780200 & (SECURITY_CONTROL_BASE_SIZE - 1)), 16, (1<<23) | (1<<22) | (1<<21) | (1<<20) | (1<<19) | (1<<18) | (1<<17) | (1<<16)} } }
};


static cpr_aging_cfg  aging_cfg_cx =
{
    .cprRevMinToRun       = 0,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO_L1,
    .sensorID             = 15,
    .kv                   = 162,
    .scalingFactor        = 10,
    .marginLimit          = 15000,
    .bypassSensorIDs      = NULL,
    .bypassSensorIDsCount = 0,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 1, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x7801FC & (SECURITY_CONTROL_BASE_SIZE - 1)), 4, (1<<11) | (1<<10) | (1<<9) | (1<<8) | (1<<7) | (1<<6) | (1<<5) | (1<<4)} } }
};


static cpr_aging_cfg  aging_cfg_gfx =
{
    .cprRevMinToRun       = 0,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO_L1,
    .sensorID             = 18,
    .kv                   = 162,
    .scalingFactor        = 10,
    .marginLimit          = 15000,
    .bypassSensorIDs      = NULL,
    .bypassSensorIDsCount = 0,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 2, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x780204 & (SECURITY_CONTROL_BASE_SIZE - 1)), 28, (1<<31) | (1<<30) | (1<<29) | (1<<28)},
                                 {SECURITY_CONTROL_BASE | (0x780208 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<3) | (1<<2) | (1<<1) | (1<<0)} } }
};


static cpr_aging_cfg  aging_cfg_mx =
{
    .cprRevMinToRun       = 0,
    .modeToRun            = CPR_VOLTAGE_MODE_TURBO,
    .sensorID             = 5,
    .kv                   = 162,
    .scalingFactor        = 10,
    .marginLimit          = 15000,
    .bypassSensorIDs      = (uint8[]) {1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22},
    .bypassSensorIDsCount = 20,
    .fusePackingFactor    = 1,
    .fuse                 = {.count = 1, .data = (struct raw_fuse_data[]) {
                                 {SECURITY_CONTROL_BASE | (0x780244 & (SECURITY_CONTROL_BASE_SIZE - 1)), 24, (1<<31) | (1<<30) | (1<<29) | (1<<28) | (1<<27) | (1<<26) | (1<<25) | (1<<24)} } }
};


/*
 * Versioned Voltage Plans
 */

static cpr_versioned_voltage_plan vdda_vvp = {
    .rail     = CPR_RAIL_VDDA,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_8405015862 },

        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_15673425822 },
     },
    .count    = 2,
    .agingCfg = NULL
};


static cpr_versioned_voltage_plan mss_vvp = {
    .rail     = CPR_RAIL_MSS,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_12958866232 },

        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_13258351293 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_mss
};


static cpr_versioned_voltage_plan lpi_cx_vvp = {
    .rail     = CPR_RAIL_LPI_CX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_11221824952 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_9645298921 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_cx
};


static cpr_versioned_voltage_plan wlan_vvp = {
    .rail     = CPR_RAIL_WLAN,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_2545652066 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_2545652066 },
     },
    .count    = 2,
    .agingCfg = NULL
};


static cpr_versioned_voltage_plan cx_vvp = {
    .rail     = CPR_RAIL_CX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_16908331946 },

        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_11549500602 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_cx
};


static cpr_versioned_voltage_plan gfx_vvp = {
    .rail     = CPR_RAIL_GFX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_15026087805 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_12600180336 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_gfx
};


static cpr_versioned_voltage_plan lpi_mx_vvp = {
    .rail     = CPR_RAIL_LPI_MX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_4531556564 },

        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_1239686952 },
     },
    .count    = 2,
    .agingCfg = &aging_cfg_mx
};


static cpr_versioned_voltage_plan mx_vvp = {
    .rail     = CPR_RAIL_MX,
    .list     = (struct voltage_plan_list[]) {
        { .version = &ver_ANY_1_0_1_255, .cfg = &voltage_plan_7284521173 },

        { .version = &ver_ANY_2_0_2_255, .cfg = &voltage_plan_7267126352 },
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

static cpr_hal_thread_cfg cx_thread_cfg = {
    .upThresh     = 2,
    .dnThresh     = 2,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50 },
    .sensorsCount = 51,
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


static cpr_hal_thread_cfg mss_thread_cfg = {
    .upThresh     = 2,
    .dnThresh     = 2,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 },
    .sensorsCount = 19,
};


static cpr_hal_thread  mss_thread = {
    .id   = 0,
    .cfg  = &mss_thread_cfg,
};


static cpr_hal_thread_cfg mx_thread_cfg = {
    .upThresh     = 2,
    .dnThresh     = 2,
    .consecUp     = 0,
    .consecDn     = 0,
    .sensors      = (uint8[]){ 0,5,12,23,24 },
    .sensorsCount = 5,
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
    .sensors      = (uint8[]){ 0,5,12,23,24 },
    .sensorsCount = 5,
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
    .sensors      = (uint8[]){ 20,21 },
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
    .sensors      = (uint8[]){ 22,23 },
    .sensorsCount = 2,
};


static cpr_hal_thread  lpi_mx_thread = {
    .id   = 3,
    .cfg  = &lpi_mx_thread_cfg,
};


/*
 * CPR Controller Config
 */

static cpr_hal_controller_cfg cx_cpr3_controller_cfg = {
    .stepQuotMin         = 8,
    .stepQuotMax         = 12,
    .sensorsTotal        = 51,
    .bypassSensors       = NULL,
    .bypassSensorsCount  = 0,
    .disableSensors      = (uint8[]){ 1,5,6,49,50,53,54 },
    .disableSensorsCount = 7,
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
    .stepQuotMax         = 12,
    .sensorsTotal        = 19,
    .bypassSensors       = NULL,
    .bypassSensorsCount  = 0,
    .disableSensors      = NULL,
    .disableSensorsCount = 0,
    .refClk              = NULL,
    .ahbClk              = NULL,
};


static cpr_hal_controller  gpu_cpr_controller = {
    .base                = 0x509a000,
    .type                = CPR_CONTROLLER_TYPE_HW_CL_ONLY,
    .cfg                 = &gpu_cpr_controller_cfg,
    .threads             = (cpr_hal_thread*[]){ &gfx_thread },
    .threadsCount        = 1,
};


static cpr_hal_controller_cfg mss_cpr3_controller_cfg = {
    .stepQuotMin         = 8,
    .stepQuotMax         = 12,
    .sensorsTotal        = 19,
    .bypassSensors       = NULL,
    .bypassSensorsCount  = 0,
    .disableSensors      = NULL,
    .disableSensorsCount = 0,
    .refClk              = "clk_rbcpr_ref",
    .ahbClk              = "clk_bus_rbcpr",
};


static cpr_hal_controller  mss_cpr3_controller = {
    .base                = 0xeb1b1000,
    .type                = CPR_CONTROLLER_TYPE_HW_CL_ONLY,
    .cfg                 = &mss_cpr3_controller_cfg,
    .threads             = (cpr_hal_thread*[]){ &mss_thread },
    .threadsCount        = 1,
};


static cpr_hal_controller_cfg mx_cpr3_controller_cfg = {
    .stepQuotMin         = 8,
    .stepQuotMax         = 12,
    .sensorsTotal        = 25,
    .bypassSensors       = (uint8[]){ 1,2,3,4,6,7,8,9,10,11,13,14,15,16,17,18,19,20,21,22 },
    .bypassSensorsCount  = 20,
    .disableSensors      = (uint8[]){ 1,2,3,4,6,7,8,9,10,11,13,14,15,16,17,18,19,20,21,22 },
    .disableSensorsCount = 20,
    .refClk              = NULL,
    .ahbClk              = NULL,
};


static cpr_hal_controller  mx_cpr3_controller = {
    .base                = 0x62c000,
    .type                = CPR_CONTROLLER_TYPE_HW_CL_ONLY,
    .cfg                 = &mx_cpr3_controller_cfg,
    .threads             = (cpr_hal_thread*[]){ &mx_thread, &vdda_thread, &lpi_cx_thread, &lpi_mx_thread },
    .threadsCount        = 4,
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
    .interruptId      = 303,
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


static cpr_rail cpr_rail_mss_cfg = {
    .id               = CPR_RAIL_MSS,
    .name             = "MSS",
    .hal              = { .controller = &mss_cpr3_controller, .thread = &mss_thread },
    .interruptId      = 302,
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
                   {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_cx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_CX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_SW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 2, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 31, (1<<31)},
                   {SECURITY_CONTROL_BASE | (0x78025C & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_gfx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_GFX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_gfx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_GFX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 2, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 31, (1<<31)},
                   {SECURITY_CONTROL_BASE | (0x78025C & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_lpi_cx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_LPI_CX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_lpi_cx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_LPI_CX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 2, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 31, (1<<31)},
                   {SECURITY_CONTROL_BASE | (0x78025C & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_lpi_mx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_LPI_MX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_lpi_mx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_LPI_MX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 2, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 31, (1<<31)},
                   {SECURITY_CONTROL_BASE | (0x78025C & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_mss_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_MSS,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_mss_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_MSS,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_HW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 2, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 31, (1<<31)},
                   {SECURITY_CONTROL_BASE | (0x78025C & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_mx_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_MX,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_SW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_mx_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_MX,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_SW_CLOSED_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 2, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 31, (1<<31)},
                   {SECURITY_CONTROL_BASE | (0x78025C & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_vdda_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_VDDA,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_vdda_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_VDDA,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 4000,
    .railCPRRevFuse    = {.count = 2, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 31, (1<<31)},
                   {SECURITY_CONTROL_BASE | (0x78025C & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_wlan_enablement_1_0_1_255 =
{
    .id                = CPR_RAIL_WLAN,
    .version           = &ver_ANY_1_0_1_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 1, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } },
};


static cpr_enablement cpr_rail_wlan_enablement_2_0_2_255 =
{
    .id                = CPR_RAIL_WLAN,
    .version           = &ver_ANY_2_0_2_255,
    .funcId            = CPR_INIT_OPEN_LOOP,
    .fuseMultiplier    = 10000,
    .stepSize          = 8000,
    .railCPRRevFuse    = {.count = 2, .data = (struct raw_fuse_data[]) {
                   {SECURITY_CONTROL_BASE | (0x780258 & (SECURITY_CONTROL_BASE_SIZE - 1)), 31, (1<<31)},
                   {SECURITY_CONTROL_BASE | (0x78025C & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<1) | (1<<0)} } },
};


static cpr_misc_cfg misc_cfg = {
    .cprRev = {.count = 1, .data = (struct raw_fuse_data[]) { {SECURITY_CONTROL_BASE | (0x780248 & (SECURITY_CONTROL_BASE_SIZE - 1)), 0, (1<<2) | (1<<1) | (1<<0)} } }
};


cpr_settings cpr_settings_845 =
{
    .hashValue        = voltage_plan_hash_value,

    .railVVPs         = (cpr_versioned_voltage_plan*[]) { &cx_vvp, &gfx_vvp, &lpi_cx_vvp, &lpi_mx_vvp, &mss_vvp, &mx_vvp, &vdda_vvp, &wlan_vvp },
    .rails            = (cpr_rail*[]) { &cpr_rail_cx_cfg, &cpr_rail_gfx_cfg, &cpr_rail_lpi_cx_cfg, &cpr_rail_lpi_mx_cfg, &cpr_rail_mss_cfg, &cpr_rail_mx_cfg, &cpr_rail_vdda_cfg, &cpr_rail_wlan_cfg },
    .railsCount       = 8,

    .controllers      = (cpr_hal_controller*[]) { &cx_cpr3_controller, &gpu_cpr_controller, &mss_cpr3_controller, &mx_cpr3_controller },
    .controllersCount = 4,

    .enablements      = (cpr_enablement*[]) { &cpr_rail_cx_enablement_1_0_1_255, &cpr_rail_cx_enablement_2_0_2_255, &cpr_rail_gfx_enablement_1_0_1_255, &cpr_rail_gfx_enablement_2_0_2_255, &cpr_rail_lpi_cx_enablement_1_0_1_255, &cpr_rail_lpi_cx_enablement_2_0_2_255, &cpr_rail_lpi_mx_enablement_1_0_1_255, &cpr_rail_lpi_mx_enablement_2_0_2_255, &cpr_rail_mss_enablement_1_0_1_255, &cpr_rail_mss_enablement_2_0_2_255, &cpr_rail_mx_enablement_1_0_1_255, &cpr_rail_mx_enablement_2_0_2_255, &cpr_rail_vdda_enablement_1_0_1_255, &cpr_rail_vdda_enablement_2_0_2_255, &cpr_rail_wlan_enablement_1_0_1_255, &cpr_rail_wlan_enablement_2_0_2_255 },
    .enablementsCount = 16,

    .miscCfg          = &misc_cfg,
};

