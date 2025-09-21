/*! \file  pm_config_target_pbs_ram.c
 *  
 *  \brief  File Contains the PMIC Set Mode Driver Implementation
 *  \details Set Mode Driver implementation is responsible for setting and getting 
 *  all mode settings such as Register values, memory values, etc.
 *  
 *    PMIC code generation Version: 1.0.0.0
 *    Date: 12-14-2017
 *    PMIC SBL Version: MSM8998-SBL-12142017-b0x01_v0x1A - Approved
 *    PBS RAM Version: PM8998_RAM_b0x01_v0x0D
 *    PBS RAM Version: PM8005_RAM_b0x01_v0x04
 *    PBS RAM Version: PMi8998_RAM_b0x01_v0x0A
 *    This file contains code for Target specific settings and modes.
 *  
 *  &copy; Copyright 2017 , 2018 by Qualcomm Technologies, Inc. All Rights Reserved
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.

$Header: //components/rel/boot.xf/2.2/QcomPkg/Library/PmicLib/target/sdm845_pm8998_pmi8998/psi/pm_config_target_pbs_ram.c#5 $ 
$DateTime: 2018/02/27 22:56:32 $  $Author: pwbldsvc $

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
pm_pbs_seq [ ][PBS_RAM_DATA_SIZE] =
{
   // "PM8998_RAM_b0x01_v0x0D"
   {
      //data  offset  base_addr  sid
      { 0x52,	0x53,	0x42,	0x50},	// W#0 - 	; ASCII for PBSR".  Later changed to Check Sum."
      { 0x01,	0x0D,	0x01,	0x18},	// W#1 - 	; 18=Dataset Start, 01=Header Ver, 0D01 = Rev D, Branch 1
      { 0x2C,	0x04,	0xFF,	0x83},	// W#2 - 	
      { 0x6C,	0x04,	0xFF,	0x83},	// W#3 - 	
      { 0xE0,	0x04,	0xFF,	0x83},	// W#4 - 	
      { 0x58,	0x05,	0xFF,	0x83},	// W#5 - 	
      { 0xC4,	0x0B,	0xFF,	0x83},	// W#6 - 	Fixed location for SLEEP dataset.  Do NOT move this GOTO line.
      { 0xE0,	0x0B,	0xFF,	0x83},	// W#7 - 	Fixed location for PON REASONS dataset.  Do NOT move this GOTO line.
      { 0xA4,	0x0B,	0xFF,	0x83},	// W#8 - 	Fixed location for WARM_RESET_DATASET.  Do NOT move this GOTO line.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#9 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#10 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#11 - 	
      { 0x00,	0x57,	0x08,	0x00},	// W#12 - 	1BYTE WRITE 0x00 TO PON__PMIC_WD_RESET_S2_CTL2; Disable PMIC WD during WR/POFF/FAULT
      { 0xFF,	0x48,	0x71,	0x00},	// W#13 - 	1BYTE WRITE 0xFF TO PBS_CLIENT0__TRIG_LATCHED_CLR; Clear all latched sequences in PBS PON Client 0
      { 0xFB,	0x08,	0x08,	0x40},	// W#14 - 	1BYTE READ PON__FSM_STATUS TO RB; Readback PON FSM Status and store to Buffer RB
      { 0x04,	0xB0,	0xFF,	0x90},	// W#15 - 	SKIP 1 Lines if RB NOT EQUAL 0x04 (MASK=0xFF); DVDD_CONFIG = State 4
      { 0x10,	0x00,	0xFF,	0x83},	// W#16 - 	; GOTO OTP_DVDD_CONFIG (OTP).  Using RAW command due to PDM linkage issues.
      { 0x01,	0xB0,	0xFF,	0x90},	// W#17 - 	SKIP 1 Lines if RB NOT EQUAL 0x01 (MASK=0xFF); PON = State 1
      { 0x10,	0x00,	0xFF,	0x83},	// W#18 - 	; GOTO OTP_DVDD_CONFIG (OTP).  Using RAW command due to PDM linkage issues.
      { 0x06,	0xB0,	0xFF,	0x90},	// W#19 - 	SKIP 1 Lines if RB NOT EQUAL 0x06 (MASK=0xFF); FAULT = State 6
      { 0xD0,	0x05,	0xFF,	0x83},	// W#20 - 	
      { 0x02,	0xB0,	0xFF,	0x90},	// W#21 - 	SKIP 1 Lines if RB NOT EQUAL 0x02 (MASK=0xFF); POFF = State 2
      { 0x94,	0x06,	0xFF,	0x83},	// W#22 - 	
      { 0x07,	0xB0,	0xFF,	0x90},	// W#23 - 	SKIP 1 Lines if RB NOT EQUAL 0x07 (MASK=0xFF); WARM RESET = State 7
      { 0xFC,	0x05,	0xFF,	0x83},	// W#24 - 	
      { 0x00,	0x91,	0x08,	0x00},	// W#25 - 	1BYTE WRITE 0x00 TO PON__PBS_INTERFACE; Undefined State - NACK PON (Note:  None of the Client GOTOs have a 'RETURN'; the ACK is done in each subroutine)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#26 - 	End of Sequence; Byte-2
      { 0xF3,	0x0A,	0x72,	0x40},	// W#27 - 	1BYTE READ PBS_CLIENT1__TRIG_LATCHED TO R3; Read Client 1 trigger latch status and store to Buffer R3
      { 0x01,	0x30,	0x01,	0x92},	// W#28 - 	SKIP 3 Lines if R3 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / Spare
      { 0x01,	0x48,	0x72,	0x00},	// W#29 - 	1BYTE WRITE 0x01 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#30 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#31 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x02,	0x30,	0x02,	0x92},	// W#32 - 	SKIP 8 Lines if R3 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = EOSR
      { 0x02,	0x48,	0x72,	0x00},	// W#33 - 	1BYTE WRITE 0x02 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#34 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#35 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x04,	0x30,	0x04,	0x92},	// W#36 - 	SKIP 3 Lines if R3 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = sleep_b
      { 0x04,	0x48,	0x72,	0x00},	// W#37 - 	1BYTE WRITE 0x04 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#38 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#39 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x08,	0x30,	0x08,	0x92},	// W#40 - 	SKIP 3 Lines if R3 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = LN_BB_CLK1_EN
      { 0x08,	0x48,	0x72,	0x00},	// W#41 - 	1BYTE WRITE 0x08 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#42 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#43 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x10,	0x30,	0x10,	0x91},	// W#44 - 	SKIP 3 Lines if R3 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = OCP (non-core VREG)
      { 0x10,	0x48,	0x72,	0x00},	// W#45 - 	1BYTE WRITE 0x10 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4
      { 0xB0,	0x0A,	0xFF,	0x83},	// W#46 - 	
      { 0x20,	0x30,	0x20,	0x92},	// W#47 - 	SKIP 5 Lines if R3 NOT EQUAL 0x20 (MASK=0x20); Bit 5 - Spare
      { 0x20,	0x48,	0x72,	0x00},	// W#48 - 	1BYTE WRITE 0x20 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 5
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#49 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#50 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x80,	0x30,	0x80,	0x92},	// W#51 - 	SKIP 3 Lines if R3 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger (Sleep/Wake)
      { 0x80,	0x48,	0x72,	0x00},	// W#52 - 	1BYTE WRITE 0x80 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7
      { 0x8C,	0x07,	0xFF,	0x83},	// W#53 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#54 - 	NOP; TBD
      { 0x68,	0x07,	0xFF,	0x83},	// W#55 - 	GOTO RAM_MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT
      { 0xF4,	0x0A,	0x73,	0x40},	// W#56 - 	1BYTE READ PBS_CLIENT2__TRIG_LATCHED TO R4; Read Client 2 trigger latch status and store to Buffer R4
      { 0x01,	0x40,	0x01,	0x92},	// W#57 - 	SKIP 3 Lines if R4 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / Spare
      { 0x01,	0x48,	0x73,	0x00},	// W#58 - 	1BYTE WRITE 0x01 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#59 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#60 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x02,	0x40,	0x02,	0x92},	// W#61 - 	SKIP 3 Lines if R4 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = EOSR
      { 0x02,	0x48,	0x73,	0x00},	// W#62 - 	1BYTE WRITE 0x02 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#63 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#64 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x04,	0x40,	0x04,	0x92},	// W#65 - 	SKIP 3 Lines if R4 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = GPIO23 (WCSS_VCTRL)
      { 0x04,	0x48,	0x73,	0x00},	// W#66 - 	1BYTE WRITE 0x04 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2
      { 0x04,	0x09,	0xFF,	0x83},	// W#67 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#68 - 	NOP; TBD
      { 0x08,	0x40,	0x08,	0x92},	// W#69 - 	SKIP 3 Lines if R4 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = GPIO3 (WLAN_RF_VCTRL)
      { 0x08,	0x48,	0x73,	0x00},	// W#70 - 	1BYTE WRITE 0x08 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3
      { 0xAC,	0x09,	0xFF,	0x83},	// W#71 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#72 - 	
      { 0x10,	0x40,	0x10,	0x92},	// W#73 - 	SKIP 3 Lines if R4 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = GPIO2 (spare PBS trigger)
      { 0x10,	0x48,	0x73,	0x00},	// W#74 - 	1BYTE WRITE 0x10 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#75 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#76 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x20,	0x40,	0x20,	0x92},	// W#77 - 	SKIP 3 Lines if R4 NOT EQUAL 0x20 (MASK=0x20); Bit 5 - GPIO20 (EUD)
      { 0x20,	0x48,	0x73,	0x00},	// W#78 - 	
      { 0x48,	0x09,	0xFF,	0x83},	// W#79 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#80 - 	
      { 0x80,	0x40,	0x80,	0x92},	// W#81 - 	SKIP 3 Lines if R4 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger
      { 0x80,	0x48,	0x73,	0x00},	// W#82 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#83 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#84 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x68,	0x07,	0xFF,	0x83},	// W#85 - 	GOTO RAM_MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT
      { 0xF5,	0x0A,	0x74,	0x40},	// W#86 - 	1BYTE READ PBS_CLIENT3__TRIG_LATCHED TO R5; Read Client 3 trigger latch status and store to Buffer R5
      { 0x01,	0x50,	0x01,	0x92},	// W#87 - 	SKIP 3 Lines if R5 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / Spare
      { 0x01,	0x48,	0x74,	0x00},	// W#88 - 	1BYTE WRITE 0x01 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#89 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#90 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x02,	0x50,	0x02,	0x92},	// W#91 - 	SKIP 3 Lines if R5 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = GPIO4 (spare)
      { 0x02,	0x48,	0x74,	0x00},	// W#92 - 	1BYTE WRITE 0x02 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#93 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#94 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x04,	0x50,	0x04,	0x92},	// W#95 - 	SKIP 3 Lines if R5 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = BA_N
      { 0x04,	0x48,	0x74,	0x00},	// W#96 - 	1BYTE WRITE 0x04 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#97 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#98 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x08,	0x50,	0x08,	0x92},	// W#99 - 	SKIP 3 Lines if R5 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = GPIO1 (spare)
      { 0x08,	0x48,	0x74,	0x00},	// W#100 - 	1BYTE WRITE 0x08 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#101 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#102 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x10,	0x50,	0x10,	0x92},	// W#103 - 	SKIP 3 Lines if R5 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = RTC Timer
      { 0x10,	0x48,	0x74,	0x00},	// W#104 - 	1BYTE WRITE 0x10 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#105 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#106 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x20,	0x50,	0x20,	0x92},	// W#107 - 	SKIP 3 Lines if R5 NOT EQUAL 0x20 (MASK=0x20); Bit 5 - RTC Alarm
      { 0x20,	0x48,	0x74,	0x00},	// W#108 - 	1BYTE WRITE 0x20 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 5
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#109 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#110 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x80,	0x50,	0x80,	0x92},	// W#111 - 	SKIP 3 Lines if R5 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger
      { 0x80,	0x48,	0x74,	0x00},	// W#112 - 	1BYTE WRITE 0x80 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#113 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#114 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x68,	0x07,	0xFF,	0x83},	// W#115 - 	GOTO RAM_MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#116 - 	
      { 0xD8,	0x09,	0xFF,	0x9B},	// W#117 - 	GOTO_SUB_NOID RECORD_PON_X_REASONS; Record PON REASONS into dataset (circular buffer with 3 iterations)
      { 0xE4,	0x80,	0x08,	0x00},	// W#118 - 	Re-initialize PON-trigger enables (in case SW disables them)
      { 0x40,	0x0A,	0xFF,	0x9B},	// W#119 - 	
      { 0xEA,	0x01,	0xFF,	0xC1},	// W#120 - 	WAIT 15016 us; Wait 15ms for VREGs to discharge completely before allowing the next PON trigger
      { 0xF6,	0xC8,	0x08,	0x40},	// W#121 - 	1BYTE READ PON__FAULT_REASON1 TO R6; Read FAULT_REASON1 and store to Buffer R6
      { 0x40,	0x61,	0x40,	0x90},	// W#122 - 	SKIP 1 Lines if R6 EQUAL 0x40 (MASK=0x40); Skip the next line if bit 6 (UVLO) = 1
      { 0x00,	0x7F,	0x08,	0x00},	// W#123 - 	1BYTE WRITE 0x00 TO PON__SMPL_CTL; Else
      { 0xEA,	0x01,	0xFF,	0xC1},	// W#124 - 	WAIT 15016 us; Wait 15ms to ensure PM8998 is the last to complete
      { 0x40,	0x91,	0x08,	0x00},	// W#125 - 	1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#126 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#127 - 	
      { 0x70,	0x07,	0xFF,	0x9B},	// W#128 - 	GOTO_SUB_NOID ASSERT_EXT_RESET; Set PON_RESET_N and PON_OUT low
      { 0xD8,	0x09,	0xFF,	0x9B},	// W#129 - 	GOTO_SUB_NOID RECORD_PON_X_REASONS; Record PON REASONS into dataset (circular buffer with 3 iterations)
      { 0x8E,	0x01,	0xFF,	0xC1},	// W#130 - 	WAIT 12208 us; Wait 400 SLEEP_CLK cycles (12.2ms) before setting PON_OUT high again (to signal Warm Reset to slave PMIC's)
      { 0x40,	0x49,	0x09,	0x10},	// W#131 - 	1BYTE WRITE 0x40 TO MISC__PON_OUT_CTL; Send PON_OUT high to all PMIC's.  Changing to SEC ACCESS (to updated in next .csv).
      { 0x02,	0x98,	0x08,	0x10},	// W#132 - 	1BYTE WRITE 0x02 TO PON__RESET_CTL; Assert warm_rb
      { 0x01,	0x00,	0xFF,	0xC1},	// W#133 - 	Wait 3 SLEEP_CLK (1+2) cycles for reset changes to propagate through clock domains
      { 0x06,	0x98,	0x08,	0x10},	// W#134 - 	1BYTE WRITE 0x06 TO PON__RESET_CTL; De-assert warm_rb
      { 0x07,	0x45,	0x1D,	0x01},	// W#135 - 	PX3
      { 0x07,	0x45,	0x1A,	0x01},	// W#136 - 	LV LDO's
      { 0x07,	0x45,	0x20,	0x01},	// W#137 - 	HV LDO's
      { 0x07,	0x45,	0x26,	0x01},	// W#138 - 	LV LDO's
      { 0x48,	0x40,	0x1A,	0x01},	// W#139 - 	Set S3 to 1.352V
      { 0x05,	0x41,	0x1A,	0x01},	// W#140 - 	Set S3 to 1.352V
      { 0xF8,	0x40,	0x20,	0x01},	// W#141 - 	Set S5 to 2.04V
      { 0x07,	0x41,	0x20,	0x01},	// W#142 - 	Set S5 to 2.04V
      { 0x10,	0x40,	0x26,	0x01},	// W#143 - 	Set S7 to 1.04V
      { 0x04,	0x41,	0x26,	0x01},	// W#144 - 	Set S7 to 1.04V
      { 0x80,	0x46,	0x26,	0x01},	// W#145 - 	Enable S7 if collapsed
      { 0xFC,	0x56,	0x09,	0x40},	// W#146 - 	Read PM8998.MISC.PC_SPARE1 (scratch for Pass 0/1/2) and store to RC
      { 0xA4,	0x0B,	0x76,	0x29},	// W#147 - 	Load R6 with 0xBA4 (start of WARM RESET DATASET)
      { 0xF8,	0x0A,	0x23,	0x89},	// W#148 - 	
      { 0xF8,	0x0A,	0x43,	0x89},	// W#149 - 	
      { 0xF8,	0x0A,	0x14,	0x89},	// W#150 - 	
      { 0xF8,	0x0A,	0x2C,	0x89},	// W#151 - 	
      { 0xF8,	0x0A,	0x5A,	0x89},	// W#152 - 	
      { 0xF8,	0x0A,	0x38,	0x89},	// W#153 - 	
      { 0xF8,	0x0A,	0x35,	0x89},	// W#154 - 	
      { 0x01,	0x00,	0x7C,	0x20},	// W#155 - 	Increment RC by 1
      { 0xFC,	0x56,	0x09,	0x08},	// W#156 - 	
      { 0x21,	0x07,	0xFF,	0xC1},	// W#157 - 	Wait 55ms for PMi to complete its PON sequence (rerun during WR)
      { 0x9B,	0x19,	0x76,	0x29},	// W#158 - 	; MOVI 2BYTE R6=0x199B.  2+0x199B x 30.5us = 200ms.  Store to Buffer R6.
      { 0x99,	0x00,	0x7D,	0x28},	// W#159 - 	; 1BYTE OFFSET=0x99.  Set offset to 0x99 (PON_RESET_N_CTL) for next GOTO_SUB
      { 0x18,	0x0C,	0x08,	0x88},	// W#160 - 	; Using RAW due to PDM Bug.  This SUB will deassert PON_RESET_N and wait 200ms for PS_HOLD to go high
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#161 - 	
      { 0x40,	0x91,	0x08,	0x00},	// W#162 - 	1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#163 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#164 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#165 - 	
      { 0x00,	0x7F,	0x08,	0x00},	// W#166 - 	1BYTE WRITE 0x00 TO PON__SMPL_CTL; Disable SMPL
      { 0x70,	0x07,	0xFF,	0x9B},	// W#167 - 	GOTO_SUB_NOID ASSERT_EXT_RESET; Set PON_RESET_N and PON_OUT low
      { 0xD8,	0x09,	0xFF,	0x9B},	// W#168 - 	GOTO_SUB_NOID RECORD_PON_X_REASONS; Record PON REASONS into dataset (circular buffer with 3 iterations)
      { 0x04,	0x98,	0x08,	0x10},	// W#169 - 	1BYTE WRITE 0x04 TO PON__RESET_CTL; Assert shutdown1_rb
      { 0x01,	0x00,	0xFF,	0xC1},	// W#170 - 	WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains
      { 0x06,	0x98,	0x08,	0x10},	// W#171 - 	1BYTE WRITE 0x06 TO PON__RESET_CTL; De-assert shutdown1_rb
      { 0x40,	0x0A,	0xFF,	0x9B},	// W#172 - 	
      { 0x5C,	0x07,	0x38,	0x89},	// W#173 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S13_CTRL; APC (Kryo Silver)
      { 0x5C,	0x07,	0x54,	0x89},	// W#174 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO21; SD 2.95V
      { 0x5C,	0x07,	0x4C,	0x89},	// W#175 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO13; SD I/O (PX2) 2.95V
      { 0x5C,	0x07,	0x41,	0x89},	// W#176 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO02; UFS 1.2V
      { 0x5C,	0x07,	0x53,	0x89},	// W#177 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO20; UFS 2.95V
      { 0x5C,	0x07,	0x57,	0x89},	// W#178 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO24; USB 3.075V
      { 0x00,	0x47,	0x57,	0x01},	// W#179 - 	Set LDO24 FOLLOW_HW_EN to 0 (set in EUD).  Other EUD LDO's (L1, L5, L12 are after BBCLK is disabled)
      { 0x8D,	0x02,	0xFF,	0xC1},	// W#180 - 	Wait 20ms for PM8005 to interpret PON_OUT low as shutdown (low for >20ms).  Disable S3C prior to S2A.
      { 0x5C,	0x07,	0x17,	0x89},	// W#181 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S2_CTRL; LPDDR4
      { 0x5C,	0x07,	0x51,	0x88},	// W#182 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LN_BB_CLK1; LNBBCLK1
      { 0x00,	0x47,	0x4B,	0x01},	// W#183 - 	Set LDO12 FOLLOW_HW_EN to 0 (set in SBL)
      { 0x5C,	0x07,	0x4B,	0x89},	// W#184 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO12; LNBBCLK1 and USB 1.8V
      { 0x5C,	0x07,	0x40,	0x89},	// W#185 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO01; UFS_PHY and USB 0.875V
      { 0x5C,	0x07,	0x59,	0x89},	// W#186 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO26; USB and UFS 1.2V
      { 0x5C,	0x07,	0xCD,	0x88},	// W#187 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL GPIO14; Stark CLK
      { 0x5C,	0x07,	0x55,	0x88},	// W#188 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL RF_CLK2; WLAN 38.4MHz
      { 0x5C,	0x07,	0x5A,	0x88},	// W#189 - 	
      { 0x5C,	0x07,	0x58,	0x89},	// W#190 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO25; WLAN CHAN0 3.3V
      { 0x5C,	0x07,	0x50,	0x89},	// W#191 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO17; WLAN 1.3V
      { 0x5C,	0x07,	0x44,	0x89},	// W#192 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO05; WCSS CX/MX 0.8V
      { 0x5C,	0x07,	0x46,	0x89},	// W#193 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL LDO07; WLAN XO 1.8V
      { 0x5C,	0x07,	0x0B,	0x88},	// W#194 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL VREF_MSM; 1.25V reference for UIM and SD
      { 0x5C,	0x07,	0x1D,	0x89},	// W#195 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S4_CTRL; 1.8V I/O
      { 0x5C,	0x07,	0x4A,	0x89},	// W#196 - 	
      { 0x5C,	0x07,	0x1A,	0x89},	// W#197 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S3_CTRL; Required for L17 and L26
      { 0x5C,	0x07,	0x5A,	0x89},	// W#198 - 	
      { 0x5C,	0x07,	0x2C,	0x89},	// W#199 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S9_CTRL; VDD_CX
      { 0x5C,	0x07,	0x14,	0x89},	// W#200 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S1_CTRL; EBI_PHY
      { 0x5C,	0x07,	0x43,	0x89},	// W#201 - 	
      { 0x5C,	0x07,	0x23,	0x89},	// W#202 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S6_CTRL; VDD_MX
      { 0x5C,	0x07,	0x26,	0x89},	// W#203 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S7_CTRL; Required for L1
      { 0x5C,	0x07,	0x50,	0x88},	// W#204 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL XO; OK to Skip LDO_XO?
      { 0x5C,	0x07,	0x20,	0x89},	// W#205 - 	GOTO DISABLE_WAIT_FUNCTION_LOCAL S5_CTRL; Required for L7 and L12 (S5 disable will be gated by VREG_XO)
      { 0x5C,	0x07,	0x24,	0x88},	// W#206 - 	
      { 0xF3,	0x08,	0x45,	0x41},	// W#207 - 	Read L6 STATUS1 and store to Buffer R3
      { 0x80,	0x31,	0x80,	0x90},	// W#208 - 	Skip the next line if L6 is enabled (STATUS1 bit7=1)
      { 0x5C,	0x07,	0x2C,	0x88},	// W#209 - 	DO NOT INCLUDE THIS LINE!!! - Need MBG enabled in the Off State for Secure Processor
      { 0x05,	0x45,	0x45,	0x01},	// W#210 - 	Set L6 to LPM mode in the Off State for Secure Processor
      { 0x46,	0x01,	0xFF,	0xC1},	// W#211 - 	WAIT 10010 us; Wait 10ms for VREGs to discharge completely before allowing the next PON trigger
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#212 - 	
      { 0x40,	0x91,	0x08,	0x00},	// W#213 - 	1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#214 - 	End of Sequence; Byte-2
      { 0x00,	0x46,	0xFF,	0x04},	// W#215 - 	1BYTE WRITE 0x00 TO 0x46; Write 0x00 to 0x46 (enable register in the peripheral)
      { 0x21,	0x00,	0xFF,	0xC1},	// W#216 - 	WAIT 1068 us; Wait 33+2 32kHz clock cycles.  Increased for PM8019 2.0.
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#217 - 	End of subroutine. PC = Stored PC.; Go back to next line of the calling function
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#218 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#219 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#220 - 	
      { 0x00,	0x43,	0x08,	0x02},	// W#221 - 	1BYTE WRITE 0x00 TO SID:2 0x0800__0x43; Set SPMI_SLAVE_RB to 0 to tolerate glitches during power down
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#222 - 	
      { 0x00,	0x99,	0x08,	0x00},	// W#223 - 	1BYTE WRITE 0x00 TO PON__PON_RESET_N_CTL; Assert PON_RESET_N low
      { 0x00,	0x49,	0x09,	0x10},	// W#224 - 	1BYTE WRITE 0x00 TO MISC__PON_OUT_CTL; Send PON_OUT low to all PMIC's.  Changing to SEC ACCESS (to updated in next .csv).
      { 0x01,	0x43,	0x08,	0x02},	// W#225 - 	1BYTE WRITE 0x01 TO SID:2 0x0800__0x43; Set SPMI_SLAVE_RB back to 1
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#226 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#227 - 	
      { 0xF7,	0x50,	0x72,	0x40},	// W#228 - 	1BYTE READ PBS_CLIENT1__SCRATCH1 TO R7; Readback PBS_CLIENT1_SCRATCH1 and store to Buffer R7
      { 0x05,	0x70,	0xFF,	0x91},	// W#229 - 	SKIP 1 Lines if R7 NOT EQUAL 0x04 (MASK=0xFF)
      { 0x02,	0x51,	0x72,	0x00},	// W#230 - 	sleep clock cycles (used by AOP for wait).  Changed from 6 to 2 cycles on PBS vD (shorter sequences MSM v2).
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#231 - 	
      { 0x04,	0x70,	0xFF,	0x90},	// W#232 - 	SKIP 1 Lines if R7 NOT EQUAL 0x04 (MASK=0xFF)
      { 0xC4,	0x07,	0xFF,	0x83},	// W#233 - 	
      { 0x03,	0x70,	0xFF,	0x90},	// W#234 - 	SKIP 1 Lines if R7 NOT EQUAL 0x03 (MASK=0xFF)
      { 0xD8,	0x07,	0xFF,	0x83},	// W#235 - 	
      { 0x02,	0x70,	0xFF,	0x90},	// W#236 - 	SKIP 1 Lines if R7 NOT EQUAL 0x02 (MASK=0xFF)
      { 0xF0,	0x07,	0xFF,	0x83},	// W#237 - 	
      { 0x01,	0x70,	0xFF,	0x90},	// W#238 - 	SKIP 1 Lines if R7 NOT EQUAL 0x01 (MASK=0xFF)
      { 0x50,	0x08,	0xFF,	0x83},	// W#239 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#240 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#241 - 	
      { 0x1C,	0x04,	0x38,	0x00},	// W#242 - 	Re-enable ADC comparators
      { 0x80,	0x46,	0x0B,	0x00},	// W#243 - 	Enable VREF_MSM
      { 0x04,	0x51,	0x72,	0x00},	// W#244 - 	Set SCRATCH2 to 0x04 (to indicate to SW that SOC wake sequence just ran)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#245 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#246 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#247 - 	
      { 0x48,	0x0B,	0xFF,	0x9B},	// W#248 - 	
      { 0x00,	0x40,	0x38,	0x00},	// W#249 - 	Disable ADC comparators (saves 10uA on 9655)
      { 0x03,	0x51,	0x72,	0x00},	// W#250 - 	Set SCRATCH2 to 0x03 (to indicate to SW that SOC sleep sequence just ran)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#251 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#252 - 	
      { 0xF7,	0x11,	0x1C,	0x40},	// W#253 - 	Readback 'secret' spare register and store to Buffer R7
      { 0x08,	0x70,	0xFF,	0x91},	// W#254 - 	0x08 is the activation code
      { 0x00,	0x44,	0xC1,	0x00},	// W#255 - 	Set to Dig Output low
      { 0x00,	0x40,	0xC1,	0x00},	// W#256 - 	Set to Dig Inpput
      { 0x18,	0x04,	0x76,	0xA1},	// W#257 - 	; 2BYTE R6=MEMADDR(0x418).  Load Buffer R6 with GOTO pointer to SLEEP_DATASET
      { 0x98,	0x0A,	0x26,	0x89},	// W#258 - 	
      { 0x98,	0x0A,	0x43,	0x89},	// W#259 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#260 - 	
      { 0x98,	0x0A,	0x5A,	0x89},	// W#261 - 	
      { 0xC5,	0x44,	0x1D,	0x01},	// W#262 - 	Re-enable PFM_IBOOST
      { 0x90,	0xA6,	0x17,	0x11},	// W#263 - 	Re-enable S2 PFM_EXIT_DIP_EN
      { 0x32,	0xA9,	0x17,	0x11},	// W#264 - 	Re-enable S2 PFM_EXIT_CL_EN
      { 0x0B,	0x71,	0x17,	0x11},	// W#265 - 	Enable S2 VDIP comparator; 20mV threshold
      { 0x90,	0xA6,	0x23,	0x11},	// W#266 - 	Re-enable S6 PFM_EXIT_DIP_EN
      { 0x32,	0xA9,	0x23,	0x11},	// W#267 - 	Re-enable S6 PFM_EXIT_CL_EN
      { 0x0A,	0x71,	0x23,	0x11},	// W#268 - 	Enable S6 VDIP comparator; 15mV threshold (new on RAM v7 / SBL v0x0A)
      { 0x90,	0xA6,	0x2C,	0x11},	// W#269 - 	Re-enable S9 PFM_EXIT_DIP_EN
      { 0x32,	0xA9,	0x2C,	0x11},	// W#270 - 	Re-enable S9 PFM_EXIT_CL_EN
      { 0x09,	0x71,	0x2C,	0x11},	// W#271 - 	Enable S9 VDIP comparator; 10mV threshold
      { 0x3A,	0x7E,	0x20,	0x01},	// W#272 - 	
      { 0x38,	0x7E,	0x1A,	0x01},	// W#273 - 	
      { 0x02,	0x51,	0x72,	0x00},	// W#274 - 	Set SCRATCH2 to 0x02 (to indicate to SW that wake sequence just ran)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#275 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#276 - 	
      { 0xF7,	0x45,	0x1A,	0x41},	// W#277 - 	Readback S3 Mode and store to Buffer R7
      { 0x04,	0x70,	0xFF,	0x90},	// W#278 - 	Skip if Mode != RM
      { 0x30,	0x7E,	0x1A,	0x01},	// W#279 - 	
      { 0xF7,	0x45,	0x20,	0x41},	// W#280 - 	Readback S5 Mode and store to Buffer R7
      { 0x04,	0x70,	0xFF,	0x90},	// W#281 - 	Skip if Mode != RM
      { 0x32,	0x7E,	0x20,	0x01},	// W#282 - 	
      { 0x18,	0x04,	0x76,	0xA1},	// W#283 - 	; 2BYTE R6=MEMADDR(0x418).  Load Buffer R6 with GOTO pointer to SLEEP_DATASET
      { 0x7C,	0x0A,	0x26,	0x89},	// W#284 - 	
      { 0x7C,	0x0A,	0x43,	0x89},	// W#285 - 	
      { 0x7C,	0x0A,	0x5A,	0x89},	// W#286 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#287 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#288 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#289 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#290 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#291 - 	
      { 0x04,	0x45,	0x26,	0x01},	// W#292 - 	Set to Retention Mode
      { 0x05,	0x45,	0x43,	0x01},	// W#293 - 	Set to LPM Mode
      { 0xF7,	0x45,	0x5A,	0x41},	// W#294 - 	Readback L27 Mode and store to Buffer R7
      { 0x05,	0x70,	0xFF,	0x90},	// W#295 - 	Skip if Mode != LPM
      { 0x04,	0x45,	0x5A,	0x01},	// W#296 - 	Set to Retention Mode
      { 0xF7,	0x45,	0x2C,	0x41},	// W#297 - 	Readback S9 Mode and store to Buffer R7
      { 0x05,	0x75,	0xFF,	0x92},	// W#298 - 	Skip if Mode != LPM or RM
      { 0x01,	0x71,	0x2C,	0x11},	// W#299 - 	Disable VDIP comparator; keep 15mV threshold
      { 0x50,	0xA6,	0x2C,	0x11},	// W#300 - 	Disable PFM_EXIT_DIP_EN to prevent using HCPFM CL
      { 0x12,	0xA9,	0x2C,	0x11},	// W#301 - 	Disable PFM_EXIT_CL_EN to prevent using HCPFM CL
      { 0xF7,	0x45,	0x23,	0x41},	// W#302 - 	Readback S6 Mode and store to Buffer R7
      { 0x05,	0x75,	0xFF,	0x92},	// W#303 - 	Skip if Mode != LPM or RM
      { 0x01,	0x71,	0x23,	0x11},	// W#304 - 	Disable VDIP comparator; keep 15mV threshold
      { 0x50,	0xA6,	0x23,	0x11},	// W#305 - 	Disable PFM_EXIT_DIP_EN to prevent using HCPFM CL
      { 0x12,	0xA9,	0x23,	0x11},	// W#306 - 	Disable PFM_EXIT_CL_EN to prevent using HCPFM CL
      { 0xF7,	0x45,	0x17,	0x41},	// W#307 - 	Readback S2 Mode and store to Buffer R7
      { 0x05,	0x75,	0xFF,	0x92},	// W#308 - 	Skip if Mode != LPM or RM
      { 0x01,	0x71,	0x17,	0x11},	// W#309 - 	Disable VDIP comparator; keep 15mV threshold
      { 0x50,	0xA6,	0x17,	0x11},	// W#310 - 	Disable PFM_EXIT_DIP_EN to prevent using HCPFM CL
      { 0x12,	0xA9,	0x17,	0x11},	// W#311 - 	Disable PFM_EXIT_CL_EN to prevent using HCPFM CL
      { 0xF7,	0x45,	0x1D,	0x41},	// W#312 - 	Readback S4 Mode and store to Buffer R7
      { 0x05,	0x75,	0xFF,	0x90},	// W#313 - 	Skip if Mode != LPM or RM
      { 0x05,	0x44,	0x1D,	0x01},	// W#314 - 	Disable PFM_IBOOST
      { 0x01,	0x51,	0x72,	0x00},	// W#315 - 	Set SCRATCH2 to 0x01 (to indicate to SW that sleep sequence just ran)
      { 0xF7,	0x11,	0x1C,	0x40},	// W#316 - 	Readback 'secret' spare register and store to Buffer R7
      { 0x08,	0x70,	0xFF,	0x91},	// W#317 - 	0x08 is the activation code
      { 0x01,	0x40,	0xC1,	0x00},	// W#318 - 	Set to Dig Output
      { 0x80,	0x44,	0xC1,	0x00},	// W#319 - 	Set to Dig Output high
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#320 - 	End of Sequence; Byte-2
      { 0xF8,	0x08,	0xD6,	0x40},	// W#321 - 	1BYTE READ GPIO23__STATUS1 TO R8; Readback GPIO23 (WCSS_VCTRL) status and store to Buffer R8
      { 0x01,	0x81,	0x01,	0x90},	// W#322 - 	SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO23 is high
      { 0x28,	0x09,	0xFF,	0x83},	// W#323 - 	GOTO WCSS_VCTRL_DISABLE; Else (i.e. GPI023 is low)
      { 0xC0,	0x4F,	0x26,	0x01},	// W#324 - 	Assert PBS_EN and PBS_MODE (Secondary Mode is set in SBL)
      { 0xF9,	0x4C,	0x09,	0x40},	// W#325 - 	1BYTE READ MISC__VSET1_LB TO R9; Read MISC register for L5 active voltage and store LSB to Buffer R9
      { 0xFA,	0x4D,	0x09,	0x40},	// W#326 - 	1BYTE READ MISC__VSET1_UB TO RA; Read MISC register for L5 active voltage and store MSB to Buffer RA
      { 0xF9,	0x40,	0x44,	0x09},	// W#327 - 	1BYTE WRITE R9 TO LDO05__VSET_LB; Write Buffer R9 value to L5A VSET (LSB) (note: PDM error incorrectly shows 2BYTE)
      { 0xFA,	0x41,	0x44,	0x09},	// W#328 - 	1BYTE WRITE RA TO LDO05__VSET_UB; Write Buffer RA value to L5A VSET (MSB) (note: PDM error incorrectly shows 2BYTE)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#329 - 	End of Sequence; Byte-2
      { 0xF9,	0x4E,	0x09,	0x40},	// W#330 - 	1BYTE READ MISC__VSET2_LB TO R9; Read MISC register for L5 retention voltage and store LSB to Buffer R9
      { 0xFA,	0x4F,	0x09,	0x40},	// W#331 - 	1BYTE READ MISC__VSET2_UB TO RA; Read MISC register for L5 retention voltage and store MSB to Buffer RA
      { 0xF9,	0x40,	0x44,	0x09},	// W#332 - 	1BYTE WRITE R9 TO LDO05__VSET_LB; Write Buffer R9 value to L5A VSET (LSB) (note: PDM error incorrectly shows 2BYTE)
      { 0xFA,	0x41,	0x44,	0x09},	// W#333 - 	1BYTE WRITE RA TO LDO05__VSET_UB; Write Buffer RA value to L5A VSET (MSB) (note: PDM error incorrectly shows 2BYTE)
      { 0xF8,	0x08,	0xD3,	0x40},	// W#334 - 	1BYTE READ GPIO020__STATUS1 TO R8; Readback GPIO20 (EUD) status and store to Buffer R8
      { 0x01,	0x81,	0x01,	0x90},	// W#335 - 	SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01)
      { 0x00,	0x4F,	0x26,	0x01},	// W#336 - 	1BYTE WRITE 0x00 TO S7_CTRL__PBS_VOTE_CTL; De-assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#337 - 	End of Sequence; Byte-2
      { 0xF8,	0x08,	0xD3,	0x40},	// W#338 - 	Readback GPIO20 (EUD) status and store to Buffer R8
      { 0x01,	0x81,	0x01,	0x90},	// W#339 - 	SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO20 is high
      { 0x78,	0x09,	0xFF,	0x83},	// W#340 - 	Else (i.e. GPI020 is low)
      { 0xC0,	0x4F,	0x26,	0x01},	// W#341 - 	Assert PBS_EN and PBS_MODE (Secondary Mode is set in SBL)
      { 0x40,	0x4F,	0x20,	0x01},	// W#342 - 	Assert PBS_MODE (Secondary Mode is set in SBL)
      { 0xF8,	0x51,	0x71,	0x40},	// W#343 - 	1Readback PBS_CLIENT0.SCRATCH2 (Option 2 config)
      { 0x00,	0x80,	0xFF,	0x90},	// W#344 - 	SKIP if OPTION2 != GND (i.e. not SS option)
      { 0x88,	0x47,	0x44,	0x01},	// W#345 - 	Assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE (Secondary Mode is set in SBL)
      { 0x88,	0x47,	0x40,	0x01},	// W#346 - 	Assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE (Secondary Mode is set in SBL)
      { 0x88,	0x47,	0x4B,	0x01},	// W#347 - 	Assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE (Secondary Mode is set in SBL)
      { 0x88,	0x47,	0x57,	0x01},	// W#348 - 	Assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE (Secondary Mode is set in SBL)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#349 - 	End of Sequence; Byte-2
      { 0x00,	0x47,	0x57,	0x01},	// W#350 - 	De-assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE
      { 0x80,	0x47,	0x4B,	0x01},	// W#351 - 	De-assert EN_FOLLOW_PMIC_AWAKE, but not MODE_FOLLOW_PMIC_AWAKE (preserve SBL setting)
      { 0x00,	0x47,	0x40,	0x01},	// W#352 - 	De-assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE
      { 0xF8,	0x51,	0x71,	0x40},	// W#353 - 	1Readback PBS_CLIENT0.SCRATCH2 (Option 2 config)
      { 0x00,	0x80,	0xFF,	0x90},	// W#354 - 	SKIP if OPTION2 != GND (i.e. not SS option)
      { 0x00,	0x47,	0x44,	0x01},	// W#355 - 	De-assert EN_FOLLOW_PMIC_AWAKE and MODE_FOLLOW_PMIC_AWAKE
      { 0xF8,	0x08,	0xC2,	0x40},	// W#356 - 	Readback GPIO03 (WLAN_RF_VCTRL) status and store to Buffer R8
      { 0x01,	0x81,	0x01,	0x90},	// W#357 - 	SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO03 is high (i.e. WLAN enabled)
      { 0x00,	0x4F,	0x20,	0x01},	// W#358 - 	De-assert PBS_MODE
      { 0xF8,	0x08,	0xD6,	0x40},	// W#359 - 	1BYTE READ GPIO23__STATUS1 TO R8; Readback GPIO23 (WCSS_VCTRL) status and store to Buffer R8
      { 0x01,	0x81,	0x01,	0x90},	// W#360 - 	SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO23 is high (i.e. WLAN enabled)
      { 0x00,	0x4F,	0x26,	0x01},	// W#361 - 	1BYTE WRITE 0x00 TO S7_CTRL__PBS_VOTE_CTL; De-assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#362 - 	End of Sequence; Byte-2
      { 0xF8,	0x08,	0xC2,	0x40},	// W#363 - 	1BYTE READ GPIO03__STATUS1 TO R8; Readback GPIO3 (WLAN_RF_VCTRL) status and store to Buffer R8
      { 0x01,	0x81,	0x01,	0x90},	// W#364 - 	SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO3 is high
      { 0xC4,	0x09,	0xFF,	0x83},	// W#365 - 	GOTO WLAN_RF_VCTRL_DISABLE; lse (i.e. GPI03 is low)
      { 0x40,	0x4F,	0x1A,	0x01},	// W#366 - 	1BYTE WRITE 0x40 TO S3_CTRL__PBS_VOTE_CTL; Assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)
      { 0x40,	0x4F,	0x20,	0x01},	// W#367 - 	1BYTE WRITE 0x40 TO S5_CTRL__PBS_VOTE_CTL; Assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#368 - 	End of Sequence; Byte-2
      { 0xF8,	0x08,	0xD3,	0x40},	// W#369 - 	Readback GPIO20 (EUD) status and store to Buffer R8
      { 0x01,	0x81,	0x01,	0x90},	// W#370 - 	SKIP 1 Lines if R8 EQUAL 0x01 (MASK=0x01); If GPIO20 is high (i.e. EUD enabled)
      { 0x00,	0x4F,	0x20,	0x01},	// W#371 - 	De-assert PBS_MODE
      { 0x00,	0x4F,	0x1A,	0x01},	// W#372 - 	1BYTE WRITE 0x00 TO S3_CTRL__PBS_VOTE_CTL; De-assert PBS_MODE to vote for Secondary Mode (set to AUTO in SBL)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#373 - 	End of Sequence; Byte-2
      { 0xEC,	0x0B,	0x70,	0x29},	// W#374 - 	2BYTE R0 = 0xBEC  MEMADDR(END_PON_REASONS_PREVIOUS); Load R0 with the copy address
      { 0xF4,	0x0B,	0x72,	0x29},	// W#375 - 	2BYTE R2 = 0xBF4  MEMADDR(END_PON_REASONS_PREVIOUS2); Load R2 with the paste address
      { 0x04,	0x00,	0x78,	0x28},	// W#376 - 	1BYTE R8 = 0x04; Loop 4 times (shift 4 DWORDS)
      { 0xF0,	0xFF,	0xF4,	0xA3},	// W#377 - 	4BYTE R4 = MEMADDR(R0); copy the last reason registers into a buffer
      { 0xF2,	0xFF,	0xF4,	0xA7},	// W#378 - 	4BYTE MEMADDR(R2) = R4; write the reasons to the location pointed to by the paste pointer
      { 0x01,	0x00,	0x78,	0x24},	// W#379 - 	
      { 0x00,	0x81,	0xFF,	0x92},	// W#380 - 	SKIP 3 Lines if R8 EQUAL 0x00 (MASK=0xFF); Loop Until loop count (R8) = 4
      { 0x04,	0x00,	0x70,	0x25},	// W#381 - 	2BYTE R0 = R0 - 0x04; Move read pointer to next DWORD
      { 0x04,	0x00,	0x72,	0x25},	// W#382 - 	2BYTE R2 = R2 - 0x04; Move write pointer to next DWORD
      { 0xE4,	0x09,	0xFF,	0x83},	// W#383 - 	
      { 0xF4,	0xC0,	0x08,	0x40},	// W#384 - 	
      { 0xF5,	0xC2,	0x08,	0x40},	// W#385 - 	
      { 0xF6,	0xC4,	0x08,	0x40},	// W#386 - 	
      { 0xF7,	0xC5,	0x08,	0x40},	// W#387 - 	
      { 0xF0,	0xFF,	0xF4,	0xA7},	// W#388 - 	4BYTE MEMADDR(R0) = R4; Write data to R0 (the last register that we copied data out of)
      { 0x04,	0x00,	0x70,	0x21},	// W#389 - 	2BYTE R0 = R0 + 0x04; Increment pointer to next DWord
      { 0xF4,	0xC7,	0x08,	0x40},	// W#390 - 	
      { 0xF5,	0xC8,	0x08,	0x40},	// W#391 - 	
      { 0xF6,	0xC9,	0x08,	0x40},	// W#392 - 	
      { 0xF7,	0x48,	0x60,	0x40},	// W#393 - 	1BYTE READ RTC_RW__RDATA0 TO R7; Fill the last byte with the RTC value.
      { 0xF8,	0x48,	0x60,	0x40},	// W#394 - 	1BYTE READ RTC_RW__RDATA0 TO R8; Fill the last byte with the RTC value.
      { 0xF8,	0xF7,	0x79,	0x26},	// W#395 - 	1BYTE R9 = R8 - R7; Ensure we read the same RTC value twice consecutively to ensure a valid reading on the free running clock.
      { 0x00,	0x91,	0xFF,	0x90},	// W#396 - 	SKIP 1 Lines if R9 EQUAL 0x00 (MASK=0xFF); If data didn't match we retry.
      { 0x24,	0x0A,	0xFF,	0x83},	// W#397 - 	
      { 0xF0,	0xFF,	0xF4,	0xA7},	// W#398 - 	4BYTE MEMADDR(R0) = R4; Write data to address stored in R0
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#399 - 	End of subroutine. PC = Stored PC.; Byte-2
      { 0x88,	0x48,	0x14,	0x01},	// W#400 - 	1BYTE WRITE 0x88 TO S1_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x17,	0x01},	// W#401 - 	1BYTE WRITE 0x88 TO S2_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x1A,	0x01},	// W#402 - 	1BYTE WRITE 0x88 TO S3_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x1D,	0x01},	// W#403 - 	1BYTE WRITE 0x88 TO S4_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x20,	0x01},	// W#404 - 	1BYTE WRITE 0x88 TO S5_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x23,	0x01},	// W#405 - 	1BYTE WRITE 0x88 TO S6_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x26,	0x01},	// W#406 - 	1BYTE WRITE 0x88 TO S7_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x29,	0x01},	// W#407 - 	1BYTE WRITE 0x88 TO S8_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x2C,	0x01},	// W#408 - 	1BYTE WRITE 0x88 TO S9_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x2F,	0x01},	// W#409 - 	1BYTE WRITE 0x88 TO S10_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x32,	0x01},	// W#410 - 	1BYTE WRITE 0x88 TO S11_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x35,	0x01},	// W#411 - 	1BYTE WRITE 0x88 TO S12_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x38,	0x01},	// W#412 - 	1BYTE WRITE 0x88 TO S13_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x80,	0x48,	0x58,	0x01},	// W#413 - 	Enable strong pulldown
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#414 - 	
      { 0xF0,	0x40,	0xFF,	0x44},	// W#415 - 	Store VSET_LB into R0
      { 0xF1,	0x41,	0xFF,	0x44},	// W#416 - 	Store VSET_UB into R1
      { 0xF2,	0x45,	0xFF,	0x44},	// W#417 - 	Store MODE_CTL1 into R2
      { 0xF3,	0x08,	0xFF,	0x44},	// W#418 - 	Store STATUS into R3 (placeholder only)
      { 0xF6,	0xFF,	0xF0,	0xA7},	// W#419 - 	Load data into location pointed to by R4
      { 0x04,	0x00,	0x76,	0x21},	// W#420 - 	Increment pointer by 4 to point at the next set
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#421 - 	
      { 0xF6,	0xFF,	0xF0,	0xA3},	// W#422 - 	Load R0 to R3 with data from location pointed to by R4
      { 0xF0,	0x40,	0xFF,	0x0C},	// W#423 - 	ReStore VSET_LB from R0
      { 0xF1,	0x41,	0xFF,	0x0C},	// W#424 - 	ReStore VSET_UB from R1
      { 0xF2,	0x45,	0xFF,	0x0C},	// W#425 - 	ReStore MODE_CTL1 from R2
      { 0x04,	0x00,	0x76,	0x21},	// W#426 - 	Increment pointer by 4 to point at the next set
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#427 - 	
      { 0xF1,	0x08,	0x38,	0x41},	// W#428 - 	
      { 0x20,	0x10,	0x20,	0x97},	// W#429 - 	Check if bit 5 (VREG_OCP) is asserted.  Exit if not.
      { 0x00,	0x46,	0x38,	0x01},	// W#430 - 	Disable S13
      { 0x00,	0x63,	0x08,	0x00},	// W#431 - 	Disable
      { 0x01,	0x00,	0xFF,	0xC1},	// W#432 - 	Wait 3 SLEEP_CLK (1+2) cycles for reset changes to propagate through clock domains
      { 0x04,	0x62,	0x08,	0x00},	// W#433 - 	Configure to Normal Shutdown
      { 0x01,	0x00,	0xFF,	0xC1},	// W#434 - 	Wait 3 SLEEP_CLK (1+2) cycles for reset changes to propagate through clock domains
      { 0x80,	0x63,	0x08,	0x00},	// W#435 - 	Enable
      { 0x03,	0x00,	0xFF,	0xC1},	// W#436 - 	Wait 5 SLEEP_CLK (3+2) cycles for reset changes to propagate through clock domains
      { 0xA5,	0x64,	0x08,	0x00},	// W#437 - 	Initiate reset
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#438 - 	
      { 0xFF,	0x00,	0xFE,	0xC1},	// W#439 - 	Wait 13.3us
      { 0xFF,	0x00,	0xFE,	0xC1},	// W#440 - 	Wait 13.3us
      { 0xFF,	0x00,	0xFE,	0xC1},	// W#441 - 	Wait 13.3us
      { 0xF0,	0x08,	0xFF,	0x44},	// W#442 - 	Read STATUS1 and store to Buffer R0
      { 0x80,	0x01,	0x80,	0x90},	// W#443 - 	Skip the next line if VREG_READY = 1 (STATUS1 bit 7)
      { 0xE4,	0x0A,	0xFF,	0x83},	// W#444 - 	
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#445 - 	
      { 0x07,	0x45,	0xFF,	0x04},	// W#446 - 	Set NPM mode
      { 0xF0,	0x42,	0xFF,	0x45},	// W#447 - 	Read 0x42+0x43 (2 Byte VSET_VALID) to R0+R1  (programmed value).  Use VSET_VALID to cover when the buck is disabled.
      { 0xF6,	0xFF,	0x74,	0xA3},	// W#448 - 	Load R4+R5 with data from location pointed to by R6 (detection value)
      { 0x00,	0xC5,	0xFF,	0x96},	// W#449 - 	
      { 0xF1,	0xF5,	0x7B,	0x26},	// W#450 - 	programmed value - detection value
      { 0x00,	0xB6,	0xFF,	0x94},	// W#451 - 	If Carry was detected, MSB is less, jump to RUN
      { 0x00,	0xB1,	0xFF,	0x90},	// W#452 - 	MSB is the same, jump to CONTINUE
      { 0x3C,	0x0B,	0xFF,	0x83},	// W#453 - 	
      { 0xF0,	0xF4,	0x7A,	0x26},	// W#454 - 	programmed value - detection value
      { 0x00,	0xA6,	0xFF,	0x90},	// W#455 - 	If Carry was detected, LSB is less, jump to RUN
      { 0x3C,	0x0B,	0xFF,	0x83},	// W#456 - 	
      { 0x02,	0x00,	0x76,	0x21},	// W#457 - 	2BYTE R6 = R6 + 0x02; Increment pointer by 2 to point to Bits 3/2
      { 0xF6,	0xFF,	0x74,	0xA3},	// W#458 - 	Load R4+R5 with data from location pointed to by R6
      { 0xF4,	0x40,	0xFF,	0x0D},	// W#459 - 	Write R4+R5 TO 0x40+0x41 (VSET)
      { 0x02,	0x00,	0x76,	0x21},	// W#460 - 	2BYTE R6 = R6 + 0x02; Increment pointer by 2 to point at the next set
      { 0x80,	0x46,	0xFF,	0x04},	// W#461 - 	Enable
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#462 - 	
      { 0x04,	0x00,	0x76,	0x21},	// W#463 - 	2BYTE R6 = R6 + 0x04; Increment pointer by 4 to point at the next set
      { 0x80,	0x46,	0xFF,	0x04},	// W#464 - 	Enable
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#465 - 	
      { 0x00,	0x00,	0x70,	0x28},	// W#466 - 	Load R0 with 0x00 (init)
      { 0xF1,	0x43,	0x48,	0x41},	// W#467 - 	
      { 0x07,	0x15,	0xFF,	0x90},	// W#468 - 	Check L9:  ON @ 3V = 0x0B.  ON @ 1.8V = 0x07.  OFF (any voltage).  check MSB bit only.
      { 0x01,	0x00,	0x70,	0x20},	// W#469 - 	Increment R0 by 1, if L9 is enabled + 1.8V
      { 0xF1,	0x43,	0x49,	0x41},	// W#470 - 	
      { 0x07,	0x15,	0xFF,	0x90},	// W#471 - 	Check L10:  ON @ 3V = 0x0B.  ON @ 1.8V = 0x07.  OFF (any voltage).  check MSB bit only.
      { 0x01,	0x00,	0x70,	0x20},	// W#472 - 	Increment R0 by 1, if L9 is enabled + 1.8V
      { 0x02,	0x00,	0xFF,	0x90},	// W#473 - 	Check R0.  Will be 2 only if both LDO's are 1.8V or off.
      { 0x00,	0x46,	0x0B,	0x00},	// W#474 - 	
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#475 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#476 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#477 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#478 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#479 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#480 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#481 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#482 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#483 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#484 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#485 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#486 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#487 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#488 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#489 - 	S6 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#490 - 	L4 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#491 - 	S1 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: Min SVS (MSB), 0: Min SVS (LSB) (special case)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#492 - 	S9 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#493 - 	L27 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#494 - 	S13 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#495 - 	S12 - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#496 - 	Reserved
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#497 - 	S7 MODE/VSET
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#498 - 	L4 MODE/VSET
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#499 - 	L27 MODE/VSET
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#500 - 	spare
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#501 - 	spare
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#502 - 	spare
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#503 - 	spare
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#504 - 	3:POFF_REASON, 2:ON_REASON, 1:WARM_RESET_REASON1, 0:PON_REASON1
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#505 - 	3:RTC_LSB, 2:FAULT_REASON2, 1:FAULT_REASON1, 0:OFF_REASON
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#506 - 	3:POFF_REASON, 2:ON_REASON, 1:WARM_RESET_REASON1, 0:PON_REASON1
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#507 - 	3:RTC_LSB, 2:FAULT_REASON2, 1:FAULT_REASON1, 0:OFF_REASON
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#508 - 	3:POFF_REASON, 2:ON_REASON, 1:WARM_RESET_REASON1, 0:PON_REASON1
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#509 - 	3:RTC_LSB, 2:FAULT_REASON2, 1:FAULT_REASON1, 0:OFF_REASON
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#510 - 	
      { 0x01,	0x0D,	0xFF,	0xF8},	// W#511 - 	Inserted by compiler.
   },
   // "PM8005_RAM_b0x01_v0x04"
   {
      //data  offset  base_addr  sid
      { 0x52,	0x53,	0x42,	0x50},	// W#0 - 	; ASCII for PBSR".  Later changed to Check Sum."
      { 0x01,	0x04,	0x01,	0x10},	// W#1 - 	; 10=Dataset Start, 01=Header Ver, 0401 = Rev 4, Branch 1
      { 0x14,	0x02,	0xFF,	0x83},	// W#2 - 	
      { 0x50,	0x02,	0xFF,	0x83},	// W#3 - 	
      { 0xF4,	0x03,	0xFF,	0x83},	// W#4 - 	Fixed location for WARM_RESET_DATASET.  Do NOT move this GOTO line.
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#5 - 	
      { 0xFF,	0x48,	0x71,	0x00},	// W#6 - 	1BYTE WRITE 0xFF TO PBS_CLIENT0__TRIG_LATCHED_CLR; Clear all latched sequences in PBS PON Client 0
      { 0xFB,	0x08,	0x08,	0x40},	// W#7 - 	1BYTE READ PON__FSM_STATUS TO RB; Readback PON FSM Status and store to Buffer RB
      { 0x04,	0xB0,	0xFF,	0x90},	// W#8 - 	SKIP 1 Lines if RB NOT EQUAL 0x04 (MASK=0xFF); DVDD_CONFIG = State 4
      { 0x08,	0x00,	0xFF,	0x83},	// W#9 - 	; GOTO OTP DVDD_CONFIG.  Using RAW command due to PDM issue with links.
      { 0x01,	0xB0,	0xFF,	0x90},	// W#10 - 	SKIP 1 Lines if RB NOT EQUAL 0x01 (MASK=0xFF); PON = State 1
      { 0x08,	0x00,	0xFF,	0x83},	// W#11 - 	; GOTO OTP DVDD_CONFIG.  Using RAW command due to PDM issue with links.
      { 0x06,	0xB0,	0xFF,	0x90},	// W#12 - 	SKIP 1 Lines if RB NOT EQUAL 0x06 (MASK=0xFF); FAULT = State 6
      { 0xAC,	0x02,	0xFF,	0x83},	// W#13 - 	
      { 0x02,	0xB0,	0xFF,	0x90},	// W#14 - 	SKIP 1 Lines if RB NOT EQUAL 0x02 (MASK=0xFF); POFF = State 2
      { 0xF4,	0x02,	0xFF,	0x83},	// W#15 - 	
      { 0x07,	0xB0,	0xFF,	0x90},	// W#16 - 	SKIP 1 Lines if RB NOT EQUAL 0x07 (MASK=0xFF); WARM RESET = State 7
      { 0xBC,	0x02,	0xFF,	0x83},	// W#17 - 	
      { 0x00,	0x91,	0x08,	0x00},	// W#18 - 	1BYTE WRITE 0x00 TO PON__PBS_INTERFACE; Undefined State - NACK PON (Note:  None of the Client GOTOs have a 'RETURN'; the ACK is done in each subroutine)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#19 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#20 - 	
      { 0xF3,	0x0A,	0x72,	0x40},	// W#21 - 	1BYTE READ PBS_CLIENT1__TRIG_LATCHED TO R3; Read Client 1 trigger latch status and store to Buffer R3
      { 0x01,	0x30,	0x01,	0x92},	// W#22 - 	SKIP 3 Lines if R3 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / Spare
      { 0x01,	0x48,	0x72,	0x00},	// W#23 - 	1BYTE WRITE 0x01 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#24 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#25 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x02,	0x30,	0x02,	0x92},	// W#26 - 	SKIP 3 Lines if R3 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = EOSR
      { 0x02,	0x48,	0x72,	0x00},	// W#27 - 	1BYTE WRITE 0x02 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#28 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#29 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x04,	0x30,	0x04,	0x92},	// W#30 - 	SKIP 3 Lines if R3 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = OCP (non-core VREG)
      { 0x04,	0x48,	0x72,	0x00},	// W#31 - 	1BYTE WRITE 0x04 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#32 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#33 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x08,	0x30,	0x08,	0x92},	// W#34 - 	SKIP 3 Lines if R3 NOT EQUAL 0x08 (MASK=0x08); Bit 3 - GPIO2 (spare trigger)
      { 0x08,	0x48,	0x72,	0x00},	// W#35 - 	1BYTE WRITE 0x08 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#36 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#37 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x80,	0x30,	0x80,	0x92},	// W#38 - 	SKIP 3 Lines if R3 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger (Sleep/Wake)
      { 0x80,	0x48,	0x72,	0x00},	// W#39 - 	1BYTE WRITE 0x80 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7
      { 0x34,	0x03,	0xFF,	0x83},	// W#40 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#41 - 	NOP; TBD
      { 0x2C,	0x03,	0xFF,	0x83},	// W#42 - 	GOTO RAM_MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT
      { 0x8C,	0x03,	0xFF,	0x9B},	// W#43 - 	
      { 0x46,	0x01,	0xFF,	0xC1},	// W#44 - 	WAIT 10010 us; Wait 10ms for VREGs to discharge completely before allowing the next PON trigger
      { 0x40,	0x91,	0x08,	0x00},	// W#45 - 	1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#46 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#47 - 	
      { 0x02,	0x98,	0x08,	0x10},	// W#48 - 	1BYTE WRITE 0x02 TO PON__RESET_CTL; Assert warm_rb
      { 0x01,	0x00,	0xFF,	0xC1},	// W#49 - 	WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains
      { 0x06,	0x98,	0x08,	0x10},	// W#50 - 	1BYTE WRITE 0x06 TO PON__RESET_CTL; De-assert warm_rb
      { 0x01,	0x00,	0xFF,	0xC1},	// W#51 - 	WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains
      { 0xF2,	0x45,	0x09,	0x40},	// W#52 - 	Read L6 MISC VSET3 LB (scratch for Pass 0/1/2) and store to R2
      { 0xF4,	0x03,	0x76,	0x29},	// W#53 - 	Load R6 with 0x3F4 (start of WARM RESET DATASET)
      { 0xA0,	0x03,	0x14,	0x89},	// W#54 - 	
      { 0xA0,	0x03,	0x17,	0x89},	// W#55 - 	
      { 0x01,	0x00,	0x72,	0x20},	// W#56 - 	Increment R2 by 1
      { 0xF2,	0x45,	0x09,	0x08},	// W#57 - 	
      { 0x01,	0x43,	0x08,	0x02},	// W#58 - 	
      { 0x40,	0x91,	0x08,	0x00},	// W#59 - 	1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#60 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#61 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#62 - 	
      { 0x04,	0x98,	0x08,	0x10},	// W#63 - 	1BYTE WRITE 0x04 TO PON__RESET_CTL; Assert shutdown1_rb
      { 0x01,	0x00,	0xFF,	0xC1},	// W#64 - 	WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains
      { 0x06,	0x98,	0x08,	0x10},	// W#65 - 	1BYTE WRITE 0x06 TO PON__RESET_CTL; De-assert shutdown1_rb
      { 0x01,	0x00,	0xFF,	0xC1},	// W#66 - 	WAIT 91 us; Wait 3 SLEEP_CLK cycles for reset changes to propagate through clock domains
      { 0x8C,	0x03,	0xFF,	0x9B},	// W#67 - 	
      { 0x0C,	0x04,	0x17,	0x89},	// W#68 - 	
      { 0x0C,	0x04,	0x1A,	0x89},	// W#69 - 	
      { 0x46,	0x01,	0xFF,	0xC1},	// W#70 - 	WAIT 10010 us; Wait 10ms for VREGs to discharge completely before allowing the next PON trigger
      { 0x0C,	0x04,	0x24,	0x88},	// W#71 - 	
      { 0x0C,	0x04,	0x2C,	0x88},	// W#72 - 	
      { 0x40,	0x91,	0x08,	0x00},	// W#73 - 	1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#74 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#75 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#76 - 	End of Sequence; Byte-2
      { 0xF7,	0x50,	0x72,	0x40},	// W#77 - 	1BYTE READ PBS_CLIENT1__SCRATCH1 TO R7; Readback PBS_CLIENT1_SCRATCH1 and store to Buffer R7
      { 0x04,	0x70,	0xFF,	0x90},	// W#78 - 	SKIP 1 Lines if R7 NOT EQUAL 0x04 (MASK=0xFF)
      { 0x5C,	0x03,	0xFF,	0x83},	// W#79 - 	
      { 0x03,	0x70,	0xFF,	0x90},	// W#80 - 	SKIP 1 Lines if R7 NOT EQUAL 0x03 (MASK=0xFF)
      { 0x68,	0x03,	0xFF,	0x83},	// W#81 - 	
      { 0x02,	0x70,	0xFF,	0x90},	// W#82 - 	SKIP 1 Lines if R7 NOT EQUAL 0x02 (MASK=0xFF)
      { 0x74,	0x03,	0xFF,	0x83},	// W#83 - 	
      { 0x01,	0x70,	0xFF,	0x90},	// W#84 - 	SKIP 1 Lines if R7 NOT EQUAL 0x01 (MASK=0xFF)
      { 0x80,	0x03,	0xFF,	0x83},	// W#85 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#86 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#87 - 	
      { 0x04,	0x51,	0x72,	0x00},	// W#88 - 	No sequence, just placeholder
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#89 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#90 - 	
      { 0x03,	0x51,	0x72,	0x00},	// W#91 - 	No sequence, just placeholder
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#92 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#93 - 	
      { 0x02,	0x51,	0x72,	0x00},	// W#94 - 	No sequence, just placeholder
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#95 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#96 - 	
      { 0x01,	0x51,	0x72,	0x00},	// W#97 - 	No sequence, just placeholder
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#98 - 	
      { 0x88,	0x48,	0x14,	0x01},	// W#99 - 	1BYTE WRITE 0x88 TO S1_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x17,	0x01},	// W#100 - 	1BYTE WRITE 0x88 TO S2_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x1A,	0x01},	// W#101 - 	1BYTE WRITE 0x88 TO S3_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0x88,	0x48,	0x1D,	0x01},	// W#102 - 	1BYTE WRITE 0x88 TO S4_CTRL__PD_CTL; Enable strong + leak pulldown
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#103 - 	
      { 0x07,	0x45,	0xFF,	0x04},	// W#104 - 	Set NPM mode
      { 0xF0,	0x42,	0xFF,	0x45},	// W#105 - 	Read 0x42+0x43 (2 Byte VSET_VALID) to R0+R1  (programmed value).  Use VSET_VALID to cover when the buck is disabled.
      { 0xF6,	0xFF,	0x74,	0xA3},	// W#106 - 	Load R4+R5 with data from location pointed to by R6 (detection value)
      { 0x00,	0x25,	0xFF,	0x96},	// W#107 - 	
      { 0xF1,	0xF5,	0x7B,	0x26},	// W#108 - 	programmed value - detection value
      { 0x00,	0xB6,	0xFF,	0x94},	// W#109 - 	If Carry was detected, MSB is less, jump to RUN
      { 0x00,	0xB1,	0xFF,	0x90},	// W#110 - 	MSB is the same, jump to CONTINUE
      { 0xE4,	0x03,	0xFF,	0x83},	// W#111 - 	
      { 0xF0,	0xF4,	0x7A,	0x26},	// W#112 - 	programmed value - detection value
      { 0x00,	0xA6,	0xFF,	0x90},	// W#113 - 	If Carry was detected, LSB is less, jump to RUN
      { 0xE4,	0x03,	0xFF,	0x83},	// W#114 - 	
      { 0x02,	0x00,	0x76,	0x21},	// W#115 - 	2BYTE R6 = R6 + 0x02; Increment pointer by 2 to point to Bits 3/2
      { 0xF6,	0xFF,	0x74,	0xA3},	// W#116 - 	Load R4+R5 with data from location pointed to by R6
      { 0xF4,	0x40,	0xFF,	0x0D},	// W#117 - 	Write R4+R5 TO 0x40+0x41 (VSET)
      { 0x02,	0x00,	0x76,	0x21},	// W#118 - 	2BYTE R6 = R6 + 0x02; Increment pointer by 2 to point at the next set
      { 0x80,	0x46,	0xFF,	0x04},	// W#119 - 	Enable
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#120 - 	
      { 0x04,	0x00,	0x76,	0x21},	// W#121 - 	2BYTE R6 = R6 + 0x04; Increment pointer by 4 to point at the next set
      { 0x80,	0x46,	0xFF,	0x04},	// W#122 - 	Enable
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#123 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#124 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#125 - 	S1c - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#126 - 	S2c - 3: SS Nom (MSB), 2: SS Nom (LSB), 1: SS Nom (MSB), 0: SS Nom (LSB)
      { 0x01,	0x04,	0xFF,	0xF8},	// W#127 - 	Inserted by compiler.
   },
   // "PMi8998_RAM_b0x01_v0x0A"
   {
      //data  offset  base_addr  sid
      { 0x52,	0x53,	0x42,	0x50},	// W#0 - 	; ASCII for PBSR".  Later changed to Check Sum."
      { 0x01,	0x09,	0x01,	0x18},	// W#1 - 	; 18=Dataset Start, 01=Header Ver, 0901 = Rev 9, Branch 1
      { 0x30,	0x08,	0xFF,	0x83},	// W#2 - 	
      { 0x74,	0x08,	0xFF,	0x83},	// W#3 - 	
      { 0xE4,	0x08,	0xFF,	0x83},	// W#4 - 	
      { 0x54,	0x09,	0xFF,	0x83},	// W#5 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#6 - 	; Reserved location for sleep dataset pointer. Sleep dataset does not exist on PMi.
      { 0xB0,	0x0F,	0xFF,	0x83},	// W#7 - 	GOTO BEGIN_PON_REASONS_DATASET; Fixed location for PON REASONS dataset.  Do NOT move this GOTO line.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#8 - 	; Reserved location for RTRR dataset pointer. Does not exist on PMi yet.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#9 - 	; Reserved location for HLOS dataset pointer. Does not exist yet.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#10 - 	; Reserved location for LPG dataset pointer. Not needed on PMi because it has LPG perph.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#11 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#12 - 	
      { 0xFF,	0x48,	0x71,	0x00},	// W#13 - 	1BYTE WRITE 0xFF TO PBS_CLIENT0__TRIG_LATCHED_CLR; Clear all latched sequences in PBS PON Client 0
      { 0xFB,	0x08,	0x08,	0x40},	// W#14 - 	1BYTE READ PON__FSM_STATUS TO RB; Readback PON FSM Status and store to Buffer RB
      { 0x04,	0xB0,	0xFF,	0x90},	// W#15 - 	SKIP 1 Lines if RB NOT EQUAL 0x04 (MASK=0xFF); DVDD_CONFIG = State 4
      { 0x10,	0x00,	0xFF,	0x83},	// W#16 - 	GOTO OTP_DVDD_CONFIG (OTP); Is there a scenario where this is ever used?
      { 0x01,	0xB0,	0xFF,	0x90},	// W#17 - 	SKIP 1 Lines if RB NOT EQUAL 0x01 (MASK=0xFF); PON = State 1
      { 0x10,	0x00,	0xFF,	0x83},	// W#18 - 	GOTO OTP_DVDD_CONFIG (OTP); Is there a scenario where this is ever used?
      { 0x06,	0xB0,	0xFF,	0x90},	// W#19 - 	SKIP 1 Lines if RB NOT EQUAL 0x06 (MASK=0xFF); FAULT = State 6
      { 0xCC,	0x09,	0xFF,	0x83},	// W#20 - 	
      { 0x02,	0xB0,	0xFF,	0x90},	// W#21 - 	SKIP 1 Lines if RB NOT EQUAL 0x02 (MASK=0xFF); POFF = State 2
      { 0x38,	0x0A,	0xFF,	0x83},	// W#22 - 	
      { 0x07,	0xB0,	0xFF,	0x90},	// W#23 - 	SKIP 1 Lines if RB NOT EQUAL 0x07 (MASK=0xFF); WARM RESET = State 7
      { 0xF4,	0x09,	0xFF,	0x83},	// W#24 - 	
      { 0x00,	0x91,	0x08,	0x00},	// W#25 - 	1BYTE WRITE 0x00 TO PON__PBS_INTERFACE; Undefined State - NACK PON (Note:  None of the Client GOTOs have a 'RETURN'; the ACK is done in each subroutine)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#26 - 	End of Sequence; Byte-2
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#27 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#28 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#29 - 	
      { 0xF3,	0x0A,	0x72,	0x40},	// W#30 - 	1BYTE READ PBS_CLIENT1__TRIG_LATCHED TO R3; Read Client 1 trigger latch status and store to Buffer R3
      { 0x01,	0x30,	0x01,	0x92},	// W#31 - 	SKIP 3 Lines if R3 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / BOB
      { 0x01,	0x48,	0x72,	0x00},	// W#32 - 	1BYTE WRITE 0x01 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#33 - 	NOP; TBS - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#34 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x02,	0x30,	0x02,	0x92},	// W#35 - 	SKIP 3 Lines if R3 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = Charger Test MUX
      { 0x02,	0x48,	0x72,	0x00},	// W#36 - 	1BYTE WRITE 0x02 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1
      { 0x6C,	0x0D,	0xFF,	0x83},	// W#37 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#38 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x04,	0x30,	0x04,	0x92},	// W#39 - 	SKIP 3 Lines if R3 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = LAB/IBB SCP
      { 0x04,	0x48,	0x72,	0x00},	// W#40 - 	1BYTE WRITE 0x04 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2
      { 0xC0,	0x0A,	0xFF,	0x83},	// W#41 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#42 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x08,	0x30,	0x08,	0x92},	// W#43 - 	SKIP 3 Lines if R3 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = EOSR_Timer
      { 0x08,	0x48,	0x72,	0x00},	// W#44 - 	1BYTE WRITE 0x08 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3
      { 0x8C,	0x0B,	0xFF,	0x83},	// W#45 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#46 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x10,	0x30,	0x10,	0x92},	// W#47 - 	SKIP 3 Lines if R3 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = LAB_ASYNC
      { 0x10,	0x48,	0x72,	0x00},	// W#48 - 	1BYTE WRITE 0x10 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4
      { 0x68,	0x0B,	0xFF,	0x83},	// W#49 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#50 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x80,	0x30,	0x80,	0x92},	// W#51 - 	SKIP 3 Lines if R3 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger (Sleep/Wake)
      { 0x80,	0x48,	0x72,	0x00},	// W#52 - 	1BYTE WRITE 0x80 TO PBS_CLIENT1__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7
      { 0xEC,	0x0A,	0xFF,	0x83},	// W#53 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#54 - 	End of Sequence; TBD
      { 0x90,	0x0A,	0xFF,	0x83},	// W#55 - 	GOTO MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#56 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#57 - 	
      { 0xF4,	0x0A,	0x73,	0x40},	// W#58 - 	1BYTE READ PBS_CLIENT2__TRIG_LATCHED TO R4; Read Client 2 trigger latch status and store to Buffer R4
      { 0x01,	0x40,	0x01,	0x92},	// W#59 - 	SKIP 3 Lines if R4 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / EOSR_TIMER
      { 0x01,	0x48,	0x73,	0x00},	// W#60 - 	1BYTE WRITE 0x01 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0
      { 0xA8,	0x0D,	0xFF,	0x83},	// W#61 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#62 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x02,	0x40,	0x02,	0x92},	// W#63 - 	SKIP 3 Lines if R4 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = BATFET OCP
      { 0x02,	0x48,	0x73,	0x00},	// W#64 - 	1BYTE WRITE 0x02 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#65 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#66 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x04,	0x40,	0x04,	0x92},	// W#67 - 	SKIP 3 Lines if R4 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = FG BCL
      { 0x04,	0x48,	0x73,	0x00},	// W#68 - 	1BYTE WRITE 0x04 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#69 - 	NOP; TBD
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#70 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x08,	0x40,	0x08,	0x92},	// W#71 - 	SKIP 3 Lines if R4 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = Fuel Gauge Test MUX
      { 0x08,	0x48,	0x73,	0x00},	// W#72 - 	1BYTE WRITE 0x08 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#73 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#74 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x10,	0x40,	0x10,	0x92},	// W#75 - 	SKIP 3 Lines if R4 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = Unused Trigger
      { 0x10,	0x48,	0x73,	0x00},	// W#76 - 	1BYTE WRITE 0x10 TO PBS_CLIENT2__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4
      { 0xA0,	0x0D,	0xFF,	0x83},	// W#77 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#78 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x80,	0x40,	0x80,	0x92},	// W#79 - 	SKIP 3 Lines if R4 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger
      { 0x80,	0x48,	0x73,	0x00},	// W#80 - 	
      { 0x0C,	0x0E,	0xFF,	0x83},	// W#81 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#82 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x90,	0x0A,	0xFF,	0x83},	// W#83 - 	GOTO MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#84 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#85 - 	
      { 0xF5,	0x0A,	0x74,	0x40},	// W#86 - 	1BYTE READ PBS_CLIENT3__TRIG_LATCHED TO R5; Read Client 3 trigger latch status and store to Buffer R5
      { 0x01,	0x50,	0x01,	0x92},	// W#87 - 	SKIP 3 Lines if R5 NOT EQUAL 0x01 (MASK=0x01); Bit 0 = DTEST MUX / PD_PHY_IRQ_MUX
      { 0x01,	0x48,	0x74,	0x00},	// W#88 - 	1BYTE WRITE 0x01 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 0
      { 0x08,	0x0C,	0xFF,	0x83},	// W#89 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#90 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x02,	0x50,	0x02,	0x92},	// W#91 - 	SKIP 3 Lines if R5 NOT EQUAL 0x02 (MASK=0x02); Bit 1 = SCHG Snarl Timer
      { 0x02,	0x48,	0x74,	0x00},	// W#92 - 	1BYTE WRITE 0x02 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 1
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#93 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#94 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x04,	0x50,	0x04,	0x92},	// W#95 - 	SKIP 3 Lines if R5 NOT EQUAL 0x04 (MASK=0x04); Bit 2 = SCHG QI/PMA ON
      { 0x04,	0x48,	0x74,	0x00},	// W#96 - 	1BYTE WRITE 0x04 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 2
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#97 - 	NOP; TBD - Save for GOTO
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#98 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x08,	0x50,	0x08,	0x92},	// W#99 - 	SKIP 3 Lines if R5 NOT EQUAL 0x08 (MASK=0x08); Bit 3 = EOSR_Timer
      { 0x08,	0x48,	0x74,	0x00},	// W#100 - 	1BYTE WRITE 0x08 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 3
      { 0x18,	0x0C,	0xFF,	0x83},	// W#101 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#102 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x10,	0x50,	0x10,	0x92},	// W#103 - 	SKIP 3 Lines if R5 NOT EQUAL 0x10 (MASK=0x10); Bit 4 = WLED_SWIRE
      { 0x10,	0x48,	0x74,	0x00},	// W#104 - 	1BYTE WRITE 0x10 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 4
      { 0x74,	0x0C,	0xFF,	0x83},	// W#105 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#106 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x80,	0x50,	0x80,	0x94},	// W#107 - 	SKIP 3 Lines if R5 NOT EQUAL 0x80 (MASK=0x80); Bit 7 = SW Trigger
      { 0x80,	0x48,	0x74,	0x00},	// W#108 - 	1BYTE WRITE 0x80 TO PBS_CLIENT3__TRIG_LATCHED_CLR; Clear Latched Trigger - Bit 7
      { 0xF7,	0x50,	0x74,	0x40},	// W#109 - 	Readback PBS_CLIENT3_SCRATCH1 and store to Buffer R7
      { 0x11,	0x70,	0xFF,	0x90},	// W#110 - 	SKIP 1 Lines if R7 NOT EQUAL 0x11 (MASK=0xFF)
      { 0x44,	0x0E,	0xFF,	0x83},	// W#111 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#112 - 	End of Sequence; EOS needed since this trigger is not defined yet.  Remove EOS once defined.
      { 0x90,	0x0A,	0xFF,	0x83},	// W#113 - 	GOTO MAINLINE_FAULT; Undefined State - Perform reset in MAINLINE_FAULT
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#114 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#115 - 	
      { 0xB0,	0x0B,	0xFF,	0x9B},	// W#116 - 	
      { 0x50,	0x0A,	0xFF,	0x83},	// W#117 - 	GOTO CLIENT0_POFF_FAULT_COMMON; Go to seq common between fault and POFF
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#118 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#119 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#120 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#121 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#122 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#123 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#124 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#125 - 	
      { 0xB0,	0x0B,	0xFF,	0x9B},	// W#126 - 	
      { 0x02,	0x00,	0x70,	0x28},	// W#127 - 	1BYTE R0 = 0x02; R0 = 0x2
      { 0xA0,	0x0A,	0x08,	0x88},	// W#128 - 	GOTO INIT_RESET PON; //FIX: GOTOSUB_NOID still not working.  Do reset stuff common to warm reset and POFF
      { 0x42,	0x90,	0x16,	0x00},	// W#129 - 	1BYTE WRITE 0x42 TO SCHG8998_MISC__STAT_CFG; Force STAT low ([7:6]=b'01) so that SMB goes to suspend
      { 0xC4,	0x80,	0x13,	0x00},	// W#130 - 	1BYTE WRITE 0xC4 TO SCHG8998_USB__USBIN_AICL_OPTIONS_CFG; Charger-WA20. Revert register to FTRIM value upon WR.
      { 0x00,	0x52,	0x11,	0x00},	// W#131 - 	1BYTE WRITE 0x00 TO SCHG8998_OTG__OTG_CURRENT_LIMIT_CFG; Charger-WA21: Revert register to FTRIM value upon WR
      { 0x00,	0xC2,	0x14,	0x10},	// W#132 - 	1BYTE WRITE 0x00 TO SCHG8998_DC__ENG_SSUPPLY_CFG3; Charger-WA21: Revert register to FTRIM value upon WR
      { 0x04,	0xDF,	0x14,	0x00},	// W#133 - 	1BYTE WRITE 0x04 TO SCHG8998_DC__CLOCK_CONTROL; CR176140: Set chicken bit high to avoid lockup at following PON.
      { 0x14,	0x00,	0xFF,	0x83},	// W#134 - 	GOTO OTP_PON (OTP); Jump to a fixed location in OTP.  We are going to re-run the PON sequence.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#135 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#136 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#137 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#138 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#139 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#140 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#141 - 	
      { 0xEA,	0x01,	0xFF,	0xC1},	// W#142 - 	Wait 15ms to delay BOB disable (i.e. allow child LDO's on PM845 to disable in sequence)
      { 0xB0,	0x0B,	0xFF,	0x9B},	// W#143 - 	
      { 0x04,	0x00,	0x70,	0x28},	// W#144 - 	1BYTE R0 = 0x04; R0=0x4. This value will be applied to the PON_RESET_CTL register in PON (bit 2 warm_rb
      { 0xA0,	0x0A,	0x08,	0x88},	// W#145 - 	GOTO INIT_RESET PON; //FIX: GOTOSUB_NOID still not working.  Do reset stuff common to warm reset and POFF
      { 0x0C,	0x10,	0xA0,	0x89},	// W#146 - 	
      { 0x00,	0x46,	0x05,	0x00},	// W#147 - 	1BYTE WRITE 0x00 TO INT__EN_CTL1; Disable INT
      { 0x42,	0x90,	0x16,	0x00},	// W#148 - 	1BYTE WRITE 0x42 TO SCHG8998_MISC__STAT_CFG; Force STAT low ([7:6]=b'01) so that SMB goes to suspend
      { 0x01,	0x45,	0x16,	0x00},	// W#149 - 	1BYTE WRITE 0x01 TO SCHG8998_MISC__SHDN_CMD; Enable Charger Shutdown. CR-0000171925
      { 0x00,	0x46,	0x40,	0x00},	// W#150 - 	1BYTE WRITE 0x00 TO FG_BATT_SOC__EN_CTL; Pause FG algorithm (CR-0000171925).  This bit emulatoes what the SHDN_N HW signal did on earlier PMIs.
      { 0x05,	0x52,	0x45,	0x00},	// W#151 - 	1BYTE WRITE 0x05 TO FG_ADC_RR__RR_ADC_CTL; Put RRADC in shutdown (this is like pausing the RRADC
      { 0x02,	0x66,	0x13,	0x00},	// W#152 - 	1BYTE WRITE 0x02 TO SCHG8998_USB__USBIN_ICL_OPTIONS; Revert USB5/1 setting to 500mA
      { 0x38,	0x63,	0x13,	0x00},	// W#153 - 	1BYTE WRITE 0x38 TO SCHG8998_USB__USBIN_OPTIONS_2_CFG; Revert Wipwr_RstN pin functionality to normal WiPwr operation (SBL changes it to EUD)
      { 0x24,	0x68,	0x13,	0x00},	// W#154 - 	1BYTE WRITE 0x24 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; Charger WA-31. Revert to UFP mode (same as FTRIM setting) to avoid unexpected PON.
      { 0x04,	0xDF,	0x14,	0x00},	// W#155 - 	1BYTE WRITE 0x04 TO SCHG8998_DC__CLOCK_CONTROL; CR176140: Set chicken bit high to avoid lockup at following PON.
      { 0x00,	0x6B,	0x41,	0x00},	// W#156 - 	1BYTE WRITE 0x00 TO FG_BATT_INFO__BATTERY_MISSING_CFG; Disable BMD
      { 0x46,	0x01,	0xFF,	0xC1},	// W#157 - 	WAIT 10010 us; Wait 10ms for VREGs to discharge completely before allowing the next PON trigger
      { 0x0C,	0x10,	0x2C,	0x88},	// W#158 - 	GOTO ROM_DISABLE_WAIT (ROM) MBG1; Disable MBG
      { 0x40,	0x91,	0x08,	0x00},	// W#159 - 	1BYTE WRITE 0x40 TO PON__PBS_INTERFACE; ACK PON
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#160 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#161 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#162 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#163 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#164 - 	
      { 0x45,	0x01,	0xFF,	0xC1},	// W#165 - 	WAIT 9980 us; Delay to let regulators discharge
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#166 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#167 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#168 - 	NOP; //This Subroutine does several things that are common to the Warm Reset and POFF seqeunces.  1) disable SPMI to avoid lockup
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#169 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#170 - 	NOP; Call RTRR subroutine
      { 0xF0,	0x98,	0x08,	0x18},	// W#171 - 	1BYTE WRITE R0 TO SID:0 0x0800__0x98; Assert whatever resets are defined in R0
      { 0x01,	0x00,	0xFF,	0xC1},	// W#172 - 	WAIT 91 us; Wait 90 uS for reset to propagate through clock domains
      { 0x06,	0x98,	0x08,	0x10},	// W#173 - 	1BYTE WRITE 0x06 TO SID:0 0x0800__0x98; Deassert Resets
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#174 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#175 - 	
      { 0x00,	0x46,	0xDC,	0x01},	// W#176 - 	1BYTE WRITE 0x00 TO IBB__ENABLE_CTL; Disable IBB
      { 0x00,	0x46,	0xDE,	0x01},	// W#177 - 	1BYTE WRITE 0x00 TO LAB__ENABLE_CTL; Disable LAB
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#178 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#179 - 	
      { 0xF0,	0x10,	0x12,	0x40},	// W#180 - 	1BYTE READ SCHG8998_BATIF__INT_RT_STS TO R0; Read status register
      { 0x00,	0x01,	0x40,	0x91},	// W#181 - 	SKIP 2 Lines if R0 EQUAL 0x00 (MASK=0x40); Skip next two lines
      { 0x1C,	0x00,	0x01,	0x88},	// W#182 - 	GOTO OTP_QI_PMA_CONFIG (OTP) REVID; Load Qi/PMA charger settings
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#183 - 	
      { 0x20,	0x00,	0x01,	0x88},	// W#184 - 	GOTO OTP_WIPWR_CONFIG (OTP) REVID; Load WiPwr Settings
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#185 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#186 - 	
      { 0xF7,	0x50,	0x72,	0x40},	// W#187 - 	1BYTE READ PBS_CLIENT1__SCRATCH1 TO R7; Readback PBS_CLIENT1_SCRATCH1 and store to Buffer R7
      { 0x04,	0x70,	0xFF,	0x90},	// W#188 - 	SKIP 1 Lines if R7 NOT EQUAL 0x04 (MASK=0xFF)
      { 0x14,	0x0B,	0xFF,	0x83},	// W#189 - 	
      { 0x03,	0x70,	0xFF,	0x90},	// W#190 - 	SKIP 1 Lines if R7 NOT EQUAL 0x03 (MASK=0xFF)
      { 0x3C,	0x0B,	0xFF,	0x83},	// W#191 - 	
      { 0x02,	0x70,	0xFF,	0x90},	// W#192 - 	SKIP 1 Lines if R7 NOT EQUAL 0x02 (MASK=0xFF)
      { 0x50,	0x0B,	0xFF,	0x83},	// W#193 - 	
      { 0x01,	0x70,	0xFF,	0x90},	// W#194 - 	SKIP 1 Lines if R7 NOT EQUAL 0x01 (MASK=0xFF)
      { 0x5C,	0x0B,	0xFF,	0x83},	// W#195 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#196 - 	
      { 0x00,	0xDF,	0x14,	0x00},	// W#197 - 	1BYTE WRITE 0x00 TO SCHG8998_DC__CLOCK_CONTROL; Set Use200k=0. Charger clock may lock up at this command CR-0000176241)
      { 0x04,	0xDF,	0x14,	0x00},	// W#198 - 	1BYTE WRITE 0x04 TO SCHG8998_DC__CLOCK_CONTROL; CR-0000176241: Toggle chicken bit to unlock charger
      { 0xC0,	0x03,	0xFE,	0xC1},	// W#199 - 	WAIT 50052 ns; wait 50us
      { 0x00,	0xDF,	0x14,	0x00},	// W#200 - 	1BYTE WRITE 0x00 TO SCHG8998_DC__CLOCK_CONTROL; CR-0000176241: Toggle chicken bit to unlock charger
      { 0xC0,	0x03,	0xFE,	0xC1},	// W#201 - 	wait 50us
      { 0x09,	0xDF,	0x14,	0x00},	// W#202 - 	Set bit0=1 (to request 19.2MHz clock), set bit3=1 (to set the flag)
      { 0x01,	0x4A,	0x59,	0x10},	// W#203 - 	1BYTE WRITE 0x01 TO CLK_DIST__PMIC_SLEEP_CTL; Set SLEEP_B to follow HW
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#204 - 	
      { 0x04,	0x51,	0x72,	0x00},	// W#205 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#206 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#207 - 	
      { 0x00,	0x4A,	0x59,	0x10},	// W#208 - 	1BYTE WRITE 0x00 TO CLK_DIST__PMIC_SLEEP_CTL; Force SLEEP_B low
      { 0x02,	0xDF,	0x14,	0x00},	// W#209 - 	1BYTE WRITE 0x02 TO SCHG8998_DC__CLOCK_CONTROL; Use 200k clock during sleep instead of gated 19.2MHz (~20% duty cycle)
      { 0x03,	0x51,	0x72,	0x00},	// W#210 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#211 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#212 - 	
      { 0x02,	0x51,	0x72,	0x00},	// W#213 - 	No sequence, just placeholder
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#214 - 	
      { 0xFF,	0xFF,	0xFF,	0xFB},	// W#215 - 	
      { 0x01,	0x51,	0x72,	0x00},	// W#216 - 	No sequence, just placeholder
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#217 - 	
      { 0xF0,	0x09,	0x09,	0x40},	// W#218 - 	1BYTE READ MISC__STATUS2 TO R0; Read DTEST status
      { 0x01,	0x01,	0x01,	0x90},	// W#219 - 	SKIP 1 Lines if R0 EQUAL 0x01 (MASK=0x01); If DTEST1=1 (Async Mode)
      { 0xA8,	0x0B,	0xFF,	0x83},	// W#220 - 	GOTO Clien1_LAB_Sync_Entry; DTEST=0 (Sync mode)
      { 0x01,	0x64,	0xDE,	0x11},	// W#221 - 	1BYTE WRITE 0x01 TO LAB__SPARE_CTL; Force Async mode
      { 0xC4,	0x63,	0xDE,	0x11},	// W#222 - 	1BYTE WRITE 0xC4 TO LAB__HIGH_BW_CTL; CR-0000175916: Optimize slope comp for async mode
      { 0xF1,	0x41,	0x72,	0x40},	// W#223 - 	1BYTE READ PBS_CLIENT1__HW_TRIG_FE_EN TO R1; Read Client1 FE_EN (need for read modify write)
      { 0xEF,	0x00,	0xF1,	0x2C},	// W#224 - 	1BYTE R1 = R1 AND 0xEF; AND(R1
      { 0xF1,	0x41,	0x72,	0x08},	// W#225 - 	1BYTE WRITE R1 TO PBS_CLIENT1__HW_TRIG_FE_EN; Disable Client1
      { 0x00,	0x80,	0xFF,	0xC5},	// W#226 - 	; Start a 1s EOSR timer
      { 0x00,	0x64,	0xDE,	0x11},	// W#227 - 	1BYTE WRITE 0x00 TO LAB__SPARE_CTL; Undo Force Async mode. LAB goes back to auto" mode"
      { 0x60,	0x00,	0xFE,	0xC1},	// W#228 - 	WAIT 5052 ns; Wait 5us
      { 0xF1,	0x41,	0x72,	0x40},	// W#229 - 	1BYTE READ PBS_CLIENT1__HW_TRIG_FE_EN TO R1; Read Client1 FE_EN (need for read modify write)
      { 0x10,	0x00,	0xF1,	0x30},	// W#230 - 	1BYTE R1 = R1 OR 0x10; OR(R1
      { 0xF1,	0x41,	0x72,	0x08},	// W#231 - 	1BYTE WRITE R1 TO PBS_CLIENT1__HW_TRIG_FE_EN; Enable Client1
      { 0xF0,	0x09,	0x09,	0x40},	// W#232 - 	1BYTE READ MISC__STATUS2 TO R0; Read DTEST status
      { 0x01,	0x01,	0x01,	0x90},	// W#233 - 	SKIP 1 Lines if R0 EQUAL 0x01 (MASK=0x01); If DTEST1=1 (ASync Mode)
      { 0x04,	0x63,	0xDE,	0x11},	// W#234 - 	1BYTE WRITE 0x04 TO LAB__HIGH_BW_CTL; CR-0000175916: Optimize slope comp for sync mode
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#235 - 	
      { 0xBC,	0x0F,	0x70,	0x29},	// W#236 - 	MEMADDR(END_PON_REASONS_PREVIOUS); Load R0 with the copy address
      { 0xC4,	0x0F,	0x72,	0x29},	// W#237 - 	MEMADDR(END_PON_REASONS_PREVIOUS2); Load R2 with the paste address
      { 0x04,	0x00,	0x78,	0x28},	// W#238 - 	1BYTE R8 = 0x04; Loop 4 times (shift 4 DWORDS)
      { 0xF0,	0xFF,	0xF4,	0xA3},	// W#239 - 	4BYTE R4 = MEMADDR(R0); copy the last reason registers into a buffer
      { 0xF2,	0xFF,	0xF4,	0xA7},	// W#240 - 	4BYTE MEMADDR(R2) = R4; write the reasons to the location pointed to by the paste pointer
      { 0x01,	0x00,	0x78,	0x24},	// W#241 - 	
      { 0x00,	0x81,	0xFF,	0x92},	// W#242 - 	SKIP 3 Lines if R8 EQUAL 0x00 (MASK=0xFF); Loop Until loop count (R8) = 4
      { 0x04,	0x00,	0x70,	0x25},	// W#243 - 	2BYTE R0 = R0 - 0x04; Move read pointer to next DWORD
      { 0x04,	0x00,	0x72,	0x25},	// W#244 - 	2BYTE R2 = R2 - 0x04; Move write pointer to next DWORD
      { 0xBC,	0x0B,	0xFF,	0x83},	// W#245 - 	
      { 0xF4,	0xC0,	0x08,	0x40},	// W#246 - 	
      { 0xF5,	0xC2,	0x08,	0x40},	// W#247 - 	
      { 0xF6,	0xC4,	0x08,	0x40},	// W#248 - 	
      { 0xF7,	0xC5,	0x08,	0x40},	// W#249 - 	
      { 0xF0,	0xFF,	0xF4,	0xA7},	// W#250 - 	4BYTE MEMADDR(R0) = R4; Write data to R0 (the last register that we copied data out of)
      { 0x04,	0x00,	0x70,	0x21},	// W#251 - 	2BYTE R0 = R0 + 0x04; Increment pointer to next DWord
      { 0xF4,	0xC7,	0x08,	0x40},	// W#252 - 	
      { 0xF5,	0xC8,	0x08,	0x40},	// W#253 - 	
      { 0xF6,	0xC9,	0x08,	0x40},	// W#254 - 	
      { 0xFF,	0x00,	0x77,	0x28},	// W#255 - 	1BYTE R7 = 0xFF; Leave RTC blank (0xFF) since RTC is not available on PMi
      { 0xF0,	0xFF,	0xF4,	0xA7},	// W#256 - 	4BYTE MEMADDR(R0) = R4; Write data to address stored in R0
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#257 - 	End of subroutine. PC = Stored PC.; Byte-2
      { 0x38,	0x53,	0x11,	0x00},	// W#258 - 	1BYTE WRITE 0x38 TO SCHG8998_OTG__OTG_CFG; CR175539: FTRIM value=0x2A. Set [4]=1 and [1]=0 -->0x38
      { 0x01,	0x40,	0x11,	0x00},	// W#259 - 	1BYTE WRITE 0x01 TO SCHG8998_OTG__CMD_OTG; Enable OTG
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#260 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#261 - 	
      { 0xF1,	0x51,	0x41,	0x40},	// W#262 - 	1BYTE READ FG_BATT_INFO__BATTERY_TEMPERATURE_MSB TO R1; Read Batt temp MSB to R1
      { 0xF0,	0x50,	0x41,	0x40},	// W#263 - 	1BYTE READ FG_BATT_INFO__BATTERY_TEMPERATURE_LSB TO R0; Read Batt temp LSB to R0
      { 0xE0,	0x0F,	0x76,	0x29},	// W#264 - 	Load R6 with first word of AFP DATASET
      { 0xF6,	0xFF,	0x74,	0xA2},	// W#265 - 	Load R4 with data from location pointed to by R6 (detection value); R4 = Hot Threshold
      { 0xE4,	0x0F,	0x76,	0x29},	// W#266 - 	Load R6 with next word of AFP DATASET
      { 0xF6,	0xFF,	0x75,	0xA2},	// W#267 - 	Load R5 with data from location pointed to by R6 (detection value); R5 = Cold Threshold
      { 0xFF,	0xFF,	0x71,	0x38},	// W#268 - 	1BYTE SHIFT R1 LEFT BY 1BIT; Next 5 commands shift R1 [2:0] to [7:5]
      { 0xFF,	0xFF,	0x71,	0x38},	// W#269 - 	
      { 0xFF,	0xFF,	0x71,	0x38},	// W#270 - 	
      { 0xFF,	0xFF,	0x71,	0x38},	// W#271 - 	
      { 0xFF,	0xFF,	0x71,	0x38},	// W#272 - 	
      { 0xFF,	0xFF,	0x70,	0x3C},	// W#273 - 	1BYTE SHIFT R0 RIGHT BY 1BIT; Next 3 commands shit R0 [7:3] to [4:0]
      { 0xFF,	0xFF,	0x70,	0x3C},	// W#274 - 	
      { 0xFF,	0xFF,	0x70,	0x3C},	// W#275 - 	
      { 0xF1,	0xF0,	0xF2,	0x32},	// W#276 - 	1BYTE R2 = R0 OR R1; OR(R0
      { 0xF2,	0xF4,	0xF6,	0x26},	// W#277 - 	1BYTE R6 = R2 - R4; R6 = Temp - HotThresh --> carry=1 if temp < HT
      { 0x01,	0x66,	0xFF,	0x90},	// W#278 - 	SKIP 1 Lines if R6 CARRY 0x01 (MASK=0xFF); Skip if Carry=1
      { 0x01,	0x41,	0x16,	0x10},	// W#279 - 	1BYTE WRITE 0x01 TO SCHG8998_MISC__AFP_MODE; Temp > Hot Threshold
      { 0xF5,	0xF2,	0xF6,	0x26},	// W#280 - 	1BYTE R6 = R5 - R2; R6 = Cold Thresh - Temp --> carry=1 if temp > CT
      { 0x01,	0x66,	0xFF,	0x90},	// W#281 - 	SKIP 1 Lines if R6 CARRY 0x01 (MASK=0xFF); Skip if carry=1
      { 0x01,	0x41,	0x16,	0x10},	// W#282 - 	1BYTE WRITE 0x01 TO SCHG8998_MISC__AFP_MODE; Temp < Cold Threshold
      { 0x00,	0x80,	0xFF,	0xC5},	// W#283 - 	; Set timer to 1s
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#284 - 	
      { 0xC0,	0x00,	0xFE,	0xC1},	// W#285 - 	WAIT 10052 ns; wait 10us
      { 0x00,	0x0B,	0xDC,	0x01},	// W#286 - 	1BYTE WRITE 0x00 TO IBB__STATUS4; Write to status reg before reading pulse count
      { 0xF0,	0x0B,	0xDC,	0x41},	// W#287 - 	1BYTE READ IBB__STATUS4 TO R0; R0 = Status4
      { 0xF3,	0x65,	0xDC,	0x41},	// W#288 - 	1BYTE READ IBB__SWIRE_WLED_CTL2 TO R3; R3 = AVDD_START
      { 0x52,	0x31,	0xFF,	0x90},	// W#289 - 	SKIP 1 Lines if R3 EQUAL 0x52 (MASK=0xFF); Skip next line if AVDD_Start = 41'd
      { 0x9C,	0x0C,	0xFF,	0x83},	// W#290 - 	GOTO Pulses_GT_49; AVDD_START !=41
      { 0x80,	0x00,	0xF0,	0x30},	// W#291 - 	1BYTE R0 = R0 OR 0x80; Set R0 bit 7 high
      { 0xF0,	0x4F,	0xD8,	0x09},	// W#292 - 	1BYTE WRITE R0 TO WLED1_CTRL__AMOLED_VOUT; AMOLED_VOUT = R0
      { 0x62,	0x65,	0xDC,	0x01},	// W#293 - 	1BYTE WRITE 0x62 TO IBB__SWIRE_WLED_CTL2; Set AVDD_START = 49'd
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#294 - 	
      { 0x0F,	0x00,	0xFF,	0x93},	// W#295 - 	SKIP 4 Lines if R0 NOT EQUAL 0x0F (MASK=0xFF); Skip next 4 lines if Status4 != F
      { 0x80,	0x47,	0xDC,	0x01},	// W#296 - 	1BYTE WRITE 0x80 TO IBB__PD_CTL; Status4 = F
      { 0x00,	0x47,	0xDE,	0x01},	// W#297 - 	
      { 0x9A,	0x5E,	0xD8,	0x01},	// W#298 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#299 - 	
      { 0x0D,	0x00,	0xFF,	0x93},	// W#300 - 	SKIP 4 Lines if R0 NOT EQUAL 0x0D (MASK=0xFF); Skip next 4 lines if Status4 != D
      { 0x00,	0x47,	0xDC,	0x01},	// W#301 - 	1BYTE WRITE 0x00 TO IBB__PD_CTL; Status4 = D
      { 0x02,	0x47,	0xDE,	0x01},	// W#302 - 	
      { 0x92,	0x5E,	0xD8,	0x01},	// W#303 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#304 - 	
      { 0x07,	0x00,	0xFF,	0x91},	// W#305 - 	SKIP 2 Lines if R0 NOT EQUAL 0x07 (MASK=0xFF); Skip next line if Status4 != 7
      { 0x01,	0x00,	0x74,	0x28},	// W#306 - 	1BYTE R4 = 0x01; Status4=7 --> VDISP=4.7V --> R4=1
      { 0xF8,	0x0C,	0xFF,	0x83},	// W#307 - 	
      { 0x05,	0x00,	0xFF,	0x91},	// W#308 - 	SKIP 2 Lines if R0 NOT EQUAL 0x05 (MASK=0xFF); Skip next line if Status4 != 5
      { 0x02,	0x00,	0x74,	0x28},	// W#309 - 	1BYTE R4 = 0x02; Status4=5 --> VDISP=4.8V --> R4=2
      { 0xF8,	0x0C,	0xFF,	0x83},	// W#310 - 	
      { 0x03,	0x00,	0xFF,	0x91},	// W#311 - 	SKIP 2 Lines if R0 NOT EQUAL 0x03 (MASK=0xFF); Skip next line if Status4 != 3
      { 0x03,	0x00,	0x74,	0x28},	// W#312 - 	1BYTE R4 = 0x03; Status4=3 --> VDISP=4.9V --> R4=3
      { 0xF8,	0x0C,	0xFF,	0x83},	// W#313 - 	
      { 0x01,	0x00,	0xFF,	0x91},	// W#314 - 	SKIP 2 Lines if R0 NOT EQUAL 0x01 (MASK=0xFF); Skip next line if Status4 != 1
      { 0x04,	0x00,	0x74,	0x28},	// W#315 - 	1BYTE R4 = 0x04; Status4=1 --> VDISP=5.0V --> R4=4
      { 0xF8,	0x0C,	0xFF,	0x83},	// W#316 - 	
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#317 - 	
      { 0x80,	0x00,	0xF4,	0x30},	// W#318 - 	1BYTE R4 = R4 OR 0x80; Set bit7 of R4 high to set the override bit. R4 = Desired Voltage
      { 0xF5,	0x41,	0xDE,	0x41},	// W#319 - 	1BYTE READ LAB__OUTPUT_VOLTAGE TO R5; Read existing LAB Vout
      { 0x80,	0x00,	0xF5,	0x30},	// W#320 - 	1BYTE R5 = R5 OR 0x80; Set bit7 of R5 high to set the override bit. R5 = Existing Voltage
      { 0xF5,	0xF4,	0xF6,	0x26},	// W#321 - 	1BYTE R6 = R5 - R4; R6 = R5 - R4 (existing Vout - desired Vout)
      { 0x01,	0x66,	0xFF,	0x95},	// W#322 - 	SKIP 6 Lines if R6 CARRY 0x01 (MASK=0xFF); Skip if Carry=1 --> current Vout < desired Vout --> go to step up
      { 0x00,	0x60,	0xFF,	0x90},	// W#323 - 	SKIP 1 Lines if R6 NOT EQUAL 0x00 (MASK=0xFF); Carry=0
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#324 - 	End of Sequence; Carry=0 & R6=0 --> Current Vout = Desired Vout
      { 0x01,	0x00,	0x75,	0x24},	// W#325 - 	1BYTE R5 = R5 - 0x01; R5 = R5 - 1 (step down by 1LSB = 100mV)
      { 0xF5,	0x41,	0xDE,	0x09},	// W#326 - 	1BYTE WRITE R5 TO LAB__OUTPUT_VOLTAGE; LAB Vout = R5
      { 0x60,	0x00,	0xFE,	0xC1},	// W#327 - 	WAIT 5052 ns; Wait 5us
      { 0x04,	0x0D,	0xFF,	0x83},	// W#328 - 	
      { 0x01,	0x00,	0x75,	0x20},	// W#329 - 	1BYTE R5 = R5 + 0x01; R5 = R5 + 1 (step up by 1LSB = 100mV)
      { 0xF5,	0x41,	0xDE,	0x09},	// W#330 - 	1BYTE WRITE R5 TO LAB__OUTPUT_VOLTAGE; LAB Vout = R5
      { 0x60,	0x00,	0xFE,	0xC1},	// W#331 - 	WAIT 5052 ns; Wait 5us
      { 0x04,	0x0D,	0xFF,	0x83},	// W#332 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#333 - 	
      { 0xF0,	0xDF,	0x13,	0x40},	// W#334 - 	1BYTE READ SCHG8998_USB__SPARE TO R0; UNUSED!!! No longer need WA2. CHG trigger pointed directly to WA1
      { 0x01,	0x01,	0xFF,	0x90},	// W#335 - 	SKIP 1 Lines if R0 EQUAL 0x01 (MASK=0xFF); Skip next like if spare bit=1
      { 0x6C,	0x0D,	0xFF,	0x83},	// W#336 - 	GOTO Client1_SCHG_PD_WA1_Crude_Sensor; Spare bit = 0 --> GOTO crude sensor WA
      { 0xF0,	0x0E,	0x13,	0x40},	// W#337 - 	1BYTE READ SCHG8998_USB__TYPE_C_STATUS_4 TO R0; Read [7] UFP_DFP_MODE
      { 0xF1,	0x68,	0x13,	0x40},	// W#338 - 	1BYTE READ SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL TO R1; R1 = Read (0x1368)
      { 0x00,	0x01,	0x80,	0x92},	// W#339 - 	SKIP 3 Lines if R0 EQUAL 0x00 (MASK=0x80); Skip next line if [7]=0 i.e. UFP mode
      { 0x04,	0x00,	0xF1,	0x30},	// W#340 - 	1BYTE R1 = R1 OR 0x04; R1 = R1 OR 0x4 --> Set bit[2] = 1
      { 0xF1,	0x68,	0x13,	0x08},	// W#341 - 	1BYTE WRITE R1 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; Set 0x1368[2]=1
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#342 - 	
      { 0xFB,	0x00,	0x71,	0x20},	// W#343 - 	1BYTE R1 = R1 + 0xFB; R1 = R1 AND 0xFB --> Set bit[2]=0
      { 0xF1,	0x68,	0x13,	0x08},	// W#344 - 	1BYTE WRITE R1 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; Set 0x1368[2]=0 --> Charger in DRP mode
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#345 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#346 - 	
      { 0xE4,	0x0D,	0x14,	0x88},	// W#347 - 	added RAM v6, PMICPR-1187
      { 0xF0,	0x0E,	0x13,	0x40},	// W#348 - 	1BYTE READ SCHG8998_USB__TYPE_C_STATUS_4 TO R0; Read [7] UFP_DFP_MODE
      { 0x00,	0x01,	0x80,	0x94},	// W#349 - 	SKIP 2 Lines if R0 EQUAL 0x00 (MASK=0x80); Skip next line if [7]=0 i.e. UFP mode
      { 0xF7,	0x59,	0x13,	0x40},	// W#350 - 	
      { 0x02,	0x00,	0xF7,	0x30},	// W#351 - 	Set Bit 1 = 1
      { 0xF7,	0x59,	0x13,	0x08},	// W#352 - 	UFP_DFP_MODE=1 --> In DFP mode --> Disable crude sensor 1359[1]=1
      { 0xF4,	0x0D,	0x14,	0x88},	// W#353 - 	added RAM v6, PMICPR-1187
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#354 - 	
      { 0xF1,	0x68,	0x13,	0x40},	// W#355 - 	1BYTE READ SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL TO R1; Check if charger is in DRP
      { 0x00,	0x10,	0x06,	0x90},	// W#356 - 	SKIP 1 Lines if R1 NOT EQUAL 0x00 (MASK=0x06); If 0x1368[2:1] != 0
      { 0x10,	0x42,	0x73,	0x00},	// W#357 - 	1BYTE WRITE 0x10 TO PBS_CLIENT2__TRIG_CTL; Trigger PBS Client2
      { 0xF4,	0x0D,	0x14,	0x88},	// W#358 - 	added RAM v6, PMICPR-1187
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#359 - 	
      { 0x42,	0x00,	0xFF,	0xC5},	// W#360 - 	; Start 2ms EOSR timer
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#361 - 	
      { 0xE4,	0x0D,	0x14,	0x88},	// W#362 - 	added RAM v6, PMICPR-1187
      { 0xF1,	0x68,	0x13,	0x40},	// W#363 - 	1BYTE READ SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL TO R1; R1 = 0x1368
      { 0x04,	0x11,	0x04,	0x96},	// W#364 - 	SKIP 4 Lines if R1 EQUAL 0x04 (MASK=0x04); Skip next four lines if 0x1368[2]=1
      { 0x04,	0x00,	0xF1,	0x30},	// W#365 - 	1BYTE R1 = R1 OR 0x04; Set R1[2]=1 --> SNK_EN_CMD=1
      { 0xF1,	0x68,	0x13,	0x08},	// W#366 - 	1BYTE WRITE R1 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; 0x1368=R1 --> SNK_EN_CMD=1
      { 0xF7,	0x59,	0x13,	0x40},	// W#367 - 	
      { 0xFD,	0x00,	0xF7,	0x2C},	// W#368 - 	Set Bit 1 = 0
      { 0xF7,	0x59,	0x13,	0x08},	// W#369 - 	Set 0x1359[1]=0 --> Enable Crude Sensor
      { 0xF4,	0x0D,	0x14,	0x88},	// W#370 - 	added RAM v6, PMICPR-1187
      { 0x42,	0x00,	0xFF,	0xC5},	// W#371 - 	; Start 2ms timer
      { 0xFB,	0x00,	0xF1,	0x2C},	// W#372 - 	1BYTE R1 = R1 AND 0xFB; Set R1[2]=0 --> SNK_EN_CMD=0
      { 0xF1,	0x68,	0x13,	0x08},	// W#373 - 	1BYTE WRITE R1 TO SCHG8998_USB__TYPE_C_INTRPT_ENB_SOFTWARE_CTRL; 0x1368=R1 --> SNK_EN_CMD=0
      { 0xF4,	0x0D,	0x14,	0x88},	// W#374 - 	added RAM v6, PMICPR-1187
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#375 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#376 - 	
      { 0xF0,	0xDF,	0x14,	0x40},	// W#377 - 	Read 0x14DF to R0
      { 0x01,	0x00,	0xF0,	0x30},	// W#378 - 	Set bit0 = high
      { 0xF0,	0xDF,	0x14,	0x08},	// W#379 - 	Set 0x14DF = R0 (set bit 0 = 1)
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#380 - 	
      { 0xF0,	0xDF,	0x14,	0x40},	// W#381 - 	Read 0x14DF to R0
      { 0x08,	0x01,	0x08,	0x91},	// W#382 - 	Skip next 2 lines if bit3=1 --> Apps is awake --> leave bit 0 high
      { 0xFE,	0x00,	0xF0,	0x2C},	// W#383 - 	Set bit0 = low
      { 0xF0,	0xDF,	0x14,	0x08},	// W#384 - 	Set 0x14DF = R0 (set bit 0 = 0)
      { 0xFF,	0xFF,	0xFF,	0x8F},	// W#385 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#386 - 	
      { 0xF4,	0x50,	0x73,	0x40},	// W#387 - 	Read Scratch 1 register
      { 0x02,	0x40,	0x02,	0x91},	// W#388 - 	Skip next lines if bit 1 is not set
      { 0xF4,	0x51,	0x73,	0x08},	// W#389 - 	Set Scratch 2 = R4 to show SW that request was read
      { 0x28,	0x0E,	0xFF,	0x83},	// W#390 - 	
      { 0xFF,	0x51,	0x73,	0x00},	// W#391 - 	Only bit 1 is assigned. Set Scratch 2 = 0xFF to indicate an error
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#392 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#393 - 	
      { 0xFA,	0x69,	0x41,	0x40},	// W#394 - 	RA = ESR PD CFG
      { 0x10,	0x41,	0x10,	0x91},	// W#395 - 	Skip next 2 commands if Scratch1[4]=1
      { 0x02,	0x00,	0xFA,	0x30},	// W#396 - 	Scratch 1 [4] = 0 means exit QM.  Set ESR_PD_MODE to CRG_DSC PD
      { 0x3C,	0x0E,	0xFF,	0x83},	// W#397 - 	
      { 0x0C,	0x00,	0xFA,	0x2C},	// W#398 - 	Scratch 1 [4] = 1 means enter QM.  Set ESR_PD_MODE to NO_PULL
      { 0xFA,	0x69,	0x41,	0x08},	// W#399 - 	Set 0x4169 = RA
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#400 - 	
      { 0xE0,	0x0F,	0x76,	0x29},	// W#401 - 	Load R6 with first word of AFP DATASET
      { 0xF0,	0x8C,	0x08,	0x40},	// W#402 - 	PERPH_RB_SPARE into R0 (Hot Threshold)
      { 0xF6,	0xFF,	0x70,	0xA6},	// W#403 - 	Load data R0 into location pointed to by R6
      { 0x04,	0x00,	0x76,	0x21},	// W#404 - 	Increment pointer by 4 to point at the next set
      { 0xF0,	0x8F,	0x08,	0x40},	// W#405 - 	SOFT_RB_SPARE into R0 (Cold Threshold)
      { 0xF6,	0xFF,	0x70,	0xA6},	// W#406 - 	Load data R0 into location pointed to by R6
      { 0x11,	0x51,	0x74,	0x00},	// W#407 - 	Indicate to SW that the sequence ran
      { 0x08,	0x42,	0x74,	0x00},	// W#408 - 	Trigger bit 3 (i.e. kickstart EOSR trigger)
      { 0xFF,	0xFF,	0xFF,	0xFC},	// W#409 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#410 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#411 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#412 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#413 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#414 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#415 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#416 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#417 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#418 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#419 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#420 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#421 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#422 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#423 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#424 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#425 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#426 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#427 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#428 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#429 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#430 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#431 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#432 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#433 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#434 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#435 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#436 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#437 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#438 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#439 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#440 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#441 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#442 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#443 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#444 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#445 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#446 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#447 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#448 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#449 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#450 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#451 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#452 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#453 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#454 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#455 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#456 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#457 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#458 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#459 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#460 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#461 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#462 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#463 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#464 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#465 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#466 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#467 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#468 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#469 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#470 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#471 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#472 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#473 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#474 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#475 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#476 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#477 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#478 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#479 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#480 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#481 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#482 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#483 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#484 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#485 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#486 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#487 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#488 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#489 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#490 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#491 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#492 - 	; 3:POFF_REASON
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#493 - 	; 7:RTC_LSB
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#494 - 	; 3:POFF_REASON
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#495 - 	; 7:RTC_LSB
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#496 - 	; 3:POFF_REASON
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#497 - 	; 7:RTC_LSB
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#498 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#499 - 	
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#500 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#501 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#502 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#503 - 	Inserted by compiler.
      { 0xAC,	0xFF,	0xFF,	0xFF},	// W#504 - 	3: spare, 2: spare, 1: spare, 0: HOT Threshold	  (0xFE0)
      { 0x00,	0xFF,	0xFF,	0xFF},	// W#505 - 	3: spare, 2: spare, 1: spare, 0: COLD Threshold	 (0xFE4)
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#506 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#507 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#508 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#509 - 	Inserted by compiler.
      { 0xFF,	0xFF,	0xFF,	0xFF},	// W#510 - 	Inserted by compiler.
      { 0x01,	0x0A,	0xFF,	0xF8},	// W#511 - 	Inserted by compiler.
   },
};