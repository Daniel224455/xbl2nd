#ifndef __EDID_H__
#define __EDID_H__
/*=============================================================================
 
  File: edid.h
 
  EDID headers and utilitiy functions
  
  $Header: //components/rel/boot.xf/2.2/QcomPkg/Library/HALDPLib/edid.h#2 $
 
  Copyright (c) 2010-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/

/* EDID handle */
typedef void                                         *EDID_HandleType;

/* -----------------------------------------------------------------------
**  Defines
** ----------------------------------------------------------------------- */
#define EDID_BLOCK_SEGMENT_ADDR                      (0x60)   //EDID is separated by multiple segments for multiple EDID blocks.
#define EDID_BLOCK_ZERO_DEVICE_ADDR                  (0xA0)   //Block zero device address when reading using DDC protocol
#define EDID_BLOCK_SIZE                              (0x80)   //Each page size in the EDID ROM
#define EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS         (0x4)    //Tentative to 4 blocks but theoretical maximum could be 128
#define EDID_MAX_NUM_OF_VIDEO_DATA_BLOCKS            (0x4)    //Tentative to 4 blocks but theoretical maximum could be 128
#define EDID_MAX_NUM_OF_BLOCKS                       (0x80)   //Maximum number of pages for EDID structure
#define EDID_EXTENSION_BLOCK_MAP_TAG_CODE            (0xF0)   //Indicates that this extension block lists all the extension tags of subsequent extension blocks.
#define EDID_VIDEO_DATA_BLOCK_TAG_CODE               (0x2)    //Video data block tag code
#define EDID_AUDIO_DATA_BLOCK_TAG_CODE               (0x1)    //Audio data block tag code
#define EDID_SPEAKER_ALLOC_DATA_BLOCK_TAG_CODE       (0x4)    //Speaker allocation data block tag code
#define EDID_VENDOR_SPECIFIC_DATA_BLOCK_TAG_CODE     (0x3)    //Vendor specific data block tag code
#define EDID_EXTENDED_DATA_BLOCK_TAG_CODE            (0x7)    //Extended data block tag code
#define EDID_EXTENDED_VIDEO_CAP_TAG_CODE             (0x0)    //Extended tag code for video capability data block
#define EDID_EXTENDED_VENDOR_SPECIFIC_VIDEO_TAG_CODE (0x1)    //Extended tag code for vendor specific video data block
#define EDID_EXTENDED_COLORIMETRY_TAG_CODE           (0x5)    //Extended tag code for colorimetry data block
#define EDID_EXTENDED_VENDOR_SPECIFIC_AUDIO_TAG_CODE (0x11)   //Extended tag code for vendor specific audio data block
#define EDID_VENDOR_SPECIFIC_BLOCK_IEEE_ID_LENGTH    (0x3)    //The IEEE Registration ID is 24 bit (3 bytes)
#define EDID_VENDOR_SPECIFIC_PHYSICAL_ADDR_LENGTH    (0x2)    //The physical address of the source is 16 bit (2 bytes)
#define EDID_EXTENDED_HDMI_VIDEO_DATA_BLOCK_TAG_CODE (0x4)    // Extended tag code for HDMI Video Data block indicating support of HDMI 2.0 features
#define EDID_EXTENDED_YUV420_VIDEO_BLOCK_TAG_CODE    (0xE)    // Extended tag code for YCbCr420 Video data block
#define EDID_EXTENDED_YUV420_CAPABILITY_TAG_CODE     (0xF)    // Extended tag code for YCbCr420 Capability map data block 
  
#define EDID_AUDIO_SHORT_DESC_SIZE                   (0x3)    //Audio short descriptor size
#define EDID_AUDIO_BIT_RATE_8KBPS                    (0x1F40) //8000 indicating 8Kbps 
#define EDID_MAX_NUM_OF_TAG_CODES                    (0x7)    //Max number of data block tag codes 

//Maximum number of bytes for EDID structure
#define EDID_MAX_BUF_SIZE                            (EDID_BLOCK_SIZE * EDID_MAX_NUM_OF_BLOCKS)
#define EDID_READ_TIMEOUT                            (0x3E8) //Timeout value for EDID read operation (1000ms)
#define EDID_CHECKSUM_OFFSET                         (0x7F)  //Offset to the EDID structure - Checksum byte
#define EDID_CEA_EXTENSION_FLAG_OFFSET               (0x7E)  //Offset to the EDID structure - CEC extension byte
#define EDID_FIRST_TIMING_DESC_OFFSET                (0x36)  //Offset to the EDID structure - 1st detailed timing descriptor
#define EDID_DETAIL_TIMING_RELATIVE_OFFSET           (0x02)  //Offset to the EDID structure - Indicates the starting offset inside a CEA block where the detail timing descriptor exsits.
#define EDID_DATA_BLOCK_RELATIVE_OFFSET              (0x04)  //Offset to the EDID structure - The start of any data block is always offset 4 from any CEA extension blocks.
#define EDID_VERSION_OFFSET                          (0x12)  //Offset to the EDID structure - EDID Version
#define EDID_REVISION_OFFSET                         (0x13)  //Offset to the EDID structure - EDID Revision
#define EDID_CEA_EXTENSION_VERSION_OFFSET            (0x81)  //Offset to CEA extension version number - v1,v2,v3 (v1 is seldom, v2 is obsolete, v3 most common)
#define EDID_CEA_EXTENSION_FIRST_DESC_OFFSET         (0x82)  //Offset to CEA extension first timing desc - indicate the offset of the first detailed timing descriptor
#define EDID_CEA_EXTENSION_CABS_RELATIVE_OFFSET      (0x03)  //Relative offset to CEA extension basic capabilities - indicates underscan, basic audio etc...
#define EDID_COMPONENT_PHYSICAL_ADDR_OFFSET          (0x98)  //Offset to Source Physical Address (CEC purposes) 
#define EDID_COMPONENT_PHYSICAL_ADDR_SIZE            (0x02)  //Source Physical Address is 2 bytes 
#define EDID_VIDEO_INPUT_DEFINITION_OFFSET           (0x14)  //Offset to Video input definition  - indicate the offset of the video input definition
#define EDID_VIDEO_SIGNAL_INTERFACE_MASK             (0x80)  //Mask to see whether the sink is Digital signal or analog signal
#define EDID_COLOR_BIT_DEPTH_MASK                    (0x70)  //Mask to for the color bit depth definition
#define EDID_COLOR_BIT_DEPTH_SHIFT                   (0x4)   //Color Bit depth start from bit(4) to bit(6) - zero based
#define EDID_DATA_BLOCK_LENGTH_MASK                  (0x1F)  //Lowest 5 bit mask to extract the amount of bytes inside various data blocks in the EDID structure
#define EDID_DATA_BLOCK_TAG_MASK                     (0xE0)  //7-5 bit indicates the various data block tag code 
#define EDID_DATA_BLOCK_TAG_SHIFT                    (0x5)   //Shift 5 bits to extract the various data block tag code 
#define EDID_SHORT_VIDEO_DESC_VID_MASK               (0x7F)  //Lowest 7 bit mask to extract the Video Indentification code in the EDID structure
#define EDID_DETAIL_TIMING_DESC_BLOCK_SIZE           (0x12)  //Each detailed timing descriptor has block size of 18
#define EDID_AUDIO_DESC_AUDIO_FORMAT_MASK            (0x78)  //Bit 6-3 indicates the audio format inside a short audio descriptor
#define EDID_AUDIO_DESC_AUDIO_FORMAT_SHIFT           (0x3)   //Shift 3 bits to extract the audio format inside a short audio descriptor
#define EDID_AUDIO_DESC_AUDIO_MAX_CHANNEL_MASK       (0x7)   //Bit 3-0 indicates the maximum audio channels inside a short audio descriptor
#define EDID_BASIC_AUDIO_SUPPORT_MASK                (0x40)  //Bit 6 indicates the whether the sink device supports basic audio or not
#define EDID_BASIC_AUDIO_SUPPORT_SHIFT               (0x6)   //Shift 6 to extract the basic audio support bit
#define EDID_YCBCR_SUPPORT_MASK                      (0x20)  //Bit 5-4 indicates YCbCr(422 and 444) sampling support.Sink shall set both the bits, so it is sufficient to check 1 bit only
#define EDID_YCBCR_SUPPORT_SHIFT                     (0x5)   //Shift 5 to extract YCbCr support. YCbCr420 support is indicated using YCbCr420 video data block.
  
#define EDID_MANUFACTURER_ID_OFFSET                  (0x08)   //Offset to Manufacturer ID
#define EDID_MONITOR_NAME_TAG                        (0xFC)   //Monitor name tag - Indicate this descriptor block contain Monitor Name
#define EDID_MONITOR_NAME_MAX_LENGTH                 (0x0D)   //Monitor Name's max length.
#define EDID_MONITOR_NAME_TERMINATION_CHAR           (0x0A)   //Monitor Name's termination char.
  
#define EDID_HORIZONTAL_SCREEN_SIZE_OFFSET           (0x15)  //Offset to Horizontal screen size  - indicate the offset of the horizontal screen size
#define EDID_VERTICAL_SCREEN_SIZE_OFFSET             (0x16)  //Offset to Vertical screen size  - indicate the offset of the vertical screen size
  
// Display descriptor offset
#define EDID_DISPLAY_DESC_TAG_OFFSET                        (0x03)  //Offset to Display Descriptor Tag Numbers
#define EDID_DISPLAY_DESC_ESTABLISH_TIMINGIII_TAG           (0xF7)  //Tag value for establish timing III indentificaton
#define EDID_DISPLAY_DESC_ESTABLISH_TIMINGIII_START_OFFSET  (0x06)  //Starting offset for establish timing III
  
#define EDID_DISPLAY_DESC_STANDARD_TIMING_TAG               (0xFA)  //Tag value for standing timing identification
#define EDID_DISPLAY_DESC_STANDARD_TIMING_START_OFFSET      (0x05)  //Starting offset for standing timing identification
#define EDID_DISPLAY_DESC_STANDARD_TIMING_DEFINITION_COUNT  (0x06)  //Count of additional standard timing definition
  
// Established timing offset
#define EDID_ESTABLISHED_TIMINGI_START_OFFSET            (0x23) // Established Timings I
#define EDID_ESTABLISHED_TIMINGII_START_OFFSET           (0x24) // Established Timings II
#define EDID_ESTABLISHED_TIMINGIII_START_OFFSET          (0x25) // Established Timings III
  
//Standard timing offset
#define EDID_STANDARD_TIMING_START_OFFSET                (0x26) //Standard timing start
#define EDID_STANDARD_TIMING_END_OFFSET                  (0x35) //Standard timing end
#define EDID_STANDARD_TIMING_ASPECTRATIO_MASK		         (0xC0) //Bits 6 & 7 : Aspect ratio
#define EDID_STANDARD_TIMING_ASPECTRATIO_SHIFT           (0x6)  //Shift 6 bits to extract aspect ratio
#define EDID_STANDARD_TIMING_VFREQUENCY_BITS_MASK        (0x3F) //Bits 0-5 for Vertical frequency.
#define EDID_STANDARD_TIMING_MIN_VFREQUENCY              (0x3C) //Min Vitical frequency.
#define EDID_STANDARD_TIMING_HRESOLUTION_SHIFT           (0x3)  //shift 3 bits left in order to get value.
#define EDID_STANDARD_TIMING_HRESOLUTION_BASE            (0xF8) //H resolution will be based on this number.
  
  //Offset to the Detailed Timing Descriptors
#define EDID_TIMING_PIXEL_CLOCK_LOWER_BYTE_OFFSET        (0x0)  //Relative Offset to the EDID detailed timing descriptors - Pixel Clock
#define EDID_TIMING_PIXEL_CLOCK_UPPER_BYTE_OFFSET        (0x1)  //Relative Offset to the EDID detailed timing descriptors - Pixel Clock
#define EDID_TIMING_DESC_H_ACTIVE_OFFSET                 (0x2)  //Relative Offset to the EDID detailed timing descriptors - H active
#define EDID_TIMING_DESC_H_BLANK_OFFSET                  (0x3)  //Relative Offset to the EDID detailed timing descriptors - H blank
#define EDID_TIMING_DESC_UPPER_H_NIBBLE_OFFSET           (0x4)  //Relative Offset to the EDID detailed timing descriptors - Upper 4 bit for each H active/blank field
#define EDID_TIMING_DESC_V_ACTIVE_OFFSET                 (0x5)  //Relative Offset to the EDID detailed timing descriptors - V active
#define EDID_TIMING_DESC_V_BLANK_OFFSET                  (0x6)  //Relative Offset to the EDID detailed timing descriptors - V blank
#define EDID_TIMING_DESC_UPPER_V_NIBBLE_OFFSET           (0x7)  //Relative Offset to the EDID detailed timing descriptors - Upper 4 bit for each V active/blank field
#define EDID_TIMING_DESC_H_IMAGE_SIZE_OFFSET             (0xC)  //Relative Offset to the EDID detailed timing descriptors - H image size
#define EDID_TIMING_DESC_V_IMAGE_SIZE_OFFSET             (0xD)  //Relative Offset to the EDID detailed timing descriptors - V image size
#define EDID_TIMING_DESC_IMAGE_SIZE_UPPER_NIBBLE_OFFSET  (0xE)  //Relative Offset to the EDID detailed timing descriptors - Image Size upper nibble V and H
#define EDID_TIMING_DESC_INTERLACE_OFFSET                (0x11) //Relative Offset to the EDID detailed timing descriptors - Interlace flag
#define EDID_TIMING_H_ACTIVE_UPPER_NIBBLE_SHIFT          (0x4)  //H Active is upper 4 bit of EDID_TIMING_DESC_UPPER_H_NIBBLE_OFFSET
#define EDID_TIMING_H_ACTIVE_UPPER_NIBBLE_MASK           (0xF)  //H Active has 4 bit mask
#define EDID_TIMING_H_BLANK_UPPER_NIBBLE_MASK            (0xF)  //H Blank is 4 bit mask
#define EDID_TIMING_V_ACTIVE_UPPER_NIBBLE_SHIFT          (0x4)  //H Active is upper 4 bit of EDID_TIMING_DESC_UPPER_H_NIBBLE_OFFSET
#define EDID_TIMING_V_ACTIVE_UPPER_NIBBLE_MASK           (0xF)  //H Active has 4 bit mask
#define EDID_TIMING_V_BLANK_UPPER_NIBBLE_MASK            (0xF)  //H Blank is 4 bit mask
#define EDID_TIMING_H_IMAGE_SIZE_UPPER_NIBBLE_MASK       (0xF)  //H image size has 4 bit mask
#define EDID_TIMING_H_IMAGE_SIZE_UPPER_NIBBLE_SHIFT      (0x4)  //H image size is upper 4 bit
#define EDID_TIMING_V_IMAGE_SIZE_UPPER_NIBBLE_MASK       (0xF)  //V Active has 4 bit mask
#define EDID_TIMING_INTERLACE_SHIFT                      (0x7)  //Interlace is in bit(7) zero based.
#define EDID_TIMING_INTERLACE_MASK                       (0x80) //Interlace flag is 1 bit only.
#define EDID_TIMING_BLOCK_0_MAX_DESC                     (0x04) //Maximum number of detail timing descriptors in block 0
#define EDID_TIMING_PIXEL_CLOCK_RANGE                    (0x0A) //The ceiling and floor value of the pixel clock when EDID value is round up or round down.
#define EDID_TIMING_ASPECT_RATIO_CHECK_RANGE             (0x32) //An arbitrary value used for aspect ratio calculation
  
  /* Vendor specific data block */
  
#define EDID_VENDOR_BLOCK_3D_STRUCTURE_ALL               (0x1)  //3D multi-present field value
#define EDID_VENDOR_BLOCK_3D_STRUCTURE_MASK              (0x2)  //3D multi-present field value
#define EDID_VENDOR_BLOCK_3D_MULTI_FIELD_LENGTH          (0x2)  //3D multi-present field contains two bytes
#define EDID_VENDOR_BLOCK_3D_SIDE_BY_SIDE_HALF_VALUE     (0x8)  //See Table H-2 for 3D structure. Side by side is special as it contains extra sub sample info
  
#define EDID_VENDOR_BLOCK_DEEP_COLOR_SHIFT               (0x3)  //Shift 3 bits to obtain the deep color modes supported
#define EDID_VENDOR_BLOCK_DEEP_COLOR_MASK                (0xf)  //Mask for deep colors is 4 bit 
#define EDID_VENDOR_BLOCK_AI_SUPPORT_SHIFT               (0x7)  //Shift 7 bits to obtain the AI support bit
#define EDID_VENDOR_BLOCK_AI_SUPPORT_MASK                (0x1)  //Mask for AI support is 1 bit.
#define EDID_VENDOR_BLOCK_DVI_DUAL_SUPPORT_SHIFT         (0x0)  //Shift 0 bits to obtain the DVI dual bit
#define EDID_VENDOR_BLOCK_DVI_DUAL_SUPPORT_MASK          (0x1)  //Mask for DVI dual support is 1 bit.
#define EDID_VENDOR_BLOCK_LATENCY_PRESENT_SHIFT          (0x7)  //Shift 7 bits to obtain the progressive present bit
#define EDID_VENDOR_BLOCK_LATENCY_PRESENT_MASK           (0x1)  //Mask for progressive present is 1 bit.
#define EDID_VENDOR_BLOCK_I_LATENCY_PRESENT_SHIFT        (0x6)  //Shift 6 bits to obtain the Interlace present bit
#define EDID_VENDOR_BLOCK_I_LATENCY_PRESENT_MASK         (0x1)  //Mask for Interlace present is 1 bit.
#define EDID_VENDOR_BLOCK_VIDEO_PRESENT_SHIFT            (0x5)  //Shift 0 bits to obtain the HDMI video present bit
#define EDID_VENDOR_BLOCK_VIDEO_PRESENT_MASK             (0x1)  //Mask for HDMI video present is 1 bit.
#define EDID_VENDOR_BLOCK_CONTENT_TYPE_SHIFT             (0x0)  //Shift 0 bits to obtain the HDMI video content type info
#define EDID_VENDOR_BLOCK_CONTENT_TYPE_MASK              (0xf)  //Mask for HDMI video content type is 4 bits.
#define EDID_VENDOR_BLOCK_IMAGE_INFO_SHIFT               (0x3)  //Shift 3 bits to obtain the HDMI Image Info field
#define EDID_VENDOR_BLOCK_IMAGE_INFO_MASK                (0x3)  //Mask for HDMI Image info field is 2 bits.
#define EDID_VENDOR_BLOCK_3D_PRESENT_SHIFT               (0x7)  //Shift 7 bits to obtain the 3D present bit
#define EDID_VENDOR_BLOCK_3D_PRESENT_MASK                (0x1)  //Mask for HDMI video present is 1 bit.
#define EDID_VENDOR_BLOCK_3D_MULTI_PRESENT_SHIFT         (0x5)  //Shift 6 bits to obtain the 3D multi present bit
#define EDID_VENDOR_BLOCK_3D_MULTI_PRESENT_MASK          (0x3)  //Mask for HDMI video present is 2 bit.
#define EDID_VENDOR_BLOCK_HDMI_VIC_LEN_SHIFT             (0x5)  //Shift 5 bits to obtain the HDMI_VIC_LEN field
#define EDID_VENDOR_BLOCK_HDMI_VIC_LEN_MASK              (0x7)  //Mask for HDMI VIC_LEN field is 3 bits.
#define EDID_VENDOR_BLOCK_HDMI_3D_LEN_SHIFT              (0x0)  //Shift 0 bits to obtain the HDMI_3D_LEN field
#define EDID_VENDOR_BLOCK_HDMI_3D_LEN_MASK               (0x1f)  //Mask for HDMI_3D_LEN field is 5 bits.
#define EDID_VENDOR_BLOCK_HDMI_VIC_X_SHIFT               (0x0)  //Shift 0 bits to obtain the HDMI_VIC_X field
#define EDID_VENDOR_BLOCK_HDMI_VIC_X_MASK                (0xff) //Mask for HDMI_VIC_X  field is 8 bits.
#define EDID_VENDOR_BLOCK_HDMI_2D_VIC_X_SHIFT            (0x4)  //Shift 4 bits to obtain the HDMI_2D_VIC_ORDER field
#define EDID_VENDOR_BLOCK_HDMI_2D_VIC_X_MASK             (0xf)  //Mask for HDMI_2D_VIC_ORDER field is 4 bits.
#define EDID_VENDOR_BLOCK_HDMI_3D_STRUCTURE_X_SHIFT      (0x0)  //Shift 0 bits to obtain the HDMI_3D_STRUCTURE_X field
#define EDID_VENDOR_BLOCK_HDMI_3D_STRUCTURE_X_MASK       (0xf)  //Mask for HDMI_3D_STRUCTURE_X field is 4 bits.
#define EDID_VENDOR_BLOCK_HDMI_3D_DETAIL_X_SHIFT         (0x4)  //Shift 4 bits to obtain the HDMI_3D_DETAIL_X field
#define EDID_VENDOR_BLOCK_HDMI_3D_DETAIL_X_MASK          (0xf)  //Mask for HDMI_3D_DETAIL_X field is 4 bits.
  
  
#define EDID_ESTABLISHED_TIMINGI_720x400p70              (1<<7)
#define EDID_ESTABLISHED_TIMINGI_720x400p88              (1<<6)
#define EDID_ESTABLISHED_TIMINGI_640x480p60              (1<<5)
#define EDID_ESTABLISHED_TIMINGI_640x480p67              (1<<4)
#define EDID_ESTABLISHED_TIMINGI_640x480p72              (1<<3)
#define EDID_ESTABLISHED_TIMINGI_640x480p75              (1<<2)
#define EDID_ESTABLISHED_TIMINGI_800x600p56              (1<<1)
#define EDID_ESTABLISHED_TIMINGI_800x600p60              (1<<0)
  
#define EDID_ESTABLISHED_TIMINGII_800x600p72             (1<<7)
#define EDID_ESTABLISHED_TIMINGII_800x600p75             (1<<6)
#define EDID_ESTABLISHED_TIMINGII_832x624p75             (1<<5)
#define EDID_ESTABLISHED_TIMINGII_1024x768p87            (1<<4)
#define EDID_ESTABLISHED_TIMINGII_1024x768p60            (1<<3)
#define EDID_ESTABLISHED_TIMINGII_1024x768p70            (1<<2)
#define EDID_ESTABLISHED_TIMINGII_1024x768p75            (1<<1)
#define EDID_ESTABLISHED_TIMINGII_1280x1024p75           (1<<0)
  
#define EDID_ESTABLISHED_TIMINGIII_1280x768p60           (1<<6)
#define EDID_ESTABLISHED_TIMINGIII_1280x960p60           (1<<3)
#define EDID_ESTABLISHED_TIMINGIII_1280x1024p60          (1<<1)
#define EDID_ESTABLISHED_TIMINGIII_1360x768p60           (1<<7)
#define EDID_ESTABLISHED_TIMINGIII_1440x900p60           (1<<5)
#define EDID_ESTABLISHED_TIMINGIII_1400x1050p60          (1<<1)
#define EDID_ESTABLISHED_TIMINGIII_1680x1050p60          (1<<5)
  
/* HDMI Video data block */
#define EDID_VIDEODATABLOCK_VERSION_BYTE_OFFSET          (0x2)
#define EDID_VIDEODATABLOCK_SCDC_MASK                    (0x80)   // Mask for SCDC_Present field
#define EDID_VIDEODATABLOCK_RRCAPABLE_MASK               (0x40)   // Mask for RR_Capable
#define EDID_VIDEODATABLOCK_SCRAMBLE340MCSC_MASK         (0x8)    // Mask for LTE_340_Mcsc_scramble
#define EDID_VIDEODATABLOCK_3D_INDPENDENTVIEW_MASK       (0x4)    // Mask for Independent_view
#define EDID_VIDEODATABLOCK_3D_DUALVIEW_MASK             (0x2)    // Mask for Dual_view
#define EDID_VIDEODATABLOCK_3D_OSD_DISPARITY_MASK        (0x1)    // Mask for 3D_OSD_Disparity

/* Maxium number of injected CEA modes */
#define EDID_MAXIMUM_INJECTED_CEA_MODES                  (16)

// Flags to be used for EDID_GetAudioModeInfo 
#define EDID_GET_AUDIO_INSERT_DEFAULT_MODE               0x00000001  // Insert a default mode if no compatible audio modes are reported

/* -----------------------------------------------------------------------
**  Enums
** ----------------------------------------------------------------------- */
typedef uint32   EDID_ErrorType;

typedef enum
{
  EDID_STATUS_OK = 0,                   /* Command successful */
  EDID_STATUS_SUCCESS = EDID_STATUS_OK,
  EDID_STATUS_BAD_HANDLE,               /* Bad Destination Handle */
  EDID_STATUS_BAD_PARAM,                /* Bad Parameter */
  EDID_STATUS_NOT_SUPPORTED,            /* Operation not supported */
  EDID_STATUS_NO_RESOURCES,             /* Out of resources */
  EDID_STATUS_FAILED,                   /* Command Failed */
  EDID_STATUS_TIMEOUT,                  /* Command not completed within expected time */
  EDID_STATUS_HW_ERROR,                 /* Hardware isn't responding as expected */
  EDID_STATUS_INVALID_STATE,            /* Not in correct state to perform task */
  EDID_STATUS_BUFFER_TOO_SMALL,         /* A function needs a larger buffer for the amount of data*/
  EDID_STATUS_MAX,
  EDID_STATUS_FORCE_32BIT = 0x7FFFFFFF
} EDID_Status;

//-------------------------------------------------------------------------------------------------
//  EDID_VideoFormatType
//
//  @brief
//      Enum definining VIC (video format) codes 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum 
{
  EDID_VIDEO_FORMAT_640x480p60_4_3   = 0,
  EDID_VIDEO_FORMAT_720x480p60_4_3,
  EDID_VIDEO_FORMAT_720x480p60_16_9,
  EDID_VIDEO_FORMAT_1280x720p60_16_9,
  EDID_VIDEO_FORMAT_1920x1080i60_16_9,
  EDID_VIDEO_FORMAT_720x480i60_4_3,
  EDID_VIDEO_FORMAT_1440x480i60_4_3   = EDID_VIDEO_FORMAT_720x480i60_4_3,
  EDID_VIDEO_FORMAT_720x480i60_16_9,
  EDID_VIDEO_FORMAT_1440x480i60_16_9  = EDID_VIDEO_FORMAT_720x480i60_16_9,
  EDID_VIDEO_FORMAT_720x240p60_4_3,
  EDID_VIDEO_FORMAT_1440x240p60_4_3   = EDID_VIDEO_FORMAT_720x240p60_4_3,
  EDID_VIDEO_FORMAT_720x240p60_16_9,
  EDID_VIDEO_FORMAT_1440x240p60_16_9  = EDID_VIDEO_FORMAT_720x240p60_16_9,
  EDID_VIDEO_FORMAT_2880x480i60_4_3,
  EDID_VIDEO_FORMAT_2880x480i60_16_9,
  EDID_VIDEO_FORMAT_2880x240p60_4_3,
  EDID_VIDEO_FORMAT_2880x240p60_16_9,
  EDID_VIDEO_FORMAT_1440x480p60_4_3,
  EDID_VIDEO_FORMAT_1440x480p60_16_9,
  EDID_VIDEO_FORMAT_1920x1080p60_16_9,
  EDID_VIDEO_FORMAT_720x576p50_4_3,
  EDID_VIDEO_FORMAT_720x576p50_16_9,
  EDID_VIDEO_FORMAT_1280x720p50_16_9,
  EDID_VIDEO_FORMAT_1920x1080i50_16_9,
  EDID_VIDEO_FORMAT_720x576i50_4_3,
  EDID_VIDEO_FORMAT_1440x576i50_4_3  =   EDID_VIDEO_FORMAT_720x576i50_4_3,
  EDID_VIDEO_FORMAT_720x576i50_16_9,
  EDID_VIDEO_FORMAT_1440x576i50_16_9 =   EDID_VIDEO_FORMAT_720x576i50_16_9,
  EDID_VIDEO_FORMAT_720x288p50_4_3,
  EDID_VIDEO_FORMAT_1440x288p50_4_3  =   EDID_VIDEO_FORMAT_720x288p50_4_3,
  EDID_VIDEO_FORMAT_720x288p50_16_9,
  EDID_VIDEO_FORMAT_1440x288p50_16_9 =   EDID_VIDEO_FORMAT_720x288p50_16_9,
  EDID_VIDEO_FORMAT_2880x576i50_4_3,
  EDID_VIDEO_FORMAT_2880x576i50_16_9,
  EDID_VIDEO_FORMAT_2880x288p50_4_3,
  EDID_VIDEO_FORMAT_2880x288p50_16_9,
  EDID_VIDEO_FORMAT_1440x576p50_4_3,
  EDID_VIDEO_FORMAT_1440x576p50_16_9,
  EDID_VIDEO_FORMAT_1920x1080p50_16_9,
  EDID_VIDEO_FORMAT_1920x1080p24_16_9,
  EDID_VIDEO_FORMAT_1920x1080p25_16_9,
  EDID_VIDEO_FORMAT_1920x1080p30_16_9,
  EDID_VIDEO_FORMAT_2880x480p60_4_3,
  EDID_VIDEO_FORMAT_2880x480p60_16_9,
  EDID_VIDEO_FORMAT_2880x576p50_4_3,
  EDID_VIDEO_FORMAT_2880x576p50_16_9,
  EDID_VIDEO_FORMAT_1920x1080i50_16_9_special, //	Format 39 is 1080i50 mode with total 1250 vertical lines.
  EDID_VIDEO_FORMAT_1920x1080i100_16_9,
  EDID_VIDEO_FORMAT_1280x720p100_16_9,
  EDID_VIDEO_FORMAT_720x576p100_4_3,
  EDID_VIDEO_FORMAT_720x576p100_16_9,
  EDID_VIDEO_FORMAT_720x576i100_4_3,
  EDID_VIDEO_FORMAT_1440x576i100_4_3  = EDID_VIDEO_FORMAT_720x576i100_4_3,
  EDID_VIDEO_FORMAT_720x576i100_16_9,
  EDID_VIDEO_FORMAT_1440x576i100_16_9 = EDID_VIDEO_FORMAT_720x576i100_16_9,
  EDID_VIDEO_FORMAT_1920x1080i120_16_9,
  EDID_VIDEO_FORMAT_1280x720p120_16_9,
  EDID_VIDEO_FORMAT_720x480p120_4_3,
  EDID_VIDEO_FORMAT_720x480p120_16_9,
  EDID_VIDEO_FORMAT_720x480i120_4_3,
  EDID_VIDEO_FORMAT_1440x480i120_4_3  = EDID_VIDEO_FORMAT_720x480i120_4_3,
  EDID_VIDEO_FORMAT_720x480i120_16_9,
  EDID_VIDEO_FORMAT_1440x480i120_16_9 = EDID_VIDEO_FORMAT_720x480i120_16_9,
  EDID_VIDEO_FORMAT_720x576p200_4_3,
  EDID_VIDEO_FORMAT_720x576p200_16_9,
  EDID_VIDEO_FORMAT_720x576i200_4_3,
  EDID_VIDEO_FORMAT_1440x576i200_4_3  = EDID_VIDEO_FORMAT_720x576i200_4_3,
  EDID_VIDEO_FORMAT_720x576i200_16_9,
  EDID_VIDEO_FORMAT_1440x576i200_16_9 = EDID_VIDEO_FORMAT_720x576i200_16_9,
  EDID_VIDEO_FORMAT_720x480p240_4_3,
  EDID_VIDEO_FORMAT_720x480p240_16_9,
  EDID_VIDEO_FORMAT_720x480i240_4_3,
  EDID_VIDEO_FORMAT_1440x480i240_4_3  = EDID_VIDEO_FORMAT_720x480i240_4_3,
  EDID_VIDEO_FORMAT_720x480i240_16_9,
  EDID_VIDEO_FORMAT_1440x480i240_16_9 = EDID_VIDEO_FORMAT_720x480i240_16_9,
  // CEA-861F defined additional modes
  EDID_VIDEO_FORMAT_1280x720p24_16_9,  // Format 60
  EDID_VIDEO_FORMAT_1280x720p25_16_9,
  EDID_VIDEO_FORMAT_1280x720p30_16_9,
  EDID_VIDEO_FORMAT_1920x1080p120_16_9,
  EDID_VIDEO_FORMAT_1920x1080p100_16_9,
  EDID_VIDEO_FORMAT_1280x720p24_64_27,  // aspect ratio is (4/3)^N where N = 1,2,3,4
  EDID_VIDEO_FORMAT_1280x720p25_64_27,
  EDID_VIDEO_FORMAT_1280x720p30_64_27,
  EDID_VIDEO_FORMAT_1280x720p50_64_27,
  EDID_VIDEO_FORMAT_1280x720p60_64_27,
  EDID_VIDEO_FORMAT_1280x720p100_64_27,
  EDID_VIDEO_FORMAT_1280x720p120_64_27,
  EDID_VIDEO_FORMAT_1920x1080p24_64_27,
  EDID_VIDEO_FORMAT_1920x1080p25_64_27,
  EDID_VIDEO_FORMAT_1920x1080p30_64_27,
  EDID_VIDEO_FORMAT_1920x1080p50_64_27,
  EDID_VIDEO_FORMAT_1920x1080p60_64_27,
  EDID_VIDEO_FORMAT_1920x1080p100_64_27,
  EDID_VIDEO_FORMAT_1920x1080p120_64_27,
  EDID_VIDEO_FORMAT_1680x720p24_64_27,
  EDID_VIDEO_FORMAT_1680x720p25_64_27,
  EDID_VIDEO_FORMAT_1680x720p30_64_27,
  EDID_VIDEO_FORMAT_1680x720p50_64_27,
  EDID_VIDEO_FORMAT_1680x720p60_64_27,
  EDID_VIDEO_FORMAT_1680x720p100_64_27,
  EDID_VIDEO_FORMAT_1680x720p120_64_27,
  EDID_VIDEO_FORMAT_2560x1080p24_64_27,
  EDID_VIDEO_FORMAT_2560x1080p25_64_27,
  EDID_VIDEO_FORMAT_2560x1080p30_64_27,
  EDID_VIDEO_FORMAT_2560x1080p50_64_27,
  EDID_VIDEO_FORMAT_2560x1080p60_64_27,
  EDID_VIDEO_FORMAT_2560x1080p100_64_27,
  EDID_VIDEO_FORMAT_2560x1080p120_64_27,
  EDID_VIDEO_FORMAT_3840x2160p24_16_9,
  EDID_VIDEO_FORMAT_3840x2160p25_16_9,
  EDID_VIDEO_FORMAT_3840x2160p30_16_9,
  EDID_VIDEO_FORMAT_3840x2160p50_16_9,
  EDID_VIDEO_FORMAT_3840x2160p60_16_9,
  EDID_VIDEO_FORMAT_4096x2160p24_256_135,
  EDID_VIDEO_FORMAT_4096x2160p25_256_135,
  EDID_VIDEO_FORMAT_4096x2160p30_256_135,
  EDID_VIDEO_FORMAT_4096x2160p50_256_135,
  EDID_VIDEO_FORMAT_4096x2160p60_256_135,
  EDID_VIDEO_FORMAT_3840x2160p24_64_27,
  EDID_VIDEO_FORMAT_3840x2160p25_64_27,
  EDID_VIDEO_FORMAT_3840x2160p30_64_27,
  EDID_VIDEO_FORMAT_3840x2160p50_64_27,
  EDID_VIDEO_FORMAT_3840x2160p60_64_27,
  // VESA modes (other than those supported by CEA specs)
  EDID_VIDEO_FORMAT_640x350p85_16_9,
  EDID_VIDEO_FORMAT_640x400p85_16_10,
  EDID_VIDEO_FORMAT_720x400p85_16_9,
  EDID_VIDEO_FORMAT_640x480p72_4_3, 
  EDID_VIDEO_FORMAT_640x480p75_4_3, 
  EDID_VIDEO_FORMAT_640x480p85_4_3, 
  EDID_VIDEO_FORMAT_800x600p56_4_3, 
  EDID_VIDEO_FORMAT_800x600p60_4_3, 
  EDID_VIDEO_FORMAT_800x600p72_4_3, 
  EDID_VIDEO_FORMAT_800x600p75_4_3, 
  EDID_VIDEO_FORMAT_800x600p85_4_3, 
  EDID_VIDEO_FORMAT_800x600p120_4_3,
  EDID_VIDEO_FORMAT_848x480p60_16_9,
  EDID_VIDEO_FORMAT_1024x768p60_4_3,
  EDID_VIDEO_FORMAT_1024x768p70_4_3,  
  EDID_VIDEO_FORMAT_1024x768p75_4_3,  
  EDID_VIDEO_FORMAT_1024x768p85_4_3, 
  EDID_VIDEO_FORMAT_1024x768p120_4_3,
  EDID_VIDEO_FORMAT_1152x768p60_3_2_RB,
  EDID_VIDEO_FORMAT_1152x864p75_4_3, 
  EDID_VIDEO_FORMAT_1280x768p60_5_3,
  EDID_VIDEO_FORMAT_1280x768p60_5_3_RB,
  EDID_VIDEO_FORMAT_1280x768p75_5_3,
  EDID_VIDEO_FORMAT_1280x768p85_5_3,
  EDID_VIDEO_FORMAT_1280x768p120_5_3,
  EDID_VIDEO_FORMAT_1280x800p60_16_10,
  EDID_VIDEO_FORMAT_1280x800p60_16_10_RB,
  EDID_VIDEO_FORMAT_1280x800p75_16_10,
  EDID_VIDEO_FORMAT_1280x800p85_16_10,
  EDID_VIDEO_FORMAT_1280x800p120_16_10,
  EDID_VIDEO_FORMAT_1280x960p60_4_3,
  EDID_VIDEO_FORMAT_1280x960p85_4_3, 
  EDID_VIDEO_FORMAT_1280x960p120_4_3,
  EDID_VIDEO_FORMAT_1280x1024p60_5_4, 
  EDID_VIDEO_FORMAT_1280x1024p75_5_4, 
  EDID_VIDEO_FORMAT_1280x1024p85_5_4, 
  EDID_VIDEO_FORMAT_1280x1024p120_5_4,
  EDID_VIDEO_FORMAT_1360x768p60_16_9,
  EDID_VIDEO_FORMAT_1360x768p120_16_9,
  EDID_VIDEO_FORMAT_1366x768p60_16_9,
  EDID_VIDEO_FORMAT_1366x768p60_16_9_RB,
  EDID_VIDEO_FORMAT_1400x1050p60_4_3,
  EDID_VIDEO_FORMAT_1400x1050p60_4_3_RB,
  EDID_VIDEO_FORMAT_1400x1050p75_4_3, 
  EDID_VIDEO_FORMAT_1400x1050p85_4_3, 
  EDID_VIDEO_FORMAT_1400x1050p120_4_3,
  EDID_VIDEO_FORMAT_1440x900p60_16_10,
  EDID_VIDEO_FORMAT_1440x900p60_16_10_RB,
  EDID_VIDEO_FORMAT_1440x900p75_16_10, 
  EDID_VIDEO_FORMAT_1440x900p85_16_10, 
  EDID_VIDEO_FORMAT_1440x900p120_16_10,
  EDID_VIDEO_FORMAT_1600x900p60_16_9,
  EDID_VIDEO_FORMAT_1600x1200p60_3_3, 
  EDID_VIDEO_FORMAT_1600x1200p65_4_3, 
  EDID_VIDEO_FORMAT_1600x1200p70_4_3, 
  EDID_VIDEO_FORMAT_1600x1200p75_4_3, 
  EDID_VIDEO_FORMAT_1600x1200p85_4_3, 
  EDID_VIDEO_FORMAT_1600x1200p120_4_3,
  EDID_VIDEO_FORMAT_1680x1050p60_16_10,
  EDID_VIDEO_FORMAT_1680x1050p60_16_10_RB,
  EDID_VIDEO_FORMAT_1680x1050p75_16_10,
  EDID_VIDEO_FORMAT_1680x1050p85_16_10,
  EDID_VIDEO_FORMAT_1680x1050p120_16_10,
  EDID_VIDEO_FORMAT_1792x1344p60_4_3,
  EDID_VIDEO_FORMAT_1792x1344p75_4_3,  
  EDID_VIDEO_FORMAT_1792x1344p120_4_3, 
  EDID_VIDEO_FORMAT_1856x1392p60_4_3,  
  EDID_VIDEO_FORMAT_1856x1392p75_4_3,  
  EDID_VIDEO_FORMAT_1856x1392p120_4_3, 
  EDID_VIDEO_FORMAT_1920x1200p60_16_10,
  EDID_VIDEO_FORMAT_1920x1200p60_16_10_RB,
  EDID_VIDEO_FORMAT_1920x1200p75_16_10,
  EDID_VIDEO_FORMAT_1920x1200p85_16_10,
  EDID_VIDEO_FORMAT_1920x1200p120_16_10,
  EDID_VIDEO_FORMAT_1920x1280p60_3_2_RB,
  EDID_VIDEO_FORMAT_1920x1440p60_4_3,  
  EDID_VIDEO_FORMAT_1920x1440p75_4_3,  
  EDID_VIDEO_FORMAT_1920x1440p120_4_3, 
  EDID_VIDEO_FORMAT_2048x1152p60_16_9_RB, 
  EDID_VIDEO_FORMAT_2560x1440p60_16_9_RB,
  EDID_VIDEO_FORMAT_2560x1440p60_16_9,
  EDID_VIDEO_FORMAT_2560x1600p60_16_10,
  EDID_VIDEO_FORMAT_2560x1600p60_16_10_RB,
  EDID_VIDEO_FORMAT_2560x1600p75_16_10, 
  EDID_VIDEO_FORMAT_2560x1600p85_16_10, 
  EDID_VIDEO_FORMAT_2560x1600p120_16_10,
  EDID_VIDEO_FORMAT_3440x1440p60_43_18_RB,
  EDID_VIDEO_FORMAT_3840x2160p60_16_9_RB,
  EDID_VIDEO_FORMAT_4096x2160p24_16_9,

  // Extended resolutions
  // For HDMI1.4 these resolutions are specified 
  // in the vendor specific data block
  // and not covered within the VIC codes
  EDID_VIDEO_FORMAT_3840x2160p30_16_9_EX,
  EDID_VIDEO_FORMAT_3840x2160p25_16_9_EX,
  EDID_VIDEO_FORMAT_3840x2160p24_16_9_EX,
  EDID_VIDEO_FORMAT_4096x2160p24_16_9_EX,
  EDID_VIDEO_FORMAT_MAX,
  EDID_VIDEO_FORMAT_32BIT = 0x7FFFFFFF
} EDID_VideoFormatType;

//-------------------------------------------------------------------------------------------------
//  EDID_PixelFormatType
//
//  @brief
//      Enum indicating pixel format support. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum 
{
  EDID_PIXEL_FORMAT_RGB888 = 0x0,
  EDID_PIXEL_FORMAT_YUV422,
  EDID_PIXEL_FORMAT_YUV444,
  EDID_PIXEL_FORMAT_YUV420,
  EDID_PIXEL_FORMAT_MAX,
  HDMI_PIXEL_FORMAT_32BIT = 0x7FFFFFFF
} EDID_PixelFormatType;

//-------------------------------------------------------------------------------------------------
//  EDID_VendorBlockExtendedResType
//
//  @brief
//      Indicates if the sink can support Extended (4Kx2K) resolution formats in the HDMI_VIC_X field
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_VENDOR_BLOCK_EXTENDED_RES_NONE  = 0x0,
  EDID_VENDOR_BLOCK_EXTENDED_RES_3840x2160p30,        /* Sink device supports Extended Resolution 3840x2160 at 30FPS*/
  EDID_VENDOR_BLOCK_EXTENDED_RES_3840x2160p25,        /* Sink device supports Extended Resolution 3840x2160 at 25FPS */
  EDID_VENDOR_BLOCK_EXTENDED_RES_3840x2160p24,        /* Sink device supports Extended Resolution 3840x2160 at 24FPS */
  EDID_VENDOR_BLOCK_EXTENDED_RES_4096x2160p24,        /* Sink device supports Extended Resolution 4096x2160 at 24FPS */
  EDID_VENDOR_BLOCK_EXTENDED_RES_MAX,
  EDID_VENDOR_BLOCK_EXTENDED_RES_32BIT = 0x7FFFFFFF
} EDID_VendorBlockExtendedResType;

//-------------------------------------------------------------------------------------------------
//  EDID_SpecificationType
//
//  @brief
//      This is used to identify the supported HDMI/DP specification. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_SPEC_TYPE_UNSUPPORTED      = 0x00000000,
  EDID_HDMI_SPEC_TYPE_1_4         = 0x00000001,
  EDID_HDMI_SPEC_TYPE_2_0         = 0x00000002,
  EDID_DP_SPEC_TYPE_1_3           = 0x00000004,
  EDID_SPEC_TYPE_MAX,
  EDID_SPEC_TYPE_32BIT            = 0x7FFFFFFF
} EDID_SpecificationType;

/* -----------------------------------------------------------------------
**  Structures
** ----------------------------------------------------------------------- */
//-------------------------------------------------------------------------------------------------
//  EDID_Config
//
//  @brief
// struct holds edid string
//
//-------------------------------------------------------------------------------------------------
//

/** Defines the EDID established timings.
*/
typedef struct {
    uint8 TimingI;              /**< Established timings table I. */
    uint8 TimingII;             /**< Established timings table II. */
    uint8 ManufacturesTiming;   /**< Manufacturer's timings table. */
} EDID_EstablishedTimings;

/** Defines the EDID standard timings.
*/
typedef struct {
   uint8  HorizontalPixels;     /**< Horizontal pixel width. */
   uint8  AspectRatio_Refresh;  /**< Aspect ratio and field refresh rate.
                                     @newpagetable */
} EDID_StandardTimings;

/** Defines the EDID detailed timings.
*/
typedef struct {
   uint16  PixelClock;
   /** Pixel clock frequency. */
   
   uint8   HPixels;
   /**< Lower eight bits of the horizontal addressable video pixels. */
   
   uint8   HBlanking;
   /**< Lower eight bits of the horizontal blanking pixels. */
   
   uint8   HPixels_Blanking;
   /**< Higher four bits of the horizontal active and blanking pixels. */
   
   uint8   VPixels;
   /**< Lower eight bits of the vertical addressable video pixels. */
   
   uint8   VBlanking;
   /**< Lower eight bits of the vertical blanking pixels. */
   
   uint8   VPixels_Blanking;
   /**< Higher four bits of the vertical active and blanking pixels. */
   
   uint8   HFrontPorch;
   /**< Lower eight bits of the horizontal front porch. */
   
   uint8   HSyncWidth;
   /**< Lower eight bits of the horizontal synchronization width. */
   
   uint8   VFrontPorch_SyncWidth;
   /**< Lower four bits of the vertical front porch and synchronization width. */
   
   uint8   HVPorchSyncWidths;
   /**< Higher two bits of the vertical/horizontal porch and synchronization
        widths. */
   
   uint8   HImageSize;
   /**< Lower eight bits of the video image width. */
   
   uint8   VImageSize;
   /**< Lower eight bits of the video image height. */
   
   uint8   HVImageSize;
   /**< Upper four bits of the video image width/height. */
   
   uint8   HBorder;
   /**< Horizontal border size. */
   
   uint8   VBorder;
   /**< Vertical border size. */
   
   uint8   SignalTimingInterface;
   /**< Detailed timing signal information. */
   
} EDID_DetailedTiming;

/**  Defines the EDID display descriptor.
*/
typedef struct {
   uint16  DescriptorHeader;  /**< Header for this descriptor: 0x0 indicates
                                   that this is a descriptor block. */
   uint8   Reserved1;         /**< Reserved. */
   uint8   TagId;             /**< Indicates the tag for this descriptor. */
   uint8   Reserved2;         /**< Reserved. */
   uint8   Descriptor[13];    /**< Descriptor string. @newpagetable */
} EDID_DisplayDescriptor;

/** Defines the EDID.
*/
typedef struct {
  uint8   Header[8];              /**< EDID header: 00 FF FF FF FF FF FF 00. */
  uint16  ManufactureID;          /**< EISA three-character ID. */
  uint16  ProductCode;            /**< Vendor-assigned code. */
  uint32  SerialNumber;           /**< Thirty-two-bit serial number. */
  uint8   WeekOfManufacture;      /**< Week number. */
  uint8   YearOfManufacture;      /**< Year number. */
  uint8   EdidVersion;            /**< EDID structure version. */
  uint8   EdidRevision;           /**< EDID structure revision. */
  uint8   VideoInputDefinition;   /**< Information on how the host's video
                                       outputs are to be configured to drive
                                       the attached display. */
  uint8   MaxHorizontalImageSize;  /**< Maximum horizontal size in centimeters. */
  uint8   MaxVerticalImageSize;    /**< Maximum vertical size in centimeters. */
  uint8   DisplayTransferCharacteristic;  /**< Characteristics of the display
                                               transfer. */
  uint8   FeatureSupport;          /**< Indicates which features are supported. */
  uint8   RedGreenLowBits;         /**< Rx1 Rx0 Ry1 Ry0 Gx1 Gx0 Gy1Gy0. */
  uint8   BlueWhiteLowBits;        /**< Bx1 Bx0 By1 By0 Wx1 Wx0 Wy1 Wy0. */
  uint8   RedX;                    /**< Red-x bits 9 to 2. */
  uint8   RedY;                    /**< Red-y bits 9 to 2. */
  uint8   GreenX;                  /**< Green-x bits 9 to 2. */
  uint8   GreenY;                  /**< Green-y bits 9 to 2. */
  uint8   BlueX;                   /**< Blue-x bits 9 to 2. */
  uint8   BlueY;                   /**< Blue-y bits 9 to 2. */
  uint8   WhiteX;                  /**< White-x bits 9 to 2. */
  uint8   WhiteY;                  /**< White-y bits 9 to 2. */

  EDID_EstablishedTimings EstablishedTimings;  /**< Established timings. */
  EDID_StandardTimings    StandardTiming[8];   /**< Standard timings. */
  EDID_DetailedTiming     DetailedTiming;      /**< Detailed timings. */
  
  /** Detailed descriptors. */
  union 
  {
      EDID_DetailedTiming    DetailedTimings[3];
      /**< Detailed timing descriptors. */

      EDID_DisplayDescriptor DisplayDescriptors[3];
      /**< Detailed display descriptors. */
      
  }EDID_DetailedInfo;
  
  uint8   ExtensionFlag;    /**< Number of optional 128-byte EDID extension
                                 blocks that follows. */
  uint8   Checksum;         /**< EDID checksum. */

} __attribute__ ((packed)) EDID_Config;

//-------------------------------------------------------------------------------------------------
//  EDID_DispModeAttrType
//
//  @brief
// struct to holds display mode after parse
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
    /* Horizontal fields */
    uint32     iHsyncPulseWidthDclk;
    /**< Horizontal synchronization pulse width in dot_clks. */

    uint32     iHsyncBackPorchDclk;
    /**< Left edge including horizontal synchronization pulse width. */

    uint32     iHsyncFrontPorchDclk;
    /**< Right edge of an inactive LCD. */

    uint32     iHsyncSkewDclk;
    /**< Delay horizontal synchronization with respect to vertical
         synchronization. */

    uint32     iHLeftBorderDClk;
    /**< Number of left border pixels. */

    uint32     iHRightBorderDClk;
    /**< Number of right border pixels. */    

    /* Vertical fields */    
    uint32     iVsyncPulseWidthLines;
    /**< Vertical synchronization pulse width in lines. */

    uint32     iVsyncFrontPorchLines;
    /**< Blanking period before hardware vertical synchronization. */

    uint32     iVsyncBackPorchLines;
    /**< Blanking period after hardware vertical synchronization. */

    uint32     iVTopBorderLines;
    /**< Number of top border lines. */

    uint32     iVBottomBorderLines;
    /**< Number of bottom border lines. */

    /* Misc fields */
    uint32     iBorderColorRgb888;
    /**< Border color used when bordering an active region. */
    
    uint32     iUnderflowColorRgb888;
    /**< Color produced when an underflow occurs. */

    bool32     bInterlaced;
    /**< Interlace mode. */

    /* Signal polarity fields */
    bool32     bIsHsyncActiveLow;
    /**< Inverse horizontal synchronization control signal. */

    bool32     bIsVsyncActiveLow;
    /**< Inverse vertical synchronization control signal. */

    bool32     bIsDataEnableActiveLow;
    /**< Inverse data enable signal. */

    uint32     uInterfaceEnableDelay;
    /**< Delay in milliseconds -- Required after the function is enabled. @newpagetable */

} EDID_Panel_ActiveTimingType;


/*!
* \b EDID_PixelFormatType       
*
* Supported display/lcd color specific formats.
*
* The supported formats and their associated bit packings
* are listed below.
* Legend: 
*
*
*                    33222222222211111111110000000000  Bit                 \n
*                    10987654321098765432109876543210  Location            \n
*                   |--------------------------------|                     \n
*  RGB_565          |                RRRRRGGGGGGBBBBB|                     \n
*                   |--------------------------------|                     \n
*  RGB_666          |              RRRRRRGGGGGGBBBBBB|                     \n
*                   |--------------------------------|                     \n
*  RGB_888          |        RRRRRRRRGGGGGGGGBBBBBBBB|                     \n
*                   |--------------------------------|                     \n
*  ARGB_1555        |                ARRRRRGGGGGBBBBB|                     \n
*                   |--------------------------------|                     \n
*  XRGB_8888        |xxxxxxxxRRRRRRRRGGGGGGGGBBBBBBBB|                     \n
*                   |--------------------------------|                     \n
*  ARGB_8888        |AAAAAAAARRRRRRRRGGGGGGGGBBBBBBBB|                     \n
*                   |--------------------------------|                     \n
*  BGR_565          |                BBBBBGGGGGGRRRRR|                     \n
*                   |--------------------------------|                     \n
*  BGR_8888         |        BBBBBBBBGGGGGGGGRRRRRRRR|                     \n
*                   |--------------------------------|                     \n
*  ABGR_1555        |                ABBBBBGGGGGRRRRR|                     \n
*                   |--------------------------------|                     \n
*  XBGR_8888        |xxxxxxxxBBBBBBBBGGGGGGGGRRRRRRRR|                     \n
*                   |--------------------------------|                     \n
*  ABGR_8888        |AAAAAAAABBBBBBBBGGGGGGGGRRRRRRRR|                     \n
*                   |--------------------------------|                     \n
*  Y_CBCR_H2V2      |                        YYYYYYYY|  Plane0             \n
* (Y_CBCR_H2V2_VC1) |                bbbbbbbbrrrrrrrr|  Plane1             \n
*                   |--------------------------------|                     \n
*  Y_CRCB_H2V2      |                        YYYYYYYY|  Plane0             \n
* (Y_CRCB_H2V2_VC1) |                rrrrrrrrbbbbbbbb|  Plane1             \n
*                   |--------------------------------|                     \n
*  YCRYCB_H2V1      |YYYYYYYYrrrrrrrrYYYYYYYYbbbbbbbb|                     \n
*                   |--------------------------------|                     \n
*  Y_CRCB_H2V1      |                Y0Y0Y0Y0Y1Y1Y1Y1|  Plane0             \n
*                   |                rrrrrrrrbbbbbbbb|  Plane1             \n
*                   |--------------------------------|                     \n
*  Y_CBCR_H2V1      |                Y0Y0Y0Y0Y1Y1Y1Y1|  Plane0             \n
*                   |                bbbbbbbbrrrrrrrr|  Plane1             \n
*                   |--------------------------------|                     \n
*  Y_CR_CB_H2V2     |                Y0Y0Y0Y0Y1Y1Y1Y1|  Plane0             \n
*                   |                        rrrrrrrr|  Plane1             \n
*                   |                        bbbbbbbb|  Plane2             \n
*                   |--------------------------------|                     \n
*  ARGB_4444        |                AAAARRRRGGGGBBBB|                     \n
*                   |--------------------------------|                     \n
*                    33222222222211111111110000000000  Bit                 \n
*                    10987654321098765432109876543210  Location            \n
*
* NOTE: Y_CBCR_H2V2_VC1 and Y_CRCB_H2V2_VC1 formats are identical in
*       packing order to Y_CBCR_H2V2 and Y_CRCB_H2V2 (respectively).  The
*       color range for the VC1 formats are WMV9, the same as main
*       profile (in advanced mode: nRangeY=nRangeC=16).
*
*/
typedef enum
{
  EDID_PIXEL_COLOR_FORMAT_NONE = 0,
  EDID_PIXEL_COLOR_FORMAT_RGB_565_16BPP,
  EDID_PIXEL_COLOR_FORMAT_RGB_666_18BPP,
  EDID_PIXEL_COLOR_FORMAT_RGB_888_24BPP,
  EDID_PIXEL_COLOR_FORMAT_ARGB_1555_16BPP,
  EDID_PIXEL_COLOR_FORMAT_XRGB_8888_32BPP,
  EDID_PIXEL_COLOR_FORMAT_ARGB_8888_32BPP,
  EDID_PIXEL_COLOR_FORMAT_BGR_565_16BPP,
  EDID_PIXEL_COLOR_FORMAT_BGR_888_24BPP,
  EDID_PIXEL_COLOR_FORMAT_ABGR_1555_16BPP,
  EDID_PIXEL_COLOR_FORMAT_XBGR_8888_32BPP,
  EDID_PIXEL_COLOR_FORMAT_ABGR_8888_32BPP,
  EDID_PIXEL_COLOR_FORMAT_Y_CBCR_H2V2_12BPP, /**< 4:2:0, Pseudo planar, Cb(U) in MSB, viewing in bytes in chroma plane, it is CrCbCrCbCrCb..., same as NV21*/
  EDID_PIXEL_COLOR_FORMAT_Y_CRCB_H2V2_12BPP, /**< 4:2:0, Pseudo planar, Cr(V) in MSB, viewing in bytes in chroma plane, it is CbCrCbCrCbCr..., same as NV12*/
  EDID_PIXEL_COLOR_FORMAT_YCRYCB_H2V1_16BPP, /**< 4:2:2, YCrYCb(YVYU) interleave */
  EDID_PIXEL_COLOR_FORMAT_YCBYCR_H2V1_16BPP, /**< 4:2:2, YCbYCr(YUYV) interleave */
  EDID_PIXEL_COLOR_FORMAT_CRYCBY_H2V1_16BPP, /**< 4:2:2, CrYCbY(VYUY) interleave */
  EDID_PIXEL_COLOR_FORMAT_CBYCRY_H2V1_16BPP, /**< 4:2:2, CbYCrY(UYVY) interleave */
  EDID_PIXEL_COLOR_FORMAT_Y_CRCB_H2V1_16BPP, /**< 4:2:2, Pseudo planar, Cr(U) in MSB */
  EDID_PIXEL_COLOR_FORMAT_Y_CBCR_H2V1_16BPP, /**< 4:2:2, Pseudo planar, Cb(V) in MSB */
  EDID_PIXEL_COLOR_FORMAT_Y_CBCR_H2V2_VC1_12BPP, /**< Identical packing to Y_CBCR_H2V2_12BPP */
  EDID_PIXEL_COLOR_FORMAT_Y_CRCB_H2V2_VC1_12BPP,  /**< Identical packing to Y_CRCB_H2V2_12BPP */
  EDID_PIXEL_COLOR_FORMAT_Y_CR_CB_H2V2_12BPP,  /**< 4:2:0, planar, Y,CR(V),Cb(U) in separate plane */
  EDID_PIXEL_COLOR_FORMAT_Y_CR_CB_H2V2_16BPP,  /**< 4:2:2, planar, Y,CR(V),Cb(U) in separate plane */
  EDID_PIXEL_COLOR_FORMAT_Y_CR_CB_H2V1_16BPP = EDID_PIXEL_COLOR_FORMAT_Y_CR_CB_H2V2_16BPP,
  EDID_PIXEL_COLOR_FORMAT_Y_CBCR_SUPERTILE_4x2_12BPP,  /**< 4:2:0, Pseudo planar, Cr(V), Cb(U) are in 4x2 tile format*/
  EDID_PIXEL_COLOR_FORMAT_RGBA_5551_16BPP,
  EDID_PIXEL_COLOR_FORMAT_RGBA_8888_32BPP,
  EDID_PIXEL_COLOR_FORMAT_AYCBCR_H1V1_32BPP ,    /**< Alpha-YUV, Interleaved packing format, 4 bytes per pixel*/
  EDID_PIXEL_COLOR_FORMAT_Y_CB_CR_H2V2_12BPP,  /**< 4:2:0, planar, Y,Cb(U),Cr(V) in separate plane */
  EDID_PIXEL_COLOR_FORMAT_Y_CB_CR_H2V2_16BPP,  /**< 4:2:2, planar, Y,Cb(U),Cr(V) in separate plane */
  EDID_PIXEL_COLOR_FORMAT_Y_CB_CR_H2V1_16BPP = EDID_PIXEL_COLOR_FORMAT_Y_CB_CR_H2V2_16BPP,
  EDID_PIXEL_COLOR_FORMAT_ARGB_4444_16BPP,
  EDID_PIXEL_COLOR_FORMAT_RGB_101010_30BPP,
  EDID_PIXEL_COLOR_FORMAT_MAX,  
  EDID_PIXEL_COLOR_FORMAT_FORCE_32BIT = 0x7FFFFFFF
} EDID_PixelColorFormatType;

/*!
* \b EDID_RotateFlipType
*
* Define all variations of the 90 degree fixed rotation and flip. Some
* rotation cases are redundant.  For example, specifying EDID_ROTATE_90_FLIP_HORZ or
* EDID_ROTATE_270_FLIP_VERT will have the same effect (and are identical to HW).
*/
typedef enum
{
  EDID_ROTATE_NONE,           /**< Do not rotate. */
  EDID_ROTATE_90,             /**< Rotate clockwise 90 degrees. */
  EDID_ROTATE_180,            /**< Rotate clockwise 180 degrees. */
  EDID_ROTATE_270,            /**< Rotate clockwise 270 degrees. */
  EDID_ROTATE_NONE_FLIP_HORZ, /**< Rotate 0 degrees, flip horizontally. */
  EDID_ROTATE_NONE_FLIP_VERT, /**< Rotate 0 degrees, flip vertically. */
  EDID_ROTATE_90_FLIP_HORZ,   /**< Rotate 90 degrees, flip horizontally. */
  EDID_ROTATE_90_FLIP_VERT,   /**< Rotate 90 degrees, flip vertically. */
  EDID_ROTATE_180_FLIP_HORZ,  /**< Rotate 180 degrees, flip horizontally. */
  EDID_ROTATE_180_FLIP_VERT,  /**< Rotate 180 degrees, flip vertically. */
  EDID_ROTATE_270_FLIP_HORZ,  /**< Rotate 270 degrees, flip horizontally. */
  EDID_ROTATE_270_FLIP_VERT,  /**< Rotate 270 degrees, flip vertically. */
  EDID_ROTATE_MAX,            /**< Maximum number of rotating modes. */
  EDID_ROTATE_FORCE_32BIT = 0x7FFFFFFF
} EDID_RotateFlipType;

/*!
* \b EDID_3D_FrameFormatType
*
* Define all variations of 3D frame format. 
*/
typedef enum
{
  EDID_3D_FRAME_FORMAT_NONE                                = 0x0,
  EDID_3D_FRAME_FORMAT_FRAME_PACKING,                               /* Top and bottom full resolution frame packing */
  EDID_3D_FRAME_FORMAT_FIELD_ALTERNATIVE,                           /* Alternative Field for left and right frame */
  EDID_3D_FRAME_FORMAT_LINE_ALTERNATIVE,                            /* Line alternative between left and right frame */
  EDID_3D_FRAME_FORMAT_SIDE_BY_SIDE_FULL,                           /* Left and right are side by side full resolution */ 
  EDID_3D_FRAME_FORMAT_L_N_DEPTH,                                   /* Left and depth frame format */
  EDID_3D_FRAME_FORMAT_L_N_DEPTH_N_GFX_N_GFX_MINUS_DEPTH,           /* Left and depth and graphics and graphics - depth format */
  EDID_3D_FRAME_FORMAT_TOP_BOTTOM,                                  /* Top and bottom half resolution */
  EDID_3D_FRAME_FORMAT_RESERVED1,                                   /* Reserved for future use */
  EDID_3D_FRAME_FORMAT_SIDE_BY_SIDE_HALF,                           /* Left and right side by side half resolution */
  EDID_3D_FRAME_FORMAT_FRAME_INTERLEAVE                    = 0x10,  /* Left and right frames are consider separate input surface */
  EDID_3D_FRAME_FORMAT_MAX,
  EDID_3D_FRAME_FORMAT_FORCE_32BIT = 0x7FFFFFFF   
} EDID_3D_FrameFormatType;


/*!
* \b EDID_Display_ModeAspectRatioType
*
*  Indicates the various aspect ratio that the sink can support
*/
typedef enum
{
  EDID_DISPLAY_MODE_ASPECT_RATIO_NONE     = 0x0,
  EDID_DISPLAY_MODE_ASPECT_RATIO_4_3,                   /* Aspect ratio of 4:3 */
  EDID_DISPLAY_MODE_ASPECT_RATIO_16_9,                  /* Aspect ratio of 16:9 */
  EDID_DISPLAY_MODE_ASPECT_RATIO_16_10,                 /* Aspect ratio of 16:10 */
  EDID_DISPLAY_MODE_ASPECT_RATIO_5_4,                   /* Aspect ratio of 5:4 */
  EDID_DISPLAY_MODE_ASPECT_RATIO_15_9,                  /* Aspect ratio of 15:9 */
  EDID_DISPLAY_MODE_ASPECT_RATIO_5_3,                   /* Aspect ratio of 5:3 */ 
  EDID_DISPLAY_MODE_ASPECT_RATIO_MAX,
  EDID_DISPLAY_MODE_ASPECT_RATIO_32BIT    = 0x7FFFFFFF
} EDID_Display_ModeAspectRatioType;


/*!
* \b EDID_Display_ModeInfoType
*
* Structure containing information pertaining to a particular mode of operation
*  of the display. A same display can have different mode of operation  
*/
typedef struct
{
  uint32                           uModeIndex;       /**< mode Index start from 0 */
  uint32                           uWidthPx;         /**< visible display width (pixels).*/
  uint32                           uWidthTotalPx;    /**< total display width including blanking (pixels) */
  uint32                           uHeightPx;        /**< visible display height (pixels).*/
  uint32                           uHeightTotalPx;   /**< total display height including blanking (pixels) */
  uint32                           uRefreshRate;     /**< 16.16 fixed format. Refresh Rate of Display */
  uint32                           uPixelRate;       /**< Pixel frequency in Hz */
  EDID_PixelColorFormatType        eColorFormat;     /**< Color format of the display */
  EDID_RotateFlipType               eRotation;        /**< Indicates the orientation of the screen */
  bool32                           bInterlaced;      /**< Indicates interlace output or progressive output */
  EDID_3D_FrameFormatType           e3DFrameFormat;   /**< Indicates the type of 3D format this mode is associated with */
  EDID_Display_ModeAspectRatioType  eAspectRatio;     /**< Indicates the aspect ratio of the current mode */
  uint32                           uPixelClk;        /**< Indicates the pixel clock for this mode. */
  bool32                           bAudioSupported;  /**< Indicates whether current HDMI mode supports Audio*/
} EDID_Display_ModeInfoType;




//-------------------------------------------------------------------------------------------------
//  EDID_DispModeAttrType
//
//  @brief
//      Structure holds display attributes of each mode. 
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  EDID_VideoFormatType              eVideoFormat;        /**< HDMI VIC code equiavalent. */
  bool32                            bVideoOnly;          /**< Only video supported */
  uint32                            uPixelFormatMap;     /**< Map of supported pixel format flags */
  EDID_VendorBlockExtendedResType   eExtHdmiVic;         /**< HDMI_VIC code for Extended resolutions in the Vendor Specific Data block */
  EDID_Panel_ActiveTimingType        sActiveTimingInfo;   /**< Stores timing info for this particular mode */
  EDID_Display_ModeInfoType          sDisplayModeInfo;    /**< Stores mode characteristics for this particular mode */
} EDID_DispModeAttrType;

//-------------------------------------------------------------------------------------------------
//  EDID_DispModeListType
//
//  @brief
//      List of modes supported by the EDID. 
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  uint32                 uNumOfElements;                        /**< Number of parsed modes */
  EDID_DispModeAttrType  aDispModeAttr[EDID_VIDEO_FORMAT_MAX];  /**< List of modes. Array size to be defined */
} EDID_DispModeListType;

//-------------------------------------------------------------------------------------------------
//  EDID_ParserInfoType
//
//  @brief
//      Structure contains EDID buffer that has to be parsed and any mode limitations
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  uint32                    uEDIDDataLength;                                      /**< EDID data length */
  void                     *pEDIDBuffer;                                          /**< Pointer to raw EDID Data. */
  uint32                    uMaxModeWidth;                                        /**< Maximum mode width (in pixels), widths above this are filtered out of the mode list. */
  uint32                    uMaxModeHeight;                                       /**< Maximum mode height (in pixels), heights above this are filtered out of the mode list. */
  uint32                    uMaxModeRefreshRate;                                  /**< Maximum refresh rate (must divide by 1000 to get frequency in HZ), refresh rates above this are filtered out of the mode list */
  uint32                    uMinModeWidth;                                        /**< Minimum mode width (in pixels), widths below this are filtered out of the mode list. */
  uint32                    uMinModeHeight;                                       /**< Minimum mode height (in pixels), heights below this are filtered out of the mode list. */
  uint32                    uMinModeRefreshRate;                                  /**< Maximum refresh rate (must divide by 1000 to get frequency in HZ), refresh rates below this are filtered out of the mode list */
  uint64                    uMaxLinkRate;                                         /**< Maximum supported link rate ( in bits per second) */
  uint32                    uNumberOfLanes;                                       /**< Number of supported lanes */
  uint32                    uInjectedModeList[EDID_MAXIMUM_INJECTED_CEA_MODES];   /**< Injected CEA mode list. */
  uint32                    uInjectedModeNum;                                     /**< The number of injected CEA mode . */
  EDID_SpecificationType    eEDIDSpecType;                                        /**< HDMI/DP Specification type */
} EDID_ParserInfoType;

/* -----------------------------------------------------------------------
** EDID Utilities
** ----------------------------------------------------------------------- */

//-------------------------------------------------------------------------------------------------
//  EDID_OpenParser
//
//  @brief
//      Function initializes EDID parser and initializes the parser handle.
//      Caller is responsible to call this API to intialize the handle 
//      and pass the intialized handle as a parameter to other APIs.
//
//  @params
//      [IN/OUT] phHandle
//          Pointer to parser handle.
//
//  @return
//      EDID_STATUS_SUCCESS intialization is succesful.
//-------------------------------------------------------------------------------------------------
//
EDID_ErrorType EDID_OpenParser(EDID_HandleType *phHandle);

//-------------------------------------------------------------------------------------------------
//  EDID_CloseParser
//
//  @brief
//      De-initializes the parser.
//
//  @params
//      [IN] hHandle
//          Parser handle. 
//
//  @return
//      EDID_STATUS_SUCCESS de-intialization is succesful.
//-------------------------------------------------------------------------------------------------
//
EDID_ErrorType EDID_CloseParser(EDID_HandleType hHandle);

//-------------------------------------------------------------------------------------------------
//  EDID_GetModeList
//
//  @brief
//      Validates EDID and parses the modes within the EDID. 
//      The API also applies any mode filters specified by caller.
//
//  @params
//      [IN] hHandle
//          Parser handle.
//      [IN] pParserConfig
//          Paramter contains EDID buffer that has to be parsed and any mode limitations.
//      [OUT] pModeList
//          List of filtered modes parsed from EDID. Must be allocated by caller.
//
//  @return
//      EDID_STATUS_SUCCESS EDID is valid and parsing is sucessful
//-------------------------------------------------------------------------------------------------
//
EDID_ErrorType EDID_GetModeList(EDID_HandleType hHandle, EDID_ParserInfoType *pParserConfig, EDID_DispModeListType *pModeList);


//-------------------------------------------------------------------------------------------------
//  EDID_GetAudioModeInfo
//
//  @brief
//      Parses Audio block info. The API must be called after EDID_GetModeList() and 
//      using the same handle as EDID_GetModeList()
//
//  @params
//      [IN] hHandle
//          Parser handle.
//      [IN] uModeIndex
//          The mode thatis currently set; indexing to EDID_DispModeListType.aDispModeAttr.
//      [IN/OUT] pAudioParam
//          Pointer to a structure containing the properties of a short audio descriptor.
//      [IN] uFlags
//          Configuration flags
//
//  @return
//      EDID_STATUS_SUCCESS parsing of audio block is successful.
//-------------------------------------------------------------------------------------------------
//
#endif // __EDID_H__
