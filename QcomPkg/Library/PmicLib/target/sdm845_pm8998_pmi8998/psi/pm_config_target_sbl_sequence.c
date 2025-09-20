 /*! \file  pm_config_target_sbl_sequence.c 
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
 *     Copyright (c) 2016 Qualcomm Technologies, Inc.  All Rights Reserved.
 */ 
 
/*=========================================================================== 
 
                        EDIT HISTORY FOR MODULE 
 
  This document is created by a code generator, therefore this section will 
  not contain comments describing changes made to the module. 
 
$Header: //components/rel/boot.xf/2.0/QcomPkg/Library/PmicLib/target/sdm845_pm8998_pmi8998/psi/pm_config_target_sbl_sequence.c#25 $  
$DateTime: 2019/01/07 03:26:04 $  $Author: pwbldsvc $ 
 
when       who     what, where, why 
--------   ---     ----------------------------------------------------------  
 
===========================================================================*/ 
 
/*=========================================================================== 
 
                     INCLUDE FILES  
 
===========================================================================*/ 
 
#include "pm_target_information.h" 
#include "pm_config_sbl.h" 
 
/*========================== PSI Sequence LUT =============================*/ 
 
//NOTES ON CREATING PSI SEQUENCE: 
 
//1. When creating PSI sequence(Table A), if configuration do not need conditional check, set cond start/end index to 0 
//2. If Reg configuration require conditional check, indicate the range of conditional check using cond start/end index 
//3. For Reg operation PM_SBL_DELAY case, address field contains delay amount in us 
//4. For Reg Operation PM_SBL_PBS_RAM case, data field contains index to access in PBS RAM Table 
//5. For Reg Operation PM_SBL_PBS_RAM case, address field contains size of PBS RAM 
 
//Table B (Conditional configuration check Table): 
//1. DO NOT use the first entry. Index 0 is used when no conditional configuration is needed (This is just to avoid confusion) 
//2. Single or multiple configuration can be added in Table B 
//3. If multiple configuration is needed, it should be entered in a consecutive order, so that it can be indexed in one range(from Table A) 
 
pm_sbl_config_info_type 
pm_sbl_config_info = 
{
 	PSI_SIGNATURE,	// PSI Signature
 	0x01,	// PSI Major Version
 	0x02,	// PSI Minor Version
 	0x01,	// Number of target configurations
 	0x62	// Total number of conditional entries on pm_sbl_cond_seq_type table
 };

pm_sbl_cond_seq_type 
pm_sbl_cond_seq [ ] = 
{	
	 // sid 	data 	mask 	register 	operator 
 	 // PMi8998_GTE_2_0 From Line 663 PMi8998 ">=" 2.0 Contains Conditions for SPMI Sequence
	 { 2,	0x00,	0xFF,	0x100,	GREATER_OR_EQUAL}, 	//1 
 	 { 2,	0x00,	0xFF,	0x101,	GREATER_OR_EQUAL}, 	//2 
 	 { 2,	0x00,	0xFF,	0x102,	GREATER_OR_EQUAL}, 	//3 
 	 { 2,	0x02,	0xFF,	0x103,	GREATER_OR_EQUAL}, 	//4 
 	 // PMi8998_EQ_2_0 From Line 669 PMi8998 "==" 2.0 Contains Conditions for SPMI Sequence
	 { 2,	0x00,	0xFF,	0x100,	EQUAL}, 	//5 
 	 { 2,	0x00,	0xFF,	0x101,	EQUAL}, 	//6 
 	 { 2,	0x00,	0xFF,	0x102,	EQUAL}, 	//7 
 	 { 2,	0x02,	0xFF,	0x103,	EQUAL}, 	//8 
 	 // PMi8998_LTE_2_0 From Line 672 PMi8998 "<=" 2.0 Contains Conditions for SPMI Sequence
	 { 2,	0x00,	0xFF,	0x100,	LESS_OR_EQUAL}, 	//9 
 	 { 2,	0x00,	0xFF,	0x101,	LESS_OR_EQUAL}, 	//10 
 	 { 2,	0x00,	0xFF,	0x102,	LESS_OR_EQUAL}, 	//11 
 	 { 2,	0x02,	0xFF,	0x103,	LESS_OR_EQUAL}, 	//12 
 	 // Clogic_5 From Line 380 CLOGIC
	 { 0,	0x0F,	0x0F,	0x704C,	EQUAL}, 	//13 
 	 // Clogic_6 From Line 383 CLOGIC
	 { 0,	0x00,	0x0F,	0x704C,	EQUAL}, 	//14 
 	 // Clogic_7 From Line 386 CLOGIC
	 { 0,	0x01,	0x0F,	0x704C,	EQUAL}, 	//15 
 	 // Clogic_8 From Line 389 CLOGIC
	 { 0,	0x02,	0x0F,	0x704C,	EQUAL}, 	//16 
 	 // Clogic_9 From Line 392 CLOGIC
	 { 0,	0x03,	0x0F,	0x704C,	EQUAL}, 	//17 
 	 // Clogic_10 From Line 395 CLOGIC
	 { 0,	0x09,	0x0F,	0x704C,	EQUAL}, 	//18 
 	 // Clogic_11 From Line 398 CLOGIC
	 { 0,	0x0A,	0x0F,	0x704C,	EQUAL}, 	//19 
 	 // Clogic_12 From Line 403 CLOGIC
	 { 0,	0xF0,	0xF0,	0x704C,	EQUAL}, 	//20 
 	 // Clogic_12p5 From Line 406 CLOGIC
	 { 0,	0x00,	0xF0,	0x704C,	EQUAL}, 	//21 
 	 // Clogic_13 From Line 409 CLOGIC
	 { 0,	0x10,	0xF0,	0x704C,	EQUAL}, 	//22 
 	 // Clogic_14 From Line 412 CLOGIC
	 { 0,	0x20,	0xF0,	0x704C,	EQUAL}, 	//23 
 	 // Clogic_15 From Line 415 CLOGIC
	 { 0,	0x30,	0xF0,	0x704C,	EQUAL}, 	//24 
 	 // Clogic_16 From Line 418 CLOGIC
	 { 0,	0x90,	0xF0,	0x704C,	EQUAL}, 	//25 
 	 // Clogic_17 From Line 421 CLOGIC
	 { 0,	0xA0,	0xF0,	0x704C,	EQUAL}, 	//26 
 	 // Clogic_18 From Line 426 CLOGIC
	 { 0,	0xF0,	0xF0,	0x704D,	EQUAL}, 	//27 
 	 // Clogic_19 From Line 429 CLOGIC
	 { 0,	0x00,	0xF0,	0x704D,	EQUAL}, 	//28 
 	 // Clogic_20 From Line 432 CLOGIC
	 { 0,	0x10,	0xF0,	0x704D,	EQUAL}, 	//29 
 	 // Clogic_21 From Line 435 CLOGIC
	 { 0,	0x20,	0xF0,	0x704D,	EQUAL}, 	//30 
 	 // Clogic_22 From Line 438 CLOGIC
	 { 0,	0x30,	0xF0,	0x704D,	EQUAL}, 	//31 
 	 // Clogic_23 From Line 441 CLOGIC
	 { 0,	0x90,	0xF0,	0x704D,	EQUAL}, 	//32 
 	 // Clogic_24 From Line 444 CLOGIC
	 { 0,	0xA0,	0xF0,	0x704D,	EQUAL}, 	//33 
 	 // Clogic_25 From Line 449 CLOGIC
	 { 0,	0x0F,	0x0F,	0x704D,	EQUAL}, 	//34 
 	 // Clogic_26 From Line 452 CLOGIC
	 { 0,	0x00,	0x0F,	0x704D,	EQUAL}, 	//35 
 	 // Clogic_27 From Line 455 CLOGIC
	 { 0,	0x01,	0x0F,	0x704D,	EQUAL}, 	//36 
 	 // Clogic_28 From Line 458 CLOGIC
	 { 0,	0x02,	0x0F,	0x704D,	EQUAL}, 	//37 
 	 // Clogic_29 From Line 461 CLOGIC
	 { 0,	0x03,	0x0F,	0x704D,	EQUAL}, 	//38 
 	 // Clogic_30 From Line 464 CLOGIC
	 { 0,	0x09,	0x0F,	0x704D,	EQUAL}, 	//39 
 	 // Clogic_31 From Line 467 CLOGIC
	 { 0,	0x0A,	0x0F,	0x704D,	EQUAL}, 	//40 
 	 // Clogic_32 From Line 472 CLOGIC
	 { 0,	0xF0,	0xF0,	0x704E,	EQUAL}, 	//41 
 	 // Clogic_32p5 From Line 475 CLOGIC
	 { 0,	0x00,	0xF0,	0x704E,	EQUAL}, 	//42 
 	 // Clogic_33 From Line 478 CLOGIC
	 { 0,	0x10,	0xF0,	0x704E,	EQUAL}, 	//43 
 	 // Clogic_34 From Line 481 CLOGIC
	 { 0,	0x20,	0xF0,	0x704E,	EQUAL}, 	//44 
 	 // Clogic_35 From Line 484 CLOGIC
	 { 0,	0x30,	0xF0,	0x704E,	EQUAL}, 	//45 
 	 // Clogic_36 From Line 487 CLOGIC
	 { 0,	0x90,	0xF0,	0x704E,	EQUAL}, 	//46 
 	 // Clogic_37 From Line 490 CLOGIC
	 { 0,	0xA0,	0xF0,	0x704E,	EQUAL}, 	//47 
 	 // Clogic_38 From Line 495 CLOGIC
	 { 0,	0x0F,	0x0F,	0x704D,	EQUAL}, 	//48 
 	 // Clogic_39 From Line 498 CLOGIC
	 { 0,	0x00,	0x0F,	0x704D,	EQUAL}, 	//49 
 	 // Clogic_40 From Line 501 CLOGIC
	 { 0,	0x01,	0x0F,	0x704D,	EQUAL}, 	//50 
 	 // Clogic_41 From Line 504 CLOGIC
	 { 0,	0x02,	0x0F,	0x704D,	EQUAL}, 	//51 
 	 // Clogic_42 From Line 507 CLOGIC
	 { 0,	0x03,	0x0F,	0x704D,	EQUAL}, 	//52 
 	 // Clogic_43 From Line 510 CLOGIC
	 { 0,	0x09,	0x0F,	0x704D,	EQUAL}, 	//53 
 	 // Clogic_44 From Line 513 CLOGIC
	 { 0,	0x0A,	0x0F,	0x704D,	EQUAL}, 	//54 
 	 // Clogic_45 From Line 518 CLOGIC
	 { 4,	0x0F,	0x0F,	0x704C,	EQUAL}, 	//55 
 	 // Clogic_46 From Line 521 CLOGIC
	 { 4,	0x00,	0x0F,	0x704C,	EQUAL}, 	//56 
 	 // Clogic_47 From Line 524 CLOGIC
	 { 4,	0x01,	0x0F,	0x704C,	EQUAL}, 	//57 
 	 // Clogic_48 From Line 527 CLOGIC
	 { 4,	0x02,	0x0F,	0x704C,	EQUAL}, 	//58 
 	 // Clogic_49 From Line 530 CLOGIC
	 { 4,	0x03,	0x0F,	0x704C,	EQUAL}, 	//59 
 	 // Clogic_50 From Line 533 CLOGIC
	 { 4,	0x09,	0x0F,	0x704C,	EQUAL}, 	//60 
 	 // Clogic_51 From Line 536 CLOGIC
	 { 4,	0x0A,	0x0F,	0x704C,	EQUAL}, 	//61 
 	 // Clogic_52 From Line 541 CLOGIC
	 { 4,	0xF0,	0xF0,	0x704C,	EQUAL}, 	//62 
 	 // Clogic_52p5 From Line 544 CLOGIC
	 { 4,	0x00,	0xF0,	0x704C,	EQUAL}, 	//63 
 	 // Clogic_53 From Line 547 CLOGIC
	 { 4,	0x10,	0xF0,	0x704C,	EQUAL}, 	//64 
 	 // Clogic_54 From Line 550 CLOGIC
	 { 4,	0x20,	0xF0,	0x704C,	EQUAL}, 	//65 
 	 // Clogic_55 From Line 553 CLOGIC
	 { 4,	0x30,	0xF0,	0x704C,	EQUAL}, 	//66 
 	 // Clogic_56 From Line 556 CLOGIC
	 { 4,	0x90,	0xF0,	0x704C,	EQUAL}, 	//67 
 	 // Clogic_57 From Line 559 CLOGIC
	 { 4,	0xA0,	0xF0,	0x704C,	EQUAL}, 	//68 
 	 // Clogic_58 From Line 564 CLOGIC
	 { 4,	0x0F,	0x0F,	0x704D,	EQUAL}, 	//69 
 	 // Clogic_59 From Line 567 CLOGIC
	 { 4,	0x00,	0x0F,	0x704D,	EQUAL}, 	//70 
 	 // Clogic_60 From Line 570 CLOGIC
	 { 4,	0x01,	0x0F,	0x704D,	EQUAL}, 	//71 
 	 // Clogic_61 From Line 573 CLOGIC
	 { 4,	0x02,	0x0F,	0x704D,	EQUAL}, 	//72 
 	 // Clogic_62 From Line 576 CLOGIC
	 { 4,	0x03,	0x0F,	0x704D,	EQUAL}, 	//73 
 	 // Clogic_63 From Line 579 CLOGIC
	 { 4,	0x09,	0x0F,	0x704D,	EQUAL}, 	//74 
 	 // Clogic_64 From Line 582 CLOGIC
	 { 4,	0x0A,	0x0F,	0x704D,	EQUAL}, 	//75 
 	 // Clogic_66 From Line 590 CLOGIC
	 { 2,	0x13,	0xFF,	0x01F1,	EQUAL}, 	//76 
 	 // Clogic_67 From Line 593 CLOGIC
	 { 3,	0x80,	0xFF,	0xD844,	EQUAL}, 	//77 
 	 // Clogic_68 From Line 596 CLOGIC
	 { 3,	0x00,	0xFF,	0xD844,	EQUAL}, 	//78 
 	 // Clogic_69 From Line 599 CLOGIC
	 { 3,	0x80,	0xFF,	0xDC44,	EQUAL}, 	//79 
 	 // Clogic_70 From Line 602 CLOGIC
	 { 3,	0x80,	0xFF,	0xDE44,	EQUAL}, 	//80 
 	 // Clogic_71 From Line 605 CLOGIC
	 { 3,	0x00,	0xFF,	0xDE44,	EQUAL}, 	//81 
 	 // Clogic_66_AND_GTE_2 From Line 609 CLOGIC
	 { 2,	0x13,	0xFF,	0x01F1,	EQUAL}, 	//82 
 	 // Clogic_66_AND_GTE_2 From Line 609 CLOGIC
	 { 2,	0x02,	0xFF,	0x0103,	GREATER_OR_EQUAL}, 	//83 
 	 // Clogic_67_AND_GTE_2 From Line 613 CLOGIC
	 { 3,	0x80,	0xFF,	0xD844,	EQUAL}, 	//84 
 	 // Clogic_67_AND_GTE_2 From Line 613 CLOGIC
	 { 2,	0x02,	0xFF,	0x0103,	GREATER_OR_EQUAL}, 	//85 
 	 // Clogic_68_AND_GTE_2 From Line 617 CLOGIC
	 { 3,	0x00,	0xFF,	0xD844,	EQUAL}, 	//86 
 	 // Clogic_68_AND_GTE_2 From Line 617 CLOGIC
	 { 2,	0x02,	0xFF,	0x0103,	GREATER_OR_EQUAL}, 	//87 
 	 // Clogic_69_AND_GTE_2 From Line 621 CLOGIC
	 { 3,	0x80,	0xFF,	0xDC44,	EQUAL}, 	//88 
 	 // Clogic_69_AND_GTE_2 From Line 621 CLOGIC
	 { 2,	0x02,	0xFF,	0x0103,	GREATER_OR_EQUAL}, 	//89 
 	 // Clogic_70_AND_GTE_2 From Line 625 CLOGIC
	 { 3,	0x80,	0xFF,	0xDE44,	EQUAL}, 	//90 
 	 // Clogic_70_AND_GTE_2 From Line 625 CLOGIC
	 { 2,	0x02,	0xFF,	0x0103,	GREATER_OR_EQUAL}, 	//91 
 	 // Clogic_71_AND_GTE_2 From Line 629 CLOGIC
	 { 3,	0x00,	0xFF,	0xDE44,	EQUAL}, 	//92 
 	 // Clogic_71_AND_GTE_2 From Line 629 CLOGIC
	 { 2,	0x02,	0xFF,	0x0103,	GREATER_OR_EQUAL}, 	//93 
 	 // PM845_OTP_VERSION_EQ_0 From Line 635 CLOGIC
	 { 0,	0x00,	0xFF,	0x0161,	EQUAL}, 	//94 
 	 // OPTION2_EQ_GND From Line 639 CLOGIC
	 { 0,	0x00,	0xFF,	0x7151,	EQUAL}, 	//95 
 	 // OPTION2_NOT_EQ_GND From Line 642 CLOGIC
	 { 0,	0x00,	0xFF,	0x7151,	GREATER}, 	//96 
 	 // OPTION2_EQ_HIGHZ From Line 645 CLOGIC
	 { 0,	0x02,	0xFF,	0x7151,	EQUAL}, 	//97 
 	 // OPTION2_EQ_VPH From Line 648 CLOGIC
	 { 0,	0x03,	0xFF,	0x7151,	EQUAL}, 	//98
 };

pm_sbl_seq_type 
pm_sbl_seq [ ] = 
{

	 //Mode - FTS_UNGANG_REGANG_PM8998_ORIG_RUMI 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x00,	 0x2F46,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S10_CTRL.EN_CTL, 0x00, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x00,	 0x35C1,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.GANG_CTL2, 0x00, 0xFF, None, PM845_OTP_VERSION_EQ_0	 # Ungang S12 from S13	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x00,	 0x36C1,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.GANG_CTL2, 0x00, 0xFF, None, PM845_OTP_VERSION_EQ_0	 # Ungang S12 from S13	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x00,	 0x32C1,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.GANG_CTL2, 0x00, 0xFF, None, PM845_OTP_VERSION_EQ_0	 # Ungang S11 from S13	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x00,	 0x33C1,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.GANG_CTL2, 0x00, 0xFF, None, PM845_OTP_VERSION_EQ_0	 # Ungang S11 from S13	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x00,	 0x3852,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.PHASE_CTL, 0x00, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x35,	 0x35C0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.GANG_CTL1, 0x35, 0xFF, None, PM845_OTP_VERSION_EQ_0     # Set Gang Leader PID to S12_CTRL	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x80,	 0x3552,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.PHASE_CTL, 0x80, 0xFF, None, PM845_OTP_VERSION_EQ_0      # Enable Multiphase	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x00,	 0x3553,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.PHASE_ID, 0x00, 0xFF, None, PM845_OTP_VERSION_EQ_0	 # Set to Master phase	 
	 { 1,	 0xA5,	 0x36D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x35,	 0x36C0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.GANG_CTL1, 0x35, 0xFF, None, PM845_OTP_VERSION_EQ_0     # Set Gang Leader PID to S12_CTR	 
	 { 1,	 0xA5,	 0x36D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x80,	 0x3652,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.PHASE_CTL, 0x80, 0xFF, None, PM845_OTP_VERSION_EQ_0      # Enable Multiphase	 
	 { 1,	 0xA5,	 0x36D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x00,	 0x3653,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.PHASE_ID, 0x00, 0xFF, None, PM845_OTP_VERSION_EQ_0	 # Set to Master phase	 
	 { 1,	 0xA5,	 0x36D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x80,	 0x36C1,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_PS.GANG_CTL2, 0x80, 0xFF, None, PM845_OTP_VERSION_EQ_0     # Set Gang Leader PID to S12_CTR	 
	 { 1,	 0xA5,	 0x32D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x35,	 0x32C0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.GANG_CTL1, 0x35, 0xFF, None, PM845_OTP_VERSION_EQ_0     # Set Gang Leader PID to S12_CTRL	 
	 { 1,	 0xA5,	 0x32D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x80,	 0x3252,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.PHASE_CTL, 0x80, 0xFF, None, PM845_OTP_VERSION_EQ_0      # Enable Multiphase	 
	 { 1,	 0xA5,	 0x32D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x01,	 0x3253,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.PHASE_ID, 0x01, 0xFF, None, PM845_OTP_VERSION_EQ_0	 # Set to Slave 1	 
	 { 1,	 0xA5,	 0x33D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x35,	 0x33C0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.GANG_CTL1, 0x35, 0xFF, None, PM845_OTP_VERSION_EQ_0    # Set Gang Leader PID to S12_CTR	 
	 { 1,	 0xA5,	 0x33D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x80,	 0x3352,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.PHASE_CTL, 0x80, 0xFF, None, PM845_OTP_VERSION_EQ_0     # Enable Multiphase	 
	 { 1,	 0xA5,	 0x33D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x01,	 0x3353,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.PHASE_ID, 0x01, 0xFF, None, PM845_OTP_VERSION_EQ_0	 # Set to Slave 1	 
	 { 1,	 0xA5,	 0x33D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x80,	 0x33C1,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_PS.GANG_CTL2, 0x80, 0xFF, None, PM845_OTP_VERSION_EQ_0     # Set Gang Leader PID to S12_CTR	 
	 { 1,	 0xA5,	 0x32D0,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x80,	 0x32C1,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S11_CTRL.GANG_CTL2, 0x80, 0xFF, None, PM845_OTP_VERSION_EQ_0      #Gang S11 to S12	 
	 { 1,	 0x3C,	 0x3540,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.VSET_LB, 0x3C, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x03,	 0x3541,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S12_CTRL.VSET_UB, 0x03, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x68,	 0x3840,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.VSET_LB, 0x68, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x03,	 0x3841,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.VSET_UB, 0x03, 0xFF, None, PM845_OTP_VERSION_EQ_0	 
	 { 1,	 0x80,	 0x3846,	 0xFF,	 PM_SBL_WRITE,	 94,	 94}, 	 //PM8998.S13_CTRL.EN_CTL, 0x80, 0xFF, None, PM845_OTP_VERSION_EQ_0
	 //Mode - FTS_UNGANG_REGANG_PM8998_COMPLETION 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x29D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x2AC1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.GANG_CTL2, 0x00	 
	 { 1,	 0xA5,	 0x29D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x2C,	 0x29C0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.GANG_CTL1, 0x2C     # Set Gang Leader PID to S9_CTRL	 
	 { 1,	 0xA5,	 0x29D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x2952,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.PHASE_CTL, 0x80      # Enable Multiphase	 
	 { 1,	 0xA5,	 0x29D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x2953,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.PHASE_ID, 0x01	 # Set to Slave 1	 
	 { 1,	 0xA5,	 0x2AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x2C,	 0x2AC0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.GANG_CTL1, 0x2C     # Set Gang Leader PID to S9_CTRL	 
	 { 1,	 0xA5,	 0x2AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x2A52,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.PHASE_CTL, 0x80      # Enable Multiphase	 
	 { 1,	 0xA5,	 0x2AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x2A53,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.PHASE_ID, 0x01	 # Set to Slave 1	 
	 { 1,	 0xA5,	 0x2AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x2AC1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.GANG_CTL2, 0x80     # Set Gang Leader PID to S9_CTRL	 
	 { 1,	 0xA5,	 0x29D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x29C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.GANG_CTL2, 0x80      #Gang S8 to S9	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x2C52,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.PHASE_CTL, 0x80      # Enable Multiphase 	 
	 { 1,	 0xA5,	 0x29D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x2BC1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_FREQ.GANG_CTL2, 0x00	 
	 { 1,	 0xA5,	 0x2BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x2C,	 0x2BC0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_FREQ.GANG_CTL1, 0x2C     # Set Gang Leader PID to S9_CTR	 
	 { 1,	 0xA5,	 0x2BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x2BC1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_FREQ.GANG_CTL2, 0x80	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x03,	 0x2B51,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_FREQ.CLK_PHASE, 0x03  # Added SBL v0x02 to make S9/S8 separation close to 180 degrees	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x2C50,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.FREQ_CTL, 0x05  # Added SBL v0x02 	 
	 { 1,	 0x68,	 0x2C40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.VSET_LB, 0x68   # Change S9/S8 	 
	 { 1,	 0x03,	 0x2C41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.VSET_UB, 0x03   # Change S9/S8 	 
	 { 1,	 0x80,	 0x2C46,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.EN_CTL, 0x80 # Redundant, for slave	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5 # S11_PS still ganged to S13_CTRL	 
	 { 1,	 0x00,	 0x33C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.GANG_CTL2, 0x00	 
	 { 1,	 0xA5,	 0x32D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x35,	 0x32C0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.GANG_CTL1, 0x35     # Set Gang Leader PID to S12_CTRL	 
	 { 1,	 0xA5,	 0x32D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x3253,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.PHASE_ID, 0x01	 # Set to Slave 1	 
	 { 1,	 0xA5,	 0x33D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x35,	 0x33C0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.GANG_CTL1, 0x35     # Set Gang Leader PID to S12_CTRL	 
	 { 1,	 0xA5,	 0x33D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x3353,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.PHASE_ID, 0x01	 # Set to Slave 1	 
	 { 1,	 0xA5,	 0x33D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x33C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.GANG_CTL2, 0x80     # Set Gang Leader PID to S12_CTRL	 
	 { 1,	 0xA5,	 0x32D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x32C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.GANG_CTL2, 0x80      #Gang S10 to S12	 
	 { 1,	 0xA5,	 0x2FD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x30C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.GANG_CTL2, 0x00	 
	 { 1,	 0xA5,	 0x2FD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x35,	 0x2FC0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.GANG_CTL1, 0x35     # Set Gang Leader PID to S12_CTRL	 
	 { 1,	 0xA5,	 0x2FD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x2F52,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.PHASE_CTL, 0x80      # Enable Multiphase	 
	 { 1,	 0xA5,	 0x2FD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x02,	 0x2F53,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.PHASE_ID, 0x02	 # Set to Slave 2	 
	 { 1,	 0xA5,	 0x30D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x35,	 0x30C0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.GANG_CTL1, 0x35     # Set Gang Leader PID to S12_CTRL	 
	 { 1,	 0xA5,	 0x30D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x3052,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.PHASE_CTL, 0x80      # Enable Multiphase	 
	 { 1,	 0xA5,	 0x30D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x02,	 0x3053,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.PHASE_ID, 0x02	 # Set to Slave 2	 
	 { 1,	 0xA5,	 0x30D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x30C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.GANG_CTL2, 0x80     # Set Gang Leader PID to S12_CTR	 
	 { 1,	 0xA5,	 0x2FD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x2FC1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.GANG_CTL2, 0x80      #Gang S10 to S12	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x37C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_FREQ.GANG_CTL2, 0x00	 
	 { 1,	 0x00,	 0x34C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_FREQ.GANG_CTL2, 0x00	 
	 { 1,	 0xA5,	 0x2FD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x31C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_FREQ.GANG_CTL2, 0x00	 
	 { 1,	 0xA5,	 0x31D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x35,	 0x31C0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_FREQ.GANG_CTL1, 0x35     # Set Gang Leader PID to S12_CTR	 
	 { 1,	 0xA5,	 0x34D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x35,	 0x34C0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_FREQ.GANG_CTL1, 0x35     # Set Gang Leader PID to S12_CTR	 
	 { 1,	 0xA5,	 0x37D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x35,	 0x37C0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_FREQ.GANG_CTL1, 0x35     # Set Gang Leader PID to S12_CTR	 
	 { 1,	 0xA5,	 0x31D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x31C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_FREQ.GANG_CTL2, 0x80	 
	 { 1,	 0xA5,	 0x34D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x34C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_FREQ.GANG_CTL2, 0x80	 
	 { 1,	 0xA5,	 0x37D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x37C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_FREQ.GANG_CTL2, 0x80	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x06,	 0x3151,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_FREQ.CLK_PHASE, 0x06  # Added SBL v0x02 to make S12/S11/S10 separation close to 120 degrees	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x3451,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_FREQ.CLK_PHASE, 0x00  # Added SBL v0x02 to make S12/S11/S10 separation close to 120 degrees	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0B,	 0x3751,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_FREQ.CLK_PHASE, 0x0B  # Added SBL v0x02 to make S12/S11/S10 separation close to 120 degrees	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x3550,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.FREQ_CTL, 0x05  # Added SBL v0x02 	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x1452,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.PHASE_CTL, 0x00  # Disable Multiphase 	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x2352,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.PHASE_CTL, 0x00  # Disable Multiphase 	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x3852,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.PHASE_CTL, 0x00  # Disable Multiphase 
	 //Mode - EUD_PIN_CTRL 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x04,	 0xD342,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO20.DIG_PULL_CTL, 0x04	   # Set GPIO20 	 
	 { 0,	 0x01,	 0xD341,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO20.DIG_VIN_CTL, 0x01	   # Set GPIO20 	 
	 { 0,	 0x00,	 0xD340,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO20.MODE_CTL, 0x0		   # Set GPIO20 	 
	 { 0,	 0x80,	 0xD346,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO20.EN_CTL, 0x80          # Enable	 
	 { 0,	 0x01,	 0x5147,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK1.CLK_EN_CTL, 0x01  # Force BBCLK1 Pin control 	 
	 { 0,	 0xA5,	 0xD3D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO20.SEC_ACCESS, 0xA5	 
	 { 0,	 0x0B,	 0xD3DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO20.PERPH_RESET_CTL3, 0x0B   # Ignore Warm_rb	 
	 { 2,	 0xB8,	 0x1363,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.SCHG8998_USB.USBIN_OPTIONS_2_CFG, 0xB8
	 //Mode - PBS_CONFIG_PM8998 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CFG, 0x80		# Assert RIF_MEM_ACCESS_EN	 
	 { 0,	 0xC0,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CTL, 0xC0		# Assert BURST and WR_EN	 
	 { 0,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Set RAM address LSB to 0x00	 
	 { 0,	 0x04,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x4		# Set RAM address MSB to 0x04	 
	 { 0,	 0,	     0x0200,	 0xFF,	 PM_SBL_PBS_RAM, 0,	 0}, 	 //"PM8998_RAM_b0x01_v0x0D"		# Load PBS RAM image	 
	 { 0,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CFG, 0x0		# De-assert RIF_MEM_ACCESS_EN	 
	 { 0,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CTL, 0x0		# De-assert BURST and WR_EN	 
	 { 0,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Clear RAM address LSB	 
	 { 0,	 0x00,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0		# Clear RAM address MSB	 
	 { 0,	 0x08,	 0x7050,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.TRIG_START_ADDR_LSB_0, 0x8		# New starting address for the PON Client in RAM 	 
	 { 0,	 0x04,	 0x7051,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.TRIG_START_ADDR_MSB_0, 0x4		# New starting address for the PON Client in RAM 	 
	 { 0,	 0x0C,	 0x7054,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.TRIG_START_ADDR_LSB_1, 0xC		# New starting address for Client 1 in RAM 	 
	 { 0,	 0x04,	 0x7055,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.TRIG_START_ADDR_MSB_1, 0x4		# New starting address for Client 1 in RAM 	 
	 { 0,	 0x10,	 0x7058,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.TRIG_START_ADDR_LSB_2, 0x10		# New starting address for Client 2 in RAM 	 
	 { 0,	 0x04,	 0x7059,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.TRIG_START_ADDR_MSB_2, 0x4		# New starting address for Client 2 in RAM 	 
	 { 0,	 0x14,	 0x705C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.TRIG_START_ADDR_LSB_3, 0x14		# New starting address for Client 3 in RAM 	 
	 { 0,	 0x04,	 0x705D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.TRIG_START_ADDR_MSB_3, 0x4		# New starting address for Client 3 in RAM 	 
	 { 0,	 0xA5,	 0x70D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x70DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.PERPH_RESET_CTL3, 0x0	 
	 { 0,	 0xA5,	 0x72D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT1.SEC_ACCESS, 0xA5	 
	 { 0,	 0x03,	 0x72DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT1.PERPH_RESET_CTL3, 0x3	 
	 { 0,	 0xA5,	 0x73D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT2.SEC_ACCESS, 0xA5	 
	 { 0,	 0x03,	 0x73DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT2.PERPH_RESET_CTL3, 0x3	 
	 { 0,	 0x00,	 0x7241,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT1.HW_TRIG_FE_EN, 0x00		# All HW Triggers unused	 
	 { 0,	 0x10,	 0x7240,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT1.HW_TRIG_RE_EN, 0x10		# Enable HW Trigger bit 4 	 
	 { 0,	 0x2C,	 0x7340,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT2.HW_TRIG_RE_EN, 0x2C		# Enable HW Trigger bit 5 	 
	 { 0,	 0x2C,	 0x7341,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT2.HW_TRIG_FE_EN, 0x2C		# Enable HW Trigger bit 5 	 
	 { 0,	 0x00,	 0x7440,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT3.HW_TRIG_RE_EN, 0x00		# All HW Triggers unused	 
	 { 0,	 0x00,	 0x7441,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT3.HW_TRIG_FE_EN, 0x00		# All HW Triggers unused	 
	 { 0,	 0xFF,	 0x7248,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT1.TRIG_LATCHED_CLR, 0xFF	 
	 { 0,	 0xFF,	 0x7348,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT2.TRIG_LATCHED_CLR, 0xFF	 
	 { 0,	 0xFF,	 0x7448,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT3.TRIG_LATCHED_CLR, 0xFF	 
	 { 0,	 0x80,	 0x7246,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT1.EN_CTL, 0x80		# Enable PBS Client 1	 
	 { 0,	 0x80,	 0x7346,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT2.EN_CTL, 0x80		# Enable PBS Client 2	 
	 { 0,	 0x80,	 0x7446,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CLIENT3.EN_CTL, 0x80		# Enable PBS Client 3	 
	 { 0,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CFG, 0x80	 
	 { 0,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CFG, 0x0
	 //Mode - PBS_CONFIG_PM8005 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 4,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CFG, 0x80		# Assert RIF_MEM_ACCESS_EN	 
	 { 4,	 0xC0,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CTL, 0xC0		# Assert BURST and WR_EN	 
	 { 4,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Set RAM address LSB to 0x00	 
	 { 4,	 0x02,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_MSB, 0x2		# Set RAM address MSB to 0x02	 
	 { 4,	 1,	     0x0080,	 0xFF,	 PM_SBL_PBS_RAM, 0,	 0}, 	 //"PM8005_RAM_b0x01_v0x04"	 
	 { 4,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CFG, 0x0		# De-assert RIF_MEM_ACCESS_EN	 
	 { 4,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CTL, 0x0		# De-assert BURST and WR_EN	 
	 { 4,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Clear RAM address LSB	 
	 { 4,	 0x00,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0		# Clear RAM address MSB	 
	 { 4,	 0x08,	 0x7050,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.TRIG_START_ADDR_LSB_0, 0x8		# New starting address for the PON Client in RAM 	 
	 { 4,	 0x02,	 0x7051,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.TRIG_START_ADDR_MSB_0, 0x2		# New starting address for the PON Client in RAM 	 
	 { 4,	 0x0C,	 0x7054,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.TRIG_START_ADDR_LSB_1, 0xC		# New starting address for Client 1 in RAM 	 
	 { 4,	 0x02,	 0x7055,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.TRIG_START_ADDR_MSB_1, 0x2		# New starting address for Client 1 in RAM 	 
	 { 4,	 0xA5,	 0x70D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.SEC_ACCESS, 0xA5	 
	 { 4,	 0x00,	 0x70DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.PERPH_RESET_CTL3, 0x0	 
	 { 4,	 0xFF,	 0x7248,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CLIENT1.TRIG_LATCHED_CLR, 0xFF	 
	 { 4,	 0x80,	 0x7246,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CLIENT1.EN_CTL, 0x80		# Enable PBS Client 1	 
	 { 4,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CFG, 0x80	 
	 { 4,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CFG, 0x0
	 //Mode - PBS_CONFIG_PMi8998 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 2,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_CFG, 0x80		# Assert RIF_MEM_ACCESS_EN	 
	 { 2,	 0xC0,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_CTL, 0xC0		# Assert Burst and WR_EN	 
	 { 2,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Set RAM address LSB to 0x00	 
	 { 2,	 0x08,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x8		# Set RAM address MSB to 0x08	 
	 { 2,	 2,	     0x0200,	 0xFF,	 PM_SBL_PBS_RAM, 0,	 0}, 	 //"PMi8998_RAM_b0x01_v0x0B"		       # Load RAM	 
	 { 2,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_CFG, 0x0		# Deassert RIF_MEM_ACCESS_EN	 
	 { 2,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_CTL, 0x0		# Dessert Burst and WR_EN	 
	 { 2,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Clear RAM address LSB	 
	 { 2,	 0x00,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0		# Clear RAM address MSB	 
	 { 2,	 0x08,	 0x7051,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.TRIG_START_ADDR_MSB_0, 0x8		# Point PBS Client 0 to RAM	 
	 { 2,	 0x08,	 0x7050,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.TRIG_START_ADDR_LSB_0, 0x8		# Point PBS Client 0 to RAM	 
	 { 2,	 0x08,	 0x7055,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.TRIG_START_ADDR_MSB_1, 0x8		# Point PBS Client 1 to RAM	 
	 { 2,	 0x0C,	 0x7054,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.TRIG_START_ADDR_LSB_1, 0xC		# Point PBS Client 1 to RAM	 
	 { 2,	 0x08,	 0x7059,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.TRIG_START_ADDR_MSB_2, 0x8		# Point PBS Client 2 to RAM	 
	 { 2,	 0x10,	 0x7058,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.TRIG_START_ADDR_LSB_2, 0x10		# Point PBS Client 2 to RAM	 
	 { 2,	 0x08,	 0x705D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.TRIG_START_ADDR_MSB_3, 0x8		# Point PBS Client 3 to RAM	 
	 { 2,	 0x14,	 0x705C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.TRIG_START_ADDR_LSB_3, 0x14		# Point PBS Client 3 to RAM	 
	 { 2,	 0xA5,	 0x70D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.SEC_ACCESS, 0xA5	 
	 { 2,	 0x00,	 0x70DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.PERPH_RESET_CTL3, 0x0		# Disable all resets on PBS Core 	 
	 { 2,	 0x0C,	 0x7440,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CLIENT3.HW_TRIG_RE_EN, 0xC		# 12/14/16: Enable RE for trig3 	 
	 { 2,	 0x04,	 0x7441,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CLIENT3.HW_TRIG_FE_EN, 0x4		# Enable trig3 	 
	 { 2,	 0x11,	 0x7340,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.PBS_CLIENT2.HW_TRIG_RE_EN, 0x11, 0xFF, PMi8998_GTE_2_0		# 12/20/16: Enable RE for trig0 	 
	 { 2,	 0x1E,	 0x7240,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.PBS_CLIENT1.HW_TRIG_RE_EN, 0x1E, 0xFF, PMi8998_GTE_2_0		# 12/20/16: Enable RE for Trig1 for Charger PD WA1&2 	 
	 { 2,	 0x12,	 0x7241,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.PBS_CLIENT1.HW_TRIG_FE_EN, 0x12, 0xFF, PMi8998_GTE_2_0		# 12/20/16: Enable FE for trig1 for Charger PD WA1/2.	 
	 { 2,	 0xFF,	 0x7248,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CLIENT1.TRIG_LATCHED_CLR, 0xFF	 
	 { 2,	 0xFF,	 0x7348,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CLIENT2.TRIG_LATCHED_CLR, 0xFF	 
	 { 2,	 0xFF,	 0x7448,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CLIENT3.TRIG_LATCHED_CLR, 0xFF	 
	 { 2,	 0x80,	 0x7346,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CLIENT2.EN_CTL, 0x80		# Enable Client2	 
	 { 2,	 0x80,	 0x7246,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CLIENT1.EN_CTL, 0x80		# Enable Client1	 
	 { 2,	 0x80,	 0x7446,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CLIENT3.EN_CTL, 0x80		# Enable Client3	 
	 { 2,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_CFG, 0x80		#	 
	 { 2,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PBS_CORE.MEM_INTF_CFG, 0x0		#	 
	 { 2,	 0x07,	 0x094B,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.MISC.PD_PHY_INT_MUX_SEL, 0x7, 0xFF, PMi8998_GTE_2_0
	 //Mode - WARM_RESET_DATASET_INIT 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0xA5,	 0x09D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MISC.SEC_ACCESS, 0xA5  # Sec Access	 
	 { 0,	 0x0B,	 0x09DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MISC.PERPH_RESET_CTL3, 0x0B  # Ignore warm_rb - otherwise, spare register gets lost after first WR Pass 0	 
	 { 4,	 0xA5,	 0x09D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.MISC.SEC_ACCESS, 0xA5  # Sec Access	 
	 { 4,	 0x0B,	 0x09DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.MISC.PERPH_RESET_CTL3, 0x0B  # Ignore warm_rb - otherwise, spare register gets lost after first WR Pass 0	 
	 { 0,	 0x00,	 0x0956,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MISC.PC_SPARE1, 0x00  # Initialize to 0, to designate Warm Reset Pass 0/1/2	 
	 { 4,	 0x00,	 0x0945,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.MISC.MISC_CTL2, 0x00  # Initialize to 0, to designate Warm Reset Pass 0/1/2	 
	 { 0,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CFG, 0x80		# Assert RIF_MEM_ACCESS_EN	 
	 { 0,	 0xC0,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CTL, 0xC0		# Assert BURST and WR_EN	 
	 { 0,	 0xA4,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xA4		# Set RAM address LSB	 
	 { 0,	 0x0B,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0B		# Set RAM address MSB	 
	 { 0,	 0xA0,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA0, 0xA0      # SS Nom = 0.928 	 
	 { 0,	 0x03,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA1, 0x03      # SS Nom = 0.928	 
	 { 0,	 0xA0,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA2, 0xA0      # SS Nom = 0.928 	 
	 { 0,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.928	 
	 { 0,	 0xA8,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xA8		# Set RAM address LSB	 
	 { 0,	 0x0B,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0B		# Set RAM address MSB	 
	 { 0,	 0xA0,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA0, 0xA0      # SS Nom = 0.928 	 
	 { 0,	 0x03,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA1, 0x03      # SS Nom = 0.928	 
	 { 0,	 0xA0,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA2, 0xA0      # SS Nom = 0.928 	 
	 { 0,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.928	 
	 { 0,	 0xAC,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xAC		# Set RAM address LSB	 
	 { 0,	 0x0B,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0B		# Set RAM address MSB	 
	 { 0,	 0xC2,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA0, 0xC2      # 0.45V 	 
	 { 0,	 0x01,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA1, 0x01      # 0.45V 	 
	 { 0,	 0xA0,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA2, 0xA0      # SS Nom = 0.928 	 
	 { 0,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.928	 
	 { 0,	 0xB0,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xB0		# Set RAM address LSB	 
	 { 0,	 0x0B,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0B		# Set RAM address MSB	 
	 { 0,	 0x68,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA0, 0x68      # SS Nom = 0.872 	 
	 { 0,	 0x03,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA1, 0x03      # SS Nom = 0.872	 
	 { 0,	 0x68,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA2, 0x68      # SS Nom = 0.872 	 
	 { 0,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.872	 
	 { 0,	 0xB4,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xB4		# Set RAM address LSB	 
	 { 0,	 0x0B,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0B		# Set RAM address MSB	 
	 { 0,	 0x68,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA0, 0x68      # SS Nom = 0.872 	 
	 { 0,	 0x03,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA1, 0x03      # SS Nom = 0.872	 
	 { 0,	 0x68,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA2, 0x68      # SS Nom = 0.872 	 
	 { 0,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.872	 
	 { 0,	 0xB8,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xB8		# Set RAM address LSB	 
	 { 0,	 0x0B,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0B		# Set RAM address MSB	 
	 { 0,	 0x68,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA0, 0x68      # SS Nom = 0.872 	 
	 { 0,	 0x03,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA1, 0x03      # SS Nom = 0.872	 
	 { 0,	 0x68,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA2, 0x68      # SS Nom = 0.872 	 
	 { 0,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.872	 
	 { 0,	 0xBC,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xBC		# Set RAM address LSB	 
	 { 0,	 0x0B,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0B		# Set RAM address MSB	 
	 { 0,	 0x3C,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA0, 0x3C      # SS Nom = 0.828 	 
	 { 0,	 0x03,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA1, 0x03      # SS Nom = 0.828	 
	 { 0,	 0x3C,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA2, 0x3C      # SS Nom = 0.828 	 
	 { 0,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.828	 
	 { 0,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CFG, 0x0		# De-assert RIF_MEM_ACCESS_EN	 
	 { 0,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CTL, 0x0		# De-assert BURST and WR_EN	 
	 { 0,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Clear RAM address LSB	 
	 { 0,	 0x00,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0		# Clear RAM address MSB	 
	 { 4,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CFG, 0x80		# Assert RIF_MEM_ACCESS_EN	 
	 { 4,	 0xC0,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CTL, 0xC0		# Assert BURST and WR_EN	 
	 { 4,	 0xF4,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_LSB, 0xF4		# Set RAM address LSB	 
	 { 4,	 0x03,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_MSB, 0x03		# Set RAM address MSB	 
	 { 4,	 0x68,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_WR_DATA0, 0x68      # SS Nom = 0.872 	 
	 { 4,	 0x03,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_WR_DATA1, 0x03      # SS Nom = 0.872	 
	 { 4,	 0x68,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_WR_DATA2, 0x68      # SS Nom = 0.872 	 
	 { 4,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.872	 
	 { 4,	 0xF8,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_LSB, 0xF8		# Set RAM address LSB	 
	 { 4,	 0x03,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_MSB, 0x03		# Set RAM address MSB	 
	 { 4,	 0x68,	 0x7048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_WR_DATA0, 0x68      # SS Nom = 0.872 	 
	 { 4,	 0x03,	 0x7049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_WR_DATA1, 0x03      # SS Nom = 0.872	 
	 { 4,	 0x68,	 0x704A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_WR_DATA2, 0x68      # SS Nom = 0.872 	 
	 { 4,	 0x03,	 0x704B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_WR_DATA3, 0x03      # SS Nom = 0.872	 
	 { 4,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CFG, 0x0		# De-assert RIF_MEM_ACCESS_EN	 
	 { 4,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CTL, 0x0		# De-assert BURST and WR_EN	 
	 { 4,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Clear RAM address LSB	 
	 { 4,	 0x00,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0		# Clear RAM address MSB
	 //Mode - PON_INIT 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0xA5,	 0x08D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.SEC_ACCESS, 0xA5	 
	 { 0,	 0xEA,	 0x0884,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.UVLO_CTL, 0xEA	 
	 { 0,	 0xA5,	 0x08D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.SEC_ACCESS, 0xA5	 
	 { 0,	 0xAD,	 0x0886,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.OVLO_CTL, 0xAD	 
	 { 4,	 0xA5,	 0x08D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PON.SEC_ACCESS, 0xA5	 
	 { 4,	 0xE7,	 0x0884,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PON.UVLO_CTL, 0xE7	 
	 { 2,	 0xA5,	 0x08D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PON.SEC_ACCESS, 0xA5	 
	 { 2,	 0xE7,	 0x0884,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PON.UVLO_CTL, 0xE7	 
	 { 0,	 0x80,	 0x087F,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.SMPL_CTL, 0x80	 
	 { 0,	 0x48,	 0x3C42,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.VDDGEN.ONE_REG_CTL, 0x48	 
	 { 0,	 0xE4,	 0x0880,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.PON_TRIGGER_EN, 0xE4	 
	 { 0,	 0x00,	 0x0882,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.OPTION_BITS, 0x00	 
	 { 0,	 0x00,	 0x0881,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.PON_TRIGGER_EDGE_CONFIG, 0x00	 
	 { 0,	 0x80,	 0x2446,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.TEMP_ALARM.EN_CTL1, 0x80	 
	 { 4,	 0x80,	 0x2446,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.TEMP_ALARM.EN_CTL1, 0x80	 
	 { 0,	 0x80,	 0x5A46,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.SLEEP_CLK1.EN_CTL, 0x80	 
	 { 2,	 0x40,	 0x3C42,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.VDDGEN.ONE_REG_CTL, 0x40	 
	 { 4,	 0x40,	 0x3C42,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.VDDGEN.ONE_REG_CTL, 0x40
	 //Mode - BUS 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0xA5,	 0x04D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BUS.SEC_ACCESS, 0xA5	 
	 { 0,	 0x01,	 0x04DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BUS.PERPH_RESET_CTL3, 0x1    # Ingore WR, shutdown1_rb.  Only follow shutdown2_rb	 
	 { 4,	 0xA5,	 0x04D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.BUS.SEC_ACCESS, 0xA5	 
	 { 4,	 0x01,	 0x04DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.BUS.PERPH_RESET_CTL3, 0x1    # Ingore WR, shutdown1_rb.  Only follow shutdown2_rb	 
	 { 2,	 0xA5,	 0x04D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.BUS.SEC_ACCESS, 0xA5	 
	 { 2,	 0x01,	 0x04DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.BUS.PERPH_RESET_CTL3, 0x1    # Ingore WR, shutdown1_rb.  Only follow shutdown2_rb
	 //Mode - INIT_GENERAL 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x80,	 0x3746,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.VADC_HC4_CAL.EN_CTL1, 0x80	 
	 { 0,	 0x80,	 0x3747,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.VADC_HC4_CAL.CONV_REQ, 0x80
	 //Mode - RESET_CONFIG_PM8998 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x08,	 0x0842,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.KPDPWR_N_RESET_S2_CTL, 0x8	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300	 
	 { 0,	 0x80,	 0x0843,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.KPDPWR_N_RESET_S2_CTL2, 0x80	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300	 
	 { 0,	 0x08,	 0x084A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.RESIN_AND_KPDPWR_RESET_S2_CTL, 0x8	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300	 
	 { 0,	 0x06,	 0x0849,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.RESIN_AND_KPDPWR_RESET_S2_TIMER, 0x6 # Change S2_TIMER from 2s to 1s 	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300	 
	 { 0,	 0x80,	 0x084B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.RESIN_AND_KPDPWR_RESET_S2_CTL2, 0x80	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300	 
	 { 0,	 0x08,	 0x085A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.PS_HOLD_RESET_CTL, 0x8	 
	 { 0,	 0x01,	 0x0856,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PON.PMIC_WD_RESET_S2_CTL, 0x1
	 //Mode - RESET_CONFIG_PM8005 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 4,	 0x00,	 0x085B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PON.PS_HOLD_RESET_CTL2, 0x0	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300	 
	 { 4,	 0x00,	 0x0853,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PON.GP1_RESET_S2_CTL2, 0x0	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300	 
	 { 4,	 0x0A,	 0x0852,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PON.GP1_RESET_S2_CTL, 0xA	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300	 
	 { 4,	 0x80,	 0x0853,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PON.GP1_RESET_S2_CTL2, 0x80	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300
	 //Mode - RESET_CONFIG_PMi8998 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 2,	 0x00,	 0x085B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.PON.PS_HOLD_RESET_CTL2, 0x0		# Disable PS_HOLD Reset 	 
	 { 0,	 0x99,	 0x012C,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //300
	 //Mode - INTERRUPT 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x80,	 0x0546,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.INT.EN_CTL1, 0x80	 
	 { 4,	 0x80,	 0x0546,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.INT.EN_CTL1, 0x80	 
	 { 2,	 0x80,	 0x0546,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.INT.EN_CTL1, 0x80
	 //Mode - WLAN_PIN_CONTROL 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x00,	 0xC240,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO03.MODE_CTL, 0x0		# Set GPIO3 	 
	 { 0,	 0x80,	 0xC246,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO03.EN_CTL, 0x80		# Enable GPIO3 	 
	 { 0,	 0x00,	 0xD640,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO23.MODE_CTL, 0x0		# Set GPIO23 	 
	 { 0,	 0x80,	 0xD646,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO23.EN_CTL, 0x80		# Enable GPIO23 	 
	 { 0,	 0x01,	 0xD641,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO23.DIG_VIN_CTL, 0x1		# Configure GPIO23 	 
	 { 0,	 0x20,	 0x094C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MISC.VSET1_LB, 0x20		# Program VSET1 	 
	 { 0,	 0x03,	 0x094D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MISC.VSET1_UB, 0x3		# Program VSET1 	 
	 { 0,	 0xEA,	 0x094E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MISC.VSET2_LB, 0xEA		# Program VSET2 	 
	 { 0,	 0x01,	 0x094F,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MISC.VSET2_UB, 0x1		# Program VSET2 	 
	 { 1,	 0x06,	 0x1A49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.MODE_CTL2, 0x6		# Set Secondary 	 
	 { 1,	 0x06,	 0x2049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.MODE_CTL2, 0x6		# Set Secondary 	 
	 { 1,	 0x06,	 0x2649,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.MODE_CTL2, 0x6		# Set Secondary 	 
	 { 0,	 0x01,	 0x5547,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK2.CLK_EN_CTL, 0x1		# Enable Pin Control Capability 	 
	 { 0,	 0x04,	 0x554C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK2.CLK_TIMER, 0x4		# Set RFCLK2 Buffer enable delay to 4 * 	 
	 { 0,	 0x02,	 0x0970,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MISC.RF_CLK_LDO_HW_EN, 0x2		# Configure WLAN pin control for LDO's to follow pin 	 
	 { 0,	 0x01,	 0x5546,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK2.EN_CTL, 0x01		# Disable SW Bit, but leave pin control - redundant since Pin Control Capability is guaranteed by the Write Once register	 
	 { 1,	 0x10,	 0x4447,	 0xFF,	 PM_SBL_WRITE,	 96,	 96}, 	 //PM8998.LDO05.FOLLOW_HWEN, 0x10, 0xFF, None, OPTION2_NOT_EQ_GND      #MODE = FOLLOW_HW_EN0	 
	 { 1,	 0x44,	 0x4647,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO07.FOLLOW_HWEN, 0x44      #MODE / Enable = FOLLOW_HW_EN2	 
	 { 1,	 0x44,	 0x5047,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO17.FOLLOW_HWEN, 0x44      #MODE / Enable = FOLLOW_HW_EN2	 
	 { 1,	 0x44,	 0x5847,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO25.FOLLOW_HWEN, 0x44      #MODE / Enable = FOLLOW_HW_EN2	 
	 { 1,	 0x40,	 0x5647,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO23.FOLLOW_HWEN, 0x40      #MODE = FOLLOW_HW_EN2	 
	 { 1,	 0x40,	 0x5147,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO18.FOLLOW_HWEN, 0x40      #MODE = FOLLOW_HW_EN2	 
	 { 1,	 0x60,	 0x4F47,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO16.FOLLOW_HWEN, 0x60      #MODE = FOLLOW_HW_EN2 and EN1   
	 //Mode - WIGIG_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x88,	 0x2047,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.FOLLOW_HWEN, 0x88    # Force Enable + Secondary Mode if sleep_b goes high 	 
	 { 1,	 0x88,	 0x2647,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.FOLLOW_HWEN, 0x88    # Force Enable + Secondary Mode if sleep_b goes high 
	 //Mode - BCL_CONFIG_PM8998 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x01,	 0x3F12,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP_MDM.INT_POLARITY_HIGH, 0x1	 
	 { 0,	 0x02,	 0x3F13,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP_MDM.INT_POLARITY_LOW, 0x2	 
	 { 0,	 0x26,	 0x3D4B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP.VCOMP_LOW0_THR, 0x26	 
	 { 0,	 0x14,	 0x3D4A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP.VCOMP_LOW1_THR, 0x14	 
	 { 0,	 0x0A,	 0x3D49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP.VCOMP_LOW2_THR, 0x0A	 
	 { 0,	 0x6E,	 0x3D48,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP.VCOMP_HI_THR, 0x6E	 
	 { 0,	 0x09,	 0x3D53,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP.LOW2_CLR_DGL, 0x9	 
	 { 0,	 0x3F,	 0x3D40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP.MODE_CTL, 0x3F	 
	 { 0,	 0x80,	 0x3D46,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_COMP.EN_CTL1, 0x80		# Enable	 
	 { 0,	 0x80,	 0x3E46,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_PLM.EN_CTL1, 0x80		# Enable	 
	 { 0,	 0xFF,	 0x3E15,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.BCL_PLM.INT_EN_SET, 0xFF  # Added SBL vA
	 //Mode - GPIO_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x05,	 0xD942,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO26.DIG_PULL_CTL, 0x5		# Set to High Z - saves 10uA during sleep and off state, for standalone case 	 
	 { 0,	 0xA5,	 0xD9D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO26.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0xD9DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO26.PERPH_RESET_CTL3, 0x0	 
	 { 0,	 0x00,	 0xC442,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO05.DIG_PULL_CTL, 0x0		# Set to 30uA pullup 	 
	 { 0,	 0x00,	 0xC542,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO06.DIG_PULL_CTL, 0x0		# Set to 30uA pullup 	 
	 { 0,	 0x00,	 0xC642,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO07.DIG_PULL_CTL, 0x0		# Set to 30uA pullup 	 
	 { 0,	 0x00,	 0xC742,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO08.DIG_PULL_CTL, 0x0		# Set to 30uA pullup 	 
	 { 0,	 0x00,	 0xD142,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO18.DIG_PULL_CTL, 0x0		# Set to 10uA pullup 	 
	 { 0,	 0x01,	 0xD241,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO19.DIG_VIN_CTL, 0x1		# Configure to VIN level to 1.8V 	 
	 { 0,	 0x01,	 0xD441,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO21.DIG_VIN_CTL, 0x1		# Configure to VIN level to 1.8V 
	 //Mode - CLOCKS 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x0D,	 0x5058,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.XO.XO_COMP, 0xD	 
	 { 0,	 0x00,	 0x5059,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.XO.XO_COMP_CTL, 0x0	 
	 { 0,	 0x80,	 0x505E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.XO.ICTL, 0x80	 
	 { 0,	 0x00,	 0x5046,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.XO.EN_CTL, 0x0	 
	 { 0,	 0x01,	 0x5048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.XO.LDO_XO_EN_CTL, 0x1		# Return back to default setting	 
	 { 0,	 0xA5,	 0x59D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.CLK_DIST.SEC_ACCESS, 0xA5	 
	 { 0,	 0x01,	 0x5941,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.CLK_DIST.CLK_CTL2, 0x1	 
	 { 0,	 0xA5,	 0x59D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.CLK_DIST.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x5944,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.CLK_DIST.CLK_CTL5, 0x0	 
	 { 0,	 0x02,	 0x514C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK1.CLK_TIMER, 0x2	 
	 { 0,	 0x02,	 0x524C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK2.CLK_TIMER, 0x2	 
	 { 0,	 0x02,	 0x534C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK3.CLK_TIMER, 0x2	 
	 { 0,	 0x03,	 0x544C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK1.CLK_TIMER, 0x3	 
	 { 0,	 0x03,	 0x554C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK2.CLK_TIMER, 0x3	 
	 { 0,	 0x03,	 0x564C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK3.CLK_TIMER, 0x3	 
	 { 0,	 0x01,	 0x5547,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK2.CLK_EN_CTL, 0x1	 
	 { 0,	 0x01,	 0x5647,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK3.CLK_EN_CTL, 0x1	 
	 { 0,	 0x01,	 0x5646,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK3.EN_CTL, 0x1		# Enable Pin Control from GPIO19 	 
	 { 0,	 0x07,	 0x5043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.XO.LDO_XO_VOLTAGE_CTL2, 0x7	 
	 { 0,	 0x03,	 0x5443,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK1.EDGE_CTL1, 0x3	 
	 { 0,	 0x02,	 0x5444,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK1.DRV_CTL1, 0x02	 
	 { 0,	 0x02,	 0x5544,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK2.DRV_CTL1, 0x02	 
	 { 0,	 0x02,	 0x5644,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK3.DRV_CTL1, 0x02	 
	 { 0,	 0x80,	 0x6046,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RTC_RW.EN_CTL1, 0x80	 
	 { 0,	 0x80,	 0x5A5B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.SLEEP_CLK1.CAL_RC4, 0x80	 
	 { 0,	 0xA5,	 0x59D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.CLK_DIST.SEC_ACCESS, 0xA5	 
	 { 0,	 0x3C,	 0x5940,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.CLK_DIST.CLK_CTL1, 0x3C	 
	 { 0,	 0x01,	 0x5346,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK3.EN_CTL, 0x1		# Enable Pin Control from GPIO21 	 
	 { 0,	 0x02,	 0x5B43,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.DIV_CLK1.DIV_CTL1, 0x2		# Set to 9.6MHz for WCD9335 / WCD9340.  Want 38.4MHz/4 	 
	 { 0,	 0xA5,	 0x54D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK1.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x54E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK1.TEST2, 0x0	 
	 { 0,	 0xA5,	 0x55D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK2.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x55E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK2.TEST2, 0x0	 
	 { 0,	 0xA5,	 0x56D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK3.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x56E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK3.TEST2, 0x0	 
	 { 0,	 0xA5,	 0x51D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK1.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x51E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK1.TEST2, 0x0	 
	 { 0,	 0xA5,	 0x52D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK2.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x52E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK2.TEST2, 0x0	 
	 { 0,	 0xA5,	 0x53D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK3.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x53E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LN_BB_CLK3.TEST2, 0x0	 
	 { 0,	 0x01,	 0x2845,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.COIN.COIN_CHG_VSET, 0x1		# Set COIN to 3.2V	 
	 { 0,	 0x03,	 0x2844,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.COIN.COIN_CHG_RSET, 0x3		# Set COIN to 800ohm Rout	 
	 { 0,	 0x80,	 0x2846,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.COIN.EN_CTL, 0x80		# Enable	 
	 { 0,	 0x01,	 0x5445,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.RF_CLK1.CLKBUFF_HOLD_CTL, 0x1
	 //Mode - LDO_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x80,	 0x4E46,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO15.EN_CTL, 0x80		#	 
	 { 1,	 0x00,	 0x5848,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO25.PD_CTL, 0x0	 
	 { 1,	 0xA5,	 0x43D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x43DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x5AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x5ADA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0x02,	 0x4E47,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO15.FOLLOW_HWEN, 0x02	 
	 { 1,	 0x02,	 0x4747,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO08.FOLLOW_HWEN, 0x02	 
	 { 1,	 0x02,	 0x4A47,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO11.FOLLOW_HWEN, 0x02	 
	 { 1,	 0x07,	 0x4049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO01.MODE_CTL2, 0x7		#	 
	 { 1,	 0x07,	 0x4149,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO02.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4249,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO03.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4349,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4449,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO05.MODE_CTL2, 0x7		# Set Secondary 	 
	 { 1,	 0x07,	 0x4649,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO07.MODE_CTL2, 0x7		# Set Secondary 	 
	 { 1,	 0x07,	 0x4749,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO08.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4849,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO09.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4949,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO10.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4A49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO11.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4B49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO12.MODE_CTL2, 0x7	 
	 { 1,	 0x80,	 0x4B47,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO12.FOLLOW_HWEN, 0x80	 
	 { 1,	 0x07,	 0x4C49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO13.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4D49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO14.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4E49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO15.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x4F49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO16.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x5049,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO17.MODE_CTL2, 0x7		# Set Secondary 	 
	 { 1,	 0x07,	 0x5149,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO18.MODE_CTL2, 0x7		# Set Secondary 	 
	 { 1,	 0x07,	 0x5249,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO19.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x5349,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO20.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x5449,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO21.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x5549,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO22.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x5649,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO23.MODE_CTL2, 0x7		# Set Secondary 	 
	 { 1,	 0x07,	 0x5749,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO24.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x5849,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO25.MODE_CTL2, 0x7		# Set Secondary 	 
	 { 1,	 0x07,	 0x5949,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO26.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x5A49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.MODE_CTL2, 0x7	 
	 { 1,	 0x07,	 0x5B49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO28.MODE_CTL2, 0x7	 
	 { 1,	 0x70,	 0x4040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO01.VSET_LB, 0x70		# L1 = 0.88V	 
	 { 1,	 0x03,	 0x4041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO01.VSET_UB, 0x3		# L1 = 0.88V	 
	 { 1,	 0xB0,	 0x4140,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO02.VSET_LB, 0xB0		# L2 = 1.2V	 
	 { 1,	 0x04,	 0x4141,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO02.VSET_UB, 0x4		# L2 = 1.2V	 
	 { 1,	 0xE8,	 0x4240,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO03.VSET_LB, 0xE8		# L3 = 1V	 
	 { 1,	 0x03,	 0x4241,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO03.VSET_UB, 0x3		# L3 = 1V	 
	 { 1,	 0xA0,	 0x4340,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.VSET_LB, 0xA0		# L4 = 0.928V	 
	 { 1,	 0x03,	 0x4341,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.VSET_UB, 0x3		# L4 = 0.928V	 
	 { 1,	 0x20,	 0x4440,	 0xFF,	 PM_SBL_WRITE,	 96,	 96}, 	 //PM8998.LDO05.VSET_LB, 0x20, 0xFF, None, OPTION2_NOT_EQ_GND		# L5 = 0.8V	 
	 { 1,	 0x03,	 0x4441,	 0xFF,	 PM_SBL_WRITE,	 96,	 96}, 	 //PM8998.LDO05.VSET_UB, 0x3, 0xFF, None, OPTION2_NOT_EQ_GND		# L5 = 0.8V	 
	 { 1,	 0x70,	 0x4440,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.LDO05.VSET_LB, 0x70, 0xFF, None, OPTION2_EQ_GND		# L5 = 0.88V	 
	 { 1,	 0x03,	 0x4441,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.LDO05.VSET_UB, 0x3, 0xFF, None, OPTION2_EQ_GND		# L5 = 0.88V	 
	 { 1,	 0x08,	 0x4640,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO07.VSET_LB, 0x8		# L7 = 1.8V	 
	 { 1,	 0x07,	 0x4641,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO07.VSET_UB, 0x7		# L7 = 1.8V	 
	 { 1,	 0xB0,	 0x4740,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO08.VSET_LB, 0xB0		# L8 = 1.2V	 
	 { 1,	 0x04,	 0x4741,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO08.VSET_UB, 0x4		# L8 = 1.2V	 
	 { 1,	 0x10,	 0x4840,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO09.VSET_LB, 0x10		# L9 = 1.808V	 
	 { 1,	 0x07,	 0x4841,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO09.VSET_UB, 0x7		# L9 = 1.808V	 
	 { 1,	 0x10,	 0x4940,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO10.VSET_LB, 0x10		# L10 = 1.808V	 
	 { 1,	 0x07,	 0x4941,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO10.VSET_UB, 0x7		# L10 = 1.808V	 
	 { 1,	 0xE8,	 0x4A40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO11.VSET_LB, 0xE8		# L11 = 1V	 
	 { 1,	 0x03,	 0x4A41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO11.VSET_UB, 0x3		# L11 = 1V	 
	 { 1,	 0x08,	 0x4B40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO12.VSET_LB, 0x8		# L12 = 1.8V	 
	 { 1,	 0x07,	 0x4B41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO12.VSET_UB, 0x7		# L12 = 1.8V	 
	 { 1,	 0x90,	 0x4C40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO13.VSET_LB, 0x90		# L13 = 2.96V	 
	 { 1,	 0x0B,	 0x4C41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO13.VSET_UB, 0xB		# L13 = 2.96V	 
	 { 1,	 0x58,	 0x4D40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO14.VSET_LB, 0x58		# L14 = 1.88V	 
	 { 1,	 0x07,	 0x4D41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO14.VSET_UB, 0x7		# L14 = 1.88V	 
	 { 1,	 0x08,	 0x4E40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO15.VSET_LB, 0x8		# L15 = 1.8V	 
	 { 1,	 0x07,	 0x4E41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO15.VSET_UB, 0x7		# L15 = 1.8V	 
	 { 1,	 0x90,	 0x4F40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO16.VSET_LB, 0x90		# L16 = 2.704V	 
	 { 1,	 0x0A,	 0x4F41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO16.VSET_UB, 0xA		# L16 = 2.704V	 
	 { 1,	 0x18,	 0x5040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO17.VSET_LB, 0x18		# L17 = 1.304V	 
	 { 1,	 0x05,	 0x5041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO17.VSET_UB, 0x5		# L17 = 1.304V	 
	 { 1,	 0x90,	 0x5140,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO18.VSET_LB, 0x90		# L18 = 2.704V	 
	 { 1,	 0x0A,	 0x5141,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO18.VSET_UB, 0xA		# L18 = 2.704V	 
	 { 1,	 0xC0,	 0x5240,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO19.VSET_LB, 0xC0		# L19 = 3.008V	 
	 { 1,	 0x0B,	 0x5241,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO19.VSET_UB, 0xB		# L19 = 3.008V	 
	 { 1,	 0x90,	 0x5340,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO20.VSET_LB, 0x90		# L20 = 2.96V	 
	 { 1,	 0x0B,	 0x5341,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO20.VSET_UB, 0xB		# L20 = 2.96V	 
	 { 1,	 0x90,	 0x5440,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO21.VSET_LB, 0x90		# L21 = 2.96V	 
	 { 1,	 0x0B,	 0x5441,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO21.VSET_UB, 0xB		# L21 = 2.96V	 
	 { 1,	 0x30,	 0x5540,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO22.VSET_LB, 0x30		# L22 = 2.864V 	 
	 { 1,	 0x0B,	 0x5541,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO22.VSET_UB, 0xB		# L22 = 2.864V 	 
	 { 1,	 0xF0,	 0x5640,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO23.VSET_LB, 0xF0		# L23 = 3.312V	 
	 { 1,	 0x0C,	 0x5641,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO23.VSET_UB, 0xC		# L23 = 3.312V	 
	 { 1,	 0x10,	 0x5740,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO24.VSET_LB, 0x10		# L24 = 3.088V	 
	 { 1,	 0x0C,	 0x5741,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO24.VSET_UB, 0xC		# L24 = 3.088V	 
	 { 1,	 0x20,	 0x5840,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO25.VSET_LB, 0x20		# L25 = 3.104V 	 
	 { 1,	 0x0C,	 0x5841,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO25.VSET_UB, 0xC		# L25 = 3.104V 	 
	 { 1,	 0xB0,	 0x5940,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO26.VSET_LB, 0xB0		# L26 = 1.2V	 
	 { 1,	 0x04,	 0x5941,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO26.VSET_UB, 0x4		# L26 = 1.2V	 
	 { 1,	 0x68,	 0x5A40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.VSET_LB, 0x68		# L27 = 0.872V	 
	 { 1,	 0x03,	 0x5A41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.VSET_UB, 0x3		# L27 = 0.872V	 
	 { 1,	 0xC0,	 0x5B40,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO28.VSET_LB, 0xC0		# L28 = 3.008V	 
	 { 1,	 0x0B,	 0x5B41,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO28.VSET_UB, 0xB		# L28 = 3.008V
	 //Mode - LDO_STEPPER_RATE 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x82,	 0x4061,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO01.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x4161,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO02.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x4261,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO03.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x4361,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x4461,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO05.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x4761,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO08.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x4A61,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO11.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x5061,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO17.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x5961,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO26.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us	 
	 { 1,	 0x82,	 0x5A61,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.STEPPER_CTL, 0x82		# Stepper rate = 4.8mV/us
	 //Mode - LDO_OCP 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xD4,	 0x4066,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO01.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4166,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO02.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4266,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO03.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4366,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4466,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO05.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4666,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO07.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4766,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO08.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4866,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO09.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4966,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO10.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4A66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO11.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4B66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO12.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4C66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO13.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4D66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO14.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4E66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO15.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x4F66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO16.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5066,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO17.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5166,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO18.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5266,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO19.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5366,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO20.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5466,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO21.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5566,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO22.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5666,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO23.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5766,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO24.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5866,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO25.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5966,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO26.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5A66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.OCP_CTL1, 0xD4		#	 
	 { 1,	 0xD4,	 0x5B66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO28.OCP_CTL1, 0xD4		#
	 //Mode - LVS_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x81,	 0x804C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LVS1.SOFT_START_CTL, 0x81		# Enable 0.25uA pulldown during soft start - shortens time with 1uF	 
	 { 1,	 0x81,	 0x814C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LVS2.SOFT_START_CTL, 0x81		# Enable 0.25uA pulldown during soft start - shortens time with 1uF
	 //Mode - S8a_FTS_LUT 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x0B,	 0x2A80,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_DAC_PT_0, 0xB	 
	 { 1,	 0x08,	 0x2A81,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_DAC_PT_1, 0x8	 
	 { 1,	 0x03,	 0x2A82,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_DAC_PT_2, 0x3	 
	 { 1,	 0x3F,	 0x2A83,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_DAC_PT_3, 0x3F	 
	 { 1,	 0x02,	 0x2A87,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_ADC_PT_1, 0x2	 
	 { 1,	 0x03,	 0x2A88,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_ADC_PT_2, 0x3	 
	 { 1,	 0x3F,	 0x2A89,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_ADC_PT_3, 0x3F	 
	 { 1,	 0x41,	 0x2A8C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_SLOPE_HI_0, 0x41	 
	 { 1,	 0x20,	 0x2A8D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_SLOPE_LO_0, 0x20	 
	 { 1,	 0x45,	 0x2A8E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_SLOPE_HI_1, 0x45	 
	 { 1,	 0x00,	 0x2A8F,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_SLOPE_LO_1, 0x0	 
	 { 1,	 0x01,	 0x2A90,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_SLOPE_HI_2, 0x1	 
	 { 1,	 0x00,	 0x2A91,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.LUT_SLOPE_LO_2, 0x0
	 //Mode - S11a_FTS_LUT 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x0B,	 0x3380,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_DAC_PT_0, 0xB	 
	 { 1,	 0x08,	 0x3381,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_DAC_PT_1, 0x8	 
	 { 1,	 0x03,	 0x3382,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_DAC_PT_2, 0x3	 
	 { 1,	 0x3F,	 0x3383,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_DAC_PT_3, 0x3F	 
	 { 1,	 0x02,	 0x3387,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_ADC_PT_1, 0x2	 
	 { 1,	 0x03,	 0x3388,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_ADC_PT_2, 0x3	 
	 { 1,	 0x3F,	 0x3389,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_ADC_PT_3, 0x3F	 
	 { 1,	 0x41,	 0x338C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_SLOPE_HI_0, 0x41	 
	 { 1,	 0x20,	 0x338D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_SLOPE_LO_0, 0x20	 
	 { 1,	 0x45,	 0x338E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_SLOPE_HI_1, 0x45	 
	 { 1,	 0x00,	 0x338F,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_SLOPE_LO_1, 0x0	 
	 { 1,	 0x01,	 0x3390,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_SLOPE_HI_2, 0x1	 
	 { 1,	 0x00,	 0x3391,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.LUT_SLOPE_LO_2, 0x0
	 //Mode - S10a_FTS_LUT 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x0C,	 0x3080,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_DAC_PT_0, 0xC	 
	 { 1,	 0x09,	 0x3081,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_DAC_PT_1, 0x9	 
	 { 1,	 0x04,	 0x3082,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_DAC_PT_2, 0x4	 
	 { 1,	 0x3F,	 0x3083,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_DAC_PT_3, 0x3F	 
	 { 1,	 0x03,	 0x3087,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_ADC_PT_1, 0x3	 
	 { 1,	 0x04,	 0x3088,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_ADC_PT_2, 0x4	 
	 { 1,	 0x3F,	 0x3089,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_ADC_PT_3, 0x3F	 
	 { 1,	 0x41,	 0x308C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_SLOPE_HI_0, 0x41	 
	 { 1,	 0x00,	 0x308D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_SLOPE_LO_0, 0x0	 
	 { 1,	 0x45,	 0x308E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_SLOPE_HI_1, 0x45	 
	 { 1,	 0x00,	 0x308F,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_SLOPE_LO_1, 0x0	 
	 { 1,	 0x01,	 0x3090,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_SLOPE_HI_2, 0x1	 
	 { 1,	 0x00,	 0x3091,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.LUT_SLOPE_LO_2, 0x0
	 //Mode - S4C_FTS_LUT 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 5,	 0x0B,	 0x1E80,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_DAC_PT_0, 0xB	 
	 { 5,	 0x08,	 0x1E81,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_DAC_PT_1, 0x8	 
	 { 5,	 0x03,	 0x1E82,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_DAC_PT_2, 0x3	 
	 { 5,	 0x3F,	 0x1E83,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_DAC_PT_3, 0x3F	 
	 { 5,	 0x02,	 0x1E87,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_ADC_PT_1, 0x2	 
	 { 5,	 0x03,	 0x1E88,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_ADC_PT_2, 0x3	 
	 { 5,	 0x3F,	 0x1E89,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_ADC_PT_3, 0x3F	 
	 { 5,	 0x41,	 0x1E8C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_SLOPE_HI_0, 0x41	 
	 { 5,	 0x20,	 0x1E8D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_SLOPE_LO_0, 0x20	 
	 { 5,	 0x45,	 0x1E8E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_SLOPE_HI_1, 0x45	 
	 { 5,	 0x00,	 0x1E8F,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_SLOPE_LO_1, 0x0	 
	 { 5,	 0x01,	 0x1E90,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_SLOPE_HI_2, 0x1	 
	 { 5,	 0x00,	 0x1E91,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.LUT_SLOPE_LO_2, 0x0
	 //Mode - FTS_9a_8a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x2CDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.PERPH_RESET_CTL3, 0x01	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x29DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.PERPH_RESET_CTL3, 0x01	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x2ADA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.PERPH_RESET_CTL3, 0x01	 
	 { 1,	 0x01,	 0x2DDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.PERPH_RESET_CTL3, 0x01	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x90,	 0x2CA6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x60,	 0x2C62,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.CORR_CFG, 0x60	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x09,	 0x2C63,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.RAMP_CFG, 0x9	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x60,	 0x2C62,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.CORR_CFG, 0x60	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x99,	 0x2D96,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x2D97,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x99,	 0x2A96,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x2A97,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x40,	 0x2A51,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.MULTIPHASE_CTL, 0x40	 
	 { 1,	 0x01,	 0x2971,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.VDIP_CFG, 0x1	 
	 { 1,	 0x09,	 0x2C71,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.VDIP_CFG, 0x9	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x2971,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.VDIP_CFG, 0x1	 
	 { 1,	 0x84,	 0x2CA8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x32,	 0x2CA9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 1,	 0x01,	 0x2C54,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.PHASE_CNT_MAX, 0x1	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x2D62,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.CL_CTL_3, 0x4	 
	 { 1,	 0x0C,	 0x2A62,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.CL_CTL_3, 0xC	 
	 { 1,	 0x1D,	 0x2A60,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.CL_CTL_1, 0x1D # PWM CL = 4.77A	 
	 { 1,	 0x13,	 0x2A61,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.CL_CTL_2, 0x13 # Changed to 0x1B on SBL v0x12.    # Changed to 0x13 on SBL v14	 
	 { 1,	 0x13,	 0x2D61,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.CL_CTL_2, 0x13 # Changed to 0x1B on SBL v0x12.    # Changed to 0x13 on SBL v14	 
	 { 1,	 0xDA,	 0x2D63,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.AUTO_CTL_1, 0xDA	 
	 { 1,	 0x3F,	 0x2D75,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x3F,	 0x2A75,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x13,	 0x2D74,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.NONOVLAP_CFG, 0x13	 
	 { 1,	 0x05,	 0x2C67,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.CFG_PFM_COMP, 0x5	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x2D66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_PS.LPM_ADC_VAL, 0x5	 
	 { 1,	 0x00,	 0x29E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S8_CTRL.CAL_EN, 0x0	 
	 { 1,	 0x00,	 0x2CE3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.CAL_EN, 0x0	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x36,	 0x2C64,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.COMPENS_CFG, 0x36		# Changed from 0x12 to 0x2D on SBL vD, then 0x24 on SBLvE.  Changed to 0x36 on SBL v0x12.	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x2CAF,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.EN_OPTIONS_DFT_4, 0x0	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x02,	 0x2CAC,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.ANTI_ESL_CFG_1, 0x2  # Changed from 0x04 to 0x02 on SBL v14	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x2CAD,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.ANTI_ESL_CFG_2, 0x4  # Changed from 0x08 to 0x04 on SBL v14	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x28,	 0x2C4B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.RETENTION_CTL2, 0x28
	 //Mode - FTS_2a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x68,	 0x1740,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.VSET_LB, 0x68	 
	 { 1,	 0x04,	 0x1741,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.VSET_UB, 0x04	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x90,	 0x17A6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x60,	 0x1762,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.CORR_CFG, 0x60	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x99,	 0x1896,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x1897,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x84,	 0x17A8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x32,	 0x17A9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x06,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_PS.CL_CTL_3, 0x6  # Increased from 0x05 to 0x06 on SBL vC	 
	 { 1,	 0x8A,	 0x1863,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_PS.AUTO_CTL_1, 0x8A	 
	 { 1,	 0x05,	 0x1866,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_PS.LPM_ADC_VAL, 0x5	 
	 { 1,	 0x3F,	 0x1875,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x13,	 0x1874,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_PS.NONOVLAP_CFG, 0x13	 
	 { 1,	 0x06,	 0x1767,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.CFG_PFM_COMP, 0x6	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0B,	 0x1771,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.VDIP_CFG, 0xB	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x17E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.CAL_EN, 0x0	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x3F,	 0x1764,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.COMPENS_CFG, 0x3F	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x28,	 0x174B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.RETENTION_CTL2, 0x28
	 //Mode - FTS_7a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x27DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0x01,	 0x26DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0x10,	 0x2640,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.VSET_LB, 0x10	 
	 { 1,	 0x04,	 0x2641,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.VSET_UB, 0x04	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x90,	 0x26A6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x60,	 0x2662,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.CORR_CFG, 0x60	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0D,	 0x2663,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.RAMP_CFG, 0x0D	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x62,	 0x2664,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.COMPENS_CFG, 0x62	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x07,	 0x26AC,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.ANTI_ESL_CFG_1, 0x7	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x26AD,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.ANTI_ESL_CFG_2, 0x1	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x99,	 0x2796,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x2797,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x84,	 0x26A8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x32,	 0x26A9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x06,	 0x2762,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.CL_CTL_3, 0x6	 
	 { 1,	 0x1D,	 0x2760,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.CL_CTL_1, 0x1D # Added on SBL vC - extend PWM CL from 3.26A to 4.77A 	 
	 { 1,	 0x8A,	 0x2763,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.AUTO_CTL_1, 0x8A	 
	 { 1,	 0x06,	 0x2766,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.LPM_ADC_VAL, 0x6	 
	 { 1,	 0x3F,	 0x2775,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0A,	 0x2671,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.VDIP_CFG, 0xA	 
	 { 1,	 0x05,	 0x2667,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.CFG_PFM_COMP, 0x5	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x13,	 0x2774,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_PS.NONOVLAP_CFG, 0x13	 
	 { 1,	 0x06,	 0x2645,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.MODE_CTL1, 0x6  #	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x26E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.CAL_EN, 0x0	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x28,	 0x264B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.RETENTION_CTL2, 0x28	 
	 { 1,	 0x01,	 0x2665,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.CFG_VREG_MON, 0x01   # New for SBL v10 - keep VREG_OK on all the time 
	 //Mode - FTS_1a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x15DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0x01,	 0x14DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x90,	 0x14A6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x99,	 0x1596,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x1597,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x84,	 0x14A8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x32,	 0x14A9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0A,	 0x1471,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.VDIP_CFG, 0xA    # Changed from 0x09 to 0x0A on SBL vE	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.CL_CTL_3, 0x05     # Changed from 0x04 to 0x05 on SBL vD	 
	 { 1,	 0x13,	 0x1560,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.CL_CTL_1, 0x13     # PWM CL 	 
	 { 1,	 0x13,	 0x1561,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.CL_CTL_2, 0x13     # HCPFM CL 	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0xDA,	 0x1563,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.AUTO_CTL_1, 0xDA	 
	 { 1,	 0x3F,	 0x1575,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x13,	 0x1574,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.NONOVLAP_CFG, 0x13	 
	 { 1,	 0x06,	 0x1467,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.CFG_PFM_COMP, 0x6	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x1566,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_PS.LPM_ADC_VAL, 0x5	 
	 { 1,	 0x00,	 0x14E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.CAL_EN, 0x0	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x60,	 0x1462,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.CORR_CFG, 0x60	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0A,	 0x1463,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.RAMP_CFG, 0xA	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x12,	 0x1464,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.COMPENS_CFG, 0x12	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x28,	 0x144B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.RETENTION_CTL2, 0x28	 
	 { 1,	 0x08,	 0x1448,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.PD_CTL, 0x08
	 //Mode - FTS_6a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x24DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0x01,	 0x23DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5     # Still ganged to S1	 
	 { 1,	 0x00,	 0x25C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_FREQ.GANG_CTL2, 0x00	 
	 { 1,	 0xA5,	 0x25D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x23,	 0x25C0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_FREQ.GANG_CTL1, 0x23     # Set Gang Leader PID to S6_CTRL	 
	 { 1,	 0xA5,	 0x25D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_FREQ.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x25C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_FREQ.GANG_CTL2, 0x80	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x90,	 0x23A6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x99,	 0x2496,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x2497,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x84,	 0x23A8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x32,	 0x23A9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0A,	 0x2371,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.VDIP_CFG, 0x0A   # Updated from 0x9 to 0xA on SBLv0x0A	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x2462,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.CL_CTL_3, 0x5	 
	 { 1,	 0x13,	 0x2461,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.CL_CTL_2, 0x13	 
	 { 1,	 0xFA,	 0x2463,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.AUTO_CTL_1, 0xFA	 
	 { 1,	 0x3F,	 0x2475,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x06,	 0x2367,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.CFG_PFM_COMP, 0x6	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x2466,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.LPM_ADC_VAL, 0x5	 
	 { 1,	 0x13,	 0x2474,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_PS.NONOVLAP_CFG, 0x13	 
	 { 1,	 0x00,	 0x23E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.CAL_EN, 0x0	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x60,	 0x2362,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.CORR_CFG, 0x60	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0A,	 0x2363,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.RAMP_CFG, 0xA	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x12,	 0x2364,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.COMPENS_CFG, 0x12    # Changed from 0x1B to 0x12 on SBL v3	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x28,	 0x234B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.RETENTION_CTL2, 0x28	 
	 { 1,	 0x88,	 0x2348,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.PD_CTL, 0x88
	 //Mode - FTS_13a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x39DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0x01,	 0x38DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x90,	 0x38A6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x99,	 0x3996,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x3997,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x84,	 0x38A8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x32,	 0x38A9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x09,	 0x3871,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.VDIP_CFG, 0x9	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x3962,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.CL_CTL_3, 0x4	 
	 { 1,	 0x13,	 0x3961,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.CL_CTL_2, 0x13    # HCPFM CL 	 
	 { 1,	 0xDA,	 0x3963,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.AUTO_CTL_1, 0xDA	 
	 { 1,	 0x04,	 0x3966,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.LPM_ADC_VAL, 0x4	 
	 { 1,	 0x3F,	 0x3975,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x13,	 0x3974,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_PS.NONOVLAP_CFG, 0x13	 
	 { 1,	 0x06,	 0x3867,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.CFG_PFM_COMP, 0x6	 
	 { 1,	 0x06,	 0x3845,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.MODE_CTL1, 0x6	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x38E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.CAL_EN, 0x0	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x40,	 0x3862,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.CORR_CFG, 0x40  # Changed from 0x60 to 0x40 on SBL v0x03	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0A,	 0x3863,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.RAMP_CFG, 0xA	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x36,	 0x3864,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.COMPENS_CFG, 0x36   # Changed from 0x12 to 0x1A on SBL v0x03.  Changed to 0x36 on SBL v0x12.	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x38AC,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.ANTI_ESL_CFG_1, 0x04   # Changed from 0x00 to 0x04 on SBL v0x03.  Changed to 0x06 on SBL vE.  Changed to 0x04 on SBL v0x12.	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x08,	 0x38AD,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.ANTI_ESL_CFG_2, 0x08   # Changed from 0x00 to 0x08 on SBL v0x03	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x28,	 0x384B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.RETENTION_CTL2, 0x28
	 //Mode - FTS_12a_11a_10a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0B,	 0x35DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.PERPH_RESET_CTL3, 0xB	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0B,	 0x32DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.PERPH_RESET_CTL3, 0xB	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0B,	 0x2FDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.PERPH_RESET_CTL3, 0xB	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0B,	 0x30DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.PERPH_RESET_CTL3, 0xB	 
	 { 1,	 0x0B,	 0x33DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.PERPH_RESET_CTL3, 0xB	 
	 { 1,	 0x0B,	 0x36DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_PS.PERPH_RESET_CTL3, 0xB	 
	 { 1,	 0x3C,	 0x3540,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.VSET_LB, 0x3C	 
	 { 1,	 0x03,	 0x3541,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.VSET_UB, 0x03	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x90,	 0x35A6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x99,	 0x3696,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x3697,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x99,	 0x3396,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x3397,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x99,	 0x3096,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.MPHASE_PI_CFG1, 0x99	 
	 { 1,	 0x93,	 0x3097,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.MPHASE_PI_CFG4, 0x93	 
	 { 1,	 0x40,	 0x3351,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.MULTIPHASE_CTL, 0x40	 
	 { 1,	 0x40,	 0x3051,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.MULTIPHASE_CTL, 0x40	 
	 { 1,	 0x01,	 0x3271,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.VDIP_CFG, 0x1	 
	 { 1,	 0x01,	 0x2F71,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.VDIP_CFG, 0x1	 
	 { 1,	 0x84,	 0x35A8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x32,	 0x35A9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x09,	 0x3571,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.VDIP_CFG, 0x9	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x3662,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_PS.CL_CTL_3, 0x05    # Changed from 0x04 to 0x05 on SBL vE	 
	 { 1,	 0x0C,	 0x3362,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.CL_CTL_3, 0xC	 
	 { 1,	 0x0C,	 0x3062,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.CL_CTL_3, 0xC	 
	 { 1,	 0x1D,	 0x3060,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.CL_CTL_1, 0x1D # PWM CL = 4.77A	 
	 { 1,	 0x1D,	 0x3061,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.CL_CTL_2, 0x1D # HCPFM CL = 4.77A	 
	 { 1,	 0xDA,	 0x3663,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_PS.AUTO_CTL_1, 0xDA	 
	 { 1,	 0x04,	 0x3666,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_PS.LPM_ADC_VAL, 0x4	 
	 { 1,	 0x3F,	 0x3675,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x3F,	 0x3375,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x3F,	 0x3075,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_PS.ADC_FRONT_CFG, 0x3F	 
	 { 1,	 0x13,	 0x3674,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_PS.NONOVLAP_CFG, 0x13	 
	 { 1,	 0x06,	 0x3567,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.CFG_PFM_COMP, 0x6	 
	 { 1,	 0x06,	 0x3545,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.MODE_CTL1, 0x6	 
	 { 1,	 0x02,	 0x3554,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.PHASE_CNT_MAX, 0x2	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x35AC,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.ANTI_ESL_CFG_1, 0x4	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x08,	 0x35AD,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.ANTI_ESL_CFG_2, 0x8	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x2FE3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S10_CTRL.CAL_EN, 0x0	 
	 { 1,	 0x00,	 0x32E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S11_CTRL.CAL_EN, 0x0	 
	 { 1,	 0x00,	 0x35E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.CAL_EN, 0x0	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x1A,	 0x3564,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.COMPENS_CFG, 0x1A	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x09,	 0x3563,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.RAMP_CFG, 0x09   # Changed from 0x0A to 0x09 on SBL vD	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x40,	 0x3562,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.CORR_CFG, 0x40	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x28,	 0x354B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.RETENTION_CTL2, 0x28
	 //Mode - FTS_1c_4c_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x0B,	 0x14DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.PERPH_RESET_CTL3, 0x0B	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x0B,	 0x1DDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_CTRL.PERPH_RESET_CTL3, 0x0B	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x0B,	 0x1EDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.PERPH_RESET_CTL3, 0x0B	 
	 { 5,	 0x0B,	 0x15DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_PS.PERPH_RESET_CTL3, 0x0B	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x90,	 0x14A6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x99,	 0x1596,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_PS.MPHASE_PI_CFG1, 0x99	 
	 { 5,	 0x93,	 0x1597,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_PS.MPHASE_PI_CFG4, 0x93	 
	 { 5,	 0x99,	 0x1E96,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.MPHASE_PI_CFG1, 0x99	 
	 { 5,	 0x93,	 0x1E97,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.MPHASE_PI_CFG4, 0x93	 
	 { 5,	 0x40,	 0x1E51,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.MULTIPHASE_CTL, 0x40	 
	 { 5,	 0x01,	 0x1D71,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_CTRL.VDIP_CFG, 0x1	 
	 { 5,	 0x09,	 0x1471,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.VDIP_CFG, 0x9	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x84,	 0x14A8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x32,	 0x14A9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x04,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_PS.CL_CTL_3, 0x4	 
	 { 5,	 0x0C,	 0x1E62,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.CL_CTL_3, 0xC	 
	 { 5,	 0xDA,	 0x1563,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_PS.AUTO_CTL_1, 0xDA	 
	 { 5,	 0x05,	 0x1566,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_PS.LPM_ADC_VAL, 0x5	 
	 { 5,	 0x3F,	 0x1575,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_PS.ADC_FRONT_CFG, 0x3F	 
	 { 5,	 0x3F,	 0x1E75,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_PS.ADC_FRONT_CFG, 0x3F	 
	 { 5,	 0x13,	 0x1574,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_PS.NONOVLAP_CFG, 0x13	 
	 { 5,	 0x06,	 0x1467,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.CFG_PFM_COMP, 0x6	 
	 { 5,	 0x06,	 0x1445,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.MODE_CTL1, 0x6	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x07,	 0x14AC,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.ANTI_ESL_CFG_1, 0x7	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x03,	 0x14AD,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.ANTI_ESL_CFG_2, 0x3	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x01,	 0x1454,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.PHASE_CNT_MAX, 0x1	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x00,	 0x1DE3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S4_CTRL.CAL_EN, 0x0	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x00,	 0x14E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.CAL_EN, 0x0	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x0A,	 0x1463,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.RAMP_CFG, 0xA	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x7A,	 0x1464,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.COMPENS_CFG, 0x7A	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x28,	 0x144B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.RETENTION_CTL2, 0x28
	 //Mode - FTS_2c_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x01,	 0x18DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.PERPH_RESET_CTL3, 0x1	 
	 { 5,	 0x01,	 0x17DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.PERPH_RESET_CTL3, 0x1	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x90,	 0x17A6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x99,	 0x1896,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.MPHASE_PI_CFG1, 0x99	 
	 { 5,	 0x93,	 0x1897,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.MPHASE_PI_CFG4, 0x93	 
	 { 5,	 0x84,	 0x17A8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x32,	 0x17A9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x09,	 0x1771,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.VDIP_CFG, 0x9	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x14,	 0x1860,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.CL_CTL_1, 0x14 # Extended from 0x13 	 
	 { 5,	 0x04,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.CL_CTL_3, 0x4	 
	 { 5,	 0xDA,	 0x1863,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.AUTO_CTL_1, 0xDA	 
	 { 5,	 0x05,	 0x1866,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.LPM_ADC_VAL, 0x5	 
	 { 5,	 0x3F,	 0x1875,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.ADC_FRONT_CFG, 0x3F	 
	 { 5,	 0x13,	 0x1874,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_PS.NONOVLAP_CFG, 0x13	 
	 { 5,	 0x06,	 0x1745,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.MODE_CTL1, 0x6	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x07,	 0x17AC,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.ANTI_ESL_CFG_1, 0x7	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x02,	 0x17AD,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.ANTI_ESL_CFG_2, 0x2	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x00,	 0x17E3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.CAL_EN, 0x0	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x0A,	 0x1763,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.RAMP_CFG, 0xA	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x7A,	 0x1764,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.COMPENS_CFG, 0x7A	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x28,	 0x174B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.RETENTION_CTL2, 0x28
	 //Mode - FTS_3c_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x90,	 0x1AA6,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.EN_OPTIONS_DFT_2, 0x90	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x99,	 0x1B96,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_PS.MPHASE_PI_CFG1, 0x99	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x93,	 0x1B97,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_PS.MPHASE_PI_CFG4, 0x93	 
	 { 5,	 0x84,	 0x1AA8,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.EN_OPTIONS_DFT_1_SHADOW, 0x84	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x0A,	 0x1B61,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_PS.CL_CTL_2, 0xA	 
	 { 5,	 0x04,	 0x1B62,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_PS.CL_CTL_3, 0x4	 
	 { 5,	 0x8A,	 0x1B63,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_PS.AUTO_CTL_1, 0x8A	 
	 { 5,	 0x3F,	 0x1B75,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_PS.ADC_FRONT_CFG, 0x3F	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x0D,	 0x1A71,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.VDIP_CFG, 0xD	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x04,	 0x1B66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_PS.LPM_ADC_VAL, 0x4	 
	 { 5,	 0x13,	 0x1B74,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_PS.NONOVLAP_CFG, 0x13	 
	 { 5,	 0x32,	 0x1AA9,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.EN_OPTIONS_DFT_2_SHADOW, 0x32	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x01,	 0x1AAC,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.ANTI_ESL_CFG_1, 0x1	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x01,	 0x1AAD,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.ANTI_ESL_CFG_2, 0x1	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x0A,	 0x1A63,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.RAMP_CFG, 0xA	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x00,	 0x1AE3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.CAL_EN, 0x0	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x3A,	 0x1A64,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.COMPENS_CFG, 0x3A	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x05,	 0x1A67,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.CFG_PFM_COMP, 0x5	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x28,	 0x1A4B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.RETENTION_CTL2, 0x28
	 //Mode - FTS_OCP 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x4C,	 0x2C66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x4C,	 0x1766,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x4C,	 0x2666,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x4C,	 0x1466,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x4C,	 0x2366,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x4C,	 0x3866,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x4C,	 0x3566,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x4C,	 0x1466,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x4C,	 0x1766,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.CFG_VREG_OCP, 0x4C	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x4C,	 0x1A66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.CFG_VREG_OCP, 0x4C
	 //Mode - FTS_PSEUDO_TRIM_PM845 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CFG, 0x80		# Assert RIF_MEM_ACCESS_EN	 
	 { 0,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CTL, 0x0		#	 
	 { 0,	 0xBC,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xBC		#	 
	 { 0,	 0x03,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x3		#	 
	 { 1,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 13,	 13}, 	 //PM8998.S1_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_5	 
	 { 1,	 0x05,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 14,	 14}, 	 //PM8998.S1_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_6	 
	 { 1,	 0x06,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 15,	 15}, 	 //PM8998.S1_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_7	 
	 { 1,	 0x07,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 16,	 16}, 	 //PM8998.S1_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_8	 
	 { 1,	 0x08,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 17,	 17}, 	 //PM8998.S1_PS.CL_CTL_3, 0x8, 0xFF, None, Clogic_9	 
	 { 1,	 0x04,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 18,	 18}, 	 //PM8998.S1_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_10	 
	 { 1,	 0x03,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 19,	 19}, 	 //PM8998.S1_PS.CL_CTL_3, 0x3, 0xFF, None, Clogic_11	 
	 { 1,	 0x00,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.SEC_ACCESS, 0x0	 
	 { 1,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x06,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 20,	 20}, 	 //PM8998.S2_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_12	 
	 { 1,	 0x06,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 21,	 21}, 	 //PM8998.S2_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_12p5	 
	 { 1,	 0x07,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 22,	 22}, 	 //PM8998.S2_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_13	 
	 { 1,	 0x08,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 23,	 23}, 	 //PM8998.S2_PS.CL_CTL_3, 0x8, 0xFF, None, Clogic_14	 
	 { 1,	 0x09,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 24,	 24}, 	 //PM8998.S2_PS.CL_CTL_3, 0x9, 0xFF, None, Clogic_15	 
	 { 1,	 0x05,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 25,	 25}, 	 //PM8998.S2_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_16	 
	 { 1,	 0x04,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 26,	 26}, 	 //PM8998.S2_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_17	 
	 { 1,	 0x00,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.SEC_ACCESS, 0x0	 
	 { 1,	 0xA5,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x2462,	 0xFF,	 PM_SBL_WRITE,	 34,	 34}, 	 //PM8998.S6_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_25	 
	 { 1,	 0x05,	 0x2462,	 0xFF,	 PM_SBL_WRITE,	 35,	 35}, 	 //PM8998.S6_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_26	 
	 { 1,	 0x06,	 0x2462,	 0xFF,	 PM_SBL_WRITE,	 36,	 36}, 	 //PM8998.S6_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_27	 
	 { 1,	 0x07,	 0x2462,	 0xFF,	 PM_SBL_WRITE,	 37,	 37}, 	 //PM8998.S6_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_28	 
	 { 1,	 0x08,	 0x2462,	 0xFF,	 PM_SBL_WRITE,	 38,	 38}, 	 //PM8998.S6_PS.CL_CTL_3, 0x8, 0xFF, None, Clogic_29	 
	 { 1,	 0x04,	 0x2462,	 0xFF,	 PM_SBL_WRITE,	 39,	 39}, 	 //PM8998.S6_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_30	 
	 { 1,	 0x03,	 0x2462,	 0xFF,	 PM_SBL_WRITE,	 40,	 40}, 	 //PM8998.S6_PS.CL_CTL_3, 0x3, 0xFF, None, Clogic_31	 
	 { 1,	 0x00,	 0x23D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.SEC_ACCESS, 0x0	 
	 { 1,	 0xA5,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x06,	 0x2762,	 0xFF,	 PM_SBL_WRITE,	 27,	 27}, 	 //PM8998.S7_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_18	 
	 { 1,	 0x06,	 0x2762,	 0xFF,	 PM_SBL_WRITE,	 28,	 28}, 	 //PM8998.S7_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_19	 
	 { 1,	 0x07,	 0x2762,	 0xFF,	 PM_SBL_WRITE,	 29,	 29}, 	 //PM8998.S7_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_20	 
	 { 1,	 0x08,	 0x2762,	 0xFF,	 PM_SBL_WRITE,	 30,	 30}, 	 //PM8998.S7_PS.CL_CTL_3, 0x8, 0xFF, None, Clogic_21	 
	 { 1,	 0x09,	 0x2762,	 0xFF,	 PM_SBL_WRITE,	 31,	 31}, 	 //PM8998.S7_PS.CL_CTL_3, 0x9, 0xFF, None, Clogic_22	 
	 { 1,	 0x05,	 0x2762,	 0xFF,	 PM_SBL_WRITE,	 32,	 32}, 	 //PM8998.S7_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_23	 
	 { 1,	 0x04,	 0x2762,	 0xFF,	 PM_SBL_WRITE,	 33,	 33}, 	 //PM8998.S7_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_24	 
	 { 1,	 0x00,	 0x26D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.SEC_ACCESS, 0x0	 
	 { 1,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x2D62,	 0xFF,	 PM_SBL_WRITE,	 41,	 41}, 	 //PM8998.S9_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_32	 
	 { 1,	 0x04,	 0x2D62,	 0xFF,	 PM_SBL_WRITE,	 42,	 42}, 	 //PM8998.S9_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_32p5	 
	 { 1,	 0x05,	 0x2D62,	 0xFF,	 PM_SBL_WRITE,	 43,	 43}, 	 //PM8998.S9_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_33	 
	 { 1,	 0x06,	 0x2D62,	 0xFF,	 PM_SBL_WRITE,	 44,	 44}, 	 //PM8998.S9_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_34	 
	 { 1,	 0x07,	 0x2D62,	 0xFF,	 PM_SBL_WRITE,	 45,	 45}, 	 //PM8998.S9_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_35	 
	 { 1,	 0x03,	 0x2D62,	 0xFF,	 PM_SBL_WRITE,	 46,	 46}, 	 //PM8998.S9_PS.CL_CTL_3, 0x3, 0xFF, None, Clogic_36	 
	 { 1,	 0x02,	 0x2D62,	 0xFF,	 PM_SBL_WRITE,	 47,	 47}, 	 //PM8998.S9_PS.CL_CTL_3, 0x2, 0xFF, None, Clogic_37	 
	 { 1,	 0x00,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.SEC_ACCESS, 0x0	 
	 { 0,	 0xC0,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0xC0		#	 
	 { 0,	 0x03,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x3	 
	 { 1,	 0xA5,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x05,	 0x3662,	 0xFF,	 PM_SBL_WRITE,	 48,	 48}, 	 //PM8998.S12_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_38	 
	 { 1,	 0x05,	 0x3662,	 0xFF,	 PM_SBL_WRITE,	 49,	 49}, 	 //PM8998.S12_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_39	 
	 { 1,	 0x06,	 0x3662,	 0xFF,	 PM_SBL_WRITE,	 50,	 50}, 	 //PM8998.S12_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_40	 
	 { 1,	 0x07,	 0x3662,	 0xFF,	 PM_SBL_WRITE,	 51,	 51}, 	 //PM8998.S12_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_41	 
	 { 1,	 0x08,	 0x3662,	 0xFF,	 PM_SBL_WRITE,	 52,	 52}, 	 //PM8998.S12_PS.CL_CTL_3, 0x8, 0xFF, None, Clogic_42	 
	 { 1,	 0x04,	 0x3662,	 0xFF,	 PM_SBL_WRITE,	 53,	 53}, 	 //PM8998.S12_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_43	 
	 { 1,	 0x03,	 0x3662,	 0xFF,	 PM_SBL_WRITE,	 54,	 54}, 	 //PM8998.S12_PS.CL_CTL_3, 0x3, 0xFF, None, Clogic_44	 
	 { 1,	 0x00,	 0x35D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.SEC_ACCESS, 0x0	 
	 { 1,	 0xA5,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x3962,	 0xFF,	 PM_SBL_WRITE,	 27,	 27}, 	 //PM8998.S13_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_18	 
	 { 1,	 0x04,	 0x3962,	 0xFF,	 PM_SBL_WRITE,	 28,	 28}, 	 //PM8998.S13_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_19	 
	 { 1,	 0x05,	 0x3962,	 0xFF,	 PM_SBL_WRITE,	 29,	 29}, 	 //PM8998.S13_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_20	 
	 { 1,	 0x06,	 0x3962,	 0xFF,	 PM_SBL_WRITE,	 30,	 30}, 	 //PM8998.S13_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_21	 
	 { 1,	 0x07,	 0x3962,	 0xFF,	 PM_SBL_WRITE,	 31,	 31}, 	 //PM8998.S13_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_22	 
	 { 1,	 0x03,	 0x3962,	 0xFF,	 PM_SBL_WRITE,	 32,	 32}, 	 //PM8998.S13_PS.CL_CTL_3, 0x3, 0xFF, None, Clogic_23	 
	 { 1,	 0x02,	 0x3962,	 0xFF,	 PM_SBL_WRITE,	 33,	 33}, 	 //PM8998.S13_PS.CL_CTL_3, 0x2, 0xFF, None, Clogic_24	 
	 { 1,	 0x00,	 0x38D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.SEC_ACCESS, 0x0	 
	 { 0,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CFG, 0x0		# De-assert RIF_MEM_ACCESS_EN	 
	 { 0,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_CTL, 0x0		# De-assert BURST and WR_EN	 
	 { 0,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Clear address LSB	 
	 { 0,	 0x00,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0		# Clear address MSB
	 //Mode - FTS_PSEUDO_TRIM_PM8005 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 4,	 0x80,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CFG, 0x80		# Assert RIF_MEM_ACCESS_EN	 
	 { 4,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CTL, 0x0		#	 
	 { 4,	 0xCC,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_LSB, 0xCC		#	 
	 { 4,	 0x01,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_MSB, 0x1		#	 
	 { 5,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x04,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 55,	 55}, 	 //PM8005.S1_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_45	 
	 { 5,	 0x04,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 56,	 56}, 	 //PM8005.S1_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_46	 
	 { 5,	 0x05,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 57,	 57}, 	 //PM8005.S1_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_47	 
	 { 5,	 0x06,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 58,	 58}, 	 //PM8005.S1_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_48	 
	 { 5,	 0x07,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 59,	 59}, 	 //PM8005.S1_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_49	 
	 { 5,	 0x03,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 60,	 60}, 	 //PM8005.S1_PS.CL_CTL_3, 0x3, 0xFF, None, Clogic_50	 
	 { 5,	 0x02,	 0x1562,	 0xFF,	 PM_SBL_WRITE,	 61,	 61}, 	 //PM8005.S1_PS.CL_CTL_3, 0x2, 0xFF, None, Clogic_51	 
	 { 5,	 0x00,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.SEC_ACCESS, 0x0	 
	 { 5,	 0xA5,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x04,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 62,	 62}, 	 //PM8005.S2_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_52	 
	 { 5,	 0x04,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 63,	 63}, 	 //PM8005.S2_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_52p5	 
	 { 5,	 0x05,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 64,	 64}, 	 //PM8005.S2_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_53	 
	 { 5,	 0x06,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 65,	 65}, 	 //PM8005.S2_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_54	 
	 { 5,	 0x07,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 66,	 66}, 	 //PM8005.S2_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_55	 
	 { 5,	 0x03,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 67,	 67}, 	 //PM8005.S2_PS.CL_CTL_3, 0x3, 0xFF, None, Clogic_56	 
	 { 5,	 0x02,	 0x1862,	 0xFF,	 PM_SBL_WRITE,	 68,	 68}, 	 //PM8005.S2_PS.CL_CTL_3, 0x2, 0xFF, None, Clogic_57	 
	 { 5,	 0x00,	 0x17D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.SEC_ACCESS, 0x0	 
	 { 5,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 5,	 0x04,	 0x1B62,	 0xFF,	 PM_SBL_WRITE,	 69,	 69}, 	 //PM8005.S3_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_58	 
	 { 5,	 0x04,	 0x1B62,	 0xFF,	 PM_SBL_WRITE,	 70,	 70}, 	 //PM8005.S3_PS.CL_CTL_3, 0x4, 0xFF, None, Clogic_59	 
	 { 5,	 0x05,	 0x1B62,	 0xFF,	 PM_SBL_WRITE,	 71,	 71}, 	 //PM8005.S3_PS.CL_CTL_3, 0x5, 0xFF, None, Clogic_60	 
	 { 5,	 0x06,	 0x1B62,	 0xFF,	 PM_SBL_WRITE,	 72,	 72}, 	 //PM8005.S3_PS.CL_CTL_3, 0x6, 0xFF, None, Clogic_61	 
	 { 5,	 0x07,	 0x1B62,	 0xFF,	 PM_SBL_WRITE,	 73,	 73}, 	 //PM8005.S3_PS.CL_CTL_3, 0x7, 0xFF, None, Clogic_62	 
	 { 5,	 0x03,	 0x1B62,	 0xFF,	 PM_SBL_WRITE,	 74,	 74}, 	 //PM8005.S3_PS.CL_CTL_3, 0x3, 0xFF, None, Clogic_63	 
	 { 5,	 0x02,	 0x1B62,	 0xFF,	 PM_SBL_WRITE,	 75,	 75}, 	 //PM8005.S3_PS.CL_CTL_3, 0x2, 0xFF, None, Clogic_64	 
	 { 5,	 0x00,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.SEC_ACCESS, 0x0	 
	 { 4,	 0x00,	 0x7040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CFG, 0x0		# De-assert RIF_MEM_ACCESS_EN	 
	 { 4,	 0x00,	 0x7041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_CTL, 0x0		# De-assert BURST and WR_EN	 
	 { 4,	 0x00,	 0x7042,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_LSB, 0x0		# Clear address LSB	 
	 { 4,	 0x00,	 0x7043,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.PBS_CORE.MEM_INTF_ADDR_MSB, 0x0		# Clear address MSB
	 //Mode - HFS_S3a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x1BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x1BDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x1CDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_FREQ.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x1BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0xA7,	 0x1B4A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.PWM_CURRENT_LIM_CTL, 0xA7	 
	 { 1,	 0xA5,	 0x1BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x88,	 0x1B4B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.PFM_CURRENT_LIM_CTL, 0x88	 
	 { 1,	 0xA5,	 0x1BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0xFE,	 0x1B70,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SWITCH_SIZE_CTL, 0xFE	 
	 { 1,	 0xA5,	 0x1BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x1B72,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.DEAD_TIME_CTRL, 0x0	 
	 { 1,	 0xA5,	 0x1BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x03,	 0x1B73,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.BLNK_TIME_CTL, 0x3	 
	 { 1,	 0x06,	 0x1B7E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.PWR_STG_MISC, 0x6	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x1A59,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.PULSE_SKIP_CTL, 0x80	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x02,	 0x1A5A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.PS_THRES_ERRAMP, 0x2	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0xA6,	 0x1A5C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.INZERO_COUNT_CTL, 0xA6	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x08,	 0x1A5D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.PFM_COUNT_CTL, 0x8	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x30,	 0x1A5E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.PS_THRES_ILOAD, 0x30	 
	 { 1,	 0xC5,	 0x1A44,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S3_CTRL.PFM_CTL, 0xC5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xC6,	 0x1A44,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S3_CTRL.PFM_CTL, 0xC6, 0xFF, None,  OPTION2_EQ_GND	 
	 { 1,	 0xC6,	 0x1A44,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S3_CTRL.PFM_CTL, 0xC6, 0xFF, None,  OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0E,	 0x1A5B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.AUTO_MODE_CTL, 0x0E  #Changed from 0xEF on SBL v4	 
	 { 1,	 0x38,	 0x1A7E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.CTLR_MISC, 0x38	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x42,	 0x1A63,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.QM_PS_EXIT, 0x42	 
	 { 1,	 0x07,	 0x1B45,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.PFM_FREQ_CFG, 0x7	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x1A54,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.BUCK_SNS_CTL, 0x1	 
	 { 1,	 0xA5,	 0x1BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x32,	 0x1B5A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.ISNS_THRES, 0x32	 
	 { 1,	 0xA5,	 0x1BD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x02,	 0x1B63,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_PS.QM_PS_ENTRY, 0x2	 
	 { 1,	 0x06,	 0x1A45,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.MODE_CTL1, 0x6	 
	 { 1,	 0x00,	 0x1A48,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.PD_CTL, 0x0	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x82,	 0x1A61,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.STEPPER_VS_CTL, 0x82
	 //Mode - HFS_S4a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x1DDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x1ED0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x1EDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x1FDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_FREQ.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x1ED0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x48,	 0x1E4A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.PWM_CURRENT_LIM_CTL, 0x48  # Changed from 0x47 on SBL v5	 
	 { 1,	 0xA5,	 0x1ED0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x87,	 0x1E4B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.PFM_CURRENT_LIM_CTL, 0x87	 
	 { 1,	 0xA5,	 0x1ED0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0xFE,	 0x1E70,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.SWITCH_SIZE_CTL, 0xFE	 
	 { 1,	 0xA5,	 0x1ED0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x1E72,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.DEAD_TIME_CTRL, 0x0	 
	 { 1,	 0x0A,	 0x1E7E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.PWR_STG_MISC, 0xA	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x1D59,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.PULSE_SKIP_CTL, 0x80	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0xA7,	 0x1D5C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.INZERO_COUNT_CTL, 0xA7	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x07,	 0x1D5D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.PFM_COUNT_CTL, 0x7	 
	 { 1,	 0xC5,	 0x1D44,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.PFM_CTL, 0xC5	 
	 { 1,	 0x32,	 0x1D7E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.CTLR_MISC, 0x32	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x62,	 0x1D63,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.QM_PS_EXIT, 0x62	 
	 { 1,	 0x07,	 0x1E45,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.PFM_FREQ_CFG, 0x7	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x1D54,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.BUCK_SNS_CTL, 0x0	 
	 { 1,	 0xA5,	 0x1ED0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0xDC,	 0x1E5A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.ISNS_THRES, 0xDC	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x1D58,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.DMAX_CTL, 0x1	 
	 { 1,	 0xA5,	 0x1ED0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0D,	 0x1E63,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_PS.QM_PS_ENTRY, 0xD	 
	 { 1,	 0x80,	 0x1D48,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.PD_CTL, 0x80	 
	 { 1,	 0x05,	 0x1D49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.MODE_CTL2, 0x5	 
	 { 1,	 0x40,	 0x1D68,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.ULS_VSET_LB, 0x40	 
	 { 1,	 0x07,	 0x1D69,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.ULS_VSET_UB, 0x7	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x82,	 0x1D61,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.STEPPER_VS_CTL, 0x82	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0F,	 0x1D5B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.AUTO_MODE_CTL, 0xF	 
	 { 1,	 0x05,	 0x1D45,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.MODE_CTL1, 0x5	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x48,	 0x1D51,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.COMP_CAP_C1_C2_CTL, 0x48	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x0C,	 0x1D52,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.COMP_CAP_C3_CTL, 0xC	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x40,	 0x1D53,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.R1_R2_CTL, 0x40	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x1D5A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.PS_THRES_ERRAMP, 0x00    # Changed from 0x02 on SBL v5	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0xD0,	 0x1D5E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.PS_THRES_ILOAD, 0xD0  # Changed from 0x00 on SBL v5	 
	 { 0,	 0x99,	 0x0032,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //50	 
	 { 1,	 0x07,	 0x1D45,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.MODE_CTL1, 0x7
	 //Mode - HFS_S5a_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x21DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x22DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_FREQ.PERPH_RESET_CTL3, 0x1	 
	 { 1,	 0x05,	 0x2045,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.MODE_CTL1, 0x5	 
	 { 0,	 0x99,	 0x0032,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //50	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0x04,	 0x2051,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.COMP_CAP_C1_C2_CTL, 0x04, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0x05,	 0x2052,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.COMP_CAP_C3_CTL, 0x05, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0x60,	 0x2053,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.R1_R2_CTL, 0x60, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0x30,	 0x205E,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.PS_THRES_ILOAD, 0x30, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0x02,	 0x205A,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.PS_THRES_ERRAMP, 0x02, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xEC,	 0x2051,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.COMP_CAP_C1_C2_CTL, 0xEC, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0x15,	 0x2052,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.COMP_CAP_C3_CTL, 0x15, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0x20,	 0x2053,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.R1_R2_CTL, 0x20, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0x00,	 0x205E,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.PS_THRES_ILOAD, 0x00, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0x03,	 0x205A,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.PS_THRES_ERRAMP, 0x03, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xEC,	 0x2051,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.COMP_CAP_C1_C2_CTL, 0xEC, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0x15,	 0x2052,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.COMP_CAP_C3_CTL, 0x15, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0x20,	 0x2053,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.R1_R2_CTL, 0x20, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0x00,	 0x205E,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.PS_THRES_ILOAD, 0x00, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0x03,	 0x205A,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.PS_THRES_ERRAMP, 0x03, 0xFF, None, OPTION2_EQ_VPH	 
	 { 0,	 0x99,	 0x0032,	 0xFF,	 PM_SBL_DELAY,	 0,	 0}, 	 //50	 
	 { 1,	 0x07,	 0x2045,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.MODE_CTL1, 0x7	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xF5,	 0x214A,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_PS.PWM_CURRENT_LIM_CTL, 0xF5, 0xFF, None, OPTION2_EQ_HIGHZ  # Changed from 0xD5 on SBL v5	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0x8D,	 0x214B,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_PS.PFM_CURRENT_LIM_CTL, 0x8D, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0x0F,	 0x205B,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.AUTO_MODE_CTL, 0xF, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0x10,	 0x205D,	 0xFF,	 PM_SBL_WRITE,	 97,	 97}, 	 //PM8998.S5_CTRL.PFM_COUNT_CTL, 0x10, 0xFF, None, OPTION2_EQ_HIGHZ	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xD5,	 0x214A,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_PS.PWM_CURRENT_LIM_CTL, 0xD5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0x8B,	 0x214B,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_PS.PFM_CURRENT_LIM_CTL, 0x8B, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0x0E,	 0x205B,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.AUTO_MODE_CTL, 0xE, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0x07,	 0x205D,	 0xFF,	 PM_SBL_WRITE,	 95,	 95}, 	 //PM8998.S5_CTRL.PFM_COUNT_CTL, 0x07, 0xFF, None, OPTION2_EQ_GND	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xD5,	 0x214A,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_PS.PWM_CURRENT_LIM_CTL, 0xD5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0x8B,	 0x214B,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_PS.PFM_CURRENT_LIM_CTL, 0x8B, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0x0E,	 0x205B,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.AUTO_MODE_CTL, 0xE, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0x07,	 0x205D,	 0xFF,	 PM_SBL_WRITE,	 98,	 98}, 	 //PM8998.S5_CTRL.PFM_COUNT_CTL, 0x07, 0xFF, None, OPTION2_EQ_VPH	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0xFE,	 0x2170,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.SWITCH_SIZE_CTL, 0xFE	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x00,	 0x2172,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.DEAD_TIME_CTRL, 0x0	 
	 { 1,	 0x0E,	 0x217E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.PWR_STG_MISC, 0xE	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x2058,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.DMAX_CTL, 0x1	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x80,	 0x2059,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.PULSE_SKIP_CTL, 0x80	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0xA7,	 0x205C,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.INZERO_COUNT_CTL, 0xA7	 
	 { 1,	 0xC5,	 0x2044,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.PFM_CTL, 0xC5    # Changed from 0xC6 on SBL v5	 
	 { 1,	 0x3A,	 0x207E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.CTLR_MISC, 0x3A	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x62,	 0x2063,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.QM_PS_EXIT, 0x62	 
	 { 1,	 0x07,	 0x2145,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.PFM_FREQ_CFG, 0x7	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x2054,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.BUCK_SNS_CTL, 0x1	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x87,	 0x215A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.ISNS_THRES, 0x87	 
	 { 1,	 0xA5,	 0x21D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.SEC_ACCESS, 0xA5	 
	 { 1,	 0x07,	 0x2163,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_PS.QM_PS_ENTRY, 0x7	 
	 { 1,	 0x06,	 0x2045,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.MODE_CTL1, 0x6	 
	 { 1,	 0x08,	 0x2048,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.PD_CTL, 0x8	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x82,	 0x2061,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.STEPPER_VS_CTL, 0x82	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x01,	 0x20DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.PERPH_RESET_CTL3, 0x1
	 //Mode - ULS_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x4C,	 0x2C68,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.ULS_VSET_LB, 0x4C	 
	 { 1,	 0x04,	 0x2C69,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S9_CTRL.ULS_VSET_UB, 0x4	 
	 { 1,	 0xCC,	 0x1768,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.ULS_VSET_LB, 0xCC	 
	 { 1,	 0x04,	 0x1769,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S2_CTRL.ULS_VSET_UB, 0x4	 
	 { 1,	 0xE0,	 0x1A68,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.ULS_VSET_LB, 0xE0	 
	 { 1,	 0x05,	 0x1A69,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.ULS_VSET_UB, 0x5	 
	 { 1,	 0x70,	 0x1D68,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.ULS_VSET_LB, 0x70	 
	 { 1,	 0x07,	 0x1D69,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.ULS_VSET_UB, 0x7	 
	 { 1,	 0xF8,	 0x2068,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.ULS_VSET_LB, 0xF8	 
	 { 1,	 0x07,	 0x2069,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.ULS_VSET_UB, 0x7	 
	 { 1,	 0x68,	 0x2668,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.ULS_VSET_LB, 0x68	 
	 { 1,	 0x04,	 0x2669,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S7_CTRL.ULS_VSET_UB, 0x4	 
	 { 1,	 0x3C,	 0x1468,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.ULS_VSET_LB, 0x3C	 
	 { 1,	 0x04,	 0x1469,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S1_CTRL.ULS_VSET_UB, 0x4	 
	 { 1,	 0x4C,	 0x2368,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.ULS_VSET_LB, 0x4C	 
	 { 1,	 0x04,	 0x2369,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S6_CTRL.ULS_VSET_UB, 0x4	 
	 { 1,	 0x64,	 0x3868,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.ULS_VSET_LB, 0x64	 
	 { 1,	 0x04,	 0x3869,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S13_CTRL.ULS_VSET_UB, 0x4	 
	 { 1,	 0xB0,	 0x3568,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.ULS_VSET_LB, 0xB0	 
	 { 1,	 0x04,	 0x3569,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S12_CTRL.ULS_VSET_UB, 0x4	 
	 { 1,	 0xA5,	 0x43D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.SEC_ACCESS, 0xA5	 
	 { 1,	 0x3C,	 0x4368,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.ULS_VSET_LB, 0x3C	 
	 { 1,	 0xA5,	 0x43D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x4369,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO04.ULS_VSET_UB, 0x4	 
	 { 1,	 0xA5,	 0x5AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.SEC_ACCESS, 0xA5	 
	 { 1,	 0x3C,	 0x5A68,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.ULS_VSET_LB, 0x3C	 
	 { 1,	 0xA5,	 0x5AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.SEC_ACCESS, 0xA5	 
	 { 1,	 0x04,	 0x5A69,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO27.ULS_VSET_UB, 0x4	 
	 { 5,	 0x70,	 0x1468,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.ULS_VSET_LB, 0x70	 
	 { 5,	 0x04,	 0x1469,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S1_CTRL.ULS_VSET_UB, 0x4	 
	 { 5,	 0x3C,	 0x1768,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.ULS_VSET_LB, 0x3C	 
	 { 5,	 0x04,	 0x1769,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S2_CTRL.ULS_VSET_UB, 0x4	 
	 { 5,	 0xBC,	 0x1A68,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.ULS_VSET_LB, 0xBC	 
	 { 5,	 0x02,	 0x1A69,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.S3_CTRL.ULS_VSET_UB, 0x2
	 //Mode - HFS_OCP 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0xA5,	 0x1AD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x4C,	 0x1A66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S3_CTRL.OCP, 0x4C	 
	 { 1,	 0xA5,	 0x1DD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x6C,	 0x1D66,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S4_CTRL.OCP, 0x6C	 
	 { 1,	 0xA5,	 0x20D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.SEC_ACCESS, 0xA5	 
	 { 1,	 0x6C,	 0x2066,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.S5_CTRL.OCP, 0x6C
	 //Mode - SECURE_PROCESSOR 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 1,	 0x07,	 0x4545,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO06.MODE_CTL1, 0x07	# Force NPM mode 	 
	 { 1,	 0x68,	 0x4540,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO06.VSET_LB, 0x68		# L6 = 1.896V 	 
	 { 1,	 0x07,	 0x4541,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO06.VSET_UB, 0x7		# L6 = 1.896V 	 
	 { 1,	 0xA5,	 0x45D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO06.SEC_ACCESS, 0xA5	 
	 { 1,	 0x1A,	 0x45F1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO06.TRIM_CTRL, 0x1A     # Now fixed in OTP trim 	 
	 { 1,	 0x00,	 0x4547,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO06.FOLLOW_HWEN, 0x0	# Disable all FOLLOW_EN	 
	 { 1,	 0x90,	 0x4566,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.LDO06.OCP_CTL1, 0x90		# Make sure OCP_SELF_SHUTDOWN is disbled 	 
	 { 0,	 0x00,	 0xD741,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO24.DIG_VIN_CTL, 0x00	   # Make sure Vin is VPH_PWR 	 
	 { 0,	 0x01,	 0xD740,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO24.MODE_CTL, 0x01	   # Set to Dig Output	 
	 { 0,	 0x80,	 0xD744,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO24.DIG_OUT_SOURCE_CTL, 0x80	   # Set to Dig Output high	 
	 { 0,	 0xA5,	 0xD7D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO24.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0xD7DA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.GPIO24.PERPH_RESET_CTL3, 0x0   # Ignore Warm_rb, Shutdown1_rb, Shutdown2_rb for VREG_SP	 
	 { 0,	 0xA5,	 0x2CD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MBG1.SEC_ACCESS, 0xA5	 
	 { 0,	 0x00,	 0x2CDA,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.MBG1.PERPH_RESET_CTL3, 0x0   # Ignore Warm_rb, Shutdown1_rb, Shutdown2_rb for VREG_SP
	 //Mode - SCHG_Config 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 2,	 0x09,	 0x14DF,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_DC.CLOCK_CONTROL, 0x09, 0xFF, PMi8998_GTE_2_0   # Added SBLv13 to address PMICPR-1187	 
	 { 2,	 0xC3,	 0x1051,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_CHGR.CHGR_CFG2, 0xC3, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x0C,	 0x107D,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_CHGR.FG_UPDATE_CFG_2_SEL, 0xC, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x14,	 0x1092,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_CHGR.JEITA_CCCOMP_CFG, 0x14, 0xFF, PMi8998_GTE_2_0		#	 
	 { 2,	 0x00,	 0x10B0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_CHGR.STEP_CHG_MODE_CFG, 0x0, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x02,	 0x1154,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_OTG.FLASH_MITIGATION, 0x2, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x03,	 0x1260,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_BATIF.LOW_BATT_DETECT_EN_CFG, 0x3, 0xFF, PMi8998_GTE_2_0		#	 
	 { 2,	 0xCA,	 0x1358,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_USB.TYPE_C_CFG, 0xCA, 0xFF, PMi8998_GTE_2_0		#	 
	 { 2,	 0x31,	 0x1359,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_USB.TYPE_C_CFG_2, 0x31, 0xFF, PMi8998_GTE_2_0 # Added SBL v15.  CR-0000176755: Set soft start 	 
	 { 2,	 0x1D,	 0x1362,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_USB.USBIN_OPTIONS_1_CFG, 0x1D, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x55,	 0x1364,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_USB.TAPER_TIMER_SEL_CFG, 0x55, 0xFF, PMi8998_GTE_2_0		#	 
	 { 2,	 0xE5,	 0x1365,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_USB.USBIN_LOAD_CFG, 0xE5, 0xFF, PMi8998_GTE_2_0		#	 
	 { 2,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.SCHG8998_DC.SEC_ACCESS, 0xA5	 
	 { 2,	 0xC0,	 0x14C1,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.SCHG8998_DC.ENG_SSUPPLY_CFG2, 0xC0	 
	 { 2,	 0xA5,	 0x14D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_DC.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x08,	 0x14C2,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_DC.ENG_SSUPPLY_CFG3, 0x8, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x16D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_MISC.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0		#	 
	 { 2,	 0x03,	 0x16C7,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_MISC.ENG_BUCKBOOST_CFG8, 0x3, 0xFF, PMi8998_GTE_2_0		#	 
	 { 2,	 0xA5,	 0x16D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_MISC.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x16E9,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_MISC.TM_IO_DTEST4_SEL, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x16D0,	 0xFF,	 PM_SBL_WRITE,	 82,	 83}, 	 //PMi8998.SCHG8998_MISC.SEC_ACCESS, 0xA5, 0xFF, None, Clogic_66_AND_GTE_2	 
	 { 2,	 0x00,	 0x16F9,	 0xFF,	 PM_SBL_WRITE,	 82,	 83}, 	 //PMi8998.SCHG8998_MISC.TR_SDCDC_USBIN_IPEAK, 0x0, 0xFF, None, Clogic_66_AND_GTE_2	 
	 { 2,	 0xA5,	 0x16D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_MISC.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x00,	 0x16FA,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.SCHG8998_MISC.TR_SDCDC_DCIN_ISNS, 0x0, 0xFF, PMi8998_GTE_2_0
	 //Mode - SFG_Config 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 2,	 0x60,	 0x4052,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BATT_SOC.LOW_PWR_CFG, 0x60	 
	 { 2,	 0xA5,	 0x40D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BATT_SOC.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x00,	 0x40DA,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BATT_SOC.PERPH_RESET_CTL3, 0x0, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x0D,	 0x416B,	 0xFF,	 PM_SBL_WRITE,	 9,	 12}, 	 //PMi8998.FG_BATT_INFO.BATTERY_MISSING_CFG, 0xD, 0xFF, PMi8998_LTE_2_0	 
	 { 2,	 0xA5,	 0x41D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BATT_INFO.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x00,	 0x41DA,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BATT_INFO.PERPH_RESET_CTL3, 0x0, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x04,	 0x42A9,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.LMH_SLCT_MTGT_FLASH, 0x4, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x42D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x03,	 0x42DA,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.PERPH_RESET_CTL3, 0x3, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x43D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_LMH.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x03,	 0x43DA,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_LMH.PERPH_RESET_CTL3, 0x3, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x44D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_MEM_IF.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x07,	 0x44DA,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_MEM_IF.PERPH_RESET_CTL3, 0x7, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x89,	 0x4586,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_ADC_RR.SKIN_HOT, 0x89	 
	 { 2,	 0x8C,	 0x4587,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_ADC_RR.SKIN_TOO_HOT, 0x8C	 
	 { 2,	 0x80,	 0x4583,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_ADC_RR.AUX_THERM_CFG, 0x80	 
	 { 2,	 0xA5,	 0x45D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_ADC_RR.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x07,	 0x45DA,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_ADC_RR.PERPH_RESET_CTL3, 0x7, 0xFF, PMi8998_GTE_2_0
	 //Mode - BCL_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 2,	 0x8D,	 0x426A,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.BCL_CFG, 0x8D	 
	 { 2,	 0x04,	 0x4270,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBAT_CMP_CFG, 0x4	 
	 { 2,	 0x52,	 0x4272,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBAT_ADC_LOW, 0x52	 
	 { 2,	 0x03,	 0x4275,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBAT_LOW, 0x3	 
	 { 2,	 0x00,	 0x4276,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBAT_TOO_LOW, 0x0	 
	 { 2,	 0x00,	 0x4280,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.IBATT_H_CFG, 0x0	 
	 { 2,	 0x00,	 0x4281,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.IBATT_TH_CFG, 0x0	 
	 { 2,	 0x00,	 0x4284,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBATT_ADC_CFG, 0x0	 
	 { 2,	 0x02,	 0x4286,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBATT_L_CFG, 0x2	 
	 { 2,	 0x00,	 0x4287,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBATT_L_INT_CFG, 0x0	 
	 { 2,	 0x01,	 0x4288,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBATT_TL_CFG, 0x1	 
	 { 2,	 0x00,	 0x4289,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBATT_TL_INT_CFG, 0x0	 
	 { 2,	 0x04,	 0x4292,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.BAN_TIMING_DX, 0x4	 
	 { 2,	 0x03,	 0x42A3,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.LMH_CFG, 0x3	 
	 { 2,	 0x06,	 0x42A4,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.LMH_DELAY_0, 0x6, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x05,	 0x42A5,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.LMH_DELAY_1, 0x5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x04,	 0x42A6,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.LMH_DELAY_2, 0x4, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x42D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x03,	 0x42C0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.BATFET_OPEN_CFG, 0x3, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x42D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x18,	 0x42C1,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.BATFET_OPEN_AX_CTL, 0x18, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x42D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x02,	 0x42C2,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.BATFET_OPEN_PX_CTL, 0x2, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0xA5,	 0x42D0,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.SEC_ACCESS, 0xA5, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x32,	 0x42C7,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.FG_BCL.IBT_THI_MIN_CFG, 0x32, 0xFF, PMi8998_GTE_2_0	 
	 { 2,	 0x03,	 0x4270,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.VBAT_CMP_CFG, 0x03   # Added SBL v1A 12/14/17 - Prism CR 2157754	 
	 { 2,	 0x80,	 0x4246,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.FG_BCL.EN_CTL, 0x80
	 //Mode - BOB_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 3,	 0x03,	 0xA036,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.BOB_CONFIG.EXT_CTRL_FOLLOW, 0x3	 
	 { 3,	 0x0F,	 0xA034,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.BOB_CONFIG.FLASH_OVHD_CTL, 0xF		#	 
	 { 3,	 0xA5,	 0xA0D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.BOB_CONFIG.SEC_ACCESS, 0xA5	 
	 { 3,	 0x04,	 0xA08F,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.BOB_CONFIG.PFM_CONFIG2, 0x4, 0xFF, PMi8998_GTE_2_0
	 //Mode - WLED_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 3,	 0x9A,	 0xD85E,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_CTRL.WLED_SHORT_PROTECT, 0x9A, 0xFF, None, Clogic_67_AND_GTE_2		#	 
	 { 3,	 0x80,	 0xD845,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_CTRL.EXT_PIN_CTL, 0x80, 0xFF, None, Clogic_67_AND_GTE_2		#	 
	 { 3,	 0xD9,	 0xD856,	 0xFF,	 PM_SBL_WRITE,	 86,	 87}, 	 //PMi8998.WLED1_CTRL.VLOOP_COMP_GM, 0xD9, 0xFF, None, Clogic_68_AND_GTE_2	 
	 { 3,	 0x84,	 0xD85B,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.WLED1_CTRL.PSM_CTRL, 0x84, 0xFF, PMi8998_GTE_2_0		#	 
	 { 3,	 0x8D,	 0xD85D,	 0xFF,	 PM_SBL_WRITE,	 86,	 87}, 	 //PMi8998.WLED1_CTRL.WLED_PFM, 0x8D, 0xFF, None, Clogic_68_AND_GTE_2		#	 
	 { 3,	 0x95,	 0xD85D,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_CTRL.WLED_PFM, 0x95, 0xFF, None, Clogic_67_AND_GTE_2		#	 
	 { 3,	 0x01,	 0xD85C,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.WLED1_CTRL.LCD_AUTO_PFM, 0x1, 0xFF, PMi8998_GTE_2_0		#	 
	 { 3,	 0x83,	 0xD85B,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_CTRL.PSM_CTRL, 0x83, 0xFF, None, Clogic_67_AND_GTE_2		#	 
	 { 3,	 0xD9,	 0xD94A,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_SINK.MODULATION_SCHEME, 0xD9, 0xFF, None, Clogic_67_AND_GTE_2	 
	 { 3,	 0x00,	 0xD950,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_SINK.LED1_MODULATOR_EN, 0x00, 0xFF, None, Clogic_67_AND_GTE_2	 
	 { 3,	 0x00,	 0xD960,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_SINK.LED2_MODULATOR_EN, 0x00, 0xFF, None, Clogic_67_AND_GTE_2	 
	 { 3,	 0x00,	 0xD970,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_SINK.LED3_MODULATOR_EN, 0x00, 0xFF, None, Clogic_67_AND_GTE_2	 
	 { 3,	 0x00,	 0xD980,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_SINK.LED4_MODULATOR_EN, 0x00, 0xFF, None, Clogic_67_AND_GTE_2	 
	 { 3,	 0x00,	 0xD946,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_SINK.CURRENT_SINK_EN, 0x00, 0xFF, None, Clogic_67_AND_GTE_2	 
	 { 3,	 0x07,	 0xD94B,	 0xFF,	 PM_SBL_WRITE,	 84,	 85}, 	 //PMi8998.WLED1_SINK.HYBRID_DIMMING_TRESH, 0x07, 0xFF, None, Clogic_67_AND_GTE_2
	 //Mode - GPIO_CONFIG_PMI8998 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 2,	 0x02,	 0xC044,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.GPIO01.DIG_OUT_SOURCE_CTL, 0x2	 
	 { 2,	 0x03,	 0xC045,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.GPIO01.DIG_OUT_DRV_CTL, 0x3	 
	 { 2,	 0x00,	 0xC041,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.GPIO01.DIG_VIN_CTL, 0x0	 
	 { 2,	 0x01,	 0xC040,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.GPIO01.MODE_CTL, 0x1
	 //Mode - IBB_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 3,	 0xA5,	 0xDCD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.SEC_ACCESS, 0xA5	 
	 { 3,	 0x43,	 0xDC56,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.NONOVERLAP_TIME_1, 0x43	 
	 { 3,	 0xA5,	 0xDCD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.SEC_ACCESS, 0xA5	 
	 { 3,	 0x01,	 0xDC57,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.NONOVERLAP_TIME_2, 0x1	 
	 { 3,	 0xA5,	 0xDCD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.SEC_ACCESS, 0xA5	 
	 { 3,	 0xBF,	 0xDC4B,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.CURRENT_LIMIT, 0xBF	 
	 { 3,	 0x85,	 0xDC50,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.PS_CTL, 0x85	 
	 { 3,	 0xA5,	 0xDCD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.SEC_ACCESS, 0xA5	 
	 { 3,	 0x43,	 0xDC56,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.NONOVERLAP_TIME_1, 0x43	 
	 { 3,	 0xA5,	 0xDCD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.SEC_ACCESS, 0xA5	 
	 { 3,	 0x01,	 0xDC57,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.NONOVERLAP_TIME_2, 0x1	 
	 { 3,	 0xA5,	 0xDCD0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.SEC_ACCESS, 0xA5	 
	 { 3,	 0xF7,	 0xDC58,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.PWRUP_PWRDN_CTL_1, 0xF7	 
	 { 3,	 0x27,	 0xDC5A,	 0xFF,	 PM_SBL_WRITE,	 88,	 89}, 	 //PMi8998.IBB.SWIRE_CTL, 0x27, 0xFF, None, Clogic_69_AND_GTE_2	 
	 { 3,	 0x52,	 0xDC65,	 0xFF,	 PM_SBL_WRITE,	 88,	 89}, 	 //PMi8998.IBB.SWIRE_WLED_CTL2, 0x52, 0xFF, None, Clogic_69_AND_GTE_2	 
	 { 3,	 0x10,	 0xDC60,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.SPARE_CTL, 0x10	 
	 { 3,	 0x00,	 0xDC61,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.NLIMIT_DAC, 0x0	 
	 { 3,	 0x1F,	 0xDC52,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.IBB.PWM_CTL_2, 0x1F, 0xFF, PMi8998_GTE_2_0	 
	 { 3,	 0x01,	 0xDC63,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.IBB.SPARE_CTL_2, 0x1, 0xFF, PMi8998_GTE_2_0	 
	 { 3,	 0x40,	 0xDC46,	 0xFF,	 PM_SBL_WRITE,	 79,	 79}, 	 //PMi8998.IBB.ENABLE_CTL, 0x40, 0xFF, None, Clogic_69	 
	 { 3,	 0x80,	 0xDC45,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.IBB.MODULE_RDY, 0x80
	 //Mode - LAB_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 3,	 0x00,	 0xDE43,	 0xFF,	 PM_SBL_WRITE,	 80,	 80}, 	 //PMi8998.LAB.PWM_MODE_SEL, 0x0, 0xFF, None, Clogic_70	 
	 { 3,	 0x80,	 0xDE49,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.LAB.IBB_EN_RDY, 0x80	 
	 { 3,	 0xFC,	 0xDE4C,	 0xFF,	 PM_SBL_WRITE,	 90,	 91}, 	 //PMi8998.LAB.CURRENT_SENSE, 0xFC, 0xFF, None, Clogic_70_AND_GTE_2	 
	 { 3,	 0x01,	 0xDE4D,	 0xFF,	 PM_SBL_WRITE,	 80,	 80}, 	 //PMi8998.LAB.LOOP_COMP_CTL, 0x1, 0xFF, None, Clogic_70	 
	 { 3,	 0x0C,	 0xDE4E,	 0xFF,	 PM_SBL_WRITE,	 90,	 91}, 	 //PMi8998.LAB.SLOPE_COMP_CURRENT, 0xC, 0xFF, None, Clogic_70_AND_GTE_2	 
	 { 3,	 0x83,	 0xDE50,	 0xFF,	 PM_SBL_WRITE,	 81,	 81}, 	 //PMi8998.LAB.PS_CTL, 0x83, 0xFF, None, Clogic_71	 
	 { 3,	 0x00,	 0xDE50,	 0xFF,	 PM_SBL_WRITE,	 80,	 80}, 	 //PMi8998.LAB.PS_CTL, 0x0, 0xFF, None, Clogic_70	 
	 { 3,	 0xCB,	 0xDE51,	 0xFF,	 PM_SBL_WRITE,	 80,	 80}, 	 //PMi8998.LAB.PWM_CTL_1, 0xCB, 0xFF, None, Clogic_70	 
	 { 3,	 0x03,	 0xDE5E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.LAB.PRECHARGE_CTL, 0x3	 
	 { 3,	 0x12,	 0xDE60,	 0xFF,	 PM_SBL_WRITE,	 90,	 91}, 	 //PMi8998.LAB.MISC_CTL, 0x12, 0xFF, None, Clogic_70_AND_GTE_2	 
	 { 3,	 0xCC,	 0xDE63,	 0xFF,	 PM_SBL_WRITE,	 92,	 93}, 	 //PMi8998.LAB.HIGH_BW_CTL, 0xCC, 0xFF, None, Clogic_71_AND_GTE_2	 
	 { 3,	 0x04,	 0xDE63,	 0xFF,	 PM_SBL_WRITE,	 90,	 91}, 	 //PMi8998.LAB.HIGH_BW_CTL, 0x4, 0xFF, None, Clogic_70_AND_GTE_2	 
	 { 3,	 0x02,	 0xDE64,	 0xFF,	 PM_SBL_WRITE,	 92,	 93}, 	 //PMi8998.LAB.SPARE_CTL, 0x2, 0xFF, None, Clogic_71_AND_GTE_2	 
	 { 3,	 0x00,	 0xDE64,	 0xFF,	 PM_SBL_WRITE,	 90,	 91}, 	 //PMi8998.LAB.SPARE_CTL, 0x0, 0xFF, None, Clogic_70_AND_GTE_2	 
	 { 3,	 0x80,	 0xDE45,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.LAB.MODULE_RDY, 0x80	 
	 { 3,	 0xA5,	 0xDED0,	 0xFF,	 PM_SBL_WRITE,	 90,	 91}, 	 //PMi8998.LAB.SEC_ACCESS, 0xA5, 0xFF, None, Clogic_70_AND_GTE_2	 
	 { 3,	 0x03,	 0xDEE2,	 0xFF,	 PM_SBL_WRITE,	 90,	 91}, 	 //PMi8998.LAB.DTEST1_ACCESS, 0x3, 0xFF, None, Clogic_70_AND_GTE_2
	 //Mode - FLASH_CONFIG 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 3,	 0xA5,	 0xD3D0,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.SEC_ACCESS, 0xA5	 
	 { 3,	 0x90,	 0xD3E5,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.TEST4, 0x90	 
	 { 3,	 0x84,	 0xD34D,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.LED1_HDRM_PRGM, 0x84	 
	 { 3,	 0x84,	 0xD34E,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.LED2_HDRM_PRGM, 0x84	 
	 { 3,	 0x84,	 0xD34F,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.LED3_HDRM_PRGM, 0x84	 
	 { 3,	 0x02,	 0xD352,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.ISC_DELAY, 0x2	 
	 { 3,	 0x10,	 0xD363,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.BCL_CFG, 0x10	 
	 { 3,	 0x01,	 0xD366,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.BOB_ILIMIT_CFG, 0x1	 
	 { 3,	 0x01,	 0xD366,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMi8998.MVFLASH1.BOB_ILIMIT_CFG, 0x1	 
	 { 3,	 0x12,	 0xD36E,	 0xFF,	 PM_SBL_WRITE,	 1,	 4}, 	 //PMi8998.MVFLASH1.MITIGATION_SEL, 0x12, 0xFF, PMi8998_GTE_2_0
	 //Mode - generateSblBranchAndVersion 
	 //sid 	data 	register 	Mask 	reg op 	cond start 	cond end	 
	 { 0,	 0x1E,	 0x0150,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.REVID.SBL_ID_0, 0x1E	 
	 { 0,	 0x01,	 0x0151,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8998.REVID.SBL_ID_1, 0x01	 
	 { 2,	 0x1E,	 0x0150,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMI8998.REVID.SBL_ID_0, 0x1E	 
	 { 2,	 0x01,	 0x0151,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PMI8998.REVID.SBL_ID_1, 0x01	 
	 { 4,	 0x1E,	 0x0150,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.REVID.SBL_ID_0, 0x1E	 
	 { 4,	 0x01,	 0x0151,	 0xFF,	 PM_SBL_WRITE,	 0,	 0}, 	 //PM8005.REVID.SBL_ID_1, 0x01
 	 // This line of data is created by PSI Compiler per request from Embedded SW Driver.It is not part of original code.
 	 {0, 0x00, 0x0000, 0xFF, PM_SBL_OPERATION_INVALID, 0, 0} 
 };

