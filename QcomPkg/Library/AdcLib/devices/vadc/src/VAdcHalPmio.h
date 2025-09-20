#ifndef __VADCHALPMIO_H__
#define __VADCHALPMIO_H__
/*
===========================================================================
*/
/**
  @file VAdcHalPmio.h
  @brief Auto-generated PMIO interface include file.

  This file contains PMIO register definitions for the following modules:
    ADC_FG1_USR

  'Include' filters applied: <none>
  'Exclude' filters applied: RESERVED DUMMY
*/
/*
  ===========================================================================

  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

  ===========================================================================


  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * PERIPHERAL: ADC_FG1_USR
 *--------------------------------------------------------------------------*/

#define PMIO_REVISION1_ADDR(x)                                                ((x) + 0x00000000)
#define PMIO_REVISION1_RMSK                                                         0xff
#define PMIO_REVISION1_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_REVISION1_ADDR(base), PMIO_REVISION1_RMSK, 0, val)
#define PMIO_REVISION1_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_REVISION1_ADDR(base), mask, shift, val)
#define PMIO_REVISION1_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_REVISION1_ADDR(base), val, len)
#define PMIO_REVISION1_DIG_MINOR_BMSK                                               0xff
#define PMIO_REVISION1_DIG_MINOR_SHFT                                                0x0

#define PMIO_REVISION2_ADDR(x)                                                ((x) + 0x00000001)
#define PMIO_REVISION2_RMSK                                                         0xff
#define PMIO_REVISION2_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_REVISION2_ADDR(base), PMIO_REVISION2_RMSK, 0, val)
#define PMIO_REVISION2_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_REVISION2_ADDR(base), mask, shift, val)
#define PMIO_REVISION2_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_REVISION2_ADDR(base), val, len)
#define PMIO_REVISION2_DIG_MAJOR_BMSK                                               0xff
#define PMIO_REVISION2_DIG_MAJOR_SHFT                                                0x0

#define PMIO_REVISION3_ADDR(x)                                                ((x) + 0x00000002)
#define PMIO_REVISION3_RMSK                                                         0xff
#define PMIO_REVISION3_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_REVISION3_ADDR(base), PMIO_REVISION3_RMSK, 0, val)
#define PMIO_REVISION3_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_REVISION3_ADDR(base), mask, shift, val)
#define PMIO_REVISION3_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_REVISION3_ADDR(base), val, len)
#define PMIO_REVISION3_ANA_MINOR_BMSK                                               0xff
#define PMIO_REVISION3_ANA_MINOR_SHFT                                                0x0

#define PMIO_REVISION4_ADDR(x)                                                ((x) + 0x00000003)
#define PMIO_REVISION4_RMSK                                                         0xff
#define PMIO_REVISION4_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_REVISION4_ADDR(base), PMIO_REVISION4_RMSK, 0, val)
#define PMIO_REVISION4_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_REVISION4_ADDR(base), mask, shift, val)
#define PMIO_REVISION4_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_REVISION4_ADDR(base), val, len)
#define PMIO_REVISION4_ANA_MAJOR_BMSK                                               0xff
#define PMIO_REVISION4_ANA_MAJOR_SHFT                                                0x0

#define PMIO_PERPH_TYPE_ADDR(x)                                               ((x) + 0x00000004)
#define PMIO_PERPH_TYPE_RMSK                                                        0xff
#define PMIO_PERPH_TYPE_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_TYPE_ADDR(base), PMIO_PERPH_TYPE_RMSK, 0, val)
#define PMIO_PERPH_TYPE_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_TYPE_ADDR(base), mask, shift, val)
#define PMIO_PERPH_TYPE_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_PERPH_TYPE_ADDR(base), val, len)
#define PMIO_PERPH_TYPE_TYPE_BMSK                                                   0xff
#define PMIO_PERPH_TYPE_TYPE_SHFT                                                    0x0

#define PMIO_PERPH_SUBTYPE_ADDR(x)                                            ((x) + 0x00000005)
#define PMIO_PERPH_SUBTYPE_RMSK                                                     0xff
#define PMIO_PERPH_SUBTYPE_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_SUBTYPE_ADDR(base), PMIO_PERPH_SUBTYPE_RMSK, 0, val)
#define PMIO_PERPH_SUBTYPE_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_SUBTYPE_ADDR(base), mask, shift, val)
#define PMIO_PERPH_SUBTYPE_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_PERPH_SUBTYPE_ADDR(base), val, len)
#define PMIO_PERPH_SUBTYPE_SUBTYPE_BMSK                                             0xff
#define PMIO_PERPH_SUBTYPE_SUBTYPE_SHFT                                              0x0

#define PMIO_STATUS1_ADDR(x)                                                  ((x) + 0x00000008)
#define PMIO_STATUS1_RMSK                                                            0x3
#define PMIO_STATUS1_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_STATUS1_ADDR(base), PMIO_STATUS1_RMSK, 0, val)
#define PMIO_STATUS1_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_STATUS1_ADDR(base), mask, shift, val)
#define PMIO_STATUS1_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_STATUS1_ADDR(base), val, len)
#define PMIO_STATUS1_REQ_STS_BMSK                                                    0x2
#define PMIO_STATUS1_REQ_STS_SHFT                                                    0x1
#define PMIO_STATUS1_REQ_STS_REQ_NOT_IN_PROGRESS_FVAL                                0x0
#define PMIO_STATUS1_REQ_STS_REQ_IN_PROGRESS_FVAL                                    0x1
#define PMIO_STATUS1_EOC_BMSK                                                        0x1
#define PMIO_STATUS1_EOC_SHFT                                                        0x0
#define PMIO_STATUS1_EOC_CONV_NOT_COMPLETE_FVAL                                      0x0
#define PMIO_STATUS1_EOC_CONV_COMPLETE_FVAL                                          0x1

#define PMIO_STATUS2_ADDR(x)                                                  ((x) + 0x00000009)
#define PMIO_STATUS2_RMSK                                                           0xe0
#define PMIO_STATUS2_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_STATUS2_ADDR(base), PMIO_STATUS2_RMSK, 0, val)
#define PMIO_STATUS2_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_STATUS2_ADDR(base), mask, shift, val)
#define PMIO_STATUS2_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_STATUS2_ADDR(base), val, len)
#define PMIO_STATUS2_CONV_SEQ_STATE_BMSK                                            0xe0
#define PMIO_STATUS2_CONV_SEQ_STATE_SHFT                                             0x5
#define PMIO_STATUS2_CONV_SEQ_STATE_IDLE_REQ_FVAL                                    0x0
#define PMIO_STATUS2_CONV_SEQ_STATE_WAIT_HOLDOFF_RST_FVAL                            0x1
#define PMIO_STATUS2_CONV_SEQ_STATE_WAIT_HOLDOFF_FVAL                                0x2
#define PMIO_STATUS2_CONV_SEQ_STATE_RST_CH_ARB_FVAL                                  0x3
#define PMIO_STATUS2_CONV_SEQ_STATE_GET_NEXT_CH_FVAL                                 0x4
#define PMIO_STATUS2_CONV_SEQ_STATE_WAIT_ADC_EOC_FVAL                                0x5
#define PMIO_STATUS2_CONV_SEQ_STATE_WAIT_ADC_EOC2_FVAL                               0x6
#define PMIO_STATUS2_CONV_SEQ_STATE_GEN_IRQ_FVAL                                     0x7

#define PMIO_IBAT_MEAS_ADDR(x)                                                ((x) + 0x0000000f)
#define PMIO_IBAT_MEAS_RMSK                                                          0x1
#define PMIO_IBAT_MEAS_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_IBAT_MEAS_ADDR(base), PMIO_IBAT_MEAS_RMSK, 0, val)
#define PMIO_IBAT_MEAS_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_IBAT_MEAS_ADDR(base), mask, shift, val)
#define PMIO_IBAT_MEAS_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_IBAT_MEAS_ADDR(base), val, len)
#define PMIO_IBAT_MEAS_SUPPORTED_BMSK                                                0x1
#define PMIO_IBAT_MEAS_SUPPORTED_SHFT                                                0x0
#define PMIO_IBAT_MEAS_SUPPORTED_IBAT_MEAS_UNSUPPORTED_FVAL                          0x0
#define PMIO_IBAT_MEAS_SUPPORTED_IBAT_MEAS_SUPPORTED_FVAL                            0x1

#define PMIO_INT_RT_STS_ADDR(x)                                               ((x) + 0x00000010)
#define PMIO_INT_RT_STS_RMSK                                                         0x1
#define PMIO_INT_RT_STS_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_RT_STS_ADDR(base), PMIO_INT_RT_STS_RMSK, 0, val)
#define PMIO_INT_RT_STS_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_RT_STS_ADDR(base), mask, shift, val)
#define PMIO_INT_RT_STS_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_RT_STS_ADDR(base), val, len)
#define PMIO_INT_RT_STS_EOC_INT_RT_STS_BMSK                                          0x1
#define PMIO_INT_RT_STS_EOC_INT_RT_STS_SHFT                                          0x0
#define PMIO_INT_RT_STS_EOC_INT_RT_STS_CONV_COMPLETE_INT_FALSE_FVAL                  0x0
#define PMIO_INT_RT_STS_EOC_INT_RT_STS_CONV_COMPLETE_INT_TRUE_FVAL                   0x1

#define PMIO_INT_SET_TYPE_ADDR(x)                                             ((x) + 0x00000011)
#define PMIO_INT_SET_TYPE_RMSK                                                       0x1
#define PMIO_INT_SET_TYPE_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_SET_TYPE_ADDR(base), PMIO_INT_SET_TYPE_RMSK, 0, val)
#define PMIO_INT_SET_TYPE_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_SET_TYPE_ADDR(base), mask, shift, val)
#define PMIO_INT_SET_TYPE_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_SET_TYPE_ADDR(base), val, len)
#define PMIO_INT_SET_TYPE_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_SET_TYPE_ADDR(base), val)
#define PMIO_INT_SET_TYPE_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_SET_TYPE_ADDR(base), mask, shift, val)
#define PMIO_INT_SET_TYPE_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_SET_TYPE_ADDR(base), val, len)
#define PMIO_INT_SET_TYPE_EOC_SET_INT_TYPE_BMSK                                      0x1
#define PMIO_INT_SET_TYPE_EOC_SET_INT_TYPE_SHFT                                      0x0
#define PMIO_INT_SET_TYPE_EOC_SET_INT_TYPE_EOC_LEVEL_FVAL                            0x0
#define PMIO_INT_SET_TYPE_EOC_SET_INT_TYPE_EOC_EDGE_FVAL                             0x1

#define PMIO_INT_POLARITY_HIGH_ADDR(x)                                        ((x) + 0x00000012)
#define PMIO_INT_POLARITY_HIGH_RMSK                                                  0x1
#define PMIO_INT_POLARITY_HIGH_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_POLARITY_HIGH_ADDR(base), PMIO_INT_POLARITY_HIGH_RMSK, 0, val)
#define PMIO_INT_POLARITY_HIGH_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_POLARITY_HIGH_ADDR(base), mask, shift, val)
#define PMIO_INT_POLARITY_HIGH_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_POLARITY_HIGH_ADDR(base), val, len)
#define PMIO_INT_POLARITY_HIGH_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_POLARITY_HIGH_ADDR(base), val)
#define PMIO_INT_POLARITY_HIGH_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_POLARITY_HIGH_ADDR(base), mask, shift, val)
#define PMIO_INT_POLARITY_HIGH_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_POLARITY_HIGH_ADDR(base), val, len)
#define PMIO_INT_POLARITY_HIGH_EOC_INT_HIGH_BMSK                                     0x1
#define PMIO_INT_POLARITY_HIGH_EOC_INT_HIGH_SHFT                                     0x0
#define PMIO_INT_POLARITY_HIGH_EOC_INT_HIGH_EOC_INT_POL_HIGH_DISABLED_FVAL           0x0
#define PMIO_INT_POLARITY_HIGH_EOC_INT_HIGH_EOC_INT_POL_HIGH_ENABLED_FVAL            0x1

#define PMIO_INT_POLARITY_LOW_ADDR(x)                                         ((x) + 0x00000013)
#define PMIO_INT_POLARITY_LOW_RMSK                                                   0x1
#define PMIO_INT_POLARITY_LOW_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_POLARITY_LOW_ADDR(base), PMIO_INT_POLARITY_LOW_RMSK, 0, val)
#define PMIO_INT_POLARITY_LOW_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_POLARITY_LOW_ADDR(base), mask, shift, val)
#define PMIO_INT_POLARITY_LOW_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_POLARITY_LOW_ADDR(base), val, len)
#define PMIO_INT_POLARITY_LOW_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_POLARITY_LOW_ADDR(base), val)
#define PMIO_INT_POLARITY_LOW_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_POLARITY_LOW_ADDR(base), mask, shift, val)
#define PMIO_INT_POLARITY_LOW_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_POLARITY_LOW_ADDR(base), val, len)
#define PMIO_INT_POLARITY_LOW_EOC_INT_LOW_BMSK                                       0x1
#define PMIO_INT_POLARITY_LOW_EOC_INT_LOW_SHFT                                       0x0
#define PMIO_INT_POLARITY_LOW_EOC_INT_LOW_EOC_INT_POL_LOW_DISABLED_FVAL              0x0
#define PMIO_INT_POLARITY_LOW_EOC_INT_LOW_EOC_INT_POL_LOW_ENABLED_FVAL               0x1

#define PMIO_INT_LATCHED_CLR_ADDR(x)                                          ((x) + 0x00000014)
#define PMIO_INT_LATCHED_CLR_RMSK                                                    0x1
#define PMIO_INT_LATCHED_CLR_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_LATCHED_CLR_ADDR(base), val)
#define PMIO_INT_LATCHED_CLR_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_LATCHED_CLR_ADDR(base), val, len)
#define PMIO_INT_LATCHED_CLR_EOC_INT_LATCHED_CLR_BMSK                                0x1
#define PMIO_INT_LATCHED_CLR_EOC_INT_LATCHED_CLR_SHFT                                0x0

#define PMIO_INT_EN_SET_ADDR(x)                                               ((x) + 0x00000015)
#define PMIO_INT_EN_SET_RMSK                                                         0x1
#define PMIO_INT_EN_SET_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_EN_SET_ADDR(base), PMIO_INT_EN_SET_RMSK, 0, val)
#define PMIO_INT_EN_SET_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_EN_SET_ADDR(base), mask, shift, val)
#define PMIO_INT_EN_SET_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_EN_SET_ADDR(base), val, len)
#define PMIO_INT_EN_SET_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_EN_SET_ADDR(base), val)
#define PMIO_INT_EN_SET_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_EN_SET_ADDR(base), mask, shift, val)
#define PMIO_INT_EN_SET_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_EN_SET_ADDR(base), val, len)
#define PMIO_INT_EN_SET_EOC_INT_EN_SET_BMSK                                          0x1
#define PMIO_INT_EN_SET_EOC_INT_EN_SET_SHFT                                          0x0
#define PMIO_INT_EN_SET_EOC_INT_EN_SET_EOC_INT_DISABLED_FVAL                         0x0
#define PMIO_INT_EN_SET_EOC_INT_EN_SET_EOC_INT_ENBLED_FVAL                           0x1

#define PMIO_INT_EN_CLR_ADDR(x)                                               ((x) + 0x00000016)
#define PMIO_INT_EN_CLR_RMSK                                                         0x1
#define PMIO_INT_EN_CLR_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_EN_CLR_ADDR(base), PMIO_INT_EN_CLR_RMSK, 0, val)
#define PMIO_INT_EN_CLR_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_EN_CLR_ADDR(base), mask, shift, val)
#define PMIO_INT_EN_CLR_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_EN_CLR_ADDR(base), val, len)
#define PMIO_INT_EN_CLR_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_EN_CLR_ADDR(base), val)
#define PMIO_INT_EN_CLR_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_EN_CLR_ADDR(base), mask, shift, val)
#define PMIO_INT_EN_CLR_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_EN_CLR_ADDR(base), val, len)
#define PMIO_INT_EN_CLR_EOC_INT_EN_CLR_BMSK                                          0x1
#define PMIO_INT_EN_CLR_EOC_INT_EN_CLR_SHFT                                          0x0
#define PMIO_INT_EN_CLR_EOC_INT_EN_CLR_EOC_INT_DISABLED_FVAL                         0x0
#define PMIO_INT_EN_CLR_EOC_INT_EN_CLR_EOC_INT_ENBLED_FVAL                           0x1

#define PMIO_INT_LATCHED_STS_ADDR(x)                                          ((x) + 0x00000018)
#define PMIO_INT_LATCHED_STS_RMSK                                                    0x1
#define PMIO_INT_LATCHED_STS_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_LATCHED_STS_ADDR(base), PMIO_INT_LATCHED_STS_RMSK, 0, val)
#define PMIO_INT_LATCHED_STS_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_LATCHED_STS_ADDR(base), mask, shift, val)
#define PMIO_INT_LATCHED_STS_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_LATCHED_STS_ADDR(base), val, len)
#define PMIO_INT_LATCHED_STS_EOC_INT_LATCHED_STS_BMSK                                0x1
#define PMIO_INT_LATCHED_STS_EOC_INT_LATCHED_STS_SHFT                                0x0
#define PMIO_INT_LATCHED_STS_EOC_INT_LATCHED_STS_EOC_INT_LATCHED_FALSE_FVAL          0x0
#define PMIO_INT_LATCHED_STS_EOC_INT_LATCHED_STS_EOC_INT_LATCHED_TRUE_FVAL           0x1

#define PMIO_INT_PENDING_STS_ADDR(x)                                          ((x) + 0x00000019)
#define PMIO_INT_PENDING_STS_RMSK                                                    0x1
#define PMIO_INT_PENDING_STS_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_PENDING_STS_ADDR(base), PMIO_INT_PENDING_STS_RMSK, 0, val)
#define PMIO_INT_PENDING_STS_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_PENDING_STS_ADDR(base), mask, shift, val)
#define PMIO_INT_PENDING_STS_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_PENDING_STS_ADDR(base), val, len)
#define PMIO_INT_PENDING_STS_EOC_INT_PENDING_STS_BMSK                                0x1
#define PMIO_INT_PENDING_STS_EOC_INT_PENDING_STS_SHFT                                0x0
#define PMIO_INT_PENDING_STS_EOC_INT_PENDING_STS_EOC_INT_PENDING_FALSE_FVAL          0x0
#define PMIO_INT_PENDING_STS_EOC_INT_PENDING_STS_EOC_INT_PENDING_TRUE_FVAL           0x1

#define PMIO_INT_MID_SEL_ADDR(x)                                              ((x) + 0x0000001a)
#define PMIO_INT_MID_SEL_RMSK                                                        0x3
#define PMIO_INT_MID_SEL_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_MID_SEL_ADDR(base), PMIO_INT_MID_SEL_RMSK, 0, val)
#define PMIO_INT_MID_SEL_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_MID_SEL_ADDR(base), mask, shift, val)
#define PMIO_INT_MID_SEL_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_MID_SEL_ADDR(base), val, len)
#define PMIO_INT_MID_SEL_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_MID_SEL_ADDR(base), val)
#define PMIO_INT_MID_SEL_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_MID_SEL_ADDR(base), mask, shift, val)
#define PMIO_INT_MID_SEL_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_MID_SEL_ADDR(base), val, len)
#define PMIO_INT_MID_SEL_INT_MID_SEL_BMSK                                            0x3
#define PMIO_INT_MID_SEL_INT_MID_SEL_SHFT                                            0x0

#define PMIO_INT_PRIORITY_ADDR(x)                                             ((x) + 0x0000001b)
#define PMIO_INT_PRIORITY_RMSK                                                       0x1
#define PMIO_INT_PRIORITY_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_PRIORITY_ADDR(base), PMIO_INT_PRIORITY_RMSK, 0, val)
#define PMIO_INT_PRIORITY_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_PRIORITY_ADDR(base), mask, shift, val)
#define PMIO_INT_PRIORITY_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_PRIORITY_ADDR(base), val, len)
#define PMIO_INT_PRIORITY_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_PRIORITY_ADDR(base), val)
#define PMIO_INT_PRIORITY_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_PRIORITY_ADDR(base), mask, shift, val)
#define PMIO_INT_PRIORITY_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_PRIORITY_ADDR(base), val, len)
#define PMIO_INT_PRIORITY_INT_PRIORITY_BMSK                                          0x1
#define PMIO_INT_PRIORITY_INT_PRIORITY_SHFT                                          0x0
#define PMIO_INT_PRIORITY_INT_PRIORITY_SR_FVAL                                       0x0
#define PMIO_INT_PRIORITY_INT_PRIORITY_A_FVAL                                        0x1

#define PMIO_ADC_DIG_PARAM_ADDR(x)                                            ((x) + 0x00000042)
#define PMIO_ADC_DIG_PARAM_RMSK                                                     0x7c
#define PMIO_ADC_DIG_PARAM_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_ADC_DIG_PARAM_ADDR(base), PMIO_ADC_DIG_PARAM_RMSK, 0, val)
#define PMIO_ADC_DIG_PARAM_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_ADC_DIG_PARAM_ADDR(base), mask, shift, val)
#define PMIO_ADC_DIG_PARAM_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_ADC_DIG_PARAM_ADDR(base), val, len)
#define PMIO_ADC_DIG_PARAM_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_ADC_DIG_PARAM_ADDR(base), val)
#define PMIO_ADC_DIG_PARAM_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_ADC_DIG_PARAM_ADDR(base), mask, shift, val)
#define PMIO_ADC_DIG_PARAM_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_ADC_DIG_PARAM_ADDR(base), val, len)
#define PMIO_ADC_DIG_PARAM_CAL_VAL_BMSK                                             0x40
#define PMIO_ADC_DIG_PARAM_CAL_VAL_SHFT                                              0x6
#define PMIO_ADC_DIG_PARAM_CAL_VAL_TIMER_CAL_FVAL                                    0x0
#define PMIO_ADC_DIG_PARAM_CAL_VAL_NEW_CAL_FVAL                                      0x1
#define PMIO_ADC_DIG_PARAM_CAL_SEL_BMSK                                             0x30
#define PMIO_ADC_DIG_PARAM_CAL_SEL_SHFT                                              0x4
#define PMIO_ADC_DIG_PARAM_CAL_SEL_NO_CAL_FVAL                                       0x0
#define PMIO_ADC_DIG_PARAM_CAL_SEL_RATIO_CAL_FVAL                                    0x1
#define PMIO_ADC_DIG_PARAM_CAL_SEL_ABS_CAL_FVAL                                      0x2
#define PMIO_ADC_DIG_PARAM_DEC_RATIO_SEL_BMSK                                        0xc
#define PMIO_ADC_DIG_PARAM_DEC_RATIO_SEL_SHFT                                        0x2
#define PMIO_ADC_DIG_PARAM_DEC_RATIO_SEL_DEC_RATIO_SHORT_FVAL                        0x0
#define PMIO_ADC_DIG_PARAM_DEC_RATIO_SEL_DEC_RATIO_MEDIUM_FVAL                       0x1
#define PMIO_ADC_DIG_PARAM_DEC_RATIO_SEL_DEC_RATIO_LONG_FVAL                         0x2

#define PMIO_FAST_AVG_CTL_ADDR(x)                                             ((x) + 0x00000043)
#define PMIO_FAST_AVG_CTL_RMSK                                                      0x87
#define PMIO_FAST_AVG_CTL_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_FAST_AVG_CTL_ADDR(base), PMIO_FAST_AVG_CTL_RMSK, 0, val)
#define PMIO_FAST_AVG_CTL_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_FAST_AVG_CTL_ADDR(base), mask, shift, val)
#define PMIO_FAST_AVG_CTL_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_FAST_AVG_CTL_ADDR(base), val, len)
#define PMIO_FAST_AVG_CTL_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_FAST_AVG_CTL_ADDR(base), val)
#define PMIO_FAST_AVG_CTL_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_FAST_AVG_CTL_ADDR(base), mask, shift, val)
#define PMIO_FAST_AVG_CTL_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_FAST_AVG_CTL_ADDR(base), val, len)
#define PMIO_FAST_AVG_CTL_FAST_AVG_EN_BMSK                                          0x80
#define PMIO_FAST_AVG_CTL_FAST_AVG_EN_SHFT                                           0x7
#define PMIO_FAST_AVG_CTL_FAST_AVG_EN_FAST_AVG_DISABLED_FVAL                         0x0
#define PMIO_FAST_AVG_CTL_FAST_AVG_EN_FAST_AVG_ENABLED_FVAL                          0x1
#define PMIO_FAST_AVG_CTL_FAST_AVG_SAMPLES_BMSK                                      0x7
#define PMIO_FAST_AVG_CTL_FAST_AVG_SAMPLES_SHFT                                      0x0
#define PMIO_FAST_AVG_CTL_FAST_AVG_SAMPLES_AVG_1_SAMPLE_FVAL                         0x0
#define PMIO_FAST_AVG_CTL_FAST_AVG_SAMPLES_AVG_2_SAMPLES_FVAL                        0x1
#define PMIO_FAST_AVG_CTL_FAST_AVG_SAMPLES_AVG_4_SAMPLES_FVAL                        0x2
#define PMIO_FAST_AVG_CTL_FAST_AVG_SAMPLES_AVG_8_SAMPLES_FVAL                        0x3
#define PMIO_FAST_AVG_CTL_FAST_AVG_SAMPLES_AVG_16_SAMPLES_FVAL                       0x4

#define PMIO_ADC_CH_SEL_CTL_ADDR(x)                                           ((x) + 0x00000044)
#define PMIO_ADC_CH_SEL_CTL_RMSK                                                    0xff
#define PMIO_ADC_CH_SEL_CTL_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_ADC_CH_SEL_CTL_ADDR(base), PMIO_ADC_CH_SEL_CTL_RMSK, 0, val)
#define PMIO_ADC_CH_SEL_CTL_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_ADC_CH_SEL_CTL_ADDR(base), mask, shift, val)
#define PMIO_ADC_CH_SEL_CTL_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_ADC_CH_SEL_CTL_ADDR(base), val, len)
#define PMIO_ADC_CH_SEL_CTL_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_ADC_CH_SEL_CTL_ADDR(base), val)
#define PMIO_ADC_CH_SEL_CTL_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_ADC_CH_SEL_CTL_ADDR(base), mask, shift, val)
#define PMIO_ADC_CH_SEL_CTL_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_ADC_CH_SEL_CTL_ADDR(base), val, len)
#define PMIO_ADC_CH_SEL_CTL_ADC_CH_SEL_BMSK                                         0xff
#define PMIO_ADC_CH_SEL_CTL_ADC_CH_SEL_SHFT                                          0x0

#define PMIO_DELAY_CTL_ADDR(x)                                                ((x) + 0x00000045)
#define PMIO_DELAY_CTL_RMSK                                                          0xf
#define PMIO_DELAY_CTL_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_DELAY_CTL_ADDR(base), PMIO_DELAY_CTL_RMSK, 0, val)
#define PMIO_DELAY_CTL_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_DELAY_CTL_ADDR(base), mask, shift, val)
#define PMIO_DELAY_CTL_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_DELAY_CTL_ADDR(base), val, len)
#define PMIO_DELAY_CTL_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_DELAY_CTL_ADDR(base), val)
#define PMIO_DELAY_CTL_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_DELAY_CTL_ADDR(base), mask, shift, val)
#define PMIO_DELAY_CTL_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_DELAY_CTL_ADDR(base), val, len)
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_BMSK                                          0xf
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_SHFT                                          0x0
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_15US_FVAL                     0x0
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_100US_FVAL                    0x1
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_200US_FVAL                    0x2
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_300US_FVAL                    0x3
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_400US_FVAL                    0x4
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_500US_FVAL                    0x5
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_600US_FVAL                    0x6
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_700US_FVAL                    0x7
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_800US_FVAL                    0x8
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_900US_FVAL                    0x9
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_1MS_FVAL                      0xa
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_2MS_FVAL                      0xb
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_4MS_FVAL                      0xc
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_6MS_FVAL                      0xd
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_8MS_FVAL                      0xe
#define PMIO_DELAY_CTL_HW_SETTLE_DELAY_HW_SETTLE_DELAY_10MS_FVAL                     0xf

#define PMIO_EN_CTL1_ADDR(x)                                                  ((x) + 0x00000046)
#define PMIO_EN_CTL1_RMSK                                                           0x80
#define PMIO_EN_CTL1_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_EN_CTL1_ADDR(base), PMIO_EN_CTL1_RMSK, 0, val)
#define PMIO_EN_CTL1_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_EN_CTL1_ADDR(base), mask, shift, val)
#define PMIO_EN_CTL1_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_EN_CTL1_ADDR(base), val, len)
#define PMIO_EN_CTL1_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_EN_CTL1_ADDR(base), val)
#define PMIO_EN_CTL1_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_EN_CTL1_ADDR(base), mask, shift, val)
#define PMIO_EN_CTL1_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_EN_CTL1_ADDR(base), val, len)
#define PMIO_EN_CTL1_ADC_EN_BMSK                                                    0x80
#define PMIO_EN_CTL1_ADC_EN_SHFT                                                     0x7
#define PMIO_EN_CTL1_ADC_EN_ADC_DISABLED_FVAL                                        0x0
#define PMIO_EN_CTL1_ADC_EN_ADC_ENABLED_FVAL                                         0x1

#define PMIO_CONV_REQ_ADDR(x)                                                 ((x) + 0x00000047)
#define PMIO_CONV_REQ_RMSK                                                          0x80
#define PMIO_CONV_REQ_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_CONV_REQ_ADDR(base), val)
#define PMIO_CONV_REQ_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_CONV_REQ_ADDR(base), val, len)
#define PMIO_CONV_REQ_REQ_BMSK                                                      0x80
#define PMIO_CONV_REQ_REQ_SHFT                                                       0x7
#define PMIO_CONV_REQ_REQ_NO_OPERATION_FVAL                                          0x0
#define PMIO_CONV_REQ_REQ_START_CONV_REQ_FVAL                                        0x1

#define PMIO_DATA0_ADDR(x)                                                    ((x) + 0x00000050)
#define PMIO_DATA0_RMSK                                                             0xff
#define PMIO_DATA0_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_DATA0_ADDR(base), PMIO_DATA0_RMSK, 0, val)
#define PMIO_DATA0_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_DATA0_ADDR(base), mask, shift, val)
#define PMIO_DATA0_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_DATA0_ADDR(base), val, len)
#define PMIO_DATA0_DATA_7_0_BMSK                                                    0xff
#define PMIO_DATA0_DATA_7_0_SHFT                                                     0x0

#define PMIO_DATA1_ADDR(x)                                                    ((x) + 0x00000051)
#define PMIO_DATA1_RMSK                                                             0xff
#define PMIO_DATA1_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_DATA1_ADDR(base), PMIO_DATA1_RMSK, 0, val)
#define PMIO_DATA1_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_DATA1_ADDR(base), mask, shift, val)
#define PMIO_DATA1_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_DATA1_ADDR(base), val, len)
#define PMIO_DATA1_DATA_15_8_BMSK                                                   0xff
#define PMIO_DATA1_DATA_15_8_SHFT                                                    0x0

#define PMIO_IBAT_DATA0_n_ADDR(base, n)                                       ((base) + (0x00000052 + 0x1 * (n)))
#define PMIO_IBAT_DATA0_n_RMSK                                                      0xff
#define PMIO_IBAT_DATA0_n_INI(ctxt, pmic, base, n, val)\
        in_pmio_field(ctxt, pmic, PMIO_IBAT_DATA0_n_ADDR(base, n), PMIO_IBAT_DATA0_n_RMSK, 0, val)
#define PMIO_IBAT_DATA0_n_INFI(ctxt, pmic, base, n, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_IBAT_DATA0_n_ADDR(base, n), mask, shift, val)
#define PMIO_IBAT_DATA0_n_INNI(ctxt, pmic, base, n, val, len)\
        in_pmio(ctxt, pmic, PMIO_IBAT_DATA0_n_ADDR(base, n), val, len)
#define PMIO_IBAT_DATA0_n_DATA_7_0_BMSK                                             0xff
#define PMIO_IBAT_DATA0_n_DATA_7_0_SHFT                                              0x0

#define PMIO_IBAT_DATA1_n_ADDR(base, n)                                       ((base) + (0x00000053 + 0x1 * (n)))
#define PMIO_IBAT_DATA1_n_RMSK                                                      0xff
#define PMIO_IBAT_DATA1_n_INI(ctxt, pmic, base, n, val)\
        in_pmio_field(ctxt, pmic, PMIO_IBAT_DATA1_n_ADDR(base, n), PMIO_IBAT_DATA1_n_RMSK, 0, val)
#define PMIO_IBAT_DATA1_n_INFI(ctxt, pmic, base, n, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_IBAT_DATA1_n_ADDR(base, n), mask, shift, val)
#define PMIO_IBAT_DATA1_n_INNI(ctxt, pmic, base, n, val, len)\
        in_pmio(ctxt, pmic, PMIO_IBAT_DATA1_n_ADDR(base, n), val, len)
#define PMIO_IBAT_DATA1_n_DATA_15_8_BMSK                                            0xff
#define PMIO_IBAT_DATA1_n_DATA_15_8_SHFT                                             0x0

#define PMIO_PERPH_RESET_CTL1_ADDR(x)                                         ((x) + 0x000000d8)
#define PMIO_PERPH_RESET_CTL1_RMSK                                                   0x1
#define PMIO_PERPH_RESET_CTL1_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL1_ADDR(base), PMIO_PERPH_RESET_CTL1_RMSK, 0, val)
#define PMIO_PERPH_RESET_CTL1_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL1_ADDR(base), mask, shift, val)
#define PMIO_PERPH_RESET_CTL1_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_PERPH_RESET_CTL1_ADDR(base), val, len)
#define PMIO_PERPH_RESET_CTL1_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_PERPH_RESET_CTL1_ADDR(base), val)
#define PMIO_PERPH_RESET_CTL1_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL1_ADDR(base), mask, shift, val)
#define PMIO_PERPH_RESET_CTL1_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PERPH_RESET_CTL1_ADDR(base), val, len)
#define PMIO_PERPH_RESET_CTL1_ENABLE_PRESET_BMSK                                     0x1
#define PMIO_PERPH_RESET_CTL1_ENABLE_PRESET_SHFT                                     0x0
#define PMIO_PERPH_RESET_CTL1_ENABLE_PRESET_MODULE_DIS_AFTER_WARM_SOFT_RESET_FVAL        0x0
#define PMIO_PERPH_RESET_CTL1_ENABLE_PRESET_MODULE_EN_AFTER_WARM_SOFT_RESET_FVAL        0x1

#define PMIO_PERPH_RESET_CTL2_ADDR(x)                                         ((x) + 0x000000d9)
#define PMIO_PERPH_RESET_CTL2_RMSK                                                   0x1
#define PMIO_PERPH_RESET_CTL2_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL2_ADDR(base), PMIO_PERPH_RESET_CTL2_RMSK, 0, val)
#define PMIO_PERPH_RESET_CTL2_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL2_ADDR(base), mask, shift, val)
#define PMIO_PERPH_RESET_CTL2_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_PERPH_RESET_CTL2_ADDR(base), val, len)
#define PMIO_PERPH_RESET_CTL2_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_PERPH_RESET_CTL2_ADDR(base), val)
#define PMIO_PERPH_RESET_CTL2_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL2_ADDR(base), mask, shift, val)
#define PMIO_PERPH_RESET_CTL2_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PERPH_RESET_CTL2_ADDR(base), val, len)
#define PMIO_PERPH_RESET_CTL2_FOLLOW_GLOBAL_SOFT_RB_BMSK                             0x1
#define PMIO_PERPH_RESET_CTL2_FOLLOW_GLOBAL_SOFT_RB_SHFT                             0x0
#define PMIO_PERPH_RESET_CTL2_FOLLOW_GLOBAL_SOFT_RB_FOLLOW_GLOBAL_SOFT_RESET_FALSE_FVAL        0x0
#define PMIO_PERPH_RESET_CTL2_FOLLOW_GLOBAL_SOFT_RB_FOLLOW_GLOBAL_SOFT_RESET_TRUE_FVAL        0x1

#define PMIO_PERPH_RESET_CTL3_ADDR(x)                                         ((x) + 0x000000da)
#define PMIO_PERPH_RESET_CTL3_RMSK                                                   0x7
#define PMIO_PERPH_RESET_CTL3_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL3_ADDR(base), PMIO_PERPH_RESET_CTL3_RMSK, 0, val)
#define PMIO_PERPH_RESET_CTL3_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL3_ADDR(base), mask, shift, val)
#define PMIO_PERPH_RESET_CTL3_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_PERPH_RESET_CTL3_ADDR(base), val, len)
#define PMIO_PERPH_RESET_CTL3_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_PERPH_RESET_CTL3_ADDR(base), val)
#define PMIO_PERPH_RESET_CTL3_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL3_ADDR(base), mask, shift, val)
#define PMIO_PERPH_RESET_CTL3_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PERPH_RESET_CTL3_ADDR(base), val, len)
#define PMIO_PERPH_RESET_CTL3_FOLLOW_WARM_RB_BMSK                                    0x4
#define PMIO_PERPH_RESET_CTL3_FOLLOW_WARM_RB_SHFT                                    0x2
#define PMIO_PERPH_RESET_CTL3_FOLLOW_WARM_RB_FOLLOW_WARM_RB_FALSE_FVAL               0x0
#define PMIO_PERPH_RESET_CTL3_FOLLOW_WARM_RB_FOLLOW_WARM_RB_TRUE_FVAL                0x1
#define PMIO_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN1_RB_BMSK                               0x2
#define PMIO_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN1_RB_SHFT                               0x1
#define PMIO_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN1_RB_FOLLOW_SHUTDOWN1_RB_FALSE_FVAL        0x0
#define PMIO_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN1_RB_FOLLOW_SHUTDOWN1_RB_TRUE_FVAL        0x1
#define PMIO_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN2_RB_BMSK                               0x1
#define PMIO_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN2_RB_SHFT                               0x0
#define PMIO_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN2_RB_FOLLOW_SHUTDOWN2_RB_FALSE_FVAL        0x0
#define PMIO_PERPH_RESET_CTL3_FOLLOW_SHUTDOWN2_RB_FOLLOW_SHUTDOWN2_RB_TRUE_FVAL        0x1

#define PMIO_PERPH_RESET_CTL4_ADDR(x)                                         ((x) + 0x000000db)
#define PMIO_PERPH_RESET_CTL4_RMSK                                                   0x1
#define PMIO_PERPH_RESET_CTL4_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL4_ADDR(base), PMIO_PERPH_RESET_CTL4_RMSK, 0, val)
#define PMIO_PERPH_RESET_CTL4_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL4_ADDR(base), mask, shift, val)
#define PMIO_PERPH_RESET_CTL4_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_PERPH_RESET_CTL4_ADDR(base), val, len)
#define PMIO_PERPH_RESET_CTL4_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_PERPH_RESET_CTL4_ADDR(base), val)
#define PMIO_PERPH_RESET_CTL4_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PERPH_RESET_CTL4_ADDR(base), mask, shift, val)
#define PMIO_PERPH_RESET_CTL4_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PERPH_RESET_CTL4_ADDR(base), val, len)
#define PMIO_PERPH_RESET_CTL4_LOCAL_SOFT_RESET_BMSK                                  0x1
#define PMIO_PERPH_RESET_CTL4_LOCAL_SOFT_RESET_SHFT                                  0x0
#define PMIO_PERPH_RESET_CTL4_LOCAL_SOFT_RESET_LOCAL_SOFT_RESET_FALSE_FVAL           0x0
#define PMIO_PERPH_RESET_CTL4_LOCAL_SOFT_RESET_LOCAL_SOFT_RESET_TRUE_FVAL            0x1

#define PMIO_INT_TEST1_ADDR(x)                                                ((x) + 0x000000e0)
#define PMIO_INT_TEST1_RMSK                                                         0x80
#define PMIO_INT_TEST1_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_TEST1_ADDR(base), PMIO_INT_TEST1_RMSK, 0, val)
#define PMIO_INT_TEST1_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_TEST1_ADDR(base), mask, shift, val)
#define PMIO_INT_TEST1_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_TEST1_ADDR(base), val, len)
#define PMIO_INT_TEST1_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_TEST1_ADDR(base), val)
#define PMIO_INT_TEST1_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_TEST1_ADDR(base), mask, shift, val)
#define PMIO_INT_TEST1_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_TEST1_ADDR(base), val, len)
#define PMIO_INT_TEST1_INT_TEST_MODE_EN_BMSK                                        0x80
#define PMIO_INT_TEST1_INT_TEST_MODE_EN_SHFT                                         0x7
#define PMIO_INT_TEST1_INT_TEST_MODE_EN_INT_TEST_MODE_FALSE_FVAL                     0x0
#define PMIO_INT_TEST1_INT_TEST_MODE_EN_INT_TEST_MODE_TRUE_FVAL                      0x1

#define PMIO_INT_TEST_VAL_ADDR(x)                                             ((x) + 0x000000e1)
#define PMIO_INT_TEST_VAL_RMSK                                                       0x1
#define PMIO_INT_TEST_VAL_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_TEST_VAL_ADDR(base), PMIO_INT_TEST_VAL_RMSK, 0, val)
#define PMIO_INT_TEST_VAL_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_INT_TEST_VAL_ADDR(base), mask, shift, val)
#define PMIO_INT_TEST_VAL_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_INT_TEST_VAL_ADDR(base), val, len)
#define PMIO_INT_TEST_VAL_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_INT_TEST_VAL_ADDR(base), val)
#define PMIO_INT_TEST_VAL_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_INT_TEST_VAL_ADDR(base), mask, shift, val)
#define PMIO_INT_TEST_VAL_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_INT_TEST_VAL_ADDR(base), val, len)
#define PMIO_INT_TEST_VAL_EOC_INT_TEST_VAL_BMSK                                      0x1
#define PMIO_INT_TEST_VAL_EOC_INT_TEST_VAL_SHFT                                      0x0
#define PMIO_INT_TEST_VAL_EOC_INT_TEST_VAL_EOC_TEST_FALSE_FVAL                       0x0
#define PMIO_INT_TEST_VAL_EOC_INT_TEST_VAL_EOC_TEST_TRUE_FVAL                        0x1

#define PMIO_TEST1_ADDR(x)                                                    ((x) + 0x000000e2)
#define PMIO_TEST1_RMSK                                                              0x3
#define PMIO_TEST1_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_TEST1_ADDR(base), PMIO_TEST1_RMSK, 0, val)
#define PMIO_TEST1_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_TEST1_ADDR(base), mask, shift, val)
#define PMIO_TEST1_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_TEST1_ADDR(base), val, len)
#define PMIO_TEST1_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_TEST1_ADDR(base), val)
#define PMIO_TEST1_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_TEST1_ADDR(base), mask, shift, val)
#define PMIO_TEST1_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_TEST1_ADDR(base), val, len)
#define PMIO_TEST1_DTEST_SEL_BMSK                                                    0x3
#define PMIO_TEST1_DTEST_SEL_SHFT                                                    0x0

#define PMIO_TEST2_ADDR(x)                                                    ((x) + 0x000000e3)
#define PMIO_TEST2_RMSK                                                              0x1
#define PMIO_TEST2_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_TEST2_ADDR(base), PMIO_TEST2_RMSK, 0, val)
#define PMIO_TEST2_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_TEST2_ADDR(base), mask, shift, val)
#define PMIO_TEST2_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_TEST2_ADDR(base), val, len)
#define PMIO_TEST2_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_TEST2_ADDR(base), val)
#define PMIO_TEST2_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_TEST2_ADDR(base), mask, shift, val)
#define PMIO_TEST2_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_TEST2_ADDR(base), val, len)
#define PMIO_TEST2_SNIFF_EN_BMSK                                                     0x1
#define PMIO_TEST2_SNIFF_EN_SHFT                                                     0x0
#define PMIO_TEST2_SNIFF_EN_SNIFF_DIS_FVAL                                           0x0
#define PMIO_TEST2_SNIFF_EN_SNIFF_EN_FVAL                                            0x1

#define PMIO_PBUS_WRITE_SYNC_CTL_ADDR(x)                                      ((x) + 0x000000e4)
#define PMIO_PBUS_WRITE_SYNC_CTL_RMSK                                                0x6
#define PMIO_PBUS_WRITE_SYNC_CTL_IN(ctxt, pmic, base, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBUS_WRITE_SYNC_CTL_ADDR(base), PMIO_PBUS_WRITE_SYNC_CTL_RMSK, 0, val)
#define PMIO_PBUS_WRITE_SYNC_CTL_INF(ctxt, pmic, base, mask, shift, val)\
        in_pmio_field(ctxt, pmic, PMIO_PBUS_WRITE_SYNC_CTL_ADDR(base), mask, shift, val)
#define PMIO_PBUS_WRITE_SYNC_CTL_INN(ctxt, pmic, base, val, len)\
        in_pmio(ctxt, pmic, PMIO_PBUS_WRITE_SYNC_CTL_ADDR(base), val, len)
#define PMIO_PBUS_WRITE_SYNC_CTL_OUT(ctxt, pmic, base, val)\
        out_pmio(ctxt, pmic, PMIO_PBUS_WRITE_SYNC_CTL_ADDR(base), val)
#define PMIO_PBUS_WRITE_SYNC_CTL_OUTF(ctxt, pmic, base, mask, shift, val)\
        out_pmio_field(ctxt, pmic, PMIO_PBUS_WRITE_SYNC_CTL_ADDR(base), mask, shift, val)
#define PMIO_PBUS_WRITE_SYNC_CTL_OUTN(ctxt, pmic, base, val, len)\
        out_pmio_array(ctxt, pmic, PMIO_PBUS_WRITE_SYNC_CTL_ADDR(base), val, len)
#define PMIO_PBUS_WRITE_SYNC_CTL_SW_CLK_REQ_BMSK                                     0x4
#define PMIO_PBUS_WRITE_SYNC_CTL_SW_CLK_REQ_SHFT                                     0x2
#define PMIO_PBUS_WRITE_SYNC_CTL_SW_CLK_REQ_SW_CLK_REQ_CLR_FVAL                      0x0
#define PMIO_PBUS_WRITE_SYNC_CTL_SW_CLK_REQ_SW_CLK_REQ_SET_FVAL                      0x1
#define PMIO_PBUS_WRITE_SYNC_CTL_SW_CLK_REQ_MODE_BMSK                                0x2
#define PMIO_PBUS_WRITE_SYNC_CTL_SW_CLK_REQ_MODE_SHFT                                0x1
#define PMIO_PBUS_WRITE_SYNC_CTL_SW_CLK_REQ_MODE_HW_CLK_REQ_MODE_FVAL                0x0
#define PMIO_PBUS_WRITE_SYNC_CTL_SW_CLK_REQ_MODE_SW_CLK_REQ_MODE_FVAL                0x1


#endif /* __VADCHALPMIO_H__ */
