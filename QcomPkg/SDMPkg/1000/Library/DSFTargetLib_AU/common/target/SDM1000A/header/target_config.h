/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
                              EDIT HISTORY

==============================================================================*/
#ifndef __TARGET_CONFIG_H__
#define __TARGET_CONFIG_H__

#include "ddr_cores.h"
#include "cabo_seq_hwiobase.h"
#include "cabo_seq_hwioreg.h"
#include "mccc_seq_hwiobase.h"
#include "mccc_seq_hwioreg.h"
#include "llcc_seq_hwiobase.h"
#include "llcc_seq_hwioreg.h"
#include "ddr_ss_seq_hwiobase.h"
#include "ddr_ss_seq_hwioreg.h"

// Define silicon target or emulation target. Some portions of the DDR System Firmware will or will not
// be compiled depending on what this macro is defined as. 
// If TARGET_SILICON is defined as a 0, it implies an emulation build.
// If TARGET_SILICON is defined as a 1, it implies a real-silicon build.
// If TARGET_SILICON is already defined (on the compiler command line), do not override that definition.
//    This will allow users to override this emulation/silicon flag at compile time without changing the DSF.
//#ifndef TARGET_SILICON
//#define TARGET_SILICON 0
//#endif // TARGET_SILICON

//Set it to 1 if target does not support BROADCAST mode and has only one Channel
#define REDIRECT_BROADCAST_TO_CH0 0


// NoC base address
#define SEQ_NOC_OFFSET SEQ_DDR_SS_GEMNOC_GEM_NOC_OFFSET

#define CS_1HOT(uint8)                  ((uint8 == 0) ? \
                                         DDR_CS0 : \
                                         DDR_CS1)

//MCCC base address

#define REG_OFFSET_MCCC_SLV(uint8)      ((uint8 == 0) ? \
                                        (SEQ_DDR_SS_MCCC_CH0_MCCC_SLV_OFFSET) : \
                                        (uint8 == 1) ? \
                                        (SEQ_DDR_SS_MCCC_CH1_MCCC_SLV_OFFSET) : \
                                        (uint8 == 2) ? \
                                        (SEQ_DDR_SS_MCCC_CH2_MCCC_SLV_OFFSET) : \
                                        (uint8 == 3) ? \
                                        (SEQ_DDR_SS_MCCC_CH3_MCCC_SLV_OFFSET) : \
                                        (uint8 == 4) ? \
                                        (SEQ_DDR_SS_MCCC_CH4_MCCC_SLV_OFFSET) : \
                                        (uint8 == 5) ? \
                                        (SEQ_DDR_SS_MCCC_CH5_MCCC_SLV_OFFSET) : \
										(uint8 == 6) ? \
                                        (SEQ_DDR_SS_MCCC_CH6_MCCC_SLV_OFFSET) : \
										(uint8 == 7) ? \
										(SEQ_DDR_SS_MCCC_CH7_MCCC_SLV_OFFSET) : \
										(SEQ_DDR_SS_MCCC_BROADCAST_MCCC_SLV_OFFSET))

#define ACTIVITY_BASED_SLEEP_EN			0

// Target Name
#define TARGET_SDM1000

// PHY Core
#define TARGET_PHY_ARCH     0x03
#define TARGET_PHY_MAJOR    0x02
#define TARGET_PHY_MINOR    0x02
#define TARGET_PHY_PATCH_V1 0x00
#define TARGET_PHY_PATCH_V2 0x00

#define TARGET_PHY_VER_V1 TARGET_CORE_VER(TARGET_PHY_ARCH, TARGET_PHY_MAJOR, TARGET_PHY_MINOR, TARGET_PHY_PATCH_V1)
#define TARGET_PHY_VER_V2 TARGET_CORE_VER(TARGET_PHY_ARCH, TARGET_PHY_MAJOR, TARGET_PHY_MINOR, TARGET_PHY_PATCH_V2)

#define TARGET_PHY_MIN_VER TARGET_PHY_VER_V1

// MC Core
#define TARGET_MC_ARCH     0x04
#define TARGET_MC_MAJOR    0x01
#define TARGET_MC_MINOR    0x00
#define TARGET_MC_PATCH_V1 0x02
#define TARGET_MC_PATCH_V2 0x02

#define TARGET_MC_VER_V1 TARGET_CORE_VER(TARGET_MC_ARCH, TARGET_MC_MAJOR, TARGET_MC_MINOR, TARGET_MC_PATCH_V1)
#define TARGET_MC_VER_V2 TARGET_CORE_VER(TARGET_MC_ARCH, TARGET_MC_MAJOR, TARGET_MC_MINOR, TARGET_MC_PATCH_V2)

#define TARGET_MC_MIN_VER TARGET_MC_VER_V1
#define MR13_BASE (1 << 5 | 1 << 4) // Set OP[4]=1 (RRO : refresh rate option) and OP[5] =1 (DMD : data mask disable)

// LLCC Core
#define TARGET_LLCC_ARCH     0x01
#define TARGET_LLCC_MAJOR    0x01
#define TARGET_LLCC_MINOR    0x01
#define TARGET_LLCC_PATCH_V1 0x01
#define TARGET_LLCC_PATCH_V2 0x01

#define TARGET_LLCC_VER_V1 TARGET_CORE_VER(TARGET_LLCC_ARCH, TARGET_LLCC_MAJOR, TARGET_LLCC_MINOR, TARGET_LLCC_PATCH_V1)
#define TARGET_LLCC_VER_V2 TARGET_CORE_VER(TARGET_LLCC_ARCH, TARGET_LLCC_MAJOR, TARGET_LLCC_MINOR, TARGET_LLCC_PATCH_V2)

#define TARGET_LLCC_MIN_VER TARGET_LLCC_VER_V1

//Reference channel
#define TARGET_ACTIVE_CHANNEL 0x0

#endif // __TARGET_CONFIG_H__
