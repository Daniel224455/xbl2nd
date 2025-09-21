#ifndef _DP_FREQTABLE_H_
#define _DP_FREQTABLE_H_
/*! \file */
/*=============================================================================

 File: dp_freqtable.h
 
    Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
    Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/

#include "dp_types.h"

/* --------------------------------------------------------------------------------------------------------------------
  Defines
---------------------------------------------------------------------------------------------------------------------*/

/* 
 * DP MND Frequency table look up
 */
typedef struct _DPFreqTableType
{
  uint32  uReqFreq;      /* Requested frequency in Hz */
  uint8   uPreDiv;       /* Pre-divider value */
  uint8   uMVal;         /* Divider M val */
  uint16  uNVal;         /* Divider N val */
  uint8   u2DVal;        /* Diver 2*D val */
} DPFreqTableType;


/* MND lookup tables for 810Mhz, range from 25Mhz (VGA) -> 270Mhz (WQXGA reduced blanking)
 */
const DPFreqTableType aDPFreqTable810Mhz[] = {
  { 25000000, 1,   5, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=25.00Mhz ActFreq=25.00Mhz Error=0.000Mhz
  { 25500000, 1,   4, 127, 127}, // SrcFreq=810.00Mhz ReqFreq=25.50Mhz ActFreq=25.50Mhz Error=0.000Mhz
  { 26000000, 1,   7, 218, 218}, // SrcFreq=810.00Mhz ReqFreq=26.00Mhz ActFreq=26.01Mhz Error=0.009Mhz
  { 26500000, 1,   7, 214, 214}, // SrcFreq=810.00Mhz ReqFreq=26.50Mhz ActFreq=26.50Mhz Error=0.005Mhz
  { 27000000, 1,   1,  30,  30}, // SrcFreq=810.00Mhz ReqFreq=27.00Mhz ActFreq=27.00Mhz Error=0.000Mhz
  { 27500000, 1,   7, 206, 206}, // SrcFreq=810.00Mhz ReqFreq=27.50Mhz ActFreq=27.50Mhz Error=0.000Mhz
  { 28000000, 1,   9, 260, 255}, // SrcFreq=810.00Mhz ReqFreq=28.00Mhz ActFreq=28.00Mhz Error=0.005Mhz
  { 28500000, 1,   7, 199, 199}, // SrcFreq=810.00Mhz ReqFreq=28.50Mhz ActFreq=28.50Mhz Error=0.000Mhz
  { 29000000, 1,   9, 251, 251}, // SrcFreq=810.00Mhz ReqFreq=29.00Mhz ActFreq=29.04Mhz Error=0.038Mhz
  { 29500000, 1,   9, 247, 247}, // SrcFreq=810.00Mhz ReqFreq=29.50Mhz ActFreq=29.50Mhz Error=0.003Mhz
  { 30000000, 1,   1,  27,  27}, // SrcFreq=810.00Mhz ReqFreq=30.00Mhz ActFreq=30.00Mhz Error=0.000Mhz
  { 30500000, 1,   9, 239, 239}, // SrcFreq=810.00Mhz ReqFreq=30.50Mhz ActFreq=30.50Mhz Error=0.002Mhz
  { 31000000, 1,   8, 209, 209}, // SrcFreq=810.00Mhz ReqFreq=31.00Mhz ActFreq=31.00Mhz Error=0.005Mhz
  { 31500000, 1,   7, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=31.50Mhz ActFreq=31.50Mhz Error=0.000Mhz
  { 32000000, 1,  10, 253, 253}, // SrcFreq=810.00Mhz ReqFreq=32.00Mhz ActFreq=32.02Mhz Error=0.016Mhz
  { 32500000, 1,  10, 249, 249}, // SrcFreq=810.00Mhz ReqFreq=32.50Mhz ActFreq=32.50Mhz Error=0.000Mhz
  { 33000000, 1,   9, 221, 221}, // SrcFreq=810.00Mhz ReqFreq=33.00Mhz ActFreq=33.00Mhz Error=0.000Mhz
  { 33500000, 1,  11, 266, 255}, // SrcFreq=810.00Mhz ReqFreq=33.50Mhz ActFreq=33.50Mhz Error=0.004Mhz
  { 34000000, 1,  11, 262, 255}, // SrcFreq=810.00Mhz ReqFreq=34.00Mhz ActFreq=34.00Mhz Error=0.004Mhz
  { 34500000, 1,   2,  47,  47}, // SrcFreq=810.00Mhz ReqFreq=34.50Mhz ActFreq=34.50Mhz Error=0.000Mhz
  { 35000000, 1,   7, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=35.00Mhz ActFreq=35.00Mhz Error=0.000Mhz
  { 35500000, 1,  11, 251, 251}, // SrcFreq=810.00Mhz ReqFreq=35.50Mhz ActFreq=35.50Mhz Error=0.002Mhz
  { 36000000, 1,   2,  45,  45}, // SrcFreq=810.00Mhz ReqFreq=36.00Mhz ActFreq=36.00Mhz Error=0.000Mhz
  { 36500000, 1,   5, 111, 111}, // SrcFreq=810.00Mhz ReqFreq=36.50Mhz ActFreq=36.50Mhz Error=0.002Mhz
  { 37000000, 1,   9, 197, 197}, // SrcFreq=810.00Mhz ReqFreq=37.00Mhz ActFreq=37.00Mhz Error=0.005Mhz
  { 37500000, 1,   5, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=37.50Mhz ActFreq=37.50Mhz Error=0.000Mhz
  { 38000000, 1,  10, 213, 213}, // SrcFreq=810.00Mhz ReqFreq=38.00Mhz ActFreq=38.01Mhz Error=0.014Mhz
  { 38500000, 1,   1,  21,  21}, // SrcFreq=810.00Mhz ReqFreq=38.50Mhz ActFreq=38.50Mhz Error=0.002Mhz
  { 39000000, 1,   9, 187, 187}, // SrcFreq=810.00Mhz ReqFreq=39.00Mhz ActFreq=39.00Mhz Error=0.000Mhz
  { 39500000, 1,   2,  41,  41}, // SrcFreq=810.00Mhz ReqFreq=39.50Mhz ActFreq=39.51Mhz Error=0.012Mhz
  { 40000000, 1,   4,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=40.00Mhz ActFreq=40.00Mhz Error=0.000Mhz
  { 40500000, 1,   1,  20,  20}, // SrcFreq=810.00Mhz ReqFreq=40.50Mhz ActFreq=40.50Mhz Error=0.000Mhz
  { 41000000, 1,   4,  79,  79}, // SrcFreq=810.00Mhz ReqFreq=41.00Mhz ActFreq=40.99Mhz Error=0.008Mhz
  { 41500000, 1,   2,  39,  39}, // SrcFreq=810.00Mhz ReqFreq=41.50Mhz ActFreq=41.50Mhz Error=0.002Mhz
  { 42000000, 1,   7, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=42.00Mhz ActFreq=42.00Mhz Error=0.000Mhz
  { 42500000, 1,  14, 267, 255}, // SrcFreq=810.00Mhz ReqFreq=42.50Mhz ActFreq=42.50Mhz Error=0.000Mhz
  { 43000000, 1,   6, 113, 113}, // SrcFreq=810.00Mhz ReqFreq=43.00Mhz ActFreq=43.01Mhz Error=0.009Mhz
  { 43500000, 1,   8, 149, 149}, // SrcFreq=810.00Mhz ReqFreq=43.50Mhz ActFreq=43.50Mhz Error=0.000Mhz
  { 44000000, 1,  12, 221, 221}, // SrcFreq=810.00Mhz ReqFreq=44.00Mhz ActFreq=43.99Mhz Error=0.011Mhz
  { 44500000, 1,   5,  91,  91}, // SrcFreq=810.00Mhz ReqFreq=44.50Mhz ActFreq=44.51Mhz Error=0.005Mhz
  { 45000000, 1,   1,  18,  18}, // SrcFreq=810.00Mhz ReqFreq=45.00Mhz ActFreq=45.00Mhz Error=0.000Mhz
  { 45500000, 1,   5,  89,  89}, // SrcFreq=810.00Mhz ReqFreq=45.50Mhz ActFreq=45.51Mhz Error=0.006Mhz
  { 46000000, 1,  13, 229, 229}, // SrcFreq=810.00Mhz ReqFreq=46.00Mhz ActFreq=45.99Mhz Error=0.011Mhz
  { 46500000, 1,  12, 209, 209}, // SrcFreq=810.00Mhz ReqFreq=46.50Mhz ActFreq=46.50Mhz Error=0.000Mhz
  { 47000000, 1,  13, 224, 224}, // SrcFreq=810.00Mhz ReqFreq=47.00Mhz ActFreq=47.01Mhz Error=0.009Mhz
  { 47500000, 1,  15, 256, 255}, // SrcFreq=810.00Mhz ReqFreq=47.50Mhz ActFreq=47.50Mhz Error=0.000Mhz
  { 48000000, 1,   8, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=48.00Mhz ActFreq=48.00Mhz Error=0.000Mhz
  { 48500000, 1,  10, 167, 167}, // SrcFreq=810.00Mhz ReqFreq=48.50Mhz ActFreq=48.50Mhz Error=0.003Mhz
  { 49000000, 1,  15, 248, 248}, // SrcFreq=810.00Mhz ReqFreq=49.00Mhz ActFreq=49.00Mhz Error=0.004Mhz
  { 49500000, 1,  11, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=49.50Mhz ActFreq=49.50Mhz Error=0.000Mhz
  { 50000000, 1,   5,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=50.00Mhz ActFreq=50.00Mhz Error=0.000Mhz
  { 50500000, 1,  16, 257, 255}, // SrcFreq=810.00Mhz ReqFreq=50.50Mhz ActFreq=50.45Mhz Error=0.050Mhz
  { 51000000, 1,  17, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=51.00Mhz ActFreq=51.00Mhz Error=0.000Mhz
  { 51500000, 1,  11, 173, 173}, // SrcFreq=810.00Mhz ReqFreq=51.50Mhz ActFreq=51.50Mhz Error=0.003Mhz
  { 52000000, 1,   7, 109, 109}, // SrcFreq=810.00Mhz ReqFreq=52.00Mhz ActFreq=52.00Mhz Error=0.004Mhz
  { 52500000, 1,   7, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=52.50Mhz ActFreq=52.50Mhz Error=0.000Mhz
  { 53000000, 1,   7, 107, 107}, // SrcFreq=810.00Mhz ReqFreq=53.00Mhz ActFreq=52.99Mhz Error=0.009Mhz
  { 53500000, 1,   7, 106, 106}, // SrcFreq=810.00Mhz ReqFreq=53.50Mhz ActFreq=53.49Mhz Error=0.009Mhz
  { 54000000, 1,   1,  15,  15}, // SrcFreq=810.00Mhz ReqFreq=54.00Mhz ActFreq=54.00Mhz Error=0.000Mhz
  { 54500000, 1,  15, 223, 223}, // SrcFreq=810.00Mhz ReqFreq=54.50Mhz ActFreq=54.49Mhz Error=0.009Mhz
  { 55000000, 1,  11, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=55.00Mhz ActFreq=55.00Mhz Error=0.000Mhz
  { 55500000, 1,   5,  73,  73}, // SrcFreq=810.00Mhz ReqFreq=55.50Mhz ActFreq=55.50Mhz Error=0.000Mhz
  { 56000000, 1,  15, 217, 217}, // SrcFreq=810.00Mhz ReqFreq=56.00Mhz ActFreq=55.99Mhz Error=0.009Mhz
  { 56500000, 1,   3,  43,  43}, // SrcFreq=810.00Mhz ReqFreq=56.50Mhz ActFreq=56.51Mhz Error=0.012Mhz
  { 57000000, 1,  19, 170, 255}, // SrcFreq=810.00Mhz ReqFreq=57.00Mhz ActFreq=57.00Mhz Error=0.000Mhz
  { 57500000, 1,  12, 169, 169}, // SrcFreq=810.00Mhz ReqFreq=57.50Mhz ActFreq=57.50Mhz Error=0.000Mhz
  { 58000000, 1,  19, 265, 255}, // SrcFreq=810.00Mhz ReqFreq=58.00Mhz ActFreq=58.05Mhz Error=0.055Mhz
  { 58500000, 1,  13, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=58.50Mhz ActFreq=58.50Mhz Error=0.000Mhz
  { 59000000, 1,  11, 151, 151}, // SrcFreq=810.00Mhz ReqFreq=59.00Mhz ActFreq=59.00Mhz Error=0.004Mhz
  { 59500000, 1,  13, 177, 177}, // SrcFreq=810.00Mhz ReqFreq=59.50Mhz ActFreq=59.50Mhz Error=0.002Mhz
  { 60000000, 1,   2,  27,  27}, // SrcFreq=810.00Mhz ReqFreq=60.00Mhz ActFreq=60.00Mhz Error=0.000Mhz
  { 60500000, 1,  18, 241, 241}, // SrcFreq=810.00Mhz ReqFreq=60.50Mhz ActFreq=60.50Mhz Error=0.002Mhz
  { 61000000, 1,  18, 239, 239}, // SrcFreq=810.00Mhz ReqFreq=61.00Mhz ActFreq=61.00Mhz Error=0.004Mhz
  { 61500000, 1,  6,   79,  79}, // SrcFreq=810.00Mhz ReqFreq=61.50Mhz ActFreq=61.50Mhz Error=0.000Mhz
  { 62000000, 1,  16, 209, 209}, // SrcFreq=810.00Mhz ReqFreq=62.00Mhz ActFreq=62.01Mhz Error=0.010Mhz
  { 62500000, 1,  21, 272, 255}, // SrcFreq=810.00Mhz ReqFreq=62.50Mhz ActFreq=62.50Mhz Error=0.000Mhz
  { 63000000, 1,   7,  90,  90}, // SrcFreq=810.00Mhz ReqFreq=63.00Mhz ActFreq=63.00Mhz Error=0.000Mhz
  { 63500000, 1,   4,  51,  51}, // SrcFreq=810.00Mhz ReqFreq=63.50Mhz ActFreq=63.50Mhz Error=0.002Mhz
  { 64000000, 1,  20, 253, 253}, // SrcFreq=810.00Mhz ReqFreq=64.00Mhz ActFreq=64.02Mhz Error=0.021Mhz
  { 64500000, 1,  9,  113, 113}, // SrcFreq=810.00Mhz ReqFreq=64.50Mhz ActFreq=64.50Mhz Error=0.000Mhz
  { 65000000, 1,  13, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=65.00Mhz ActFreq=65.00Mhz Error=0.000Mhz
  { 65500000, 1,  19, 235, 235}, // SrcFreq=810.00Mhz ReqFreq=65.50Mhz ActFreq=65.50Mhz Error=0.005Mhz
  { 66000000, 1,  11, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=66.00Mhz ActFreq=66.00Mhz Error=0.000Mhz
  { 66500000, 1,  11, 134, 134}, // SrcFreq=810.00Mhz ReqFreq=66.50Mhz ActFreq=66.49Mhz Error=0.007Mhz
  { 67000000, 1,  11, 133, 133}, // SrcFreq=810.00Mhz ReqFreq=67.00Mhz ActFreq=67.00Mhz Error=0.004Mhz
  { 67500000, 1,   1,  12,  12}, // SrcFreq=810.00Mhz ReqFreq=67.50Mhz ActFreq=67.50Mhz Error=0.000Mhz
  { 68000000, 1,  23, 274, 255}, // SrcFreq=810.00Mhz ReqFreq=68.00Mhz ActFreq=68.00Mhz Error=0.004Mhz
  { 68500000, 1,  23, 272, 255}, // SrcFreq=810.00Mhz ReqFreq=68.50Mhz ActFreq=68.49Mhz Error=0.007Mhz
  { 69000000, 1,  23, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=69.00Mhz ActFreq=69.00Mhz Error=0.000Mhz
  { 69500000, 1,  23, 268, 255}, // SrcFreq=810.00Mhz ReqFreq=69.50Mhz ActFreq=69.50Mhz Error=0.003Mhz  
  { 70000000, 1,   7,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=70.00Mhz ActFreq=70.00Mhz Error=0.000Mhz
  { 70500000, 1,   2,  23,  23}, // SrcFreq=810.00Mhz ReqFreq=70.50Mhz ActFreq=70.50Mhz Error=0.000Mhz
  { 71000000, 1,  22, 251, 251}, // SrcFreq=810.00Mhz ReqFreq=71.00Mhz ActFreq=71.00Mhz Error=0.004Mhz
  { 71500000, 1,   3,  34,  34}, // SrcFreq=810.00Mhz ReqFreq=71.50Mhz ActFreq=71.47Mhz Error=0.029Mhz
  { 72000000, 1,   4,  45,  45}, // SrcFreq=810.00Mhz ReqFreq=72.00Mhz ActFreq=72.00Mhz Error=0.000Mhz
  { 72500000, 1,  23, 257, 255}, // SrcFreq=810.00Mhz ReqFreq=72.50Mhz ActFreq=72.50Mhz Error=0.000Mhz
  { 73000000, 1,  21, 233, 233}, // SrcFreq=810.00Mhz ReqFreq=73.00Mhz ActFreq=73.00Mhz Error=0.004Mhz
  { 73500000, 1,  25, 276, 255}, // SrcFreq=810.00Mhz ReqFreq=73.50Mhz ActFreq=73.50Mhz Error=0.000Mhz
  { 74000000, 1,  19, 208, 208}, // SrcFreq=810.00Mhz ReqFreq=74.00Mhz ActFreq=74.00Mhz Error=0.003Mhz
  { 74250000, 1,  11, 120, 120}, // SrcFreq=810.00Mhz ReqFreq=74.00Mhz ActFreq=74.01Mhz Error=0.013Mhz
  { 74500000, 1,   8,  87,  87}, // SrcFreq=810.00Mhz ReqFreq=74.50Mhz ActFreq=74.50Mhz Error=0.002Mhz
  { 75000000, 1,   5,  54,  54}, // SrcFreq=810.00Mhz ReqFreq=75.00Mhz ActFreq=75.00Mhz Error=0.000Mhz
  { 75500000, 1,  11, 118, 118}, // SrcFreq=810.00Mhz ReqFreq=75.50Mhz ActFreq=75.51Mhz Error=0.008Mhz
  { 76000000, 1,  26, 277, 255}, // SrcFreq=810.00Mhz ReqFreq=76.00Mhz ActFreq=76.02Mhz Error=0.019Mhz
  { 76500000, 1,  17, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=76.50Mhz ActFreq=76.50Mhz Error=0.000Mhz
  { 77000000, 1,  25, 263, 255}, // SrcFreq=810.00Mhz ReqFreq=77.00Mhz ActFreq=77.00Mhz Error=0.004Mhz
  { 77500000, 1,  20, 209, 209}, // SrcFreq=810.00Mhz ReqFreq=77.50Mhz ActFreq=77.50Mhz Error=0.000Mhz
  { 78000000, 1,  13, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=78.00Mhz ActFreq=78.00Mhz Error=0.000Mhz
  { 78500000, 1,  22, 227, 227}, // SrcFreq=810.00Mhz ReqFreq=78.50Mhz ActFreq=78.50Mhz Error=0.002Mhz
  { 79000000, 1,   4,  41,  41}, // SrcFreq=810.00Mhz ReqFreq=79.00Mhz ActFreq=78.99Mhz Error=0.011Mhz
  { 79500000, 1,  16, 163, 163}, // SrcFreq=810.00Mhz ReqFreq=79.50Mhz ActFreq=79.50Mhz Error=0.000Mhz
  { 80000000, 1,   8,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=80.00Mhz ActFreq=80.00Mhz Error=0.000Mhz
  { 80500000, 1,  16, 161, 161}, // SrcFreq=810.00Mhz ReqFreq=80.50Mhz ActFreq=80.50Mhz Error=0.003Mhz
  { 81000000, 1,   1,  10,  10}, // SrcFreq=810.00Mhz ReqFreq=81.00Mhz ActFreq=81.00Mhz Error=0.000Mhz
  { 81500000, 1,  16, 159, 159}, // SrcFreq=810.00Mhz ReqFreq=81.50Mhz ActFreq=81.49Mhz Error=0.006Mhz
  { 82000000, 1,  25, 247, 247}, // SrcFreq=810.00Mhz ReqFreq=82.00Mhz ActFreq=82.00Mhz Error=0.004Mhz
  { 82500000, 1,  11, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=82.50Mhz ActFreq=82.50Mhz Error=0.000Mhz
  { 83000000, 1,  29, 283, 255}, // SrcFreq=810.00Mhz ReqFreq=83.00Mhz ActFreq=83.00Mhz Error=0.004Mhz
  { 83500000, 1,  10,  97,  97}, // SrcFreq=810.00Mhz ReqFreq=83.50Mhz ActFreq=83.51Mhz Error=0.005Mhz
  { 84000000, 1,  14, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=84.00Mhz ActFreq=84.00Mhz Error=0.000Mhz
  { 84500000, 1,  29, 278, 255}, // SrcFreq=810.00Mhz ReqFreq=84.50Mhz ActFreq=84.50Mhz Error=0.004Mhz
  { 85000000, 1,  17, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=85.00Mhz ActFreq=85.00Mhz Error=0.000Mhz
  { 85500000, 1,  19, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=85.50Mhz ActFreq=85.50Mhz Error=0.000Mhz
  { 86000000, 1,  12, 113, 113}, // SrcFreq=810.00Mhz ReqFreq=86.00Mhz ActFreq=86.00Mhz Error=0.004Mhz
  { 86500000, 1,  11, 103, 103}, // SrcFreq=810.00Mhz ReqFreq=86.50Mhz ActFreq=86.50Mhz Error=0.005Mhz
  { 87000000, 1,  29, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=87.00Mhz ActFreq=87.00Mhz Error=0.000Mhz
  { 87500000, 1,  27, 250, 250}, // SrcFreq=810.00Mhz ReqFreq=87.50Mhz ActFreq=87.50Mhz Error=0.000Mhz
  { 88000000, 1,  29, 267, 255}, // SrcFreq=810.00Mhz ReqFreq=88.00Mhz ActFreq=87.99Mhz Error=0.013Mhz
  { 88500000, 1,  13, 119, 119}, // SrcFreq=810.00Mhz ReqFreq=88.50Mhz ActFreq=88.50Mhz Error=0.000Mhz
  { 89000000, 1,  10,  91,  91}, // SrcFreq=810.00Mhz ReqFreq=89.00Mhz ActFreq=89.00Mhz Error=0.003Mhz
  { 89500000, 1,  20, 181, 181}, // SrcFreq=810.00Mhz ReqFreq=89.50Mhz ActFreq=89.50Mhz Error=0.003Mhz  
  { 90000000, 1,   1,   9,   9}, // SrcFreq=810.00Mhz ReqFreq=90.00Mhz ActFreq=90.00Mhz Error=0.000Mhz
  { 90500000, 1,  20, 179, 179}, // SrcFreq=810.00Mhz ReqFreq=90.50Mhz ActFreq=90.50Mhz Error=0.003Mhz
  { 91000000, 1,  10,  89,  89}, // SrcFreq=810.00Mhz ReqFreq=91.00Mhz ActFreq=91.00Mhz Error=0.003Mhz
  { 91500000, 1,  27, 239, 239}, // SrcFreq=810.00Mhz ReqFreq=91.50Mhz ActFreq=91.50Mhz Error=0.000Mhz
  { 92000000, 1,  31, 273, 255}, // SrcFreq=810.00Mhz ReqFreq=92.00Mhz ActFreq=91.99Mhz Error=0.013Mhz
  { 92500000, 1,  29, 254, 254}, // SrcFreq=810.00Mhz ReqFreq=92.50Mhz ActFreq=92.50Mhz Error=0.000Mhz
  { 93000000, 1,  31, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=93.00Mhz ActFreq=93.00Mhz Error=0.000Mhz
  { 93500000, 1,   3,  26,  26}, // SrcFreq=810.00Mhz ReqFreq=93.50Mhz ActFreq=93.46Mhz Error=0.038Mhz
  { 94000000, 1,  13, 112, 112}, // SrcFreq=810.00Mhz ReqFreq=94.00Mhz ActFreq=93.99Mhz Error=0.007Mhz
  { 94500000, 1,   7,  60,  60}, // SrcFreq=810.00Mhz ReqFreq=94.50Mhz ActFreq=94.50Mhz Error=0.000Mhz
  { 95000000, 1,  19, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=95.00Mhz ActFreq=95.00Mhz Error=0.000Mhz
  { 95500000, 1,  27, 229, 229}, // SrcFreq=810.00Mhz ReqFreq=95.50Mhz ActFreq=95.50Mhz Error=0.002Mhz
  { 96000000, 1,  16, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=96.00Mhz ActFreq=96.00Mhz Error=0.000Mhz
  { 96500000, 1,  33, 277, 255}, // SrcFreq=810.00Mhz ReqFreq=96.50Mhz ActFreq=96.50Mhz Error=0.002Mhz
  { 97000000, 1,  20, 167, 167}, // SrcFreq=810.00Mhz ReqFreq=97.00Mhz ActFreq=97.00Mhz Error=0.004Mhz
  { 97500000, 1,  13, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=97.50Mhz ActFreq=97.50Mhz Error=0.000Mhz
  { 98000000, 1,  34, 281, 255}, // SrcFreq=810.00Mhz ReqFreq=98.00Mhz ActFreq=98.01Mhz Error=0.007Mhz
  { 98500000, 1,   9,  74,  74}, // SrcFreq=810.00Mhz ReqFreq=98.50Mhz ActFreq=98.51Mhz Error=0.014Mhz
  { 99000000, 1,  11,  90,  90}, // SrcFreq=810.00Mhz ReqFreq=99.00Mhz ActFreq=99.00Mhz Error=0.000Mhz
  { 99500000, 1,   7,  57,  57}, // SrcFreq=810.00Mhz ReqFreq=99.50Mhz ActFreq=99.50Mhz Error=0.002Mhz
  {100000000, 1,  10,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=100.00Mhz ActFreq=100.00Mhz Error=0.000Mhz
  {100500000, 1,  17, 137, 137}, // SrcFreq=810.00Mhz ReqFreq=100.50Mhz ActFreq=100.50Mhz Error=0.000Mhz
  {101000000, 1,  36, 289, 255}, // SrcFreq=810.00Mhz ReqFreq=101.00Mhz ActFreq=100.95Mhz Error=0.050Mhz
  {101500000, 1,  36, 287, 255}, // SrcFreq=810.00Mhz ReqFreq=101.50Mhz ActFreq=101.55Mhz Error=0.049Mhz
  {102000000, 1,  17, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=102.00Mhz ActFreq=102.00Mhz Error=0.000Mhz
  {102500000, 1,  31, 245, 245}, // SrcFreq=810.00Mhz ReqFreq=102.50Mhz ActFreq=102.50Mhz Error=0.000Mhz
  {103000000, 1,  22, 173, 173}, // SrcFreq=810.00Mhz ReqFreq=103.00Mhz ActFreq=103.00Mhz Error=0.004Mhz
  {103500000, 1,  23, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=103.50Mhz ActFreq=103.50Mhz Error=0.000Mhz
  {104000000, 1,  33, 257, 255}, // SrcFreq=810.00Mhz ReqFreq=104.00Mhz ActFreq=104.00Mhz Error=0.003Mhz
  {104500000, 1,   4,  31,  31}, // SrcFreq=810.00Mhz ReqFreq=104.50Mhz ActFreq=104.52Mhz Error=0.016Mhz
  {105000000, 1,   7,  54,  54}, // SrcFreq=810.00Mhz ReqFreq=105.00Mhz ActFreq=105.00Mhz Error=0.000Mhz
  {105500000, 1,  31, 238, 238}, // SrcFreq=810.00Mhz ReqFreq=105.50Mhz ActFreq=105.50Mhz Error=0.004Mhz
  {106000000, 1,  14, 107, 107}, // SrcFreq=810.00Mhz ReqFreq=106.00Mhz ActFreq=106.00Mhz Error=0.004Mhz
  {106500000, 1,  38, 289, 255}, // SrcFreq=810.00Mhz ReqFreq=106.50Mhz ActFreq=106.50Mhz Error=0.000Mhz
  {107000000, 1,   7,  53,  53}, // SrcFreq=810.00Mhz ReqFreq=107.00Mhz ActFreq=107.01Mhz Error=0.007Mhz
  {107500000, 1,  28, 211, 211}, // SrcFreq=810.00Mhz ReqFreq=107.50Mhz ActFreq=107.50Mhz Error=0.000Mhz
  {108000000, 1,   2,  15,  15}, // SrcFreq=810.00Mhz ReqFreq=108.00Mhz ActFreq=108.00Mhz Error=0.000Mhz
  {108500000, 1,  28, 209, 209}, // SrcFreq=810.00Mhz ReqFreq=108.50Mhz ActFreq=108.50Mhz Error=0.005Mhz
  {109000000, 1,  37, 275, 255}, // SrcFreq=810.00Mhz ReqFreq=109.00Mhz ActFreq=108.99Mhz Error=0.009Mhz
  {109500000, 1,  38, 281, 255}, // SrcFreq=810.00Mhz ReqFreq=109.50Mhz ActFreq=109.50Mhz Error=0.000Mhz
  {110000000, 1,  11,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=110.00Mhz ActFreq=110.00Mhz Error=0.000Mhz
  {110500000, 1,   3,  22,  22}, // SrcFreq=810.00Mhz ReqFreq=110.50Mhz ActFreq=110.45Mhz Error=0.045Mhz
  {111000000, 1,  37, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=111.00Mhz ActFreq=111.00Mhz Error=0.000Mhz
  {111500000, 1,  34, 247, 247}, // SrcFreq=810.00Mhz ReqFreq=111.50Mhz ActFreq=111.50Mhz Error=0.002Mhz
  {112000000, 1,  30, 217, 217}, // SrcFreq=810.00Mhz ReqFreq=112.00Mhz ActFreq=111.99Mhz Error=0.006Mhz
  {112500000, 1,   5,  36,  36}, // SrcFreq=810.00Mhz ReqFreq=112.50Mhz ActFreq=112.50Mhz Error=0.000Mhz
  {113000000, 1,   6,  43,  43}, // SrcFreq=810.00Mhz ReqFreq=113.00Mhz ActFreq=112.99Mhz Error=0.006Mhz
  {113500000, 1,  22, 157, 157}, // SrcFreq=810.00Mhz ReqFreq=113.50Mhz ActFreq=113.50Mhz Error=0.003Mhz
  {114000000, 1,  19, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=114.00Mhz ActFreq=114.00Mhz Error=0.000Mhz
  {114500000, 1,  27, 191, 191}, // SrcFreq=810.00Mhz ReqFreq=114.50Mhz ActFreq=114.50Mhz Error=0.003Mhz
  {115000000, 1,  23, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=115.00Mhz ActFreq=115.00Mhz Error=0.000Mhz
  {115500000, 1,  42, 295, 255}, // SrcFreq=810.00Mhz ReqFreq=115.50Mhz ActFreq=115.50Mhz Error=0.000Mhz
  {116000000, 1,  42, 293, 255}, // SrcFreq=810.00Mhz ReqFreq=116.00Mhz ActFreq=116.00Mhz Error=0.005Mhz
  {116500000, 1,  21, 146, 146}, // SrcFreq=810.00Mhz ReqFreq=116.50Mhz ActFreq=116.51Mhz Error=0.007Mhz
  {117000000, 1,  13,  90,  90}, // SrcFreq=810.00Mhz ReqFreq=117.00Mhz ActFreq=117.00Mhz Error=0.000Mhz
  {117500000, 1,  28, 193, 193}, // SrcFreq=810.00Mhz ReqFreq=117.50Mhz ActFreq=117.50Mhz Error=0.000Mhz
  {118000000, 1,  37, 254, 254}, // SrcFreq=810.00Mhz ReqFreq=118.00Mhz ActFreq=118.00Mhz Error=0.004Mhz
  {118500000, 1,  43, 294, 255}, // SrcFreq=810.00Mhz ReqFreq=118.50Mhz ActFreq=118.50Mhz Error=0.000Mhz
  {119000000, 1,  31, 211, 211}, // SrcFreq=810.00Mhz ReqFreq=119.00Mhz ActFreq=119.00Mhz Error=0.005Mhz
  {119500000, 1,   9,  61,  61}, // SrcFreq=810.00Mhz ReqFreq=119.50Mhz ActFreq=119.51Mhz Error=0.008Mhz
  {120000000, 1,   4,  27,  27}, // SrcFreq=810.00Mhz ReqFreq=120.00Mhz ActFreq=120.00Mhz Error=0.000Mhz
  {120500000, 1,  18, 121, 121}, // SrcFreq=810.00Mhz ReqFreq=120.50Mhz ActFreq=120.50Mhz Error=0.004Mhz
  {121000000, 1,  36, 241, 241}, // SrcFreq=810.00Mhz ReqFreq=121.00Mhz ActFreq=121.00Mhz Error=0.004Mhz
  {121500000, 1,   3,  20,  20}, // SrcFreq=810.00Mhz ReqFreq=121.50Mhz ActFreq=121.50Mhz Error=0.000Mhz
  {122000000, 1,  36, 239, 239}, // SrcFreq=810.00Mhz ReqFreq=122.00Mhz ActFreq=122.00Mhz Error=0.008Mhz
  {122500000, 1,  31, 205, 205}, // SrcFreq=810.00Mhz ReqFreq=122.50Mhz ActFreq=122.49Mhz Error=0.012Mhz
  {123000000, 1,  41, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=123.00Mhz ActFreq=123.00Mhz Error=0.000Mhz  
  {123500000, 1,  34, 223, 223}, // SrcFreq=810.00Mhz ReqFreq=123.50Mhz ActFreq=123.50Mhz Error=0.002Mhz  
  {124000000, 1,  32, 209, 209}, // SrcFreq=810.00Mhz ReqFreq=124.00Mhz ActFreq=124.02Mhz Error=0.019Mhz  
  {124500000, 1,  45, 293, 255}, // SrcFreq=810.00Mhz ReqFreq=124.50Mhz ActFreq=124.40Mhz Error=0.097Mhz  
  {125000000, 1,  25, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=125.00Mhz ActFreq=125.00Mhz Error=0.000Mhz  
  {125500000, 1,  11,  71,  71}, // SrcFreq=810.00Mhz ReqFreq=125.50Mhz ActFreq=125.49Mhz Error=0.007Mhz  
  {126000000, 1,   7,  45,  45}, // SrcFreq=810.00Mhz ReqFreq=126.00Mhz ActFreq=126.00Mhz Error=0.000Mhz  
  {126500000, 1,  47, 301, 255}, // SrcFreq=810.00Mhz ReqFreq=126.50Mhz ActFreq=126.48Mhz Error=0.022Mhz  
  {127000000, 1,  45, 287, 255}, // SrcFreq=810.00Mhz ReqFreq=127.00Mhz ActFreq=127.00Mhz Error=0.003Mhz  
  {127500000, 1,  17, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=127.50Mhz ActFreq=127.50Mhz Error=0.000Mhz  
  {128000000, 1,  46, 291, 255}, // SrcFreq=810.00Mhz ReqFreq=128.00Mhz ActFreq=128.04Mhz Error=0.041Mhz  
  {128500000, 1,  33, 208, 208}, // SrcFreq=810.00Mhz ReqFreq=128.50Mhz ActFreq=128.51Mhz Error=0.010Mhz  
  {129000000, 1,  43, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=129.00Mhz ActFreq=129.00Mhz Error=0.000Mhz  
  {129500000, 1,  47, 294, 255}, // SrcFreq=810.00Mhz ReqFreq=129.50Mhz ActFreq=129.49Mhz Error=0.010Mhz  
  {130000000, 1,  13,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=130.00Mhz ActFreq=130.00Mhz Error=0.000Mhz
  {130500000, 1,  29, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=130.50Mhz ActFreq=130.50Mhz Error=0.000Mhz  
  {131000000, 1,  49, 303, 255}, // SrcFreq=810.00Mhz ReqFreq=131.00Mhz ActFreq=130.99Mhz Error=0.010Mhz  
  {131500000, 1,  25, 154, 154}, // SrcFreq=810.00Mhz ReqFreq=131.50Mhz ActFreq=131.49Mhz Error=0.006Mhz  
  {132000000, 1,  22, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=132.00Mhz ActFreq=132.00Mhz Error=0.000Mhz  
  {132500000, 1,  44, 269, 255}, // SrcFreq=810.00Mhz ReqFreq=132.50Mhz ActFreq=132.49Mhz Error=0.009Mhz  
  {133000000, 1,  11,  67,  67}, // SrcFreq=810.00Mhz ReqFreq=133.00Mhz ActFreq=132.99Mhz Error=0.015Mhz  
  {133500000, 1,  15,  91,  91}, // SrcFreq=810.00Mhz ReqFreq=133.50Mhz ActFreq=133.52Mhz Error=0.016Mhz  
  {134000000, 1,  45, 272, 255}, // SrcFreq=810.00Mhz ReqFreq=134.00Mhz ActFreq=134.00Mhz Error=0.007Mhz  
  {134500000, 1,  45, 271, 255}, // SrcFreq=810.00Mhz ReqFreq=134.50Mhz ActFreq=134.50Mhz Error=0.002Mhz
  {135000000, 1,   1,   6,   6}, // SrcFreq=810.00Mhz ReqFreq=135.00Mhz ActFreq=135.00Mhz Error=0.000Mhz
  {135500000, 1,  45, 269, 255}, // SrcFreq=810.00Mhz ReqFreq=135.50Mhz ActFreq=135.50Mhz Error=0.002Mhz  
  {136000000, 1,  45, 268, 255}, // SrcFreq=810.00Mhz ReqFreq=136.00Mhz ActFreq=136.00Mhz Error=0.007Mhz  
  {136500000, 1,  46, 273, 255}, // SrcFreq=810.00Mhz ReqFreq=136.50Mhz ActFreq=136.48Mhz Error=0.016Mhz  
  {137000000, 1,  23, 136, 136}, // SrcFreq=810.00Mhz ReqFreq=137.00Mhz ActFreq=136.99Mhz Error=0.015Mhz  
  {137500000, 1,  46, 271, 255}, // SrcFreq=810.00Mhz ReqFreq=137.50Mhz ActFreq=137.49Mhz Error=0.009Mhz  
  {138000000, 1,  23, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=138.00Mhz ActFreq=138.00Mhz Error=0.000Mhz  
  {138500000, 1,  33, 193, 193}, // SrcFreq=810.00Mhz ReqFreq=138.50Mhz ActFreq=138.50Mhz Error=0.003Mhz  
  {139000000, 1,  29, 169, 169}, // SrcFreq=810.00Mhz ReqFreq=139.00Mhz ActFreq=138.99Mhz Error=0.006Mhz
  {139500000, 1,  31, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=139.50Mhz ActFreq=139.50Mhz Error=0.000Mhz
  {140000000, 1,  14,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=140.00Mhz ActFreq=140.00Mhz Error=0.000Mhz
  {140500000, 1,  17,  98,  98}, // SrcFreq=810.00Mhz ReqFreq=140.50Mhz ActFreq=140.51Mhz Error=0.010Mhz
  {141000000, 1,  47, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=141.00Mhz ActFreq=141.00Mhz Error=0.000Mhz
  {141500000, 1,  29, 166, 166}, // SrcFreq=810.00Mhz ReqFreq=141.50Mhz ActFreq=141.51Mhz Error=0.006Mhz
  {142000000, 1,  44, 251, 251}, // SrcFreq=810.00Mhz ReqFreq=142.00Mhz ActFreq=141.99Mhz Error=0.008Mhz  
  {142500000, 1,  19, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=142.50Mhz ActFreq=142.50Mhz Error=0.000Mhz
  {143000000, 1,   3,  17, 17},  // SrcFreq=810.00Mhz ReqFreq=143.00Mhz ActFreq=142.94Mhz Error=0.059Mhz
  {143500000, 1,  45, 254, 254}, // SrcFreq=810.00Mhz ReqFreq=143.50Mhz ActFreq=143.50Mhz Error=0.004Mhz  
  {144000000, 1,   8,  45,  45}, // SrcFreq=810.00Mhz ReqFreq=144.00Mhz ActFreq=144.00Mhz Error=0.000Mhz
  {144500000, 1,  38, 213, 213}, // SrcFreq=810.00Mhz ReqFreq=144.50Mhz ActFreq=144.50Mhz Error=0.007Mhz  
  {145000000, 1,  29, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=145.00Mhz ActFreq=145.00Mhz Error=0.000Mhz  
  {145500000, 1,  30, 167, 167}, // SrcFreq=810.00Mhz ReqFreq=145.50Mhz ActFreq=145.51Mhz Error=0.009Mhz  
  {146000000, 1,  42, 233, 233}, // SrcFreq=810.00Mhz ReqFreq=146.00Mhz ActFreq=146.00Mhz Error=0.009Mhz  
  {146500000, 1,  17,  94,  94}, // SrcFreq=810.00Mhz ReqFreq=146.50Mhz ActFreq=146.49Mhz Error=0.011Mhz
  {147000000, 1,  49, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=147.00Mhz ActFreq=147.00Mhz Error=0.000Mhz  
  {147500000, 1,  55, 302, 255}, // SrcFreq=810.00Mhz ReqFreq=147.50Mhz ActFreq=147.52Mhz Error=0.017Mhz  
  {148000000, 1,  55, 301, 255}, // SrcFreq=810.00Mhz ReqFreq=148.00Mhz ActFreq=148.00Mhz Error=0.007Mhz  
  {148500000, 1,  11,  60,  60}, // SrcFreq=810.00Mhz ReqFreq=148.50Mhz ActFreq=148.50Mhz Error=0.000Mhz
  {149000000, 1,  55, 299, 255}, // SrcFreq=810.00Mhz ReqFreq=149.00Mhz ActFreq=149.00Mhz Error=0.003Mhz
  {149500000, 1,  55, 298, 255}, // SrcFreq=810.00Mhz ReqFreq=149.50Mhz ActFreq=149.50Mhz Error=0.003Mhz
  {150000000, 1,   5,  27,  27}, // SrcFreq=810.00Mhz ReqFreq=150.00Mhz ActFreq=150.00Mhz Error=0.000Mhz
  {150500000, 1,  55, 296, 255}, // SrcFreq=810.00Mhz ReqFreq=150.50Mhz ActFreq=150.50Mhz Error=0.007Mhz  
  {151000000, 1,  11,  59,  59}, // SrcFreq=810.00Mhz ReqFreq=151.00Mhz ActFreq=151.02Mhz Error=0.017Mhz  
  {151500000, 1,  26, 139, 139}, // SrcFreq=810.00Mhz ReqFreq=151.50Mhz ActFreq=151.51Mhz Error=0.011Mhz  
  {152000000, 1,  58, 309, 255}, // SrcFreq=810.00Mhz ReqFreq=152.00Mhz ActFreq=152.00Mhz Error=0.000Mhz
  {152500000, 1,  45, 239, 239}, // SrcFreq=810.00Mhz ReqFreq=152.50Mhz ActFreq=152.50Mhz Error=0.000Mhz
  {153000000, 1,  17,  90,  90}, // SrcFreq=810.00Mhz ReqFreq=153.00Mhz ActFreq=153.00Mhz Error=0.000Mhz
  {153500000, 1,  47, 248, 248}, // SrcFreq=810.00Mhz ReqFreq=153.50Mhz ActFreq=153.50Mhz Error=0.001Mhz
  {154000000, 1,  50, 263, 255}, // SrcFreq=810.00Mhz ReqFreq=154.00Mhz ActFreq=154.00Mhz Error=0.000Mhz
  {154500000, 1,  33, 173, 173}, // SrcFreq=810.00Mhz ReqFreq=154.50Mhz ActFreq=154.50Mhz Error=0.000Mhz
  {155000000, 1,  31, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=155.00Mhz ActFreq=155.00Mhz Error=0.000Mhz  
  {155500000, 1, 43, 224, 224},  // SrcFreq=810.00Mhz ReqFreq=155.50Mhz ActFreq=155.50Mhz Error=0.001Mhz  
  {156000000, 1,  26, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=156.00Mhz ActFreq=156.00Mhz Error=0.000Mhz
  {156500000, 1,  57, 295, 255}, // SrcFreq=810.00Mhz ReqFreq=156.50Mhz ActFreq=156.50Mhz Error=0.001Mhz
  {157000000, 1,  44, 227, 227}, // SrcFreq=810.00Mhz ReqFreq=157.00Mhz ActFreq=157.00Mhz Error=0.000Mhz  
  {157500000, 1,   7,  36,  36}, // SrcFreq=810.00Mhz ReqFreq=157.50Mhz ActFreq=157.50Mhz Error=0.000Mhz
  {158000000, 1,  55, 282, 255}, // SrcFreq=810.00Mhz ReqFreq=158.00Mhz ActFreq=158.00Mhz Error=0.000Mhz
  {158500000, 1,   9,  46,  46}, // SrcFreq=810.00Mhz ReqFreq=158.50Mhz ActFreq=158.52Mhz Error=0.020Mhz
  {159000000, 1,  53, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=159.00Mhz ActFreq=159.00Mhz Error=0.000Mhz
  {159500000, 1,  51, 259, 255}, // SrcFreq=810.00Mhz ReqFreq=159.50Mhz ActFreq=159.50Mhz Error=0.002Mhz  
  {160000000, 1,  16,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=160.00Mhz ActFreq=160.00Mhz Error=0.000Mhz
  {160500000, 1,  43, 217, 217}, // SrcFreq=810.00Mhz ReqFreq=160.50Mhz ActFreq=160.50Mhz Error=0.000Mhz
  {161000000, 1,  32, 161, 161}, // SrcFreq=810.00Mhz ReqFreq=161.00Mhz ActFreq=161.00Mhz Error=0.000Mhz  
  {161500000, 1,  63, 316, 255}, // SrcFreq=810.00Mhz ReqFreq=161.50Mhz ActFreq=161.50Mhz Error=0.003Mhz
  {162000000, 1,   1,   5,   5}, // SrcFreq=810.00Mhz ReqFreq=162.00Mhz ActFreq=162.00Mhz Error=0.000Mhz
  {162500000, 1,  64, 319, 255}, // SrcFreq=810.00Mhz ReqFreq=162.50Mhz ActFreq=162.50Mhz Error=0.000Mhz
  {163000000, 1,  33, 164, 164}, // SrcFreq=810.00Mhz ReqFreq=163.00Mhz ActFreq=163.00Mhz Error=0.000Mhz
  {163500000, 1,  22, 109, 109}, // SrcFreq=810.00Mhz ReqFreq=163.50Mhz ActFreq=163.50Mhz Error=0.000Mhz
  {164000000, 1,  49, 242, 242}, // SrcFreq=810.00Mhz ReqFreq=164.00Mhz ActFreq=164.00Mhz Error=0.000Mhz  
  {164500000, 1,  53, 261, 255}, // SrcFreq=810.00Mhz ReqFreq=164.50Mhz ActFreq=164.50Mhz Error=0.001Mhz
  {165000000, 1,  11,  54,  54}, // SrcFreq=810.00Mhz ReqFreq=165.00Mhz ActFreq=165.00Mhz Error=0.000Mhz
  {165500000, 1,  19,  93,  93}, // SrcFreq=810.00Mhz ReqFreq=165.50Mhz ActFreq=165.50Mhz Error=0.002Mhz
  {166000000, 1,  58, 283, 255}, // SrcFreq=810.00Mhz ReqFreq=166.00Mhz ActFreq=166.00Mhz Error=0.000Mhz
  {166500000, 1,  37, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=166.50Mhz ActFreq=166.50Mhz Error=0.000Mhz
  {167000000, 1,  20,  97,  97}, // SrcFreq=810.00Mhz ReqFreq=167.00Mhz ActFreq=167.00Mhz Error=0.000Mhz  
  {167500000, 1,  61, 295, 255}, // SrcFreq=810.00Mhz ReqFreq=167.50Mhz ActFreq=167.50Mhz Error=0.000Mhz  
  {168000000, 1,  28, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=168.00Mhz ActFreq=168.00Mhz Error=0.000Mhz
  {168500000, 1,  57, 274, 255}, // SrcFreq=810.00Mhz ReqFreq=168.50Mhz ActFreq=168.50Mhz Error=0.004Mhz
  {169000000, 1,  29, 139, 139}, // SrcFreq=810.00Mhz ReqFreq=169.00Mhz ActFreq=169.00Mhz Error=0.000Mhz
  {169500000, 1,  59, 282, 255}, // SrcFreq=810.00Mhz ReqFreq=169.50Mhz ActFreq=169.50Mhz Error=0.000Mhz
  {170000000, 1,  17,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=170.00Mhz ActFreq=170.00Mhz Error=0.000Mhz
  {170500000, 1,   4,  19,  19}, // SrcFreq=810.00Mhz ReqFreq=170.50Mhz ActFreq=170.48Mhz Error=0.023Mhz
  {171000000, 1,  19,  90,  90}, // SrcFreq=810.00Mhz ReqFreq=171.00Mhz ActFreq=171.00Mhz Error=0.000Mhz
  {171500000, 1,  65, 307, 255}, // SrcFreq=810.00Mhz ReqFreq=171.50Mhz ActFreq=171.50Mhz Error=0.002Mhz
  {172000000, 1,  55, 259, 255}, // SrcFreq=810.00Mhz ReqFreq=172.00Mhz ActFreq=172.00Mhz Error=0.000Mhz  
  {172500000, 1,  23, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=172.50Mhz ActFreq=172.50Mhz Error=0.000Mhz
  {173000000, 1,  22, 103, 103}, // SrcFreq=810.00Mhz ReqFreq=173.00Mhz ActFreq=173.00Mhz Error=0.000Mhz
  {173500000, 1,   3,  14,  14}, // SrcFreq=810.00Mhz ReqFreq=173.50Mhz ActFreq=173.44Mhz Error=0.058Mhz
  {174000000, 1,  29, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=174.00Mhz ActFreq=174.00Mhz Error=0.000Mhz
  {174500000, 1,  67, 311, 255}, // SrcFreq=810.00Mhz ReqFreq=174.50Mhz ActFreq=174.50Mhz Error=0.002Mhz
  {175000000, 1,  35, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=175.00Mhz ActFreq=175.00Mhz Error=0.000Mhz
  {175500000, 1,  13,  60,  60}, // SrcFreq=810.00Mhz ReqFreq=175.50Mhz ActFreq=175.50Mhz Error=0.000Mhz
  {176000000, 1,  68, 313, 255}, // SrcFreq=810.00Mhz ReqFreq=176.00Mhz ActFreq=176.00Mhz Error=0.000Mhz
  {176500000, 1,  56, 257, 255}, // SrcFreq=810.00Mhz ReqFreq=176.50Mhz ActFreq=176.50Mhz Error=0.002Mhz
  {177000000, 1,  59, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=177.00Mhz ActFreq=177.00Mhz Error=0.000Mhz
  {177500000, 1,  71, 234, 234}, // SrcFreq=810.00Mhz ReqFreq=177.50Mhz ActFreq=177.50Mhz Error=0.000Mhz  
  {178000000, 1,  69, 314, 255}, // SrcFreq=810.00Mhz ReqFreq=178.00Mhz ActFreq=178.00Mhz Error=0.000Mhz
  {178500000, 1,  67, 304, 255}, // SrcFreq=810.00Mhz ReqFreq=178.50Mhz ActFreq=178.50Mhz Error=0.000Mhz
  {179000000, 1,  40, 181, 181}, // SrcFreq=810.00Mhz ReqFreq=179.00Mhz ActFreq=179.00Mhz Error=0.000Mhz
  {179500000, 1,  39, 176, 176}, // SrcFreq=810.00Mhz ReqFreq=179.50Mhz ActFreq=179.50Mhz Error=0.001Mhz
  {180000000, 1,   2,   9,   9}, // SrcFreq=810.00Mhz ReqFreq=180.00Mhz ActFreq=180.00Mhz Error=0.000Mhz  
  {180500000, 1,  41, 184, 184}, // SrcFreq=810.00Mhz ReqFreq=180.50Mhz ActFreq=180.50Mhz Error=0.001Mhz
  {181000000, 1,  40, 179, 179}, // SrcFreq=810.00Mhz ReqFreq=181.00Mhz ActFreq=181.00Mhz Error=0.000Mhz  
  {181500000, 1,  67, 299, 255}, // SrcFreq=810.00Mhz ReqFreq=181.50Mhz ActFreq=181.50Mhz Error=0.000Mhz
  {182000000, 1,  71, 316, 255}, // SrcFreq=810.00Mhz ReqFreq=182.00Mhz ActFreq=182.00Mhz Error=0.000Mhz
  {182500000, 1,  73, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=182.50Mhz ActFreq=182.50Mhz Error=0.000Mhz
  {183000000, 1,  61, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=183.00Mhz ActFreq=183.00Mhz Error=0.000Mhz  
  {183500000, 1,  70, 309, 255}, // SrcFreq=810.00Mhz ReqFreq=183.50Mhz ActFreq=183.50Mhz Error=0.001Mhz
  {184000000, 1,  72, 317, 255}, // SrcFreq=810.00Mhz ReqFreq=184.00Mhz ActFreq=184.00Mhz Error=0.000Mhz
  {184500000, 1,  41, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=184.50Mhz ActFreq=184.50Mhz Error=0.000Mhz  
  {185000000, 1,  37, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=185.00Mhz ActFreq=185.00Mhz Error=0.000Mhz
  {185500000, 1,  30, 131, 131}, // SrcFreq=810.00Mhz ReqFreq=185.50Mhz ActFreq=185.50Mhz Error=0.004Mhz
  {186000000, 1,  31, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=186.00Mhz ActFreq=186.00Mhz Error=0.000Mhz  
  {186500000, 1,  67, 291, 255}, // SrcFreq=810.00Mhz ReqFreq=186.50Mhz ActFreq=186.50Mhz Error=0.001Mhz
  {187000000, 1,   3,  13,  13}, // SrcFreq=810.00Mhz ReqFreq=187.00Mhz ActFreq=187.00Mhz Error=0.000Mhz
  {187500000, 1,  25, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=187.50Mhz ActFreq=187.50Mhz Error=0.000Mhz
  {188000000, 1,  68, 293, 255}, // SrcFreq=810.00Mhz ReqFreq=188.00Mhz ActFreq=188.00Mhz Error=0.000Mhz
  {188500000, 1,  64, 275, 255}, // SrcFreq=810.00Mhz ReqFreq=188.50Mhz ActFreq=188.50Mhz Error=0.002Mhz
  {189000000, 1,   7,  30,  30}, // SrcFreq=810.00Mhz ReqFreq=189.00Mhz ActFreq=189.00Mhz Error=0.000Mhz  
  {189500000, 1,  51, 218, 218}, // SrcFreq=810.00Mhz ReqFreq=189.50Mhz ActFreq=189.50Mhz Error=0.005Mhz  
  {190000000, 1,  19,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=190.00Mhz ActFreq=190.00Mhz Error=0.000Mhz
  {190500000, 1,  75, 319, 255}, // SrcFreq=810.00Mhz ReqFreq=190.50Mhz ActFreq=190.50Mhz Error=0.000Mhz
  {191000000, 1,  54, 229, 255}, // SrcFreq=810.00Mhz ReqFreq=191.00Mhz ActFreq=191.00Mhz Error=0.000Mhz
  {191500000, 1,  74, 313, 255}, // SrcFreq=810.00Mhz ReqFreq=191.50Mhz ActFreq=191.50Mhz Error=0.002Mhz
  {192000000, 1,  32, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=192.00Mhz ActFreq=192.00Mhz Error=0.000Mhz
  {192500000, 1,  77, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=192.50Mhz ActFreq=192.50Mhz Error=0.000Mhz
  {193000000, 1,  66, 277, 255}, // SrcFreq=810.00Mhz ReqFreq=193.00Mhz ActFreq=193.00Mhz Error=0.000Mhz
  {193500000, 1,  43, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=193.50Mhz ActFreq=193.50Mhz Error=0.000Mhz
  {194000000, 1,  57, 238, 238}, // SrcFreq=810.00Mhz ReqFreq=194.00Mhz ActFreq=194.00Mhz Error=0.000Mhz
  {194500000, 1,  79, 329, 255}, // SrcFreq=810.00Mhz ReqFreq=194.50Mhz ActFreq=194.50Mhz Error=0.001Mhz
  {195000000, 1,  13,  54,  54}, // SrcFreq=810.00Mhz ReqFreq=195.00Mhz ActFreq=195.00Mhz Error=0.000Mhz
  {195500000, 1,   7,  29,  29}, // SrcFreq=810.00Mhz ReqFreq=195.50Mhz ActFreq=195.49Mhz Error=0.011Mhz
  {196000000, 1,  68, 281, 255}, // SrcFreq=810.00Mhz ReqFreq=196.00Mhz ActFreq=196.00Mhz Error=0.000Mhz
  {196500000, 1,  41, 169, 169}, // SrcFreq=810.00Mhz ReqFreq=196.50Mhz ActFreq=196.50Mhz Error=0.000Mhz
  {197000000, 1,   9,  37,  37}, // SrcFreq=810.00Mhz ReqFreq=197.00Mhz ActFreq=197.00Mhz Error=0.000Mhz
  {197500000, 1,  79, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=197.50Mhz ActFreq=197.50Mhz Error=0.000Mhz
  {198000000, 1,  11,  45,  45}, // SrcFreq=810.00Mhz ReqFreq=198.00Mhz ActFreq=198.00Mhz Error=0.000Mhz
  {198500000, 1,  62, 253, 253}, // SrcFreq=810.00Mhz ReqFreq=198.50Mhz ActFreq=198.50Mhz Error=0.002Mhz
  {199000000, 1,  71, 289, 255}, // SrcFreq=810.00Mhz ReqFreq=199.00Mhz ActFreq=199.00Mhz Error=0.000Mhz
  {199500000, 1,  83, 337, 255}, // SrcFreq=810.00Mhz ReqFreq=199.50Mhz ActFreq=199.50Mhz Error=0.000Mhz
  {200000000, 1,  20,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=200.00Mhz ActFreq=200.00Mhz Error=0.000Mhz
  {200500000, 1,  25, 101, 101}, // SrcFreq=810.00Mhz ReqFreq=200.50Mhz ActFreq=200.50Mhz Error=0.005Mhz
  {201000000, 1,  67, 170, 255}, // SrcFreq=810.00Mhz ReqFreq=201.00Mhz ActFreq=201.00Mhz Error=0.000Mhz
  {201500000, 1,  50, 201, 201}, // SrcFreq=810.00Mhz ReqFreq=201.50Mhz ActFreq=201.50Mhz Error=0.002Mhz
  {202000000, 1,  84, 337, 255}, // SrcFreq=810.00Mhz ReqFreq=202.00Mhz ActFreq=202.00Mhz Error=0.000Mhz  
  {202500000, 1,   1,   4,   4}, // SrcFreq=810.00Mhz ReqFreq=202.50Mhz ActFreq=202.50Mhz Error=0.000Mhz
  {203000000, 1,  85, 339, 255}, // SrcFreq=810.00Mhz ReqFreq=203.00Mhz ActFreq=203.10Mhz Error=0.097Mhz
  {203500000, 1,  51, 203, 203}, // SrcFreq=810.00Mhz ReqFreq=203.50Mhz ActFreq=203.50Mhz Error=0.002Mhz
  {204000000, 1,  34, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=204.00Mhz ActFreq=204.00Mhz Error=0.000Mhz
  {204500000, 1,  51, 202, 202}, // SrcFreq=810.00Mhz ReqFreq=204.50Mhz ActFreq=204.50Mhz Error=0.005Mhz
  {205000000, 1,  41, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=205.00Mhz ActFreq=205.00Mhz Error=0.000Mhz
  {205500000, 1,  86, 339, 255}, // SrcFreq=810.00Mhz ReqFreq=205.50Mhz ActFreq=205.49Mhz Error=0.013Mhz
  {206000000, 1,  59, 232, 232}, // SrcFreq=810.00Mhz ReqFreq=206.00Mhz ActFreq=205.99Mhz Error=0.009Mhz
  {206500000, 1,  77, 302, 255}, // SrcFreq=810.00Mhz ReqFreq=206.50Mhz ActFreq=206.52Mhz Error=0.023Mhz
  {207000000, 1,  23,  90,  90}, // SrcFreq=810.00Mhz ReqFreq=207.00Mhz ActFreq=207.00Mhz Error=0.000Mhz
  {207500000, 1,  83, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=207.50Mhz ActFreq=207.50Mhz Error=0.000Mhz
  {208000000, 1,  85, 331, 255}, // SrcFreq=810.00Mhz ReqFreq=208.00Mhz ActFreq=208.01Mhz Error=0.006Mhz
  {208500000, 1,  87, 338, 255}, // SrcFreq=810.00Mhz ReqFreq=208.50Mhz ActFreq=208.49Mhz Error=0.009Mhz
  {209000000, 1,   8,  31,  31}, // SrcFreq=810.00Mhz ReqFreq=209.00Mhz ActFreq=209.03Mhz Error=0.032Mhz
  {209500000, 1,  15,  58,  58}, // SrcFreq=810.00Mhz ReqFreq=209.50Mhz ActFreq=209.48Mhz Error=0.017Mhz
  {210000000, 1,   7,  27,  27}, // SrcFreq=810.00Mhz ReqFreq=210.00Mhz ActFreq=210.00Mhz Error=0.000Mhz
  {210500000, 1,  79, 304, 255}, // SrcFreq=810.00Mhz ReqFreq=210.50Mhz ActFreq=210.49Mhz Error=0.007Mhz
  {211000000, 1,  31, 119, 119}, // SrcFreq=810.00Mhz ReqFreq=211.00Mhz ActFreq=211.01Mhz Error=0.008Mhz
  {211500000, 1,  47, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=211.50Mhz ActFreq=211.50Mhz Error=0.000Mhz
  {212000000, 1,  67, 256, 255}, // SrcFreq=810.00Mhz ReqFreq=212.00Mhz ActFreq=211.99Mhz Error=0.008Mhz
  {212500000, 1,  85, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=212.50Mhz ActFreq=212.50Mhz Error=0.000Mhz
  {213000000, 1,  71, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=213.00Mhz ActFreq=213.00Mhz Error=0.000Mhz
  {213500000, 1,  34, 129, 129}, // SrcFreq=810.00Mhz ReqFreq=213.50Mhz ActFreq=213.49Mhz Error=0.012Mhz
  {214000000, 1,  79, 299, 255}, // SrcFreq=810.00Mhz ReqFreq=214.00Mhz ActFreq=214.01Mhz Error=0.013Mhz
  {214500000, 1,  76, 287, 255}, // SrcFreq=810.00Mhz ReqFreq=214.50Mhz ActFreq=214.49Mhz Error=0.005Mhz
  {215000000, 1,  43, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=215.00Mhz ActFreq=215.00Mhz Error=0.000Mhz
  {215500000, 1,  29, 109, 109}, // SrcFreq=810.00Mhz ReqFreq=215.50Mhz ActFreq=215.50Mhz Error=0.005Mhz
  {216000000, 1,   4,  15,  15}, // SrcFreq=810.00Mhz ReqFreq=216.00Mhz ActFreq=216.00Mhz Error=0.000Mhz
  {216500000, 1,  58, 217, 217}, // SrcFreq=810.00Mhz ReqFreq=216.50Mhz ActFreq=216.50Mhz Error=0.002Mhz
  {217000000, 1,  86, 321, 255}, // SrcFreq=810.00Mhz ReqFreq=217.00Mhz ActFreq=217.01Mhz Error=0.009Mhz
  {217500000, 1,  29, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=217.50Mhz ActFreq=217.50Mhz Error=0.000Mhz
  {218000000, 1,  88, 327, 255}, // SrcFreq=810.00Mhz ReqFreq=218.00Mhz ActFreq=217.98Mhz Error=0.018Mhz
  {218500000, 1,  58, 215, 215}, // SrcFreq=810.00Mhz ReqFreq=218.50Mhz ActFreq=218.51Mhz Error=0.012Mhz
  {219000000, 1,  73, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=219.00Mhz ActFreq=219.00Mhz Error=0.000Mhz
  {219500000, 1,  71, 262, 255}, // SrcFreq=810.00Mhz ReqFreq=219.50Mhz ActFreq=219.50Mhz Error=0.004Mhz
  {220000000, 1,  22,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=220.00Mhz ActFreq=220.00Mhz Error=0.000Mhz
  {220500000, 1,  49, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=220.50Mhz ActFreq=220.50Mhz Error=0.000Mhz
  {221000000, 1,   3,  11,  11}, // SrcFreq=810.00Mhz ReqFreq=221.00Mhz ActFreq=220.91Mhz Error=0.091Mhz
  {221500000, 1,  67, 245, 245}, // SrcFreq=810.00Mhz ReqFreq=221.50Mhz ActFreq=221.51Mhz Error=0.010Mhz
  {222000000, 1,  37, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=222.00Mhz ActFreq=222.00Mhz Error=0.000Mhz
  {222500000, 1,  89, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=222.50Mhz ActFreq=222.50Mhz Error=0.000Mhz
  {223000000, 1,  68, 247, 247}, // SrcFreq=810.00Mhz ReqFreq=223.00Mhz ActFreq=223.00Mhz Error=0.004Mhz
  {223500000, 1,  93, 337, 255}, // SrcFreq=810.00Mhz ReqFreq=223.50Mhz ActFreq=223.53Mhz Error=0.031Mhz
  {224000000, 1,  86, 311, 255}, // SrcFreq=810.00Mhz ReqFreq=224.00Mhz ActFreq=223.99Mhz Error=0.013Mhz
  {224500000, 1,  74, 267, 255}, // SrcFreq=810.00Mhz ReqFreq=224.50Mhz ActFreq=224.49Mhz Error=0.006Mhz
  {225000000, 1,   5,  18,  18}, // SrcFreq=810.00Mhz ReqFreq=225.00Mhz ActFreq=225.00Mhz Error=0.000Mhz
  {225500000, 1,  76, 273, 255}, // SrcFreq=810.00Mhz ReqFreq=225.50Mhz ActFreq=225.49Mhz Error=0.005Mhz
  {226000000, 1,  89, 319, 255}, // SrcFreq=810.00Mhz ReqFreq=226.00Mhz ActFreq=225.99Mhz Error=0.013Mhz
  {226500000, 1,  92, 329, 255}, // SrcFreq=810.00Mhz ReqFreq=226.50Mhz ActFreq=226.50Mhz Error=0.005Mhz
  {227000000, 1,  44, 157, 157}, // SrcFreq=810.00Mhz ReqFreq=227.00Mhz ActFreq=227.01Mhz Error=0.006Mhz
  {227500000, 1,  91, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=227.50Mhz ActFreq=227.50Mhz Error=0.000Mhz
  {228000000, 1,  38, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=228.00Mhz ActFreq=228.00Mhz Error=0.000Mhz
  {228500000, 1,  90, 319, 255}, // SrcFreq=810.00Mhz ReqFreq=228.50Mhz ActFreq=228.53Mhz Error=0.027Mhz
  {229000000, 1,  54, 191, 191}, // SrcFreq=810.00Mhz ReqFreq=229.00Mhz ActFreq=229.01Mhz Error=0.005Mhz
  {229500000, 1,  17,  60,  60}, // SrcFreq=810.00Mhz ReqFreq=229.50Mhz ActFreq=229.50Mhz Error=0.000Mhz
  {230000000, 1,  23,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=230.00Mhz ActFreq=230.00Mhz Error=0.000Mhz
  {230500000, 1,  35, 123, 123}, // SrcFreq=810.00Mhz ReqFreq=230.50Mhz ActFreq=230.49Mhz Error=0.012Mhz
  {231000000, 1,  77, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=231.00Mhz ActFreq=231.00Mhz Error=0.000Mhz
  {231500000, 1,   2,   7,   7}, // SrcFreq=810.00Mhz ReqFreq=231.50Mhz ActFreq=231.43Mhz Error=0.071Mhz
  {232000000, 1,  59, 206, 206}, // SrcFreq=810.00Mhz ReqFreq=232.00Mhz ActFreq=231.99Mhz Error=0.010Mhz
  {232500000, 1,  31, 108, 108}, // SrcFreq=810.00Mhz ReqFreq=232.50Mhz ActFreq=232.50Mhz Error=0.000Mhz
  {233000000, 1,  21,  73,  73}, // SrcFreq=810.00Mhz ReqFreq=233.00Mhz ActFreq=233.01Mhz Error=0.014Mhz
  {233500000, 1,  81, 281, 255}, // SrcFreq=810.00Mhz ReqFreq=233.50Mhz ActFreq=233.49Mhz Error=0.012Mhz
  {234000000, 1,  13,  45,  45}, // SrcFreq=810.00Mhz ReqFreq=234.00Mhz ActFreq=234.00Mhz Error=0.000Mhz
  {234500000, 1,  11,  38,  38}, // SrcFreq=810.00Mhz ReqFreq=234.50Mhz ActFreq=234.47Mhz Error=0.026Mhz
  {235000000, 1,  47, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=235.00Mhz ActFreq=235.00Mhz Error=0.000Mhz
  {235500000, 1,  91, 313, 255}, // SrcFreq=810.00Mhz ReqFreq=235.50Mhz ActFreq=235.50Mhz Error=0.005Mhz
  {236000000, 1,  81, 278, 255}, // SrcFreq=810.00Mhz ReqFreq=236.00Mhz ActFreq=236.01Mhz Error=0.007Mhz
  {236500000, 1,  40, 137, 137}, // SrcFreq=810.00Mhz ReqFreq=236.50Mhz ActFreq=236.50Mhz Error=0.004Mhz
  {237000000, 1,  79, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=237.00Mhz ActFreq=237.00Mhz Error=0.000Mhz
  {237500000, 1,  95, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=237.50Mhz ActFreq=237.50Mhz Error=0.000Mhz
  {238000000, 1,  62, 211, 211}, // SrcFreq=810.00Mhz ReqFreq=238.00Mhz ActFreq=238.01Mhz Error=0.009Mhz
  {238500000, 1,  53, 180, 180}, // SrcFreq=810.00Mhz ReqFreq=238.50Mhz ActFreq=238.50Mhz Error=0.000Mhz
  {239000000, 1,  18,  61,  61}, // SrcFreq=810.00Mhz ReqFreq=239.00Mhz ActFreq=239.02Mhz Error=0.016Mhz
  {239500000, 1,  89, 301, 255}, // SrcFreq=810.00Mhz ReqFreq=239.50Mhz ActFreq=239.50Mhz Error=0.002Mhz
  {240000000, 1,   8,  27,  27}, // SrcFreq=810.00Mhz ReqFreq=240.00Mhz ActFreq=240.00Mhz Error=0.000Mhz
  {240500000, 1, 106, 357, 255}, // SrcFreq=810.00Mhz ReqFreq=240.50Mhz ActFreq=240.50Mhz Error=0.004Mhz
  {241000000, 1,  36, 121, 121}, // SrcFreq=810.00Mhz ReqFreq=241.00Mhz ActFreq=240.99Mhz Error=0.008Mhz
  {241500000, 1,  48, 161, 161}, // SrcFreq=810.00Mhz ReqFreq=241.50Mhz ActFreq=241.49Mhz Error=0.009Mhz
  {242000000, 1,  72, 241, 241}, // SrcFreq=810.00Mhz ReqFreq=242.00Mhz ActFreq=241.99Mhz Error=0.008Mhz
  {242500000, 1,  97, 324, 255}, // SrcFreq=810.00Mhz ReqFreq=242.50Mhz ActFreq=242.50Mhz Error=0.000Mhz
  {243000000, 1,   3,  10,  10}, // SrcFreq=810.00Mhz ReqFreq=243.00Mhz ActFreq=243.00Mhz Error=0.000Mhz
  {243500000, 1,  49, 163, 163}, // SrcFreq=810.00Mhz ReqFreq=243.50Mhz ActFreq=243.50Mhz Error=0.003Mhz
  {244000000, 1,  97, 322, 255}, // SrcFreq=810.00Mhz ReqFreq=244.00Mhz ActFreq=244.01Mhz Error=0.006Mhz
  {244500000, 1,  99, 328, 255}, // SrcFreq=810.00Mhz ReqFreq=244.50Mhz ActFreq=244.48Mhz Error=0.018Mhz
  {245000000, 1,  49, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=245.00Mhz ActFreq=245.00Mhz Error=0.000Mhz
  {245500000, 1, 107, 353, 255}, // SrcFreq=810.00Mhz ReqFreq=245.50Mhz ActFreq=245.52Mhz Error=0.024Mhz
  {246000000, 1,  41, 135, 135}, // SrcFreq=810.00Mhz ReqFreq=246.00Mhz ActFreq=246.00Mhz Error=0.000Mhz
  {246500000, 1,   7,  23,  23}, // SrcFreq=810.00Mhz ReqFreq=246.50Mhz ActFreq=246.52Mhz Error=0.022Mhz
  {247000000, 1,  68, 223, 223}, // SrcFreq=810.00Mhz ReqFreq=247.00Mhz ActFreq=247.00Mhz Error=0.004Mhz
  {247500000, 1,  11,  36,  36}, // SrcFreq=810.00Mhz ReqFreq=247.50Mhz ActFreq=247.50Mhz Error=0.000Mhz
  {248000000, 1, 109, 356, 255}, // SrcFreq=810.00Mhz ReqFreq=248.00Mhz ActFreq=248.01Mhz Error=0.006Mhz
  {248500000, 1, 104, 339, 255}, // SrcFreq=810.00Mhz ReqFreq=248.50Mhz ActFreq=248.50Mhz Error=0.004Mhz
  {249000000, 1,  83, 270, 255}, // SrcFreq=810.00Mhz ReqFreq=249.00Mhz ActFreq=249.00Mhz Error=0.000Mhz
  {249500000, 1,  73, 237, 237}, // SrcFreq=810.00Mhz ReqFreq=249.50Mhz ActFreq=249.49Mhz Error=0.006Mhz
  {250000000, 1,  25,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=250.00Mhz ActFreq=250.00Mhz Error=0.000Mhz  
  {255000000, 1,  17,  54,  54}, // SrcFreq=810.00Mhz ReqFreq=255.00Mhz ActFreq=255.00Mhz Error=0.000Mhz  
  {260000000, 1,  26,  81,  81}, // SrcFreq=810.00Mhz ReqFreq=260.00Mhz ActFreq=260.00Mhz Error=0.000Mhz  
  {265000000, 1,  53, 162, 162}, // SrcFreq=810.00Mhz ReqFreq=265.00Mhz ActFreq=265.00Mhz Error=0.000Mhz  
  {270000000, 1,   1,   3,   3}, // SrcFreq=810.00Mhz ReqFreq=270.00Mhz ActFreq=270.00Mhz Error=0.000Mhz

};


/* MND lookup tables for 1350Mhz, range from 25Mhz (VGA) -> 270Mhz (WQXGA reduced blanking) */
const DPFreqTableType aDPFreqTable1350Mhz[] = {
  {25000000, 2,   1,  27,  27}, // SrcFreq=675.00Mhz ReqFreq=25.00Mhz ActFreq=25.00Mhz Error=0.000Mhz
  {25500000, 2,   9, 238, 238}, // SrcFreq=675.00Mhz ReqFreq=25.50Mhz ActFreq=25.53Mhz Error=0.025Mhz
  {26000000, 2,   1,  26,  26}, // SrcFreq=675.00Mhz ReqFreq=26.00Mhz ActFreq=25.96Mhz Error=0.038Mhz
  {26500000, 2,   9, 229, 229}, // SrcFreq=675.00Mhz ReqFreq=26.50Mhz ActFreq=26.53Mhz Error=0.028Mhz
  {27000000, 2,   1,  25,  25}, // SrcFreq=675.00Mhz ReqFreq=27.00Mhz ActFreq=27.00Mhz Error=0.000Mhz
  {27500000, 2,   9, 221, 221}, // SrcFreq=675.00Mhz ReqFreq=27.50Mhz ActFreq=27.49Mhz Error=0.011Mhz
  {28000000, 2,   9, 217, 217}, // SrcFreq=675.00Mhz ReqFreq=28.00Mhz ActFreq=28.00Mhz Error=0.005Mhz
  {28500000, 2,  10, 237, 237}, // SrcFreq=675.00Mhz ReqFreq=28.50Mhz ActFreq=28.48Mhz Error=0.019Mhz
  {29000000, 2,  11, 256, 255}, // SrcFreq=675.00Mhz ReqFreq=29.00Mhz ActFreq=29.00Mhz Error=0.004Mhz
  {29500000, 2,   8, 183, 183}, // SrcFreq=675.00Mhz ReqFreq=29.50Mhz ActFreq=29.51Mhz Error=0.008Mhz
  {30000000, 2,   2,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=30.00Mhz ActFreq=30.00Mhz Error=0.000Mhz
  {30500000, 2,   8, 177, 177}, // SrcFreq=675.00Mhz ReqFreq=30.50Mhz ActFreq=30.51Mhz Error=0.008Mhz
  {31000000, 2,   9, 196, 196}, // SrcFreq=675.00Mhz ReqFreq=31.00Mhz ActFreq=30.99Mhz Error=0.005Mhz
  {31500000, 2,   7, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=31.50Mhz ActFreq=31.50Mhz Error=0.000Mhz
  {32000000, 2,  11, 232, 232}, // SrcFreq=675.00Mhz ReqFreq=32.00Mhz ActFreq=32.00Mhz Error=0.004Mhz
  {32500000, 2,   9, 187, 187}, // SrcFreq=675.00Mhz ReqFreq=32.50Mhz ActFreq=32.49Mhz Error=0.013Mhz
  {33000000, 2,  11, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=33.00Mhz ActFreq=33.00Mhz Error=0.000Mhz
  {33500000, 2,  13, 262, 255}, // SrcFreq=675.00Mhz ReqFreq=33.50Mhz ActFreq=33.49Mhz Error=0.008Mhz
  {34000000, 2,   7, 139, 139}, // SrcFreq=675.00Mhz ReqFreq=34.00Mhz ActFreq=33.99Mhz Error=0.007Mhz
  {34500000, 2,   7, 137, 137}, // SrcFreq=675.00Mhz ReqFreq=34.50Mhz ActFreq=34.49Mhz Error=0.011Mhz
  {35000000, 2,   7, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=35.00Mhz ActFreq=35.00Mhz Error=0.000Mhz
  {35500000, 2,   1,  19,  19}, // SrcFreq=675.00Mhz ReqFreq=35.50Mhz ActFreq=35.53Mhz Error=0.026Mhz
  {36000000, 2,   4,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=36.00Mhz ActFreq=36.00Mhz Error=0.000Mhz
  {36500000, 2,   2,  37,  37}, // SrcFreq=675.00Mhz ReqFreq=36.50Mhz ActFreq=36.49Mhz Error=0.014Mhz
  {37000000, 2,   4,  73,  73}, // SrcFreq=675.00Mhz ReqFreq=37.00Mhz ActFreq=36.99Mhz Error=0.014Mhz
  {37500000, 2,   1,  18,  18}, // SrcFreq=675.00Mhz ReqFreq=37.50Mhz ActFreq=37.50Mhz Error=0.000Mhz
  {38000000, 2,  13, 231, 231}, // SrcFreq=675.00Mhz ReqFreq=38.00Mhz ActFreq=37.99Mhz Error=0.013Mhz
  {38500000, 2,  15, 263, 255}, // SrcFreq=675.00Mhz ReqFreq=38.50Mhz ActFreq=38.50Mhz Error=0.002Mhz
  {39000000, 2,  13, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=39.00Mhz ActFreq=39.00Mhz Error=0.000Mhz
  {39500000, 2,  11, 188, 188}, // SrcFreq=675.00Mhz ReqFreq=39.50Mhz ActFreq=39.49Mhz Error=0.005Mhz
  {40000000, 2,   8, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=40.00Mhz ActFreq=40.00Mhz Error=0.000Mhz
  {40500000, 2,   3,  50,  50}, // SrcFreq=675.00Mhz ReqFreq=40.50Mhz ActFreq=40.50Mhz Error=0.000Mhz
  {41000000, 2,  13, 214, 214}, // SrcFreq=675.00Mhz ReqFreq=41.00Mhz ActFreq=41.00Mhz Error=0.005Mhz
  {41500000, 2,  15, 244, 244}, // SrcFreq=675.00Mhz ReqFreq=41.50Mhz ActFreq=41.50Mhz Error=0.004Mhz
  {42000000, 2,  14, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=42.00Mhz ActFreq=42.00Mhz Error=0.000Mhz
  {42500000, 2,  17, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=42.50Mhz ActFreq=42.50Mhz Error=0.000Mhz
  {43000000, 2,  10, 157, 157}, // SrcFreq=675.00Mhz ReqFreq=43.00Mhz ActFreq=42.99Mhz Error=0.006Mhz
  {43500000, 2,  17, 264, 255}, // SrcFreq=675.00Mhz ReqFreq=43.50Mhz ActFreq=43.47Mhz Error=0.034Mhz
  {44000000, 2,   3,  46,  46}, // SrcFreq=675.00Mhz ReqFreq=44.00Mhz ActFreq=44.02Mhz Error=0.022Mhz
  {44500000, 2,   6,  91,  91}, // SrcFreq=675.00Mhz ReqFreq=44.50Mhz ActFreq=44.51Mhz Error=0.005Mhz
  {45000000, 2,   1,  15,  15}, // SrcFreq=675.00Mhz ReqFreq=45.00Mhz ActFreq=45.00Mhz Error=0.000Mhz
  {45500000, 2,   6,  89,  89}, // SrcFreq=675.00Mhz ReqFreq=45.50Mhz ActFreq=45.51Mhz Error=0.006Mhz
  {46000000, 2,   3,  44,  44}, // SrcFreq=675.00Mhz ReqFreq=46.00Mhz ActFreq=46.02Mhz Error=0.023Mhz
  {46500000, 2,  17, 247, 247}, // SrcFreq=675.00Mhz ReqFreq=46.50Mhz ActFreq=46.46Mhz Error=0.043Mhz
  {47000000, 2,  11, 158, 158}, // SrcFreq=675.00Mhz ReqFreq=47.00Mhz ActFreq=46.99Mhz Error=0.006Mhz
  {47500000, 2,  19, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=47.50Mhz ActFreq=47.50Mhz Error=0.000Mhz
  {48000000, 2,  16, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=48.00Mhz ActFreq=48.00Mhz Error=0.000Mhz
  {48500000, 2,  12, 167, 167}, // SrcFreq=675.00Mhz ReqFreq=48.50Mhz ActFreq=48.50Mhz Error=0.003Mhz
  {49000000, 2,   9, 124, 124}, // SrcFreq=675.00Mhz ReqFreq=49.00Mhz ActFreq=48.99Mhz Error=0.008Mhz
  {49500000, 2,  11, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=49.50Mhz ActFreq=49.50Mhz Error=0.000Mhz
  {50000000, 2,   2,  27,  27}, // SrcFreq=675.00Mhz ReqFreq=50.00Mhz ActFreq=50.00Mhz Error=0.000Mhz
  {50500000, 2,  19, 254, 254}, // SrcFreq=675.00Mhz ReqFreq=50.50Mhz ActFreq=50.49Mhz Error=0.008Mhz
  {51000000, 2,  17, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=51.00Mhz ActFreq=51.00Mhz Error=0.000Mhz
  {51500000, 2,  19, 249, 249}, // SrcFreq=675.00Mhz ReqFreq=51.50Mhz ActFreq=51.51Mhz Error=0.006Mhz
  {52000000, 2,   1,  13,  13}, // SrcFreq=675.00Mhz ReqFreq=52.00Mhz ActFreq=51.92Mhz Error=0.077Mhz
  {52500000, 2,   7,  90,  90}, // SrcFreq=675.00Mhz ReqFreq=52.50Mhz ActFreq=52.50Mhz Error=0.000Mhz
  {53000000, 2,  19, 242, 242}, // SrcFreq=675.00Mhz ReqFreq=53.00Mhz ActFreq=53.00Mhz Error=0.004Mhz
  {53500000, 2,  13, 164, 164}, // SrcFreq=675.00Mhz ReqFreq=53.50Mhz ActFreq=53.51Mhz Error=0.006Mhz
  {54000000, 2,   2,  25,  25}, // SrcFreq=675.00Mhz ReqFreq=54.00Mhz ActFreq=54.00Mhz Error=0.000Mhz
  {54500000, 2,  13, 161, 161}, // SrcFreq=675.00Mhz ReqFreq=54.50Mhz ActFreq=54.50Mhz Error=0.003Mhz
  {55000000, 2,  11, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=55.00Mhz ActFreq=55.00Mhz Error=0.000Mhz
  {55500000, 2,  19, 231, 231}, // SrcFreq=675.00Mhz ReqFreq=55.50Mhz ActFreq=55.52Mhz Error=0.019Mhz
  {56000000, 2,  19, 229, 229}, // SrcFreq=675.00Mhz ReqFreq=56.00Mhz ActFreq=56.00Mhz Error=0.004Mhz
  {56500000, 2,  19, 227, 227}, // SrcFreq=675.00Mhz ReqFreq=56.50Mhz ActFreq=56.50Mhz Error=0.002Mhz
  {57000000, 2,  19, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=57.00Mhz ActFreq=57.00Mhz Error=0.000Mhz
  {57500000, 2,  23, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=57.50Mhz ActFreq=57.50Mhz Error=0.000Mhz
  {58000000, 2,  11, 128, 128}, // SrcFreq=675.00Mhz ReqFreq=58.00Mhz ActFreq=58.01Mhz Error=0.008Mhz
  {58500000, 2,  13, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=58.50Mhz ActFreq=58.50Mhz Error=0.000Mhz
  {59000000, 2,  16, 183, 183}, // SrcFreq=675.00Mhz ReqFreq=59.00Mhz ActFreq=59.02Mhz Error=0.016Mhz
  {59500000, 2,  23, 261, 255}, // SrcFreq=675.00Mhz ReqFreq=59.50Mhz ActFreq=59.48Mhz Error=0.017Mhz
  {60000000, 2,   4,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=60.00Mhz ActFreq=60.00Mhz Error=0.000Mhz
  {60500000, 2,  19, 212, 212}, // SrcFreq=675.00Mhz ReqFreq=60.50Mhz ActFreq=60.50Mhz Error=0.005Mhz
  {61000000, 2,  15, 166, 166}, // SrcFreq=675.00Mhz ReqFreq=61.00Mhz ActFreq=60.99Mhz Error=0.006Mhz
  {61500000, 2,  25, 274, 255}, // SrcFreq=675.00Mhz ReqFreq=61.50Mhz ActFreq=61.59Mhz Error=0.088Mhz
  {62000000, 2,   9,  98,  98}, // SrcFreq=675.00Mhz ReqFreq=62.00Mhz ActFreq=61.99Mhz Error=0.010Mhz
  {62500000, 2,   5,  54,  54}, // SrcFreq=675.00Mhz ReqFreq=62.50Mhz ActFreq=62.50Mhz Error=0.000Mhz
  {63000000, 2,   7,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=63.00Mhz ActFreq=63.00Mhz Error=0.000Mhz
  {63500000, 2,  19, 202, 202}, // SrcFreq=675.00Mhz ReqFreq=63.50Mhz ActFreq=63.49Mhz Error=0.010Mhz
  {64000000, 2,  11, 116, 116}, // SrcFreq=675.00Mhz ReqFreq=64.00Mhz ActFreq=64.01Mhz Error=0.009Mhz
  {64500000, 2,  15, 157, 157}, // SrcFreq=675.00Mhz ReqFreq=64.50Mhz ActFreq=64.49Mhz Error=0.010Mhz
  {65000000, 2,  13, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=65.00Mhz ActFreq=65.00Mhz Error=0.000Mhz
  {65500000, 2,  23, 237, 237}, // SrcFreq=675.00Mhz ReqFreq=65.50Mhz ActFreq=65.51Mhz Error=0.006Mhz
  {66000000, 2,  22, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=66.00Mhz ActFreq=66.00Mhz Error=0.000Mhz
  {66500000, 2,  20, 203, 203}, // SrcFreq=675.00Mhz ReqFreq=66.50Mhz ActFreq=66.50Mhz Error=0.002Mhz
  {67000000, 2,  27, 272, 255}, // SrcFreq=675.00Mhz ReqFreq=67.00Mhz ActFreq=67.00Mhz Error=0.004Mhz
  {67500000, 2,   1,  10,  10}, // SrcFreq=675.00Mhz ReqFreq=67.50Mhz ActFreq=67.50Mhz Error=0.000Mhz
  {68000000, 2,  27, 268, 255}, // SrcFreq=675.00Mhz ReqFreq=68.00Mhz ActFreq=68.00Mhz Error=0.004Mhz
  {68500000, 2,  27, 266, 255}, // SrcFreq=675.00Mhz ReqFreq=68.50Mhz ActFreq=68.52Mhz Error=0.015Mhz
  {69000000, 2,  23, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=69.00Mhz ActFreq=69.00Mhz Error=0.000Mhz
  {69500000, 2,   7,  68,  68}, // SrcFreq=675.00Mhz ReqFreq=69.50Mhz ActFreq=69.49Mhz Error=0.015Mhz
  {70000000, 2,  14, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=70.00Mhz ActFreq=70.00Mhz Error=0.000Mhz
  {70500000, 2,  26, 249, 249}, // SrcFreq=675.00Mhz ReqFreq=70.50Mhz ActFreq=70.48Mhz Error=0.018Mhz
  {71000000, 2,   2,  19,  19}, // SrcFreq=675.00Mhz ReqFreq=71.00Mhz ActFreq=71.05Mhz Error=0.053Mhz
  {71500000, 2,  25, 236, 236}, // SrcFreq=675.00Mhz ReqFreq=71.50Mhz ActFreq=71.50Mhz Error=0.004Mhz
  {72000000, 2,   8,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=72.00Mhz ActFreq=72.00Mhz Error=0.000Mhz
  {72500000, 2,  29, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=72.50Mhz ActFreq=72.50Mhz Error=0.000Mhz
  {73000000, 2,   4,  37,  37}, // SrcFreq=675.00Mhz ReqFreq=73.00Mhz ActFreq=72.97Mhz Error=0.027Mhz
  {73500000, 2,  27, 248, 248}, // SrcFreq=675.00Mhz ReqFreq=73.50Mhz ActFreq=73.49Mhz Error=0.012Mhz
  {74000000, 2,  25, 228, 228}, // SrcFreq=675.00Mhz ReqFreq=74.00Mhz ActFreq=74.01Mhz Error=0.013Mhz
  {74250000, 2,  11, 100, 100}, // SrcFreq=675.00Mhz ReqFreq=74.00Mhz ActFreq=74.01Mhz Error=0.013Mhz
  {74500000, 2,  17, 154, 154}, // SrcFreq=675.00Mhz ReqFreq=74.50Mhz ActFreq=74.51Mhz Error=0.013Mhz
  {75000000, 2,   1,   9,   9}, // SrcFreq=675.00Mhz ReqFreq=75.00Mhz ActFreq=75.00Mhz Error=0.000Mhz
  {75500000, 2,  17, 152, 152}, // SrcFreq=675.00Mhz ReqFreq=75.50Mhz ActFreq=75.49Mhz Error=0.007Mhz
  {76000000, 2,  17, 151, 151}, // SrcFreq=675.00Mhz ReqFreq=76.00Mhz ActFreq=75.99Mhz Error=0.007Mhz
  {76500000, 2,  17, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=76.50Mhz ActFreq=76.50Mhz Error=0.000Mhz
  {77000000, 2,  30, 263, 255}, // SrcFreq=675.00Mhz ReqFreq=77.00Mhz ActFreq=77.00Mhz Error=0.004Mhz
  {77500000, 2,  31, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=77.50Mhz ActFreq=77.50Mhz Error=0.000Mhz
  {78000000, 2,  26, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=78.00Mhz ActFreq=78.00Mhz Error=0.000Mhz
  {78500000, 2,   5,  43,  43}, // SrcFreq=675.00Mhz ReqFreq=78.50Mhz ActFreq=78.49Mhz Error=0.012Mhz
  {79000000, 2,  11,  94,  94}, // SrcFreq=675.00Mhz ReqFreq=79.00Mhz ActFreq=78.99Mhz Error=0.011Mhz
  {79500000, 2,  33, 280, 255}, // SrcFreq=675.00Mhz ReqFreq=79.50Mhz ActFreq=79.55Mhz Error=0.054Mhz
  {80000000, 2,  16, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=80.00Mhz ActFreq=80.00Mhz Error=0.000Mhz
  {80500000, 2,  13, 109, 109}, // SrcFreq=675.00Mhz ReqFreq=80.50Mhz ActFreq=80.50Mhz Error=0.005Mhz
  {81000000, 2,   3,  25,  25}, // SrcFreq=675.00Mhz ReqFreq=81.00Mhz ActFreq=81.00Mhz Error=0.000Mhz
  {81500000, 2,  32, 265, 255}, // SrcFreq=675.00Mhz ReqFreq=81.50Mhz ActFreq=81.51Mhz Error=0.009Mhz
  {82000000, 2,  13, 107, 107}, // SrcFreq=675.00Mhz ReqFreq=82.00Mhz ActFreq=82.01Mhz Error=0.009Mhz
  {82500000, 2,  11,  90,  90}, // SrcFreq=675.00Mhz ReqFreq=82.50Mhz ActFreq=82.50Mhz Error=0.000Mhz
  {83000000, 2,  15, 122, 122}, // SrcFreq=675.00Mhz ReqFreq=83.00Mhz ActFreq=82.99Mhz Error=0.008Mhz
  {83500000, 2,  12,  97,  97}, // SrcFreq=675.00Mhz ReqFreq=83.50Mhz ActFreq=83.51Mhz Error=0.005Mhz
  {84000000, 2,  28, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=84.00Mhz ActFreq=84.00Mhz Error=0.000Mhz
  {84500000, 2,   1,   8,   8}, // SrcFreq=675.00Mhz ReqFreq=84.50Mhz ActFreq=84.38Mhz Error=0.125Mhz
  {85000000, 2,  17, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=85.00Mhz ActFreq=85.00Mhz Error=0.000Mhz
  {85500000, 2,  19, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=85.50Mhz ActFreq=85.50Mhz Error=0.000Mhz
  {86000000, 2,  33, 259, 255}, // SrcFreq=675.00Mhz ReqFreq=86.00Mhz ActFreq=86.00Mhz Error=0.004Mhz
  {86500000, 2,  36, 281, 255}, // SrcFreq=675.00Mhz ReqFreq=86.50Mhz ActFreq=86.48Mhz Error=0.023Mhz
  {87000000, 2,  29, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=87.00Mhz ActFreq=87.00Mhz Error=0.000Mhz
  {87500000, 2,   7,  54,  54}, // SrcFreq=675.00Mhz ReqFreq=87.50Mhz ActFreq=87.50Mhz Error=0.000Mhz
  {88000000, 2,   3,  23,  23}, // SrcFreq=675.00Mhz ReqFreq=88.00Mhz ActFreq=88.04Mhz Error=0.043Mhz
  {88500000, 2,  35, 267, 255}, // SrcFreq=675.00Mhz ReqFreq=88.50Mhz ActFreq=88.48Mhz Error=0.017Mhz
  {89000000, 2,  12,  91,  91}, // SrcFreq=675.00Mhz ReqFreq=89.00Mhz ActFreq=89.01Mhz Error=0.011Mhz
  {89500000, 2,  24, 181, 181}, // SrcFreq=675.00Mhz ReqFreq=89.50Mhz ActFreq=89.50Mhz Error=0.003Mhz
  {90000000, 2,   2,  15,  15}, // SrcFreq=675.00Mhz ReqFreq=90.00Mhz ActFreq=90.00Mhz Error=0.000Mhz
  {90500000, 2,  24, 179, 179}, // SrcFreq=675.00Mhz ReqFreq=90.50Mhz ActFreq=90.50Mhz Error=0.003Mhz
  {91000000, 2,  12,  89,  89}, // SrcFreq=675.00Mhz ReqFreq=91.00Mhz ActFreq=91.01Mhz Error=0.011Mhz
  {91500000, 2,  37, 273, 255}, // SrcFreq=675.00Mhz ReqFreq=91.50Mhz ActFreq=91.48Mhz Error=0.016Mhz
  {92000000, 2,   3,  22,  22}, // SrcFreq=675.00Mhz ReqFreq=92.00Mhz ActFreq=92.05Mhz Error=0.045Mhz
  {92500000, 2,  37, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=92.50Mhz ActFreq=92.50Mhz Error=0.000Mhz
  {93000000, 2,  31, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=93.00Mhz ActFreq=93.00Mhz Error=0.000Mhz
  {93500000, 2,  41, 296, 255}, // SrcFreq=675.00Mhz ReqFreq=93.50Mhz ActFreq=93.50Mhz Error=0.003Mhz
  {94000000, 2,  11,  79,  79}, // SrcFreq=675.00Mhz ReqFreq=94.00Mhz ActFreq=93.99Mhz Error=0.013Mhz
  {94500000, 2,   7,  50,  50}, // SrcFreq=675.00Mhz ReqFreq=94.50Mhz ActFreq=94.50Mhz Error=0.000Mhz
  {95000000, 2,  19, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=95.00Mhz ActFreq=95.00Mhz Error=0.000Mhz
  {95500000, 2,  29, 205, 205}, // SrcFreq=675.00Mhz ReqFreq=95.50Mhz ActFreq=95.49Mhz Error=0.012Mhz
  {96000000, 2,  32, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=96.00Mhz ActFreq=96.00Mhz Error=0.000Mhz
  {96500000, 2,   1,   7,   7}, // SrcFreq=675.00Mhz ReqFreq=96.50Mhz ActFreq=96.43Mhz Error=0.071Mhz
  {97000000, 2,  24, 167, 167}, // SrcFreq=675.00Mhz ReqFreq=97.00Mhz ActFreq=97.01Mhz Error=0.006Mhz
  {97500000, 2,  13,  90,  90}, // SrcFreq=675.00Mhz ReqFreq=97.50Mhz ActFreq=97.50Mhz Error=0.000Mhz
  {98000000, 2,   9,  62,  62}, // SrcFreq=675.00Mhz ReqFreq=98.00Mhz ActFreq=97.98Mhz Error=0.016Mhz
  {98500000, 2,  34, 233, 233}, // SrcFreq=675.00Mhz ReqFreq=98.50Mhz ActFreq=98.50Mhz Error=0.002Mhz
  {99000000, 2,  11,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=99.00Mhz ActFreq=99.00Mhz Error=0.000Mhz
  {99500000, 2,  37, 251, 251}, // SrcFreq=675.00Mhz ReqFreq=99.50Mhz ActFreq=99.50Mhz Error=0.002Mhz
  {100000000, 2,   4,  27,  27}, // SrcFreq=675.00Mhz ReqFreq=100.00Mhz ActFreq=100.00Mhz Error=0.000Mhz
  {100500000, 2,  39, 262, 255}, // SrcFreq=675.00Mhz ReqFreq=100.50Mhz ActFreq=100.48Mhz Error=0.023Mhz
  {101000000, 2,  41, 274, 255}, // SrcFreq=675.00Mhz ReqFreq=101.00Mhz ActFreq=101.00Mhz Error=0.004Mhz
  {101500000, 2,  20, 133, 133}, // SrcFreq=675.00Mhz ReqFreq=101.50Mhz ActFreq=101.50Mhz Error=0.004Mhz
  {102000000, 2,  34, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=102.00Mhz ActFreq=102.00Mhz Error=0.000Mhz
  {102500000, 2,  41, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=102.50Mhz ActFreq=102.50Mhz Error=0.000Mhz
  {103000000, 2,  38, 249, 249}, // SrcFreq=675.00Mhz ReqFreq=103.00Mhz ActFreq=103.01Mhz Error=0.012Mhz
  {103500000, 2,  23, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=103.50Mhz ActFreq=103.50Mhz Error=0.000Mhz
  {104000000, 2,  45, 292, 255}, // SrcFreq=675.00Mhz ReqFreq=104.00Mhz ActFreq=104.02Mhz Error=0.024Mhz
  {104500000, 2,  37, 239, 239}, // SrcFreq=675.00Mhz ReqFreq=104.50Mhz ActFreq=104.50Mhz Error=0.002Mhz
  {105000000, 2,   7,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=105.00Mhz ActFreq=105.00Mhz Error=0.000Mhz
  {105500000, 2,   5,  32,  32}, // SrcFreq=675.00Mhz ReqFreq=105.50Mhz ActFreq=105.47Mhz Error=0.031Mhz
  {106000000, 2,  19, 121, 121}, // SrcFreq=675.00Mhz ReqFreq=106.00Mhz ActFreq=105.99Mhz Error=0.008Mhz
  {106500000, 2,  47, 298, 255}, // SrcFreq=675.00Mhz ReqFreq=106.50Mhz ActFreq=106.46Mhz Error=0.040Mhz
  {107000000, 2,  13,  82,  82}, // SrcFreq=675.00Mhz ReqFreq=107.00Mhz ActFreq=107.01Mhz Error=0.012Mhz
  {107500000, 2,  43, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=107.50Mhz ActFreq=107.50Mhz Error=0.000Mhz
  {108000000, 2,   4,  25,  25}, // SrcFreq=675.00Mhz ReqFreq=108.00Mhz ActFreq=108.00Mhz Error=0.000Mhz
  {108500000, 2,   9,  56,  56}, // SrcFreq=675.00Mhz ReqFreq=108.50Mhz ActFreq=108.48Mhz Error=0.018Mhz
  {109000000, 2,  26, 161, 161}, // SrcFreq=675.00Mhz ReqFreq=109.00Mhz ActFreq=109.01Mhz Error=0.006Mhz
  {109500000, 2,  49, 302, 255}, // SrcFreq=675.00Mhz ReqFreq=109.50Mhz ActFreq=109.52Mhz Error=0.020Mhz
  {110000000, 2,  22, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=110.00Mhz ActFreq=110.00Mhz Error=0.000Mhz
  {110500000, 2,  46, 281, 255}, // SrcFreq=675.00Mhz ReqFreq=110.50Mhz ActFreq=110.50Mhz Error=0.002Mhz
  {111000000, 2,  37, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=111.00Mhz ActFreq=111.00Mhz Error=0.000Mhz
  {111500000, 2,  37, 224, 224}, // SrcFreq=675.00Mhz ReqFreq=111.50Mhz ActFreq=111.50Mhz Error=0.004Mhz
  {112000000, 2,  37, 223, 223}, // SrcFreq=675.00Mhz ReqFreq=112.00Mhz ActFreq=112.00Mhz Error=0.004Mhz
  {112500000, 2,   1,   6,   6}, // SrcFreq=675.00Mhz ReqFreq=112.50Mhz ActFreq=112.50Mhz Error=0.000Mhz
  {113000000, 2,  38, 227, 227}, // SrcFreq=675.00Mhz ReqFreq=113.00Mhz ActFreq=113.00Mhz Error=0.004Mhz
  {113500000, 2,  19, 113, 113}, // SrcFreq=675.00Mhz ReqFreq=113.50Mhz ActFreq=113.50Mhz Error=0.004Mhz
  {114000000, 2,  38, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=114.00Mhz ActFreq=114.00Mhz Error=0.000Mhz
  {114500000, 2,  19, 112, 112}, // SrcFreq=675.00Mhz ReqFreq=114.50Mhz ActFreq=114.51Mhz Error=0.009Mhz
  {115000000, 2,  23, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=115.00Mhz ActFreq=115.00Mhz Error=0.000Mhz
  {115500000, 2,  45, 263, 255}, // SrcFreq=675.00Mhz ReqFreq=115.50Mhz ActFreq=115.49Mhz Error=0.006Mhz
  {116000000, 2,  11,  64,  64}, // SrcFreq=675.00Mhz ReqFreq=116.00Mhz ActFreq=116.02Mhz Error=0.016Mhz
  {116500000, 2,  34, 197, 197}, // SrcFreq=675.00Mhz ReqFreq=116.50Mhz ActFreq=116.50Mhz Error=0.003Mhz
  {117000000, 2,  13,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=117.00Mhz ActFreq=117.00Mhz Error=0.000Mhz
  {117500000, 2,  47, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=117.50Mhz ActFreq=117.50Mhz Error=0.000Mhz
  {118000000, 2,  25, 143, 143}, // SrcFreq=675.00Mhz ReqFreq=118.00Mhz ActFreq=118.01Mhz Error=0.007Mhz
  {118500000, 2,  23, 131, 131}, // SrcFreq=675.00Mhz ReqFreq=118.50Mhz ActFreq=118.51Mhz Error=0.011Mhz
  {119000000, 2,  52, 295, 255}, // SrcFreq=675.00Mhz ReqFreq=119.00Mhz ActFreq=118.98Mhz Error=0.017Mhz
  {119500000, 2,  37, 209, 209}, // SrcFreq=675.00Mhz ReqFreq=119.50Mhz ActFreq=119.50Mhz Error=0.002Mhz
  {120000000, 2,   8,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=120.00Mhz ActFreq=120.00Mhz Error=0.000Mhz
  {120500000, 2,   5,  28,  28}, // SrcFreq=675.00Mhz ReqFreq=120.50Mhz ActFreq=120.54Mhz Error=0.036Mhz
  {121000000, 2,  19, 106, 106}, // SrcFreq=675.00Mhz ReqFreq=121.00Mhz ActFreq=120.99Mhz Error=0.009Mhz
  {121500000, 2,   9,  50,  50}, // SrcFreq=675.00Mhz ReqFreq=121.50Mhz ActFreq=121.50Mhz Error=0.000Mhz
  {122000000, 2,  15,  83,  83}, // SrcFreq=675.00Mhz ReqFreq=122.00Mhz ActFreq=121.99Mhz Error=0.012Mhz
  {122500000, 2,  49, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=122.50Mhz ActFreq=122.50Mhz Error=0.000Mhz
  {123000000, 2,  41, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=123.00Mhz ActFreq=123.00Mhz Error=0.000Mhz
  {123500000, 2,  43, 235, 235}, // SrcFreq=675.00Mhz ReqFreq=123.50Mhz ActFreq=123.51Mhz Error=0.011Mhz
  {124000000, 2,   9,  49,  49}, // SrcFreq=675.00Mhz ReqFreq=124.00Mhz ActFreq=123.98Mhz Error=0.020Mhz
  {124500000, 2,  45, 244, 244}, // SrcFreq=675.00Mhz ReqFreq=124.50Mhz ActFreq=124.49Mhz Error=0.012Mhz
  {125000000, 2,   5,  27,  27}, // SrcFreq=675.00Mhz ReqFreq=125.00Mhz ActFreq=125.00Mhz Error=0.000Mhz
  {125500000, 2,  37, 199, 199}, // SrcFreq=675.00Mhz ReqFreq=125.50Mhz ActFreq=125.50Mhz Error=0.003Mhz
  {126000000, 2,  14,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=126.00Mhz ActFreq=126.00Mhz Error=0.000Mhz
  {126500000, 2,   3,  16,  16}, // SrcFreq=675.00Mhz ReqFreq=126.50Mhz ActFreq=126.56Mhz Error=0.063Mhz
  {127000000, 2,  54, 287, 255}, // SrcFreq=675.00Mhz ReqFreq=127.00Mhz ActFreq=127.00Mhz Error=0.003Mhz
  {127500000, 2,  17,  90,  90}, // SrcFreq=675.00Mhz ReqFreq=127.50Mhz ActFreq=127.50Mhz Error=0.000Mhz
  {128000000, 2,  11,  58,  58}, // SrcFreq=675.00Mhz ReqFreq=128.00Mhz ActFreq=128.02Mhz Error=0.017Mhz
  {128500000, 2,  59, 310, 255}, // SrcFreq=675.00Mhz ReqFreq=128.50Mhz ActFreq=128.47Mhz Error=0.032Mhz
  {129000000, 2,  43, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=129.00Mhz ActFreq=129.00Mhz Error=0.000Mhz
  {129500000, 2,  33, 172, 172}, // SrcFreq=675.00Mhz ReqFreq=129.50Mhz ActFreq=129.51Mhz Error=0.006Mhz
  {130000000, 2,  26, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=130.00Mhz ActFreq=130.00Mhz Error=0.000Mhz
  {130500000, 2,  29, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=130.50Mhz ActFreq=130.50Mhz Error=0.000Mhz
  {131000000, 2,  59, 304, 255}, // SrcFreq=675.00Mhz ReqFreq=131.00Mhz ActFreq=131.00Mhz Error=0.003Mhz
  {131500000, 2,  15,  77,  77}, // SrcFreq=675.00Mhz ReqFreq=131.50Mhz ActFreq=131.49Mhz Error=0.006Mhz
  {132000000, 2,  44, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=132.00Mhz ActFreq=132.00Mhz Error=0.000Mhz
  {132500000, 2,  53, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=132.50Mhz ActFreq=132.50Mhz Error=0.000Mhz
  {133000000, 2,  40, 203, 203}, // SrcFreq=675.00Mhz ReqFreq=133.00Mhz ActFreq=133.00Mhz Error=0.005Mhz
  {133500000, 2,  53, 268, 255}, // SrcFreq=675.00Mhz ReqFreq=133.50Mhz ActFreq=133.49Mhz Error=0.011Mhz
  {134000000, 2,  27, 136, 136}, // SrcFreq=675.00Mhz ReqFreq=134.00Mhz ActFreq=134.01Mhz Error=0.007Mhz
  {134500000, 2,  54, 271, 255}, // SrcFreq=675.00Mhz ReqFreq=134.50Mhz ActFreq=134.50Mhz Error=0.002Mhz
  {135000000, 2,   1,   5,   5}, // SrcFreq=675.00Mhz ReqFreq=135.00Mhz ActFreq=135.00Mhz Error=0.000Mhz
  {135500000, 2,  54, 269, 255}, // SrcFreq=675.00Mhz ReqFreq=135.50Mhz ActFreq=135.50Mhz Error=0.002Mhz
  {136000000, 2,  27, 134, 134}, // SrcFreq=675.00Mhz ReqFreq=136.00Mhz ActFreq=136.01Mhz Error=0.007Mhz
  {136500000, 2,  55, 272, 255}, // SrcFreq=675.00Mhz ReqFreq=136.50Mhz ActFreq=136.49Mhz Error=0.011Mhz
  {137000000, 2,  41, 202, 202}, // SrcFreq=675.00Mhz ReqFreq=137.00Mhz ActFreq=137.00Mhz Error=0.005Mhz
  {137500000, 2,  11,  54,  54}, // SrcFreq=675.00Mhz ReqFreq=137.50Mhz ActFreq=137.50Mhz Error=0.000Mhz
  {138000000, 2,  46, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=138.00Mhz ActFreq=138.00Mhz Error=0.000Mhz
  {138500000, 2,  63, 307, 255}, // SrcFreq=675.00Mhz ReqFreq=138.50Mhz ActFreq=138.52Mhz Error=0.018Mhz
  {139000000, 2,   7,  34,  34}, // SrcFreq=675.00Mhz ReqFreq=139.00Mhz ActFreq=138.97Mhz Error=0.029Mhz
  {139500000, 2,  31, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=139.50Mhz ActFreq=139.50Mhz Error=0.000Mhz
  {140000000, 2,  28, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=140.00Mhz ActFreq=140.00Mhz Error=0.000Mhz
  {140500000, 2,  46, 221, 221}, // SrcFreq=675.00Mhz ReqFreq=140.50Mhz ActFreq=140.50Mhz Error=0.002Mhz
  {141000000, 2,  47, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=141.00Mhz ActFreq=141.00Mhz Error=0.000Mhz
  {141500000, 2,  61, 291, 255}, // SrcFreq=675.00Mhz ReqFreq=141.50Mhz ActFreq=141.49Mhz Error=0.005Mhz
  {142000000, 2,  65, 309, 255}, // SrcFreq=675.00Mhz ReqFreq=142.00Mhz ActFreq=141.99Mhz Error=0.010Mhz
  {142500000, 2,  19,  90,  90}, // SrcFreq=675.00Mhz ReqFreq=142.50Mhz ActFreq=142.50Mhz Error=0.000Mhz
  {143000000, 2,  25, 118, 118}, // SrcFreq=675.00Mhz ReqFreq=143.00Mhz ActFreq=143.01Mhz Error=0.008Mhz
  {143500000, 2,  27, 127, 127}, // SrcFreq=675.00Mhz ReqFreq=143.50Mhz ActFreq=143.50Mhz Error=0.004Mhz
  {144000000, 2,  16,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=144.00Mhz ActFreq=144.00Mhz Error=0.000Mhz
  {144500000, 2,  67, 313, 255}, // SrcFreq=675.00Mhz ReqFreq=144.50Mhz ActFreq=144.49Mhz Error=0.011Mhz
  {145000000, 2,  29, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=145.00Mhz ActFreq=145.00Mhz Error=0.000Mhz
  {145500000, 2,  61, 283, 255}, // SrcFreq=675.00Mhz ReqFreq=145.50Mhz ActFreq=145.49Mhz Error=0.005Mhz
  {146000000, 2,  69, 319, 255}, // SrcFreq=675.00Mhz ReqFreq=146.00Mhz ActFreq=146.00Mhz Error=0.003Mhz
  {146500000, 2,  51, 235, 235}, // SrcFreq=675.00Mhz ReqFreq=146.50Mhz ActFreq=146.49Mhz Error=0.011Mhz
  {147000000, 2,  49, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=147.00Mhz ActFreq=147.00Mhz Error=0.000Mhz
  {147500000, 2,  59, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=147.50Mhz ActFreq=147.50Mhz Error=0.000Mhz
  {148000000, 2,  41, 187, 187}, // SrcFreq=675.00Mhz ReqFreq=148.00Mhz ActFreq=147.99Mhz Error=0.005Mhz
  {148500000, 2,  11,  50,  50}, // SrcFreq=675.00Mhz ReqFreq=148.50Mhz ActFreq=148.50Mhz Error=0.000Mhz
  {149000000, 2,  66, 299, 255}, // SrcFreq=675.00Mhz ReqFreq=149.00Mhz ActFreq=149.00Mhz Error=0.003Mhz
  {149500000, 2,  33, 149, 149}, // SrcFreq=675.00Mhz ReqFreq=149.50Mhz ActFreq=149.50Mhz Error=0.003Mhz
  {150000000, 2,   2,   9,   9}, // SrcFreq=675.00Mhz ReqFreq=150.00Mhz ActFreq=150.00Mhz Error=0.000Mhz
  {150500000, 2,  33, 148, 148}, // SrcFreq=675.00Mhz ReqFreq=150.50Mhz ActFreq=150.51Mhz Error=0.007Mhz
  {151000000, 2,  17,  76,  76}, // SrcFreq=675.00Mhz ReqFreq=151.00Mhz ActFreq=150.99Mhz Error=0.013Mhz
  {151500000, 2,  68, 303, 255}, // SrcFreq=675.00Mhz ReqFreq=151.50Mhz ActFreq=151.49Mhz Error=0.015Mhz
  {152000000, 2,  59, 262, 255}, // SrcFreq=675.00Mhz ReqFreq=152.00Mhz ActFreq=152.00Mhz Error=0.004Mhz
  {152500000, 2,  61, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=152.50Mhz ActFreq=152.50Mhz Error=0.000Mhz
  {153000000, 2,  17,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=153.00Mhz ActFreq=153.00Mhz Error=0.000Mhz
  {153500000, 2,  73, 321, 255}, // SrcFreq=675.00Mhz ReqFreq=153.50Mhz ActFreq=153.50Mhz Error=0.005Mhz
  {154000000, 2,  47, 206, 206}, // SrcFreq=675.00Mhz ReqFreq=154.00Mhz ActFreq=154.00Mhz Error=0.005Mhz
  {154500000, 2,  65, 284, 255}, // SrcFreq=675.00Mhz ReqFreq=154.50Mhz ActFreq=154.49Mhz Error=0.011Mhz
  {155000000, 2,  31, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=155.00Mhz ActFreq=155.00Mhz Error=0.000Mhz
  {155500000, 2,  44, 191, 191}, // SrcFreq=675.00Mhz ReqFreq=155.50Mhz ActFreq=155.50Mhz Error=0.003Mhz
  {156000000, 2,  52, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=156.00Mhz ActFreq=156.00Mhz Error=0.000Mhz
  {156500000, 2,  67, 289, 255}, // SrcFreq=675.00Mhz ReqFreq=156.50Mhz ActFreq=156.49Mhz Error=0.012Mhz
  {157000000, 2,  10,  43,  43}, // SrcFreq=675.00Mhz ReqFreq=157.00Mhz ActFreq=156.98Mhz Error=0.023Mhz
  {157500000, 2,   7,  30,  30}, // SrcFreq=675.00Mhz ReqFreq=157.50Mhz ActFreq=157.50Mhz Error=0.000Mhz
  {158000000, 2,  11,  47,  47}, // SrcFreq=675.00Mhz ReqFreq=158.00Mhz ActFreq=157.98Mhz Error=0.021Mhz
  {158500000, 2,  58, 247, 247}, // SrcFreq=675.00Mhz ReqFreq=158.50Mhz ActFreq=158.50Mhz Error=0.002Mhz
  {159000000, 2,  53, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=159.00Mhz ActFreq=159.00Mhz Error=0.000Mhz
  {159500000, 2,  69, 292, 255}, // SrcFreq=675.00Mhz ReqFreq=159.50Mhz ActFreq=159.50Mhz Error=0.003Mhz
  {160000000, 2,  32, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=160.00Mhz ActFreq=160.00Mhz Error=0.000Mhz
  {160500000, 2,  73, 307, 255}, // SrcFreq=675.00Mhz ReqFreq=160.50Mhz ActFreq=160.50Mhz Error=0.005Mhz
  {161000000, 2,  26, 109, 109}, // SrcFreq=675.00Mhz ReqFreq=161.00Mhz ActFreq=161.01Mhz Error=0.009Mhz
  {161500000, 2,  39, 163, 163}, // SrcFreq=675.00Mhz ReqFreq=161.50Mhz ActFreq=161.50Mhz Error=0.003Mhz
  {162000000, 2,   6,  25,  25}, // SrcFreq=675.00Mhz ReqFreq=162.00Mhz ActFreq=162.00Mhz Error=0.000Mhz
  {162500000, 2,  13,  54,  54}, // SrcFreq=675.00Mhz ReqFreq=162.50Mhz ActFreq=162.50Mhz Error=0.000Mhz
  {163000000, 2,  78, 323, 255}, // SrcFreq=675.00Mhz ReqFreq=163.00Mhz ActFreq=163.00Mhz Error=0.003Mhz
  {163500000, 2,  70, 289, 255}, // SrcFreq=675.00Mhz ReqFreq=163.50Mhz ActFreq=163.49Mhz Error=0.005Mhz
  {164000000, 2,  69, 284, 255}, // SrcFreq=675.00Mhz ReqFreq=164.00Mhz ActFreq=164.00Mhz Error=0.004Mhz
  {164500000, 2,  29, 119, 119}, // SrcFreq=675.00Mhz ReqFreq=164.50Mhz ActFreq=164.50Mhz Error=0.004Mhz
  {165000000, 2,  11,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=165.00Mhz ActFreq=165.00Mhz Error=0.000Mhz
  {165500000, 2,  51, 208, 208}, // SrcFreq=675.00Mhz ReqFreq=165.50Mhz ActFreq=165.50Mhz Error=0.005Mhz
  {166000000, 2,  15,  61,  61}, // SrcFreq=675.00Mhz ReqFreq=166.00Mhz ActFreq=165.98Mhz Error=0.016Mhz
  {166500000, 2,  37, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=166.50Mhz ActFreq=166.50Mhz Error=0.000Mhz
  {167000000, 2,  24,  97,  97}, // SrcFreq=675.00Mhz ReqFreq=167.00Mhz ActFreq=167.01Mhz Error=0.010Mhz
  {167500000, 2,  67, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=167.50Mhz ActFreq=167.50Mhz Error=0.000Mhz
  {168000000, 2,  56, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=168.00Mhz ActFreq=168.00Mhz Error=0.000Mhz
  {168500000, 2,   1,   4,   4}, // SrcFreq=675.00Mhz ReqFreq=168.50Mhz ActFreq=168.75Mhz Error=0.250Mhz
  {169000000, 2,  85, 339, 255}, // SrcFreq=675.00Mhz ReqFreq=169.00Mhz ActFreq=169.25Mhz Error=0.248Mhz
  {169500000, 2,  57, 227, 227}, // SrcFreq=675.00Mhz ReqFreq=169.50Mhz ActFreq=169.49Mhz Error=0.007Mhz
  {170000000, 2,  34, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=170.00Mhz ActFreq=170.00Mhz Error=0.000Mhz
  {170500000, 2,  73, 289, 255}, // SrcFreq=675.00Mhz ReqFreq=170.50Mhz ActFreq=170.50Mhz Error=0.002Mhz
  {171000000, 2,  19,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=171.00Mhz ActFreq=171.00Mhz Error=0.000Mhz
  {171500000, 2,  78, 307, 255}, // SrcFreq=675.00Mhz ReqFreq=171.50Mhz ActFreq=171.50Mhz Error=0.002Mhz
  {172000000, 2,  53, 208, 208}, // SrcFreq=675.00Mhz ReqFreq=172.00Mhz ActFreq=172.00Mhz Error=0.005Mhz
  {172500000, 2,  23,  90,  90}, // SrcFreq=675.00Mhz ReqFreq=172.50Mhz ActFreq=172.50Mhz Error=0.000Mhz
  {173000000, 2,  61, 238, 238}, // SrcFreq=675.00Mhz ReqFreq=173.00Mhz ActFreq=173.00Mhz Error=0.004Mhz
  {173500000, 2,  73, 284, 255}, // SrcFreq=675.00Mhz ReqFreq=173.50Mhz ActFreq=173.50Mhz Error=0.004Mhz
  {174000000, 2,  58, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=174.00Mhz ActFreq=174.00Mhz Error=0.000Mhz
  {174500000, 2,  38, 147, 147}, // SrcFreq=675.00Mhz ReqFreq=174.50Mhz ActFreq=174.49Mhz Error=0.010Mhz
  {175000000, 2,   7,  27,  27}, // SrcFreq=675.00Mhz ReqFreq=175.00Mhz ActFreq=175.00Mhz Error=0.000Mhz
  {175500000, 2,  13,  50,  50}, // SrcFreq=675.00Mhz ReqFreq=175.50Mhz ActFreq=175.50Mhz Error=0.000Mhz
  {176000000, 2,  85, 326, 255}, // SrcFreq=675.00Mhz ReqFreq=176.00Mhz ActFreq=176.00Mhz Error=0.003Mhz
  {176500000, 2,  74, 283, 255}, // SrcFreq=675.00Mhz ReqFreq=176.50Mhz ActFreq=176.50Mhz Error=0.002Mhz
  {177000000, 2,  59, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=177.00Mhz ActFreq=177.00Mhz Error=0.000Mhz
  {177500000, 2,  71, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=177.50Mhz ActFreq=177.50Mhz Error=0.000Mhz
  {178000000, 2,  77, 292, 255}, // SrcFreq=675.00Mhz ReqFreq=178.00Mhz ActFreq=178.00Mhz Error=0.003Mhz
  {178500000, 2,  87, 329, 255}, // SrcFreq=675.00Mhz ReqFreq=178.50Mhz ActFreq=178.50Mhz Error=0.005Mhz
  {179000000, 2,  48, 181, 181}, // SrcFreq=675.00Mhz ReqFreq=179.00Mhz ActFreq=179.01Mhz Error=0.006Mhz
  {179500000, 2,  71, 267, 255}, // SrcFreq=675.00Mhz ReqFreq=179.50Mhz ActFreq=179.49Mhz Error=0.006Mhz
  {180000000, 2,   4,  15,  15}, // SrcFreq=675.00Mhz ReqFreq=180.00Mhz ActFreq=180.00Mhz Error=0.000Mhz
  {180500000, 2,  73, 273, 255}, // SrcFreq=675.00Mhz ReqFreq=180.50Mhz ActFreq=180.49Mhz Error=0.005Mhz
  {181000000, 2,  48, 179, 179}, // SrcFreq=675.00Mhz ReqFreq=181.00Mhz ActFreq=181.01Mhz Error=0.006Mhz
  {181500000, 2,  89, 331, 255}, // SrcFreq=675.00Mhz ReqFreq=181.50Mhz ActFreq=181.50Mhz Error=0.005Mhz
  {182000000, 2,  79, 293, 255}, // SrcFreq=675.00Mhz ReqFreq=182.00Mhz ActFreq=182.00Mhz Error=0.003Mhz
  {182500000, 2,  73, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=182.50Mhz ActFreq=182.50Mhz Error=0.000Mhz
  {183000000, 2,  61, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=183.00Mhz ActFreq=183.00Mhz Error=0.000Mhz
  {183500000, 2,  28, 103, 103}, // SrcFreq=675.00Mhz ReqFreq=183.50Mhz ActFreq=183.50Mhz Error=0.005Mhz
  {184000000, 2,  94, 345, 255}, // SrcFreq=675.00Mhz ReqFreq=184.00Mhz ActFreq=183.91Mhz Error=0.087Mhz
  {184500000, 2,  41, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=184.50Mhz ActFreq=184.50Mhz Error=0.000Mhz
  {185000000, 2,  37, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=185.00Mhz ActFreq=185.00Mhz Error=0.000Mhz
  {185500000, 2,  36, 131, 131}, // SrcFreq=675.00Mhz ReqFreq=185.50Mhz ActFreq=185.50Mhz Error=0.004Mhz
  {186000000, 2,  62, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=186.00Mhz ActFreq=186.00Mhz Error=0.000Mhz
  {186500000, 2,  21,  76,  76}, // SrcFreq=675.00Mhz ReqFreq=186.50Mhz ActFreq=186.51Mhz Error=0.013Mhz
  {187000000, 2,  41, 148, 148}, // SrcFreq=675.00Mhz ReqFreq=187.00Mhz ActFreq=186.99Mhz Error=0.007Mhz
  {187500000, 2,   5,  18,  18}, // SrcFreq=675.00Mhz ReqFreq=187.50Mhz ActFreq=187.50Mhz Error=0.000Mhz
  {188000000, 2,  83, 298, 255}, // SrcFreq=675.00Mhz ReqFreq=188.00Mhz ActFreq=188.00Mhz Error=0.003Mhz
  {188500000, 2,  74, 265, 255}, // SrcFreq=675.00Mhz ReqFreq=188.50Mhz ActFreq=188.49Mhz Error=0.009Mhz
  {189000000, 2,   7,  25,  25}, // SrcFreq=675.00Mhz ReqFreq=189.00Mhz ActFreq=189.00Mhz Error=0.000Mhz
  {189500000, 2,  89, 317, 255}, // SrcFreq=675.00Mhz ReqFreq=189.50Mhz ActFreq=189.51Mhz Error=0.011Mhz
  {190000000, 2,  38, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=190.00Mhz ActFreq=190.00Mhz Error=0.000Mhz
  {190500000, 2,  81, 287, 255}, // SrcFreq=675.00Mhz ReqFreq=190.50Mhz ActFreq=190.51Mhz Error=0.005Mhz
  {191000000, 2,  88, 311, 255}, // SrcFreq=675.00Mhz ReqFreq=191.00Mhz ActFreq=191.00Mhz Error=0.003Mhz
  {191500000, 2, 101, 356, 255}, // SrcFreq=675.00Mhz ReqFreq=191.50Mhz ActFreq=191.50Mhz Error=0.003Mhz
  {192000000, 2,  64, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=192.00Mhz ActFreq=192.00Mhz Error=0.000Mhz
  {192500000, 2,  77, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=192.50Mhz ActFreq=192.50Mhz Error=0.000Mhz
  {193000000, 2, 101, 353, 255}, // SrcFreq=675.00Mhz ReqFreq=193.00Mhz ActFreq=193.13Mhz Error=0.130Mhz
  {193500000, 2,  43, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=193.50Mhz ActFreq=193.50Mhz Error=0.000Mhz
  {194000000, 2,  73, 254, 254}, // SrcFreq=675.00Mhz ReqFreq=194.00Mhz ActFreq=194.00Mhz Error=0.004Mhz
  {194500000, 2,  17,  59,  59}, // SrcFreq=675.00Mhz ReqFreq=194.50Mhz ActFreq=194.49Mhz Error=0.008Mhz
  {195000000, 2,  13,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=195.00Mhz ActFreq=195.00Mhz Error=0.000Mhz
  {195500000, 2,  95, 328, 255}, // SrcFreq=675.00Mhz ReqFreq=195.50Mhz ActFreq=195.50Mhz Error=0.003Mhz
  {196000000, 2,   9,  31,  31}, // SrcFreq=675.00Mhz ReqFreq=196.00Mhz ActFreq=195.97Mhz Error=0.032Mhz
  {196500000, 2,  85, 292, 255}, // SrcFreq=675.00Mhz ReqFreq=196.50Mhz ActFreq=196.49Mhz Error=0.010Mhz
  {197000000, 2,  68, 233, 233}, // SrcFreq=675.00Mhz ReqFreq=197.00Mhz ActFreq=197.00Mhz Error=0.004Mhz
  {197500000, 2,  79, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=197.50Mhz ActFreq=197.50Mhz Error=0.000Mhz
  {198000000, 2,  22,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=198.00Mhz ActFreq=198.00Mhz Error=0.000Mhz
  {198500000, 2,   5,  17,  17}, // SrcFreq=675.00Mhz ReqFreq=198.50Mhz ActFreq=198.53Mhz Error=0.029Mhz
  {199000000, 2,  74, 251, 251}, // SrcFreq=675.00Mhz ReqFreq=199.00Mhz ActFreq=199.00Mhz Error=0.004Mhz
  {199500000, 2,  73, 247, 247}, // SrcFreq=675.00Mhz ReqFreq=199.50Mhz ActFreq=199.49Mhz Error=0.006Mhz
  {200000000, 2,   8,  27,  27}, // SrcFreq=675.00Mhz ReqFreq=200.00Mhz ActFreq=200.00Mhz Error=0.000Mhz
  {200500000, 2,  30, 101, 101}, // SrcFreq=675.00Mhz ReqFreq=200.50Mhz ActFreq=200.50Mhz Error=0.005Mhz
  {201000000, 2,  67, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=201.00Mhz ActFreq=201.00Mhz Error=0.000Mhz
  {201500000, 2,  20,  67,  67}, // SrcFreq=675.00Mhz ReqFreq=201.50Mhz ActFreq=201.49Mhz Error=0.007Mhz
  {202000000, 2,  41, 137, 137}, // SrcFreq=675.00Mhz ReqFreq=202.00Mhz ActFreq=202.01Mhz Error=0.007Mhz
  {202500000, 2,   3,  10,  10}, // SrcFreq=675.00Mhz ReqFreq=202.50Mhz ActFreq=202.50Mhz Error=0.000Mhz
  {203000000, 2,  40, 133, 133}, // SrcFreq=675.00Mhz ReqFreq=203.00Mhz ActFreq=203.01Mhz Error=0.008Mhz
  {203500000, 2,  41, 136, 136}, // SrcFreq=675.00Mhz ReqFreq=203.50Mhz ActFreq=203.49Mhz Error=0.007Mhz
  {204000000, 2,  68, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=204.00Mhz ActFreq=204.00Mhz Error=0.000Mhz
  {204500000, 2, 103, 340, 255}, // SrcFreq=675.00Mhz ReqFreq=204.50Mhz ActFreq=204.49Mhz Error=0.015Mhz
  {205000000, 2,  41, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=205.00Mhz ActFreq=205.00Mhz Error=0.000Mhz
  {205500000, 2, 109, 358, 255}, // SrcFreq=675.00Mhz ReqFreq=205.50Mhz ActFreq=205.52Mhz Error=0.017Mhz
  {206000000, 2, 112, 367, 255}, // SrcFreq=675.00Mhz ReqFreq=206.00Mhz ActFreq=205.99Mhz Error=0.005Mhz
  {206500000, 2,  93, 304, 255}, // SrcFreq=675.00Mhz ReqFreq=206.50Mhz ActFreq=206.50Mhz Error=0.003Mhz
  {207000000, 2,  23,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=207.00Mhz ActFreq=207.00Mhz Error=0.000Mhz
  {207500000, 2,  83, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=207.50Mhz ActFreq=207.50Mhz Error=0.000Mhz
  {208000000, 2,  53, 172, 172}, // SrcFreq=675.00Mhz ReqFreq=208.00Mhz ActFreq=207.99Mhz Error=0.006Mhz
  {208500000, 2,  80, 259, 255}, // SrcFreq=675.00Mhz ReqFreq=208.50Mhz ActFreq=208.49Mhz Error=0.006Mhz
  {209000000, 2,  74, 239, 239}, // SrcFreq=675.00Mhz ReqFreq=209.00Mhz ActFreq=209.00Mhz Error=0.004Mhz
  {209500000, 2,   9,  29,  29}, // SrcFreq=675.00Mhz ReqFreq=209.50Mhz ActFreq=209.48Mhz Error=0.017Mhz
  {210000000, 2,  14,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=210.00Mhz ActFreq=210.00Mhz Error=0.000Mhz
  {210500000, 2,  92, 295, 255}, // SrcFreq=675.00Mhz ReqFreq=210.50Mhz ActFreq=210.51Mhz Error=0.008Mhz
  {211000000, 2, 116, 371, 255}, // SrcFreq=675.00Mhz ReqFreq=211.00Mhz ActFreq=211.05Mhz Error=0.051Mhz
  {211500000, 2,  47, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=211.50Mhz ActFreq=211.50Mhz Error=0.000Mhz
  {212000000, 2,  87, 277, 255}, // SrcFreq=675.00Mhz ReqFreq=212.00Mhz ActFreq=212.00Mhz Error=0.004Mhz
  {212500000, 2,  17,  54,  54}, // SrcFreq=675.00Mhz ReqFreq=212.50Mhz ActFreq=212.50Mhz Error=0.000Mhz
  {213000000, 2,  71, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=213.00Mhz ActFreq=213.00Mhz Error=0.000Mhz
  {213500000, 2,  99, 313, 255}, // SrcFreq=675.00Mhz ReqFreq=213.50Mhz ActFreq=213.50Mhz Error=0.002Mhz
  {214000000, 2, 110, 347, 255}, // SrcFreq=675.00Mhz ReqFreq=214.00Mhz ActFreq=213.98Mhz Error=0.023Mhz
  {214500000, 2, 109, 343, 255}, // SrcFreq=675.00Mhz ReqFreq=214.50Mhz ActFreq=214.50Mhz Error=0.004Mhz
  {215000000, 2,  43, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=215.00Mhz ActFreq=215.00Mhz Error=0.000Mhz
  {215500000, 2,  68, 213, 213}, // SrcFreq=675.00Mhz ReqFreq=215.50Mhz ActFreq=215.49Mhz Error=0.007Mhz
  {216000000, 2,   8,  25,  25}, // SrcFreq=675.00Mhz ReqFreq=216.00Mhz ActFreq=216.00Mhz Error=0.000Mhz
  {216500000, 2,  17,  53,  53}, // SrcFreq=675.00Mhz ReqFreq=216.50Mhz ActFreq=216.51Mhz Error=0.009Mhz
  {217000000, 2, 118, 367, 255}, // SrcFreq=675.00Mhz ReqFreq=217.00Mhz ActFreq=217.03Mhz Error=0.030Mhz
  {217500000, 2,  29,  90,  90}, // SrcFreq=675.00Mhz ReqFreq=217.50Mhz ActFreq=217.50Mhz Error=0.000Mhz
  {218000000, 2,  83, 257, 255}, // SrcFreq=675.00Mhz ReqFreq=218.00Mhz ActFreq=218.00Mhz Error=0.004Mhz
  {218500000, 2,  56, 173, 173}, // SrcFreq=675.00Mhz ReqFreq=218.50Mhz ActFreq=218.50Mhz Error=0.003Mhz
  {219000000, 2,  73, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=219.00Mhz ActFreq=219.00Mhz Error=0.000Mhz
  {219500000, 2,  93, 286, 255}, // SrcFreq=675.00Mhz ReqFreq=219.50Mhz ActFreq=219.49Mhz Error=0.007Mhz
  {220000000, 2,  44, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=220.00Mhz ActFreq=220.00Mhz Error=0.000Mhz
  {220500000, 2,  49, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=220.50Mhz ActFreq=220.50Mhz Error=0.000Mhz
  {221000000, 2,  92, 281, 255}, // SrcFreq=675.00Mhz ReqFreq=221.00Mhz ActFreq=221.00Mhz Error=0.004Mhz
  {221500000, 2,  21,  64,  64}, // SrcFreq=675.00Mhz ReqFreq=221.50Mhz ActFreq=221.48Mhz Error=0.016Mhz
  {222000000, 2,  74, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=222.00Mhz ActFreq=222.00Mhz Error=0.000Mhz
  {222500000, 2,  89, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=222.50Mhz ActFreq=222.50Mhz Error=0.000Mhz
  {223000000, 2, 112, 339, 255}, // SrcFreq=675.00Mhz ReqFreq=223.00Mhz ActFreq=223.01Mhz Error=0.009Mhz
  {223500000, 2,  99, 299, 255}, // SrcFreq=675.00Mhz ReqFreq=223.50Mhz ActFreq=223.49Mhz Error=0.005Mhz
  {224000000, 2,  75, 226, 226}, // SrcFreq=675.00Mhz ReqFreq=224.00Mhz ActFreq=224.00Mhz Error=0.004Mhz
  {224500000, 2, 127, 382, 255}, // SrcFreq=675.00Mhz ReqFreq=224.50Mhz ActFreq=224.41Mhz Error=0.089Mhz
  {225000000, 2,   1,   3,   3}, // SrcFreq=675.00Mhz ReqFreq=225.00Mhz ActFreq=225.00Mhz Error=0.000Mhz
  {225500000, 2, 128, 383, 255}, // SrcFreq=675.00Mhz ReqFreq=225.50Mhz ActFreq=225.59Mhz Error=0.087Mhz
  {226000000, 2,  75, 224, 224}, // SrcFreq=675.00Mhz ReqFreq=226.00Mhz ActFreq=226.00Mhz Error=0.004Mhz
  {226500000, 2, 101, 301, 255}, // SrcFreq=675.00Mhz ReqFreq=226.50Mhz ActFreq=226.50Mhz Error=0.005Mhz
  {227000000, 2,  38, 113, 113}, // SrcFreq=675.00Mhz ReqFreq=227.00Mhz ActFreq=226.99Mhz Error=0.009Mhz
  {227500000, 2,  91, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=227.50Mhz ActFreq=227.50Mhz Error=0.000Mhz
  {228000000, 2,  76, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=228.00Mhz ActFreq=228.00Mhz Error=0.000Mhz
  {228500000, 2,  87, 257, 255}, // SrcFreq=675.00Mhz ReqFreq=228.50Mhz ActFreq=228.50Mhz Error=0.002Mhz
  {229000000, 2, 115, 339, 255}, // SrcFreq=675.00Mhz ReqFreq=229.00Mhz ActFreq=228.98Mhz Error=0.018Mhz
  {229500000, 2,  17,  50,  50}, // SrcFreq=675.00Mhz ReqFreq=229.50Mhz ActFreq=229.50Mhz Error=0.000Mhz
  {230000000, 2,  46, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=230.00Mhz ActFreq=230.00Mhz Error=0.000Mhz
  {230500000, 2,  14,  41,  41}, // SrcFreq=675.00Mhz ReqFreq=230.50Mhz ActFreq=230.49Mhz Error=0.012Mhz
  {231000000, 2,  77, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=231.00Mhz ActFreq=231.00Mhz Error=0.000Mhz
  {231500000, 2,  95, 277, 255}, // SrcFreq=675.00Mhz ReqFreq=231.50Mhz ActFreq=231.50Mhz Error=0.002Mhz
  {232000000, 2, 133, 387, 255}, // SrcFreq=675.00Mhz ReqFreq=232.00Mhz ActFreq=231.98Mhz Error=0.023Mhz
  {232500000, 2,  31,  90,  90}, // SrcFreq=675.00Mhz ReqFreq=232.50Mhz ActFreq=232.50Mhz Error=0.000Mhz
  {233000000, 2,  68, 197, 197}, // SrcFreq=675.00Mhz ReqFreq=233.00Mhz ActFreq=232.99Mhz Error=0.005Mhz
  {233500000, 2, 119, 344, 255}, // SrcFreq=675.00Mhz ReqFreq=233.50Mhz ActFreq=233.50Mhz Error=0.003Mhz
  {234000000, 2,  26,  75,  75}, // SrcFreq=675.00Mhz ReqFreq=234.00Mhz ActFreq=234.00Mhz Error=0.000Mhz
  {234500000, 2, 107, 308, 255}, // SrcFreq=675.00Mhz ReqFreq=234.50Mhz ActFreq=234.50Mhz Error=0.003Mhz
  {235000000, 2,  47, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=235.00Mhz ActFreq=235.00Mhz Error=0.000Mhz
  {235500000, 2, 127, 364, 255}, // SrcFreq=675.00Mhz ReqFreq=235.50Mhz ActFreq=235.51Mhz Error=0.008Mhz
  {236000000, 2,  93, 266, 255}, // SrcFreq=675.00Mhz ReqFreq=236.00Mhz ActFreq=236.00Mhz Error=0.004Mhz
  {236500000, 2,  48, 137, 137}, // SrcFreq=675.00Mhz ReqFreq=236.50Mhz ActFreq=236.50Mhz Error=0.004Mhz
  {237000000, 2,  79, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=237.00Mhz ActFreq=237.00Mhz Error=0.000Mhz
  {237500000, 2,  19,  54,  54}, // SrcFreq=675.00Mhz ReqFreq=237.50Mhz ActFreq=237.50Mhz Error=0.000Mhz
  {238000000, 2,  61, 173, 173}, // SrcFreq=675.00Mhz ReqFreq=238.00Mhz ActFreq=238.01Mhz Error=0.006Mhz
  {238500000, 2,  53, 150, 150}, // SrcFreq=675.00Mhz ReqFreq=238.50Mhz ActFreq=238.50Mhz Error=0.000Mhz
  {239000000, 2,  74, 209, 209}, // SrcFreq=675.00Mhz ReqFreq=239.00Mhz ActFreq=239.00Mhz Error=0.005Mhz
  {239500000, 2,  11,  31,  31}, // SrcFreq=675.00Mhz ReqFreq=239.50Mhz ActFreq=239.52Mhz Error=0.016Mhz
  {240000000, 2,  16,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=240.00Mhz ActFreq=240.00Mhz Error=0.000Mhz
  {240500000, 2, 119, 334, 255}, // SrcFreq=675.00Mhz ReqFreq=240.50Mhz ActFreq=240.49Mhz Error=0.006Mhz
  {241000000, 2, 141, 395, 255}, // SrcFreq=675.00Mhz ReqFreq=241.00Mhz ActFreq=240.95Mhz Error=0.051Mhz
  {241500000, 2, 122, 341, 255}, // SrcFreq=675.00Mhz ReqFreq=241.50Mhz ActFreq=241.50Mhz Error=0.004Mhz
  {242000000, 2, 128, 357, 255}, // SrcFreq=675.00Mhz ReqFreq=242.00Mhz ActFreq=242.02Mhz Error=0.017Mhz
  {242500000, 2,  97, 270, 255}, // SrcFreq=675.00Mhz ReqFreq=242.50Mhz ActFreq=242.50Mhz Error=0.000Mhz
  {243000000, 2,   9,  25,  25}, // SrcFreq=675.00Mhz ReqFreq=243.00Mhz ActFreq=243.00Mhz Error=0.000Mhz
  {243500000, 2, 136, 377, 255}, // SrcFreq=675.00Mhz ReqFreq=243.50Mhz ActFreq=243.50Mhz Error=0.001Mhz
  {244000000, 2, 137, 379, 255}, // SrcFreq=675.00Mhz ReqFreq=244.00Mhz ActFreq=244.00Mhz Error=0.003Mhz
  {244500000, 2, 117, 323, 255}, // SrcFreq=675.00Mhz ReqFreq=244.50Mhz ActFreq=244.50Mhz Error=0.005Mhz
  {245000000, 2,  49, 135, 135}, // SrcFreq=675.00Mhz ReqFreq=245.00Mhz ActFreq=245.00Mhz Error=0.000Mhz
  {245500000, 2,   4,  11,  11}, // SrcFreq=675.00Mhz ReqFreq=245.50Mhz ActFreq=245.45Mhz Error=0.045Mhz
  {246000000, 2,  82, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=246.00Mhz ActFreq=246.00Mhz Error=0.000Mhz
  {246500000, 2, 107, 293, 255}, // SrcFreq=675.00Mhz ReqFreq=246.50Mhz ActFreq=246.50Mhz Error=0.002Mhz
  {247000000, 2, 131, 358, 255}, // SrcFreq=675.00Mhz ReqFreq=247.00Mhz ActFreq=247.00Mhz Error=0.003Mhz
  {247500000, 2,  11,  30,  30}, // SrcFreq=675.00Mhz ReqFreq=247.50Mhz ActFreq=247.50Mhz Error=0.000Mhz
  {248000000, 2, 133, 362, 255}, // SrcFreq=675.00Mhz ReqFreq=248.00Mhz ActFreq=248.00Mhz Error=0.003Mhz
  {248500000, 2, 141, 383, 255}, // SrcFreq=675.00Mhz ReqFreq=248.50Mhz ActFreq=248.50Mhz Error=0.001Mhz
  {249000000, 2,  83, 225, 225}, // SrcFreq=675.00Mhz ReqFreq=249.00Mhz ActFreq=249.00Mhz Error=0.000Mhz
  {249500000, 2, 129, 349, 255}, // SrcFreq=675.00Mhz ReqFreq=249.50Mhz ActFreq=249.50Mhz Error=0.001Mhz
  {250000000, 2,  10,  27,  27}, // SrcFreq=675.00Mhz ReqFreq=250.00Mhz ActFreq=250.00Mhz Error=0.000Mhz
  {255000000, 2,  17,  45,  45}, // SrcFreq=675.00Mhz ReqFreq=255.00Mhz ActFreq=255.00Mhz Error=0.000Mhz
  {260000000, 2,  52, 135,  135},// SrcFreq=675.00Mhz ReqFreq=260.00Mhz ActFreq=260.00Mhz Error=0.000Mhz    
  {265000000, 2,  53, 135,  135},// SrcFreq=675.00Mhz ReqFreq=265.00Mhz ActFreq=265.00Mhz Error=0.000Mhz
  {270000000, 2,   2,   5,    5},// SrcFreq=675.00Mhz ReqFreq=270.00Mhz ActFreq=270.00Mhz Error=0.000Mhz
};

#endif //_DP_FREQTABLE_H_
