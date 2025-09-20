/*! \file  pm_config_target_pbs_ram.c 
 *   
 *  \brief  File Contains the PMIC Set Mode Driver Implementation 
 *  \details Set Mode Driver implementation is responsible for setting and getting 
 *  all mode settings such as Register values, memory values, etc. 
 *   
 *    PMIC code generation Version: 1.0.0.0 
 *    Date: 12/4/2018 
 *    PMIC PSI Version: MSM8998-SBL-12042018-b0x01_v0x1E - Approved 
 *    PBS RAM Version: PM8998_RAM_b0x01_v0x0D 
 *    PBS RAM Version: PM8005_RAM_b0x01_v0x04 
 *    PBS RAM Version: PMi8998_RAM_b0x01_v0x0B 
 *    This file contains code for Target specific settings and modes. 
 *   
 *   Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
 */ 
 
/*=========================================================================== 
 
                        EDIT HISTORY FOR MODULE 
 
  This document is created by a code generator, therefore this section will 
  not contain comments describing changes made to the module. 
 
$Header: //components/rel/boot.xf/2.0/QcomPkg/Library/PmicLib/target/sdm845_pm8998_pmi8998/psi/pm_config_target_pbs_ram.c#25 $  
$DateTime: 2019/01/07 03:26:04 $  $Author: pwbldsvc $ 
 
when       who     what, where, why 
--------   ---     ----------------------------------------------------------  
 
===========================================================================*/ 
 
/*=========================================================================== 
 
                     INCLUDE FILES  
 
===========================================================================*/ 
 
#include "pm_target_information.h" 
#include "pm_config_sbl.h" 
 
 
/*========================== PBS RAM LUT =============================*/ 
 
// To handle multiple PBS RAM configuration for different rev of the same PMIC or for multiple PMICs,
// a double dimension array of PBS RAM data is used. The data field of the specific command (in pm_config_target_sbl_sequence.c)
// to program PBS RAM will hold the index to the PBS RAM that needs to be used. during programming.
// Example:
// 1.sid; 2.data; 3.base_address; 4.offset;  5.reg_operation; 6.rev_id_operation; 7.rev_id;
// {0, 0x00, 0x0000, 0x000, PM_SBL_PBS_RAM, EQUAL, REV_ID_2_0},  //data = 0:  Use the 1st set of PBS RAM data if PMIC Rev ID = REV_ID_2_0
// {0, 0x01, 0x0000, 0x000, PM_SBL_PBS_RAM, EQUAL, REV_ID_1_0},  //data = 1:  Use the 2nd set of PBS RAM data if PMIC Rev ID = REV_ID_1_0

pm_pbs_ram_data_type 
pm_pbs_seq [ ][PBS_RAM_DATA_SIZE]  = 
{
  /* PM8998_RAM_b0x01_v0x0D */
	{
	 // data, offset, baseaddr, sid 
	  { 0x52, 0x53, 0x42, 0x50 }, /*W# 0  Src Line# 130 - ; ASCII for PBSR".  Later changed to Check Sum."*/
	  { 0x01, 0x0D, 0x01, 0x18 }, /*W# 1  Src Line# 131 - ; 18=Dataset Start, 01=Header Ver, 0D01 = Rev D, Branch 1*/
	  { 0x2C, 0x04, 0xFF, 0x83 }, /*W# 2  Src Line# 132 - */
	  { 0x6C, 0x04, 0xFF, 0x83 }, /*W# 3  Src Line# 133 - */
	  { 0xE0, 0x04, 0xFF, 0x83 }, /*W# 4  Src Line# 134 - */
	  { 0x58, 0x05, 0xFF, 0x83 }, /*W# 5  Src Line# 135 - */
	  { 0xC4, 0x0B, 0xFF, 0x83 }, /*W# 6  Src Line# 137 - Fixed location for SLEEP dataset.  Do NOT move this GOTO line.*/
	  { 0xE0, 0x0B, 0xFF, 0x83 }, /*W# 7  Src Line# 138 - Fixed location for PON REASONS dataset.  Do NOT move this GOTO line.*/
	  { 0xA4, 0x0B, 0xFF, 0x83 }, /*W# 8  Src Line# 139 - Fixed location for WARM_RESET_DATASET.  Do NOT move this GOTO line.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 9  Src Line# 140 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 10  Src Line# 141 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 11  Src Line# 147 - */
	  { 0x00, 0x57, 0x08, 0x00 }, /*W# 12  Src Line# 148 - 1BYTE WRITE 0x00 TO PON__PMIC_WD_RESET_S2_CTL2; Disable PMIC WD during WR/POFF/FAULT*/
	  { 0xFF, 0x48, 0x71, 0x00 }, /*W# 13  Src Line# 149 - 1BYTE WRITE 0xFF TO PBS_CLIENT0__TRIG_LATCHED_CLR; Clear all latched sequences in PBS PON Client 0*/
	  { 0xFB, 0x08, 0x08, 0x40 }, /*W# 14  Src Line# 150 - 1BYTE READ PON__FSM_STATUS TO RB; Readback PON FSM Status and store to Buffer RB*/
	  { 0x04, 0xB0, 0xFF, 0x90 }, /*W# 15  Src Line# 151 - SKIP 1 Lines if RB NOT EQUAL 0x04 (MASK=0xFF); DVDD_CONFIG = State 4*/
	  { 0x10, 0x00, 0xFF, 0x83 }, /*W# 16  Src Line# 153 - ; GOTO OTP_DVDD_CONFIG (OTP).  Using RAW command due to PDM linkage issues.*/
	  { 0x01, 0xB0, 0xFF, 0x90 }, /*W# 17  Src Line# 156 - SKIP 1 Lines if RB NOT EQUAL 0x01 (MASK=0xFF); PON = State 1*/
	  { 0x10, 0x00, 0xFF, 0x83 }, /*W# 18  Src Line# 158 - ; GOTO OTP_DVDD_CONFIG (OTP).  Using RAW command due to PDM linkage issues.*/
	  { 0x06, 0xB0, 0xFF, 0x90 }, /*W# 19  Src Line# 161 - SKIP 1 Lines if RB NOT EQUAL 0x06 (MASK=0xFF); FAULT = State 6*/
	  { 0xD0, 0x05, 0xFF, 0x83 }, /*W# 20  Src Line# 163 - */
	  { 0x02, 0xB0, 0xFF, 0x90 }, /*W# 21  Src Line# 166 - SKIP 1 Lines if RB NOT EQUAL 0x02 (MASK=0xFF); POFF = State 2*/
	  { 0x94, 0x06, 0xFF, 0x83 }, /*W# 22  Src Line# 168 - */
	  { 0x07, 0xB0, 0xFF, 0x90 }, /*W# 23  Src Line# 171 - SKIP 1 Lines if RB NOT EQUAL 0x07 (MASK=0xFF); WARM RESET = State 7*/
	  { 0xFC, 0x05, 0xFF, 0x83 }, /*W# 24  Src Line# 173 - */
	  { 0x00, 0x91, 0x08, 0x00 }, /*W# 25  Src Line# 176 - 1BYTE WRITE 0x00 TO PON__PBS_INTERFACE; Undefined State - NACK PON (Note:  None of the Client GOTOs have a 'RETURN'; the ACK is done in each subroutine)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 26  Src Line# 177 - End of Sequence; Byte-2*/
	  { 0xF3, 0x0A, 0x72, 0x40 }, /*W# 27  Src Line# 183 - 1BYTE READ PBS_CLIENT1__TRIG_LATCHED TO R3; Read Client 1 trigger latch status and store to Buffer R3*/
	  { 0x01, 0x30, 0x01, 0x92 }, /*W# 28  Src Line# 184 - SKIP 3 Lines if R3 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / Spare*/
	  { 0x01, 0x48, 0x72, 0x00 }, /*W# 29  Src Line# 186 - 1BYTE WRITE 0x01 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 30  Src Line# 187 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 31  Src Line# 188 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x02, 0x30, 0x02, 0x92 }, /*W# 32  Src Line# 191 - SKIP 8 Lines if R3 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = EOSR*/
	  { 0x02, 0x48, 0x72, 0x00 }, /*W# 33  Src Line# 193 - 1BYTE WRITE 0x02 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 34  Src Line# 194 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 35  Src Line# 195 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x04, 0x30, 0x04, 0x92 }, /*W# 36  Src Line# 198 - SKIP 3 Lines if R3 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = sleep_b*/
	  { 0x04, 0x48, 0x72, 0x00 }, /*W# 37  Src Line# 200 - 1BYTE WRITE 0x04 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 38  Src Line# 201 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 39  Src Line# 202 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x08, 0x30, 0x08, 0x92 }, /*W# 40  Src Line# 205 - SKIP 3 Lines if R3 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = LN_BB_CLK1_EN*/
	  { 0x08, 0x48, 0x72, 0x00 }, /*W# 41  Src Line# 207 - 1BYTE WRITE 0x08 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 42  Src Line# 208 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 43  Src Line# 209 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x10, 0x30, 0x10, 0x91 }, /*W# 44  Src Line# 212 - SKIP 3 Lines if R3 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = OCP (non-core VREG)*/
	  { 0x10, 0x48, 0x72, 0x00 }, /*W# 45  Src Line# 214 - 1BYTE WRITE 0x10 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4*/
	  { 0xB0, 0x0A, 0xFF, 0x83 }, /*W# 46  Src Line# 215 - */
	  { 0x20, 0x30, 0x20, 0x92 }, /*W# 47  Src Line# 218 - SKIP 5 Lines if R3 NOT EQUAL 0x20 (MASK=0x20); Bit 5 - Spare*/
	  { 0x20, 0x48, 0x72, 0x00 }, /*W# 48  Src Line# 220 - 1BYTE WRITE 0x20 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 5*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 49  Src Line# 221 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 50  Src Line# 222 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x80, 0x30, 0x80, 0x92 }, /*W# 51  Src Line# 225 - SKIP 3 Lines if R3 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger (Sleep/Wake)*/
	  { 0x80, 0x48, 0x72, 0x00 }, /*W# 52  Src Line# 227 - 1BYTE WRITE 0x80 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7*/
	  { 0x8C, 0x07, 0xFF, 0x83 }, /*W# 53  Src Line# 228 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 54  Src Line# 229 - NOP; TBD*/
	  { 0x68, 0x07, 0xFF, 0x83 }, /*W# 55  Src Line# 232 - GOTO RAM_MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT*/
	  { 0xF4, 0x0A, 0x73, 0x40 }, /*W# 56  Src Line# 238 - 1BYTE READ PBS_CLIENT2__TRIG_LATCHED TO R4; Read Client 2 trigger latch status and store to Buffer R4*/
	  { 0x01, 0x40, 0x01, 0x92 }, /*W# 57  Src Line# 239 - SKIP 3 Lines if R4 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / Spare*/
	  { 0x01, 0x48, 0x73, 0x00 }, /*W# 58  Src Line# 241 - 1BYTE WRITE 0x01 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 59  Src Line# 242 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 60  Src Line# 243 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x02, 0x40, 0x02, 0x92 }, /*W# 61  Src Line# 246 - SKIP 3 Lines if R4 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = EOSR*/
	  { 0x02, 0x48, 0x73, 0x00 }, /*W# 62  Src Line# 248 - 1BYTE WRITE 0x02 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 63  Src Line# 249 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 64  Src Line# 250 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x04, 0x40, 0x04, 0x92 }, /*W# 65  Src Line# 253 - SKIP 3 Lines if R4 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = GPIO23 (WCSS_VCTRL)*/
	  { 0x04, 0x48, 0x73, 0x00 }, /*W# 66  Src Line# 255 - 1BYTE WRITE 0x04 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2*/
	  { 0x04, 0x09, 0xFF, 0x83 }, /*W# 67  Src Line# 256 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 68  Src Line# 257 - NOP; TBD*/
	  { 0x08, 0x40, 0x08, 0x92 }, /*W# 69  Src Line# 260 - SKIP 3 Lines if R4 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = GPIO3 (WLAN_RF_VCTRL)*/
	  { 0x08, 0x48, 0x73, 0x00 }, /*W# 70  Src Line# 262 - 1BYTE WRITE 0x08 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3*/
	  { 0xAC, 0x09, 0xFF, 0x83 }, /*W# 71  Src Line# 263 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 72  Src Line# 264 - */
	  { 0x10, 0x40, 0x10, 0x92 }, /*W# 73  Src Line# 267 - SKIP 3 Lines if R4 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = GPIO2 (spare PBS trigger)*/
	  { 0x10, 0x48, 0x73, 0x00 }, /*W# 74  Src Line# 269 - 1BYTE WRITE 0x10 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 75  Src Line# 270 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 76  Src Line# 271 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x20, 0x40, 0x20, 0x92 }, /*W# 77  Src Line# 274 - SKIP 3 Lines if R4 NOT EQUAL 0x20 (MASK=0x20); Bit 5 - GPIO20 (EUD)*/
	  { 0x20, 0x48, 0x73, 0x00 }, /*W# 78  Src Line# 276 - */
	  { 0x48, 0x09, 0xFF, 0x83 }, /*W# 79  Src Line# 277 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 80  Src Line# 278 - */
	  { 0x80, 0x40, 0x80, 0x92 }, /*W# 81  Src Line# 281 - SKIP 3 Lines if R4 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger*/
	  { 0x80, 0x48, 0x73, 0x00 }, /*W# 82  Src Line# 283 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 83  Src Line# 284 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 84  Src Line# 285 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x68, 0x07, 0xFF, 0x83 }, /*W# 85  Src Line# 288 - GOTO RAM_MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT*/
	  { 0xF5, 0x0A, 0x74, 0x40 }, /*W# 86  Src Line# 294 - 1BYTE READ PBS_CLIENT3__TRIG_LATCHED TO R5; Read Client 3 trigger latch status and store to Buffer R5*/
	  { 0x01, 0x50, 0x01, 0x92 }, /*W# 87  Src Line# 295 - SKIP 3 Lines if R5 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / Spare*/
	  { 0x01, 0x48, 0x74, 0x00 }, /*W# 88  Src Line# 297 - 1BYTE WRITE 0x01 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 89  Src Line# 298 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 90  Src Line# 299 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x02, 0x50, 0x02, 0x92 }, /*W# 91  Src Line# 302 - SKIP 3 Lines if R5 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = GPIO4 (spare)*/
	  { 0x02, 0x48, 0x74, 0x00 }, /*W# 92  Src Line# 304 - 1BYTE WRITE 0x02 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 93  Src Line# 305 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 94  Src Line# 306 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x04, 0x50, 0x04, 0x92 }, /*W# 95  Src Line# 309 - SKIP 3 Lines if R5 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = BA_N*/
	  { 0x04, 0x48, 0x74, 0x00 }, /*W# 96  Src Line# 311 - 1BYTE WRITE 0x04 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 97  Src Line# 312 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 98  Src Line# 313 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x08, 0x50, 0x08, 0x92 }, /*W# 99  Src Line# 316 - SKIP 3 Lines if R5 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = GPIO1 (spare)*/
	  { 0x08, 0x48, 0x74, 0x00 }, /*W# 100  Src Line# 318 - 1BYTE WRITE 0x08 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 101  Src Line# 319 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 102  Src Line# 320 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x10, 0x50, 0x10, 0x92 }, /*W# 103  Src Line# 324 - SKIP 3 Lines if R5 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = RTC Timer*/
	  { 0x10, 0x48, 0x74, 0x00 }, /*W# 104  Src Line# 326 - 1BYTE WRITE 0x10 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 105  Src Line# 327 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 106  Src Line# 328 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x20, 0x50, 0x20, 0x92 }, /*W# 107  Src Line# 331 - SKIP 3 Lines if R5 NOT EQUAL 0x20 (MASK=0x20); Bit 5 - RTC Alarm*/
	  { 0x20, 0x48, 0x74, 0x00 }, /*W# 108  Src Line# 333 - 1BYTE WRITE 0x20 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 5*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 109  Src Line# 334 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 110  Src Line# 335 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x80, 0x50, 0x80, 0x92 }, /*W# 111  Src Line# 338 - SKIP 3 Lines if R5 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger*/
	  { 0x80, 0x48, 0x74, 0x00 }, /*W# 112  Src Line# 340 - 1BYTE WRITE 0x80 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 113  Src Line# 341 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 114  Src Line# 342 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x68, 0x07, 0xFF, 0x83 }, /*W# 115  Src Line# 345 - GOTO RAM_MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 116  Src Line# 349 - */
	  { 0xD8, 0x09, 0xFF, 0x9B }, /*W# 117  Src Line# 350 - GOTO_SUB_NOID RECORD_PON_X_REASONS; Record PON REASONS into dataset (circular buffer with 3 iterations)*/
	  { 0xE4, 0x80, 0x08, 0x00 }, /*W# 118  Src Line# 352 - Re-initialize PON-trigger enables (in case SW disables them)*/
	  { 0x40, 0x0A, 0xFF, 0x9B }, /*W# 119  Src Line# 354 - */
	  { 0xEA, 0x01, 0xFF, 0xC1 }, /*W# 120  Src Line# 356 - WAIT 15016 us; Wait 15ms for VREGs to discharge completely before allowing the next PON trigger*/
	  { 0xF6, 0xC8, 0x08, 0x40 }, /*W# 121  Src Line# 357 - 1BYTE READ PON__FAULT_REASON1 TO R6; Read FAULT_REASON1 and store to Buffer R6*/
	  { 0x40, 0x61, 0x40, 0x90 }, /*W# 122  Src Line# 358 - SKIP 1 Lines if R6 EQUAL 0x40 (MASK=0x40); Skip the next line if bit 6 (UVLO) = 1*/
	  { 0x00, 0x7F, 0x08, 0x00 }, /*W# 123  Src Line# 360 - 1BYTE WRITE 0x00 TO PON__SMPL_CTL; Else*/
	  { 0xEA, 0x01, 0xFF, 0xC1 }, /*W# 124  Src Line# 365 - WAIT 15016 us; Wait 15ms to ensure PM8998 is the last to complete*/
	  { 0x40, 0x91, 0x08, 0x00 }, /*W# 125  Src Line# 366 - 1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 126  Src Line# 367 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 127  Src Line# 371 - */
	  { 0x70, 0x07, 0xFF, 0x9B }, /*W# 128  Src Line# 372 - GOTO_SUB_NOID ASSERT_EXT_RESET; Set PON_RESET_N and PON_OUT low*/
	  { 0xD8, 0x09, 0xFF, 0x9B }, /*W# 129  Src Line# 373 - GOTO_SUB_NOID RECORD_PON_X_REASONS; Record PON REASONS into dataset (circular buffer with 3 iterations)*/
	  { 0x8E, 0x01, 0xFF, 0xC1 }, /*W# 130  Src Line# 374 - WAIT 12208 us; Wait 400 SLEEP_CLK cycles (12.2ms) before setting PON_OUT high again (to signal Warm Reset to slave PMIC's)*/
	  { 0x40, 0x49, 0x09, 0x10 }, /*W# 131  Src Line# 375 - 1BYTE WRITE 0x40 TO MISC__PON_OUT_CTL; Send PON_OUT high to all PMIC's.  Changing to SEC ACCESS (to updated in next .csv).*/
	  { 0x02, 0x98, 0x08, 0x10 }, /*W# 132  Src Line# 376 - 1BYTE WRITE 0x02 TO PON__RESET_CTL; Assert warm_rb*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 133  Src Line# 377 - Wait 3 SLEEP_CLK (1+2) cycles for reset changes to propagate through clock domains*/
	  { 0x06, 0x98, 0x08, 0x10 }, /*W# 134  Src Line# 378 - 1BYTE WRITE 0x06 TO PON__RESET_CTL; De-assert warm_rb*/
	  { 0x07, 0x45, 0x1D, 0x01 }, /*W# 135  Src Line# 387 - PX3*/
	  { 0x07, 0x45, 0x1A, 0x01 }, /*W# 136  Src Line# 388 - LV LDO's*/
	  { 0x07, 0x45, 0x20, 0x01 }, /*W# 137  Src Line# 389 - HV LDO's*/
	  { 0x07, 0x45, 0x26, 0x01 }, /*W# 138  Src Line# 390 - LV LDO's*/
	  { 0x48, 0x40, 0x1A, 0x01 }, /*W# 139  Src Line# 402 - Set S3 to 1.352V*/
	  { 0x05, 0x41, 0x1A, 0x01 }, /*W# 140  Src Line# 403 - Set S3 to 1.352V*/
	  { 0xF8, 0x40, 0x20, 0x01 }, /*W# 141  Src Line# 404 - Set S5 to 2.04V*/
	  { 0x07, 0x41, 0x20, 0x01 }, /*W# 142  Src Line# 405 - Set S5 to 2.04V*/
	  { 0x10, 0x40, 0x26, 0x01 }, /*W# 143  Src Line# 406 - Set S7 to 1.04V*/
	  { 0x04, 0x41, 0x26, 0x01 }, /*W# 144  Src Line# 407 - Set S7 to 1.04V*/
	  { 0x80, 0x46, 0x26, 0x01 }, /*W# 145  Src Line# 409 - Enable S7 if collapsed*/
	  { 0xFC, 0x56, 0x09, 0x40 }, /*W# 146  Src Line# 413 - Read PM8998.MISC.PC_SPARE1 (scratch for Pass 0/1/2) and store to RC*/
	  { 0xA4, 0x0B, 0x76, 0x29 }, /*W# 147  Src Line# 415 - Load R6 with 0xBA4 (start of WARM RESET DATASET)*/
	  { 0xF8, 0x0A, 0x23, 0x89 }, /*W# 148  Src Line# 416 - */
	  { 0xF8, 0x0A, 0x43, 0x89 }, /*W# 149  Src Line# 417 - */
	  { 0xF8, 0x0A, 0x14, 0x89 }, /*W# 150  Src Line# 418 - */
	  { 0xF8, 0x0A, 0x2C, 0x89 }, /*W# 151  Src Line# 419 - */
	  { 0xF8, 0x0A, 0x5A, 0x89 }, /*W# 152  Src Line# 420 - */
	  { 0xF8, 0x0A, 0x38, 0x89 }, /*W# 153  Src Line# 421 - */
	  { 0xF8, 0x0A, 0x35, 0x89 }, /*W# 154  Src Line# 422 - */
	  { 0x01, 0x00, 0x7C, 0x20 }, /*W# 155  Src Line# 424 - Increment RC by 1*/
	  { 0xFC, 0x56, 0x09, 0x08 }, /*W# 156  Src Line# 425 - */
	  { 0x21, 0x07, 0xFF, 0xC1 }, /*W# 157  Src Line# 443 - Wait 55ms for PMi to complete its PON sequence (rerun during WR)*/
	  { 0x9B, 0x19, 0x76, 0x29 }, /*W# 158  Src Line# 445 - ; MOVI 2BYTE R6=0x199B.  2+0x199B x 30.5us = 200ms.  Store to Buffer R6.*/
	  { 0x99, 0x00, 0x7D, 0x28 }, /*W# 159  Src Line# 446 - ; 1BYTE OFFSET=0x99.  Set offset to 0x99 (PON_RESET_N_CTL) for next GOTO_SUB*/
	  { 0x18, 0x0C, 0x08, 0x88 }, /*W# 160  Src Line# 447 - ; Using RAW due to PDM Bug.  This SUB will deassert PON_RESET_N and wait 200ms for PS_HOLD to go high*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 161  Src Line# 448 - */
	  { 0x40, 0x91, 0x08, 0x00 }, /*W# 162  Src Line# 449 - 1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 163  Src Line# 450 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 164  Src Line# 451 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 165  Src Line# 455 - */
	  { 0x00, 0x7F, 0x08, 0x00 }, /*W# 166  Src Line# 456 - 1BYTE WRITE 0x00 TO PON__SMPL_CTL; Disable SMPL*/
	  { 0x70, 0x07, 0xFF, 0x9B }, /*W# 167  Src Line# 457 - GOTO_SUB_NOID ASSERT_EXT_RESET; Set PON_RESET_N and PON_OUT low*/
	  { 0xD8, 0x09, 0xFF, 0x9B }, /*W# 168  Src Line# 458 - GOTO_SUB_NOID RECORD_PON_X_REASONS; Record PON REASONS into dataset (circular buffer with 3 iterations)*/
	  { 0x04, 0x98, 0x08, 0x10 }, /*W# 169  Src Line# 459 - 1BYTE WRITE 0x04 TO PON__RESET_CTL; Assert shutdown1_rb*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 170  Src Line# 460 - WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains*/
	  { 0x06, 0x98, 0x08, 0x10 }, /*W# 171  Src Line# 461 - 1BYTE WRITE 0x06 TO PON__RESET_CTL; De-assert shutdown1_rb*/
	  { 0x40, 0x0A, 0xFF, 0x9B }, /*W# 172  Src Line# 464 - */
	  { 0x5C, 0x07, 0x38, 0x89 }, /*W# 173  Src Line# 466 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S13_CTRL; APC (Kryo Silver)*/
	  { 0x5C, 0x07, 0x54, 0x89 }, /*W# 174  Src Line# 467 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO21; SD 2.95V*/
	  { 0x5C, 0x07, 0x4C, 0x89 }, /*W# 175  Src Line# 468 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO13; SD I/O (PX2) 2.95V*/
	  { 0x5C, 0x07, 0x41, 0x89 }, /*W# 176  Src Line# 469 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO02; UFS 1.2V*/
	  { 0x5C, 0x07, 0x53, 0x89 }, /*W# 177  Src Line# 470 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO20; UFS 2.95V*/
	  { 0x5C, 0x07, 0x57, 0x89 }, /*W# 178  Src Line# 471 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO24; USB 3.075V*/
	  { 0x00, 0x47, 0x57, 0x01 }, /*W# 179  Src Line# 472 - Set LDO24 FOLLOW_HW_EN to 0 (set in EUD).  Other EUD LDO's (L1, L5, L12 are after BBCLK is disabled)*/
	  { 0x8D, 0x02, 0xFF, 0xC1 }, /*W# 180  Src Line# 474 - Wait 20ms for PM8005 to interpret PON_OUT low as shutdown (low for >20ms).  Disable S3C prior to S2A.*/
	  { 0x5C, 0x07, 0x17, 0x89 }, /*W# 181  Src Line# 475 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S2_CTRL; LPDDR4*/
	  { 0x5C, 0x07, 0x51, 0x88 }, /*W# 182  Src Line# 477 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LN_BB_CLK1; LNBBCLK1*/
	  { 0x00, 0x47, 0x4B, 0x01 }, /*W# 183  Src Line# 479 - Set LDO12 FOLLOW_HW_EN to 0 (set in SBL)*/
	  { 0x5C, 0x07, 0x4B, 0x89 }, /*W# 184  Src Line# 480 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO12; LNBBCLK1 and USB 1.8V*/
	  { 0x5C, 0x07, 0x40, 0x89 }, /*W# 185  Src Line# 482 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO01; UFS_PHY and USB 0.875V*/
	  { 0x5C, 0x07, 0x59, 0x89 }, /*W# 186  Src Line# 483 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO26; USB and UFS 1.2V*/
	  { 0x5C, 0x07, 0xCD, 0x88 }, /*W# 187  Src Line# 485 - GOTO DISABLE_WAIT_FUNCTION_LOCAL GPIO14; Stark CLK*/
	  { 0x5C, 0x07, 0x55, 0x88 }, /*W# 188  Src Line# 486 - GOTO DISABLE_WAIT_FUNCTION_LOCAL RF_CLK2; WLAN 38.4MHz*/
	  { 0x5C, 0x07, 0x5A, 0x88 }, /*W# 189  Src Line# 487 - */
	  { 0x5C, 0x07, 0x58, 0x89 }, /*W# 190  Src Line# 489 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO25; WLAN CHAN0 3.3V*/
	  { 0x5C, 0x07, 0x50, 0x89 }, /*W# 191  Src Line# 490 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO17; WLAN 1.3V*/
	  { 0x5C, 0x07, 0x44, 0x89 }, /*W# 192  Src Line# 491 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO05; WCSS CX/MX 0.8V*/
	  { 0x5C, 0x07, 0x46, 0x89 }, /*W# 193  Src Line# 492 - GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO07; WLAN XO 1.8V*/
	  { 0x5C, 0x07, 0x0B, 0x88 }, /*W# 194  Src Line# 494 - GOTO DISABLE_WAIT_FUNCTION_LOCAL VREF_MSM; 1.25V reference for UIM and SD*/
	  { 0x5C, 0x07, 0x1D, 0x89 }, /*W# 195  Src Line# 495 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S4_CTRL; 1.8V I/O*/
	  { 0x5C, 0x07, 0x4A, 0x89 }, /*W# 196  Src Line# 497 - */
	  { 0x5C, 0x07, 0x1A, 0x89 }, /*W# 197  Src Line# 498 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S3_CTRL; Required for L17 and L26*/
	  { 0x5C, 0x07, 0x5A, 0x89 }, /*W# 198  Src Line# 499 - */
	  { 0x5C, 0x07, 0x2C, 0x89 }, /*W# 199  Src Line# 500 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S9_CTRL; VDD_CX*/
	  { 0x5C, 0x07, 0x14, 0x89 }, /*W# 200  Src Line# 501 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S1_CTRL; EBI_PHY*/
	  { 0x5C, 0x07, 0x43, 0x89 }, /*W# 201  Src Line# 502 - */
	  { 0x5C, 0x07, 0x23, 0x89 }, /*W# 202  Src Line# 503 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S6_CTRL; VDD_MX*/
	  { 0x5C, 0x07, 0x26, 0x89 }, /*W# 203  Src Line# 504 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S7_CTRL; Required for L1*/
	  { 0x5C, 0x07, 0x50, 0x88 }, /*W# 204  Src Line# 505 - GOTO DISABLE_WAIT_FUNCTION_LOCAL XO; OK to Skip LDO_XO?*/
	  { 0x5C, 0x07, 0x20, 0x89 }, /*W# 205  Src Line# 506 - GOTO DISABLE_WAIT_FUNCTION_LOCAL S5_CTRL; Required for L7 and L12 (S5 disable will be gated by VREG_XO)*/
	  { 0x5C, 0x07, 0x24, 0x88 }, /*W# 206  Src Line# 507 - */
	  { 0xF3, 0x08, 0x45, 0x41 }, /*W# 207  Src Line# 510 - Read L6 STATUS1 and store to Buffer R3*/
	  { 0x80, 0x31, 0x80, 0x90 }, /*W# 208  Src Line# 511 - Skip the next line if L6 is enabled (STATUS1 bit7=1)*/
	  { 0x5C, 0x07, 0x2C, 0x88 }, /*W# 209  Src Line# 513 - DO NOT INCLUDE THIS LINE!!! - Need MBG enabled in the Off State for Secure Processor*/
	  { 0x05, 0x45, 0x45, 0x01 }, /*W# 210  Src Line# 519 - Set L6 to LPM mode in the Off State for Secure Processor*/
	  { 0x46, 0x01, 0xFF, 0xC1 }, /*W# 211  Src Line# 521 - WAIT 10010 us; Wait 10ms for VREGs to discharge completely before allowing the next PON trigger*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 212  Src Line# 522 - */
	  { 0x40, 0x91, 0x08, 0x00 }, /*W# 213  Src Line# 523 - 1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 214  Src Line# 524 - End of Sequence; Byte-2*/
	  { 0x00, 0x46, 0xFF, 0x04 }, /*W# 215  Src Line# 527 - 1BYTE WRITE 0x00 TO 0x46; Write 0x00 to 0x46 (enable register in the peripheral)*/
	  { 0x21, 0x00, 0xFF, 0xC1 }, /*W# 216  Src Line# 528 - WAIT 1068 us; Wait 33+2 32kHz clock cycles.  Increased for PM8019 2.0.*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 217  Src Line# 535 - End of subroutine. PC = Stored PC.; Go back to next line of the calling function*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 218  Src Line# 538 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 219  Src Line# 539 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 220  Src Line# 542 - */
	  { 0x00, 0x43, 0x08, 0x02 }, /*W# 221  Src Line# 543 - 1BYTE WRITE 0x00 TO SID:2 0x0800__0x43; Set SPMI_SLAVE_RB to 0 to tolerate glitches during power down*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 222  Src Line# 545 - */
	  { 0x00, 0x99, 0x08, 0x00 }, /*W# 223  Src Line# 546 - 1BYTE WRITE 0x00 TO PON__PON_RESET_N_CTL; Assert PON_RESET_N low*/
	  { 0x00, 0x49, 0x09, 0x10 }, /*W# 224  Src Line# 547 - 1BYTE WRITE 0x00 TO MISC__PON_OUT_CTL; Send PON_OUT low to all PMIC's.  Changing to SEC ACCESS (to updated in next .csv).*/
	  { 0x01, 0x43, 0x08, 0x02 }, /*W# 225  Src Line# 548 - 1BYTE WRITE 0x01 TO SID:2 0x0800__0x43; Set SPMI_SLAVE_RB back to 1*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 226  Src Line# 549 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 227  Src Line# 555 - */
	  { 0xF7, 0x50, 0x72, 0x40 }, /*W# 228  Src Line# 556 - 1BYTE READ PBS_CLIENT1__SCRATCH1 TO R7; Readback PBS_CLIENT1_SCRATCH1 and store to Buffer R7*/
	  { 0x05, 0x70, 0xFF, 0x91 }, /*W# 229  Src Line# 558 - SKIP 1 Lines if R7 NOT EQUAL 0x04 (MASK=0xFF)*/
	  { 0x02, 0x51, 0x72, 0x00 }, /*W# 230  Src Line# 560 - sleep clock cycles (used by AOP for wait).  Changed from 6 to 2 cycles on PBS vD (shorter sequences MSM v2).*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 231  Src Line# 561 - */
	  { 0x04, 0x70, 0xFF, 0x90 }, /*W# 232  Src Line# 564 - SKIP 1 Lines if R7 NOT EQUAL 0x04 (MASK=0xFF)*/
	  { 0xC4, 0x07, 0xFF, 0x83 }, /*W# 233  Src Line# 566 - */
	  { 0x03, 0x70, 0xFF, 0x90 }, /*W# 234  Src Line# 569 - SKIP 1 Lines if R7 NOT EQUAL 0x03 (MASK=0xFF)*/
	  { 0xD8, 0x07, 0xFF, 0x83 }, /*W# 235  Src Line# 571 - */
	  { 0x02, 0x70, 0xFF, 0x90 }, /*W# 236  Src Line# 574 - SKIP 1 Lines if R7 NOT EQUAL 0x02 (MASK=0xFF)*/
	  { 0xF0, 0x07, 0xFF, 0x83 }, /*W# 237  Src Line# 576 - */
	  { 0x01, 0x70, 0xFF, 0x90 }, /*W# 238  Src Line# 579 - SKIP 1 Lines if R7 NOT EQUAL 0x01 (MASK=0xFF)*/
	  { 0x50, 0x08, 0xFF, 0x83 }, /*W# 239  Src Line# 581 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 240  Src Line# 584 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 241  Src Line# 591 - */
	  { 0x1C, 0x04, 0x38, 0x00 }, /*W# 242  Src Line# 592 - Re-enable ADC comparators*/
	  { 0x80, 0x46, 0x0B, 0x00 }, /*W# 243  Src Line# 593 - Enable VREF_MSM*/
	  { 0x04, 0x51, 0x72, 0x00 }, /*W# 244  Src Line# 595 - Set SCRATCH2 to 0x04 (to indicate to SW that SOC wake sequence just ran)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 245  Src Line# 596 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 246  Src Line# 603 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 247  Src Line# 604 - */
	  { 0x48, 0x0B, 0xFF, 0x9B }, /*W# 248  Src Line# 607 - */
	  { 0x00, 0x40, 0x38, 0x00 }, /*W# 249  Src Line# 609 - Disable ADC comparators (saves 10uA on 9655)*/
	  { 0x03, 0x51, 0x72, 0x00 }, /*W# 250  Src Line# 611 - Set SCRATCH2 to 0x03 (to indicate to SW that SOC sleep sequence just ran)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 251  Src Line# 612 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 252  Src Line# 620 - */
	  { 0xF7, 0x11, 0x1C, 0x40 }, /*W# 253  Src Line# 621 - Readback 'secret' spare register and store to Buffer R7*/
	  { 0x08, 0x70, 0xFF, 0x91 }, /*W# 254  Src Line# 622 - 0x08 is the activation code*/
	  { 0x00, 0x44, 0xC1, 0x00 }, /*W# 255  Src Line# 624 - Set to Dig Output low*/
	  { 0x00, 0x40, 0xC1, 0x00 }, /*W# 256  Src Line# 625 - Set to Dig Inpput*/
	  { 0x18, 0x04, 0x76, 0xA1 }, /*W# 257  Src Line# 628 - ; 2BYTE R6=MEMADDR(0x418).  Load Buffer R6 with GOTO pointer to SLEEP_DATASET*/
	  { 0x98, 0x0A, 0x26, 0x89 }, /*W# 258  Src Line# 630 - */
	  { 0x98, 0x0A, 0x43, 0x89 }, /*W# 259  Src Line# 631 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 260  Src Line# 635 - */
	  { 0x98, 0x0A, 0x5A, 0x89 }, /*W# 261  Src Line# 638 - */
	  { 0xC5, 0x44, 0x1D, 0x01 }, /*W# 262  Src Line# 646 - Re-enable PFM_IBOOST*/
	  { 0x90, 0xA6, 0x17, 0x11 }, /*W# 263  Src Line# 648 - Re-enable S2 PFM_EXIT_DIP_EN*/
	  { 0x32, 0xA9, 0x17, 0x11 }, /*W# 264  Src Line# 649 - Re-enable S2 PFM_EXIT_CL_EN*/
	  { 0x0B, 0x71, 0x17, 0x11 }, /*W# 265  Src Line# 650 - Enable S2 VDIP comparator; 20mV threshold*/
	  { 0x90, 0xA6, 0x23, 0x11 }, /*W# 266  Src Line# 652 - Re-enable S6 PFM_EXIT_DIP_EN*/
	  { 0x32, 0xA9, 0x23, 0x11 }, /*W# 267  Src Line# 653 - Re-enable S6 PFM_EXIT_CL_EN*/
	  { 0x0A, 0x71, 0x23, 0x11 }, /*W# 268  Src Line# 654 - Enable S6 VDIP comparator; 15mV threshold (new on RAM v7 / SBL v0x0A)*/
	  { 0x90, 0xA6, 0x2C, 0x11 }, /*W# 269  Src Line# 656 - Re-enable S9 PFM_EXIT_DIP_EN*/
	  { 0x32, 0xA9, 0x2C, 0x11 }, /*W# 270  Src Line# 657 - Re-enable S9 PFM_EXIT_CL_EN*/
	  { 0x09, 0x71, 0x2C, 0x11 }, /*W# 271  Src Line# 658 - Enable S9 VDIP comparator; 10mV threshold*/
	  { 0x3A, 0x7E, 0x20, 0x01 }, /*W# 272  Src Line# 662 - */
	  { 0x38, 0x7E, 0x1A, 0x01 }, /*W# 273  Src Line# 663 - */
	  { 0x02, 0x51, 0x72, 0x00 }, /*W# 274  Src Line# 665 - Set SCRATCH2 to 0x02 (to indicate to SW that wake sequence just ran)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 275  Src Line# 666 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 276  Src Line# 673 - */
	  { 0xF7, 0x45, 0x1A, 0x41 }, /*W# 277  Src Line# 675 - Readback S3 Mode and store to Buffer R7*/
	  { 0x04, 0x70, 0xFF, 0x90 }, /*W# 278  Src Line# 676 - Skip if Mode != RM*/
	  { 0x30, 0x7E, 0x1A, 0x01 }, /*W# 279  Src Line# 678 - */
	  { 0xF7, 0x45, 0x20, 0x41 }, /*W# 280  Src Line# 681 - Readback S5 Mode and store to Buffer R7*/
	  { 0x04, 0x70, 0xFF, 0x90 }, /*W# 281  Src Line# 682 - Skip if Mode != RM*/
	  { 0x32, 0x7E, 0x20, 0x01 }, /*W# 282  Src Line# 684 - */
	  { 0x18, 0x04, 0x76, 0xA1 }, /*W# 283  Src Line# 687 - ; 2BYTE R6=MEMADDR(0x418).  Load Buffer R6 with GOTO pointer to SLEEP_DATASET*/
	  { 0x7C, 0x0A, 0x26, 0x89 }, /*W# 284  Src Line# 689 - */
	  { 0x7C, 0x0A, 0x43, 0x89 }, /*W# 285  Src Line# 690 - */
	  { 0x7C, 0x0A, 0x5A, 0x89 }, /*W# 286  Src Line# 691 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 287  Src Line# 699 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 288  Src Line# 700 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 289  Src Line# 701 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 290  Src Line# 702 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 291  Src Line# 703 - */
	  { 0x04, 0x45, 0x26, 0x01 }, /*W# 292  Src Line# 706 - Set to Retention Mode*/
	  { 0x05, 0x45, 0x43, 0x01 }, /*W# 293  Src Line# 707 - Set to LPM Mode*/
	  { 0xF7, 0x45, 0x5A, 0x41 }, /*W# 294  Src Line# 711 - Readback L27 Mode and store to Buffer R7*/
	  { 0x05, 0x70, 0xFF, 0x90 }, /*W# 295  Src Line# 712 - Skip if Mode != LPM*/
	  { 0x04, 0x45, 0x5A, 0x01 }, /*W# 296  Src Line# 714 - Set to Retention Mode*/
	  { 0xF7, 0x45, 0x2C, 0x41 }, /*W# 297  Src Line# 721 - Readback S9 Mode and store to Buffer R7*/
	  { 0x05, 0x75, 0xFF, 0x92 }, /*W# 298  Src Line# 722 - Skip if Mode != LPM or RM*/
	  { 0x01, 0x71, 0x2C, 0x11 }, /*W# 299  Src Line# 725 - Disable VDIP comparator; keep 15mV threshold*/
	  { 0x50, 0xA6, 0x2C, 0x11 }, /*W# 300  Src Line# 726 - Disable PFM_EXIT_DIP_EN to prevent using HCPFM CL*/
	  { 0x12, 0xA9, 0x2C, 0x11 }, /*W# 301  Src Line# 727 - Disable PFM_EXIT_CL_EN to prevent using HCPFM CL*/
	  { 0xF7, 0x45, 0x23, 0x41 }, /*W# 302  Src Line# 730 - Readback S6 Mode and store to Buffer R7*/
	  { 0x05, 0x75, 0xFF, 0x92 }, /*W# 303  Src Line# 731 - Skip if Mode != LPM or RM*/
	  { 0x01, 0x71, 0x23, 0x11 }, /*W# 304  Src Line# 734 - Disable VDIP comparator; keep 15mV threshold*/
	  { 0x50, 0xA6, 0x23, 0x11 }, /*W# 305  Src Line# 735 - Disable PFM_EXIT_DIP_EN to prevent using HCPFM CL*/
	  { 0x12, 0xA9, 0x23, 0x11 }, /*W# 306  Src Line# 736 - Disable PFM_EXIT_CL_EN to prevent using HCPFM CL*/
	  { 0xF7, 0x45, 0x17, 0x41 }, /*W# 307  Src Line# 739 - Readback S2 Mode and store to Buffer R7*/
	  { 0x05, 0x75, 0xFF, 0x92 }, /*W# 308  Src Line# 740 - Skip if Mode != LPM or RM*/
	  { 0x01, 0x71, 0x17, 0x11 }, /*W# 309  Src Line# 743 - Disable VDIP comparator; keep 15mV threshold*/
	  { 0x50, 0xA6, 0x17, 0x11 }, /*W# 310  Src Line# 744 - Disable PFM_EXIT_DIP_EN to prevent using HCPFM CL*/
	  { 0x12, 0xA9, 0x17, 0x11 }, /*W# 311  Src Line# 745 - Disable PFM_EXIT_CL_EN to prevent using HCPFM CL*/
	  { 0xF7, 0x45, 0x1D, 0x41 }, /*W# 312  Src Line# 748 - Readback S4 Mode and store to Buffer R7*/
	  { 0x05, 0x75, 0xFF, 0x90 }, /*W# 313  Src Line# 750 - Skip if Mode != LPM or RM*/
	  { 0x05, 0x44, 0x1D, 0x01 }, /*W# 314  Src Line# 753 - Disable PFM_IBOOST*/
	  { 0x01, 0x51, 0x72, 0x00 }, /*W# 315  Src Line# 756 - Set SCRATCH2 to 0x01 (to indicate to SW that sleep sequence just ran)*/
	  { 0xF7, 0x11, 0x1C, 0x40 }, /*W# 316  Src Line# 759 - Readback 'secret' spare register and store to Buffer R7*/
	  { 0x08, 0x70, 0xFF, 0x91 }, /*W# 317  Src Line# 760 - 0x08 is the activation code*/
	  { 0x01, 0x40, 0xC1, 0x00 }, /*W# 318  Src Line# 762 - Set to Dig Output*/
	  { 0x80, 0x44, 0xC1, 0x00 }, /*W# 319  Src Line# 763 - Set to Dig Output high*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 320  Src Line# 766 - End of Sequence; Byte-2*/
	  { 0xF8, 0x08, 0xD6, 0x40 }, /*W# 321  Src Line# 774 - 1BYTE READ GPIO23__STATUS1 TO R8; Readback GPIO23 (WCSS_VCTRL) status and store to Buffer R8*/
	  { 0x01, 0x81, 0x01, 0x90 }, /*W# 322  Src Line# 775 - SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO23 is high*/
	  { 0x28, 0x09, 0xFF, 0x83 }, /*W# 323  Src Line# 777 - GOTO WCSS_VCTRL_DISABLE; Else (i.e. GPI023 is low)*/
	  { 0xC0, 0x4F, 0x26, 0x01 }, /*W# 324  Src Line# 780 - Assert PBS_EN and PBS_MODE (Secondary Mode is set in SBL)*/
	  { 0xF9, 0x4C, 0x09, 0x40 }, /*W# 325  Src Line# 781 - 1BYTE READ MISC__VSET1_LB TO R9; Read MISC register for L5 active voltage and store LSB to Buffer R9*/
	  { 0xFA, 0x4D, 0x09, 0x40 }, /*W# 326  Src Line# 782 - 1BYTE READ MISC__VSET1_UB TO RA; Read MISC register for L5 active voltage and store MSB to Buffer RA*/
	  { 0xF9, 0x40, 0x44, 0x09 }, /*W# 327  Src Line# 783 - 1BYTE WRITE R9 TO LDO05__VSET_LB; Write Buffer R9 value to L5A VSET (LSB) (note: PDM error incorrectly shows 2BYTE)*/
	  { 0xFA, 0x41, 0x44, 0x09 }, /*W# 328  Src Line# 784 - 1BYTE WRITE RA TO LDO05__VSET_UB; Write Buffer RA value to L5A VSET (MSB) (note: PDM error incorrectly shows 2BYTE)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 329  Src Line# 785 - End of Sequence; Byte-2*/
	  { 0xF9, 0x4E, 0x09, 0x40 }, /*W# 330  Src Line# 788 - 1BYTE READ MISC__VSET2_LB TO R9; Read MISC register for L5 retention voltage and store LSB to Buffer R9*/
	  { 0xFA, 0x4F, 0x09, 0x40 }, /*W# 331  Src Line# 789 - 1BYTE READ MISC__VSET2_UB TO RA; Read MISC register for L5 retention voltage and store MSB to Buffer RA*/
	  { 0xF9, 0x40, 0x44, 0x09 }, /*W# 332  Src Line# 790 - 1BYTE WRITE R9 TO LDO05__VSET_LB; Write Buffer R9 value to L5A VSET (LSB) (note: PDM error incorrectly shows 2BYTE)*/
	  { 0xFA, 0x41, 0x44, 0x09 }, /*W# 333  Src Line# 791 - 1BYTE WRITE RA TO LDO05__VSET_UB; Write Buffer RA value to L5A VSET (MSB) (note: PDM error incorrectly shows 2BYTE)*/
	  { 0xF8, 0x08, 0xD3, 0x40 }, /*W# 334  Src Line# 793 - 1BYTE READ GPIO020__STATUS1 TO R8; Readback GPIO20 (EUD) status and store to Buffer R8*/
	  { 0x01, 0x81, 0x01, 0x90 }, /*W# 335  Src Line# 794 - SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01)*/
	  { 0x00, 0x4F, 0x26, 0x01 }, /*W# 336  Src Line# 796 - 1BYTE WRITE 0x00 TO S7_CTRL__PBS_VOTE_CTL; De-assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 337  Src Line# 804 - End of Sequence; Byte-2*/
	  { 0xF8, 0x08, 0xD3, 0x40 }, /*W# 338  Src Line# 807 - Readback GPIO20 (EUD) status and store to Buffer R8*/
	  { 0x01, 0x81, 0x01, 0x90 }, /*W# 339  Src Line# 808 - SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO20 is high*/
	  { 0x78, 0x09, 0xFF, 0x83 }, /*W# 340  Src Line# 810 - Else (i.e. GPI020 is low)*/
	  { 0xC0, 0x4F, 0x26, 0x01 }, /*W# 341  Src Line# 813 - Assert PBS_EN and PBS_MODE (Secondary Mode is set in SBL)*/
	  { 0x40, 0x4F, 0x20, 0x01 }, /*W# 342  Src Line# 814 - Assert PBS_MODE (Secondary Mode is set in SBL)*/
	  { 0xF8, 0x51, 0x71, 0x40 }, /*W# 343  Src Line# 817 - 1Readback PBS_CLIENT0.SCRATCH2 (Option 2 config)*/
	  { 0x00, 0x80, 0xFF, 0x90 }, /*W# 344  Src Line# 818 - SKIP if OPTION2 != GND (i.e. not SS option)*/
	  { 0x88, 0x47, 0x44, 0x01 }, /*W# 345  Src Line# 820 - Assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE (Secondary Mode is set in SBL)*/
	  { 0x88, 0x47, 0x40, 0x01 }, /*W# 346  Src Line# 823 - Assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE (Secondary Mode is set in SBL)*/
	  { 0x88, 0x47, 0x4B, 0x01 }, /*W# 347  Src Line# 824 - Assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE (Secondary Mode is set in SBL)*/
	  { 0x88, 0x47, 0x57, 0x01 }, /*W# 348  Src Line# 825 - Assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE (Secondary Mode is set in SBL)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 349  Src Line# 826 - End of Sequence; Byte-2*/
	  { 0x00, 0x47, 0x57, 0x01 }, /*W# 350  Src Line# 829 - De-assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE*/
	  { 0x80, 0x47, 0x4B, 0x01 }, /*W# 351  Src Line# 830 - De-assert EN_FOLLOW_PMIC_AWAKE, but not MODE_FOLLOW_PMIC_AWAKE (preserve SBL setting)*/
	  { 0x00, 0x47, 0x40, 0x01 }, /*W# 352  Src Line# 831 - De-assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE*/
	  { 0xF8, 0x51, 0x71, 0x40 }, /*W# 353  Src Line# 834 - 1Readback PBS_CLIENT0.SCRATCH2 (Option 2 config)*/
	  { 0x00, 0x80, 0xFF, 0x90 }, /*W# 354  Src Line# 835 - SKIP if OPTION2 != GND (i.e. not SS option)*/
	  { 0x00, 0x47, 0x44, 0x01 }, /*W# 355  Src Line# 837 - De-assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE*/
	  { 0xF8, 0x08, 0xC2, 0x40 }, /*W# 356  Src Line# 840 - Readback GPIO03 (WLAN_RF_VCTRL) status and store to Buffer R8*/
	  { 0x01, 0x81, 0x01, 0x90 }, /*W# 357  Src Line# 841 - SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO03 is high (i.e. WLAN enabled)*/
	  { 0x00, 0x4F, 0x20, 0x01 }, /*W# 358  Src Line# 843 - De-assert PBS_MODE*/
	  { 0xF8, 0x08, 0xD6, 0x40 }, /*W# 359  Src Line# 850 - 1BYTE READ GPIO23__STATUS1 TO R8; Readback GPIO23 (WCSS_VCTRL) status and store to Buffer R8*/
	  { 0x01, 0x81, 0x01, 0x90 }, /*W# 360  Src Line# 851 - SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO23 is high (i.e. WLAN enabled)*/
	  { 0x00, 0x4F, 0x26, 0x01 }, /*W# 361  Src Line# 853 - 1BYTE WRITE 0x00 TO S7_CTRL__PBS_VOTE_CTL; De-assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 362  Src Line# 860 - End of Sequence; Byte-2*/
	  { 0xF8, 0x08, 0xC2, 0x40 }, /*W# 363  Src Line# 865 - 1BYTE READ GPIO03__STATUS1 TO R8; Readback GPIO3 (WLAN_RF_VCTRL) status and store to Buffer R8*/
	  { 0x01, 0x81, 0x01, 0x90 }, /*W# 364  Src Line# 866 - SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO3 is high*/
	  { 0xC4, 0x09, 0xFF, 0x83 }, /*W# 365  Src Line# 868 - GOTO WLAN_RF_VCTRL_DISABLE; lse (i.e. GPI03 is low)*/
	  { 0x40, 0x4F, 0x1A, 0x01 }, /*W# 366  Src Line# 871 - 1BYTE WRITE 0x40 TO S3_CTRL__PBS_VOTE_CTL; Assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)*/
	  { 0x40, 0x4F, 0x20, 0x01 }, /*W# 367  Src Line# 872 - 1BYTE WRITE 0x40 TO S5_CTRL__PBS_VOTE_CTL; Assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 368  Src Line# 873 - End of Sequence; Byte-2*/
	  { 0xF8, 0x08, 0xD3, 0x40 }, /*W# 369  Src Line# 876 - Readback GPIO20 (EUD) status and store to Buffer R8*/
	  { 0x01, 0x81, 0x01, 0x90 }, /*W# 370  Src Line# 878 - SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO20 is high (i.e. EUD enabled)*/
	  { 0x00, 0x4F, 0x20, 0x01 }, /*W# 371  Src Line# 880 - De-assert PBS_MODE*/
	  { 0x00, 0x4F, 0x1A, 0x01 }, /*W# 372  Src Line# 883 - 1BYTE WRITE 0x00 TO S3_CTRL__PBS_VOTE_CTL; De-assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 373  Src Line# 884 - End of Sequence; Byte-2*/
	  { 0xEC, 0x0B, 0x70, 0x29 }, /*W# 374  Src Line# 898 - 2BYTE R0 = 0xBEC  MEMADDR(END_PON_REASONS_PREVIOUS); Load R0 with the copy address*/
	  { 0xF4, 0x0B, 0x72, 0x29 }, /*W# 375  Src Line# 899 - 2BYTE R2 = 0xBF4  MEMADDR(END_PON_REASONS_PREVIOUS2); Load R2 with the paste address*/
	  { 0x04, 0x00, 0x78, 0x28 }, /*W# 376  Src Line# 900 - 1BYTE R8 = 0x04; Loop 4 times (shift 4 DWORDS)*/
	  { 0xF0, 0xFF, 0xF4, 0xA3 }, /*W# 377  Src Line# 906 - 4BYTE R4 = MEMADDR(R0); copy the last reason registers into a buffer*/
	  { 0xF2, 0xFF, 0xF4, 0xA7 }, /*W# 378  Src Line# 907 - 4BYTE MEMADDR(R2) = R4; write the reasons to the location pointed to by the paste pointer*/
	  { 0x01, 0x00, 0x78, 0x24 }, /*W# 379  Src Line# 908 - */
	  { 0x00, 0x81, 0xFF, 0x92 }, /*W# 380  Src Line# 909 - SKIP 3 Lines if R8 EQUAL 0x00 (MASK=0xFF); Loop Until loop count (R8) = 4*/
	  { 0x04, 0x00, 0x70, 0x25 }, /*W# 381  Src Line# 911 - 2BYTE R0 = R0 - 0x04; Move read pointer to next DWORD*/
	  { 0x04, 0x00, 0x72, 0x25 }, /*W# 382  Src Line# 912 - 2BYTE R2 = R2 - 0x04; Move write pointer to next DWORD*/
	  { 0xE4, 0x09, 0xFF, 0x83 }, /*W# 383  Src Line# 913 - */
	  { 0xF4, 0xC0, 0x08, 0x40 }, /*W# 384  Src Line# 916 - */
	  { 0xF5, 0xC2, 0x08, 0x40 }, /*W# 385  Src Line# 917 - */
	  { 0xF6, 0xC4, 0x08, 0x40 }, /*W# 386  Src Line# 918 - */
	  { 0xF7, 0xC5, 0x08, 0x40 }, /*W# 387  Src Line# 919 - */
	  { 0xF0, 0xFF, 0xF4, 0xA7 }, /*W# 388  Src Line# 920 - 4BYTE MEMADDR(R0) = R4; Write data to R0 (the last register that we copied data out of)*/
	  { 0x04, 0x00, 0x70, 0x21 }, /*W# 389  Src Line# 921 - 2BYTE R0 = R0 + 0x04; Increment pointer to next DWord*/
	  { 0xF4, 0xC7, 0x08, 0x40 }, /*W# 390  Src Line# 922 - */
	  { 0xF5, 0xC8, 0x08, 0x40 }, /*W# 391  Src Line# 923 - */
	  { 0xF6, 0xC9, 0x08, 0x40 }, /*W# 392  Src Line# 924 - */
	  { 0xF7, 0x48, 0x60, 0x40 }, /*W# 393  Src Line# 930 - 1BYTE READ RTC_RW__RDATA0 TO R7; Fill the last byte with the RTC value.*/
	  { 0xF8, 0x48, 0x60, 0x40 }, /*W# 394  Src Line# 931 - 1BYTE READ RTC_RW__RDATA0 TO R8; Fill the last byte with the RTC value.*/
	  { 0xF8, 0xF7, 0x79, 0x26 }, /*W# 395  Src Line# 932 - 1BYTE R9 = R8 - R7; Ensure we read the same RTC value twice consecutively to ensure a valid reading on the free running clock.*/
	  { 0x00, 0x91, 0xFF, 0x90 }, /*W# 396  Src Line# 933 - SKIP 1 Lines if R9 EQUAL 0x00 (MASK=0xFF); If data didn't match we retry.*/
	  { 0x24, 0x0A, 0xFF, 0x83 }, /*W# 397  Src Line# 935 - */
	  { 0xF0, 0xFF, 0xF4, 0xA7 }, /*W# 398  Src Line# 938 - 4BYTE MEMADDR(R0) = R4; Write data to address stored in R0*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 399  Src Line# 939 - End of subroutine. PC = Stored PC.; Byte-2*/
	  { 0x88, 0x48, 0x14, 0x01 }, /*W# 400  Src Line# 944 - 1BYTE WRITE 0x88 TO S1_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x17, 0x01 }, /*W# 401  Src Line# 945 - 1BYTE WRITE 0x88 TO S2_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x1A, 0x01 }, /*W# 402  Src Line# 946 - 1BYTE WRITE 0x88 TO S3_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x1D, 0x01 }, /*W# 403  Src Line# 947 - 1BYTE WRITE 0x88 TO S4_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x20, 0x01 }, /*W# 404  Src Line# 948 - 1BYTE WRITE 0x88 TO S5_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x23, 0x01 }, /*W# 405  Src Line# 949 - 1BYTE WRITE 0x88 TO S6_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x26, 0x01 }, /*W# 406  Src Line# 950 - 1BYTE WRITE 0x88 TO S7_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x29, 0x01 }, /*W# 407  Src Line# 951 - 1BYTE WRITE 0x88 TO S8_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x2C, 0x01 }, /*W# 408  Src Line# 952 - 1BYTE WRITE 0x88 TO S9_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x2F, 0x01 }, /*W# 409  Src Line# 953 - 1BYTE WRITE 0x88 TO S10_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x32, 0x01 }, /*W# 410  Src Line# 954 - 1BYTE WRITE 0x88 TO S11_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x35, 0x01 }, /*W# 411  Src Line# 955 - 1BYTE WRITE 0x88 TO S12_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x38, 0x01 }, /*W# 412  Src Line# 956 - 1BYTE WRITE 0x88 TO S13_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x80, 0x48, 0x58, 0x01 }, /*W# 413  Src Line# 958 - Enable strong pulldown*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 414  Src Line# 959 - */
	  { 0xF0, 0x40, 0xFF, 0x44 }, /*W# 415  Src Line# 976 - Store VSET_LB into R0*/
	  { 0xF1, 0x41, 0xFF, 0x44 }, /*W# 416  Src Line# 977 - Store VSET_UB into R1*/
	  { 0xF2, 0x45, 0xFF, 0x44 }, /*W# 417  Src Line# 978 - Store MODE_CTL1 into R2*/
	  { 0xF3, 0x08, 0xFF, 0x44 }, /*W# 418  Src Line# 979 - Store STATUS into R3 (placeholder only)*/
	  { 0xF6, 0xFF, 0xF0, 0xA7 }, /*W# 419  Src Line# 980 - Load data into location pointed to by R4*/
	  { 0x04, 0x00, 0x76, 0x21 }, /*W# 420  Src Line# 981 - Increment pointer by 4 to point at the next set*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 421  Src Line# 982 - */
	  { 0xF6, 0xFF, 0xF0, 0xA3 }, /*W# 422  Src Line# 987 - Load R0 to R3 with data from location pointed to by R4*/
	  { 0xF0, 0x40, 0xFF, 0x0C }, /*W# 423  Src Line# 988 - ReStore VSET_LB from R0*/
	  { 0xF1, 0x41, 0xFF, 0x0C }, /*W# 424  Src Line# 989 - ReStore VSET_UB from R1*/
	  { 0xF2, 0x45, 0xFF, 0x0C }, /*W# 425  Src Line# 990 - ReStore MODE_CTL1 from R2*/
	  { 0x04, 0x00, 0x76, 0x21 }, /*W# 426  Src Line# 992 - Increment pointer by 4 to point at the next set*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 427  Src Line# 993 - */
	  { 0xF1, 0x08, 0x38, 0x41 }, /*W# 428  Src Line# 999 - */
	  { 0x20, 0x10, 0x20, 0x97 }, /*W# 429  Src Line# 1000 - Check if bit 5 (VREG_OCP) is asserted.  Exit if not.*/
	  { 0x00, 0x46, 0x38, 0x01 }, /*W# 430  Src Line# 1003 - Disable S13*/
	  { 0x00, 0x63, 0x08, 0x00 }, /*W# 431  Src Line# 1004 - Disable*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 432  Src Line# 1005 - Wait 3 SLEEP_CLK (1+2) cycles for reset changes to propagate through clock domains*/
	  { 0x04, 0x62, 0x08, 0x00 }, /*W# 433  Src Line# 1006 - Configure to Normal Shutdown*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 434  Src Line# 1007 - Wait 3 SLEEP_CLK (1+2) cycles for reset changes to propagate through clock domains*/
	  { 0x80, 0x63, 0x08, 0x00 }, /*W# 435  Src Line# 1008 - Enable*/
	  { 0x03, 0x00, 0xFF, 0xC1 }, /*W# 436  Src Line# 1009 - Wait 5 SLEEP_CLK (3+2) cycles for reset changes to propagate through clock domains*/
	  { 0xA5, 0x64, 0x08, 0x00 }, /*W# 437  Src Line# 1010 - Initiate reset*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 438  Src Line# 1013 - */
	  { 0xFF, 0x00, 0xFE, 0xC1 }, /*W# 439  Src Line# 1018 - Wait 13.3us*/
	  { 0xFF, 0x00, 0xFE, 0xC1 }, /*W# 440  Src Line# 1019 - Wait 13.3us*/
	  { 0xFF, 0x00, 0xFE, 0xC1 }, /*W# 441  Src Line# 1021 - Wait 13.3us*/
	  { 0xF0, 0x08, 0xFF, 0x44 }, /*W# 442  Src Line# 1022 - Read STATUS1 and store to Buffer R0*/
	  { 0x80, 0x01, 0x80, 0x90 }, /*W# 443  Src Line# 1023 - Skip the next line if VREG_READY = 1 (STATUS1 bit 7)*/
	  { 0xE4, 0x0A, 0xFF, 0x83 }, /*W# 444  Src Line# 1024 - */
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 445  Src Line# 1026 - */
	  { 0x07, 0x45, 0xFF, 0x04 }, /*W# 446  Src Line# 1033 - Set NPM mode*/
	  { 0xF0, 0x42, 0xFF, 0x45 }, /*W# 447  Src Line# 1035 - Read 0x42+0x43 (2 Byte VSET_VALID) to R0+R1  (programmed value).  Use VSET_VALID to cover when the buck is disabled.*/
	  { 0xF6, 0xFF, 0x74, 0xA3 }, /*W# 448  Src Line# 1036 - Load R4+R5 with data from location pointed to by R6 (detection value)*/
	  { 0x00, 0xC5, 0xFF, 0x96 }, /*W# 449  Src Line# 1039 - */
	  { 0xF1, 0xF5, 0x7B, 0x26 }, /*W# 450  Src Line# 1047 - programmed value - detection value*/
	  { 0x00, 0xB6, 0xFF, 0x94 }, /*W# 451  Src Line# 1050 - If Carry was detected, MSB is less, jump to RUN*/
	  { 0x00, 0xB1, 0xFF, 0x90 }, /*W# 452  Src Line# 1053 - MSB is the same, jump to CONTINUE*/
	  { 0x3C, 0x0B, 0xFF, 0x83 }, /*W# 453  Src Line# 1056 - */
	  { 0xF0, 0xF4, 0x7A, 0x26 }, /*W# 454  Src Line# 1061 - programmed value - detection value*/
	  { 0x00, 0xA6, 0xFF, 0x90 }, /*W# 455  Src Line# 1063 - If Carry was detected, LSB is less, jump to RUN*/
	  { 0x3C, 0x0B, 0xFF, 0x83 }, /*W# 456  Src Line# 1066 - */
	  { 0x02, 0x00, 0x76, 0x21 }, /*W# 457  Src Line# 1069 - 2BYTE R6 = R6 + 0x02; Increment pointer by 2 to point to Bits 3/2*/
	  { 0xF6, 0xFF, 0x74, 0xA3 }, /*W# 458  Src Line# 1070 - Load R4+R5 with data from location pointed to by R6*/
	  { 0xF4, 0x40, 0xFF, 0x0D }, /*W# 459  Src Line# 1071 - Write R4+R5 TO 0x40+0x41 (VSET)*/
	  { 0x02, 0x00, 0x76, 0x21 }, /*W# 460  Src Line# 1072 - 2BYTE R6 = R6 + 0x02; Increment pointer by 2 to point at the next set*/
	  { 0x80, 0x46, 0xFF, 0x04 }, /*W# 461  Src Line# 1073 - Enable*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 462  Src Line# 1074 - */
	  { 0x04, 0x00, 0x76, 0x21 }, /*W# 463  Src Line# 1077 - 2BYTE R6 = R6 + 0x04; Increment pointer by 4 to point at the next set*/
	  { 0x80, 0x46, 0xFF, 0x04 }, /*W# 464  Src Line# 1078 - Enable*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 465  Src Line# 1079 - */
	  { 0x00, 0x00, 0x70, 0x28 }, /*W# 466  Src Line# 1084 - Load R0 with 0x00 (init)*/
	  { 0xF1, 0x43, 0x48, 0x41 }, /*W# 467  Src Line# 1085 - */
	  { 0x07, 0x15, 0xFF, 0x90 }, /*W# 468  Src Line# 1086 - Check L9:  ON @ 3V = 0x0B.  ON @ 1.8V = 0x07.  OFF (any voltage).  check MSB bit only.*/
	  { 0x01, 0x00, 0x70, 0x20 }, /*W# 469  Src Line# 1088 - Increment R0 by 1, if L9 is enabled + 1.8V*/
	  { 0xF1, 0x43, 0x49, 0x41 }, /*W# 470  Src Line# 1090 - */
	  { 0x07, 0x15, 0xFF, 0x90 }, /*W# 471  Src Line# 1091 - Check L10:  ON @ 3V = 0x0B.  ON @ 1.8V = 0x07.  OFF (any voltage).  check MSB bit only.*/
	  { 0x01, 0x00, 0x70, 0x20 }, /*W# 472  Src Line# 1093 - Increment R0 by 1, if L9 is enabled + 1.8V*/
	  { 0x02, 0x00, 0xFF, 0x90 }, /*W# 473  Src Line# 1095 - Check R0.  Will be 2 only if both LDO's are 1.8V or off.*/
	  { 0x00, 0x46, 0x0B, 0x00 }, /*W# 474  Src Line# 1097 - */
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 475  Src Line# 1099 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 476  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 477  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 478  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 479  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 480  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 481  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 482  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 483  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 484  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 485  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 486  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 487  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 488  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 489  Src Line# 1105 - S6 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 490  Src Line# 1106 - L4 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 491  Src Line# 1107 - S1 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: Min SVS (MSB), 0: Min SVS (LSB) (special case)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 492  Src Line# 1108 - S9 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 493  Src Line# 1109 - L27 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 494  Src Line# 1110 - S13 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 495  Src Line# 1111 - S12 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 496  Src Line# 1112 - Reserved*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 497  Src Line# 1118 - S7 MODE/VSET*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 498  Src Line# 1119 - L4 MODE/VSET*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 499  Src Line# 1120 - L27 MODE/VSET*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 500  Src Line# 1121 - spare*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 501  Src Line# 1122 - spare*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 502  Src Line# 1123 - spare*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 503  Src Line# 1124 - spare*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 504  Src Line# 1131 - 3:POFF_REASON, 2:ON_REASON, 1:WARM_RESET_REASON1, 0:PON_REASON1*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 505  Src Line# 1132 - 3:RTC_LSB, 2:FAULT_REASON2, 1:FAULT_REASON1, 0:OFF_REASON*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 506  Src Line# 1135 - 3:POFF_REASON, 2:ON_REASON, 1:WARM_RESET_REASON1, 0:PON_REASON1*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 507  Src Line# 1138 - 3:RTC_LSB, 2:FAULT_REASON2, 1:FAULT_REASON1, 0:OFF_REASON*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 508  Src Line# 1141 - 3:POFF_REASON, 2:ON_REASON, 1:WARM_RESET_REASON1, 0:PON_REASON1*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 509  Src Line# 1144 - 3:RTC_LSB, 2:FAULT_REASON2, 1:FAULT_REASON1, 0:OFF_REASON*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 510  Src Line# 1145 - */
	  { 0x01, 0x0D, 0xFF, 0xF8 }, /*W# 511  Src Line# 0 - Inserted by compiler.*/
	},
  /* PM8005_RAM_b0x01_v0x04 */
	{
	 // data, offset, baseaddr, sid 
	  { 0x52, 0x53, 0x42, 0x50 }, /*W# 0  Src Line# 54 - ; ASCII for PBSR".  Later changed to Check Sum."*/
	  { 0x01, 0x04, 0x01, 0x10 }, /*W# 1  Src Line# 55 - ; 10=Dataset Start, 01=Header Ver, 0401 = Rev 4, Branch 1*/
	  { 0x14, 0x02, 0xFF, 0x83 }, /*W# 2  Src Line# 56 - */
	  { 0x50, 0x02, 0xFF, 0x83 }, /*W# 3  Src Line# 57 - */
	  { 0xF4, 0x03, 0xFF, 0x83 }, /*W# 4  Src Line# 59 - Fixed location for WARM_RESET_DATASET.  Do NOT move this GOTO line.*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 5  Src Line# 62 - */
	  { 0xFF, 0x48, 0x71, 0x00 }, /*W# 6  Src Line# 63 - 1BYTE WRITE 0xFF TO PBS_CLIENT0__TRIG_LATCHED_CLR; Clear all latched sequences in PBS PON Client 0*/
	  { 0xFB, 0x08, 0x08, 0x40 }, /*W# 7  Src Line# 64 - 1BYTE READ PON__FSM_STATUS TO RB; Readback PON FSM Status and store to Buffer RB*/
	  { 0x04, 0xB0, 0xFF, 0x90 }, /*W# 8  Src Line# 65 - SKIP 1 Lines if RB NOT EQUAL 0x04 (MASK=0xFF); DVDD_CONFIG = State 4*/
	  { 0x08, 0x00, 0xFF, 0x83 }, /*W# 9  Src Line# 67 - ; GOTO OTP DVDD_CONFIG.  Using RAW command due to PDM issue with links.*/
	  { 0x01, 0xB0, 0xFF, 0x90 }, /*W# 10  Src Line# 70 - SKIP 1 Lines if RB NOT EQUAL 0x01 (MASK=0xFF); PON = State 1*/
	  { 0x08, 0x00, 0xFF, 0x83 }, /*W# 11  Src Line# 72 - ; GOTO OTP DVDD_CONFIG.  Using RAW command due to PDM issue with links.*/
	  { 0x06, 0xB0, 0xFF, 0x90 }, /*W# 12  Src Line# 75 - SKIP 1 Lines if RB NOT EQUAL 0x06 (MASK=0xFF); FAULT = State 6*/
	  { 0xAC, 0x02, 0xFF, 0x83 }, /*W# 13  Src Line# 77 - */
	  { 0x02, 0xB0, 0xFF, 0x90 }, /*W# 14  Src Line# 80 - SKIP 1 Lines if RB NOT EQUAL 0x02 (MASK=0xFF); POFF = State 2*/
	  { 0xF4, 0x02, 0xFF, 0x83 }, /*W# 15  Src Line# 82 - */
	  { 0x07, 0xB0, 0xFF, 0x90 }, /*W# 16  Src Line# 85 - SKIP 1 Lines if RB NOT EQUAL 0x07 (MASK=0xFF); WARM RESET = State 7*/
	  { 0xBC, 0x02, 0xFF, 0x83 }, /*W# 17  Src Line# 87 - */
	  { 0x00, 0x91, 0x08, 0x00 }, /*W# 18  Src Line# 90 - 1BYTE WRITE 0x00 TO PON__PBS_INTERFACE; Undefined State - NACK PON (Note:  None of the Client GOTOs have a 'RETURN'; the ACK is done in each subroutine)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 19  Src Line# 91 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 20  Src Line# 94 - */
	  { 0xF3, 0x0A, 0x72, 0x40 }, /*W# 21  Src Line# 95 - 1BYTE READ PBS_CLIENT1__TRIG_LATCHED TO R3; Read Client 1 trigger latch status and store to Buffer R3*/
	  { 0x01, 0x30, 0x01, 0x92 }, /*W# 22  Src Line# 96 - SKIP 3 Lines if R3 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / Spare*/
	  { 0x01, 0x48, 0x72, 0x00 }, /*W# 23  Src Line# 98 - 1BYTE WRITE 0x01 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 24  Src Line# 99 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 25  Src Line# 100 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x02, 0x30, 0x02, 0x92 }, /*W# 26  Src Line# 103 - SKIP 3 Lines if R3 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = EOSR*/
	  { 0x02, 0x48, 0x72, 0x00 }, /*W# 27  Src Line# 105 - 1BYTE WRITE 0x02 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 28  Src Line# 106 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 29  Src Line# 107 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x04, 0x30, 0x04, 0x92 }, /*W# 30  Src Line# 110 - SKIP 3 Lines if R3 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = OCP (non-core VREG)*/
	  { 0x04, 0x48, 0x72, 0x00 }, /*W# 31  Src Line# 112 - 1BYTE WRITE 0x04 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 32  Src Line# 113 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 33  Src Line# 114 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x08, 0x30, 0x08, 0x92 }, /*W# 34  Src Line# 117 - SKIP 3 Lines if R3 NOT EQUAL 0x08 (MASK=0x08); Bit 3 - GPIO2 (spare trigger)*/
	  { 0x08, 0x48, 0x72, 0x00 }, /*W# 35  Src Line# 119 - 1BYTE WRITE 0x08 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 36  Src Line# 120 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 37  Src Line# 121 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x80, 0x30, 0x80, 0x92 }, /*W# 38  Src Line# 124 - SKIP 3 Lines if R3 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger (Sleep/Wake)*/
	  { 0x80, 0x48, 0x72, 0x00 }, /*W# 39  Src Line# 126 - 1BYTE WRITE 0x80 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7*/
	  { 0x34, 0x03, 0xFF, 0x83 }, /*W# 40  Src Line# 127 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 41  Src Line# 128 - NOP; TBD*/
	  { 0x2C, 0x03, 0xFF, 0x83 }, /*W# 42  Src Line# 131 - GOTO RAM_MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT*/
	  { 0x8C, 0x03, 0xFF, 0x9B }, /*W# 43  Src Line# 135 - */
	  { 0x46, 0x01, 0xFF, 0xC1 }, /*W# 44  Src Line# 136 - WAIT 10010 us; Wait 10ms for VREGs to discharge completely before allowing the next PON trigger*/
	  { 0x40, 0x91, 0x08, 0x00 }, /*W# 45  Src Line# 137 - 1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 46  Src Line# 138 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 47  Src Line# 142 - */
	  { 0x02, 0x98, 0x08, 0x10 }, /*W# 48  Src Line# 144 - 1BYTE WRITE 0x02 TO PON__RESET_CTL; Assert warm_rb*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 49  Src Line# 145 - WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains*/
	  { 0x06, 0x98, 0x08, 0x10 }, /*W# 50  Src Line# 146 - 1BYTE WRITE 0x06 TO PON__RESET_CTL; De-assert warm_rb*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 51  Src Line# 147 - WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains*/
	  { 0xF2, 0x45, 0x09, 0x40 }, /*W# 52  Src Line# 149 - Read L6 MISC VSET3 LB (scratch for Pass 0/1/2) and store to R2*/
	  { 0xF4, 0x03, 0x76, 0x29 }, /*W# 53  Src Line# 152 - Load R6 with 0x3F4 (start of WARM RESET DATASET)*/
	  { 0xA0, 0x03, 0x14, 0x89 }, /*W# 54  Src Line# 153 - */
	  { 0xA0, 0x03, 0x17, 0x89 }, /*W# 55  Src Line# 154 - */
	  { 0x01, 0x00, 0x72, 0x20 }, /*W# 56  Src Line# 161 - Increment R2 by 1*/
	  { 0xF2, 0x45, 0x09, 0x08 }, /*W# 57  Src Line# 162 - */
	  { 0x01, 0x43, 0x08, 0x02 }, /*W# 58  Src Line# 164 - */
	  { 0x40, 0x91, 0x08, 0x00 }, /*W# 59  Src Line# 165 - 1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 60  Src Line# 166 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 61  Src Line# 169 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 62  Src Line# 171 - */
	  { 0x04, 0x98, 0x08, 0x10 }, /*W# 63  Src Line# 173 - 1BYTE WRITE 0x04 TO PON__RESET_CTL; Assert shutdown1_rb*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 64  Src Line# 174 - WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains*/
	  { 0x06, 0x98, 0x08, 0x10 }, /*W# 65  Src Line# 175 - 1BYTE WRITE 0x06 TO PON__RESET_CTL; De-assert shutdown1_rb*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 66  Src Line# 176 - WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains*/
	  { 0x8C, 0x03, 0xFF, 0x9B }, /*W# 67  Src Line# 178 - */
	  { 0x0C, 0x04, 0x17, 0x89 }, /*W# 68  Src Line# 180 - */
	  { 0x0C, 0x04, 0x1A, 0x89 }, /*W# 69  Src Line# 181 - */
	  { 0x46, 0x01, 0xFF, 0xC1 }, /*W# 70  Src Line# 183 - WAIT 10010 us; Wait 10ms for VREGs to discharge completely before allowing the next PON trigger*/
	  { 0x0C, 0x04, 0x24, 0x88 }, /*W# 71  Src Line# 184 - */
	  { 0x0C, 0x04, 0x2C, 0x88 }, /*W# 72  Src Line# 185 - */
	  { 0x40, 0x91, 0x08, 0x00 }, /*W# 73  Src Line# 186 - 1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 74  Src Line# 187 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 75  Src Line# 190 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 76  Src Line# 191 - End of Sequence; Byte-2*/
	  { 0xF7, 0x50, 0x72, 0x40 }, /*W# 77  Src Line# 198 - 1BYTE READ PBS_CLIENT1__SCRATCH1 TO R7; Readback PBS_CLIENT1_SCRATCH1 and store to Buffer R7*/
	  { 0x04, 0x70, 0xFF, 0x90 }, /*W# 78  Src Line# 200 - SKIP 1 Lines if R7 NOT EQUAL 0x04 (MASK=0xFF)*/
	  { 0x5C, 0x03, 0xFF, 0x83 }, /*W# 79  Src Line# 202 - */
	  { 0x03, 0x70, 0xFF, 0x90 }, /*W# 80  Src Line# 205 - SKIP 1 Lines if R7 NOT EQUAL 0x03 (MASK=0xFF)*/
	  { 0x68, 0x03, 0xFF, 0x83 }, /*W# 81  Src Line# 207 - */
	  { 0x02, 0x70, 0xFF, 0x90 }, /*W# 82  Src Line# 210 - SKIP 1 Lines if R7 NOT EQUAL 0x02 (MASK=0xFF)*/
	  { 0x74, 0x03, 0xFF, 0x83 }, /*W# 83  Src Line# 212 - */
	  { 0x01, 0x70, 0xFF, 0x90 }, /*W# 84  Src Line# 215 - SKIP 1 Lines if R7 NOT EQUAL 0x01 (MASK=0xFF)*/
	  { 0x80, 0x03, 0xFF, 0x83 }, /*W# 85  Src Line# 217 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 86  Src Line# 220 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 87  Src Line# 223 - */
	  { 0x04, 0x51, 0x72, 0x00 }, /*W# 88  Src Line# 224 - No sequence, just placeholder*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 89  Src Line# 225 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 90  Src Line# 228 - */
	  { 0x03, 0x51, 0x72, 0x00 }, /*W# 91  Src Line# 229 - No sequence, just placeholder*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 92  Src Line# 230 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 93  Src Line# 233 - */
	  { 0x02, 0x51, 0x72, 0x00 }, /*W# 94  Src Line# 234 - No sequence, just placeholder*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 95  Src Line# 235 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 96  Src Line# 238 - */
	  { 0x01, 0x51, 0x72, 0x00 }, /*W# 97  Src Line# 239 - No sequence, just placeholder*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 98  Src Line# 240 - */
	  { 0x88, 0x48, 0x14, 0x01 }, /*W# 99  Src Line# 248 - 1BYTE WRITE 0x88 TO S1_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x17, 0x01 }, /*W# 100  Src Line# 249 - 1BYTE WRITE 0x88 TO S2_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x1A, 0x01 }, /*W# 101  Src Line# 250 - 1BYTE WRITE 0x88 TO S3_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0x88, 0x48, 0x1D, 0x01 }, /*W# 102  Src Line# 251 - 1BYTE WRITE 0x88 TO S4_CTRL__PD_CTL; Enable strong + leak pulldown*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 103  Src Line# 252 - */
	  { 0x07, 0x45, 0xFF, 0x04 }, /*W# 104  Src Line# 259 - Set NPM mode*/
	  { 0xF0, 0x42, 0xFF, 0x45 }, /*W# 105  Src Line# 261 - Read 0x42+0x43 (2 Byte VSET_VALID) to R0+R1  (programmed value).  Use VSET_VALID to cover when the buck is disabled.*/
	  { 0xF6, 0xFF, 0x74, 0xA3 }, /*W# 106  Src Line# 262 - Load R4+R5 with data from location pointed to by R6 (detection value)*/
	  { 0x00, 0x25, 0xFF, 0x96 }, /*W# 107  Src Line# 265 - */
	  { 0xF1, 0xF5, 0x7B, 0x26 }, /*W# 108  Src Line# 273 - programmed value - detection value*/
	  { 0x00, 0xB6, 0xFF, 0x94 }, /*W# 109  Src Line# 276 - If Carry was detected, MSB is less, jump to RUN*/
	  { 0x00, 0xB1, 0xFF, 0x90 }, /*W# 110  Src Line# 279 - MSB is the same, jump to CONTINUE*/
	  { 0xE4, 0x03, 0xFF, 0x83 }, /*W# 111  Src Line# 282 - */
	  { 0xF0, 0xF4, 0x7A, 0x26 }, /*W# 112  Src Line# 287 - programmed value - detection value*/
	  { 0x00, 0xA6, 0xFF, 0x90 }, /*W# 113  Src Line# 289 - If Carry was detected, LSB is less, jump to RUN*/
	  { 0xE4, 0x03, 0xFF, 0x83 }, /*W# 114  Src Line# 292 - */
	  { 0x02, 0x00, 0x76, 0x21 }, /*W# 115  Src Line# 295 - 2BYTE R6 = R6 + 0x02; Increment pointer by 2 to point to Bits 3/2*/
	  { 0xF6, 0xFF, 0x74, 0xA3 }, /*W# 116  Src Line# 296 - Load R4+R5 with data from location pointed to by R6*/
	  { 0xF4, 0x40, 0xFF, 0x0D }, /*W# 117  Src Line# 297 - Write R4+R5 TO 0x40+0x41 (VSET)*/
	  { 0x02, 0x00, 0x76, 0x21 }, /*W# 118  Src Line# 298 - 2BYTE R6 = R6 + 0x02; Increment pointer by 2 to point at the next set*/
	  { 0x80, 0x46, 0xFF, 0x04 }, /*W# 119  Src Line# 299 - Enable*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 120  Src Line# 300 - */
	  { 0x04, 0x00, 0x76, 0x21 }, /*W# 121  Src Line# 303 - 2BYTE R6 = R6 + 0x04; Increment pointer by 4 to point at the next set*/
	  { 0x80, 0x46, 0xFF, 0x04 }, /*W# 122  Src Line# 304 - Enable*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 123  Src Line# 305 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 124  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 125  Src Line# 310 - S1c - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 126  Src Line# 311 - S2c - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)*/
	  { 0x01, 0x04, 0xFF, 0xF8 }, /*W# 127  Src Line# 0 - Inserted by compiler.*/
	},
  /* PMi8998_RAM_b0x01_v0x0B */
	{
	 // data, offset, baseaddr, sid 
	  { 0x52, 0x53, 0x42, 0x50 }, /*W# 0  Src Line# 74 - ; ASCII for PBSR".  Later changed to Check Sum."*/
	  { 0x01, 0x09, 0x01, 0x18 }, /*W# 1  Src Line# 75 - ; 18=Dataset Start, 01=Header Ver, 0901 = Rev 9, Branch 1*/
	  { 0x30, 0x08, 0xFF, 0x83 }, /*W# 2  Src Line# 76 - */
	  { 0x74, 0x08, 0xFF, 0x83 }, /*W# 3  Src Line# 77 - */
	  { 0xE4, 0x08, 0xFF, 0x83 }, /*W# 4  Src Line# 78 - */
	  { 0x54, 0x09, 0xFF, 0x83 }, /*W# 5  Src Line# 79 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 6  Src Line# 80 - ; Reserved location for sleep dataset pointer. Sleep dataset does not exist on PMi.*/
	  { 0xB0, 0x0F, 0xFF, 0x83 }, /*W# 7  Src Line# 81 - GOTO BEGIN_PON_REASONS_DATASET; Fixed location for PON REASONS dataset.  Do NOT move this GOTO line.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 8  Src Line# 82 - ; Reserved location for RTRR dataset pointer. Does not exist on PMi yet.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 9  Src Line# 83 - ; Reserved location for HLOS dataset pointer. Does not exist yet.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 10  Src Line# 84 - ; Reserved location for LPG dataset pointer. Not needed on PMi because it has LPG perph.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 11  Src Line# 85 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 12  Src Line# 88 - */
	  { 0xFF, 0x48, 0x71, 0x00 }, /*W# 13  Src Line# 89 - 1BYTE WRITE 0xFF TO PBS_CLIENT0__TRIG_LATCHED_CLR; Clear all latched sequences in PBS PON Client 0*/
	  { 0xFB, 0x08, 0x08, 0x40 }, /*W# 14  Src Line# 90 - 1BYTE READ PON__FSM_STATUS TO RB; Readback PON FSM Status and store to Buffer RB*/
	  { 0x04, 0xB0, 0xFF, 0x90 }, /*W# 15  Src Line# 91 - SKIP 1 Lines if RB NOT EQUAL 0x04 (MASK=0xFF); DVDD_CONFIG = State 4*/
	  { 0x10, 0x00, 0xFF, 0x83 }, /*W# 16  Src Line# 93 - GOTO OTP_DVDD_CONFIG (OTP); Is there a scenario where this is ever used?*/
	  { 0x01, 0xB0, 0xFF, 0x90 }, /*W# 17  Src Line# 96 - SKIP 1 Lines if RB NOT EQUAL 0x01 (MASK=0xFF); PON = State 1*/
	  { 0x10, 0x00, 0xFF, 0x83 }, /*W# 18  Src Line# 98 - GOTO OTP_DVDD_CONFIG (OTP); Is there a scenario where this is ever used?*/
	  { 0x06, 0xB0, 0xFF, 0x90 }, /*W# 19  Src Line# 101 - SKIP 1 Lines if RB NOT EQUAL 0x06 (MASK=0xFF); FAULT = State 6*/
	  { 0xCC, 0x09, 0xFF, 0x83 }, /*W# 20  Src Line# 103 - */
	  { 0x02, 0xB0, 0xFF, 0x90 }, /*W# 21  Src Line# 106 - SKIP 1 Lines if RB NOT EQUAL 0x02 (MASK=0xFF); POFF = State 2*/
	  { 0x38, 0x0A, 0xFF, 0x83 }, /*W# 22  Src Line# 108 - */
	  { 0x07, 0xB0, 0xFF, 0x90 }, /*W# 23  Src Line# 111 - SKIP 1 Lines if RB NOT EQUAL 0x07 (MASK=0xFF); WARM RESET = State 7*/
	  { 0xF4, 0x09, 0xFF, 0x83 }, /*W# 24  Src Line# 113 - */
	  { 0x00, 0x91, 0x08, 0x00 }, /*W# 25  Src Line# 116 - 1BYTE WRITE 0x00 TO PON__PBS_INTERFACE; Undefined State - NACK PON (Note:  None of the Client GOTOs have a 'RETURN'; the ACK is done in each subroutine)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 26  Src Line# 117 - End of Sequence; Byte-2*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 27  Src Line# 118 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 28  Src Line# 119 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 29  Src Line# 122 - */
	  { 0xF3, 0x0A, 0x72, 0x40 }, /*W# 30  Src Line# 123 - 1BYTE READ PBS_CLIENT1__TRIG_LATCHED TO R3; Read Client 1 trigger latch status and store to Buffer R3*/
	  { 0x01, 0x30, 0x01, 0x92 }, /*W# 31  Src Line# 124 - SKIP 3 Lines if R3 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / BOB*/
	  { 0x01, 0x48, 0x72, 0x00 }, /*W# 32  Src Line# 126 - 1BYTE WRITE 0x01 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 33  Src Line# 127 - NOP; TBS - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 34  Src Line# 128 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x02, 0x30, 0x02, 0x92 }, /*W# 35  Src Line# 131 - SKIP 3 Lines if R3 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = Charger Test MUX*/
	  { 0x02, 0x48, 0x72, 0x00 }, /*W# 36  Src Line# 133 - 1BYTE WRITE 0x02 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1*/
	  { 0x4C, 0x0D, 0xFF, 0x83 }, /*W# 37  Src Line# 134 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 38  Src Line# 135 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x04, 0x30, 0x04, 0x92 }, /*W# 39  Src Line# 138 - SKIP 3 Lines if R3 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = LAB/IBB SCP*/
	  { 0x04, 0x48, 0x72, 0x00 }, /*W# 40  Src Line# 140 - 1BYTE WRITE 0x04 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2*/
	  { 0xC0, 0x0A, 0xFF, 0x83 }, /*W# 41  Src Line# 141 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 42  Src Line# 142 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x08, 0x30, 0x08, 0x92 }, /*W# 43  Src Line# 145 - SKIP 3 Lines if R3 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = EOSR_Timer*/
	  { 0x08, 0x48, 0x72, 0x00 }, /*W# 44  Src Line# 147 - 1BYTE WRITE 0x08 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3*/
	  { 0x68, 0x0B, 0xFF, 0x83 }, /*W# 45  Src Line# 148 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 46  Src Line# 149 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x10, 0x30, 0x10, 0x92 }, /*W# 47  Src Line# 152 - SKIP 3 Lines if R3 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = LAB_ASYNC*/
	  { 0x10, 0x48, 0x72, 0x00 }, /*W# 48  Src Line# 154 - 1BYTE WRITE 0x10 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4*/
	  { 0x84, 0x0B, 0xFF, 0x83 }, /*W# 49  Src Line# 155 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 50  Src Line# 156 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x80, 0x30, 0x80, 0x92 }, /*W# 51  Src Line# 159 - SKIP 3 Lines if R3 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger (Sleep/Wake)*/
	  { 0x80, 0x48, 0x72, 0x00 }, /*W# 52  Src Line# 161 - 1BYTE WRITE 0x80 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7*/
	  { 0xEC, 0x0A, 0xFF, 0x83 }, /*W# 53  Src Line# 162 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 54  Src Line# 163 - End of Sequence; TBD*/
	  { 0x90, 0x0A, 0xFF, 0x83 }, /*W# 55  Src Line# 166 - GOTO MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 56  Src Line# 167 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 57  Src Line# 170 - */
	  { 0xF4, 0x0A, 0x73, 0x40 }, /*W# 58  Src Line# 171 - 1BYTE READ PBS_CLIENT2__TRIG_LATCHED TO R4; Read Client 2 trigger latch status and store to Buffer R4*/
	  { 0x01, 0x40, 0x01, 0x92 }, /*W# 59  Src Line# 172 - SKIP 3 Lines if R4 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / EOSR_TIMER*/
	  { 0x01, 0x48, 0x73, 0x00 }, /*W# 60  Src Line# 174 - 1BYTE WRITE 0x01 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0*/
	  { 0x88, 0x0D, 0xFF, 0x83 }, /*W# 61  Src Line# 175 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 62  Src Line# 176 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x02, 0x40, 0x02, 0x92 }, /*W# 63  Src Line# 179 - SKIP 3 Lines if R4 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = BATFET OCP*/
	  { 0x02, 0x48, 0x73, 0x00 }, /*W# 64  Src Line# 181 - 1BYTE WRITE 0x02 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 65  Src Line# 182 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 66  Src Line# 183 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x04, 0x40, 0x04, 0x92 }, /*W# 67  Src Line# 186 - SKIP 3 Lines if R4 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = FG BCL*/
	  { 0x04, 0x48, 0x73, 0x00 }, /*W# 68  Src Line# 188 - 1BYTE WRITE 0x04 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 69  Src Line# 189 - NOP; TBD*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 70  Src Line# 190 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x08, 0x40, 0x08, 0x92 }, /*W# 71  Src Line# 193 - SKIP 3 Lines if R4 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = Fuel Gauge Test MUX*/
	  { 0x08, 0x48, 0x73, 0x00 }, /*W# 72  Src Line# 195 - 1BYTE WRITE 0x08 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 73  Src Line# 196 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 74  Src Line# 197 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x10, 0x40, 0x10, 0x92 }, /*W# 75  Src Line# 200 - SKIP 3 Lines if R4 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = Unused Trigger*/
	  { 0x10, 0x48, 0x73, 0x00 }, /*W# 76  Src Line# 202 - 1BYTE WRITE 0x10 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4*/
	  { 0x80, 0x0D, 0xFF, 0x83 }, /*W# 77  Src Line# 203 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 78  Src Line# 204 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x80, 0x40, 0x80, 0x92 }, /*W# 79  Src Line# 207 - SKIP 3 Lines if R4 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger*/
	  { 0x80, 0x48, 0x73, 0x00 }, /*W# 80  Src Line# 209 - */
	  { 0xEC, 0x0D, 0xFF, 0x83 }, /*W# 81  Src Line# 210 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 82  Src Line# 211 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x90, 0x0A, 0xFF, 0x83 }, /*W# 83  Src Line# 214 - GOTO MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 84  Src Line# 215 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 85  Src Line# 218 - */
	  { 0xF5, 0x0A, 0x74, 0x40 }, /*W# 86  Src Line# 219 - 1BYTE READ PBS_CLIENT3__TRIG_LATCHED TO R5; Read Client 3 trigger latch status and store to Buffer R5*/
	  { 0x01, 0x50, 0x01, 0x92 }, /*W# 87  Src Line# 220 - SKIP 3 Lines if R5 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / PD_PHY_IRQ_MUX*/
	  { 0x01, 0x48, 0x74, 0x00 }, /*W# 88  Src Line# 222 - 1BYTE WRITE 0x01 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0*/
	  { 0xE8, 0x0B, 0xFF, 0x83 }, /*W# 89  Src Line# 223 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 90  Src Line# 224 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x02, 0x50, 0x02, 0x92 }, /*W# 91  Src Line# 227 - SKIP 3 Lines if R5 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = SCHG Snarl Timer*/
	  { 0x02, 0x48, 0x74, 0x00 }, /*W# 92  Src Line# 229 - 1BYTE WRITE 0x02 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 93  Src Line# 230 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 94  Src Line# 231 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x04, 0x50, 0x04, 0x92 }, /*W# 95  Src Line# 234 - SKIP 3 Lines if R5 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = SCHG QI/PMA ON*/
	  { 0x04, 0x48, 0x74, 0x00 }, /*W# 96  Src Line# 236 - 1BYTE WRITE 0x04 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 97  Src Line# 237 - NOP; TBD - Save for GOTO*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 98  Src Line# 238 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x08, 0x50, 0x08, 0x92 }, /*W# 99  Src Line# 241 - SKIP 3 Lines if R5 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = EOSR_Timer*/
	  { 0x08, 0x48, 0x74, 0x00 }, /*W# 100  Src Line# 243 - 1BYTE WRITE 0x08 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3*/
	  { 0xF8, 0x0B, 0xFF, 0x83 }, /*W# 101  Src Line# 244 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 102  Src Line# 245 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x10, 0x50, 0x10, 0x92 }, /*W# 103  Src Line# 248 - SKIP 3 Lines if R5 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = WLED_SWIRE*/
	  { 0x10, 0x48, 0x74, 0x00 }, /*W# 104  Src Line# 250 - 1BYTE WRITE 0x10 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4*/
	  { 0x54, 0x0C, 0xFF, 0x83 }, /*W# 105  Src Line# 251 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 106  Src Line# 252 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x80, 0x50, 0x80, 0x94 }, /*W# 107  Src Line# 255 - SKIP 3 Lines if R5 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger*/
	  { 0x80, 0x48, 0x74, 0x00 }, /*W# 108  Src Line# 257 - 1BYTE WRITE 0x80 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7*/
	  { 0xF7, 0x50, 0x74, 0x40 }, /*W# 109  Src Line# 260 - Readback PBS_CLIENT3_SCRATCH1 and store to Buffer R7*/
	  { 0x11, 0x70, 0xFF, 0x90 }, /*W# 110  Src Line# 261 - SKIP 1 Lines if R7 NOT EQUAL 0x11 (MASK=0xFF)*/
	  { 0x24, 0x0E, 0xFF, 0x83 }, /*W# 111  Src Line# 263 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 112  Src Line# 272 - End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.*/
	  { 0x90, 0x0A, 0xFF, 0x83 }, /*W# 113  Src Line# 275 - GOTO MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 114  Src Line# 276 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 115  Src Line# 279 - */
	  { 0x90, 0x0B, 0xFF, 0x9B }, /*W# 116  Src Line# 280 - */
	  { 0x50, 0x0A, 0xFF, 0x83 }, /*W# 117  Src Line# 281 - GOTO CLIENT0_POFF_FAULT_COMMON; Go to seq common between fault and POFF*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 118  Src Line# 282 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 119  Src Line# 283 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 120  Src Line# 284 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 121  Src Line# 285 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 122  Src Line# 286 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 123  Src Line# 287 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 124  Src Line# 288 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 125  Src Line# 291 - */
	  { 0x90, 0x0B, 0xFF, 0x9B }, /*W# 126  Src Line# 292 - */
	  { 0x02, 0x00, 0x70, 0x28 }, /*W# 127  Src Line# 293 - 1BYTE R0 = 0x02; R0 = 0x2*/
	  { 0xA0, 0x0A, 0x08, 0x88 }, /*W# 128  Src Line# 294 - GOTO INIT_RESET PON; //FIX: GOTOSUB_NOID still not working.  Do reset stuff common to warm reset and POFF*/
	  { 0x42, 0x90, 0x16, 0x00 }, /*W# 129  Src Line# 295 - 1BYTE WRITE 0x42 TO SCHG8998_MISC__STAT_CFG; Force STAT low ([7:6]=b'01) so that SMB goes to suspend*/
	  { 0xC4, 0x80, 0x13, 0x00 }, /*W# 130  Src Line# 296 - 1BYTE WRITE 0xC4 TO SCHG8998_USB__USBIN_AICL_OPTIONS_CFG; Charger-WA20. Revert register to FTRIM value upon WR.*/
	  { 0x00, 0x52, 0x11, 0x00 }, /*W# 131  Src Line# 297 - 1BYTE WRITE 0x00 TO SCHG8998_OTG__OTG_CURRENT_LIMIT_CFG; Charger-WA21: Revert register to FTRIM value upon WR*/
	  { 0x00, 0xC2, 0x14, 0x10 }, /*W# 132  Src Line# 298 - 1BYTE WRITE 0x00 TO SCHG8998_DC__ENG_SSUPPLY_CFG3; Charger-WA21: Revert register to FTRIM value upon WR*/
	  { 0x04, 0xDF, 0x14, 0x00 }, /*W# 133  Src Line# 299 - 1BYTE WRITE 0x04 TO SCHG8998_DC__CLOCK_CONTROL; CR176140: Set chicken bit high to avoid lockup at following PON.*/
	  { 0x14, 0x00, 0xFF, 0x83 }, /*W# 134  Src Line# 300 - GOTO OTP_PON (OTP); Jump to a fixed location in OTP.  We are going to re-run the PON sequence.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 135  Src Line# 301 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 136  Src Line# 302 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 137  Src Line# 303 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 138  Src Line# 304 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 139  Src Line# 305 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 140  Src Line# 306 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 141  Src Line# 307 - */
	  { 0xEA, 0x01, 0xFF, 0xC1 }, /*W# 142  Src Line# 310 - Wait 15ms to delay BOB disable (i.e. allow child LDO's on PM845 to disable in sequence)*/
	  { 0x90, 0x0B, 0xFF, 0x9B }, /*W# 143  Src Line# 311 - */
	  { 0x04, 0x00, 0x70, 0x28 }, /*W# 144  Src Line# 312 - 1BYTE R0 = 0x04; R0=0x4. This value will be applied to the PON_RESET_CTL register in PON (bit 2 warm_rb*/
	  { 0xA0, 0x0A, 0x08, 0x88 }, /*W# 145  Src Line# 313 - GOTO INIT_RESET PON; //FIX: GOTOSUB_NOID still not working.  Do reset stuff common to warm reset and POFF*/
	  { 0x0C, 0x10, 0xA0, 0x89 }, /*W# 146  Src Line# 314 - */
	  { 0x00, 0x46, 0x05, 0x00 }, /*W# 147  Src Line# 315 - 1BYTE WRITE 0x00 TO INT__EN_CTL1; Disable INT*/
	  { 0x42, 0x90, 0x16, 0x00 }, /*W# 148  Src Line# 322 - 1BYTE WRITE 0x42 TO SCHG8998_MISC__STAT_CFG; Force STAT low ([7:6]=b'01) so that SMB goes to suspend*/
	  { 0x01, 0x45, 0x16, 0x00 }, /*W# 149  Src Line# 323 - 1BYTE WRITE 0x01 TO SCHG8998_MISC__SHDN_CMD; Enable Charger Shutdown. CR-0000171925*/
	  { 0x00, 0x46, 0x40, 0x00 }, /*W# 150  Src Line# 324 - 1BYTE WRITE 0x00 TO FG_BATT_SOC__EN_CTL; Pause FG algorithm (CR-0000171925).  This bit emulatoes what the SHDN_N HW signal did on earlier PMIs.*/
	  { 0x05, 0x52, 0x45, 0x00 }, /*W# 151  Src Line# 325 - 1BYTE WRITE 0x05 TO FG_ADC_RR__RR_ADC_CTL; Put RRADC in shutdown (this is like pausing the RRADC*/
	  { 0x02, 0x66, 0x13, 0x00 }, /*W# 152  Src Line# 326 - 1BYTE WRITE 0x02 TO SCHG8998_USB__USBIN_ICL_OPTIONS; Revert USB5/1 setting to 500mA*/
	  { 0x38, 0x63, 0x13, 0x00 }, /*W# 153  Src Line# 327 - 1BYTE WRITE 0x38 TO SCHG8998_USB__USBIN_OPTIONS_2_CFG; Revert Wipwr_RstN pin functionality to normal WiPwr operation (SBL changes it to EUD)*/
	  { 0x24, 0x68, 0x13, 0x00 }, /*W# 154  Src Line# 328 - 1BYTE WRITE 0x24 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; Charger WA-31. Revert to UFP mode (same as FTRIM setting) to avoid unexpected PON.*/
	  { 0x04, 0xDF, 0x14, 0x00 }, /*W# 155  Src Line# 329 - 1BYTE WRITE 0x04 TO SCHG8998_DC__CLOCK_CONTROL; CR176140: Set chicken bit high to avoid lockup at following PON.*/
	  { 0x00, 0x6B, 0x41, 0x00 }, /*W# 156  Src Line# 330 - 1BYTE WRITE 0x00 TO FG_BATT_INFO__BATTERY_MISSING_CFG; Disable BMD*/
	  { 0x46, 0x01, 0xFF, 0xC1 }, /*W# 157  Src Line# 331 - WAIT 10010 us; Wait 10ms for VREGs to discharge completely before allowing the next PON trigger*/
	  { 0x0C, 0x10, 0x2C, 0x88 }, /*W# 158  Src Line# 332 - GOTO ROM_DISABLE_WAIT (ROM) MBG1; Disable MBG*/
	  { 0x40, 0x91, 0x08, 0x00 }, /*W# 159  Src Line# 333 - 1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 160  Src Line# 334 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 161  Src Line# 335 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 162  Src Line# 336 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 163  Src Line# 337 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 164  Src Line# 340 - */
	  { 0x45, 0x01, 0xFF, 0xC1 }, /*W# 165  Src Line# 341 - WAIT 9980 us; Delay to let regulators discharge*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 166  Src Line# 342 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 167  Src Line# 343 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 168  Src Line# 346 - NOP; //This Subroutine does several things that are common to the Warm Reset and POFF seqeunces.  1) disable SPMI to avoid lockup*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 169  Src Line# 348 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 170  Src Line# 349 - NOP; Call RTRR subroutine*/
	  { 0xF0, 0x98, 0x08, 0x18 }, /*W# 171  Src Line# 350 - 1BYTE WRITE R0 TO SID:0 0x0800__0x98; Assert whatever resets are defined in R0*/
	  { 0x01, 0x00, 0xFF, 0xC1 }, /*W# 172  Src Line# 351 - WAIT 91 us; Wait 90 uS for reset to propagate through clock domains*/
	  { 0x06, 0x98, 0x08, 0x10 }, /*W# 173  Src Line# 352 - 1BYTE WRITE 0x06 TO SID:0 0x0800__0x98; Deassert Resets*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 174  Src Line# 353 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 175  Src Line# 354 - */
	  { 0x00, 0x46, 0xDC, 0x01 }, /*W# 176  Src Line# 357 - 1BYTE WRITE 0x00 TO IBB__ENABLE_CTL; Disable IBB*/
	  { 0x00, 0x46, 0xDE, 0x01 }, /*W# 177  Src Line# 358 - 1BYTE WRITE 0x00 TO LAB__ENABLE_CTL; Disable LAB*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 178  Src Line# 359 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 179  Src Line# 360 - */
	  { 0xF0, 0x10, 0x12, 0x40 }, /*W# 180  Src Line# 363 - 1BYTE READ SCHG8998_BATIF__INT_RT_STS TO R0; Read status register*/
	  { 0x00, 0x01, 0x40, 0x91 }, /*W# 181  Src Line# 364 - SKIP 2 Lines if R0 EQUAL 0x00 (MASK=0x40); Skip next two lines*/
	  { 0x1C, 0x00, 0x01, 0x88 }, /*W# 182  Src Line# 366 - GOTO OTP_QI_PMA_CONFIG (OTP) REVID; Load Qi/PMA charger settings*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 183  Src Line# 367 - */
	  { 0x20, 0x00, 0x01, 0x88 }, /*W# 184  Src Line# 370 - GOTO OTP_WIPWR_CONFIG (OTP) REVID; Load WiPwr Settings*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 185  Src Line# 371 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 186  Src Line# 372 - */
	  { 0xF7, 0x50, 0x72, 0x40 }, /*W# 187  Src Line# 379 - 1BYTE READ PBS_CLIENT1__SCRATCH1 TO R7; Readback PBS_CLIENT1_SCRATCH1 and store to Buffer R7*/
	  { 0x04, 0x70, 0xFF, 0x90 }, /*W# 188  Src Line# 381 - SKIP 1 Lines if R7 NOT EQUAL 0x04 (MASK=0xFF)*/
	  { 0x14, 0x0B, 0xFF, 0x83 }, /*W# 189  Src Line# 383 - */
	  { 0x03, 0x70, 0xFF, 0x90 }, /*W# 190  Src Line# 386 - SKIP 1 Lines if R7 NOT EQUAL 0x03 (MASK=0xFF)*/
	  { 0x3C, 0x0B, 0xFF, 0x83 }, /*W# 191  Src Line# 388 - */
	  { 0x02, 0x70, 0xFF, 0x90 }, /*W# 192  Src Line# 391 - SKIP 1 Lines if R7 NOT EQUAL 0x02 (MASK=0xFF)*/
	  { 0x50, 0x0B, 0xFF, 0x83 }, /*W# 193  Src Line# 393 - */
	  { 0x01, 0x70, 0xFF, 0x90 }, /*W# 194  Src Line# 396 - SKIP 1 Lines if R7 NOT EQUAL 0x01 (MASK=0xFF)*/
	  { 0x5C, 0x0B, 0xFF, 0x83 }, /*W# 195  Src Line# 398 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 196  Src Line# 401 - */
	  { 0x00, 0xDF, 0x14, 0x00 }, /*W# 197  Src Line# 404 - 1BYTE WRITE 0x00 TO SCHG8998_DC__CLOCK_CONTROL; Set Use200k=0. Charger clock may lock up at this command CR-0000176241)*/
	  { 0x04, 0xDF, 0x14, 0x00 }, /*W# 198  Src Line# 405 - 1BYTE WRITE 0x04 TO SCHG8998_DC__CLOCK_CONTROL; CR-0000176241: Toggle chicken bit to unlock charger*/
	  { 0xC0, 0x03, 0xFE, 0xC1 }, /*W# 199  Src Line# 406 - WAIT 50052 ns; wait 50us*/
	  { 0x00, 0xDF, 0x14, 0x00 }, /*W# 200  Src Line# 407 - 1BYTE WRITE 0x00 TO SCHG8998_DC__CLOCK_CONTROL; CR-0000176241: Toggle chicken bit to unlock charger*/
	  { 0xC0, 0x03, 0xFE, 0xC1 }, /*W# 201  Src Line# 411 - wait 50us*/
	  { 0x09, 0xDF, 0x14, 0x00 }, /*W# 202  Src Line# 412 - Set bit0=1 (to request 19.2MHz clock), set bit3=1 (to set the flag)*/
	  { 0x01, 0x4A, 0x59, 0x10 }, /*W# 203  Src Line# 415 - 1BYTE WRITE 0x01 TO CLK_DIST__PMIC_SLEEP_CTL; Set SLEEP_B to follow HW*/
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 204  Src Line# 416 - */
	  { 0x04, 0x51, 0x72, 0x00 }, /*W# 205  Src Line# 417 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 206  Src Line# 418 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 207  Src Line# 421 - */
	  { 0x00, 0x4A, 0x59, 0x10 }, /*W# 208  Src Line# 422 - 1BYTE WRITE 0x00 TO CLK_DIST__PMIC_SLEEP_CTL; Force SLEEP_B low*/
	  { 0x02, 0xDF, 0x14, 0x00 }, /*W# 209  Src Line# 424 - 1BYTE WRITE 0x02 TO SCHG8998_DC__CLOCK_CONTROL; Use 200k clock during sleep instead of gated 19.2MHz (~20% duty cycle)*/
	  { 0x03, 0x51, 0x72, 0x00 }, /*W# 210  Src Line# 425 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 211  Src Line# 426 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 212  Src Line# 429 - */
	  { 0x02, 0x51, 0x72, 0x00 }, /*W# 213  Src Line# 430 - No sequence, just placeholder*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 214  Src Line# 431 - */
	  { 0xFF, 0xFF, 0xFF, 0xFB }, /*W# 215  Src Line# 434 - */
	  { 0x01, 0x51, 0x72, 0x00 }, /*W# 216  Src Line# 435 - No sequence, just placeholder*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 217  Src Line# 436 - */
	  { 0x00, 0x64, 0xDE, 0x11 }, /*W# 218  Src Line# 443 - 1BYTE WRITE 0x00 TO LAB__SPARE_CTL; Undo Force Async mode. LAB goes back to auto" mode"*/
	  { 0x60, 0x00, 0xFE, 0xC1 }, /*W# 219  Src Line# 444 - 5usec*/
	  { 0xF0, 0x09, 0x09, 0x40 }, /*W# 220  Src Line# 445 - 1BYTE READ MISC__STATUS2 TO R0; Read DTEST status*/
	  { 0x01, 0x01, 0x01, 0x92 }, /*W# 221  Src Line# 446 - SKIP 1 Lines if R0 EQUAL 0x01 (MASK=0x01); If DTEST1=1 (ASync Mode)*/
	  { 0xC0, 0x03, 0xFE, 0xC1 }, /*W# 222  Src Line# 448 - WAIT 50052 ns; wait 50us*/
	  { 0x20, 0x61, 0xDE, 0x11 }, /*W# 223  Src Line# 449 - 1BYTE WRITE 0x20 TO LAB__PWM_CTRL_3; Decrease minD to default x2CLK*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 224  Src Line# 450 - */
	  { 0x01, 0x64, 0xDE, 0x11 }, /*W# 225  Src Line# 453 - 1BYTE WRITE 0x01 TO LAB__SPARE_CTL; Force Async mode*/
	  { 0x60, 0x61, 0xDE, 0x11 }, /*W# 226  Src Line# 454 - 1BYTE WRITE 0x60 TO LAB__PWM_CTRL_3; Increase minD to x8CLK*/
	  { 0x00, 0x80, 0xFF, 0xC5 }, /*W# 227  Src Line# 455 - ; Start a 1s EOSR timer*/
	  { 0xBC, 0x0F, 0x70, 0x29 }, /*W# 228  Src Line# 459 - MEMADDR(END_PON_REASONS_PREVIOUS); Load R0 with the copy address*/
	  { 0xC4, 0x0F, 0x72, 0x29 }, /*W# 229  Src Line# 460 - MEMADDR(END_PON_REASONS_PREVIOUS2); Load R2 with the paste address*/
	  { 0x04, 0x00, 0x78, 0x28 }, /*W# 230  Src Line# 462 - 1BYTE R8 = 0x04; Loop 4 times (shift 4 DWORDS)*/
	  { 0xF0, 0xFF, 0xF4, 0xA3 }, /*W# 231  Src Line# 469 - 4BYTE R4 = MEMADDR(R0); copy the last reason registers into a buffer*/
	  { 0xF2, 0xFF, 0xF4, 0xA7 }, /*W# 232  Src Line# 470 - 4BYTE MEMADDR(R2) = R4; write the reasons to the location pointed to by the paste pointer*/
	  { 0x01, 0x00, 0x78, 0x24 }, /*W# 233  Src Line# 471 - */
	  { 0x00, 0x81, 0xFF, 0x92 }, /*W# 234  Src Line# 472 - SKIP 3 Lines if R8 EQUAL 0x00 (MASK=0xFF); Loop Until loop count (R8) = 4*/
	  { 0x04, 0x00, 0x70, 0x25 }, /*W# 235  Src Line# 474 - 2BYTE R0 = R0 - 0x04; Move read pointer to next DWORD*/
	  { 0x04, 0x00, 0x72, 0x25 }, /*W# 236  Src Line# 475 - 2BYTE R2 = R2 - 0x04; Move write pointer to next DWORD*/
	  { 0x9C, 0x0B, 0xFF, 0x83 }, /*W# 237  Src Line# 476 - */
	  { 0xF4, 0xC0, 0x08, 0x40 }, /*W# 238  Src Line# 479 - */
	  { 0xF5, 0xC2, 0x08, 0x40 }, /*W# 239  Src Line# 480 - */
	  { 0xF6, 0xC4, 0x08, 0x40 }, /*W# 240  Src Line# 481 - */
	  { 0xF7, 0xC5, 0x08, 0x40 }, /*W# 241  Src Line# 482 - */
	  { 0xF0, 0xFF, 0xF4, 0xA7 }, /*W# 242  Src Line# 483 - 4BYTE MEMADDR(R0) = R4; Write data to R0 (the last register that we copied data out of)*/
	  { 0x04, 0x00, 0x70, 0x21 }, /*W# 243  Src Line# 484 - 2BYTE R0 = R0 + 0x04; Increment pointer to next DWord*/
	  { 0xF4, 0xC7, 0x08, 0x40 }, /*W# 244  Src Line# 485 - */
	  { 0xF5, 0xC8, 0x08, 0x40 }, /*W# 245  Src Line# 486 - */
	  { 0xF6, 0xC9, 0x08, 0x40 }, /*W# 246  Src Line# 487 - */
	  { 0xFF, 0x00, 0x77, 0x28 }, /*W# 247  Src Line# 488 - 1BYTE R7 = 0xFF; Leave RTC blank (0xFF) since RTC is not available on PMi*/
	  { 0xF0, 0xFF, 0xF4, 0xA7 }, /*W# 248  Src Line# 489 - 4BYTE MEMADDR(R0) = R4; Write data to address stored in R0*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 249  Src Line# 490 - End of subroutine. PC = Stored PC.; Byte-2*/
	  { 0x38, 0x53, 0x11, 0x00 }, /*W# 250  Src Line# 498 - 1BYTE WRITE 0x38 TO SCHG8998_OTG__OTG_CFG; CR175539: FTRIM value=0x2A. Set [4]=1 and [1]=0 -->0x38*/
	  { 0x01, 0x40, 0x11, 0x00 }, /*W# 251  Src Line# 499 - 1BYTE WRITE 0x01 TO SCHG8998_OTG__CMD_OTG; Enable OTG*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 252  Src Line# 500 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 253  Src Line# 501 - */
	  { 0xF1, 0x51, 0x41, 0x40 }, /*W# 254  Src Line# 504 - 1BYTE READ FG_BATT_INFO__BATTERY_TEMPERATURE_MSB TO R1; Read Batt temp MSB to R1*/
	  { 0xF0, 0x50, 0x41, 0x40 }, /*W# 255  Src Line# 505 - 1BYTE READ FG_BATT_INFO__BATTERY_TEMPERATURE_LSB TO R0; Read Batt temp LSB to R0*/
	  { 0xE0, 0x0F, 0x76, 0x29 }, /*W# 256  Src Line# 513 - Load R6 with first word of AFP DATASET*/
	  { 0xF6, 0xFF, 0x74, 0xA2 }, /*W# 257  Src Line# 514 - Load R4 with data from location pointed to by R6 (detection value); R4 = Hot Threshold*/
	  { 0xE4, 0x0F, 0x76, 0x29 }, /*W# 258  Src Line# 515 - Load R6 with next word of AFP DATASET*/
	  { 0xF6, 0xFF, 0x75, 0xA2 }, /*W# 259  Src Line# 516 - Load R5 with data from location pointed to by R6 (detection value); R5 = Cold Threshold*/
	  { 0xFF, 0xFF, 0x71, 0x38 }, /*W# 260  Src Line# 519 - 1BYTE SHIFT R1 LEFT BY 1BIT; Next 5 commands shift R1 [2:0] to [7:5]*/
	  { 0xFF, 0xFF, 0x71, 0x38 }, /*W# 261  Src Line# 520 - */
	  { 0xFF, 0xFF, 0x71, 0x38 }, /*W# 262  Src Line# 521 - */
	  { 0xFF, 0xFF, 0x71, 0x38 }, /*W# 263  Src Line# 522 - */
	  { 0xFF, 0xFF, 0x71, 0x38 }, /*W# 264  Src Line# 523 - */
	  { 0xFF, 0xFF, 0x70, 0x3C }, /*W# 265  Src Line# 524 - 1BYTE SHIFT R0 RIGHT BY 1BIT; Next 3 commands shit R0 [7:3] to [4:0]*/
	  { 0xFF, 0xFF, 0x70, 0x3C }, /*W# 266  Src Line# 525 - */
	  { 0xFF, 0xFF, 0x70, 0x3C }, /*W# 267  Src Line# 526 - */
	  { 0xF1, 0xF0, 0xF2, 0x32 }, /*W# 268  Src Line# 528 - 1BYTE R2 = R0 OR R1; OR(R0*/
	  { 0xF2, 0xF4, 0xF6, 0x26 }, /*W# 269  Src Line# 534 - 1BYTE R6 = R2 - R4; R6 = Temp - HotThresh --> carry=1 if temp < HT*/
	  { 0x01, 0x66, 0xFF, 0x90 }, /*W# 270  Src Line# 535 - SKIP 1 Lines if R6 CARRY 0x01 (MASK=0xFF); Skip if Carry=1*/
	  { 0x01, 0x41, 0x16, 0x10 }, /*W# 271  Src Line# 537 - 1BYTE WRITE 0x01 TO SCHG8998_MISC__AFP_MODE; Temp > Hot Threshold*/
	  { 0xF5, 0xF2, 0xF6, 0x26 }, /*W# 272  Src Line# 544 - 1BYTE R6 = R5 - R2; R6 = Cold Thresh - Temp --> carry=1 if temp > CT*/
	  { 0x01, 0x66, 0xFF, 0x90 }, /*W# 273  Src Line# 545 - SKIP 1 Lines if R6 CARRY 0x01 (MASK=0xFF); Skip if carry=1*/
	  { 0x01, 0x41, 0x16, 0x10 }, /*W# 274  Src Line# 547 - 1BYTE WRITE 0x01 TO SCHG8998_MISC__AFP_MODE; Temp < Cold Threshold*/
	  { 0x00, 0x80, 0xFF, 0xC5 }, /*W# 275  Src Line# 554 - ; Set timer to 1s*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 276  Src Line# 555 - */
	  { 0xC0, 0x00, 0xFE, 0xC1 }, /*W# 277  Src Line# 562 - WAIT 10052 ns; wait 10us*/
	  { 0x00, 0x0B, 0xDC, 0x01 }, /*W# 278  Src Line# 563 - 1BYTE WRITE 0x00 TO IBB__STATUS4; Write to status reg before reading pulse count*/
	  { 0xF0, 0x0B, 0xDC, 0x41 }, /*W# 279  Src Line# 564 - 1BYTE READ IBB__STATUS4 TO R0; R0 = Status4*/
	  { 0xF3, 0x65, 0xDC, 0x41 }, /*W# 280  Src Line# 565 - 1BYTE READ IBB__SWIRE_WLED_CTL2 TO R3; R3 = AVDD_START*/
	  { 0x52, 0x31, 0xFF, 0x90 }, /*W# 281  Src Line# 566 - SKIP 1 Lines if R3 EQUAL 0x52 (MASK=0xFF); Skip next line if AVDD_Start = 41'd*/
	  { 0x7C, 0x0C, 0xFF, 0x83 }, /*W# 282  Src Line# 568 - GOTO Pulses_GT_49; AVDD_START !=41*/
	  { 0x80, 0x00, 0xF0, 0x30 }, /*W# 283  Src Line# 571 - 1BYTE R0 = R0 OR 0x80; Set R0 bit 7 high*/
	  { 0xF0, 0x4F, 0xD8, 0x09 }, /*W# 284  Src Line# 572 - 1BYTE WRITE R0 TO WLED1_CTRL__AMOLED_VOUT; AMOLED_VOUT = R0*/
	  { 0x62, 0x65, 0xDC, 0x01 }, /*W# 285  Src Line# 573 - 1BYTE WRITE 0x62 TO IBB__SWIRE_WLED_CTL2; Set AVDD_START = 49'd*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 286  Src Line# 574 - */
	  { 0x0F, 0x00, 0xFF, 0x93 }, /*W# 287  Src Line# 577 - SKIP 4 Lines if R0 NOT EQUAL 0x0F (MASK=0xFF); Skip next 4 lines if Status4 != F*/
	  { 0x80, 0x47, 0xDC, 0x01 }, /*W# 288  Src Line# 585 - 1BYTE WRITE 0x80 TO IBB__PD_CTL; Status4 = F*/
	  { 0x00, 0x47, 0xDE, 0x01 }, /*W# 289  Src Line# 586 - */
	  { 0x9A, 0x5E, 0xD8, 0x01 }, /*W# 290  Src Line# 587 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 291  Src Line# 588 - */
	  { 0x0D, 0x00, 0xFF, 0x93 }, /*W# 292  Src Line# 591 - SKIP 4 Lines if R0 NOT EQUAL 0x0D (MASK=0xFF); Skip next 4 lines if Status4 != D*/
	  { 0x00, 0x47, 0xDC, 0x01 }, /*W# 293  Src Line# 599 - 1BYTE WRITE 0x00 TO IBB__PD_CTL; Status4 = D*/
	  { 0x02, 0x47, 0xDE, 0x01 }, /*W# 294  Src Line# 600 - */
	  { 0x92, 0x5E, 0xD8, 0x01 }, /*W# 295  Src Line# 601 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 296  Src Line# 602 - */
	  { 0x07, 0x00, 0xFF, 0x91 }, /*W# 297  Src Line# 605 - SKIP 2 Lines if R0 NOT EQUAL 0x07 (MASK=0xFF); Skip next line if Status4 != 7*/
	  { 0x01, 0x00, 0x74, 0x28 }, /*W# 298  Src Line# 607 - 1BYTE R4 = 0x01; Status4=7 --> VDISP=4.7V --> R4=1*/
	  { 0xD8, 0x0C, 0xFF, 0x83 }, /*W# 299  Src Line# 608 - */
	  { 0x05, 0x00, 0xFF, 0x91 }, /*W# 300  Src Line# 611 - SKIP 2 Lines if R0 NOT EQUAL 0x05 (MASK=0xFF); Skip next line if Status4 != 5*/
	  { 0x02, 0x00, 0x74, 0x28 }, /*W# 301  Src Line# 613 - 1BYTE R4 = 0x02; Status4=5 --> VDISP=4.8V --> R4=2*/
	  { 0xD8, 0x0C, 0xFF, 0x83 }, /*W# 302  Src Line# 614 - */
	  { 0x03, 0x00, 0xFF, 0x91 }, /*W# 303  Src Line# 617 - SKIP 2 Lines if R0 NOT EQUAL 0x03 (MASK=0xFF); Skip next line if Status4 != 3*/
	  { 0x03, 0x00, 0x74, 0x28 }, /*W# 304  Src Line# 619 - 1BYTE R4 = 0x03; Status4=3 --> VDISP=4.9V --> R4=3*/
	  { 0xD8, 0x0C, 0xFF, 0x83 }, /*W# 305  Src Line# 620 - */
	  { 0x01, 0x00, 0xFF, 0x91 }, /*W# 306  Src Line# 623 - SKIP 2 Lines if R0 NOT EQUAL 0x01 (MASK=0xFF); Skip next line if Status4 != 1*/
	  { 0x04, 0x00, 0x74, 0x28 }, /*W# 307  Src Line# 625 - 1BYTE R4 = 0x04; Status4=1 --> VDISP=5.0V --> R4=4*/
	  { 0xD8, 0x0C, 0xFF, 0x83 }, /*W# 308  Src Line# 626 - */
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 309  Src Line# 629 - */
	  { 0x80, 0x00, 0xF4, 0x30 }, /*W# 310  Src Line# 632 - 1BYTE R4 = R4 OR 0x80; Set bit7 of R4 high to set the override bit. R4 = Desired Voltage*/
	  { 0xF5, 0x41, 0xDE, 0x41 }, /*W# 311  Src Line# 633 - 1BYTE READ LAB__OUTPUT_VOLTAGE TO R5; Read existing LAB Vout*/
	  { 0x80, 0x00, 0xF5, 0x30 }, /*W# 312  Src Line# 634 - 1BYTE R5 = R5 OR 0x80; Set bit7 of R5 high to set the override bit. R5 = Existing Voltage*/
	  { 0xF5, 0xF4, 0xF6, 0x26 }, /*W# 313  Src Line# 641 - 1BYTE R6 = R5 - R4; R6 = R5 - R4 (existing Vout - desired Vout)*/
	  { 0x01, 0x66, 0xFF, 0x95 }, /*W# 314  Src Line# 642 - SKIP 6 Lines if R6 CARRY 0x01 (MASK=0xFF); Skip if Carry=1 --> current Vout < desired Vout --> go to step up*/
	  { 0x00, 0x60, 0xFF, 0x90 }, /*W# 315  Src Line# 644 - SKIP 1 Lines if R6 NOT EQUAL 0x00 (MASK=0xFF); Carry=0*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 316  Src Line# 646 - End of Sequence; Carry=0 & R6=0 --> Current Vout = Desired Vout*/
	  { 0x01, 0x00, 0x75, 0x24 }, /*W# 317  Src Line# 649 - 1BYTE R5 = R5 - 0x01; R5 = R5 - 1 (step down by 1LSB = 100mV)*/
	  { 0xF5, 0x41, 0xDE, 0x09 }, /*W# 318  Src Line# 650 - 1BYTE WRITE R5 TO LAB__OUTPUT_VOLTAGE; LAB Vout = R5*/
	  { 0x60, 0x00, 0xFE, 0xC1 }, /*W# 319  Src Line# 651 - WAIT 5052 ns; Wait 5us*/
	  { 0xE4, 0x0C, 0xFF, 0x83 }, /*W# 320  Src Line# 652 - */
	  { 0x01, 0x00, 0x75, 0x20 }, /*W# 321  Src Line# 655 - 1BYTE R5 = R5 + 0x01; R5 = R5 + 1 (step up by 1LSB = 100mV)*/
	  { 0xF5, 0x41, 0xDE, 0x09 }, /*W# 322  Src Line# 656 - 1BYTE WRITE R5 TO LAB__OUTPUT_VOLTAGE; LAB Vout = R5*/
	  { 0x60, 0x00, 0xFE, 0xC1 }, /*W# 323  Src Line# 657 - WAIT 5052 ns; Wait 5us*/
	  { 0xE4, 0x0C, 0xFF, 0x83 }, /*W# 324  Src Line# 658 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 325  Src Line# 659 - */
	  { 0xF0, 0xDF, 0x13, 0x40 }, /*W# 326  Src Line# 662 - 1BYTE READ SCHG8998_USB__SPARE TO R0; UNUSED!!! No longer need WA2. CHG trigger pointed directly to WA1*/
	  { 0x01, 0x01, 0xFF, 0x90 }, /*W# 327  Src Line# 663 - SKIP 1 Lines if R0 EQUAL 0x01 (MASK=0xFF); Skip next like if spare bit=1*/
	  { 0x4C, 0x0D, 0xFF, 0x83 }, /*W# 328  Src Line# 665 - GOTO Client1_SCHG_PD_WA1_Crude_Sensor; Spare bit = 0 --> GOTO crude sensor WA*/
	  { 0xF0, 0x0E, 0x13, 0x40 }, /*W# 329  Src Line# 668 - 1BYTE READ SCHG8998_USB__TYPE_C_STATUS_4 TO R0; Read [7] UFP_DFP_MODE*/
	  { 0xF1, 0x68, 0x13, 0x40 }, /*W# 330  Src Line# 669 - 1BYTE READ SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL TO R1; R1 = Read (0x1368)*/
	  { 0x00, 0x01, 0x80, 0x92 }, /*W# 331  Src Line# 670 - SKIP 3 Lines if R0 EQUAL 0x00 (MASK=0x80); Skip next line if [7]=0 i.e. UFP mode*/
	  { 0x04, 0x00, 0xF1, 0x30 }, /*W# 332  Src Line# 678 - 1BYTE R1 = R1 OR 0x04; R1 = R1 OR 0x4 --> Set bit[2] = 1*/
	  { 0xF1, 0x68, 0x13, 0x08 }, /*W# 333  Src Line# 679 - 1BYTE WRITE R1 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; Set 0x1368[2]=1*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 334  Src Line# 680 - */
	  { 0xFB, 0x00, 0x71, 0x20 }, /*W# 335  Src Line# 683 - 1BYTE R1 = R1 + 0xFB; R1 = R1 AND 0xFB --> Set bit[2]=0*/
	  { 0xF1, 0x68, 0x13, 0x08 }, /*W# 336  Src Line# 684 - 1BYTE WRITE R1 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; Set 0x1368[2]=0 --> Charger in DRP mode*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 337  Src Line# 685 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 338  Src Line# 686 - */
	  { 0xC4, 0x0D, 0x14, 0x88 }, /*W# 339  Src Line# 689 - added RAM v6, PMICPR-1187*/
	  { 0xF0, 0x0E, 0x13, 0x40 }, /*W# 340  Src Line# 690 - 1BYTE READ SCHG8998_USB__TYPE_C_STATUS_4 TO R0; Read [7] UFP_DFP_MODE*/
	  { 0x00, 0x01, 0x80, 0x94 }, /*W# 341  Src Line# 691 - SKIP 2 Lines if R0 EQUAL 0x00 (MASK=0x80); Skip next line if [7]=0 i.e. UFP mode*/
	  { 0xF7, 0x59, 0x13, 0x40 }, /*W# 342  Src Line# 700 - */
	  { 0x02, 0x00, 0xF7, 0x30 }, /*W# 343  Src Line# 701 - Set Bit 1 = 1*/
	  { 0xF7, 0x59, 0x13, 0x08 }, /*W# 344  Src Line# 702 - UFP_DFP_MODE=1 --> In DFP mode --> Disable crude sensor 1359[1]=1*/
	  { 0xD4, 0x0D, 0x14, 0x88 }, /*W# 345  Src Line# 705 - added RAM v6, PMICPR-1187*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 346  Src Line# 706 - */
	  { 0xF1, 0x68, 0x13, 0x40 }, /*W# 347  Src Line# 709 - 1BYTE READ SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL TO R1; Check if charger is in DRP*/
	  { 0x00, 0x10, 0x06, 0x90 }, /*W# 348  Src Line# 710 - SKIP 1 Lines if R1 NOT EQUAL 0x00 (MASK=0x06); If 0x1368[2:1] != 0*/
	  { 0x10, 0x42, 0x73, 0x00 }, /*W# 349  Src Line# 712 - 1BYTE WRITE 0x10 TO PBS_CLIENT2__TRIG_CTL; Trigger PBS Client2*/
	  { 0xD4, 0x0D, 0x14, 0x88 }, /*W# 350  Src Line# 719 - added RAM v6, PMICPR-1187*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 351  Src Line# 720 - */
	  { 0x42, 0x00, 0xFF, 0xC5 }, /*W# 352  Src Line# 723 - ; Start 2ms EOSR timer*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 353  Src Line# 724 - */
	  { 0xC4, 0x0D, 0x14, 0x88 }, /*W# 354  Src Line# 731 - added RAM v6, PMICPR-1187*/
	  { 0xF1, 0x68, 0x13, 0x40 }, /*W# 355  Src Line# 732 - 1BYTE READ SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL TO R1; R1 = 0x1368*/
	  { 0x04, 0x11, 0x04, 0x96 }, /*W# 356  Src Line# 733 - SKIP 4 Lines if R1 EQUAL 0x04 (MASK=0x04); Skip next four lines if 0x1368[2]=1*/
	  { 0x04, 0x00, 0xF1, 0x30 }, /*W# 357  Src Line# 741 - 1BYTE R1 = R1 OR 0x04; Set R1[2]=1 --> SNK_EN_CMD=1*/
	  { 0xF1, 0x68, 0x13, 0x08 }, /*W# 358  Src Line# 742 - 1BYTE WRITE R1 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; 0x1368=R1 --> SNK_EN_CMD=1*/
	  { 0xF7, 0x59, 0x13, 0x40 }, /*W# 359  Src Line# 745 - */
	  { 0xFD, 0x00, 0xF7, 0x2C }, /*W# 360  Src Line# 746 - Set Bit 1 = 0*/
	  { 0xF7, 0x59, 0x13, 0x08 }, /*W# 361  Src Line# 747 - Set 0x1359[1]=0 --> Enable Crude Sensor*/
	  { 0xD4, 0x0D, 0x14, 0x88 }, /*W# 362  Src Line# 750 - added RAM v6, PMICPR-1187*/
	  { 0x42, 0x00, 0xFF, 0xC5 }, /*W# 363  Src Line# 751 - ; Start 2ms timer*/
	  { 0xFB, 0x00, 0xF1, 0x2C }, /*W# 364  Src Line# 758 - 1BYTE R1 = R1 AND 0xFB; Set R1[2]=0 --> SNK_EN_CMD=0*/
	  { 0xF1, 0x68, 0x13, 0x08 }, /*W# 365  Src Line# 759 - 1BYTE WRITE R1 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; 0x1368=R1 --> SNK_EN_CMD=0*/
	  { 0xD4, 0x0D, 0x14, 0x88 }, /*W# 366  Src Line# 760 - added RAM v6, PMICPR-1187*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 367  Src Line# 761 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 368  Src Line# 763 - */
	  { 0xF0, 0xDF, 0x14, 0x40 }, /*W# 369  Src Line# 767 - Read 0x14DF to R0*/
	  { 0x01, 0x00, 0xF0, 0x30 }, /*W# 370  Src Line# 768 - Set bit0 = high*/
	  { 0xF0, 0xDF, 0x14, 0x08 }, /*W# 371  Src Line# 769 - Set 0x14DF = R0 (set bit 0 = 1)*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 372  Src Line# 770 - */
	  { 0xF0, 0xDF, 0x14, 0x40 }, /*W# 373  Src Line# 773 - Read 0x14DF to R0*/
	  { 0x08, 0x01, 0x08, 0x91 }, /*W# 374  Src Line# 774 - Skip next 2 lines if bit3=1 --> Apps is awake --> leave bit 0 high*/
	  { 0xFE, 0x00, 0xF0, 0x2C }, /*W# 375  Src Line# 775 - Set bit0 = low*/
	  { 0xF0, 0xDF, 0x14, 0x08 }, /*W# 376  Src Line# 776 - Set 0x14DF = R0 (set bit 0 = 0)*/
	  { 0xFF, 0xFF, 0xFF, 0x8F }, /*W# 377  Src Line# 778 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 378  Src Line# 781 - */
	  { 0xF4, 0x50, 0x73, 0x40 }, /*W# 379  Src Line# 785 - Read Scratch 1 register*/
	  { 0x02, 0x40, 0x02, 0x91 }, /*W# 380  Src Line# 786 - Skip next lines if bit 1 is not set*/
	  { 0xF4, 0x51, 0x73, 0x08 }, /*W# 381  Src Line# 788 - Set Scratch 2 = R4 to show SW that request was read*/
	  { 0x08, 0x0E, 0xFF, 0x83 }, /*W# 382  Src Line# 789 - */
	  { 0xFF, 0x51, 0x73, 0x00 }, /*W# 383  Src Line# 791 - Only bit 1 is assigned. Set Scratch 2 = 0xFF to indicate an error*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 384  Src Line# 792 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 385  Src Line# 794 - */
	  { 0xFA, 0x69, 0x41, 0x40 }, /*W# 386  Src Line# 797 - RA = ESR PD CFG*/
	  { 0x10, 0x41, 0x10, 0x91 }, /*W# 387  Src Line# 798 - Skip next 2 commands if Scratch1[4]=1*/
	  { 0x02, 0x00, 0xFA, 0x30 }, /*W# 388  Src Line# 802 - Scratch 1 [4] = 0 means exit QM.  Set ESR_PD_MODE to CRG_DSC PD*/
	  { 0x1C, 0x0E, 0xFF, 0x83 }, /*W# 389  Src Line# 803 - */
	  { 0x0C, 0x00, 0xFA, 0x2C }, /*W# 390  Src Line# 805 - Scratch 1 [4] = 1 means enter QM.  Set ESR_PD_MODE to NO_PULL*/
	  { 0xFA, 0x69, 0x41, 0x08 }, /*W# 391  Src Line# 807 - Set 0x4169 = RA*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 392  Src Line# 808 - */
	  { 0xE0, 0x0F, 0x76, 0x29 }, /*W# 393  Src Line# 815 - Load R6 with first word of AFP DATASET*/
	  { 0xF0, 0x8C, 0x08, 0x40 }, /*W# 394  Src Line# 816 - PERPH_RB_SPARE into R0 (Hot Threshold)*/
	  { 0xF6, 0xFF, 0x70, 0xA6 }, /*W# 395  Src Line# 817 - Load data R0 into location pointed to by R6*/
	  { 0x04, 0x00, 0x76, 0x21 }, /*W# 396  Src Line# 819 - Increment pointer by 4 to point at the next set*/
	  { 0xF0, 0x8F, 0x08, 0x40 }, /*W# 397  Src Line# 820 - SOFT_RB_SPARE into R0 (Cold Threshold)*/
	  { 0xF6, 0xFF, 0x70, 0xA6 }, /*W# 398  Src Line# 821 - Load data R0 into location pointed to by R6*/
	  { 0x11, 0x51, 0x74, 0x00 }, /*W# 399  Src Line# 823 - Indicate to SW that the sequence ran*/
	  { 0x08, 0x42, 0x74, 0x00 }, /*W# 400  Src Line# 825 - Trigger bit 3 (i.e. kickstart EOSR trigger)*/
	  { 0xFF, 0xFF, 0xFF, 0xFC }, /*W# 401  Src Line# 834 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 402  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 403  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 404  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 405  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 406  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 407  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 408  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 409  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 410  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 411  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 412  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 413  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 414  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 415  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 416  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 417  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 418  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 419  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 420  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 421  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 422  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 423  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 424  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 425  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 426  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 427  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 428  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 429  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 430  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 431  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 432  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 433  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 434  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 435  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 436  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 437  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 438  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 439  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 440  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 441  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 442  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 443  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 444  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 445  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 446  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 447  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 448  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 449  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 450  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 451  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 452  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 453  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 454  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 455  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 456  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 457  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 458  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 459  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 460  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 461  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 462  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 463  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 464  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 465  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 466  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 467  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 468  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 469  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 470  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 471  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 472  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 473  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 474  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 475  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 476  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 477  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 478  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 479  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 480  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 481  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 482  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 483  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 484  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 485  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 486  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 487  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 488  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 489  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 490  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 491  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 492  Src Line# 847 - ; 3:POFF_REASON*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 493  Src Line# 848 - ; 7:RTC_LSB*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 494  Src Line# 855 - ; 3:POFF_REASON*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 495  Src Line# 862 - ; 7:RTC_LSB*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 496  Src Line# 869 - ; 3:POFF_REASON*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 497  Src Line# 876 - ; 7:RTC_LSB*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 498  Src Line# 877 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 499  Src Line# 878 - */
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 500  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 501  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 502  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 503  Src Line# 0 - Inserted by compiler.*/
	  { 0xAC, 0xFF, 0xFF, 0xFF }, /*W# 504  Src Line# 884 - 3: spare, 2: spare, 1: spare, 0: HOT Threshold	  (0xFE0)*/
	  { 0x00, 0xFF, 0xFF, 0xFF }, /*W# 505  Src Line# 885 - 3: spare, 2: spare, 1: spare, 0: COLD Threshold	 (0xFE4)*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 506  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 507  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 508  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 509  Src Line# 0 - Inserted by compiler.*/
	  { 0xFF, 0xFF, 0xFF, 0xFF }, /*W# 510  Src Line# 0 - Inserted by compiler.*/
	  { 0x01, 0x0B, 0xFF, 0xF8 }, /*W# 511  Src Line# 0 - Inserted by compiler.*/
	},
};
