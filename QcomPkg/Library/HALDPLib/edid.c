/*=============================================================================
 
  File: edid.c
 
  EDID functions and parser utilities 
  
 
  Copyright (c) 2010-2020 Qualcomm Technologies, Inc.  
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

/* Calculates the number of elements of an array */
#define NUMBER_OF_ELEMENTS(arr)     (sizeof(arr) / sizeof(arr[0]))

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
  EDID_VIDEO_ASPECT_RATIO_1_1,
  EDID_VIDEO_ASPECT_RATIO_4_3,
  EDID_VIDEO_ASPECT_RATIO_16_9,
  EDID_VIDEO_ASPECT_RATIO_5_4,  
  EDID_VIDEO_ASPECT_RATIO_16_10,  
  EDID_VIDEO_ASPECT_RATIO_9_5,
  EDID_VIDEO_ASPECT_RATIO_5_3,
  EDID_VIDEO_ASPECT_RATIO_7_3,
  EDID_VIDEO_ASPECT_RATIO_53_30,
  EDID_VIDEO_ASPECT_RATIO_64_27,
  EDID_VIDEO_ASPECT_RATIO_64_35,
  EDID_VIDEO_ASPECT_RATIO_85_48,
  EDID_VIDEO_ASPECT_RATIO_256_135,
  EDID_VIDEO_ASPECT_RATIO_683_384,
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

//-------------------------------------------------------------------------------------------------
//  EDID_Standard_Type
//
//  @brief
//      Enum indicating EDID standard. 
//
//-------------------------------------------------------------------------------------------------
//
typedef enum
{
  EDID_Standard_CEA                       = 0,   /**< CEA Standard */
  EDID_Standard_VESA                             /**< VESA Standard */
} EDID_Standard_Type;


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
  uint32                    uTotalBlankH;         /**< Horizontal blanking pixels */
  bool32                    bActiveLowH;          /**< Horizontal sync polarity */
  uint32                    uActiveV;             /**< Vertical active pixels */
  uint32                    uFrontPorchV;         /**< Vertical front porch pixels */
  uint32                    uPulseWidthV;         /**< Vertical sync pulse width pixels */
  uint32                    uBackPorchV;          /**< Vertical back porch pixels */
  uint32                    uTotalBlankV;         /**< Vertical blanking pixels */
  bool32                    bActiveLowV;          /**< Vertical sync polarity */
  uint32                    uPixelFreq;           /**< Must divide by 1000 to get the actual frequency in MHZ */
  uint32                    uRefreshRate;         /**< Must divide by 1000 to get the actual frequency in HZ */
  bool32                    bInterlaced;          /**< Indicates format is interlaced or not */
  EDID_SpecificationType    eEDIDSpecType;        /**< HDMI/DP specification type */
  uint32                    uPixelRepeatFactor;   /**< Pixel repetition factor denoted as 2^n where is n is the repetition factor */
  EDID_Video_AspectRatio    eAspectRatio;         /**< Aspect ratio of the format */
  uint32                    uVicId;                /**< VIC of CEA blocks */
  EDID_Standard_Type        eStandard;            /**< Indicates the EDID standard */
} EDID_DispModeTimingType;


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
  bool32                    bEmbeddedDisplay;                                     /**< Embedded display  */
  EDID_SpecificationType    eEDIDSpecType;                                        /**< HDMI/DP Specification type */
  uint32                    uNumOfCEABlocks;                                      /**< Number of CEA blocks */
} EDID_ParserContextType;


/* -----------------------------------------------------------------------
** Defines
** ----------------------------------------------------------------------- */

// Maximum number of EDID parser contexts
#define EDID_MAX_CTX_NUM                                      1

#define EDID_STANDARD_TIMING_FREQ_DELTA                       2           // Maximum acceptable delta in refresh rate (in hertz)

// Indicates that the mode is supported by all specifications
#define EDID_SPEC_ALL                                         (EDID_HDMI_SPEC_TYPE_1_4 |  \
                                                               EDID_HDMI_SPEC_TYPE_2_0 |  \
                                                               EDID_DP_SPEC_TYPE_1_3)

// Indicates that the mode is supported by HDMI2.0 and DP1.3 specifications
#define EDID_SPEC_HDMI2P0_DP1P3                               (EDID_HDMI_SPEC_TYPE_2_0 |  \
                                                               EDID_DP_SPEC_TYPE_1_3)

// Minimum and Maximum VIC id, currently it is from 1 to 219 on the spec
#define EDID_MIN_VIC_ID                                       1
#define EDID_MAX_VIC_ID                                       219


// EDID context containing EDID parsing info
EDID_ParserContextType gEDIDSharedCtx[EDID_MAX_CTX_NUM];

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
  {EDID_VIDEO_FORMAT_640x480p60_4_3,        640,     16,      96,      48,      160,   TRUE,  480,     10,      2,       33,      45,    TRUE,  25174,   59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     1,       EDID_Standard_CEA}, // 2:SD 640 x 480p59.94
  {EDID_VIDEO_FORMAT_640x480p60_4_3,        640,     16,      96,      48,      160,   TRUE,  480,     10,      2,       33,      45,    TRUE,  25175,   59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 63:640 x 480p59.94
  {EDID_VIDEO_FORMAT_640x480p60_4_3,        640,     16,      96,      48,      160,   TRUE,  480,     10,      2,       33,      45,    TRUE,  25200,   60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     1,       EDID_Standard_CEA}, // 1:SD 640 x 480p60
//{EDID_VIDEO_FORMAT_640x350p85_64_35,      640,     32,      64,      96,      192,   FALSE, 350,     32,      3,       60,      95,    TRUE,  31500,   85080,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_35,   0,       EDID_Standard_VESA}, // 60:640 x 350p85
//{EDID_VIDEO_FORMAT_640x400p85_16_10,      640,     32,      64,      96,      192,   TRUE,  400,     1,       3,       41,      45,    FALSE, 31500,   85080,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 61:640 x 400p85
  {EDID_VIDEO_FORMAT_640x480p73_4_3,        640,     24,      40,      128,     192,   TRUE,  480,     9,       3,       28,      40,    TRUE,  31500,   72809,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 64:640 x 480p72
  {EDID_VIDEO_FORMAT_640x480p75_4_3,        640,     16,      64,      120,     200,   TRUE,  480,     1,       3,       16,      20,    TRUE,  31500,   75000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 65:640 x 480p75
//{EDID_VIDEO_FORMAT_640x480p85_4_3,        640,     56,      56,      80,      192,   TRUE,  480,     1,       3,       25,      29,    TRUE,  36000,   85008,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 66:640 x 480p85
  {EDID_VIDEO_FORMAT_720x576p50_5_4,        720,     12,      64,      68,      144,   TRUE,  576,     5,       5,       39,      49,    TRUE,  27000,   50000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_4,     17,      EDID_Standard_CEA}, // 3:SD 720 x 576p50
  {EDID_VIDEO_FORMAT_720x480p60_3_2,        720,     16,      62,      60,      138,   TRUE,  480,     9,       6,       30,      45,    TRUE,  27000,   59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_3_2,     2,       EDID_Standard_CEA}, // 5:SD 720 x 480p59.94
  {EDID_VIDEO_FORMAT_720x480p60_3_2,        720,     16,      62,      60,      138,   TRUE,  480,     9,       6,       30,      45,    TRUE,  27026,   60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_3_2,     2,       EDID_Standard_CEA}, // 4:SD 720 x 480p60
//{EDID_VIDEO_FORMAT_720x400p85_9_5,        720,     36,      72,      108,     216,   TRUE,  400,     1,       3,       42,      46,    FALSE, 35500,   85039,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_9_5,     0,       EDID_Standard_VESA}, // 62:720 x 400p85
  {EDID_VIDEO_FORMAT_800x600p56_4_3,        800,     24,      72,      128,     224,   FALSE, 600,     1,       2,       22,      25,    FALSE, 36000,   56250,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 67:800 x 600p56
  {EDID_VIDEO_FORMAT_800x600p60_4_3,        800,     40,      128,     88,      256,   FALSE, 600,     1,       4,       23,      28,    FALSE, 40000,   60317,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 68:800 x 600p60
  {EDID_VIDEO_FORMAT_800x600p75_4_3,        800,     16,      80,      160,     256,   FALSE, 600,     1,       3,       21,      25,    FALSE, 49500,   75000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 70:800 x 600p75
  {EDID_VIDEO_FORMAT_800x600p72_4_3,        800,     56,      120,     64,      240,   FALSE, 600,     37,      6,       23,      66,    FALSE, 50000,   72188,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 69:800 x 600p72
//{EDID_VIDEO_FORMAT_800x600p85_4_3,        800,     32,      64,      152,     248,   FALSE, 600,     1,       3,       27,      31,    FALSE, 56250,   85061,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 71:800 x 600p85
//{EDID_VIDEO_FORMAT_800x600p120_4_3,       800,     48,      32,      80,      160,   FALSE, 600,     3,       4,       29,      36,    TRUE,  73250,   119972,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 72:800 x 600p120 RB1
  {EDID_VIDEO_FORMAT_848x480p60_53_30,      848,     16,      112,     112,     240,   FALSE, 480,     6,       8,       23,      37,    FALSE, 33750,   60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_53_30,   0,       EDID_Standard_VESA}, // 73:848 x 480p60
  {EDID_VIDEO_FORMAT_1024x768p60_4_3,       1024,    24,      136,     160,     320,   TRUE,  768,     3,       6,       29,      38,    TRUE,  65000,   60004,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 74:1024 x 768p60
  {EDID_VIDEO_FORMAT_1024x768p70_4_3,       1024,    24,      136,     144,     304,   TRUE,  768,     3,       6,       29,      38,    TRUE,  75000,   70069,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 75:1024 x 768p70
  {EDID_VIDEO_FORMAT_1024x768p75_4_3,       1024,    16,      96,      176,     288,   FALSE, 768,     1,       3,       28,      32,    FALSE, 78750,   75029,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 76:1024 x 768p75
//{EDID_VIDEO_FORMAT_1024x768p85_4_3,       1024,    48,      96,      208,     352,   FALSE, 768,     1,       3,       36,      40,    FALSE, 94500,   84997,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 77:1024 x 768p85
//{EDID_VIDEO_FORMAT_1024x768p120_4_3,      1024,    48,      32,      80,      160,   FALSE, 768,     3,       4,       38,      45,    TRUE,  115500,  119989,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 78:1024 x 768p120 RB1
  {EDID_VIDEO_FORMAT_1152x864p75_4_3,       1152,    64,      128,     256,     448,   FALSE, 864,     1,       3,       32,      36,    FALSE, 108000,  75000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 79:1152 x 864p75
  {EDID_VIDEO_FORMAT_1280x720p24_16_9,      1280,    1760,    40,      220,     2020,  FALSE, 720,     5,       5,       20,      30,    FALSE, 59340,   23976,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    60,      EDID_Standard_CEA}, // 7:HD 720p24
  {EDID_VIDEO_FORMAT_1280x720p24_16_9,      1280,    1760,    40,      220,     2020,  FALSE, 720,     5,       5,       20,      30,    FALSE, 59400,   24000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    60,      EDID_Standard_CEA}, // 6:HD 720p24
  {EDID_VIDEO_FORMAT_1280x768p60_5_3,       1280,    48,      32,      80,      160,   FALSE, 768,     3,       7,       12,      22,    TRUE,  68250,   59995,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_3,     0,       EDID_Standard_VESA}, // 81:1280 x 768p60 RB1
  {EDID_VIDEO_FORMAT_1280x800p60_16_10,     1280,    48,      32,      80,      160,   FALSE, 800,     3,       6,       14,      23,    TRUE,  71000,   59910,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 86:1280 x 800p60 RB1
  {EDID_VIDEO_FORMAT_1280x720p30_16_9,      1280,    1760,    40,      220,     2020,  FALSE, 720,     5,       5,       20,      30,    FALSE, 74175,   29970,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    62,      EDID_Standard_CEA}, // 10:HD 720p30
  {EDID_VIDEO_FORMAT_1280x720p60_16_9,      1280,    110,     40,      220,     370,   FALSE, 720,     5,       5,       20,      30,    FALSE, 74175,   59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    4,       EDID_Standard_CEA}, // 13:HD 720p60
  {EDID_VIDEO_FORMAT_1280x720p25_16_9,      1280,    2420,    40,      220,     2680,  FALSE, 720,     5,       5,       20,      30,    FALSE, 74250,   25000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    61,      EDID_Standard_CEA}, // 8:HD 720p25
  {EDID_VIDEO_FORMAT_1280x720p30_16_9,      1280,    1760,    40,      220,     2020,  FALSE, 720,     5,       5,       20,      30,    FALSE, 74250,   30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    62,      EDID_Standard_CEA}, // 9:HD 720p30
  {EDID_VIDEO_FORMAT_1280x720p50_16_9,      1280,    440,     40,      220,     700,   FALSE, 720,     5,       5,       20,      30,    FALSE, 74250,   50000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    19,      EDID_Standard_CEA}, // 11:HD 720p50
  {EDID_VIDEO_FORMAT_1280x720p60_16_9,      1280,    110,     40,      220,     370,   FALSE, 720,     5,       5,       20,      30,    FALSE, 74250,   60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    4,       EDID_Standard_CEA}, // 12:HD 720p60
  {EDID_VIDEO_FORMAT_1280x720p60_16_9,      1280,    110,     40,      220,     370,   FALSE, 720,     5,       5,       20,      30,    FALSE, 74250,   60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    85,      EDID_Standard_CEA}, // 80:1280 x 720p60
  {EDID_VIDEO_FORMAT_1280x768p60_5_3,       1280,    64,      128,     192,     384,   TRUE,  768,     3,       7,       20,      30,    FALSE, 79500,   59870,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_3,     0,       EDID_Standard_VESA}, // 82:1280 x 768p60
  {EDID_VIDEO_FORMAT_1280x800p60_16_10,     1280,    72,      128,     200,     400,   TRUE,  800,     3,       6,       22,      31,    FALSE, 83500,   59810,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 87:M15 -1280x800p60 -Audio
  {EDID_VIDEO_FORMAT_1280x768p75_5_3,       1280,    80,      128,     208,     416,   TRUE,  768,     3,       7,       27,      37,    FALSE, 102250,  74893,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_3,     0,       EDID_Standard_VESA}, // 83:1280 x 768p75
  {EDID_VIDEO_FORMAT_1280x800p75_16_10,     1280,    80,      128,     208,     416,   TRUE,  800,     3,       6,       29,      38,    FALSE, 106500,  74934,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 88:1280 x 800p75
  {EDID_VIDEO_FORMAT_1280x960p60_4_3,       1280,    96,      112,     312,     520,   FALSE, 960,     1,       3,       36,      40,    FALSE, 108000,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 91:1280 x 960p60
  {EDID_VIDEO_FORMAT_1280x1024p60_5_4,      1280,    48,      112,     248,     408,   FALSE, 1024,    1,       3,       38,      42,    FALSE, 108000,  60020,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_4,     0,       EDID_Standard_VESA}, // 94:1280 x 1024p60
//{EDID_VIDEO_FORMAT_1280x768p85_5_3,       1280,    80,      136,     216,     432,   TRUE,  768,     3,       7,       31,      41,    FALSE, 117500,  84837,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_3,     0,       EDID_Standard_VESA}, // 84:1280 x 768p85
//{EDID_VIDEO_FORMAT_1280x800p85_16_10,     1280,    80,      136,     216,     432,   TRUE,  800,     3,       6,       34,      43,    FALSE, 122500,  84880,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 89:1280 x 800p85
  {EDID_VIDEO_FORMAT_1280x1024p75_5_4,      1280,    16,      144,     248,     408,   FALSE, 1024,    1,       3,       38,      42,    FALSE, 135000,  75025,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_4,     0,       EDID_Standard_VESA}, // 95:1280 x 1024p75
//{EDID_VIDEO_FORMAT_1280x768p120_5_3,      1280,    48,      32,      80,      160,   FALSE, 768,     3,       7,       35,      45,    TRUE,  140250,  119798,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_3,     0,       EDID_Standard_VESA}, // 85:1280 x 768p120 RB1
//{EDID_VIDEO_FORMAT_1280x800p120_16_10,    1280,    48,      32,      80,      160,   FALSE, 800,     3,       6,       38,      47,    TRUE,  146250,  119909,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 90:1280 x 800p120 RB1
//{EDID_VIDEO_FORMAT_1280x960p85_4_3,       1280,    64,      160,     224,     448,   FALSE, 960,     1,       3,       47,      51,    FALSE, 148500,  85002,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 92:1280 x 960p85
//{EDID_VIDEO_FORMAT_1280x1024p85_5_4,      1280,    64,      160,     224,     448,   FALSE, 1024,    1,       3,       44,      48,    FALSE, 157500,  85024,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_4,     0,       EDID_Standard_VESA}, // 96:1280 x 1024p85
//{EDID_VIDEO_FORMAT_1280x960p120_4_3,      1280,    48,      32,      80,      160,   FALSE, 960,     3,       4,       50,      57,    TRUE,  175500,  119838,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 93:1280 x 960p120 RB1
//{EDID_VIDEO_FORMAT_1280x1024p120_5_4,     1280,    48,      32,      80,      160,   FALSE, 1024,    3,       7,       50,      60,    TRUE,  187250,  119958,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_5_4,     0,       EDID_Standard_VESA}, // 97:1280 x 1024p120 RB1
  {EDID_VIDEO_FORMAT_1360x768p60_85_48,     1360,    64,      112,     256,     432,   FALSE, 768,     3,       6,       18,      27,    FALSE, 85500,   60015,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_85_48,   0,       EDID_Standard_VESA}, // 98:1360 x 768p60
//{EDID_VIDEO_FORMAT_1360x768p120_85_48,    1360,    48,      32,      80,      160,   FALSE, 768,     3,       5,       37,      45,    TRUE,  148250,  119967,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_85_48,   0,       EDID_Standard_VESA}, // 99:1360 x 768p120 RB1
  {EDID_VIDEO_FORMAT_1366x768p60_683_384,   1366,    14,      56,      64,      134,   FALSE, 768,     1,       3,       28,      32,    FALSE, 72000,   60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_683_384, 0,       EDID_Standard_VESA}, // 101:M3 -1368x768p60 -Audio
  {EDID_VIDEO_FORMAT_1366x768p60_683_384,   1366,    70,      143,     213,     426,   FALSE, 768,     3,       3,       24,      30,    FALSE, 85500,   59790,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_683_384, 0,       EDID_Standard_VESA}, // 100:1360 x 768p59.79
  {EDID_VIDEO_FORMAT_1400x1050p60_4_3,      1400,    48,      32,      80,      160,   FALSE, 1050,    3,       4,       23,      30,    TRUE,  101000,  59948,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 102:1400 x 1050p60 RB1
  {EDID_VIDEO_FORMAT_1400x1050p60_4_3,      1400,    88,      144,     232,     464,   TRUE,  1050,    3,       4,       32,      39,    FALSE, 121750,  59978,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 103:1400 x 1050p60
  {EDID_VIDEO_FORMAT_1400x1050p75_4_3,      1400,    104,     144,     248,     496,   TRUE,  1050,    3,       4,       42,      49,    FALSE, 156000,  74867,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 104:1400 x 1050p75
//{EDID_VIDEO_FORMAT_1400x1050p85_4_3,      1400,    104,     152,     256,     512,   TRUE,  1050,    3,       4,       48,      55,    FALSE, 179500,  84960,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 105:1400 x 1050p85
//{EDID_VIDEO_FORMAT_1400x1050p120_4_3,     1400,    48,      32,      80,      160,   FALSE, 1050,    3,       4,       55,      62,    TRUE,  208000,  119904,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 106:1400 x 1050p120 RB1
  {EDID_VIDEO_FORMAT_1440x900p60_16_10,     1440,    48,      32,      80,      160,   FALSE, 900,     3,       6,       17,      26,    TRUE,  88750,   59901,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 107:1440 x 900p60 RB1
  {EDID_VIDEO_FORMAT_1440x900p60_16_10,     1440,    80,      152,     232,     464,   TRUE,  900,     3,       6,       25,      34,    FALSE, 106500,  59887,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 108:M4 -1440 x 900p60 -Audio
  {EDID_VIDEO_FORMAT_1440x900p75_16_10,     1440,    96,      152,     248,     496,   TRUE,  900,     3,       6,       33,      42,    FALSE, 136750,  74984,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 109:1440 x 900p75
//{EDID_VIDEO_FORMAT_1440x900p85_16_10,     1440,    104,     152,     256,     512,   TRUE,  900,     3,       6,       39,      48,    FALSE, 157000,  84842,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 110:1440 x 900p85
//{EDID_VIDEO_FORMAT_1440x900p120_16_10,    1440,    48,      32,      80,      160,   FALSE, 900,     3,       6,       44,      53,    TRUE,  182750,  119852,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 111:1440 x 900p120 RB1
  {EDID_VIDEO_FORMAT_1600x900p60_16_9,      1600,    24,      80,      96,      200,   FALSE, 900,     1,       3,       96,      100,   FALSE, 108000,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 112:M2 -1600x900p60 -Audio
  {EDID_VIDEO_FORMAT_1600x1200p60_4_3,      1600,    64,      192,     304,     560,   FALSE, 1200,    1,       3,       46,      50,    FALSE, 162000,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 113:1600 x 1200p60
  {EDID_VIDEO_FORMAT_1600x1200p65_4_3,      1600,    64,      192,     304,     560,   FALSE, 1200,    1,       3,       46,      50,    FALSE, 175500,  65000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 114:1600 x 1200p65
  {EDID_VIDEO_FORMAT_1600x1200p70_4_3,      1600,    64,      192,     304,     560,   FALSE, 1200,    1,       3,       46,      50,    FALSE, 189000,  70000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 115:1600 x 1200p70
  {EDID_VIDEO_FORMAT_1600x1200p75_4_3,      1600,    64,      192,     304,     560,   FALSE, 1200,    1,       3,       46,      50,    FALSE, 202500,  75000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 116:1600 x 1200p75
//{EDID_VIDEO_FORMAT_1600x1200p85_4_3,      1600,    64,      192,     304,     560,   FALSE, 1200,    1,       3,       46,      50,    FALSE, 229500,  85000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 117:1600 x 1200p85
//{EDID_VIDEO_FORMAT_1600x1200p120_4_3,     1600,    48,      32,      80,      160,   FALSE, 1200,    3,       4,       64,      71,    TRUE,  268250,  119917,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 118:1600 x 1200p120 RB1
  {EDID_VIDEO_FORMAT_1680x720p24_7_3,       1680,    1360,    40,      220,     1620,  FALSE, 720,     5,       5,       20,      30,    FALSE, 59340,   23976,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_7_3,     79,      EDID_Standard_CEA}, // 15:21:9 HD 24p
  {EDID_VIDEO_FORMAT_1680x720p30_7_3,       1680,    700,     40,      220,     960,   FALSE, 720,     5,       5,       20,      30,    FALSE, 59340,   29970,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_7_3,     81,      EDID_Standard_CEA}, // 18:21:9 HD 30p
  {EDID_VIDEO_FORMAT_1680x720p24_7_3,       1680,    1360,    40,      220,     1620,  FALSE, 720,     5,       5,       20,      30,    FALSE, 59400,   24000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_7_3,     79,      EDID_Standard_CEA}, // 14:21:9 HD 24p
  {EDID_VIDEO_FORMAT_1680x720p25_7_3,       1680,    1228,    40,      220,     1488,  FALSE, 720,     5,       5,       20,      30,    FALSE, 59400,   25000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_7_3,     80,      EDID_Standard_CEA}, // 16:21:9 HD 25p
  {EDID_VIDEO_FORMAT_1680x720p30_7_3,       1680,    700,     40,      220,     960,   FALSE, 720,     5,       5,       20,      30,    FALSE, 59400,   30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_7_3,     81,      EDID_Standard_CEA}, // 17:21:9 HD 30p
  {EDID_VIDEO_FORMAT_1680x720p50_7_3,       1680,    260,     40,      220,     520,   FALSE, 720,     5,       5,       20,      30,    FALSE, 82500,   50000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_7_3,     82,      EDID_Standard_CEA}, // 19:21:9 HD 50p
  {EDID_VIDEO_FORMAT_1680x720p60_7_3,       1680,    260,     40,      220,     520,   FALSE, 720,     5,       5,       20,      30,    FALSE, 98901,   59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_7_3,     83,      EDID_Standard_CEA}, // 21:21:9 HD 60p
  {EDID_VIDEO_FORMAT_1680x720p60_7_3,       1680,    260,     40,      220,     520,   FALSE, 720,     5,       5,       20,      30,    FALSE, 99000,   60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_7_3,     83,      EDID_Standard_CEA}, // 20:21:9 HD 60p
  {EDID_VIDEO_FORMAT_1680x1050p60_16_10,    1680,    48,      32,      80,      160,   FALSE, 1050,    3,       6,       21,      30,    TRUE,  119000,  59883,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 119:1680 x 1050p60 RB1
  {EDID_VIDEO_FORMAT_1680x1050p60_16_10,    1680,    104,     176,     280,     560,   TRUE,  1050,    3,       6,       30,      39,    FALSE, 146250,  59954,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 120:M8 -1680x1050p60 -Audio
  {EDID_VIDEO_FORMAT_1680x1050p75_16_10,    1680,    120,     176,     296,     592,   TRUE,  1050,    3,       6,       40,      49,    FALSE, 187000,  74892,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 121:1680 x 1050p75
//{EDID_VIDEO_FORMAT_1680x1050p85_16_10,    1680,    128,     176,     304,     608,   TRUE,  1050,    3,       6,       46,      55,    FALSE, 214750,  84941,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 122:1680 x 1050p85
//{EDID_VIDEO_FORMAT_1680x1050p120_16_10,   1680,    48,      32,      80,      160,   FALSE, 1050,    3,       6,       53,      62,    TRUE,  245500,  119986,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 123:1680 x 1050p120 RB1
  {EDID_VIDEO_FORMAT_1792x1344p60_4_3,      1792,    128,     200,     328,     656,   TRUE,  1344,    1,       3,       46,      50,    FALSE, 204750,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 124:1792 x 1344p60
  {EDID_VIDEO_FORMAT_1792x1344p75_4_3,      1792,    96,      216,     352,     664,   TRUE,  1344,    1,       3,       69,      73,    FALSE, 261000,  74997,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 125:1792 x 1344p75
//{EDID_VIDEO_FORMAT_1792x1344p120_4_3,     1792,    48,      32,      80,      160,   FALSE, 1344,    3,       4,       72,      79,    TRUE,  333250,  119974,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 126:1792 x 1344p120 RB1
  {EDID_VIDEO_FORMAT_1856x1392p60_4_3,      1856,    96,      224,     352,     672,   TRUE,  1392,    1,       3,       43,      47,    FALSE, 218250,  59995,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 127:1856 x 1392p60
  {EDID_VIDEO_FORMAT_1856x1392p75_4_3,      1856,    128,     224,     352,     704,   TRUE,  1392,    1,       3,       104,     108,   FALSE, 288000,  75000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 128:1856 x 1392p75
//{EDID_VIDEO_FORMAT_1856x1392p120_4_3,     1856,    48,      32,      80,      160,   FALSE, 1392,    3,       4,       75,      82,    TRUE,  356500,  119970,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 129:1856 x 1392p120 RB1
  {EDID_VIDEO_FORMAT_1920x1080p24_16_9,     1920,    638,     44,      148,     830,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 74175,   23976,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    32,      EDID_Standard_CEA}, // 23:FHD 24p
  {EDID_VIDEO_FORMAT_1920x1080p30_16_9,     1920,    88,      44,      148,     280,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 74175,   29970,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    34,      EDID_Standard_CEA}, // 26:FHD 30p
  {EDID_VIDEO_FORMAT_1920x1080p24_16_9,     1920,    638,     44,      148,     830,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 74250,   24000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    32,      EDID_Standard_CEA}, // 22:FHD 24p
  {EDID_VIDEO_FORMAT_1920x1080p25_16_9,     1920,    528,     44,      148,     720,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 74250,   25000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    33,      EDID_Standard_CEA}, // 24:FHD 25p
  {EDID_VIDEO_FORMAT_1920x1080p30_16_9,     1920,    88,      44,      148,     280,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 74250,   30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    34,      EDID_Standard_CEA}, // 25:FHD 30p
  {EDID_VIDEO_FORMAT_1920x1080p60_16_9,     1920,    88,      44,      148,     280,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 148351,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    16,      EDID_Standard_CEA}, // 29:FHD 60p
  {EDID_VIDEO_FORMAT_1920x1080p50_16_9,     1920,    528,     44,      148,     720,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 148500,  50000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    31,      EDID_Standard_CEA}, // 27:FHD 50p
  {EDID_VIDEO_FORMAT_1920x1080p60_16_9,     1920,    88,      44,      148,     280,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 148500,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    16,      EDID_Standard_CEA}, // 28:FHD 60p
  {EDID_VIDEO_FORMAT_1920x1080p60_16_9,     1920,    88,      44,      148,     280,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 148500,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    82,      EDID_Standard_CEA}, // 130:1920 x1080p60
  {EDID_VIDEO_FORMAT_1920x1200p60_16_10,    1920,    48,      32,      80,      160,   FALSE, 1200,    3,       6,       26,      35,    TRUE,  154000,  59950,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 131:1920 x 1200p60 RB1
  {EDID_VIDEO_FORMAT_1920x1200p60_16_10,    1920,    136,     200,     336,     672,   TRUE,  1200,    3,       6,       36,      45,    FALSE, 193250,  59885,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 132:M7 -1920x1200p60 -Audio
  {EDID_VIDEO_FORMAT_1920x1440p60_4_3,      1920,    128,     208,     344,     680,   TRUE,  1440,    1,       3,       56,      60,    FALSE, 234000,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 136:1920 x 1440p60
  {EDID_VIDEO_FORMAT_1920x1200p75_16_10,    1920,    136,     208,     344,     688,   TRUE,  1200,    3,       6,       46,      55,    FALSE, 245250,  74930,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 133:1920 x 1200p75
//{EDID_VIDEO_FORMAT_1920x1080p120_16_9,    1920,    8,       32,      40,      80,    FALSE, 1080,    50,      8,       6,       64,    TRUE,  274560,  120000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 148:1920 x 1080p120 RB2
//{EDID_VIDEO_FORMAT_1920x1200p85_16_10,    1920,    144,     208,     352,     704,   TRUE,  1200,    3,       6,       53,      62,    FALSE, 281250,  84932,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 134:1920 x 1200p85
//{EDID_VIDEO_FORMAT_1920x1080p120_16_9,    1920,    48,      32,      80,      160,   FALSE, 1080,    3,       5,       56,      64,    TRUE,  285500,  119982,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 166:1920 x 1080p120 RB1
//{EDID_VIDEO_FORMAT_1920x1080p120_16_9,    1920,    88,      44,      148,     280,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 296703,  119880,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    63,      EDID_Standard_CEA}, // 32:FHD 120p
//{EDID_VIDEO_FORMAT_1920x1080p100_16_9,    1920,    528,     44,      148,     720,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 297000,  100000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    64,      EDID_Standard_CEA}, // 30:FHD 100p
//{EDID_VIDEO_FORMAT_1920x1080p120_16_9,    1920,    88,      44,      148,     280,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 297000,  120000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    63,      EDID_Standard_CEA}, // 31:FHD 120p
  {EDID_VIDEO_FORMAT_1920x1440p75_4_3,      1920,    144,     224,     352,     720,   TRUE,  1440,    1,       3,       56,      60,    FALSE, 297000,  75000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 137:1920 x 1440p75
//{EDID_VIDEO_FORMAT_1920x1200p120_16_10,   1920,    48,      32,      80,      160,   FALSE, 1200,    3,       6,       62,      71,    TRUE,  317000,  119909,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 135:1920 x 1200p120 RB1
//{EDID_VIDEO_FORMAT_1920x1080p144_16_9,    1920,    8,       32,      40,      80,    FALSE, 1080,    63,      8,       6,       77,    TRUE,  333216,  144000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 149:1920 x 1080p144 RB2
//{EDID_VIDEO_FORMAT_1920x1080p144_16_9,    1920,    48,      32,      80,      160,   FALSE, 1080,    3,       5,       69,      77,    TRUE,  346500,  143981,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 167:1920 x 1080p144 RB1
//{EDID_VIDEO_FORMAT_1920x1080p120_16_9,    1920,    160,     208,     368,     736,   TRUE,  1080,    3,       5,       72,      80,    FALSE, 369500,  119930,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 161:1920 x 1080p120 Non-RB
//{EDID_VIDEO_FORMAT_1920x1440p120_4_3,     1920,    48,      32,      80,      160,   FALSE, 1440,    3,       4,       78,      85,    TRUE,  380500,  119956,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_4_3,     0,       EDID_Standard_VESA}, // 138:1920 x 1440p120 RB1
//{EDID_VIDEO_FORMAT_1920x1080p144_16_9,    1920,    168,     208,     376,     752,   TRUE,  1080,    3,       5,       89,      97,    FALSE, 452500,  143882,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 162:1920 x 1080p144 Non-RB
  {EDID_VIDEO_FORMAT_2048x1152p60_16_9,     2048,    26,      80,      96,      202,   FALSE, 1152,    1,       3,       44,      48,    FALSE, 162000,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 139:2048 x 1152p60
//{EDID_VIDEO_FORMAT_2048x2048p120_1_1,     2048,    8,       32,      40,      80,    FALSE, 2048,    106,     8,       6,       120,   TRUE,  553620,  120000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_1_1,     0,       EDID_Standard_VESA}, // 190:2048 x 2048p120 RB2
//{EDID_VIDEO_FORMAT_2048x2048p120_1_1,     2048,    48,      32,      80,      160,   FALSE, 2048,    3,       10,      107,     120,   TRUE,  574250,  119962,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_1_1,     0,       EDID_Standard_VESA}, // 191:2048 x 2048p120 RB1
  {EDID_VIDEO_FORMAT_2560x1080p25_64_27,    2560,    448,     44,      148,     640,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 90000,   25000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   87,      EDID_Standard_CEA}, // 35:21:9 FHD 25p
  {EDID_VIDEO_FORMAT_2560x1080p24_64_27,    2560,    998,     44,      148,     1190,  FALSE, 1080,    4,       5,       11,      20,    FALSE, 98901,   23976,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   86,      EDID_Standard_CEA}, // 34:21:9 FHD 24p
  {EDID_VIDEO_FORMAT_2560x1080p24_64_27,    2560,    998,     44,      148,     1190,  FALSE, 1080,    4,       5,       11,      20,    FALSE, 99000,   24000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   86,      EDID_Standard_CEA}, // 33:21:9 FHD 24p
  {EDID_VIDEO_FORMAT_2560x1080p30_64_27,    2560,    768,     44,      148,     960,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 118681,  29970,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   88,      EDID_Standard_CEA}, // 37:21:9 FHD 30p
  {EDID_VIDEO_FORMAT_2560x1080p30_64_27,    2560,    768,     44,      148,     960,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 118800,  30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   88,      EDID_Standard_CEA}, // 36:21:9 FHD 30p
  {EDID_VIDEO_FORMAT_2560x1080p50_64_27,    2560,    548,     44,      148,     740,   FALSE, 1080,    4,       5,       36,      45,    FALSE, 185625,  50000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   89,      EDID_Standard_CEA}, // 38:21:9 FHD 50p
  {EDID_VIDEO_FORMAT_2560x1080p60_64_27,    2560,    248,     44,      148,     440,   FALSE, 1080,    4,       5,       11,      20,    FALSE, 197802,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   90,      EDID_Standard_CEA}, // 40:21:9 FHD 60p
  {EDID_VIDEO_FORMAT_2560x1080p60_64_27,    2560,    248,     44,      148,     440,   FALSE, 1080,    4,       5,       11,      20,    FALSE, 198000,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   90,      EDID_Standard_CEA}, // 39:21:9 FHD 60p
  {EDID_VIDEO_FORMAT_2560x1440p60_16_9,     2560,    8,       32,      40,      80,    FALSE, 1440,    27,      8,       6,       41,    TRUE,  234590,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 147:2560 x 1440p60 RB2
  {EDID_VIDEO_FORMAT_2560x1440p60_16_9,     2560,    48,      32,      80,      160,   FALSE, 1440,    3,       5,       33,      41,    TRUE,  241500,  59951,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 165:2560 x 1440p60 RB1
  {EDID_VIDEO_FORMAT_2560x1600p60_16_10,    2560,    8,       32,      40,      80,    FALSE, 1600,    32,      8,       6,       46,    TRUE,  260726,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 159:2560 x 1600p60 RB2
  {EDID_VIDEO_FORMAT_2560x1600p60_16_10,    2560,    48,      32,      80,      160,   FALSE, 1600,    3,       6,       37,      46,    TRUE,  268500,  59972,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 140:2560 x 1600p60 RB1
  {EDID_VIDEO_FORMAT_2560x1600p60_16_10,    2560,    192,     280,     472,     944,   TRUE,  1600,    3,       6,       49,      58,    FALSE, 348500,  59987,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 141:M9 -2560x1600p60 -Audio
//{EDID_VIDEO_FORMAT_2560x1080p100_64_27,   2560,    218,     44,      148,     410,   FALSE, 1080,    4,       5,       161,     170,   FALSE, 371250,  100000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   91,      EDID_Standard_CEA}, // 41:21:9 FHD 100p
  {EDID_VIDEO_FORMAT_2560x1600p75_16_10,    2560,    208,     280,     488,     976,   TRUE,  1600,    3,       6,       63,      72,    FALSE, 443250,  74972,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 142:2560 x 1600p75
//{EDID_VIDEO_FORMAT_2560x1440p120_16_9,    2560,    8,       32,      40,      80,    FALSE, 1440,    71,      8,       6,       85,    TRUE,  483120,  120000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 150:2560 x 1440p120 RB2
//{EDID_VIDEO_FORMAT_2560x1080p120_64_27,   2560,    548,     44,      148,     740,   FALSE, 1080,    4,       5,       161,     170,   FALSE, 494505,  119880,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   92,      EDID_Standard_CEA}, // 43:21:9 FHD 120p
//{EDID_VIDEO_FORMAT_2560x1080p120_64_27,   2560,    548,     44,      148,     740,   FALSE, 1080,    4,       5,       161,     170,   FALSE, 495000,  120000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_64_27,   92,      EDID_Standard_CEA}, // 42:21:9 FHD 120p
//{EDID_VIDEO_FORMAT_2560x1440p120_16_9,    2560,    48,      32,      80,      160,   FALSE, 1440,    3,       5,       77,      85,    TRUE,  497750,  119998,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 168:2560 x 1440p120 RB1
//{EDID_VIDEO_FORMAT_2560x1600p85_16_10,    2560,    208,     280,     488,     976,   TRUE,  1600,    3,       6,       73,      82,    FALSE, 505250,  84951,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 143:2560 x 1600p85
//{EDID_VIDEO_FORMAT_2560x1600p120_16_10,   2560,    8,       32,      40,      80,    FALSE, 1600,    80,      8,       6,       94,    TRUE,  536659,  120000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 160:2560 x 1600p120 RB2
//{EDID_VIDEO_FORMAT_2560x1600p120_16_10,   2560,    48,      32,      80,      160,   FALSE, 1600,    3,       6,       85,      94,    TRUE,  552750,  119963,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 144:2560 x 1600p120 RB1
//{EDID_VIDEO_FORMAT_2560x1440p144_16_9,    2560,    8,       32,      40,      80,    FALSE, 1440,    89,      8,       6,       103,   TRUE,  586586,  144000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 151:2560 x 1440p144 RB2
//{EDID_VIDEO_FORMAT_2560x1440p144_16_9,    2560,    48,      32,      80,      160,   FALSE, 1440,    3,       5,       95,      103,   TRUE,  604250,  143973,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 169:2560 x 1440p144 RB1
//{EDID_VIDEO_FORMAT_2560x2560p120_1_1,     2560,    8,       32,      40,      80,    FALSE, 2560,    136,     8,       6,       150,   TRUE,  858528,  120000,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_1_1,     0,       EDID_Standard_VESA}, // 188:2560 x 2560p120 RB2
//{EDID_VIDEO_FORMAT_2560x2560p120_1_1,     2560,    48,      32,      80,      160,   FALSE, 2560,    3,       10,      137,     150,   TRUE,  884500,  119994,  FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_1_1,     0,       EDID_Standard_VESA}, // 189:2560 x 2560p120 RB1
  {EDID_VIDEO_FORMAT_3440x1440p60_43_18,    3440,    8,       32,      40,      80,    FALSE, 1440,    27,      8,       6,       41,    TRUE,  312787,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_43_18,   0,       EDID_Standard_VESA}, // 152:3440 x 1440p60 RB2
  {EDID_VIDEO_FORMAT_3440x1440p60_43_18,    3440,    48,      32,      80,      160,   FALSE, 1440,    3,       10,      28,      41,    TRUE,  319750,  59973,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_43_18,   0,       EDID_Standard_VESA}, // 170:3440 x 1440p60 RB1
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9,     3840,    8,       32,      40,      80,    FALSE, 2160,    17,      8,       6,       31,    TRUE,  257404,  29970,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 158:3840 x 2160p29.97 RB2
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9,     3840,    8,       32,      40,      80,    FALSE, 2160,    17,      8,       6,       31,    TRUE,  257661,  30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 157:3840 x 2160p30 RB2
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9,     3840,    48,      32,      80,      160,   FALSE, 2160,    3,       5,       23,      31,    TRUE,  262500,  29952,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 176:3840 x 2160p29.97 RB1
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9,     3840,    48,      32,      80,      160,   FALSE, 2160,    3,       5,       23,      31,    TRUE,  262750,  29981,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 175:3840 x 2160p30 RB1
  {EDID_VIDEO_FORMAT_3840x2160p24_16_9,     3840,    1276,    88,      296,     1660,  FALSE, 2160,    8,       10,      72,      90,    FALSE, 296703,  23976,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    93,      EDID_Standard_CEA}, // 45:UHD 24p
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9,     3840,    176,     88,      296,     560,   FALSE, 2160,    8,       10,      72,      90,    FALSE, 296703,  29970,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    95,      EDID_Standard_CEA}, // 48:UHD 30p
  {EDID_VIDEO_FORMAT_3840x2160p24_16_9,     3840,    1276,    88,      296,     1660,  FALSE, 2160,    8,       10,      72,      90,    FALSE, 297000,  24000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    93,      EDID_Standard_CEA}, // 44:UHD 24p
  {EDID_VIDEO_FORMAT_3840x2160p25_16_9,     3840,    1056,    88,      296,     1440,  FALSE, 2160,    8,       10,      72,      90,    FALSE, 297000,  25000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    94,      EDID_Standard_CEA}, // 46:UHD 25p
  {EDID_VIDEO_FORMAT_3840x2160p30_16_9,     3840,    176,     88,      296,     560,   FALSE, 2160,    8,       10,      72,      90,    FALSE, 297000,  30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    95,      EDID_Standard_CEA}, // 47:UHD 30p
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9,     3840,    8,       32,      40,      80,    FALSE, 2160,    48,      8,       6,       62,    TRUE,  522092,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 154:3840 x 2160p59.94 RB2
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9,     3840,    8,       32,      40,      80,    FALSE, 2160,    48,      8,       6,       62,    TRUE,  522614,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 153:3840 x 2160p60 RB2
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9,     3840,    48,      32,      80,      160,   FALSE, 2160,    3,       5,       54,      62,    TRUE,  532750,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 172:3840 x 2160p59.94 RB1
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9,     3840,    48,      32,      80,      160,   FALSE, 2160,    3,       5,       54,      62,    TRUE,  533250,  59997,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 171:3840 x 2160p60 RB1
  {EDID_VIDEO_FORMAT_3840x2400p60_16_10,    3840,    8,       32,      40,      80,    FALSE, 2400,    55,      8,       6,       69,    TRUE,  580128,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 187:3840 x 2400p59.94 RB2
  {EDID_VIDEO_FORMAT_3840x2400p60_16_10,    3840,    8,       32,      40,      80,    FALSE, 2400,    55,      8,       6,       69,    TRUE,  580708,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 186:3840 x 2400p60 RB2
  {EDID_VIDEO_FORMAT_3840x2400p60_16_10,    3840,    48,      32,      80,      160,   FALSE, 2400,    3,       6,       60,      69,    TRUE,  592000,  59943,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 185:3840 x 2400p59.94 RB1
  {EDID_VIDEO_FORMAT_3840x2400p60_16_10,    3840,    48,      32,      80,      160,   FALSE, 2400,    3,       6,       60,      69,    TRUE,  592500,  59994,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_10,   0,       EDID_Standard_VESA}, // 184:3840 x 2400p60 RB1
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9,     3840,    176,     88,      296,     560,   FALSE, 2160,    8,       10,      72,      90,    FALSE, 593406,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    97,      EDID_Standard_CEA}, // 51:UHD 60p
  {EDID_VIDEO_FORMAT_3840x2160p50_16_9,     3840,    1056,    88,      296,     1440,  FALSE, 2160,    8,       10,      72,      90,    FALSE, 594000,  50000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    96,      EDID_Standard_CEA}, // 49:UHD 50p
  {EDID_VIDEO_FORMAT_3840x2160p60_16_9,     3840,    176,     88,      296,     560,   FALSE, 2160,    8,       10,      72,      90,    FALSE, 594000,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    97,      EDID_Standard_CEA}, // 50:UHD 60p
  {EDID_VIDEO_FORMAT_4096x2160p30_256_135,  4096,    8,       32,      40,      80,    FALSE, 2160,    17,      8,       6,       31,    TRUE,  274214,  29970,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 0,       EDID_Standard_VESA}, // 156:4096 x 2160p29.97 RB2
  {EDID_VIDEO_FORMAT_4096x2160p30_256_135,  4096,    8,       32,      40,      80,    FALSE, 2160,    17,      8,       6,       31,    TRUE,  274488,  30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 0,       EDID_Standard_VESA}, // 155:4096 x 2160p30 RB2
  {EDID_VIDEO_FORMAT_4096x2160p30_256_135,  4096,    48,      32,      80,      160,   FALSE, 2160,    3,       10,      18,      31,    TRUE,  279500,  29974,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 0,       EDID_Standard_VESA}, // 174:4096 x 2160p29.97 RB1
  {EDID_VIDEO_FORMAT_4096x2160p30_256_135,  4096,    48,      32,      80,      160,   FALSE, 2160,    3,       10,      18,      31,    TRUE,  279750,  30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 0,       EDID_Standard_VESA}, // 173:4096 x 2160p30 RB1
  {EDID_VIDEO_FORMAT_4096x2160p24_256_135,  4096,    1020,    88,      296,     1404,  FALSE, 2160,    8,       10,      72,      90,    FALSE, 296703,  23976,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 98,      EDID_Standard_CEA}, // 53:4K 24p
  {EDID_VIDEO_FORMAT_4096x2160p30_256_135,  4096,    88,      88,      128,     304,   FALSE, 2160,    8,       10,      72,      90,    FALSE, 296703,  29970,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 100,     EDID_Standard_CEA}, // 56:4K 30p
  {EDID_VIDEO_FORMAT_4096x2160p24_256_135,  4096,    1020,    88,      296,     1404,  FALSE, 2160,    8,       10,      72,      90,    FALSE, 297000,  24000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 98,      EDID_Standard_CEA}, // 52:4K 24p
  {EDID_VIDEO_FORMAT_4096x2160p25_256_135,  4096,    968,     88,      128,     1184,  FALSE, 2160,    8,       10,      72,      90,    FALSE, 297000,  25000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 99,      EDID_Standard_CEA}, // 54:4K 25p
  {EDID_VIDEO_FORMAT_4096x2160p30_256_135,  4096,    88,      88,      128,     304,   FALSE, 2160,    8,       10,      72,      90,    FALSE, 297000,  30000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 100,     EDID_Standard_CEA}, // 55:4K 30p
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,  4096,    8,       32,      40,      80,    FALSE, 2160,    48,      8,       6,       62,    TRUE,  556188,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 0,       EDID_Standard_VESA}, // 146:4096 x 2160p59.94 RB2
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,  4096,    8,       32,      40,      80,    FALSE, 2160,    48,      8,       6,       62,    TRUE,  556744,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 0,       EDID_Standard_VESA}, // 145:M12 -4096x2160p60 RB2-Audio
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,  4096,    48,      32,      80,      160,   FALSE, 2160,    3,       10,      49,      62,    TRUE,  566750,  59930,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 0,       EDID_Standard_VESA}, // 164:4096 x 2160p59.94 RB1
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,  4096,    48,      32,      80,      160,   FALSE, 2160,    3,       10,      49,      62,    TRUE,  567250,  59983,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 0,       EDID_Standard_VESA}, // 163:4096 x 2160p60 RB1
  {EDID_VIDEO_FORMAT_4096x2304p60_16_9,     4096,    8,       32,      40,      80,    FALSE, 2304,    52,      8,       6,       66,    TRUE,  593233,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 183:4096 x 2304p59.94 RB2
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,  4096,    88,      88,      128,     304,   FALSE, 2160,    8,       10,      72,      90,    FALSE, 593406,  59940,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 102,     EDID_Standard_CEA}, // 59:4K 60p
  {EDID_VIDEO_FORMAT_4096x2304p60_16_9,     4096,    8,       32,      40,      80,    FALSE, 2304,    52,      8,       6,       66,    TRUE,  593827,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 182:4096 x 2304p60 RB2
  {EDID_VIDEO_FORMAT_4096x2160p50_256_135,  4096,    968,     88,      128,     1184,  FALSE, 2160,    8,       10,      72,      90,    FALSE, 594000,  50000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 101,     EDID_Standard_CEA}, // 57:4K 50p
  {EDID_VIDEO_FORMAT_4096x2160p60_256_135,  4096,    88,      88,      128,     304,   FALSE, 2160,    8,       10,      72,      90,    FALSE, 594000,  60000,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_256_135, 102,     EDID_Standard_CEA}, // 58:4K 60p
  {EDID_VIDEO_FORMAT_4096x2304p60_16_9,     4096,    48,      32,      80,      160,   FALSE, 2304,    3,       5,       58,      66,    TRUE,  604500,  59930,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 181:4096 x 2304p59.94 RB1
  {EDID_VIDEO_FORMAT_4096x2304p60_16_9,     4096,    48,      32,      80,      160,   FALSE, 2304,    3,       5,       58,      66,    TRUE,  605000,  59980,   FALSE,   EDID_SPEC_ALL,   0,    EDID_VIDEO_ASPECT_RATIO_16_9,    0,       EDID_Standard_VESA}, // 180:4096 x 2304p60 RB1
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
static bool32 EDID_ModeSupported(EDID_SpecificationType eEDIDSpecType, EDID_VideoFormatType eVideoMode, uint32 *pIndex);

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
    pEdidSharedData->bEmbeddedDisplay    = pParserConfig->bEmbeddedDisplay;

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

        // Save number of CEA blocks
        pEdidSharedData->uNumOfCEABlocks     = uNumOfCEABlocks;

        // Copy over the list of modes
        DISP_OSAL_MemCpyEx(pModeList, sizeof(EDID_DispModeListType), &pEdidSharedData->sDispModeList, sizeof(EDID_DispModeListType));
      }
    }
  }

  return eStatus;
}
//-------------------------------------------------------------------------------------------------
//  HDMI_EDID_Parser_GetVendorInfo
//
//  @brief
//      Parses the EDID structure and find out the vendor specific information.
//
//  @params
//      [IN] pVendorInfo
//          Pointer to the vendor information.
//
//  @return
//      EDID_STATUS_SUCCESS if data is successfully extracted.
//-------------------------------------------------------------------------------------------------
//
EDID_Status EDID_GetVendorInfo(EDID_HandleType hHandle, EDID_VendorInfoType* pVendorInfo)
{
  EDID_Status          eStatus                = EDID_STATUS_SUCCESS;
  bool32               bAudioSupport          = FALSE;
  bool32               bDispModeSupportAudio  = FALSE;
  uint32               uNumOfCEABlocks        = 0;
  uint32               uCEABlockIndex         = 0;
  uint32               uStartingOffset        = 0;
  uint32               uDescOffset            = 0;
  uint8                uI                     = 0;
  uint8                uJ                     = 0;
  EDID_ParserContextType *pEdidSharedData     = (EDID_ParserContextType*)hHandle;

  if ((NULL == hHandle) ||
      (NULL == pVendorInfo) ||
      (EDID_CEA_EXTENSION_VERSION_OFFSET >= sizeof(pEdidSharedData->auEDIDCache)))
  {
    eStatus = EDID_STATUS_BAD_PARAM;
  }
  else
  {

    uNumOfCEABlocks = EDID_GetNumOfCEAExtBlocks(pEdidSharedData);

    /* Skip Block 2 if block 2 is a BLOCK MAP */
    if (EDID_EXTENSION_BLOCK_MAP_TAG_CODE == pEdidSharedData->auEDIDCache[EDID_CEA_EXTENSION_VERSION_OFFSET - 1])
    {
      uStartingOffset += EDID_BLOCK_SIZE;
      if (uNumOfCEABlocks)
      {
        uNumOfCEABlocks -= 1;
      }
    }

    uStartingOffset += EDID_BLOCK_SIZE;
    bDispModeSupportAudio = EDID_IsDispModeSupportAudio(pEdidSharedData);

    for (uCEABlockIndex = 0; uCEABlockIndex < uNumOfCEABlocks; uCEABlockIndex++)
    {
      bAudioSupport = (pEdidSharedData->auEDIDCache[uStartingOffset + EDID_CEA_EXTENSION_CABS_RELATIVE_OFFSET] & EDID_BASIC_AUDIO_SUPPORT_MASK) >> EDID_BASIC_AUDIO_SUPPORT_SHIFT;

      /* No Audio support - DVI or the selected display resolution does not supports audio */
      if (!bAudioSupport || !bDispModeSupportAudio)
      {
        pVendorInfo->bVideoOnly = TRUE;
      }

      if (pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uPhysicalAddress)
      {
        pVendorInfo->sLipSyncInfo.uVideoLatency = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uVideoLatency;

        /* Selected display resolution does not support audio no need to report the audio latency */
        if (bDispModeSupportAudio)
        {
          pVendorInfo->sLipSyncInfo.uAudioLatency          = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uAudioLatency;
          pVendorInfo->sLipSyncInfo.uInterlaceAudioLatency = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uInterlacedAudioLatency;
        }
        pVendorInfo->sLipSyncInfo.uInterlaceVideoLatency  = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uInterlacedVideoLatency;
        pVendorInfo->uSupportedDeepColorModeMask          = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uSupportedDeepColorModeMask;
        pVendorInfo->uPhysicalAddr                        = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uPhysicalAddress;
        pVendorInfo->bAISupport                           = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].bAISupport;
        pVendorInfo->bDVIDualSupport                      = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].bDVIDualSupport;
        pVendorInfo->uSupportedContentFilterMask          = pEdidSharedData->sDataBlockDesc.sVendorDataBlockDesc[uCEABlockIndex].uSupportedContentFilterMask;
      }
      else
      {
        eStatus = EDID_STATUS_FAILED;
      }
    }

    pVendorInfo->uMaxScreenWidthInMillimeter  = (pEdidSharedData->auEDIDCache[EDID_HORIZONTAL_SCREEN_SIZE_OFFSET] * 10);  //Change to mm from cm by multiplying 10. 
    pVendorInfo->uMaxScreenHeightInMillimeter = (pEdidSharedData->auEDIDCache[EDID_VERTICAL_SCREEN_SIZE_OFFSET] * 10);    //Change to mm from cm by multiplying 10.
    pVendorInfo->uManufacturerId[0]           = (pEdidSharedData->auEDIDCache[EDID_MANUFACTURER_ID_OFFSET]);
    pVendorInfo->uManufacturerId[1]           = (pEdidSharedData->auEDIDCache[EDID_MANUFACTURER_ID_OFFSET + 1]);
    pVendorInfo->uProductId                   = (pEdidSharedData->auEDIDCache[EDID_MANUFACTURER_ID_OFFSET + 3] << 8) | pEdidSharedData->auEDIDCache[EDID_MANUFACTURER_ID_OFFSET + 2];

    for (uI = 0; uI < EDID_TIMING_BLOCK_0_MAX_DESC; uI++)
    {
      uDescOffset = EDID_FIRST_TIMING_DESC_OFFSET + (uI * EDID_DETAIL_TIMING_DESC_BLOCK_SIZE);

      //Monitor Name descriptor.        
      if ((0                     == pEdidSharedData->auEDIDCache[uDescOffset] ) &&
          (EDID_MONITOR_NAME_TAG == pEdidSharedData->auEDIDCache[uDescOffset + 3]))
      {
        // find name length 
        for (uJ = 0; uJ < EDID_MONITOR_NAME_MAX_LENGTH; uJ++)
        {
          if (pEdidSharedData->auEDIDCache[uDescOffset + 5 + uJ] == EDID_MONITOR_NAME_TERMINATION_CHAR)
          {
            break;
          }
        }
        pVendorInfo->uNameDescriptionLength = uJ;
        DISP_OSAL_MemCpyEx(&pVendorInfo->uNameDescription[0],              sizeof(pVendorInfo->uNameDescription),
                           &pEdidSharedData->auEDIDCache[uDescOffset + 5], pVendorInfo->uNameDescriptionLength);
        break;
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
    
    if (uModeIndex < EDID_DISP_MODE_LIST_MAX_NUM)
    {
      EDID_VideoFormatType eVideoFormat = pEdidSharedData->sDispModeList.aDispModeAttr[uModeIndex].eVideoFormat;

      if (eVideoFormat < EDID_VIDEO_FORMAT_MAX)
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

  if (EDID_CEA_EXTENSION_VERSION_OFFSET < sizeof(pEdidSharedData->auEDIDCache))
  {
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
}

//-------------------------------------------------------------------------------------------------
//  EDID_Parser_GetColorBitDepth
//
//  @brief
//      Parses the EDID structure and find out the color bit depth.
//
//  @params
//      [IN] pEdidSharedData
//          EDID parser contect
//
//  @return
//      uint32    Color depth in bits per color.
//-------------------------------------------------------------------------------------------------
//
static uint32 EDID_Parser_GetColorBitDepth(EDID_ParserContextType *pEdidSharedData)
{
  uint32    uBitsPerColor  = 8;
  
  //Check if digital interface 
  if(pEdidSharedData->auEDIDCache[EDID_VIDEO_INPUT_DEFINITION_OFFSET] & EDID_VIDEO_SIGNAL_INTERFACE_MASK)
  {
    uint32 uColorBitCode = (pEdidSharedData->auEDIDCache[EDID_VIDEO_INPUT_DEFINITION_OFFSET] & EDID_COLOR_BIT_DEPTH_MASK) >> EDID_COLOR_BIT_DEPTH_SHIFT;

    switch(uColorBitCode)
    {
      case 1:
        uBitsPerColor = 6;
        break;
      case 2:
        uBitsPerColor = 8;
        break;
      default:
        uBitsPerColor = 8;
        break;
    }
  }
  
  return uBitsPerColor;
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
         if (pEdidSharedData->bEmbeddedDisplay == FALSE)
         {
            EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, eVideoFormat, EDID_VIDEO_YUV420_NOT_SUPPORTED);
         }
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
  uint8  uEstablishedTimingIII = pDataBuf[uEstablishTimingOffset + 1];
  uint32 uIndex;

  if ((uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1280x768p60) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1280x768p60_5_3, &uIndex)))
  {
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED); 
  }

  // Move to the next byte of establish timing
  uEstablishedTimingIII = pDataBuf[uEstablishTimingOffset + 2];
  
  if ((uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1280x960p60) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1280x960p60_4_3, &uIndex)))
  {
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED); 
  }

  if ((uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1280x1024p60) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1280x1024p60_5_4, &uIndex)))
  {
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED); 
  }

  // Move to the next byte of establish timing
  uEstablishedTimingIII = pDataBuf[uEstablishTimingOffset + 3];

  if ((uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1360x768p60) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1360x768p60_16_9, &uIndex)))
  {
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED); 
  }

  if ((uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1440x900p60) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1440x900p60_16_10, &uIndex)))
  {
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);      
  }
  
  if ((uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1400x1050p60) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1400x1050p60_4_3, &uIndex)))
  {
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  // Move to the next byte of establish timing
  uEstablishedTimingIII = pDataBuf[uEstablishTimingOffset + 4];

  if ((uEstablishedTimingIII & EDID_ESTABLISHED_TIMINGIII_1680x1050p60) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1680x1050p60_16_10, &uIndex)))
  {  
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED); 
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
  uint32                uWorkingOffset    = uBufferIndex;
  uint32                uI                = 0;
  uint32                uMaxNumOfElement  = NUMBER_OF_ELEMENTS(gEDIDSupportedVideoModeLUT);

  if (pDataBuf)
  {
    // Move to the first short video descriptor
    uWorkingOffset++;

    for (uI = 0; uI < uNumOfDispMode; uI++)
    {
      // Check for Valid Video ID code
      if (0 != pDataBuf[uWorkingOffset + uI])
      {
        uint32 uVicId = (uint32)((pDataBuf[uWorkingOffset + uI] & EDID_SHORT_VIDEO_DESC_VID_MASK));
        uint32 uIndex;

        if ((EDID_MIN_VIC_ID <= uVicId) &&
            (EDID_MAX_VIC_ID >= uVicId))
        {
          for(uIndex = 0; uIndex < uMaxNumOfElement; uIndex++)
          {
            if ((EDID_Standard_CEA == gEDIDSupportedVideoModeLUT[uIndex].eStandard) &&
                (uVicId == gEDIDSupportedVideoModeLUT[uIndex].uVicId))
            {
              EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, eYUVSupport);
              break;
            }
          }
        }
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
  bool32               bActiveLowV      = FALSE;
  bool32               bActiveLowH      = FALSE;
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
  uint32               uFPorchH         = 0;
  uint32               uSyncPulseH      = 0;
  uint32               uFPorchV         = 0;
  uint32               uSyncPulseV      = 0;
  uint32               uDataBits        = 0;
  uint32               uRefreshRate     = 0;

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
    uBlankV <<= 8; //Shift 8 bits 
    uBlankV  |= (uint32)pDataBuf[EDID_TIMING_DESC_V_BLANK_OFFSET]; //Shift 8 bits 

    uFPorchH  = pDataBuf[EDID_TIMING_DESC_H_FP_OFFSET];
    uDataBits = pDataBuf[EDID_TIMING_DESC_H_V_PORCH_UPPER_OFFSET];
    uDataBits &= 0xC0;	  /* bits 6, 7 */
    uFPorchH |= (uDataBits << 2);

    uSyncPulseH  = pDataBuf[EDID_TIMING_DESC_H_SP_OFFSET];
    uDataBits = pDataBuf[EDID_TIMING_DESC_H_V_PORCH_UPPER_OFFSET];
    uDataBits &= 0x30;	  /* bits 4, 5 */
    uSyncPulseH |= (uDataBits << 4);

    uFPorchV  = pDataBuf[EDID_TIMING_DESC_V_FP_SP_OFFSET];
    uFPorchV >>= 4;
    uDataBits = pDataBuf[EDID_TIMING_DESC_H_V_PORCH_UPPER_OFFSET];
    uDataBits &= 0x0C;	  /* bits 2, 3 */
    uFPorchV |= (uDataBits << 2);

    uSyncPulseV  = pDataBuf[EDID_TIMING_DESC_V_FP_SP_OFFSET];
    uSyncPulseV  &= 0x0F;    /* Lower 4 bits */
    uDataBits = pDataBuf[EDID_TIMING_DESC_H_V_PORCH_UPPER_OFFSET];
    uDataBits &= 0x03;	  /* bits 0, 1 */
    uSyncPulseV |= (uDataBits << 4);
    
    uImageSizeH   = (pDataBuf[EDID_TIMING_DESC_IMAGE_SIZE_UPPER_NIBBLE_OFFSET] >> EDID_TIMING_H_IMAGE_SIZE_UPPER_NIBBLE_SHIFT) &
                     EDID_TIMING_H_IMAGE_SIZE_UPPER_NIBBLE_MASK;
    uImageSizeH <<= 8;
    uImageSizeH  |= (uint32)pDataBuf[EDID_TIMING_DESC_H_IMAGE_SIZE_OFFSET];

    uImageSizeV   = (pDataBuf[EDID_TIMING_DESC_IMAGE_SIZE_UPPER_NIBBLE_OFFSET]& EDID_TIMING_V_IMAGE_SIZE_UPPER_NIBBLE_MASK);
    uImageSizeV <<= 8;
    uImageSizeV  |= (uint32)pDataBuf[EDID_TIMING_DESC_V_IMAGE_SIZE_OFFSET];

    // Calculating aspect ratio 4:3
    if ((0 == uImageSizeH) || 
        (0 == uImageSizeV)) 
    {
      uResult  = uActiveH * 3;
      uResult1 = uActiveV * 4;
    }
    else
    {
      uResult  = uImageSizeH * 3;
      uResult1 = uImageSizeV * 4;
    }

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

    uMaxNumOfElement  = NUMBER_OF_ELEMENTS(gEDIDSupportedVideoModeLUT);
    uIndex            = uMaxNumOfElement >> 2;

    // Break table in half and search using H Active
    if(uActiveH <= gEDIDSupportedVideoModeLUT[uIndex].uActiveH)
      uIndex = 0;

    bInterlaced = (pDataBuf[EDID_TIMING_DESC_INTERLACE_OFFSET] & EDID_TIMING_INTERLACE_MASK) >> EDID_TIMING_INTERLACE_SHIFT;

    // Interlace mode the active V should multiply by 2 because EDID reports field height only
    if(bInterlaced)
      uActiveV = uActiveV << 1;


  uDataBits = pDataBuf[EDID_TIMING_DESC_INTERLACE_OFFSET];
    if (uDataBits & 0x10)  /* bit 4 */
    {
       if (uDataBits & 0x08)
       {
          if (uDataBits & 0x04)
          {
               bActiveLowV     =  FALSE; /* vsync positive */
          }
          else
          {
               bActiveLowV     =  TRUE; /* vsync negative */
          }
       }
       
       if (uDataBits & 0x02)
       {
               bActiveLowH     =  FALSE; /* hsync positive */
       }
       else
       {
               bActiveLowH     =  TRUE; /* hsync negative */
       }
    }

    uRefreshRate           = Q16_FLOAT_VALUE((float)(uPixelClock * 1000) / ((uActiveH+ uBlankH) * (uActiveV+ uBlankV)));

    if (pEdidSharedData->bEmbeddedDisplay == TRUE)
    {
        EDID_DispModeListType    *pDispModeList = &pEdidSharedData->sDispModeList;
        uint32 uIndex                           = pDispModeList->uNumOfElements;

        /* insert detail timing into mode list at entry 0 */
        pDispModeList->aDispModeAttr[uIndex].bVideoOnly      = FALSE;
        pDispModeList->aDispModeAttr[uIndex].eVideoFormat    = 0xff;  /* pu soem number here */
        pDispModeList->aDispModeAttr[uIndex].uPixelFormatMap = (1 << EDID_PIXEL_FORMAT_RGB888); //Default pixel format support
        pDispModeList->uNumOfElements++;

        // Update the timing info
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncPulseWidthDclk  = uSyncPulseH;
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncBackPorchDclk   = (uBlankH - uSyncPulseH - uFPorchH); 
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iHsyncFrontPorchDclk  = uFPorchH;
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncPulseWidthLines = uSyncPulseV;
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncBackPorchLines  = (uBlankV - uSyncPulseV - uFPorchV);
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.iVsyncFrontPorchLines = uFPorchV;
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.bInterlaced           = bInterlaced;
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.bIsHsyncActiveLow     = bActiveLowH;
        pDispModeList->aDispModeAttr[uIndex].sActiveTimingInfo.bIsVsyncActiveLow     = bActiveLowV;

        pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uWidthPx               = uActiveH;
        pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uWidthTotalPx          = uActiveH + uBlankH;

        pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uHeightPx              = uActiveV;
        pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uHeightTotalPx         = uActiveV + uBlankV;

        pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uRefreshRate           = uRefreshRate;

        pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uPixelRate             = uPixelClock * 1000;
        pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.bAudioSupported        = FALSE;
        pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eColorFormat           = (EDID_Parser_GetColorBitDepth(pEdidSharedData) == 6) ? EDID_PIXEL_COLOR_FORMAT_RGB_666_18BPP : EDID_PIXEL_COLOR_FORMAT_RGB_888_24BPP;

        DISP_OSAL_LOG_INFO("DP_Parse_Detail_Desc: ndx=%d pclk=%d w=%d blankH=%d h=%d blankV=%d frate=%x\n", uIndex, uPixelClock * 1000, pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uWidthPx, uBlankH, pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uHeightPx, uBlankV, pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uRefreshRate);


         if (bAspectRatio4_3 == TRUE)
         {
            pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_4_3;
         }
         else
         {
             pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eAspectRatio = EDID_DISPLAY_MODE_ASPECT_RATIO_16_9;
         }
    }
    else
    {
         while (uIndex < uMaxNumOfElement)
         {
           if ((bInterlaced        == gEDIDSupportedVideoModeLUT[uIndex].bInterlaced)  &&
               (uActiveH           == gEDIDSupportedVideoModeLUT[uIndex].uActiveH)     &&
               (uBlankH            == gEDIDSupportedVideoModeLUT[uIndex].uTotalBlankH) &&
               (uBlankV            == gEDIDSupportedVideoModeLUT[uIndex].uTotalBlankV) && 
               (uActiveV           == gEDIDSupportedVideoModeLUT[uIndex].uActiveV)     &&
               (bAspectRatio4_3    == (bool32)(EDID_VIDEO_ASPECT_RATIO_4_3 == gEDIDSupportedVideoModeLUT[uIndex].eAspectRatio)))
           {
             // Pixel clock needs to check against a range to include non integer value of the refresh rates
             if(((gEDIDSupportedVideoModeLUT[uIndex].uPixelFreq - EDID_TIMING_PIXEL_CLOCK_RANGE)  < uPixelClock) &&  (uPixelClock < gEDIDSupportedVideoModeLUT[uIndex].uPixelFreq + EDID_TIMING_PIXEL_CLOCK_RANGE))
             { 
               // Check whether HDMI TX core supports this mode
               if (pEdidSharedData->eEDIDSpecType & gEDIDSupportedVideoModeLUT[uIndex].eEDIDSpecType)
               {
                *pDispMode = uIndex;
                 eStatus   = EDID_STATUS_SUCCESS;
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
    
    if ((EDID_VIDEO_FORMAT_MAX  >  eVideoFormat)                                               && 
        (EDID_SPEC_TYPE_UNSUPPORTED != gEDIDSupportedVideoModeLUT[eVideoFormat].eEDIDSpecType) &&
        (EDID_DISP_MODE_LIST_MAX_NUM > pDispModeList->uNumOfElements))
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
                // HW can not support YUV420 sampling for this mode, do not add to the list
                eLocalStatus  = EDID_STATUS_NOT_SUPPORTED;
              }
              else if (EDID_VIDEO_YUV420_ALSO_SUPPORTED == eYUV420Support)
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
              //  - If the EDID indicates that the TV/monitor supports audio (pSharedData->bAudioSupport) and
              if (TRUE == pEdidSharedData->bAudioSupport)
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
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uRefreshRate           = Q16_FLOAT_VALUE((float)gEDIDSupportedVideoModeLUT[eVideoFormat].uRefreshRate / 1000);
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uPixelRate             = gEDIDSupportedVideoModeLUT[eVideoFormat].uPixelFreq * 1000;
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.bAudioSupported        = bAudioSupported;
              pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.eColorFormat           = (EDID_Parser_GetColorBitDepth(pEdidSharedData) == 6) ? EDID_PIXEL_COLOR_FORMAT_RGB_666_18BPP : EDID_PIXEL_COLOR_FORMAT_RGB_888_24BPP;

              DISP_OSAL_LOG_INFO("EDID_Parser_InsertDispFormat: numOfElements=%d videoFormat=%d w=%d h=%d frate=%x\n", pDispModeList->uNumOfElements, eVideoFormat, gEDIDSupportedVideoModeLUT[eVideoFormat].uActiveH, gEDIDSupportedVideoModeLUT[eVideoFormat].uActiveV, pDispModeList->aDispModeAttr[uIndex].sDisplayModeInfo.uRefreshRate);

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
            uint32        uBitsPerPixel = 3 * EDID_Parser_GetColorBitDepth(pEdidSharedData);    // default to RGB
            
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

                if ((EDID_VIDEO_YUV420_ONLY_SUPPORTED == eYUV420Support) ||
                    (EDID_VIDEO_YUV420_ALSO_SUPPORTED == eYUV420Support))
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
  uint32                    uMaxNumOfElement = NUMBER_OF_ELEMENTS(gEDIDSupportedVideoModeLUT);
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
          if ((FALSE        == gEDIDSupportedVideoModeLUT[uIndex].bInterlaced)  &&
              (uActiveH     == gEDIDSupportedVideoModeLUT[uIndex].uActiveH)     &&
              (uActiveV     == gEDIDSupportedVideoModeLUT[uIndex].uActiveV)     && 
              (EDID_ABS((gEDIDSupportedVideoModeLUT[uIndex].uRefreshRate/1000.0) - uRefreshRate) <= EDID_STANDARD_TIMING_FREQ_DELTA))
          {            
            /* The resolution read from the standard timing corresponds to VESA , hence no YUV420 flag is set*/ 
            EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED); 

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
  uint32 uIndex;

  // Parse the established timing section  to find out which modes are supported
  uint8 uEstablishedTimingI   = pDataBuf[EDID_ESTABLISHED_TIMINGI_START_OFFSET]; 
  uint8 uEstablishedTimingII  = pDataBuf[EDID_ESTABLISHED_TIMINGII_START_OFFSET];
  uint8 uEstablishedTimingIII = pDataBuf[EDID_ESTABLISHED_TIMINGIII_START_OFFSET]; 

  // Establish Timing I
  if ((EDID_ESTABLISHED_TIMINGI_640x480p60 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_640x480p60_4_3, &uIndex)))
  {

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set 
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGI_640x480p72 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_640x480p72_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGI_640x480p75 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_640x480p75_4_3, &uIndex)))
  {

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGI_800x600p56 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_800x600p56_4_3, &uIndex)))
  {
    
    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGI_800x600p60 & uEstablishedTimingI) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_800x600p60_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  // Established Timing II */
  
  if ((EDID_ESTABLISHED_TIMINGII_800x600p72 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_800x600p72_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_800x600p75 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_800x600p75_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_1024x768p60 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1024x768p60_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_1024x768p70 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1024x768p70_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex ,EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_1024x768p75 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1024x768p75_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGII_1280x1024p75 & uEstablishedTimingII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1280x1024p75_5_4, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  // Established Timing III */
  if ((EDID_ESTABLISHED_TIMINGIII_1280x768p60 & uEstablishedTimingIII)                 &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1280x768p60_5_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1280x960p60 & uEstablishedTimingIII)                 &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1280x960p60_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1280x1024p60 & uEstablishedTimingIII)                 &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1280x1024p60_5_4, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1360x768p60 & uEstablishedTimingIII)&&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1360x768p60_16_9, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1440x900p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1440x900p60_16_10, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1400x1050p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1400x1050p60_4_3, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }

  if ((EDID_ESTABLISHED_TIMINGIII_1680x1050p60 & uEstablishedTimingIII) &&
      (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, EDID_VIDEO_FORMAT_1680x1050p60_16_10, &uIndex)))
  { 

    // The resolution read from the establish section corresponds to VESA , hence no YUV420 is set
    EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }
}

//-------------------------------------------------------------------------------------------------
//  EDID_ModeSupported
//
//  @brief
//      Verifies whether the specified mode is supported.
//
//  @params
//      [IN] eEDIDSpecType
//          EDID spec type
//      [IN] eVideoMode
//          Video mode format that needs to verified.
//      [IN]pIndex
//           Index in the list of driver supported modes
//
//  @return
//      TRUE if the mode is supported.
//-------------------------------------------------------------------------------------------------
//
static bool32 EDID_ModeSupported(EDID_SpecificationType eEDIDSpecType, EDID_VideoFormatType eVideoMode, uint32 *pIndex)
{
  bool32               bResult        = FALSE;
  uint32               uNumOfElements = NUMBER_OF_ELEMENTS(gEDIDSupportedVideoModeLUT);
  uint32               uIndex;

  for (uIndex = 0; uIndex < uNumOfElements; uIndex++)
  {
    if ((eVideoMode    == gEDIDSupportedVideoModeLUT[uIndex].eVideoFormat)  &&
        (eEDIDSpecType &  gEDIDSupportedVideoModeLUT[uIndex].eEDIDSpecType))
    {
     *pIndex = uIndex;
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
        uint32               uIndex;

        if (TRUE == EDID_ModeSupported(pEdidSharedData->eEDIDSpecType, eVideoFormat, &uIndex))
        {
          /* Insert the video format. This function makes sure that each format is not entered more than once */
          EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, uIndex, EDID_VIDEO_YUV420_NOT_SUPPORTED);
        }

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
    //EDID_Parser_InsertDispFormat(pEdidSharedData, pDispModeList, EDID_VIDEO_FORMAT_640x480p60_4_3, EDID_VIDEO_YUV420_NOT_SUPPORTED);
  }
  return eStatus;
}

//-------------------------------------------------------------------------------------------------
//  EDID_GetMode_ByIndex
//
//  @brief
//      Retrieve information  from the EDID table based on the provided
//      index and sets all the value required to return it
//
//  @params
//      [IN/OUT] pModeInfo
//          EDID_DispModeAttrType that contains EDID table data
//      [IN] uModeIndex
//          Index into the EDID table
//
//  @return
//      EDID_STATUS_SUCCESS if read from EDID table is successful
//-------------------------------------------------------------------------------------------------
//
EDID_Status EDID_GetMode_ByIndex(EDID_DispModeAttrType *pModeInfo, uint32 uModeIndex) 
{
  EDID_Status eStatus = EDID_STATUS_SUCCESS;

  if (uModeIndex >= NUMBER_OF_ELEMENTS(gEDIDSupportedVideoModeLUT)) {
    eStatus = EDID_STATUS_FAILED;
    return eStatus;
  }

  pModeInfo->sActiveTimingInfo.iHsyncPulseWidthDclk  = gEDIDSupportedVideoModeLUT[uModeIndex].uPulseWidthH;
  pModeInfo->sActiveTimingInfo.iHsyncBackPorchDclk   = gEDIDSupportedVideoModeLUT[uModeIndex].uBackPorchH;
  pModeInfo->sActiveTimingInfo.iHsyncFrontPorchDclk  = gEDIDSupportedVideoModeLUT[uModeIndex].uFrontPorchH;
  pModeInfo->sActiveTimingInfo.iVsyncPulseWidthLines = gEDIDSupportedVideoModeLUT[uModeIndex].uPulseWidthV;
  pModeInfo->sActiveTimingInfo.iVsyncBackPorchLines  = gEDIDSupportedVideoModeLUT[uModeIndex].uBackPorchV;
  pModeInfo->sActiveTimingInfo.iVsyncFrontPorchLines = gEDIDSupportedVideoModeLUT[uModeIndex].uFrontPorchV;
  pModeInfo->sActiveTimingInfo.bInterlaced           = pModeInfo->sDisplayModeInfo.bInterlaced = gEDIDSupportedVideoModeLUT[uModeIndex].bInterlaced;
  pModeInfo->sActiveTimingInfo.bIsHsyncActiveLow     = gEDIDSupportedVideoModeLUT[uModeIndex].bActiveLowH;
  pModeInfo->sActiveTimingInfo.bIsVsyncActiveLow     = gEDIDSupportedVideoModeLUT[uModeIndex].bActiveLowV;

  pModeInfo->sDisplayModeInfo.uWidthPx               = gEDIDSupportedVideoModeLUT[uModeIndex].uActiveH;
  pModeInfo->sDisplayModeInfo.uWidthTotalPx          = pModeInfo->sDisplayModeInfo.uWidthPx +
                                                                                  pModeInfo->sActiveTimingInfo.iHsyncPulseWidthDclk +
                                                                                  pModeInfo->sActiveTimingInfo.iHsyncBackPorchDclk +
                                                                                  pModeInfo->sActiveTimingInfo.iHsyncFrontPorchDclk;
  pModeInfo->sDisplayModeInfo.uHeightPx              = gEDIDSupportedVideoModeLUT[uModeIndex].uActiveV;
  pModeInfo->sDisplayModeInfo.uHeightTotalPx         = pModeInfo->sDisplayModeInfo.uHeightPx +
                                                                                  pModeInfo->sActiveTimingInfo.iVsyncPulseWidthLines +
                                                                                  pModeInfo->sActiveTimingInfo.iVsyncBackPorchLines +
                                                                                  pModeInfo->sActiveTimingInfo.iVsyncFrontPorchLines;
  pModeInfo->sDisplayModeInfo.uRefreshRate           = Q16_FLOAT_VALUE((float)gEDIDSupportedVideoModeLUT[uModeIndex].uRefreshRate / 1000);
  pModeInfo->sDisplayModeInfo.uPixelRate             = gEDIDSupportedVideoModeLUT[uModeIndex].uPixelFreq * 1000;

  return eStatus;
}


#ifdef __cplusplus
}
#endif


