/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
================================================================================*/

#include "ddrss.h"
#include "ddr_phy.h"
#include "mc.h"
#include "target_config.h"
#include "ddr_phy_seq_hwioreg.h"
#include "ddr_ss_seq_hwiobase.h"
#include "cabo_seq_hwioreg.h"

// AC Timing Package Receiving Mode
#define AC_TIMING_SEQUENCE_MODE


// Entry type
#define ENTRY_TYPE_MASK                     0xC000
#define ENTRY_TYPE_SHIFT                    14

// Entry types
#define TERMINATE                           0
#define TARGET                              1
#define PARAMETER                           2
#define TMRS                                3

// Entry subtype
#define SUBTYPE_MASK                        0x3000
#define SUBTYPE_SHIFT                       12

// Terminate entry subtypes
#define TERM                                0
#define QUIT                                1
#define ACTIMINGS1                          2
#define ACTIMINGS2                          3
#define ACTIMINGS3                          2
#define VOLTAGEPLAN                         3

// Quit and/or clear entry
#define QUIT_BIT_MASK                       0x0800
#define TARGET_RESTRICTIONS_CLEAR_MASK      0x07FF
#define MANUFACTURER_ID_BIT                 0x001
#define REVISION_ID_1_BIT                   0x002
#define REVISION_ID_2_BIT                   0x004
#define DENSITY_BIT                         0x008
#define LOW_FREQ_BIT                        0x010
#define HIGH_FREQ_BIT                       0x020
#define IO_WIDTH_BIT                        0x040
#define REVISION_ID_1_RANK_1_BIT            0x080
#define REVISION_ID_2_RANK_1_BIT            0x100
#define DENSITY_RANK_1_BIT                  0x200
#define IO_WIDTH_RANK_1_BIT                 0x400

// Target entry subtypes
#define MODE_REGISTER                       0
#define FREQUENCY_MASK                      1

// Mode register entry
#define MODE_SUBTYPE_MASK                   0x0F00
#define MODE_SUBTYPE_SHIFT                  8
#define MODE_VALUE_MASK                     0x00FF
#define MANUFACTURER_ID                     0
#define REVISION_ID_1                       1
#define REVISION_ID_2                       2
#define DENSITY                             3
#define IO_WIDTH                            4
#define REVISION_ID_1_RANK_1                5
#define REVISION_ID_2_RANK_1                6
#define DENSITY_RANK_1                      7
#define IO_WIDTH_RANK_1                     8

// Frequency mask entry
#define FREQ_MASK_SUBTYPE_MASK              0x0C00
#define FREQ_MASK_SUBTYPE_SHIFT             10
#define LOW_FREQ                            0
#define HIGH_FREQ                           3
#define HIGH_FREQ_MASK                      0x007F
#define HIGH_FREQ_SHIFT                     8
#define LOW_FREQ_MASK                       0x00FF
#define CLEAR_BIT_MASK                      0x0200

// Parameter entry
#define PARAMETER_TYPE_MASK                 0x3000
#define PARAMETER_TYPE_SHIFT                12
#define MSM_PARAMETER                       0
#define DRAM_PARAMETER                      2
#define SAFE_MODE                           1
#define PARAMETER_INDEX_MASK                0x2F00
#define PARAMETER_INDEX_SHIFT               8
#define PARAMETER_VALUE_MASK                0x00FF
#define CA_PU                               0
#define CA_PD                               1
#define CA_VOH                              2
#define CK_PU                               4
#define CK_PD                               5
#define CK_VOH                              6
#define DQ_PU                               8
#define DQ_PD                               9
#define DQ_VOH                              10
#define DQ_ODT                              11
#define DQS_PU                              12
#define DQS_PD                              13
#define DQS_VOH                             14
#define DQS_ODT                             15
#define DRAM_CA_ODT                         32
#define DRAM_DQ_ODT                         33
#define DRAM_DQ_VOH                         34
//#define DRAM_DQ_PU                        35
#define DRAM_DQ_PD                          36

//Safe Mode Parameter
#define SELF_REFRESH_MODE                   0
#define REFRESH_RATE_POLLING                1
#define PERIODIC_ZQ_CAL                     2
#define PERIODIC_TRAINING                   3
#define LPDDR4Y_LP_CA_BUFFER_SUPPORT        4
#define LPDDR4Y_SINGLE_ENDED_CLOCK_SUPPORT  5
#define BASE_LOGGING                        6
#define EXTENDED_LLCC_LOGGING               7
#define EXTENDED_MC_LOGGING                 8
#define EXTENDED_PHY_LOGGING                9
#define PARTIAL_ARRAY_SELF_REFRESH          10
#define LPDDR4Y_SINGLE_ENDED_WR_DQS_SUPPORT 11
#define LPDDR4Y_SINGLE_ENDED_RD_DQS_SUPPORT 12
#define POWER_DOWN                          13
#define CLOCK_STOP                          14
#define AUTO_PRECHARGE                      15
#define SAFE_MODE_FEATURE_INDEX_RANGE       9
#define SAFE_MODE_INDEX_MASK                0x0800
#define SAFE_MODE_INDEX_SHFT                11
#define SAFE_MODE_FEATURE                   0x001E
#define SAFE_MODE_FEATURE_SHFT              1

//AC TIMING Parameter
#define ADDR_OFFSET_TZQLAT                  0x0144
#define ADDR_OFFSET_TSR                     0x0504
#define ADDR_OFFSET_TXSR                    0x013c
#define ADDR_OFFSET_TXSR_ABORT              0x0140
#define ADDR_OFFSET_TXP                     0x041c
#define ADDR_OFFSET_TCCD                    0x0310
#define ADDR_OFFSET_TRTP                    0x0214
#define ADDR_OFFSET_TRCD                    0x020c
#define ADDR_OFFSET_TRPPB                   0x0210
#define ADDR_OFFSET_TRPAB                   0x0134
#define ADDR_OFFSET_TRAS                    0x0208
#define ADDR_OFFSET_TRRD                    0x0138
#define ADDR_OFFSET_TPPD                    0x0110
#define ADDR_OFFSET_TFAW                    0x0100
#define ADDR_OFFSET_TDQSCKMIN               0x031c
#define ADDR_OFFSET_TDQSCKMAX               0x0318
#define ADDR_OFFSET_TFC                     0x0500
#define ADDR_OFFSET_TREFI                   0xFFFF
#define ADDR_OFFSET_TMMR                    0x0108
#define ADDR_OFFSET_TMRW                    0x010c
#define ADDR_OFFSET_TMRD                    0x0104
#define ADDR_OFFSET_TODTONMIN               0x0330
#define ADDR_OFFSET_TODTONMAX               0x032c
#define ADDR_OFFSET_TODTOFFMIN              0x0328
#define ADDR_OFFSET_TODTOFFMAX              0x0324
#define ADDR_OFFSET_TCKE                    0x0404
#define ADDR_OFFSET_TCMDCKE                 0x040c
#define ADDR_OFFSET_TCKELCK                 0x0408
#define ADDR_OFFSET_TCKCKEH                 0x0400
#define ADDR_OFFSET_TMRWCKEL                0x0410
#define INDEX_TREFI                         17
#define ACTIMING_PAR_MASK                   0x07F0
#define ACTIMING_PAR_SHFT                   4
#define ACTIMING1_MASK                      0x0000000F
#define ACTIMING2_MASK                      0x00000FFF
#define ACTIMING2_SHFT                      4
#define ACTIMING3_MASK                      0x00000F00
#define ACTIMING3_SHFT                      8
#define ACTIMING_NCK_MASK                   0x000000FF
#define ACTIMING_NCK_SHFT                   20
#define ACTIMING_DERATE_MASK                0x00000800
#define ACTIMING_DERATE_SHFT                17
#define ACTIMING1_FLAG                      0x01
#define ACTIMING2_FLAG                      0x02
#define ACTIMING12_FLAG                     0x03
#define ACTIMING3_FLAG                      0x04
#define ACTIMING_RDY_FLAG                   0x07
#define AC_TIMING_PAR_INDEX_RANGE           30
#define AC_VALUE_MASK                       0xFFFFF
#define CXO_PERIOD_IN_PS                    52083

// Voltage Plan Parameter
#define VOLTAGE_RAIL_LIMIT                  7
#define VDDCX_MASK                          0x000F
#define VDDMX_MASK                          0x00F0
#define VDDMX_SHFT                          4
#define VDDA_MASK                           0x0F00
#define VDDA_SHFT                           8

// MRW entry
#define MR_ADDR_MASK                        0x3F00
#define MR_ADDR_SHIFT                       8
#define MR_VALUE_MASK                       0x00FF

// MRR entry
#define MRR_ENTRY_MASK 0xFFC0
#define MRR_ENTRY_VALUE 0x9FC0
#define MRR_ADDR_MASK 0x3F

#define MRR_DATA_MAX (0x100-0xC4)
uint8 mrr_data_index = 0;
uint8 *mrr_data_ptr = (uint8 *)(SHRM_MR_DUMP_ADDR + 0xC4);

// Private functions
boolean DDRSS_eCDT_Decode_Terminate_Entry(BOOT_DDR_STRUCT *ddr, uint16 entry);
boolean DDRSS_eCDT_Decode_Target_Entry(BOOT_DDR_STRUCT *ddr, uint16 entry);
boolean DDRSS_eCDT_Decode_Parameter_Entry(BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel, uint16 entry);
boolean DDRSS_eCDT_Decode_TMRS_Entry(BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel, uint16 entry);
void DDRSS_eCDT_Populate_PRFS_MC_Masks(BOOT_DDR_STRUCT *ddr);
boolean DDRSS_eCDT_AC_Timing_Exe(void);
boolean DDRSS_eCDT_Switchboard_Update(BOOT_DDR_STRUCT *ddr);

/** @Global_variable
*    @brief
*     termination variable describes when to terminate the eCDT Decode. When termination goes to 1, eCDT Decode will terminate.
*    g_target_mismatch_mask
*/

static boolean  g_terminate = FALSE;
static uint8    g_ac_timing_par_index = 0;
static uint8    g_ac_timing_data_ready = 0;
static uint16    g_target_mismatch_mask = 0;
static uint16   g_freq_mask = 0;
static uint16   g_prfs_mask = 0;
static uint16   g_mc_mask = 0;
static uint32   g_ac_timing_value = 0;


//================================================================================================//
//Take list List from eCDT Encoder by opening the .bin file and taking entries
//================================================================================================//
/** @API 1. eCDT_Decode
*    @brief eCDT_Decode will decode list from .bin file
*/

//================================================================================================//
//Decode DDRSS_eCDT_Decode_Entry
//================================================================================================//
/** @API 2. DDRSS_eCDT_Decode_Entry
*    @brief DDRSS_eCDT_Decode_Entry will decode different types of entries using the list coming from populate_ecdt_from_file API.
*/

boolean DDRSS_eCDT_Decode_Entry(BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel, uint16* list)
{
    uint8  type = 0;
    uint16 entry = 0;
    uint16 index = 0;

    if(list == NULL) return TRUE;

    for(index = 0; list[index] != 0; index++)
    {
        // Get entry type
        entry = list[index];
        type = (entry & ENTRY_TYPE_MASK) >> ENTRY_TYPE_SHIFT;

        // Process entry based on type
        if(type == TERMINATE) //TERMINATE is 0
        {
            if(DDRSS_eCDT_Decode_Terminate_Entry(ddr, entry) == FALSE)
            {
                return FALSE;
            }
        }
        else if(type == TARGET) //TARGET is 1
        {
            if(DDRSS_eCDT_Decode_Target_Entry(ddr, entry) == FALSE)
            {
                return FALSE;
            }
        }
        else if(type == PARAMETER) // PARAMETER is 2
        {
            // Decode parameter only if target matches
            if(g_target_mismatch_mask == 0)
            {
                if(DDRSS_eCDT_Decode_Parameter_Entry(ddr, channel, entry) == FALSE)
                {
                    return FALSE;
                }
            }
        }
        else if(type == TMRS) // TMRS is 3
        {
            // Decode TMRS only if target matches
            if(g_target_mismatch_mask == 0)
            {
                if(DDRSS_eCDT_Decode_TMRS_Entry(ddr, channel, entry) == FALSE)
                {
                    return FALSE;
                }
            }
        }
        else
        {
            return FALSE;
        }

        // Stop processing entries if termination is requested
        if(g_terminate == TRUE)
        {
            break;
        }
    }

    return TRUE;
}


//================================================================================================//
//DDRSS_eCDT_Decode_Terminate_Entry
//    To decode Terminate Entry
//================================================================================================//
/** @API 3. DDRSS_eCDT_Decode_Terminate_Entry
*    @brief DDRSS_eCDT_Decode_Terminate_Entry has two different sub entries.
*    If TERM is accounted the API will not consider any otehr entry and will temrinate
*   If Quit is found, the API will terminate only if Mismatch is found to be 0.
*/
boolean DDRSS_eCDT_Decode_Terminate_Entry(BOOT_DDR_STRUCT *ddr, uint16 entry)
{
    uint8 subtype = 0;
    uint8 clear_mask = 0;

    // Get entry subtype
    subtype = (entry & SUBTYPE_MASK) >> SUBTYPE_SHIFT;

    // Process entry based on subtype
    if(subtype == TERM) // TERM is 0
    {
        g_terminate = TRUE;
        return TRUE;
    }
    else if(subtype == QUIT) // QUIT is 1
    {
        // Check if we should quit the decoding
        if(entry & QUIT_BIT_MASK)
        {
            if(g_target_mismatch_mask == 0)
            {
                g_terminate = TRUE;
                return TRUE;
            }
        }

        // Get clear mask
        clear_mask = entry & TARGET_RESTRICTIONS_CLEAR_MASK;

        // Clear mismatches for mode register targets
        if(clear_mask & MANUFACTURER_ID_BIT)
        {
            g_target_mismatch_mask &= ~MANUFACTURER_ID_BIT;
        }

        if(clear_mask & REVISION_ID_1_BIT)
        {
            g_target_mismatch_mask &= ~REVISION_ID_1_BIT;
        }

        if(clear_mask & REVISION_ID_2_BIT)
        {
            g_target_mismatch_mask &= ~REVISION_ID_2_BIT;
        }

        if(clear_mask & DENSITY_BIT)
        {
            g_target_mismatch_mask &= ~DENSITY_BIT;
        }

        if(clear_mask & IO_WIDTH_BIT)
        {
            g_target_mismatch_mask &= ~IO_WIDTH_BIT;
        }

        if(clear_mask & REVISION_ID_1_RANK_1_BIT)
        {
            g_target_mismatch_mask &= ~REVISION_ID_1_RANK_1_BIT;
        }

        if(clear_mask & REVISION_ID_2_RANK_1_BIT)
        {
            g_target_mismatch_mask &= ~REVISION_ID_2_RANK_1_BIT;
        }

        if(clear_mask & DENSITY_RANK_1_BIT)
        {
            g_target_mismatch_mask &= ~DENSITY_RANK_1_BIT;
        }

        if(clear_mask & IO_WIDTH_RANK_1_BIT)
        {
            g_target_mismatch_mask &= ~IO_WIDTH_RANK_1_BIT;
        }

        if((clear_mask & LOW_FREQ_BIT) || (clear_mask & HIGH_FREQ_BIT))
        {
            if(clear_mask & LOW_FREQ_BIT)
            {
                g_freq_mask &= HIGH_FREQ_MASK << HIGH_FREQ_SHIFT;
            }

            if(clear_mask & HIGH_FREQ_BIT)
            {
                g_freq_mask &= LOW_FREQ_MASK;
            }

            DDRSS_eCDT_Populate_PRFS_MC_Masks(ddr);
        }
    }
    //AC Timings-1 See eCDT User Guide 2.1 For more detail
    else if(subtype == ACTIMINGS1) //ACTIMINGS1 is 2
     {
        // eCDT 2.0 -> 2.1 User Guide Added Code Starts Here...

#ifdef AC_TIMING_SEQUENCE_MODE
        if(g_ac_timing_data_ready != 0)
        {
            return FALSE;
        }
        else
        {
#endif
            uint32 temp = (uint32) entry;
#ifdef AC_TIMING_SEQUENCE_MODE
            g_ac_timing_value = 0;
#else
            g_ac_timing_value &=~ ACTIMING1_MASK; // Clear the mask bits
#endif
            g_ac_timing_value |=(temp & ACTIMING1_MASK);   
                //AC Timing Value bit 0 to 3 is the bit 0 to 3 in AC Timing s1 
                //see eCDT User Guide 2.1 for more detail
                // ACTIMING1_MASK == 0x0000000F
            temp = (entry & ACTIMING_PAR_MASK) >> ACTIMING_PAR_SHFT; 
                //AC Timing Parameter is between bit 4 and bit 10 of AC Timing s1 
                //see eCDT User Guide 2.1 for more detail
                //ACTIMING_PAR_MASK == 0x07F0
                //ACTIMING_PAR_SHFT == 4
        
            g_ac_timing_par_index = (uint8)temp;
            g_ac_timing_value &=~(ACTIMING_DERATE_MASK << ACTIMING_DERATE_SHFT);
            g_ac_timing_value |= ((entry & ACTIMING_DERATE_MASK) << ACTIMING_DERATE_SHFT);
                //AC Timing-3 bit 0 is the derate copy to ac timing value bit 28
                // ACTIMING_DERATE_MASK == 0x0800
                // ACTIMING_DERATE_SHFT == 17

            // Set AC1 Value Flag
#ifdef AC_TIMING_SEQUENCE_MODE
            g_ac_timing_data_ready = ACTIMING1_FLAG;
        }
#else
            g_ac_timing_data_ready |= ACTIMING1_FLAG;
         
            if(g_ac_timing_data_ready == ACTIMING_RDY_FLAG) //ACTIMING_RDY_FLAG == 7
            {
                if(DDRSS_eCDT_AC_Timing_Exe()==TRUE) // Call the EXE Function
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
#endif
    }
    //AC Timings-2 See eCDT User Guide 2.1 For more detail
    else if(subtype == ACTIMINGS2) // ACTIMINGS2 is 3
    {
        // AC Timings -2 Code Here
#ifdef AC_TIMING_SEQUENCE_MODE
        if(g_ac_timing_data_ready != ACTIMING1_FLAG)
        {
            return FALSE;
        }
        else
        {
#endif
            uint32 temp = (uint32) entry;
#ifndef AC_TIMING_SEQUENCE_MODE
            g_ac_timing_value &=~(ACTIMING2_MASK << ACTIMING2_SHFT);
#endif
            g_ac_timing_value|=((temp & ACTIMING2_MASK) << ACTIMING2_SHFT);
                //AC Timing Value bit 4 to 15 is AC TIMINGs2 bit 0 to 11 
                //see eCDT User Guide 2.1 for more detail
                //ACTIMING2_MASK == ACTIMING2_MASK
                //ACTIMING2_SHFT == 4

            // Set AC2 Value Flag
            g_ac_timing_data_ready |= ACTIMING2_FLAG;
            
#ifndef AC_TIMING_SEQUENCE_MODE
            if(g_ac_timing_data_ready == ACTIMING_RDY_FLAG) //ACTIMING_RDY_FLAG==7
            {
                if(DDRSS_eCDT_AC_Timing_Exe()==TRUE)
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
#else
        }
#endif
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

//================================================================================================//
//DDRSS_eCDT_Decode_Target_Entry
//    To decode Target Entry
//================================================================================================//

/** @API 4. DDRSS_eCDT_Decode_Target_Entry
*    @brief DDRSS_eCDT_Decode_Target_Entry has two different sub entries.
*    MODE_REGISTER and FREQUENCY_MASK.
*    MODE_REGISTER type has 4 types which can be found depending on the Modesubtype entry values. Depending on the MODE_REGISTER type, if the DRAM MODE_REGISTER entries matches the the list entry Mismatch bit related to the corresponding MODE_REGISTER value is reset.
*    FREQUENCY_MASK type has 2 types i.e. Low freq and High freq. This frequency mask is required to use fill the register values for PRFS and BIMC registers. Freqeuencymask is used to find g_prfs_mask and g_mc_mask which will later apply settings to corresponding registers.
*/
boolean DDRSS_eCDT_Decode_Target_Entry(BOOT_DDR_STRUCT *ddr, uint16 entry)
{
    uint8 subtype = 0;
    uint8 freq_index = 0;
    uint8 rail_index = 0;
    uint8 mr_field = 0;
    uint8 mr_value = 0;
    uint8 freq_mask_subtype = 0;

    // Voltage Plan Parameter Rail Index Content see eCDT User Guide 2.1 pg 18
    uint32 voltage_rail[VOLTAGE_RAIL_LIMIT]={MIN_SVS, 
                                             LOW_SVS, 
                                             SVS, 
                                             SVS_L1, 
                                             NOMINAL,
                                             TURBO,
                                             TURBO_L1}; 

    // Get entry subtype
    subtype = (entry & SUBTYPE_MASK) >> SUBTYPE_SHIFT;

    // Process entry based on subtype
    if(subtype == MODE_REGISTER) //MODE_REGISTER =0
    {
        // Confirm pointer to detected memory device information from mode register fields was provided

        if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].populated_chipselect == 0)

        {
            return FALSE;
        }

        // Get mode register field and value
        mr_field = (entry & MODE_SUBTYPE_MASK) >> MODE_SUBTYPE_SHIFT;
        mr_value = (entry & MODE_VALUE_MASK);
        // MODE_SUBTYPE_MASK = 0x0F00
        // MODE_SUBTYPE_SHIFT = 8
        // MODE_VALUE_MASK = 0x00FF

        // Check field value matches detected memory device information
        if(mr_field == MANUFACTURER_ID) //MANUFACTURER_ID = 0
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].manufacturer_id != mr_value)

            {
                g_target_mismatch_mask |= MANUFACTURER_ID_BIT;
            }
        }
        else if(mr_field == REVISION_ID_1) //REVISION_ID_1 = 1
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].revision_id_1[0] != mr_value)

            {
                g_target_mismatch_mask |= REVISION_ID_1_BIT;
            }
        }
        else if(mr_field == REVISION_ID_2) //REVISION_ID_2 = 2
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].revision_id_2[0] != mr_value)

            {
                g_target_mismatch_mask |= REVISION_ID_2_BIT;
            }
        }
        else if(mr_field == DENSITY) //DENSITY = 3
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_density[0] != (mr_value >> 2))

            {
                g_target_mismatch_mask |= DENSITY_BIT;
            }
        }
        else if(mr_field == IO_WIDTH) //IO_WIDTH = 4
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_io_width[0] != (mr_value >> 6))

            {
                g_target_mismatch_mask |= IO_WIDTH_BIT;
            }
        }
        else if(mr_field == REVISION_ID_1_RANK_1) //REVISION_ID_1_RANK_1 = 5
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].revision_id_1[1] != mr_value)

            {
                g_target_mismatch_mask |= REVISION_ID_1_RANK_1_BIT;
            }
        }
        else if(mr_field == REVISION_ID_2_RANK_1) //REVISION_ID_2_RANK_1 = 6
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].revision_id_2[1] != mr_value)

            {
                g_target_mismatch_mask |= REVISION_ID_2_RANK_1_BIT;
            }
        }
        else if(mr_field == DENSITY_RANK_1) //DENSITY_RANK_1 = 7
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_density[1] != (mr_value >> 2))

            {
                g_target_mismatch_mask |= DENSITY_RANK_1_BIT;
            }
        }
        else if(mr_field == IO_WIDTH_RANK_1) //IO_WIDTH_RANK_1 = 8
        {

            if(ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].device_io_width[1] != (mr_value >> 6))

            {
                g_target_mismatch_mask |= IO_WIDTH_RANK_1_BIT;
            }
        }
        else
        {
            return FALSE;
        }
    }
    else if(subtype == FREQUENCY_MASK) //FREQUENCY_MASK = 1
    {
        freq_mask_subtype = (entry & FREQ_MASK_SUBTYPE_MASK) >> FREQ_MASK_SUBTYPE_SHIFT;
        // FREQ_MASK_SUBTYPE_MASK 0x0C00
        // FREQ_MASK_SUBTYPE_SHIFT = 10
        if(freq_mask_subtype == LOW_FREQ) //LOW_FREQ = 0
        {
            g_freq_mask &= HIGH_FREQ_MASK << HIGH_FREQ_SHIFT;
            g_freq_mask |= (entry & LOW_FREQ_MASK); 
            // HIGH_FREQ_MASK = 0x007F
            // HIGH_FREQ_SHIFT = 8
            // LOW_FREQ_MASK = 0x00FF
            
            // Check if high frequency mask should be cleared
            if(entry & CLEAR_BIT_MASK) // CLEAR_BIT_MASK = 0x0200
            {
                g_freq_mask &= LOW_FREQ_MASK;
            }
        }
        else if(freq_mask_subtype == HIGH_FREQ) //HIGH_FREQ = 3
        {
            g_freq_mask &= LOW_FREQ_MASK;
            g_freq_mask |= (entry & HIGH_FREQ_MASK) << HIGH_FREQ_SHIFT;
            // LOW_FREQ_MASK = 0x00FF
            // HIGH_FREQ_MASK = 0x007F
            // HIGH_FREQ_SHIFT = 8

            // Check if low frequency mask should be cleared
            if(entry & CLEAR_BIT_MASK) // CLEAR_BIT_MASK = 0x0200
            {
                g_freq_mask &= HIGH_FREQ_MASK << HIGH_FREQ_SHIFT;
            }
        }
        else
        {
            return FALSE;
        }

        DDRSS_eCDT_Populate_PRFS_MC_Masks(ddr);
    }

    //AC TIMINGS-3 SEE eCDT User Guide 2.1 for more detail
    else if(subtype == ACTIMINGS3) // ACTIMINGS3 is 2
    {
        //AC TIMINGS-3 CODE HERE
#ifdef AC_TIMING_SEQUENCE_MODE
        if(g_ac_timing_data_ready != ACTIMING12_FLAG) //ACTIMING12_FLAG = 0x03
        {
            return FALSE;
        }
        else
        {
#endif
            uint32 temp = (uint32) entry;
#ifndef AC_TIMING_SEQUENCE_MODE
            g_ac_timing_value &=~(ACTIMING3_MASK << ACTIMING3_SHFT); 
#endif
            g_ac_timing_value |= ((temp & ACTIMING3_MASK) << ACTIMING3_SHFT); 
                //AC Timing Value bt 16-19 is ACTIMING-3 bit 8 - 11 
                //see eCDT User Guide 2.1 for detail
                //ACTIMING3_MASK == 0x00000F00
                //ACTIMING3_SHFT == 8
#ifndef AC_TIMING_SEQUENCE_MODE
            g_ac_timing_value &=~(ACTIMING_NCK_MASK << ACTIMING_NCK_SHFT);
#endif
            g_ac_timing_value |= ((temp & ACTIMING_NCK_MASK) << ACTIMING_NCK_SHFT);
                // bit 1-7 of AC Timing-3 copy to ac timing value bit 20 to 27 
                //see eCDT User Guide 2.1 for more detail
                //ACTIMING_NCK_MASK == 0x000000FF
                //ACTIMING_NCK_SHFT == 20

#ifndef AC_TIMING_SEQUENCE_MODE    
            //AC3 Value Ready Flag Set
            g_ac_timing_data_ready |= ACTIMING3_FLAG; // ACTIMING3_FLAG == 0x04

            if(g_ac_timing_data_ready == ACTIMING_RDY_FLAG) // ACTIMING_RDY_FLAG =7
            {
#endif
            if(DDRSS_eCDT_AC_Timing_Exe()==TRUE)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    // Voltage Plan See eCDT User Guide 2.1 for more detail
    else if(subtype == VOLTAGEPLAN) // VOLTAGEPLAN is 3
    {
        // Voltage Plan Code Here
        // Voltage Plan Parameter set here see eCDT 2.1 User Guide page 18 for more detail
        for(freq_index = 0; freq_index < MAX_NUM_CLOCK_PLAN; freq_index++)
        {
            if((g_freq_mask >> freq_index) & 1)
            {
                //Voltage Plan Mask and Shift based on eCDT User Guide 2.1 pg 8
                rail_index = (uint8)(entry & VDDCX_MASK); //VDDCX_MASK = 0x000F
                if(rail_index >= VOLTAGE_RAIL_LIMIT)
                {
                    return FALSE;
                }
                ddr->aop_params.mc_states[freq_index].vddcx = voltage_rail[rail_index];
                rail_index = (uint8)((entry & VDDMX_MASK) >> VDDMX_SHFT);
                // VDDMX_MASK = 0X00F0 VDDMX_SHFT = 4
                if(rail_index >= VOLTAGE_RAIL_LIMIT)
                {
                    return FALSE;
                }
                ddr->aop_params.mc_states[freq_index].vddmx = voltage_rail[rail_index];
                rail_index = (uint8)((entry & VDDA_MASK) >> VDDA_SHFT);
                // VDDA_MASK = 0X0F00 VDDA_SHFT = 8
                if(rail_index >= VOLTAGE_RAIL_LIMIT)
                {
                    return FALSE;
                }
                ddr->aop_params.mc_states[freq_index].vdda = voltage_rail[rail_index];
            }
        }
    }

    return TRUE;
}

//================================================================================================//
//DDRSS_eCDT_Decode_Parameter_Entry
//    To decode Parameter Entry
//================================================================================================//
/** @API 5. DDRSS_eCDT_Decode_Parameter_Entry
*    @brief DDRSS_eCDT_Decode_Parameter_Entry has two different sub entries.
*    MSM_PARAMETER_TYPE and DRAM_PARAMETER_TYPE.
*    MSM_PARAMETER_TYPE applies the settings to all the PRFS registers while DRAM_PARAMETER_TYPE applies settings to all the BIMC registers.
*/
#define PRFS_OFFSET 0x4

boolean DDRSS_eCDT_Decode_Parameter_Entry(BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel, uint16 entry)
{
    uint8  ch = 0;
    uint8  type = 0;
    uint8  shift = 0;
    uint8  index = 0;
    uint8  value = 0;
    uint8  mc_index = 0;
    uint8  phy_mask = 0;
    uint8  phy_index = 0;
    uint8  prfs_index = 0;
    uint32 mask = 0;
    uint32 reg_offset = 0;
    uint32 channels = ddr->shrm_ddr_ptr->misc.enabled_channels;

    PHY_BUS dq = DQ;
    PHY_BUS ca = CA;
    type =  (uint8)((entry & PARAMETER_TYPE_MASK) >> PARAMETER_TYPE_SHIFT);
    index = (uint8)((entry & PARAMETER_INDEX_MASK) >> PARAMETER_INDEX_SHIFT); 
    value = (uint8)(entry & PARAMETER_VALUE_MASK);

    // eCDT 2.0 -> 2.1 Added parameter From Here
    uint8   safe_mode_parameter = 0;
    // eCDT 2.0 -> 2.1 Added parameter End Here

    if(type == DRAM_PARAMETER) //DRAM_PARAMETER = 2
    {
        // Storing mode registers value at respective places w.r.t frequency in ecdt runtime bimc structure
        for(mc_index = 0; mc_index < NUM_FREQ_SWITCH_STRUCTS; mc_index++)
        {
            if(((g_mc_mask >> mc_index) & 0x1)
                || (index == DRAM_DQ_VOH)) // Set PU-Cal the same for all frequencies
            {
                if(index == DRAM_CA_ODT)
                {
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR11 &= 0x8F;
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR11 |= (value << 4);
                }
                else if(index == DRAM_DQ_ODT)
                {
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR11 &= 0xF8;
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR11 |= value;
                }
                else if(index == DRAM_DQ_VOH)
                {
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR3 &= 0xFE;
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR3 |= value;
                }
                else if(index == DRAM_DQ_PD)
                {
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR3 &= 0xC7;
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR3 |= (value << 3);
                }
                //else if(index == DRAM_DQ_PU)
                //{
                //    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR22 &= 0xF8;
                //    ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR22 |= value;
                //}
                else
                {
                    return FALSE;
                }
            }
        } // mc_index
    }
    else if(type == MSM_PARAMETER) //MSM_PARAMETER = 0
    {
        if(index < DQ_PU)
        {
            phy_mask = 0x1;
        }
        else
        {
            phy_mask = 0x6;
        }
        
       for(ch = 0; ch < NUM_CH; ch++)
        {
             // Skip channels which are not enabled
             if(((channels >> ch) & 0x1) == 0x0) continue;
                for(prfs_index = 0; prfs_index < NUM_PRFS_BANDS; prfs_index++)
                {
                    if((g_prfs_mask >> prfs_index) & 0x1)
                    {
                        for(phy_index = 0; phy_index < (ca + dq); phy_index++)
                        {
                            if((phy_mask >> phy_index) & 0x1)
                            {
                                reg_offset = REG_OFFSET_DDR_PHY_CH(ch) + (PRFS_OFFSET * prfs_index) + (DDR_PHY_OFFSET * phy_index);
                                
                                if((index == DQ_PU) || (index == CA_PU))
                                {
                                    mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, PULL_UP_DQ_CNTL);
                                    shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, PULL_UP_DQ_CNTL);
                                }
                                else if((index == DQ_PD) || (index == CA_PD))
                                {
                                    mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODRV_DQ);
                                    shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODRV_DQ);
                                }
                                else if(index == DQ_ODT)
                                {
                                    mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODT_IN_DQ);
                                    shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODT_IN_DQ);
                                }
                                else if((index == DQ_VOH) || (index == DQS_VOH)
                                    || (index == CA_VOH) || (index == CK_VOH))
                                {
                                    mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, DRIVER_VOH_MODE);
                                    shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, DRIVER_VOH_MODE);
                                }
                                else if((index == DQS_PU) || (index == CK_PU))
                                {
                                    mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, PULL_UP_DQS_CNTL);
                                    shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, PULL_UP_DQS_CNTL);
                                }
                                else if((index == DQS_PD) || (index == CK_PD))
                                {
                                    mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODRV_DQS);
                                    shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODRV_DQS);
                                }
                                else if(index == DQS_ODT)
                                {
                                    mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODT_IN_DQS);
                                    shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODT_IN_DQS);
                                }
                                else
                                {
                                    return FALSE;
                                }
                                
                                // Read, modify, and write register
                                HWIO_OUTXM(reg_offset, DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, mask, (uint32)(value) << shift);
                            }
                        } // phy_index
                    }
                } // prfs_index

        } // ch
    }
    else if(type == SAFE_MODE) // SAFE_MODE == 1
    {
        index = ((entry & SAFE_MODE_INDEX_MASK) >> SAFE_MODE_INDEX_SHFT); 
            // SAFE_MODE_INDEX_SHFT == 11
            // SAFE_MODE_INDEX_MASK == 0x0800
            // See eCDT Uer Guide 2.1 pg 8 for more detail
        value = entry & 0x01; // GET LAST BIT OF entry
        if(index == 0)
        {
            safe_mode_parameter = (uint8)((entry & SAFE_MODE_FEATURE) >> SAFE_MODE_FEATURE_SHFT);
                //SAFE_MODE_FEATURE = 0x001E, SAFE_MODE_FEATURE_SHFT=1
                // See eCDT Uer Guide 2.1 pg 8 for more detail

            //Safe Mode Feature Operation detail at eCDT 2.1 pg 19
            switch(safe_mode_parameter)
            {
                case SELF_REFRESH_MODE:
                    ddr->shrm_ddr_ptr->ddr_runtime.hw_self_refresh_enable = value;
                    break;
                case REFRESH_RATE_POLLING:
                     ddr->shrm_ddr_ptr->ddr_runtime.MR4_polling_enable = value;
                    break;
                case PERIODIC_ZQ_CAL:
                    ddr->shrm_ddr_ptr->ddr_runtime.periodic_zq_calibration_enable = value;
                    break;
                case PERIODIC_TRAINING:
                    ddr->shrm_ddr_ptr->ddr_runtime.periodic_training_enable = value;
                    break;
                case PARTIAL_ARRAY_SELF_REFRESH:
                    ddr->shrm_ddr_ptr->ddr_runtime.partial_array_self_refresh_enable = value;
                    break;
                case POWER_DOWN:
                    ddr->shrm_ddr_ptr->ddr_runtime.power_down_enable = value;
                    break;
                case CLOCK_STOP:
                    ddr->shrm_ddr_ptr->ddr_runtime.clock_stop_enable = value;
                    break;
                case AUTO_PRECHARGE:
                    ddr->shrm_ddr_ptr->ddr_runtime.auto_precharge_enable = value;
                    break;
                case LPDDR4Y_LP_CA_BUFFER_SUPPORT:
                    if(value == 0x01)
                    {
                        ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support |= 0x01;
                        // Set the last bit
                    }
                    else
                    {
                        ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support &=~ 0x01;
                        // clear the last bit
                    }
                    break;
                case LPDDR4Y_SINGLE_ENDED_CLOCK_SUPPORT:
                    if(value == 0x01)
                    {
                        ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support |= 0x02;
                        // set the bit 1
                    }
                    else
                    {
                        ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support &=~ 0x02;
                        // Clear the bit 1
                    }
                    break;
                case LPDDR4Y_SINGLE_ENDED_WR_DQS_SUPPORT:
                    if(value == 0x01)
                    {
                        ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support |= 0x04;
                        // set the bit 1
                    }
                    else
                    {
                        ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support &=~ 0x04;
                        // Clear the bit 1
                    }
                    break;
                case LPDDR4Y_SINGLE_ENDED_RD_DQS_SUPPORT:
                    if(value == 0x01)
                    {
                        ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support |= 0x08;
                        // set the bit 1
                    }
                    else
                    {
                        ddr->shrm_ddr_ptr->ddr_runtime.lpddr4y_support &=~ 0x08;
                        // Clear the bit 1
                    }
                    break;
                case BASE_LOGGING:
                    ddr->shrm_ddr_ptr->ddr_runtime.base_logging_enable = value;
                    break;
                case EXTENDED_LLCC_LOGGING:
                    ddr->shrm_ddr_ptr->ddr_runtime.llcc_extended_logging_enable = value;
                    break;
                case EXTENDED_MC_LOGGING:
                    ddr->shrm_ddr_ptr->ddr_runtime.mc_extended_logging_enable = value;
                    break;
                case EXTENDED_PHY_LOGGING:
                    ddr->shrm_ddr_ptr->ddr_runtime.phy_extended_logging_enable = value;
                
            }
        }
        else
        {
            // MRR entry uses an entry which was before reserved under safe mode
            if((entry & MRR_ENTRY_MASK) == MRR_ENTRY_VALUE)
            {
                index = entry & MRR_ADDR_MASK;
                value = MC_MR_Read(REG_OFFSET_MC_BROADCAST, DDR_CS0, index) & 0xFF;
                
                // Save the value read to internal memory which will be dumped after watchdog reset
                if(mrr_data_index < MRR_DATA_MAX)
                {
                    mrr_data_ptr[mrr_data_index] = value;
                    mrr_data_index++;   
                }
                
                ddr_printf(DDR_NORMAL, "eCDT MRR - MA[5:0] = 0x%02x, OP[7:0] = 0x%02x\n", index, value);
            }
            else
            {
                // Safe Mode Frequency Enable Bit triggers nothing at 1
                if(value == 1)
                {
                    return FALSE;
                }
                else
                {
                    // Safemode Frequency Enable is 0, update the switchboard
                    return DDRSS_eCDT_Switchboard_Update(ddr);
                }
            }
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

//================================================================================================//
//DDRSS_eCDT_Decode_TMRS_Entry
//    To decode TMRS Entry
//================================================================================================//
/** @API 3. DDRSS_eCDT_Decode_TMRS_Entry
*    @brief DDRSS_eCDT_Decode_TMRS_Entry triggers a mode register write to all channels and ranks.
*/
boolean DDRSS_eCDT_Decode_TMRS_Entry(BOOT_DDR_STRUCT *ddr, DDR_CHANNEL channel, uint16 entry)
{
    uint8 ch = 0;
    uint8 cs = 0;
    uint8 mr_addr = 0;
    uint8 mr_value = 0;
    DDR_CHIPSELECT populated_chipselect = DDR_CS_NONE;
    
    // Get mode register address
    mr_addr = (entry & MR_ADDR_MASK) >> MR_ADDR_SHIFT;
    
    // Get mode regsiter value
    mr_value = entry & MR_VALUE_MASK;
    
    ddr_printf(DDR_NORMAL, "eCDT MRW - MA[5:0] = 0x%02x, OP[7:0] = 0x%02x\n", mr_addr, mr_value);
    
    for(ch = 0; ch < NUM_CH; ch++)
    {
        if((channel >> ch) & 0x1)
        {
            // Get populated chipselects
            populated_chipselect = ddr->shrm_ddr_ptr->detected_ddr_device[TARGET_ACTIVE_CHANNEL].populated_chipselect;
            
            for(cs = 0; cs < NUM_CS; cs++)
            {
                if((populated_chipselect >> cs) & 0x1)
                {
                    // Write mode register
                    MC_MR_Write(REG_OFFSET_MC_CH(ch), CS_1HOT(cs), mr_addr, mr_value);
                }
            }
        }
    }

    return TRUE;
}

//================================================================================================//
//DDRSS_eCDT_Populate_PRFS_MC_Masks
//    To populate PRFS and MC index masks
//================================================================================================//

/** @API 4. DDRSS_eCDT_Populate_PRFS_MC_Masks
*    @brief DDRSS_eCDT_Populate_PRFS_MC_Masks
*/
void DDRSS_eCDT_Populate_PRFS_MC_Masks(BOOT_DDR_STRUCT *ddr)
{
    uint8 freq_index = 0;
    uint8 prfs_index = 0;
    uint8 mc_index = 0;
    uint32 clk_khz = 0;
    uint32 clk_in_ps = 0;

    // Clear PRFS and MC index masks
    g_prfs_mask = 0;
    g_mc_mask = 0;

    // Populate PRFS and MC index masks
    for(freq_index = 0; freq_index < MAX_NUM_CLOCK_PLAN; freq_index++)
    {
        if((g_freq_mask >> freq_index) & 1)
        {
            clk_khz = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[freq_index].clk_freq_in_khz;

            prfs_index = DDRSS_Get_Freq_Index(ddr, clk_khz);
            g_prfs_mask |= (1 << prfs_index);

            clk_in_ps = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[freq_index].clk_period_in_psec;

            mc_index = MC_Freq_Switch_Params_Index(ddr->shrm_ddr_ptr, clk_in_ps);
            g_mc_mask |= (1 << mc_index);
        }
    }
}

//================================================================================================//
//DDRSS_eCDT_Read_Parameter
//    To read parameter default values
//================================================================================================//

/** @API 4. DDRSS_eCDT_Read_Parameter
*    @brief DDRSS_eCDT_Read_Parameter
*/
int HAL_DDR_Read_eCDT_Parameter_Values(BOOT_DDR_STRUCT *ddr, uint32 clk_freq_khz, void* buffer_ptr, size_t buffer_size)
{
    uint8 type = 0;
    uint8 index = 0;
    uint8 mc_index = 0;
    uint8 prfs_index = 0;
    uint8 phy_index = 0;
    uint8 clk_index = 0;
    uint8 param_index = 0;
    uint8 shift = 0;
    uint32 mask = 0;
    uint32 reg_offset = 0;
    uint8 value = 0;
    uint32 clk_in_ps = 0;
    uint8 *output_ptr = NULL;
    int out_count = 0;
    
    uint8 parameters[] = 
    {
        CA_PU,
        CA_PD,
        CA_VOH,
        CK_PU,
        CK_PD,
        //CK_VOH,
        DQ_PU,
        DQ_PD,
        DQ_VOH,
        DQ_ODT,
        DQS_PU,
        DQS_PD,
        //DQS_VOH,
        DQS_ODT,
        DRAM_CA_ODT,
        DRAM_DQ_ODT,
        DRAM_DQ_VOH,
        //DRAM_DQ_PU,
        DRAM_DQ_PD,
    };
    
    // Check buffer is large enough
    if(buffer_size < (sizeof(parameters) * 2))
    {
        return -1;
    }
    
    // Convert point to easier type to work with
    output_ptr = (uint8*)buffer_ptr;
    
    // Get index into clock plan
    clk_index = DDRSS_Find_Clk_Index(ddr, clk_freq_khz);
    
    for(param_index = 0; param_index < sizeof(parameters); param_index++)
    {
        type = (parameters[param_index] >> 4) & 0x3;
        index = parameters[param_index];

        if(type == DRAM_PARAMETER)
        {
            clk_in_ps = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[clk_index].clk_period_in_psec;
            mc_index = MC_Freq_Switch_Params_Index(ddr->shrm_ddr_ptr, clk_in_ps);

            if(index == DRAM_CA_ODT)
            {
                value = (ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR11 & 0x70) >> 4;
            }
            else if(index == DRAM_DQ_ODT)
            {
                value = ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR11 & 0x7;
            }
            else if(index == DRAM_DQ_VOH)
            {
                value = ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR3 & 0x1;
            }
            else if(index == DRAM_DQ_PD)
            {
                value = (ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR3 & 0x38) >> 3;
            }
            //else if(index == DRAM_DQ_PU)
            //{
            //    value = ddr->shrm_ddr_ptr->ddr_runtime.mc_freq_switch[mc_index].MR22 & 0x7;
            //}
            else
            {
                return -1;
            }
        }
        else if(type == MSM_PARAMETER)
        {
            prfs_index = DDRSS_Get_Freq_Index(ddr, clk_freq_khz);
            
            if(index < DQ_PU)
            {
                phy_index = 0;
            }
            else
            {
                phy_index = 1;
            }
            
            reg_offset = REG_OFFSET_DDR_PHY_CH(7) + (PRFS_OFFSET * prfs_index) + (DDR_PHY_OFFSET * phy_index);  //channel modified for poipu
            
            if((index == DQ_PU) || (index == CA_PU))
            {
                mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, PULL_UP_DQ_CNTL);
                shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, PULL_UP_DQ_CNTL);
            }
            else if((index == DQ_PD) || (index == CA_PD))
            {
                mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODRV_DQ);
                shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODRV_DQ);
            }
            else if(index == DQ_ODT)
            {
                mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODT_IN_DQ);
                shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODT_IN_DQ);
            }
            //else if((index == DQ_VOH) || (index == DQS_VOH)
            //    || (index == CA_VOH) || (index == CK_VOH))
            else if((index == DQ_VOH) || (index == CA_VOH))
            {
                mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, DRIVER_VOH_MODE);
                shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, DRIVER_VOH_MODE);
            }
            else if((index == DQS_PU) || (index == CK_PU))
            {
                mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, PULL_UP_DQS_CNTL);
                shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, PULL_UP_DQS_CNTL);
            }
            else if((index == DQS_PD) || (index == CK_PD))
            {
                mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODRV_DQS);
                shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODRV_DQS);
            }
            else if(index == DQS_ODT)
            {
                mask = HWIO_FMSK(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODT_IN_DQS);
                shift = HWIO_SHFT(DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, ODT_IN_DQS);
            }
            else
            {
                return -1;
            }
            
            // Read bit field
            value = HWIO_INXM(reg_offset, DDR_PHY_DDRPHY_PADEXT_PRFS_0_FPM_CNTL0_CFG, mask) >> shift;
        }
        else
        {
            return -1;
        }
        
        // Save paramter index and value
        output_ptr[param_index * 2] = index;
        output_ptr[(param_index * 2) + 1] =  value;
        out_count+=2;
    } // param_index
    
    return out_count;
}

// AC Timing Function detail see eCDT User Guide 2.1 pg 16
uint16 g_addr_offset[AC_TIMING_PAR_INDEX_RANGE] =
{
    ADDR_OFFSET_TZQLAT,
    ADDR_OFFSET_TSR,
    ADDR_OFFSET_TXSR,
    ADDR_OFFSET_TXSR_ABORT,
    ADDR_OFFSET_TXP,
    ADDR_OFFSET_TCCD,
    ADDR_OFFSET_TRTP,
    ADDR_OFFSET_TRCD,
    ADDR_OFFSET_TRPPB,
    ADDR_OFFSET_TRPAB,
    ADDR_OFFSET_TRAS, 
    ADDR_OFFSET_TRRD, 
    ADDR_OFFSET_TPPD, 
    ADDR_OFFSET_TFAW, 
    ADDR_OFFSET_TDQSCKMIN, 
    ADDR_OFFSET_TDQSCKMAX, 
    ADDR_OFFSET_TFC, 
    ADDR_OFFSET_TREFI, 
    ADDR_OFFSET_TMMR, 
    ADDR_OFFSET_TMRW, 
    ADDR_OFFSET_TMRD, 
    ADDR_OFFSET_TODTONMIN, 
    ADDR_OFFSET_TODTONMAX, 
    ADDR_OFFSET_TODTOFFMIN, 
    ADDR_OFFSET_TODTOFFMAX, 
    ADDR_OFFSET_TCKE, 
    ADDR_OFFSET_TCMDCKE, 
    ADDR_OFFSET_TCKELCK, 
    ADDR_OFFSET_TCKCKEH, 
    ADDR_OFFSET_TMRWCKEL
};

//================================================================================================//
//HAL_DDR_Read_eCDT_AC_Timing_Values
//    Reads registers containing AC timing values which can be overriden through eCDT
//    and writes them to a buffer
//================================================================================================//
int HAL_DDR_Read_eCDT_AC_Timing_Values(void* buffer_ptr, size_t buffer_size)
{
    uint8 i = 0;
    uint32 value = 0;
    uint32 *output_ptr = NULL;
    int out_count = 0;
    
    // Check buffer is large enough
    if(buffer_size < (sizeof(g_addr_offset) * 4))
    {
        return -1;
    }
    
    // Convert point to easier type to work with
    output_ptr = (uint32*)buffer_ptr;
    
    // Loop through AC timing values
    for(i = 0; i < (sizeof(g_addr_offset) / sizeof(uint16)); i++)
    {
        // Read value
        if(i == INDEX_TREFI) //tREFI Special Case
        {
            value = HWIO_INXFI(REG_OFFSET_MC_SHKE_BROADCAST, SHKE_REFRESH_TIMER_CTRL_RANK_n, 0, TREFI);
            value = (CXO_PERIOD_IN_PS * value) / 1000; // converting from cycles of CXO (19.2MHz clock) to ns
        }
        else
        {
            value = in_dword(REG_OFFSET_MC_DTC_BROADCAST + g_addr_offset[i]);
        }
        
        // Save AC timing value
        output_ptr[i] =  value;
        out_count += 4;
    }
    
    return out_count;
}

//================================================================================================//
//DDRSS_eCDT_AC_Timing_Exe
//    Assign Global Variable g_ac_timing_value to the according Register 
//    based on the Global Index Value g_ac_timing_par_index
//    Clear the Global Data Ready Flag g_ac_timing_data_ready
//================================================================================================//
boolean DDRSS_eCDT_AC_Timing_Exe(void)
{
    g_ac_timing_data_ready = 0; // Reset AC Timing Data Ready Flag
    uint32 temp;

    if(g_ac_timing_par_index == INDEX_TREFI) //tREFI Special Case
    {
        temp = g_ac_timing_value & AC_VALUE_MASK; // The value is the last 20 bits
        temp = (temp * 1000) / CXO_PERIOD_IN_PS; // converting from ns to cycles of CXO (19.2MHz clock)
        
        HWIO_OUTXFI(REG_OFFSET_MC_SHKE_BROADCAST, SHKE_REFRESH_TIMER_CTRL_RANK_n, 0, TREFI, temp);
        HWIO_OUTXFI(REG_OFFSET_MC_SHKE_BROADCAST, SHKE_REFRESH_TIMER_CTRL_RANK_n, 1, TREFI, temp);
    }
    else
    {
        out_dword(REG_OFFSET_MC_DTC_BROADCAST + g_addr_offset[g_ac_timing_par_index], g_ac_timing_value);
    } 
    //g_ac_timing_value = 0; // clear the global variable after register setting
    return TRUE;
}

//================================================================================================//
//DDRSS_eCDT_Switchboard_Update
//    Find new ddr max frequency from Frequency Mask
//    Update runtime max_ddr_frequency to the new frequency if it different
//    Update the Switchboard with new frequency 
//================================================================================================//
boolean DDRSS_eCDT_Switchboard_Update(BOOT_DDR_STRUCT *ddr)
{
    // get old max frequency from runtime
    uint32 old_max_ddr_frequency = ddr->shrm_ddr_ptr->ddr_runtime.max_ddr_frequency;
    uint32 new_max_ddr_frequency = 0;
    // get old prfs frequency index with old ddr max frequency
    uint8 old_prfs_index = DDRSS_Get_Freq_Index(ddr, old_max_ddr_frequency);
    uint8 freq_index = 0;
    uint8 freq_enable = 0;
    uint8 new_prfs_index;
    uint8 ii;
    
    
    // Sweep the frequency mask and find the new max_ddr_frequency
    for(freq_index = 0; freq_index < MAX_NUM_CLOCK_PLAN; freq_index++)
    {
        freq_enable = ((g_freq_mask >> freq_index) & 1);
        // according to the frequency mask, set the freq_enable 
        ddr-> aop_params. mc_states[freq_index].freq_enable = freq_enable;
        if(freq_enable)
        {
            new_max_ddr_frequency = ddr->shrm_ddr_ptr->ddr_runtime.clock_plan[freq_index].clk_freq_in_khz;
        }
    }
    
    // convert new_ddr_frequency to new prfs frequency index
    // if there is no valid input frequency, return false
    if(new_max_ddr_frequency == 0)
    {
        return FALSE;
    }
    else
    {
        new_prfs_index = DDRSS_Get_Freq_Index(ddr, new_max_ddr_frequency);
    }
    
    // if the new frequency is less than 1017.6MHz(prfs index 3),do nothing and return false
    if(new_prfs_index < 3)
    {
        return FALSE;
    }
    
    // if the frequency is not changed, we do nothing and return false. 
    if(new_max_ddr_frequency == old_max_ddr_frequency)
    {
        return TRUE;
    }
    else
    {
        // check for index boundary
        if((old_prfs_index < NUM_PRFS_BANDS) && (new_prfs_index < NUM_PRFS_BANDS))
        {
            // if the frequency is changed, we update the runtime max_ddr_frequency
            ddr->shrm_ddr_ptr->ddr_runtime.max_ddr_frequency = new_max_ddr_frequency;
        
            // For all the switchboard, if the old frequency switch board returns 1, 
            // we need to find out if it is H by check the lower index
            // if it is H, we clear the index bit, and set the new_ddr_frequency index bit
            // Training_pivot_plots_Switchboard 
            for(ii = 0; ii < 2; ii++)
            {
                //only update the switch board, when this frequency was set
                if(ddr->training_params.Training_pivot_plots_Switchboard[old_prfs_index][ii] == 1)
                {
                    // Check for H
                    // array index protection
                    if(old_prfs_index <1)
                    {
                        return FALSE;
                    }
                    else if(ddr->training_params.Training_pivot_plots_Switchboard[old_prfs_index - 1][ii] == 0)
                    {
                        // Clear the old frequency bit
                        ddr->training_params.Training_pivot_plots_Switchboard[old_prfs_index][ii] = 0;
                        // Set the new frequence bits
                        ddr->training_params.Training_pivot_plots_Switchboard[new_prfs_index][ii] = 1;
                    }
                }
            }
            //DCC_Training_Switchboard
            for(ii = 0; ii < 1; ii++)
            {
                //only update the switch board, when this frequency was set
                if(ddr->training_params.DCC_Training_Switchboard[old_prfs_index][ii] == 1)
                {
                    // Check for H
                    // array index protection
                    if(old_prfs_index <1)
                    {
                        return FALSE;
                    }
                    else if(ddr->training_params.DCC_Training_Switchboard[old_prfs_index - 1][ii] == 0)
                    {
                        // Clear the old frequency bit
                        ddr->training_params.DCC_Training_Switchboard[old_prfs_index][ii] = 0;
                        // Set the new frequence bits
                        ddr->training_params.DCC_Training_Switchboard[new_prfs_index][ii] = 1;
                    }
                }
            }
            //WRLVL_Training_Switchboard
            for(ii = 0; ii < 1; ii++)
            {
                //only update the switch board, when this frequency was set
                if(ddr->training_params.WRLVL_Training_Switchboard[old_prfs_index][ii] == 1)
                {
                    // Check for H
                    // array index protection
                    if(old_prfs_index <1)
                    {
                        return FALSE;
                    }
                    else if(ddr->training_params.WRLVL_Training_Switchboard[old_prfs_index - 1][ii] == 0)
                    {
                        // Clear the old frequency bit
                        ddr->training_params.WRLVL_Training_Switchboard[old_prfs_index][ii] = 0;
                        // Set the new frequence bits
                        ddr->training_params.WRLVL_Training_Switchboard[new_prfs_index][ii] = 1;
                    }
                }
            }
            //WR_Training_Switchboard
            for(ii = 0; ii < 5; ii++)
            {
                //only update the switch board, when this frequency was set
                if(ddr->training_params.WR_Training_Switchboard[old_prfs_index][ii] == 1)
                {
                    // Check for H
                    // array index protection
                    if(old_prfs_index <1)
                    {
                        return FALSE;
                    }
                    else if(ddr->training_params.WR_Training_Switchboard[old_prfs_index - 1][ii] == 0)
                    {
                        // Clear the old frequency bit
                        ddr->training_params.WR_Training_Switchboard[old_prfs_index][ii] = 0;
                        // Set the new frequence bits
                        ddr->training_params.WR_Training_Switchboard[new_prfs_index][ii] = 1;
                    }
                }
            }
            //RD_Training_Switchboard
            for(ii = 0; ii < 5; ii++)
            {
                //only update the switch board, when this frequency was set
                if(ddr->training_params.RD_Training_Switchboard[old_prfs_index][ii] == 1)
                {
                    // Check for H
                    // array index protection
                    if(old_prfs_index <1)
                    {
                        return FALSE;
                    }
                    else if(ddr->training_params.RD_Training_Switchboard[old_prfs_index - 1][ii] == 0)
                    {
                        // Clear the old frequency bit
                        ddr->training_params.RD_Training_Switchboard[old_prfs_index][ii] = 0;
                        // Set the new frequence bits
                        ddr->training_params.RD_Training_Switchboard[new_prfs_index][ii] = 1;
                    }
                }
            }
            
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}