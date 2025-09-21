/*=============================================================================
 
  File: edid.c
 
  EDID functions and parser utilities 
  
  $Header: //components/rel/boot.xf/2.2/QcomPkg/Library/HALDPLib/edid.c#2 $$
  $DateTime: 2018/02/02 04:03:43 $$
  $Author: pwbldsvc $$
 
  Copyright (c) 2010-2018 Qualcomm Technologies, Inc.  
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */
#include "dp_i.h"
#include "dp_host.h"
#include "edid.h"

/* -----------------------------------------------------------------------
** Enums
** ----------------------------------------------------------------------- */

#define Q16                         (16)
#define Q16_MAX                     (1 << Q16)
/* Convert an integer to a Q16.16 value. */
#define Q16_VALUE(x)                ((int32)((uint64)x << Q16))

/* Convert a float to Q16.16 value */
#define Q16_FLOAT_VALUE(x)          ((int32)(Q16_VALUE(x) + (int32)((float)((x) - (uint32)(x)) * (float)(0x10000))))


//-------------------------------------------------------------------------------------------------
//  EDID_Video_AspectRatio
//
//  @brief
//      Defines the aspect ratio of the input video format 
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_VIDEO_ASPECT_RATIO_NONE          = 0,
  EDID_VIDEO_ASPECT_RATIO_4_3,
  EDID_VIDEO_ASPECT_RATIO_16_9,
  EDID_VIDEO_ASPECT_RATIO_5_4,  
  EDID_VIDEO_ASPECT_RATIO_16_10,  
  EDID_VIDEO_ASPECT_RATIO_5_3,
  EDID_VIDEO_ASPECT_RATIO_64_27,
  EDID_VIDEO_ASPECT_RATIO_256_135,
  EDID_VIDEO_ASPECT_RATIO_43_18,
  EDID_VIDEO_ASPECT_RATIO_3_2,
  EDID_VIDEO_ASPECT_RATIO_RESERVED,
  EDID_VIDEO_ASPECT_RATIO_MAX,
  EDID_VIDEO_ASPECT_RATIO_FORCE_32BIT,
} EDID_Video_AspectRatio;

//-------------------------------------------------------------------------------------------------
//  EDID_VendorBlock3DStructureType
//
//  @brief
//      Contains 3D structures indicated by the vendor data block 
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_VENDOR_BLOCK_3D_FORMAT_NONE                                  = 0x0,
  EDID_VENDOR_BLOCK_3D_FORMAT_FRAME_PACKING                         = 0x1,
  EDID_VENDOR_BLOCK_3D_FORMAT_FIELD_ALTERNATIVE                     = 0x2,
  EDID_VENDOR_BLOCK_3D_FORMAT_LINE_ALTERNATIVE                      = 0x4, 
  EDID_VENDOR_BLOCK_3D_FORMAT_SIDE_BY_SIDE_FULL                     = 0x8, 
  EDID_VENDOR_BLOCK_3D_FORMAT_L_PLUS_DEPTH                          = 0x10, 
  EDID_VENDOR_BLOCK_3D_FORMAT_L_PLUS_DEPTH_PLUS_GRAPHIC_DEPTH       = 0x20, 
  EDID_VENDOR_BLOCK_3D_FORMAT_TOP_BOTTOM                            = 0x40, 
  EDID_VENDOR_BLOCK_3D_FORMAT_SIDE_BY_SIDE_HALF_HORI_SUB_SAMPLE     = 0x100, 
  EDID_VENDOR_BLOCK_3D_FORMAT_SIDE_BY_SIDE_HALF_QUINCUNX_SUB_SAMPLE = 0x8000, 
  EDID_VENDOR_BLOCK_3D_FORMAT_MAX,
  EDID_VENDOR_BLOCK_3D_FORMAT_32BIT = 0x7FFFFFFF
} EDID_VendorBlock3DStructureType;

//-------------------------------------------------------------------------------------------------
//  EDID_VendorBlockPresentBitType
//
//  @brief
//      Contains all the present bits within the vendor specific data block
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_VENDOR_BLOCK_PRESENT_BIT_NONE         = 0,
  EDID_VENDOR_BLOCK_PRESENT_BIT_LATENCY      = 1,     /**< Progressive video/audio latency */
  EDID_VENDOR_BLOCK_PRESENT_BIT_I_LATENCY    = 2,     /**< Interlace video/audio latency */
  EDID_VENDOR_BLOCK_PRESENT_BIT_HDMI_VIDEO   = 4,     /**< Indicate extra HDMI video specific information */
  EDID_VENDOR_BLOCK_PRESENT_BIT_3D           = 8,     /**< Indicate all mandatory 3D formats are supported */
  EDID_VENDOR_BLOCK_PRESENT_BIT_MAX,
  EDID_VENDOR_BLOCK_PRESENT_BIT_32BIT = 0x7FFFFFFF
} EDID_VendorBlockPresentBitType;

//-------------------------------------------------------------------------------------------------
//  EDID_VendorBlockImageSizeInfoType
//
//  @brief
//      Indicates whether the image size field in the EDID structure is correct or not
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_VENDOR_BLOCK_IMAGE_SIZE_NO_ADDITIONAL_INFO       = 0,
  EDID_VENDOR_BLOCK_IMAGE_SIZE_RATIO_TRUE_SIZE_FALSE,        /**< Image size has correct aspect ratio but size not guarantee correct */
  EDID_VENDOR_BLOCK_IMAGE_SIZE_ROUND_NEAREST_CENTIMETER,     /**< Image size is correct and round to nearest centimeter */
  EDID_VENDOR_BLOCK_IMAGE_SIZE_DIVIDE_BY_FIVE,               /**< Image size is correct and divide by 5, only applicable when size is greater than 255cm */
  EDID_VENDOR_BLOCK_IMAGE_SIZE_MAX,
  EDID_VENDOR_BLOCK_IMAGE_SIZE_32BIT = 0x7FFFFFFF
} EDID_VendorBlockImageSizeInfoType;

//-------------------------------------------------------------------------------------------------
//  EDID_VendorBlockContentFilterType
//
//  @brief
//      Indicates if the sink can support various filtering pertaining to various content types
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_VENDOR_BLOCK_CONTENT_NONE     = 0x0,
  EDID_VENDOR_BLOCK_CONTENT_GRAPHICS = 0x1,        /**< Sink device applies no filtering to the pixel data */
  EDID_VENDOR_BLOCK_CONTENT_PHOTO    = 0x2,        /**< Sink device supports filtering for still pictures */
  EDID_VENDOR_BLOCK_CONTENT_CINEMA   = 0x4,        /**< Sink device supports filtering for cinema contents */
  EDID_VENDOR_BLOCK_CONTENT_GAME     = 0x8,        /**< Sink device supports processing with low audio and video latency */
  EDID_VENDOR_BLOCK_CONTENT_MAX,
  EDID_VENDOR_BLOCK_CONTENT_32BIT = 0x7FFFFFFF
} EDID_VendorBlockContentFilterType;

//-------------------------------------------------------------------------------------------------
//  EDID_AVI_ScanType
//
//  @brief
//      Defines HDMI display scan type. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AVI_SCAN_NONE = 0,  /**< Scan type will be up to the default of the HDMI display */
  EDID_AVI_SCAN_OVERSCAN,  /**<  Select overscan option for the HDMI display */
  EDID_AVI_SCAN_UNDERSCAN, /**<  Select underscan option for the HDMI display */
  EDID_AVI_SCAN_RESERVED,  /**< Reserved for future */
  EDID_AVI_SCAN_MAX,
  EDID_AVI_SCAN_FORCE_32BIT = 0x7FFFFFFF
} EDID_AVI_ScanType;

//-------------------------------------------------------------------------------------------------
//  EDID_AVI_ContentFilterType
//
//  @brief
//      Defines content type supported by sink. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{ 
  EDID_CONTENT_TYPE_NONE     = 0x0,
  EDID_CONTENT_TYPE_GRAPHICS = 0x1,     /**< Sink device applies no filtering to the pixel data */
  EDID_CONTENT_TYPE_PHOTO    = 0x2,     /**< Sink device supports filtering for still pictures */
  EDID_CONTENT_TYPE_CINEMA   = 0x4,     /**< Sink device supports filtering for cinema contents */
  EDID_CONTENT_TYPE_GAME     = 0x8,     /**< Sink device supports processing with low audio and video latency */
  EDID_CONTENT_TYPE_MAX,
  EDID_CONTENT_TYPE_32BIT    = 0x7FFFFFFF,
} EDID_AVI_ContentFilterType;


//-------------------------------------------------------------------------------------------------
//  EDID_AVI_ActiveFormatType
//
//  @brief
//      Defines the display aspect ratio of the active pixels. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AVI_ACTIVE_FORMAT_NONE = 0,
  EDID_AVI_ACTIVE_FORMAT_RESERVED,                 /**< Reserved for future use */
  EDID_AVI_ACTIVE_FORMAT_BOX_16_9_TOP,             /**< Active format is 16:9 and shift top */
  EDID_AVI_ACTIVE_FORMAT_BOX_14_9_TOP,             /**< Active format is 14:9 and shift top */
  EDID_AVI_ACTIVE_FORMAT_BOX_16_9_CENTER,          /**< Active format is 16:9 and shift center */
  EDID_AVI_ACTIVE_FORMAT_RESERVED1,                /**< Reserved for future use */
  EDID_AVI_ACTIVE_FORMAT_RESERVED2,                /**< Reserved for future use */
  EDID_AVI_ACTIVE_FORMAT_RESERVED3,                /**< Reserved for future use */
  EDID_AVI_ACTIVE_FORMAT_CODED_FRAME,              /**< Active format is same as coded frame */
  EDID_AVI_ACTIVE_FORMAT_4_3_CENTER,               /**< Active format is 4:3 shifted center */
  EDID_AVI_ACTIVE_FORMAT_16_9_CENTER,              /**< Active format is 16:9 shifted center */
  EDID_AVI_ACTIVE_FORMAT_14_9_CENTER,              /**< Active format is 14:9 shifted center */
  EDID_AVI_ACTIVE_FORMAT_RESERVED4,                /**< Reserved for future use */
  EDID_AVI_ACTIVE_FORMAT_4_3_PROTECT_14_9_CENTER,  /**< Active format is 4:3 with shoot and protect and 14:9 center */
  EDID_AVI_ACTIVE_FORMAT_16_9_PROTECT_14_9_CENTER, /**< Active format is 16:9 with shoot and protect and 14:9 center */
  EDID_AVI_ACTIVE_FORMAT_16_9_PROTECT_4_3_CENTER,  /**< Active format is 16:9 with shoot and protect and 4:3 center */
  EDID_AVI_ACTIVE_FORMAT_MAX,
  EDID_AVI_ACTIVE_FORMAT_FORCE_32BIT = 0x7FFFFFFF,
} EDID_AVI_ActiveFormatType;


//-------------------------------------------------------------------------------------------------
//  EDID_AudioChannelNumType
//
//  @brief
//      The various audio channel number for HDMI Display. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AUDIO_CHANNEL_NONE = 0x0,  /**< 0 Channels */
  EDID_AUDIO_CHANNEL_2    = 0x1,  /**< 2 Channels */
  EDID_AUDIO_CHANNEL_4    = 0x2,  /**< 4 Channels */
  EDID_AUDIO_CHANNEL_6    = 0x4,  /**< 6 Channels */
  EDID_AUDIO_CHANNEL_8    = 0x8,  /**< 8 Channels */
  EDID_AUDIO_CHANNEL_MAX,
  EDID_AUDIO_CHANNEL_FORCE_32BIT = 0x7FFFFFFF
} EDID_AudioChannelType;

//-------------------------------------------------------------------------------------------------
//  EDID_AudioSampleRateType
//
//  @brief
//      The various audio sample rate for HDMI Display. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AUDIO_SAMPLE_RATE_NONE              = 0x0,
  EDID_AUDIO_SAMPLE_RATE_32KHZ             = 0x1,
  EDID_AUDIO_SAMPLE_RATE_44_1KHZ           = 0x2,
  EDID_AUDIO_SAMPLE_RATE_48KHZ             = 0x4,
  EDID_AUDIO_SAMPLE_RATE_88_2KHZ           = 0x8,
  EDID_AUDIO_SAMPLE_RATE_96KHZ             = 0x10,
  EDID_AUDIO_SAMPLE_RATE_176_4KHZ          = 0x20,
  EDID_AUDIO_SAMPLE_RATE_192KHZ            = 0x40,
  EDID_AUDIO_SAMPLE_RATE_MAX,
  EDID_AUDIO_SAMPLE_RATE_FORCE_32BIT = 0x7FFFFFFF
} EDID_AudioSampleRateType;

//-------------------------------------------------------------------------------------------------
//  EDID_AudioSampleBitDepthType
//
//  @brief
//      The various audio sample bit depth for HDMI Display. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AUDIO_BIT_DEPTH_NONE   = 0x0,              /**< No Audio Bit Depth */
  EDID_AUDIO_BIT_DEPTH_16_BIT = 0x1,              /**< Audio Bit Depth 16bit per sample*/
  EDID_AUDIO_BIT_DEPTH_20_BIT = 0x2,              /**< Audio Bit Depth 20bit per sample*/
  EDID_AUDIO_BIT_DEPTH_24_BIT = 0x4,              /**< Audio Bit Depth 24bit per sample*/
  EDID_AUDIO_BIT_DEPTH_MAX,
  EDID_AUDIO_BIT_DEPTH_FORCE_32BIT = 0x7FFFFFFF
} EDID_AudioSampleBitDepthType;

//-------------------------------------------------------------------------------------------------
//  EDID_AudioFormatType
//
//  @brief
//      Defines the audio formats supported by the HDMI display. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AUDIO_FORMAT_RESERVED = 0,                  /**< Reserved for future */
  EDID_AUDIO_FORMAT_LPCM,                          /**< Audio codec type Linear PCM */
  EDID_AUDIO_FORMAT_AC3,                           /**< Audio codec type AC3 */
  EDID_AUDIO_FORMAT_MPEG1_LAYER1_AND_2,            /**< Audio codec type MPEG1 Layer1 and 2 */
  EDID_AUDIO_FORMAT_MP3,                           /**< Audio codec type MP3 */
  EDID_AUDIO_FORMAT_MPEG2,                         /**< Audio codec type MPEG2 */
  EDID_AUDIO_FORMAT_AAC,                           /**< Audio codec type AAC */
  EDID_AUDIO_FORMAT_DTS,                           /**< Audio codec type DTS */
  EDID_AUDIO_FORMAT_ATRAC,                         /**< Audio codec type ATRAC */
  EDID_AUDIO_FORMAT_ONE_BIT_AUDIO,                 /**< Audio codec type 1 bit audio */
  EDID_AUDIO_FORMAT_DOLBY_DIGITAL_PLUS,            /**< Audio codec type Dolby Digital+ */
  EDID_AUDIO_FORMAT_DTS_HD,                        /**< Audio codec type DTS HD */
  EDID_AUDIO_FORMAT_MAT,                           /**< Audio codec type MAT */
  EDID_AUDIO_FORMAT_DST,                           /**< Audio codec type DST */
  EDID_AUDIO_FORMAT_WMA_PRO,                       /**< Audio codec type WMA PRO */
  EDID_AUDIO_FORMAT_RESERVED1,                     /**< Reserved for future */
  EDID_AUDIO_FORMAT_MAX,
  EDID_AUDIO_FORMAT_FORCE_32BIT = 0x7FFFFFFF
} EDID_AudioFormatType;

//-------------------------------------------------------------------------------------------------
//  EDID_AudioChannelAllocType
//
//  @brief
//      Defines the mapping between the audio channel and sink device speakers. 
//
//      FL  - Front Left
//      FC  - Front Center
//      FR  - Front Right
//      FLC - Front Left Center
//      FRC - Front Right Center
//      RL  - Rear Left
//      RC  - Rear Center
//      RR  - Rear Right
//      RLC - Rear Left Center
//      RRC - Rear Right Center
//      LFE - Low Frequencey Effect
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AUDIO_CHANNEL_ALLOC_CODE_0  = 0x0, /**< Allocate to FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_1,        /**< Allocate to LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_2,        /**< Allocate to FC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_3,        /**< Allocate to FC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_4,        /**< Allocate to RC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_5,        /**< Allocate to RC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_6,        /**< Allocate to RC, FC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_7,        /**< Allocate to RC, FC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_8,        /**< Allocate to RR, RL, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_9,        /**< Allocate to RR, RL, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_10,       /**< Allocate to RR, RL, FC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_11,       /**< Allocate to RR, RL, FC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_12,       /**< Allocate to RC, RR, RL, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_13,       /**< Allocate to RC, RR, RL, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_14,       /**< Allocate to RC, RR, RL, FC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_15,       /**< Allocate to RC, RR, RL, FC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_16,       /**< Allocate to RRC, RLC, RR, RL, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_17,       /**< Allocate to RRC, RLC, RR, RL, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_18,       /**< Allocate to RRC, RLC, RR, RL, FC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_19,       /**< Allocate to RRC, RLC, RR, RL, FC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_20,       /**< Allocate to FRC, FLC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_21,       /**< Allocate to FRC, FLC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_22,       /**< Allocate to FRC, FLC, FC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_23,       /**< Allocate to FRC, FLC, FC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_24,       /**< Allocate to FRC, FLC, RC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_25,       /**< Allocate to FRC, FLC, RC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_26,       /**< Allocate to FRC, FLC, RC, FC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_27,       /**< Allocate to FRC, FLC, RC, FC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_28,       /**< Allocate to FRC, FLC, RR, RL, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_29,       /**< Allocate to FRC, FLC, RR, RL, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_30,       /**< Allocate to FRC, FLC, RR, RL, FC, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_31,       /**< Allocate to FRC, FLC, RR, RL, FC, LFE, FR, FL speakers */
  EDID_AUDIO_CHANNEL_ALLOC_CODE_MAX,      
  EDID_AUDIO_CHANNEL_ALLOC_CODE_FORCE_32BIT = 0x7FFFFFFF
}EDID_AudioChannelAllocType;

//-------------------------------------------------------------------------------------------------
//  EDID_AudioLevelShiftType
//
//  @brief
//      Defines the audio attenuation level to be shifted by the sink device. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AUDIO_LEVEL_SHIFT_0DB   = 0x0,  /**< 0 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_1DB,          /**< 1 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_2DB,          /**< 2 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_3DB,          /**< 3 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_4DB,          /**< 4 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_5DB,          /**< 5 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_6DB,          /**< 6 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_7DB,          /**< 7 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_8DB,          /**< 8 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_9DB,          /**< 9 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_10DB,         /**< 10 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_11DB,         /**< 11 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_12DB,         /**< 12 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_13DB,         /**< 13 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_14DB,         /**< 14 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_15DB,         /**< 15 Decibel */
  EDID_AUDIO_LEVEL_SHIFT_MAX,
  EDID_AUDIO_LEVEL_SHIFT_FORCE_32BIT = 0x7FFFFFFF
}EDID_AudioLevelShiftType;

//-------------------------------------------------------------------------------------------------
//  EDID_AudioSpeakerLocationType
//
//  @brief
//      Defines various audio speaker locations. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AUDIO_SPEAKER_LOCATION_NONE   = 0x0,  /**< No Speakers */
  EDID_AUDIO_SPEAKER_LOCATION_FL     = 0x1,  /**< FL  - Front Left */
  EDID_AUDIO_SPEAKER_LOCATION_FC     = 0x2,  /**< FC  - Front Center */
  EDID_AUDIO_SPEAKER_LOCATION_FR     = 0x4,  /**< FR  - Front Right */
  EDID_AUDIO_SPEAKER_LOCATION_FLC    = 0x8,  /**< FLC - Front Left Center */
  EDID_AUDIO_SPEAKER_LOCATION_FRC    = 0x10, /**< FRC - Front Right Center */
  EDID_AUDIO_SPEAKER_LOCATION_RL     = 0x20, /**< RL  - Rear Left */
  EDID_AUDIO_SPEAKER_LOCATION_RC     = 0x40, /**< RC  - Rear Center */
  EDID_AUDIO_SPEAKER_LOCATION_RR     = 0x80, /**< RR  - Rear Right */
  EDID_AUDIO_SPEAKER_LOCATION_RLC    = 0x100,/**< RLC - Rear Left Center */
  EDID_AUDIO_SPEAKER_LOCATION_RRC    = 0x200,/**< RRC - Rear Right Center */
  EDID_AUDIO_SPEAKER_LOCATION_LFE    = 0x400,/**< LFE - Low Frequency Effect */
  EDID_AUDIO_SPEAKER_LOCATION_MAX,
  EDID_AUDIO_SPEAKER_LOCATION_FORCE_32BIT = 0x7FFFFFFF
} EDID_AudioSpeakerLocationType;

//-------------------------------------------------------------------------------------------------
//  EDID_AVI_ColorFormatType
//
//  @brief
//      Defines AVI color formats. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AVI_COLOR_FORMAT_RGB,       
  EDID_AVI_COLOR_FORMAT_YUV422,    
  EDID_AVI_COLOR_FORMAT_YUV444,
  EDID_AVI_COLOR_FORMAT_YUV420,
  EDID_AVI_COLOR_FORMAT_RESERVED,
  EDID_AVI_COLOR_FORMAT_MAX,
  EDID_AVI_COLOR_FORMAT_32BIT = 0x7FFFFFFF
} EDID_AVI_ColorFormatType;

//-------------------------------------------------------------------------------------------------
//  EDID_AVI_Quantization
//
//  @brief
//      Defines the color encoding standard for the input video data. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AVI_QUANTIZATION_NONE          = 0,
  EDID_AVI_QUANTIZATION_LIMITED_RANGE,
  EDID_AVI_QUANTIZATION_FULL_RANGE,
  EDID_AVI_QUANTIZATION_MAX,
  EDID_AVI_QUANTIZATION_FORCE_32BIT = 0x7FFFFFFF
} EDID_AVI_Quantization;

//-------------------------------------------------------------------------------------------------
//  EDID_AVI_ScaleInfo
//
//  @brief
//      Defines whether source data has been scaled in x and y direction. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AVI_SCALE_NONE          = 0, /**< No scaling */
  EDID_AVI_SCALE_HORIZONTAL,        /**< Horizontal scaling */
  EDID_AVI_SCALE_VERTICAL,          /**< Vertical scaling */
  EDID_AVI_SCALE_BOTH,              /**< Both horizontal and vertical scaling */
  EDID_AVI_SCALE_MAX,
  EDID_AVI_SCALE_FORCE_32BIT,
} EDID_AVI_ScaleInfo;

//-------------------------------------------------------------------------------------------------
//  EDID_AVI_Colorimetry
//
//  @brief
//      Defines the color encoding standard for the input video data. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_AVI_COLORIMETRY_NONE       = 0,
  EDID_AVI_COLORIMETRY_ITU601,
  EDID_AVI_COLORIMETRY_ITU709,
  EDID_AVI_COLORIMETRY_XVYCC601,
  EDID_AVI_COLORIMETRY_XVYCC709,
  EDID_AVI_COLORIMETRY_SYCC601,
  EDID_AVI_COLORIMTERY_ADOBEYCC601,
  EDID_AVI_COLORIMTERY_ADOBERGB     = 8,
  EDID_AVI_COLORIMETRY_MAX,
  EDID_AVI_COLORIMETRY_FORCE_32BIT = 0x7FFFFFFF
} EDID_AVI_Colorimetry;

//-------------------------------------------------------------------------------------------------
//  EDID_VideoFormatYUV420Support_Type
//
//  @brief
//      Enum indicating YUV420 support. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_VIDEO_YUV420_NOT_SUPPORTED                       = 0,   /**< Sink does not support YUV420 pixel data for this video format */
  EDID_VIDEO_YUV420_ALSO_SUPPORTED,                            /**< Sink supports YUV420 pixel format for this video format  apart from RGB888*/
  EDID_VIDEO_YUV420_ONLY_SUPPORTED,                            /**< Sink only supports YUV420 pixel data for this video format */
  EDID_VIDEO_YUV420_SUPPORTED_MAX,
  EDID_VIDEO_YUV420_SUPPORTED_32BIT = 0x7FFFFFFF
} EDID_VideoFormatYUV420Support_Type;


/* -----------------------------------------------------------------------
** Structures
** ----------------------------------------------------------------------- */

//-------------------------------------------------------------------------------------------------
//  EDID_DataBlockType
//
//  @brief
//      Contains all the data block offsets from the EDID ROM
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  /* Block 0 has fixed detail timing block offset, hence no need to include here for dynamic calculation */
  struct
  {
    uint32 uDetailTimingBlockOffset;      /* Offset of the 1st detail timing data block */
    uint32 uDataBlockLength;              /* Length of this data block */
  } sDetailTimingBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

  /* There could be more than one video data block within the same CEA extension block */
  struct 
  {
    uint32 uVideoDataBlockOffset[EDID_MAX_NUM_OF_VIDEO_DATA_BLOCKS];         /* Offset of the video data block */
    uint32 uDataBlockLength[EDID_MAX_NUM_OF_VIDEO_DATA_BLOCKS];              /* Length of this data block */
  } sVideoDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

  struct
  {
    uint32 uAudioDataBlockOffset;         /* Offset of the audio data block */
    uint32 uDataBlockLength;              /* Length of this data block */
  } sAudioDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

  struct 
  {
    uint32 uVideoDataBlockOffset;         /* Offset of the Extended video data block */
    uint32 uDataBlockLength;              /* Length of this data block */
  } sExtVideoDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

  struct
  {
    uint32 uAudioDataBlockOffset;         /* Offset of the audio data block */
    uint32 uDataBlockLength;              /* Length of this data block */
  } sExtAudioDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

  struct
  {
    uint32                                  uVendorSpecificBlockOffset;     /* Offset of the vendor specific data block */
    uint32                                  uVendorSpecificBlockExtOffset;  /* Offset to the extension fields */
    uint32                                  uDataBlockLength;               /* Length of this data block */
    uint32                                  ePresetBitFlags;                /* Indicate all the present bits (Use EDID_VendorBlockPresentBitType) */
    uint32                                  uIEEERegistrationID;            /* 24 bit IEEE registration ID */
    bool32                                  bAISupport;                     /* Supports ACP, ISRC1 and ISRC2 packets or not */
    bool32                                  bDVIDualSupport;                /* Supports DVI dual mode or not */
    uint16                                  uPhysicalAddress;               /* 16 bit source physical address for CEC purposes*/
    uint8                                   uSupportedDeepColorModeMask;    /* Deep color modes that are supported by the sinkk device */
    uint8                                   uMaxTMDSClock;                  /* Maximum TMDS clock supported by the sink device*/
    uint8                                   uVideoLatency;                  /* Video latency for progressive video */
    uint8                                   uAudioLatency;                  /* Audio latency for progressive video */
    uint8                                   uInterlacedVideoLatency;        /* Video latency for interlaced video */
    uint8                                   uInterlacedAudioLatency;        /* Audio latency for interlaced video */
    EDID_VendorBlockImageSizeInfoType  eImageSizeInfo;                 /* Image size information */
    uint8                                   uSupportedContentFilterMask;    /* Supported filters for various content types (Use EDID_VendorBlockContentFilterType)*/
    uint8                                   uVideo2DDescLen;                /* Stores the value of the HDMI VIC LEN */
    uint8                                   uVideo3DDescLen;                /* Stores the value of the HDMI 3D LEN */
    uint8                                   u3DMultiPresentFieldVal;        /* Stores the type of 3D descriptors inside the data block */
  } sVendorDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

  struct
  {
    uint32    uSpeakerAllocBlockOffset;      /* Offset of the speaker allocation data block */
    uint32    uDataBlockLength;              /* Length of this data block */
  } sSpeakerDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];
  
  struct
  {
    uint32    uVideoCapDataBlockOffset;     /* Offset to the video capability data block */
    uint32    uDataBlockLength;             /* Length of this data block */
  } sVideoCapDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

  struct
  {
    uint32    uColorimetryDataBlockOffset;  /* Offset to the colorimetry data block */
    uint32    uDataBlockLength;             /* Length of this data block */
  } sColorimetryDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

  struct
  {
    uint32    uHDMIVideoDataBlockOffset;    /* Offset to the HDMI Video data block (HDMI 2.0 features support) */
    uint32    uDataBlockLength;             /* Length of this data block */
  } sHDMIVideoDataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];
  
  struct
  {
    uint32    uYUV420DataBlockOffset;       /* Offset to the YCbCr420 Video data block */
    uint32    uDataBlockLength;             /* Length of this data block */
  } sYUV420DataBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];
  struct
  {
    uint32    uYUV420CapabilityMapOffset;    /* Offset to the YCbCr420 capability data map block */
    uint32    uDataBlockLength;              /* Length of this data block */
  } sYUV420CapabilityMapBlockDesc[EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS];

} EDID_DataBlockType;

//-------------------------------------------------------------------------------------------------
//  EDID_AVI_LetterBoxInfoType
//
//  @brief
//      Defines letter box width and height. 
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  uint32 uEndOfTopBarLine;        /**< Height of the top letter box bar */
  uint32 uStartOfBottomBarLine;   /**< Height of the bottom letter box bar */
  uint32 uEndOfLeftBarLine;       /**< Width of the left letter box bar */
  uint32 uStartOfRightBarLine;    /**< Width of the right letter box bar */
} EDID_AVI_LetterBoxInfoType;


//-------------------------------------------------------------------------------------------------
//  EDID_AudioModeInfoType
//
//  @brief
//      Structure capturing all audio capabilities reported by the sink device . 
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  uint32                         uAudioModeIndex;            /**< Index to query each of the audio short descriptors reported by the sink device */
  EDID_AudioFormatType           eAudioFormat;               /**< Audio format this mode supports */
  uint16                         uAudioSampleRateMask;       /**< All sampling rates this mode can support (Use HDMI_AudioSampleRateType) */
  uint16                         uAudioChannelMask;          /**< Maximum number of audio channels this mode can support */
  uint32                         uAudioSpeakerLocationMask;  /**< All speaker locations that the sink device can support (Use HDMI_AudioSpeakerLocationType) */
  union
  {
    uint32                       uAudioSampleBitDepthMask;   /**< For LPCM audio format, this field indicates all the sample bit depths this mode can support (Use HDMI_AudioSampleBitDepthType) */
    uint32                       uAudioMaxBitRate;           /**< Compressed audio, this field indicates the maximum bitrate this mode can support */
  }audioStreamInfo;
  uint32                         uReserved[7];               /**< Reserved for future use */
  
} EDID_AudioModeInfoType;


//-------------------------------------------------------------------------------------------------
//  EDID_DispModeTimingType
//
//  @brief
//      Structure holds info specific to video format such as timing, mode capabilities
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  EDID_VideoFormatType      eVideoFormat;         /**< Video Format ID */
  uint32                    uActiveH;             /**< Horizontal active pixels */
  uint32                    uFrontPorchH;         /**< Horizontal front porch pixels */
  uint32                    uPulseWidthH;         /**< Horizontal sync pulse width pixels */
  uint32                    uBackPorchH;          /**< Horizontal back porch pixels */
  bool32                    bActiveLowH;          /**< Horizontal sync polarity */
  uint32                    uActiveV;             /**< Vertical active pixels */
  uint32                    uFrontPorchV;         /**< Vertical front porch pixels */
  uint32                    uPulseWidthV;         /**< Vertical sync pulse width pixels */
  uint32                    uBackPorchV;          /**< Vertical back porch pixels */
  bool32                    bActiveLowV;          /**< Vertical sync polarity */
  uint32                    uPixelFreq;           /**< Must divide by 1000 to get the actual frequency in MHZ */
  uint32                    uRefreshRate;         /**< Must divide by 1000 to get the actual frequency in HZ */
  bool32                    bInterlaced;          /**< Indicates format is interlaced or not */
  EDID_SpecificationType    eEDIDSpecType;        /**< HDMI/DP specification type */
  uint32                    uPixelRepeatFactor;   /**< Pixel repetition factor denoted as 2^n where is n is the repetition factor */
  EDID_Video_AspectRatio    eAspectRatio;         /**< Aspect ratio of the format */
  uint32                    uModeFlags;           /**< Flags to indicate whether certain features are supported */
} EDID_DispModeTimingType;

//-------------------------------------------------------------------------------------------------
//  EDID_VideoModePropertyType
//
//  @brief
//      Structure holds detailed timing info for the video format. 
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  EDID_VideoFormatType  eVideoCode;         /**< Video Format ID */
  uint32                uActiveH;           /**< Horizontal active pixels */
  uint32                uActiveV;           /**< Vertical active pixels */
  bool32                bInterlaced;        /**< Indicates format is interlaced or not */
  uint32                uTotalH;            /**< Horizontal total pixels */
  uint32                uTotalBlankH;       /**< Horizontal blanking pixels */
  uint32                uTotalV;            /**< Vertical total pixels */
  uint32                uTotalBlankV;       /**< Vertical blanking pixels */
  uint32                fFreqH;             /**< Must divide by 1000 to get the actual frequency in MHZ */
  uint32                fFreqV;             /**< Must divide by 1000 to get the actual frequency in MHZ */
  uint32                fPixelFreq;         /**< Must divide by 1000 to get the actual frequency in MHZ */
  uint32                fRefreshRate;       /**< Must divide by 1000 to get the actual frequency in MHZ */
  bool32                bAspectRatio4_3;    /**< Indicates whether aspect ratio is 4:3 */
} EDID_VideoModePropertyType;

//-------------------------------------------------------------------------------------------------
//  EDID_ParserContextType
//
//  @brief
//      Structure holds information about EDID parser context. 
//
//-------------------------------------------------------------------------------------------------
//
typedef struct
{
  bool32                    bInitialized;                                         /**< Indicates whether context is initialized*/
  bool32                    bEDIDParserComplete;                                  /**< Indicates whether the EDID parsing is complete */
  bool32                    bAudioSupport;                                        /**< Tracks whether basic audio is supported in EDID */
  bool32                    bYUVSupport;                                          /**< Indicates YUV support */ 
  uint32                    uEDIDDataLength;                                      /**< EDID data length */
  uint32                    uActiveModeIndex;                                     /**< Current mode that is set */
  EDID_DispModeListType     sDispModeList;                                        /**< List of modes parsed by the parser */
  uint8                     auEDIDCache[EDID_MAX_BUF_SIZE];                       /**< Cached EDID Data*/
  EDID_DataBlockType        sDataBlockDesc;                                       /**< Structure containing all block offsets */
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
  uint32                    uNumCompatibleAudioModes;                             /**< Total number of audio modes that are compabile with the audio output */
  EDID_SpecificationType    eEDIDSpecType;                                        /**< HDMI/DP Specification type */
} EDID_ParserContextType;


/* -----------------------------------------------------------------------
** Defines
** ----------------------------------------------------------------------- */

// Maximum number of EDID parser contexts
#define EDID_MAX_CTX_NUM                                      1

// Flags to be used for EDID_DispModeTimingType.uModeFlags
#define HDMI_AUDIO_SUPPORTED                                  0x00000001 
#define HDMI_YUV420_SUPPORTED                                 0x00000002

#define EDID_STANDARD_TIMING_FREQ_DELTA                       2           // Maximum acceptable delta in refresh rate (in hertz)

// Indicates that the mode is supported by all specifications
#define EDID_SPEC_ALL                                         (EDID_HDMI_SPEC_TYPE_1_4 |  \
                                                               EDID_HDMI_SPEC_TYPE_2_0 |  \
                                                               EDID_DP_SPEC_TYPE_1_3)

// Indicates that the mode is supported by HDMI2.0 and DP1.3 specifications
#define EDID_SPEC_HDMI2P0_DP1P3                               (EDID_HDMI_SPEC_TYPE_2_0 |  \
                                                               EDID_DP_SPEC_TYPE_1_3)


// EDID context containing EDID parsing info
#ifdef KUOGEE
EDID_ParserContextType gEDIDSharedCtx[EDID_MAX_CTX_NUM] = {0};
#else
EDID_ParserContextType gEDIDSharedCtx[EDID_MAX_CTX_NUM];
#endif

/* -----------------------------------------------------------------------
** Macros
** ----------------------------------------------------------------------- */

#define EDID_GETCTX(_id_)  (_id_ < EDID_MAX_CTX_NUM) ? (&gEDIDSharedCtx[_id_]) : (NULL)

// Returns absolute value
#define EDID_ABS(a)        (((a) >= 0)  ? (a) : (-(a)))

/* -----------------------------------------------------------------------
** Tables
** ----------------------------------------------------------------------- */
// Table contains info specific to each video format such as timing info, mode capabilities
EDID_DispModeTimingType gEDIDSupportedVideoModeLUT[EDID_VIDEO_FORMAT_MAX]=
{
  {EDID_VIDEO_FORMAT_640x480p60_4_3,                  640,    16,     96,     48,     TRUE,   480,    10,     2,      33,     TRUE,   25200,    60000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x480p60_4_3,                  720,    16,     62,     60,     TRUE,   480,    9,      6,      30,     TRUE,   27027,    60000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x480p60_16_9,                 720,    16,     62,     60,     TRUE,   480,    9,      6,      30,     TRUE,   27027,    60000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p60_16_9,                1280,   110,    40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  74250,    60000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080i60_16_9,               1920,   88,     44,     148,    FALSE,  540,    2,      5,      15,     FALSE,  74250,    60000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x480i60_4_3,                 720,    38,     124,    114,    TRUE,   240,    4,      3,      15,     TRUE,   27027,    60000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   1,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x480i60_16_9,                720,    38,     124,    114,    TRUE,   240,    4,      3,      15,     TRUE,   27027,    60000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   1,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x240p60_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   1,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x240p60_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   1,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x480i60_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x480i60_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x240p60_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x240p60_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x480p60_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x480p60_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p60_16_9,               1920,   88,     44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  148500,   60000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x576p50_4_3,                  720,    12,     64,     68,     TRUE,   576,    5,      5,      39,     TRUE,   27000,    50000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x576p50_16_9,                 720,    12,     64,     68,     TRUE,   576,    5,      5,      39,     TRUE,   27000,    50000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p50_16_9,                1280,   440,    40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  74250,    50000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080i50_16_9,               1920,   528,    44,     148,    FALSE,  540,    2,      5,      15,     FALSE,  74250,    50000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x576i50_4_3,                 720,    24,     126,    138,    TRUE,   288,    2,      3,      19,     TRUE,   27000,    50000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   1,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x576i50_16_9,                720,    24,     126,    138,    TRUE,   288,    2,      3,      19,     TRUE,   27000,    50000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   1,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x288p50_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   1,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x288p50_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   1,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x576i50_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x576i50_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x288p50_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x288p50_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x576p50_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x576p50_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p50_16_9,               1920,   528,    44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  148500,   50000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p24_16_9,               1920,   638,    44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  74250,    24000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p25_16_9,               1920,   528,    44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  74250,    25000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p30_16_9,               1920,   88,     44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  74250,    30000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x480p60_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x480p60_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x576p50_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2880x576p50_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080i50_16_9_special,       1920,   32,     168,    184,    FALSE,  540,    23,     5,      57,     TRUE,   72000,    50000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080i100_16_9,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p100_16_9,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x576p100_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x576p100_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x576i100_4_3,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x576i100_16_9,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080i120_16_9,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p120_16_9,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x480p120_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x480p120_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x480i120_4_3,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x480i120_16_9,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x576p200_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x576p200_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x576i200_4_3,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x576i200_16_9,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x480p240_4_3,                 0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_720x480p240_16_9,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x480i240_4_3,                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1440x480i240_16_9,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},

  // CEA 861-F
  {EDID_VIDEO_FORMAT_1280x720p24_16_9,                1280,   1760,   40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  59400,    24000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p25_16_9,                1280,   2420,   40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  74250,    25000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p30_16_9,                1280,   1760,   40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  59400,    30000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p120_16_9,              1280,   88,     44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  297000,   120000,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p100_16_9,              1280,   528,    44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  297000,   100000,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p24_64_27,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p25_64_27,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p30_64_27,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p50_64_27,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p60_64_27,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p100_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1280x720p120_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p24_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p25_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p30_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p50_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p60_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p100_64_27,             0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1920x1080p120_64_27,             0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1680x720p24_64_27,               1680,   1360,   40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  59400,    24000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1680x720p25_64_27,               1680,   1228,   40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  59400,    25000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1680x720p30_64_27,               1680,   700,    40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  59400,    30000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1680x720p50_64_27,               1680,   260,    40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  82500,    50000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1680x720p60_64_27,               1680,   260,    40,     220,    FALSE,  720,    5,      5,      20,     FALSE,  99000,    60000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1680x720p100_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_1680x720p120_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2560x1080p24_64_27,              2560,   998,    44,     148,    FALSE,  1080,   4,      5,      11,     FALSE,  99000,    24000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_10,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2560x1080p25_64_27,              2560,   448,    44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  90000,    25000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_10,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2560x1080p30_64_27,              2560,   768,    44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  118800,   30000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_10,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2560x1080p50_64_27,              2560,   548,    44,     148,    FALSE,  1080,   4,      5,      36,     FALSE,  185000,   50000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_10,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2560x1080p60_64_27,              2560,   248,    44,     148,    FALSE,  1080,   4,      5,      11,     FALSE,  198000,   60000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_10,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2560x1080p100_64_27,             0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_2560x1080p120_64_27,             0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p24_16_9,               3840,   1276,   88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  297000,   24000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p25_16_9,               3840,   1056,   88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  297000,   25000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9,               3840,   176,    88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  297000,   30000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p50_16_9,               3840,   1056,   88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  594000,   50000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED|HDMI_YUV420_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9,               3840,   176,    88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  594000,   60000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED|HDMI_YUV420_SUPPORTED},
  {EDID_VIDEO_FORMAT_4096x2160p24_256_135,            4096,   1020,   88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  297000,   24000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_256_135,    HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_4096x2160p25_256_135,            0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_256_135,    HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_4096x2160p30_256_135,            0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_256_135,    HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_4096x2160p50_256_135,            4096,   968,    88,     128,    FALSE,  2160,   8,      10,     72,     FALSE,  594000,   50000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_256_135,    HDMI_AUDIO_SUPPORTED|HDMI_YUV420_SUPPORTED},
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,            4096,   88,     88,     128,    FALSE,  2160,   8,      10,     72,     FALSE,  594000,   60000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_256_135,    HDMI_AUDIO_SUPPORTED|HDMI_YUV420_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p24_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_256_135,    HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p25_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p30_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p50_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p60_64_27,              0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      HDMI_AUDIO_SUPPORTED},

  // VESA 
  {EDID_VIDEO_FORMAT_640x350p85_16_9,                 640,    32,     64,     96,     FALSE,  350,    32,     3,      60,     TRUE,   31500,    85080,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_640x400p85_16_10,                640,    32,     64,     96,     TRUE,   400,    1,      3,      41,     FALSE,  31500,    85080,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_10,      0},
  {EDID_VIDEO_FORMAT_720x400p85_16_9,                 720,    36,     72,     108,    TRUE,   400,    1,      3,      42,     FALSE,  35500,    85039,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_640x480p72_4_3,                  640,    24,     40,     128,    TRUE,   480,    9,      3,      28,     TRUE,   31500,    72809,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_4_3,        0},
  {EDID_VIDEO_FORMAT_640x480p75_4_3,                  640,    16,     64,     120,    TRUE,   480,    1,      3,      16,     TRUE,   31500,    75000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_640x480p85_4_3,                  640,    56,     56,     80,     TRUE,   480,    1,      3,      25,     TRUE,   36000,    85008,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_800x600p56_4_3,                  800,    24,     72,     128,    FALSE,  600,    1,      2,      22,     FALSE,  36000,    56250,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_800x600p60_4_3,                  800,    40,     128,    88,     FALSE,  600,    1,      4,      23,     FALSE,  40000,    60317,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_800x600p72_4_3,                  800,    56,     120,    64,     FALSE,  600,    37,     6,      23,     FALSE,  50000,    72188,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_800x600p75_4_3,                  800,    16,     180,    160,    FALSE,  600,    1,      3,      21,     FALSE,  49500,    75000,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_800x600p85_4_3,                  800,    32,     64,     152,    FALSE,  600,    1,      3,      27,     FALSE,  56250,    85061,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_800x600p120_4_3,                 800,    48,     32,     80,     FALSE,  600,    3,      4,      29,     TRUE,   73250,    119972,   TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_848x480p60_16_9,                 848,    16,     112,    112,    FALSE,  480,    6,      8,      23,     FALSE,  33750,    60000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1024x768p60_4_3,                 1024,   24,     136,    160,    TRUE,   768,    3,      6,      29,     TRUE,   65000,    60004,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1024x768p70_4_3,                 1024,   24,     136,    144,    TRUE,   768,    3,      6,      29,     TRUE,   65000,    70069,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1024x768p75_4_3,                 1024,   16,     96,     176,    FALSE,  768,    1,      3,      28,     FALSE,  75000,    75029,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1024x768p85_4_3,                 1024,   48,     96,     208,    FALSE,  768,    1,      3,      36,     FALSE,  78750,    84997,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1024x768p120_4_3,                1024,   48,     32,     80,     FALSE,  768,    3,      4,      38,     TRUE,   94500,    119989,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1152x768p60_3_2_RB,              1152,   8,      32,     40,     TRUE,   768,    8,      8,      6,      FALSE,  58338,    59939,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_3_2,        0},
  {EDID_VIDEO_FORMAT_1152x864p75_4_3,                 1152,   64,     128,    256,    FALSE,  864,    1,      3,      32,     FALSE,  108000,   75000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x768p60_5_3,                 1280,   64,     128,    192,    TRUE,   768,    3,      7,      20,     FALSE,  79500,    59870,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x768p60_5_3_RB,              1280,   48,     32,     80,     TRUE,   768,    3,      7,      12,     FALSE,  68250,    59995,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x768p75_5_3,                 1280,   80,     128,    208,    TRUE,   768,    3,      7,      27,     FALSE,  102250,   74893,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x768p85_5_3,                 1280,   80,     136,    216,    FALSE,  768,    3,      7,      31,     TRUE,   117500,   84837,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x768p120_5_3,                1280,   48,     32,     80,     FALSE,  768,    3,      7,      35,     TRUE,   140250,   119798,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x800p60_16_10,               1280,   72,     128,    200,    TRUE,   800,    3,      6,      22,     FALSE,  83500,    59810,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x800p60_16_10_RB,            1280,   48,     32,     80,     TRUE,   800,    3,      6,      14,     FALSE,  71000,    59910,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x800p75_16_10,               1280,   80,     128,    208,    TRUE,   800,    3,      6,      29,     FALSE,  106500,   74934,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x800p85_16_10,               1280,   80,     136,    216,    TRUE,   800,    3,      6,      34,     FALSE,  122500,   84880,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x800p120_16_10,              1280,   48,     32,     80,     FALSE,  800,    3,      6,      38,     TRUE,   146250,   119909,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x960p60_4_3,                 1280,   96,     112,    312,    FALSE,  960,    1,      3,      36,     TRUE,   108000,   60000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x960p85_4_3,                 1280,   64,     160,    224,    FALSE,  960,    1,      3,      47,     TRUE,   148500,   85002,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x960p120_4_3,                1280,   48,     32,     80,     FALSE,  960,    3,      4,      50,     TRUE,   175500,   119838,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x1024p60_5_4,                1280,   48,     112,    248,    FALSE,  1024,   1,      3,      38,     FALSE,  108000,   60020,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x1024p75_5_4,                1280,   16,     144,    248,    FALSE,  1024,   1,      3,      38,     FALSE,  135000,   75025,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x1024p85_5_4,                1280,   64,     160,    224,    FALSE,  1024,   1,      3,      44,     FALSE,  157500,   85024,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1280x1024p120_5_4,               1280,   48,     32,     80,     FALSE,  1024,   3,      7,      50,     TRUE,   187250,   119958,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1360x768p60_16_9,                1360,   64,     112,    256,    FALSE,  768,    3,      6,      18,     FALSE,  85500,    60015,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1360x768p120_16_9,               1360,   48,     32,     80,     FALSE,  768,    3,      5,      57,     TRUE,   148250,   119967,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1366x768p60_16_9,                1366,   70,     143,    213,    FALSE,  768,    3,      3,      24,     FALSE,  85500,    59790,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1366x768p60_16_9_RB,             1366,   14,     56,     64,     FALSE,  768,    1,      3,      28,     FALSE,  72000,    60000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1400x1050p60_4_3,                1400,   88,     144,    232,    TRUE,   1050,   3,      4,      32,     FALSE,  121750,   59978,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1400x1050p60_4_3_RB,             1400,   48,     32,     80,     TRUE,   1050,   3,      4,      23,     FALSE,  101000,   59948,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1400x1050p75_4_3,                1400,   104,    144,    248,    TRUE,   1050,   3,      4,      42,     FALSE,  156000,   74867,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1400x1050p85_4_3,                1400,   104,    152,    256,    TRUE,   1050,   3,      4,      48,     FALSE,  179500,   84960,    TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1400x1050p120_4_3,               1400,   48,     32,     80,     FALSE,  1050,   3,      4,      55,     TRUE,   208000,   119904,   TRUE,   EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1440x900p60_16_10,               1440,   80,     152,    232,    FALSE,  900,    3,      6,      25,     TRUE,   106500,   59887,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1440x900p60_16_10_RB,            1440,   48,     32,     80,     TRUE,   900,    3,      6,      17,     FALSE,  88750,    59901,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1440x900p75_16_10,               1440,   96,     152,    248,    TRUE,   900,    3,      6,      33,     FALSE,  136750,   74984,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1440x900p85_16_10,               1440,   144,    152,    256,    TRUE,   900,    3,      6,      39,     FALSE,  157000,   84842,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1440x900p120_16_10,              1440,   48,     32,     80,     FALSE,  900,    3,      6,      44,     TRUE,   182750,   119852,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1600x900p60_16_9,                1600,   24,     80,     96,     TRUE,   900,    1,      3,      96,     TRUE,   108000,   60000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1600x1200p60_3_3,                1600,   64,     192,    304,    FALSE,  1200,   1,      3,      46,     FALSE,  162000,   60000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1600x1200p65_4_3,                1600,   64,     192,    304,    FALSE,  1200,   1,      3,      46,     FALSE,  175500,   65000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1600x1200p70_4_3,                1600,   64,     192,    304,    FALSE,  1200,   1,      3,      46,     FALSE,  189000,   70000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1600x1200p75_4_3,                1600,   64,     192,    304,    FALSE,  1200,   1,      3,      46,     FALSE,  202500,   75000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1600x1200p85_4_3,                1600,   64,     192,    304,    FALSE,  1200,   1,      3,      46,     FALSE,  229500,   85000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1600x1200p120_4_3,               1600,   48,     32,     80,     FALSE,  1200,   3,      4,      64,     TRUE,   268250,   119917,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1680x1050p60_16_10,              1680,   104,    176,    280,    TRUE,   1050,   3,      6,      30,     FALSE,  146250,   59954,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1680x1050p60_16_10_RB,           1680,   48,     32,     80,     TRUE,   1050,   3,      6,      21,     FALSE,  119000,   59883,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1680x1050p75_16_10,              1680,   120,    176,    296,    TRUE,   1050,   3,      6,      40,     FALSE,  187000,   74892,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1680x1050p85_16_10,              1680,   128,    176,    304,    TRUE,   1050,   3,      6,      46,     FALSE,  214750,   84941,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1680x1050p120_16_10,             1680,   48,     32,     80,     FALSE,  1200,   3,      4,      53,     TRUE,   245500,   119986,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1792x1344p60_4_3,                1792,   128,    200,    328,    TRUE,   1344,   1,      3,      46,     FALSE,  204750,   60000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1792x1344p75_4_3,                1792,   96,     216,    352,    TRUE,   1344,   1,      3,      69,     FALSE,  261000,   74997,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1792x1344p120_4_3,               1792,   48,     32,     80,     FALSE,  1344,   3,      4,      72,     TRUE,   333250,   119974,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1856x1392p60_4_3,                1856,   96,     224,    352,    TRUE,   1392,   1,      3,      43,     FALSE,  218250,   59995,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1856x1392p75_4_3,                1856,   128,    224,    352,    TRUE,   1392,   1,      3,      104,    FALSE,  288000,   75000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1856x1392p120_4_3,               1856,   48,     32,     80,     FALSE,  1392,   3,      4,      75,     TRUE,   356500,   119970,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1920x1200p60_16_10,              1920,   136,    200,    336,    FALSE,  1200,   3,      6,      36,     TRUE,   193250,   59885,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1920x1200p60_16_10_RB,           1920,   48,     32,     80,     TRUE,   1200,   3,      6,      26,     FALSE,  154000,   59950,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1920x1200p75_16_10,              1920,   136,    208,    344,    TRUE,   1200,   3,      6,      46,     FALSE,  245250,   74930,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1920x1200p85_16_10,              1920,   144,    208,    352,    TRUE,   1200,   3,      6,      53,     FALSE,  281250,   84932,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1920x1200p120_16_10,             1920,   48,     32,     80,     FALSE,  1200,   3,      6,      62,     TRUE,   317000,   119909,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1920x1280p60_3_2_RB,             1920,   8,      32,     40,     TRUE,   1280,   23,     8,      6,      FALSE,  157882,   59940,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_3_2 ,       0},
  {EDID_VIDEO_FORMAT_1920x1440p60_4_3,                1920,   128,    208,    344,    FALSE,  1440,   1,      3,      56,     TRUE,   234000,   60000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1920x1440p75_4_3,                1920,   144,    224,    352,    TRUE,   1440,   1,      3,      56,     FALSE,  297000,   75000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_1920x1440p120_4_3,               1920,   48,     32,     80,     FALSE,  1440,   3,      4,      78,     TRUE,   380500,   119956,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_2048x1152p60_16_9_RB,            2048,   26,     80,     96,     FALSE,  1152,   1,      3,      44,     FALSE,  162000,   60000,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_2560x1440p60_16_9_RB,            2560,   48,     32,     80,     FALSE,  1440,   3,      5,      33,     FALSE,  241500,   59951,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_2560x1440p60_16_9,               2560,   192,    272,    464,    FALSE,  1440,   3,      5,      45,     FALSE,  312250,   59961,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},

  {EDID_VIDEO_FORMAT_2560x1600p60_16_10,              2560,   192,    280,    472,    TRUE,   1600,   3,      6,      49,     FALSE,  348500,   59987,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_10,      0},
  {EDID_VIDEO_FORMAT_2560x1600p60_16_10_RB,           2560,   48,     32,     80,     FALSE,  1600,   3,      6,      37,     TRUE,   268500,   59972,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_10,      0},
  {EDID_VIDEO_FORMAT_2560x1600p75_16_10,              2560,   208,    280,    488,    TRUE,   1600,   3,      6,      63,     FALSE,  443250,   74972,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_10,      0},
  {EDID_VIDEO_FORMAT_2560x1600p85_16_10,              2560,   208,    280,    488,    TRUE,   1600,   3,      6,      73,     FALSE,  505250,   84951,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_10,      0},
  {EDID_VIDEO_FORMAT_2560x1600p120_16_10,             2560,   48,     32,     80,     FALSE,  1600,   3,      6,      85,     TRUE,   552750,   119963,   FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_10,      0},
  {EDID_VIDEO_FORMAT_3440x1440p60_43_18_RB,           3440,   8,      32,     40,     TRUE ,  1440,   27,     8,      6,      TRUE,   312630,   59970,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_43_18,      0 },
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9_RB,            3840,   48,     32,     80,     FALSE,  2160,   3,      5,      54,     FALSE,  533280,   59997,    FALSE,  EDID_SPEC_HDMI2P0_DP1P3,      0,  EDID_VIDEO_ASPECT_RATIO_16_9,       0},
  {EDID_VIDEO_FORMAT_4096x2160p24_16_9,               0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,        0,        FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_64_27,      0},

  // Extended resolutions (HDMI 1.4)
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9_EX,            3840,   176,    88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  297000,   30000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p25_16_9_EX,            3840,   1056,   88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  297000,   25000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_3840x2160p24_16_9_EX,            3840,   1276,   88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  297000,   24000,    FALSE,  EDID_SPEC_ALL,                0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},
  {EDID_VIDEO_FORMAT_4096x2160p24_16_9_EX,            4096,   1020,   88,     296,    FALSE,  2160,   8,      10,     72,     FALSE,  297000,   24000,    FALSE,  EDID_SPEC_TYPE_UNSUPPORTED,   0,  EDID_VIDEO_ASPECT_RATIO_16_9,       HDMI_AUDIO_SUPPORTED},

};

// LUT is sorted from lowest Active H to highest Active H - ease searching
const EDID_VideoModePropertyType asEDIDDispModeLUT[] = 
{
  /* All 640 H Active */
  {EDID_VIDEO_FORMAT_640x350p85_16_9,               640,    350,    FALSE,  832,    192,    445,    95,     37861,    85080,    31500,    85080,    FALSE},
  {EDID_VIDEO_FORMAT_640x400p85_16_10,              640,    400,    FALSE,  832,    192,    445,    45,     37861,    85080,    31500,    85080,    FALSE},
  {EDID_VIDEO_FORMAT_640x480p60_4_3,                640,    480,    FALSE,  800,    160,    525,    45,     31469,    59940,    25175,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_640x480p72_4_3,                640,    480,    FALSE,  832,    192,    520,    40,     37861,    72809,    31500,    72809,    TRUE},
  {EDID_VIDEO_FORMAT_640x480p75_4_3,                640,    480,    FALSE,  840,    200,    500,    20,     37500,    75000,    31500,    75000,    TRUE},
  {EDID_VIDEO_FORMAT_640x480p85_4_3,                640,    480,    FALSE,  832,    192,    509,    29,     43270,    85008,    36000,    85008,    TRUE},
  /* All 720 H Active*/                                                              
  {EDID_VIDEO_FORMAT_720x400p85_16_9,               720,    400,    FALSE,  936,    216,    446,    46,     37928,    85039,    35500,    85039,    FALSE},
  {EDID_VIDEO_FORMAT_720x576p50_4_3,                720,    576,    FALSE,  864,    144,    625,    49,     31250,    50000,    27000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_720x480p60_4_3,                720,    480,    FALSE,  858,    138,    525,    45,     31469,    59940,    27000,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_720x480p60_4_3,                720,    480,    FALSE,  858,    138,    525,    45,     31500,    60000,    27027,    60000,    TRUE},
  {EDID_VIDEO_FORMAT_720x576p100_4_3,               720,    576,    FALSE,  864,    144,    625,    49,     62500,    100000,   54000,    100000,   TRUE},
  {EDID_VIDEO_FORMAT_720x480p120_4_3,               720,    480,    FALSE,  858,    138,    525,    45,     62937,    119880,   54000,    119880,   TRUE},
  {EDID_VIDEO_FORMAT_720x480p120_4_3,               720,    480,    FALSE,  858,    138,    525,    45,     63000,    120000,   54054,    120000,   TRUE},
  {EDID_VIDEO_FORMAT_720x576p200_4_3,               720,    576,    FALSE,  864,    144,    625,    49,     125000,   200000,   108000,   200000,   TRUE},
  {EDID_VIDEO_FORMAT_720x480p240_4_3,               720,    480,    FALSE,  858,    138,    525,    45,     125874,   239760,   108000,   239000,   TRUE},
  {EDID_VIDEO_FORMAT_720x480p240_4_3,               720,    480,    FALSE,  858,    138,    525,    45,     126000,   240000,   108108,   240000,   TRUE},
  /* All 800 H Active */                                                           
  {EDID_VIDEO_FORMAT_800x600p56_4_3,                800,    600,    FALSE,  1024,   224,    625,    25,     35157,    56250,    36000,    56250,    TRUE},
  {EDID_VIDEO_FORMAT_800x600p60_4_3,                800,    600,    FALSE,  1056,   256,    628,    28,     37879,    60317,    40000,    60317,    TRUE},
  {EDID_VIDEO_FORMAT_800x600p72_4_3,                800,    600,    FALSE,  1040,   240,    666,    66,     48077,    72188,    50000,    72188,    TRUE},
  {EDID_VIDEO_FORMAT_800x600p75_4_3,                800,    600,    FALSE,  1056,   256,    625,    25,     46875,    75000,    49500,    75000,    TRUE},
  {EDID_VIDEO_FORMAT_800x600p85_4_3,                800,    600,    FALSE,  1048,   248,    631,    31,     53674,    85061,    56250,    85061,    TRUE},
  {EDID_VIDEO_FORMAT_800x600p120_4_3,               800,    600,    FALSE,  960,    160,    636,    36,     76303,    119972,   73250,    119972,   TRUE},
  /* All 848 H Active */                                                             
  {EDID_VIDEO_FORMAT_848x480p60_16_9,               848,    480,    FALSE,  1088,   240,    517,    37,     31021,    60000,    33750,    60000,    FALSE},
  /* All 1024 H Active */                                                           
  {EDID_VIDEO_FORMAT_1024x768p60_4_3,               1024,   768,    FALSE,  1344,   320,    806,    38,     48363,    60004,    65000,    60004,    TRUE},
  {EDID_VIDEO_FORMAT_1024x768p70_4_3,               1024,   768,    FALSE,  1328,   304,    806,    38,     48946,    70069,    65000,    70069,    TRUE},
  {EDID_VIDEO_FORMAT_1024x768p75_4_3,               1024,   768,    FALSE,  1312,   288,    800,    32,     57165,    75029,    75000,    75029,    TRUE},
  {EDID_VIDEO_FORMAT_1024x768p85_4_3,               1024,   768,    FALSE,  1376,   352,    808,    40,     57232,    84997,    78750,    84997,    TRUE},
  {EDID_VIDEO_FORMAT_1024x768p120_4_3,              1024,   768,    FALSE,  1184,   160,    813,    45,     79815,    119989,   94500,    119989,   TRUE},
  /* All 1152 H Active */                                                          
  {EDID_VIDEO_FORMAT_1152x864p75_4_3,               1152,   864,    FALSE,  1600,   448,    900,    36,     67500,    75000,    108000,   75000,    TRUE},
  {EDID_VIDEO_FORMAT_1152x768p60_3_2_RB,            1152,   768,    FALSE,  1232,   80,     790,    22,     47352,    59939,    58338,    59939,    FALSE},
  /* All 1280 H Active */                                                           
  {EDID_VIDEO_FORMAT_1280x720p50_16_9,              1280,   720,    FALSE,  1980,   700,    750,    30,     37500,    50000,    74250,    50000,    FALSE},
  {EDID_VIDEO_FORMAT_1280x720p60_16_9,              1280,   720,    FALSE,  1650,   370,    750,    30,     44955,    59940,    74176,    59940,    FALSE},
  {EDID_VIDEO_FORMAT_1280x720p60_16_9,              1280,   720,    FALSE,  1650,   370,    750,    30,     45000,    60000,    74250,    60000,    FALSE},
  {EDID_VIDEO_FORMAT_1280x768p60_5_3_RB,            1280,   768,    FALSE,  1440,   160,    790,    22,     47396,    59995,    68250,    59995,    FALSE},
  {EDID_VIDEO_FORMAT_1280x768p60_5_3,               1280,   768,    FALSE,  1664,   384,    798,    30,     47776,    59870,    79500,    59870,    FALSE},
  {EDID_VIDEO_FORMAT_1280x768p75_5_3,               1280,   768,    FALSE,  1696,   416,    805,    37,     60289,    74893,    102250,   74893,    FALSE},
  {EDID_VIDEO_FORMAT_1280x768p85_5_3,               1280,   768,    FALSE,  1712,   432,    809,    41,     68634,    84837,    117500,   84837,    FALSE},
  {EDID_VIDEO_FORMAT_1280x768p120_5_3,              1280,   768,    FALSE,  1440,   160,    813,    45,     97396,    119798,   140250,   119798,   FALSE},
  {EDID_VIDEO_FORMAT_1280x800p60_16_10_RB,          1280,   800,    FALSE,  1440,   160,    823,    23,     49306,    59910,    71000,    59910,    FALSE},
  {EDID_VIDEO_FORMAT_1280x800p60_16_10,             1280,   800,    FALSE,  1680,   400,    831,    31,     49703,    59810,    83500,    59810,    FALSE},
  {EDID_VIDEO_FORMAT_1280x800p75_16_10,             1280,   800,    FALSE,  1696,   416,    838,    38,     62795,    74934,    106500,   74934,    FALSE},
  {EDID_VIDEO_FORMAT_1280x800p85_16_10,             1280,   800,    FALSE,  1712,   432,    843,    43,     71554,    84880,    122500,   84880,    FALSE},
  {EDID_VIDEO_FORMAT_1280x800p120_16_10,            1280,   800,    FALSE,  1440,   160,    847,    47,     101563,   119909,   146250,   119909,   FALSE},
  {EDID_VIDEO_FORMAT_1280x960p60_4_3,               1280,   960,    FALSE,  1800,   520,    1000,   40,     60000,    60000,    108000,   60000,    TRUE},
  {EDID_VIDEO_FORMAT_1280x960p85_4_3,               1280,   960,    FALSE,  1728,   448,    1011,   51,     85938,    85002,    148500,   85002,    TRUE},
  {EDID_VIDEO_FORMAT_1280x960p120_4_3,              1280,   960,    FALSE,  1440,   160,    1017,   57,     121875,   119838,   175500,   119838,   TRUE},
  {EDID_VIDEO_FORMAT_1280x1024p60_5_4,              1280,   1024,   FALSE,  1688,   408,    1066,   42,     63982,    60020,    108000,   60020,    FALSE},
  {EDID_VIDEO_FORMAT_1280x1024p75_5_4,              1280,   1024,   FALSE,  1688,   408,    1066,   42,     79977,    75025,    135000,   75025,    FALSE},
  {EDID_VIDEO_FORMAT_1280x1024p85_5_4,              1280,   1024,   FALSE,  1728,   448,    1072,   48,     91146,    85024,    157500,   85024,    FALSE},
  {EDID_VIDEO_FORMAT_1280x1024p120_5_4,             1280,   1024,   FALSE,  1440,   160,    1084,   60,     130035,   119958,   187250,   119958,   FALSE},
  /* All 1360 H Active */                                                            
  {EDID_VIDEO_FORMAT_1360x768p60_16_9,              1360,   768,    FALSE,  1792,   432,    795,    27,     47713,    60015,    85500,    60015,    FALSE},
  {EDID_VIDEO_FORMAT_1360x768p120_16_9,             1360,   768,    FALSE,  1520,   160,    813,    45,     97533,    119967,   148250,   119967,   FALSE},
  /* All 1366 H Active */                                                            
  {EDID_VIDEO_FORMAT_1366x768p60_16_9,              1366,   768,    FALSE,  1792,   426,    798,    30,     47713,    59790,    85500,    59790,    FALSE},
  {EDID_VIDEO_FORMAT_1366x768p60_16_9_RB,           1366,   768,    FALSE,  1500,   134,    800,    32,     48000,    60000,    72000,    60000,    FALSE},
  /* All 1400 H Active */                                                              
  {EDID_VIDEO_FORMAT_1400x1050p60_4_3_RB,           1400,   1050,   FALSE,  1560,   160,    1080,   30,     64744,    59948,    101000,   59948,    TRUE},
  {EDID_VIDEO_FORMAT_1400x1050p60_4_3,              1400,   1050,   FALSE,  1864,   464,    1089,   39,     65317,    59978,    121750,   59978,    TRUE},
  {EDID_VIDEO_FORMAT_1400x1050p75_4_3,              1400,   1050,   FALSE,  1896,   496,    1099,   49,     82279,    74867,    156000,   74867,    TRUE},
  {EDID_VIDEO_FORMAT_1400x1050p85_4_3,              1400,   1050,   FALSE,  1912,   512,    1105,   55,     93881,    84960,    179500,   84960,    TRUE},
  {EDID_VIDEO_FORMAT_1400x1050p120_4_3,             1400,   1050,   FALSE,  1560,   160,    1112,   62,     133334,   119904,   208000,   119904,   TRUE},
  /* All 1440 H Active */                                                            
  {EDID_VIDEO_FORMAT_1440x576i50_4_3,               1440,   576,    TRUE,   1728,   288,    625,    24,     15625,    50000,    27000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_720x288p50_4_3,                1440,   288,    FALSE,  1728,   288,    312,    24,     15625,    50080,    27000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_720x288p50_4_3,                1440,   288,    FALSE,  1728,   288,    313,    25,     15625,    49920,    27000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_720x288p50_4_3,                1440,   288,    FALSE,  1728,   288,    314,    26,     15625,    49761,    27000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_1440x576p50_4_3,               1440,   576,    FALSE,  1728,   288,    625,    49,     31250,    50000,    54000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_1440x480i60_4_3,               1440,   480,    TRUE,   1716,   276,    525,    22,     15734,    59940,    27000,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_1440x240p60_4_3,               1440,   240,    FALSE,  1716,   276,    262,    22,     15734,    60054,    27000,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_1440x240p60_4_3,               1440,   240,    FALSE,  1716,   276,    263,    23,     15734,    59826,    27000,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_1440x480p60_4_3,               1440,   480,    FALSE,  1716,   276,    525,    45,     31469,    59940,    54000,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_1440x480i60_4_3,               1440,   480,    TRUE,   1716,   276,    525,    22,     15750,    60000,    27027,    60000,    TRUE},
  {EDID_VIDEO_FORMAT_1440x240p60_4_3,               1440,   240,    FALSE,  1716,   276,    262,    22,     15750,    60115,    27027,    60000,    TRUE},
  {EDID_VIDEO_FORMAT_1440x240p60_4_3,               1440,   240,    FALSE,  1716,   276,    263,    23,     15750,    59886,    27027,    60000,    TRUE},
  {EDID_VIDEO_FORMAT_1440x480p60_4_3,               1440,   480,    FALSE,  1716,   276,    525,    45,     31500,    60000,    54054,    60000,    TRUE},
  {EDID_VIDEO_FORMAT_1440x576i100_4_3,              1440,   576,    TRUE,   1728,   288,    625,    24,     31250,    100000,   54000,    100000,   TRUE},
  {EDID_VIDEO_FORMAT_1440x480i120_4_3,              1440,   480,    TRUE,   1716,   276,    525,    22,     31469,    119880,   54000,    119880,   TRUE},
  {EDID_VIDEO_FORMAT_1440x480i120_4_3,              1440,   480,    TRUE,   1716,   276,    525,    22,     31500,    120000,   54054,    120000,   TRUE},
  {EDID_VIDEO_FORMAT_1440x576i200_4_3,              1440,   576,    TRUE,   1728,   288,    625,    24,     62500,    200000,   108000,   200000,   TRUE},
  {EDID_VIDEO_FORMAT_1440x480i240_4_3,              1440,   480,    TRUE,   1716,   276,    525,    22,     62937,    239760,   108000,   239000,   TRUE},
  {EDID_VIDEO_FORMAT_1440x480i240_4_3,              1440,   480,    TRUE,   1716,   276,    525,    22,     63000,    240000,   108108,   240000,   TRUE},
  {EDID_VIDEO_FORMAT_1440x900p60_16_10_RB,          1440,   900,    FALSE,  1600,   160,    926,    26,     55469,    59901,    88750,    59901,    FALSE},
  {EDID_VIDEO_FORMAT_1440x900p60_16_10,             1440,   900,    FALSE,  1904,   464,    934,    34,     55935,    59887,    106500,   59887,    FALSE},
  {EDID_VIDEO_FORMAT_1440x900p75_16_10,             1440,   900,    FALSE,  1936,   496,    942,    42,     70636,    74984,    136750,   74984,    FALSE},
  {EDID_VIDEO_FORMAT_1440x900p85_16_10,             1440,   900,    FALSE,  1952,   512,    948,    48,     80431,    84842,    157000,   84842,    FALSE},
  {EDID_VIDEO_FORMAT_1440x900p120_16_10,            1440,   900,    FALSE,  1600,   160,    953,    53,     114219,   119852,   182750,   119852,   FALSE},
  /* All 1600 H Active */                                                           
  {EDID_VIDEO_FORMAT_1600x900p60_16_9,              1600,   900,    FALSE,  1800,   200,    1000,   100,    60000,    60000,    108000,   60000,    FALSE},
  {EDID_VIDEO_FORMAT_1600x1200p60_3_3,              1600,   1200,   FALSE,  2160,   560,    1250,   50,     75000,    60000,    162000,   60000,    TRUE},
  {EDID_VIDEO_FORMAT_1600x1200p65_4_3,              1600,   1200,   FALSE,  2160,   560,    1250,   50,     81250,    65000,    175500,   65000,    TRUE},
  {EDID_VIDEO_FORMAT_1600x1200p70_4_3,              1600,   1200,   FALSE,  2160,   560,    1250,   50,     87500,    70000,    189000,   70000,    TRUE},
  {EDID_VIDEO_FORMAT_1600x1200p75_4_3,              1600,   1200,   FALSE,  2160,   560,    1250,   50,     93750,    75000,    202500,   75000,    TRUE},
  {EDID_VIDEO_FORMAT_1600x1200p85_4_3,              1600,   1200,   FALSE,  2160,   560,    1250,   50,     106250,   85000,    229500,   85000,    TRUE},
  {EDID_VIDEO_FORMAT_1600x1200p120_4_3,             1600,   1200,   FALSE,  1760,   160,    1271,   71,     152415,   119917,   268250,   119917,   TRUE},
  /* All 1680 H Active */                                                           
  {EDID_VIDEO_FORMAT_1680x1050p60_16_10_RB,         1680,   1050,   FALSE,  1840,   160,    1080,   30,     64674,    59883,    119000,   59883,    FALSE},
  {EDID_VIDEO_FORMAT_1680x1050p60_16_10,            1680,   1050,   FALSE,  2240,   560,    1089,   39,     65290,    59954,    146250,   59954,    FALSE},
  {EDID_VIDEO_FORMAT_1680x1050p75_16_10,            1680,   1050,   FALSE,  2272,   592,    1099,   49,     82307,    74892,    187000,   74892,    FALSE},
  {EDID_VIDEO_FORMAT_1680x1050p85_16_10,            1680,   1050,   FALSE,  2288,   608,    1105,   55,     93860,    84941,    214750,   84941,    FALSE},
  {EDID_VIDEO_FORMAT_1680x1050p120_16_10,           1680,   1050,   FALSE,  1840,   160,    1112,   62,     133424,   119986,   245500,   119986,   FALSE},
  /* All 1792 H Active */                                                          
  {EDID_VIDEO_FORMAT_1792x1344p60_4_3,              1792,   1344,   FALSE,  2448,   656,    1394,   50,     83640,    60000,    204750,   60000,    TRUE},
  {EDID_VIDEO_FORMAT_1792x1344p75_4_3,              1792,   1344,   FALSE,  2456,   664,    1417,   73,     106271,   74997,    261000,   74997,    TRUE},
  {EDID_VIDEO_FORMAT_1792x1344p120_4_3,             1792,   1344,   FALSE,  1952,   160,    1423,   79,     170723,   119974,   333250,   119974,   TRUE},
  {EDID_VIDEO_FORMAT_1856x1392p60_4_3,              1856,   1392,   FALSE,  2528,   672,    1439,   47,     86334,    59995,    218250,   59995,    TRUE},
  /* All 1856 H Active */                                                           
  {EDID_VIDEO_FORMAT_1856x1392p75_4_3,              1856,   1392,   FALSE,  2560,   704,    1500,   108,    112500,   75000,    288000,   75000,    TRUE},
  {EDID_VIDEO_FORMAT_1856x1392p120_4_3,             1856,   1392,   FALSE,  2016,   160,    1474,   82,     176836,   119970,   356500,   119970,   TRUE},
  /* All 1920 H Active */                                                            
  {EDID_VIDEO_FORMAT_1920x1080p24_16_9,             1920,   1080,   FALSE,  2750,   830,    1125,   45,     26973,    23976,    74176,    23976,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080p24_16_9,             1920,   1080,   FALSE,  2750,   830,    1125,   45,     27000,    24000,    74250,    24000,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080p25_16_9,             1920,   1080,   FALSE,  2640,   720,    1125,   45,     28125,    25000,    74250,    25000,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080p30_16_9,             1920,   1080,   FALSE,  2200,   280,    1125,   45,     33716,    29970,    74176,    29970,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080p30_16_9,             1920,   1080,   FALSE,  2200,   280,    1125,   45,     33750,    30000,    74250,    30000,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080p50_16_9,             1920,   1080,   FALSE,  2640,   720,    1125,   45,     56250,    50000,    148500,   50000,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080i50_16_9,             1920,   1080,   TRUE,   2640,   720,    1125,   22,     28125,    50000,    74250,    50000,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080i60_16_9,             1920,   1080,   TRUE,   2200,   280,    1125,   22,     33716,    59940,    74176,    59940,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080p60_16_9,             1920,   1080,   FALSE,  2200,   280,    1125,   45,     67433,    59940,    148352,   59940,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080i60_16_9,             1920,   1080,   TRUE,   2200,   280,    1125,   22,     33750,    60000,    74250,    60000,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080p60_16_9,             1920,   1080,   FALSE,  2200,   280,    1125,   45,     67500,    60000,    148500,   60000,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1080i100_16_9,            1920,   1080,   TRUE,   2640,   720,    1125,   22,     56250,    100000,   148500,   100000,   FALSE},
  {EDID_VIDEO_FORMAT_1920x1080i120_16_9,            1920,   1080,   TRUE,   2200,   280,    1125,   22,     67432,    119880,   148352,   119980,   FALSE},
  {EDID_VIDEO_FORMAT_1920x1080i120_16_9,            1920,   1080,   TRUE,   2200,   280,    1125,   22,     67500,    120000,   148500,   120000,   FALSE},
  {EDID_VIDEO_FORMAT_1920x1200p60_16_10_RB,         1920,   1200,   FALSE,  2080,   160,    1235,   35,     74038,    59950,    154000,   59950,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1200p60_16_10,            1920,   1200,   FALSE,  2592,   672,    1245,   45,     74556,    59885,    193250,   59885,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1200p75_16_10,            1920,   1200,   FALSE,  2608,   688,    1255,   55,     94038,    74930,    245250,   74930,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1200p85_16_10,            1920,   1200,   FALSE,  2624,   704,    1262,   62,     107184,   84932,    281250,   84932,    FALSE},
  {EDID_VIDEO_FORMAT_1920x1200p120_16_10,           1920,   1200,   FALSE,  2080,   160,    1271,   71,     152404,   119909,   317000,   119909,   FALSE},
  {EDID_VIDEO_FORMAT_1920x1280p60_3_2_RB,           1920,   1280,   FALSE,  2000,   80,     1317,   37,     78941,    599406,   157882,   599406,   FALSE},

  {EDID_VIDEO_FORMAT_1920x1440p60_4_3,              1920,   1440,   FALSE,  2600,   680,    1500,   60,     90000,    60000,    234000,   60000,    TRUE},
  {EDID_VIDEO_FORMAT_1920x1440p75_4_3,              1920,   1440,   FALSE,  2640,   720,    1500,   60,     112500,   75000,    297000,   75000,    TRUE},
  {EDID_VIDEO_FORMAT_1920x1440p120_4_3,             1920,   1440,   FALSE,  2080,   160,    1525,   85,     182933,   119956,   380500,   119956,   TRUE},
  /* All 2048 H Active */                                                           
  {EDID_VIDEO_FORMAT_2048x1152p60_16_9_RB,          2048,   1152,   FALSE,  2250,   202,    1200,   48,     72000,    60000,    162000,   60000,    FALSE},
  /* All 2560 H Active */                                                            
  {EDID_VIDEO_FORMAT_2560x1440p60_16_9_RB,          2560,   1440,   FALSE,  2720,   160,    1481,   41,     88787,    59951,    241500,   59951,    FALSE},
  {EDID_VIDEO_FORMAT_2560x1440p60_16_9,             2560,   1440,   FALSE,  3488,   928,    1493,   53,     89521,    59961,    312250,   59961,    FALSE},
  {EDID_VIDEO_FORMAT_2560x1600p60_16_10_RB,         2560,   1600,   FALSE,  2720,   160,    1646,   46,     98713,    59972,    268500,   59972,    FALSE},
  {EDID_VIDEO_FORMAT_2560x1600p60_16_10,            2560,   1600,   FALSE,  3504,   944,    1658,   58,     99458,    59987,    348500,   59987,    FALSE},
  {EDID_VIDEO_FORMAT_2560x1600p75_16_10,            2560,   1600,   FALSE,  3536,   976,    1672,   72,     125354,   74972,    443250,   74972,    FALSE},
  {EDID_VIDEO_FORMAT_2560x1600p85_16_10,            2560,   1600,   FALSE,  3536,   976,    1682,   82,     142888,   84951,    505250,   84951,    FALSE},
  {EDID_VIDEO_FORMAT_2560x1600p120_16_10,           2560,   1600,   FALSE,  2720,   160,    1694,   94,     203217,   119963,   552750,   119963,   FALSE},
  /* All 2880 H Active */                                                        
  {EDID_VIDEO_FORMAT_2880x576i50_4_3,               2880,   576,    TRUE,   3456,   576,    625,    24,     15625,    50000,    54000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_2880x288p50_4_3,               2880,   576,    FALSE,  3456,   576,    312,    24,     15625,    50080,    54000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_2880x288p50_4_3,               2880,   576,    FALSE,  3456,   576,    313,    25,     15625,    49920,    54000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_2880x288p50_4_3,               2880,   576,    FALSE,  3456,   576,    314,    26,     15625,    49761,    54000,    50000,    TRUE},
  {EDID_VIDEO_FORMAT_2880x576p50_4_3,               2880,   576,    FALSE,  3456,   576,    625,    49,     31250,    50000,    108000,   50000,    TRUE},
  {EDID_VIDEO_FORMAT_2880x480i60_4_3,               2880,   480,    TRUE,   3432,   552,    525,    22,     15734,    59940,    54000,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_2880x240p60_4_3,               2880,   480,    FALSE,  3432,   552,    262,    22,     15734,    60054,    54000,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_2880x240p60_4_3,               2880,   480,    FALSE,  3432,   552,    263,    23,     15734,    59940,    54000,    59940,    TRUE},
  {EDID_VIDEO_FORMAT_2880x480p60_4_3,               2880,   480,    FALSE,  3432,   552,    525,    45,     31469,    59940,    108000,   59940,    TRUE},
  {EDID_VIDEO_FORMAT_2880x480i60_4_3,               2880,   480,    TRUE,   3432,   552,    525,    22,     15750,    60000,    54054,    60000,    TRUE},
  {EDID_VIDEO_FORMAT_2880x240p60_4_3,               2880,   240,    FALSE,  3432,   552,    262,    22,     15750,    60115,    54054,    60000,    TRUE},
  {EDID_VIDEO_FORMAT_2880x240p60_4_3,               2880,   240,    FALSE,  3432,   552,    262,    23,     15750,    59886,    54054,    60000,    TRUE},
  {EDID_VIDEO_FORMAT_2880x480p60_4_3,               2880,   480,    FALSE,  3432,   552,    525,    45,     31500,    60000,    108108,   60000,    TRUE},

  /* All 3440 H Active */
  {EDID_VIDEO_FORMAT_3440x1440p60_43_18_RB,         3440,   1440,   FALSE,  3520,   80 ,    1481,   41,     88815,    59970,     312630,  59970,    FALSE},

  /* All 3840 H Active */                                                  
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9,             3840,   2160,   FALSE,  4400,   560,   2250,   90,      67000,    30000,    297000,   30000,    FALSE},
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9,             3840,   2160,   FALSE,  4000,   160,   2222,   62,     133313,    60000,    533250,   60000,    FALSE},

  /* All 4096 H Active */                                                  
  {EDID_VIDEO_FORMAT_4096x2160p24_256_135,          4096,   2160,   FALSE,  5500,   1404,   2250,   90,     54000,    24000,    297000,   24000,    FALSE},
  {EDID_VIDEO_FORMAT_4096x2160p25_256_135,          4096,   2160,   FALSE,  5280,   1184,   2250,   90,     56250,    25000,    297000,   25000,    FALSE},
  {EDID_VIDEO_FORMAT_4096x2160p30_256_135,          4096,   2160,   FALSE,  4400,   304,    2250,   90,     67500,    30000,    297000,   30000,    FALSE},
  {EDID_VIDEO_FORMAT_4096x2160p50_256_135,          4096,   2160,   FALSE,  5280,   1184,   2250,   90,     112500,   50000,    594000,   60000,    FALSE},
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,          4096,   2160,   FALSE,  4176,   80,     2222,   62,     133320,   60000,    556744,   60000,    FALSE},
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,          4096,   2160,   FALSE,  4400,   304,    2250,   90,     135000,   60000,    594000,   60000,    FALSE},
};

// Mapping of HDMI_EDID_VendorBlockExtndedResType to HDMI_VideoFormatType
EDID_VideoFormatType aEDIDExtResToVideoFormatMap[EDID_VENDOR_BLOCK_EXTENDED_RES_MAX] = 
{
  0x0,
  EDID_VIDEO_FORMAT_3840x2160p30_16_9_EX,
  EDID_VIDEO_FORMAT_3840x2160p25_16_9_EX,
  EDID_VIDEO_FORMAT_3840x2160p24_16_9_EX,
  EDID_VIDEO_FORMAT_4096x2160p24_16_9_EX
};

/* -----------------------------------------------------------------------
** Forward definitions 
** ----------------------------------------------------------------------- */

/* EDID_DataBlockOffsetSetups
 */
static void EDID_DataBlockOffsetSetup(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uNumOfCEABlocks);

/* EDID_IsValidCheckSum
 */
static bool32 EDID_IsValidCheckSum(uint8* pEdid);

/* EDID_GetNumOfCEAExtBlocks
 */
static uint32 EDID_GetNumOfCEAExtBlocks(EDID_ParserContextType *pEdidSharedData);

/* EDID_IsDispModeSupportAudio
 */
static bool32 EDID_IsDispModeSupportAudio(EDID_ParserContextType *pEdidSharedData);

/* EDID_Parser_SetDefaultAudioMode
 */
static void EDID_Parser_SetDefaultAudioMode(EDID_AudioModeInfoType* pAudioModeInfo);

/* EDID_Parser_GetAudioModeInfo
 */
static EDID_Status EDID_Parser_GetAudioModeInfo(EDID_ParserContextType *pEdidSharedData, EDID_AudioModeInfoType* pAudioModeInfo, uint32 uFlags);

/* EDID_Parser_GetSinkCapability
 */
static void EDID_Parser_GetSinkCapability(EDID_ParserContextType *pEdidSharedData);

/* EDID_Parser_DetailTiming
 */
static EDID_Status EDID_Parser_DetailTiming(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList);

/* EDID_Parser_DisplayDescriptor
 */
static void EDID_Parser_DisplayDescriptor(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uDescOffset, EDID_DispModeListType* pDispModeList);

/* EDID_Parser_EstablishTimingIII
 */
void EDID_Parser_EstablishTimingIII(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uEstablishTimingOffset, EDID_DispModeListType* pDispModeList);

/* EDID_InsertShortVideoDesc
 */
static void EDID_InsertShortVideoDesc(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList, uint32 uNumOfDispMode, uint32 uBufferIndex, EDID_VideoFormatYUV420Support_Type eYUVSupport);

/* EDID_Parser_Detail_Desc
 */
static EDID_Status EDID_Parser_Detail_Desc(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_VideoFormatType* pDispMode);

/* EDID_Parser_InsertDispFormat
 */
static void EDID_Parser_InsertDispFormat(EDID_ParserContextType *pEdidSharedData, EDID_DispModeListType* pDispModeList, EDID_VideoFormatType eVideoFormat, EDID_VideoFormatYUV420Support_Type eYUV420Support);

/* EDID_RejectFilteredModes
 */
static bool32 EDID_RejectFilteredModes(EDID_ParserContextType *pEdidSharedData, EDID_VideoFormatType eVideoFormat, EDID_VideoFormatYUV420Support_Type eYUV420Support);

/* EDID_Parser_StandardTimings
 */
static void EDID_Parser_StandardTimings(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uStandardTimingStartOffset, uint32 uStandardTimingEndOffset, EDID_DispModeListType* pDispModeList );

/* EDID_Parser_EstablishedTimings
 */
static void EDID_Parser_EstablishedTimings(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList);

/* EDID_ModeSupported
 */
static bool32 EDID_ModeSupported(EDID_ParserContextType *pEdidSharedData, EDID_VideoFormatType eVideoMode);

/* EDID_Inject_CEA_Mode
 */
static void EDID_Inject_CEA_Mode(EDID_ParserContextType *pEdidSharedData, EDID_DispModeListType* pDispModeList);

/* EDID_Parser_GetDispMode
 */
static EDID_Status EDID_Parser_GetDispMode(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList, uint32 uNumOfCEABlocks, bool32 bShortDesc);

/* EDID_AppendExtendedResolutionDispModes
 */
static void EDID_AppendExtendedResolutionDispModes(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uStartingOffset, EDID_DispModeListType* pDispModeList, uint32 uCEABlockIndex);

/* EDID_Parser_VendorBlockInfo
 */
static EDID_Status EDID_Parser_VendorBlockInfo(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList, uint32 uNumOfCEABlocks);


/* -----------------------------------------------------------------------
**  Public Functions
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
EDID_ErrorType EDID_OpenParser(EDID_HandleType *phHandle)
{
  EDID_ErrorType eStatus = EDID_STATUS_OK;

  if (NULL == phHandle)
  {
    eStatus = EDID_STATUS_BAD_PARAM;
  }
  else
  {
    uint32 uI = 0;

    // Search through the context list to check if there are any available contexts
    for (uI = 0; uI < EDID_MAX_CTX_NUM; uI++)
    {
      EDID_ParserContextType *pEdidSharedData = EDID_GETCTX(uI);

      if (NULL != pEdidSharedData)
      {
        DISP_OSAL_MemZero(pEdidSharedData, sizeof(EDID_ParserContextType));
        *phHandle = (EDID_HandleType)pEdidSharedData;
        break;
      }
    }

    // Return error if there are no available contexts
    if (uI == EDID_MAX_CTX_NUM)
    {
      eStatus = EDID_STATUS_NO_RESOURCES;
    }
  }

  return eStatus;
}

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
EDID_ErrorType EDID_CloseParser(EDID_HandleType hHandle)
{
  EDID_ErrorType eStatus = EDID_STATUS_OK;
  
  if (NULL == hHandle)
  {
    eStatus = EDID_STATUS_BAD_PARAM;
  }
  else
  {
    EDID_ParserContextType *pEdidSharedData = (EDID_ParserContextType*)hHandle;

    // Clear any context info
    DISP_OSAL_MemZero(pEdidSharedData, sizeof(EDID_ParserContextType));
  }

  return eStatus;
}

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
EDID_ErrorType EDID_GetModeList(EDID_HandleType hHandle, EDID_ParserInfoType *pParserConfig, EDID_DispModeListType *pModeList)
{
  EDID_ErrorType eStatus = EDID_STATUS_OK;

  if ((NULL == hHandle) || 
      (NULL == pParserConfig) || 
      (NULL == pParserConfig->pEDIDBuffer) ||
      (EDID_MAX_BUF_SIZE <  pParserConfig->uEDIDDataLength) ||
      (NULL == pModeList))
  {
    eStatus = EDID_STATUS_BAD_PARAM;
  }
  else
  {

    EDID_ParserContextType *pEdidSharedData = (EDID_ParserContextType*)hHandle;
    uint32                  uNumOfCEABlocks = 0;
    uint32                  uI              = 0;

    // Initialize EDID Cache
    DISP_OSAL_MemZero(pEdidSharedData->auEDIDCache, (sizeof(uint8) * EDID_MAX_BUF_SIZE));
    pEdidSharedData->uEDIDDataLength  = 0;
    pEdidSharedData->bEDIDParserComplete = FALSE;
  
    // Initialize Supported Modes list
    DISP_OSAL_MemZero(&pEdidSharedData->sDispModeList, sizeof(EDID_DispModeListType));

    // Reset the list of compatible audio modes
    pEdidSharedData->uNumCompatibleAudioModes = 0;

    // Copy over EDID
    DISP_OSAL_MemCpyEx(pEdidSharedData->auEDIDCache, EDID_MAX_BUF_SIZE, pParserConfig->pEDIDBuffer, pParserConfig->uEDIDDataLength);
    pEdidSharedData->uEDIDDataLength = pParserConfig->uEDIDDataLength;


    // Copy of EDID specification
    pEdidSharedData->eEDIDSpecType       = pParserConfig->eEDIDSpecType;

    // Copy over resolution limitations
    pEdidSharedData->uMaxModeWidth       = pParserConfig->uMaxModeWidth;
    pEdidSharedData->uMaxModeHeight      = pParserConfig->uMaxModeHeight;
    pEdidSharedData->uMaxModeRefreshRate = pParserConfig->uMaxModeRefreshRate;
    pEdidSharedData->uMinModeWidth       = pParserConfig->uMinModeWidth;
    pEdidSharedData->uMinModeHeight      = pParserConfig->uMinModeHeight;
    pEdidSharedData->uMinModeRefreshRate = pParserConfig->uMinModeRefreshRate;
    pEdidSharedData->uMaxLinkRate        = pParserConfig->uMaxLinkRate;
    pEdidSharedData->uNumberOfLanes      = pParserConfig->uNumberOfLanes;

    for (uI=0; uI<pParserConfig->uInjectedModeNum; uI++)
    {
      pEdidSharedData->uInjectedModeList[uI] = pParserConfig->uInjectedModeList[uI];
    }
    pEdidSharedData->uInjectedModeNum = pParserConfig->uInjectedModeNum;

    // Validate EDID
    if ((1     != pEdidSharedData->auEDIDCache[EDID_VERSION_OFFSET]) || 
        (FALSE == EDID_IsValidCheckSum(pEdidSharedData->auEDIDCache)))
    {
      eStatus = EDID_STATUS_INVALID_STATE;
    }
    else
    {
      uNumOfCEABlocks = EDID_GetNumOfCEAExtBlocks(pEdidSharedData);

      // Initialize the data block offsets
      EDID_DataBlockOffsetSetup(pEdidSharedData, pEdidSharedData->auEDIDCache, uNumOfCEABlocks);

      // Parse all the supported 2D display modes */
      if (EDID_STATUS_SUCCESS  != (eStatus = EDID_Parser_GetDispMode(pEdidSharedData, pEdidSharedData->auEDIDCache, &pEdidSharedData->sDispModeList, uNumOfCEABlocks, FALSE)))
      {
        // Do nothing
      }
      else if (EDID_STATUS_SUCCESS != (eStatus = EDID_Parser_VendorBlockInfo(pEdidSharedData, pEdidSharedData->auEDIDCache, &pEdidSharedData->sDispModeList, uNumOfCEABlocks)))
      {
        // Do nothing
      }
      else
      {
        // Edid parsing is complete 
        pEdidSharedData->bEDIDParserComplete = TRUE;
        // Copy over the list of modes
        DISP_OSAL_MemCpyEx(pModeList, sizeof(EDID_DispModeListType), &pEdidSharedData->sDispModeList, sizeof(EDID_DispModeListType));
      }
    }
  }

  return eStatus;
}

//-------------------------------------------------------------------------------------------------
//  EDID_DataBlockOffsetSetup
//
//  @brief
//      Initializes the various data block offsets.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect.
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN] uNumOfCEABlocks
//          Number of CEA blocks in this EDID.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_DataBlockOffsetSetup(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uNumOfCEABlocks)
{
  uint32 uWorkingOffset   = 0;
  uint32 uStartingOffset  = 0;
  uint32 uDataTagCode     = 0;
  uint32 uNumOfBytes      = 0;
  uint32 uIndex           = 0;
  uint32 uI               = 0;
  uint32 uNumOfBlocks     = (uNumOfCEABlocks > EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS)? EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS : uNumOfCEABlocks;
  uint32 uVideoBlockCount = 0;

  /* Initialize global variable for all the data block offsets to be 0 */
  DISP_OSAL_MemZero(&pEdidSharedData->sDataBlockDesc, sizeof(EDID_DataBlockType));
  
  /* Check if block 2 is a block map or not, all relative working offset needs to shift by 1 block size if block 2 is a block map */
  if(EDID_EXTENSION_BLOCK_MAP_TAG_CODE == pDataBuf[EDID_CEA_EXTENSION_VERSION_OFFSET - 1])
  {
    uStartingOffset += EDID_BLOCK_SIZE;
    if(uNumOfBlocks)
      uNumOfBlocks   -= 1;
  }

  /* First data block offset always start from block 2 or greater */
  uStartingOffset += EDID_BLOCK_SIZE;

  for(uI = 0; uI < uNumOfBlocks; uI++)
  {
    /* Beginning of each extension block */
    uStartingOffset += (uI * EDID_BLOCK_SIZE);

    /* Starting offset of the detail timing descriptors, it also indicates the end of all data block within a CEA extension block */
    pEdidSharedData->sDataBlockDesc.sDetailTimingBlockDesc[uI].uDetailTimingBlockOffset = pDataBuf[uStartingOffset + EDID_DETAIL_TIMING_RELATIVE_OFFSET] + uStartingOffset;

    /* Offset to the first tag code in an CEA block */
    uWorkingOffset = uStartingOffset + EDID_DATA_BLOCK_RELATIVE_OFFSET;

    /* First check if there is any data block */
    if(uWorkingOffset != pEdidSharedData->sDataBlockDesc.sDetailTimingBlockDesc[uI].uDetailTimingBlockOffset)
    {
      /* Reset the video block count for each CEA extension block */
      uVideoBlockCount = 0;

      for(uIndex = 0; uIndex < EDID_MAX_NUM_OF_TAG_CODES; uIndex++)
      {
        /* Extract the tag code */
        uDataTagCode = (pDataBuf[uWorkingOffset] & EDID_DATA_BLOCK_TAG_MASK) >> EDID_DATA_BLOCK_TAG_SHIFT;
        /* Find out the length in the data block */
        uNumOfBytes = pDataBuf[uWorkingOffset] & EDID_DATA_BLOCK_LENGTH_MASK;

        switch(uDataTagCode)
        {
          case EDID_VIDEO_DATA_BLOCK_TAG_CODE:
          {
            /* Video data block offset */
            pEdidSharedData->sDataBlockDesc.sVideoDataBlockDesc[uI].uVideoDataBlockOffset[uVideoBlockCount] = uWorkingOffset;
            pEdidSharedData->sDataBlockDesc.sVideoDataBlockDesc[uI].uDataBlockLength[uVideoBlockCount]      = uNumOfBytes;
            uVideoBlockCount++;

            break;
          }
          case EDID_AUDIO_DATA_BLOCK_TAG_CODE:
          {
            /* Don't overwrite if the Audio data block offset has already been determined */
            /* In each CEA block there could be multiple Audio data blocks */
            /* The EDID parser only parses the first valid audio data block */
            if ((uI < EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS) && 
                (0 == pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uI].uAudioDataBlockOffset) && 
                (0 == pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uI].uDataBlockLength))
            {
              EDID_Status             eLocalStatus    = EDID_STATUS_OK;
              uint32                 uAudioModeIndex = 0;
              EDID_AudioModeInfoType sAudioModeInfo;
              
              /* Audio data block offset is relevant to the video data block size */
              /* 1. Jump through each block and look at the audio modes in that block
               * 2. Keep track of how many modes are comatible with our MSM.  This data will be used to decide
               *    whether we need to inject a default audio mode during mode enumeration
               */
              pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uI].uAudioDataBlockOffset    = uWorkingOffset;
              pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uI].uDataBlockLength         = uNumOfBytes;

              do
              {
                DISP_OSAL_MemZero(&sAudioModeInfo, sizeof(EDID_AudioModeInfoType));
                sAudioModeInfo.uAudioModeIndex = uAudioModeIndex;
              
                if (EDID_STATUS_SUCCESS == (eLocalStatus = EDID_Parser_GetAudioModeInfo(pEdidSharedData, &sAudioModeInfo, 0x0)))
                {
                  /* Found at least one mode in Audio data block that supports LPCM */
                  if (EDID_AUDIO_FORMAT_LPCM == sAudioModeInfo.eAudioFormat)
                  {
                    // PCM is the only mode audio driver supports, keep track of the number of modes we discover
                    pEdidSharedData->uNumCompatibleAudioModes++;
                  }
                  
                  // Loop to the next audio mode index
                  uAudioModeIndex++;
                }
              } while (EDID_STATUS_SUCCESS == eLocalStatus);

              /* If we did not find a compaible audio mode, reset the block so we keep searching the next block */
              if (0 == pEdidSharedData->uNumCompatibleAudioModes)
              {
                pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uI].uAudioDataBlockOffset    = 0;
                pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uI].uDataBlockLength         = 0;
              }              
            }            
            break;
          }
          case EDID_SPEAKER_ALLOC_DATA_BLOCK_TAG_CODE:
          {
            /* Speaker allocation data block offset is relevant to the video data block size */
            pEdidSharedData->sDataBlockDesc.sSpeakerDataBlockDesc[uI].uSpeakerAllocBlockOffset = uWorkingOffset;
            pEdidSharedData->sDataBlockDesc.sSpeakerDataBlockDesc[uI].uDataBlockLength         = uNumOfBytes;
            break;
          }
          case EDID_VENDOR_SPECIFIC_DATA_BLOCK_TAG_CODE:
          {
            /* Speaker allocation data block offset is relevant to the video data block size */
            pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uI].uVendorSpecificBlockOffset = uWorkingOffset;
            pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uI].uDataBlockLength           = uNumOfBytes;
            /* Extension field exists */
            if(uNumOfBytes > EDID_VENDOR_SPECIFIC_BLOCK_IEEE_ID_LENGTH + EDID_VENDOR_SPECIFIC_PHYSICAL_ADDR_LENGTH)
            {
              pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uI].uVendorSpecificBlockExtOffset = uWorkingOffset + EDID_VENDOR_SPECIFIC_BLOCK_IEEE_ID_LENGTH + 
                                                                                               EDID_VENDOR_SPECIFIC_PHYSICAL_ADDR_LENGTH + 1;
            }
            break;
          }
          case EDID_EXTENDED_DATA_BLOCK_TAG_CODE:
          {
            /* Move to next byte to find out the extended tag code */
            switch(pDataBuf[uWorkingOffset + 1])
            {
              case EDID_EXTENDED_COLORIMETRY_TAG_CODE:
              {
                pEdidSharedData->sDataBlockDesc.sColorimetryDataBlockDesc[uI].uColorimetryDataBlockOffset = uWorkingOffset;
                pEdidSharedData->sDataBlockDesc.sColorimetryDataBlockDesc[uI].uDataBlockLength            = uNumOfBytes;
                break;
              }
              case EDID_EXTENDED_VENDOR_SPECIFIC_VIDEO_TAG_CODE:
              {
                /* Video data block offset */
                pEdidSharedData->sDataBlockDesc.sExtVideoDataBlockDesc[uI].uVideoDataBlockOffset = uWorkingOffset;
                pEdidSharedData->sDataBlockDesc.sExtVideoDataBlockDesc[uI].uDataBlockLength      = uNumOfBytes;
                break;
              }
              case EDID_EXTENDED_VENDOR_SPECIFIC_AUDIO_TAG_CODE:
              {
                /* Audio data block offset is relevant to the video data block size */
                pEdidSharedData->sDataBlockDesc.sExtAudioDataBlockDesc[uI].uAudioDataBlockOffset = uWorkingOffset;
                pEdidSharedData->sDataBlockDesc.sExtAudioDataBlockDesc[uI].uDataBlockLength      = uNumOfBytes;
                break;
              }
              case EDID_EXTENDED_VIDEO_CAP_TAG_CODE:
              {
                pEdidSharedData->sDataBlockDesc.sVideoCapDataBlockDesc[uI].uVideoCapDataBlockOffset = uWorkingOffset;
                pEdidSharedData->sDataBlockDesc.sVideoCapDataBlockDesc[uI].uDataBlockLength         = uNumOfBytes;
                break;
              }
              case EDID_EXTENDED_HDMI_VIDEO_DATA_BLOCK_TAG_CODE:
              {
                pEdidSharedData->sDataBlockDesc.sHDMIVideoDataBlockDesc[uI].uHDMIVideoDataBlockOffset = uWorkingOffset;
                pEdidSharedData->sDataBlockDesc.sHDMIVideoDataBlockDesc[uI].uDataBlockLength = uNumOfBytes;
                break;
              }
              case EDID_EXTENDED_YUV420_VIDEO_BLOCK_TAG_CODE:
              {
                pEdidSharedData->sDataBlockDesc.sYUV420DataBlockDesc[uI].uYUV420DataBlockOffset = uWorkingOffset;
                pEdidSharedData->sDataBlockDesc.sYUV420DataBlockDesc[uI].uDataBlockLength = uNumOfBytes;
                break;
              }
              case EDID_EXTENDED_YUV420_CAPABILITY_TAG_CODE:
              {
                pEdidSharedData->sDataBlockDesc.sYUV420CapabilityMapBlockDesc[uI].uYUV420CapabilityMapOffset = uWorkingOffset;
                pEdidSharedData->sDataBlockDesc.sYUV420CapabilityMapBlockDesc[uI].uDataBlockLength = uNumOfBytes;
                break;
              }

              default:
                uIndex = EDID_MAX_NUM_OF_TAG_CODES; /* Exit For Loop */
                break;
            }
            break;
          }
          default:
          {
            uIndex = EDID_MAX_NUM_OF_TAG_CODES; /* Exit For Loop */
            break;
          }
        }
        uWorkingOffset += uNumOfBytes;
        /* Next byte will be the next tag code */
        uWorkingOffset++;

        /* Check whether we completed iterating all data blocks within this CEA block */
        if(uWorkingOffset == pEdidSharedData->sDataBlockDesc.sDetailTimingBlockDesc[uI].uDetailTimingBlockOffset)
        {
          /* Move to the next CEA extension block */
          break;
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_IsValidCheckSum
//
//  @brief
//      Calculates the 1 byte sum of all 128 bytes in the EDID Block passed 
//      and returns TRUE if it is equal to 0.
//
//  @params
//      [IN] pEdid
//          Buffer that contains EDID data.
//
//  @return
//      TRUE if checksum is 0.
//-------------------------------------------------------------------------------------------------
//
static bool32 EDID_IsValidCheckSum(uint8* pEdid)
{
  uint8    uCheckSum = 0;
  uint32  *pDataBuf  = (uint32 *)pEdid; 
  uint32   uI;

  // Calculate the checksum of the BLOCK 0 EDID 4 bytes at a time
  for (uI = 0; uI < (EDID_BLOCK_SIZE >> 2); uI++)
  {
    uCheckSum += (uint8)(*pDataBuf & 0xff);
    uCheckSum += (uint8)((*pDataBuf & 0xff00) >> 8);
    uCheckSum += (uint8)((*pDataBuf & 0xff0000) >> 16);
    uCheckSum += (uint8)((*pDataBuf & 0xff000000) >> 24);
    pDataBuf++;
  }
  return (uCheckSum == 0)? TRUE:FALSE;
}

//-------------------------------------------------------------------------------------------------
//  EDID_GetNumOfCEAExtBlocks
//
//  @brief
//      Parses the edid to find the number of CEA extension blocks.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect.
//
//  @return
//      Number of CEA blocks.
//-------------------------------------------------------------------------------------------------
//
static uint32 EDID_GetNumOfCEAExtBlocks(EDID_ParserContextType *pEdidSharedData)
{
  uint32               uNumOfCEABlocks     = 0;
  
  uNumOfCEABlocks  = pEdidSharedData->auEDIDCache[EDID_CEA_EXTENSION_FLAG_OFFSET];

  // Make sure that the number of CEA blocks number is not larger than maximum number of CEA blocks.
  if (uNumOfCEABlocks > EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS)
  {
    DISP_OSAL_LOG_WARN("Number of CEA extension blocks is %d, greater than HDMI_EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS", uNumOfCEABlocks);
    uNumOfCEABlocks = EDID_MAX_NUM_OF_CEA_EXTENSION_BLOCKS;
  }

  return uNumOfCEABlocks;
}

//-------------------------------------------------------------------------------------------------
//  EDID_IsDispModeSupportAudio
//
//  @brief
//      Checks whether the current active display mode supports audio.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect.
//
//  @return
//      TRUE if audio is supported.
//-------------------------------------------------------------------------------------------------
//
static bool32 EDID_IsDispModeSupportAudio(EDID_ParserContextType *pEdidSharedData)
{
  bool32                  bResult                 = FALSE;
  uint32 uModeIndex                               = 0;

    /* Get the current active mode index */
    uModeIndex = pEdidSharedData->uActiveModeIndex;
    
    if (uModeIndex < EDID_VIDEO_FORMAT_MAX)
    {
      EDID_VideoFormatType eVideoFormat = pEdidSharedData->sDispModeList.aDispModeAttr[uModeIndex].eVideoFormat;

      if ((eVideoFormat < EDID_VIDEO_FORMAT_MAX) && 
          (HDMI_AUDIO_SUPPORTED & gEDIDSupportedVideoModeLUT[eVideoFormat].uModeFlags))
      {
        bResult = !pEdidSharedData->sDispModeList.aDispModeAttr[uModeIndex].bVideoOnly;
      }
    }

  return bResult;
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_SetDefaultAudioMode
//
//  @brief
//      Configure the audio mode for the default audio mode required by the HDMI specification:
//
//   An HDMI Sink that is capable of accepting any audio format is required to accept two channel
//  (IEC 60958-formatted) L-PCM audio at sample rates of 32kHz, 44.1kHz, and 48kHz
//
//  @params
//      [IN] pAudioModeInfo
//          Pointer to a structure containing the properties of a short audio descriptor.
//  @return
//      none.
//-------------------------------------------------------------------------------------------------
//
static void EDID_Parser_SetDefaultAudioMode(EDID_AudioModeInfoType* pAudioModeInfo)
{
  if (pAudioModeInfo)
  {
    pAudioModeInfo->eAudioFormat                             = EDID_AUDIO_FORMAT_LPCM;
    pAudioModeInfo->uAudioChannelMask                        = EDID_AUDIO_CHANNEL_2;
    pAudioModeInfo->uAudioSampleRateMask                     = EDID_AUDIO_SAMPLE_RATE_32KHZ   |
                                                               EDID_AUDIO_SAMPLE_RATE_44_1KHZ | 
                                                               EDID_AUDIO_SAMPLE_RATE_48KHZ;
    pAudioModeInfo->audioStreamInfo.uAudioSampleBitDepthMask = EDID_AUDIO_BIT_DEPTH_16_BIT;
    pAudioModeInfo->uAudioSpeakerLocationMask                = EDID_AUDIO_SPEAKER_LOCATION_FL |
                                                               EDID_AUDIO_SPEAKER_LOCATION_FR;
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_GetAudioModeInfo
//
//  @brief
//      Parses the EDID structure and find out the audio capabilities 
//      from each audio short descriptors.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pAudioModeInfo
//          Pointer to a structure containing the properties of a short audio descriptor.
//      [IN] uFlags
//          Configuration flags
//
//  @return
//      EDID_STATUS_SUCCESS.
//-------------------------------------------------------------------------------------------------
//
static EDID_Status EDID_Parser_GetAudioModeInfo(EDID_ParserContextType *pEdidSharedData, EDID_AudioModeInfoType* pAudioModeInfo, uint32 uFlags)
{
  EDID_Status            eStatus     = EDID_STATUS_OK;

  if ((NULL == pAudioModeInfo) ||
      (NULL == pEdidSharedData))
  {
    eStatus = EDID_STATUS_BAD_PARAM;
  }
  else
  {
    uint8   *pDataBuf              = pEdidSharedData->auEDIDCache;
    uint32   uDataTagCode          = 0;
    uint32   uNumOfBytes           = 0;
    uint32   uWorkingOffset        = 0;
    uint32   uI                    = 0;
    uint32   uCEABlockIndex        = 0;
    uint32   uStartingOffset       = 0;
    uint32   uNumOfCEABlocks       = EDID_GetNumOfCEAExtBlocks(pEdidSharedData);
    bool32   bDispModeSupportAudio = EDID_IsDispModeSupportAudio(pEdidSharedData);


    /* Send audio mode info only when the the current display mode supports audio*/
    if ((TRUE == bDispModeSupportAudio) &&
        (uNumOfCEABlocks > 0))
    {

      /* Skip Block 2 if block 2 is a BLOCK MAP */
      if (EDID_EXTENSION_BLOCK_MAP_TAG_CODE == pDataBuf[EDID_CEA_EXTENSION_VERSION_OFFSET - 1])
      {
        uStartingOffset += EDID_BLOCK_SIZE;
        if (uNumOfCEABlocks)
        {
          uNumOfCEABlocks -= 1;
        }
      }

      uStartingOffset += EDID_BLOCK_SIZE;

      for (uCEABlockIndex = 0; uCEABlockIndex < uNumOfCEABlocks; uCEABlockIndex++)
      {
        /* Whether any audio descriptors exists */
        if ((pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uCEABlockIndex].uAudioDataBlockOffset) || 
            (pEdidSharedData->sDataBlockDesc.sExtAudioDataBlockDesc[uCEABlockIndex].uAudioDataBlockOffset))
        {
          bool32 bAudioSupport = (pDataBuf[uStartingOffset + EDID_CEA_EXTENSION_CABS_RELATIVE_OFFSET] & EDID_BASIC_AUDIO_SUPPORT_MASK) >> EDID_BASIC_AUDIO_SUPPORT_SHIFT;

          // If the sink reports basic audio support
          if (bAudioSupport)
          {
            if ((EDID_GET_AUDIO_INSERT_DEFAULT_MODE & uFlags) &&
                (0 == pEdidSharedData->uNumCompatibleAudioModes) &&
                (0 == pAudioModeInfo->uAudioModeIndex))
            {
              // If there are no compatible audio modes in the Audio blocks but the CEA block indicates audio is supported,
              // insert the default audio mode as required by HDMI specification and return;
              
              EDID_Parser_SetDefaultAudioMode(pAudioModeInfo);
              
              eStatus           = EDID_STATUS_OK;
              break;
            }
              
            // Reset the status to clear errors from previous block iterations              
            eStatus = EDID_STATUS_OK;

            // For all other indicies, continue to parse the sink's audio capabilities
            if (pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uCEABlockIndex].uAudioDataBlockOffset)
            {
              uWorkingOffset = pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uCEABlockIndex].uAudioDataBlockOffset;

              /* Extract the video data tag code */
              uDataTagCode = (pDataBuf[uWorkingOffset] & EDID_DATA_BLOCK_TAG_MASK) >> EDID_DATA_BLOCK_TAG_SHIFT;

              /* Check whether the audio tag is correct */
              if (EDID_AUDIO_DATA_BLOCK_TAG_CODE == uDataTagCode)
              {
                /* Find out the amount of descriptors within the audio data block */
                uNumOfBytes = pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uCEABlockIndex].uDataBlockLength;
              }
              else
              {
                eStatus = EDID_STATUS_FAILED;
              }
            }

            /* TODO: what happens for extended audio modes */
            if (pEdidSharedData->sDataBlockDesc.sExtAudioDataBlockDesc[uCEABlockIndex].uAudioDataBlockOffset)
            {
              uWorkingOffset = pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uCEABlockIndex].uAudioDataBlockOffset + 1;
              /* Extract the audio data tag code */
              uDataTagCode = (pDataBuf[uWorkingOffset] & EDID_DATA_BLOCK_TAG_MASK) >> EDID_DATA_BLOCK_TAG_SHIFT;
              if (EDID_EXTENDED_VENDOR_SPECIFIC_AUDIO_TAG_CODE == uDataTagCode)
              {
                /* Find out the amount of descriptors within the audio data block - Minus one for the extended tag code byte */
                uNumOfBytes = pEdidSharedData->sDataBlockDesc.sAudioDataBlockDesc[uCEABlockIndex].uDataBlockLength - 1;
              }
              else
              {
                eStatus = EDID_STATUS_FAILED;
              }
            }

            if (EDID_STATUS_SUCCESS == eStatus)
            {
              /* Move to next byte - 1st byte of the 1st audio descriptor */
              uWorkingOffset++;

              /* Move to the corresponding audio data descriptor */
              uWorkingOffset += pAudioModeInfo->uAudioModeIndex * EDID_AUDIO_SHORT_DESC_SIZE;

              /* Ensure the modeIndex is within the audio data block size */
              if ((pAudioModeInfo->uAudioModeIndex * EDID_AUDIO_SHORT_DESC_SIZE) < uNumOfBytes)
              {
                pAudioModeInfo->eAudioFormat = (EDID_AudioFormatType)((pDataBuf[uWorkingOffset] & EDID_AUDIO_DESC_AUDIO_FORMAT_MASK) >> EDID_AUDIO_DESC_AUDIO_FORMAT_SHIFT);
                pAudioModeInfo->uAudioChannelMask = (pDataBuf[uWorkingOffset] & EDID_AUDIO_DESC_AUDIO_MAX_CHANNEL_MASK) + 1;
                switch (pAudioModeInfo->uAudioChannelMask)
                {
                case 2:
                  pAudioModeInfo->uAudioChannelMask = (EDID_AUDIO_CHANNEL_2);
                  break;
                case 4:
                  pAudioModeInfo->uAudioChannelMask = (EDID_AUDIO_CHANNEL_4 | EDID_AUDIO_CHANNEL_2);
                  break;
                case 6:
                  pAudioModeInfo->uAudioChannelMask = (EDID_AUDIO_CHANNEL_6 | EDID_AUDIO_CHANNEL_4 | EDID_AUDIO_CHANNEL_2);
                  break;
                case 8:
                  pAudioModeInfo->uAudioChannelMask = (EDID_AUDIO_CHANNEL_8 | EDID_AUDIO_CHANNEL_6 | EDID_AUDIO_CHANNEL_4 | EDID_AUDIO_CHANNEL_2);
                  break;
                default:
                  pAudioModeInfo->uAudioChannelMask = (EDID_AUDIO_CHANNEL_NONE);
                  break;
                }
                pAudioModeInfo->uAudioSampleRateMask = 0;
                /* Move to byte 2 of audio short descriptor */
                uWorkingOffset++;
                for (uI = 0; uI < 7; uI++)
                {
                  if ((pDataBuf[uWorkingOffset] >> uI) & 0x1)
                  {
                    switch (uI)
                    {
                    case 0:
                      pAudioModeInfo->uAudioSampleRateMask |= EDID_AUDIO_SAMPLE_RATE_32KHZ;
                      break;
                    case 1:
                      pAudioModeInfo->uAudioSampleRateMask |= EDID_AUDIO_SAMPLE_RATE_44_1KHZ;
                      break;
                    case 2:
                      pAudioModeInfo->uAudioSampleRateMask |= EDID_AUDIO_SAMPLE_RATE_48KHZ;
                      break;
                    case 3:
                      pAudioModeInfo->uAudioSampleRateMask |= EDID_AUDIO_SAMPLE_RATE_88_2KHZ;
                      break;
                    case 4:
                      pAudioModeInfo->uAudioSampleRateMask |= EDID_AUDIO_SAMPLE_RATE_96KHZ;
                      break;
                    case 5:
                      pAudioModeInfo->uAudioSampleRateMask |= EDID_AUDIO_SAMPLE_RATE_176_4KHZ;
                      break;
                    case 6:
                      pAudioModeInfo->uAudioSampleRateMask |= EDID_AUDIO_SAMPLE_RATE_192KHZ;
                      break;
                    default:
                      pAudioModeInfo->uAudioSampleRateMask |= EDID_AUDIO_SAMPLE_RATE_32KHZ;
                      break;
                    }
                  }
                }
                pAudioModeInfo->audioStreamInfo.uAudioSampleBitDepthMask = 0;
                /* Move to byte 3 of audio short descriptor */
                uWorkingOffset++;
                if (EDID_AUDIO_FORMAT_LPCM == pAudioModeInfo->eAudioFormat)
                {
                  for (uI = 0; uI < 3; uI++)
                  {
                    if ((pDataBuf[uWorkingOffset] >> uI) & 0x1)
                    {
                      switch (uI)
                      {
                      case 0:
                        pAudioModeInfo->audioStreamInfo.uAudioSampleBitDepthMask |= EDID_AUDIO_BIT_DEPTH_16_BIT;
                        break;
                      case 1:
                        pAudioModeInfo->audioStreamInfo.uAudioSampleBitDepthMask |= EDID_AUDIO_BIT_DEPTH_20_BIT;
                        break;
                      case 2:
                        pAudioModeInfo->audioStreamInfo.uAudioSampleBitDepthMask |= EDID_AUDIO_BIT_DEPTH_24_BIT;
                        break;
                      default:
                        pAudioModeInfo->audioStreamInfo.uAudioSampleBitDepthMask |= EDID_AUDIO_BIT_DEPTH_16_BIT;
                        break;
                      }
                    }
                  }
                }
                else
                {
                  pAudioModeInfo->audioStreamInfo.uAudioMaxBitRate = pDataBuf[uWorkingOffset] * EDID_AUDIO_BIT_RATE_8KBPS;  /* Need to multiply 8Kbps to get maximum bit rate */
                }
              }
              else
              {
                eStatus = EDID_STATUS_FAILED;
              }
            }

            if (eStatus == EDID_STATUS_SUCCESS)
            {
              if (pEdidSharedData->sDataBlockDesc.sSpeakerDataBlockDesc[uCEABlockIndex].uSpeakerAllocBlockOffset)
              {
                /* Speaker allocation block offset */
                uWorkingOffset = pEdidSharedData->sDataBlockDesc.sSpeakerDataBlockDesc[uCEABlockIndex].uSpeakerAllocBlockOffset;

                /* Extract the speaker allocation data tag code if it exists */
                uDataTagCode = (pDataBuf[uWorkingOffset] & EDID_DATA_BLOCK_TAG_MASK) >> EDID_DATA_BLOCK_TAG_SHIFT;

                /* Check whether the speaker allocation tag exists */
                if (EDID_SPEAKER_ALLOC_DATA_BLOCK_TAG_CODE == uDataTagCode)
                {
                  /* Move to next byte */
                  uWorkingOffset++;
                  for (uI = 0; uI < 7; uI++)
                  {
                    if ((pDataBuf[uWorkingOffset] >> uI) & 0x1)
                    {
                      switch (uI)
                      {
                      case 0:
                        pAudioModeInfo->uAudioSpeakerLocationMask |= (EDID_AUDIO_SPEAKER_LOCATION_FL | EDID_AUDIO_SPEAKER_LOCATION_FR);
                        break;
                      case 1:
                        pAudioModeInfo->uAudioSpeakerLocationMask |= EDID_AUDIO_SPEAKER_LOCATION_LFE;
                        break;
                      case 2:
                        pAudioModeInfo->uAudioSpeakerLocationMask |= EDID_AUDIO_SPEAKER_LOCATION_FC;
                        break;
                      case 3:
                        pAudioModeInfo->uAudioSpeakerLocationMask |= (EDID_AUDIO_SPEAKER_LOCATION_RL | EDID_AUDIO_SPEAKER_LOCATION_RR);
                        break;
                      case 4:
                        pAudioModeInfo->uAudioSpeakerLocationMask |= EDID_AUDIO_SPEAKER_LOCATION_RC;
                        break;
                      case 5:
                        pAudioModeInfo->uAudioSpeakerLocationMask |= (EDID_AUDIO_SPEAKER_LOCATION_FLC | EDID_AUDIO_SPEAKER_LOCATION_FRC);
                        break;
                      case 6:
                        pAudioModeInfo->uAudioSpeakerLocationMask |= (EDID_AUDIO_SPEAKER_LOCATION_RLC | EDID_AUDIO_SPEAKER_LOCATION_RRC);
                        break;
                      default:
                        pAudioModeInfo->uAudioSpeakerLocationMask |= (EDID_AUDIO_SPEAKER_LOCATION_FL | EDID_AUDIO_SPEAKER_LOCATION_FR);
                        break;
                      }
                    }
                  }
                }//if(EDID_SPEAKER_ALLOC_DATA_BLOCK_TAG_CODE == uDataTagCode)
              }
            }//if(eStatus == EDID_STATUS_SUCCESS)
          }//if(bAudioSupport)
        }
        else if ((0 == pEdidSharedData->uNumCompatibleAudioModes) && 
                 (0 == pAudioModeInfo->uAudioModeIndex))
        {
          // If none of the CEA blocks have compatible audio blocks (0==uNumCompatibleAudioModes)
          // but EDID indicates that audio is supported,
          // insert the default audio mode as required by HDMI specification and return;         
          EDID_Parser_SetDefaultAudioMode(pAudioModeInfo);
          
          eStatus           = EDID_STATUS_OK;
          break;
        }
        else
        {
          // Sink does not support audio or this is a resolution for which audio is not supported.
          eStatus = EDID_STATUS_NOT_SUPPORTED;
        }
      }
    }
    else if (bDispModeSupportAudio)
    {
      // If there are injected CEA modes, return basic audio capabilities in mode index 0.
      if ((pAudioModeInfo->uAudioModeIndex == 0) &&
          (pEdidSharedData->uInjectedModeNum > 0))
      {
        /* In the event that video/audio data block doesn't exists - basic audio capabilities should still be returned */
        EDID_Parser_SetDefaultAudioMode(pAudioModeInfo);
        eStatus           = EDID_STATUS_OK;
      }
    }
    else
    {
      // Sink does not support audio or this is a resolution for which audio is not supported.
      eStatus = EDID_STATUS_NOT_SUPPORTED;
    }
  }

  return eStatus;
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_GetSinkCapability
//
//  @brief
//      Parses the CEA extension Byte 3 to extract the sink capability 
//      (audio support, YUV support).
//
//      [IN/OUT] pEdidSharedData
//          EDID parser contect
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_Parser_GetSinkCapability(EDID_ParserContextType *pEdidSharedData)
{
  uint32 uNumOfCEABlocks = 0;
  uint32 uStartingOffset = 0;

  // Initialize
  pEdidSharedData->bAudioSupport = FALSE;
  pEdidSharedData->bYUVSupport   = FALSE;

  uNumOfCEABlocks = EDID_GetNumOfCEAExtBlocks(pEdidSharedData);
  
  // Skip Block 2 if block 2 is a BLOCK MAP
  if (EDID_EXTENSION_BLOCK_MAP_TAG_CODE == pEdidSharedData->auEDIDCache[EDID_CEA_EXTENSION_VERSION_OFFSET - 1])
  {
    uStartingOffset += EDID_BLOCK_SIZE;
    if (uNumOfCEABlocks)
    {
      uNumOfCEABlocks -= 1;
    }
  }

  uStartingOffset += EDID_BLOCK_SIZE;

  if (uNumOfCEABlocks)
  {
    if (pEdidSharedData->auEDIDCache[uStartingOffset + EDID_CEA_EXTENSION_CABS_RELATIVE_OFFSET] & EDID_BASIC_AUDIO_SUPPORT_MASK)
    {
      pEdidSharedData->bAudioSupport = TRUE;
    }

    if ((pEdidSharedData->auEDIDCache[uStartingOffset + EDID_CEA_EXTENSION_CABS_RELATIVE_OFFSET] & EDID_YCBCR_SUPPORT_MASK))
    {
      pEdidSharedData->bYUVSupport = TRUE;
    }
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_DetailTiming
//
//  @brief
//      Parses the EDID structure and find out all the video mode supported using 
//      detailed timing descriptors of the first block i.e EDID block.
//
//  @params
//      [IN/OUT] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//
//  @return
//      EDID_STATUS_SUCCESS if parsing is successful.
//-------------------------------------------------------------------------------------------------
//
static EDID_Status EDID_Parser_DetailTiming(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList)
{
  EDID_Status            eStatus            = EDID_STATUS_SUCCESS;
  EDID_VideoFormatType  eVideoFormat       = EDID_VIDEO_FORMAT_640x480p60_4_3;
  uint32                uDescOffset        = 0;
  uint32                uI                 = 0;

 /* Maximum 4 timing descriptor in block 0 */
 for(uI = 0; uI < EDID_TIMING_BLOCK_0_MAX_DESC; uI++)
 {
   uDescOffset = EDID_FIRST_TIMING_DESC_OFFSET + (uI * EDID_DETAIL_TIMING_DESC_BLOCK_SIZE);

   if(pDataBuf[uDescOffset])
   {
     eStatus = EDID_Parser_Detail_Desc(pEdidSharedData, &pDataBuf[uDescOffset], &eVideoFormat);
     
     if(EDID_STATUS_SUCCESS == eStatus)
     {
       /* The resolution read from the EDID block won't support audio , hence no audio support flag is set*/ 
       EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, eVideoFormat, EDID_VIDEO_YUV420_NOT_SUPPORTED);
     }
   }
   else
   {
     /* The first 3 bytes should be 0 if it is used as display descriptor */
     if((!pDataBuf[uDescOffset]) && (!pDataBuf[uDescOffset + 1]) && (!pDataBuf[uDescOffset + 2]))
     {
       EDID_Parser_DisplayDescriptor(pEdidSharedData, pDataBuf, uDescOffset, pDispModeList);
     }
   }
  }
  return eStatus;
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_DisplayDescriptor
//
//  @brief
//      Parses the display descriptor block of the EDID.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN] uDescOffset
//          Starting offset of display descriptor.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_Parser_DisplayDescriptor(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uDescOffset, EDID_DispModeListType* pDispModeList)
{

  // Additional standard timing
  if (EDID_DISPLAY_DESC_STANDARD_TIMING_TAG == pDataBuf[uDescOffset + EDID_DISPLAY_DESC_TAG_OFFSET])
  {
    uint32 uStandardTimingStartOffset = uDescOffset + EDID_DISPLAY_DESC_STANDARD_TIMING_START_OFFSET;
    uint32 uStandardTimingEndOffset = uStandardTimingStartOffset + 2 * EDID_DISPLAY_DESC_STANDARD_TIMING_DEFINITION_COUNT  - 1;
    
    EDID_Parser_StandardTimings(pEdidSharedData, pDataBuf, uStandardTimingStartOffset, uStandardTimingEndOffset, pDispModeList); 
  }

  // Establish timing III
  if(EDID_DISPLAY_DESC_ESTABLISH_TIMINGIII_TAG == pDataBuf[uDescOffset + EDID_DISPLAY_DESC_TAG_OFFSET])
  {
    EDID_Parser_EstablishTimingIII(pEdidSharedData, pDataBuf, uDescOffset + EDID_DISPLAY_DESC_ESTABLISH_TIMINGIII_START_OFFSET, pDispModeList); 
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_EstablishTimingIII
//
//  @brief
//      Parses the establish timing III section of the display descriptor.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN] uEstablishTimingOffset
//          Starting offset of establish timing III section.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//
//  @return
//      EDID_STATUS_SUCCESS if data is read from the DPP file.
//-------------------------------------------------------------------------------------------------
//
void EDID_Parser_EstablishTimingIII(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uEstablishTimingOffset, EDID_DispModeListType* pDispModeList)
{
  // Skip the first byte of the establish timing section
  uint8 uEstablishedTimingIII = pDataBuf[uEstablishTimingOffset + 1];

  if (uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1280x768p60)
  {         
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1280x768p60_5_3,EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  // Move to the next byte of establish timing
  uEstablishedTimingIII = pDataBuf[uEstablishTimingOffset + 2];
  
  if (uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1280x960p60)
  {         
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1280x960p60_4_3,EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if (uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1280x1024p60)
  {         
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1280x1024p60_5_4,EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  // Move to the next byte of establish timing
  uEstablishedTimingIII = pDataBuf[uEstablishTimingOffset + 3];

  if (uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1360x768p60)
  {         
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1360x768p60_16_9,EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if (uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1440x900p60)
  {         
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1440x900p60_16_10,EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }
  
  if (uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1400x1050p60)
  {         
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1400x1050p60_4_3,EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  // Move to the next byte of establish timing
  uEstablishedTimingIII = pDataBuf[uEstablishTimingOffset + 4];

  if (uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1680x1050p60)
  {         
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1680x1050p60_16_10,EDID_VIDEO_YUV420_NOT_SUPPORTED);    
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_InsertShortVideoDesc
//
//  @brief
//      Populates the global display mode list.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//      [IN] uNumOfDispMode
//          Total number of modes to iterate.
//      [IN] uBufferIndex
//          Working Offset of the cache buffer.
//      [IN] eYUVSupport
//           YUV 420 support.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_InsertShortVideoDesc(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList, uint32 uNumOfDispMode, uint32 uBufferIndex, EDID_VideoFormatYUV420Support_Type eYUVSupport)
{
  uint32                uWorkingOffset = uBufferIndex;
  uint32                uI             = 0;

  if (pDataBuf)
  {
    EDID_VideoFormatType  eVideoFormat;

    // Move to the first short video descriptor
    uWorkingOffset++;

    for (uI = 0; uI < uNumOfDispMode; uI++)
    {
      // Check for Valid Video ID code
      if (0 != pDataBuf[uWorkingOffset + uI])
      {
        //Subtract 1 because it is zero based in the driver, while the Video identification code is 1 based in the CEA_861D spec
        eVideoFormat = (EDID_VideoFormatType)((pDataBuf[uWorkingOffset + uI] & EDID_SHORT_VIDEO_DESC_VID_MASK) - 1); 
        EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, eVideoFormat, eYUVSupport);
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_Detail_Desc
//
//  @brief
//      Parses the EDID structure and find out all the video mode supported 
//      using detailed timing descriptors.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [OUT] pDispMode
//          Matching display mode.
//
//  @return
//      EDID_STATUS_SUCCESS if matching mode is found.
//-------------------------------------------------------------------------------------------------
//
static EDID_Status EDID_Parser_Detail_Desc(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_VideoFormatType* pDispMode)
{
  EDID_Status           eStatus          = EDID_STATUS_FAILED;
  bool32               bAspectRatio4_3  = FALSE;
  bool32               bInterlaced      = FALSE;
  uint32               uResult          = 0;
  uint32               uResult1         = 0;
  uint32               uActiveH         = 0;
  uint32               uActiveV         = 0;
  uint32               uBlankH          = 0;
  uint32               uBlankV          = 0;
  uint32               uIndex           = 0;
  uint32               uMaxNumOfElement = 0;
  uint32               uImageSizeH      = 0;
  uint32               uImageSizeV      = 0;
  uint32               uPixelClock      = 0;

  if (pDataBuf)
  {
    uPixelClock   = pDataBuf[EDID_TIMING_PIXEL_CLOCK_UPPER_BYTE_OFFSET];
    uPixelClock <<= 8;
    uPixelClock  |= (uint32)pDataBuf[EDID_TIMING_PIXEL_CLOCK_LOWER_BYTE_OFFSET];
    uPixelClock   = uPixelClock * 10; // Convert to KHZ for LUT search

    // See VESA Spec
    uActiveH   =((pDataBuf[EDID_TIMING_DESC_UPPER_H_NIBBLE_OFFSET] >> EDID_TIMING_H_ACTIVE_UPPER_NIBBLE_SHIFT) & 
                  EDID_TIMING_H_ACTIVE_UPPER_NIBBLE_MASK);
    uActiveH <<= 8; //Shift 8 bits 
    uActiveH  |= (uint32)pDataBuf[EDID_TIMING_DESC_H_ACTIVE_OFFSET]; //Shift 8 bits 

    uBlankH   =(pDataBuf[EDID_TIMING_DESC_UPPER_H_NIBBLE_OFFSET] & EDID_TIMING_H_BLANK_UPPER_NIBBLE_MASK);
    uBlankH <<= 8; //Shift 8 bits 
    uBlankH  |= (uint32)pDataBuf[EDID_TIMING_DESC_H_BLANK_OFFSET]; //Shift 8 bits 

    uActiveV   =((pDataBuf[EDID_TIMING_DESC_UPPER_V_NIBBLE_OFFSET] >> EDID_TIMING_V_ACTIVE_UPPER_NIBBLE_SHIFT) & 
                  EDID_TIMING_V_ACTIVE_UPPER_NIBBLE_MASK);
    uActiveV <<= 8; //Shift 8 bits 
    uActiveV  |= (uint32)pDataBuf[EDID_TIMING_DESC_V_ACTIVE_OFFSET]; //Shift 8 bits 

    uBlankV   =(pDataBuf[EDID_TIMING_DESC_UPPER_V_NIBBLE_OFFSET] & EDID_TIMING_V_BLANK_UPPER_NIBBLE_MASK);
    uBlankV <<= sizeof(uint8); //Shift 8 bits 
    uBlankV  |= (uint32)pDataBuf[EDID_TIMING_DESC_V_BLANK_OFFSET]; //Shift 8 bits 

    uImageSizeH   = (pDataBuf[EDID_TIMING_DESC_IMAGE_SIZE_UPPER_NIBBLE_OFFSET] >> EDID_TIMING_H_IMAGE_SIZE_UPPER_NIBBLE_SHIFT) &
                     EDID_TIMING_H_IMAGE_SIZE_UPPER_NIBBLE_MASK;
    uImageSizeH <<= 8;
    uImageSizeH  |= (uint32)pDataBuf[EDID_TIMING_DESC_H_IMAGE_SIZE_OFFSET];

    uImageSizeV   = (pDataBuf[EDID_TIMING_DESC_IMAGE_SIZE_UPPER_NIBBLE_OFFSET]& EDID_TIMING_V_IMAGE_SIZE_UPPER_NIBBLE_MASK);
    uImageSizeV <<= 8;
    uImageSizeV  |= (uint32)pDataBuf[EDID_TIMING_DESC_V_IMAGE_SIZE_OFFSET];

    // Calculating aspect ratio 4:3
    uResult  = uImageSizeH * 3;
    uResult1 = uImageSizeV * 4;

    // Due to rounding of the image size in the EDID info, we need to provide a range to check against
    if(uResult == uResult1)
    {
      bAspectRatio4_3 = TRUE;
    }
    else if(uResult1 > uResult)
    {
      if(EDID_TIMING_ASPECT_RATIO_CHECK_RANGE > (uResult1 - uResult))
      {
        bAspectRatio4_3 = TRUE;
      }
    }
    else
    {
      if(EDID_TIMING_ASPECT_RATIO_CHECK_RANGE > (uResult - uResult1))
      {
        bAspectRatio4_3 = TRUE;
      }
    }

    uMaxNumOfElement  = (sizeof(asEDIDDispModeLUT)/ sizeof(EDID_VideoModePropertyType));
    uIndex            = uMaxNumOfElement >> 2;

    // Break table in half and search using H Active
    if(uActiveH <= asEDIDDispModeLUT[uIndex].uActiveH)
      uIndex = 0;

    bInterlaced = (pDataBuf[EDID_TIMING_DESC_INTERLACE_OFFSET] & EDID_TIMING_INTERLACE_MASK) >> EDID_TIMING_INTERLACE_SHIFT;

    // Interlace mode the active V should multiply by 2 because EDID reports field height only
    if(bInterlaced)
      uActiveV = uActiveV << 1;

    while(uIndex < uMaxNumOfElement)
    {
      if(bInterlaced == asEDIDDispModeLUT[uIndex].bInterlaced  &&
         uActiveH    == asEDIDDispModeLUT[uIndex].uActiveH     &&
         uBlankH     == asEDIDDispModeLUT[uIndex].uTotalBlankH &&
         uBlankV     == asEDIDDispModeLUT[uIndex].uTotalBlankV && 
         uActiveV    == asEDIDDispModeLUT[uIndex].uActiveV)
      {
        // Pixel clock needs to check against a range to include non integer value of the refresh rates
        if(((asEDIDDispModeLUT[uIndex].fPixelFreq - EDID_TIMING_PIXEL_CLOCK_RANGE)  < uPixelClock) &&  (uPixelClock < asEDIDDispModeLUT[uIndex].fPixelFreq + EDID_TIMING_PIXEL_CLOCK_RANGE))
        {
          // 16:9 VIC code of the same video format is 1 greater than the 4:3 aspect ratio VIC
          if(asEDIDDispModeLUT[uIndex].bAspectRatio4_3 && (!bAspectRatio4_3))
            *pDispMode = (EDID_VideoFormatType)(asEDIDDispModeLUT[uIndex].eVideoCode + 1);
          else
            *pDispMode = (EDID_VideoFormatType)asEDIDDispModeLUT[uIndex].eVideoCode;

          // Check whether HDMI TX core supports this mode
          if (pEdidSharedData->eEDIDSpecType & gEDIDSupportedVideoModeLUT[*pDispMode].eEDIDSpecType)
          {
            eStatus = EDID_STATUS_SUCCESS;
          }            
          break;
        }
      }
      uIndex++;
    }
    if (eStatus != EDID_STATUS_SUCCESS)
    {
        DISP_OSAL_LOG_WARN("DP_Parser_Detail_Desc: No MATCH for itlaced=%d W=%d H=%d BlankH=%d BlankV=%d pclk=%d\n", bInterlaced, uActiveH, uActiveV, uBlankH, uBlankV, uPixelClock);
    }
  }

  return eStatus;
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_InsertDispFormat
//
//  @brief
//      If a video modes is not present in the mode list, inserts it to the mode list, 
//      else updates the information.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//      [IN] eVideoFormat
//          Video mode to be inserted.
//      [IN] eYUV420Support
//          YUV 420 support format.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_Parser_InsertDispFormat(EDID_ParserContextType *pEdidSharedData, EDID_DispModeListType* pDispModeList, EDID_VideoFormatType eVideoFormat, EDID_VideoFormatYUV420Support_Type eYUV420Support)
{
  if (pDispModeList)
  {
    
    if ((EDID_VIDEO_FORMAT_MAX  >  eVideoFormat) && 
        (EDID_SPEC_TYPE_UNSUPPORTED != gEDIDSupportedVideoModeLUT[eVideoFormat].eEDIDSpecType))
    {    
      uint32 uNumOfModes = pDispModeList->uNumOfElements;
      uint32 uIndex = 0;

      // Check if limiting to add this mode.
      if (EDID_RejectFilteredModes(pEdidSharedData, eVideoFormat, eYUV420Support))
      {  
        //Search for duplicate entry in the mode list
        for (uIndex = 0; (uIndex < uNumOfModes) && (uIndex < EDID_VIDEO_FORMAT_MAX) && (pDispModeList->aDispModeAttr[uIndex].eVideoFormat != eVideoFormat); uIndex++);

        if (uIndex < EDID_VIDEO_FORMAT_MAX)
        {
          // Request to add new mode
          if (uIndex == uNumOfModes)
          {
            uint32     uPixelFormatMap = (1 << EDID_PIXEL_FORMAT_RGB888); //Default pixel format support
            EDID_Status eLocalStatus    = EDID_STATUS_SUCCESS;

            // YUV formats are supported only for HDMI 2.0
            if (EDID_HDMI_SPEC_TYPE_2_0 & pEdidSharedData->eEDIDSpecType)
            {
              // Check if sink supports YUV444 and YUV422
              if (pEdidSharedData->bYUVSupport)
              {
                uPixelFormatMap |= ((1 << EDID_PIXEL_FORMAT_YUV444)| (1 << EDID_PIXEL_FORMAT_YUV422));
              }
              // Check Sink and source YUV420 support for this mode
              if (EDID_VIDEO_YUV420_ONLY_SUPPORTED == eYUV420Support)
              {
                if (HDMI_YUV420_SUPPORTED & gEDIDSupportedVideoModeLUT[eVideoFormat].uModeFlags)
                {  
                   uPixelFormatMap = (1 << EDID_PIXEL_FORMAT_YUV420); 
                }
                else
                {
                  // HW can not support YUV420 sampling for this mode, do not add to the list
                  eLocalStatus  = EDID_STATUS_NOT_SUPPORTED;
                }
              }
              else if ((EDID_VIDEO_YUV420_ALSO_SUPPORTED == eYUV420Support) && 
                       (HDMI_YUV420_SUPPORTED & gEDIDSupportedVideoModeLUT[eVideoFormat].uModeFlags))
              {
                // HW can support YUV420 sampling this mode
                uPixelFormatMap |= (1 << EDID_PIXEL_FORMAT_YUV420);
              }
            }    

            // There is no duplicate entry, mode and pixel format is supported add to the mode list
            if (EDID_STATUS_OK == eLocalStatus)
            {
              bool32 bAudioSupported = FALSE;
              
              // Audio is supported only if:
              // 1. If the EDID indicates that the TV/monitor supports audio (pSharedData->bAudioSupport) and
              // 2. If the particular mode supports audio (HDMI_AUDIO_SUPPORTED)
              if ((TRUE == pEdidSharedData->bAudioSupport) && 
                  (HDMI_AUDIO_SUPPORTED & gEDIDSupportedVideoModeLUT[eVideoFormat].uModeFlags))
              {
                bAudioSupported = TRUE;
              }
              
              pDispModeList->aDispModeAttr[uIndex].bVideoOnly      = !bAudioSupported;
              pDispModeList->aDispModeAttr[uIndex].eVideoFormat    = eVideoFormat;
              pDispModeList->aDispModeAttr[uIndex].uPixelFormatMap = uPixelFormatMap;
              pDispModeList->uNumOfElements++;



              // Update the timing info
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncPulseWidthDclk  = gEDIDSupportedVideoModeLUT[eVideoFormat].uPulseWidthH;
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncBackPorchDclk   = gEDIDSupportedVideoModeLUT[eVideoFormat].uBackPorchH;
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncFrontPorchDclk  = gEDIDSupportedVideoModeLUT[eVideoFormat].uFrontPorchH;
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncPulseWidthLines = gEDIDSupportedVideoModeLUT[eVideoFormat].uPulseWidthV;
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncBackPorchLines  = gEDIDSupportedVideoModeLUT[eVideoFormat].uBackPorchV;
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncFrontPorchLines = gEDIDSupportedVideoModeLUT[eVideoFormat].uFrontPorchV;
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.bInterlaced           = pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.bInterlaced = gEDIDSupportedVideoModeLUT[eVideoFormat].bInterlaced;
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.bIsHsyncActiveLow     = gEDIDSupportedVideoModeLUT[eVideoFormat].bActiveLowH;
              pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.bIsVsyncActiveLow     = gEDIDSupportedVideoModeLUT[eVideoFormat].bActiveLowV;

              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uWidthPx               = gEDIDSupportedVideoModeLUT[eVideoFormat].uActiveH;
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uWidthTotalPx          = pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uWidthPx +
                                                                                             pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncPulseWidthDclk +
                                                                                             pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncBackPorchDclk +
                                                                                             pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncFrontPorchDclk;
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uHeightPx              = gEDIDSupportedVideoModeLUT[eVideoFormat].uActiveV;
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uHeightTotalPx         = pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uHeightPx +
                                                                                             pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncPulseWidthLines +
                                                                                             pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncBackPorchLines +
                                                                                             pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncFrontPorchLines;
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uRefreshRate           = Q16_FLOAT_VALUE(gEDIDSupportedVideoModeLUT[eVideoFormat].uRefreshRate / 1000);
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uPixelRate             = gEDIDSupportedVideoModeLUT[eVideoFormat].uPixelFreq * 1000;
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.bAudioSupported        = bAudioSupported;
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eColorFormat           = EDID_PIXEL_COLOR_FORMAT_RGB_888_24BPP; // default for now

              DISP_OSAL_LOG_INFO("EDID_Parser_InsertDispFormat:videoFormat=%d w=%d h=%d frate=%x\n", eVideoFormat,
gEDIDSupportedVideoModeLUT[eVideoFormat].uActiveH, gEDIDSupportedVideoModeLUT[eVideoFormat].uActiveV,
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uRefreshRate);

              switch(gEDIDSupportedVideoModeLUT[eVideoFormat].eAspectRatio)
              {
                  case EDID_VIDEO_ASPECT_RATIO_NONE:
                      pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_NONE;
                      break;
                  case EDID_VIDEO_ASPECT_RATIO_4_3:
                      pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_4_3;
                      break;
                  case EDID_VIDEO_ASPECT_RATIO_16_9:
                      pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_16_9;
                      break;
                  case EDID_VIDEO_ASPECT_RATIO_5_4:
                      pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_5_4;
                      break;                    
                  case EDID_VIDEO_ASPECT_RATIO_16_10:
                      pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_16_10;
                      break;                    
                  case EDID_VIDEO_ASPECT_RATIO_5_3:
                      pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_5_3;                 
                      break;                    
                  default:
                      // Default to 16:9 for all other aspect ratios
                      pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_16_9;
                      break;
              }
            }
          }
          // Request to add a display format already added
          else
          {
            // YUV formats are supported only for HDMI 2.0
            if (EDID_HDMI_SPEC_TYPE_2_0 & pEdidSharedData->eEDIDSpecType)
            {
              // Update YUV420 support only if current request is to enable this support
              if (EDID_VIDEO_YUV420_NOT_SUPPORTED != eYUV420Support)
              {
                if (EDID_VIDEO_YUV420_ALSO_SUPPORTED == eYUV420Support)
                {
                  pDispModeList->aDispModeAttr[uIndex].uPixelFormatMap |= (1 << EDID_PIXEL_FORMAT_YUV420);
                }
                else if (EDID_VIDEO_YUV420_ONLY_SUPPORTED == eYUV420Support)
                {
                  pDispModeList->aDispModeAttr[uIndex].uPixelFormatMap = (1 << EDID_PIXEL_FORMAT_YUV420); 
                }
              }
            }
          }
        }
      }
    }    
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_RejectFilteredModes
//
//  @brief
//      Check if the mode is not greater (or lesser) than user specified maximum (or minimum)
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] eVideoFormat  
//          Video Format
//      [IN] eYUV420Support
//          YUV 420 support
//
//  @return
//      TRUE if mode has not been rejected, FALSE otherwise
//-------------------------------------------------------------------------------------------------
//
static bool32 EDID_RejectFilteredModes(EDID_ParserContextType *pEdidSharedData, EDID_VideoFormatType eVideoFormat, EDID_VideoFormatYUV420Support_Type eYUV420Support)
{
    bool32 bRet = TRUE;
   
    if ((NULL != pEdidSharedData) &&
        (EDID_VIDEO_FORMAT_MAX > eVideoFormat))
    {
        EDID_DispModeTimingType *pHDMIMode = &gEDIDSupportedVideoModeLUT[eVideoFormat];

        // Check if video format satisfies maximum link rate limit
        if ((pEdidSharedData->uMaxLinkRate > 0) &&
            (pEdidSharedData->uNumberOfLanes > 0))
        {
            uint64        uMaxBandwidth = 0;
            uint64        uBandwidth    = 0;
            uint32        uBitsPerPixel = 24;    // default to RGB
            
            // Calculate maximum supported bandwidth
            uMaxBandwidth = pEdidSharedData->uMaxLinkRate * pEdidSharedData->uNumberOfLanes;
            uMaxBandwidth *= 8;
            uMaxBandwidth /= 10;

            // Calculate bandwidth for this specific format
            uBandwidth = pHDMIMode->uPixelFreq * 1000;
            
            // Check if sink supports YUV444 and YUV422
            if (pEdidSharedData->bYUVSupport)
            {
                // For YUV422
                uBitsPerPixel = 16;

                if (((EDID_VIDEO_YUV420_ONLY_SUPPORTED == eYUV420Support) ||
                     (EDID_VIDEO_YUV420_ALSO_SUPPORTED == eYUV420Support)) &&
                    (HDMI_YUV420_SUPPORTED & pHDMIMode->uModeFlags))
                {
                   // For YUV420
                   uBitsPerPixel = 12;
                }
            }

            uBandwidth *= uBitsPerPixel;

            if(uBandwidth > uMaxBandwidth)
            {
                bRet = FALSE;
            }
        }

        if(TRUE == bRet)
        {
            if ((pEdidSharedData->uMaxModeWidth > 0) &&
                (pHDMIMode->uActiveH > pEdidSharedData->uMaxModeWidth))
            {
              // Rejected based on width
              bRet = FALSE;
            }
            else if ((pEdidSharedData->uMaxModeHeight > 0) &&
                     (pHDMIMode->uActiveV > pEdidSharedData->uMaxModeHeight))
            {
              // Rejected based on height
              bRet = FALSE;
            }
            else if ((pEdidSharedData->uMaxModeRefreshRate > 0) &&
                     (pHDMIMode->uRefreshRate > pEdidSharedData->uMaxModeRefreshRate))
            {
              // Rejected based on refrefresh rate
              bRet = FALSE;
            }
            else if ((pEdidSharedData->uMinModeWidth > 0) &&
                     (pHDMIMode->uActiveH < pEdidSharedData->uMinModeWidth))
            {
              // Rejected based on width
              bRet = FALSE;
            }
            else if ((pEdidSharedData->uMinModeHeight > 0) &&
                     (pHDMIMode->uActiveV < pEdidSharedData->uMinModeHeight))
            {
              // Rejected based on height
              bRet = FALSE;
            }
            else if ((pEdidSharedData->uMinModeRefreshRate > 0) &&
                     (pHDMIMode->uRefreshRate < pEdidSharedData->uMinModeRefreshRate))
            {
              // Rejected based on refrefresh rate
              bRet = FALSE;
            }
        }
    }
    
    return bRet;
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_StandardTimings
//
//  @brief
//      Parses the EDID standard timing section and find outall the 
//      standard video mode supported in standard timing section.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN] uStandardTimingStartOffset
//          Starting offset of the standard timing section.
//      [IN] uStandardTimingEndOffset
//          End offset of the standard timing section.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_Parser_StandardTimings(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uStandardTimingStartOffset, uint32 uStandardTimingEndOffset, EDID_DispModeListType* pDispModeList )
{
  EDID_VideoFormatType      eVideoFormat     = EDID_VIDEO_FORMAT_640x480p60_4_3;
  uint32                    uMaxNumOfElement = (sizeof(asEDIDDispModeLUT)/ sizeof(EDID_VideoModePropertyType));
  uint32                    uI               = 0;
  uint32                    uIndex           = 0;
  uint32                    uActiveH         = 0;
  uint32                    uActiveV         = 0;
  uint32                    uRefreshRate     = 0;   
  uint32                    uEdidRevision    = 0;
  uint32                    uAspectRatio     = 0;

  // As some mode is only reported in standard timing section, so need add standard timing parser.
  for ( uI = uStandardTimingStartOffset; uI < uStandardTimingEndOffset; uI += 2)
  {
    //First byte: Horizontal resolution. Multiply by 8, then add 248 for actual value.
    uActiveH = (pDataBuf[uI] << EDID_STANDARD_TIMING_HRESOLUTION_SHIFT) + EDID_STANDARD_TIMING_HRESOLUTION_BASE;
    if (uActiveH > EDID_STANDARD_TIMING_HRESOLUTION_BASE + 8) // pDataBuf[uI] could be 0 or 1 if there is no stand time.
    {
      // Bits 6 & 7 : Aspect ratio. Actual vertical resolution depends on horizontal resolution.
      // 00=16:10, 01=4:3, 10=5:4, 11=16:9 
      uAspectRatio = (pDataBuf[uI + 1] & EDID_STANDARD_TIMING_ASPECTRATIO_MASK) >> EDID_STANDARD_TIMING_ASPECTRATIO_SHIFT;

      //Standard timing support modes with fixed aspect ratios.
      switch (uAspectRatio)
      {
        case 0:
           uEdidRevision = pDataBuf[EDID_REVISION_OFFSET];
        
           //EDID structures prior to version 1 revision 3 defined the bit combination of 0 0 to indicate a 1:1 aspect ratio 
           if(0x03 > uEdidRevision)
           {  
              uActiveV = uActiveH;
           }
           else
           {          
             uActiveV = uActiveH * 10 / 16;
           }
           break;
        
        case 1:
           uActiveV = uActiveH * 3 / 4;
           break;
        
        case 2:
           uActiveV = uActiveH * 4 / 5;
           break;
        
        case 3:
           uActiveV = uActiveH * 9 / 16;
           break;
        default:
           //Not support standard timing identification
           uActiveV = 0;
          break;
      }      
      if (uActiveV > 0)
      {
        // bits 0-5 are vertical frequency. Add 60 to get actual value.
        // Vertical frequency for standard timing is at least 60 Hz, therefore we have to add 60 Hz to the value read from EDID to get the correct frequency
        uRefreshRate = ((pDataBuf[uI + 1] & EDID_STANDARD_TIMING_VFREQUENCY_BITS_MASK) + EDID_STANDARD_TIMING_MIN_VFREQUENCY);
        uIndex = 0;

        while(uIndex < uMaxNumOfElement)
        {
          if ((FALSE        == asEDIDDispModeLUT[uIndex].bInterlaced)  &&
              (uActiveH     == asEDIDDispModeLUT[uIndex].uActiveH)     &&
              (uActiveV     == asEDIDDispModeLUT[uIndex].uActiveV)     && 
              (EDID_ABS((asEDIDDispModeLUT[uIndex].fRefreshRate/1000.0) - uRefreshRate) <= EDID_STANDARD_TIMING_FREQ_DELTA))
          {            
            eVideoFormat = asEDIDDispModeLUT[uIndex].eVideoCode;

            /* The resolution read from the standard timing corresponds to VESA , hence no YUV420 flag is set*/ 
            EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, eVideoFormat, EDID_VIDEO_YUV420_NOT_SUPPORTED); 

            break;
          }
          uIndex++;
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_EstablishedTimings
//
//  @brief
//      Parses the EDID established timing section and find out all the 
//      established video mode supported in standard timing section.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_Parser_EstablishedTimings(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList)
{

  // Parse the established timing section  to find out which modes are supported
  uint8 uEstablishedTimingI   = pDataBuf[EDID_ESTABLISHED_TIMINGI_START_OFFSET]; 
  uint8 uEstablishedTimingII  = pDataBuf[EDID_ESTABLISHED_TIMINGII_START_OFFSET];
  uint8 uEstablishedTimingIII = pDataBuf[EDID_ESTABLISHED_TIMINGIII_START_OFFSET]; 

  // Establish Timing I
  if ((EDID_ESTABLISHED_TIMINGI_640x480p60 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_640x480p60_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set 
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_640x480p60_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGI_640x480p72 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_640x480p72_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_640x480p72_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGI_640x480p75 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_640x480p75_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_640x480p75_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGI_800x600p56 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_800x600p56_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_800x600p56_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGI_800x600p60 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_800x600p60_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_800x600p60_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  // Established Timing II */
  
  if ((EDID_ESTABLISHED_TIMINGII_800x600p72 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_800x600p72_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_800x600p72_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_800x600p75 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_800x600p75_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_800x600p75_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_1024x768p60 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1024x768p60_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1024x768p60_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_1024x768p70 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1024x768p70_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1024x768p70_4_3 ,EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_1024x768p75 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1024x768p75_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1024x768p75_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_1280x1024p75 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1280x1024p75_5_4)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1280x1024p75_5_4, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  // Established Timing III */
  if ((EDID_ESTABLISHED_TIMINGIII_1280x768p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1280x768p60_5_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1280x768p60_5_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1280x960p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1280x960p60_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1280x960p60_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1280x1024p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1280x1024p60_5_4)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1280x1024p60_5_4, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1360x768p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1360x768p60_16_9)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1360x768p60_16_9, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1440x900p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1440x900p60_16_10)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1440x900p60_16_10, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1400x1050p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1400x1050p60_4_3)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1400x1050p60_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1680x1050p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData, EDID_VIDEO_FORMAT_1680x1050p60_16_10)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_1680x1050p60_16_10, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_ModeSupported
//
//  @brief
//      Verifies whether the specified mode is supported.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] eVideoMode
//          Video mode format that needs to verified.
//
//  @return
//      TRUE if the mode is supported.
//-------------------------------------------------------------------------------------------------
//
static bool32 EDID_ModeSupported(EDID_ParserContextType *pEdidSharedData, EDID_VideoFormatType eVideoMode)
{
  bool32               bResult     = FALSE;
  uint32               uIndex;

  for (uIndex = 0; uIndex < EDID_VIDEO_FORMAT_MAX; uIndex++)
  {
    if ((eVideoMode                     == gEDIDSupportedVideoModeLUT[uIndex].eVideoFormat)  &&
        (pEdidSharedData->eEDIDSpecType &  gEDIDSupportedVideoModeLUT[uIndex].eEDIDSpecType))
    {
      bResult = TRUE;
    }
  }

  return bResult;
}

//-------------------------------------------------------------------------------------------------
//  EDID_Inject_CEA_Mode
//
//  @brief
//      Inject CEA mode if required
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_Inject_CEA_Mode(EDID_ParserContextType *pEdidSharedData, EDID_DispModeListType* pDispModeList)
{
  uint32                   uI;

  if ((NULL != pEdidSharedData) &&
      (NULL != pDispModeList))
  {
    // Inject CEA mode.
    for (uI = 0; uI < pEdidSharedData->uInjectedModeNum; uI++)
    {
      uint32 uTemp = pDispModeList->uNumOfElements;
      
      EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, pEdidSharedData->uInjectedModeList[uI] - 1, FALSE);
      if ((uTemp != pDispModeList->uNumOfElements) ||
          ( EDID_VIDEO_FORMAT_MAX > uTemp))
      {
        pDispModeList->aDispModeAttr[uTemp].bVideoOnly = FALSE;
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_AppendExtendedResolutionDispModes
//
//  @brief
//      Parses the EDID structure and find out the Extended Resolution (4Kx2K) 
//      capabilities from vendor specific data block.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN] uStartingOffset
//          Starting offset to pDatBuf containing the 3D descriptions.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//      [IN] uCEABlockIndex
//          Index to which CEA extension block we wanted to parse.
//
//  @return
//      VOID.
//-------------------------------------------------------------------------------------------------
//
static void EDID_AppendExtendedResolutionDispModes(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, uint32 uStartingOffset, EDID_DispModeListType* pDispModeList, uint32 uCEABlockIndex)
{
  uint32      uWorkingOffset = 0;

  /* Starting offset should be pointing to the byte after HDMI_VIC_LEN field */
  uWorkingOffset = uStartingOffset;

  if (pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVideo2DDescLen > 0)
  {
    uint32 uI;

    for (uI = 0; uI<pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVideo2DDescLen; uI++)
    {
      /* Extract the Extended Resolution format*/
      EDID_VendorBlockExtendedResType eExtResFormat = (EDID_VendorBlockExtendedResType)((pDataBuf[uWorkingOffset++] >> EDID_VENDOR_BLOCK_HDMI_VIC_X_SHIFT) & EDID_VENDOR_BLOCK_HDMI_VIC_X_MASK);

      if (eExtResFormat < EDID_VENDOR_BLOCK_EXTENDED_RES_MAX)
      {
        /* Find the VideoFormat corresponding to this extended resolution */
        EDID_VideoFormatType eVideoFormat = aEDIDExtResToVideoFormatMap[eExtResFormat];

        /* Insert the video format. This function makes sure that each format is not entered more than once */
        EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, eVideoFormat, EDID_VIDEO_YUV420_NOT_SUPPORTED);

        if ((0 != pDispModeList->uNumOfElements) && 
            (eVideoFormat == pDispModeList->aDispModeAttr[pDispModeList->uNumOfElements-1].eVideoFormat))
        {
          // If the mode is added then update the HDMI_VIC code info for Vendor Specific Info frame
          pDispModeList->aDispModeAttr[pDispModeList->uNumOfElements-1].eExtHdmiVic = eExtResFormat;
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_VendorBlockInfo
//
//  @brief
//      Parses the EDID structure and find out the vendor capabilities from vendor data block.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//      [IN] uNumOfCEABlocks
//          Number of CEA extension blocks.
//
//  @return
//      EDID_STATUS_SUCCESS if vendor bloack parsing is successful.
//-------------------------------------------------------------------------------------------------
//
static EDID_Status EDID_Parser_VendorBlockInfo(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList, uint32 uNumOfCEABlocks)
{
  EDID_Status  eStatus        = EDID_STATUS_SUCCESS;
  
  if((NULL == pDataBuf) || (NULL == pDispModeList))
  {
    eStatus = EDID_STATUS_FAILED;
  }
  else
  {
    uint32      uWorkingOffset = 0;
    uint32      uDataTagCode   = 0;
    uint32      uNumOfBytes    = 0;
    uint32      uCounter       = 0;
    uint32      uNumOfBlocks   = uNumOfCEABlocks;
    uint32      uCEABlockIndex = 0;

    /* Skip Block 2 if block 2 is a BLOCK MAP */
    if(EDID_EXTENSION_BLOCK_MAP_TAG_CODE == pDataBuf[EDID_CEA_EXTENSION_VERSION_OFFSET - 1])
    {
      if(uNumOfBlocks)
      {
        uNumOfBlocks -= 1;
      }  
    }

    for(uCEABlockIndex = 0; uCEABlockIndex < uNumOfBlocks; uCEABlockIndex++)
    {
      uWorkingOffset = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVendorSpecificBlockOffset;
      if(uWorkingOffset)
      {
        /* Extract the vendor specific data tag code */
        uDataTagCode = (pDataBuf[uWorkingOffset] & EDID_DATA_BLOCK_TAG_MASK) >> EDID_DATA_BLOCK_TAG_SHIFT;
        if(EDID_VENDOR_SPECIFIC_DATA_BLOCK_TAG_CODE == uDataTagCode)
        {
          /* Find out the number of bytes within the vendor data block - Minus one for the extended tag code byte */
          uNumOfBytes = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uDataBlockLength;
          
          /* Move 1 byte to access the IEEE registration ID */
          uWorkingOffset++;

          /* Extract IEEE ID */
          pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uIEEERegistrationID = pDataBuf[uWorkingOffset];
          uWorkingOffset++;
          pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uIEEERegistrationID |= pDataBuf[uWorkingOffset] << 8;
          uWorkingOffset++;
          pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uIEEERegistrationID |= pDataBuf[uWorkingOffset] << 16;

          /* Move 1 byte to access the 16 bit physical address of the sink device */
          uWorkingOffset++;

          pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uPhysicalAddress = (pDataBuf[uWorkingOffset+1] << 8)| pDataBuf[uWorkingOffset];
        }
        else
          eStatus = EDID_STATUS_FAILED;

        if(EDID_STATUS_SUCCESS == eStatus)
        {
          /* Check whether extension fields exists */
          if(pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVendorSpecificBlockExtOffset)
          {
            uWorkingOffset = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVendorSpecificBlockExtOffset;
            while((uWorkingOffset - pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVendorSpecificBlockOffset) < pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uDataBlockLength)
            {
              switch(uCounter)
              {
                case 0:
                {
                  pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uSupportedDeepColorModeMask = (pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_DEEP_COLOR_SHIFT ) & EDID_VENDOR_BLOCK_DEEP_COLOR_MASK;
                  pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].bAISupport                  = (pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_AI_SUPPORT_SHIFT ) & EDID_VENDOR_BLOCK_AI_SUPPORT_MASK;
                  pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].bDVIDualSupport             = (pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_DVI_DUAL_SUPPORT_SHIFT ) & EDID_VENDOR_BLOCK_DVI_DUAL_SUPPORT_MASK;
                  break;
                }
                case 1:
                {
                  pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uMaxTMDSClock = pDataBuf[uWorkingOffset] * 5; /* HDMI Spec requires this clock rate to multiply by 5MHZ */
                  break;
                }
                case 2:
                {
                  if((pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_LATENCY_PRESENT_SHIFT ) & EDID_VENDOR_BLOCK_LATENCY_PRESENT_MASK)
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags |=  EDID_VENDOR_BLOCK_PRESENT_BIT_LATENCY;

                  if((pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_I_LATENCY_PRESENT_SHIFT ) & EDID_VENDOR_BLOCK_I_LATENCY_PRESENT_MASK)
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags |=  EDID_VENDOR_BLOCK_PRESENT_BIT_I_LATENCY;

                  if((pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_VIDEO_PRESENT_SHIFT ) & EDID_VENDOR_BLOCK_VIDEO_PRESENT_MASK)
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags |=  EDID_VENDOR_BLOCK_PRESENT_BIT_HDMI_VIDEO;

                  /* Parse video content Type */
                  pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uSupportedContentFilterMask = ((pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_CONTENT_TYPE_SHIFT ) & EDID_VENDOR_BLOCK_CONTENT_TYPE_MASK);

                  break;
                }
                case 3:
                {
                  /* Check for Progressive latency present bit */
                  if(pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags & EDID_VENDOR_BLOCK_PRESENT_BIT_LATENCY)
                  {
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVideoLatency           = pDataBuf[uWorkingOffset];
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uInterlacedVideoLatency = pDataBuf[uWorkingOffset];
                  }
                  else
                  {
                    /* Check 3D present bit */
                    if((pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_3D_PRESENT_SHIFT ) & EDID_VENDOR_BLOCK_3D_PRESENT_MASK)
                    {
                      pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags |=  EDID_VENDOR_BLOCK_PRESENT_BIT_3D;
                    }
                    
                    /* Parse Image size */
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].eImageSizeInfo = 
                      (EDID_VendorBlockImageSizeInfoType)((pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_IMAGE_INFO_SHIFT ) & EDID_VENDOR_BLOCK_IMAGE_INFO_MASK);

                    /* Parse Multi present field value */
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].u3DMultiPresentFieldVal = 
                      ((pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_3D_MULTI_PRESENT_SHIFT ) & EDID_VENDOR_BLOCK_3D_MULTI_PRESENT_MASK);
                    
                  }
                  break;
                }
                case 4:
                {
                  /* Check for Progressive latency present bit */
                  if(pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags & EDID_VENDOR_BLOCK_PRESENT_BIT_LATENCY)
                  {
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uAudioLatency           = pDataBuf[uWorkingOffset];
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uInterlacedAudioLatency = pDataBuf[uWorkingOffset];
                  }
                  else
                  {
                    /* Extract the HDMI_VIC_LEN and the HDMI 3D LEN fields */
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVideo2DDescLen = 
                      (pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_HDMI_VIC_LEN_SHIFT) & EDID_VENDOR_BLOCK_HDMI_VIC_LEN_MASK;

                    /* Extract the HDMI_VIC_LEN and the HDMI 3D LEN fields */
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVideo3DDescLen = 
                      (pDataBuf[uWorkingOffset] >> EDID_VENDOR_BLOCK_HDMI_3D_LEN_SHIFT) & EDID_VENDOR_BLOCK_HDMI_3D_LEN_MASK;
                  }
                  break;
                }
                case 5:
                {
                  /* Check for Interlace latency present bit */
                  if(pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags & EDID_VENDOR_BLOCK_PRESENT_BIT_I_LATENCY)
                  {
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uInterlacedVideoLatency = pDataBuf[uWorkingOffset];
                  }
                  else
                  {
                    if(pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVideo2DDescLen)
                    {
                      /* Append any extended resolution formats as the supported display modes */
                      EDID_AppendExtendedResolutionDispModes(pEdidSharedData, pDataBuf, uWorkingOffset, pDispModeList, uCEABlockIndex);
                    }
                    if(pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVideo3DDescLen ||
                       pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].u3DMultiPresentFieldVal ||
                      (pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags & EDID_VENDOR_BLOCK_PRESENT_BIT_3D))
                    {
                      // TODO: Append any 3D formats as the supported display modes
                    }
                  }
                  break;
                }
                case 6:
                {
                  /* Check for Interlace latency present bit */
                  if(pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].ePresetBitFlags & EDID_VENDOR_BLOCK_PRESENT_BIT_I_LATENCY)
                  {
                    pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uInterlacedAudioLatency = pDataBuf[uWorkingOffset];
                  }
                  /* All subsequent 3D information will be handled through EDID_Append3DDispModes()
                     At this point we should exit the loop as all vendor specific information is parsed */
                  uWorkingOffset = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uDataBlockLength;
                  break;
                }
              }
              uWorkingOffset++;
              uCounter++;
            }
          }
        }
      }
    }
  }

  return eStatus;
}


//-------------------------------------------------------------------------------------------------
//  EDID_Parser_GetDispMode
//
//  @brief
//      Parses the EDID structure and find out all the supported video modes.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//      [IN] pDataBuf
//          Buffer that contains EDID data.
//      [IN/OUT] pDispModeList
//          Array that stores all the display mode supported.
//      [IN] uNumOfCEABlocks
//          Number of CEA blocks proceeding Block 0.
//      [IN] bShortDesc
//          Short Video descriptor or detailed timing descriptors.
//
//  @return
//      EDID_STATUS_SUCCESS if parsing is successful.
//-------------------------------------------------------------------------------------------------
//
static EDID_Status EDID_Parser_GetDispMode(EDID_ParserContextType *pEdidSharedData, uint8* pDataBuf, EDID_DispModeListType* pDispModeList, uint32 uNumOfCEABlocks, bool32 bShortDesc)
{
  EDID_Status            eStatus        = EDID_STATUS_SUCCESS;
  uint32                uNumOfBlocks   = uNumOfCEABlocks;

  if(NULL == pDataBuf)
  {
     eStatus = EDID_STATUS_BAD_PARAM;
  }
  else 
  {
    // extract the sink capability 
    EDID_Parser_GetSinkCapability(pEdidSharedData);

    // Add the modes as per priority order 

    //Parse the detail timing present in the EDID block
    EDID_Parser_DetailTiming(pEdidSharedData, pDataBuf, pDispModeList);

    // Add CEA block modes
    if(0 < uNumOfBlocks)
    {
      uint32                uNumOfDispMode   = 0;
      uint32                uVideoDataTag    = 0;
      uint32                uWorkingOffset   = 0;
      uint32                uCEABlockIndex   = 0;
      uint32                uCEAExtensionVer = 0;
      bool32                bSkipBlockMap    = FALSE;
      uint32                uDescOffset      = 0;
      uint32                uVideoBlockCount = 0;

      /* Move to the next block if block 2 is a BLOCK MAP */
      if(EDID_EXTENSION_BLOCK_MAP_TAG_CODE == pDataBuf[EDID_CEA_EXTENSION_VERSION_OFFSET - 1])
      {
        bSkipBlockMap = TRUE;

        if(uNumOfBlocks)
        {
          uNumOfBlocks -= 1;
        }  
      }

      /* Iterate through all the CEA extension blocks and extract all applicable display modes */
      for(uCEABlockIndex = 0; uCEABlockIndex < uNumOfBlocks; uCEABlockIndex++)
      {
        if(bSkipBlockMap)
        {
          uCEAExtensionVer = pDataBuf[EDID_CEA_EXTENSION_VERSION_OFFSET + ((uCEABlockIndex + 1) * EDID_BLOCK_SIZE)];
          /* Block 2 is a block map, hence break out of loop when index reaches 1 less the total number of extension blocks */
        }
        else
        {
          uCEAExtensionVer = pDataBuf[EDID_CEA_EXTENSION_VERSION_OFFSET + (uCEABlockIndex * EDID_BLOCK_SIZE)];
        }  

        switch(uCEAExtensionVer)
        {
          case 1:
            break;
          //Obsolete from CEA_861D spec
          case 2:  
            break;
          case 3:
          {
            bShortDesc = TRUE;
            break;
          }
          default:
            break;
        }

        //
        // when usingVersion 3 CEA Extension: All CEA video formats shall be advertised using Short Video Descriptors,  
        // even if they are also advertised using the Detailed Timing Descriptors.
        // Hence parsing of detailed timing descriptor is not required for the extension block 
        //
        if(bShortDesc)
        {
          for(uVideoBlockCount = 0; uVideoBlockCount < EDID_MAX_NUM_OF_VIDEO_DATA_BLOCKS; uVideoBlockCount++)
          {
            uWorkingOffset = pEdidSharedData->sDataBlockDesc.sVideoDataBlockDesc[uCEABlockIndex].uVideoDataBlockOffset[uVideoBlockCount];

            if(uWorkingOffset && (uWorkingOffset < pEdidSharedData->uEDIDDataLength))
            {
              
              // Extract the video data tag code 
              uVideoDataTag = (pDataBuf[uWorkingOffset] & EDID_DATA_BLOCK_TAG_MASK) >> EDID_DATA_BLOCK_TAG_SHIFT;

              // Check whether this block has video data block
              if(EDID_VIDEO_DATA_BLOCK_TAG_CODE == uVideoDataTag)
              {
                uNumOfDispMode = pDataBuf[uWorkingOffset] & EDID_DATA_BLOCK_LENGTH_MASK;

                /* Determine whether the detected mode can be inserted to the display mode list */
                if(uNumOfDispMode)
                {
                  /* Adding short video descriptor to the modes list, YUV420 support is not informed in this block */
                  EDID_InsertShortVideoDesc(pEdidSharedData, pDataBuf, pDispModeList, uNumOfDispMode, uWorkingOffset, EDID_VIDEO_YUV420_NOT_SUPPORTED);
                }  
              }  
            }
            else
            {
              /* Exit loop */
              break;
            }
          }
          
          // Handle YCbCr420 Capability Map data block */
          if ((pEdidSharedData->sDataBlockDesc.sYUV420DataBlockDesc[uCEABlockIndex].uYUV420DataBlockOffset) && ( pEdidSharedData->sDataBlockDesc.sYUV420DataBlockDesc[uCEABlockIndex].uDataBlockLength))
          {
            // TODO
          }

          /* Handle YCbCr420 Video Data block*/
          if ((pEdidSharedData->sDataBlockDesc.sYUV420DataBlockDesc[uCEABlockIndex].uYUV420DataBlockOffset) && ( pEdidSharedData->sDataBlockDesc.sYUV420DataBlockDesc[uCEABlockIndex].uDataBlockLength))
          {
            uWorkingOffset = pEdidSharedData->sDataBlockDesc.sYUV420DataBlockDesc[uCEABlockIndex].uYUV420DataBlockOffset + 1;
 
            // Next byte contain the extension code
            if (EDID_EXTENDED_YUV420_VIDEO_BLOCK_TAG_CODE == pDataBuf[uWorkingOffset])
            {
              // Num of modes is equal to bytes after the extension tag code byte
              uNumOfDispMode = pEdidSharedData->sDataBlockDesc.sYUV420DataBlockDesc[uCEABlockIndex].uDataBlockLength - 1 ; 

              /* This block lists the additional modes that uses YUV420 sampling mode only */
              if (uNumOfDispMode)
              {
                /* Adding short video descriptor to the modes list, audio support based on  audio bcpas read from the extension block */
                EDID_InsertShortVideoDesc(pEdidSharedData, pDataBuf, pDispModeList, uNumOfDispMode, uWorkingOffset, EDID_VIDEO_YUV420_ONLY_SUPPORTED);
              }
            }
          }
        }
        //
        // When using CEA Extension Version 1, all of the CEA video formats listed in E-EDID are described using
        // Detailed Timing Descriptors 
        //
        else
        {
          // Parse the CEA blocks accordingly for the detail timing descriptors 
          if(pEdidSharedData->sDataBlockDesc.sDetailTimingBlockDesc[uCEABlockIndex].uDetailTimingBlockOffset)
          {
            /* Reset counting index */
            uint32   uI                            = 0;
            bool32   bExitLoop                     = FALSE;
            EDID_VideoFormatType  eVideoFormat     = EDID_VIDEO_FORMAT_640x480p60_4_3; 

            while(!bExitLoop)
            {
              uDescOffset = pEdidSharedData->sDataBlockDesc.sDetailTimingBlockDesc[uCEABlockIndex].uDetailTimingBlockOffset + (uI * EDID_DETAIL_TIMING_DESC_BLOCK_SIZE);

              if((uDescOffset < pEdidSharedData->uEDIDDataLength) && pDataBuf[uDescOffset])
              {
                if (EDID_STATUS_SUCCESS == (eStatus = EDID_Parser_Detail_Desc(pEdidSharedData, &pDataBuf[uDescOffset], &eVideoFormat)))
                {                  
                  (void) EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, eVideoFormat, EDID_VIDEO_YUV420_NOT_SUPPORTED);
                }

                // Increment to the next detail timing descriptor 
                uI++;
              }
              else
              {
                bExitLoop = TRUE;
              }
            }
          }
        }
      }    
    }    
    // Handle modes in the standard timing section
    EDID_Parser_StandardTimings(pEdidSharedData, pDataBuf, EDID_STANDARD_TIMING_START_OFFSET, EDID_STANDARD_TIMING_END_OFFSET, pDispModeList);

    // Handle modes in the established timing section
    EDID_Parser_EstablishedTimings(pEdidSharedData, pDataBuf, pDispModeList);

    if (uNumOfCEABlocks == 0) 
    { 
        EDID_Inject_CEA_Mode(pEdidSharedData, pDispModeList);
    }

    //Need to add default 640 by 480 timings, in case not described in the EDID structure or CEA extension block.
    //All DTV sink devices should support this mode
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_640x480p60_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }
  return eStatus;
}


#ifdef __cplusplus
}
#endif


